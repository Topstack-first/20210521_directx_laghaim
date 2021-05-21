#include "stdafx.h"
// FxShadowSlash2.cpp: implementation of the CFxShadowSlash2 class.
//
//////////////////////////////////////////////////////////////////////

#include "sfx.h"
#include "main.h"
#include "ImageDefine.h"
#include "NkCharacter.h"
#include "FxShadowSlash2.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFxShadowSlash2::CFxShadowSlash2()
{
	around_ring			= NULL;
	m_fScale			= 0.0f;
	m_fFade				= 1.0f;
	m_nTotalFrame		= FX_SHADOWSLASH2_FRAMECOUNT;
}

CFxShadowSlash2::~CFxShadowSlash2()
{
	DeleteRes();
}


void CFxShadowSlash2::LoadRes()
{
	DeleteRes();

	around_ring = new CCylinderPlane;
	if( around_ring )
		around_ring->Create( 11.0f, 11.0f, 7.0f, 20, 0.0f, TRUE, 2.0f );
}

void CFxShadowSlash2::DeleteRes()
{
	SAFE_DELETE(around_ring);
}

BOOL CFxShadowSlash2::Render(EffectSort &effect_sort)
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
	m_fFade = 1.0f - (m_fScale * 0.45f);

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
	/*	///하이콤보 시 수정.
		if( effect_sort.pNkChaFrom )
		{
			vDrawPos = D3DVECTOR(effect_sort.pNkChaFrom->m_wx,
								 effect_sort.pNkChaFrom->m_wy,
								 effect_sort.pNkChaFrom->m_wz);
		}
	*/
	vDrawPos = effect_sort.vPos; ///하이콤보 시 수정.

	float red, green, blue;
	red = green = blue = 0.0f;

	if( around_ring )
	{
		GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_SHOWUP2_ROUND ] ) );

		red = 0.8f - (0.3f * m_fScale * 0.45f);
		green = 0.2f - (0.1f * m_fScale * 0.45f);
		blue = 0.3f - (0.2f * m_fScale * 0.45f);

		around_ring->SetColor(red, green, blue, m_fFade);
		around_ring->SetRotationY( g_PI );
		around_ring->RotateY( m_fScale * 10.0f );
		around_ring->Scale( 1.0f, (m_fScale * 20.0f)+ 1.0f, 1.0f );
		around_ring->SetTranslationXYZ(vDrawPos.x, vDrawPos.y, vDrawPos.z);
		around_ring->Render();
	}

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE);

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState[2] );

	return FALSE;
}

