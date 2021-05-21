#include "stdafx.h"
#include "sfx.h"
#include "main.h"
#include "NkCharacter.h"
#include "ImageDefine.h"



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFxKhanHitEffects::CFxKhanHitEffects()
{
	ring				= NULL;
	wave				= NULL;
	m_fScale			= 0.0f;
	m_fFade				= 1.0f;
	m_nTotalFrame		= FX_KHANHITEFFECTS_FRAMECOUNT;

	Create();
}

CFxKhanHitEffects::~CFxKhanHitEffects()
{
	SAFE_DELETE(ring);
	SAFE_DELETE(wave);
}

void CFxKhanHitEffects::Create()
{
	SAFE_DELETE(ring);
	SAFE_DELETE(wave);

	ring = new CCylinderPlane;
	if( ring )
		ring->Create( 5.5f, 10.5f, 0.0f, 20, 0.0f, FALSE, 10.0f );

	wave = new CCylinderPlane;
	if( wave )
		wave->Create( 4.0f, 12.0f, 0.0f, 20, 0.0f, FALSE, 20.0f );
}

void CFxKhanHitEffects::LoadRes(IndexedTexture *pIndexedTexture)
{}

void CFxKhanHitEffects::DeleteRes()
{}

BOOL CFxKhanHitEffects::Render(EffectSort &effect_sort)
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

	D3DVECTOR	vDrawPos;
	vDrawPos = D3DVECTOR(effect_sort.vTargetPos.x, effect_sort.vTargetPos.y, effect_sort.vTargetPos.z);

	float red, green, blue;

	if( ring )
	{
		red = 1.0f - (1.0f * m_fScale * 0.15f);
		green = 0.3f - (0.3f * m_fScale * 0.15f);
		blue = 0.3f - (0.3f * m_fScale * 0.15f);

		// nate 2004 - 7 - image
		if( g_pCapsyongTexture )
			GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_KHANHIT_RING ] ) );

		ring->SetColor(red, green, blue, m_fFade);
		ring->SetRotationY( g_PI * 0.5f );
		ring->RotateY( m_fScale * 20.0f );
		ring->Scale( m_fScale + 1.0f, 1.0f, m_fScale + 1.0f );
		ring->SetTranslationXYZ(vDrawPos.x, vDrawPos.y + 3.0f + (m_fScale*0.5f), vDrawPos.z);
		ring->Render();
	}


	if( wave )
	{
		red = 1.0f - (1.0f * m_fScale * 0.25f);
		green = 0.7f - (0.7f * m_fScale * 0.25f);
		blue = 0.7f - (0.7f * m_fScale * 0.25f);

		// nate 2004 - 7 - image
		if( g_pCapsyongTexture )
			GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_KHANHIT_WAVE ] ) );

		wave->SetColor(red, green, blue, m_fFade);
		wave->SetRotationY( g_PI * 0.5f );
		wave->Scale( m_fScale + 1.0f, 1.0f, m_fScale + 1.0f );
		wave->SetTranslationXYZ(vDrawPos.x, vDrawPos.y + 3.0f + m_fScale, vDrawPos.z);
		wave->Render();
	}

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE);

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState[2] );

	return FALSE;
}


