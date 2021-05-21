#include "stdafx.h"
#include <assert.h>
#include "sfx.h"
#include "main.h"
#include "Land.h"
#include "NkCharacter.h"
#include "NkMob.h"
#include "GUtil.h"
#include "DirtSnd.h"
#include "UIMgr.h"
#include "LoadEffectSound.h"
// nate 2004 - 7 - image
#include "ImageDefine.h"
#include "Fx_BossRaid_Human1_02.h"




extern int g_EffectSound[];
extern float g_fDSDistance;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFxBossRaidHuman1_02::CFxBossRaidHuman1_02()
{
	m_pLolo				= NULL;
}

CFxBossRaidHuman1_02::~CFxBossRaidHuman1_02()
{
	SAFE_DELETE(m_pLolo);
}

void CFxBossRaidHuman1_02::Create()
{}

// nate 2004 - 8 ImageManager
void CFxBossRaidHuman1_02::LoadRes()
{

	if (!m_pLolo)
	{
		m_pLolo = new CLolos;
		m_pLolo->SetIndexedTexture( g_pCapsyongTexture );
		m_pLolo->Read("data/effect/lolos/eff_missile.LOL");
	}



}

void CFxBossRaidHuman1_02::DeleteRes()
{}

BOOL CFxBossRaidHuman1_02::Render(EffectSort &ef_sort)
{
	if( !GET_D3DDEVICE() )
		return true;

	D3DVECTOR	vCurPos;

	CNkMob* pMob = ef_sort.pMobFrom;

	switch( ef_sort.num )
	{
	case 0: // 대기상태
		if(  pMob->m_nMobNum == 514 && pMob->GetMotionState() == MOB_EMPTY1 && pMob->GetCurFrame() >= 26 )
		{
			//////////////////////////////////////////////////////////////////////////
			// 프레임 계산

			ef_sort.vPos = ef_sort.vTargetPos - ef_sort.vCurPos;			// 목표를 향하게 하는 방향 백터 만든다.
			ef_sort.vPos = Normalize( ef_sort.vPos );						// 노멀 방향 백터로 만든다.

			ef_sort.vSortPos.x = (float)atan2(ef_sort.vPos.z, ef_sort.vPos.x); // 목표 방향 값

			D3DVECTOR vecTemp = ef_sort.vPos * 20.0f;
			vCurPos = D3DVECTOR(0.0f, 0.0f, 0.0f);
			vCurPos.x = ef_sort.vCurPos.x	+= vecTemp.x;
			vCurPos.z = ef_sort.vCurPos.z	+= vecTemp.z;
			float dist = DistVecToVec( ef_sort.vTargetPos, vCurPos );

			ef_sort.vSortPos.y = (dist - 20.0f) / (float)400 ;

			EffectSort effect_sort;
			ZeroMemory(&effect_sort, sizeof(EffectSort));
			effect_sort.nType		= FX_FIREBALLBOOM;
			effect_sort.vPos		= D3DVECTOR(vCurPos.x, pCMyApp->m_pUIMgr->m_pLand->GetHFHeight(vCurPos.x, vCurPos.z)+20.0f, vCurPos.z);
			effect_sort.vTargetPos	= D3DVECTOR(vCurPos.x, pCMyApp->m_pUIMgr->m_pLand->GetHFHeight(vCurPos.x, vCurPos.z)+20.0f, vCurPos.z);;
			pCMyApp->m_pFxSet->InsertFxEffect(effect_sort);

//			if (g_pDSound)
//				g_pDSound->Play(g_EffectSound[EFF_SND_EXPLO],// 0,0,0);
//								(vCurPos.x - g_pRoh->m_wx)/(g_fDSDistance*2.0f),
//								0.0f, //(vCurPos.y - g_pRoh->m_wy)/g_fDSDistance,
//								(vCurPos.z - g_pRoh->m_wz)/(g_fDSDistance*2.0f));


			ef_sort.num = 1;
			ef_sort.dwColor	= timeGetTime();
		}

		break;

	case 1: // 발사 및 1차 폭팔
		{
			//////////////////////////////////////////////////////////////////////////
			// 프레임 계산

			// 시간으로 계산시 나아간 거리 계산
			DWORD dwTime = timeGetTime() - ef_sort.dwColor ;

			// 랜더할 거리 계산
			vCurPos			= D3DVECTOR(0.0f, 0.0f, 0.0f);
			D3DVECTOR vCur	= ef_sort.vPos * ( ((float)dwTime) * ef_sort.vSortPos.y ); // 시간당 얼마를 가는지 계산
			vCurPos.x		= ef_sort.vCurPos.x + vCur.x;
			vCurPos.z		= ef_sort.vCurPos.z + vCur.z;

			//////////////////////////////////////////////////////////////////////////
			// Render

			DWORD dwLighting;

			D3DMATRIX matTrans;
			D3DMATRIX matRotate;
			D3DMATRIX matScale;
			D3DMATRIX matWorld;

			GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_LIGHTING, &dwLighting);
			GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE);

			GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND,   D3DBLEND_SRCALPHA );
			GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND,  D3DBLEND_INVSRCALPHA );

//			GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_LIGHTING, &dwLighting );
//			GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_ZWRITEENABLE, &dwZWrite );
///			GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_SRCBLEND, &dwSrc );
//			GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_DESTBLEND, &dwDest );

//			GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ZWRITEENABLE, TRUE );
//			GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE );
//			GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND, D3DBLEND_ONE );
//			GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND,   D3DBLEND_SRCALPHA );

			//////////////////////////////////////////////////////////////////////////

			D3DUtil_SetIdentityMatrix(matTrans);
			D3DUtil_SetIdentityMatrix(matRotate);
			D3DUtil_SetIdentityMatrix(matScale);

			// 시간으로 계산시
			D3DUtil_SetTranslateMatrix( matTrans, vCurPos.x,
										pCMyApp->m_pUIMgr->m_pLand->GetHFHeight(vCurPos.x, vCurPos.z) + 42.0f,
										vCurPos.z );
			Nk3DUtil_SetRotateCCWYMatrix( matRotate, ef_sort.vSortPos.x ); // vSortPos.x를 방향값으로 사용.
			D3DMath_MatrixMultiply( matWorld, matRotate, matTrans );

			GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_WORLD, &matWorld );

			m_pLolo->Render2(GET_D3DDEVICE(), -1, 0, FALSE, FALSE, FALSE, &matWorld );

			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_ONE);
			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_ONE);


			GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, dwLighting);

//			GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, dwLighting );
//			GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ZWRITEENABLE, dwZWrite );
//			GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND, dwSrc );
//			GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND, dwDest );

			//////////////////////////////////////////////////////////////////////////
			// 종료 시점 계산
			float dist = DistVecToVec( ef_sort.vTargetPos, vCurPos ); // 목표지접 과 미사일의 거리 측정

			if( dist <= 5.0f || dwTime > 450 ) // 목표거리 와 일정하게 가까워지면 폭팔 시킨다.
			{
				vCurPos.y = pCMyApp->m_pUIMgr->m_pLand->GetHFHeight(vCurPos.x, vCurPos.z)+42.0f;

				EffectSort effect_sort;
				ZeroMemory(&effect_sort, sizeof(EffectSort));
				effect_sort.nType		= FX_LIGHTBALLBOOM;
				effect_sort.vPos		= vCurPos;
				effect_sort.vTargetPos	= vCurPos;
				pCMyApp->m_pFxSet->InsertFxEffect(effect_sort);
				if (g_pDSound)
					g_pDSound->Play(g_EffectSound[EFF_SND_GUN_2],
									(vCurPos.x - g_pRoh->m_wx)/(g_fDSDistance*2.0f),
									0.0f, //(vCurPos.y - g_pRoh->m_wy)/g_fDSDistance,
									(vCurPos.z - g_pRoh->m_wz)/(g_fDSDistance*2.0f));

				ef_sort.num = 2;
				ef_sort.dwColor = timeGetTime();
			}
		}
		break;


	case 2: // 최종 2차 폭팔
		{
			DWORD dwTime = timeGetTime() - ef_sort.dwColor ;

			if( dwTime >= 1300 )
			{
				return TRUE;
			}
			else if( dwTime >= 250 )
			{
				D3DVECTOR vTemp = ef_sort.vTargetPos;

				vTemp.x += (float)(rand()%50 + 5) * (float)( ((rand()%2) == 0)? -1.0f : 1.0f );
				vTemp.z += (float)(rand()%50 + 5) * (float)( ((rand()%2) == 0)? -1.0f : 1.0f );
				vTemp.y = pCMyApp->m_pUIMgr->m_pLand->GetHFHeight(vTemp.x, vTemp.z) ;

				EffectSort effect_sort;
				ZeroMemory(&effect_sort, sizeof(EffectSort));
				effect_sort.nType		= FX_FIREBALLBOOM2;
				effect_sort.vPos		= vTemp;
				effect_sort.vTargetPos	= vTemp;
				pCMyApp->m_pFxSet->InsertFxEffect(effect_sort);
				if (g_pDSound)
					g_pDSound->Play(g_EffectSound[EFF_SND_BOOM1],
									(vTemp.x - g_pRoh->m_wx)/(g_fDSDistance*2.0f),
									0.0f, //(vTemp.y - g_pRoh->m_wy)/g_fDSDistance,
									(vTemp.z - g_pRoh->m_wz)/(g_fDSDistance*2.0f));
			}
		}
		break;
	}
	return FALSE;
}



