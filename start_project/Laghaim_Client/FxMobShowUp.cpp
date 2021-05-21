#include "stdafx.h"
#include "sfx.h"
#include "main.h"
#include "NkCharacter.h"
// nate 2004 - 7 - image
#include "ImageDefine.h"



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFxMobShowUp::CFxMobShowUp()
{
	ground_corona		= NULL;
	shining_ring		= NULL;
	shining_lignt		= NULL;
	m_fScale			= 1.0f;
	m_fFade				= 1.0f;
	m_nTotalFrame		= FX_MOBSHOWUP_FRAMECOUNT;

	Create();
}

CFxMobShowUp::~CFxMobShowUp()
{
	SAFE_DELETE(ground_corona);
	SAFE_DELETE(shining_ring);
	SAFE_DELETE(shining_lignt);
}

void CFxMobShowUp::Create()
{
	SAFE_DELETE(ground_corona);
	SAFE_DELETE(shining_ring);
	SAFE_DELETE(shining_lignt);

	ground_corona = new CRectPlane;
	if( ground_corona )
		ground_corona->Create( 10.0f, 10.0f, FALSE );

	shining_ring = new CCylinderPlane;
	if( shining_ring )
		shining_ring->Create( 7.5f, 15.0f, 0.0f, 20, 0.0f, FALSE, 20.0f );

	shining_lignt = new CCylinderPlane;
	if( shining_lignt )
		shining_lignt->Create( 10.0f, 10.0f, 5.5f, 20, 0.0f, TRUE, 20.0f );

}

void CFxMobShowUp::LoadRes(IndexedTexture *pIndexedTexture)
{}
void CFxMobShowUp::DeleteRes()
{}

BOOL CFxMobShowUp::Render(EffectSort &effect_sort)
{
	if( !GET_D3DDEVICE() )
		return true;

	if(effect_sort.nCurFrame >= m_nTotalFrame)
	{
		m_fScale = 0.0f;
		m_fFade = 1.0f;
		return TRUE;
	}

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
	red = green = blue = 1.0f;

	float maxScale = 2.0f;

	if( effect_sort.num == 38 )  				// 워자이언트 리젠될때 나오는 효과
	{
		m_fScale = (maxScale * effect_sort.nCurFrame) / m_nTotalFrame;
		m_fFade = 1.0f - (m_fScale * 0.45f);

		red = 0.4f - (0.4f * m_fScale * 0.35f);
		green = 1.0f - (1.0f * m_fScale * 0.35f);
		blue = 0.7f - (0.7f * m_fScale * 0.35f);

		if( ground_corona )
		{
			// nate 2004 - 7 - image
			GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_MOBSHOWUP_LIFE ] ) );

			ground_corona->SetColor(red * 0.45f, green * 0.45f, blue * 0.45f, m_fFade * 0.45f);
			ground_corona->SetRotationX( g_PI * 0.5f );
			ground_corona->Scale( 3.0f, 3.0f, 3.0f );
			ground_corona->SetTranslationXYZ(vDrawPos.x, vDrawPos.y + 5.5f, vDrawPos.z);
			ground_corona->Render();

			// nate 2004 - 7 - image
			GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_MOBSHOWUP_CORONA ] ) );

			ground_corona->SetColor(red * 0.35f, green * 0.35f, blue * 0.35f, m_fFade * 0.35f);
			ground_corona->SetRotationX( g_PI * 0.5f );
			ground_corona->RotateZ( m_fScale * 5.0f );
			ground_corona->Scale( m_fScale * 1.5f, 1.0f, m_fScale * 1.5f );
			ground_corona->SetTranslationXYZ(vDrawPos.x, vDrawPos.y + 5.5f + m_fScale*2.0f, vDrawPos.z);
			ground_corona->Render();
		}

		float ground_height = 1.0f;
		float life_scale = 1.0f;

		life_scale = effect_sort.nCurFrame * 0.085f;

		if( effect_sort.nCurFrame <= 15 )
		{
			m_fScale = ((float)effect_sort.nCurFrame / 15);
			m_fFade = 1.0f - m_fScale;

			red = 0.4f * m_fScale;
			green = 1.0f * m_fScale;
			blue = 0.65f * m_fScale;

			ground_height = m_fScale * 18.5f;
		}
		else
		{
			m_fScale = ((float)(effect_sort.nCurFrame-15) / 5);
			m_fFade = 1.0f - m_fScale;

			red = 0.4f - (0.4f * m_fScale);
			green = 1.0f - (1.0f * m_fScale);
			blue = 0.65f - (0.65f * m_fScale);

			ground_height = m_fFade * 18.5f;
		}

		if( shining_ring )
		{
			// nate 2004 - 7 - image
			GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_MOBSHOWUP_ROUND ] ) );

			shining_ring->SetColor(red * 0.15f, green * 0.35f, blue * 0.35f, m_fFade * 0.35f);
			shining_ring->SetRotationY( g_PI * 0.5f );
			shining_ring->RotateY( m_fScale * 50.0f );
			shining_ring->Scale( life_scale + 0.15f, 1.0f, life_scale + 0.15f );
			shining_ring->SetTranslationXYZ(vDrawPos.x, vDrawPos.y + 1.75f, vDrawPos.z);
			shining_ring->Render();
		}

		if( shining_lignt )
		{
			// nate 2004 - 7 - image
			GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_MOBSHOWUP_LASER ] ) );

			shining_lignt->SetColor(red * 0.35f, green * 0.35f, blue * 0.35f, m_fFade * 0.35f);
			shining_lignt->SetRotationY( g_PI * 0.5f );
			shining_lignt->RotateY( m_fScale * 50.0f );
			shining_lignt->Scale( life_scale*0.5f + 1.0f, ground_height, life_scale*0.5f + 1.0f );
			shining_lignt->SetTranslationXYZ(vDrawPos.x, vDrawPos.y + 1.75f, vDrawPos.z);
			shining_lignt->Render();
		}
	}
	else
	{
		// 모든 몹들이 리젠될때 나오는 효과
		m_fScale = (maxScale * effect_sort.nCurFrame) / m_nTotalFrame;
		m_fFade = 1.0f - (m_fScale * 0.45f);

		red = 0.4f - (0.4f * m_fScale * 0.35f);
		green = 1.0f - (1.0f * m_fScale * 0.35f);
		blue = 0.7f - (0.7f * m_fScale * 0.35f);

		if( ground_corona )
		{
			// nate 2004 - 7 - image
			GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_MOBSHOWUP_CORONA ] ) );

			ground_corona->SetColor(red * 0.35f, green * 0.35f, blue * 0.35f, m_fFade * 0.35f);
			ground_corona->SetRotationX( g_PI * 0.5f );
			ground_corona->RotateZ( m_fScale * 5.0f );
			ground_corona->Scale( m_fScale * 1.5f, 1.0f, m_fScale * 1.5f );
			ground_corona->SetTranslationXYZ(vDrawPos.x, vDrawPos.y + 5.5f + m_fScale*3.0f, vDrawPos.z);
			ground_corona->Render();
		}

		float ground_height = 1.0f;
		float life_scale = 1.0f;

		life_scale = effect_sort.nCurFrame * 0.085f;

		if( effect_sort.nCurFrame <= 15 )
		{
			m_fScale = ((float)effect_sort.nCurFrame / 15);
			m_fFade = 1.0f - m_fScale;

			red = 0.84f * m_fScale;
			green = 1.0f * m_fScale;
			blue = 0.65f * m_fScale;

			ground_height = m_fScale * 5.5f;
		}
		else
		{
			m_fScale = ((float)(effect_sort.nCurFrame-15) / 5);
			m_fFade = 1.0f - m_fScale;

			red = 0.84f - (0.84f * m_fScale);
			green = 1.0f - (1.0f * m_fScale);
			blue = 0.65f - (0.65f * m_fScale);

			ground_height = m_fFade * 5.5f;
		}

		if( shining_ring )
		{
			// nate 2004 - 7 - image
			GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_MOBSHOWUP_ROUND ] ) );

			shining_ring->SetColor(red * 0.15f, green * 0.25f, blue * 0.25f, m_fFade * 0.25f);
			shining_ring->SetRotationY( g_PI * 0.5f );
			shining_ring->RotateY( m_fScale * 50.0f );
			shining_ring->Scale( life_scale + 0.15f, 1.0f, life_scale + 0.15f );
			shining_ring->SetTranslationXYZ(vDrawPos.x, vDrawPos.y + 1.75f, vDrawPos.z);
			shining_ring->Render();
		}

		if( shining_lignt )
		{
			// nate 2004 - 7 - image
			GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_MOBSHOWUP_LASER ] ) );

			shining_lignt->SetColor(red * 0.25f, green * 0.25f, blue * 0.25f, m_fFade * 0.25f);
			shining_lignt->SetRotationY( g_PI * 0.5f );
			shining_lignt->RotateY( m_fScale * 50.0f );
			shining_lignt->Scale( life_scale*0.5f + 1.0f, ground_height, life_scale*0.5f + 1.0f );
			shining_lignt->SetTranslationXYZ(vDrawPos.x, vDrawPos.y + 1.75f, vDrawPos.z);
			shining_lignt->Render();
		}

		if( effect_sort.Index >= 10 )
			effect_sort.Index = 0;

		m_fScale = ((float)effect_sort.Index / 10);
		m_fFade = 1.0f - m_fScale;

		if( ground_corona )
		{
			// nate 2004 - 7 - image
			GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_MOBSHOWUP_CORONA ] ) );

			red = 0.25f - (0.25f * m_fScale * 0.55f);
			green = 0.5f - (0.5f * m_fScale * 0.55f);
			blue = 0.25f - (0.25f * m_fScale * 0.55f);

			ground_corona->SetColor(red, green, blue, m_fFade);
			ground_corona->SetRotationX( g_PI * 0.5f );
			ground_corona->RotateZ( m_fScale * 5.0f );
			ground_corona->Scale( m_fScale * 3.5f, m_fScale * 3.5f, m_fScale * 3.5f );
			ground_corona->SetTranslationXYZ(vDrawPos.x, vDrawPos.y + 1.75f, vDrawPos.z);
			ground_corona->Render();
		}
	}

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE);

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState[2] );

	return FALSE;
}

