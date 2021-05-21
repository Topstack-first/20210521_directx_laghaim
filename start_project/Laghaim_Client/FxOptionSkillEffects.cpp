#include "stdafx.h"
#include "sfx.h"
#include "main.h"
#include "NkCharacter.h"
#include "Land.h"
#include "skill.h"
#include "ImageDefine.h"
#include "DirtSnd.h"
#include "LoadEffectSound.h"




extern int g_EffectSound[];
extern float g_fDSDistance;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFxOptionSkillEffects::CFxOptionSkillEffects()
{
	skill					= NULL;
	corona					= NULL;
	m_fScale				= 0.0f;
	m_fFade					= 0.0f;
	fFlag					= 0.03f;
	m_nTotalFrame			= FX_OPTIONSKILLEFFECTS_FRAMECOUNT;

	Create();
}

CFxOptionSkillEffects::~CFxOptionSkillEffects()
{
	SAFE_DELETE(skill);
	SAFE_DELETE(corona);
}

void CFxOptionSkillEffects::Create()
{
	SAFE_DELETE(skill);
	SAFE_DELETE(corona);

	skill = new CRectPlane;
	if( skill )
		skill->Create( 17.0f, 17.0f, FALSE );

	corona = new CRectPlane;
	if( corona )
		corona->Create( 17.0f, 17.0f, FALSE );
}

void CFxOptionSkillEffects::LoadRes(IndexedTexture *pIndexedTexture)
{}
void CFxOptionSkillEffects::DeleteRes()
{}

BOOL CFxOptionSkillEffects::Render(EffectSort &effect_sort)
{
	if( !GET_D3DDEVICE() || !corona || !skill )
		return true;

	if( !effect_sort.pNkChaTo )
		return TRUE;

	if( effect_sort.pNkChaTo->m_InChaSkill != 0 || effect_sort.pNkChaTo->m_activate_buff_skill != 0 )
	{
		if( effect_sort.nCurFrame >= m_nTotalFrame )
			effect_sort.nCurFrame = 0;
	}
	else
	{
		effect_sort.pNkChaTo->m_ShowSkillEffect = FALSE;
		m_fScale = 0.0f;
		m_fFade = 1.0f;
		effect_sort.nCurFrame = m_nTotalFrame;
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
	if( effect_sort.pNkChaTo )
		vDrawVec = D3DVECTOR(effect_sort.pNkChaTo->m_wx, effect_sort.pNkChaTo->m_wy, effect_sort.pNkChaTo->m_wz);


	// 0 ºÒÄ­, 1 Ä«ÀÌ¸³Åæ, 2 ¿¡ÀÌµð¾Æ, 3 ÈÞ¸Õ
	// shield 1.Ä«ÀÌ = ÆÄ¶û, 2.¿¡ÀÌµð¾Æ = »¡°­, 3.ÈÞ¸Õ = ³ì»ö
	// mutant 0.ºÒÄ­ = »¡°­, 1.Ä«ÀÌ = ÆÄ¶û, 2.¿¡ÀÌµð¾Æ = ³ì»ö, 3.ÈÞ¸Õ = ³ë¶û
	// skill 0.ºÒÄ­ = ³ë¶û, 1.Ä«ÀÌ = ÆÄ¶û, 2.¿¡ÀÌµð¾Æ = »¡°­, 3.ÈÞ¸Õ = ³ì»ö
	// pCha->m_Sex, pCha->m_Race;

	int  chaskill = 0;
	if( effect_sort.pNkChaTo->m_Race == RACE_PEROM )
	{
		chaskill = effect_sort.pNkChaTo->m_activate_buff_skill;
	}
	else
	{
		chaskill = effect_sort.pNkChaTo->m_InChaSkill;
	}


	float red, green, blue;

	switch( chaskill )
	{
	case EFFECT_SKILL_IRON_MUSCLE:
	case EFFECT_PEROM_OPTIONSKILL_S134:
		// nate 2004 - 7 - image
		GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_OPTIONSKILL_DEFENSE ] ) );
		red = 0.9f * 0.5f;// - (0.9f * m_fScale * 0.3f);
		green = 0.9f * 0.5f;// - (0.9f * m_fScale * 0.3f);
		blue = 0.484f * 0.5f;// - (0.484f * m_fScale * 0.3f);

		skill->SetColor(red, green, blue, m_fFade);
		skill->SetRotationX( g_PI * 0.5f );
		skill->RotateZ( -m_fScale * 75.0f );
		skill->Scale( 1.0f, 1.0f, 1.0f );
		skill->SetTranslationXYZ(vDrawVec.x, vDrawVec.y + 3.5f, vDrawVec.z);
		skill->Render();

		if( effect_sort.Index >= 50 )
			effect_sort.Index = 0;

		m_fScale = ((float)effect_sort.Index / 50);
		m_fFade = 1.0f - m_fScale;

		red = 0.9f - (0.9f * m_fScale * 0.7f);
		green = 0.9f - (0.9f * m_fScale * 0.7f);
		blue = 0.484f - (0.484f * m_fScale * 0.7f);

		skill->SetColor(red, green, blue, m_fFade);
		skill->SetRotationX( g_PI * 0.5f );
		skill->RotateZ( m_fScale * 3.0f );
		skill->Scale( 1.0f + (m_fScale*0.5f), 1.0f + (m_fScale*0.5f), 1.0f + (m_fScale*0.5f));
		skill->SetTranslationXYZ(vDrawVec.x, vDrawVec.y + 3.5f + (m_fScale * 3.0), vDrawVec.z);
		skill->Render();

		// nate 2004 - 7 - image
		GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_OPTIONSKILL_CORONA ] ) );

		corona->SetColor(m_fFade, m_fFade, m_fFade, m_fFade);
		corona->SetRotationX( g_PI * 0.5f );
		corona->RotateZ( m_fScale * 75.0f );
		corona->Scale( RandomNum(1.3f, 1.5f), RandomNum(1.3f, 1.5f), RandomNum(1.3f, 1.5f) );
		corona->SetTranslationXYZ(vDrawVec.x, vDrawVec.y + 3.5f, vDrawVec.z);
		corona->Render();
		if (g_pDSound)
		{
			float x, y, z;
			x = (vDrawVec.x - g_pRoh->m_wx) / g_fDSDistance;
			y = (vDrawVec.y - g_pRoh->m_wy) / g_fDSDistance;
			z = (vDrawVec.z - g_pRoh->m_wz) / g_fDSDistance;
			//	if(!g_pDSound->IsPlaying(g_EffectSound[EFF_SND_IRON_SKIN]))
			g_pDSound->Play(g_EffectSound[EFF_SND_IRON_SKIN], x, y, z);
		}
		break;
	case EFFECT_SKILL_FURY_EXPLOSION:
	case EFFECT_SKILL_THOUGHT_STORM:
	case EFFECT_SKILL_PIERCING:
	case EFFECT_PEROM_OPTIONSKILL_S148:
		// nate 2004 - 7 - image
		GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_OPTIONSKILL_OFFENSE ] ) );
		red = 0.4f * 0.5f;// - (0.4f * m_fScale * 0.3f);
		green = 0.9f * 0.5f;// - (0.9f * m_fScale * 0.3f);
		blue = 0.384f * 0.5f;// - (0.384f * m_fScale * 0.3f);

		skill->SetColor(red, green, blue, m_fFade);
		skill->SetRotationX( g_PI * 0.5f );
		skill->RotateZ( m_fScale * 75.0f );
		skill->Scale( 1.0f, 1.0f, 1.0f );
		skill->SetTranslationXYZ(vDrawVec.x, vDrawVec.y + 3.5f, vDrawVec.z);
		skill->Render();

		if( effect_sort.Index >= 45 )
			effect_sort.Index = 0;

		m_fScale = ((float)effect_sort.Index / 45);
		m_fFade = 1.0f - m_fScale;

		red = 0.4f - (0.4f * m_fScale);
		green = 0.9f - (0.9f * m_fScale);
		blue = 0.384f - (0.384f * m_fScale);

		skill->SetColor(red, green, blue, m_fFade);
		skill->SetRotationX( g_PI * 0.5f );
		skill->RotateZ( -m_fScale * 3.0f );
		skill->Scale( 1.0f + (m_fScale * 0.5f), 1.0f + (m_fScale * 0.5f), 1.0f + (m_fScale * 0.5f));
		skill->SetTranslationXYZ(vDrawVec.x, vDrawVec.y + 3.5f, vDrawVec.z);
		skill->Render();

		// nate 2004 - 7 - image
		GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_OPTIONSKILL_CORONA ] ) );
		corona->SetColor(m_fFade, m_fFade, m_fFade, m_fFade);
		corona->SetRotationX( g_PI * 0.5f );
		corona->RotateZ( m_fScale * 75.0f );
		corona->Scale( 1.0f + m_fScale, 1.0f + m_fScale, 1.0f + m_fScale);
		corona->SetTranslationXYZ(vDrawVec.x, vDrawVec.y + 3.5f, vDrawVec.z);
		corona->Render();
		break;
	case EFFECT_SKILL_BLESSING:
	case EFFECT_PEROM_OPTIONSKILL_S149:
		// nate 2004 - 7 - image
		GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_OPTIONSKILL_CARE ] ) );
		red = 0.9f * 0.5f;// - (0.9f * m_fScale * 0.75f);
		green = 0.4f * 0.5f;// - (0.4f * m_fScale * 0.75f);
		blue = 0.384f * 0.5f;// - (0.384f * m_fScale * 0.75f);

		skill->SetColor(red, green, blue, m_fFade);
		skill->SetRotationX( g_PI * 0.5f );
		skill->RotateZ( m_fScale * 35.0f );
		skill->Scale( 1.0f, 1.0f, 1.0f );
		skill->SetTranslationXYZ(vDrawVec.x, vDrawVec.y + 3.0f, vDrawVec.z);
		skill->Render();

		skill->SetColor(red, green, blue, m_fFade);
		skill->SetRotationX( g_PI * 0.5f );
		skill->RotateZ( -m_fScale * 70.0f );
		skill->Scale( 1.0f, 1.0f, 1.0f);
		skill->SetTranslationXYZ(vDrawVec.x, vDrawVec.y + 4.0f, vDrawVec.z);
		skill->Render();

		if( effect_sort.Index >= 60 )
			effect_sort.Index = 0;

		m_fScale = ((float)effect_sort.Index / 60);
		m_fFade = 1.0f - m_fScale;

		// nate 2004 - 7 - image
		GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_OPTIONSKILL_CORONA ] ) );
		corona->SetColor(m_fFade * 0.5f, m_fFade * 0.5f, m_fFade * 0.5f, m_fFade * 0.5f);
		corona->SetRotationX( g_PI * 0.5f );
		corona->RotateZ( m_fScale * 25.0f );
		corona->Scale( RandomNum(1.0f, 1.2f), RandomNum(1.0f, 1.2f), RandomNum(1.0f, 1.2f) );
		corona->SetTranslationXYZ(vDrawVec.x, vDrawVec.y + 3.5f, vDrawVec.z);
		corona->Render();
		break;
	case EFFECT_SKILL_PROTECTION:
	case EFFECT_SKILL_PEACEFUL:
	case EFFECT_PEROM_OPTIONSKILL_S150:
		// nate 2004 - 7 - image
		GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_OPTIONSKILL_PEACE ] ) );
		red = 0.1f * 0.5f;// - (0.1f * m_fScale * 0.3f);
		green = 0.4f * 0.5f;// - (0.4f * m_fScale * 0.3f);
		blue = 0.984f * 0.5f;// - (0.984f * m_fScale * 0.3f);

		skill->SetColor(red, green, blue, m_fFade);
		skill->SetRotationX( g_PI * 0.5f );
		skill->RotateZ( m_fScale * 50.0f );
		skill->Scale( 1.0f, 1.0f, 1.0f );
		skill->SetTranslationXYZ(vDrawVec.x, vDrawVec.y + 3.5f, vDrawVec.z);
		skill->Render();

		skill->SetColor(red, green, blue, m_fFade);
		skill->SetRotationX( g_PI * 0.5f );
		skill->RotateZ( m_fScale * 35.0f );
		skill->Scale( 1.0f, 1.0f, 1.0f);
		skill->SetTranslationXYZ(vDrawVec.x, vDrawVec.y + 3.5f, vDrawVec.z);
		skill->Render();

		if( effect_sort.Index >= 40 )
			effect_sort.Index = 0;

		m_fScale = ((float)effect_sort.Index / 40);
		m_fFade = 1.0f - m_fScale;

		// nate 2004 - 7 - image
		GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_OPTIONSKILL_CORONA ] ) );
		corona->SetColor(0.35f, 0.35f, 0.35f, 0.35f);
		corona->SetRotationX( g_PI * 0.5f );
		corona->RotateZ( m_fScale * 25.0f );
		corona->Scale( 1.0f + (m_fScale*0.5f), 1.0f + (m_fScale*0.5f), 1.0f + (m_fScale*0.5f));
		corona->SetTranslationXYZ(vDrawVec.x, vDrawVec.y + 3.5f + m_fScale, vDrawVec.z);
		corona->Render();
		break;
	case EFFECT_SKILL_SUMMON_PARTITION:
		break;
	default:
		// nate 2004 - 7 - image
		GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_OPTIONSKILL_DEFENSE ] ) );
		red = green = blue = 1.0f;
		skill->SetColor(red, green, blue, m_fFade);
		skill->SetRotationX( g_PI * 0.5f );
		skill->RotateZ( m_fScale * 75.0f );
		skill->Scale( 1.0f, 1.0f, 1.0f );
		skill->SetTranslationXYZ(vDrawVec.x, vDrawVec.y + 3.5f, vDrawVec.z);
		skill->Render();
		break;
	}

	skill->SetColor(red, green, blue, m_fFade);
	skill->SetRotationX( g_PI * 0.5f );
	skill->RotateZ( m_fScale * 75.0f );
	skill->SetTranslationXYZ(vDrawVec.x, vDrawVec.y + 3.5f, vDrawVec.z);

	// nate 2004 - 7 - image
	GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_OPTIONSKILL_CORONA ] ) );
	corona->SetColor(1.0f, 1.0f, 1.0f, 1.0f);
	corona->SetRotationX( g_PI * 0.5f );
	corona->RotateZ( m_fScale * 75.0f );
	corona->Scale( RandomNum(1.0f, 1.5f), RandomNum(1.0f, 1.5f), RandomNum(1.0f, 1.5f) );
	corona->SetTranslationXYZ(vDrawVec.x, vDrawVec.y + 3.5f, vDrawVec.z);

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState[2] );

	return FALSE;
}