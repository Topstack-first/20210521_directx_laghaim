#include "stdafx.h"
#include "sfx.h"
#include "main.h"
#include "NkCharacter.h"
#include "Land.h"
#include "ImageDefine.h"



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFxEatEffects::CFxEatEffects()
{
	eat_smile				= NULL;
	m_fScale				= 0.0f;
	m_fFade					= 0.0f;
	m_nTotalFrame			= FX_EATEFFECTS_FRAMECOUNT;

	Create();
}

CFxEatEffects::~CFxEatEffects()
{
	SAFE_DELETE(eat_smile);
}

void CFxEatEffects::Create()
{
	SAFE_DELETE(eat_smile);

	eat_smile = new CRectPlane;
	if( eat_smile )
		eat_smile->Create( 4.0f, 4.0f, FALSE );
}

void CFxEatEffects::LoadRes(IndexedTexture *pIndexedTexture)
{}
void CFxEatEffects::DeleteRes()
{}

BOOL CFxEatEffects::Render(EffectSort &effect_sort)
{
	if( !GET_D3DDEVICE() || !eat_smile )
		return true;

	if( !effect_sort.pNkChaTo )
		return TRUE;

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
	// Set any appropiate state
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND,   D3DBLEND_SRCALPHA );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND,  D3DBLEND_INVSRCALPHA );

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
		vDrawVec = D3DVECTOR(effect_sort.pNkChaTo->m_wx, pCMyApp->GetLandPtr()->GetHFHeight(effect_sort.pNkChaTo->m_wx, effect_sort.pNkChaTo->m_wz), effect_sort.pNkChaTo->m_wz);

	float Add_Height[NUM_RACE][NUM_SEX] = {	29.0f, 26.5f,
											27.5f, 26.5f,
											22.0f, 22.0f,
											27.5f, 26.5f,
											32.0f, 29.0f,
											32.0f, 29.0f
										  };

	if( eat_smile )
	{
		// nate 2004 - 7 - image
		if( g_pCapsyongTexture )
			GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_EAT_LIGHT ] ) );

		eat_smile->SetColor(1.0f, 1.0f, 1.0f, m_fFade);
		eat_smile->SetBillBoard( vViewVec, FALSE, 0.0f );
		eat_smile->Scale( 2.0f, 2.0f, 2.0f );
		eat_smile->SetTranslationXYZ(vDrawVec.x, vDrawVec.y + Add_Height[effect_sort.pNkChaTo->m_Race][effect_sort.pNkChaTo->m_Sex], vDrawVec.z);
		eat_smile->Render();

		// nate 2004 - 7 - image
		if( g_pCapsyongTexture )
			GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_EAT_SMILE ] ) );

		eat_smile->SetColor(1.0f, 1.0f, 1.0f, m_fFade);
		eat_smile->SetBillBoard( vViewVec, FALSE, 0.0f );
		eat_smile->SetTranslationXYZ(vDrawVec.x, vDrawVec.y + Add_Height[effect_sort.pNkChaTo->m_Race][effect_sort.pNkChaTo->m_Sex], vDrawVec.z);
		eat_smile->Render();
	}

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState[2] );

	return FALSE;
}

