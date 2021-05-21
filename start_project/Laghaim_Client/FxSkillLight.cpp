#include "stdafx.h"
#include "sfx.h"
#include "main.h"
#include "NkCharacter.h"
#include "Land.h"
#include "ImageDefine.h"



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFxSkillLight::CFxSkillLight()
{
	skill_light				= NULL;
	m_fScale				= 0.0f;
	m_fFade					= 0.0f;
	m_nTotalFrame			= FX_SKILL_LIGHT_FRAMECOUNT;

	Create();
}

CFxSkillLight::~CFxSkillLight()
{
	SAFE_DELETE(skill_light);
}

void CFxSkillLight::Create()
{
	SAFE_DELETE(skill_light);

	skill_light = new CRectPlane;
	if( skill_light )
		skill_light->Create( 1.0f, 35.0f, 0.0f, FALSE );
}

void CFxSkillLight::LoadRes(IndexedTexture *pIndexedTexture)
{}
void CFxSkillLight::DeleteRes()
{}

BOOL CFxSkillLight::Render(EffectSort &effect_sort)
{
	if( !GET_D3DDEVICE() || !skill_light )
		return true;

	if(effect_sort.nCurFrame >= m_nTotalFrame)
	{
		m_fScale = 0.0f;
		m_fFade = 1.0f;
		return TRUE;
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
	vDrawVec = D3DVECTOR(effect_sort.vPos.x, effect_sort.vPos.y, effect_sort.vPos.z);

	// nate 2004 - 7 - image
	GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_SKILLLIGHT ] ) );

	float red, green, blue;
	red = 0.4f - (0.4f * m_fScale * 0.3f);
	green = 0.4f - (0.4f * m_fScale * 0.3f);
	blue = 0.95f - (0.95f * m_fScale * 0.3f);

	skill_light->SetColor(red, green, blue, m_fFade);
	skill_light->SetBillBoard( vViewVec, TRUE, 0.0f );
	skill_light->Scale( 1.0f, m_fScale + 1.0f, 1.0f );
	skill_light->SetTranslationXYZ(vDrawVec.x, vDrawVec.y + m_fScale, vDrawVec.z);
	skill_light->Render();

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState[2] );

	return FALSE;
}

