#include "stdafx.h"
#include "sfx.h"
#include "main.h"
#include "NkCharacter.h"
#include "ImageDefine.h"
#include "FxSummonSquad.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFxSummonSquad::CFxSummonSquad()
{
	m_pRect = new CRectPlane;
	if( m_pRect )
		m_pRect->Create( 10.0f, 10.0f, FALSE );
}

CFxSummonSquad::~CFxSummonSquad()
{
	DeleteRes();
}

void CFxSummonSquad::LoadRes()
{}

void CFxSummonSquad::DeleteRes()
{
	SAFE_DELETE( m_pRect );
}

BOOL CFxSummonSquad::Render(EffectSort &effect_sort)
{
	if( !GET_D3DDEVICE() || !m_pRect )
		return true;

	if( !effect_sort.pNkChaFrom
			|| !GET_D3DDEVICE() )
	{
		effect_sort.nCurFrame = FX_SUMMONSQUAD_FRAMECOUNT;
		return TRUE;
	}

	D3DVECTOR vDrawPos = D3DVECTOR( effect_sort.pNkChaFrom->m_wx, effect_sort.pNkChaFrom->m_wy + 2.0f, effect_sort.pNkChaFrom->m_wz  );

	// Set any appropiate state
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

	float fScale = 0;	// scale
	if( effect_sort.nCurFrame < (FX_SUMMONSQUAD_FRAMECOUNT/2) )
		fScale = effect_sort.nCurFrame;
	else
		fScale = FX_SUMMONSQUAD_FRAMECOUNT - effect_sort.nCurFrame;

	float fFade = (FX_SUMMONSQUAD_FRAMECOUNT - (effect_sort.nCurFrame % FX_SUMMONSQUAD_FRAMECOUNT)) / (float)FX_SUMMONSQUAD_FRAMECOUNT;	// scale fade
	float fAngle = effect_sort.nCurFrame * 2.0f / 8.0f ;

	GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_SUMMONSQUAD ] ) );

	m_pRect->SetBillBoard(D3DVECTOR( 0, -1, 0 ), FALSE, 0.0f);	// Make Billboard
	m_pRect->Scale(fScale, fScale, 0.0f);						// Scale
	m_pRect->SetColor(1.0f, 1.0f, 1.0f, fFade);					// color
	m_pRect->RotateZ( fAngle );									// Rotation
	m_pRect->SetTranslationXYZ(vDrawPos.x, vDrawPos.y, vDrawPos.z);
	m_pRect->Render();

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState[2] );

	return TRUE;
}
