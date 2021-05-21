#include "stdafx.h"
#define D3D_OVERLOADS

#include <d3d.h>
#include "FxSet.h"
#include "FxShoot.h"
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

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFxShoot::CFxShoot()
{
	m_fLightSize[0] = 4.8f;
	m_fLightSize[1] = 5.5f;
	m_fLightSize[2] = 4.5f;
	m_lverRect[0] = D3DLVERTEX( D3DVECTOR(-m_fLightSize[0], -m_fLightSize[0], 0), 0xffffffff, 0, 0.0f, 1.0f );
	m_lverRect[1] = D3DLVERTEX( D3DVECTOR(-m_fLightSize[0], m_fLightSize[0], 0), 0xffffffff, 0, 0.0f, 0.0f );
	m_lverRect[2] = D3DLVERTEX( D3DVECTOR( m_fLightSize[0], -m_fLightSize[0], 0), 0xffffffff, 0, 1.0f, 1.0f );
	m_lverRect[3] = D3DLVERTEX( D3DVECTOR( m_fLightSize[0], m_fLightSize[0], 0), 0xffffffff, 0, 1.0f, 0.0f );
}

CFxShoot::~CFxShoot()
{}

void CFxShoot::LoadRes(IndexedTexture *pIndexedTexture)
{}

void CFxShoot::DeleteRes()
{}

void CFxShoot::FrameMove()
{}

void CFxShoot::Render(EffectSort &effect_sort)
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

	// set the translation portion of the InvViewMatrix to the location of the particles in world space
	InvViewMatrix._41 = effect_sort.vPos.x;
	InvViewMatrix._42 = effect_sort.vPos.y;
	InvViewMatrix._43 = effect_sort.vPos.z;

	GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &InvViewMatrix );

	// nate 2004 - 7 - image
	lpSurface = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_SHOOT ] );
	GET_D3DDEVICE()->SetTexture(0, lpSurface);

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND,   D3DBLEND_SRCALPHA );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND,  D3DBLEND_INVSRCALPHA );

	int nFrame = effect_sort.nCurFrame;
	if (nFrame < 0)
		nFrame = 0;
	if (nFrame > 2)
		nFrame = 2;

	MakeLVertexRect(m_lverRect, m_fLightSize[nFrame], m_fLightSize[nFrame], TRUE);

	m_lverRect[0].color = effect_sort.dwColor;
	m_lverRect[1].color = effect_sort.dwColor;
	m_lverRect[2].color = effect_sort.dwColor;
	m_lverRect[3].color = effect_sort.dwColor;
	GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_LVERTEX,
									m_lverRect, 4, 0 );

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE);
}
