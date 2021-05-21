#include "stdafx.h"


#include "main.h"
#include "FxSet.h"
#include "NkCharacter.h"
#include "NkMob.h"

#include "LHFX_Perom_Strike.h"

const int male_frame = 43;
const int fmale_frame = 34;

LHFX_Perom_Strike::LHFX_Perom_Strike(void)
	:	m_lolo(NULL)
{
}

LHFX_Perom_Strike::~LHFX_Perom_Strike(void)
{
	SAFE_DELETE_ARRAY(m_lolo);
}

void LHFX_Perom_Strike::LoadRes()
{
	if (!m_lolo)
	{
		m_lolo = new CLolos[2];
		m_lolo[0].SetIndexedTexture( g_pCapsyongTexture );
		m_lolo[0].Read("data/effect/lolos/pm_skill_wheelwind1.lol");
		m_lolo[1].SetIndexedTexture( g_pCapsyongTexture );
		m_lolo[1].Read("data/effect/lolos/pw_skill_wheelwind1.lol");
	}
}


void LHFX_Perom_Strike::DeleteRes()
{
}

bool LHFX_Perom_Strike::Render(EffectSort &effect_sort)
{
	if( !GET_D3DDEVICE() || !m_lolo )
		return false;

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

	D3DMATRIX matWorld, matView, matRot;
	DWORD dwLighting, dwZWrite, dwSrc, dwDest;

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
		if( effect_sort.pNkChaFrom->m_Sex == 0 )
			m_lolo[0].Render(GET_D3DDEVICE(), effect_sort.nCurFrame, FALSE, FALSE);
		else if( effect_sort.pNkChaFrom->m_Sex == 1 )
			m_lolo[1].Render(GET_D3DDEVICE(), effect_sort.nCurFrame, FALSE, FALSE);
	}

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, dwLighting );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ZWRITEENABLE, dwZWrite );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND, dwSrc );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND, dwDest );

	return false;
}