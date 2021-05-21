#include "stdafx.h"
// FxSpiritMode2.cpp: implementation of the CFxSpiritMode2 class.
//
//////////////////////////////////////////////////////////////////////
#include "main.h"
#include "ImageDefine.h"
#include "NkCharacter.h"
#include "FxSpiritMode2.h"


extern BOOL g_bGhost; ///070723 일본버그리포트 수정 고스트 모드땐 스피릿 안 보이게...

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFxSpiritMode2::CFxSpiritMode2()
{
	around_ring			= NULL;
	around_ring2		= NULL;
	m_fScale			= 0.0f;
	m_fFade				= 1.0f;
	m_nTotalFrame		= FX_SPIRITMODE2_FRAMECOUNT;
}

CFxSpiritMode2::~CFxSpiritMode2()
{
	DeleteRes();
}


void CFxSpiritMode2::LoadRes()
{
	DeleteRes();

	around_ring = new CCylinderPlane;
	around_ring2 = new CCylinderPlane;
	around_ring->Create( 11.0f, 8.0f, 7.0f, 20, 0.0f, TRUE, 2.0f );
	around_ring2->Create( 14.0f, 9.5f, 7.0f, 20, 0.0f, TRUE, 2.0f );
}

void CFxSpiritMode2::DeleteRes()
{
	SAFE_DELETE(around_ring);
	SAFE_DELETE(around_ring2);
}

BOOL CFxSpiritMode2::Render(EffectSort &effect_sort)
{
	if( !GET_D3DDEVICE() )
		return TRUE;

	if(!effect_sort.pNkChaFrom )
		return TRUE;

	if(!effect_sort.pNkChaFrom->m_bSpiritModeEffect || g_bGhost ) ///070723 일본버그리포트 수정 고스트 모드땐 스피릿 안 보이게...
		return TRUE;
	else
	{
		if( effect_sort.nCurFrame >= m_nTotalFrame )
			effect_sort.nCurFrame = 0;
	}

	float maxScale = 2.0f;
	m_fScale = (maxScale * effect_sort.nCurFrame) / m_nTotalFrame;

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
	if( effect_sort.pNkChaFrom )
	{
		vDrawPos = D3DVECTOR(effect_sort.pNkChaFrom->m_wx,
							 effect_sort.pNkChaFrom->m_wy,
							 effect_sort.pNkChaFrom->m_wz);
	}

	float red, green, blue;
	red = green = blue = 1.0f;

	if( around_ring )
	{
		GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_SPIRITMODE2 ] ) );

		around_ring->SetColor(red, green, blue, 0.8f);
		around_ring->SetRotationY( g_PI );
		around_ring->RotateY( m_fScale * 1.57f );
		if( effect_sort.nCurFrame <= 20 )
			around_ring->Scale( 1.0f - ( m_fScale * 0.3f ) , 4.0f - ( m_fScale * 2.0f ), 1.0f - ( m_fScale * 0.3f ) );
		else
			around_ring->Scale( 0.7f + ( ( m_fScale - 1 ) * 0.3f) , 2.0f + ( ( m_fScale - 1 ) * 2.0f), 0.7f + ( ( m_fScale - 1 ) * 0.3f));

		around_ring->SetTranslationXYZ(vDrawPos.x, vDrawPos.y, vDrawPos.z);
		around_ring->Render();
	}
	if( around_ring2 )
	{
		around_ring2->SetColor(red, green, blue, 0.8f);
		around_ring2->SetRotationY( g_PI );
		around_ring2->RotateY( -m_fScale * 1.57f );
		if( effect_sort.nCurFrame <= 20 )
			around_ring2->Scale( 1.0f - ( m_fScale * 0.3f ) , 2.0f - ( m_fScale * 1.0f ), 1.0f - ( m_fScale * 0.3f ) );
		else
			around_ring2->Scale( 0.7f + ( ( m_fScale - 1 ) * 0.3f), 1.0f + ( ( m_fScale - 1 ) * 1.0f), 0.7f + ( ( m_fScale - 1 ) * 0.3f) );
		around_ring2->SetTranslationXYZ(vDrawPos.x, vDrawPos.y, vDrawPos.z);
		around_ring2->Render();
	}

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE);

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState[2] );

	return FALSE;
}