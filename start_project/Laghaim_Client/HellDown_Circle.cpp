#include "stdafx.h"
#include "main.h"
#include "UIMgr.h"
#include "NkCharacter.h"
#include "NkMob.h"
#include "skill.h"
#include "HellDown_Circle.h"
#include "ImageDefine.h"
#include "UIMgr.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFxHellDown_Circle::CFxHellDown_Circle()
{
	m_pMasic_Circle			= NULL;
	m_fScale				= 0.0f;
	m_fFade					= 0.0f;
	m_nTotalFrame			= FX_HELLDOWN_FRAMECOUNT;

	Create();
}

CFxHellDown_Circle::~CFxHellDown_Circle()
{
	DeleteRes();
}

void CFxHellDown_Circle::Create()
{
	SAFE_DELETE(m_pMasic_Circle);

	m_pMasic_Circle = new CRectPlane;
	if( m_pMasic_Circle )
		m_pMasic_Circle->Create( 17.0f, 17.0f, FALSE );
}

void CFxHellDown_Circle::LoadRes(IndexedTexture *pIndexedTexture)
{

}

void CFxHellDown_Circle::DeleteRes()
{
	SAFE_DELETE(m_pMasic_Circle);
}


void CFxHellDown_Circle::Render(EffectSort &effect_sort)
{
	if( !GET_D3DDEVICE() || !m_pMasic_Circle )
		return;

	if(!effect_sort.pMobTo
			&& !effect_sort.pNkChaTo)
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

	if (effect_sort.pMobTo)
		vDrawVec = D3DVECTOR(effect_sort.pMobTo->m_wx, effect_sort.pMobTo->m_wy, effect_sort.pMobTo->m_wz);
	else if (effect_sort.pNkChaTo)
		vDrawVec = D3DVECTOR(effect_sort.pNkChaTo->m_wx, effect_sort.pNkChaTo->m_wy, effect_sort.pNkChaTo->m_wz);
	else
		return;

	GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_MASICCIRCLE ] ) );

	m_pMasic_Circle->SetColor(m_fFade * 0.7f, m_fFade * 0.7f, m_fFade * 0.7f, m_fFade * 0.7f);
	m_pMasic_Circle->SetRotationX( g_PI * 0.5f );
	m_pMasic_Circle->RotateZ( m_fScale );
	m_pMasic_Circle->Scale( m_fScale, m_fScale, m_fScale );
	m_pMasic_Circle->SetTranslationXYZ(vDrawVec.x, vDrawVec.y + 45.0f - 3.0f * m_fScale, vDrawVec.z);
	if(pCMyApp->m_bHellDownSec)
	{
		m_pMasic_Circle->Scale( m_fScale * 4.0f, m_fScale * 4.0f, m_fScale * 4.0f);
		m_pMasic_Circle->SetTranslationXYZ(vDrawVec.x, vDrawVec.y + 36.0f, vDrawVec.z);
	}
	m_pMasic_Circle->Render();

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState[2] );
}
