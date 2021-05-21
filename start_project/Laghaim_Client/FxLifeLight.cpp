#include "stdafx.h"
#include "sfx.h"
#include "main.h"
#include "NkCharacter.h"
#include "Land.h"
#include "ImageDefine.h"



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFxLifeLight::CFxLifeLight()
{
	life_light				= NULL;
	m_fScale				= 0.0f;
	m_fFade					= 0.0f;
	m_nTotalFrame			= FX_LIFELIGHT_FRAMECOUNT;

	Create();
}

CFxLifeLight::~CFxLifeLight()
{
	SAFE_DELETE(life_light);
}

void CFxLifeLight::Create()
{
	SAFE_DELETE(life_light);

	life_light = new CRectPlane;
	if( life_light )
		life_light->Create( 4.0f, 4.0f, FALSE );
}

void CFxLifeLight::LoadRes(IndexedTexture *pIndexedTexture)
{}
void CFxLifeLight::DeleteRes()
{}

BOOL CFxLifeLight::Render(EffectSort &effect_sort)
{
	if( !GET_D3DDEVICE() || !effect_sort.pNkChaFrom )
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

	if( effect_sort.Index >= 30 )
		effect_sort.Index = 0;

	m_fScale = ((float)effect_sort.Index / 30);
	m_fFade = 1.0f - m_fScale;

	float red, green, blue;
	red = 0.3f - (0.3f * m_fScale * 0.5f);
	green = 0.9f - (0.9f * m_fScale * 0.5f);
	blue = 0.384f - (0.384f * m_fScale * 0.5f);


	// nate 2004 - 7 - image
	if( g_pCapsyongTexture )
		GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_LIFELIGHT ] ) );

	float step = g_2_PI / 60;
	vDrawVec.x += 10.0f * (float)sin(step*effect_sort.nCurFrame);
	vDrawVec.z += 10.0f * (float)cos(step*effect_sort.nCurFrame);

	if( life_light )
	{
		life_light->SetColor(red, green, blue, m_fFade);
		life_light->SetBillBoard( vViewVec, FALSE, 0.0f );
		life_light->Scale( 1.0f, 1.0f, 1.0f );
		life_light->SetTranslationXYZ(vDrawVec.x, vDrawVec.y + Add_Height[effect_sort.pNkChaFrom->m_Race][effect_sort.pNkChaFrom->m_Sex], vDrawVec.z);
		life_light->Render();

		vDrawVec = D3DVECTOR(effect_sort.pNkChaFrom->m_wx, effect_sort.pNkChaFrom->m_wy, effect_sort.pNkChaFrom->m_wz);
		vDrawVec.x += 10.0f * (float)sin(step*effect_sort.nCurFrame+step*15);
		vDrawVec.z += 10.0f * (float)cos(step*effect_sort.nCurFrame+step*15);

		life_light->SetColor(red, green, blue, m_fFade);
		life_light->SetBillBoard( vViewVec, FALSE, 0.0f );
		life_light->Scale( 1.0f, 1.0f, 1.0f );
		life_light->SetTranslationXYZ(vDrawVec.x, vDrawVec.y + Add_Height[effect_sort.pNkChaFrom->m_Race][effect_sort.pNkChaFrom->m_Sex], vDrawVec.z);
		life_light->Render();

		vDrawVec = D3DVECTOR(effect_sort.pNkChaFrom->m_wx, effect_sort.pNkChaFrom->m_wy, effect_sort.pNkChaFrom->m_wz);
		vDrawVec.x += 10.0f * (float)sin(step*effect_sort.nCurFrame+step*30);
		vDrawVec.z += 10.0f * (float)cos(step*effect_sort.nCurFrame+step*30);

		life_light->SetColor(red, green, blue, m_fFade);
		life_light->SetBillBoard( vViewVec, FALSE, 0.0f );
		life_light->Scale( 1.0f, 1.0f, 1.0f );
		life_light->SetTranslationXYZ(vDrawVec.x, vDrawVec.y + Add_Height[effect_sort.pNkChaFrom->m_Race][effect_sort.pNkChaFrom->m_Sex], vDrawVec.z);
		life_light->Render();

		vDrawVec = D3DVECTOR(effect_sort.pNkChaFrom->m_wx, effect_sort.pNkChaFrom->m_wy, effect_sort.pNkChaFrom->m_wz);
		vDrawVec.x += 10.0f * (float)sin(step*effect_sort.nCurFrame+step*45);
		vDrawVec.z += 10.0f * (float)cos(step*effect_sort.nCurFrame+step*45);

		life_light->SetColor(red, green, blue, m_fFade);
		life_light->SetBillBoard( vViewVec, FALSE, 0.0f );
		life_light->Scale( 1.0f, 1.0f, 1.0f );
		life_light->SetTranslationXYZ(vDrawVec.x, vDrawVec.y + Add_Height[effect_sort.pNkChaFrom->m_Race][effect_sort.pNkChaFrom->m_Sex], vDrawVec.z);
		life_light->Render();
	}

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState[2] );

	return FALSE;
}
