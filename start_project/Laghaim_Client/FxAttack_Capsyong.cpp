#include "stdafx.h"
#include <assert.h>
#include "sfx.h"
#include "NkCharacter.h"
#include "NkMob.h"
#include "ImageDefine.h"
#include "FxAttack_Capsyong.h"
#include "main.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFxAttack_Capsyong::CFxAttack_Capsyong()
{
	m_pCapsyong = NULL;
	m_pBpd = NULL;
}

CFxAttack_Capsyong::~CFxAttack_Capsyong()
{
	DeleteRes();
}

void CFxAttack_Capsyong::LoadRes()
{
	DeleteRes();

	m_pCapsyong = new CCapsyong(g_pCapsyongTexture);
	if( VF_ERR_NONE > m_pCapsyong->Read("data/effect/csy/Spore.CSY") )
	{
		SAFE_DELETE(m_pCapsyong);
		return;
	}

	m_pBpd = new CBiped;
	if( VF_ERR_NONE > m_pBpd->Read("data/effect/bpd/Spore.BPD") )
	{
		SAFE_DELETE(m_pBpd);
		return;
	}

}

void CFxAttack_Capsyong::DeleteRes()
{
	SAFE_DELETE(m_pCapsyong);
	SAFE_DELETE(m_pBpd);
}

void CFxAttack_Capsyong::Render(EffectSort &effect_sort)
{
	if( !GET_D3DDEVICE() )
		return;

	D3DMATRIX matWorld, matTrans;
	DWORD dwLighting, dwZWrite;

	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_LIGHTING, &dwLighting);
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_ZWRITEENABLE, &dwZWrite);

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ZWRITEENABLE, TRUE);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE);

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND,   D3DBLEND_SRCALPHA );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND,  D3DBLEND_INVSRCALPHA );

	if(m_pCapsyong && m_pBpd)
	{
		D3DUtil_SetIdentityMatrix( matTrans );
		D3DUtil_SetTranslateMatrix( matTrans, effect_sort.vPos.x, effect_sort.vPos.y, effect_sort.vPos.z);
		GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_WORLD, &matTrans );
		m_pCapsyong->Animate(m_pBpd, effect_sort.nCurFrame, TRUE );
		m_pCapsyong->Render(GET_D3DDEVICE(), -1, FALSE, FALSE, NULL, effect_sort.nCurFrame);
	}

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, dwLighting);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ZWRITEENABLE, dwZWrite);
}