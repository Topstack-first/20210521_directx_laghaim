#include "stdafx.h"
#define D3D_OVERLOADS

#include <d3d.h>
#include "FxSet.h"
#include "main.h"
#include "D3DMath.h"
#include "D3DUtil.h"
#include "sfxUtil.h"
#include "FxBalloonKill.h"
#include "CapSyong.h"
#include "sfx.h"
#include "sfxUtil.h"
#include "main.h"
#include "IndexedTexture.h"
#include "NkCharacter.h"

CFxBalloonKill::CFxBalloonKill()
	: m_pLolo(NULL)
{
}

CFxBalloonKill::~CFxBalloonKill()
{
	SAFE_DELETE(m_pLolo)
}

void CFxBalloonKill::LoadRes()
{
	if( m_pLolo == NULL )
	{
		m_pLolo = new CLolos;
		m_pLolo->SetIndexedTexture(g_pCapsyongTexture);
		m_pLolo->Read("data/effect/lolos/mon_13y_ballon-effect.LOL");
	}
}

BOOL CFxBalloonKill::Render(EffectSort& effect_sort)
{
	if( !GET_D3DDEVICE() )
		return TRUE;

	BOOL bFinished = FALSE;
	
	DWORD dwAlphaTest;
	DWORD dwAlphaBlend;
	DWORD dwSrcBlend;
	DWORD dwDestBlend;
	DWORD dwCull;

	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_ALPHATESTENABLE, &dwAlphaTest );
	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, &dwAlphaBlend );
	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_SRCBLEND, &dwSrcBlend);
	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_DESTBLEND, &dwDestBlend);	
	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_CULLMODE, &dwCull);	
	
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_LIGHTING, FALSE);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, TRUE);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHATESTENABLE, TRUE);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_CULLMODE, D3DCULL_NONE);

	D3DMATRIX matWorld;
	D3DMATRIX matRotate;
	D3DMATRIX matView;
	D3DMATRIX matInv;
	
	GET_D3DDEVICE()->GetTransform(D3DTRANSFORMSTATE_VIEW, &matView );
	matView._41 = 0;
	matView._42 = 0;
	matView._43 = 0;
	D3DMath_MatrixInvert(matInv, matView);
	D3DUtil_SetRotateYMatrix(matRotate, 90 * D3DX_PI / 180.0f);
	D3DMath_MatrixMultiply(matInv, matRotate, matInv);
	matInv._41 = effect_sort.vPos.x;
	matInv._42 = effect_sort.vPos.y + 20.0f;
	matInv._43 = effect_sort.vPos.z;
	matWorld = matInv;

	GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_WORLD, &matWorld );
	
	if( m_pLolo )
		m_pLolo->Render(GET_D3DDEVICE(), effect_sort.nCurFrame, FALSE, FALSE);

	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_LIGHTING, TRUE);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, dwAlphaTest);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHATESTENABLE, dwAlphaBlend);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_SRCBLEND, dwSrcBlend);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_DESTBLEND, dwDestBlend);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_CULLMODE, dwCull);

	return bFinished;
}