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
// nate 2004 - 7 - image
#include "ImageDefine.h"
#include "NkCharacter.h"
#include "NkMob.h"



extern BOOL g_bDungeon;
// nate 2004 - 7 글로벌 ShadowRect사용


// nate 2004 - 8 ImageManager
#define FX_NULCLEARBOOM_SIZE 50.0f

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFxNuclearBoom::CFxNuclearBoom()
	: m_fScale(0)
	, m_fFade(0)
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

	m_fBaseSize[0] = 70.0f;
	m_fBaseSize[1] = 70.0f;
	m_fBaseSize[2] = 75.0f;
	m_fBaseSize[3] = 75.0f;
	m_fBaseSize[4] = 75.0f;

	m_fBaseSize[5] = 87.0f;
	m_fBaseSize[6] = 87.0f;
	m_fBaseSize[7] = 87.0f;
	m_fBaseSize[8] = 85.0f;
	m_fBaseSize[9] = 85.0f;

	m_fBaseSize[10] = 90.0f;
	m_fBaseSize[11] = 90.0f;
	m_fBaseSize[12] = 90.0f;
	m_fBaseSize[13] = 87.0f;
	m_fBaseSize[14] = 87.0f;

	m_fBaseSize[15] = 75.0f;
	m_fBaseSize[16] = 60.0f;
	m_fBaseSize[17] = 50.0f;
	m_fBaseSize[18] = 40.0f;
	m_fBaseSize[19] = 30.0f;

	sphere				= NULL;
	light 				= NULL;
	wave				= NULL;
	wave2				= NULL;

	Create();
}

CFxNuclearBoom::~CFxNuclearBoom()
{
	SAFE_DELETE(sphere);
	SAFE_DELETE(light);
	SAFE_DELETE(wave);
	SAFE_DELETE(wave2);

	DeleteRes();
}

void CFxNuclearBoom::Create()
{
	SAFE_DELETE(sphere);
	SAFE_DELETE(light);
	SAFE_DELETE(wave);
	SAFE_DELETE(wave2);

	sphere = new CSphere;
	if( sphere )
		sphere->Create();

	light = new CRectPlane;
	if( light )
		light->Create( 20.0f, 20.0f, FALSE );

	wave = new CCylinderPlane;
	if( wave )
		wave->Create( 8.0f, 4.0f, 2.0f, 20, TRUE );
	wave2 = new CCylinderPlane;
	if( wave2 )
		wave2->Create( 2.0f, 10.0f, 0.0f, 20, FALSE );
}

void CFxNuclearBoom::LoadRes(IndexedTexture *pIndexedTexture, CShadowRect *pShadowRect)
{}

void CFxNuclearBoom::DeleteRes()
{}

BOOL CFxNuclearBoom::Render(EffectSort &effect_sort)
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
	matWorld._42 = vPosTo.y + effect_sort.nCurFrame + 10.0f;
	matWorld._43 = vPosTo.z;

	// nate 2004 - 8 ImageManager
	MakeLVertexRect(m_lverBoom, FX_NULCLEARBOOM_SIZE, FX_NULCLEARBOOM_SIZE, TRUE);

	if (effect_sort.nCurFrame < FX_NUCLEARBOOM_FRAMECOUNT)
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
	lpSurface = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_NUCLEARBOOM ] ) ;
	GET_D3DDEVICE()->SetTexture(0, lpSurface);
	GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &matWorld );

	GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_LVERTEX,
									m_lverBoom, 4, 0 );

	float maxScale = 3.0f;

	m_fScale = (maxScale * effect_sort.nCurFrame) / FX_NUCLEARBOOM_FRAMECOUNT;
	m_fFade = 1.0f - (m_fScale * 0.3f);

	DWORD  dwTextureState[3];

	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG1, &dwTextureState[0] );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG2, &dwTextureState[1] );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAOP,   &dwTextureState[2] );

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );

	// nate 2004 - 7 - image
	lpSurface = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_NUCLEARBOOM_DUST ] ) ;

	sphere->SetColor(m_fFade * 0.25f, m_fFade * 0.25f, m_fFade * 0.25f, m_fFade * 0.25f);
	sphere->SetScaling(m_fScale * 30.0f, m_fScale * 20.0f, m_fScale * 30.0f);
	sphere->SetTranslationXYZ(vPosTo.x, vPosTo.y, vPosTo.z);
	sphere->Render();

	float red, green, blue;
	red = 0.9f - (0.9f * m_fScale * 0.3f);
	green = 0.9f - (0.9f * m_fScale * 0.3f);
	blue = 0.49f - (0.49f * m_fScale * 0.3f);

	if( light )
	{
		// nate 2004 - 7 - image
		GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_NUCLEARBOOM_LIGHT ] ) );

		light->SetColor(red * 0.35f, green * 0.35f, blue * 0.35f, m_fFade * 0.35f);
		light->SetRotationX( g_PI * 0.5f );
		light->RotateZ( m_fScale * 5.0f );
		light->Scale( m_fScale*2.5f + 1.0f, 1.0f, m_fScale*2.5f + 1.0f );
		light->SetTranslationXYZ(vPosTo.x, vPosTo.y + 10.5f, vPosTo.z);
		light->Render();
	}

	// nate 2004 - 7 - image
	GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_NUCLEARBOOM_WAVE ] ) );

	if( wave )
	{
		wave->SetColor(red * 0.5f, green * 0.5f, blue * 0.5f, m_fFade * 0.5f);
		wave->SetRotationY( g_PI * 0.5f );
		wave->RotateY( m_fScale * 7.0f );
		wave->Scale( m_fScale*3.5f + 1.0f, 1.0f, m_fScale*3.5f + 1.0f );
		wave->SetTranslationXYZ(vPosTo.x, vPosTo.y + 5.5f, vPosTo.z);
		wave->Render();
	}

	if( wave2 )
	{
		wave2->SetColor(red * 0.35f, green * 0.35f, blue * 0.35f, m_fFade * 0.35f);
		wave2->SetRotationY( g_PI * 0.5f );
		wave2->Scale( m_fScale*5.0f + 1.0f, 1.0f, m_fScale*5.0f + 1.0f );
		wave2->SetTranslationXYZ(vPosTo.x, vPosTo.y + 5.5f, vPosTo.z);
		wave2->Render();
	}

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState[2] );

	// nate 2004 - 7 글로벌 ShadowRect사용
	if( g_pShadowRect )
	{
		// nate 2004 - 7 - image
		lpSurface = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_NUCLEARBOOM_BASE ] ) ;

		if (g_bDungeon)
			// nate 2004 - 7 글로벌 ShadowRect사용
			g_pShadowRect->SetTileCoord(vPosTo, m_fBaseSize[effect_sort.nCurFrame]*0.6, FALSE);		// 부울값은 시간인가 아닌가
		else
			// nate 2004 - 7 글로벌 ShadowRect사용
			g_pShadowRect->SetTileCoord(vPosTo, m_fBaseSize[effect_sort.nCurFrame], FALSE);		// 부울값은 시간인가 아닌가

		// nate 2004 - 7 글로벌 ShadowRect사용
		g_pShadowRect->Render(lpSurface, FALSE);	// 부울값은 리얼그림자인가 아닌가?
	}

	if (effect_sort.nCurFrame >= FX_NUCLEARBOOM_FRAMECOUNT - 1)
	{
		return TRUE;
	}

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE);

	return bFinished;
}