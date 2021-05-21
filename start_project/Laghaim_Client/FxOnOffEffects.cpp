#include "stdafx.h"
#include "sfx.h"
#include "main.h"
#include "NkCharacter.h"
#include "Land.h"
#include "ImageDefine.h"



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFxOnOffEffects::CFxOnOffEffects()
{
	effects					= NULL;
	m_fScale				= 0.0f;
	m_fFade					= 0.0f;
	m_nTotalFrame			= FX_ONOFFEFFECTS_FRAMECOUNT;

	Create();
}

CFxOnOffEffects::~CFxOnOffEffects()
{
	SAFE_DELETE(effects);
}

void CFxOnOffEffects::Create()
{
	SAFE_DELETE(effects);

	effects = new CRectPlane;
	if( effects )
		effects->Create( 4.0f, 4.0f, FALSE );
}

void CFxOnOffEffects::LoadRes(IndexedTexture *pIndexedTexture)
{}
void CFxOnOffEffects::DeleteRes()
{}

BOOL CFxOnOffEffects::Render(EffectSort &effect_sort)
{
	if( !GET_D3DDEVICE() || !effects )
		return true;

	if( !effect_sort.pNkChaFrom )
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
	if( effect_sort.pNkChaFrom )
		vDrawVec = D3DVECTOR(effect_sort.pNkChaFrom->m_wx, effect_sort.pNkChaFrom->m_wy, effect_sort.pNkChaFrom->m_wz);

	float Add_Height[NUM_RACE][NUM_SEX] = {	  29.0f, 26.5f
											  , 27.5f, 26.5f
											  , 22.0f, 22.0f
											  , 27.5f, 26.5f
											  , 29.5f, 27.0f
											  , 29.5f, 27.0f
										  };


	// nate 2004 - 7 - image
	GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_ONOFF_LIGHT ] ) );

	effects->SetColor(1.0f, 1.0f, 1.0f, m_fFade);
	effects->SetBillBoard( vViewVec, FALSE, 0.0f );
	effects->Scale( 2.0f, 2.0f, 2.0f );
	effects->SetTranslationXYZ(vDrawVec.x, vDrawVec.y + Add_Height[effect_sort.pNkChaFrom->m_Race][effect_sort.pNkChaFrom->m_Sex] + m_fScale, vDrawVec.z);
	effects->Render();

	if( effect_sort.Index == 1 )
		// nate 2004 - 7 - image
		GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_ONOFF_ON ] ) );
	else
		// nate 2004 - 7 - image
		GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_ONOFF_OFF ] ) );

	effects->SetColor(1.0f, 1.0f, 1.0f, m_fFade);
	effects->SetBillBoard( vViewVec, FALSE, 0.0f );
	effects->SetTranslationXYZ(vDrawVec.x, vDrawVec.y + Add_Height[effect_sort.pNkChaFrom->m_Race][effect_sort.pNkChaFrom->m_Sex] + m_fScale, vDrawVec.z);
	effects->Render();

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState[2] );

	return FALSE;
}


