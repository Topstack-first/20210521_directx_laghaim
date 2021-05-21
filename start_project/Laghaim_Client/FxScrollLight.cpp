#include "stdafx.h"
#include "sfx.h"
#include "main.h"
#include "NkCharacter.h"
#include "Land.h"
#include "ImageDefine.h"



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFxScrollLight::CFxScrollLight()
{
	scroll_light			= NULL;
	m_fScale				= 0.0f;
	m_fFade					= 0.0f;
	m_nTotalFrame			= FX_SCROLL_LIGHT_FRAMECOUNT;

	Create();
}

CFxScrollLight::~CFxScrollLight()
{
	SAFE_DELETE(scroll_light);
}

void CFxScrollLight::Create()
{
	SAFE_DELETE(scroll_light);

	scroll_light = new CRectPlane;
	if( scroll_light )
		scroll_light->Create( 5.0f, 15.0f, 0.0f, FALSE );	// 스크롤 효과
}

void CFxScrollLight::LoadRes(IndexedTexture *pIndexedTexture)
{}
void CFxScrollLight::DeleteRes()
{}

BOOL CFxScrollLight::Render(EffectSort &effect_sort)
{
	if( !GET_D3DDEVICE() || !scroll_light )
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

	D3DVECTOR	vDrawVec;
	vDrawVec = D3DVECTOR(effect_sort.vPos.x, effect_sort.vPos.y, effect_sort.vPos.z);

	// nate 2004 - 7 - image
	GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_SCROLLLIGHT ] ) );

	scroll_light->SetColor(m_fFade, m_fFade, m_fFade, m_fFade);
	scroll_light->SetBillBoard( vViewVec, TRUE, 0.0f );
	scroll_light->Scale( 1.0f, m_fScale + 1.0f, 1.0f );
	scroll_light->SetTranslationXYZ(vDrawVec.x, vDrawVec.y + m_fScale, vDrawVec.z);
	scroll_light->Render();

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState[2] );

	return FALSE;
}

