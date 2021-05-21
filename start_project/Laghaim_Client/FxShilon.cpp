#include "stdafx.h"
#define D3D_OVERLOADS

#include <d3d.h>
#include "FxSet.h"
#include "FxShilon.h"
#include "main.h"
#include "IndexedTexture.h"
#include "ShadowRect.h"
#include "GUtil.h"
#include "DirtSnd.h"
#include "LoadEffectSound.h"
#include "ImageDefine.h"
#include "NkCharacter.h"
#include "NkMob.h"
#include "D3DMath.h"


extern BOOL g_bDungeon;
extern float g_fTimeKey;


#define FX_SHILON_BOOMSIZE 30.0f
#define FX_SHILON_FRAGSIZE 3.0f

CFxShilon::CFxShilon()
{
	m_Texcode[0].tu = 0.0f;
	m_Texcode[0].tv = 0.0f;
	m_Texcode[1].tu = 0.25f;
	m_Texcode[1].tv = 0.0f;
	m_Texcode[2].tu = 0.5f;
	m_Texcode[2].tv = 0.0f;
	m_Texcode[3].tu = 0.75f;
	m_Texcode[3].tv = 0.0f;

	m_Texcode[4].tu = 0.0f;
	m_Texcode[4].tv = 0.5f;
	m_Texcode[5].tu = 0.25f;
	m_Texcode[5].tv = 0.5f;
	m_Texcode[6].tu = 0.5f;
	m_Texcode[6].tv = 0.5f;
	m_Texcode[7].tu = 0.75f;
	m_Texcode[7].tv = 0.5f;

	m_nFrameIdx[0] = 0;
	m_nFrameIdx[1] = 0;
	m_nFrameIdx[2] = 1;
	m_nFrameIdx[3] = 1;
	m_nFrameIdx[4] = 2;

	m_nFrameIdx[5] = 2;
	m_nFrameIdx[6] = 3;
	m_nFrameIdx[7] = 3;
	m_nFrameIdx[8] = 4;
	m_nFrameIdx[9] = 4;

	m_nFrameIdx[10] = 5;
	m_nFrameIdx[11] = 5;
	m_nFrameIdx[12] = 6;
	m_nFrameIdx[13] = 6;

	m_nFrameIdx[14] = 7;

	m_nFrameIdx[15] = 7;
	m_nFrameIdx[16] = 7;
	m_nFrameIdx[17] = 7;
	m_nFrameIdx[18] = 7;
	m_nFrameIdx[19] = 7;

	MakeLVertexRect(m_lverBoom, FX_SHILON_BOOMSIZE, FX_SHILON_BOOMSIZE, TRUE);
	m_lverBoom[0].color = 0xffffd29f;
	m_lverBoom[1].color = 0xffffd29f;
	m_lverBoom[2].color = 0xffffd29f;
	m_lverBoom[3].color = 0xffffd29f;

	m_fBaseSize[0] = 70.0f;
	m_fBaseSize[1] = 70.0f;
	m_fBaseSize[2] = 75.0f;
	m_fBaseSize[3] = 75.0f;
	m_fBaseSize[4] = 75.0f;

	m_fBaseSize[5] = 87.0f;
	m_fBaseSize[6] = 87.0f;
	m_fBaseSize[7] = 87.0f;
	m_fBaseSize[8] = 85.0f;
	m_fBaseSize[9] = 90.0f;

	m_fBaseSize[10] = 90.0f;
	m_fBaseSize[11] = 90.0f;
	m_fBaseSize[12] = 70.0f;
	m_fBaseSize[13] = 50.0f;
	m_fBaseSize[14] = 40.0f;

	m_fBaseSize[15] = 30.0f;
}

CFxShilon::~CFxShilon()
{
	DeleteRes();
}

void CFxShilon::LoadRes(IndexedTexture *pIndexedTexture, CShadowRect *pShadowRect)
{}
void CFxShilon::DeleteRes()
{}

BOOL CFxShilon::Render(EffectSort &effect_sort)
{
	if( !GET_D3DDEVICE() )
		return true;

	BOOL bFinished = FALSE;

	LPDIRECTDRAWSURFACE7 lpSurface = NULL;
	D3DVECTOR vViewVec;
	D3DMATRIX matRx, matRy, matWorld;

	vViewVec = pCMyApp->GetEyeDirection();

	float ry = atan2f(vViewVec.x, vViewVec.z);
	float rx = atan2f(vViewVec.x*sinf(ry)+vViewVec.z*cosf(ry), vViewVec.y) - (g_PI / 2.0f) ;

	D3DUtil_SetRotateXMatrix(matRx, rx);
	D3DUtil_SetRotateYMatrix(matRy, ry);
	D3DMath_MatrixMultiply(matWorld, matRx, matRy);

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE,   TRUE );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ALPHATESTENABLE,   FALSE );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND,   D3DBLEND_SRCALPHA );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND,  D3DBLEND_INVSRCALPHA );

	///////////////////////////////////////////////////////////////////////
	// 잔상 그리기.
	// 방향벡터를 구한다.
	D3DVECTOR vDir;
	D3DVECTOR vDrawPos;
	BOOL bDrawTail = TRUE;
	///////////////////////////////////////////////////////////////////////
	// 폭파 그리기.
	matWorld._41 = effect_sort.vPos.x;
	matWorld._42 = effect_sort.vPos.y + 25;
	matWorld._43 = effect_sort.vPos.z;

	if (effect_sort.nCurFrame < FX_SHILON_FRAMECOUNT)
	{
		m_lverBoom[0].tu = m_Texcode[m_nFrameIdx[effect_sort.nCurFrame]].tu;
		m_lverBoom[0].tv = m_Texcode[m_nFrameIdx[effect_sort.nCurFrame]].tv+0.5f;
		m_lverBoom[1].tu = m_Texcode[m_nFrameIdx[effect_sort.nCurFrame]].tu;
		m_lverBoom[1].tv = m_Texcode[m_nFrameIdx[effect_sort.nCurFrame]].tv;
		m_lverBoom[2].tu = m_Texcode[m_nFrameIdx[effect_sort.nCurFrame]].tu+0.25f;
		m_lverBoom[2].tv = m_Texcode[m_nFrameIdx[effect_sort.nCurFrame]].tv+0.5f;
		m_lverBoom[3].tu = m_Texcode[m_nFrameIdx[effect_sort.nCurFrame]].tu+0.25f;
		m_lverBoom[3].tv = m_Texcode[m_nFrameIdx[effect_sort.nCurFrame]].tv;
	}

	// nate 2004 - 7 - image
	lpSurface = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_SHILONBOOM ] );
	GET_D3DDEVICE()->SetTexture(0, lpSurface);
	GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &matWorld );

	GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_LVERTEX,
									m_lverBoom, 4, 0 );

	if (effect_sort.nCurFrame >= FX_SHILON_FRAMECOUNT - 1)
		bFinished = TRUE;

	if( g_pShadowRect )
	{
		// nate 2004 - 7 - image
		lpSurface = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_SHILONBASE ] );

		if (g_bDungeon)
			// nate 2004 - 7 글로벌 ShadowRect사용
			g_pShadowRect->SetTileCoord(effect_sort.vPos, m_fBaseSize[effect_sort.nCurFrame]*0.6, FALSE);		// 부울값은 시간인가 아닌가
		else
			// nate 2004 - 7 글로벌 ShadowRect사용
			g_pShadowRect->SetTileCoord(effect_sort.vPos, m_fBaseSize[effect_sort.nCurFrame], FALSE);		// 부울값은 시간인가 아닌가

		// nate 2004 - 7 글로벌 ShadowRect사용
		g_pShadowRect->Render(lpSurface, FALSE);	// 부울값은 리얼그림자인가 아닌가?
	}

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE);

	return bFinished;
}