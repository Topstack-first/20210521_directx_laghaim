#include "stdafx.h"
#include "main.h"
#include "UIMgr.h"
#include "NkCharacter.h"
#include "NkMob.h"
#include "skill.h"
#include "FxHellDown_Circle2.h"
#include "ImageDefine.h"
#include "UIMgr.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFxHellDown_Circle2::CFxHellDown_Circle2()
{
	m_pMasic_Circle			= NULL;
	m_fScale				= 0.0f;
	m_fFade					= 0.0f;
	m_nTotalFrame			= FX_HELLDOWN_FRAMECOUNT;

	Create();
}

CFxHellDown_Circle2::~CFxHellDown_Circle2()
{
	DeleteRes();
}

void CFxHellDown_Circle2::Create()
{
	DeleteRes();

	m_pMasic_Circle = new CRectPlane;
	if( m_pMasic_Circle )
		m_pMasic_Circle->Create( 17.0f, 17.0f, FALSE );
}

void CFxHellDown_Circle2::LoadRes(IndexedTexture *pIndexedTexture)
{

}
void CFxHellDown_Circle2::DeleteRes()
{
	SAFE_DELETE(m_pMasic_Circle);
}


void CFxHellDown_Circle2::Render(EffectSort &effect_sort)
{
	if( !GET_D3DDEVICE() || !effect_sort.pNkChaFrom)
		return;

	m_fScale = (g_2_PI * effect_sort.nCurFrame) / m_nTotalFrame;
	m_fFade = 1.0f - (m_fScale / g_2_PI );

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

	if (effect_sort.pNkChaFrom)
		vDrawVec = D3DVECTOR(effect_sort.pNkChaFrom->m_wx, effect_sort.pNkChaFrom->m_wy, effect_sort.pNkChaFrom->m_wz);

	// nate 2004 - 7 - image
	if( g_pCapsyongTexture )
		GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_MASICCIRCLE2 ] ) );

	if( m_pMasic_Circle )
	{
		m_pMasic_Circle->SetColor(m_fFade * 0.5f, m_fFade * 0.5f, m_fFade * 0.5f, m_fFade * 0.5f);
		m_pMasic_Circle->SetRotationX( g_PI * 0.5f );
		m_pMasic_Circle->RotateZ( m_fScale * 1.0f );
		m_pMasic_Circle->Scale( 1.0f + m_fScale, 1.0f + m_fScale, 1.0f + m_fScale );
		if(pCMyApp->m_bHellDownSec)
		{
			m_pMasic_Circle->Scale( m_fScale * 3.0f, m_fScale * 3.0f, m_fScale * 3.0f);
			pCMyApp->m_bHellDownSec = FALSE;
		}
		m_pMasic_Circle->SetTranslationXYZ(vDrawVec.x, vDrawVec.y + 3.0f, vDrawVec.z);
		m_pMasic_Circle->Render();
	}

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState[2] );
}
