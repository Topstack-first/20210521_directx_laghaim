#include "stdafx.h"


#include "main.h"
#include "FxSet.h"

// #include "sfxUtil.h"
// #include "DirtSnd.h"
// #include "LoadEffectSound.h"
// #include "ImageDefine.h"
// #include "GUtil.h"
#include "NkCharacter.h"
#include "NkMob.h"

#include "LHFX_Perom_Field.h"

const int male_frame = 43;
const int fmale_frame = 36;

LHFX_Perom_Field::LHFX_Perom_Field(void)
	: m_lolo(NULL)
{
	memset(m_effect_texture, 0, sizeof(m_effect_texture));
}

LHFX_Perom_Field::~LHFX_Perom_Field(void)
{
	SAFE_DELETE_ARRAY(m_lolo);
}

void LHFX_Perom_Field::LoadRes()
{
	if (!m_lolo)
	{
		m_lolo = new CLolos[2];
		m_lolo[0].SetIndexedTexture( g_pCapsyongTexture );
		m_lolo[0].Read("data/effect/lolos/pm_skill_field.lol");
		m_lolo[1].SetIndexedTexture( g_pCapsyongTexture );
		m_lolo[1].Read("data/effect/lolos/pw_skill_field.lol");
	}

	g_pCapsyongTexture->SetTexturePath("data/effect/textr/");

	m_effect_texture[FxPerom_EffectType__S134] = g_pCapsyongTexture->CreateTextureFromFile("vertical02.sek");
	m_effect_texture[FxPerom_EffectType__S139] = g_pCapsyongTexture->CreateTextureFromFile("vertical02_g.sek");
	m_effect_texture[FxPerom_EffectType__S140] = g_pCapsyongTexture->CreateTextureFromFile("vertical02_p.sek");
	m_effect_texture[FxPerom_EffectType__S141] = g_pCapsyongTexture->CreateTextureFromFile("vertical02_r.sek");
	m_effect_texture[FxPerom_EffectType__S148] = g_pCapsyongTexture->CreateTextureFromFile("vertical02_y.sek");
	m_effect_texture[FxPerom_EffectType__S149] = g_pCapsyongTexture->CreateTextureFromFile("vertical02_bi.sek");
	m_effect_texture[FxPerom_EffectType__S150] = g_pCapsyongTexture->CreateTextureFromFile("vertical02_rr.sek");
}

void LHFX_Perom_Field::DeleteRes()
{
}

bool LHFX_Perom_Field::Render(EffectSort &effect_sort)
{
	D3DMATRIX matWorld, matView, matRot;
	DWORD dwLighting, dwZWrite, dwSrc, dwDest;

	if( !effect_sort.pNkChaFrom )
		return true;

	int frame = 0;
	if( effect_sort.pNkChaFrom->m_Sex == 0)
		frame = male_frame;
	else
		frame = fmale_frame;
	if( effect_sort.nCurFrame > frame )
	{
		effect_sort.nCurFrame = frame;
		return true;
	}

	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_LIGHTING, &dwLighting );
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_ZWRITEENABLE, &dwZWrite );
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_SRCBLEND, &dwSrc );
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_DESTBLEND, &dwDest );

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ZWRITEENABLE, TRUE );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND, D3DBLEND_ONE );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND,   D3DBLEND_SRCALPHA );

	D3DUtil_SetIdentityMatrix( matWorld );
	D3DUtil_SetTranslateMatrix( matWorld, effect_sort.pNkChaFrom->m_wx, effect_sort.pNkChaFrom->m_wy, effect_sort.pNkChaFrom->m_wz);
	D3DUtil_SetIdentityMatrix( matRot );
	D3DUtil_SetRotateYMatrix( matRot, -effect_sort.pNkChaFrom->m_dir );
	D3DMath_MatrixMultiply( matWorld, matRot, matWorld );

	pCMyApp->SetViewMatrix(matView);

	GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_WORLD, &matWorld );
	GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_VIEW, &matView);

	if( effect_sort.pNkChaFrom )
	{
		if( effect_sort.nType == FX_PEROM_SKILL_S134 )
		{
			if( effect_sort.pNkChaFrom->m_Sex == 0 )
				m_lolo[0].Render(GET_D3DDEVICE(), effect_sort.nCurFrame, FALSE, FALSE);
			else if( effect_sort.pNkChaFrom->m_Sex == 1 )
				m_lolo[1].Render(GET_D3DDEVICE(), effect_sort.nCurFrame, FALSE, FALSE);
		}
		else
		{
			if( effect_sort.pNkChaFrom->m_Sex == 0 )
				m_lolo[0].Render2(GET_D3DDEVICE(), GetTypeToTextureIndex(effect_sort.nType), effect_sort.nCurFrame, FALSE, FALSE);
			else if( effect_sort.pNkChaFrom->m_Sex == 1 )
				m_lolo[1].Render2(GET_D3DDEVICE(), GetTypeToTextureIndex(effect_sort.nType),effect_sort.nCurFrame, FALSE, FALSE);
		}
	}

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, dwLighting );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ZWRITEENABLE, dwZWrite );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND, dwSrc );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND, dwDest );

	return false;
}


int LHFX_Perom_Field::GetTypeToTextureIndex(int type)
{
	switch(type)
	{
	case FX_PEROM_SKILL_S134:
		return m_effect_texture[FxPerom_EffectType__S134];
	case FX_PEROM_SKILL_S139:
		return m_effect_texture[FxPerom_EffectType__S139];
	case FX_PEROM_SKILL_S140:
		return m_effect_texture[FxPerom_EffectType__S140];
	case FX_PEROM_SKILL_S141:
		return m_effect_texture[FxPerom_EffectType__S141];
	case FX_PEROM_SKILL_S148:
		return m_effect_texture[FxPerom_EffectType__S148];
	case FX_PEROM_SKILL_S149:
		return m_effect_texture[FxPerom_EffectType__S149];
	case FX_PEROM_SKILL_S150:
		return m_effect_texture[FxPerom_EffectType__S150];
	}

	return m_effect_texture[FxPerom_EffectType__S134];
}