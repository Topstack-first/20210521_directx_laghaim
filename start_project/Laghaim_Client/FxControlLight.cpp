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

CFxControlLight::CFxControlLight()
{
	gauge					= NULL;
	gauge_real				= NULL;
	m_fScale				= 0.0f;
	m_fFade					= 0.0f;
	m_nTotalFrame			= FX_SHIELD_FRAMECOUNT;

	Create();
}

CFxControlLight::~CFxControlLight()
{
	SAFE_DELETE(gauge);
	SAFE_DELETE(gauge_real);
}

void CFxControlLight::Create()
{
	SAFE_DELETE(gauge);
	SAFE_DELETE(gauge_real);

	gauge = new CCylinderPlane;
	if( gauge )
		gauge->Create( 5.5f, 5.5f, 25.0f, 20, 0.0f, TRUE );

	gauge_real = new CCylinderPlane;
	if( gauge_real )
		gauge_real->Create( 5.0f, 5.0f, 25.0f, 20, 0.0f, TRUE );
}

void CFxControlLight::LoadRes(IndexedTexture *pIndexedTexture)
{}
void CFxControlLight::DeleteRes()
{}

BOOL CFxControlLight::Render(EffectSort &effect_sort)
{
	if( !GET_D3DDEVICE() || !gauge || !gauge_real )
		return true;

	if( !effect_sort.pMobTo )
		return TRUE;

	if( effect_sort.pMobTo->m_AttackVital <= 0 )
	{
		effect_sort.nCurFrame = m_nTotalFrame;
		m_fScale = 0.0f;
		m_fFade = 1.0f;
		return TRUE;
	}
	else
	{
		if( effect_sort.nCurFrame >= m_nTotalFrame )
			effect_sort.nCurFrame = 0;
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

	D3DVECTOR	vDrawPos;
	if( effect_sort.pMobTo )
		vDrawPos = D3DVECTOR(effect_sort.pMobTo->m_wx, effect_sort.pMobTo->m_wy, effect_sort.pMobTo->m_wz);

	// nate 2004 - 7 - image
	if( g_pCapsyongTexture )
		GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_CONTROLLIGHT_GAUGE ] ));

	float Red = 0.1f;
	float Green = 0.1f;
	float Blue = 0.1f;
	float Scale = 1.0f;
	float Height = 5.0f;

	gauge->SetColor(Red, Green, Blue, m_fFade);
	gauge->SetRotationY(g_PI);
	gauge->RotateY( -m_fScale * 4.2f );
	gauge->Scale( 1.0f, 1.0f, 1.0f);
	gauge->SetTranslationXYZ(vDrawPos.x, vDrawPos.y + Height, vDrawPos.z);
	gauge->Render();

	if( (int)effect_sort.Index == MONSTER_CONTROL_DAMAGE )
	{
		Red = 1.0f;
		Green = 0.3f;
		Blue = 0.3f;
	}
	else if( (int)effect_sort.Index == MONSTER_CONTROL_GUARD )
	{
		Red = 0.3f;
		Green = 0.3f;
		Blue = 1.0f;
	}

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND,   D3DBLEND_SRCALPHA );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND,  D3DBLEND_INVSRCALPHA );

	Scale = effect_sort.pMobTo->m_AttackVital * 0.01f;

	gauge_real->SetColor(Red, Green, Blue, 1.0f);
	gauge_real->SetRotationY(g_PI);
	gauge_real->RotateY( -m_fScale * 4.2f );
	gauge_real->Scale( 1.0f, Scale, 1.0f);
	gauge_real->SetTranslationXYZ(vDrawPos.x, vDrawPos.y + Height, vDrawPos.z);
	gauge_real->Render();

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE);

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState[2] );

	return FALSE;
}

