#include "stdafx.h"
#include "sfx.h"
#include "main.h"
#include "NkCharacter.h"
#include "Land.h"
#include "ImageDefine.h"



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFxLifeCommon::CFxLifeCommon()
{
	life_ring				= NULL;
	m_fScale				= 0.0f;
	m_fFade					= 0.0f;
	m_nTotalFrame			= FX_LIFECOMMON_FRAMECOUNT;

	Create();
}

CFxLifeCommon::~CFxLifeCommon()
{
	SAFE_DELETE(life_ring);
}

void CFxLifeCommon::Create()
{
	SAFE_DELETE(life_ring);

	life_ring = new CCylinderPlane;
	if( life_ring )
		life_ring->Create( 7.0f, 8.0f, 0.5f, 20.0f, TRUE );
}

void CFxLifeCommon::LoadRes(IndexedTexture *pIndexedTexture)
{}
void CFxLifeCommon::DeleteRes()
{}

BOOL CFxLifeCommon::Render(EffectSort &effect_sort)
{
	if( !GET_D3DDEVICE() )
		return true;

	if( !effect_sort.pNkChaFrom )
		return TRUE;

	if( effect_sort.pNkChaFrom->m_InChaAidiaSkill == 0 )
	{
		effect_sort.pNkChaFrom->m_ShowAidiaSkillEffect = FALSE;
		m_fScale = 0.0f;
		m_fFade = 1.0f;
		effect_sort.nCurFrame = m_nTotalFrame;
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

	D3DVECTOR	vDrawVec;
	if( effect_sort.pNkChaFrom )
		vDrawVec = D3DVECTOR(effect_sort.pNkChaFrom->m_wx, effect_sort.pNkChaFrom->m_wy, effect_sort.pNkChaFrom->m_wz);

	float Add_Height[NUM_RACE][NUM_SEX] = {	29.0f*0.5f, 26.5f*0.5f,
											27.5f*0.5f, 26.5f*0.5f,
											22.0f*0.5f, 22.0f*0.5f,
											27.5f*0.5f, 26.5f*0.5f,
											29.5f*0.5f, 27.0f*0.5f,
											29.5f*0.5f, 27.0f*0.5f
										  };

	vDrawVec.y += Add_Height[effect_sort.pNkChaFrom->m_Race][effect_sort.pNkChaFrom->m_Sex];

	float red, green, blue;
	red = 0.2f;// - (0.2f * m_fScale * 0.3f);
	green = 0.3f;// - (0.3f * m_fScale * 0.3f);
	blue = 0.9f;// - (0.9f * m_fScale * 0.3f);

	// nate 2004 - 7 - image
	if( g_pCapsyongTexture )
		GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_LIFECOMMONT ] ) );

	if( life_ring )
	{
		life_ring->SetColor(red, green, blue, m_fFade);
		life_ring->SetRotationY(g_PI);
		life_ring->RotateZ( g_PI );
		life_ring->RotateY( -m_fScale * 80.0f );
		life_ring->Scale( 1.3f, 1.0f, 1.3f );
		life_ring->SetTranslationXYZ(vDrawVec.x, vDrawVec.y, vDrawVec.z);
		life_ring->Render();

		life_ring->SetColor(red, green, blue, m_fFade);
		life_ring->SetRotationY(g_PI);
		life_ring->RotateZ( g_PI * 0.75f );
		life_ring->RotateY( m_fScale * 90.0f );
		life_ring->Scale( 1.2f, 1.0f, 1.2f );
		life_ring->SetTranslationXYZ(vDrawVec.x, vDrawVec.y, vDrawVec.z);
		life_ring->Render();

		life_ring->SetColor(red, green, blue, m_fFade);
		life_ring->SetRotationY(g_PI);
		life_ring->RotateZ( g_PI * 0.5f );
		life_ring->RotateY( m_fScale * 100.0f );
		life_ring->Scale( 1.1f, 1.0f, 1.1f );
		life_ring->SetTranslationXYZ(vDrawVec.x, vDrawVec.y, vDrawVec.z);
		life_ring->Render();

		life_ring->SetColor(red, green, blue, m_fFade);
		life_ring->SetRotationY(g_PI);
		life_ring->RotateZ( g_PI * 0.25f );
		life_ring->RotateY( -m_fScale * 80.0f );
		life_ring->Scale( 1.25f, 1.0f, 1.25f );
		life_ring->SetTranslationXYZ(vDrawVec.x, vDrawVec.y, vDrawVec.z);
		life_ring->Render();
	}

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState[2] );

	return FALSE;
}


