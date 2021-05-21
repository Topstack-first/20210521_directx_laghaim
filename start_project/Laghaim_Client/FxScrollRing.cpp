#include "stdafx.h"
#include "sfx.h"
#include "main.h"
#include "NkCharacter.h"
#include "NkMob.h"
#include "Land.h"
#include "ImageDefine.h"



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFxScrollRing::CFxScrollRing()
{
	ring					= NULL;
	m_fScale				= 0.0f;
	m_fFade					= 0.0f;
	m_nTotalFrame			= FX_SCROLLRING_FRAMECOUNT;

	Create();
}

CFxScrollRing::~CFxScrollRing()
{
	SAFE_DELETE(ring);
}

void CFxScrollRing::Create()
{
	SAFE_DELETE(ring);

	ring = new CRectPlane;
	if( ring )
		ring->Create( 25.0f, 25.0f, FALSE );
}

void CFxScrollRing::LoadRes(IndexedTexture *pIndexedTexture)
{}
void CFxScrollRing::DeleteRes()
{}

BOOL CFxScrollRing::Render(EffectSort &effect_sort)
{
	if( !GET_D3DDEVICE() || !ring )
		return true;

	if( !effect_sort.pNkChaFrom )
		return TRUE;

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

	D3DVECTOR	vDrawVecFrom;
	if( effect_sort.pNkChaFrom )
		vDrawVecFrom = D3DVECTOR(effect_sort.pNkChaFrom->m_wx, effect_sort.pNkChaFrom->m_wy, effect_sort.pNkChaFrom->m_wz);

	float red, green, blue;
	red = 0.7f - (0.7f * m_fScale * 0.3f);
	green = 0.3f - (0.3f * m_fScale * 0.3f);
	blue = 0.7f - (0.7f * m_fScale * 0.3f);

	// nate 2004 - 7 - image
	GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_SCROLLRING ] ) );

	ring->SetColor(red * 0.5f, green * 0.5f, blue * 0.5f, m_fFade * 0.5f);
	ring->SetRotationX( g_PI * 0.5f );
	ring->SetTranslationXYZ(vDrawVecFrom.x, vDrawVecFrom.y + 1.5f, vDrawVecFrom.z);
	ring->Render();

	ring->SetColor(red * 0.5f, green * 0.5f, blue * 0.5f, m_fFade * 0.5f);
	ring->SetRotationX( g_PI * 0.5f );
	ring->SetTranslationXYZ(vDrawVecFrom.x, vDrawVecFrom.y + 1.5f + m_fScale*1.5f, vDrawVecFrom.z);
	ring->Render();

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState[2] );

	return FALSE;
}

