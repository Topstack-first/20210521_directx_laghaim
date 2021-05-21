#include "stdafx.h"
#include "main.h"
#include "NkCharacter.h"
#include "NkMob.h"
#include "ShadowRect.h"
#include "FxHellDown_Lightning.h"
#include "ImageDefine.h"


extern BOOL	g_bDungeon;


#define FX_LIGHTNING_LWIDTH 20.0f
#define FX_LIGHTNING_LHEIGHT 60.0f

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFxHellDown_Lightning::CFxHellDown_Lightning()
{
	m_nTotalFrame = 65;

	m_lverRect[0] = D3DLVERTEX( D3DVECTOR(-FX_LIGHTNING_LWIDTH, 0, 0), 0xffffffff, 0, 0.0f, 1.0f );
	m_lverRect[1] = D3DLVERTEX( D3DVECTOR(-FX_LIGHTNING_LWIDTH, FX_LIGHTNING_LHEIGHT, 0), 0xffffffff, 0, 0.0f, 0.0f );
	m_lverRect[2] = D3DLVERTEX( D3DVECTOR( FX_LIGHTNING_LWIDTH, 0, 0), 0xffffffff, 0, 0.25f, 1.0f );
	m_lverRect[3] = D3DLVERTEX( D3DVECTOR( FX_LIGHTNING_LWIDTH, FX_LIGHTNING_LHEIGHT, 0), 0xffffffff, 0, 0.25f, 0.0f );

	m_lverRect[4] = D3DLVERTEX( D3DVECTOR(-FX_LIGHTNING_LWIDTH, 0, 0), 0xffffffff, 0, 0.25f, 1.0f );
	m_lverRect[5] = D3DLVERTEX( D3DVECTOR(-FX_LIGHTNING_LWIDTH, FX_LIGHTNING_LHEIGHT, 0), 0xffffffff, 0, 0.25f, 0.0f );
	m_lverRect[6] = D3DLVERTEX( D3DVECTOR( FX_LIGHTNING_LWIDTH, 0, 0), 0xffffffff, 0, 0.5f, 1.0f );
	m_lverRect[7] = D3DLVERTEX( D3DVECTOR( FX_LIGHTNING_LWIDTH, FX_LIGHTNING_LHEIGHT, 0), 0xffffffff, 0, 0.5f, 0.0f );

	m_lverRect[8] = D3DLVERTEX( D3DVECTOR(-FX_LIGHTNING_LWIDTH, 0, 0), 0xffffffff, 0, 0.5f, 1.0f );
	m_lverRect[9] = D3DLVERTEX( D3DVECTOR(-FX_LIGHTNING_LWIDTH, FX_LIGHTNING_LHEIGHT, 0), 0xffffffff, 0, 0.5f, 0.0f );
	m_lverRect[10] = D3DLVERTEX( D3DVECTOR( FX_LIGHTNING_LWIDTH, 0, 0), 0xffffffff, 0, 0.75f, 1.0f );
	m_lverRect[11] = D3DLVERTEX( D3DVECTOR( FX_LIGHTNING_LWIDTH, FX_LIGHTNING_LHEIGHT, 0), 0xffffffff, 0, 0.75f, 0.0f );

	m_lverRect[12] = D3DLVERTEX( D3DVECTOR(-FX_LIGHTNING_LWIDTH, 0, 0), 0xffffffff, 0, 0.75f, 1.0f );
	m_lverRect[13] = D3DLVERTEX( D3DVECTOR(-FX_LIGHTNING_LWIDTH, FX_LIGHTNING_LHEIGHT, 0), 0xffffffff, 0, 0.75f, 0.0f );
	m_lverRect[14] = D3DLVERTEX( D3DVECTOR( FX_LIGHTNING_LWIDTH, 0, 0), 0xffffffff, 0, 1.0f, 1.0f );
	m_lverRect[15] = D3DLVERTEX( D3DVECTOR( FX_LIGHTNING_LWIDTH, FX_LIGHTNING_LHEIGHT, 0), 0xffffffff, 0, 1.0f, 0.0f );

	// 바닥에 그려지는 폭파
	m_nSparkSize[0] = 30;
	m_nSparkSize[1] = 40;
	m_nSparkSize[2] = 57;
	m_nSparkSize[3] = 60;
	m_nSparkSize[4] = 60;
	m_nSparkSize[5] = 55;
	m_nSparkSize[6] = 45;
	m_nSparkSize[7] = 30;
	m_nSparkSize[8] = 15;
	m_nSparkSize[9] = 10;
}

CFxHellDown_Lightning::~CFxHellDown_Lightning()
{
	DeleteRes();
}

void CFxHellDown_Lightning::LoadRes(IndexedTexture *pIndexedTexture, CShadowRect *pShadowRect)
{}

void CFxHellDown_Lightning::DeleteRes()
{}

void CFxHellDown_Lightning::Render(EffectSort &effect_sort, int nFrame)
{
	if( !GET_D3DDEVICE() )
		return;

	if(!effect_sort.pNkChaTo
			&& !effect_sort.pMobTo)
		return;

	LPDIRECTDRAWSURFACE7 lpSurface = NULL;
	D3DVECTOR vViewVec;
	D3DMATRIX matRx, matRy, matWorld;
	vViewVec = pCMyApp->GetEyeDirection();

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND,   D3DBLEND_ONE );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND,  D3DBLEND_ONE );

	static int offset = 0;

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ZENABLE, TRUE);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, FALSE);

	D3DVECTOR				vPos;

	if (effect_sort.pMobTo)
		vPos = D3DVECTOR(effect_sort.pMobTo->m_wx, effect_sort.pMobTo->m_wy + 20.0f, effect_sort.pMobTo->m_wz);
	else if (effect_sort.pNkChaTo)
		vPos = D3DVECTOR(effect_sort.pNkChaTo->m_wx, effect_sort.pNkChaTo->m_wy + 20.0f, effect_sort.pNkChaTo->m_wz);
	else
		return;

	if (nFrame < 40)
	{
		float ry = atan2f(vViewVec.x, vViewVec.z);
		float rx = 0.0f;

		D3DUtil_SetRotateXMatrix(matRx, rx);
		D3DUtil_SetRotateYMatrix(matRy, ry);
		D3DMath_MatrixMultiply(matWorld, matRx, matRy);

		matWorld._41 = vPos.x;
		matWorld._42 = vPos.y;
		matWorld._43 = vPos.z;

		GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &matWorld );

		// 텍스처 택하기.
		// nate 104 - 7 - image
		lpSurface = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_HELLDOWN_LIGHTNING ] );
		GET_D3DDEVICE()->SetTexture(0, lpSurface);

		float scale, fade;

		scale = (float)nFrame / 30;
		fade = 1.0f - (scale * 0.75f);

		float red, green, blue;
		red = green = blue = 1.0f;

		m_lverRect[offset*4].color =D3DRGBA(red, green, blue, 1.0f);
		m_lverRect[offset*4 + 1].color =D3DRGBA(red, green, blue, 1.0f);
		m_lverRect[offset*4 + 2].color =D3DRGBA(red, green, blue, 1.0f);
		m_lverRect[offset*4 + 3].color =D3DRGBA(red, green, blue, 1.0f);

		GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_LVERTEX,
										&m_lverRect[offset*4], 4, 0 );
	}

	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, FALSE);

	// 먼저 바닥부터 그림.
	if (nFrame < 40)
	{
		D3DUtil_SetIdentityMatrix(matWorld);
		matWorld._41 = vPos.x;
		matWorld._42 = vPos.y;		// y축 보정.
		matWorld._43 = vPos.z;

		GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &matWorld );

		// nate 104 - 7 - image
		if( g_pCapsyongTexture )
			lpSurface = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_LIGHTNINGSPARK ] );

		if (g_bDungeon)
			// nate 104 - 7 글로벌 ShadowRect사용
			g_pShadowRect->SetTileCoord(vPos, m_nSparkSize[nFrame]*0.6, FALSE);
		else
			// nate 104 - 7 글로벌 ShadowRect사용
			g_pShadowRect->SetTileCoord(vPos, m_nSparkSize[nFrame]*0.3, FALSE);
	}

	// 번개 잔상 그리기.
	int i;
	if (nFrame < 40)
	{
		for (i=0; i < 40; i++)
			RenderFrag2(effect_sort, nFrame);
	}
	else
	{
		for (i=m_nTotalFrame-nFrame+5; i > 0; i-=2)
			RenderFrag2(effect_sort, nFrame);
	}

	offset++;
	if (offset > 3)
		offset = 0;

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE);
}

void CFxHellDown_Lightning::Render2(EffectSort &effect_sort, int nFrame)
{
	if( !GET_D3DDEVICE() )
		return;

	if(!effect_sort.pNkChaTo
			&& !effect_sort.pMobTo)
		return;

	LPDIRECTDRAWSURFACE7 lpSurface = NULL;
	D3DVECTOR vViewVec;
	D3DMATRIX matRx, matRy, matWorld;
	vViewVec = pCMyApp->GetEyeDirection();

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND,   D3DBLEND_ONE );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND,  D3DBLEND_ONE );

	static int offset = 0;
	D3DVECTOR	vPos;

	if (effect_sort.pMobTo)
		vPos = D3DVECTOR(effect_sort.pMobTo->m_wx + 10.0f, effect_sort.pMobTo->m_wy + 20.0f, effect_sort.pMobTo->m_wz + 10.0f);
	else if (effect_sort.pNkChaTo)
		vPos = D3DVECTOR(effect_sort.pNkChaTo->m_wx + 10.0f, effect_sort.pNkChaTo->m_wy + 20.0f, effect_sort.pNkChaTo->m_wz + 10.0f);

	if (nFrame < 45)
	{
		float ry = atan2f(vViewVec.x, vViewVec.z);
		float rx = 0.0f;

		D3DUtil_SetRotateXMatrix(matRx, rx);
		D3DUtil_SetRotateYMatrix(matRy, ry);
		D3DMath_MatrixMultiply(matWorld, matRx, matRy);

		matWorld._41 = vPos.x;
		matWorld._42 = vPos.y;
		matWorld._43 = vPos.z;

		GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &matWorld );

		// 텍스처 택하기.
		// nate 454 - 7 - image
		if( g_pCapsyongTexture )
			lpSurface = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_HELLDOWN_LIGHTNING ] );
		GET_D3DDEVICE()->SetTexture(0, lpSurface);

		float scale, fade;

		scale = (float)nFrame / 30;
		fade = 1.0f - (scale * 0.75f);

		float red, green, blue;
		red = green = blue = 1.0f;

		m_lverRect[offset*4].color =D3DRGBA(red, green, blue, fade);
		m_lverRect[offset*4 + 1].color =D3DRGBA(red, green, blue, fade);
		m_lverRect[offset*4 + 2].color =D3DRGBA(red, green, blue, fade);
		m_lverRect[offset*4 + 3].color =D3DRGBA(red, green, blue, fade);

		GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_LVERTEX,
										&m_lverRect[offset*4], 4, 0 );
	}

	// 먼저 바닥부터 그림.
	if (nFrame < 45)
	{
		D3DUtil_SetIdentityMatrix(matWorld);
		matWorld._41 = vPos.x;
		matWorld._42 = vPos.y;		// y축 보정.
		matWorld._43 = vPos.z;

		GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &matWorld );

		// nate 454 - 7 - image
		lpSurface = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_LIGHTNINGSPARK ] );

		if (g_bDungeon)
			// nate 454 - 7 글로벌 ShadowRect사용
			g_pShadowRect->SetTileCoord(vPos, m_nSparkSize[nFrame]*0.6, FALSE);
		else
			// nate 454 - 7 글로벌 ShadowRect사용
			g_pShadowRect->SetTileCoord(vPos, m_nSparkSize[nFrame]*0.3, FALSE);
	}

	// 번개 잔상 그리기.
	int i;
	if (nFrame < 45)
	{
		for (i=0; i < 45; i++)
			RenderFrag2(effect_sort, nFrame);
	}
	else
	{
		for (i=m_nTotalFrame-nFrame+5; i > 0; i-=2)
			RenderFrag2(effect_sort, nFrame);
	}

	offset++;
	if (offset > 3)
		offset = 0;

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE);
}

void CFxHellDown_Lightning::Render3(EffectSort &effect_sort, int nFrame)
{
	if( !GET_D3DDEVICE() )
		return;

	if(!effect_sort.pNkChaTo
			&& !effect_sort.pMobTo)
		return;

	LPDIRECTDRAWSURFACE7 lpSurface = NULL;
	D3DVECTOR vViewVec;
	D3DMATRIX matRx, matRy, matWorld;
	vViewVec = pCMyApp->GetEyeDirection();

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND,   D3DBLEND_ONE );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND,  D3DBLEND_ONE );

	static int offset = 0;
	D3DVECTOR	vPos;

	if (effect_sort.pMobTo)
		vPos = D3DVECTOR(effect_sort.pMobTo->m_wx - 10.0f, effect_sort.pMobTo->m_wy + 20.0f, effect_sort.pMobTo->m_wz + 10.0f);
	else if (effect_sort.pNkChaTo)
		vPos = D3DVECTOR(effect_sort.pNkChaTo->m_wx - 10.0f, effect_sort.pNkChaTo->m_wy + 20.0f, effect_sort.pNkChaTo->m_wz + 10.0f);

	if (nFrame < 45)
	{
		float ry = atan2f(vViewVec.x, vViewVec.z);
		float rx = 0.0f;

		D3DUtil_SetRotateXMatrix(matRx, rx);
		D3DUtil_SetRotateYMatrix(matRy, ry);
		D3DMath_MatrixMultiply(matWorld, matRx, matRy);

		matWorld._41 = vPos.x;
		matWorld._42 = vPos.y;
		matWorld._43 = vPos.z;

		GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &matWorld );

		// 텍스처 택하기.
		// nate 454 - 7 - image
		if( g_pCapsyongTexture )
			lpSurface = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_HELLDOWN_LIGHTNING ] );
		GET_D3DDEVICE()->SetTexture(0, lpSurface);

		float scale, fade;

		scale = (float)nFrame / 30;
		fade = 1.0f - (scale * 0.75f);

		float red, green, blue;
		//  red = green = blue = 0.0f;
		red = green = blue = 1.0f;

		m_lverRect[offset*4].color =D3DRGBA(red, green, blue, fade);
		m_lverRect[offset*4 + 1].color =D3DRGBA(red, green, blue, fade);
		m_lverRect[offset*4 + 2].color =D3DRGBA(red, green, blue, fade);
		m_lverRect[offset*4 + 3].color =D3DRGBA(red, green, blue, fade);

		GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_LVERTEX,
										&m_lverRect[offset*4], 4, 0 );
	}

	// 먼저 바닥부터 그림.
	if (nFrame < 45)
	{
		D3DUtil_SetIdentityMatrix(matWorld);
		matWorld._41 = vPos.x;
		matWorld._42 = vPos.y;		// y축 보정.
		matWorld._43 = vPos.z;

		GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &matWorld );

		// nate 454 - 7 - image
		lpSurface = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_LIGHTNINGSPARK ] );

		if (g_bDungeon)
			// nate 454 - 7 글로벌 ShadowRect사용
			g_pShadowRect->SetTileCoord(vPos, m_nSparkSize[nFrame]*0.6, FALSE);
		else
			// nate 454 - 7 글로벌 ShadowRect사용
			g_pShadowRect->SetTileCoord(vPos, m_nSparkSize[nFrame]*0.3, FALSE);
	}

	// 번개 잔상 그리기.
	int i;
	if (nFrame < 45)
	{
		for (i=0; i < 45; i++)
			RenderFrag2(effect_sort, nFrame);
	}
	else
	{
		for (i=m_nTotalFrame-nFrame+5; i > 0; i-=2)
			RenderFrag2(effect_sort, nFrame);
	}

	offset++;
	if (offset > 3)
		offset = 0;

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE);
}

void CFxHellDown_Lightning::Render4(EffectSort &effect_sort, int nFrame)
{
	if( !GET_D3DDEVICE() )
		return;

	if(!effect_sort.pNkChaTo
			&& !effect_sort.pMobTo)
		return;

	LPDIRECTDRAWSURFACE7 lpSurface = NULL;
	D3DVECTOR vViewVec;
	D3DMATRIX matRx, matRy, matWorld;
	vViewVec = pCMyApp->GetEyeDirection();

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND,   D3DBLEND_ONE );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND,  D3DBLEND_ONE );

	static int offset = 0;
	D3DVECTOR	vPos;

	if (effect_sort.pMobTo)
		vPos = D3DVECTOR(effect_sort.pMobTo->m_wx - 10.0f, effect_sort.pMobTo->m_wy + 20.0f, effect_sort.pMobTo->m_wz - 10.0f);
	else if (effect_sort.pNkChaTo)
		vPos = D3DVECTOR(effect_sort.pNkChaTo->m_wx - 10.0f, effect_sort.pNkChaTo->m_wy + 20.0f, effect_sort.pNkChaTo->m_wz - 10.0f);
	else
		return;

	if (nFrame < 45)
	{
		float ry = atan2f(vViewVec.x, vViewVec.z);
		float rx = 0.0f;

		D3DUtil_SetRotateXMatrix(matRx, rx);
		D3DUtil_SetRotateYMatrix(matRy, ry);
		D3DMath_MatrixMultiply(matWorld, matRx, matRy);

		matWorld._41 = vPos.x;
		matWorld._42 = vPos.y;
		matWorld._43 = vPos.z;

		GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &matWorld );

		// 텍스처 택하기.
		// nate 454 - 7 - image
		if( g_pCapsyongTexture )
			lpSurface = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_HELLDOWN_LIGHTNING ] );
		GET_D3DDEVICE()->SetTexture(0, lpSurface);

		float scale, fade;

		scale = (float)nFrame /30;
		fade = 1.0f - (scale * 0.75f);

		float red, green, blue;
		red = green = blue = 1.0f;

		m_lverRect[offset*4].color =D3DRGBA(red, green, blue, fade);
		m_lverRect[offset*4 + 1].color =D3DRGBA(red, green, blue, fade);
		m_lverRect[offset*4 + 2].color =D3DRGBA(red, green, blue, fade);
		m_lverRect[offset*4 + 3].color =D3DRGBA(red, green, blue, fade);

		GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_LVERTEX,
										&m_lverRect[offset*4], 4, 0 );
	}

	// 먼저 바닥부터 그림.
	if (nFrame < 45)
	{
		D3DUtil_SetIdentityMatrix(matWorld);
		matWorld._41 = vPos.x;
		matWorld._42 = vPos.y;		// y축 보정.
		matWorld._43 = vPos.z;

		GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &matWorld );

		// nate 454 - 7 - image
		lpSurface = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_LIGHTNINGSPARK ] );

		if (g_bDungeon)
			// nate 454 - 7 글로벌 ShadowRect사용
			g_pShadowRect->SetTileCoord(vPos, m_nSparkSize[nFrame]*0.6, FALSE);
		else
			// nate 454 - 7 글로벌 ShadowRect사용
			g_pShadowRect->SetTileCoord(vPos, m_nSparkSize[nFrame]*0.3, FALSE);
	}

	// 번개 잔상 그리기.
	int i;
	if (nFrame < 45)
	{
		for (i=0; i < 45; i++)
			RenderFrag2(effect_sort, nFrame);
	}
	else
	{
		for (i=m_nTotalFrame-nFrame+5; i > 0; i-=2)
			RenderFrag2(effect_sort, nFrame);
	}

	offset++;
	if (offset > 3)
		offset = 0;

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE);
}

void CFxHellDown_Lightning::Render5(EffectSort &effect_sort, int nFrame)
{
	if( !GET_D3DDEVICE() )
		return;

	if(!effect_sort.pNkChaTo
			&& !effect_sort.pMobTo)
		return;

	LPDIRECTDRAWSURFACE7 lpSurface = NULL;
	D3DVECTOR vViewVec;
	D3DMATRIX matRx, matRy, matWorld;
	vViewVec = pCMyApp->GetEyeDirection();

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND,   D3DBLEND_ONE );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND,  D3DBLEND_ONE );

	static int offset = 0;
	D3DVECTOR	vPos;

	if (effect_sort.pMobTo)
		vPos = D3DVECTOR(effect_sort.pMobTo->m_wx + 10.0f, effect_sort.pMobTo->m_wy + 20.0f, effect_sort.pMobTo->m_wz - 10.0f);
	else if (effect_sort.pNkChaTo)
		vPos = D3DVECTOR(effect_sort.pNkChaTo->m_wx + 10.0f, effect_sort.pNkChaTo->m_wy + 20.0f, effect_sort.pNkChaTo->m_wz - 10.0f);
	else
		return;

	if (nFrame < 45)
	{
		float ry = atan2f(vViewVec.x, vViewVec.z);
		float rx = 0.0f;

		D3DUtil_SetRotateXMatrix(matRx, rx);
		D3DUtil_SetRotateYMatrix(matRy, ry);
		D3DMath_MatrixMultiply(matWorld, matRx, matRy);

		matWorld._41 = vPos.x;
		matWorld._42 = vPos.y;
		matWorld._43 = vPos.z;

		GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &matWorld );

		// 텍스처 택하기.
		// nate 454 - 7 - image
		lpSurface = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_HELLDOWN_LIGHTNING ] );
		GET_D3DDEVICE()->SetTexture(0, lpSurface);

		float scale, fade;

		scale = (float)nFrame / 30;
		fade = 1.0f - (scale * 0.75f);

		float red, green, blue;
		red = green = blue = 1.0f;

		m_lverRect[offset*4].color =D3DRGBA(red, green, blue, fade);
		m_lverRect[offset*4 + 1].color =D3DRGBA(red, green, blue, fade);
		m_lverRect[offset*4 + 2].color =D3DRGBA(red, green, blue, fade);
		m_lverRect[offset*4 + 3].color =D3DRGBA(red, green, blue, fade);

		GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_LVERTEX,
										&m_lverRect[offset*4], 4, 0 );
	}

	// 먼저 바닥부터 그림.
	if (nFrame < 45)
	{
		D3DUtil_SetIdentityMatrix(matWorld);
		matWorld._41 = vPos.x;
		matWorld._42 = vPos.y;		// y축 보정.
		matWorld._43 = vPos.z;

		GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &matWorld );

		// nate 454 - 7 - image
		lpSurface = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_LIGHTNINGSPARK ] );

		if (g_bDungeon)
			// nate 454 - 7 글로벌 ShadowRect사용
			g_pShadowRect->SetTileCoord(vPos, m_nSparkSize[nFrame]*0.6, FALSE);
		else
			// nate 454 - 7 글로벌 ShadowRect사용
			g_pShadowRect->SetTileCoord(vPos, m_nSparkSize[nFrame]*0.3, FALSE);
	}

	// 번개 잔상 그리기.
	int i;
	if (nFrame < 45)
	{
		for (i=0; i < 45; i++)
			RenderFrag2(effect_sort, nFrame);
	}
	else
	{
		for (i=m_nTotalFrame-nFrame+5; i > 0; i-=2)
			RenderFrag2(effect_sort, nFrame);
	}

	offset++;
	if (offset > 3)
		offset = 0;

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE);
}

void CFxHellDown_Lightning::RenderFrag(EffectSort &effect_sort, int nFrame)
{
	if( !GET_D3DDEVICE() )
		return;

	if(!effect_sort.pNkChaTo
			&& !effect_sort.pMobTo)
		return;

	LPDIRECTDRAWSURFACE7 lpSurface = NULL;
	D3DVECTOR vViewVec;
	D3DMATRIX matRx, matRy, matRz, matWorld;

	D3DVECTOR				vPos;

	if (effect_sort.pMobTo)
		vPos = D3DVECTOR(effect_sort.pMobTo->m_wx, effect_sort.pMobTo->m_wy, effect_sort.pMobTo->m_wz);
	else if (effect_sort.pNkChaTo)
		vPos = D3DVECTOR(effect_sort.pNkChaTo->m_wx, effect_sort.pNkChaTo->m_wy, effect_sort.pNkChaTo->m_wz);

	// 임시 변수
	D3DVECTOR vRand, vInvRand, vOri;
	float fWidth = 4.0f;

	vRand = D3DVECTOR(4.0f, 10.0f, 0.0f);
	vOri = D3DVECTOR(0.0f, 0.0f, 0.0f);

	vInvRand = vOri - vRand;

	vInvRand.x = vInvRand.y;
	vInvRand.y = vInvRand.x;
	vInvRand.z = 0.0f;

	vInvRand = Normalize(vInvRand);
	vInvRand *= fWidth;
	// 임시 변수 끝.

	// 몇번째 사각형을 쓸 것인가를 결정.
	int nWhich = 0;

	nWhich = (int) (rnd()*3.0f + 0.5f);

	vViewVec = pCMyApp->GetEyeDirection();

	// 큰 사각형 그리기.
	float ry = atan2f(vViewVec.x, vViewVec.z);
	float rx = 0.0f;

	D3DUtil_SetRotateXMatrix(matRx, rx);
	D3DUtil_SetRotateYMatrix(matRy, ry);
	D3DMath_MatrixMultiply(matWorld, matRx, matRy);

	matWorld._41 = vPos.x;
	matWorld._42 = vPos.y;
	matWorld._43 = vPos.z;

	GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &matWorld );
	// nate 2004 - 7 - image
	if( g_pCapsyongTexture )
		lpSurface = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_LIGHTNING ] );
	GET_D3DDEVICE()->SetTexture(0, lpSurface);

	float scale, fade;
	scale = (float)nFrame / 30;
	fade = 1.0f - (scale * 0.75f);

	float red, green, blue;
	red = 1.0f;
	green = blue = 0.1f;

	m_lverFrag[0] = D3DLVERTEX( D3DVECTOR(vOri.x + vInvRand.x, vOri.y - vInvRand.y, vOri.z),
								D3DRGBA(red, green, blue, fade), 0, 0.25f*nWhich, 1.0f );
	m_lverFrag[1] = D3DLVERTEX( D3DVECTOR(vRand.x + vInvRand.x, vRand.y - vInvRand.y, vRand.z),
								D3DRGBA(red, green, blue, fade), 0, 0.25f*nWhich, 0.0f );
	m_lverFrag[2] = D3DLVERTEX( D3DVECTOR( vOri.x - vInvRand.x, vOri.y + vInvRand.y, vOri.z ),
								D3DRGBA(red, green, blue, fade), 0, 0.25f*(nWhich+1), 1.0f );
	m_lverFrag[3] = D3DLVERTEX( D3DVECTOR( vRand.x - vInvRand.x, vRand.y + vInvRand.y, vRand.z),
								D3DRGBA(red, green, blue, fade), 0, 0.25f*(nWhich+1), 0.0f );

	GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_LVERTEX, m_lverFrag, 4, 0 );
}

void CFxHellDown_Lightning::RenderFrag2(EffectSort &effect_sort, int nFrame)
{
	if( !GET_D3DDEVICE() )
		return;

	if(!effect_sort.pNkChaTo
			&& !effect_sort.pMobTo)
		return;

	LPDIRECTDRAWSURFACE7 lpSurface = NULL;
	D3DVECTOR vViewVec;
	D3DMATRIX matRx, matRy, matRz, matId, matWorld, matView, matViewOld;

	D3DVECTOR				vPos;

	if (effect_sort.pMobTo)
		vPos = D3DVECTOR(effect_sort.pMobTo->m_wx, effect_sort.pMobTo->m_wy, effect_sort.pMobTo->m_wz);
	else if (effect_sort.pNkChaTo)
		vPos = D3DVECTOR(effect_sort.pNkChaTo->m_wx, effect_sort.pNkChaTo->m_wy, effect_sort.pNkChaTo->m_wz);
	else
		return;

	// 임시 변수
	D3DVECTOR vRand, vInvVec, vOri, vVec, vVec2;
	float fThetha = 0.0f;
	float fWidth = 10.0f;

	fThetha = rnd() * g_2_PI;
	vRand.x = cosf(fThetha)*fWidth*(rnd()+0.2f);
	vRand.z = sinf(fThetha)*fWidth*(rnd()+0.2f);
	vRand.y = rnd() * 13.0f;
	vOri = D3DVECTOR(0.0f, 0.0f, 0.0f);

	D3DUtil_SetIdentityMatrix(matWorld);
	matWorld._41 = vPos.x;
	matWorld._42 = vPos.y;
	matWorld._43 = vPos.z;

	GET_D3DDEVICE()->GetTransform(D3DTRANSFORMSTATE_VIEW, &matViewOld);

	D3DMath_MatrixMultiply(matView, matWorld, matViewOld);

	D3DMath_VectorMatrixMultiply(vVec, vOri, matView);
	D3DMath_VectorMatrixMultiply(vVec2, vRand, matView);

	vInvVec = vVec;
	vInvVec -= vVec2;

	float x_temp = vInvVec.x;
	vInvVec.x = vInvVec.y;
	vInvVec.y = x_temp;
	vInvVec.z = 0.0f;

	vInvVec = Normalize(vInvVec);
	vInvVec *= fWidth;
	// 임시 변수 끝.

	// 몇번째 사각형을 쓸 것인가를 결정.
	int nWhich = 0;
	nWhich = (int) (rnd()*3.0f + 0.5f);

	D3DUtil_SetIdentityMatrix(matId);

	GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &matId );
	GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_VIEW, &matId );
	// nate 2004 - 7 - image
	if( g_pCapsyongTexture )
		lpSurface = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_LIGHTNING ] );
	GET_D3DDEVICE()->SetTexture(0, lpSurface);

	float scale, fade;

	scale = (float)nFrame / 30;
	fade = 1.0f - (scale * 0.75f);

	float red, green, blue;
	red = 1.0f;
	green = blue = 0.1f;

	m_lverFrag[0] = D3DLVERTEX( D3DVECTOR(vVec.x + vInvVec.x, vVec.y - vInvVec.y, vVec.z),
								D3DRGBA(red, green, blue, fade), 0, 0.25f*nWhich, 1.0f );
	m_lverFrag[1] = D3DLVERTEX( D3DVECTOR(vVec2.x + vInvVec.x, vVec2.y - vInvVec.y, vVec2.z),
								D3DRGBA(red, green, blue, fade), 0, 0.25f*nWhich, 0.0f );
	m_lverFrag[2] = D3DLVERTEX( D3DVECTOR( vVec.x - vInvVec.x, vVec.y + vInvVec.y, vVec.z ),
								D3DRGBA(red, green, blue, fade), 0, 0.25f*(nWhich+1), 1.0f );
	m_lverFrag[3] = D3DLVERTEX( D3DVECTOR( vVec2.x - vInvVec.x, vVec2.y + vInvVec.y, vVec2.z),
								D3DRGBA(red, green, blue, fade), 0, 0.25f*(nWhich+1), 0.0f );

	GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_LVERTEX, m_lverFrag, 4, 0 );
	GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_VIEW, &matViewOld );
}
