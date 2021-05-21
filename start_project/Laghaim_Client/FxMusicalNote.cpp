#include "stdafx.h"
#include "sfx.h"
#include "main.h"
#include "NkCharacter.h"
#include "Land.h"
#include "skill.h"
// nate 2004 - 7 - image
#include "ImageDefine.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFxMusicalNote::CFxMusicalNote()
{
	musical_note				= NULL;
	Create();
}

CFxMusicalNote::~CFxMusicalNote()
{
	SAFE_DELETE(musical_note);
}

void CFxMusicalNote::Create()
{
	SAFE_DELETE(musical_note);

	musical_note = new CRectPlane;
	if( musical_note )
		musical_note->Create( 4.0f, 4.0f, FALSE );
}

void CFxMusicalNote::LoadRes(IndexedTexture *pIndexedTexture)
{}
void CFxMusicalNote::DeleteRes()
{}

BOOL CFxMusicalNote::Render(EffectSort &effect_sort)
{
	if( !GET_D3DDEVICE() || !musical_note )
		return true;

	if( !effect_sort.pNkChaFrom )
		return TRUE;

	int total_frame = FX_LIFELIGHT_FRAMECOUNT;

	if( effect_sort.pNkChaFrom->m_InChaAidiaSkill == 0 )
	{
		effect_sort.pNkChaFrom->m_ShowAidiaSkillEffect = FALSE;
		effect_sort.nCurFrame = total_frame;
		return TRUE;
	}
	else
	{
		if( effect_sort.nCurFrame >= total_frame )
			effect_sort.nCurFrame = 0;
	}

	float maxScale = 3.0f;

	float fScale = (maxScale * effect_sort.nCurFrame) / total_frame;
	float fFade = 1.0f - (fScale * 0.3f);

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

	fScale = ((float)effect_sort.Index / 30);
	fFade = 1.0f - fScale;

	float red = 0;
	float green = 0;
	float blue = 0;

	if( effect_sort.pNkChaFrom->m_InChaAidiaSkill == SKILL_MELODY_OF_DEF )
	{
		red = 0.3f - (0.3f * fScale * 0.5f);
		green = 0.3f - (0.3f * fScale * 0.5f);
		blue = 0.9f - (0.9f * fScale * 0.5f);
	}
	else if( effect_sort.pNkChaFrom->m_InChaAidiaSkill == SKILL_MELODY_OF_GUD )
	{
		red = 0.3f - (0.3f * fScale * 0.5f);
		green = 0.9f - (0.9f * fScale * 0.5f);
		blue = 0.384f - (0.384f * fScale * 0.5f);
	}

	// nate 2004 - 7 - image
	GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_MUSICALNOTE_LIFE ] ) );

	float step = g_2_PI / 60;
	vDrawVec.x += 10.0f * (float)sin(step*effect_sort.nCurFrame);
	vDrawVec.z += 10.0f * (float)cos(step*effect_sort.nCurFrame);

	musical_note->SetColor(red, green, blue, fFade);
	musical_note->SetBillBoard( vViewVec, FALSE, 0.0f );
	musical_note->Scale( 1.0f, 1.0f, 1.0f );
	musical_note->SetTranslationXYZ(vDrawVec.x, vDrawVec.y + Add_Height[effect_sort.pNkChaFrom->m_Race][effect_sort.pNkChaFrom->m_Sex], vDrawVec.z);
	musical_note->Render();

	vDrawVec = D3DVECTOR(effect_sort.pNkChaFrom->m_wx, effect_sort.pNkChaFrom->m_wy, effect_sort.pNkChaFrom->m_wz);
	vDrawVec.x += 10.0f * (float)sin(step*effect_sort.nCurFrame+step*15);
	vDrawVec.z += 10.0f * (float)cos(step*effect_sort.nCurFrame+step*15);

	musical_note->SetColor(red, green, blue, fFade);
	musical_note->SetBillBoard( vViewVec, FALSE, 0.0f );
	musical_note->Scale( 1.0f, 1.0f, 1.0f );
	musical_note->SetTranslationXYZ(vDrawVec.x, vDrawVec.y + Add_Height[effect_sort.pNkChaFrom->m_Race][effect_sort.pNkChaFrom->m_Sex], vDrawVec.z);
	musical_note->Render();

	vDrawVec = D3DVECTOR(effect_sort.pNkChaFrom->m_wx, effect_sort.pNkChaFrom->m_wy, effect_sort.pNkChaFrom->m_wz);
	vDrawVec.x += 10.0f * (float)sin(step*effect_sort.nCurFrame+step*30);
	vDrawVec.z += 10.0f * (float)cos(step*effect_sort.nCurFrame+step*30);

	musical_note->SetColor(red, green, blue, fFade);
	musical_note->SetBillBoard( vViewVec, FALSE, 0.0f );
	musical_note->Scale( 1.0f, 1.0f, 1.0f );
	musical_note->SetTranslationXYZ(vDrawVec.x, vDrawVec.y + Add_Height[effect_sort.pNkChaFrom->m_Race][effect_sort.pNkChaFrom->m_Sex], vDrawVec.z);
	musical_note->Render();

	vDrawVec = D3DVECTOR(effect_sort.pNkChaFrom->m_wx, effect_sort.pNkChaFrom->m_wy, effect_sort.pNkChaFrom->m_wz);
	vDrawVec.x += 10.0f * (float)sin(step*effect_sort.nCurFrame+step*45);
	vDrawVec.z += 10.0f * (float)cos(step*effect_sort.nCurFrame+step*45);

	musical_note->SetColor(red, green, blue, fFade);
	musical_note->SetBillBoard( vViewVec, FALSE, 0.0f );
	musical_note->Scale( 1.0f, 1.0f, 1.0f );
	musical_note->SetTranslationXYZ(vDrawVec.x, vDrawVec.y + Add_Height[effect_sort.pNkChaFrom->m_Race][effect_sort.pNkChaFrom->m_Sex], vDrawVec.z);
	musical_note->Render();

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState[2] );

	return FALSE;
}
