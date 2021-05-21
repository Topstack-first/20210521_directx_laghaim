#include "stdafx.h"
// FxTornadoHammer.cpp: implementation of the CFxBossRaidKai3_01 class.
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
#include "FX_BossRaid_Kai3_01.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFxBossRaidKai3_01::CFxBossRaidKai3_01()
{
	m_pLolo = NULL;
}

CFxBossRaidKai3_01::~CFxBossRaidKai3_01()
{
	SAFE_DELETE(m_pLolo);
}

void CFxBossRaidKai3_01::LoadRes()
{
	if (!m_pLolo)
	{
		m_pLolo = new CLolos;
		m_pLolo->SetIndexedTexture( g_pCapsyongTexture );
		m_pLolo->Read("data/effect/lolos/boss_k03_attack01_eff.LOL");
	}



}

void CFxBossRaidKai3_01::DeleteRes()
{}

BOOL CFxBossRaidKai3_01::Render(EffectSort &effect_sort)
{
	if( !GET_D3DDEVICE() )
		return TRUE;

	if( effect_sort.pMobFrom->GetMotionState() != MOB_ATTACK )
		return TRUE;

	if( effect_sort.nCurFrame > FX_BOSSRAID_KAI3_01_FRAMECOUNT ) // 이펙트 제거 처리
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

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState[2] );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE);

	return FALSE;
}