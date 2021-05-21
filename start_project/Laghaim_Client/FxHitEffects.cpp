#include "stdafx.h"
#include "sfx.h"
#include "main.h"
#include "ImageDefine.h"



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFxHitEffects::CFxHitEffects()
{
	hit					= NULL;
	m_fScale			= 0.0f;
	m_fFade				= 1.0f;
	m_nTotalFrame		= FX_HITEFFECTS_FRAMECOUNT;

	Create();
}

CFxHitEffects::~CFxHitEffects()
{
	SAFE_DELETE(hit);
}

void CFxHitEffects::Create()
{
	SAFE_DELETE(hit);

	hit = new CRectPlane;
	if( hit )
		hit->Create( 10.0f, 10.0f, FALSE );
}


void CFxHitEffects::LoadRes(IndexedTexture *pIndexedTexture)
{}
void CFxHitEffects::DeleteRes()
{}

void CFxHitEffects::SetTextureNum()
{}

BOOL CFxHitEffects::Render(EffectSort &effect_sort)
{
	if( !GET_D3DDEVICE() )
		return true;

	if(effect_sort.nCurFrame >= m_nTotalFrame)
	{
		m_fScale = 0.0f;
		m_fFade = 1.0f;
		return TRUE;
	}

	float maxScale = 2.0f;

	m_fScale = (maxScale * effect_sort.nCurFrame) / m_nTotalFrame;
	m_fFade = 1.0f - (m_fScale * 0.15f);

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE);

	// Set any appropiate state
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND,   D3DBLEND_SRCALPHA );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND,  D3DBLEND_INVSRCALPHA );

	DWORD  dwTextureState[3];

	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG1, &dwTextureState[0] );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG2, &dwTextureState[1] );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAOP,   &dwTextureState[2] );

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );

	D3DVECTOR	vViewVec;
	vViewVec = pCMyApp->GetEyeDirection();

	// °Å¸®¸¦ ¶³¾î¶ß¸°´Ù.
	D3DVECTOR		vAdj;
	vAdj = effect_sort.vTargetPos - effect_sort.vPos;
	vAdj = Normalize(vAdj);

	D3DVECTOR		vDrawPos;
	vDrawPos = D3DVECTOR( effect_sort.vTargetPos.x - (vAdj.x*5),
						  effect_sort.vTargetPos.y,
						  effect_sort.vTargetPos.z - (vAdj.z*5) );

	// nate 2004 - 7 - image
	if( g_pCapsyongTexture )
		GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_HITEFFECTS ] ) );

	if( hit )
	{
		hit->SetColor(1.0f, 1.0f, 1.0f, m_fFade);
		hit->SetBillBoard(vViewVec, FALSE, 0.0f);
		hit->Scale(m_fScale, m_fScale, 0.0f);
		hit->SetTranslationXYZ(vDrawPos.x, vDrawPos.y, vDrawPos.z);
		hit->Render();
	}

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE);

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState[2] );

	return FALSE;
}
