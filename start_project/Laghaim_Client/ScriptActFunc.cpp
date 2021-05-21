#include "stdafx.h"

///튜토리얼 시 추가

#define D3D_OVERLOADS

#include <d3d.h>
#include <assert.h>
#include "main.h"
#include "headers.h"
#include "Frustum.h"
#include "Item.h"
#include "ItemRender.h"
#include "ItemProto.h"
#include "basicbutton.h"
#include "Nk2DFrame.h"
#include "NkMob.h"
#include "Capsyong.h"
#include "IndexedTexture.h"
#include "D3DMath.h"
#include "DirtSnd.h"
#include "Land.h"
#include "ClrAlloc.h"
#include "LoadEffectSound.h"
#include "uimgr.h"
#include "FxSet.h"
#include "NkCharacter.h"
#include "NKObjQuadNode.h"
#include "GUtil.h"
#include "IndexedTexture.h"
#include "ImageDefine.h"
#include "ControlInven.h"
#include "ControlScore.h"
#include "ControlShop.h"
#include "ControlBottom.h"
#include "RShiftMenu.h"
#include "ControlParty.h"
#include "ControlExchange.h"
#include "Tcpipcon.h"
#include "ShadowRect.h"

#include "ScriptActFunc.h"
#include "EventScriptActor.h"

#include "string_res.h"
#include "g_stringmanager.h"








// nate 2004 - 7 글로벌 ShadowRect사용

extern DWORD				g_dwClientCountry;
//extern void Convert640Pos( RECT *Rect ); // 640*480 해상도 기준의 좌표를 현재 해상도로 컨버팅시켜준다.

BOOL CActFuncManager::ReadyAction( int ActionNum )
{
	SAFE_DELETE( m_pFunc );

	switch( ActionNum )
	{
	case EVENT_ACTION_TUTORIAL_START: // 튜토리얼 시작부.

		m_pFunc = new CTutorialStartAct();

		break;

	case EVENT_ACTION_TUTORIAL_MOVE: // 튜토리얼 이동 체험.

		m_pFunc = new CTutorialMoveAct();

		break;

	case EVENT_ACTION_TUTORIAL_CAM: // 튜토리얼 시점회전 체험.

		m_pFunc = new CTutorialCamAct();

		break;

	case EVENT_ACTION_TUTORIAL_HUNT: // 튜토리얼 사냥 체험.

		m_pFunc = new CTutorialHuntAct();

		break;

	case EVENT_ACTION_TUTORIAL_POINTUP: // 튜토리얼 포인트 업 체험.

		m_pFunc = new CTutorialPointAct();

		break;

	case EVENT_ACTION_TUTORIAL_POSIONBUY: // 튜토리얼 물약사기 체험.

		m_pFunc = new CTutorialPosionBuyAct();

		break;

	case EVENT_ACTION_TUTORIAL_POSIONUSE: // 튜토리얼 물약사용 체험.

		m_pFunc = new CTutorialPosionUseAct();

		break;

	case EVENT_ACTION_TUTORIAL_EQUIPBUY: // 튜토리얼 장비구입 체험.

		m_pFunc = new CTutorialEquipBuyAct();

		break;

	case EVENT_ACTION_TUTORIAL_EQUIPUSE: // 튜토리얼 장비착용 체험.

		m_pFunc = new CTutorialEquipUseAct();

		break;

	case EVENT_ACTION_TUTORIAL_TOPMENU: // 튜토리얼 기본 메뉴 체험.

		m_pFunc = new CTutorialTopMenuAct();

		break;

	case EVENT_ACTION_TUTORIAL_PARTY: // 튜토리얼 파티 체험.

		m_pFunc = new CTutorialPartyAct();

		break;

	case EVENT_ACTION_TUTORIAL_EXCHANGE: // 튜토리얼 교환 체험.

		m_pFunc = new CTutorialExchangeAct();

		break;

	case EVENT_ACTION_TUTORIAL_PEACEMODE: // 튜토리얼 전투모드/편화모드 체험.

		m_pFunc = new CTutorialPeaceModeAct();

		break;

	case EVENT_ACTION_TUTORIAL_END: // 튜토리얼 종료.

		m_pFunc = new CTutorialEndAct();

		break;

	}

	if( !m_pFunc )
		return FALSE;

	return TRUE;
}

void CActFuncManager::ReleaseAction( int ActionNum )
{
// 해제시 뭔가 별도로 할게 잇을때 쓰면 된다. (이 녀석들 소멸자로 제대로 안들어가고있다 소멸자에서 할거 여기서해주자)
	switch( ActionNum )
	{
	case EVENT_ACTION_TUTORIAL_POINTUP: // 튜토리얼 포인트 찍기
	case EVENT_ACTION_TUTORIAL_POSIONUSE: // 튜토리얼 포션 사용
	case EVENT_ACTION_TUTORIAL_EQUIPUSE: // 튜토리얼 장비 착용
	case EVENT_ACTION_TUTORIAL_TOPMENU: // 튜토리얼 메뉴바 체험.
	case EVENT_ACTION_TUTORIAL_EXCHANGE: // 튜토리얼 교환 체험.
		g_pNk2DFrame->CloseAllWindow(); // 포인트 창을 닫아준다.
		break;
	}

	// 튜토리얼이라면 항상 액션을 넘어갈때 교관의 대사나 Info를 지워준다.
	if( ActionNum >= EVENT_ACTION_TUTORIAL_START && ActionNum <= EVENT_ACTION_TUTORIAL_END ) // 튜토리얼이라면..
	{
		pCMyApp->StopHero(); // 한번 멈춰주는게 좋다. 도중 클릭이 적용되는 경우가 있기때문에...

		CNkMob *pMob = NULL;
		LHMobMap_it prMob;
		for (prMob = pCMyApp->m_pUIMgr->m_mob_map.begin(); prMob != pCMyApp->m_pUIMgr->m_mob_map.end(); prMob++)
		{
			pMob = (*prMob).second;
			if( pMob->m_nMobNum == 57 ) // 경비 소장
			{
				pMob->m_bShowMobChatPopup = FALSE; // 말풍선 꺼버린다.
				pMob->m_LastChatTime = timeGetTime() - 8000;

				break;
			}
		}

		g_pNk2DFrame->SetInfoBox( "" ); // 이렇게 하면 출력되던 공지가 없어지도록 SetInfoBox 함수를 수정하자.
	}

	SAFE_DELETE( m_pFunc );
}



CTutorialStartAct::CTutorialStartAct()
{
	int i = 0;

//	pCMyApp->m_pUIMgr->EraseAllList();

	// 주인공 위치 초기화.
	/*  // 주인공 위치는 서버에서 준다.
		float fx = 2228.81f;
		float fy = 133.89f;
		float fz = 10412.27f;
		float dir = 0.0f;

		float	fHeight = pCMyApp->GetLandPtr()->GetHFHeight( fx, fz );
		fy = CNKObjQuadNode::GetHeightOnObject( fz, fx, fy );
		if( fy < fHeight )
			fy = fHeight;

		pCMyApp->SetHeroInitPos( fx, fy, fz, 0.0f );	// 카메라 설정도 필요함으로.
		g_pRoh->SetCombat( FALSE );
		g_pRoh->SetMotionState(CHA_STAND);
	*/
	// 주인공 방향 가끔이상해지는 버그때문에 강제 땜빵(서버문제 아님!)
	g_pRoh->m_dir = 0.0f;
	g_pRoh->m_toDir = 0.0f;

	float fx = g_pRoh->m_wx;
	float fy = g_pRoh->m_wy;
	float fz = g_pRoh->m_wz;
	float dir = 0.0f;

	// 카메라 초기화.
	float CamDist = 100.0f;
	float CamEyeHeight = 90.0f;
	float CamAtHeight = 20.0f;

	D3DVECTOR vEye;
	D3DVECTOR vAt;
	vEye.x = fx + ( cos(dir) * CamDist ); // 서버가 말한 발생지점의 방향 반대로 일정량.
	vEye.y = fy + CamEyeHeight; // 지형 높이에서 일정량 위.
	vEye.z = fz + ( sin(dir) * CamDist ); // 서버가 말한 발생지점의 방향 반대로 일정량.
	vAt.x = fx; // 서버가 말한 발생지점의 방향 반대로 일정량.
	vAt.y = fy + CamAtHeight; // 지형 높이에서 일정량 위.
	vAt.z = fz; // 서버가 말한 발생지점의 방향 반대로 일정량.

	pCMyApp->SetCameraInitPos( vEye, vAt ); // 직접 셋팅하기 귀찮아서 pCMyApp 껄 직접 고쳐버림. 씬이 끝나면 기억해둔 이전 카메라 좌표로 다시 바꿔줘야한다.
	D3DMATRIX matProj;

	float Fov = 0.75f;
	float Aspect = 0.75f;
	float zNear = 0.1f;
	float zFar = 10000.0f;
	D3DUtil_SetProjectionMatrix( matProj, Fov, Aspect, zNear, zFar );
	pCMyApp->GetDevice()->SetTransform( D3DTRANSFORMSTATE_PROJECTION, &matProj);
	pCMyApp->GetFrustum()->InitFrustum( Fov, Aspect, zNear, 10000.0f );


	m_dwOldElapsed = 0;
	m_dwStartTick = timeGetTime(); // 생성시 시각을 기억.
}

CTutorialStartAct::~CTutorialStartAct()
{
}


#define TUTORIAL_START_WOLF_NUM 30 // 나오는 늑대 갯수
#define TUTORIAL_START_TIGER_NUM 20 // 나오는 백호 갯수

#define TUTORIAL_START_TIMEFLAG_FADEEND 2000 // 페이드 인 끝나는 타임.
#define TUTORIAL_START_TIMEFLAG_MOTION1 2500 // 두리번 거리기 시작하는 타임. 일단 보류
#define TUTORIAL_START_TIMEFLAG_MOBAPPEAR 4000 // 몹들 등장
#define TUTORIAL_START_TIMEFLAG_MOBRUN    4500 // 몹들 달려들기 시작함.
#define TUTORIAL_START_TIMEFLAG_MOTION2 7000 // 둘러쌓인뒤 두리번 시작하는 타임. 
#define TUTORIAL_START_TIMEFLAG_MOBDESTROY    10000 // 몹들 공격당함.
#define TUTORIAL_START_TIMEFLAG_MOBDESTROY2   11000 // 몹들 공격당함.
//#define TUTORIAL_START_TIMEFLAG_TEACHERAPPEAR 12000 // 교관 등장
//#define TUTORIAL_START_TIMEFLAG_ACTIONEND    17000 // 액션 종료
#define TUTORIAL_START_TIMEFLAG_TEACHERAPPEAR 10500 // 교관 등장
#define TUTORIAL_START_TIMEFLAG_ACTIONEND    15500 // 액션 종료


bool CTutorialStartAct::Render()
{
	int i = 0;
	DWORD Elapsed = timeGetTime() - m_dwStartTick;

	// 임시용 변수들.
	CNkMob* pMob;
	float fx = 0.0f;
	float fz = 0.0f;
	float dir = 0.0f;
	float dist = 0.0f;
	float fTemp = 0.0f;


	// 특정 타이밍의 1회성 발생.
	if( Elapsed < TUTORIAL_START_TIMEFLAG_FADEEND ) // 시작 시 페이드 인.
	{
		int Alpha = Elapsed * 255 / TUTORIAL_START_TIMEFLAG_FADEEND;
		DWORD color = RGBA_MAKE( 0, 0, 0, Alpha );
		pCMyApp->m_pFxSet->RenderFadeScreen( color );
	}
	if( m_dwOldElapsed < TUTORIAL_START_TIMEFLAG_FADEEND && Elapsed >= TUTORIAL_START_TIMEFLAG_FADEEND )
	{
		/*      // 서버에서 주기로 했다.
				EffectSort ef_sort;
				ZeroMemory(&ef_sort, sizeof(EffectSort));
				ef_sort.nType = FX_TELEPORT;
				ef_sort.vPos.x = g_pRoh->m_wx;
				ef_sort.vPos.z = g_pRoh->m_wz;
				ef_sort.vPos.y = g_pRoh->m_wy;

				if( pCMyApp->m_pFxSet )
					pCMyApp->m_pFxSet->InsertFxEffect(ef_sort);
		*/
	}

	/////////////////////////////////////////////////////////////////////////////
	// 처음 떨어진후 어리벙 두리번
	if( m_dwOldElapsed < TUTORIAL_START_TIMEFLAG_MOTION1 && Elapsed >= TUTORIAL_START_TIMEFLAG_MOTION1 )
	{
		g_pRoh->m_toDir -= 1.57f;
	}
	if( m_dwOldElapsed < TUTORIAL_START_TIMEFLAG_MOTION1+1000 && Elapsed >= TUTORIAL_START_TIMEFLAG_MOTION1+1000 )
	{
		g_pRoh->m_toDir += 3.14f;
	}
	if( m_dwOldElapsed < TUTORIAL_START_TIMEFLAG_MOTION1+1500 && Elapsed >= TUTORIAL_START_TIMEFLAG_MOTION1+1500 )
	{
		g_pRoh->m_toDir -= 1.57f;
	}
	/////////////////////////////////////////////////////////////////////////////



	/////////////////////////////////////////////////////////////////////////////
	// 몹 등장 및 달려듬.
	if( m_dwOldElapsed < TUTORIAL_START_TIMEFLAG_MOBAPPEAR && Elapsed >= TUTORIAL_START_TIMEFLAG_MOBAPPEAR )
	{

		for( i = 0 ; i < TUTORIAL_START_WOLF_NUM ; ++i )
		{
			fTemp = 6.28f / (float)TUTORIAL_START_WOLF_NUM; //간격.

			dir = i * fTemp + RandomNum(-0.5f,0.5f);
			dist = 230.0f + RandomNum(-20.0f,30.0f);


			fx = g_pRoh->m_wx + cos(dir) * dist; // 주인공으로부터 dir 방향으로 dist 만큼 떨어진 거리.
			fz = g_pRoh->m_wz + sin(dir) * dist;

			pCMyApp->m_pUIMgr->AddMob( 23, i, fx, fz, dir-3.14f, 100 );
		}

		for( i = 0 ; i < TUTORIAL_START_TIGER_NUM ; ++i )
		{
			fTemp = 6.28f / (float)TUTORIAL_START_TIGER_NUM; //간격.

			dir = i * fTemp + RandomNum(-0.7f,0.7f);
			dist = 230.0f + RandomNum(-20.0f,30.0f);


			fx = g_pRoh->m_wx + cos(dir) * dist; // 주인공으로부터 dir 방향으로 dist 만큼 떨어진 거리.
			fz = g_pRoh->m_wz + sin(dir) * dist;

			pCMyApp->m_pUIMgr->AddMob( 24, i+TUTORIAL_START_WOLF_NUM, fx, fz, dir-3.14f, 100 );
		}

	}
	if( m_dwOldElapsed < TUTORIAL_START_TIMEFLAG_MOBRUN && Elapsed >= TUTORIAL_START_TIMEFLAG_MOBRUN ) // 달려오게...
	{
		for( i = 0 ; i < TUTORIAL_START_WOLF_NUM ; ++i )
		{
			if( (pMob = pCMyApp->m_pUIMgr->FindMobById(i)) && pMob->m_nMobNum == 23 )
			{
				dir = pMob->m_dir+3.14f;
				dist = 50.0f + RandomNum(-10.0f,10.0f);

				fx = g_pRoh->m_wx + cos(dir) * dist; // 주인공으로부터 dir 방향으로 dist 만큼 떨어진 거리.
				fz = g_pRoh->m_wz + sin(dir) * dist;

				dist = DistPlToPlAbs( fx, fz, pMob->m_wx, pMob->m_wz);

				pMob->Move( fx, fz, (int)(dist/3.5f) );
			}
		}

		for( i = 0 ; i < TUTORIAL_START_TIGER_NUM ; ++i )
		{
			if( (pMob = pCMyApp->m_pUIMgr->FindMobById(i+TUTORIAL_START_WOLF_NUM)) && pMob->m_nMobNum == 24 )
			{
				dir = pMob->m_dir+3.14f;
				dist = 50.0f + RandomNum(-10.0f,10.0f);

				fx = g_pRoh->m_wx + cos(dir) * dist; // 주인공으로부터 dir 방향으로 dist 만큼 떨어진 거리.
				fz = g_pRoh->m_wz + sin(dir) * dist;

				dist = DistPlToPlAbs( fx, fz, pMob->m_wx, pMob->m_wz);

				pMob->Move( fx, fz, (int)(dist/3.0f) );
			}
		}
	}
	/////////////////////////////////////////////////////////////////////////////


	/////////////////////////////////////////////////////////////////////////////
	// 몹에게 둘러쌓여 두리번.
	if( m_dwOldElapsed < TUTORIAL_START_TIMEFLAG_MOTION2 && Elapsed >= TUTORIAL_START_TIMEFLAG_MOTION2 )
	{
		g_pRoh->m_toDir -= 1.0f;
	}
	if( m_dwOldElapsed < TUTORIAL_START_TIMEFLAG_MOTION2+500 && Elapsed >= TUTORIAL_START_TIMEFLAG_MOTION2+500 )
	{
		g_pRoh->m_toDir += 2.0f;
	}
	if( m_dwOldElapsed < TUTORIAL_START_TIMEFLAG_MOTION2+1000 && Elapsed >= TUTORIAL_START_TIMEFLAG_MOTION2+1000 )
	{
		g_pRoh->m_toDir -= 1.0f;

		g_pRoh->SetCombat( TRUE );
		g_pRoh->SetMotionState(CHA_STAND);
	}
	/////////////////////////////////////////////////////////////////////////////


	/////////////////////////////////////////////////////////////////////////////
	// 몹 섬멸 TUTORIAL_START_TIMEFLAG_MOBDESTROY
	if( m_dwOldElapsed < TUTORIAL_START_TIMEFLAG_MOBDESTROY && Elapsed >= TUTORIAL_START_TIMEFLAG_MOBDESTROY )
	{
		// 플레임 스피어 폭발효과를 넣는다.
		for( i = 0 ; i < 20 ; ++i )
		{
			dir = i * 0.628f + RandomNum(-0.5f,0.5f);
			dist = 50.0f + RandomNum(-10.0f,10.0f);

			fx = g_pRoh->m_wx + cos(dir) * dist; // 주인공으로부터 dir 방향으로 dist 만큼 떨어진 거리.
			fz = g_pRoh->m_wz + sin(dir) * dist;

			EffectSort ef_sort;
			ef_sort.nType = FX_METEORITEBOOM;

			ef_sort.vPos.x = fx;
			ef_sort.vPos.y = pCMyApp->GetLandPtr()->GetHFHeight( fx, fz ) + RandomNum(-10.0f,10.0f);
			ef_sort.vPos.z = fz;

			pCMyApp->m_pFxSet->InsertFxEffect(ef_sort);
		}
	}


	if( Elapsed >= TUTORIAL_START_TIMEFLAG_MOBDESTROY && Elapsed <= TUTORIAL_START_TIMEFLAG_MOBDESTROY+500 ) // 카메라 지진 효과.
	{
		D3DVECTOR vEye;
		D3DVECTOR vAt;
		pCMyApp->GetEyePos( vEye );
		pCMyApp->GetEyeAtPos( vAt );
		int nTemp = (rand()%10)-5; // 랜덤하게..
		vEye.y += (float)nTemp; // 지형 높이에서 일정량 위.
		vAt.y += (float)nTemp; // 지형 높이에서 일정량 위.

		pCMyApp->SetCameraInitPos( vEye, vAt ); // 직접 셋팅하기 귀찮아서 pCMyApp 껄 직접 고쳐버림. 씬이 끝나면 기억해둔 이전 카메라 좌표로 다시 바꿔줘야한다.
	}
	if( m_dwOldElapsed < TUTORIAL_START_TIMEFLAG_MOBDESTROY+500 && Elapsed >= TUTORIAL_START_TIMEFLAG_MOBDESTROY+500 ) // 카메라 높이 되돌려주기.
	{
		D3DVECTOR vEye;
		D3DVECTOR vAt;
		pCMyApp->GetEyePos( vEye );
		pCMyApp->GetEyeAtPos( vAt );
		vEye.y = (float)g_pRoh->m_wy + 100.0f; // 지형 높이에서 일정량 위.
		vAt.y = (float)g_pRoh->m_wy + 20.0f; // 지형 높이에서 일정량 위.

		pCMyApp->SetCameraInitPos( vEye, vAt ); // 직접 셋팅하기 귀찮아서 pCMyApp 껄 직접 고쳐버림. 씬이 끝나면 기억해둔 이전 카메라 좌표로 다시 바꿔줘야한다.
	}


	if( m_dwOldElapsed < TUTORIAL_START_TIMEFLAG_MOBDESTROY+500 && Elapsed >= TUTORIAL_START_TIMEFLAG_MOBDESTROY+500 ) // 몹 죽이ㄱ.
	{

		for( i = 0 ; i < TUTORIAL_START_WOLF_NUM ; ++i )
		{
			if( (pMob = pCMyApp->m_pUIMgr->FindMobById(i)) && pMob->m_nMobNum == 23 )
			{
				pMob->Kill();
			}
		}
		for( i = 0 ; i < TUTORIAL_START_TIGER_NUM ; ++i )
		{
			if( (pMob = pCMyApp->m_pUIMgr->FindMobById(i+TUTORIAL_START_WOLF_NUM)) && pMob->m_nMobNum == 24 )
			{
				pMob->Kill();
//				pMob->SetAttackVital( 50 ); // 백호들은 피 반으로.
			}
		}

	}
	/*
		// 한방더
		if( m_dwOldElapsed < TUTORIAL_START_TIMEFLAG_MOBDESTROY2 && Elapsed >= TUTORIAL_START_TIMEFLAG_MOBDESTROY2 )
		{
			// 플레임 스피어 폭발효과를 넣는다.
			for( i = 0 ; i < 20 ; ++i )
			{
				dir = i * 0.628f + RandomNum(-0.5f,0.5f);
				dist = 50.0f + RandomNum(-10.0f,10.0f);

				fx = g_pRoh->m_wx + cos(dir) * dist; // 주인공으로부터 dir 방향으로 dist 만큼 떨어진 거리.
				fz = g_pRoh->m_wz + sin(dir) * dist;

				EffectSort ef_sort;
				ef_sort.nType = FX_METEORITEBOOM;
	//			ef_sort.Index = 1; // 바닥 갈라지는거 안표시하도록...

				ef_sort.vPos.x = fx;
				ef_sort.vPos.y = pCMyApp->GetLandPtr()->GetHFHeight( fx, fz ) + RandomNum(-10.0f,10.0f);
				ef_sort.vPos.z = fz;

				pCMyApp->m_pFxSet->InsertFxEffect(ef_sort);
			}
		}


		if( Elapsed >= TUTORIAL_START_TIMEFLAG_MOBDESTROY2 && Elapsed <= TUTORIAL_START_TIMEFLAG_MOBDESTROY2+500 ) // 카메라 지진 효과.
		{
			D3DVECTOR vEye;
			D3DVECTOR vAt;
			pCMyApp->GetEyePos( vEye );
			pCMyApp->GetEyeAtPos( vAt );
			int nTemp = (rand()%10)-5; // 랜덤하게..
			vEye.y += (float)nTemp; // 지형 높이에서 일정량 위.
			vAt.y += (float)nTemp; // 지형 높이에서 일정량 위.

			pCMyApp->SetCameraInitPos( vEye, vAt ); // 직접 셋팅하기 귀찮아서 pCMyApp 껄 직접 고쳐버림. 씬이 끝나면 기억해둔 이전 카메라 좌표로 다시 바꿔줘야한다.
		}
		if( m_dwOldElapsed < TUTORIAL_START_TIMEFLAG_MOBDESTROY2+500 && Elapsed >= TUTORIAL_START_TIMEFLAG_MOBDESTROY2+500 ) // 카메라 높이 되돌려주기.
		{
			D3DVECTOR vEye;
			D3DVECTOR vAt;
			pCMyApp->GetEyePos( vEye );
			pCMyApp->GetEyeAtPos( vAt );
			vEye.y = (float)g_pRoh->m_wy + 100.0f; // 지형 높이에서 일정량 위.
			vAt.y = (float)g_pRoh->m_wy + 20.0f; // 지형 높이에서 일정량 위.

			pCMyApp->SetCameraInitPos( vEye, vAt ); // 직접 셋팅하기 귀찮아서 pCMyApp 껄 직접 고쳐버림. 씬이 끝나면 기억해둔 이전 카메라 좌표로 다시 바꿔줘야한다.
		}


		if( m_dwOldElapsed < TUTORIAL_START_TIMEFLAG_MOBDESTROY2+500 && Elapsed >= TUTORIAL_START_TIMEFLAG_MOBDESTROY2+500 ) // 몹 죽이ㄱ.
		{
			for( i = 0 ; i < TUTORIAL_START_TIGER_NUM ; ++i )
			{
				if( pMob = pCMyApp->m_pUIMgr->FindMobById(i+TUTORIAL_START_WOLF_NUM) && pMob->m_nMobNum == 24 )
				{
					pMob->Kill();
				}
			}
		}
	*/
	/////////////////////////////////////////////////////////////////////////////


	/////////////////////////////////////////////////////////////////////////////
	// 교관 등장
	if( m_dwOldElapsed < TUTORIAL_START_TIMEFLAG_TEACHERAPPEAR && Elapsed >= TUTORIAL_START_TIMEFLAG_TEACHERAPPEAR )
	{
		/*
				dir = -1.57f;
				dist = 130.0f;
				fx = g_pRoh->m_wx + cos(dir) * dist; // 주인공으로부터 dir 방향으로 dist 만큼 떨어진 거리.
				fz = g_pRoh->m_wz + sin(dir) * dist;

				pCMyApp->m_pUIMgr->AddMob( 57, 1000, fx, fz, dir-3.14f, 100 ); // 교관은 인덱스를 임의로 크게.

				if( pMob = pCMyApp->m_pUIMgr->FindMobById(1000) )
				{
					dist = 40.0f;
					fx = g_pRoh->m_wx + cos(dir) * dist; // 주인공으로부터 dir 방향으로 dist 만큼 떨어진 거리.
					fz = g_pRoh->m_wz + sin(dir) * dist;

					dist = DistPlToPlAbs( fx, fz, pMob->m_wx, pMob->m_wz);

					pMob->Move( fx, fz, (int)(dist/2.0f) );
				}
		*/
		if (g_pTcpIp)
			g_pTcpIp->SendNetMessage("tutorial 1\n"); // 교관 등장 요청.

	}

	if( m_dwOldElapsed < TUTORIAL_START_TIMEFLAG_TEACHERAPPEAR+3000 && Elapsed >= TUTORIAL_START_TIMEFLAG_TEACHERAPPEAR+3000 )
	{
		// 주인공이 교관을 바라보게.
		g_pRoh->m_toDir -= 1.57f;

		g_pRoh->SetCombat( FALSE );
		g_pRoh->SetMotionState(CHA_STAND);
	}

	/////////////////////////////////////////////////////////////////////////////


	/*
		// 특정 타이밍 이후 계속 출력되야하는 녀석들.
		if( Elapsed >= TUTORIAL_START_TIMEFLAG_FADEEND )  // 이 시간 이후론 주인공은 계속 출력.
		{
			pCMyApp->DrawPlayer();
		}
	//	if( Elapsed >= TUTORIAL_START_TIMEFLAG_MONAPPEAR )  // 이 시간 이후론 주인공은 계속 출력.
		{
			pCMyApp->m_pUIMgr->RenderMobs();
		}
	*/
	m_dwOldElapsed = Elapsed;

	return TRUE;
}

int  CTutorialStartAct::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	return 1; // 이 연출 중엔 다른 움직임 안되도록.
}

bool CTutorialStartAct::EndCheck()
{
	DWORD Elapsed = timeGetTime() - m_dwStartTick;

	if( Elapsed >= TUTORIAL_START_TIMEFLAG_ACTIONEND )
		return TRUE;

	return FALSE;
}




CTutorialMoveAct::CTutorialMoveAct()
{
	m_nPointPhase = 0;
	m_Pointing1.Create( 25.0f, 25.0f, TRUE );
	m_Pointing2.Create( 8.0f, 10.0f, TRUE );

	m_dwOldElapsed = 0;
	m_dwStartTick = timeGetTime();

	m_bEndFlag = FALSE;
}

CTutorialMoveAct::~CTutorialMoveAct()
{

}

#define TUTORIAL_MOVE_POINT_NUM 4
POINT MovePoint[TUTORIAL_MOVE_POINT_NUM] =
{
	{ 2196, 10407 }
	, { 2232, 10471 }
	, { 2298, 10419 }
	, { 2237, 10403 }
}; // 이동할 위치 목록

bool CTutorialMoveAct::Render()
{


	if( !GET_D3DDEVICE() )
		return FALSE;

	char strTemp[1024];


	DWORD Elapsed = timeGetTime() - m_dwStartTick;
	DWORD BeforeElapsed = Elapsed - m_dwOldElapsed;

	if( m_nPointPhase < TUTORIAL_MOVE_POINT_NUM ) // 포인트 이동 중.
	{
		DWORD alphaTest, alphaBlend, srcBlendOld, destBlendOld, dwLighting, dwAmbient, dwFogEnable;
		DWORD  dwTextureState[3];
		// 상태값 보존.
		GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_FOGENABLE, &dwFogEnable );
		GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, &alphaBlend );
		GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_ALPHATESTENABLE, &alphaTest );
		GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_SRCBLEND, &srcBlendOld );
		GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_DESTBLEND, &destBlendOld );
		GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_LIGHTING, &dwLighting );
		GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_AMBIENT, &dwAmbient);
		GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG1, &dwTextureState[0] );
		GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG2, &dwTextureState[1] );
		GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAOP,   &dwTextureState[2] );

		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_FOGENABLE, FALSE );
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE);
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, TRUE);
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA);
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA);
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHATESTENABLE   , FALSE );

		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE2X );


		float	fHeight = pCMyApp->GetLandPtr()->GetHFHeight( (float)MovePoint[m_nPointPhase].x, (float)MovePoint[m_nPointPhase].y );
		fHeight += 1.0f;


		m_Pointing1.SetRotationY( g_PI * 0.5f );
		float fScale = 0.8f;
		fScale += sin( (float)Elapsed*0.005f ) * 0.2f;
		m_Pointing1.SetColor( 255.0f, 255.0f, 255.0f, 255.0f );
		m_Pointing1.Scale( fScale, fScale, fScale );
		m_Pointing1.SetTranslationXYZ( (float)MovePoint[m_nPointPhase].x, fHeight, (float)MovePoint[m_nPointPhase].y );
		GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_TELEPORT_SHIELD ] ) );
		m_Pointing1.Render();

		fHeight += 8.0f;
		fHeight += sin( (float)Elapsed*0.01f ) * 1.0f;

		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCCOLOR);
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_ONE);
		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );

		D3DVECTOR	vViewVec;
		vViewVec = pCMyApp->GetEyeDirection();
		m_Pointing2.SetColor( 255.0f, 255.0f, 255.0f, 0.0f );
		m_Pointing2.SetBillBoard(vViewVec, TRUE, g_PI * 0.5f);
		m_Pointing2.SetTranslationXYZ( (float)MovePoint[m_nPointPhase].x, fHeight, (float)MovePoint[m_nPointPhase].y );
		GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_TUTORIAL_ARROW ] ) );
		m_Pointing2.Render();


		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState[2] );

		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_FOGENABLE, dwFogEnable );
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ALPHATESTENABLE, alphaTest );
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, alphaBlend );
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND, srcBlendOld);
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND, destBlendOld);
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_AMBIENT, dwAmbient);
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, dwLighting );


		// 페이즈 증가 체크.
		float dist = DistPlToPlAbs( (float)MovePoint[m_nPointPhase].x, (float)MovePoint[m_nPointPhase].y
									, g_pRoh->m_wx, g_pRoh->m_wz);

		if( dist < 10.0f ) // 일정거리 안으로 들어왔으면...
		{
			++m_nPointPhase;

			if( m_nPointPhase >= TUTORIAL_MOVE_POINT_NUM )
			{
				// IDS_TUTORIAL_TALK1 : 테스트를 통과하셨습니다. 교관에게 다시 말을 걸어주세요.
				g_pNk2DFrame->SetInfoBox( (char*)G_STRING(IDS_TUTORIAL_TALK1) );
			}
			else
			{
				// IDS_TUTORIAL_TALK2 : 다음 포인트로 이동하세요.
				g_pNk2DFrame->SetInfoBox( (char*)G_STRING(IDS_TUTORIAL_TALK2) );
			}

		}

		// 주기적으로 재촉하자.
		if( timeGetTime()%10000 < 50 )
		{
			CNkMob *pMob = NULL;
			LHMobMap_it prMob;
			for (prMob = pCMyApp->m_pUIMgr->m_mob_map.begin(); prMob != pCMyApp->m_pUIMgr->m_mob_map.end(); prMob++)
			{
				pMob = (*prMob).second;
				if( pMob->m_nMobNum == 57 ) // 경비 소장
				{
					// IDS_TUTORIAL_TALK3 : 화살표가 움직이는 지점으로 이동해야 한다.
					sprintf( strTemp, (char*)G_STRING(IDS_TUTORIAL_TALK3) );

					pMob->SetMobChatMsg( strTemp );
					break;
				}
			}
		}
	}

	m_dwOldElapsed = Elapsed;

	return TRUE;
}

int  CTutorialMoveAct::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	switch( msg )
	{
	case WM_LBUTTONDOWN:

		if( m_nPointPhase >= TUTORIAL_MOVE_POINT_NUM ) // 전부 이동 후
		{
			//------------------------------------------------------------------------
			// 전체 몹들 중에서 클릭된 몹을 찾아서 처리.
			//------------------------------------------------------------------------
			CNkMob *pMob = NULL;
			LHMobMap_it prMob;

			for (prMob = pCMyApp->m_pUIMgr->m_mob_map.begin(); prMob != pCMyApp->m_pUIMgr->m_mob_map.end(); prMob++)
			{
				pMob = (*prMob).second;
				// Color Pickng Buffer 와 몹의 Color 가 동일하다면 ( 선택되었다면 )
				if (pMob->m_wColorIdx == pCMyApp->m_pUIMgr->GetPickColor() )
				{
					// NPC 가 클릭 되었다면 return 1;
					if( pMob->m_nMobNum == 57 ) // 경비 소장 클릭시.
						m_bEndFlag = TRUE;

					return 1; // 클릭후 동작안하도록.
				}
			}
		}

		break;
	}

	return 0; // 이 연출 중엔 다른 움직임 되도록.
}

bool CTutorialMoveAct::EndCheck()
{
	if( m_bEndFlag )
	{
		return TRUE;
	}

	return FALSE;
}


// 튜토리얼 시점 변경 체험.
CTutorialCamAct::CTutorialCamAct()
{
	m_nPointPhase = 0;

	m_dwOldElapsed = 0;
	m_dwStartTick = timeGetTime();

	m_Data1 = 0;

	m_bEndFlag = FALSE;
	/*
		m_pTeacher = NULL; // 교관 포인터를 가지고 있는다.
		CNkMob *pMob = NULL;
		LHMobMap_it prMob;
		for (prMob = pCMyApp->m_pUIMgr->m_mob_map.begin(); prMob != pCMyApp->m_pUIMgr->m_mob_map.end(); prMob++)
		{
			pMob = (*prMob).second;
			if( pMob->m_nMobNum == 57 ) // 경비 소장
			{
				m_pTeacher = pMob;
				break;
			}
		}
	*/
}

CTutorialCamAct::~CTutorialCamAct()
{

}

#define TUTORIAL_CAM_PHASE_NUM 4 // 상 하 좌 우 줌인 줌아웃

bool CTutorialCamAct::Render()
{


	if( !GET_D3DDEVICE() )
		return FALSE;

//	CNkMob* pMob;
	char strTemp[1024];


	DWORD Elapsed = timeGetTime() - m_dwStartTick;
	DWORD BeforeElapsed = Elapsed - m_dwOldElapsed;

	if( m_nPointPhase < TUTORIAL_CAM_PHASE_NUM )
	{
		DWORD alphaTest, alphaBlend, srcBlendOld, destBlendOld, dwLighting, dwAmbient, dwFogEnable;
		DWORD  dwTextureState[3];
		DWORD dwZenable;
		// 상태값 보존.
		GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_ZENABLE, &dwZenable );
		GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_FOGENABLE, &dwFogEnable );
		GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, &alphaBlend );
		GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_ALPHATESTENABLE, &alphaTest );
		GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_SRCBLEND, &srcBlendOld );
		GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_DESTBLEND, &destBlendOld );
		GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_LIGHTING, &dwLighting );
		GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_AMBIENT, &dwAmbient);
		GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG1, &dwTextureState[0] );
		GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG2, &dwTextureState[1] );
		GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAOP,   &dwTextureState[2] );

		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ZENABLE, FALSE);
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_FOGENABLE, FALSE );
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE);
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, TRUE);
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA);
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_ONE);
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHATESTENABLE   , TRUE );

		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG2 );


		D3DTLVERTEX TLVec[4];
		DWORD Alpha = 100;

		Alpha += sin( (float)Elapsed*0.007f ) * 100.0f;

		RECT rect;
		float tu1 = 0.0f;
		float tv1 = 0.0f;
		float tu2 = 0.0f;
		float tv2 = 0.0f;

		float ScreenCenterX = (float)(g_pNk2DFrame->GetClientWidth() / 2);
		float ScreenCenterY = (float)(g_pNk2DFrame->GetClientHeight() / 2);


		CNkMob *pMob = NULL;
		LHMobMap_it prMob;
		for (prMob = pCMyApp->m_pUIMgr->m_mob_map.begin(); prMob != pCMyApp->m_pUIMgr->m_mob_map.end(); prMob++)
		{
			pMob = (*prMob).second;
			if( pMob->m_nMobNum == 57 ) // 경비 소장
				break;
		}


		switch( m_nPointPhase )
		{
		case 0: // 상
			rect.left = ScreenCenterX-40.0f;
			rect.top = ScreenCenterY-150.0f;
			rect.right = ScreenCenterX+40.0f;
			rect.bottom = ScreenCenterY-50.0f;

//			Convert640Pos( &rect );

			TLVec[0] = D3DTLVERTEX( D3DVECTOR((float)rect.left,(float)rect.bottom,0.0f), 1, RGBA_MAKE(255,255,255,Alpha), 0, 0.0f, 1.0f );
			TLVec[1] = D3DTLVERTEX( D3DVECTOR((float)rect.left,(float)rect.top,0.0f), 1, RGBA_MAKE(255,255,255,Alpha), 0, 0.0f, 0.0f );
			TLVec[2] = D3DTLVERTEX( D3DVECTOR((float)rect.right,(float)rect.top,0.0f), 1, RGBA_MAKE(255,255,255,Alpha), 0, 1.0f, 0.0f );
			TLVec[3] = D3DTLVERTEX( D3DVECTOR((float)rect.right,(float)rect.bottom,0.0f), 1, RGBA_MAKE(255,255,255,Alpha), 0, 1.0f, 1.0f );

			GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_TUTORIAL_ARROW ] ) );

			if( pMob )
			{
				// IDS_TUTORIAL_TALK4 : 마우스 오른쪽 버튼을 클릭한 채 위쪽 방향으로 마우스를 움직여라
				sprintf( strTemp, (char*)G_STRING(IDS_TUTORIAL_TALK4) );
				pMob->SetMobChatMsg( strTemp );
			}


			break;
		case 1: // 하
			rect.left = ScreenCenterX-40.0f;
			rect.top = ScreenCenterY+50.0f;
			rect.right = ScreenCenterX+40.0f;
			rect.bottom = ScreenCenterY+150.0f;

//			Convert640Pos( &rect );

			TLVec[0] = D3DTLVERTEX( D3DVECTOR((float)rect.left,(float)rect.bottom,0.0f), 1, RGBA_MAKE(255,255,255,Alpha), 0, 0.0f, 0.0f );
			TLVec[1] = D3DTLVERTEX( D3DVECTOR((float)rect.left,(float)rect.top,0.0f), 1, RGBA_MAKE(255,255,255,Alpha), 0, 0.0f, 1.0f );
			TLVec[2] = D3DTLVERTEX( D3DVECTOR((float)rect.right,(float)rect.top,0.0f), 1, RGBA_MAKE(255,255,255,Alpha), 0, 1.0f, 1.0f );
			TLVec[3] = D3DTLVERTEX( D3DVECTOR((float)rect.right,(float)rect.bottom,0.0f), 1, RGBA_MAKE(255,255,255,Alpha), 0, 1.0f, 0.0f );

			GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_TUTORIAL_ARROW ] ) );

			if( pMob )
			{
				// IDS_TUTORIAL_TALK5 : 마우스 오른쪽 버튼을 클릭한 채 아래쪽 방향으로 마우스를 움직여라
				sprintf( strTemp, (char*)G_STRING(IDS_TUTORIAL_TALK5) );
				pMob->SetMobChatMsg( strTemp );
			}

			break;
		case 2: // 좌
			rect.left = ScreenCenterX-200.0f;
			rect.top = ScreenCenterY-40.0f;
			rect.right = ScreenCenterX-100.0f;
			rect.bottom = ScreenCenterY+40.0f;

//			Convert640Pos( &rect );

			TLVec[0] = D3DTLVERTEX( D3DVECTOR((float)rect.left,(float)rect.bottom,0.0f), 1, RGBA_MAKE(255,255,255,Alpha), 0, 0.0f, 0.0f );
			TLVec[1] = D3DTLVERTEX( D3DVECTOR((float)rect.left,(float)rect.top,0.0f), 1, RGBA_MAKE(255,255,255,Alpha), 0, 1.0f, 0.0f );
			TLVec[2] = D3DTLVERTEX( D3DVECTOR((float)rect.right,(float)rect.top,0.0f), 1, RGBA_MAKE(255,255,255,Alpha), 0, 1.0f, 1.0f );
			TLVec[3] = D3DTLVERTEX( D3DVECTOR((float)rect.right,(float)rect.bottom,0.0f), 1, RGBA_MAKE(255,255,255,Alpha), 0, 0.0f, 1.0f );

			GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_TUTORIAL_ARROW ] ) );

			if( pMob )
			{
				// IDS_TUTORIAL_TALK6 : 마우스 오른쪽 버튼을 클릭한 채 왼쪽 방향으로 마우스를 움직여라
				sprintf( strTemp, (char*)G_STRING(IDS_TUTORIAL_TALK6) );
				pMob->SetMobChatMsg( strTemp );
			}

			break;

		case 3: // 우
			rect.left = ScreenCenterX+100.0f;
			rect.top = ScreenCenterY-40.0f;
			rect.right = ScreenCenterX+200.0f;
			rect.bottom = ScreenCenterY+40.0f;

//			Convert640Pos( &rect );

			TLVec[0] = D3DTLVERTEX( D3DVECTOR((float)rect.left,(float)rect.bottom,0.0f), 1, RGBA_MAKE(255,255,255,Alpha), 0, 1.0f, 1.0f );
			TLVec[1] = D3DTLVERTEX( D3DVECTOR((float)rect.left,(float)rect.top,0.0f), 1, RGBA_MAKE(255,255,255,Alpha), 0, 0.0f, 1.0f );
			TLVec[2] = D3DTLVERTEX( D3DVECTOR((float)rect.right,(float)rect.top,0.0f), 1, RGBA_MAKE(255,255,255,Alpha), 0, 0.0f, 0.0f );
			TLVec[3] = D3DTLVERTEX( D3DVECTOR((float)rect.right,(float)rect.bottom,0.0f), 1, RGBA_MAKE(255,255,255,Alpha), 0, 1.0f, 0.0f );

			GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_TUTORIAL_ARROW ] ) );

			if( pMob )
			{
				// IDS_TUTORIAL_TALK7 : 마우스 오른쪽 버튼을 클릭한 채 오른쪽 방향으로 마우스를 움직여라
				sprintf( strTemp, (char*)G_STRING(IDS_TUTORIAL_TALK7) );
				pMob->SetMobChatMsg( strTemp );
			}

			break;

		case 4: // 줌인
			rect.left = ScreenCenterX-50.0f;
			rect.top = ScreenCenterY-150.0f;
			rect.right = ScreenCenterX+50.0f;
			rect.bottom = ScreenCenterY-100.0f;
			Alpha = 255;

//			Convert640Pos( &rect );


			TLVec[0] = D3DTLVERTEX( D3DVECTOR((float)rect.left,(float)rect.bottom,0.0f), 1, RGBA_MAKE(255,255,255,Alpha), 0, 0.0f, 1.0f );
			TLVec[1] = D3DTLVERTEX( D3DVECTOR((float)rect.left,(float)rect.top,0.0f), 1, RGBA_MAKE(255,255,255,Alpha), 0, 0.0f, 0.0f );
			TLVec[2] = D3DTLVERTEX( D3DVECTOR((float)rect.right,(float)rect.top,0.0f), 1, RGBA_MAKE(255,255,255,Alpha), 0, 1.0f, 0.0f );
			TLVec[3] = D3DTLVERTEX( D3DVECTOR((float)rect.right,(float)rect.bottom,0.0f), 1, RGBA_MAKE(255,255,255,Alpha), 0, 1.0f, 1.0f );

			if( timeGetTime()%1000 > 500 )
				GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_MOBSHOWUP_LASER ] ) ); // 이미지는 나중에 바꾸자.
			else
				GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_TUTORIAL_ARROW ] ) );

			break;
		case 5: // 줌아웃
			rect.left = ScreenCenterX-50.0f;
			rect.top = ScreenCenterY-150.0f;
			rect.right = ScreenCenterX+50.0f;
			rect.bottom = ScreenCenterY-100.0f;
			Alpha = 255;
//			Convert640Pos( &rect );

			TLVec[0] = D3DTLVERTEX( D3DVECTOR((float)rect.left,(float)rect.bottom,0.0f), 1, RGBA_MAKE(255,255,255,Alpha), 0, 0.0f, 1.0f );
			TLVec[1] = D3DTLVERTEX( D3DVECTOR((float)rect.left,(float)rect.top,0.0f), 1, RGBA_MAKE(255,255,255,Alpha), 0, 0.0f, 0.0f );
			TLVec[2] = D3DTLVERTEX( D3DVECTOR((float)rect.right,(float)rect.top,0.0f), 1, RGBA_MAKE(255,255,255,Alpha), 0, 1.0f, 0.0f );
			TLVec[3] = D3DTLVERTEX( D3DVECTOR((float)rect.right,(float)rect.bottom,0.0f), 1, RGBA_MAKE(255,255,255,Alpha), 0, 1.0f, 1.0f );

			if( timeGetTime()%1000 > 500 )
				GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_TUTORIAL_ARROW ] ) );
			else
				GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_WARGIANTDIE_CORONA ] ) );

			break;
		}


		GET_D3DDEVICE()->DrawPrimitive(D3DPT_TRIANGLEFAN    , D3DFVF_TLVERTEX, TLVec, 4, 0);




		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState[2] );

		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ZENABLE, dwZenable);
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_FOGENABLE, dwFogEnable );
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ALPHATESTENABLE, alphaTest );
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, alphaBlend );
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND, srcBlendOld);
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND, destBlendOld);
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_AMBIENT, dwAmbient);
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, dwLighting );
	}

	m_dwOldElapsed = Elapsed;

	return TRUE;
}

int  CTutorialCamAct::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	int x, y;

	switch( msg )
	{
	case WM_MOUSEMOVE:

		x = LOWORD (lParam);
		y = HIWORD (lParam);

		// 페이즈 증가 체크
		if( pCMyApp->m_bRotateCamera ) // 카메라 회전 중에만 체크.
		{
			switch( m_nPointPhase )
			{
			case 0: // 상

				if( pCMyApp->m_ptCurPos.y - y > 0 ) // 위쪽움직임만...
					m_Data1 += pCMyApp->m_ptCurPos.y - y; // 누적 이동량을 체크.

				if( m_Data1 > 30 ) // 일정량이상 원하는 방향으로 움직였으면...
				{
					++m_nPointPhase;
					m_Data1 = 0;
				}

				break;

			case 1: // 하

				if( pCMyApp->m_ptCurPos.y - y < 0 ) // 아래쪽움직임만...
					m_Data1 += pCMyApp->m_ptCurPos.y - y; // 누적 이동량을 체크.

				if( m_Data1 < -40 ) // 일정량이상 원하는 방향으로 움직였으면...
				{
					++m_nPointPhase;
					m_Data1 = 0;
				}
				break;

			case 2: // 좌

				if( pCMyApp->m_ptCurPos.x - x > 0 ) // 왼쪽움직임만...
					m_Data1 += pCMyApp->m_ptCurPos.x - x; // 누적 이동량을 체크.

				if( m_Data1 > 70 ) // 일정량이상 원하는 방향으로 움직였으면...
				{
					++m_nPointPhase;
					m_Data1 = 0;
				}
				break;

			case 3: // 우

				if( pCMyApp->m_ptCurPos.x - x < 0 ) // 오른쪽움직임만...
					m_Data1 += pCMyApp->m_ptCurPos.x - x; // 누적 이동량을 체크.

				if( m_Data1 < -70 ) // 일정량이상 원하는 방향으로 움직였으면...
				{
					++m_nPointPhase;
					m_Data1 = 0;
				}
				break;
			}
		}
		break;

	case 0x020A: // 휠마우스 메세지(리스트 영역안에서의 휠 메세지를 강제로 스크롤에 연동시켜준다.)

		switch( m_nPointPhase )
		{
		case 4: // 줌인

			if( (short)HIWORD(wParam) > 0 )
			{
				m_Data1 += (short)HIWORD(wParam); // 누적 이동량을 체크.

				if( m_Data1 > 500 ) // 일정량이상 원하는 방향으로 움직였으면...
				{
					++m_nPointPhase;
					m_Data1 = 0;
				}
			}

			break;
		case 5: // 줌아웃

			if( (short)HIWORD(wParam) < 0 )
			{
				m_Data1 += (short)HIWORD(wParam); // 누적 이동량을 체크.

				if( m_Data1 < -500 ) // 일정량이상 원하는 방향으로 움직였으면...
				{
					++m_nPointPhase;
					m_Data1 = 0;
				}
			}

			break;
		}

		break;
	}

	return 0; // 이 연출 중엔 다른 움직임 되도록.
}

bool CTutorialCamAct::EndCheck()
{
	if( m_nPointPhase >= TUTORIAL_CAM_PHASE_NUM )
	{
		pCMyApp->EndRotateCamera(); // 혹시 클릭중이라고 인식하지 않도록...

		return TRUE;
	}

	return FALSE;
}






// 튜토리얼 사냥 체험.
CTutorialHuntAct::CTutorialHuntAct()
{
	m_dwOldElapsed = 0;
	m_dwStartTick = timeGetTime();

	m_bEndFlag = FALSE;
}

CTutorialHuntAct::~CTutorialHuntAct()
{

}

bool CTutorialHuntAct::Render()
{


	if( !GET_D3DDEVICE() )
		return FALSE;

//	CNkMob* pMob;
	float fx = 0.0f;
	float fz = 0.0f;
	float dir = 0.0f;
	float dist = 0.0f;
	float fTemp = 0.0f;
	char strTemp[1024];


	DWORD Elapsed = timeGetTime() - m_dwStartTick;
	DWORD BeforeElapsed = Elapsed - m_dwOldElapsed;

	if( m_dwOldElapsed < 500 && Elapsed >= 500 )
	{
		/*
				dir = RandomNum(-1.5f,1.5f);
				dist = 50.0f + RandomNum(-20.0f,30.0f);

				fx = g_pRoh->m_wx + cos(dir) * dist; // 주인공으로부터 dir 방향으로 dist 만큼 떨어진 거리.
				fz = g_pRoh->m_wz + sin(dir) * dist;

				pCMyApp->m_pUIMgr->AddMob( 23, 100, fx, fz, dir-3.14f, 100 );

				if( pMob = pCMyApp->m_pUIMgr->FindMobById(100) )
				{
					EffectSort ef_sort;
					ZeroMemory(&ef_sort, sizeof(EffectSort));
					ef_sort.nType = FX_TELEPORT;
					ef_sort.vPos.x = pMob->m_wx;
					ef_sort.vPos.z = pMob->m_wz;
					ef_sort.vPos.y = pMob->m_wy;

					if( pCMyApp->m_pFxSet )
						pCMyApp->m_pFxSet->InsertFxEffect(ef_sort);
				}




				dir = RandomNum(-1.5f,1.5f);
				dist = 50.0f + RandomNum(-20.0f,30.0f);

				fx = g_pRoh->m_wx + cos(dir) * dist; // 주인공으로부터 dir 방향으로 dist 만큼 떨어진 거리.
				fz = g_pRoh->m_wz + sin(dir) * dist;

				pCMyApp->m_pUIMgr->AddMob( 23, 101, fx, fz, dir-3.14f, 100 );
				if( pMob = pCMyApp->m_pUIMgr->FindMobById(101) )
				{
					EffectSort ef_sort;
					ZeroMemory(&ef_sort, sizeof(EffectSort));
					ef_sort.nType = FX_TELEPORT;
					ef_sort.vPos.x = pMob->m_wx;
					ef_sort.vPos.z = pMob->m_wz;
					ef_sort.vPos.y = pMob->m_wy;

					if( pCMyApp->m_pFxSet )
						pCMyApp->m_pFxSet->InsertFxEffect(ef_sort);
				}
		*/
		if (g_pTcpIp)
			g_pTcpIp->SendNetMessage("tutorial 4\n"); // 늑대 등장 요청

	}
	/*
		// 특정 타이밍 이후 계속 출력되야하는 녀석들.
		if( Elapsed >= 500 )  // 이 시간 이후론 주인공은 계속 출력.
		{
			if( ( pMob = pCMyApp->m_pUIMgr->FindMobById(100) ) && !pMob->IsDead() && !pMob->IsMoving() && pMob->GetMotionState() != 1 )
			{
				// 간단한 AI를 지정해준다.
				dist = DistPlToPlAbs( g_pRoh->m_wx, g_pRoh->m_wz, pMob->m_wx, pMob->m_wz);

				if( dist > 20.0f ) // 사정거리 보다 멀면.
				{
					float dir = (float)atan2( pMob->m_wz-g_pRoh->m_wz, pMob->m_wx-g_pRoh->m_wx ); // 방향을 구하고.
					dir -= 0.5f; // 약간 다른곳으로도 갈수있게..
					float fx = g_pRoh->m_wx + cos(dir) * 15.0f; // 주인공으로부터 dir 방향으로 dist 만큼 떨어진 거리.
					float fz = g_pRoh->m_wz + sin(dir) * 15.0f;

					int Frame = (int)((dist-15.0f)/2.0f);

					pMob->Move( fx, fz, Frame );
				}
				else
				{
					if( timeGetTime()%2000 < 50 ) // 주기적으로 공격을 시도하도록
					{
						pMob->Attack( g_pRoh->m_wx, g_pRoh->m_wz );
						pMob->SetAttackChaId( g_pRoh->m_nVirId );
					}
				}
			}


			if( (pMob = pCMyApp->m_pUIMgr->FindMobById(101)) && !pMob->IsDead() && !pMob->IsMoving() && pMob->GetMotionState() != 1 )
			{
				// 간단한 AI를 지정해준다.
				dist = DistPlToPlAbs( g_pRoh->m_wx, g_pRoh->m_wz, pMob->m_wx, pMob->m_wz);

				if( dist > 20.0f ) // 사정거리 보다 멀면.
				{
					float dir = (float)atan2( pMob->m_wz-g_pRoh->m_wz, pMob->m_wx-g_pRoh->m_wx ); // 방향을 구하고.
					dir += 1.0f; // 약간 다른곳으로도 갈수있게..
					float fx = g_pRoh->m_wx + cos(dir) * 10.0f; // 주인공으로부터 dir 방향으로 dist 만큼 떨어진 거리.
					float fz = g_pRoh->m_wz + sin(dir) * 10.0f;

					int Frame = (int)((dist-10.0f)/2.0f);

					pMob->Move( fx, fz, Frame );
				}
				else
				{
					if( timeGetTime()%3000 < 50 ) // 주기적으로 공격을 시도하도록
					{
						pMob->Attack( g_pRoh->m_wx, g_pRoh->m_wz );
						pMob->SetAttackChaId( g_pRoh->m_nVirId );
					}
				}
			}
		}
	*/
	// 주기적으로 재촉하자.
	if( timeGetTime()%10000 < 50 )
	{
		CNkMob *pMob = NULL;
		LHMobMap_it prMob;
		for (prMob = pCMyApp->m_pUIMgr->m_mob_map.begin(); prMob != pCMyApp->m_pUIMgr->m_mob_map.end(); prMob++)
		{
			pMob = (*prMob).second;
			if( pMob->m_nMobNum == 57 ) // 경비 소장
			{
				// IDS_TUTORIAL_TALK8 : 늑대들을 모두 잡아야 한다.
				sprintf( strTemp, (char*)G_STRING(IDS_TUTORIAL_TALK8) );

				pMob->SetMobChatMsg( strTemp );
				break;
			}
		}
	}


	m_dwOldElapsed = Elapsed;

	return TRUE;
}

int  CTutorialHuntAct::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	return 0;
}

bool CTutorialHuntAct::EndCheck()
{
	DWORD Elapsed = timeGetTime() - m_dwStartTick;

	if( g_pRoh->m_Level != 1 ) // 두마리 잡으면 레벨업시켜주니까 레벨로 종료를 체크해준다
	{
		return TRUE;
	}


	/*  // 늑대가 없는지를 체크하지말고 레벨로 체크하기로 했다.
		if( Elapsed >= 5000 ) // 늑대 등장 시간 이후 부터 체크. (렉을 고려해서 시간을 매우 여유롭게 준다.)
		{
			CNkMob *pMob = NULL;
			LHMobMap_it prMob;

			for (prMob = pCMyApp->m_pUIMgr->m_mob_map.begin(); prMob != pCMyApp->m_pUIMgr->m_mob_map.end(); prMob++)
			{
				pMob = (*prMob).second;
				if( pMob->m_nMobNum == 23 ) // 늑대가 있으면...
					return FALSE; // 아직 안 끝났다.
			}

			return TRUE; // 늑대 등장 시간 이후 늑대가 없으면...
		}
	*/


	/*
		if( Elapsed >= 500 ) // 늑대 등장 시간 이후 부터 체크.
		{
			CNkMob* pMob;
			CNkMob* pMob2;

			pMob = pCMyApp->m_pUIMgr->FindMobById(100);
			pMob2 = pCMyApp->m_pUIMgr->FindMobById(101);

			if( (!pMob||pMob->IsDead()) && (!pMob2||pMob2->IsDead()) ) // 둘다 잡았으면...
			{
				g_pRoh->SetLevel(2); // 레벨도 업.
				g_pRoh->SetLevelupPoint( 3 ); // 포인트도 준다.

				// 레벨업 효과.
				EffectSort ef_sort;
				ZeroMemory(&ef_sort, sizeof(EffectSort));
				ef_sort.nType = FX_LEVELUP;
				ef_sort.pNkChaTo = g_pRoh;

				ef_sort.vPos = D3DVECTOR( g_pRoh->m_wx,
					pCMyApp->GetLandPtr()->GetHFHeight(g_pRoh->m_wx, g_pRoh->m_wz),
					g_pRoh->m_wz );

				g_pRoh->InsertEffectNum( pCMyApp->m_pFxSet->InsertFxEffect( ef_sort ) );

				ef_sort.nCurFrame = -5;
				g_pRoh->InsertEffectNum( pCMyApp->m_pFxSet->InsertFxEffect( ef_sort ) );

				if ( g_pDSound )
					g_pDSound->Play(g_EffectSound[ EFF_SND_LEVELUP ]);


				return TRUE;
			}
		}
	*/

	return FALSE;
}



// 튜토리얼 포인트 업 체험.
CTutorialPointAct::CTutorialPointAct()
{
	m_dwOldElapsed = 0;
	m_dwStartTick = timeGetTime();

	m_bEndFlag = FALSE;

	g_pNk2DFrame->ShowInterfaceWindow( TRUE, SCORE ); // 포인트 창을 열어준다.
}

CTutorialPointAct::~CTutorialPointAct()
{
}

bool CTutorialPointAct::Render()
{
	char strTemp[1024];
	/*
		if( !g_pNk2DFrame )
			return FALSE;

		CControlScore *pScore = NULL;

		pScore = (CControlScore*)( g_pNk2DFrame->GetInterface(SCORE) );

		if( pScore )
		{
			pScore->Draw();
		}
	*/
	// 주기적으로 재촉하자.
	if( timeGetTime()%10000 < 50 )
	{
		CNkMob *pMob = NULL;
		LHMobMap_it prMob;
		for (prMob = pCMyApp->m_pUIMgr->m_mob_map.begin(); prMob != pCMyApp->m_pUIMgr->m_mob_map.end(); prMob++)
		{
			pMob = (*prMob).second;
			if( pMob->m_nMobNum == 57 ) // 경비 소장
			{
				// IDS_TUTORIAL_TALK9 : 포인트를 모두 사용해야 한다.
				sprintf( strTemp, (char *)G_STRING(IDS_TUTORIAL_TALK9) );

				pMob->SetMobChatMsg( strTemp );
				break;
			}
		}
	}


	return TRUE;
}

int  CTutorialPointAct::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	/*
		int i = 0;

		if( !g_pNk2DFrame )
			return -1;

		CControlScore *pScore = NULL;

		pScore = (CControlScore*)( g_pNk2DFrame->GetInterface(SCORE) );

		if( !pScore )
			return -1;

		if( msg == WM_LBUTTONDOWN )
		{
			i = 4; // 힘
			pScore->m_PointUpBtn[i].MsgProc(hWnd, msg, wParam, lParam);
			i = 5; // 지혜
			pScore->m_PointUpBtn[i].MsgProc(hWnd, msg, wParam, lParam);
			i = 6; // 기민
			pScore->m_PointUpBtn[i].MsgProc(hWnd, msg, wParam, lParam);
			i = 7; // 체질
			pScore->m_PointUpBtn[i].MsgProc(hWnd, msg, wParam, lParam);
			i = 8; // 통솔
			pScore->m_PointUpBtn[i].MsgProc(hWnd, msg, wParam, lParam);
		}
		else if( msg == WM_LBUTTONUP )
		{
			// 포인트 업 버튼에 대해서 별도의 메시지 처리를 해준다.
			if( g_pRoh->m_LevelupPoint >0 )
			{
				i = 4; // 힘
				pScore->m_PointUpBtn[i].MsgProc(hWnd, msg, wParam, lParam);
				if( pScore->m_PointUpBtn[i].GetState() == BTN_ACTION)
				{
					++g_pRoh->m_Str;
					pScore->SetStr();
					--g_pRoh->m_LevelupPoint;
					pScore->SetLevelupPoint();
					pScore->m_PointUpBtn[i].m_iBtnFlg = BTN_NORMAL;
				}

				i = 5; // 지혜
				pScore->m_PointUpBtn[i].MsgProc(hWnd, msg, wParam, lParam);
				if( pScore->m_PointUpBtn[i].GetState() == BTN_ACTION)
				{
					++g_pRoh->m_Int;
					pScore->SetInt();
					--g_pRoh->m_LevelupPoint;
					pScore->SetLevelupPoint();
					pScore->m_PointUpBtn[i].m_iBtnFlg = BTN_NORMAL;
				}

				i = 6; // 기민
				pScore->m_PointUpBtn[i].MsgProc(hWnd, msg, wParam, lParam);
				if( pScore->m_PointUpBtn[i].GetState() == BTN_ACTION)
				{
					++g_pRoh->m_Dex;
					pScore->SetDex();
					--g_pRoh->m_LevelupPoint;
					pScore->SetLevelupPoint();
					pScore->m_PointUpBtn[i].m_iBtnFlg = BTN_NORMAL;
				}

				i = 7; // 체질
				pScore->m_PointUpBtn[i].MsgProc(hWnd, msg, wParam, lParam);
				if( pScore->m_PointUpBtn[i].GetState() == BTN_ACTION)
				{
					++g_pRoh->m_Con;
					pScore->SetCon();
					--g_pRoh->m_LevelupPoint;
					pScore->SetLevelupPoint();
					pScore->m_PointUpBtn[i].m_iBtnFlg = BTN_NORMAL;
				}

				i = 8; // 통솔
				pScore->m_PointUpBtn[i].MsgProc(hWnd, msg, wParam, lParam);
				if( pScore->m_PointUpBtn[i].GetState() == BTN_ACTION)
				{
					++g_pRoh->m_Cha;
					pScore->SetCha();
					--g_pRoh->m_LevelupPoint;
					pScore->SetLevelupPoint();
					pScore->m_PointUpBtn[i].m_iBtnFlg = BTN_NORMAL;
				}
			}
		}

		return 1; // 이녀석은 다른 녀석이 메세지 못받게 하자.
	*/
	return 0;
}

bool CTutorialPointAct::EndCheck()
{
	if(g_pRoh->m_LevelupPoint <= 0 )
	{
		return TRUE;
	}

	return FALSE;
}


// 튜토리얼 물약사기 체험.
CTutorialPosionBuyAct::CTutorialPosionBuyAct()
{
	m_dwOldElapsed = 0;
	m_dwStartTick = timeGetTime();

	m_bEndFlag = FALSE;
}

CTutorialPosionBuyAct::~CTutorialPosionBuyAct()
{
}

#define TUTORIAL_POSIONBUY_TIMEFLAG_SHOPAPPEAR 500 // 상인 등장

bool CTutorialPosionBuyAct::Render()
{


	if( !GET_D3DDEVICE() )
		return FALSE;
	/*
		CNkMob* pMob;
		float fx = 0.0f;
		float fz = 0.0f;
		float dir = 0.0f;
		float dist = 0.0f;
		float fTemp = 0.0f;
	*/

	DWORD Elapsed = timeGetTime() - m_dwStartTick;
	DWORD BeforeElapsed = Elapsed - m_dwOldElapsed;


	if( m_dwOldElapsed < TUTORIAL_POSIONBUY_TIMEFLAG_SHOPAPPEAR && Elapsed >= TUTORIAL_POSIONBUY_TIMEFLAG_SHOPAPPEAR )
	{
		/*
				fx = 2206.54f; // 특정 좌표를 정해둔다
				fz = 10409.18f;

				pCMyApp->m_pUIMgr->AddMob( 94, 1002, fx, fz, 0.0f, 100 );

				if( pMob = pCMyApp->m_pUIMgr->FindMobById(1002) )
					EffectSort ef_sort;
					ZeroMemory(&ef_sort, sizeof(EffectSort));
					ef_sort.nType = FX_TELEPORT;
					ef_sort.vPos.x = pMob->m_wx;
					ef_sort.vPos.z = pMob->m_wz;
					ef_sort.vPos.y = pMob->m_wy;

					if( pCMyApp->m_pFxSet )
						pCMyApp->m_pFxSet->InsertFxEffect(ef_sort);
				}
		*/
		if (g_pTcpIp)
			g_pTcpIp->SendNetMessage("tutorial 6 -1\n"); // 물약상인 등장 요청

	}




	m_dwOldElapsed = Elapsed;



	return TRUE;
}

int  CTutorialPosionBuyAct::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	if( !g_pNk2DFrame )
		return -1;

	if( !g_pNk2DFrame->GetControlShop() )
		return -1;

	char strTemp[1024];
	CNkMob *pMob = NULL;
	LHMobMap_it prMob;
//	int x, y;

	switch( msg )
	{
	case WM_LBUTTONDOWN:
		/*
				// 더블 클릭인지 체크.
				if( timeGetTime() - m_dwPrevClickTick < 500 ) // 0.5초 사이에 다시한번 클릭한 경우.
				{
					if( g_pNk2DFrame->IsShopVisible() )
					{
						int slot_x_num = -1;
						int slot_y_num = -1;
						int vnum = -1;

						x = LOWORD (lParam);
						y = HIWORD (lParam);

						if (!g_pNk2DFrame->GetControlShop()->IsInside(x, y))
							return 0;

						if (x >= g_pNk2DFrame->GetControlShop()->m_shopslotstart_x && y >= g_pNk2DFrame->GetControlShop()->m_shopslotstart_y)
						{
							if( g_pRoh->m_ExtraSlot ) // 엑스트라 슬롯이 있다면 안사지게... (06-01-05 원석)
								return 1;

							slot_x_num = ( (x - g_pNk2DFrame->GetControlShop()->m_shopslotstart_x) / SLOT_WIDTH);
							slot_y_num = ( (y - g_pNk2DFrame->GetControlShop()->m_shopslotstart_y) / SLOT_HEIGHT);

							vnum = g_pNk2DFrame->GetControlShop()->GetSlotVnum(slot_x_num, slot_y_num); // v

							if( vnum >= 0 )
							{
								// 인벤에 아이템을 넣고 돈을 깍는다.
								g_pRoh->m_ExtraSlot = new CItem;
								if(g_pRoh->m_ExtraSlot)
								{
									g_pRoh->m_ExtraSlot->m_Index = 0;
									g_pRoh->m_ExtraSlot->m_Vnum = vnum;
									g_pRoh->m_ExtraSlot->m_ProtoNum = g_ItemList.FindItem(vnum);
								}

								g_pRoh->AddToInven( 0, &x, &y ); // 빈공간을 찾아서 넣어주는 녀석. m_ExtraSlot 슬롯 아이템을 넣는다.

								SAFE_DELETE( g_pRoh->m_ExtraSlot ); // 다시 지워준다.
							}
						}
						return 1;
					}
				}

				m_dwPrevClickTick = timeGetTime();

		*/


		//------------------------------------------------------------------------
		// 전체 몹들 중에서 클릭된 몹을 찾아서 처리.
		//------------------------------------------------------------------------
		for (prMob = pCMyApp->m_pUIMgr->m_mob_map.begin(); prMob != pCMyApp->m_pUIMgr->m_mob_map.end(); prMob++)
		{
			pMob = (*prMob).second;
			// Color Pickng Buffer 와 몹의 Color 가 동일하다면 ( 선택되었다면 )
			if (pMob->m_wColorIdx == pCMyApp->m_pUIMgr->GetPickColor() )
			{
				if( pMob->m_nMobNum == 57 ) // 교관 NPC 클릭시.
				{
					// 인벤상에 물약이 있는지 체크한다.

					if( g_pRoh->FindItemByVnumFromAll(140) ) // '작은치료물약'을 가지고 있는지 여부.
					{
						m_bEndFlag = TRUE;
					}
					else // 만약 작은 치료 물약을 안가지고 클릭했으면.
					{
						// IDS_TUTORIAL_TALK10 : '작은치료물약'을 가지고 와야 한다. 다시 가서 사오도록
						sprintf( strTemp, (char *)G_STRING(IDS_TUTORIAL_TALK10) );
						pMob->SetMobChatMsg( strTemp );

						if (g_pTcpIp)
							g_pTcpIp->SendNetMessage("tutorial 6 0\n"); // 구입 실패 초기화 요청
					}
				}
			}
		}

		break;
	}

	return 0;
}

bool CTutorialPosionBuyAct::EndCheck()
{
	if( m_bEndFlag )
	{
		if (g_pTcpIp)
			g_pTcpIp->SendNetMessage("tutorial 6 1\n"); // 구입 성공 Npc 제거 요청

		return TRUE;
	}

	return FALSE;
}




// 튜토리얼 물약사용 체험.
CTutorialPosionUseAct::CTutorialPosionUseAct()
{
	m_dwOldElapsed = 0;
	m_dwStartTick = timeGetTime();

	m_bEndFlag = FALSE;
}

CTutorialPosionUseAct::~CTutorialPosionUseAct()
{
	g_pNk2DFrame->CloseAllWindow();
}


bool CTutorialPosionUseAct::Render()
{


	if( !GET_D3DDEVICE() )
		return FALSE;

	char strTemp[1024];
	/*
		CNkMob* pMob;
		float fx = 0.0f;
		float fz = 0.0f;
		float dir = 0.0f;
		float dist = 0.0f;
		float fTemp = 0.0f;
	*/

	DWORD Elapsed = timeGetTime() - m_dwStartTick;
	DWORD BeforeElapsed = Elapsed - m_dwOldElapsed;

	g_pRoh->m_Vital = g_pRoh->m_MaxVital/2; // 피를 가라로 깍는다.

	// 주기적으로 재촉하자.
	if( timeGetTime()%10000 < 50 )
	{
		CNkMob *pMob = NULL;
		LHMobMap_it prMob;
		for (prMob = pCMyApp->m_pUIMgr->m_mob_map.begin(); prMob != pCMyApp->m_pUIMgr->m_mob_map.end(); prMob++)
		{
			pMob = (*prMob).second;
			if( pMob->m_nMobNum == 57 ) // 경비 소장
			{
				// IDS_TUTORIAL_TALK11 : '작은치료물약'을 퀵슬롯에 넣고 사용해 보거라
				sprintf( strTemp, (char *)G_STRING(IDS_TUTORIAL_TALK11) );

				pMob->SetMobChatMsg( strTemp );
				break;
			}
		}
	}


	m_dwOldElapsed = Elapsed;

	return TRUE;
}

int  CTutorialPosionUseAct::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	int x,y;

	if( !g_pNk2DFrame )
		return -1;

	if( !g_pNk2DFrame->GetControlBottom() )
		return -1;

	switch( msg )
	{
	case WM_KEYDOWN:

		x = -1;

		switch( (int)wParam )
		{
		case '1':
		case VK_NUMPAD1:
			x = 0;
			break;

		case '2':
		case VK_NUMPAD2:
			x = 1;
			break;

		case '3':
		case VK_NUMPAD3:
			x = 2;
			break;

		case '4':
		case VK_NUMPAD4:
			x = 3;
			break;

		case '5':
		case VK_NUMPAD5:
			x = 4;
			break;

		case '6':
		case VK_NUMPAD6:
			x = 5;
			break;
		}

		if( x >= 0 && x <=5)
		{
			if( g_pRoh->m_QuickSlot[x] && g_pRoh->m_QuickSlot[x]->m_Vnum == 140 ) // 작은 치료 물약을 사용한 거라면...
			{
				m_bEndFlag = TRUE;
				SAFE_DELETE(g_pRoh->m_QuickSlot[x]); // 직접 지워버린다.
				g_pRoh->CountQuickBySlot(x);
				return 1; // 실제로 지우진 않는다.
			}
		}

		break;

	case WM_RBUTTONDOWN:

		x = LOWORD (lParam);
		y = HIWORD (lParam);

		// 마우스 오른쪽 클릭으로 사용하는 경우.
		if( g_pNk2DFrame->GetControlBottom()->IsInsideQuickSlot(x,y) )
		{
			int slot_num = g_pNk2DFrame->GetControlBottom()->GetCurQuickSlotNum();
			// 마우스 우클릭 퀵슬롯 동작 처리..부분..
			if( slot_num >= 0 && slot_num <QUICK_SLOT_NUM*QUICK_SLOT_PAGE)
			{
				if( g_pRoh->m_QuickSlot[slot_num] && g_pRoh->m_QuickSlot[slot_num]->m_Vnum == 140 ) // 작은 치료 물약을 사용한 거라면...
				{
					m_bEndFlag = TRUE;
					SAFE_DELETE(g_pRoh->m_QuickSlot[slot_num]); // 직접 지워버린다.
					g_pRoh->CountQuickBySlot(slot_num);
					return 1; // 실제로 지우진 않는다.
				}
			}
		}

		break;
	}


	return 0;
}

bool CTutorialPosionUseAct::EndCheck()
{
	if( m_bEndFlag )
	{
		g_pRoh->m_Vital = g_pRoh->m_MaxVital; // 피도 가라로 채운다.

		return TRUE;
	}
	/*
		// 주인공 피로 체크하는 방식으로 바꿨다.
		if( g_pRoh->m_Vital == g_pRoh->m_MaxVital )
		{
			return TRUE;
		}
	*/
	return FALSE;
}



// 튜토리얼 장비사기 체험.
CTutorialEquipBuyAct::CTutorialEquipBuyAct()
{
	m_dwOldElapsed = 0;
	m_dwStartTick = timeGetTime();

	m_bEndFlag = FALSE;
}

CTutorialEquipBuyAct::~CTutorialEquipBuyAct()
{
}

bool CTutorialEquipBuyAct::Render()
{


	if( !GET_D3DDEVICE() )
		return FALSE;

//	CNkMob* pMob;
	float fx = 0.0f;
	float fz = 0.0f;
	float dir = 0.0f;
	float dist = 0.0f;
	float fTemp = 0.0f;


	DWORD Elapsed = timeGetTime() - m_dwStartTick;
	DWORD BeforeElapsed = Elapsed - m_dwOldElapsed;


	if( m_dwOldElapsed < TUTORIAL_POSIONBUY_TIMEFLAG_SHOPAPPEAR && Elapsed >= TUTORIAL_POSIONBUY_TIMEFLAG_SHOPAPPEAR )
	{
		if (g_pTcpIp)
			g_pTcpIp->SendNetMessage("tutorial 8 -1\n"); // 장비상인 등장 요청
	}

	m_dwOldElapsed = Elapsed;

	return TRUE;
}

int  CTutorialEquipBuyAct::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	if( !g_pNk2DFrame )
		return -1;

	if( !g_pNk2DFrame->GetControlShop() )
		return -1;

	char strTemp[1024];
	CNkMob *pMob = NULL;
	LHMobMap_it prMob;
//	int x, y;

	switch( msg )
	{
	case WM_LBUTTONDOWN:

		//------------------------------------------------------------------------
		// 전체 몹들 중에서 클릭된 몹을 찾아서 처리.
		//------------------------------------------------------------------------
		for (prMob = pCMyApp->m_pUIMgr->m_mob_map.begin(); prMob != pCMyApp->m_pUIMgr->m_mob_map.end(); prMob++)
		{
			pMob = (*prMob).second;
			// Color Pickng Buffer 와 몹의 Color 가 동일하다면 ( 선택되었다면 )
			if (pMob->m_wColorIdx == pCMyApp->m_pUIMgr->GetPickColor() )
			{
				if( pMob->m_nMobNum == 57 ) // 교관 NPC 클릭시.
				{
					// 인벤상에 물약이 있는지 체크한다.

					if( g_pRoh->FindItemByVnumFromAll(1592) && g_pRoh->FindItemByVnumFromAll(1593)
							&& g_pRoh->FindItemByVnumFromAll(1594) && g_pRoh->FindItemByVnumFromAll(1595) ) // '트레이닝 세트'을 가지고 있는지 여부.
					{
						m_bEndFlag = TRUE;
					}
					else // 만약 작은 치료 물약을 안가지고 클릭했으면.
					{
						// IDS_TUTORIAL_TALK12 : '트레이닝 세트'를 전부 가지고 있어야 한다. 다시 가서 사오도록
						sprintf( strTemp, (char *)G_STRING(IDS_TUTORIAL_TALK12) );
						pMob->SetMobChatMsg( strTemp );

						if (g_pTcpIp)
							g_pTcpIp->SendNetMessage("tutorial 8 0\n"); // 구입 실패 초기화 요청
					}
				}

//				return 1; // 클릭후 동작안하도록.
			}
		}

		break;
	}

	return 0;
}

bool CTutorialEquipBuyAct::EndCheck()
{
	if( m_bEndFlag )
	{
		if (g_pTcpIp)
			g_pTcpIp->SendNetMessage("tutorial 8 1\n"); // 구입 성공 Npc 제거 요청

		return TRUE;
	}

	return FALSE;
}


// 튜토리얼 장비 착용
CTutorialEquipUseAct::CTutorialEquipUseAct()
{
	m_dwOldElapsed = 0;
	m_dwStartTick = timeGetTime();

	m_bEndFlag = FALSE;
}

CTutorialEquipUseAct::~CTutorialEquipUseAct()
{
}


bool CTutorialEquipUseAct::Render()
{


	if( !GET_D3DDEVICE() )
		return FALSE;

	char strTemp[1024];
//	CNkMob* pMob;
	float fx = 0.0f;
	float fz = 0.0f;
	float dir = 0.0f;
	float dist = 0.0f;
	float fTemp = 0.0f;


	DWORD Elapsed = timeGetTime() - m_dwStartTick;
	DWORD BeforeElapsed = Elapsed - m_dwOldElapsed;


	// 주기적으로 재촉하자.
	if( timeGetTime()%10000 < 50 )
	{
		CNkMob *pMob = NULL;
		LHMobMap_it prMob;
		for (prMob = pCMyApp->m_pUIMgr->m_mob_map.begin(); prMob != pCMyApp->m_pUIMgr->m_mob_map.end(); prMob++)
		{
			pMob = (*prMob).second;
			if( pMob->m_nMobNum == 57 ) // 경비 소장
			{
				// IDS_TUTORIAL_TALK13 : '트레이닝셋트'를 전부 착용해야 한다.
				sprintf( strTemp, (char *)G_STRING(IDS_TUTORIAL_TALK13) );

				pMob->SetMobChatMsg( strTemp );
				break;
			}
		}
	}

	m_dwOldElapsed = Elapsed;

	return TRUE;
}

int  CTutorialEquipUseAct::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	return 0;
}

bool CTutorialEquipUseAct::EndCheck()
{
	if( (g_pRoh->m_Wearing[IARMOR_ARMOR]&&g_pRoh->m_Wearing[IARMOR_ARMOR]->m_Vnum==1592) // 트레이닝 자켓
			&& (g_pRoh->m_Wearing[IARMOR_PANTS]&&g_pRoh->m_Wearing[IARMOR_PANTS]->m_Vnum==1593) // 트레이닝 바지
			&& (g_pRoh->m_Wearing[IARMOR_SHOES]&&g_pRoh->m_Wearing[IARMOR_SHOES]->m_Vnum==1594) // 트레이닝 신발
			&& (g_pRoh->m_Wearing[IARMOR_HELMET]&&g_pRoh->m_Wearing[IARMOR_HELMET]->m_Vnum==1595) // 트레이닝 헬멧
	  ) // 트레이닝 셋트 착용 확인
	{
		return TRUE;
	}

	return FALSE;
}


// 튜토리얼 메뉴바 체험
CTutorialTopMenuAct::CTutorialTopMenuAct()
{
	m_dwOldElapsed = 0;
	m_dwStartTick = timeGetTime();

	m_nPointPhase = 0; // 어떤거 눌러봐야하는 지.

	m_bEndFlag = FALSE;

	g_pNk2DFrame->CloseAllWindow();
}

CTutorialTopMenuAct::~CTutorialTopMenuAct()
{
}

#define TUTORIAL_TOPMENU_PHASE_NUM 9 // '종료'제외 '종료'' 넣을거면 10로..

bool CTutorialTopMenuAct::Render()
{


	if( !GET_D3DDEVICE() )
		return FALSE;

//	CNkMob* pMob;
	float fx = 0.0f;
	float fz = 0.0f;
	float dir = 0.0f;
	float dist = 0.0f;
	float fTemp = 0.0f;
	char strTemp[1024];


	DWORD Elapsed = timeGetTime() - m_dwStartTick;
	DWORD BeforeElapsed = Elapsed - m_dwOldElapsed;

	switch( m_nPointPhase )
	{
	case 0: // 캐릭터.

		if( g_pNk2DFrame->IsInterfaceVisible(SCORE) )
		{
			CNkMob *pMob = NULL;
			LHMobMap_it prMob;
			for (prMob = pCMyApp->m_pUIMgr->m_mob_map.begin(); prMob != pCMyApp->m_pUIMgr->m_mob_map.end(); prMob++)
			{
				pMob = (*prMob).second;
				if( pMob->m_nMobNum == 57 ) // 경비 소장
				{
					// IDS_TUTORIAL_TALK14 : 캐릭터의 레벨이나 스테이터스를 확인할 때 사용하는 버튼이다.
					sprintf( strTemp, (char *)G_STRING(IDS_TUTORIAL_TALK14) );

					pMob->SetMobChatMsg( strTemp );
					break;
				}
			}

			++m_nPointPhase;
		}
		else
		{
			// 주기적으로 재촉하자.
			if( timeGetTime()%10000 < 50 )
			{
				CNkMob *pMob = NULL;
				LHMobMap_it prMob;
				for (prMob = pCMyApp->m_pUIMgr->m_mob_map.begin(); prMob != pCMyApp->m_pUIMgr->m_mob_map.end(); prMob++)
				{
					pMob = (*prMob).second;
					if( pMob->m_nMobNum == 57 ) // 경비 소장
					{
						// IDS_TUTORIAL_TALK15 : 캐릭터 정보창을 열어보게. 화면우측 상단의 버튼이나 'c'키를 누르면 확인 할 수 있다.
						sprintf( strTemp, (char *)G_STRING(IDS_TUTORIAL_TALK15) );

						pMob->SetMobChatMsg( strTemp );
						break;
					}
				}
			}
		}

		break;

	case 1: // 인벤.

		if( g_pNk2DFrame->IsInvenVisible() )
		{
			CNkMob *pMob = NULL;
			LHMobMap_it prMob;
			for (prMob = pCMyApp->m_pUIMgr->m_mob_map.begin(); prMob != pCMyApp->m_pUIMgr->m_mob_map.end(); prMob++)
			{
				pMob = (*prMob).second;
				if( pMob->m_nMobNum == 57 ) // 경비 소장
				{
					// IDS_TUTORIAL_TALK16 : 자신의 착용장비나 소유 물품을 확인할 때 사용하는 버튼이다.
					sprintf( strTemp, (char *)G_STRING(IDS_TUTORIAL_TALK16) );

					pMob->SetMobChatMsg( strTemp );
					break;
				}
			}

			++m_nPointPhase;
		}
		else
		{
			// 주기적으로 재촉하자.
			if( timeGetTime()%10000 < 50 )
			{
				CNkMob *pMob = NULL;
				LHMobMap_it prMob;
				for (prMob = pCMyApp->m_pUIMgr->m_mob_map.begin(); prMob != pCMyApp->m_pUIMgr->m_mob_map.end(); prMob++)
				{
					pMob = (*prMob).second;
					if( pMob->m_nMobNum == 57 ) // 경비 소장
					{
						// IDS_TUTORIAL_TALK17 : 인벤토리 정보창을 열어보게. 화면우측 상단의 버튼이나 'i'키를 누르면 확인 할 수 있다.
						sprintf( strTemp, (char *)G_STRING(IDS_TUTORIAL_TALK17) );

						pMob->SetMobChatMsg( strTemp );
						break;
					}
				}
			}
		}

		break;

	case 2: // 스킬.

		if( g_pNk2DFrame->IsSkillVisible() )
		{
			CNkMob *pMob = NULL;
			LHMobMap_it prMob;
			for (prMob = pCMyApp->m_pUIMgr->m_mob_map.begin(); prMob != pCMyApp->m_pUIMgr->m_mob_map.end(); prMob++)
			{
				pMob = (*prMob).second;
				if( pMob->m_nMobNum == 57 ) // 경비 소장
				{
					// IDS_TUTORIAL_TALK18 : 캐릭터의 스킬 레벨을 관리할 수 있지. 다음 창은 애완동물정보를 나타내는 창이지만 지금은 애완동물이 없으니 넘어가겠다.
					sprintf( strTemp, (char *)G_STRING(IDS_TUTORIAL_TALK18) );

					pMob->SetMobChatMsg( strTemp );
					break;
				}
			}

			++m_nPointPhase;
		}
		else
		{
			// 주기적으로 재촉하자.
			if( timeGetTime()%10000 < 50 )
			{
				CNkMob *pMob = NULL;
				LHMobMap_it prMob;
				for (prMob = pCMyApp->m_pUIMgr->m_mob_map.begin(); prMob != pCMyApp->m_pUIMgr->m_mob_map.end(); prMob++)
				{
					pMob = (*prMob).second;
					if( pMob->m_nMobNum == 57 ) // 경비 소장
					{
						// IDS_TUTORIAL_TALK19 : 스킬 정보창을 열어보게. 화면우측 상단의 버튼이나 't'키를 누르면 확인 할 수 있다.
						sprintf( strTemp, (char *)G_STRING(IDS_TUTORIAL_TALK19) );

						pMob->SetMobChatMsg( strTemp );
						break;
					}
				}
			}
		}

		break;

	case 3: // 애완동물
		++m_nPointPhase; // 라비용은 없어서 못여니 넘기자.

		break;

	case 4: // 맵

		if( g_pNk2DFrame->IsInterfaceVisible(MAP) )
		{
			CNkMob *pMob = NULL;
			LHMobMap_it prMob;
			for (prMob = pCMyApp->m_pUIMgr->m_mob_map.begin(); prMob != pCMyApp->m_pUIMgr->m_mob_map.end(); prMob++)
			{
				pMob = (*prMob).second;
				if( pMob->m_nMobNum == 57 ) // 경비 소장
				{
					// IDS_TUTORIAL_TALK20 : 현재 자신의 위치를 확인할 때 사용하는 버튼이다.
					sprintf( strTemp, (char *)G_STRING(IDS_TUTORIAL_TALK20) );

					pMob->SetMobChatMsg( strTemp );
					break;
				}
			}

			++m_nPointPhase;
		}
		else
		{
			// 주기적으로 재촉하자.
			if( timeGetTime()%10000 < 50 )
			{
				CNkMob *pMob = NULL;
				LHMobMap_it prMob;
				for (prMob = pCMyApp->m_pUIMgr->m_mob_map.begin(); prMob != pCMyApp->m_pUIMgr->m_mob_map.end(); prMob++)
				{
					pMob = (*prMob).second;
					if( pMob->m_nMobNum == 57 ) // 경비 소장
					{
						// IDS_TUTORIAL_TALK21 : 미니맵 정보창을 열어보게. 화면우측 상단의 버튼이나 'm'키를 누르면 확인 할 수 있다.
						sprintf( strTemp, (char *)G_STRING(IDS_TUTORIAL_TALK21) );

						pMob->SetMobChatMsg( strTemp );
						break;
					}
				}
			}
		}

		break;

	case 5: // 도움말

		if( g_pNk2DFrame->IsHelpVisible() )
		{
			CNkMob *pMob = NULL;
			LHMobMap_it prMob;
			for (prMob = pCMyApp->m_pUIMgr->m_mob_map.begin(); prMob != pCMyApp->m_pUIMgr->m_mob_map.end(); prMob++)
			{
				pMob = (*prMob).second;
				if( pMob->m_nMobNum == 57 ) // 경비 소장
				{
					// IDS_TUTORIAL_TALK22 : 간략한 도움말을 확인할 때사용하는 버튼이다.
					sprintf( strTemp, (char *)G_STRING(IDS_TUTORIAL_TALK22) );

					pMob->SetMobChatMsg( strTemp );
					break;
				}
			}

			++m_nPointPhase;
		}
		else
		{
			// 주기적으로 재촉하자.
			if( timeGetTime()%10000 < 50 )
			{
				CNkMob *pMob = NULL;
				LHMobMap_it prMob;
				for (prMob = pCMyApp->m_pUIMgr->m_mob_map.begin(); prMob != pCMyApp->m_pUIMgr->m_mob_map.end(); prMob++)
				{
					pMob = (*prMob).second;
					if( pMob->m_nMobNum == 57 ) // 경비 소장
					{
						// IDS_TUTORIAL_TALK23 : 도움말 정보창을 열어보게. 화면우측 상단의 버튼이나 'F1'키를 누르면 확인 할 수 있다.
						sprintf( strTemp, (char *)G_STRING(IDS_TUTORIAL_TALK23) );

						pMob->SetMobChatMsg( strTemp );
						break;
					}
				}
			}
		}

		break;

	case 6: // 파티

		if( g_pNk2DFrame->IsPartyVisible() )
		{
			CNkMob *pMob = NULL;
			LHMobMap_it prMob;
			for (prMob = pCMyApp->m_pUIMgr->m_mob_map.begin(); prMob != pCMyApp->m_pUIMgr->m_mob_map.end(); prMob++)
			{
				pMob = (*prMob).second;
				if( pMob->m_nMobNum == 57 ) // 경비 소장
				{
					// IDS_TUTORIAL_TALK24 : 자신이 속한 파티의 정보를 확인할때 사용하는 버튼이다.
					sprintf( strTemp, (char *)G_STRING(IDS_TUTORIAL_TALK24) );

					pMob->SetMobChatMsg( strTemp );
					break;
				}
			}

			++m_nPointPhase;

			//  [4/30/2009 ppmmjj83] 일본은 후견인을 막아놔서 튜토리얼두 그냥 넘긴다
			if( g_dwClientCountry == CTRY_JPN ) ++m_nPointPhase;
		}
		else
		{
			// 주기적으로 재촉하자.
			if( timeGetTime()%10000 < 50 )
			{
				CNkMob *pMob = NULL;
				LHMobMap_it prMob;
				for (prMob = pCMyApp->m_pUIMgr->m_mob_map.begin(); prMob != pCMyApp->m_pUIMgr->m_mob_map.end(); prMob++)
				{
					pMob = (*prMob).second;
					if( pMob->m_nMobNum == 57 ) // 경비 소장
					{
						// IDS_TUTORIAL_TALK25 : 파티 정보창을 열어보게. 화면우측 상단의 버튼이나 'p'키를 누르면 확인 할 수 있다.
						sprintf( strTemp, (char *)G_STRING(IDS_TUTORIAL_TALK25) );

						pMob->SetMobChatMsg( strTemp );
						break;
					}
				}
			}
		}

		break;

	case 7: // 후견인

		if( g_pNk2DFrame->IsHelperVisible() )
		{
			CNkMob *pMob = NULL;
			LHMobMap_it prMob;
			for (prMob = pCMyApp->m_pUIMgr->m_mob_map.begin(); prMob != pCMyApp->m_pUIMgr->m_mob_map.end(); prMob++)
			{
				pMob = (*prMob).second;
				if( pMob->m_nMobNum == 57 ) // 경비 소장
				{
					// IDS_TUTORIAL_TALK26 : 자신의 후견인 정보를 확안할 때 사용하는 버튼이다.
					sprintf( strTemp, (char *)G_STRING(IDS_TUTORIAL_TALK26) );

					pMob->SetMobChatMsg( strTemp );
					break;
				}
			}

			if( g_SvrType != ST_ADULT_ONLY ) // 현재 본 서버는 길드정보 아이콘이 없다. 후견인창에서 끝내준다.
				// IDS_TUTORIAL_TALK27 : 후견인 창을 닫으시면 다음 단계로 진행합니다.
				g_pNk2DFrame->SetInfoBox( (char*)G_STRING(IDS_TUTORIAL_TALK27) );

			++m_nPointPhase;
		}
		else
		{
			// 주기적으로 재촉하자.
			if( timeGetTime()%10000 < 50 )
			{
				CNkMob *pMob = NULL;
				LHMobMap_it prMob;
				for (prMob = pCMyApp->m_pUIMgr->m_mob_map.begin(); prMob != pCMyApp->m_pUIMgr->m_mob_map.end(); prMob++)
				{
					pMob = (*prMob).second;
					if( pMob->m_nMobNum == 57 ) // 경비 소장
					{
						// IDS_TUTORIAL_TALK27 : 후견인 정보창을 열어보게. 화면우측 상단의 버튼이나 'h'키를 누르면 확인 할 수 있다.
						sprintf( strTemp, (char *)G_STRING(IDS_TUTORIAL_TALK27) );

						pMob->SetMobChatMsg( strTemp );
						break;
					}
				}
			}
		}

		break;

	case 8: // 길드 정보

		if( g_SvrType == ST_ADULT_ONLY )
		{
			if( g_pNk2DFrame->IsControlGuild() )
			{
				CNkMob *pMob = NULL;
				LHMobMap_it prMob;
				for (prMob = pCMyApp->m_pUIMgr->m_mob_map.begin(); prMob != pCMyApp->m_pUIMgr->m_mob_map.end(); prMob++)
				{
					pMob = (*prMob).second;
					if( pMob->m_nMobNum == 57 ) // 경비 소장
					{
						// IDS_TUTORIAL_TALK28 : 자신의 길드 정보를 확안할 때 사용하는 버튼이다.
						sprintf( strTemp, (char *)G_STRING(IDS_TUTORIAL_TALK28) );

						pMob->SetMobChatMsg( strTemp );
						break;
					}
				}

				// IDS_TUTORIAL_TALK29 : 길드 정보 창을 닫으시면 다음 단계로 진행합니다.
				g_pNk2DFrame->SetInfoBox( (char*)G_STRING(IDS_TUTORIAL_TALK29) );

				++m_nPointPhase;
			}
			else
			{
				// 주기적으로 재촉하자.
				if( timeGetTime()%10000 < 50 )
				{
					CNkMob *pMob = NULL;
					LHMobMap_it prMob;
					for (prMob = pCMyApp->m_pUIMgr->m_mob_map.begin(); prMob != pCMyApp->m_pUIMgr->m_mob_map.end(); prMob++)
					{
						pMob = (*prMob).second;
						if( pMob->m_nMobNum == 57 ) // 경비 소장
						{
							// IDS_TUTORIAL_TALK30 : 길드 정보창을 열어보게. 화면우측 상단의 버튼이나 'g'키를 누르면 확인 할 수 있다.
							sprintf( strTemp, (char *)G_STRING(IDS_TUTORIAL_TALK30) );

							pMob->SetMobChatMsg( strTemp );
							break;
						}
					}
				}
			}
		}
		else // 신규 서버만 길드까지...
		{
			++m_nPointPhase; // 바로 넘긴다.
		}

		break;

	case TUTORIAL_TOPMENU_PHASE_NUM:

		if( g_SvrType == ST_ADULT_ONLY )
		{
			if( !g_pNk2DFrame->IsControlGuild() )
				m_bEndFlag = TRUE;
		}
		else
		{
			if( !g_pNk2DFrame->IsHelperVisible() )
				m_bEndFlag = TRUE;
		}

		break;
	}



	m_dwOldElapsed = Elapsed;

	return TRUE;
}

int  CTutorialTopMenuAct::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	return 0;
}

bool CTutorialTopMenuAct::EndCheck()
{
	if( m_bEndFlag )
	{
		return TRUE;
	}

	return FALSE;
}



// 튜토리얼 파티신청 체험.
CTutorialPartyAct::CTutorialPartyAct()
{
	m_dwOldElapsed = 0;
	m_dwStartTick = timeGetTime();

	m_bEndFlag = FALSE;

	m_nPointPhase = 0;
	m_dwEndFlagTick = 0;

	g_pNk2DFrame->CloseAllWindow();

//	m_pTeacher = NULL; // 교관 포인터를 가지고 있는다.
	CNkMob *pMob = NULL;
	LHMobMap_it prMob;
	for (prMob = pCMyApp->m_pUIMgr->m_mob_map.begin(); prMob != pCMyApp->m_pUIMgr->m_mob_map.end(); prMob++)
	{
		pMob = (*prMob).second;
		if( pMob->m_nMobNum == 57 ) // 경비 소장
			break;
	}

	// 경비소장의 더미 캐릭터 객체를 만들어준다.(그리지 않으므로 데이터만 존재한다.)
	if( pMob )
	{
		pCMyApp->m_pUIMgr->AddPlayer( pMob->m_nVirId, pMob->GetMobName(),0,0, FALSE, pMob->m_wx, pMob->m_wz,pMob->m_dir );
	}
}

CTutorialPartyAct::~CTutorialPartyAct()
{
	// 경비소장의 더미 캐릭터 객체를 지운다.
	CNkMob *pMob = NULL;
	LHMobMap_it prMob;
	for (prMob = pCMyApp->m_pUIMgr->m_mob_map.begin(); prMob != pCMyApp->m_pUIMgr->m_mob_map.end(); prMob++)
	{
		pMob = (*prMob).second;
		if( pMob->m_nMobNum == 57 ) // 경비 소장
			break;
	}
	if( pMob )
	{
		pCMyApp->m_pUIMgr->DeletePlayer( pMob->m_nVirId ); // 다음 액션에서도 더미 캐릭터가 필요하지만 지우고 다시 만들어준다. 구분을 확실히 하기 위해
	}
}

bool CTutorialPartyAct::Render()
{


	if( !GET_D3DDEVICE() )
		return FALSE;

//	CNkMob* pMob;
	float fx = 0.0f;
	float fz = 0.0f;
	float dir = 0.0f;
	float dist = 0.0f;
	float fTemp = 0.0f;
	char strTemp[1024];


	DWORD Elapsed = timeGetTime() - m_dwStartTick;
	DWORD BeforeElapsed = Elapsed - m_dwOldElapsed;

	CNkMob *pTemp = NULL;
	CNkMob *pMob = NULL;
	LHMobMap_it prMob;
	for (prMob = pCMyApp->m_pUIMgr->m_mob_map.begin(); prMob != pCMyApp->m_pUIMgr->m_mob_map.end(); prMob++)
	{
		pTemp = (*prMob).second;
		if( pTemp->m_nMobNum == 57 ) // 경비 소장
		{
			pMob = pTemp;
			break;
		}
	}

	if( m_nPointPhase == 0 ) // 신청 전.
	{
		// 주기적으로 재촉하자.
		if( timeGetTime()%10000 < 50 )
		{
			if( pMob )
			{
				// IDS_TUTORIAL_TALK31 : 나에게 파티를 신청해 보거라.키보드 오른쪽 Shift 키를 누른 상태에서 나를 클릭해야 신청 버튼이 나온다.
				sprintf( strTemp, (char *)G_STRING(IDS_TUTORIAL_TALK31) );

				pMob->SetMobChatMsg( strTemp );
			}
		}
	}
	else // 신청 후. 미니맵 종료.
	{
		if( pMob ) // 바닥에 파티원 마트 표시.
		{
			g_pShadowRect->SetTileCoord(D3DVECTOR(pMob->m_wx, pMob->m_wy, pMob->m_wz), 20, FALSE);
			g_pShadowRect->Render(g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_PARTY_TEX ] ), FALSE);
		}

		if( !g_pNk2DFrame->IsInterfaceVisible(MAP) && !m_dwEndFlagTick ) // m_dwEndFlagTick 을 미니맵을 한번 열었나 플래그로 사용.
		{
			if( pMob )
			{
				// IDS_TUTORIAL_TALK32 : 미니맵 인터페이스를 열고 파티 표시를 확인해 봐라.
				sprintf( strTemp, (char *)G_STRING(IDS_TUTORIAL_TALK32) );

				pMob->SetMobChatMsg( strTemp );
			}
		}


		// 종료 조건 체크.
		if( g_pNk2DFrame->IsInterfaceVisible(MAP) && !m_dwEndFlagTick ) // m_dwEndFlagTick 을 미니맵을 한번 열었나 플래그로 사용.
		{
			m_dwEndFlagTick = timeGetTime();
			// IDS_TUTORIAL_TALK33 : 미니맵 인터페이스를 닫으면 다음 단계로 진행합니다.
			g_pNk2DFrame->SetInfoBox( (char*)G_STRING(IDS_TUTORIAL_TALK33) );

			if( pMob )
			{
				// IDS_TUTORIAL_TALK34 : 미니맵에 반짝이는 하늘색 점이 나의 위치를 나타낸다.
				sprintf( strTemp, (char*)G_STRING(IDS_TUTORIAL_TALK34) );

				pMob->SetMobChatMsg( strTemp );
			}
		}

		if( m_dwEndFlagTick && !g_pNk2DFrame->IsInterfaceVisible(MAP) ) // 봤다 닫은 경우.
		{
			m_bEndFlag = TRUE;
		}
	}




	m_dwOldElapsed = Elapsed;

	return TRUE;
}

int  CTutorialPartyAct::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
//	CNkMob* pMob;
	int x,y;
	char strTemp[1024];

	if( !g_pNk2DFrame )
		return 0;

	if( !g_pNk2DFrame->GetRShiftMenu() )
		return 0;

	switch( msg )
	{
	case WM_LBUTTONDOWN:

		x = LOWORD (lParam);
		y = HIWORD (lParam);

		if( m_nPointPhase == 0 && (GetKeyState(VK_SHIFT) & 0x80) && pCMyApp->m_pUIMgr->m_whichShift == 1 ) // 신청전 상황. 시프트 눌림 체크.
		{
			//------------------------------------------------------------------------
			// 전체 몹들 중에서 클릭된 몹을 찾아서 처리.
			//------------------------------------------------------------------------
			CNkMob *pMob = NULL;
			LHMobMap_it prMob;

			for (prMob = pCMyApp->m_pUIMgr->m_mob_map.begin(); prMob != pCMyApp->m_pUIMgr->m_mob_map.end(); prMob++)
			{
				pMob = (*prMob).second;
				// Color Pickng Buffer 와 몹의 Color 가 동일하다면 ( 선택되었다면 )
				if (pMob->m_wColorIdx == pCMyApp->m_pUIMgr->GetPickColor() )
				{
					// NPC 가 클릭 되었다면 return 1;
					if( pMob->m_nMobNum == 57 ) // 경비 소장 클릭시.
					{
						CNkCharacter *pCha = new CNkCharacter(); // 임의로 더미 캐릭터를 만든다

						if( pCha )
						{
							// RShiftMenu를 여는데 필요한 정보만 셋팅해준다.
							pCha->SetInitPos(pMob->m_wx, pMob->m_wz);
							pCha->m_Race = RACE_BULKAN; // 임의로 불칸
							pCha->SetInfo( pMob->m_nVirId, pMob->GetMobName() );

							g_pNk2DFrame->ShowRShiftMenu( pCha );

							SAFE_DELETE( pCha );
						}
					}

					return 1; // 클릭후 동작안하도록.
				}
			}
		}

		break;

	case WM_LBUTTONUP:

		x = LOWORD (lParam);
		y = HIWORD (lParam);

		if( m_nPointPhase == 0 ) // 신청전 상황.
		{
			if( g_pNk2DFrame->IsRShiftVisible() && g_pNk2DFrame->GetRShiftMenu()->IsInside(x,y) ) // 혹시 신청창이 보이는 상태라면..
			{
				if( g_pNk2DFrame->GetRShiftMenu()->m_pBtn[CRShiftMenu::eParty] && g_pNk2DFrame->GetRShiftMenu()->m_pBtn[CRShiftMenu::eParty]->IsInside(x,y) ) // 버튼2에서 땠으면...
				{
					++m_nPointPhase; // 파티 신청이 된 상태로 바꿔준다.
					g_pNk2DFrame->CloseAllWindow(); // 예외적으로 한번 다 꺼주자.

					// 파티 처리
					g_pNk2DFrame->GetControlParty()->SetParty( TRUE );
					g_pRoh->m_bIsParty = TRUE;

					// 자기 자신도 멤버로 추가해줘야한다.
					g_pNk2DFrame->GetControlParty()->AddMember( g_pRoh->m_nCharIndex, g_pRoh->m_pstrName );

					CNkMob *pMob = NULL;
					LHMobMap_it prMob;
					for (prMob = pCMyApp->m_pUIMgr->m_mob_map.begin(); prMob != pCMyApp->m_pUIMgr->m_mob_map.end(); prMob++)
					{
						pMob = (*prMob).second;
						if( pMob->m_nMobNum == 57 ) // 경비 소장
							break;
					}

					if( pMob )
					{
						g_pNk2DFrame->GetControlParty()->AddMember( pMob->m_nVirId, pMob->GetMobName() );

						sprintf( strTemp, "%d 100 %d %d\n", pMob->m_nVirId, (int)pMob->m_wx, (int)pMob->m_wz  );
						g_pRoh->GetPvInfo( strTemp ); // 미니맵 위치표시용.
					}

					return 1; // 메시지 끊기
				}
				g_pNk2DFrame->CloseAllWindow(); // RShiftMenu 가 보이는 상태에서 마우스 클릭했으면 RShiftMenu를 닫아준다.
			}
		}

		break;
	}

	return 0;
}

bool CTutorialPartyAct::EndCheck()
{
	if( m_bEndFlag )
	{
		return TRUE;
	}

	return FALSE;
}




// 튜토리얼 교환 체험.
CTutorialExchangeAct::CTutorialExchangeAct()
{
	m_dwOldElapsed = 0;
	m_dwStartTick = timeGetTime();

	m_bEndFlag = FALSE;

	m_nPointPhase = 0;

	g_pNk2DFrame->CloseAllWindow();

//	m_pTeacher = NULL; // 교관 포인터를 가지고 있는다.
	CNkMob *pMob = NULL;
	LHMobMap_it prMob;
	for (prMob = pCMyApp->m_pUIMgr->m_mob_map.begin(); prMob != pCMyApp->m_pUIMgr->m_mob_map.end(); prMob++)
	{
		pMob = (*prMob).second;
		if( pMob->m_nMobNum == 57 ) // 경비 소장
			break;
	}

	// 경비소장의 더미 캐릭터 객체를 만들어준다.(그리지 않으므로 데이터만 존재한다.)
	if( pMob )
	{
		pCMyApp->m_pUIMgr->AddPlayer( pMob->m_nVirId, pMob->GetMobName(),0,0, FALSE, pMob->m_wx, pMob->m_wz,	pMob->m_dir );
	}
}

CTutorialExchangeAct::~CTutorialExchangeAct()
{
	// 경비소장의 더미 캐릭터 객체를 지운다.
	CNkMob *pMob = NULL;
	LHMobMap_it prMob;
	for (prMob = pCMyApp->m_pUIMgr->m_mob_map.begin(); prMob != pCMyApp->m_pUIMgr->m_mob_map.end(); prMob++)
	{
		pMob = (*prMob).second;
		if( pMob->m_nMobNum == 57 ) // 경비 소장
			break;
	}

	if( pMob )
	{
		pCMyApp->m_pUIMgr->DeletePlayer( pMob->m_nVirId ); // 다음 액션에서도 더미 캐릭터가 필요하지만 지우고 다시 만들어준다. 구분을 확실히 하기 위해
	}
}

bool CTutorialExchangeAct::Render()
{


	if( !GET_D3DDEVICE() )
		return FALSE;

//	CNkMob* pMob;
	float fx = 0.0f;
	float fz = 0.0f;
	float dir = 0.0f;
	float dist = 0.0f;
	float fTemp = 0.0f;
	char strTemp[1024];

	DWORD Elapsed = timeGetTime() - m_dwStartTick;
	DWORD BeforeElapsed = Elapsed - m_dwOldElapsed;


	if( m_dwOldElapsed < TUTORIAL_POSIONBUY_TIMEFLAG_SHOPAPPEAR && Elapsed >= TUTORIAL_POSIONBUY_TIMEFLAG_SHOPAPPEAR )
	{
		if (g_pTcpIp)
			g_pTcpIp->SendNetMessage("tutorial 12 -1\n"); // 마법상인 등장 요청
	}


	if( m_nPointPhase == 0 ) // 고기 구입 전.
	{
		// 주기적으로 재촉하자.
		if( timeGetTime()%10000 < 50 )
		{
			CNkMob *pMob = NULL;
			LHMobMap_it prMob;
			for (prMob = pCMyApp->m_pUIMgr->m_mob_map.begin(); prMob != pCMyApp->m_pUIMgr->m_mob_map.end(); prMob++)
			{
				pMob = (*prMob).second;
				if( pMob->m_nMobNum == 57 ) // 경비 소장
				{
					// IDS_TUTORIAL_TALK35 : 상인에게서 늑대고기를 사와야 한다.
					sprintf( strTemp, (char *)G_STRING(IDS_TUTORIAL_TALK35) );

					pMob->SetMobChatMsg( strTemp );
					break;
				}
			}
		}
	}
	else if( m_nPointPhase == 1 ) // 신청전
	{
		// 주기적으로 재촉하자.
		if( timeGetTime()%10000 < 50 )
		{
			CNkMob *pMob = NULL;
			LHMobMap_it prMob;
			for (prMob = pCMyApp->m_pUIMgr->m_mob_map.begin(); prMob != pCMyApp->m_pUIMgr->m_mob_map.end(); prMob++)
			{
				pMob = (*prMob).second;
				if( pMob->m_nMobNum == 57 ) // 경비 소장
				{
					// IDS_TUTORIAL_TALK36 :나에게 교환을 신청해 보거라.키보드 오른쪽 Shift 키를 누른 상태에서 나를 클릭하면 신청 버튼이 나온다.
					sprintf( strTemp, (char *)G_STRING(IDS_TUTORIAL_TALK36) );

					pMob->SetMobChatMsg( strTemp );
					break;
				}
			}
		}
	}
	else if( m_nPointPhase == 2 ) // 신청 후
	{
		// 종료 조건 체크.
		if( !g_pNk2DFrame->IsExchangeVisible() ) // 만약 교환창을 다시 꺼버렸다면...
		{
			m_nPointPhase = 1; // 다시 교환신청을 재촉하도록.
		}
	}
	/*
		else // 제대로 교환 완료.
		{
			if( !g_pNk2DFrame->IsExchangeVisible() ) // 만약 교환창을 끄면 종료.
			{
				m_bEndFlag = TRUE;
			}
		}
	*/


	m_dwOldElapsed = Elapsed;

	return TRUE;
}

int  CTutorialExchangeAct::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	CNkMob *pMob = NULL;
	LHMobMap_it prMob;
	int x,y;
	char strTemp[1024];

	if( !g_pNk2DFrame )
		return 0;

	if( !g_pNk2DFrame->GetRShiftMenu() )
		return 0;

	switch( msg )
	{
	case WM_LBUTTONDOWN:

		x = LOWORD (lParam);
		y = HIWORD (lParam);

		if( m_nPointPhase == 0 ) // 늑대고기 사기전
		{
			//------------------------------------------------------------------------
			// 전체 몹들 중에서 클릭된 몹을 찾아서 처리.
			//------------------------------------------------------------------------
			for (prMob = pCMyApp->m_pUIMgr->m_mob_map.begin(); prMob != pCMyApp->m_pUIMgr->m_mob_map.end(); prMob++)
			{
				pMob = (*prMob).second;
				// Color Pickng Buffer 와 몹의 Color 가 동일하다면 ( 선택되었다면 )
				if (pMob->m_wColorIdx == pCMyApp->m_pUIMgr->GetPickColor() )
				{
					if( pMob->m_nMobNum == 57 ) // 교관 NPC 클릭시.
					{
						// 인벤상에 물약이 있는지 체크한다.

						if( g_pRoh->FindItemByVnumFromAll(142) ) // 늑대고기가 있는지 찾는다
						{
							++m_nPointPhase;
						}
						else // 만약 늑대고기를 안가지고 클릭했으면.
						{
							// IDS_TUTORIAL_TALK37 : '늑대고기'를 가지고 있어야 한다. 다시 가서 사오도록
							sprintf( strTemp, (char *)G_STRING(IDS_TUTORIAL_TALK37) );
							pMob->SetMobChatMsg( strTemp );

							if (g_pTcpIp)
								g_pTcpIp->SendNetMessage("tutorial 12 0\n"); // 구입 실패 초기화 요청
						}
					}
					break;
				}
			}
		}

		// else 가 아닌 이유는 위의 체크를 거치고 바로 밑을 체크하기 위해서.

		if( m_nPointPhase == 1 && (GetKeyState(VK_SHIFT) & 0x80) && pCMyApp->m_pUIMgr->m_whichShift == 1 ) // 신청전 상황. 시프트 눌림 체크.
		{
			//------------------------------------------------------------------------
			// 전체 몹들 중에서 클릭된 몹을 찾아서 처리.
			//------------------------------------------------------------------------
			CNkMob *pMob = NULL;
			LHMobMap_it prMob;

			for (prMob = pCMyApp->m_pUIMgr->m_mob_map.begin(); prMob != pCMyApp->m_pUIMgr->m_mob_map.end(); prMob++)
			{
				pMob = (*prMob).second;
				// Color Pickng Buffer 와 몹의 Color 가 동일하다면 ( 선택되었다면 )
				if (pMob->m_wColorIdx == pCMyApp->m_pUIMgr->GetPickColor() )
				{
					// NPC 가 클릭 되었다면 return 1;
					if( pMob->m_nMobNum == 57 ) // 경비 소장 클릭시.
					{
						CNkCharacter *pCha = new CNkCharacter(); // 임의로 더미 캐릭터를 만든다

						if( pCha )
						{
							// RShiftMenu를 여는데 필요한 정보만 셋팅해준다.
							pCha->SetInitPos(pMob->m_wx, pMob->m_wz);
							pCha->m_Race = RACE_BULKAN; // 임의로 불칸
							pCha->SetInfo( pMob->m_nVirId, pMob->GetMobName() );

							g_pNk2DFrame->ShowRShiftMenu( pCha );

							SAFE_DELETE( pCha );
						}
					}

					return 1; // 클릭후 동작안하도록.
				}
			}
		}

		break;

	case WM_LBUTTONUP:

		x = LOWORD (lParam);
		y = HIWORD (lParam);

		if( m_nPointPhase == 1 ) // 신청전 상황.
		{
			if( g_pNk2DFrame->IsRShiftVisible() && g_pNk2DFrame->GetRShiftMenu()->IsInside(x,y) ) // 혹시 신청창이 보이는 상태라면..
			{
				if( g_pNk2DFrame->GetRShiftMenu()->m_pBtn[CRShiftMenu::eExchange] && g_pNk2DFrame->GetRShiftMenu()->m_pBtn[CRShiftMenu::eExchange]->IsInside(x,y) ) // 버튼2에서 땠으면...
				{
					++m_nPointPhase; // 신청이 된 상태로 바꿔준다.

					// 교환창 오픈 처리.
					g_pNk2DFrame->ShowExchangeWindow( TRUE, TRUE );

					CNkMob *pMob = NULL;
					LHMobMap_it prMob;

					for (prMob = pCMyApp->m_pUIMgr->m_mob_map.begin(); prMob != pCMyApp->m_pUIMgr->m_mob_map.end(); prMob++)
					{
						pMob = (*prMob).second;
						if( pMob->m_nMobNum == 57 ) // 경비 소장 클릭시.
						{
							g_pNk2DFrame->GetControlExchange()->SetOppName( pMob->GetMobName() );
							g_pNk2DFrame->GetControlExchange()->SetOppLevel( 500 );
							g_pNk2DFrame->GetControlExchange()->ExchTrade();
						}
					}

					return 1;
				}

				g_pNk2DFrame->CloseAllWindow(); // RShiftMenu 가 보이는 상태에서 마우스 클릭했으면 RShiftMenu를 닫아준다.

			}
		}
		else if( m_nPointPhase == 2 ) // 교환창이 열린상황.
		{
			if( g_pNk2DFrame->IsExchangeVisible() && g_pNk2DFrame->GetControlExchange()->IsInside(x,y) ) // 혹시 신청창이 보이는 상태라면..
			{
				if( g_pNk2DFrame->GetControlExchange()->m_ExgBtn2.IsInside(x,y) ) // 버튼2에서 땠으면...
				{
					// 교환창에서 늑대고기를 찾는다.
					CItem *pItem = g_pRoh->m_ExgInven;
					while( pItem )
					{
						if( pItem->m_Vnum == 142 )
						{
							m_bEndFlag = TRUE;
							break;

						}
						else
							pItem = pItem->m_Next;
					}

					g_pNk2DFrame->GetControlExchange()->m_bTrade = TRUE; // 강제로 교환이 가능하도록..
					g_pNk2DFrame->GetControlExchange()->ExchChange(); // 아이템 리스트를 바꿔치기 한다.
				}
			}
		}

		break;
	}

	return 0;
}

bool CTutorialExchangeAct::EndCheck()
{
	if( m_bEndFlag )
	{
		if (g_pTcpIp)
			g_pTcpIp->SendNetMessage("tutorial 12 1\n"); // 마법상인 제거 요청

		return TRUE;
	}

	return FALSE;
}


// 튜토리얼 평화/배틀모드 체험.
CTutorialPeaceModeAct::CTutorialPeaceModeAct()
{
	m_dwOldElapsed = 0;
	m_dwStartTick = timeGetTime();

	m_nPointPhase = 0; // 변경 누적횟수로 사용.
	m_bPrevMode = g_pRoh->GetCombat();

	m_bEndFlag = FALSE;
}

CTutorialPeaceModeAct::~CTutorialPeaceModeAct()
{
}

bool CTutorialPeaceModeAct::Render()
{
	char strTemp[1024];


	// 주기적으로 재촉하자.
	if( timeGetTime()%10000 < 50 )
	{
		CNkMob *pMob = NULL;
		LHMobMap_it prMob;
		for (prMob = pCMyApp->m_pUIMgr->m_mob_map.begin(); prMob != pCMyApp->m_pUIMgr->m_mob_map.end(); prMob++)
		{
			pMob = (*prMob).second;
			if( pMob->m_nMobNum == 57 ) // 경비 소장
			{
				// IDS_TUTORIAL_TALK38 : 전투모드와 평화모드를 몇번 전환해 보거라.
				sprintf( strTemp, (char *)G_STRING(IDS_TUTORIAL_TALK38) );
				pMob->SetMobChatMsg( strTemp );
				break;
			}
		}
	}

	if( m_bPrevMode != g_pRoh->GetCombat() ) // 혹시 모드가 바뀌었다면..
	{
		++m_nPointPhase; // 누적횟수 증가.

		if( g_pRoh->GetCombat() ) // 바뀌기 전꺼라는걸 주의
			// IDS_TUTORIAL_TALK39 : 바로 그 상태가 전투모드다.
			sprintf( strTemp, (char *)G_STRING(IDS_TUTORIAL_TALK39) );
		else
			// IDS_TUTORIAL_TALK40 : 바로 그 상태가 평화모드다.
			sprintf( strTemp, (char *)G_STRING(IDS_TUTORIAL_TALK40) );

		CNkMob *pMob = NULL;
		LHMobMap_it prMob;
		for (prMob = pCMyApp->m_pUIMgr->m_mob_map.begin(); prMob != pCMyApp->m_pUIMgr->m_mob_map.end(); prMob++)
		{
			pMob = (*prMob).second;
			if( pMob->m_nMobNum == 57 ) // 경비 소장
			{

				pMob->SetMobChatMsg( strTemp );
				break;
			}
		}
	}

	m_bPrevMode = g_pRoh->GetCombat();

	return TRUE;
}

int  CTutorialPeaceModeAct::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	return 0;
}

bool CTutorialPeaceModeAct::EndCheck()
{
	if( m_nPointPhase > 4 ) // 일정수 이상 해봤으면...
	{
		return TRUE;
	}

	return FALSE;
}


// 튜토리얼 종료. (패킷보내기위해...)
CTutorialEndAct::CTutorialEndAct()
{
}

CTutorialEndAct::~CTutorialEndAct()
{
}

bool CTutorialEndAct::Render()
{
	return TRUE;
}

int  CTutorialEndAct::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	return 0;
}

bool CTutorialEndAct::EndCheck()
{
	// 튜토리얼 종료시 파티도 해체해준다.
	if( g_pRoh->m_bIsParty ) // 파티가 있을 경우에만 처리
	{
		g_pNk2DFrame->GetControlParty()->RemoveMemberAll();
		g_pNk2DFrame->GetControlParty()->SetParty( FALSE );
		g_pRoh->m_bIsParty = FALSE;
	}

	if (g_pTcpIp)
		g_pTcpIp->SendNetMessage("tutorial 0\n"); // 튜토리얼 완료패킷

	return TRUE; // 바로 끝나게..
}

/* C&P 용.
// 튜토리얼 포인트 업 체험.
CTutorialPointAct::CTutorialPointAct()
{
	m_dwOldElapsed = 0;
	m_dwStartTick = timeGetTime();

	m_bEndFlag = FALSE;
}

CTutorialPointAct::~CTutorialPointAct()
{
}

bool CTutorialPointAct::Render()
{


	if( !GET_D3DDEVICE() )
		return FALSE;

	CNkMob* pMob;
	float fx = 0.0f;
	float fz = 0.0f;
	float dir = 0.0f;
	float dist = 0.0f;
	float fTemp = 0.0f;


	DWORD Elapsed = timeGetTime() - m_dwStartTick;
	DWORD BeforeElapsed = Elapsed - m_dwOldElapsed;






	m_dwOldElapsed = Elapsed;

	return TRUE;
}

int  CTutorialPointAct::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	int x,y;

 	switch( msg )
	{
	case WM_RBUTTONDOWN:

		x = LOWORD (lParam);
		y = HIWORD (lParam);

		break;
	}

	return 0;
}

bool CTutorialPointAct::EndCheck()
{
	if( m_bEndFlag )
		return TRUE;

  return FALSE;
}
*/