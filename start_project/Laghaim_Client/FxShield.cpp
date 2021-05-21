#include "stdafx.h"
#include "sfx.h"
#include "main.h"
#include "NkCharacter.h"
#include "Land.h"
#include "ImageDefine.h"




//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFxShield::CFxShield()
{
	shield_round1			= NULL;
	shield_round2			= NULL;
	m_fScale				= 0.0f;
	m_fFade					= 0.0f;
	m_nTotalFrame			= FX_SHIELD_FRAMECOUNT;

	Create();
}

CFxShield::~CFxShield()
{
	SAFE_DELETE(shield_round1);
	SAFE_DELETE(shield_round2);
}

void CFxShield::Create()
{
	SAFE_DELETE(shield_round1);
	SAFE_DELETE(shield_round2);

	shield_round1 = new CRectPlane;
	if( shield_round1 )
		shield_round1->Create( 30.5f, 30.5f, TRUE );

	shield_round2 = new CRectPlane;
	if( shield_round2 )
		shield_round2->Create( 30.5f, 30.5f, TRUE );
}

void CFxShield::LoadRes(IndexedTexture *pIndexedTexture)
{}
void CFxShield::DeleteRes()
{}

BOOL CFxShield::Render(EffectSort &effect_sort)
{
	if( !GET_D3DDEVICE() )
		return true;

	if( !effect_sort.pNkChaTo )
		return TRUE;

	if( !effect_sort.pNkChaTo->isShieldLoad )
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

	if( g_pRoh->GetCombat() == FALSE )
		return TRUE;

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

	D3DVECTOR	vDrawVec;
	if( effect_sort.pNkChaTo )
		vDrawVec = D3DVECTOR(effect_sort.pNkChaTo->m_wx, effect_sort.pNkChaTo->m_wy, effect_sort.pNkChaTo->m_wz);

	float Red = 0.0f;
	float Green = 0.0f;
	float Blue = 0.0f;
	float Scale = 0.0f;
	float Height = 0.0f;

	switch ( effect_sort.pNkChaTo->m_Race )
	{
	case 1:

		Red = 0.0f;
		Green = 0.4f;
		Blue = 1.0f;

		if( effect_sort.pNkChaTo->m_Sex == 0 ) // male
		{
			Scale = 1.0f;
			Height = 11.0f;
		}
		else if( effect_sort.pNkChaTo->m_Sex == 1 ) // female
		{
			Scale = 0.95f;
			Height = 10.5f;
		}

		if( effect_sort.pNkChaTo->GetMotionState() == CHA_SWIM ||
				effect_sort.pNkChaTo->GetMotionState() == CHA_SWIM_STAND )
			Height = 0.0f;

		break;
	case 2:

		Red = 1.0f;
		Green = 0.309f;
		Blue = 0.376f;

		Scale = 0.9f;
		Height = 10.0f;
		break;
	case 3:

		Red = 0.415f;
		Green = 0.9f;
		Blue = 0.384f;

		if( effect_sort.pNkChaTo->m_Sex == 0 ) // male
		{
			Scale = 1.0f;
			Height = 11.0f;
		}
		else if( effect_sort.pNkChaTo->m_Sex == 1 ) // female
		{
			Scale = 0.95f;
			Height = 10.5f;
		}
		if( effect_sort.pNkChaTo->GetMotionState() == CHA_SWIM ||
				effect_sort.pNkChaTo->GetMotionState() == CHA_SWIM_STAND )
			Height = 0.0f;
		break;

	default:
		break;
	}

	if( shield_round1 )
	{
		// nate 2004 - 7 - image
		GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_SHIELD1 ] ) );
		shield_round1->SetColor(Red, Green, Blue, m_fFade);
		shield_round1->SetRotationY( g_PI * 0.5f );
		shield_round1->RotateZ( -m_fScale * 70.0f );
		shield_round1->Scale( Scale, Scale, Scale );

		shield_round1->SetTranslationXYZ(vDrawVec.x, vDrawVec.y + Height, vDrawVec.z);
		shield_round1->Render();
	}

	if( shield_round2 )
	{
		// nate 2004 - 7 - image
		GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_SHIELD2 ] ) );
		shield_round2->SetColor(Red, Green, Blue, m_fFade);
		shield_round2->SetRotationY( g_PI * 0.5f );
		shield_round2->RotateX( -m_fScale * 70.5f );
		shield_round2->Scale( Scale, Scale, Scale );
		shield_round2->SetTranslationXYZ(vDrawVec.x, vDrawVec.y + Height, vDrawVec.z);
	}

	if( shield_round2 )
	{
		// nate 2004 - 7 - image
		GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_SHIELD2 ] ) );

		if( (effect_sort.pNkChaTo->m_nManaShield > 20) ||
				(effect_sort.pNkChaTo->m_nElectricShield > 20) ||
				(effect_sort.pNkChaTo->m_nSafetyShield > 20) )
			shield_round2->Render();
	}

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE);

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState[2] );

	return FALSE;
}

