#include "stdafx.h"
// FxTornadoHammer.cpp: implementation of the CFxBossRaidAidia3_02 class.
//
//////////////////////////////////////////////////////////////////////

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
#include "FxStoneRance.h"
#include "FX_BossRaid_Aidia3_02.h"

#define STONE_NUM 20
#define STONE_RANGE 100.0f



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFxBossRaidAidia3_02::CFxBossRaidAidia3_02()
{
	m_pLolo = NULL;
}

CFxBossRaidAidia3_02::~CFxBossRaidAidia3_02()
{
	SAFE_DELETE(m_pLolo);
}

void CFxBossRaidAidia3_02::LoadRes()
{
	if (!m_pLolo)
	{
		m_pLolo = new CLolos;
		m_pLolo->SetIndexedTexture( g_pCapsyongTexture );
		m_pLolo->Read("data/effect/lolos/eff_adia_attack02.LOL");
	}



}

void CFxBossRaidAidia3_02::DeleteRes()
{}

BOOL CFxBossRaidAidia3_02::Render(EffectSort &effect_sort)
{
	if( !GET_D3DDEVICE() )
		return TRUE;

	if( effect_sort.pMobFrom->GetMotionState() != MOB_EMPTY1 )
		return TRUE;

	if( effect_sort.nCurFrame > FX_BOSSRAID_AIDIA3_02_FRAMECOUNT ) // 이펙트 제거 처리
		return TRUE;

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND,   D3DBLEND_SRCALPHA );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND,  D3DBLEND_ONE);

	DWORD  dwTextureState[3];

	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG1, &dwTextureState[0] );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG2, &dwTextureState[1] );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAOP,   &dwTextureState[2] );

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );

	D3DMATRIX matTrans, matRotate, matWorld, matS;

	effect_sort.nCurFrame = effect_sort.pMobFrom->GetCurFrame();

	if(m_pLolo)
	{
		D3DUtil_SetRotateYMatrix(matRotate, -effect_sort.pMobFrom->m_dir);
		D3DUtil_SetTranslateMatrix( matTrans, effect_sort.pMobFrom->m_wx, effect_sort.pMobFrom->m_wy, effect_sort.pMobFrom->m_wz);
		D3DMath_MatrixMultiply( matWorld, matRotate, matTrans );
		GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_WORLD, &matWorld );
		m_pLolo->Render(GET_D3DDEVICE(), effect_sort.nCurFrame, FALSE, FALSE);
	}

	if( effect_sort.nCurFrame >= 56 )
	{
		if( effect_sort.num == 0 )
			effect_sort.num = 1;
	}

	if( effect_sort.num == 1 )
	{
		float distance_X[STONE_NUM];
		float distance_Z[STONE_NUM];

		for( int i = 0; i < STONE_NUM; i++ )
		{
			distance_X[i] = RandomNum(-STONE_RANGE, STONE_RANGE);
			distance_Z[i] = RandomNum(-STONE_RANGE, STONE_RANGE);

			if( distance_X[i] == 0.0f )
			{
				distance_X[i] = RandomNum(-STONE_RANGE, STONE_RANGE);
			}

			if( distance_Z[i] == 0.0f )
			{
				distance_Z[i] = RandomNum(-STONE_RANGE, STONE_RANGE);
			}

			D3DVECTOR vDrawPos;

			vDrawPos = D3DVECTOR( effect_sort.pMobFrom->m_wx + distance_X[i],
								  effect_sort.pMobFrom->m_wy,
								  effect_sort.pMobFrom->m_wz + distance_Z[i] );

			EffectSort effect_sort2;
			ZeroMemory(&effect_sort2, sizeof(EffectSort));
			effect_sort2.nType		= FX_BOSSRAID_AIDIA3_02_STONE;
			effect_sort2.vPos		= vDrawPos;

			pCMyApp->m_pFxSet->InsertFxEffect(effect_sort2);
			effect_sort.num = 2;
		}
	}

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState[2] );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE);

	return FALSE;
}