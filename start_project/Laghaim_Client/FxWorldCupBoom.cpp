#include "stdafx.h"
#include "sfx.h"
#include "main.h"
#include "ImageDefine.h"



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFxWorldCupBoom::CFxWorldCupBoom()
{
	hit					= NULL;
	m_fScale			= 0.0f;
	m_fFade				= 1.0f;
	m_nTotalFrame		= FX_HITBOOMEFFECTS_FRAMECOUNT;

	Create();
}

CFxWorldCupBoom::~CFxWorldCupBoom()
{
	SAFE_DELETE(hit);
}

void CFxWorldCupBoom::Create()
{
	SAFE_DELETE(hit);

	hit = new CRectPlane;
	if( hit )
		hit->Create( 10.0f, 10.0f, FALSE );
}

void CFxWorldCupBoom::LoadRes(IndexedTexture *pIndexedTexture)
{}
void CFxWorldCupBoom::DeleteRes()
{}

BOOL CFxWorldCupBoom::Render(EffectSort &effect_sort)
{
	if( !GET_D3DDEVICE() || !hit )
		return true;

	if(effect_sort.nCurFrame >= m_nTotalFrame)
	{
		m_fScale = 0.0f;
		m_fFade = 1.0f;
		return TRUE;
	}

	float maxScale = 2.5f;
	m_fScale = (maxScale * effect_sort.nCurFrame) / m_nTotalFrame;
	m_fFade = 1.0f - (m_fScale * 0.3f);

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

	D3DVECTOR		vDrawPos;
	vDrawPos = D3DVECTOR( effect_sort.vPos.x, effect_sort.vPos.y, effect_sort.vPos.z );

	float red, green, blue;
	red = green = blue = 0.0f;

	// nate 2004 - 7 - image
	GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_WORLDCUPBOOM_HIT ] ) );

	hit->SetColor(1.0f, 1.0f, 1.0f, m_fFade);
	hit->SetBillBoard(vViewVec, FALSE, 0.0f);
	hit->Scale(m_fScale, m_fScale, 0.0f);
	hit->SetTranslationXYZ(vDrawPos.x, vDrawPos.y, vDrawPos.z);
	hit->Render();

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE);

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState[2] );

	return FALSE;
}

