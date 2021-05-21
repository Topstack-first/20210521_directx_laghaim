#include "stdafx.h"
#include "sfx.h"
#include "main.h"
#include "NkCharacter.h"
#include "ImageDefine.h"


extern BOOL g_bWaitFxCharWave;
extern BOOL g_bFinishFxCharWave;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFxCharWave::CFxCharWave()
{
	wave				= NULL;
	wave2				= NULL;
	light				= NULL;
	m_fScale			= 0.0f;
	m_fFade				= 1.0f;
	m_nTotalFrame		= FX_CHARWAVE_FRAMECOUNT;

	Create();
}

CFxCharWave::~CFxCharWave()
{
	SAFE_DELETE(wave);
	SAFE_DELETE(wave2);
	SAFE_DELETE(light);
}

void CFxCharWave::Create()
{
	SAFE_DELETE(wave);
	SAFE_DELETE(wave2);
	SAFE_DELETE(light);

	wave = new CCylinderPlane;
	if( wave )
		wave->Create( 12.0f, 10.0f, 5.0f, 20, 0.0f, TRUE );

	wave2 = new CCylinderPlane;
	if( wave2 )
		wave2->Create( 12.25f, 10.25f, 4.5f, 20, 0.0f, TRUE );

	light = new CRectPlane;
	if( light )
		light->Create( 15.0f, 15.0f, FALSE );
}

void CFxCharWave::LoadRes(IndexedTexture *pIndexedTexture)
{}

void CFxCharWave::DeleteRes()
{}

BOOL CFxCharWave::Render(EffectSort &effect_sort)
{
	if( !GET_D3DDEVICE() )
		return true;

	if(effect_sort.nCurFrame >= m_nTotalFrame)
	{
		m_fScale = 0.0f;
		m_fFade = 1.0f;
		return TRUE;
	}

	if( g_bWaitFxCharWave )
		return FALSE;

	if( g_bFinishFxCharWave )
	{
		effect_sort.nCurFrame = m_nTotalFrame;
		m_fScale = 0.0f;
		m_fFade = 1.0f;
		return TRUE;
	}

	float rotate = (float)effect_sort.nCurFrame / 5000;

	float maxScale = 2.0f;

	m_fScale = (maxScale * effect_sort.nCurFrame) / m_nTotalFrame;
	m_fFade = 1.0f - (m_fScale * 0.45f);

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND,   D3DBLEND_SRCALPHA );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND,  D3DBLEND_INVSRCALPHA);

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

	// nate 2004 - 7 - image
	GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_CHARWAVE_WAVE ] ) );

	if( wave )
	{
		red = 0.858f;
		green = 0.984f;
		blue = 1.0f;

		wave->SetColor(red, green, blue, m_fScale);
		wave->SetRotationY(g_PI);
		wave->RotateY( -rotate * 80.0f );
		wave->Scale( 1.0f, 1.0f + m_fScale, 1.0f );
		wave->SetTranslationXYZ(vDrawPos.x, vDrawPos.y, vDrawPos.z);
		wave->Render();
	}

	if( wave2 )
	{
		red = 0.384f;
		green = 0.658f;
		blue = 1.0f;

		wave2->SetColor(red, green, blue, m_fFade);
		wave2->SetRotationY(g_PI);
		wave2->RotateY( -rotate * 85.0f );
		wave2->Scale( 1.0f, 1.0f + m_fScale*2.0f, 1.0f );
		wave2->SetTranslationXYZ(vDrawPos.x, vDrawPos.y, vDrawPos.z);
		wave2->Render();
	}

	if( light )
	{
		red = 0.49f - (0.49f * m_fScale * 0.75f);
		green = 0.9f - (0.9f * m_fScale * 0.75f);
		blue = 0.484f - (0.484f * m_fScale * 0.75f);

		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND,   D3DBLEND_ONE );
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND,  D3DBLEND_ONE );

		// nate 2004 - 7 - image
		if( g_pCapsyongTexture )
			GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_CHARWAVE_LIGHT ] ) );

		light->SetColor(red * 0.25f, green * 0.25f, blue * 0.25f, m_fFade * 0.25f);
		light->SetRotationX( g_PI * 0.5f );
		light->RotateZ( m_fScale * 15.0f );
		light->Scale( m_fScale + 1.5f, m_fScale + 1.5f, 1.0f );
		light->SetTranslationXYZ(vDrawPos.x, vDrawPos.y + m_fScale, vDrawPos.z);
		light->Render();
	}

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE);

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState[2] );

	return FALSE;
}


