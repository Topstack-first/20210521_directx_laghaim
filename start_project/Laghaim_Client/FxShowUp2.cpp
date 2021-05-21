#include "stdafx.h"
#include "sfx.h"
#include "main.h"
#include "ImageDefine.h"



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CFxShowUp2::CFxShowUp2()
{
	ground_ring			= NULL;
	ground_corona		= NULL;
	shining_ring		= NULL;
	shining_lignt		= NULL;
	m_fScale			= 0.0f;
	m_fFade				= 1.0f;
	m_nTotalFrame		= FX_SHOWUP2_FRAMECOUNT;

	Create();
}

CFxShowUp2::~CFxShowUp2()
{
	SAFE_DELETE(ground_ring);
	SAFE_DELETE(ground_corona);
	SAFE_DELETE(shining_ring);
	SAFE_DELETE(shining_lignt);
}

void CFxShowUp2::Create()
{
	SAFE_DELETE(ground_ring);
	SAFE_DELETE(ground_corona);
	SAFE_DELETE(shining_ring);
	SAFE_DELETE(shining_lignt);

	ground_ring = new CRectPlane;
	if( ground_ring )
		ground_ring->Create( 25.0f, 25.0f, FALSE );

	ground_corona = new CRectPlane;
	if( ground_corona )
		ground_corona->Create( 25.0f, 25.0f, FALSE );

	shining_ring = new CCylinderPlane;
	if( shining_ring )
		shining_ring->Create( 10.5f, 8.0f, 1.0f, 20, 0.0f, FALSE, 5.0f );

	shining_lignt = new CCylinderPlane;
	if( shining_lignt )
		shining_lignt->Create( 10.0f, 10.0f, 7.5f, 20, 0.0f, TRUE, 2.0f );
}


void CFxShowUp2::LoadRes(IndexedTexture *pIndexedTexture)
{}

void CFxShowUp2::DeleteRes()
{}

void CFxShowUp2::SetTextureNum()
{}

BOOL CFxShowUp2::Render(EffectSort &effect_sort)
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
	vDrawPos = D3DVECTOR(effect_sort.vPos.x, effect_sort.vPos.y, effect_sort.vPos.z);

	float red, green, blue;
	red = green = blue = 0.0f;

	if( ground_corona )
	{
		// nate 2004 - 7 - image
		GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_SHOWUP2_CORONA ] ) );

		red = 0.35f - (0.35f * m_fScale * 0.25f);
		green = 0.35f - (0.35f * m_fScale * 0.25f);
		blue = 1.0f - (1.0f * m_fScale * 0.25f);

		ground_corona->SetColor(red, green, blue, m_fFade);
		ground_corona->SetRotationX( g_PI * 0.5f );
		ground_corona->RotateZ( m_fScale * 5.0f );
		ground_corona->SetTranslationXYZ(vDrawPos.x, vDrawPos.y + 1.5f, vDrawPos.z);
		ground_corona->Render();

		red = 0.85f - (0.85f * m_fScale * 0.45f);
		green = 0.85f - (0.85f * m_fScale * 0.45f);
		blue = 1.0f - (1.0f * m_fScale * 0.45f);

		ground_corona->SetColor(red, green, blue, m_fFade);
		ground_corona->SetRotationX( g_PI * 0.5f );
		ground_corona->RotateZ( m_fScale * 3.0f );
		ground_corona->Scale( m_fScale * 0.55f, m_fScale * 0.55f, 1.0f );
		ground_corona->SetTranslationXYZ(vDrawPos.x, vDrawPos.y + (m_fScale*15.0f), vDrawPos.z);
		ground_corona->Render();
	}

	if( shining_lignt )
	{
		// nate 2004 - 7 - image
		GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_SHOWUP2_ROUND ] ) );

		red = 0.28f - (0.28f * m_fScale * 0.45f);
		green = 0.28f - (0.28f * m_fScale * 0.45f);
		blue = 1.0f - (1.0f * m_fScale * 0.45f);

		shining_lignt->SetColor(red, green, blue, m_fFade);
		shining_lignt->SetRotationY( g_PI );
		shining_lignt->RotateY( m_fScale * 3.0f );
		shining_lignt->Scale( 1.0f, (m_fScale*2.0f)+ 1.0f, 1.0f );
		shining_lignt->SetTranslationXYZ(vDrawPos.x, vDrawPos.y, vDrawPos.z);
		shining_lignt->Render();
	}

	if( effect_sort.Index >= 5 )
		effect_sort.Index = 0;

	m_fScale = ((float)effect_sort.Index / 5);
	m_fFade = 1.0f - m_fScale;

	if( ground_ring )
	{
		// nate 2004 - 7 - image
		GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_SHOWUP2_SHIELD ] ) );
		red = (0.5f * m_fScale * 0.5f);
		green = (0.5f * m_fScale * 0.5f);
		blue = (1.0f * m_fScale * 0.5f);

		ground_ring->SetColor(red, green, blue, m_fFade);
		ground_ring->SetRotationX( g_PI * 0.5f );
		ground_ring->Scale( m_fScale * 0.75f, m_fScale * 0.75f, 1.0f );
		ground_ring->SetTranslationXYZ(vDrawPos.x, vDrawPos.y + 1.75f, vDrawPos.z);
		ground_ring->Render();

		red = (0.25f * m_fScale * 0.75f);
		green = (0.25f * m_fScale * 0.75f);
		blue = (1.0f * m_fScale * 0.75f);

		ground_ring->SetRotationX( g_PI * 0.5f );
		ground_ring->Scale( m_fScale, m_fScale, 1.0f );
		ground_ring->SetTranslationXYZ(vDrawPos.x, vDrawPos.y + 1.5f, vDrawPos.z);
		ground_ring->Render();

		ground_ring->SetRotationX( g_PI * 0.5f );
		ground_ring->SetTranslationXYZ(vDrawPos.x, vDrawPos.y + 35.0f - (m_fScale*35.0f), vDrawPos.z);
		ground_ring->Render();
	}

	if( shining_ring )
	{
		red = (0.15f * m_fScale * 0.55f);
		green = (0.15f * m_fScale * 0.55f);
		blue = (1.0f * m_fScale * 0.55f);

		// nate 2004 - 7 - image
		GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_SHOWUP2_LEVELUP ] ) );

		shining_ring->SetColor(red, green, blue, m_fFade);
		shining_ring->SetRotationY( g_PI );
		shining_ring->RotateY( m_fScale * 5.0f );
		shining_ring->SetTranslationXYZ(vDrawPos.x, vDrawPos.y + 33.0f - (m_fScale*35.0f), vDrawPos.z);
		shining_ring->Render();
	}

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE);

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState[2] );

	return FALSE;
}
