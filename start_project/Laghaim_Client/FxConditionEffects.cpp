#include "stdafx.h"
#include "sfx.h"
#include "main.h"
#include "NkCharacter.h"
#include "NkMob.h"
#include "Land.h"
#include "skill.h"
#include "ImageDefine.h"



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFxConditionEffects::CFxConditionEffects()
{
	condition				= NULL;
	corona					= NULL;
	m_fScale				= 0.0f;
	m_fFade					= 0.0f;
	m_nTotalFrame			= FX_CONDITIONEFFECTS_FRAMECOUNT;

	m_pSpiderWire			= 0;

	Create();
}

CFxConditionEffects::~CFxConditionEffects()
{
	SAFE_DELETE(m_pSpiderWire);
	SAFE_DELETE(condition);
	SAFE_DELETE(corona);
}

void CFxConditionEffects::Create()
{
	SAFE_DELETE(condition);
	SAFE_DELETE(corona);

	condition = new CRectPlane;
	if( condition )
		condition->Create( 4.0f, 4.0f, FALSE );

	corona = new CRectPlane;
	if( corona )
		corona->Create( 8.0f, 8.0f, FALSE );

	if( !m_pSpiderWire )
	{
		m_pSpiderWire = new CLolos;
		m_pSpiderWire->SetIndexedTexture( g_pCapsyongTexture );
		m_pSpiderWire->Read("data/effect/lolos/spider03.LOL");
	}

}

void CFxConditionEffects::LoadRes(IndexedTexture *pIndexedTexture)
{}
void CFxConditionEffects::DeleteRes()
{}

BOOL CFxConditionEffects::Render(EffectSort &effect_sort)
{
	if( !GET_D3DDEVICE() || !condition || !corona )
		return true;

	if(!effect_sort.pNkChaTo
			&& !effect_sort.pMobTo)
		return TRUE;

	if( effect_sort.pNkChaTo )
	{
		if( effect_sort.pNkChaTo->m_CancelState == 0 )
		{
			effect_sort.pNkChaTo->m_ShowConditionEffect = FALSE;
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
	}
	else if( effect_sort.pMobTo )
	{
		if( effect_sort.pMobTo->m_CancelState == 0 )
		{
			effect_sort.pMobTo->m_ShowConditionEffect = FALSE;
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
	}

	D3DVECTOR	vViewVec;
	vViewVec = pCMyApp->GetEyeDirection();

	float effects_Height = 0.0f;
	int statConditionType = 0;

	D3DVECTOR	vDrawVec;

	if( effect_sort.pNkChaTo )
	{
		vDrawVec = D3DVECTOR(effect_sort.pNkChaTo->m_wx, effect_sort.pNkChaTo->m_wy, effect_sort.pNkChaTo->m_wz);

		//===========================================
		// nate 2005 - 01
		// new race
		// 0 불칸, 1 카이립톤, 2 에이디아, 3 휴먼, 4 프리크
		// [pCha->m_Sex], [pCha->m_Race]
		// 마비나 석화등의 머리위에 뜨는 빌보드 셋팅
		//===========================================
		float Add_Height[NUM_RACE][NUM_SEX] = {	29.0f, 26.5f,
												27.5f, 26.5f,
												22.0f, 22.0f,
												27.5f, 26.5f,
												30.0f, 27.5f,
												30.0f, 27.5f
											  };



		effects_Height = Add_Height[effect_sort.pNkChaTo->m_Race][effect_sort.pNkChaTo->m_Sex];

		statConditionType = effect_sort.pNkChaTo->m_ChaCondition;
	}
	else if( effect_sort.pMobTo )
	{
		vDrawVec = D3DVECTOR(effect_sort.pMobTo->m_wx, effect_sort.pMobTo->m_wy, effect_sort.pMobTo->m_wz);

		switch (effect_sort.pMobTo->m_Height)
		{
		case MOB_HEIGHT_HIGH_NUM:
			effects_Height = MOB_HEIGHT_HIGH_VAL + 12.0f;
			break;
		case MOB_HEIGHT_LOW_NUM:
			effects_Height = MOB_HEIGHT_LOW_VAL + 12.0f;
			break;
		default: // MOB_HEIGHT_MIDDLE_NUM:
			effects_Height = MOB_HEIGHT_MIDDLE_VAL + 12.0f;
			break;
		}

		statConditionType = effect_sort.pMobTo->m_ChaCondition;
	}

	if( effect_sort.Index >= 30 )
		effect_sort.Index = 0;

	m_fScale = ((float)effect_sort.Index / 30);
	m_fFade = 1.0f - m_fScale;

	DWORD  dwTextureState[3];
	DWORD srcBlendOld, destBlendOld;
	DWORD alphaBlend, dwLighting, /*dwAmbient,*/ dwFogEnable;
	switch( statConditionType )
	{
	case CONDITION_STONY:
	case CONDITION_PARALYSIS:
	case CONDITION_CHAOS:
	case CONDITION_FREEZE:
	case CONDITION_SHOCK:
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE);
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND,   D3DBLEND_ONE );
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND,  D3DBLEND_ONE );

		GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG1, &dwTextureState[0] );
		GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG2, &dwTextureState[1] );
		GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAOP,   &dwTextureState[2] );

		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );
		break;
	case CONDITION_ENTANGLE:

		GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_FOGENABLE, &dwFogEnable );
		GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, &alphaBlend );
		GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_LIGHTING, &dwLighting );
		//GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_AMBIENT, &dwAmbient);

		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_FOGENABLE, FALSE );
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, TRUE);
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE );
		//GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_AMBIENT, 0xffffff );

		GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_SRCBLEND, &srcBlendOld );
		GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_DESTBLEND, &destBlendOld );

		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA );
		//GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND, D3DBLEND_ONE );
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND, D3DBLEND_ONE );

		GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG1, &dwTextureState[0] );
		GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG2, &dwTextureState[1] );
		GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAOP,   &dwTextureState[2] );

		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );
		break;
	}

	float red, green, blue;

	switch( statConditionType )
	{
	case CONDITION_STONY:
		// nate 2004 - 7 - image
		GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_CONDITION_STONE ] ) );
		red = 0.4f;// - (0.4f * m_fScale * 0.25f);
		green = 0.4f;// - (0.4f * m_fScale * 0.25f);
		blue = 0.984f;// - (0.984f * m_fScale * 0.25f);
		break;
	case CONDITION_PARALYSIS:
		// nate 2004 - 7 - image
		GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_CONDITION_MAVI ] ) );
		red = 0.4f;// - (0.4f * m_fScale * 0.25f);
		green = 0.9f;// - (0.9f * m_fScale * 0.25f);
		blue = 0.384f;// - (0.384f * m_fScale * 0.25f);
		break;
	case CONDITION_CHAOS:
		// nate 2004 - 7 - image
		GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_CONDITION_CHAOS ] ) );
		red = 1.0f;// - (1.0f * m_fScale * 0.25f);
		green = 0.4f;// - (0.4f * m_fScale * 0.25f);
		blue = 0.4f;// - (0.4f * m_fScale * 0.25f);
		break;
	case CONDITION_FREEZE:
		// nate 2004 - 7 - image
		GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_CONDITION_COLD ] ) );
		red = 0.4f;// - (0.4f * m_fScale * 0.25f);
		green = 0.4f;// - (0.4f * m_fScale * 0.25f);
		blue = 0.984f;// - (0.984f * m_fScale * 0.25f);
		break;
	default:
		// nate 2004 - 7 - image
		GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_CONDITION_STONE ] ) );
		red = green = blue = 1.0f;
		break;
	}

	switch(statConditionType)
	{
	case CONDITION_STONY:
	case CONDITION_PARALYSIS:
	case CONDITION_CHAOS:
	case CONDITION_FREEZE:
	case CONDITION_SHOCK:
		condition->SetColor(red, green, blue, m_fFade);
		condition->SetBillBoard( vViewVec, FALSE, 0.0f );
		condition->SetTranslationXYZ(vDrawVec.x, vDrawVec.y + effects_Height, vDrawVec.z);
		condition->Render();

		// nate 2004 - 7 - image
		GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_CONDITION_CORONA ] ) );

		if( m_fScale >= 0.7f )
		{
			corona->SetColor(red, green, blue, m_fFade);
			corona->SetBillBoard( vViewVec, FALSE, 0.0f );
			corona->Scale( m_fScale, m_fScale, m_fScale );
			corona->SetTranslationXYZ(vDrawVec.x, vDrawVec.y + effects_Height, vDrawVec.z);
			corona->Render();
		}
		break;
	case CONDITION_ENTANGLE:
		if (m_pSpiderWire)
		{
			int a = 0;
			DWORD dwTime = timeGetTime() - effect_sort.dwColor;
			float fTime = (float)dwTime / 1000.f;

			if (fTime < 1.0f )
			{
				a = 0xaf * fTime;
			}
			else a = 0xa0;
			int w = (a<<24) + (a<<16) + (a<<8) + a;
			D3DMATRIX matTrans, matView;
			D3DUtil_SetIdentityMatrix( matTrans );
			D3DUtil_SetTranslateMatrix( matTrans, vDrawVec.x, vDrawVec.y, vDrawVec.z);
			pCMyApp->SetViewMatrix(matView);
			GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_WORLD,	&matTrans );
			GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_VIEW,	&matView);
			m_pSpiderWire->RenderRaw(GET_D3DDEVICE(), effect_sort.nCurFrame, FALSE, FALSE, FALSE, &matTrans, &matView, FALSE, FALSE, w);
		}
	}

	switch( statConditionType )
	{
	case CONDITION_STONY:
	case CONDITION_PARALYSIS:
	case CONDITION_CHAOS:
	case CONDITION_FREEZE:
	case CONDITION_SHOCK:
		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState[2] );
		break;
	case CONDITION_ENTANGLE:
		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState[2] );

		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_FOGENABLE, dwFogEnable );
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, alphaBlend );
		//GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_AMBIENT, dwAmbient);
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, dwLighting );

		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND, srcBlendOld);
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND, destBlendOld);
		break;
	}

	return FALSE;
}

CFxConditionEffects2::CFxConditionEffects2()
	: m_fScale(0)
	, m_fFade(0)
	, m_nTotalFrame(0)
{
	m_pSpiderWire = NULL;

	Create();
}

CFxConditionEffects2::~CFxConditionEffects2()
{
	SAFE_DELETE(m_pSpiderWire);
}

void CFxConditionEffects2::Create()
{
	if( !m_pSpiderWire )
	{
		m_pSpiderWire = new CLolos;
		m_pSpiderWire->SetIndexedTexture( g_pCapsyongTexture );
		m_pSpiderWire->Read("data/effect/lolos/spider03.LOL");
	}



}

void CFxConditionEffects2::LoadRes(IndexedTexture *pIndexedTexture)
{
}
void CFxConditionEffects2::DeleteRes()
{
}

BOOL CFxConditionEffects2::Render(EffectSort &effect_sort)
{

	if( !GET_D3DDEVICE() )
		return true;

	if(!effect_sort.pNkChaTo && !effect_sort.pMobTo)
		return TRUE;

	if( effect_sort.pNkChaTo )
	{
		if( effect_sort.pNkChaTo->m_CancelState2 == 0 )
		{
			effect_sort.pNkChaTo->m_ShowConditionEffect2 = FALSE;
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
	}
	else if( effect_sort.pMobTo )
	{
		if( effect_sort.pMobTo->m_CancelState2 == 0 )
		{
			effect_sort.pMobTo->m_ShowConditionEffect2 = FALSE;
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
	}

	int statConditionType = 0;

	D3DVECTOR vDrawVec;
	if( effect_sort.pNkChaTo )
	{
		vDrawVec = D3DVECTOR(effect_sort.pNkChaTo->m_wx, effect_sort.pNkChaTo->m_wy, effect_sort.pNkChaTo->m_wz);
		statConditionType = effect_sort.pNkChaTo->m_ChaCondition2;
	}
	else if( effect_sort.pMobTo )
	{
		vDrawVec = D3DVECTOR(effect_sort.pMobTo->m_wx, effect_sort.pMobTo->m_wy, effect_sort.pMobTo->m_wz);
		statConditionType = effect_sort.pMobTo->m_ChaCondition2;
	}

	m_fScale = ((float)effect_sort.Index / 30);
	m_fFade = 1.0f - m_fScale;

	DWORD  dwTextureState[3];
	DWORD srcBlendOld, destBlendOld;
	DWORD alphaBlend, dwLighting, /*dwAmbient,*/ dwFogEnable;
	switch( statConditionType )
	{
	case CONDITION_ENTANGLE:
		GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_FOGENABLE, &dwFogEnable );
		GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, &alphaBlend );
		GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_LIGHTING, &dwLighting );

		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_FOGENABLE, FALSE );
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, TRUE);
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE );

		GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_SRCBLEND, &srcBlendOld );
		GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_DESTBLEND, &destBlendOld );

		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA );
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND, D3DBLEND_ONE );

		GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG1, &dwTextureState[0] );
		GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG2, &dwTextureState[1] );
		GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAOP,   &dwTextureState[2] );

		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );
		break;
	}

	switch(statConditionType)
	{
	case CONDITION_ENTANGLE:
		if (m_pSpiderWire)
		{
			int a = 0;
			DWORD dwTime = timeGetTime() - effect_sort.dwColor;
			float fTime = (float)dwTime / 1000.f;

			if (fTime < 1.0f )
			{
				a = 0xaf * ( 1.0f-fTime );
				int w = (a<<24) + (a<<16) + (a<<8) + a;
				D3DMATRIX matTrans, matView;
				D3DUtil_SetIdentityMatrix( matTrans );
				D3DUtil_SetTranslateMatrix( matTrans, vDrawVec.x, vDrawVec.y, vDrawVec.z);
				pCMyApp->SetViewMatrix(matView);
				GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_WORLD,	&matTrans );
				GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_VIEW,	&matView);
				m_pSpiderWire->RenderRaw(GET_D3DDEVICE(), effect_sort.nCurFrame, FALSE, FALSE, FALSE, &matTrans, &matView, FALSE, FALSE, w);
			}
			else
			{
				if (effect_sort.pMobTo)
				{
					effect_sort.pMobTo->SetChaCondition2(CONDITION_NORMAL, FALSE);
				}

				if (effect_sort.pNkChaTo)
				{
					effect_sort.pNkChaTo->SetChaCondition2(CONDITION_NORMAL, FALSE);
				}
			}
		}
	}

	switch( statConditionType )
	{
	case CONDITION_ENTANGLE:
		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState[2] );

		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_FOGENABLE, dwFogEnable );
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, alphaBlend );
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, dwLighting );

		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND, srcBlendOld);
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND, destBlendOld);
		break;
	}

	return FALSE;
}
