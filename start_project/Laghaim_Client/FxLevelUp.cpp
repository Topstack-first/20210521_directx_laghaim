#include "stdafx.h"
#include "sfx.h"
#include "main.h"
#include "NkCharacter.h"
#include "Land.h"
#include "ImageDefine.h"



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFxLevelUp::CFxLevelUp()
{
	light				= NULL;
	corona				= NULL;
	round_light			= NULL;
	m_fScale			= 0.0f;
	m_fFade				= 1.0f;
	m_nTotalFrame		= FX_LEVELUP_FRAMECOUNT;

	Create();
}

CFxLevelUp::~CFxLevelUp()
{
	SAFE_DELETE(light);
	SAFE_DELETE(corona);
	SAFE_DELETE(round_light);
}

void CFxLevelUp::Create()
{
	SAFE_DELETE(light);
	SAFE_DELETE(corona);
	SAFE_DELETE(round_light);

	light = new CRectPlane;
	if( light )
		light->Create( 0.35f, 30.0f, 5.5f, TRUE );

	corona = new CRectPlane;
	if( corona )
		corona->Create( 25.0f, 25.0f, FALSE );

	round_light = new CCylinderPlane;
	if( round_light )
		round_light->Create( 10.0f, 10.0f, 7.5f, 20, 0.0f, TRUE, 20.0f );
}


void CFxLevelUp::LoadRes(IndexedTexture *pIndexedTexture)
{}

void CFxLevelUp::DeleteRes()
{}

void CFxLevelUp::SetTextureNum()
{}

BOOL CFxLevelUp::Render(EffectSort &effect_sort)
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
	// Set any appropiate state
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
	if( effect_sort.pNkChaTo )
		vDrawPos = D3DVECTOR(effect_sort.pNkChaTo->m_wx, pCMyApp->GetLandPtr()->GetHFHeight(effect_sort.pNkChaTo->m_wx, effect_sort.pNkChaTo->m_wz), effect_sort.pNkChaTo->m_wz);

	float red, green, blue;
	red = green = blue = 0.0f;

	red = 0.38f - (0.38f * m_fScale * 0.15f);
	green = 0.38f - (0.38f * m_fScale * 0.15f);
	blue = 1.0f - (1.0f * m_fScale * 0.15f);

	// nate 2004 - 7 - image
	if( g_pCapsyongTexture )
		GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_LEVELUP_LIGHT ] ) );

	if( light )
	{
		for( int i = 0; i < 30; i++ )
		{
			light->SetColor(red, green, blue, m_fFade);
			light->SetRotationY(g_PI * RandomNum(-1.0f, 1.0f));
			light->RotateX(g_PI * RandomNum(-1.0f, 1.0f));
			light->Scale(1.5f, 0.0f, RandomNum(-1.5f, 1.75f));
			light->SetTranslationXYZ(vDrawPos.x, vDrawPos.y + 12.0f, vDrawPos.z);
			light->Render();
		}
	}

	// nate 2004 - 7 - image
	if( g_pCapsyongTexture )
		GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_LEVELUP_ROUND ] ) );

	red = 0.28f - (0.28f * m_fScale * 0.35f);
	green = 0.28f - (0.28f * m_fScale * 0.35f);
	blue = 0.85f - (0.85f * m_fScale * 0.35f);

	if( round_light )
	{
		round_light->SetColor(red, green, blue, m_fFade);
		round_light->SetRotationY( g_PI );
		round_light->RotateY( m_fScale * 3.0f );
		round_light->Scale( 1.0f, (m_fScale*10.0f)+ 1.0f, 1.0f );
		round_light->SetTranslationXYZ(vDrawPos.x, vDrawPos.y, vDrawPos.z);
		round_light->Render();

		round_light->SetColor(red, green, blue, m_fFade);
		round_light->SetRotationY( g_PI );
		round_light->RotateY( m_fScale * 6.0f );
		round_light->Scale( m_fFade, (m_fScale*20.0f)+ 1.0f, m_fFade );
		round_light->SetTranslationXYZ(vDrawPos.x, vDrawPos.y, vDrawPos.z);
		round_light->Render();
	}

	// nate 2004 - 7 - image
	if( g_pCapsyongTexture )
		GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_LEVELUP_CORONA ] ) );

	red = 0.38f - (0.38f * m_fScale * 0.45f);
	green = 0.38f - (0.38f * m_fScale * 0.45f);
	blue = 1.0f - (1.0f * m_fScale * 0.45f);

	if( corona )
	{
		corona->SetColor(red, green, blue, m_fFade);
		corona->ReCreate(m_fScale * 0.25f);
		corona->SetBillBoard(vViewVec, FALSE, 0.0f);
		corona->Scale(m_fScale, m_fScale, 0.0f);
		corona->SetTranslationXYZ(vDrawPos.x, vDrawPos.y + 12.0f, vDrawPos.z);
		corona->Render();
	}

	if( effect_sort.Index >= 5 )
		effect_sort.Index = 0;

	m_fScale = ((float)effect_sort.Index / 5);
	m_fFade = 1.0f - m_fScale;

	red = 0.58f - (0.58f * m_fScale * 0.55f);
	green = 0.58f - (0.58f * m_fScale * 0.55f);
	blue = 1.0f - (1.0f * m_fScale * 0.55f);

	if( corona )
	{
		corona->SetColor(red, green, blue, m_fFade);
		corona->SetRotationX( g_PI * 0.5f );
		corona->RotateZ( m_fScale * 5.0f );
		corona->Scale( m_fScale, m_fScale, 1.0f );
		corona->SetTranslationXYZ(vDrawPos.x, vDrawPos.y + 1.5f, vDrawPos.z);
		corona->Render();

		corona->SetRotationX( g_PI * 0.5f );
		corona->RotateZ( m_fScale * 5.0f );
		corona->SetTranslationXYZ(vDrawPos.x, vDrawPos.y + (m_fScale*25.0f), vDrawPos.z);
		corona->Render();
	}

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE);

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState[2] );

	return FALSE;
}
