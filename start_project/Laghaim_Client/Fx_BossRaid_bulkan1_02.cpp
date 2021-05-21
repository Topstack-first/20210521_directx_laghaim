#include "stdafx.h"
#define D3D_OVERLOADS


#include <d3d.h>
#include <time.h>
#include <assert.h>
#include "FxSet.h"
#include "main.h"
#include "D3DMath.h"
#include "D3DUtil.h"
#include "sfxUtil.h"
#include "SFXBaseShape.h"
#include "IndexedTexture.h"
#include "ShadowRect.h"
#include "WeatherMgr.h"
#include "DirtSnd.h"
#include "LoadEffectSound.h"
#include "ImageDefine.h"
#include "CapSyong.h"
#include "GUtil.h"
#include "Capsyong.h"
#include "NkCharacter.h"
#include "NkMob.h"
#include "Fx_BossRaid_bulkan1_02.h"
#include "NkMob.h"
#include "UIMgr.h"
#include "Land.h"




extern int					g_EffectSound[];
extern float				g_fDSDistance;

CFxBossRaidBulkan1_02::CFxBossRaidBulkan1_02()
	: m_fScale(0)
{
	memset(m_LoloTextrIndex, 0, sizeof(m_LoloTextrIndex));

	m_pLolo = NULL;

	m_Smoke = new CRectPlane;
	if( m_Smoke )
		m_Smoke->Create(30.0f, 30.0f, FALSE);
}


CFxBossRaidBulkan1_02::~CFxBossRaidBulkan1_02()
{
	DeleteRes();
}

void CFxBossRaidBulkan1_02::LoadRes()
{
	PFILE *fpLolo = NULL;

	if (!m_pLolo) // 땅을 가르며 나아가는 lolo 오브젝트
	{
		m_pLolo = new CLolos;
		m_pLolo->SetIndexedTexture( g_pCapsyongTexture );
		m_pLolo->Read("data/effect/lolos/eff_moving_energe.LOL");
	}




	g_pCapsyongTexture->SetTexturePath("data/effect/textr/");

	m_LoloTextrIndex[0] = g_pCapsyongTexture->CreateTextureFromFile("moving_energe02.sek");
	m_LoloTextrIndex[1] = g_pCapsyongTexture->CreateTextureFromFile("moving_energe03.sek");
}


void CFxBossRaidBulkan1_02::DeleteRes()
{
	SAFE_DELETE( m_pLolo );
	SAFE_DELETE( m_Smoke );
}


void CFxBossRaidBulkan1_02::FrameMove( EffectSort &ef_sort, int nAddFrame)
{
	LoloFrameMove( ef_sort, nAddFrame );
	//ParticleFRameMove( ef_sort, nAddFrame );
}



void CFxBossRaidBulkan1_02::LoloFrameMove( EffectSort &ef_sort, int nAddFrame )
{
	int i=0;

	switch(ef_sort.num)
	{
	case 0: // 에펙트 대기
		{
			CNkMob* pMob = ef_sort.pMobFrom;

			if( pMob->m_nMobNum == 511 ) // 보스레이드 불칸 보스 1
			{
				if( pMob->GetMotionState() == MOB_EMPTY1 && pMob->GetCurFrame() > 25 ) // MOB_EMPTY1 == attack2 동작
				{

					D3DMATRIX matRot;
					D3DVECTOR vTmp;

					vTmp = D3DVECTOR(0.0f, 0.0f, 0.0f);
					D3DUtil_SetIdentityMatrix(matRot);
					Nk3DUtil_SetRotateCCWYMatrix(matRot,pMob->m_dir);
					D3DMath_VectorMatrixMultiply(vTmp, pMob->m_pCapSyong->m_LineVector[0], matRot);

					ef_sort.vCurPos = vTmp;
					ef_sort.vCurPos.x += pMob->m_wx;
					ef_sort.vCurPos.y = 0.0f; // += pMob->m_wy;
					ef_sort.vCurPos.z += pMob->m_wz;

//					vTmp = D3DVECTOR(0.0f, 0.0f, 0.0f);
//					D3DUtil_SetIdentityMatrix(matRot);
//					Nk3DUtil_SetRotateCCWYMatrix(matRot,pMob->m_dir);
//					D3DMath_VectorMatrixMultiply(vTmp, pMob->m_pCapSyong->m_LineVector[1], matRot);

//					ef_sort.vTargetPos = vTmp;
//					ef_sort.vTargetPos.x += pMob->m_wx;
//					ef_sort.vTargetPos.y = 0.0f; // += pMob->m_wy;
//					ef_sort.vTargetPos.z += pMob->m_wz;


					//ef_sort.vCurPos	= D3DVECTOR( pMob->m_wx, 0.0f, pMob->m_wz);	// 시작 지점
					//////////////////////////////////////////////////////////////////////////
					// 무조건 목표를 향해서 발사
					//ef_sort.vPos = ef_sort.vTargetPos - ef_sort.vCurPos;							// 목표를 향하게 하는 방향 백터 만든다.
					//////////////////////////////////////////////////////////////////////////
					// 무조건 몬서트의 앞 방향으로 발사
					ef_sort.vPos = ef_sort.vCurPos - D3DVECTOR(pMob->m_wx, 0.0f, pMob->m_wz);		// 목표를 향하게 하는 방향 백터 만든다.
					//////////////////////////////////////////////////////////////////////////
					ef_sort.vPos = Normalize( ef_sort.vPos );						// 노멀 방향 백터로 만든다.

					ef_sort.vSortPos.x = (float)atan2(ef_sort.vPos.z, ef_sort.vPos.x); // 목표 방향 값

					m_vCurPos = D3DVECTOR(0.0f, 0.0f, 0.0f);
					m_vCurPos.x = ef_sort.vCurPos.x	+= ef_sort.vPos.x;
					m_vCurPos.z = ef_sort.vCurPos.z	+= ef_sort.vPos.z;
					ef_sort.vTargetPos = ef_sort.vCurPos;
					//ef_sort.vPos		*= 10.0f; // 기본 속도 속도
					m_fScale = ef_sort.vSortPos.y = 4.0f;  // 시작 스케일
					ef_sort.dwColor		= timeGetTime();

					ef_sort.num = 1;

//					if (g_pDSound)
//					{
//						// 도끼 찍는 사운드
//						g_pDSound->Play(g_EffectSound[EFF_SND_EXPLO],
//										(m_vCurPos.x - g_pRoh->m_wx)/(g_fDSDistance*2.0f),
//										0.0f, //(vCurPos.y - g_pRoh->m_wy)/g_fDSDistance,
//										(m_vCurPos.z - g_pRoh->m_wz)/(g_fDSDistance*2.0f));
//					// 도끼 모양의 기가 방출되어 나가는 이팩트
//						//g_pDSound->Play(g_EffectSound[EFF_SND_IRON_SKIN],
//						//				0.0f, //(m_vCurPos.x - g_pRoh->m_wx)/g_fDSDistance,
//						//				0.0f, //(vCurPos.y - g_pRoh->m_wy)/g_fDSDistance,
//						//				0.0f); //(m_vCurPos.z - g_pRoh->m_wz)/g_fDSDistance);
//
//					}
				}
			}
		}
		break;

	case 1: // 이펙트 시작
		{
			/*
						// 프레임으로 계산 했을 시
						float dist = DistVecToVec( pEfsort[0].vTargetPos, pEfsort[0].vCurPos );

						if( dist >= 400.0f )
						{
							ef_sort.num = 2;
							break;
						}

						ef_sort.vCurPos.x  += ef_sort.vPos.x;
						ef_sort.vCurPos.z  += ef_sort.vPos.z;
						ef_sort.vSortPos.y += 0.05f; // 스케일
			*/

			// 시간으로 계산시
			DWORD dwTime = timeGetTime() - ef_sort.dwColor ;
			float fTagetRange = 400.0f;
			float fReachTime  = 500.0f;

			if( dwTime >= (DWORD)fReachTime )
			{
				ef_sort.num = 2;
				break;
			}

			// 랜더할 거리 계산
			m_vCurPos		= D3DVECTOR(0.0f, 0.0f, 0.0f);
			D3DVECTOR vCur	= ef_sort.vPos * (fTagetRange * ( (float)dwTime / fReachTime));
			m_vCurPos.x		= ef_sort.vCurPos.x + vCur.x;
			m_vCurPos.z		= ef_sort.vCurPos.z + vCur.z;

			//랜더할 스케일 계산
			m_fScale = ef_sort.vSortPos.y + ( 6.0f * ( (float)dwTime / fReachTime ));
		}
		break;

	case 2: // v파티클이 끝나길 기다리는
		ef_sort.num = 3;
		break;

	case 3:
		ef_sort.nCurFrame = FX_BOSSRAID_BULKAN1_02_FRAMECOUNT;
		// 이 조건에 들어오면 이펙트를 삭제한다.
		break;
	}
}

void CFxBossRaidBulkan1_02::ParticleFRameMove( EffectSort &ef_sort, int nAddFrame )
{
	int i=0;

	switch(ef_sort.num)
	{
	case 0: // 이펙트 대기
		break;

	case 1: // 이펙트 시작
	case 2:
		for(i=0; i< FX_BOSSRAID_BULKAN1_02_MAX_PARTICLE; i++ )
		{
			if(ef_sort.num != 2 && ef_sort.pParticle[i].texIndex == 0 ) // 앞으로 나아가는 이펙트가 끝나지 않았다면 파키클을 계속 생성해준다.
			{



				ef_sort.pParticle[i].texIndex = 1; // 생성된 파티클 이다.
			}
			else // 생성된 파티클들은 움직여준다.
			{

//				if() // 이조건은 파티클이 죽었는지 화인해 눚ㄴ다.
//					ef_sort.pParticle[i].texIndex == 0 // 생명을 다한 파티클이다.
			}
		}
		break;

	case 3: // 이펙트 끝
		// 이조건에 들어오면 이펙트를 삭제한다.
		break;
	}
}

void CFxBossRaidBulkan1_02::Render(EffectSort &ef_sort)
{
	if( !GET_D3DDEVICE() )
		return;

	if( ef_sort.num == 3)
		return;

	if( ef_sort.num != 1)
		return;

	DWORD dwLighting, dwZWrite, dwSrc, dwDest;

	D3DMATRIX matTrans;
	D3DMATRIX matRotate;
	D3DMATRIX matScale;
	D3DMATRIX matWorld;

	float scale = 0.2f;


	EarthCrackRender( ef_sort );
	SmokeRender( ef_sort );

	for( int i=0 ; i<FX_BOSSRAID_BULKAN1_02_MAX_COUNT; i++)
	{
		GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_LIGHTING, &dwLighting );
		GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_ZWRITEENABLE, &dwZWrite );
		GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_SRCBLEND, &dwSrc );
		GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_DESTBLEND, &dwDest );

		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ZWRITEENABLE, TRUE );
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE );
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND, D3DBLEND_ONE );
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND,   D3DBLEND_SRCALPHA );

		//////////////////////////////////////////////////////////////////////////

		D3DUtil_SetIdentityMatrix(matTrans);
		D3DUtil_SetIdentityMatrix(matRotate);
		D3DUtil_SetIdentityMatrix(matScale);
		/*
				// 프레임으로 계산시
				D3DUtil_SetTranslateMatrix( matTrans, pEfsort[i].vCurPos.x,
											pCMyApp->m_pUIMgr->m_pLand->GetHFHeight(pEfsort[i].vCurPos.x, pEfsort[i].vCurPos.z), // 지형을 타고 달려야 하기때문에 지형에서 y값을 가지고 온다.
											pEfsort[i].vCurPos.z );
				Nk3DUtil_SetRotateCCWYMatrix( matRotate, pEfsort[i].vSortPos.x ); // vSortPos.x를 방향값으로 사용.
				D3DMath_MatrixMultiply( matWorld, matRotate, matTrans );

				D3DUtil_SetScaleMatrix( matScale, 2.0f+pEfsort[i].vSortPos.y, 2.0f+pEfsort[i].vSortPos.y, 2.0f+pEfsort[i].vSortPos.y/2.0f );
				D3DMath_MatrixMultiply( matWorld, matScale, matWorld );
		*/

		// 시간으로 계산시
		D3DUtil_SetTranslateMatrix( matTrans, m_vCurPos.x,
									pCMyApp->m_pUIMgr->m_pLand->GetHFHeight(m_vCurPos.x, m_vCurPos.z), // 지형을 타고 달려야 하기때문에 지형에서 y값을 가지고 온다.
									m_vCurPos.z );
		Nk3DUtil_SetRotateCCWYMatrix( matRotate, ef_sort.vSortPos.x ); // vSortPos.x를 방향값으로 사용.
		D3DMath_MatrixMultiply( matWorld, matRotate, matTrans );

		scale = scale + ((float)(timeGetTime()%2)+1) * 0.2f;
		D3DUtil_SetScaleMatrix( matScale, m_fScale + ( scale * i), m_fScale + ( scale * i), m_fScale + ( scale * i));
		D3DMath_MatrixMultiply( matWorld, matScale, matWorld );

		GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_WORLD, &matWorld );

		if( m_pLolo)
		{
			int index = timeGetTime() % 3;
			index = (index + i) % 3;
			switch( index )
			{
			case 0:
			case 1:
				//case 2:
				m_pLolo->Render2(GET_D3DDEVICE(), m_LoloTextrIndex[index], 0, FALSE, FALSE, FALSE, &matWorld );
				break;
			case 2:
				m_pLolo->Render2(GET_D3DDEVICE(), -1, 0, FALSE, FALSE, FALSE, &matWorld );
				break;
			}
		}

		//////////////////////////////////////////////////////////////////////////

//		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
//		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
//		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState[2] );


		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, dwLighting );
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ZWRITEENABLE, dwZWrite );
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND, dwSrc );
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND, dwDest );

	}


}


void CFxBossRaidBulkan1_02::SmokeRender(EffectSort &ef_sort)
{
	DWORD LiveTime = timeGetTime() - ef_sort.dwColor;

	if( LiveTime >= 1000 )
		return;

	float up_height  = (LiveTime / 1000.0f ) * 10.0f + 4.0f;
	float smoke_fade = 0.9f - ((LiveTime / 1000.0f ) * 0.9f );

	DWORD dwTextureState[3];
	DWORD dwLighting, dwZWrite, dwSrc, dwDest;

	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_LIGHTING, &dwLighting );
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_ZWRITEENABLE, &dwZWrite );
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_SRCBLEND, &dwSrc );
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_DESTBLEND, &dwDest );

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND,   D3DBLEND_SRCALPHA );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND,  D3DBLEND_INVSRCALPHA );

	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG1, &dwTextureState[0] );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG2, &dwTextureState[1] );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAOP,   &dwTextureState[2] );

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );


	LPDIRECTDRAWSURFACE7 lpSurface = NULL;
	lpSurface = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_HELLCRY_SMOKE ] ) ;
	GET_D3DDEVICE()->SetTexture(0, lpSurface);

	D3DVECTOR	vViewVec;
	vViewVec = pCMyApp->GetEyeDirection();

	ef_sort.vTargetPos.y = pCMyApp->m_pUIMgr->m_pLand->GetHFHeight(ef_sort.vTargetPos.x, ef_sort.vTargetPos.z) + 5.0f;

	if( m_Smoke )
	{
		m_Smoke->SetColor(1.0f, 1.0f, 1.0f, smoke_fade);
		m_Smoke->SetBillBoard(vViewVec, FALSE, 0.0f);
		m_Smoke->Scale(3.0f, 3.0f, 3.0f);
		m_Smoke->SetTranslationXYZ(ef_sort.vTargetPos.x , ef_sort.vTargetPos.y + up_height* 1.5f, ef_sort.vTargetPos.z);
		m_Smoke->Render();

		m_Smoke->SetColor(1.0f, 1.0f, 1.0f, smoke_fade);
		m_Smoke->SetBillBoard(vViewVec, FALSE, 0.0f);
		m_Smoke->Scale(2.0f, 2.0f, 2.0f);
		m_Smoke->SetTranslationXYZ(ef_sort.vTargetPos.x + 15.0f, ef_sort.vTargetPos.y + up_height, ef_sort.vTargetPos.z);
		m_Smoke->Render();

		m_Smoke->SetColor(1.0f, 1.0f, 1.0f, smoke_fade);
		m_Smoke->SetBillBoard(vViewVec, FALSE, 0.0f);
		m_Smoke->Scale(4.0f, 4.0f, 4.0f);
		m_Smoke->SetTranslationXYZ(ef_sort.vTargetPos.x, ef_sort.vTargetPos.y + up_height, ef_sort.vTargetPos.z + 15.0f);
		m_Smoke->Render();

		m_Smoke->SetColor(1.0f, 1.0f, 1.0f, smoke_fade);
		m_Smoke->SetBillBoard(vViewVec, FALSE, 0.0f);
		m_Smoke->Scale(2.0f, 3.0f, 2.0f);
		m_Smoke->SetTranslationXYZ(ef_sort.vTargetPos.x - 15.0f, ef_sort.vTargetPos.y + up_height, ef_sort.vTargetPos.z);
		m_Smoke->Render();

		m_Smoke->SetColor(1.0f, 1.0f, 1.0f, smoke_fade);
		m_Smoke->SetBillBoard(vViewVec, FALSE, 0.0f);
		m_Smoke->Scale(2.0f, 2.0f, 2.0f);
		m_Smoke->SetTranslationXYZ(ef_sort.vTargetPos.x, ef_sort.vTargetPos.y + up_height, ef_sort.vTargetPos.z - 15.0f);
		m_Smoke->Render();

		m_Smoke->SetColor(1.0f, 1.0f, 1.0f, smoke_fade);
		m_Smoke->SetBillBoard(vViewVec, FALSE, 0.0f);
		m_Smoke->Scale(4.0f, 5.0f, 4.0f);
		m_Smoke->SetTranslationXYZ(ef_sort.vTargetPos.x + 15.0f, ef_sort.vTargetPos.y + up_height, ef_sort.vTargetPos.z + 15.0f);
		m_Smoke->Render();

		m_Smoke->SetColor(1.0f, 1.0f, 1.0f, smoke_fade);
		m_Smoke->SetBillBoard(vViewVec, FALSE, 0.0f);
		m_Smoke->Scale(2.0f, 2.0f, 2.0f);
		m_Smoke->SetTranslationXYZ(ef_sort.vTargetPos.x - 15.0f, ef_sort.vTargetPos.y + up_height, ef_sort.vTargetPos.z - 15.0f);
		m_Smoke->Render();
	}

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState[2] );

	// RenderState 되돌리기
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, dwLighting );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ZWRITEENABLE, dwZWrite );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND, dwSrc );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND, dwDest );

	//GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ZWRITEENABLE, TRUE );
	//GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE );
}


void CFxBossRaidBulkan1_02::EarthCrackRender(EffectSort &ef_sort)
{
	LPDIRECTDRAWSURFACE7 lpSurface = NULL;
	D3DVECTOR	vDrawPos;
	vDrawPos = ef_sort.vTargetPos;

	// 글로벌 ShadowRect사용하여 지형에 그린다.
	if( g_pShadowRect )
	{
		// nate 2004 - 7 - image
		lpSurface = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_STONERANCE2 ] ) ;
		// nate 2004 - 7 글로벌 ShadowRect사용
		g_pShadowRect->SetTileCoord(vDrawPos, 120, FALSE);		// 부울값은 시간인가 아닌가
		g_pShadowRect->Render(lpSurface, FALSE);	// 부울값은 리얼그림자인가 아닌가?
	}
}
