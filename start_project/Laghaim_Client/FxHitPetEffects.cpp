#include "stdafx.h"
#include "sfx.h"
#include "main.h"
#include "ImageDefine.h"



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFxHitPetEffects::CFxHitPetEffects()
{
	hit					= NULL;
	hit_light			= NULL;
	m_fScale			= 0.0f;
	m_fFade				= 1.0f;
	m_nTotalFrame		= FX_HITPETEFFECTS_FRAMECOUNT;

	Create();
}

CFxHitPetEffects::~CFxHitPetEffects()
{
	SAFE_DELETE(hit);
	SAFE_DELETE(hit_light);
}

void CFxHitPetEffects::Create()
{
	SAFE_DELETE(hit);
	SAFE_DELETE(hit_light);

	hit = new CRectPlane;
	if( hit )
		hit->Create( 4.0f, 4.0f, FALSE );

	hit_light = new CRectPlane;
	if( hit_light )
		hit_light->Create( 8.0f, 8.0f, FALSE );
}


void CFxHitPetEffects::LoadRes(IndexedTexture *pIndexedTexture)
{}
void CFxHitPetEffects::DeleteRes()
{}
void CFxHitPetEffects::SetTextureNum()
{}

BOOL CFxHitPetEffects::Render(EffectSort &effect_sort)
{
	if( !GET_D3DDEVICE() )
		return true;

	if(effect_sort.nCurFrame >= m_nTotalFrame)
	{
		m_fScale = 0.0f;
		m_fFade = 1.0f;
		return TRUE;
	}

	float maxScale = 3.0f;

	m_fScale = (maxScale * effect_sort.nCurFrame) / m_nTotalFrame;
	m_fFade = 1.0f - (m_fScale * 0.3f);

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND,   D3DBLEND_ONE );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND,  D3DBLEND_ONE );

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

	float red, green, blue;

	if( hit )
	{
		red = 1.0f - (1.0f * m_fScale * 0.15f);
		green = 0.3f - (0.3f * m_fScale * 0.15f);
		blue = 0.3f - (0.3f * m_fScale * 0.15f);
		// nate 2004 - 7 - image
		if( g_pCapsyongTexture )
			GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_HITPET ] ) );
		hit->SetColor(red, green, blue, m_fFade);
		hit->ReCreate(m_fScale * 0.25f);
		hit->SetBillBoard(vViewVec, FALSE, 0.0f);
		hit->Scale(m_fScale, m_fScale, 0.0f);
		hit->SetTranslationXYZ(vDrawPos.x, vDrawPos.y, vDrawPos.z);
		hit->Render();
	}

	if( hit_light )
	{
		red = 0.8f - (0.8f * m_fScale * 0.125f);
		green = 0.8f - (0.8f * m_fScale * 0.125f);
		blue = 1.0f - (1.0f * m_fScale * 0.125f);

		// nate 2004 - 7 - image
		if( g_pCapsyongTexture )
			GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_HITPET_LIGHT ] ) );

		hit_light->SetColor(red, green, blue, m_fFade);
		hit_light->ReCreate(-m_fScale * 0.25f);
		hit_light->SetBillBoard(vViewVec, FALSE, 0.0f);
		hit_light->Scale(m_fScale, m_fScale, 0.0f);
		hit_light->SetTranslationXYZ(vDrawPos.x, vDrawPos.y, vDrawPos.z);
		hit_light->Render();
	}

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE);

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState[2] );

	return FALSE;
}

