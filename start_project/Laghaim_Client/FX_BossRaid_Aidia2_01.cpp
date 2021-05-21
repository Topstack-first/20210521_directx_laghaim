#include "stdafx.h"

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
#include "FX_BossRaid_Aidia2_01.h"

CFxBossRaidAidia2_01::CFxBossRaidAidia2_01()
{
	m_pLolo = NULL;

	if (!m_pLolo) // 하늘에서 떨어지는 기 형태의 검
	{
		m_pLolo = new CLolos;
		m_pLolo->SetIndexedTexture( g_pCapsyongTexture );
		m_pLolo->Read("data/effect/lolos/eff_adia02_attack01.LOL", TRUE);
	}





}

CFxBossRaidAidia2_01::~CFxBossRaidAidia2_01()
{
//	DeleteRes();
}

void CFxBossRaidAidia2_01::LoadRes()
{

}

void CFxBossRaidAidia2_01::DeleteRes()
{
	SAFE_DELETE(m_pLolo);
}

bool CFxBossRaidAidia2_01::Render(EffectSort &effect_sort)
{
	//변수 설정
	DWORD srcBlendOld, destBlendOld;
	DWORD alphaBlend, dwLighting, dwAmbient, dwFogEnable;

	// 상태값 보존.
	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_FOGENABLE, &dwFogEnable );
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, &alphaBlend );
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_LIGHTING, &dwLighting );
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_AMBIENT, &dwAmbient);

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_FOGENABLE, FALSE );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, TRUE);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_AMBIENT, 0xffffff );

	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_SRCBLEND, &srcBlendOld );
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_DESTBLEND, &destBlendOld );

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND, D3DBLEND_ONE );

	if (effect_sort.nCurFrame > 18 && effect_sort.nCurFrame <= 30 )
	{
		D3DMATRIX matWorld, matTrans, matRotate;

		D3DUtil_SetRotateYMatrix(matRotate, -effect_sort.pMobFrom->m_dir);
		D3DUtil_SetTranslateMatrix( matTrans, effect_sort.pMobFrom->m_wx, effect_sort.pMobFrom->m_wy, effect_sort.pMobFrom->m_wz);
		D3DMath_MatrixMultiply( matWorld, matRotate, matTrans );

		GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_WORLD,	&matWorld );
		m_pLolo->Render(GET_D3DDEVICE(), effect_sort.nCurFrame);
	}

	//상태 복원
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_FOGENABLE, dwFogEnable );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, alphaBlend );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_AMBIENT, dwAmbient);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, dwLighting );

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND, srcBlendOld);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND, destBlendOld);

	return FALSE;
}
