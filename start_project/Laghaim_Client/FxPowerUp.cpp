#include "stdafx.h"
#include "sfx.h"
#include "main.h"
#include "NkCharacter.h"
// nate 2004 - 7 - image
#include "ImageDefine.h"



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFxPowerUp::CFxPowerUp()
{
	wave				= NULL;
	wave2				= NULL;
	light				= NULL;
	m_fScale			= 0.0f;
	m_fFade				= 1.0f;
	m_nTotalFrame		= FX_CHARWAVE_FRAMECOUNT;

	Create();
}

CFxPowerUp::~CFxPowerUp()
{
	SAFE_DELETE(wave);
	SAFE_DELETE(wave2);
	SAFE_DELETE(light);
}

void CFxPowerUp::Create()
{
	SAFE_DELETE(wave);
	SAFE_DELETE(wave2);
	SAFE_DELETE(light);

	wave = new CCylinderPlane;
	if( wave )
		wave->Create( 12.0f, 10.0f, 5.0f, 20, 0.0f, TRUE, 4.0f );

	wave2 = new CCylinderPlane;
	if( wave2 )
		wave2->Create( 11.0f, 10.0f, 3.0f, 20, 0.0f, TRUE, 2.0f );

	light = new CRectPlane;
	if( light )
		light->Create( 15.0f, 15.0f, FALSE );
}

void CFxPowerUp::LoadRes(IndexedTexture *pIndexedTexture)
{}
void CFxPowerUp::DeleteRes()
{}

BOOL CFxPowerUp::Render(EffectSort &effect_sort)
{
	if( !GET_D3DDEVICE() )
		return true;

	if( !effect_sort.pNkChaFrom )
		return TRUE;

	if( effect_sort.pNkChaFrom->m_ShowMoonEventEffect == FALSE )
	{
		m_fScale = 0.0f;
		m_fFade = 1.0f;
		return TRUE;
	}

	float rotate = (float)effect_sort.nCurFrame / 1500;

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
	if( effect_sort.pNkChaFrom )
		vDrawPos = D3DVECTOR(effect_sort.pNkChaFrom->m_wx, effect_sort.pNkChaFrom->m_wy, effect_sort.pNkChaFrom->m_wz);

	float red, green, blue;

	if( effect_sort.num == 0 )
	{
		if( effect_sort.Index >= 60 )
		{
			effect_sort.Index = 60;
			effect_sort.num = 1;
		}
	}
	else if( effect_sort.num == 1 )
	{
		if( effect_sort.Index <= 0 )
		{
			effect_sort.Index = 0;
			effect_sort.num = 0;
		}
	}

	m_fScale = ((float)effect_sort.Index / 60);
	m_fFade = 1.0f - m_fScale;

	if( wave )
	{
		// nate 2004 - 7 - image
		GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_POWERUP_WAVE ] ) );

		red = 0.55f * m_fScale;
		green = 0.35f * m_fScale;
		blue = 0.15f * m_fScale;

		wave->SetColor(red, green, blue, m_fScale*0.5f);
		wave->SetRotationY(g_PI);
		wave->RotateY( -rotate * 80.0f );
		wave->Scale( 1.0f, 3.0f + m_fScale, 1.0f );
		wave->SetTranslationXYZ(vDrawPos.x, vDrawPos.y, vDrawPos.z);
		wave->Render();
	}

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND,   D3DBLEND_ONE );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND,  D3DBLEND_ONE );

	red = 0.95f - (0.95f * m_fScale * 0.85f);
	green = 0.25f - (0.25f * m_fScale * 0.85f);
	blue = 0.25f - (0.25f * m_fScale * 0.85f);

	if( wave2 )
	{
		// nate 2004 - 7 - image
		GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_POWERUP_WAVE2 ] ) );

		wave2->SetColor(red, green, blue, m_fFade);
		wave2->SetRotationY(g_PI);
		wave2->RotateY( -rotate * 82.5f );
		wave2->Scale( 1.0f, 1.0f + m_fScale*15.0f, 1.0f );
		wave2->SetTranslationXYZ(vDrawPos.x, vDrawPos.y, vDrawPos.z);
		wave2->Render();
	}

	if( light )
	{
		// nate 2004 - 7 - image
		GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_POWERUP_LIGHT ] ) );

		light->SetColor(red * 0.75f, green * 0.75f, blue * 0.75f, m_fFade * 0.75f);
		light->SetRotationX( g_PI * 0.5f );
		light->RotateZ( m_fScale * 15.0f );
		light->Scale( m_fScale + 1.5f, m_fScale + 1.5f, 1.0f );
		light->SetTranslationXYZ(vDrawPos.x, vDrawPos.y + m_fScale, vDrawPos.z);
		light->Render();
	}


	if( wave2 )
	{
		red = 0.55f - (0.55f * m_fScale * 0.35f);
		green = 0.55f - (0.55f * m_fScale * 0.35f);
		blue = 0.95f - (0.95f * m_fScale * 0.35f);

		wave2->SetColor(red, green, blue, m_fFade);
		wave2->SetScaling( 1.15f, 1.5f, 1.15f );
		wave2->RotateY( m_fScale * 7.0f );
		wave2->SetTranslationXYZ(vDrawPos.x, vDrawPos.y + 2.0f, vDrawPos.z);
		wave2->Render();

		wave2->SetColor(red, green, blue, m_fFade);
		wave2->SetScaling( 1.0f, 1.5f, 1.0f );
		wave2->RotateY( m_fScale * 9.0f );
		wave2->SetTranslationXYZ(vDrawPos.x, vDrawPos.y + m_fScale, vDrawPos.z);
		wave2->Render();
	}

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE);

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState[2] );

	return FALSE;
}


