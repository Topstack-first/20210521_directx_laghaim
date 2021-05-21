#include "stdafx.h"
#include "main.h"
#include "ImageDefine.h"
#include "FxHellDown_Around.h"
#include "NkCharacter.h"
#include "UIMgr.h"



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFxHellDown_Around::CFxHellDown_Around()
{
	m_pAround_Ring		= NULL;
	m_fScale			= 0.0f;
	m_nTotalFrame		= FX_HELLDOWN_FRAMECOUNT;

	Create();
}

CFxHellDown_Around::~CFxHellDown_Around()
{
	DeleteRes();
}

void CFxHellDown_Around::Create()
{
	DeleteRes();

	m_pAround_Ring = new CCylinderPlane;
	if( m_pAround_Ring )
		m_pAround_Ring->Create( 12.0f, 12.0f, 3.0f, 20, 0.0f, TRUE, 1.0f );
}

void CFxHellDown_Around::LoadRes(IndexedTexture *pIndexedTexture)
{}

void CFxHellDown_Around::DeleteRes()
{
	SAFE_DELETE(m_pAround_Ring);
}

void CFxHellDown_Around::Render(EffectSort &effect_sort)
{
	if( !GET_D3DDEVICE() )
		return;

	if(!effect_sort.pNkChaFrom)
		return;

	m_fScale = (g_2_PI * effect_sort.nCurFrame) / m_nTotalFrame;

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

	D3DVECTOR	vDrawVec;

	if (effect_sort.pNkChaFrom)
		vDrawVec = D3DVECTOR(effect_sort.pNkChaFrom->m_wx, effect_sort.pNkChaFrom->m_wy, effect_sort.pNkChaFrom->m_wz);

	if( g_pCapsyongTexture )
		GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_AROUND ] ) );

	if( m_pAround_Ring )
	{
		m_pAround_Ring->SetColor(1.0f, 1.0f, 1.0f, 1.0f);
		m_pAround_Ring->SetRotationY( g_PI );
		m_pAround_Ring->RotateY( -m_fScale );
		m_pAround_Ring->Scale( 1.0f, 3.5f, 1.0f);
		m_pAround_Ring->SetTranslationXYZ(vDrawVec.x, vDrawVec.y + 10.0f, vDrawVec.z);
		m_pAround_Ring->Render();
	}

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState[2] );

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE);
}
