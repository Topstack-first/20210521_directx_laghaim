#include "stdafx.h"

#define D3D_OVERLOADS
#include <d3d.h>
#include <assert.h>
#include "main.h"
#include "Frustum.h"
#include "Nk2DFrame.h"
#include "NkMob.h"
#include "Capsyong.h"
#include "IndexedTexture.h"
#include "D3DMath.h"
#include "DirtSnd.h"
#include "Land.h"
#include "ClrAlloc.h"
#include "ImageDefine.h"
#include "LoadEffectSound.h"

#include "EventLegKnockAppear.h"


extern IndexedTexture *g_pMobTexture;

extern float g_fDSDistance;



CEventLegKnockAppear::CEventLegKnockAppear()
	: m_PosX(0)
	, m_PosY(0)
	, m_PosZ(0)
	, m_dir(0)
	, m_fCamDist(0)
	, m_fCamEyeHeight(0)
	, m_fCamAtHeight(0)
{
	m_pCapSyong = NULL;
	m_pBiped = NULL;
	m_pFadeEffect = NULL;
	m_pNameOutput = NULL;
}

CEventLegKnockAppear::~CEventLegKnockAppear()
{
	DeleteRes(); // 혹시 모르니 한번더 확인.(UiMgr 쪽의 EndEventScene() 에서도 한번 호출함.)
}

void CEventLegKnockAppear::Init( D3DVECTOR vOldEye, D3DVECTOR vOldAt ) // 호출될때 마다 초기화해야하는 종류의 것들.
{
	m_vOldEye = vOldEye; // 이전의 카메라를 목원하기 위해 기억.
	m_vOldAt = vOldAt;

	m_vEye.x = m_PosX + ( cos(m_dir) * 55.0f ); // 서버가 말한 발생지점의 방향 반대로 일정량.
	m_vEye.y = m_PosY + 30.0f; // 지형 높이에서 일정량 위.
	m_vEye.z = m_PosZ + ( sin(m_dir) * 55.0f ); // 서버가 말한 발생지점의 방향 반대로 일정량.
	m_vAt.x = m_PosX; // 서버가 말한 발생지점의 방향 반대로 일정량.
	m_vAt.y = m_PosY + 30.0f; // 지형 높이에서 일정량 위.
	m_vAt.z = m_PosZ; // 서버가 말한 발생지점의 방향 반대로 일정량.

	pCMyApp->SetCameraInitPos( m_vEye, m_vAt ); // 직접 셋팅하기 귀찮아서 pCMyApp 껄 직접 고쳐버림. 씬이 끝나면 기억해둔 이전 카메라 좌표로 다시 바꿔줘야한다.
}

void CEventLegKnockAppear::LoadRes()
{

	m_pCapSyong = new CCapsyong(g_pMobTexture);
	if( VF_ERR_NONE > m_pCapSyong->Read( "./data/Npc/csy/legknock.CSY" ) )
	{
		SAFE_DELETE(m_pCapSyong);
		return;
	}

	m_pBiped = new CBiped;

	if( VF_ERR_NONE > m_pBiped->Read("./data/Npc/bpd/legknock_jump00.BPD") )
	{
		SAFE_DELETE(m_pBiped);
		return;
	}


	m_pFadeEffect = new CRectPlane;
	if( m_pFadeEffect )
		m_pFadeEffect->Create( 1024.0f, 1024.0f, FALSE ); // 화면을 다 덮을 수 있는 크기.

	m_pNameOutput = new CRectPlane;
	if( m_pNameOutput )
		m_pNameOutput->Create( 100.0f, 100.0f, FALSE );
}

void CEventLegKnockAppear::DeleteRes()
{
	SAFE_DELETE( m_pNameOutput );
	SAFE_DELETE( m_pFadeEffect );
	SAFE_DELETE( m_pCapSyong );
	SAFE_DELETE( m_pBiped );
}

BOOL CEventLegKnockAppear::Render( DWORD dwEventStartTime )
{
	static int PrevFrame = 0;

	D3DMATRIX matTrans, matRotate, matWorld;
	DWORD alphaTest, alphaBlend, srcBlendOld, destBlendOld, dwLighting, dwAmbient, dwFogEnable;

	DWORD dwNowTick = timeGetTime();
	DWORD dwElapsedTick = dwNowTick - dwEventStartTime; // 이벤트가 시작된후 진행된 틱.(전 프레임이 아님.)
	DWORD nCurFrame = (dwElapsedTick-1000) * 20 / 1000; // 1초를 20프레임으로 계산해서 현재 몇 프레임인지를 계산한다. (앞의 페이드 부분을 제외하고 계산)

	// 카메라 줌에 따라 시야각 등이 달라지기 때문에 프로젝션 매트릭스와 플러스텀도 초기화해준다.
	D3DMATRIX matProj;

	float Fov = 0.75f;
	float Aspect = 0.75f;
	float zNear = 0.1f;
	float zFar = 10000.0f;
	D3DUtil_SetProjectionMatrix( matProj, Fov, Aspect, zNear, zFar );
	GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_PROJECTION, &matProj);
	pCMyApp->GetFrustum()->InitFrustum( Fov, Aspect, zNear, 10000.0f );


	// 상태값 보존.
	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_FOGENABLE, &dwFogEnable );
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, &alphaBlend );
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_ALPHATESTENABLE, &alphaTest );
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_SRCBLEND, &srcBlendOld );
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_DESTBLEND, &destBlendOld );
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_LIGHTING, &dwLighting );
	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_AMBIENT, &dwAmbient);

	if( dwElapsedTick < 1000 ) // 페이드 인
	{
		D3DTLVERTEX TLVec[4];
		DWORD Alpha = (float)dwElapsedTick * 0.3f;

		if( Alpha > 255 )
			Alpha = 255;

		TLVec[0] = D3DTLVERTEX( D3DVECTOR(0,1024,0), 1, RGBA_MAKE(0,0,0,Alpha), 0, 0, 1.0f );
		TLVec[1] = D3DTLVERTEX( D3DVECTOR(0,0,0), 1, RGBA_MAKE(0,0,0,Alpha), 0, 0, 0 );
		TLVec[2] = D3DTLVERTEX( D3DVECTOR(1024,0,0), 1, RGBA_MAKE(0,0,0,Alpha), 0, 1.0f, 0 );
		TLVec[3] = D3DTLVERTEX( D3DVECTOR(1024,1024,0), 1, RGBA_MAKE(0,0,0,Alpha), 0, 1.0f, 1.0f );

		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_FOGENABLE, FALSE );
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE);
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, TRUE);
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCCOLOR);
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_SRCALPHA);

		GET_D3DDEVICE()->SetTexture(0, NULL );
		GET_D3DDEVICE()->DrawPrimitive(D3DPT_TRIANGLEFAN    , D3DFVF_TLVERTEX, TLVec, 4, 0);
	}
	else if( dwElapsedTick < 13000 ) // 레그넉 등장 부.
	{
		if( nCurFrame >= 220 )
			nCurFrame = 219; // 넘어가면 강제 보정.


		// 카메라 셋팅.
		float fDist = 55.0f; // 거리 조절용.
		float fEyeHeight = 30.0f; // 높이 조절용.
		float fAtHeight = 30.0f; // 높이 조절용.

		if( nCurFrame > 20 ) // 멀어지게.
		{
			int nTemp = nCurFrame-20;

			if( nTemp > 50 )
				nTemp = 50;

			fDist += (float)(nTemp)*0.5f;
		}

		if( nCurFrame > 110 && nCurFrame <= 120 ) // 카메라가 점프에 맞춰서올라가게.
		{
			int nTemp = nCurFrame-110;

			fDist += (float)(nTemp*10);
			fEyeHeight += (float)(nTemp*4);
			fAtHeight += (float)(nTemp*4);

			// 효과음 출력.
			if( PrevFrame <= 110 && nCurFrame > 110 )
			{
				if (g_pDSound)
				{
					// 때리는 넘만 정의되면 된다.
					float x, y, z;
					x = 0.0f; // 화면을 맞추므로 좌표는 원점으로..
					y = 0.0f;
					z = 0.0f;

					g_pDSound->Play(g_EffectSound[EFF_SND_STAR_FLARE], x, y, z);
				}
			}
		}
		else if( nCurFrame > 120 && nCurFrame <= 125 ) // 카메라가 착지에 맞춰서 내려오게.
		{
			int nTemp = 125- nCurFrame;

			fEyeHeight += (float)(nTemp*6);
			fAtHeight += (float)(nTemp*6);
		}
		else if( nCurFrame > 125 && nCurFrame <= 138 ) // 쿵하고 흔들리는 카메라 워크
		{
			int nTemp = (rand()%10)-5; // 랜덤하게..

			fEyeHeight += (float)nTemp; // 높이만 조절
			fAtHeight += (float)nTemp;
		}

		if( nCurFrame > 140 )
		{
			int nTemp = nCurFrame-140;

			if( nTemp > 25 )
				nTemp = 25;

			fDist -= (float)(nTemp)*0.5f;
			fEyeHeight -= (float)(nTemp)*1.5f; // Eye는 낮추고
			fAtHeight += (float)(nTemp)*0.8f; // At은 올린다.

			// 효과음 출력.
			if( PrevFrame <= 170 && nCurFrame > 170 )
			{
				if (g_pDSound)
				{
					// 때리는 넘만 정의되면 된다.
					float x, y, z;
					x = 0.0f; // 화면을 맞추므로 좌표는 원점으로..
					y = 0.0f;
					z = 0.0f;

					g_pDSound->Play(g_EffectSound[EFF_SND_FIRE_PILLAR], x, y, z);
				}
			}
		}

		// 카메라 셋팅.
		m_vEye.x = m_PosX + ( cos(m_dir) * fDist ); // 서버가 말한 발생지점의 방향 반대로 일정량.
		m_vEye.y = m_PosY + fEyeHeight; // 지형 높이에서 일정량 위.
		m_vEye.z = m_PosZ + ( sin(m_dir) * fDist ); // 서버가 말한 발생지점의 방향 반대로 일정량.
		m_vAt.x = m_PosX; // 서버가 말한 발생지점의 방향 반대로 일정량.
		m_vAt.y = m_PosY + fAtHeight; // 지형 높이에서 일정량 위.
		m_vAt.z = m_PosZ; // 서버가 말한 발생지점의 방향 반대로 일정량.

		pCMyApp->SetCameraInitPos( m_vEye, m_vAt ); // 직접 셋팅하기 귀찮아서 pCMyApp 껄 직접 고쳐버림. 씬이 끝나면 기억해둔 이전 카메라 좌표로 다시 바꿔줘야한다.


		if( m_pCapSyong && m_pBiped )
		{
			// 몬스터 위치 셋팅
			D3DUtil_SetTranslateMatrix( matTrans, m_PosX, m_PosY, m_PosZ);
			Nk3DUtil_SetRotateCCWYMatrix( matRotate, m_dir);
			D3DMath_MatrixMultiply( matWorld, matRotate, matTrans );
			GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_WORLD, &matWorld );

			m_pCapSyong->Animate( m_pBiped, nCurFrame );
			m_pCapSyong->Render( GET_D3DDEVICE() );
		}



		// 프레임에 맞춰서 돌 튀기는 이펙트 추가
		if( PrevFrame < 1 && nCurFrame >= 1 )
		{
			// 돌 튀기는 이펙트 추가.
			EffectSort ef_sort;
			ZeroMemory(&ef_sort, sizeof(EffectSort));

			ef_sort.nType = FX_STONERANCE;
			ef_sort.Index = 2;

			ef_sort.vPos = D3DVECTOR( m_PosX + (cos(m_dir-g_PI_DIV_2)*13.0f), m_PosY, m_PosZ + (sin(m_dir-g_PI_DIV_2)*13.0f) );
			pCMyApp->m_pFxSet->InsertFxEffect(ef_sort);

			if (g_pDSound)
			{
				// 때리는 넘만 정의되면 된다.
				float x, y, z;
				x = 0.0f; // 화면을 맞추므로 좌표는 원점으로..
				y = 0.0f;
				z = 0.0f;

				g_pDSound->Play(g_EffectSound[EFF_SND_MISSLEBOMB], x, y, z);
			}
		}
		if( PrevFrame < 28 && nCurFrame >= 28 )
		{
			// 돌 튀기는 이펙트 추가.
			EffectSort ef_sort;
			ZeroMemory(&ef_sort, sizeof(EffectSort));

			ef_sort.nType = FX_STONERANCE;
			ef_sort.Index = 2;

			ef_sort.vPos = D3DVECTOR( m_PosX, m_PosY, m_PosZ );
			pCMyApp->m_pFxSet->InsertFxEffect(ef_sort);

			if (g_pDSound)
			{
				// 때리는 넘만 정의되면 된다.
				float x, y, z;
				x = 0.0f; // 화면을 맞추므로 좌표는 원점으로..
				y = 0.0f;
				z = 0.0f;

				g_pDSound->Play(g_EffectSound[EFF_SND_MISSLEBOMB], x, y, z);
			}
		}
		if( PrevFrame < 64 && nCurFrame >= 64 )
		{
			// 돌 튀기는 이펙트 추가.
			EffectSort ef_sort;
			ZeroMemory(&ef_sort, sizeof(EffectSort));

			ef_sort.nType = FX_STONERANCE;
			ef_sort.Index = 2;

			ef_sort.vPos = D3DVECTOR( m_PosX + (cos(m_dir+g_PI_DIV_2)*13.0f), m_PosY, m_PosZ + (sin(m_dir+g_PI_DIV_2)*13.0f) );
			pCMyApp->m_pFxSet->InsertFxEffect(ef_sort);
			/*
						if (g_pDSound)
						{
							// 때리는 넘만 정의되면 된다.
							float x, y, z;
							x = 0.0f; // 화면을 맞추므로 좌표는 원점으로..
							y = 0.0f;
							z = 0.0f;

							g_pDSound->Play(g_EffectSound[EFF_SND_MISSLEBOMB], x, y, z);
						}
			*/
		}
		if( PrevFrame < 110 && nCurFrame >= 110 )
		{
			// 돌 튀기는 이펙트 추가.
			EffectSort ef_sort;
			ZeroMemory(&ef_sort, sizeof(EffectSort));

			ef_sort.nType = FX_STONERANCE;
			ef_sort.Index = 2;

			ef_sort.vPos = D3DVECTOR( m_PosX, m_PosY, m_PosZ );
			pCMyApp->m_pFxSet->InsertFxEffect(ef_sort);
			/*
						if (g_pDSound)
						{
							// 때리는 넘만 정의되면 된다.
							float x, y, z;
							x = 0.0f; // 화면을 맞추므로 좌표는 원점으로..
							y = 0.0f;
							z = 0.0f;

							g_pDSound->Play(g_EffectSound[EFF_SND_MISSLEBOMB], x, y, z);
						}
			*/
		}
	}

	if( dwElapsedTick > 8000 ) // 이름 출력
	{
		D3DTLVERTEX TLVec[4];
		DWORD Alpha = 255;

		float ScreenCenterX = (float)(g_pNk2DFrame->GetClientWidth() / 2);
		float PosY = 100.0f;
//		ScreenCenterX = 150;
		float rectwidth = 120.0f;
		float rectheight = 0.0f;

		rectheight = ( dwElapsedTick-8000 ) * 0.07f;

		if( rectheight > 100.0f )
			rectheight = 100.0f;

		TLVec[0] = D3DTLVERTEX( D3DVECTOR(ScreenCenterX-rectwidth,PosY+rectheight,0), 1, RGBA_MAKE(255,255,255,Alpha), 0, 0, 1.0f );
		TLVec[1] = D3DTLVERTEX( D3DVECTOR(ScreenCenterX-rectwidth,PosY-rectheight,0), 1, RGBA_MAKE(255,255,255,Alpha), 0, 0, 0 );
		TLVec[2] = D3DTLVERTEX( D3DVECTOR(ScreenCenterX+rectwidth,PosY-rectheight,0), 1, RGBA_MAKE(255,255,255,Alpha), 0, 1.0f, 0 );
		TLVec[3] = D3DTLVERTEX( D3DVECTOR(ScreenCenterX+rectwidth,PosY+rectheight,0), 1, RGBA_MAKE(255,255,255,Alpha), 0, 1.0f, 1.0f );

		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_FOGENABLE, FALSE );
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE);
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, TRUE);
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA);
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA );

		GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_LEGKNOCK_NAME ] ) );
		GET_D3DDEVICE()->DrawPrimitive(D3DPT_TRIANGLEFAN    , D3DFVF_TLVERTEX, TLVec, 4, 0);
	}

	if( dwElapsedTick > 12000 ) // 페이드 아웃 부분. (else if 가 아닌 이유는 모델이 출력되는 중에 페이드 아웃이 되야하기 때문.)
	{
		D3DTLVERTEX TLVec[4];
		DWORD Alpha = 255 - (float)(dwElapsedTick-12000) * 0.3f;

		if( Alpha > 255 )
			Alpha = 0;

		TLVec[0] = D3DTLVERTEX( D3DVECTOR(0,1024,0), 1, RGBA_MAKE(0,0,0,Alpha), 0, 0, 1.0f );
		TLVec[1] = D3DTLVERTEX( D3DVECTOR(0,0,0), 1, RGBA_MAKE(0,0,0,Alpha), 0, 0, 0 );
		TLVec[2] = D3DTLVERTEX( D3DVECTOR(1024,0,0), 1, RGBA_MAKE(0,0,0,Alpha), 0, 1.0f, 0 );
		TLVec[3] = D3DTLVERTEX( D3DVECTOR(1024,1024,0), 1, RGBA_MAKE(0,0,0,Alpha), 0, 1.0f, 1.0f );

		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_FOGENABLE, FALSE );
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE);
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, TRUE);
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCCOLOR);
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_SRCALPHA);

		GET_D3DDEVICE()->SetTexture(0, NULL );
		GET_D3DDEVICE()->DrawPrimitive(D3DPT_TRIANGLEFAN    , D3DFVF_TLVERTEX, TLVec, 4, 0);
	}

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_FOGENABLE, dwFogEnable );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ALPHATESTENABLE, alphaTest );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, alphaBlend );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND, srcBlendOld);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND, destBlendOld);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_AMBIENT, dwAmbient);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, dwLighting );


	PrevFrame = nCurFrame;

	if( dwElapsedTick >= 13000 ) // 일정초가 지낫으면 종료한다. // 앞에 페이드 1초, 뒤에 여운1초, 페이드 1초가 추가된 수치.
	{
		pCMyApp->SetCameraInitPos( m_vOldEye, m_vOldAt ); // 씬이 시작하기 전의 카메라 좌표로 돌려준다.
		pCMyApp->UpdateProjectMatrix(); // 프로젝션 매트릭스나 플러스텀 정보도 건드리기 때문에 이걸 꼭해줘야 돌아온다.

		return TRUE;
	}

	return FALSE; // 아직 끝나지 않았다.
}