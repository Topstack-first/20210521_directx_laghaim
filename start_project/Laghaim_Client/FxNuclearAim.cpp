#include "stdafx.h"
#include "sfx.h"
#include "main.h"
#include "NkCharacter.h"
#include "Land.h"
// nate 2004 - 7 - image
#include "ImageDefine.h"



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFxNuclearAim::CFxNuclearAim()
{
	sign					= NULL;
	sign2					= NULL;
	wave					= NULL;
	wave2					= NULL;
	m_fScale				= 0.0f;
	m_fFade					= 0.0f;
	m_nTotalFrame			= FX_NUCLEARAIM_FRAMECOUNT;

	Create();
}

CFxNuclearAim::~CFxNuclearAim()
{
	SAFE_DELETE(sign);
	SAFE_DELETE(sign2);
	SAFE_DELETE(wave);
	SAFE_DELETE(wave2);
}

void CFxNuclearAim::Create()
{
	SAFE_DELETE(sign);
	SAFE_DELETE(sign2);
	SAFE_DELETE(wave);
	SAFE_DELETE(wave2);

	sign = new CRectPlane;
	if( sign )
		sign->Create( 20.0f, 20.0f, FALSE );
	sign2 = new CRectPlane;
	if( sign2 )
		sign2->Create( 20.0f, 20.0f, FALSE );

	wave = new CCylinderPlane;
	if( wave )
		wave->Create( 8.0f, 5.0f, 1.0f, 20, TRUE );
	wave2 = new CCylinderPlane;
	if( wave2 )
		wave2->Create( 3.0f, 8.0f, 0.0f, 20, FALSE );
}

void CFxNuclearAim::LoadRes(IndexedTexture *pIndexedTexture)
{}
void CFxNuclearAim::DeleteRes()
{}


BOOL CFxNuclearAim::Render(EffectSort &effect_sort)
{
	if( !GET_D3DDEVICE() )
		return true;

	if(effect_sort.nCurFrame >= 300)
	{
		m_fScale = 0.0f;
		m_fFade = 1.0f;
		effect_sort.nCurFrame = m_nTotalFrame;
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

	D3DVECTOR	vViewVec;
	vViewVec = pCMyApp->GetEyeDirection();

	D3DVECTOR	vDrawVec;
	vDrawVec = effect_sort.vTargetPos;

	if( effect_sort.Index >= 50 )
		effect_sort.Index = 0;

	m_fScale = (3.0f * effect_sort.Index) / 50;
	m_fFade = 1.0f - (m_fScale*0.3f);

	float red, green, blue;
	red = 1.0f - (1.0f * m_fScale * 0.3f);
	green = 0.39f - (0.39f * m_fScale * 0.3f);
	blue = 0.39f - (0.39f * m_fScale * 0.3f);

	if( sign )
	{
		// nate 2004 - 7 - image
		GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_NUCLEARAIM_SIGN ] ) );

		sign->SetColor(red, green, blue, m_fFade);
		sign->SetRotationX( g_PI * 0.5f );
		sign->RotateZ( m_fScale * 5.0f );
		sign2->Scale( 0.5f, 0.5f, 1.0f );
		sign->SetTranslationXYZ(vDrawVec.x, vDrawVec.y + 3.5f, vDrawVec.z);
		sign->Render();

		sign->SetColor(red, green, blue, m_fFade);
		sign->SetRotationX( g_PI * 0.5f );
		sign->RotateZ( -m_fScale * 5.0f );
		sign->Scale( 0.5f, 0.5f, 1.0f );
		sign->SetTranslationXYZ(vDrawVec.x, vDrawVec.y + 3.5f, vDrawVec.z);
		sign->Render();
	}

	if( sign2 )
	{
		// nate 2004 - 7 - image
		GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_NUCLEARAIM_LIGHT ] ) );

		sign2->SetColor(red * 0.35f, green * 0.35f, blue * 0.35f, m_fFade * 0.35f);
		sign2->SetRotationX( g_PI * 0.5f );
		sign2->RotateZ( m_fScale * 5.0f );
		sign2->Scale( m_fScale*1.25f + 1.0f, 1.0f, m_fScale*1.25f + 1.0f );
		sign2->SetTranslationXYZ(vDrawVec.x, vDrawVec.y + 5.5f, vDrawVec.z);
		sign2->Render();
	}

	// nate 2004 - 7 - image
	GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_NUCLEARAIM_WAVE ] ) );

	if( wave )
	{
		wave->SetColor(red * 0.5f, green * 0.5f, blue * 0.5f, m_fFade * 0.5f);
		wave->SetRotationY( g_PI * 0.5f );
		wave->RotateY( m_fScale * 7.0f );
		wave->Scale( m_fScale*1.5f + 1.0f, 1.0f, m_fScale*1.5f + 1.0f );
		wave->SetTranslationXYZ(vDrawVec.x, vDrawVec.y + 5.5f, vDrawVec.z);
		wave->Render();
	}

	if( wave2 )
	{
		wave2->SetColor(red * 0.35f, green * 0.35f, blue * 0.35f, m_fFade * 0.35f);
		wave2->SetRotationY( g_PI * 0.5f );
		wave2->Scale( m_fScale*2.0f + 1.0f, 1.0f, m_fScale*2.0f + 1.0f );
		wave2->SetTranslationXYZ(vDrawVec.x, vDrawVec.y + 5.5f, vDrawVec.z);
		wave2->Render();
	}

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState[2] );

	return FALSE;
}
