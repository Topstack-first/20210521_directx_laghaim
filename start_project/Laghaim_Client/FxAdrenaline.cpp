#include "stdafx.h"
// FxAdrenaline.cpp: implementation of the CFxAdrenaline class.
//
//////////////////////////////////////////////////////////////////////
#include "sfx.h"
#include "main.h"
#include "NkCharacter.h"
#include "Land.h"
#include "skill.h"
#include "ImageDefine.h"
#include "FxAdrenaline.h"



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFxAdrenaline::CFxAdrenaline()
{
	circle_up				= NULL;
	circle_down				= NULL;
	m_fScale				= 0.0f;
	m_fFade					= 0.0f;
	m_nTotalFrame			= FX_ADRENALINE_FRAMECOUNT;
}

CFxAdrenaline::~CFxAdrenaline()
{
	DeleteRes();
}

void CFxAdrenaline::LoadRes()
{
	DeleteRes();

	circle_up = new CRectPlane;
	if( circle_up )
		circle_up->Create( 17.0f, 17.0f, FALSE );

	circle_down = new CRectPlane;
	if( circle_down )
		circle_down->Create( 17.0f, 17.0f, FALSE );
}

void CFxAdrenaline::DeleteRes()
{
	SAFE_DELETE(circle_up);
	SAFE_DELETE(circle_down);
}

BOOL CFxAdrenaline::Render(EffectSort &effect_sort)
{
	if( !GET_D3DDEVICE() || !circle_up || !circle_down )
		return TRUE;

	if( !effect_sort.pNkChaFrom->m_bShowAdrenaline )
		return TRUE;

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

	D3DVECTOR	vDrawVec;
	if( effect_sort.pNkChaFrom )
	{
		vDrawVec = D3DVECTOR(effect_sort.pNkChaFrom->m_wx,
							 effect_sort.pNkChaFrom->m_wy,
							 effect_sort.pNkChaFrom->m_wz);
	}

	float red, green, blue;
	GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_ADRENALINE ] ) );
	red = 0.3f - (0.3f * m_fScale * 0.7f);
	green = 0.3f - (0.3f * m_fScale * 0.7f);
	blue = 1.0 - (1.0 * m_fScale * 0.7f);

	circle_up->SetColor(red, green, blue, m_fFade);
	circle_up->SetRotationX( g_PI * 0.5f );
	circle_up->RotateZ( -m_fScale * 75.0f );
	circle_up->Scale( 1.0f, 1.0f, 1.0f );
	circle_up->SetTranslationXYZ(vDrawVec.x, vDrawVec.y + 3.5f, vDrawVec.z);
	circle_up->Render();

	if( effect_sort.nCurFrame >= 50 )
		effect_sort.nCurFrame = 0;

	m_fScale = ((float)effect_sort.nCurFrame / 50);
	m_fFade = 1.0f - m_fScale;

	circle_up->SetColor(red, green, blue, m_fFade);
	circle_up->SetRotationX( g_PI * 0.5f );
	circle_up->RotateZ( m_fScale * 3.0f );
	circle_up->Scale( 1.0f + (m_fScale*0.5f), 1.0f + (m_fScale*0.5f), 1.0f + (m_fScale*0.5f));
	circle_up->SetTranslationXYZ(vDrawVec.x, vDrawVec.y + 3.5f + (m_fScale * 3.0), vDrawVec.z);
	circle_up->Render();

	GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_OPTIONSKILL_CORONA ] ) );

	circle_down->SetColor(red, green, blue, m_fFade);
	circle_down->SetRotationX( g_PI * 0.5f );
	circle_down->RotateZ( m_fScale * 75.0f );
	circle_down->Scale( RandomNum(1.3f, 1.5f), RandomNum(1.3f, 1.5f), RandomNum(1.3f, 1.5f) );
	circle_down->SetTranslationXYZ(vDrawVec.x, vDrawVec.y + 3.5f, vDrawVec.z);
	circle_down->Render();

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState[2] );

	return FALSE;
}
