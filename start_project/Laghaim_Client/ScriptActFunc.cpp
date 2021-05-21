#include "stdafx.h"

///Ʃ�丮�� �� �߰�

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








// nate 2004 - 7 �۷ι� ShadowRect���

extern DWORD				g_dwClientCountry;
//extern void Convert640Pos( RECT *Rect ); // 640*480 �ػ� ������ ��ǥ�� ���� �ػ󵵷� �����ý����ش�.

BOOL CActFuncManager::ReadyAction( int ActionNum )
{
	SAFE_DELETE( m_pFunc );

	switch( ActionNum )
	{
	case EVENT_ACTION_TUTORIAL_START: // Ʃ�丮�� ���ۺ�.

		m_pFunc = new CTutorialStartAct();

		break;

	case EVENT_ACTION_TUTORIAL_MOVE: // Ʃ�丮�� �̵� ü��.

		m_pFunc = new CTutorialMoveAct();

		break;

	case EVENT_ACTION_TUTORIAL_CAM: // Ʃ�丮�� ����ȸ�� ü��.

		m_pFunc = new CTutorialCamAct();

		break;

	case EVENT_ACTION_TUTORIAL_HUNT: // Ʃ�丮�� ��� ü��.

		m_pFunc = new CTutorialHuntAct();

		break;

	case EVENT_ACTION_TUTORIAL_POINTUP: // Ʃ�丮�� ����Ʈ �� ü��.

		m_pFunc = new CTutorialPointAct();

		break;

	case EVENT_ACTION_TUTORIAL_POSIONBUY: // Ʃ�丮�� ������ ü��.

		m_pFunc = new CTutorialPosionBuyAct();

		break;

	case EVENT_ACTION_TUTORIAL_POSIONUSE: // Ʃ�丮�� ������ ü��.

		m_pFunc = new CTutorialPosionUseAct();

		break;

	case EVENT_ACTION_TUTORIAL_EQUIPBUY: // Ʃ�丮�� ����� ü��.

		m_pFunc = new CTutorialEquipBuyAct();

		break;

	case EVENT_ACTION_TUTORIAL_EQUIPUSE: // Ʃ�丮�� ������� ü��.

		m_pFunc = new CTutorialEquipUseAct();

		break;

	case EVENT_ACTION_TUTORIAL_TOPMENU: // Ʃ�丮�� �⺻ �޴� ü��.

		m_pFunc = new CTutorialTopMenuAct();

		break;

	case EVENT_ACTION_TUTORIAL_PARTY: // Ʃ�丮�� ��Ƽ ü��.

		m_pFunc = new CTutorialPartyAct();

		break;

	case EVENT_ACTION_TUTORIAL_EXCHANGE: // Ʃ�丮�� ��ȯ ü��.

		m_pFunc = new CTutorialExchangeAct();

		break;

	case EVENT_ACTION_TUTORIAL_PEACEMODE: // Ʃ�丮�� �������/��ȭ��� ü��.

		m_pFunc = new CTutorialPeaceModeAct();

		break;

	case EVENT_ACTION_TUTORIAL_END: // Ʃ�丮�� ����.

		m_pFunc = new CTutorialEndAct();

		break;

	}

	if( !m_pFunc )
		return FALSE;

	return TRUE;
}

void CActFuncManager::ReleaseAction( int ActionNum )
{
// ������ ���� ������ �Ұ� ������ ���� �ȴ�. (�� �༮�� �Ҹ��ڷ� ����� �ȵ����ִ� �Ҹ��ڿ��� �Ұ� ���⼭������)
	switch( ActionNum )
	{
	case EVENT_ACTION_TUTORIAL_POINTUP: // Ʃ�丮�� ����Ʈ ���
	case EVENT_ACTION_TUTORIAL_POSIONUSE: // Ʃ�丮�� ���� ���
	case EVENT_ACTION_TUTORIAL_EQUIPUSE: // Ʃ�丮�� ��� ����
	case EVENT_ACTION_TUTORIAL_TOPMENU: // Ʃ�丮�� �޴��� ü��.
	case EVENT_ACTION_TUTORIAL_EXCHANGE: // Ʃ�丮�� ��ȯ ü��.
		g_pNk2DFrame->CloseAllWindow(); // ����Ʈ â�� �ݾ��ش�.
		break;
	}

	// Ʃ�丮���̶�� �׻� �׼��� �Ѿ�� ������ ��糪 Info�� �����ش�.
	if( ActionNum >= EVENT_ACTION_TUTORIAL_START && ActionNum <= EVENT_ACTION_TUTORIAL_END ) // Ʃ�丮���̶��..
	{
		pCMyApp->StopHero(); // �ѹ� �����ִ°� ����. ���� Ŭ���� ����Ǵ� ��찡 �ֱ⶧����...

		CNkMob *pMob = NULL;
		LHMobMap_it prMob;
		for (prMob = pCMyApp->m_pUIMgr->m_mob_map.begin(); prMob != pCMyApp->m_pUIMgr->m_mob_map.end(); prMob++)
		{
			pMob = (*prMob).second;
			if( pMob->m_nMobNum == 57 ) // ��� ����
			{
				pMob->m_bShowMobChatPopup = FALSE; // ��ǳ�� ��������.
				pMob->m_LastChatTime = timeGetTime() - 8000;

				break;
			}
		}

		g_pNk2DFrame->SetInfoBox( "" ); // �̷��� �ϸ� ��µǴ� ������ ���������� SetInfoBox �Լ��� ��������.
	}

	SAFE_DELETE( m_pFunc );
}



CTutorialStartAct::CTutorialStartAct()
{
	int i = 0;

//	pCMyApp->m_pUIMgr->EraseAllList();

	// ���ΰ� ��ġ �ʱ�ȭ.
	/*  // ���ΰ� ��ġ�� �������� �ش�.
		float fx = 2228.81f;
		float fy = 133.89f;
		float fz = 10412.27f;
		float dir = 0.0f;

		float	fHeight = pCMyApp->GetLandPtr()->GetHFHeight( fx, fz );
		fy = CNKObjQuadNode::GetHeightOnObject( fz, fx, fy );
		if( fy < fHeight )
			fy = fHeight;

		pCMyApp->SetHeroInitPos( fx, fy, fz, 0.0f );	// ī�޶� ������ �ʿ�������.
		g_pRoh->SetCombat( FALSE );
		g_pRoh->SetMotionState(CHA_STAND);
	*/
	// ���ΰ� ���� �����̻������� ���׶����� ���� ����(�������� �ƴ�!)
	g_pRoh->m_dir = 0.0f;
	g_pRoh->m_toDir = 0.0f;

	float fx = g_pRoh->m_wx;
	float fy = g_pRoh->m_wy;
	float fz = g_pRoh->m_wz;
	float dir = 0.0f;

	// ī�޶� �ʱ�ȭ.
	float CamDist = 100.0f;
	float CamEyeHeight = 90.0f;
	float CamAtHeight = 20.0f;

	D3DVECTOR vEye;
	D3DVECTOR vAt;
	vEye.x = fx + ( cos(dir) * CamDist ); // ������ ���� �߻������� ���� �ݴ�� ������.
	vEye.y = fy + CamEyeHeight; // ���� ���̿��� ������ ��.
	vEye.z = fz + ( sin(dir) * CamDist ); // ������ ���� �߻������� ���� �ݴ�� ������.
	vAt.x = fx; // ������ ���� �߻������� ���� �ݴ�� ������.
	vAt.y = fy + CamAtHeight; // ���� ���̿��� ������ ��.
	vAt.z = fz; // ������ ���� �߻������� ���� �ݴ�� ������.

	pCMyApp->SetCameraInitPos( vEye, vAt ); // ���� �����ϱ� �����Ƽ� pCMyApp �� ���� ���Ĺ���. ���� ������ ����ص� ���� ī�޶� ��ǥ�� �ٽ� �ٲ�����Ѵ�.
	D3DMATRIX matProj;

	float Fov = 0.75f;
	float Aspect = 0.75f;
	float zNear = 0.1f;
	float zFar = 10000.0f;
	D3DUtil_SetProjectionMatrix( matProj, Fov, Aspect, zNear, zFar );
	pCMyApp->GetDevice()->SetTransform( D3DTRANSFORMSTATE_PROJECTION, &matProj);
	pCMyApp->GetFrustum()->InitFrustum( Fov, Aspect, zNear, 10000.0f );


	m_dwOldElapsed = 0;
	m_dwStartTick = timeGetTime(); // ������ �ð��� ���.
}

CTutorialStartAct::~CTutorialStartAct()
{
}


#define TUTORIAL_START_WOLF_NUM 30 // ������ ���� ����
#define TUTORIAL_START_TIGER_NUM 20 // ������ ��ȣ ����

#define TUTORIAL_START_TIMEFLAG_FADEEND 2000 // ���̵� �� ������ Ÿ��.
#define TUTORIAL_START_TIMEFLAG_MOTION1 2500 // �θ��� �Ÿ��� �����ϴ� Ÿ��. �ϴ� ����
#define TUTORIAL_START_TIMEFLAG_MOBAPPEAR 4000 // ���� ����
#define TUTORIAL_START_TIMEFLAG_MOBRUN    4500 // ���� �޷���� ������.
#define TUTORIAL_START_TIMEFLAG_MOTION2 7000 // �ѷ����ε� �θ��� �����ϴ� Ÿ��. 
#define TUTORIAL_START_TIMEFLAG_MOBDESTROY    10000 // ���� ���ݴ���.
#define TUTORIAL_START_TIMEFLAG_MOBDESTROY2   11000 // ���� ���ݴ���.
//#define TUTORIAL_START_TIMEFLAG_TEACHERAPPEAR 12000 // ���� ����
//#define TUTORIAL_START_TIMEFLAG_ACTIONEND    17000 // �׼� ����
#define TUTORIAL_START_TIMEFLAG_TEACHERAPPEAR 10500 // ���� ����
#define TUTORIAL_START_TIMEFLAG_ACTIONEND    15500 // �׼� ����


bool CTutorialStartAct::Render()
{
	int i = 0;
	DWORD Elapsed = timeGetTime() - m_dwStartTick;

	// �ӽÿ� ������.
	CNkMob* pMob;
	float fx = 0.0f;
	float fz = 0.0f;
	float dir = 0.0f;
	float dist = 0.0f;
	float fTemp = 0.0f;


	// Ư�� Ÿ�̹��� 1ȸ�� �߻�.
	if( Elapsed < TUTORIAL_START_TIMEFLAG_FADEEND ) // ���� �� ���̵� ��.
	{
		int Alpha = Elapsed * 255 / TUTORIAL_START_TIMEFLAG_FADEEND;
		DWORD color = RGBA_MAKE( 0, 0, 0, Alpha );
		pCMyApp->m_pFxSet->RenderFadeScreen( color );
	}
	if( m_dwOldElapsed < TUTORIAL_START_TIMEFLAG_FADEEND && Elapsed >= TUTORIAL_START_TIMEFLAG_FADEEND )
	{
		/*      // �������� �ֱ�� �ߴ�.
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
	// ó�� �������� ��� �θ���
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
	// �� ���� �� �޷���.
	if( m_dwOldElapsed < TUTORIAL_START_TIMEFLAG_MOBAPPEAR && Elapsed >= TUTORIAL_START_TIMEFLAG_MOBAPPEAR )
	{

		for( i = 0 ; i < TUTORIAL_START_WOLF_NUM ; ++i )
		{
			fTemp = 6.28f / (float)TUTORIAL_START_WOLF_NUM; //����.

			dir = i * fTemp + RandomNum(-0.5f,0.5f);
			dist = 230.0f + RandomNum(-20.0f,30.0f);


			fx = g_pRoh->m_wx + cos(dir) * dist; // ���ΰ����κ��� dir �������� dist ��ŭ ������ �Ÿ�.
			fz = g_pRoh->m_wz + sin(dir) * dist;

			pCMyApp->m_pUIMgr->AddMob( 23, i, fx, fz, dir-3.14f, 100 );
		}

		for( i = 0 ; i < TUTORIAL_START_TIGER_NUM ; ++i )
		{
			fTemp = 6.28f / (float)TUTORIAL_START_TIGER_NUM; //����.

			dir = i * fTemp + RandomNum(-0.7f,0.7f);
			dist = 230.0f + RandomNum(-20.0f,30.0f);


			fx = g_pRoh->m_wx + cos(dir) * dist; // ���ΰ����κ��� dir �������� dist ��ŭ ������ �Ÿ�.
			fz = g_pRoh->m_wz + sin(dir) * dist;

			pCMyApp->m_pUIMgr->AddMob( 24, i+TUTORIAL_START_WOLF_NUM, fx, fz, dir-3.14f, 100 );
		}

	}
	if( m_dwOldElapsed < TUTORIAL_START_TIMEFLAG_MOBRUN && Elapsed >= TUTORIAL_START_TIMEFLAG_MOBRUN ) // �޷�����...
	{
		for( i = 0 ; i < TUTORIAL_START_WOLF_NUM ; ++i )
		{
			if( (pMob = pCMyApp->m_pUIMgr->FindMobById(i)) && pMob->m_nMobNum == 23 )
			{
				dir = pMob->m_dir+3.14f;
				dist = 50.0f + RandomNum(-10.0f,10.0f);

				fx = g_pRoh->m_wx + cos(dir) * dist; // ���ΰ����κ��� dir �������� dist ��ŭ ������ �Ÿ�.
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

				fx = g_pRoh->m_wx + cos(dir) * dist; // ���ΰ����κ��� dir �������� dist ��ŭ ������ �Ÿ�.
				fz = g_pRoh->m_wz + sin(dir) * dist;

				dist = DistPlToPlAbs( fx, fz, pMob->m_wx, pMob->m_wz);

				pMob->Move( fx, fz, (int)(dist/3.0f) );
			}
		}
	}
	/////////////////////////////////////////////////////////////////////////////


	/////////////////////////////////////////////////////////////////////////////
	// ������ �ѷ��׿� �θ���.
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
	// �� ���� TUTORIAL_START_TIMEFLAG_MOBDESTROY
	if( m_dwOldElapsed < TUTORIAL_START_TIMEFLAG_MOBDESTROY && Elapsed >= TUTORIAL_START_TIMEFLAG_MOBDESTROY )
	{
		// �÷��� ���Ǿ� ����ȿ���� �ִ´�.
		for( i = 0 ; i < 20 ; ++i )
		{
			dir = i * 0.628f + RandomNum(-0.5f,0.5f);
			dist = 50.0f + RandomNum(-10.0f,10.0f);

			fx = g_pRoh->m_wx + cos(dir) * dist; // ���ΰ����κ��� dir �������� dist ��ŭ ������ �Ÿ�.
			fz = g_pRoh->m_wz + sin(dir) * dist;

			EffectSort ef_sort;
			ef_sort.nType = FX_METEORITEBOOM;

			ef_sort.vPos.x = fx;
			ef_sort.vPos.y = pCMyApp->GetLandPtr()->GetHFHeight( fx, fz ) + RandomNum(-10.0f,10.0f);
			ef_sort.vPos.z = fz;

			pCMyApp->m_pFxSet->InsertFxEffect(ef_sort);
		}
	}


	if( Elapsed >= TUTORIAL_START_TIMEFLAG_MOBDESTROY && Elapsed <= TUTORIAL_START_TIMEFLAG_MOBDESTROY+500 ) // ī�޶� ���� ȿ��.
	{
		D3DVECTOR vEye;
		D3DVECTOR vAt;
		pCMyApp->GetEyePos( vEye );
		pCMyApp->GetEyeAtPos( vAt );
		int nTemp = (rand()%10)-5; // �����ϰ�..
		vEye.y += (float)nTemp; // ���� ���̿��� ������ ��.
		vAt.y += (float)nTemp; // ���� ���̿��� ������ ��.

		pCMyApp->SetCameraInitPos( vEye, vAt ); // ���� �����ϱ� �����Ƽ� pCMyApp �� ���� ���Ĺ���. ���� ������ ����ص� ���� ī�޶� ��ǥ�� �ٽ� �ٲ�����Ѵ�.
	}
	if( m_dwOldElapsed < TUTORIAL_START_TIMEFLAG_MOBDESTROY+500 && Elapsed >= TUTORIAL_START_TIMEFLAG_MOBDESTROY+500 ) // ī�޶� ���� �ǵ����ֱ�.
	{
		D3DVECTOR vEye;
		D3DVECTOR vAt;
		pCMyApp->GetEyePos( vEye );
		pCMyApp->GetEyeAtPos( vAt );
		vEye.y = (float)g_pRoh->m_wy + 100.0f; // ���� ���̿��� ������ ��.
		vAt.y = (float)g_pRoh->m_wy + 20.0f; // ���� ���̿��� ������ ��.

		pCMyApp->SetCameraInitPos( vEye, vAt ); // ���� �����ϱ� �����Ƽ� pCMyApp �� ���� ���Ĺ���. ���� ������ ����ص� ���� ī�޶� ��ǥ�� �ٽ� �ٲ�����Ѵ�.
	}


	if( m_dwOldElapsed < TUTORIAL_START_TIMEFLAG_MOBDESTROY+500 && Elapsed >= TUTORIAL_START_TIMEFLAG_MOBDESTROY+500 ) // �� ���̤�.
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
//				pMob->SetAttackVital( 50 ); // ��ȣ���� �� ������.
			}
		}

	}
	/*
		// �ѹ��
		if( m_dwOldElapsed < TUTORIAL_START_TIMEFLAG_MOBDESTROY2 && Elapsed >= TUTORIAL_START_TIMEFLAG_MOBDESTROY2 )
		{
			// �÷��� ���Ǿ� ����ȿ���� �ִ´�.
			for( i = 0 ; i < 20 ; ++i )
			{
				dir = i * 0.628f + RandomNum(-0.5f,0.5f);
				dist = 50.0f + RandomNum(-10.0f,10.0f);

				fx = g_pRoh->m_wx + cos(dir) * dist; // ���ΰ����κ��� dir �������� dist ��ŭ ������ �Ÿ�.
				fz = g_pRoh->m_wz + sin(dir) * dist;

				EffectSort ef_sort;
				ef_sort.nType = FX_METEORITEBOOM;
	//			ef_sort.Index = 1; // �ٴ� �������°� ��ǥ���ϵ���...

				ef_sort.vPos.x = fx;
				ef_sort.vPos.y = pCMyApp->GetLandPtr()->GetHFHeight( fx, fz ) + RandomNum(-10.0f,10.0f);
				ef_sort.vPos.z = fz;

				pCMyApp->m_pFxSet->InsertFxEffect(ef_sort);
			}
		}


		if( Elapsed >= TUTORIAL_START_TIMEFLAG_MOBDESTROY2 && Elapsed <= TUTORIAL_START_TIMEFLAG_MOBDESTROY2+500 ) // ī�޶� ���� ȿ��.
		{
			D3DVECTOR vEye;
			D3DVECTOR vAt;
			pCMyApp->GetEyePos( vEye );
			pCMyApp->GetEyeAtPos( vAt );
			int nTemp = (rand()%10)-5; // �����ϰ�..
			vEye.y += (float)nTemp; // ���� ���̿��� ������ ��.
			vAt.y += (float)nTemp; // ���� ���̿��� ������ ��.

			pCMyApp->SetCameraInitPos( vEye, vAt ); // ���� �����ϱ� �����Ƽ� pCMyApp �� ���� ���Ĺ���. ���� ������ ����ص� ���� ī�޶� ��ǥ�� �ٽ� �ٲ�����Ѵ�.
		}
		if( m_dwOldElapsed < TUTORIAL_START_TIMEFLAG_MOBDESTROY2+500 && Elapsed >= TUTORIAL_START_TIMEFLAG_MOBDESTROY2+500 ) // ī�޶� ���� �ǵ����ֱ�.
		{
			D3DVECTOR vEye;
			D3DVECTOR vAt;
			pCMyApp->GetEyePos( vEye );
			pCMyApp->GetEyeAtPos( vAt );
			vEye.y = (float)g_pRoh->m_wy + 100.0f; // ���� ���̿��� ������ ��.
			vAt.y = (float)g_pRoh->m_wy + 20.0f; // ���� ���̿��� ������ ��.

			pCMyApp->SetCameraInitPos( vEye, vAt ); // ���� �����ϱ� �����Ƽ� pCMyApp �� ���� ���Ĺ���. ���� ������ ����ص� ���� ī�޶� ��ǥ�� �ٽ� �ٲ�����Ѵ�.
		}


		if( m_dwOldElapsed < TUTORIAL_START_TIMEFLAG_MOBDESTROY2+500 && Elapsed >= TUTORIAL_START_TIMEFLAG_MOBDESTROY2+500 ) // �� ���̤�.
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
	// ���� ����
	if( m_dwOldElapsed < TUTORIAL_START_TIMEFLAG_TEACHERAPPEAR && Elapsed >= TUTORIAL_START_TIMEFLAG_TEACHERAPPEAR )
	{
		/*
				dir = -1.57f;
				dist = 130.0f;
				fx = g_pRoh->m_wx + cos(dir) * dist; // ���ΰ����κ��� dir �������� dist ��ŭ ������ �Ÿ�.
				fz = g_pRoh->m_wz + sin(dir) * dist;

				pCMyApp->m_pUIMgr->AddMob( 57, 1000, fx, fz, dir-3.14f, 100 ); // ������ �ε����� ���Ƿ� ũ��.

				if( pMob = pCMyApp->m_pUIMgr->FindMobById(1000) )
				{
					dist = 40.0f;
					fx = g_pRoh->m_wx + cos(dir) * dist; // ���ΰ����κ��� dir �������� dist ��ŭ ������ �Ÿ�.
					fz = g_pRoh->m_wz + sin(dir) * dist;

					dist = DistPlToPlAbs( fx, fz, pMob->m_wx, pMob->m_wz);

					pMob->Move( fx, fz, (int)(dist/2.0f) );
				}
		*/
		if (g_pTcpIp)
			g_pTcpIp->SendNetMessage("tutorial 1\n"); // ���� ���� ��û.

	}

	if( m_dwOldElapsed < TUTORIAL_START_TIMEFLAG_TEACHERAPPEAR+3000 && Elapsed >= TUTORIAL_START_TIMEFLAG_TEACHERAPPEAR+3000 )
	{
		// ���ΰ��� ������ �ٶ󺸰�.
		g_pRoh->m_toDir -= 1.57f;

		g_pRoh->SetCombat( FALSE );
		g_pRoh->SetMotionState(CHA_STAND);
	}

	/////////////////////////////////////////////////////////////////////////////


	/*
		// Ư�� Ÿ�̹� ���� ��� ��µǾ��ϴ� �༮��.
		if( Elapsed >= TUTORIAL_START_TIMEFLAG_FADEEND )  // �� �ð� ���ķ� ���ΰ��� ��� ���.
		{
			pCMyApp->DrawPlayer();
		}
	//	if( Elapsed >= TUTORIAL_START_TIMEFLAG_MONAPPEAR )  // �� �ð� ���ķ� ���ΰ��� ��� ���.
		{
			pCMyApp->m_pUIMgr->RenderMobs();
		}
	*/
	m_dwOldElapsed = Elapsed;

	return TRUE;
}

int  CTutorialStartAct::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	return 1; // �� ���� �߿� �ٸ� ������ �ȵǵ���.
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
}; // �̵��� ��ġ ���

bool CTutorialMoveAct::Render()
{


	if( !GET_D3DDEVICE() )
		return FALSE;

	char strTemp[1024];


	DWORD Elapsed = timeGetTime() - m_dwStartTick;
	DWORD BeforeElapsed = Elapsed - m_dwOldElapsed;

	if( m_nPointPhase < TUTORIAL_MOVE_POINT_NUM ) // ����Ʈ �̵� ��.
	{
		DWORD alphaTest, alphaBlend, srcBlendOld, destBlendOld, dwLighting, dwAmbient, dwFogEnable;
		DWORD  dwTextureState[3];
		// ���°� ����.
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


		// ������ ���� üũ.
		float dist = DistPlToPlAbs( (float)MovePoint[m_nPointPhase].x, (float)MovePoint[m_nPointPhase].y
									, g_pRoh->m_wx, g_pRoh->m_wz);

		if( dist < 10.0f ) // �����Ÿ� ������ ��������...
		{
			++m_nPointPhase;

			if( m_nPointPhase >= TUTORIAL_MOVE_POINT_NUM )
			{
				// IDS_TUTORIAL_TALK1 : �׽�Ʈ�� ����ϼ̽��ϴ�. �������� �ٽ� ���� �ɾ��ּ���.
				g_pNk2DFrame->SetInfoBox( (char*)G_STRING(IDS_TUTORIAL_TALK1) );
			}
			else
			{
				// IDS_TUTORIAL_TALK2 : ���� ����Ʈ�� �̵��ϼ���.
				g_pNk2DFrame->SetInfoBox( (char*)G_STRING(IDS_TUTORIAL_TALK2) );
			}

		}

		// �ֱ������� ��������.
		if( timeGetTime()%10000 < 50 )
		{
			CNkMob *pMob = NULL;
			LHMobMap_it prMob;
			for (prMob = pCMyApp->m_pUIMgr->m_mob_map.begin(); prMob != pCMyApp->m_pUIMgr->m_mob_map.end(); prMob++)
			{
				pMob = (*prMob).second;
				if( pMob->m_nMobNum == 57 ) // ��� ����
				{
					// IDS_TUTORIAL_TALK3 : ȭ��ǥ�� �����̴� �������� �̵��ؾ� �Ѵ�.
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

		if( m_nPointPhase >= TUTORIAL_MOVE_POINT_NUM ) // ���� �̵� ��
		{
			//------------------------------------------------------------------------
			// ��ü ���� �߿��� Ŭ���� ���� ã�Ƽ� ó��.
			//------------------------------------------------------------------------
			CNkMob *pMob = NULL;
			LHMobMap_it prMob;

			for (prMob = pCMyApp->m_pUIMgr->m_mob_map.begin(); prMob != pCMyApp->m_pUIMgr->m_mob_map.end(); prMob++)
			{
				pMob = (*prMob).second;
				// Color Pickng Buffer �� ���� Color �� �����ϴٸ� ( ���õǾ��ٸ� )
				if (pMob->m_wColorIdx == pCMyApp->m_pUIMgr->GetPickColor() )
				{
					// NPC �� Ŭ�� �Ǿ��ٸ� return 1;
					if( pMob->m_nMobNum == 57 ) // ��� ���� Ŭ����.
						m_bEndFlag = TRUE;

					return 1; // Ŭ���� ���۾��ϵ���.
				}
			}
		}

		break;
	}

	return 0; // �� ���� �߿� �ٸ� ������ �ǵ���.
}

bool CTutorialMoveAct::EndCheck()
{
	if( m_bEndFlag )
	{
		return TRUE;
	}

	return FALSE;
}


// Ʃ�丮�� ���� ���� ü��.
CTutorialCamAct::CTutorialCamAct()
{
	m_nPointPhase = 0;

	m_dwOldElapsed = 0;
	m_dwStartTick = timeGetTime();

	m_Data1 = 0;

	m_bEndFlag = FALSE;
	/*
		m_pTeacher = NULL; // ���� �����͸� ������ �ִ´�.
		CNkMob *pMob = NULL;
		LHMobMap_it prMob;
		for (prMob = pCMyApp->m_pUIMgr->m_mob_map.begin(); prMob != pCMyApp->m_pUIMgr->m_mob_map.end(); prMob++)
		{
			pMob = (*prMob).second;
			if( pMob->m_nMobNum == 57 ) // ��� ����
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

#define TUTORIAL_CAM_PHASE_NUM 4 // �� �� �� �� ���� �ܾƿ�

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
		// ���°� ����.
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
			if( pMob->m_nMobNum == 57 ) // ��� ����
				break;
		}


		switch( m_nPointPhase )
		{
		case 0: // ��
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
				// IDS_TUTORIAL_TALK4 : ���콺 ������ ��ư�� Ŭ���� ä ���� �������� ���콺�� ��������
				sprintf( strTemp, (char*)G_STRING(IDS_TUTORIAL_TALK4) );
				pMob->SetMobChatMsg( strTemp );
			}


			break;
		case 1: // ��
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
				// IDS_TUTORIAL_TALK5 : ���콺 ������ ��ư�� Ŭ���� ä �Ʒ��� �������� ���콺�� ��������
				sprintf( strTemp, (char*)G_STRING(IDS_TUTORIAL_TALK5) );
				pMob->SetMobChatMsg( strTemp );
			}

			break;
		case 2: // ��
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
				// IDS_TUTORIAL_TALK6 : ���콺 ������ ��ư�� Ŭ���� ä ���� �������� ���콺�� ��������
				sprintf( strTemp, (char*)G_STRING(IDS_TUTORIAL_TALK6) );
				pMob->SetMobChatMsg( strTemp );
			}

			break;

		case 3: // ��
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
				// IDS_TUTORIAL_TALK7 : ���콺 ������ ��ư�� Ŭ���� ä ������ �������� ���콺�� ��������
				sprintf( strTemp, (char*)G_STRING(IDS_TUTORIAL_TALK7) );
				pMob->SetMobChatMsg( strTemp );
			}

			break;

		case 4: // ����
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
				GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_MOBSHOWUP_LASER ] ) ); // �̹����� ���߿� �ٲ���.
			else
				GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_TUTORIAL_ARROW ] ) );

			break;
		case 5: // �ܾƿ�
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

		// ������ ���� üũ
		if( pCMyApp->m_bRotateCamera ) // ī�޶� ȸ�� �߿��� üũ.
		{
			switch( m_nPointPhase )
			{
			case 0: // ��

				if( pCMyApp->m_ptCurPos.y - y > 0 ) // ���ʿ����Ӹ�...
					m_Data1 += pCMyApp->m_ptCurPos.y - y; // ���� �̵����� üũ.

				if( m_Data1 > 30 ) // �������̻� ���ϴ� �������� ����������...
				{
					++m_nPointPhase;
					m_Data1 = 0;
				}

				break;

			case 1: // ��

				if( pCMyApp->m_ptCurPos.y - y < 0 ) // �Ʒ��ʿ����Ӹ�...
					m_Data1 += pCMyApp->m_ptCurPos.y - y; // ���� �̵����� üũ.

				if( m_Data1 < -40 ) // �������̻� ���ϴ� �������� ����������...
				{
					++m_nPointPhase;
					m_Data1 = 0;
				}
				break;

			case 2: // ��

				if( pCMyApp->m_ptCurPos.x - x > 0 ) // ���ʿ����Ӹ�...
					m_Data1 += pCMyApp->m_ptCurPos.x - x; // ���� �̵����� üũ.

				if( m_Data1 > 70 ) // �������̻� ���ϴ� �������� ����������...
				{
					++m_nPointPhase;
					m_Data1 = 0;
				}
				break;

			case 3: // ��

				if( pCMyApp->m_ptCurPos.x - x < 0 ) // �����ʿ����Ӹ�...
					m_Data1 += pCMyApp->m_ptCurPos.x - x; // ���� �̵����� üũ.

				if( m_Data1 < -70 ) // �������̻� ���ϴ� �������� ����������...
				{
					++m_nPointPhase;
					m_Data1 = 0;
				}
				break;
			}
		}
		break;

	case 0x020A: // �ٸ��콺 �޼���(����Ʈ �����ȿ����� �� �޼����� ������ ��ũ�ѿ� ���������ش�.)

		switch( m_nPointPhase )
		{
		case 4: // ����

			if( (short)HIWORD(wParam) > 0 )
			{
				m_Data1 += (short)HIWORD(wParam); // ���� �̵����� üũ.

				if( m_Data1 > 500 ) // �������̻� ���ϴ� �������� ����������...
				{
					++m_nPointPhase;
					m_Data1 = 0;
				}
			}

			break;
		case 5: // �ܾƿ�

			if( (short)HIWORD(wParam) < 0 )
			{
				m_Data1 += (short)HIWORD(wParam); // ���� �̵����� üũ.

				if( m_Data1 < -500 ) // �������̻� ���ϴ� �������� ����������...
				{
					++m_nPointPhase;
					m_Data1 = 0;
				}
			}

			break;
		}

		break;
	}

	return 0; // �� ���� �߿� �ٸ� ������ �ǵ���.
}

bool CTutorialCamAct::EndCheck()
{
	if( m_nPointPhase >= TUTORIAL_CAM_PHASE_NUM )
	{
		pCMyApp->EndRotateCamera(); // Ȥ�� Ŭ�����̶�� �ν����� �ʵ���...

		return TRUE;
	}

	return FALSE;
}






// Ʃ�丮�� ��� ü��.
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

				fx = g_pRoh->m_wx + cos(dir) * dist; // ���ΰ����κ��� dir �������� dist ��ŭ ������ �Ÿ�.
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

				fx = g_pRoh->m_wx + cos(dir) * dist; // ���ΰ����κ��� dir �������� dist ��ŭ ������ �Ÿ�.
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
			g_pTcpIp->SendNetMessage("tutorial 4\n"); // ���� ���� ��û

	}
	/*
		// Ư�� Ÿ�̹� ���� ��� ��µǾ��ϴ� �༮��.
		if( Elapsed >= 500 )  // �� �ð� ���ķ� ���ΰ��� ��� ���.
		{
			if( ( pMob = pCMyApp->m_pUIMgr->FindMobById(100) ) && !pMob->IsDead() && !pMob->IsMoving() && pMob->GetMotionState() != 1 )
			{
				// ������ AI�� �������ش�.
				dist = DistPlToPlAbs( g_pRoh->m_wx, g_pRoh->m_wz, pMob->m_wx, pMob->m_wz);

				if( dist > 20.0f ) // �����Ÿ� ���� �ָ�.
				{
					float dir = (float)atan2( pMob->m_wz-g_pRoh->m_wz, pMob->m_wx-g_pRoh->m_wx ); // ������ ���ϰ�.
					dir -= 0.5f; // �ణ �ٸ������ε� �����ְ�..
					float fx = g_pRoh->m_wx + cos(dir) * 15.0f; // ���ΰ����κ��� dir �������� dist ��ŭ ������ �Ÿ�.
					float fz = g_pRoh->m_wz + sin(dir) * 15.0f;

					int Frame = (int)((dist-15.0f)/2.0f);

					pMob->Move( fx, fz, Frame );
				}
				else
				{
					if( timeGetTime()%2000 < 50 ) // �ֱ������� ������ �õ��ϵ���
					{
						pMob->Attack( g_pRoh->m_wx, g_pRoh->m_wz );
						pMob->SetAttackChaId( g_pRoh->m_nVirId );
					}
				}
			}


			if( (pMob = pCMyApp->m_pUIMgr->FindMobById(101)) && !pMob->IsDead() && !pMob->IsMoving() && pMob->GetMotionState() != 1 )
			{
				// ������ AI�� �������ش�.
				dist = DistPlToPlAbs( g_pRoh->m_wx, g_pRoh->m_wz, pMob->m_wx, pMob->m_wz);

				if( dist > 20.0f ) // �����Ÿ� ���� �ָ�.
				{
					float dir = (float)atan2( pMob->m_wz-g_pRoh->m_wz, pMob->m_wx-g_pRoh->m_wx ); // ������ ���ϰ�.
					dir += 1.0f; // �ణ �ٸ������ε� �����ְ�..
					float fx = g_pRoh->m_wx + cos(dir) * 10.0f; // ���ΰ����κ��� dir �������� dist ��ŭ ������ �Ÿ�.
					float fz = g_pRoh->m_wz + sin(dir) * 10.0f;

					int Frame = (int)((dist-10.0f)/2.0f);

					pMob->Move( fx, fz, Frame );
				}
				else
				{
					if( timeGetTime()%3000 < 50 ) // �ֱ������� ������ �õ��ϵ���
					{
						pMob->Attack( g_pRoh->m_wx, g_pRoh->m_wz );
						pMob->SetAttackChaId( g_pRoh->m_nVirId );
					}
				}
			}
		}
	*/
	// �ֱ������� ��������.
	if( timeGetTime()%10000 < 50 )
	{
		CNkMob *pMob = NULL;
		LHMobMap_it prMob;
		for (prMob = pCMyApp->m_pUIMgr->m_mob_map.begin(); prMob != pCMyApp->m_pUIMgr->m_mob_map.end(); prMob++)
		{
			pMob = (*prMob).second;
			if( pMob->m_nMobNum == 57 ) // ��� ����
			{
				// IDS_TUTORIAL_TALK8 : ������� ��� ��ƾ� �Ѵ�.
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

	if( g_pRoh->m_Level != 1 ) // �θ��� ������ �����������ִϱ� ������ ���Ḧ üũ���ش�
	{
		return TRUE;
	}


	/*  // ���밡 �������� üũ�������� ������ üũ�ϱ�� �ߴ�.
		if( Elapsed >= 5000 ) // ���� ���� �ð� ���� ���� üũ. (���� ����ؼ� �ð��� �ſ� �����Ӱ� �ش�.)
		{
			CNkMob *pMob = NULL;
			LHMobMap_it prMob;

			for (prMob = pCMyApp->m_pUIMgr->m_mob_map.begin(); prMob != pCMyApp->m_pUIMgr->m_mob_map.end(); prMob++)
			{
				pMob = (*prMob).second;
				if( pMob->m_nMobNum == 23 ) // ���밡 ������...
					return FALSE; // ���� �� ������.
			}

			return TRUE; // ���� ���� �ð� ���� ���밡 ������...
		}
	*/


	/*
		if( Elapsed >= 500 ) // ���� ���� �ð� ���� ���� üũ.
		{
			CNkMob* pMob;
			CNkMob* pMob2;

			pMob = pCMyApp->m_pUIMgr->FindMobById(100);
			pMob2 = pCMyApp->m_pUIMgr->FindMobById(101);

			if( (!pMob||pMob->IsDead()) && (!pMob2||pMob2->IsDead()) ) // �Ѵ� �������...
			{
				g_pRoh->SetLevel(2); // ������ ��.
				g_pRoh->SetLevelupPoint( 3 ); // ����Ʈ�� �ش�.

				// ������ ȿ��.
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



// Ʃ�丮�� ����Ʈ �� ü��.
CTutorialPointAct::CTutorialPointAct()
{
	m_dwOldElapsed = 0;
	m_dwStartTick = timeGetTime();

	m_bEndFlag = FALSE;

	g_pNk2DFrame->ShowInterfaceWindow( TRUE, SCORE ); // ����Ʈ â�� �����ش�.
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
	// �ֱ������� ��������.
	if( timeGetTime()%10000 < 50 )
	{
		CNkMob *pMob = NULL;
		LHMobMap_it prMob;
		for (prMob = pCMyApp->m_pUIMgr->m_mob_map.begin(); prMob != pCMyApp->m_pUIMgr->m_mob_map.end(); prMob++)
		{
			pMob = (*prMob).second;
			if( pMob->m_nMobNum == 57 ) // ��� ����
			{
				// IDS_TUTORIAL_TALK9 : ����Ʈ�� ��� ����ؾ� �Ѵ�.
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
			i = 4; // ��
			pScore->m_PointUpBtn[i].MsgProc(hWnd, msg, wParam, lParam);
			i = 5; // ����
			pScore->m_PointUpBtn[i].MsgProc(hWnd, msg, wParam, lParam);
			i = 6; // ���
			pScore->m_PointUpBtn[i].MsgProc(hWnd, msg, wParam, lParam);
			i = 7; // ü��
			pScore->m_PointUpBtn[i].MsgProc(hWnd, msg, wParam, lParam);
			i = 8; // ���
			pScore->m_PointUpBtn[i].MsgProc(hWnd, msg, wParam, lParam);
		}
		else if( msg == WM_LBUTTONUP )
		{
			// ����Ʈ �� ��ư�� ���ؼ� ������ �޽��� ó���� ���ش�.
			if( g_pRoh->m_LevelupPoint >0 )
			{
				i = 4; // ��
				pScore->m_PointUpBtn[i].MsgProc(hWnd, msg, wParam, lParam);
				if( pScore->m_PointUpBtn[i].GetState() == BTN_ACTION)
				{
					++g_pRoh->m_Str;
					pScore->SetStr();
					--g_pRoh->m_LevelupPoint;
					pScore->SetLevelupPoint();
					pScore->m_PointUpBtn[i].m_iBtnFlg = BTN_NORMAL;
				}

				i = 5; // ����
				pScore->m_PointUpBtn[i].MsgProc(hWnd, msg, wParam, lParam);
				if( pScore->m_PointUpBtn[i].GetState() == BTN_ACTION)
				{
					++g_pRoh->m_Int;
					pScore->SetInt();
					--g_pRoh->m_LevelupPoint;
					pScore->SetLevelupPoint();
					pScore->m_PointUpBtn[i].m_iBtnFlg = BTN_NORMAL;
				}

				i = 6; // ���
				pScore->m_PointUpBtn[i].MsgProc(hWnd, msg, wParam, lParam);
				if( pScore->m_PointUpBtn[i].GetState() == BTN_ACTION)
				{
					++g_pRoh->m_Dex;
					pScore->SetDex();
					--g_pRoh->m_LevelupPoint;
					pScore->SetLevelupPoint();
					pScore->m_PointUpBtn[i].m_iBtnFlg = BTN_NORMAL;
				}

				i = 7; // ü��
				pScore->m_PointUpBtn[i].MsgProc(hWnd, msg, wParam, lParam);
				if( pScore->m_PointUpBtn[i].GetState() == BTN_ACTION)
				{
					++g_pRoh->m_Con;
					pScore->SetCon();
					--g_pRoh->m_LevelupPoint;
					pScore->SetLevelupPoint();
					pScore->m_PointUpBtn[i].m_iBtnFlg = BTN_NORMAL;
				}

				i = 8; // ���
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

		return 1; // �̳༮�� �ٸ� �༮�� �޼��� ���ް� ����.
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


// Ʃ�丮�� ������ ü��.
CTutorialPosionBuyAct::CTutorialPosionBuyAct()
{
	m_dwOldElapsed = 0;
	m_dwStartTick = timeGetTime();

	m_bEndFlag = FALSE;
}

CTutorialPosionBuyAct::~CTutorialPosionBuyAct()
{
}

#define TUTORIAL_POSIONBUY_TIMEFLAG_SHOPAPPEAR 500 // ���� ����

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
				fx = 2206.54f; // Ư�� ��ǥ�� ���صд�
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
			g_pTcpIp->SendNetMessage("tutorial 6 -1\n"); // ������� ���� ��û

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
				// ���� Ŭ������ üũ.
				if( timeGetTime() - m_dwPrevClickTick < 500 ) // 0.5�� ���̿� �ٽ��ѹ� Ŭ���� ���.
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
							if( g_pRoh->m_ExtraSlot ) // ����Ʈ�� ������ �ִٸ� �Ȼ�����... (06-01-05 ����)
								return 1;

							slot_x_num = ( (x - g_pNk2DFrame->GetControlShop()->m_shopslotstart_x) / SLOT_WIDTH);
							slot_y_num = ( (y - g_pNk2DFrame->GetControlShop()->m_shopslotstart_y) / SLOT_HEIGHT);

							vnum = g_pNk2DFrame->GetControlShop()->GetSlotVnum(slot_x_num, slot_y_num); // v

							if( vnum >= 0 )
							{
								// �κ��� �������� �ְ� ���� ��´�.
								g_pRoh->m_ExtraSlot = new CItem;
								if(g_pRoh->m_ExtraSlot)
								{
									g_pRoh->m_ExtraSlot->m_Index = 0;
									g_pRoh->m_ExtraSlot->m_Vnum = vnum;
									g_pRoh->m_ExtraSlot->m_ProtoNum = g_ItemList.FindItem(vnum);
								}

								g_pRoh->AddToInven( 0, &x, &y ); // ������� ã�Ƽ� �־��ִ� �༮. m_ExtraSlot ���� �������� �ִ´�.

								SAFE_DELETE( g_pRoh->m_ExtraSlot ); // �ٽ� �����ش�.
							}
						}
						return 1;
					}
				}

				m_dwPrevClickTick = timeGetTime();

		*/


		//------------------------------------------------------------------------
		// ��ü ���� �߿��� Ŭ���� ���� ã�Ƽ� ó��.
		//------------------------------------------------------------------------
		for (prMob = pCMyApp->m_pUIMgr->m_mob_map.begin(); prMob != pCMyApp->m_pUIMgr->m_mob_map.end(); prMob++)
		{
			pMob = (*prMob).second;
			// Color Pickng Buffer �� ���� Color �� �����ϴٸ� ( ���õǾ��ٸ� )
			if (pMob->m_wColorIdx == pCMyApp->m_pUIMgr->GetPickColor() )
			{
				if( pMob->m_nMobNum == 57 ) // ���� NPC Ŭ����.
				{
					// �κ��� ������ �ִ��� üũ�Ѵ�.

					if( g_pRoh->FindItemByVnumFromAll(140) ) // '����ġ�Ṱ��'�� ������ �ִ��� ����.
					{
						m_bEndFlag = TRUE;
					}
					else // ���� ���� ġ�� ������ �Ȱ����� Ŭ��������.
					{
						// IDS_TUTORIAL_TALK10 : '����ġ�Ṱ��'�� ������ �;� �Ѵ�. �ٽ� ���� �������
						sprintf( strTemp, (char *)G_STRING(IDS_TUTORIAL_TALK10) );
						pMob->SetMobChatMsg( strTemp );

						if (g_pTcpIp)
							g_pTcpIp->SendNetMessage("tutorial 6 0\n"); // ���� ���� �ʱ�ȭ ��û
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
			g_pTcpIp->SendNetMessage("tutorial 6 1\n"); // ���� ���� Npc ���� ��û

		return TRUE;
	}

	return FALSE;
}




// Ʃ�丮�� ������ ü��.
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

	g_pRoh->m_Vital = g_pRoh->m_MaxVital/2; // �Ǹ� ����� ��´�.

	// �ֱ������� ��������.
	if( timeGetTime()%10000 < 50 )
	{
		CNkMob *pMob = NULL;
		LHMobMap_it prMob;
		for (prMob = pCMyApp->m_pUIMgr->m_mob_map.begin(); prMob != pCMyApp->m_pUIMgr->m_mob_map.end(); prMob++)
		{
			pMob = (*prMob).second;
			if( pMob->m_nMobNum == 57 ) // ��� ����
			{
				// IDS_TUTORIAL_TALK11 : '����ġ�Ṱ��'�� �����Կ� �ְ� ����� ���Ŷ�
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
			if( g_pRoh->m_QuickSlot[x] && g_pRoh->m_QuickSlot[x]->m_Vnum == 140 ) // ���� ġ�� ������ ����� �Ŷ��...
			{
				m_bEndFlag = TRUE;
				SAFE_DELETE(g_pRoh->m_QuickSlot[x]); // ���� ����������.
				g_pRoh->CountQuickBySlot(x);
				return 1; // ������ ������ �ʴ´�.
			}
		}

		break;

	case WM_RBUTTONDOWN:

		x = LOWORD (lParam);
		y = HIWORD (lParam);

		// ���콺 ������ Ŭ������ ����ϴ� ���.
		if( g_pNk2DFrame->GetControlBottom()->IsInsideQuickSlot(x,y) )
		{
			int slot_num = g_pNk2DFrame->GetControlBottom()->GetCurQuickSlotNum();
			// ���콺 ��Ŭ�� ������ ���� ó��..�κ�..
			if( slot_num >= 0 && slot_num <QUICK_SLOT_NUM*QUICK_SLOT_PAGE)
			{
				if( g_pRoh->m_QuickSlot[slot_num] && g_pRoh->m_QuickSlot[slot_num]->m_Vnum == 140 ) // ���� ġ�� ������ ����� �Ŷ��...
				{
					m_bEndFlag = TRUE;
					SAFE_DELETE(g_pRoh->m_QuickSlot[slot_num]); // ���� ����������.
					g_pRoh->CountQuickBySlot(slot_num);
					return 1; // ������ ������ �ʴ´�.
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
		g_pRoh->m_Vital = g_pRoh->m_MaxVital; // �ǵ� ����� ä���.

		return TRUE;
	}
	/*
		// ���ΰ� �Ƿ� üũ�ϴ� ������� �ٲ��.
		if( g_pRoh->m_Vital == g_pRoh->m_MaxVital )
		{
			return TRUE;
		}
	*/
	return FALSE;
}



// Ʃ�丮�� ����� ü��.
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
			g_pTcpIp->SendNetMessage("tutorial 8 -1\n"); // ������ ���� ��û
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
		// ��ü ���� �߿��� Ŭ���� ���� ã�Ƽ� ó��.
		//------------------------------------------------------------------------
		for (prMob = pCMyApp->m_pUIMgr->m_mob_map.begin(); prMob != pCMyApp->m_pUIMgr->m_mob_map.end(); prMob++)
		{
			pMob = (*prMob).second;
			// Color Pickng Buffer �� ���� Color �� �����ϴٸ� ( ���õǾ��ٸ� )
			if (pMob->m_wColorIdx == pCMyApp->m_pUIMgr->GetPickColor() )
			{
				if( pMob->m_nMobNum == 57 ) // ���� NPC Ŭ����.
				{
					// �κ��� ������ �ִ��� üũ�Ѵ�.

					if( g_pRoh->FindItemByVnumFromAll(1592) && g_pRoh->FindItemByVnumFromAll(1593)
							&& g_pRoh->FindItemByVnumFromAll(1594) && g_pRoh->FindItemByVnumFromAll(1595) ) // 'Ʈ���̴� ��Ʈ'�� ������ �ִ��� ����.
					{
						m_bEndFlag = TRUE;
					}
					else // ���� ���� ġ�� ������ �Ȱ����� Ŭ��������.
					{
						// IDS_TUTORIAL_TALK12 : 'Ʈ���̴� ��Ʈ'�� ���� ������ �־�� �Ѵ�. �ٽ� ���� �������
						sprintf( strTemp, (char *)G_STRING(IDS_TUTORIAL_TALK12) );
						pMob->SetMobChatMsg( strTemp );

						if (g_pTcpIp)
							g_pTcpIp->SendNetMessage("tutorial 8 0\n"); // ���� ���� �ʱ�ȭ ��û
					}
				}

//				return 1; // Ŭ���� ���۾��ϵ���.
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
			g_pTcpIp->SendNetMessage("tutorial 8 1\n"); // ���� ���� Npc ���� ��û

		return TRUE;
	}

	return FALSE;
}


// Ʃ�丮�� ��� ����
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


	// �ֱ������� ��������.
	if( timeGetTime()%10000 < 50 )
	{
		CNkMob *pMob = NULL;
		LHMobMap_it prMob;
		for (prMob = pCMyApp->m_pUIMgr->m_mob_map.begin(); prMob != pCMyApp->m_pUIMgr->m_mob_map.end(); prMob++)
		{
			pMob = (*prMob).second;
			if( pMob->m_nMobNum == 57 ) // ��� ����
			{
				// IDS_TUTORIAL_TALK13 : 'Ʈ���̴׼�Ʈ'�� ���� �����ؾ� �Ѵ�.
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
	if( (g_pRoh->m_Wearing[IARMOR_ARMOR]&&g_pRoh->m_Wearing[IARMOR_ARMOR]->m_Vnum==1592) // Ʈ���̴� ����
			&& (g_pRoh->m_Wearing[IARMOR_PANTS]&&g_pRoh->m_Wearing[IARMOR_PANTS]->m_Vnum==1593) // Ʈ���̴� ����
			&& (g_pRoh->m_Wearing[IARMOR_SHOES]&&g_pRoh->m_Wearing[IARMOR_SHOES]->m_Vnum==1594) // Ʈ���̴� �Ź�
			&& (g_pRoh->m_Wearing[IARMOR_HELMET]&&g_pRoh->m_Wearing[IARMOR_HELMET]->m_Vnum==1595) // Ʈ���̴� ���
	  ) // Ʈ���̴� ��Ʈ ���� Ȯ��
	{
		return TRUE;
	}

	return FALSE;
}


// Ʃ�丮�� �޴��� ü��
CTutorialTopMenuAct::CTutorialTopMenuAct()
{
	m_dwOldElapsed = 0;
	m_dwStartTick = timeGetTime();

	m_nPointPhase = 0; // ��� ���������ϴ� ��.

	m_bEndFlag = FALSE;

	g_pNk2DFrame->CloseAllWindow();
}

CTutorialTopMenuAct::~CTutorialTopMenuAct()
{
}

#define TUTORIAL_TOPMENU_PHASE_NUM 9 // '����'���� '����'' �����Ÿ� 10��..

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
	case 0: // ĳ����.

		if( g_pNk2DFrame->IsInterfaceVisible(SCORE) )
		{
			CNkMob *pMob = NULL;
			LHMobMap_it prMob;
			for (prMob = pCMyApp->m_pUIMgr->m_mob_map.begin(); prMob != pCMyApp->m_pUIMgr->m_mob_map.end(); prMob++)
			{
				pMob = (*prMob).second;
				if( pMob->m_nMobNum == 57 ) // ��� ����
				{
					// IDS_TUTORIAL_TALK14 : ĳ������ �����̳� �������ͽ��� Ȯ���� �� ����ϴ� ��ư�̴�.
					sprintf( strTemp, (char *)G_STRING(IDS_TUTORIAL_TALK14) );

					pMob->SetMobChatMsg( strTemp );
					break;
				}
			}

			++m_nPointPhase;
		}
		else
		{
			// �ֱ������� ��������.
			if( timeGetTime()%10000 < 50 )
			{
				CNkMob *pMob = NULL;
				LHMobMap_it prMob;
				for (prMob = pCMyApp->m_pUIMgr->m_mob_map.begin(); prMob != pCMyApp->m_pUIMgr->m_mob_map.end(); prMob++)
				{
					pMob = (*prMob).second;
					if( pMob->m_nMobNum == 57 ) // ��� ����
					{
						// IDS_TUTORIAL_TALK15 : ĳ���� ����â�� �����. ȭ����� ����� ��ư�̳� 'c'Ű�� ������ Ȯ�� �� �� �ִ�.
						sprintf( strTemp, (char *)G_STRING(IDS_TUTORIAL_TALK15) );

						pMob->SetMobChatMsg( strTemp );
						break;
					}
				}
			}
		}

		break;

	case 1: // �κ�.

		if( g_pNk2DFrame->IsInvenVisible() )
		{
			CNkMob *pMob = NULL;
			LHMobMap_it prMob;
			for (prMob = pCMyApp->m_pUIMgr->m_mob_map.begin(); prMob != pCMyApp->m_pUIMgr->m_mob_map.end(); prMob++)
			{
				pMob = (*prMob).second;
				if( pMob->m_nMobNum == 57 ) // ��� ����
				{
					// IDS_TUTORIAL_TALK16 : �ڽ��� ������� ���� ��ǰ�� Ȯ���� �� ����ϴ� ��ư�̴�.
					sprintf( strTemp, (char *)G_STRING(IDS_TUTORIAL_TALK16) );

					pMob->SetMobChatMsg( strTemp );
					break;
				}
			}

			++m_nPointPhase;
		}
		else
		{
			// �ֱ������� ��������.
			if( timeGetTime()%10000 < 50 )
			{
				CNkMob *pMob = NULL;
				LHMobMap_it prMob;
				for (prMob = pCMyApp->m_pUIMgr->m_mob_map.begin(); prMob != pCMyApp->m_pUIMgr->m_mob_map.end(); prMob++)
				{
					pMob = (*prMob).second;
					if( pMob->m_nMobNum == 57 ) // ��� ����
					{
						// IDS_TUTORIAL_TALK17 : �κ��丮 ����â�� �����. ȭ����� ����� ��ư�̳� 'i'Ű�� ������ Ȯ�� �� �� �ִ�.
						sprintf( strTemp, (char *)G_STRING(IDS_TUTORIAL_TALK17) );

						pMob->SetMobChatMsg( strTemp );
						break;
					}
				}
			}
		}

		break;

	case 2: // ��ų.

		if( g_pNk2DFrame->IsSkillVisible() )
		{
			CNkMob *pMob = NULL;
			LHMobMap_it prMob;
			for (prMob = pCMyApp->m_pUIMgr->m_mob_map.begin(); prMob != pCMyApp->m_pUIMgr->m_mob_map.end(); prMob++)
			{
				pMob = (*prMob).second;
				if( pMob->m_nMobNum == 57 ) // ��� ����
				{
					// IDS_TUTORIAL_TALK18 : ĳ������ ��ų ������ ������ �� ����. ���� â�� �ֿϵ��������� ��Ÿ���� â������ ������ �ֿϵ����� ������ �Ѿ�ڴ�.
					sprintf( strTemp, (char *)G_STRING(IDS_TUTORIAL_TALK18) );

					pMob->SetMobChatMsg( strTemp );
					break;
				}
			}

			++m_nPointPhase;
		}
		else
		{
			// �ֱ������� ��������.
			if( timeGetTime()%10000 < 50 )
			{
				CNkMob *pMob = NULL;
				LHMobMap_it prMob;
				for (prMob = pCMyApp->m_pUIMgr->m_mob_map.begin(); prMob != pCMyApp->m_pUIMgr->m_mob_map.end(); prMob++)
				{
					pMob = (*prMob).second;
					if( pMob->m_nMobNum == 57 ) // ��� ����
					{
						// IDS_TUTORIAL_TALK19 : ��ų ����â�� �����. ȭ����� ����� ��ư�̳� 't'Ű�� ������ Ȯ�� �� �� �ִ�.
						sprintf( strTemp, (char *)G_STRING(IDS_TUTORIAL_TALK19) );

						pMob->SetMobChatMsg( strTemp );
						break;
					}
				}
			}
		}

		break;

	case 3: // �ֿϵ���
		++m_nPointPhase; // ������ ��� ������ �ѱ���.

		break;

	case 4: // ��

		if( g_pNk2DFrame->IsInterfaceVisible(MAP) )
		{
			CNkMob *pMob = NULL;
			LHMobMap_it prMob;
			for (prMob = pCMyApp->m_pUIMgr->m_mob_map.begin(); prMob != pCMyApp->m_pUIMgr->m_mob_map.end(); prMob++)
			{
				pMob = (*prMob).second;
				if( pMob->m_nMobNum == 57 ) // ��� ����
				{
					// IDS_TUTORIAL_TALK20 : ���� �ڽ��� ��ġ�� Ȯ���� �� ����ϴ� ��ư�̴�.
					sprintf( strTemp, (char *)G_STRING(IDS_TUTORIAL_TALK20) );

					pMob->SetMobChatMsg( strTemp );
					break;
				}
			}

			++m_nPointPhase;
		}
		else
		{
			// �ֱ������� ��������.
			if( timeGetTime()%10000 < 50 )
			{
				CNkMob *pMob = NULL;
				LHMobMap_it prMob;
				for (prMob = pCMyApp->m_pUIMgr->m_mob_map.begin(); prMob != pCMyApp->m_pUIMgr->m_mob_map.end(); prMob++)
				{
					pMob = (*prMob).second;
					if( pMob->m_nMobNum == 57 ) // ��� ����
					{
						// IDS_TUTORIAL_TALK21 : �̴ϸ� ����â�� �����. ȭ����� ����� ��ư�̳� 'm'Ű�� ������ Ȯ�� �� �� �ִ�.
						sprintf( strTemp, (char *)G_STRING(IDS_TUTORIAL_TALK21) );

						pMob->SetMobChatMsg( strTemp );
						break;
					}
				}
			}
		}

		break;

	case 5: // ����

		if( g_pNk2DFrame->IsHelpVisible() )
		{
			CNkMob *pMob = NULL;
			LHMobMap_it prMob;
			for (prMob = pCMyApp->m_pUIMgr->m_mob_map.begin(); prMob != pCMyApp->m_pUIMgr->m_mob_map.end(); prMob++)
			{
				pMob = (*prMob).second;
				if( pMob->m_nMobNum == 57 ) // ��� ����
				{
					// IDS_TUTORIAL_TALK22 : ������ ������ Ȯ���� ������ϴ� ��ư�̴�.
					sprintf( strTemp, (char *)G_STRING(IDS_TUTORIAL_TALK22) );

					pMob->SetMobChatMsg( strTemp );
					break;
				}
			}

			++m_nPointPhase;
		}
		else
		{
			// �ֱ������� ��������.
			if( timeGetTime()%10000 < 50 )
			{
				CNkMob *pMob = NULL;
				LHMobMap_it prMob;
				for (prMob = pCMyApp->m_pUIMgr->m_mob_map.begin(); prMob != pCMyApp->m_pUIMgr->m_mob_map.end(); prMob++)
				{
					pMob = (*prMob).second;
					if( pMob->m_nMobNum == 57 ) // ��� ����
					{
						// IDS_TUTORIAL_TALK23 : ���� ����â�� �����. ȭ����� ����� ��ư�̳� 'F1'Ű�� ������ Ȯ�� �� �� �ִ�.
						sprintf( strTemp, (char *)G_STRING(IDS_TUTORIAL_TALK23) );

						pMob->SetMobChatMsg( strTemp );
						break;
					}
				}
			}
		}

		break;

	case 6: // ��Ƽ

		if( g_pNk2DFrame->IsPartyVisible() )
		{
			CNkMob *pMob = NULL;
			LHMobMap_it prMob;
			for (prMob = pCMyApp->m_pUIMgr->m_mob_map.begin(); prMob != pCMyApp->m_pUIMgr->m_mob_map.end(); prMob++)
			{
				pMob = (*prMob).second;
				if( pMob->m_nMobNum == 57 ) // ��� ����
				{
					// IDS_TUTORIAL_TALK24 : �ڽ��� ���� ��Ƽ�� ������ Ȯ���Ҷ� ����ϴ� ��ư�̴�.
					sprintf( strTemp, (char *)G_STRING(IDS_TUTORIAL_TALK24) );

					pMob->SetMobChatMsg( strTemp );
					break;
				}
			}

			++m_nPointPhase;

			//  [4/30/2009 ppmmjj83] �Ϻ��� �İ����� ���Ƴ��� Ʃ�丮��� �׳� �ѱ��
			if( g_dwClientCountry == CTRY_JPN ) ++m_nPointPhase;
		}
		else
		{
			// �ֱ������� ��������.
			if( timeGetTime()%10000 < 50 )
			{
				CNkMob *pMob = NULL;
				LHMobMap_it prMob;
				for (prMob = pCMyApp->m_pUIMgr->m_mob_map.begin(); prMob != pCMyApp->m_pUIMgr->m_mob_map.end(); prMob++)
				{
					pMob = (*prMob).second;
					if( pMob->m_nMobNum == 57 ) // ��� ����
					{
						// IDS_TUTORIAL_TALK25 : ��Ƽ ����â�� �����. ȭ����� ����� ��ư�̳� 'p'Ű�� ������ Ȯ�� �� �� �ִ�.
						sprintf( strTemp, (char *)G_STRING(IDS_TUTORIAL_TALK25) );

						pMob->SetMobChatMsg( strTemp );
						break;
					}
				}
			}
		}

		break;

	case 7: // �İ���

		if( g_pNk2DFrame->IsHelperVisible() )
		{
			CNkMob *pMob = NULL;
			LHMobMap_it prMob;
			for (prMob = pCMyApp->m_pUIMgr->m_mob_map.begin(); prMob != pCMyApp->m_pUIMgr->m_mob_map.end(); prMob++)
			{
				pMob = (*prMob).second;
				if( pMob->m_nMobNum == 57 ) // ��� ����
				{
					// IDS_TUTORIAL_TALK26 : �ڽ��� �İ��� ������ Ȯ���� �� ����ϴ� ��ư�̴�.
					sprintf( strTemp, (char *)G_STRING(IDS_TUTORIAL_TALK26) );

					pMob->SetMobChatMsg( strTemp );
					break;
				}
			}

			if( g_SvrType != ST_ADULT_ONLY ) // ���� �� ������ ������� �������� ����. �İ���â���� �����ش�.
				// IDS_TUTORIAL_TALK27 : �İ��� â�� �����ø� ���� �ܰ�� �����մϴ�.
				g_pNk2DFrame->SetInfoBox( (char*)G_STRING(IDS_TUTORIAL_TALK27) );

			++m_nPointPhase;
		}
		else
		{
			// �ֱ������� ��������.
			if( timeGetTime()%10000 < 50 )
			{
				CNkMob *pMob = NULL;
				LHMobMap_it prMob;
				for (prMob = pCMyApp->m_pUIMgr->m_mob_map.begin(); prMob != pCMyApp->m_pUIMgr->m_mob_map.end(); prMob++)
				{
					pMob = (*prMob).second;
					if( pMob->m_nMobNum == 57 ) // ��� ����
					{
						// IDS_TUTORIAL_TALK27 : �İ��� ����â�� �����. ȭ����� ����� ��ư�̳� 'h'Ű�� ������ Ȯ�� �� �� �ִ�.
						sprintf( strTemp, (char *)G_STRING(IDS_TUTORIAL_TALK27) );

						pMob->SetMobChatMsg( strTemp );
						break;
					}
				}
			}
		}

		break;

	case 8: // ��� ����

		if( g_SvrType == ST_ADULT_ONLY )
		{
			if( g_pNk2DFrame->IsControlGuild() )
			{
				CNkMob *pMob = NULL;
				LHMobMap_it prMob;
				for (prMob = pCMyApp->m_pUIMgr->m_mob_map.begin(); prMob != pCMyApp->m_pUIMgr->m_mob_map.end(); prMob++)
				{
					pMob = (*prMob).second;
					if( pMob->m_nMobNum == 57 ) // ��� ����
					{
						// IDS_TUTORIAL_TALK28 : �ڽ��� ��� ������ Ȯ���� �� ����ϴ� ��ư�̴�.
						sprintf( strTemp, (char *)G_STRING(IDS_TUTORIAL_TALK28) );

						pMob->SetMobChatMsg( strTemp );
						break;
					}
				}

				// IDS_TUTORIAL_TALK29 : ��� ���� â�� �����ø� ���� �ܰ�� �����մϴ�.
				g_pNk2DFrame->SetInfoBox( (char*)G_STRING(IDS_TUTORIAL_TALK29) );

				++m_nPointPhase;
			}
			else
			{
				// �ֱ������� ��������.
				if( timeGetTime()%10000 < 50 )
				{
					CNkMob *pMob = NULL;
					LHMobMap_it prMob;
					for (prMob = pCMyApp->m_pUIMgr->m_mob_map.begin(); prMob != pCMyApp->m_pUIMgr->m_mob_map.end(); prMob++)
					{
						pMob = (*prMob).second;
						if( pMob->m_nMobNum == 57 ) // ��� ����
						{
							// IDS_TUTORIAL_TALK30 : ��� ����â�� �����. ȭ����� ����� ��ư�̳� 'g'Ű�� ������ Ȯ�� �� �� �ִ�.
							sprintf( strTemp, (char *)G_STRING(IDS_TUTORIAL_TALK30) );

							pMob->SetMobChatMsg( strTemp );
							break;
						}
					}
				}
			}
		}
		else // �ű� ������ ������...
		{
			++m_nPointPhase; // �ٷ� �ѱ��.
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



// Ʃ�丮�� ��Ƽ��û ü��.
CTutorialPartyAct::CTutorialPartyAct()
{
	m_dwOldElapsed = 0;
	m_dwStartTick = timeGetTime();

	m_bEndFlag = FALSE;

	m_nPointPhase = 0;
	m_dwEndFlagTick = 0;

	g_pNk2DFrame->CloseAllWindow();

//	m_pTeacher = NULL; // ���� �����͸� ������ �ִ´�.
	CNkMob *pMob = NULL;
	LHMobMap_it prMob;
	for (prMob = pCMyApp->m_pUIMgr->m_mob_map.begin(); prMob != pCMyApp->m_pUIMgr->m_mob_map.end(); prMob++)
	{
		pMob = (*prMob).second;
		if( pMob->m_nMobNum == 57 ) // ��� ����
			break;
	}

	// �������� ���� ĳ���� ��ü�� ������ش�.(�׸��� �����Ƿ� �����͸� �����Ѵ�.)
	if( pMob )
	{
		pCMyApp->m_pUIMgr->AddPlayer( pMob->m_nVirId, pMob->GetMobName(),0,0, FALSE, pMob->m_wx, pMob->m_wz,pMob->m_dir );
	}
}

CTutorialPartyAct::~CTutorialPartyAct()
{
	// �������� ���� ĳ���� ��ü�� �����.
	CNkMob *pMob = NULL;
	LHMobMap_it prMob;
	for (prMob = pCMyApp->m_pUIMgr->m_mob_map.begin(); prMob != pCMyApp->m_pUIMgr->m_mob_map.end(); prMob++)
	{
		pMob = (*prMob).second;
		if( pMob->m_nMobNum == 57 ) // ��� ����
			break;
	}
	if( pMob )
	{
		pCMyApp->m_pUIMgr->DeletePlayer( pMob->m_nVirId ); // ���� �׼ǿ����� ���� ĳ���Ͱ� �ʿ������� ����� �ٽ� ������ش�. ������ Ȯ���� �ϱ� ����
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
		if( pTemp->m_nMobNum == 57 ) // ��� ����
		{
			pMob = pTemp;
			break;
		}
	}

	if( m_nPointPhase == 0 ) // ��û ��.
	{
		// �ֱ������� ��������.
		if( timeGetTime()%10000 < 50 )
		{
			if( pMob )
			{
				// IDS_TUTORIAL_TALK31 : ������ ��Ƽ�� ��û�� ���Ŷ�.Ű���� ������ Shift Ű�� ���� ���¿��� ���� Ŭ���ؾ� ��û ��ư�� ���´�.
				sprintf( strTemp, (char *)G_STRING(IDS_TUTORIAL_TALK31) );

				pMob->SetMobChatMsg( strTemp );
			}
		}
	}
	else // ��û ��. �̴ϸ� ����.
	{
		if( pMob ) // �ٴڿ� ��Ƽ�� ��Ʈ ǥ��.
		{
			g_pShadowRect->SetTileCoord(D3DVECTOR(pMob->m_wx, pMob->m_wy, pMob->m_wz), 20, FALSE);
			g_pShadowRect->Render(g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_PARTY_TEX ] ), FALSE);
		}

		if( !g_pNk2DFrame->IsInterfaceVisible(MAP) && !m_dwEndFlagTick ) // m_dwEndFlagTick �� �̴ϸ��� �ѹ� ������ �÷��׷� ���.
		{
			if( pMob )
			{
				// IDS_TUTORIAL_TALK32 : �̴ϸ� �������̽��� ���� ��Ƽ ǥ�ø� Ȯ���� ����.
				sprintf( strTemp, (char *)G_STRING(IDS_TUTORIAL_TALK32) );

				pMob->SetMobChatMsg( strTemp );
			}
		}


		// ���� ���� üũ.
		if( g_pNk2DFrame->IsInterfaceVisible(MAP) && !m_dwEndFlagTick ) // m_dwEndFlagTick �� �̴ϸ��� �ѹ� ������ �÷��׷� ���.
		{
			m_dwEndFlagTick = timeGetTime();
			// IDS_TUTORIAL_TALK33 : �̴ϸ� �������̽��� ������ ���� �ܰ�� �����մϴ�.
			g_pNk2DFrame->SetInfoBox( (char*)G_STRING(IDS_TUTORIAL_TALK33) );

			if( pMob )
			{
				// IDS_TUTORIAL_TALK34 : �̴ϸʿ� ��¦�̴� �ϴû� ���� ���� ��ġ�� ��Ÿ����.
				sprintf( strTemp, (char*)G_STRING(IDS_TUTORIAL_TALK34) );

				pMob->SetMobChatMsg( strTemp );
			}
		}

		if( m_dwEndFlagTick && !g_pNk2DFrame->IsInterfaceVisible(MAP) ) // �ô� ���� ���.
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

		if( m_nPointPhase == 0 && (GetKeyState(VK_SHIFT) & 0x80) && pCMyApp->m_pUIMgr->m_whichShift == 1 ) // ��û�� ��Ȳ. ����Ʈ ���� üũ.
		{
			//------------------------------------------------------------------------
			// ��ü ���� �߿��� Ŭ���� ���� ã�Ƽ� ó��.
			//------------------------------------------------------------------------
			CNkMob *pMob = NULL;
			LHMobMap_it prMob;

			for (prMob = pCMyApp->m_pUIMgr->m_mob_map.begin(); prMob != pCMyApp->m_pUIMgr->m_mob_map.end(); prMob++)
			{
				pMob = (*prMob).second;
				// Color Pickng Buffer �� ���� Color �� �����ϴٸ� ( ���õǾ��ٸ� )
				if (pMob->m_wColorIdx == pCMyApp->m_pUIMgr->GetPickColor() )
				{
					// NPC �� Ŭ�� �Ǿ��ٸ� return 1;
					if( pMob->m_nMobNum == 57 ) // ��� ���� Ŭ����.
					{
						CNkCharacter *pCha = new CNkCharacter(); // ���Ƿ� ���� ĳ���͸� �����

						if( pCha )
						{
							// RShiftMenu�� ���µ� �ʿ��� ������ �������ش�.
							pCha->SetInitPos(pMob->m_wx, pMob->m_wz);
							pCha->m_Race = RACE_BULKAN; // ���Ƿ� ��ĭ
							pCha->SetInfo( pMob->m_nVirId, pMob->GetMobName() );

							g_pNk2DFrame->ShowRShiftMenu( pCha );

							SAFE_DELETE( pCha );
						}
					}

					return 1; // Ŭ���� ���۾��ϵ���.
				}
			}
		}

		break;

	case WM_LBUTTONUP:

		x = LOWORD (lParam);
		y = HIWORD (lParam);

		if( m_nPointPhase == 0 ) // ��û�� ��Ȳ.
		{
			if( g_pNk2DFrame->IsRShiftVisible() && g_pNk2DFrame->GetRShiftMenu()->IsInside(x,y) ) // Ȥ�� ��ûâ�� ���̴� ���¶��..
			{
				if( g_pNk2DFrame->GetRShiftMenu()->m_pBtn[CRShiftMenu::eParty] && g_pNk2DFrame->GetRShiftMenu()->m_pBtn[CRShiftMenu::eParty]->IsInside(x,y) ) // ��ư2���� ������...
				{
					++m_nPointPhase; // ��Ƽ ��û�� �� ���·� �ٲ��ش�.
					g_pNk2DFrame->CloseAllWindow(); // ���������� �ѹ� �� ������.

					// ��Ƽ ó��
					g_pNk2DFrame->GetControlParty()->SetParty( TRUE );
					g_pRoh->m_bIsParty = TRUE;

					// �ڱ� �ڽŵ� ����� �߰�������Ѵ�.
					g_pNk2DFrame->GetControlParty()->AddMember( g_pRoh->m_nCharIndex, g_pRoh->m_pstrName );

					CNkMob *pMob = NULL;
					LHMobMap_it prMob;
					for (prMob = pCMyApp->m_pUIMgr->m_mob_map.begin(); prMob != pCMyApp->m_pUIMgr->m_mob_map.end(); prMob++)
					{
						pMob = (*prMob).second;
						if( pMob->m_nMobNum == 57 ) // ��� ����
							break;
					}

					if( pMob )
					{
						g_pNk2DFrame->GetControlParty()->AddMember( pMob->m_nVirId, pMob->GetMobName() );

						sprintf( strTemp, "%d 100 %d %d\n", pMob->m_nVirId, (int)pMob->m_wx, (int)pMob->m_wz  );
						g_pRoh->GetPvInfo( strTemp ); // �̴ϸ� ��ġǥ�ÿ�.
					}

					return 1; // �޽��� ����
				}
				g_pNk2DFrame->CloseAllWindow(); // RShiftMenu �� ���̴� ���¿��� ���콺 Ŭ�������� RShiftMenu�� �ݾ��ش�.
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




// Ʃ�丮�� ��ȯ ü��.
CTutorialExchangeAct::CTutorialExchangeAct()
{
	m_dwOldElapsed = 0;
	m_dwStartTick = timeGetTime();

	m_bEndFlag = FALSE;

	m_nPointPhase = 0;

	g_pNk2DFrame->CloseAllWindow();

//	m_pTeacher = NULL; // ���� �����͸� ������ �ִ´�.
	CNkMob *pMob = NULL;
	LHMobMap_it prMob;
	for (prMob = pCMyApp->m_pUIMgr->m_mob_map.begin(); prMob != pCMyApp->m_pUIMgr->m_mob_map.end(); prMob++)
	{
		pMob = (*prMob).second;
		if( pMob->m_nMobNum == 57 ) // ��� ����
			break;
	}

	// �������� ���� ĳ���� ��ü�� ������ش�.(�׸��� �����Ƿ� �����͸� �����Ѵ�.)
	if( pMob )
	{
		pCMyApp->m_pUIMgr->AddPlayer( pMob->m_nVirId, pMob->GetMobName(),0,0, FALSE, pMob->m_wx, pMob->m_wz,	pMob->m_dir );
	}
}

CTutorialExchangeAct::~CTutorialExchangeAct()
{
	// �������� ���� ĳ���� ��ü�� �����.
	CNkMob *pMob = NULL;
	LHMobMap_it prMob;
	for (prMob = pCMyApp->m_pUIMgr->m_mob_map.begin(); prMob != pCMyApp->m_pUIMgr->m_mob_map.end(); prMob++)
	{
		pMob = (*prMob).second;
		if( pMob->m_nMobNum == 57 ) // ��� ����
			break;
	}

	if( pMob )
	{
		pCMyApp->m_pUIMgr->DeletePlayer( pMob->m_nVirId ); // ���� �׼ǿ����� ���� ĳ���Ͱ� �ʿ������� ����� �ٽ� ������ش�. ������ Ȯ���� �ϱ� ����
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
			g_pTcpIp->SendNetMessage("tutorial 12 -1\n"); // �������� ���� ��û
	}


	if( m_nPointPhase == 0 ) // ��� ���� ��.
	{
		// �ֱ������� ��������.
		if( timeGetTime()%10000 < 50 )
		{
			CNkMob *pMob = NULL;
			LHMobMap_it prMob;
			for (prMob = pCMyApp->m_pUIMgr->m_mob_map.begin(); prMob != pCMyApp->m_pUIMgr->m_mob_map.end(); prMob++)
			{
				pMob = (*prMob).second;
				if( pMob->m_nMobNum == 57 ) // ��� ����
				{
					// IDS_TUTORIAL_TALK35 : ���ο��Լ� �����⸦ ��;� �Ѵ�.
					sprintf( strTemp, (char *)G_STRING(IDS_TUTORIAL_TALK35) );

					pMob->SetMobChatMsg( strTemp );
					break;
				}
			}
		}
	}
	else if( m_nPointPhase == 1 ) // ��û��
	{
		// �ֱ������� ��������.
		if( timeGetTime()%10000 < 50 )
		{
			CNkMob *pMob = NULL;
			LHMobMap_it prMob;
			for (prMob = pCMyApp->m_pUIMgr->m_mob_map.begin(); prMob != pCMyApp->m_pUIMgr->m_mob_map.end(); prMob++)
			{
				pMob = (*prMob).second;
				if( pMob->m_nMobNum == 57 ) // ��� ����
				{
					// IDS_TUTORIAL_TALK36 :������ ��ȯ�� ��û�� ���Ŷ�.Ű���� ������ Shift Ű�� ���� ���¿��� ���� Ŭ���ϸ� ��û ��ư�� ���´�.
					sprintf( strTemp, (char *)G_STRING(IDS_TUTORIAL_TALK36) );

					pMob->SetMobChatMsg( strTemp );
					break;
				}
			}
		}
	}
	else if( m_nPointPhase == 2 ) // ��û ��
	{
		// ���� ���� üũ.
		if( !g_pNk2DFrame->IsExchangeVisible() ) // ���� ��ȯâ�� �ٽ� �����ȴٸ�...
		{
			m_nPointPhase = 1; // �ٽ� ��ȯ��û�� �����ϵ���.
		}
	}
	/*
		else // ����� ��ȯ �Ϸ�.
		{
			if( !g_pNk2DFrame->IsExchangeVisible() ) // ���� ��ȯâ�� ���� ����.
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

		if( m_nPointPhase == 0 ) // ������ �����
		{
			//------------------------------------------------------------------------
			// ��ü ���� �߿��� Ŭ���� ���� ã�Ƽ� ó��.
			//------------------------------------------------------------------------
			for (prMob = pCMyApp->m_pUIMgr->m_mob_map.begin(); prMob != pCMyApp->m_pUIMgr->m_mob_map.end(); prMob++)
			{
				pMob = (*prMob).second;
				// Color Pickng Buffer �� ���� Color �� �����ϴٸ� ( ���õǾ��ٸ� )
				if (pMob->m_wColorIdx == pCMyApp->m_pUIMgr->GetPickColor() )
				{
					if( pMob->m_nMobNum == 57 ) // ���� NPC Ŭ����.
					{
						// �κ��� ������ �ִ��� üũ�Ѵ�.

						if( g_pRoh->FindItemByVnumFromAll(142) ) // �����Ⱑ �ִ��� ã�´�
						{
							++m_nPointPhase;
						}
						else // ���� �����⸦ �Ȱ����� Ŭ��������.
						{
							// IDS_TUTORIAL_TALK37 : '������'�� ������ �־�� �Ѵ�. �ٽ� ���� �������
							sprintf( strTemp, (char *)G_STRING(IDS_TUTORIAL_TALK37) );
							pMob->SetMobChatMsg( strTemp );

							if (g_pTcpIp)
								g_pTcpIp->SendNetMessage("tutorial 12 0\n"); // ���� ���� �ʱ�ȭ ��û
						}
					}
					break;
				}
			}
		}

		// else �� �ƴ� ������ ���� üũ�� ��ġ�� �ٷ� ���� üũ�ϱ� ���ؼ�.

		if( m_nPointPhase == 1 && (GetKeyState(VK_SHIFT) & 0x80) && pCMyApp->m_pUIMgr->m_whichShift == 1 ) // ��û�� ��Ȳ. ����Ʈ ���� üũ.
		{
			//------------------------------------------------------------------------
			// ��ü ���� �߿��� Ŭ���� ���� ã�Ƽ� ó��.
			//------------------------------------------------------------------------
			CNkMob *pMob = NULL;
			LHMobMap_it prMob;

			for (prMob = pCMyApp->m_pUIMgr->m_mob_map.begin(); prMob != pCMyApp->m_pUIMgr->m_mob_map.end(); prMob++)
			{
				pMob = (*prMob).second;
				// Color Pickng Buffer �� ���� Color �� �����ϴٸ� ( ���õǾ��ٸ� )
				if (pMob->m_wColorIdx == pCMyApp->m_pUIMgr->GetPickColor() )
				{
					// NPC �� Ŭ�� �Ǿ��ٸ� return 1;
					if( pMob->m_nMobNum == 57 ) // ��� ���� Ŭ����.
					{
						CNkCharacter *pCha = new CNkCharacter(); // ���Ƿ� ���� ĳ���͸� �����

						if( pCha )
						{
							// RShiftMenu�� ���µ� �ʿ��� ������ �������ش�.
							pCha->SetInitPos(pMob->m_wx, pMob->m_wz);
							pCha->m_Race = RACE_BULKAN; // ���Ƿ� ��ĭ
							pCha->SetInfo( pMob->m_nVirId, pMob->GetMobName() );

							g_pNk2DFrame->ShowRShiftMenu( pCha );

							SAFE_DELETE( pCha );
						}
					}

					return 1; // Ŭ���� ���۾��ϵ���.
				}
			}
		}

		break;

	case WM_LBUTTONUP:

		x = LOWORD (lParam);
		y = HIWORD (lParam);

		if( m_nPointPhase == 1 ) // ��û�� ��Ȳ.
		{
			if( g_pNk2DFrame->IsRShiftVisible() && g_pNk2DFrame->GetRShiftMenu()->IsInside(x,y) ) // Ȥ�� ��ûâ�� ���̴� ���¶��..
			{
				if( g_pNk2DFrame->GetRShiftMenu()->m_pBtn[CRShiftMenu::eExchange] && g_pNk2DFrame->GetRShiftMenu()->m_pBtn[CRShiftMenu::eExchange]->IsInside(x,y) ) // ��ư2���� ������...
				{
					++m_nPointPhase; // ��û�� �� ���·� �ٲ��ش�.

					// ��ȯâ ���� ó��.
					g_pNk2DFrame->ShowExchangeWindow( TRUE, TRUE );

					CNkMob *pMob = NULL;
					LHMobMap_it prMob;

					for (prMob = pCMyApp->m_pUIMgr->m_mob_map.begin(); prMob != pCMyApp->m_pUIMgr->m_mob_map.end(); prMob++)
					{
						pMob = (*prMob).second;
						if( pMob->m_nMobNum == 57 ) // ��� ���� Ŭ����.
						{
							g_pNk2DFrame->GetControlExchange()->SetOppName( pMob->GetMobName() );
							g_pNk2DFrame->GetControlExchange()->SetOppLevel( 500 );
							g_pNk2DFrame->GetControlExchange()->ExchTrade();
						}
					}

					return 1;
				}

				g_pNk2DFrame->CloseAllWindow(); // RShiftMenu �� ���̴� ���¿��� ���콺 Ŭ�������� RShiftMenu�� �ݾ��ش�.

			}
		}
		else if( m_nPointPhase == 2 ) // ��ȯâ�� ������Ȳ.
		{
			if( g_pNk2DFrame->IsExchangeVisible() && g_pNk2DFrame->GetControlExchange()->IsInside(x,y) ) // Ȥ�� ��ûâ�� ���̴� ���¶��..
			{
				if( g_pNk2DFrame->GetControlExchange()->m_ExgBtn2.IsInside(x,y) ) // ��ư2���� ������...
				{
					// ��ȯâ���� �����⸦ ã�´�.
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

					g_pNk2DFrame->GetControlExchange()->m_bTrade = TRUE; // ������ ��ȯ�� �����ϵ���..
					g_pNk2DFrame->GetControlExchange()->ExchChange(); // ������ ����Ʈ�� �ٲ�ġ�� �Ѵ�.
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
			g_pTcpIp->SendNetMessage("tutorial 12 1\n"); // �������� ���� ��û

		return TRUE;
	}

	return FALSE;
}


// Ʃ�丮�� ��ȭ/��Ʋ��� ü��.
CTutorialPeaceModeAct::CTutorialPeaceModeAct()
{
	m_dwOldElapsed = 0;
	m_dwStartTick = timeGetTime();

	m_nPointPhase = 0; // ���� ����Ƚ���� ���.
	m_bPrevMode = g_pRoh->GetCombat();

	m_bEndFlag = FALSE;
}

CTutorialPeaceModeAct::~CTutorialPeaceModeAct()
{
}

bool CTutorialPeaceModeAct::Render()
{
	char strTemp[1024];


	// �ֱ������� ��������.
	if( timeGetTime()%10000 < 50 )
	{
		CNkMob *pMob = NULL;
		LHMobMap_it prMob;
		for (prMob = pCMyApp->m_pUIMgr->m_mob_map.begin(); prMob != pCMyApp->m_pUIMgr->m_mob_map.end(); prMob++)
		{
			pMob = (*prMob).second;
			if( pMob->m_nMobNum == 57 ) // ��� ����
			{
				// IDS_TUTORIAL_TALK38 : �������� ��ȭ��带 ��� ��ȯ�� ���Ŷ�.
				sprintf( strTemp, (char *)G_STRING(IDS_TUTORIAL_TALK38) );
				pMob->SetMobChatMsg( strTemp );
				break;
			}
		}
	}

	if( m_bPrevMode != g_pRoh->GetCombat() ) // Ȥ�� ��尡 �ٲ���ٸ�..
	{
		++m_nPointPhase; // ����Ƚ�� ����.

		if( g_pRoh->GetCombat() ) // �ٲ�� ������°� ����
			// IDS_TUTORIAL_TALK39 : �ٷ� �� ���°� ��������.
			sprintf( strTemp, (char *)G_STRING(IDS_TUTORIAL_TALK39) );
		else
			// IDS_TUTORIAL_TALK40 : �ٷ� �� ���°� ��ȭ����.
			sprintf( strTemp, (char *)G_STRING(IDS_TUTORIAL_TALK40) );

		CNkMob *pMob = NULL;
		LHMobMap_it prMob;
		for (prMob = pCMyApp->m_pUIMgr->m_mob_map.begin(); prMob != pCMyApp->m_pUIMgr->m_mob_map.end(); prMob++)
		{
			pMob = (*prMob).second;
			if( pMob->m_nMobNum == 57 ) // ��� ����
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
	if( m_nPointPhase > 4 ) // ������ �̻� �غ�����...
	{
		return TRUE;
	}

	return FALSE;
}


// Ʃ�丮�� ����. (��Ŷ����������...)
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
	// Ʃ�丮�� ����� ��Ƽ�� ��ü���ش�.
	if( g_pRoh->m_bIsParty ) // ��Ƽ�� ���� ��쿡�� ó��
	{
		g_pNk2DFrame->GetControlParty()->RemoveMemberAll();
		g_pNk2DFrame->GetControlParty()->SetParty( FALSE );
		g_pRoh->m_bIsParty = FALSE;
	}

	if (g_pTcpIp)
		g_pTcpIp->SendNetMessage("tutorial 0\n"); // Ʃ�丮�� �Ϸ���Ŷ

	return TRUE; // �ٷ� ������..
}

/* C&P ��.
// Ʃ�丮�� ����Ʈ �� ü��.
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