#include "stdafx.h"
// FxFlyingCrash2.cpp: implementation of the CFxFlyingCrash2 class.
//
//////////////////////////////////////////////////////////////////////

#include <assert.h>
#include "sfx.h"
#include "main.h"
#include "ImageDefine.h"
#include "FxFlyingCrash2.h"
#include "NkCharacter.h"
#include "NkMob.h"
#include "IndexedTexture.h"
#include "ShadowRect.h"



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CFxFlyingCrash2::CFxFlyingCrash2()
{
	m_pLolo = NULL;
	eng_sphere = NULL;
	eng_sphere = new CSphere;
	if( eng_sphere )
		eng_sphere->Create();
}

CFxFlyingCrash2::~CFxFlyingCrash2()
{
	DeleteRes();
}

void CFxFlyingCrash2::LoadRes()
{
	if(!m_pLolo)
	{
		m_pLolo = new CLolos;
		m_pLolo->SetIndexedTexture( g_pCapsyongTexture );
		m_pLolo->Read("data/effect/lolos/flyingattack001.LOL");
	}



}

void CFxFlyingCrash2::DeleteRes()
{
	SAFE_DELETE(m_pLolo);
	SAFE_DELETE(eng_sphere);
}

void CFxFlyingCrash2::Render(EffectSort &effect_sort)
{
	if( !GET_D3DDEVICE() || !m_pLolo )
		return;

	LPDIRECTDRAWSURFACE7 lpSurface = NULL;
	D3DMATRIX matTrans, matView,matRotate, matWorld,matWorld2,matTemp;
	DWORD dwLighting, dwZWrite, dwSrc, dwDest;
	D3DMATRIX matRx, matRy;

	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_LIGHTING, &dwLighting );
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_ZWRITEENABLE, &dwZWrite );
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_SRCBLEND, &dwSrc );
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_DESTBLEND, &dwDest );

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ZWRITEENABLE, TRUE );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND, D3DBLEND_ONE );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND,   D3DBLEND_SRCALPHA );

	D3DVECTOR vDir, vPosTo;

	// 타겟 좌표를 구한다.
	vPosTo = effect_sort.vTargetPos;

	if( effect_sort.nCurFrame < 8 )
	{
		if (m_pLolo)
		{
			D3DUtil_SetIdentityMatrix( matTrans );
			D3DUtil_SetTranslateMatrix( matTrans, vPosTo.x, vPosTo.y, vPosTo.z);

			//스케일 조절
			D3DMATRIX		matS;
			D3DUtil_SetScaleMatrix(matS, 4.0f, 0.3f, 4.0f);
			D3DMath_MatrixMultiply(matTrans, matS, matTrans);
			GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_WORLD, &matTrans );

			m_pLolo->Render(GET_D3DDEVICE(), effect_sort.nCurFrame, FALSE, FALSE);
		}
	}
	if( effect_sort.nCurFrame >= 6
			&& effect_sort.nCurFrame < 10 )
	{
		if( g_pShadowRect )
		{
			lpSurface = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_EARTHQUAKE_BASE ] ) ;
			g_pShadowRect->SetTileCoord(vPosTo, 70, FALSE);
			g_pShadowRect->Render(lpSurface, FALSE);
		}

		if( eng_sphere )
		{
			GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_EARTHQUAKE_ENG ] ) );
			float smoke_fade = 1.0f - ( (effect_sort.nCurFrame-20.0f) / 10.0f);
			float scale =  (effect_sort.nCurFrame + 3.0f) * 7.0f;
			eng_sphere->SetColor(1.0f, 1.0f, 1.0f, smoke_fade );
			eng_sphere->SetScaling( scale * 0.6f, scale * 0.6f, scale * 0.6f );
			eng_sphere->SetTranslationXYZ(vPosTo.x, vPosTo.y, vPosTo.z);
			eng_sphere->Render();
		}
	}

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, dwLighting );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ZWRITEENABLE, dwZWrite );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND, dwSrc );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND, dwDest );
}