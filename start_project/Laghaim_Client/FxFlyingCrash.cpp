#include "stdafx.h"
// FxFlyingCrash.cpp: implementation of the CFxFlyingCrash class.
//
//////////////////////////////////////////////////////////////////////
#include "sfx.h"
#include "main.h"
#include "ImageDefine.h"
#include "NkCharacter.h"
#include "FxFlyingCrash.h"



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CFxFlyingCrash::CFxFlyingCrash()
{
	ground_ring			= NULL;
	ground_corona		= NULL;
	m_fScale			= 0.0f;
	m_fFade				= 1.0f;
	m_nTotalFrame		= FX_FLYINGCRASH_FRAMECOUNT;
}

CFxFlyingCrash::~CFxFlyingCrash()
{
	DeleteRes();
}

void CFxFlyingCrash::LoadRes()
{
	DeleteRes();

	ground_ring = new CRectPlane;
	if( ground_ring )
		ground_ring->Create( 25.0f, 25.0f, FALSE );
	ground_corona = new CRectPlane;
	if( ground_corona )
		ground_corona->Create( 25.0f, 25.0f, FALSE );
}

void CFxFlyingCrash::DeleteRes()
{
	SAFE_DELETE(ground_ring);
	SAFE_DELETE(ground_corona);
}

BOOL CFxFlyingCrash::Render(EffectSort &effect_sort)
{
	if( !GET_D3DDEVICE() )
		return true;

	if(effect_sort.nCurFrame >= 5)
	{
		m_fScale = 0.0f;
		m_fFade = 1.0f;
		return TRUE;
	}

	float maxScale = 2.0f;

	m_fScale = (maxScale * effect_sort.nCurFrame) / 4;
	m_fFade = 1.0f - m_fScale;

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

	D3DVECTOR	vDrawVec;
	if( effect_sort.pNkChaFrom )
	{
		vDrawVec = D3DVECTOR(effect_sort.pNkChaFrom->m_wx,
							 effect_sort.pNkChaFrom->m_wy,
							 effect_sort.pNkChaFrom->m_wz);
	}
	float red, green, blue;
	red = 0.15f - (0.15f * m_fScale * 0.95f);
	green = 0.15f - (0.15f * m_fScale * 0.95f);
	blue = 1.0f - (1.0f * m_fScale * 0.95f);

	if( ground_corona )
	{
		GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_SHOWUP2_CORONA ] ) );

		ground_corona->SetColor(red, green, blue, m_fFade);
		ground_corona->SetRotationX( g_PI * 0.5f );
		ground_corona->RotateZ( m_fScale * 5.0f );
		ground_corona->Scale( m_fScale, m_fScale, 1.0f );
		ground_corona->SetTranslationXYZ(vDrawVec.x, vDrawVec.y + 1.5f, vDrawVec.z);
		ground_corona->Render();
	}
	if( ground_ring )
	{
		// nate 2004 - 7 - image
		GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_SHOWUP2_SHIELD ] ) );

		ground_ring->SetColor(red, green, blue, m_fFade);
		ground_ring->SetRotationX( g_PI * 0.5f );
		ground_ring->Scale( m_fScale * 0.6f, m_fScale * 0.6f, 1.0f );
		ground_ring->SetTranslationXYZ(vDrawVec.x, vDrawVec.y + 1.5f, vDrawVec.z);
		ground_ring->Render();
	}

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE);
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState[2] );

	return FALSE;
}
