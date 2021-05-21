#include "stdafx.h"
#define D3D_OVERLOADS

#include <d3d.h>
#include "FxSet.h"
#include "main.h"
#include "D3DMath.h"
#include "D3DUtil.h"
#include "sfxUtil.h"
#include "CapSyong.h"
#include "sfx.h"
#include "sfxUtil.h"
#include "main.h"
#include "IndexedTexture.h"
#include "NkCharacter.h"
#include "NkMob.h"
#include "FxCommonLolo.h"

CFxCommonLolo::CFxCommonLolo()
	: m_pLolo(NULL)
{
}

CFxCommonLolo::~CFxCommonLolo()
{
	SAFE_DELETE(m_pLolo)
}

void CFxCommonLolo::SetFilename(const std::string& filename)
{
	m_filename = "data/effect/lolos/";
	m_filename += filename;
}

void CFxCommonLolo::LoadRes()
{
	if( m_pLolo == NULL )
	{
		m_pLolo = new CLolos;
		m_pLolo->SetIndexedTexture(g_pCapsyongTexture);
		m_pLolo->Read((char*)m_filename.c_str());
	}
}

BOOL CFxCommonLolo::Render(EffectSort& effect_sort)
{
	if( !GET_D3DDEVICE() )
		return TRUE;

	if( effect_sort.pMobFrom == NULL )
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
	D3DMATRIX matTrans;
	D3DMATRIX matPos;
	D3DMATRIX matRotate;
	D3DMATRIX matScale;

	CNkMob* pMob = effect_sort.pMobFrom;
	float fScale = effect_sort.vSortPos.x;

	D3DUtil_SetIdentityMatrix(matWorld);
	D3DUtil_SetTranslateMatrix(matPos, effect_sort.vPos.x, effect_sort.vPos.y, effect_sort.vPos.z);
	D3DUtil_SetTranslateMatrix(matTrans, pMob->m_wx, pMob->m_wy, pMob->m_wz);
	Nk3DUtil_SetRotateCCWYMatrix(matRotate, pMob->m_dir);
	D3DUtil_SetScaleMatrix(matScale, fScale, fScale, fScale);
	D3DMath_MatrixMultiply(matWorld, matWorld, matScale);
	D3DMath_MatrixMultiply(matWorld, matWorld, matPos);
	D3DMath_MatrixMultiply(matWorld, matWorld, matRotate);
	D3DMath_MatrixMultiply(matWorld, matWorld, matTrans);
	
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