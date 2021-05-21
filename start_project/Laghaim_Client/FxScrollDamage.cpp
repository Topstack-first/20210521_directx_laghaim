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

CFxScrollDamage::CFxScrollDamage()
{
	ring					= NULL;
	m_fScale				= 0.0f;
	m_fFade					= 0.0f;
	m_nTotalFrame			= FX_SCROLLDAMAGE_FRAMECOUNT;

	Create();
}

CFxScrollDamage::~CFxScrollDamage()
{
	SAFE_DELETE(ring);
}

void CFxScrollDamage::Create()
{
	SAFE_DELETE(ring);

	ring = new CRectPlane;
	if( ring )
		ring->Create( 25.0f, 25.0f, FALSE );
}

void CFxScrollDamage::LoadRes(IndexedTexture *pIndexedTexture)
{}
void CFxScrollDamage::DeleteRes()
{}

BOOL CFxScrollDamage::Render(EffectSort &effect_sort)
{
	if( !GET_D3DDEVICE() || !ring )
		return true;

	if(!effect_sort.pNkChaTo
			&& !effect_sort.pMobTo)
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

	D3DVECTOR	vDrawVecTo;

	if( effect_sort.pNkChaTo )
		vDrawVecTo = D3DVECTOR(effect_sort.pNkChaTo->m_wx, effect_sort.pNkChaTo->m_wy, effect_sort.pNkChaTo->m_wz);
	if( effect_sort.pMobTo )
		vDrawVecTo = D3DVECTOR(effect_sort.pMobTo->m_wx, effect_sort.pMobTo->m_wy, effect_sort.pMobTo->m_wz);

	float red, green, blue;
	red = 0.7f - (0.7f * m_fScale * 0.3f);
	green = 0.3f - (0.3f * m_fScale * 0.3f);
	blue = 0.7f - (0.7f * m_fScale * 0.3f);

	// nate 2004 - 7 - image
	GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_SCROLLDAMAGE ] ) );
	ring->SetColor(red * 0.5f, green * 0.5f, blue * 0.5f, m_fFade * 0.5f);
	ring->SetRotationX( g_PI * 0.5f );
	ring->RotateZ( m_fScale * 5.0f );
	ring->SetTranslationXYZ(vDrawVecTo.x, vDrawVecTo.y + 1.5f, vDrawVecTo.z);
	ring->Render();

	ring->SetColor(red * 0.5f, green * 0.5f, blue * 0.5f, m_fFade * 0.5f);
	ring->SetRotationX( g_PI * 0.5f );
	ring->RotateZ( -m_fScale * 7.0f );
	ring->SetTranslationXYZ(vDrawVecTo.x, vDrawVecTo.y + 1.5f, vDrawVecTo.z);
	ring->Render();

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState[2] );

	return FALSE;
}

