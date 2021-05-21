#include "stdafx.h"
#define D3D_OVERLOADS
#include <d3d.h>
#include "FxSet.h"
#include "FxMeteoriteBoom.h"
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


extern float g_fTimeKey;
// nate 2004 - 7 글로벌 ShadowRect사용


// nate 2004 - 8 ImageManager
#define FX_METEORITEBOOM_SIZE		35.0f

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CFxMeteoriteBoom::CFxMeteoriteBoom()
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

	m_nFrameIdx[0] = 0;
	m_nFrameIdx[1] = 0;
	m_nFrameIdx[2] = 1;
	m_nFrameIdx[3] = 2;
	m_nFrameIdx[4] = 3;

	m_nFrameIdx[5] = 4;
	m_nFrameIdx[6] = 5;
	m_nFrameIdx[7] = 5;
	m_nFrameIdx[8] = 4;
	m_nFrameIdx[9] = 4;

	m_nFrameIdx[10] = 5;
	m_nFrameIdx[11] = 6;
	m_nFrameIdx[12] = 7;
	m_nFrameIdx[13] = 7;

	m_nFrameIdx[14] = 8;

	m_nFrameIdx[15] = 8;
	m_nFrameIdx[16] = 9;
	m_nFrameIdx[17] = 9;
	m_nFrameIdx[18] = 11;
	m_nFrameIdx[19] = 11;

	// nate 2004 - 8 ImageManager
	MakeLVertexRect(m_lverBoom, FX_METEORITEBOOM_SIZE, FX_METEORITEBOOM_SIZE, TRUE);
	m_lverBoom[0].color = 0xffffffff;
	m_lverBoom[1].color = 0xffffffff;
	m_lverBoom[2].color = 0xffffffff;
	m_lverBoom[3].color = 0xffffffff;
}

CFxMeteoriteBoom::~CFxMeteoriteBoom()
{
	DeleteRes();
}

void CFxMeteoriteBoom::LoadRes(IndexedTexture *pIndexedTexture, CShadowRect *pShadowRect)
{}

void CFxMeteoriteBoom::DeleteRes()
{}

BOOL CFxMeteoriteBoom::Render(EffectSort &effect_sort)
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
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND,   D3DBLEND_ONE );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND,  D3DBLEND_ONE );


	if( effect_sort.Index == 1 ) // 이게 1이면 바닥등에 안가려지고 풀로 그려지게..
	{
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ZENABLE, FALSE );
	}


	///////////////////////////////////////////////////////////////////////
	// 폭파 그리기.
	matWorld._41 = effect_sort.vPos.x;
	matWorld._42 = effect_sort.vPos.y + (effect_sort.nCurFrame*0.2f);
	matWorld._43 = effect_sort.vPos.z;

	if (effect_sort.nCurFrame < FX_STONESHOTBOOM_FRAMECOUNT)
	{
		m_lverBoom[0].tu = m_Texcode[m_nFrameIdx[effect_sort.nCurFrame]].tu;
		m_lverBoom[0].tv = m_Texcode[m_nFrameIdx[effect_sort.nCurFrame]].tv+0.25f;
		m_lverBoom[1].tu = m_Texcode[m_nFrameIdx[effect_sort.nCurFrame]].tu;
		m_lverBoom[1].tv = m_Texcode[m_nFrameIdx[effect_sort.nCurFrame]].tv;
		m_lverBoom[2].tu = m_Texcode[m_nFrameIdx[effect_sort.nCurFrame]].tu+0.25f;
		m_lverBoom[2].tv = m_Texcode[m_nFrameIdx[effect_sort.nCurFrame]].tv+0.25f;
		m_lverBoom[3].tu = m_Texcode[m_nFrameIdx[effect_sort.nCurFrame]].tu+0.25f;
		m_lverBoom[3].tv = m_Texcode[m_nFrameIdx[effect_sort.nCurFrame]].tv;
	}

	// nate 2004 - 7 - image
	lpSurface = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_METEORITEBOOM ] ) ;
	GET_D3DDEVICE()->SetTexture(0, lpSurface);
	GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &matWorld );

	GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_LVERTEX,
									m_lverBoom, 4, 0 );

	if( effect_sort.Index == 1 ) // 이게 1이면 바닥등에 안가려지고 풀로 그려지게..
	{
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ZENABLE, TRUE ); // 다시돌려준다
	}

	if (effect_sort.nCurFrame >= FX_FIREBALLBOOM_FRAMECOUNT - 1)
		bFinished = TRUE;

	if( effect_sort.Index != 1 ) // 이게 1이면 바닥 금가는건 안표시되도록.
	{
		// nate 2004 - 7 글로벌 ShadowRect사용
		if( g_pShadowRect )
		{
			// nate 2004 - 7 - image
			lpSurface = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_METEORITEBOOM_BASE ] ) ;

			// nate 2004 - 7 글로벌 ShadowRect사용
			g_pShadowRect->SetTileCoord(effect_sort.vPos, 40, FALSE);		// 부울값은 시간인가 아닌가
			g_pShadowRect->Render(lpSurface, FALSE);			// 부울값은 리얼그림자인가 아닌가?
		}
	}

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE);

	return bFinished;
}

