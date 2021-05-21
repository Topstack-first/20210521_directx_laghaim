#include "stdafx.h"
#include "sfx.h"
#include "main.h"
#include "NkCharacter.h"
#include "ImageDefine.h"



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFxHealEffects::CFxHealEffects()
{
	guard				= NULL;
	guard2				= NULL;
	light				= NULL;
	explosion			= NULL;
	heal_round1			= NULL;
	heal_round2			= NULL;
	heal_round3			= NULL;
	m_fScale			= 0.0f;
	m_fFade				= 1.0f;
	m_nTotalFrame		= FX_HEALEFFECTS_FRAMECOUNT;

	Create();
}

CFxHealEffects::~CFxHealEffects()
{
	SAFE_DELETE(guard);
	SAFE_DELETE(guard2);
	SAFE_DELETE(light);
	SAFE_DELETE(explosion);
	SAFE_DELETE(heal_round1);
	SAFE_DELETE(heal_round2);
	SAFE_DELETE(heal_round3);
}

void CFxHealEffects::Create()
{
	SAFE_DELETE(guard);
	SAFE_DELETE(guard2);
	SAFE_DELETE(light);
	SAFE_DELETE(explosion);
	SAFE_DELETE(heal_round1);
	SAFE_DELETE(heal_round2);
	SAFE_DELETE(heal_round3);

	guard = new CCylinderPlane;
	if( guard )
		guard->Create( 7.0f, 7.0f, 5.0f, 20, 0.0f, TRUE );

	guard2 = new CCylinderPlane;
	if( guard2 )
		guard2->Create( 7.5f, 7.5f, 3.0f, 20, 0.0f, TRUE );

	light = new CRectPlane;
	if( light )
		light->Create( 0.35f, 30.0f, 5.5f, TRUE );

	explosion = new CSphere;
	if( explosion )
		explosion->Create();

	heal_round1 = new CRectPlane;
	if( heal_round1 )
		heal_round1->Create( 5.0f, 5.0f, TRUE );

	heal_round2 = new CRectPlane;
	if( heal_round2 )
		heal_round2->Create( 5.0f, 5.0f, TRUE );

	heal_round3 = new CRectPlane;
	if( heal_round3 )
		heal_round3->Create( 5.0f, 5.0f, TRUE );
}

void CFxHealEffects::LoadRes(IndexedTexture *pIndexedTexture)
{}

void CFxHealEffects::DeleteRes()
{}

BOOL CFxHealEffects::Render(EffectSort &effect_sort)
{
	if( !GET_D3DDEVICE() || !effect_sort.pNkChaTo )
		return TRUE;

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
	if( effect_sort.pNkChaTo )
		vDrawPos = D3DVECTOR(effect_sort.pNkChaTo->m_wx, effect_sort.pNkChaTo->m_wy, effect_sort.pNkChaTo->m_wz);

	float red, green, blue;
	red = green = blue = 0.0f;

	// nate 2004 - 7 - image
	if( g_pCapsyongTexture )
		GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_HEAL_GUARD ] ));

	red = RandomNum(0.0f, 1.0f);
	green = RandomNum(0.0f, 1.0f);
	blue = RandomNum(0.0f, 1.0f);

	if( explosion )
	{
		explosion->SetColor(red, green, blue, m_fFade);
		explosion->SetTranslationXYZ(vDrawPos.x, vDrawPos.y + 22.0f, vDrawPos.z);
		explosion->Render();
	}

	red = 0.315f;
	green = 0.9f;
	blue = 0.284f;

	float guard_scale;
	guard_scale = maxScale - (m_fScale * 1.0f);

	if( guard )
	{
		guard->SetColor(red, green, blue, m_fFade);
		guard->SetRotationY(g_PI);
		guard->RotateY( -m_fScale * 4.2f );
		guard->Scale( guard_scale, m_fScale * 5.0f, guard_scale );
		guard->SetTranslationXYZ(vDrawPos.x, vDrawPos.y, vDrawPos.z);
		guard->Render();
	}

	red = 0.0f;
	green = 0.9f;
	blue = 0.0f;

	if( guard2 )
	{
		guard2->SetColor(red, green, blue, m_fFade);
		guard2->SetRotationY(g_PI);
		guard2->RotateY( -m_fScale * 4.2f );
		guard2->Scale( guard_scale, m_fScale * 5.0f, guard_scale );
		guard2->SetTranslationXYZ(vDrawPos.x, vDrawPos.y, vDrawPos.z);
		guard2->Render();
	}

	// nate 2004 - 7 - image
	if( g_pCapsyongTexture )
		GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_HEAL_LIGHT ] ));

	for( int i = 0; i < 3; i++ )
	{
		red = RandomNum(0.0f, 1.0f);
		green = RandomNum(0.0f, 1.0f);
		blue = RandomNum(0.0f, 1.0f);

		if( light )
		{
			light->SetColor(red, green, blue, m_fFade);
			light->SetRotationY(g_PI * RandomNum(-1.0f, 1.0f));
			light->RotateX(g_PI * RandomNum(-1.0f, 1.0f));
			light->Scale(1.5f, 0.0f, RandomNum(1.0f, 1.2f));
			light->SetTranslationXYZ(vDrawPos.x, vDrawPos.y + 10.0f, vDrawPos.z);
			light->Render();
		}
	}

	// nate 2004 - 7 - image
	if( g_pCapsyongTexture )
		GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_HEAL_ROUND ] ) );

	red = RandomNum(0.0f, 1.0f);
	green = RandomNum(0.0f, 1.0f);
	blue = RandomNum(0.0f, 1.0f);

	if( heal_round1 )
	{
		heal_round1->SetColor(red, green, blue, m_fFade);
		heal_round1->SetRotationY( g_PI * 0.5f );
		heal_round1->RotateZ( -m_fScale * 3.5f );
		heal_round1->SetTranslationXYZ(vDrawPos.x, vDrawPos.y + 22.0f, vDrawPos.z);
		heal_round1->Render();
	}

	red = RandomNum(0.0f, 1.0f);
	green = RandomNum(0.0f, 1.0f);
	blue = RandomNum(0.0f, 1.0f);

	if( heal_round2 )
	{
		heal_round2->SetColor(red, green, blue, m_fFade);
		heal_round2->SetRotationX( g_PI * 0.5f );
		heal_round2->RotateZ( m_fScale * -3.5f );
		heal_round2->SetTranslationXYZ(vDrawPos.x, vDrawPos.y + 22.0f, vDrawPos.z);
		heal_round2->Render();
	}

	red = RandomNum(0.0f, 1.0f);
	green = RandomNum(0.0f, 1.0f);
	blue = RandomNum(0.0f, 1.0f);

	if( heal_round3 )
	{
		heal_round3->SetColor(red, green, blue, m_fFade);
		heal_round3->SetRotationY( g_PI * 0.5f );
		heal_round3->RotateX( -m_fScale * 3.5f );
		heal_round3->SetTranslationXYZ(vDrawPos.x, vDrawPos.y + 22.0f, vDrawPos.z);
		heal_round3->Render();
	}

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE);

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState[2] );

	return FALSE;
}

