#include "stdafx.h"
#define D3D_OVERLOADS
#include <d3d.h>
#include "FxSet.h"
#include "FxFireBoomShort.h"
#include "main.h"
#include "IndexedTexture.h"
#include "ShadowRect.h"
#include "GUtil.h"
#include "DirtSnd.h"
#include "LoadEffectSound.h"
#include "ImageDefine.h"
#include "NkCharacter.h"
#include "NkMob.h"


extern BOOL g_bDungeon;
// nate 2004 - 7 글로벌 ShadowRect사용


////////////////////////////////////////////////////////////////////////////////////////////
// CFxFireBoomShort
CFxFireBoomShort::CFxFireBoomShort()
{
	m_fBoomSize = 25.0f;

	m_Texcode[0].tu = 0.0f;
	m_Texcode[0].tv = 0.0f;
	m_Texcode[1].tu = 0.25f;
	m_Texcode[1].tv = 0.0f;
	m_Texcode[2].tu = 0.5f;
	m_Texcode[2].tv = 0.0f;
	m_Texcode[3].tu = 0.75f;
	m_Texcode[3].tv = 0.0f;

	m_Texcode[4].tu = 0.0f;
	m_Texcode[4].tv = 0.25f;
	m_Texcode[5].tu = 0.25f;
	m_Texcode[5].tv = 0.25f;
	m_Texcode[6].tu = 0.5f;
	m_Texcode[6].tv = 0.25f;
	m_Texcode[7].tu = 0.75f;
	m_Texcode[7].tv = 0.25f;

	m_Texcode[8].tu = 0.0f;
	m_Texcode[8].tv = 0.5f;
	m_Texcode[9].tu = 0.25f;
	m_Texcode[9].tv = 0.5f;
	m_Texcode[10].tu = 0.5f;
	m_Texcode[10].tv = 0.5f;
	m_Texcode[11].tu = 0.75f;
	m_Texcode[11].tv = 0.5f;

	m_fBaseSize[0] = 70.0f;
	m_fBaseSize[1] = 67.0f;
	m_fBaseSize[2] = 66.0f;
	m_fBaseSize[3] = 63.0f;
	m_fBaseSize[4] = 60.0f;

	m_fBaseSize[5] = 57.0f;
	m_fBaseSize[6] = 53.0f;
	m_fBaseSize[7] = 50.0f;
	m_fBaseSize[8] = 47.0f;
	m_fBaseSize[9] = 43.0f;

	m_fBaseSize[10] = 40.0f;
	m_fBaseSize[11] = 35.0f;
	m_fBaseSize[12] = 30.0f;

}

CFxFireBoomShort::~CFxFireBoomShort()
{
	DeleteRes();
}

void CFxFireBoomShort::LoadRes(IndexedTexture *pIndexedTexture, CShadowRect *pShadowRect)
{}
void CFxFireBoomShort::DeleteRes()
{}

BOOL CFxFireBoomShort::Render(EffectSort &effect_sort)
{
	if( !GET_D3DDEVICE() )
		return true;

	LPDIRECTDRAWSURFACE7 lpSurface = NULL;
	D3DVECTOR vViewVec;
	D3DMATRIX matRx, matRy, matWorld;
	BOOL bFinished = FALSE;

	vViewVec = pCMyApp->GetEyeDirection();

	float ry = atan2f(vViewVec.x, vViewVec.z);
	float rx = atan2f(vViewVec.x*sinf(ry)+vViewVec.z*cosf(ry), vViewVec.y) - (g_PI / 2.0f) ;

	D3DUtil_SetRotateXMatrix(matRx, rx);
	D3DUtil_SetRotateYMatrix(matRy, ry);
	D3DMath_MatrixMultiply(matWorld, matRx, matRy);

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND,   D3DBLEND_ONE );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND,  D3DBLEND_ONE );

	D3DVECTOR vPosTo;
	vPosTo = D3DVECTOR(effect_sort.vTargetPos.x, effect_sort.vTargetPos.y, effect_sort.vTargetPos.z);

	///////////////////////////////////////////////////////////////////////
	// 폭파 그리기.
	matWorld._41 = vPosTo.x;
	matWorld._42 = vPosTo.y + FX_FIREBALL_HEIGHT;
	matWorld._43 = vPosTo.z;

	MakeLVertexRect(m_lverBoom, m_fBoomSize, m_fBoomSize, TRUE);

	if (effect_sort.nCurFrame < FX_FIREBALLBOOM_FRAMECOUNT)
	{
		m_lverBoom[0].tu = m_Texcode[effect_sort.nCurFrame].tu;
		m_lverBoom[0].tv = m_Texcode[effect_sort.nCurFrame].tv+0.25f;
		m_lverBoom[1].tu = m_Texcode[effect_sort.nCurFrame].tu;
		m_lverBoom[1].tv = m_Texcode[effect_sort.nCurFrame].tv;
		m_lverBoom[2].tu = m_Texcode[effect_sort.nCurFrame].tu+0.25f;
		m_lverBoom[2].tv = m_Texcode[effect_sort.nCurFrame].tv+0.25f;
		m_lverBoom[3].tu = m_Texcode[effect_sort.nCurFrame].tu+0.25f;
		m_lverBoom[3].tv = m_Texcode[effect_sort.nCurFrame].tv;
	}

	// nate 2004 - 7 - image
	if( g_pCapsyongTexture )
		lpSurface = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_FIREBOOMSHORT ] ) ;

	GET_D3DDEVICE()->SetTexture(0, lpSurface);
	GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &matWorld );
	GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_LVERTEX,
									m_lverBoom, 4, 0 );

	// nate 2004 - 7 글로벌 ShadowRect사용
	if( g_pShadowRect )
	{
		// nate 2004 - 7 - image
		if( g_pCapsyongTexture )
			lpSurface = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_FIREBOOMSHORT_BASE ] ) ;

		if (g_bDungeon)
			// nate 2004 - 7 글로벌 ShadowRect사용
			g_pShadowRect->SetTileCoord(vPosTo, m_fBaseSize[effect_sort.nCurFrame]*0.6, FALSE);		// 부울값은 시간인가 아닌가
		else
			// nate 2004 - 7 글로벌 ShadowRect사용
			g_pShadowRect->SetTileCoord(vPosTo, m_fBaseSize[effect_sort.nCurFrame], FALSE);		// 부울값은 시간인가 아닌가

		// nate 2004 - 7 글로벌 ShadowRect사용
		g_pShadowRect->Render(lpSurface, FALSE);	// 부울값은 리얼그림자인가 아닌가?
	}

	if (effect_sort.nCurFrame >= FX_FIREBALLBOOM_FRAMECOUNT - 1)
		return TRUE;

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE);

	return bFinished;
}