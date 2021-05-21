#include "stdafx.h"
#define D3D_OVERLOADS
#include <d3d.h>
#include <assert.h>
#include <mmsystem.h>

#include "main.h"
#include "D3DMath.h"
#include "D3DUtil.h"
#include "IndexedTexture.h"
#include "WeatherMgr.h"
#include "DirtSnd.h"
#include "LoadEffectSound.h"
#include "ImageDefine.h"
#include "CapSyong.h"
#include "GUtil.h"
#include "NkCharacter.h"
#include "Nk2DFrame.h"
#include "Sheet.h"
#include "Country.h"

#include "CommonConfig.h"

extern int g_EffectSound[];
extern BOOL g_bDungeon;
extern DWORD g_dwClientCountry;
extern SVRTYPE	g_SvrType;

float FloatRandom(float from, float to);
int g_nRandomSpeed;

BOOL g_bSnow = TRUE;
BOOL g_bSakura = FALSE;

#define LOGIN_SNOW_X	150.0f
#define LOGIN_SNOW_Y	190.0f
#define LOGIN_SNOW_Z	350.0f

CWeatherMgr::CWeatherMgr()
	: m_dwBaseTime(0)
	, m_nCurWeatherState(0)
	, m_nSakuraType(0)
{
	m_pRain = new CRains;
	m_nRainWeight = 10;
	m_bShowRain = FALSE;
	m_bShowSnow = FALSE;
	m_bTonado = FALSE;
	m_fTonadoAng = 0.0f;
	m_bEnableWeatherSystem = TRUE;
	m_bLogin = FALSE;
}

CWeatherMgr::~CWeatherMgr()
{
	if (m_pRain)
	{
		delete m_pRain;
		m_pRain = NULL;
	}
}

void CWeatherMgr::LoadRes()
{
}

void CWeatherMgr::DeleteRes()
{
}

void CWeatherMgr::StartRain(int weight)
{
	if (g_bSnow)
	{
		if( g_bSakura )
		{
			switch( weight )
			{
			case 0:
				StartSakura(0);
				break;
			case 3:
				StartSakura(0);
				StartSakura(1, TRUE);
				break;
			case 10:
				StartSakura(0);
				StartSakura(2, TRUE);
				break;
			case 20:
				StartSakura(0);
				StartSakura(3, TRUE);
				break;
			}
		}
		else
		{
			StartSnow(weight);
		}
	}
	else
	{
		m_nRainWeight = weight;

		if( g_bSakura )
			StopSakura();
		else
			StopSnow();

		m_bShowRain = TRUE;
	}
}

void CWeatherMgr::StopWeatherSystem()
{
	// 날씨를 맑게 만들고 난 후에 시스템 중지.
	SetWeather(0);
	m_bEnableWeatherSystem = FALSE;
}

void CWeatherMgr::DungeonWeatherSystem()
{
	// 날씨를 맑게 만들고 난 후에 시스템 중지.
	SetWeather(0);
	m_bEnableWeatherSystem = FALSE;

	if (!g_pDSound || g_bSnow)
		return;

	if (g_pDSound->IsPlaying(g_EffectSound[EFF_SND_L_RAIN]))
	{
		g_pDSound->Stop(g_EffectSound[EFF_SND_L_RAIN]);
	}

	if (g_pDSound->IsPlaying(g_EffectSound[EFF_SND_H_RAIN]))
	{
		g_pDSound->Stop(g_EffectSound[EFF_SND_H_RAIN]);
	}
}

void CWeatherMgr::ResumeWeatherSystem()
{
	m_bEnableWeatherSystem = TRUE;
	SetWeather(m_nCurWeatherState);
}

void CWeatherMgr::SetWeather(int weather)
{
	m_nCurWeatherState = weather;

	if(
		pCMyApp->GetCurWorld() == 11) // 11 : WORLD_GUILD_WAR
		m_nCurWeatherState = weather = 0;

	if( pCMyApp->IsCurWorldID_BossraidZone()
			|| pCMyApp->GetCurWorld() == 19
	  )
		m_nCurWeatherState = weather = 0;

	if (m_bEnableWeatherSystem)
	{
		switch (weather)
		{
		case 0: // 맑음. 디폴트로 걍 가라...
			if (IsRaining())
				StopRain();
			break;
		case 1: // 흐림
			if (IsRaining())
				StartRain(0);
			if (!g_bSnow && g_pDSound && g_pDSound->IsPlaying(g_EffectSound[EFF_SND_L_RAIN]))
			{
				g_pDSound->Play(g_EffectSound[EFF_SND_L_RAIN_STOP]);
				g_pDSound->Stop(g_EffectSound[EFF_SND_L_RAIN]);
			}
			break;
		case 2: // 비 쪼까 옴..
			StartRain(3);
			if (!g_bSnow && g_pDSound && !g_pDSound->IsPlaying(g_EffectSound[EFF_SND_L_RAIN]))
				g_pDSound->Loop(g_EffectSound[EFF_SND_L_RAIN]);
			if (!g_bSnow && g_pDSound && g_pDSound->IsPlaying(g_EffectSound[EFF_SND_H_RAIN]))
			{
				g_pDSound->Play(g_EffectSound[EFF_SND_H_RAIN_STOP]);
				g_pDSound->Stop(g_EffectSound[EFF_SND_H_RAIN]);
			}
			break;
		case 3: // 비 마니 옴.
			StartRain(10);
			if (!g_bSnow && g_pDSound && g_pDSound->IsPlaying(g_EffectSound[EFF_SND_L_RAIN]))
			{
				g_pDSound->Play(g_EffectSound[EFF_SND_L_RAIN_STOP]);
				g_pDSound->Stop(g_EffectSound[EFF_SND_L_RAIN]);
			}
			if (!g_bSnow && g_pDSound && !g_pDSound->IsPlaying(g_EffectSound[EFF_SND_H_RAIN]))
				g_pDSound->Loop(g_EffectSound[EFF_SND_H_RAIN]);
			break;
		case 4: // 비 왕창 옴...
			StartRain(20);

			if (!g_bSnow && g_pDSound && !g_pDSound->IsPlaying(g_EffectSound[EFF_SND_H_RAIN])) // 다른 존에서 이동시 꺼져있을 수 있기때문에 체크. (05-12-12 원석)
				g_pDSound->Loop(g_EffectSound[EFF_SND_H_RAIN]);

			break;
		default: // 맑음
			if (IsRaining())
				StopRain();
			break;
		}
	}
}

void CWeatherMgr::StopRain()
{
	if (g_bSnow)
	{
		if( g_bSakura )
		{
			StopSakura();
		}
		else
		{
			StopSnow();
		}
	}
	else
	{
		m_bShowRain = FALSE;
	}
}

void CWeatherMgr::StartSnow(int weight, BOOL bTonado)
{
	m_nRainWeight = weight;
	m_bTonado = bTonado;
	m_fTonadoAng = 0.0f;
	m_dwBaseTime = timeGetTime();
	StopRain();
	m_bShowSnow = TRUE;

	if( CommonConfig::Instance()->GetEventSakura() == true )
	{
		m_bShowRain = FALSE;
	}
}

void CWeatherMgr::StopSnow()
{
	m_bShowSnow = FALSE;
}

void CWeatherMgr::StartSakura(int weight, BOOL bTonado)
{
	if( weight == 0 )
		m_nRainWeight = weight;
	else
	{
		m_nSakuraType = weight;
		m_nRainWeight = 1;
	}
	m_bTonado = bTonado;
	m_fTonadoAng = 0.1f;
	m_dwBaseTime = timeGetTime();
	StopRain();
	m_bShowSnow = TRUE;
	m_bShowRain = FALSE;
}

void CWeatherMgr::StopSakura()
{
	m_bShowSnow = FALSE;
}

void CWeatherMgr::RenderRain()
{
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, TRUE);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA);

	D3DUtil_SetTranslateMatrix(m_matWorld, pCMyApp->GetEyeAtPos());
	GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_WORLD, &m_matWorld);
	GET_D3DDEVICE()->GetTransform(D3DTRANSFORMSTATE_VIEW, &m_matView);

	// nate 2004 - 7 m_pTestTexture제거
	m_pRain->Render3( g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_WEATHERMGR_RAIN ] ) ,
					  g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_WEATHERMGR_DROP ] ) ,
					  &m_matWorld, &m_matView, m_nRainWeight, FALSE);

	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, FALSE);
}

void CWeatherMgr::RenderSnow()
{
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, TRUE);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA);

	if (m_bLogin)
	{
		D3DUtil_SetTranslateMatrix(m_matWorld, D3DVECTOR(LOGIN_SNOW_X, LOGIN_SNOW_Y, LOGIN_SNOW_Z));
	}
	else
	{
		D3DUtil_SetTranslateMatrix(m_matWorld, pCMyApp->GetEyeAtPos());
	}
	if (m_bTonado)
	{
		D3DUtil_SetRotateYMatrix(m_matRotate, m_fTonadoAng);
		D3DMath_MatrixMultiply( m_matWorld, m_matRotate, m_matWorld);

		DWORD dwCurTime = timeGetTime();
		if ( dwCurTime - m_dwBaseTime >= 25 )
		{
			m_fTonadoAng += 0.175f;
			m_dwBaseTime = dwCurTime;
		}
	}
	GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_WORLD, &m_matWorld);
	GET_D3DDEVICE()->GetTransform(D3DTRANSFORMSTATE_VIEW, &m_matView);

	m_pRain->Render3( g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_WEATHERMGR_RAIN ] ) ,
					  g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_WEATHERMGR_DROP ] ) ,
					  &m_matWorld, &m_matView, m_nRainWeight, TRUE);

	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, FALSE);
}

void CWeatherMgr::RenderSakura()
{
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, TRUE);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA);

	if (m_bLogin)
	{
		D3DUtil_SetTranslateMatrix(m_matWorld, D3DVECTOR(LOGIN_SNOW_X, LOGIN_SNOW_Y, LOGIN_SNOW_Z));
	}
	else
	{
		D3DUtil_SetTranslateMatrix(m_matWorld, pCMyApp->GetEyeAtPos());
	}
	if (m_bTonado)
	{
		D3DUtil_SetRotateYMatrix(m_matRotate, m_fTonadoAng);
		D3DMath_MatrixMultiply( m_matWorld, m_matRotate, m_matWorld);

		DWORD dwCurTime = timeGetTime();
		if ( dwCurTime - m_dwBaseTime >= 25 )
		{
			m_fTonadoAng += 0.175f;
			m_dwBaseTime = dwCurTime;
		}
	}
	GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_WORLD, &m_matWorld);
	GET_D3DDEVICE()->GetTransform(D3DTRANSFORMSTATE_VIEW, &m_matView);

	m_pRain->Render3( g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_WEATHERMGR_SAKURA_DROP ] ) ,
					  g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_WEATHERMGR_DROP ] ) ,
					  &m_matWorld, &m_matView, m_nRainWeight, TRUE);

	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, FALSE);
}

void CWeatherMgr::Render()
{
	if( m_bShowSnow )
	{
		if( g_bSakura )
		{
			RenderSakura();
		}
		else
		{
			RenderSnow();
		}
	}

	if (m_bShowRain)
	{
		RenderRain();
	}
}

D3DLVERTEX CRains::m_tmpLvertex[4];
int	CRains::m_nInit = 0;

CRainObj::CRainObj(float x, float z, float centerX, float centerY, float centerZ)
	: m_Next(NULL)
	, m_fSpeed(0)
	, m_fSpd_x(0)
	, m_move(0)
{
	if (centerX != 0.0f)
	{
		m_RainBottom.x = x + centerX;
		m_RainBottom.z = z + centerZ;
		// 시작 높이를 RAIN_START로 했지만, 바닥이 0이 아니니.. 이것도 잘 하여야 할것이다...
		m_RainBottom.y = RAIN_START + centerY;
	}
	else
	{
		m_RainBottom.x = x + g_pRoh->m_wx;
		m_RainBottom.z = z + g_pRoh->m_wz;
		// 시작 높이를 RAIN_START로 했지만, 바닥이 0이 아니니.. 이것도 잘 하여야 할것이다...
		m_RainBottom.y = RAIN_START + g_pRoh->m_wy;
	}
	m_PosY = RAIN_START;
}

//----------------------------------------------------------------------------
//-- Constructor : CRains
//----------------------------------------------------------------------------
//-- 2005.06.11
//-- Lyul / 수정완료

//-- m_bLogin 을 초기화 해주지 않았다
//-- 비가 와야 되는데 항상 눈으로 생성되어서 오류
CRains::CRains()
{
	m_LastTime = g_nRandomSpeed = timeGetTime();
	m_RainList = NULL;
	m_bLogin = false;

	for (int i = 0; i < RAIN_MAX; i++)
	{
		m_RainIndex[i*MAX_INDEX+0] = i*MAX_VERTEX+0;
		m_RainIndex[i*MAX_INDEX+1] = i*MAX_VERTEX+1;
		m_RainIndex[i*MAX_INDEX+2] = i*MAX_VERTEX+2;
		m_RainIndex[i*MAX_INDEX+3] = i*MAX_VERTEX+1;
		m_RainIndex[i*MAX_INDEX+4] = i*MAX_VERTEX+3;
		m_RainIndex[i*MAX_INDEX+5] = i*MAX_VERTEX+2;
	}
	for ( int i = 0; i < DROP_MAX; i++)
	{
		m_DropIndex[i*MAX_INDEX+0] = i*MAX_VERTEX+0;
		m_DropIndex[i*MAX_INDEX+1] = i*MAX_VERTEX+1;
		m_DropIndex[i*MAX_INDEX+2] = i*MAX_VERTEX+2;
		m_DropIndex[i*MAX_INDEX+3] = i*MAX_VERTEX+1;
		m_DropIndex[i*MAX_INDEX+4] = i*MAX_VERTEX+3;
		m_DropIndex[i*MAX_INDEX+5] = i*MAX_VERTEX+2;
	}
	/*rainIndex[0] = 0;
	rainIndex[1] = 1;
	rainIndex[2] = 2;
	rainIndex[3] = 1;
	rainIndex[4] = 3;
	rainIndex[5] = 2;*/
	// 정적 변수 초기화.
	if (m_nInit == 0)
	{
		m_nInit = 1;

		m_tmpLvertex[0] = D3DLVERTEX(D3DVECTOR(SNOW_WIDTH, SNOW_WIDTH, 0),
									 0x00ffffff, 0L, 0, 1);
		m_tmpLvertex[1] = D3DLVERTEX(D3DVECTOR(SNOW_WIDTH, SNOW_WIDTH, 0),
									 0x00ffffff, 0L, 0, 0);
		m_tmpLvertex[2] = D3DLVERTEX(D3DVECTOR(SNOW_WIDTH, SNOW_WIDTH, 0),
									 0x00ffffff, 0L, 1, 1);
		m_tmpLvertex[3] = D3DLVERTEX(D3DVECTOR(SNOW_WIDTH, SNOW_WIDTH, 0),
									 0x00ffffff, 0L, 1, 0);
	}
}

CRains::~CRains()
{
	RemoveAllRains();
}

void CRains::AddRain(int weight)
{
	for (int i = 0; i < weight; i++)
	{
		float x = FloatRandom(-RAIN_AREA, RAIN_AREA);
		float z = FloatRandom(-RAIN_AREA, RAIN_AREA);
		CRainObj *pRain;
		if (m_bLogin)
			pRain = new CRainObj(x, z, LOGIN_SNOW_X, LOGIN_SNOW_Y, LOGIN_SNOW_Z);
		else
			pRain = new CRainObj(x, z);
		
		pRain->m_fSpeed	= 1.0f;
		pRain->m_fSpd_x	= 0.0001f;
		pRain->m_move	= FloatRandom( -3.0f, 3.0f );

		pRain->m_Next = m_RainList;
		m_RainList = pRain;
	}
}

void CRains::RemoveAllRains()
{
	while(m_RainList)
	{
		CRainObj *pRain = m_RainList;
		m_RainList = pRain->m_Next;

		delete pRain;
	}
}


void CRains::Render(LPDIRECTDRAWSURFACE7 rainTexture, LPDIRECTDRAWSURFACE7 dropTexture,
					D3DMATRIX *pWorldMat, D3DMATRIX *pViewMat, int weight, BOOL isSnow)
{
	float speed, move;
	if (isSnow)
	{
		speed = SNOW_SPEED;
		move = SNOW_MOVE;
	}
	else
	{
		speed = RAIN_SPEED;
		move = RAIN_MOVE;
	}

	CRainObj *pRain, *pBeforeRain = NULL, *pDelRain;
	int curTime = timeGetTime();
	if (curTime > m_LastTime + RAIN_TIME)
	{
		m_LastTime = curTime;

		pRain = m_RainList;
		while (pRain)
		{
			pRain->m_RainBottom.y -= speed;
			if (pRain->m_RainBottom.y > 0)
			{
				pRain->m_RainBottom.x += FloatRandom(-move, move);
				pRain->m_RainBottom.z += FloatRandom(-move, move);
			}
			// 이게 바닥0을 기준으로 speed만큼 내려갔으면 없애버리는데...
			// 바닥이 0은 아니겠지? 0 ~ -speed 사이에선... 물방울 튀는 걸 보여준다...
			if (pRain->m_RainBottom.y < -(speed*2))
			{
				pDelRain = pRain;
				if (pBeforeRain)
				{
					pRain = pBeforeRain->m_Next = pRain->m_Next;
				}
				else
				{
					pRain = m_RainList = pRain->m_Next;
				}

				delete pDelRain;
				continue;
			}
			pBeforeRain = pRain;
			pRain = pRain->m_Next;
		}

		AddRain(weight);
	}

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE );

	D3DMATRIX matView, matInv;
	//D3DMath_MatrixMultiply(matView, *pWorldMat, *pViewMat);
	//D3DMath_MatrixInvert(matInv, matView);
	matView = *pViewMat;
	D3DMath_MatrixInvert(matInv, *pViewMat);


	pRain = m_RainList;
	int rain_count = 0;
	int drop_count = 0;
	while (pRain)
	{
		D3DVECTOR rainVector, rainVector2;
		//D3DLVERTEX rainVertex[4];
		if (isSnow)
		{
			// 눈의 위치를 View 좌표로 변환
			D3DMath_VectorMatrixMultiply(rainVector, pRain->m_RainBottom, matView);
			// View 좌표를 정면으로 바라보는 정사각형을 생성.
			m_RainVertex[rain_count*MAX_VERTEX+0] = D3DLVERTEX(D3DVECTOR(rainVector.x + SNOW_WIDTH,
													rainVector.y + SNOW_WIDTH,
													rainVector.z),
													0x00ffffff, 0L, 0, 1);
			m_RainVertex[rain_count*MAX_VERTEX+1] = D3DLVERTEX(D3DVECTOR(rainVector.x + SNOW_WIDTH,
													rainVector.y - SNOW_WIDTH,
													rainVector.z),
													0x00ffffff, 0L, 0, 0);
			m_RainVertex[rain_count*MAX_VERTEX+2] = D3DLVERTEX(D3DVECTOR(rainVector.x - SNOW_WIDTH,
													rainVector.y + SNOW_WIDTH,
													rainVector.z),
													0x00ffffff, 0L, 1, 1);
			m_RainVertex[rain_count*MAX_VERTEX+3] = D3DLVERTEX(D3DVECTOR(rainVector.x - SNOW_WIDTH,
													rainVector.y - SNOW_WIDTH,
													rainVector.z),
													0x00ffffff, 0L, 1, 0);

			// View에서 생성된 사각형을 다시 월드 좌표로 변환...
			NKMath_LVertexMatrixMultiply(m_RainVertex[rain_count*MAX_VERTEX+0], m_RainVertex[rain_count*MAX_VERTEX+0], matInv);
			NKMath_LVertexMatrixMultiply(m_RainVertex[rain_count*MAX_VERTEX+1], m_RainVertex[rain_count*MAX_VERTEX+1], matInv);
			NKMath_LVertexMatrixMultiply(m_RainVertex[rain_count*MAX_VERTEX+2], m_RainVertex[rain_count*MAX_VERTEX+2], matInv);
			NKMath_LVertexMatrixMultiply(m_RainVertex[rain_count*MAX_VERTEX+3], m_RainVertex[rain_count*MAX_VERTEX+3], matInv);
		}
		else
		{
			// 높이에 따른 점을 생성하고, 그 점을 View 좌표로 변환한다.
			rainVector2 = pRain->m_RainBottom;
			rainVector2.y += RAIN_HEIGHT;

			D3DMath_VectorMatrixMultiply(rainVector, pRain->m_RainBottom, matView);
			D3DMath_VectorMatrixMultiply(rainVector2, rainVector2, matView);

			// 직각인 벡터를 구한다.
			D3DVECTOR rainInv;
			rainInv = rainVector;
			rainInv -= rainVector2;

			FLOAT x_temp = rainInv.x;
			rainInv.x = rainInv.y;
			rainInv.y = x_temp;
			rainInv.z = 0;
			rainInv = Normalize(rainInv);
			rainInv *= RAIN_WIDTH;

			// View 좌표의 사각형
			m_RainVertex[rain_count*MAX_VERTEX+0] = D3DLVERTEX(D3DVECTOR(rainVector.x + rainInv.x,
													rainVector.y - rainInv.y,
													rainVector.z),
													0x00ffffff, 0L, 0, 1);
			m_RainVertex[rain_count*MAX_VERTEX+1] = D3DLVERTEX(D3DVECTOR(rainVector2.x + rainInv.x,
													rainVector2.y - rainInv.y,
													rainVector2.z),
													0x00ffffff, 0L, 0, 0);
			m_RainVertex[rain_count*MAX_VERTEX+2] = D3DLVERTEX(D3DVECTOR(rainVector.x - rainInv.x,
													rainVector.y + rainInv.y,
													rainVector.z),
													0x00ffffff, 0L, 1, 1);
			m_RainVertex[rain_count*MAX_VERTEX+3] = D3DLVERTEX(D3DVECTOR(rainVector2.x - rainInv.x,
													rainVector2.y + rainInv.y,
													rainVector2.z),
													0x00ffffff, 0L, 1, 0);

			// 월드 좌표로 다시 변환...
			NKMath_LVertexMatrixMultiply(m_RainVertex[rain_count*MAX_VERTEX+0], m_RainVertex[rain_count*MAX_VERTEX+0], matInv);
			NKMath_LVertexMatrixMultiply(m_RainVertex[rain_count*MAX_VERTEX+1], m_RainVertex[rain_count*MAX_VERTEX+1], matInv);
			NKMath_LVertexMatrixMultiply(m_RainVertex[rain_count*MAX_VERTEX+2], m_RainVertex[rain_count*MAX_VERTEX+2], matInv);
			NKMath_LVertexMatrixMultiply(m_RainVertex[rain_count*MAX_VERTEX+3], m_RainVertex[rain_count*MAX_VERTEX+3], matInv);
			if (m_RainVertex[rain_count*MAX_VERTEX+0].y < 0.0f)
				m_RainVertex[rain_count*MAX_VERTEX+0].y = 0.0f;
			if (m_RainVertex[rain_count*MAX_VERTEX+2].y < 0.0f)
				m_RainVertex[rain_count*MAX_VERTEX+2].y = 0.0f;
		}

		// 비가 바닥보다 낮아졌을때... 0.0f보다 낮을때.. 비 튀기는 걸 그려준다...
		// 근데.. 바닥이 0이 아니니.. 이를 어쩌나...
		if (!isSnow && pRain->m_RainBottom.y <= 0.0f && drop_count < DROP_MAX)
		{
			float drop_width;
			if (pRain->m_RainBottom.y <= -speed)
			{
				drop_width = DROP_WIDTH * 2.0f;
			}
			else
			{
				drop_width = DROP_WIDTH;
			}
			// 비가 바닥에 닿았으면, DROP_HEIGHT 높이에 바닥과 평행하게 사각형을 만든다.
			m_DropVertex[drop_count*MAX_VERTEX+0] = D3DLVERTEX(D3DVECTOR(pRain->m_RainBottom.x - drop_width,
													DROP_HEIGHT,
													pRain->m_RainBottom.z - drop_width),
													0x00ffffff, 0L, 0, 1);
			m_DropVertex[drop_count*MAX_VERTEX+1] = D3DLVERTEX(D3DVECTOR(pRain->m_RainBottom.x - drop_width,
													DROP_HEIGHT,
													pRain->m_RainBottom.z + drop_width),
													0x00ffffff, 0L, 0, 0);
			m_DropVertex[drop_count*MAX_VERTEX+2] = D3DLVERTEX(D3DVECTOR(pRain->m_RainBottom.x + drop_width,
													DROP_HEIGHT,
													pRain->m_RainBottom.z - drop_width),
													0x00ffffff, 0L, 1, 1);
			m_DropVertex[drop_count*MAX_VERTEX+3] = D3DLVERTEX(D3DVECTOR(pRain->m_RainBottom.x + drop_width,
													DROP_HEIGHT,
													pRain->m_RainBottom.z + drop_width),
													0x00ffffff, 0L, 1, 0);

			drop_count++;
		}

		pRain = pRain->m_Next;
		rain_count++;
		if (rain_count >= RAIN_MAX)
			break;
	}
	// 비나 눈 그리기...
	if (rain_count > 0)
	{
		if (GET_D3DDEVICE()->SetTexture(0, rainTexture) != DD_OK)
		{
			GET_D3DDEVICE()->SetTexture(0, NULL);
		}
		GET_D3DDEVICE()->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, D3DFVF_LVERTEX ,
											   m_RainVertex, rain_count*MAX_VERTEX, m_RainIndex, rain_count*MAX_INDEX, NULL );
		/*GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP , D3DFVF_LVERTEX ,
						rainVertex, 4, NULL );*/
		/*GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP , D3DFVF_VERTEX ,
						rainVertex, 4, NULL );*/
	}

	// 튀기는 물 그리기....
	if (drop_count > 0)
	{
		if (GET_D3DDEVICE()->SetTexture(0, dropTexture) != DD_OK)
		{
			GET_D3DDEVICE()->SetTexture(0, NULL);
		}
		GET_D3DDEVICE()->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, D3DFVF_LVERTEX ,
											   m_DropVertex, drop_count*MAX_VERTEX, m_DropIndex, drop_count*MAX_INDEX, NULL );
	}
	// 다시 조명 켠다.
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE );
}

void CRains::Drop()
{
}

void CRains::Render2(LPDIRECTDRAWSURFACE7 rainTexture, LPDIRECTDRAWSURFACE7 dropTexture,
					 D3DMATRIX *pWorldMat, D3DMATRIX *pViewMat, int weight, BOOL isSnow)
{
	float speed, move;
	if (isSnow)
	{
		speed = SNOW_SPEED;
		move = SNOW_MOVE;
	}
	else
	{
		speed = RAIN_SPEED;
		move = RAIN_MOVE;
	}

	CRainObj *pRain, *pBeforeRain = NULL, *pDelRain;
	int curTime = timeGetTime();
	if (curTime > m_LastTime + RAIN_TIME)
	{
		m_LastTime = curTime;

		pRain = m_RainList;
		while (pRain)
		{
			pRain->m_RainBottom.y -= speed;
			if (pRain->m_RainBottom.y > 0)
			{
				pRain->m_RainBottom.x += FloatRandom(-move, move);
				pRain->m_RainBottom.z += FloatRandom(-move, move);
			}
			// 이게 바닥0을 기준으로 speed만큼 내려갔으면 없애버리는데...
			// 바닥이 0은 아니겠지? 0 ~ -speed 사이에선... 물방울 튀는 걸 보여준다...
			if (pRain->m_RainBottom.y < -(speed*2))
			{
				pDelRain = pRain;
				if (pBeforeRain)
				{
					pRain = pBeforeRain->m_Next = pRain->m_Next;
				}
				else
				{
					pRain = m_RainList = pRain->m_Next;
				}

				delete pDelRain;
				continue;
			}
			pBeforeRain = pRain;
			pRain = pRain->m_Next;
		}

		AddRain(weight);
	}

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE );

	D3DMATRIX matView, matInv;
	D3DMath_MatrixMultiply(matView, *pWorldMat, *pViewMat);
	D3DMath_MatrixInvert(matInv, matView);
	//matView = *pViewMat;
	//D3DMath_MatrixInvert(matInv, *pViewMat);

	pRain = m_RainList;
	int rain_count = 0;
	int drop_count = 0;
	while (pRain)
	{
		D3DVECTOR rainVector, rainVector2;
		//D3DLVERTEX rainVertex[4];
		if (isSnow)
		{
			// 눈의 위치를 View 좌표로 변환
			D3DMath_VectorMatrixMultiply(rainVector, pRain->m_RainBottom, matView);
			// View 좌표를 정면으로 바라보는 정사각형을 생성.
			m_RainVertex[rain_count*MAX_VERTEX+0] = D3DLVERTEX(D3DVECTOR(rainVector.x + SNOW_WIDTH,
													rainVector.y + SNOW_WIDTH,
													rainVector.z),
													0x00ffffff, 0L, 0, 1);
			m_RainVertex[rain_count*MAX_VERTEX+1] = D3DLVERTEX(D3DVECTOR(rainVector.x + SNOW_WIDTH,
													rainVector.y - SNOW_WIDTH,
													rainVector.z),
													0x00ffffff, 0L, 0, 0);
			m_RainVertex[rain_count*MAX_VERTEX+2] = D3DLVERTEX(D3DVECTOR(rainVector.x - SNOW_WIDTH,
													rainVector.y + SNOW_WIDTH,
													rainVector.z),
													0x00ffffff, 0L, 1, 1);
			m_RainVertex[rain_count*MAX_VERTEX+3] = D3DLVERTEX(D3DVECTOR(rainVector.x - SNOW_WIDTH,
													rainVector.y - SNOW_WIDTH,
													rainVector.z),
													0x00ffffff, 0L, 1, 0);

			// View에서 생성된 사각형을 다시 월드 좌표로 변환...
			//NKMath_LVertexMatrixMultiply(m_RainVertex[rain_count*MAX_VERTEX+0], m_RainVertex[rain_count*MAX_VERTEX+0], matInv);
			//NKMath_LVertexMatrixMultiply(m_RainVertex[rain_count*MAX_VERTEX+1], m_RainVertex[rain_count*MAX_VERTEX+1], matInv);
			//NKMath_LVertexMatrixMultiply(m_RainVertex[rain_count*MAX_VERTEX+2], m_RainVertex[rain_count*MAX_VERTEX+2], matInv);
			//NKMath_LVertexMatrixMultiply(m_RainVertex[rain_count*MAX_VERTEX+3], m_RainVertex[rain_count*MAX_VERTEX+3], matInv);
		}
		else
		{
			// 높이에 따른 점을 생성하고, 그 점을 View 좌표로 변환한다.
			rainVector2 = pRain->m_RainBottom;
			rainVector2.y += RAIN_HEIGHT;
			if (rainVector2.y < 0.0f)
				rainVector2.y = 0.0f;

			D3DVECTOR bottom = pRain->m_RainBottom;
			if (bottom.y < 0.0f)
				bottom.y = 0.0f;
			D3DMath_VectorMatrixMultiply(rainVector, bottom, matView);
			D3DMath_VectorMatrixMultiply(rainVector2, rainVector2, matView);

			// 직각인 벡터를 구한다.
			D3DVECTOR rainInv;
			rainInv = rainVector;
			rainInv -= rainVector2;

			FLOAT x_temp = rainInv.x;
			rainInv.x = rainInv.y;
			rainInv.y = x_temp;
			rainInv.z = 0;
			rainInv = Normalize(rainInv);
			rainInv *= RAIN_WIDTH;

			m_tmpLvertex[0].x = rainVector.x + rainInv.x;
			m_tmpLvertex[0].y = rainVector.y - rainInv.y;
			m_tmpLvertex[0].z = rainVector.z;

			m_tmpLvertex[1].x = rainVector2.x + rainInv.x;
			m_tmpLvertex[1].y = rainVector2.y - rainInv.y;
			m_tmpLvertex[1].z = rainVector2.z;

			m_tmpLvertex[2].x = rainVector.x - rainInv.x;
			m_tmpLvertex[2].y = rainVector.y + rainInv.y;
			m_tmpLvertex[2].z = rainVector.z;

			m_tmpLvertex[3].x = rainVector2.x - rainInv.x;
			m_tmpLvertex[3].y = rainVector2.y + rainInv.y;
			m_tmpLvertex[3].z = rainVector2.z;

			memcpy (&m_RainVertex[rain_count*MAX_VERTEX], m_tmpLvertex, sizeof(D3DLVERTEX)*MAX_VERTEX);
			// View 좌표의 사각형
			/*
			m_RainVertex[rain_count*MAX_VERTEX+0] = D3DLVERTEX(D3DVECTOR(rainVector.x + rainInv.x,
										rainVector.y - rainInv.y,
										rainVector.z),
										0x00ffffff, 0L, 0, 1);
			m_RainVertex[rain_count*MAX_VERTEX+1] = D3DLVERTEX(D3DVECTOR(rainVector2.x + rainInv.x,
										rainVector2.y - rainInv.y,
										rainVector2.z),
										0x00ffffff, 0L, 0, 0);
			m_RainVertex[rain_count*MAX_VERTEX+2] = D3DLVERTEX(D3DVECTOR(rainVector.x - rainInv.x,
										rainVector.y + rainInv.y,
										rainVector.z),
										0x00ffffff, 0L, 1, 1);
			m_RainVertex[rain_count*MAX_VERTEX+3] = D3DLVERTEX(D3DVECTOR(rainVector2.x - rainInv.x,
										rainVector2.y + rainInv.y,
										rainVector2.z),
										0x00ffffff, 0L, 1, 0);

			// 월드 좌표로 다시 변환...

			NKMath_LVertexMatrixMultiply(m_RainVertex[rain_count*MAX_VERTEX+0], m_RainVertex[rain_count*MAX_VERTEX+0], matInv);
			NKMath_LVertexMatrixMultiply(m_RainVertex[rain_count*MAX_VERTEX+1], m_RainVertex[rain_count*MAX_VERTEX+1], matInv);
			NKMath_LVertexMatrixMultiply(m_RainVertex[rain_count*MAX_VERTEX+2], m_RainVertex[rain_count*MAX_VERTEX+2], matInv);
			NKMath_LVertexMatrixMultiply(m_RainVertex[rain_count*MAX_VERTEX+3], m_RainVertex[rain_count*MAX_VERTEX+3], matInv);
			*/

			// 이건 몰라서 지운다. - 정원.
			/*
			if (m_RainVertex[rain_count*MAX_VERTEX+0].y < 0.0f)
				m_RainVertex[rain_count*MAX_VERTEX+0].y = 0.0f;
			if (m_RainVertex[rain_count*MAX_VERTEX+2].y < 0.0f)
				m_RainVertex[rain_count*MAX_VERTEX+2].y = 0.0f;
			*/
		}

		// 비가 바닥보다 낮아졌을때... 0.0f보다 낮을때.. 비 튀기는 걸 그려준다...
		// 근데.. 바닥이 0이 아니니.. 이를 어쩌나...
		if (!isSnow && pRain->m_RainBottom.y <= 0.0f && drop_count < DROP_MAX)
		{
			float drop_width;
			if (pRain->m_RainBottom.y <= -speed)
			{
				drop_width = DROP_WIDTH * 2.0f;
			}
			else
			{
				drop_width = DROP_WIDTH;
			}
			// 비가 바닥에 닿았으면, DROP_HEIGHT 높이에 바닥과 평행하게 사각형을 만든다.

			m_tmpLvertex[0].x = pRain->m_RainBottom.x - drop_width;
			m_tmpLvertex[0].y = DROP_HEIGHT;
			m_tmpLvertex[0].z = pRain->m_RainBottom.z - drop_width;

			m_tmpLvertex[1].x = pRain->m_RainBottom.x - drop_width;
			m_tmpLvertex[1].y = DROP_HEIGHT;
			m_tmpLvertex[1].z = pRain->m_RainBottom.z + drop_width;

			m_tmpLvertex[2].x = pRain->m_RainBottom.x + drop_width;
			m_tmpLvertex[2].y = DROP_HEIGHT;
			m_tmpLvertex[2].z = pRain->m_RainBottom.z - drop_width;

			m_tmpLvertex[3].x = pRain->m_RainBottom.x + drop_width;
			m_tmpLvertex[3].y = DROP_HEIGHT;
			m_tmpLvertex[3].z = pRain->m_RainBottom.z + drop_width;

			memcpy(&m_DropVertex[drop_count*MAX_VERTEX], m_tmpLvertex, sizeof(D3DLVERTEX)*MAX_VERTEX);

			/*
			m_DropVertex[drop_count*MAX_VERTEX+0] = D3DLVERTEX(D3DVECTOR(pRain->m_RainBottom.x - drop_width,
										DROP_HEIGHT,
										pRain->m_RainBottom.z - drop_width),
										0x00ffffff, 0L, 0, 1);
			m_DropVertex[drop_count*MAX_VERTEX+1] = D3DLVERTEX(D3DVECTOR(pRain->m_RainBottom.x - drop_width,
										DROP_HEIGHT,
										pRain->m_RainBottom.z + drop_width),
										0x00ffffff, 0L, 0, 0);
			m_DropVertex[drop_count*MAX_VERTEX+2] = D3DLVERTEX(D3DVECTOR(pRain->m_RainBottom.x + drop_width,
										DROP_HEIGHT,
										pRain->m_RainBottom.z - drop_width),
										0x00ffffff, 0L, 1, 1);
			m_DropVertex[drop_count*MAX_VERTEX+3] = D3DLVERTEX(D3DVECTOR(pRain->m_RainBottom.x + drop_width,
										DROP_HEIGHT,
										pRain->m_RainBottom.z + drop_width),
										0x00ffffff, 0L, 1, 0);
			*/

			drop_count++;
		}

		pRain = pRain->m_Next;
		rain_count++;
		if (rain_count >= RAIN_MAX)
			break;
	}
	// 비나 눈 그리기...
	if (rain_count > 0)
	{
		if (GET_D3DDEVICE()->SetTexture(0, rainTexture) != DD_OK)
		{
			GET_D3DDEVICE()->SetTexture(0, NULL);
		}

		D3DMATRIX matWorldOld, matViewOld, matId;

		// 상태값 가져오기
		GET_D3DDEVICE()->GetTransform( D3DTRANSFORMSTATE_WORLD, &matWorldOld);
		GET_D3DDEVICE()->GetTransform( D3DTRANSFORMSTATE_VIEW, &matViewOld);

		// NULL로 설정하면 identity matrix.
		D3DUtil_SetIdentityMatrix(matId);
		GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &matId);
		GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_VIEW, &matId);

		GET_D3DDEVICE()->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, D3DFVF_LVERTEX ,
											   m_RainVertex, rain_count*MAX_VERTEX, m_RainIndex, rain_count*MAX_INDEX, NULL );
		// 상태값 되돌리기.
		GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &matWorldOld);
		GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_VIEW, &matViewOld);
		/*GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP , D3DFVF_LVERTEX ,
						rainVertex, MAX_VERTEX, NULL );*/
		/*GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP , D3DFVF_VERTEX ,
						rainVertex, MAX_VERTEX, NULL );*/
	}

	// 튀기는 물 그리기....
	if (drop_count > 0)
	{
		if (GET_D3DDEVICE()->SetTexture(0, dropTexture) != DD_OK)
		{
			GET_D3DDEVICE()->SetTexture(0, NULL);
		}
		GET_D3DDEVICE()->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, D3DFVF_LVERTEX ,
											   m_DropVertex, drop_count*MAX_VERTEX, m_DropIndex, drop_count*MAX_INDEX, NULL );
	}
	// 다시 조명 켠다.
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE );
}

void CRains::Render3(LPDIRECTDRAWSURFACE7 rainTexture, LPDIRECTDRAWSURFACE7 dropTexture,
					 D3DMATRIX *pWorldMat, D3DMATRIX *pViewMat, int weight, BOOL isSnow)
{
	float speed, move;
	if (isSnow)
	{
		speed = SNOW_SPEED;
		move = SNOW_MOVE;
	}
	else
	{
		speed = RAIN_SPEED;
		move = RAIN_MOVE;
	}

	CRainObj *pRain, *pBeforeRain = NULL, *pDelRain;
	int curTime = timeGetTime();

	if( g_bSakura )
	{
		if (curTime > m_LastTime + RAIN_TIME)
		{
			m_LastTime = curTime;

			pRain = m_RainList;

			while (pRain)
			{
				// 내리는 스피드에 가중치를 주자 ( 나름 벚꽃답게.. )
				SakuraSpeed(pRain);
				//pRain->m_fSpeed -= 0.01;


				pRain->m_RainBottom.y -= speed;
				pRain->m_PosY -= speed;

				if (pRain->m_PosY > 0)
				{
					move = 0;
					pRain->m_RainBottom.x += pRain->m_move;
					pRain->m_RainBottom.z += pRain->m_move;
				}

				// 이게 바닥0을 기준으로 speed만큼 내려갔으면 없애버리는데...
				// 바닥이 0은 아니겠지? 0 ~ -speed 사이에선... 물방울 튀는 걸 보여준다...
				if (pRain->m_PosY < 0 )
				{
					pDelRain = pRain;
					if (pBeforeRain)
						pRain = pBeforeRain->m_Next = pRain->m_Next;

					else
						pRain = m_RainList = pRain->m_Next;

					delete pDelRain;
					continue;
				}

				if (m_bLogin)
				{
					if (pRain->m_RainBottom.x - LOGIN_SNOW_X > RAIN_AREA + move)
						pRain->m_RainBottom.x -= RAIN_AREA * 2;
					else if (LOGIN_SNOW_X - pRain->m_RainBottom.x > RAIN_AREA + move)
						pRain->m_RainBottom.x += RAIN_AREA * 2;
					if (pRain->m_RainBottom.z - LOGIN_SNOW_Z > RAIN_AREA + move)
						pRain->m_RainBottom.z -= RAIN_AREA * 2;
					else if (LOGIN_SNOW_Z - pRain->m_RainBottom.z > RAIN_AREA + move)
						pRain->m_RainBottom.z += RAIN_AREA * 2;

				}
				else
				{
					if (pRain->m_RainBottom.x - g_pRoh->m_wx > RAIN_AREA + move)
						pRain->m_RainBottom.x -= RAIN_AREA * 2;

					else if (g_pRoh->m_wx - pRain->m_RainBottom.x > RAIN_AREA + move)
						pRain->m_RainBottom.x += RAIN_AREA * 2;

					if (pRain->m_RainBottom.z - g_pRoh->m_wz > RAIN_AREA + move)
						pRain->m_RainBottom.z -= RAIN_AREA * 2;

					else if (g_pRoh->m_wz - pRain->m_RainBottom.z > RAIN_AREA + move)
						pRain->m_RainBottom.z += RAIN_AREA * 2;
				}

				pBeforeRain = pRain;
				pRain = pRain->m_Next;
			}

			AddRain(weight);
		}
	}
	else
	{
		if(curTime > m_LastTime + RAIN_TIME)
		{
			m_LastTime = curTime;

			pRain = m_RainList;
			while (pRain)
			{
				pRain->m_RainBottom.y -= speed;
				pRain->m_PosY -= speed;
				if (pRain->m_PosY > 0)
				{
					pRain->m_RainBottom.x += FloatRandom(-move, move);
					pRain->m_RainBottom.z += FloatRandom(-move, move);
				}
				// 이게 바닥0을 기준으로 speed만큼 내려갔으면 없애버리는데...
				// 바닥이 0은 아니겠지? 0 ~ -speed 사이에선... 물방울 튀는 걸 보여준다...
				if (pRain->m_PosY < -(speed*2))
				{
					pDelRain = pRain;
					if (pBeforeRain)
					{
						pRain = pBeforeRain->m_Next = pRain->m_Next;
					}
					else
					{
						pRain = m_RainList = pRain->m_Next;
					}

					delete pDelRain;
					continue;
				}

				if (m_bLogin)
				{
					if (pRain->m_RainBottom.x - LOGIN_SNOW_X > RAIN_AREA + move)
					{
						pRain->m_RainBottom.x -= RAIN_AREA * 2;
					}
					else if (LOGIN_SNOW_X - pRain->m_RainBottom.x > RAIN_AREA + move)
					{
						pRain->m_RainBottom.x += RAIN_AREA * 2;
					}
					if (pRain->m_RainBottom.z - LOGIN_SNOW_Z > RAIN_AREA + move)
					{
						pRain->m_RainBottom.z -= RAIN_AREA * 2;
					}
					else if (LOGIN_SNOW_Z - pRain->m_RainBottom.z > RAIN_AREA + move)
					{
						pRain->m_RainBottom.z += RAIN_AREA * 2;
					}
				}
				else
				{
					if (pRain->m_RainBottom.x - g_pRoh->m_wx > RAIN_AREA + move)
					{
						pRain->m_RainBottom.x -= RAIN_AREA * 2;
					}
					else if (g_pRoh->m_wx - pRain->m_RainBottom.x > RAIN_AREA + move)
					{
						pRain->m_RainBottom.x += RAIN_AREA * 2;
					}
					if (pRain->m_RainBottom.z - g_pRoh->m_wz > RAIN_AREA + move)
					{
						pRain->m_RainBottom.z -= RAIN_AREA * 2;
					}
					else if (g_pRoh->m_wz - pRain->m_RainBottom.z > RAIN_AREA + move)
					{
						pRain->m_RainBottom.z += RAIN_AREA * 2;
					}
				}

				pBeforeRain = pRain;
				pRain = pRain->m_Next;
			}

			AddRain(weight);
		}
	}

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE );

	D3DMATRIX matView, matInv;	
	matView = *pViewMat;
	D3DMath_MatrixInvert(matInv, matView);
	
	pRain = m_RainList;
	int rain_count = 0;
	int drop_count = 0;
	while (pRain)
	{
		D3DVECTOR rainVector, rainVector2;
		
		if (isSnow)
		{
			// 눈의 위치를 View 좌표로 변환
			D3DMath_VectorMatrixMultiply(rainVector, pRain->m_RainBottom, matView);
			// View 좌표를 정면으로 바라보는 정사각형을 생성.
			m_RainVertex[rain_count*MAX_VERTEX+0] = D3DLVERTEX(D3DVECTOR(rainVector.x + SNOW_WIDTH,
													rainVector.y + SNOW_WIDTH,
													rainVector.z),
													0x00ffffff, 0L, 0, 1);
			m_RainVertex[rain_count*MAX_VERTEX+1] = D3DLVERTEX(D3DVECTOR(rainVector.x + SNOW_WIDTH,
													rainVector.y - SNOW_WIDTH,
													rainVector.z),
													0x00ffffff, 0L, 0, 0);
			m_RainVertex[rain_count*MAX_VERTEX+2] = D3DLVERTEX(D3DVECTOR(rainVector.x - SNOW_WIDTH,
													rainVector.y + SNOW_WIDTH,
													rainVector.z),
													0x00ffffff, 0L, 1, 1);
			m_RainVertex[rain_count*MAX_VERTEX+3] = D3DLVERTEX(D3DVECTOR(rainVector.x - SNOW_WIDTH,
													rainVector.y - SNOW_WIDTH,
													rainVector.z),
													0x00ffffff, 0L, 1, 0);			
		}
		else
		{
			// 높이에 따른 점을 생성하고, 그 점을 View 좌표로 변환한다.
			rainVector2 = pRain->m_RainBottom;
			rainVector2.y += RAIN_HEIGHT;
			if (rainVector2.y < 0.0f)
				rainVector2.y = 0.0f;

			D3DVECTOR bottom = pRain->m_RainBottom;
			if (bottom.y < 0.0f)
				bottom.y = 0.0f;
			D3DMath_VectorMatrixMultiply(rainVector, bottom, matView);
			D3DMath_VectorMatrixMultiply(rainVector2, rainVector2, matView);

			// 직각인 벡터를 구한다.
			D3DVECTOR rainInv;
			rainInv = rainVector;
			rainInv -= rainVector2;

			FLOAT x_temp = rainInv.x;
			rainInv.x = rainInv.y;
			rainInv.y = x_temp;
			rainInv.z = 0;
			rainInv = Normalize(rainInv);
			rainInv *= RAIN_WIDTH;

			m_tmpLvertex[0].x = rainVector.x + rainInv.x;
			m_tmpLvertex[0].y = rainVector.y - rainInv.y;
			m_tmpLvertex[0].z = rainVector.z;

			m_tmpLvertex[1].x = rainVector2.x + rainInv.x;
			m_tmpLvertex[1].y = rainVector2.y - rainInv.y;
			m_tmpLvertex[1].z = rainVector2.z;

			m_tmpLvertex[2].x = rainVector.x - rainInv.x;
			m_tmpLvertex[2].y = rainVector.y + rainInv.y;
			m_tmpLvertex[2].z = rainVector.z;

			m_tmpLvertex[3].x = rainVector2.x - rainInv.x;
			m_tmpLvertex[3].y = rainVector2.y + rainInv.y;
			m_tmpLvertex[3].z = rainVector2.z;

			memcpy (&m_RainVertex[rain_count*MAX_VERTEX], m_tmpLvertex, sizeof(D3DLVERTEX)*MAX_VERTEX);
		}

		// 비가 바닥보다 낮아졌을때... 0.0f보다 낮을때.. 비 튀기는 걸 그려준다...
		// 근데.. 바닥이 0이 아니니.. 이를 어쩌나...
		if (!isSnow && pRain->m_PosY <= 0.0f && drop_count < DROP_MAX)
		{
			float drop_width;
			if (pRain->m_PosY <= -speed)
			{
				drop_width = DROP_WIDTH * 2.0f;
			}
			else
			{
				drop_width = DROP_WIDTH;
			}
			// 비가 바닥에 닿았으면, DROP_HEIGHT 높이에 바닥과 평행하게 사각형을 만든다.

			m_tmpLvertex[0].x = pRain->m_RainBottom.x - drop_width;
			m_tmpLvertex[0].y = pRain->m_RainBottom.y - pRain->m_PosY; //DROP_HEIGHT;
			m_tmpLvertex[0].z = pRain->m_RainBottom.z - drop_width;

			m_tmpLvertex[1].x = pRain->m_RainBottom.x - drop_width;
			m_tmpLvertex[1].y = pRain->m_RainBottom.y - pRain->m_PosY; //DROP_HEIGHT;
			m_tmpLvertex[1].z = pRain->m_RainBottom.z + drop_width;

			m_tmpLvertex[2].x = pRain->m_RainBottom.x + drop_width;
			m_tmpLvertex[2].y = pRain->m_RainBottom.y - pRain->m_PosY; //DROP_HEIGHT;
			m_tmpLvertex[2].z = pRain->m_RainBottom.z - drop_width;

			m_tmpLvertex[3].x = pRain->m_RainBottom.x + drop_width;
			m_tmpLvertex[3].y = pRain->m_RainBottom.y - pRain->m_PosY; //DROP_HEIGHT;
			m_tmpLvertex[3].z = pRain->m_RainBottom.z + drop_width;

			memcpy(&m_DropVertex[drop_count*MAX_VERTEX], m_tmpLvertex, sizeof(D3DLVERTEX)*MAX_VERTEX);

			drop_count++;
		}

		pRain = pRain->m_Next;
		rain_count++;
		if (rain_count >= RAIN_MAX)
			break;
	}
	// 비나 눈 그리기...
	if (rain_count > 0)
	{
		if (GET_D3DDEVICE()->SetTexture(0, rainTexture) != DD_OK)
		{
			GET_D3DDEVICE()->SetTexture(0, NULL);
		}

		D3DMATRIX matWorldOld, matViewOld, matId;

		// 상태값 가져오기
		GET_D3DDEVICE()->GetTransform( D3DTRANSFORMSTATE_WORLD, &matWorldOld);
		GET_D3DDEVICE()->GetTransform( D3DTRANSFORMSTATE_VIEW, &matViewOld);

		// NULL로 설정하면 identity matrix.
		D3DUtil_SetIdentityMatrix(matId);
		GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &matId);
		GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_VIEW, &matId);

		GET_D3DDEVICE()->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, D3DFVF_LVERTEX ,
											   m_RainVertex, rain_count*MAX_VERTEX, m_RainIndex, rain_count*MAX_INDEX, NULL );
		// 상태값 되돌리기.
		GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_VIEW, &matViewOld);
		// 튀기는 물 그리기....
		if (drop_count > 0)
		{
			if (GET_D3DDEVICE()->SetTexture(0, dropTexture) != DD_OK)
			{
				GET_D3DDEVICE()->SetTexture(0, NULL);
			}
			GET_D3DDEVICE()->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, D3DFVF_LVERTEX ,
												   m_DropVertex, drop_count*MAX_VERTEX, m_DropIndex, drop_count*MAX_INDEX, NULL );
		}
		// 상태값 되돌리기.
		GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &matWorldOld);
	}

	// 다시 조명 켠다.
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE );
}

#define	r_m  (unsigned long)2147483647
#define	r_q  (unsigned long)127773

#define	r_a (unsigned int)16807
#define	r_r (unsigned int)2836

float FloatRandom(float from, float to)
{
	register int lo, hi, test;

	hi   = g_nRandomSpeed/r_q;
	lo   = g_nRandomSpeed%r_q;

	test = r_a*lo - r_r*hi;

	if (test > 0)
		g_nRandomSpeed = test;
	else
		g_nRandomSpeed = test + r_m;

	return ((g_nRandomSpeed % (int)(to - from + 1)) + from);
}

static double nSpeed = 0;

void CRains::SakuraSpeed(CRainObj* sakuraobj)
{

	sakuraobj->m_fSpeed -= sakuraobj->m_fSpd_x;
	sakuraobj->m_fSpd_x = sakuraobj->m_fSpd_x * 2;

	if(sakuraobj->m_move < 0)
	{
		sakuraobj->m_move += 0.0005f;
	}

	else
	{
		sakuraobj->m_move -= 0.0005f;
	}


	if( sakuraobj->m_fSpeed < 0.0f )
	{
		sakuraobj->m_fSpeed = 0.5f;
		sakuraobj->m_fSpd_x = 0.0001f;
		sakuraobj->m_move = FloatRandom(-0.5f, 0.5f);
		nSpeed = 0;
	}
}
