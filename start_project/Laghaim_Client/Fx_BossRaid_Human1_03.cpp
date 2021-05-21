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
#include "Fx_BossRaid_human1_03.h"
#include "NkMob.h"
#include "UIMgr.h"
#include "Land.h"





extern int					g_EffectSound[];
extern float				g_fDSDistance;


CFxBossRaidHuman1_03::CFxBossRaidHuman1_03()
	: m_BeforTime(0)
{
	m_pLoloR = NULL;
	m_pLoloL = NULL;
}


CFxBossRaidHuman1_03::~CFxBossRaidHuman1_03()
{
	DeleteRes();
}

void CFxBossRaidHuman1_03::LoadRes()
{
	PFILE *fpLolo = NULL;

	if (!m_pLoloL) // 땅을 가르며 나아가는 lolo 오브젝트
	{
		m_pLoloL = new CLolos;
		m_pLoloL->SetIndexedTexture( g_pCapsyongTexture );
		m_pLoloL->Read("data/effect/lolos/eff_knife_L.LOL");
	}

	if (!m_pLoloR) // 땅을 가르며 나아가는 lolo 오브젝트
	{
		m_pLoloR = new CLolos;
		m_pLoloR->SetIndexedTexture( g_pCapsyongTexture );
		m_pLoloR->Read("data/effect/lolos/eff_knife_R.LOL");
	}




	m_BeforTime = 0;
}


void CFxBossRaidHuman1_03::DeleteRes()
{
	SAFE_DELETE( m_pLoloL );
	SAFE_DELETE( m_pLoloR );
}


void CFxBossRaidHuman1_03::FrameMove( EffectSort &ef_sort, int nAddFrame )
{
	int i=0;

	EffectSort* pEfSortR = (EffectSort*)ef_sort.effects;
	switch(ef_sort.num)
	{
	case 0: // 에펙트 대기
		{
			CNkMob* pMob = ef_sort.pMobFrom;

			if( pMob->m_nMobNum == 514 && pMob->GetMotionState() == MOB_EMPTY2 && pMob->GetCurFrame() >= 28 ) // MOB_EMPTY1 == attack2 동작
			{
				//////////////////////////////////////////////////////////////////////////
				// left

				D3DMATRIX matRot;
				D3DVECTOR vTmp;

				D3DUtil_SetIdentityMatrix(matRot);
				Nk3DUtil_SetRotateCCWYMatrix(matRot,pMob->m_dir);
				D3DMath_VectorMatrixMultiply(vTmp, pMob->m_pCapSyong->m_LineVector[1], matRot);

				ef_sort.vCurPos = vTmp;
				ef_sort.vCurPos.x += pMob->m_wx;
				ef_sort.vCurPos.y += pMob->m_wy;
				ef_sort.vCurPos.z += pMob->m_wz;

				ef_sort.vTargetPos = D3DVECTOR( ef_sort.pNkChaTo->m_wx, ef_sort.pNkChaTo->m_wy, ef_sort.pNkChaTo->m_wz );

				ef_sort.vPos = ef_sort.vTargetPos - ef_sort.vCurPos;			// 목표를 향하게 하는 방향 백터 만든다.
				ef_sort.vPos = Normalize( ef_sort.vPos );						// 노멀 방향 백터로 만든다.
				ef_sort.vSortPos.x = (float)atan2(ef_sort.vPos.z, ef_sort.vPos.x); // 목표 방향 값

				ef_sort.vPos *= 5.0f;

				ef_sort.vCurPos.x	+= ef_sort.vPos.x;
				ef_sort.vCurPos.y	+= ef_sort.vPos.y;
				ef_sort.vCurPos.z	+= ef_sort.vPos.z;

				//////////////////////////////////////////////////////////////////////////
				// right

				vTmp = D3DVECTOR(0.0f, 0.0f, 0.0f);
				D3DUtil_SetIdentityMatrix(matRot);
				Nk3DUtil_SetRotateCCWYMatrix(matRot,pMob->m_dir);
				D3DMath_VectorMatrixMultiply(vTmp, pMob->m_pCapSyong->m_LineVector[0], matRot);

				pEfSortR->vCurPos = vTmp;
				pEfSortR->vCurPos.x += pMob->m_wx;
				pEfSortR->vCurPos.y += pMob->m_wy;
				pEfSortR->vCurPos.z += pMob->m_wz;

				pEfSortR->vTargetPos = D3DVECTOR( ef_sort.pNkChaTo->m_wx, ef_sort.pNkChaTo->m_wy, ef_sort.pNkChaTo->m_wz );

				pEfSortR->vPos = pEfSortR->vTargetPos - pEfSortR->vCurPos;			// 목표를 향하게 하는 방향 백터 만든다.
				pEfSortR->vPos = Normalize( pEfSortR->vPos );						// 노멀 방향 백터로 만든다.
				pEfSortR->vSortPos.x = (float)atan2(pEfSortR->vPos.z, pEfSortR->vPos.x); // 목표 방향 값

				pEfSortR->vPos *= 5.0f;

				pEfSortR->vCurPos.x	+= pEfSortR->vPos.x;
				pEfSortR->vCurPos.y	+= pEfSortR->vPos.y;
				pEfSortR->vCurPos.z	+= pEfSortR->vPos.z;

				//////////////////////////////////////////////////////////////////////////

				ef_sort.num = 1;
				ef_sort.dwColor = timeGetTime();
			}
		}
		break;

	case 1: // 이펙트 시작
		{
			float fSpeed = 15.0f;
			//////////////////////////////////////////////////////////////////////////
			// left
			ef_sort.vTargetPos = D3DVECTOR( ef_sort.pNkChaTo->m_wx, ef_sort.pNkChaTo->m_wy, ef_sort.pNkChaTo->m_wz );

			float dist = DistVecToVec( ef_sort.vTargetPos, ef_sort.vCurPos );

			DWORD	dwTime = timeGetTime() - ef_sort.dwColor;

			if(dist <= 10.0f )
				ef_sort.num = 2;

			// 랜더할 거리 계산
			ef_sort.vPos = ef_sort.vTargetPos - ef_sort.vCurPos;			// 목표를 향하게 하는 방향 백터 만든다.
			ef_sort.vPos = Normalize( ef_sort.vPos );
			ef_sort.vSortPos.x = (float)atan2(ef_sort.vPos.z, ef_sort.vPos.x); // 목표 방향 값

			ef_sort.vPos *= fSpeed; // 시간당 얼마를 가는지 계산
			ef_sort.vCurPos.x += ef_sort.vPos.x;
			ef_sort.vCurPos.y += ef_sort.vPos.y;
			ef_sort.vCurPos.z += ef_sort.vPos.z;

			//////////////////////////////////////////////////////////////////////////
			// right

			pEfSortR->vTargetPos = D3DVECTOR( ef_sort.pNkChaTo->m_wx, ef_sort.pNkChaTo->m_wy, ef_sort.pNkChaTo->m_wz );

			dist = DistVecToVec( pEfSortR->vTargetPos, pEfSortR->vCurPos );

			// 랜더할 거리 계산
			pEfSortR->vPos = pEfSortR->vTargetPos - pEfSortR->vCurPos;			// 목표를 향하게 하는 방향 백터 만든다.
			pEfSortR->vPos = Normalize( pEfSortR->vPos );
			pEfSortR->vSortPos.x = (float)atan2(pEfSortR->vPos.z, pEfSortR->vPos.x); // 목표 방향 값

			pEfSortR->vPos *= fSpeed; // 시간당 얼마를 가는지 계산
			pEfSortR->vCurPos.x += pEfSortR->vPos.x;
			pEfSortR->vCurPos.y += pEfSortR->vPos.y;
			pEfSortR->vCurPos.z += pEfSortR->vPos.z;


		}
		break;

	case 2: // v파티클이 끝나길 기다리는
		{
			D3DVECTOR vTemp = ef_sort.vTargetPos;
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

			ef_sort.num = 3;
		}
		break;

	case 3:
		// 이 조건에 들어오면 이펙트를 삭제한다.
		break;
	}
}



BOOL CFxBossRaidHuman1_03::Render(EffectSort &ef_sort)
{
	if( ef_sort.num == 3 )
		return TRUE;

	if( !GET_D3DDEVICE() || ef_sort.num != 1)
		return FALSE;

	EffectSort* pEfSortR = (EffectSort*)ef_sort.effects;

	DWORD dwLighting, dwZWrite, dwSrc, dwDest;

	D3DMATRIX matTrans;
	D3DMATRIX matRotate;
	D3DMATRIX matScale;
	D3DMATRIX matWorld;

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
	D3DUtil_SetIdentityMatrix(matWorld);

	// 시간으로 계산시
	D3DUtil_SetTranslateMatrix( matTrans, pEfSortR->vCurPos.x, pEfSortR->vCurPos.y, pEfSortR->vCurPos.z );
	Nk3DUtil_SetRotateCCWYMatrix( matRotate, pEfSortR->vSortPos.x ); // vSortPos.x를 방향값으로 사용.
	D3DMath_MatrixMultiply( matWorld, matRotate, matTrans );

	GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_WORLD, &matWorld );

	m_pLoloR->Render(GET_D3DDEVICE(), ef_sort.nCurFrame, FALSE, FALSE, FALSE, &matWorld);

	//m_pLoloL->Render2(GET_D3DDEVICE(), -1, 1, FALSE, FALSE, FALSE, &matWorld );

	//////////////////////////////////////////////////////////////////////////

	D3DUtil_SetIdentityMatrix(matTrans);
	D3DUtil_SetIdentityMatrix(matRotate);
	D3DUtil_SetIdentityMatrix(matScale);
	D3DUtil_SetIdentityMatrix(matWorld);

	// 시간으로 계산시
	D3DUtil_SetTranslateMatrix( matTrans, ef_sort.vCurPos.x, ef_sort.vCurPos.y, ef_sort.vCurPos.z );
	Nk3DUtil_SetRotateCCWYMatrix( matRotate, ef_sort.vSortPos.x ); // vSortPos.x를 방향값으로 사용.
	D3DMath_MatrixMultiply( matWorld, matRotate, matTrans );

	GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_WORLD, &matWorld );

	m_pLoloL->Render(GET_D3DDEVICE(), ef_sort.nCurFrame, FALSE, FALSE, FALSE, &matWorld);

	//m_pLoloL->Render2(GET_D3DDEVICE(), -1, 1, FALSE, FALSE, FALSE, &matWorld );

	//////////////////////////////////////////////////////////////////////////

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, dwLighting );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ZWRITEENABLE, dwZWrite );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND, dwSrc );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND, dwDest );

	return FALSE;
}

