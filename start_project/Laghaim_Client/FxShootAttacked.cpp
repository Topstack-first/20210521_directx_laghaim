#include "stdafx.h"
#define D3D_OVERLOADS

#include <d3d.h>
#include "FxSet.h"
#include "FxShootAttacked.h"
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

#define FX_SHOOTATTACKED_SIZE 4.0f

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFxShootAttacked::CFxShootAttacked()
{
	m_lverRect[0] = D3DLVERTEX( D3DVECTOR(-FX_SHOOTATTACKED_SIZE, -FX_SHOOTATTACKED_SIZE, 0), 0xff559d6e, 0, 0.0f, 1.0f );
	m_lverRect[1] = D3DLVERTEX( D3DVECTOR(-FX_SHOOTATTACKED_SIZE, FX_SHOOTATTACKED_SIZE, 0), 0xff559d6e, 0, 0.0f, 0.0f );
	m_lverRect[2] = D3DLVERTEX( D3DVECTOR( FX_SHOOTATTACKED_SIZE, -FX_SHOOTATTACKED_SIZE, 0), 0xff559d6e, 0, 0.25f, 1.0f );
	m_lverRect[3] = D3DLVERTEX( D3DVECTOR( FX_SHOOTATTACKED_SIZE, FX_SHOOTATTACKED_SIZE, 0), 0xff559d6e, 0, 0.25f, 0.0f );

	m_lverRect[4] = D3DLVERTEX( D3DVECTOR(-FX_SHOOTATTACKED_SIZE, -FX_SHOOTATTACKED_SIZE, 0), 0xff559d6e, 0, 0.25f, 1.0f );
	m_lverRect[5] = D3DLVERTEX( D3DVECTOR(-FX_SHOOTATTACKED_SIZE, FX_SHOOTATTACKED_SIZE, 0), 0xff559d6e, 0, 0.25f, 0.0f );
	m_lverRect[6] = D3DLVERTEX( D3DVECTOR( FX_SHOOTATTACKED_SIZE, -FX_SHOOTATTACKED_SIZE, 0), 0xff559d6e, 0, 0.5f, 1.0f );
	m_lverRect[7] = D3DLVERTEX( D3DVECTOR( FX_SHOOTATTACKED_SIZE, FX_SHOOTATTACKED_SIZE, 0), 0xff559d6e, 0, 0.5f, 0.0f );

	m_lverRect[8] = D3DLVERTEX( D3DVECTOR(-FX_SHOOTATTACKED_SIZE, -FX_SHOOTATTACKED_SIZE, 0), 0xff559d6e, 0, 0.5f, 1.0f );
	m_lverRect[9] = D3DLVERTEX( D3DVECTOR(-FX_SHOOTATTACKED_SIZE, FX_SHOOTATTACKED_SIZE, 0), 0xff559d6e, 0, 0.5f, 0.0f );
	m_lverRect[10] = D3DLVERTEX( D3DVECTOR( FX_SHOOTATTACKED_SIZE, -FX_SHOOTATTACKED_SIZE, 0), 0xff559d6e, 0, 0.75f, 1.0f );
	m_lverRect[11] = D3DLVERTEX( D3DVECTOR( FX_SHOOTATTACKED_SIZE, FX_SHOOTATTACKED_SIZE, 0), 0xff559d6e, 0, 0.75f, 0.0f );

	m_lverRect[12] = D3DLVERTEX( D3DVECTOR(-FX_SHOOTATTACKED_SIZE, -FX_SHOOTATTACKED_SIZE, 0), 0xff559d6e, 0, 0.75f, 1.0f );
	m_lverRect[13] = D3DLVERTEX( D3DVECTOR(-FX_SHOOTATTACKED_SIZE, FX_SHOOTATTACKED_SIZE, 0), 0xff559d6e, 0, 0.75f, 0.0f );
	m_lverRect[14] = D3DLVERTEX( D3DVECTOR( FX_SHOOTATTACKED_SIZE, -FX_SHOOTATTACKED_SIZE, 0), 0xff559d6e, 0, 1.0f, 1.0f );
	m_lverRect[15] = D3DLVERTEX( D3DVECTOR( FX_SHOOTATTACKED_SIZE, FX_SHOOTATTACKED_SIZE, 0), 0xff559d6e, 0, 1.0f, 0.0f );
}

CFxShootAttacked::~CFxShootAttacked()
{
	DeleteRes();
}

void CFxShootAttacked::LoadRes(IndexedTexture *pIndexedTexture)
{}

void CFxShootAttacked::DeleteRes()
{}

void CFxShootAttacked::Render(EffectSort &effect_sort)
{
	if( !GET_D3DDEVICE() )
		return;

	D3DVECTOR vViewVec;
	D3DMATRIX matRx, matRy, matRz, matWorld;
	LPDIRECTDRAWSURFACE7 lpSurface = NULL;

	D3DMATRIX ViewMatrix;
	// this guarantees a clean Matrix before doing operations with it
	D3DUtil_SetIdentityMatrix( ViewMatrix );
	// get the ViewMatrix from D3D
	GET_D3DDEVICE()->GetTransform( D3DTRANSFORMSTATE_VIEW, &ViewMatrix );

	D3DMATRIX InvViewMatrix;
	// this guarantees a clean Matrix before doing operations with it
	D3DUtil_SetIdentityMatrix( InvViewMatrix );
	// get the Inverse of the ViewMatrix, kinda like your reflection looking at you in the mirror
	D3DMath_MatrixInvert( InvViewMatrix, ViewMatrix );

	// 휴먼 타격효과 2배로 크게 보기
	D3DMATRIX		matS;
	D3DUtil_SetScaleMatrix(matS, 2.0f, 2.0f, 1.0f);
	D3DMath_MatrixMultiply(InvViewMatrix, matS, InvViewMatrix);

	// set the translation portion of the InvViewMatrix to the location of the particles in world space
	InvViewMatrix._41 = effect_sort.vPos.x;
	InvViewMatrix._42 = effect_sort.vPos.y;
	InvViewMatrix._43 = effect_sort.vPos.z;

	GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &InvViewMatrix );

	// nate 2004 - 7 - image
	if( g_pCapsyongTexture )
		lpSurface = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_SHOOTATTACKED ] );

	GET_D3DDEVICE()->SetTexture(0, lpSurface);

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND,   D3DBLEND_SRCALPHA );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND,  D3DBLEND_INVSRCALPHA );

	int nFrame = effect_sort.nCurFrame;

	if (nFrame < 0)
		nFrame = 0;
	if (nFrame > 3)
		nFrame = 3;

	m_lverRect[nFrame*4].color = effect_sort.dwColor;
	m_lverRect[nFrame*4+1].color = effect_sort.dwColor;
	m_lverRect[nFrame*4+2].color = effect_sort.dwColor;
	m_lverRect[nFrame*4+3].color = effect_sort.dwColor;

	GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_LVERTEX, &m_lverRect[nFrame*4], 4, 0 );

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE);
}


