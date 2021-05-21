#include "stdafx.h"
#define D3D_OVERLOADS
#include <d3d.h>
#include "FxSet.h"
#include "FxBee.h"
#include "FxBeeBoom.h"
#include "ShadowRect.h"
#include "main.h"
#include "Capsyong.h"
#include "IndexedTexture.h"
#include "ShadowRect.h"
#include "GUtil.h"
#include "DirtSnd.h"
#include "LoadEffectSound.h"
// nate 2004 - 7 - image
#include "ImageDefine.h"
#include "NkCharacter.h"
#include "NkMob.h"
#include "D3DMath.h"


extern float g_fTimeKey;
extern float g_fDSDistance;

extern int g_EffectSound[];

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFxBeeBoom::CFxBeeBoom()
{
	m_fTailSize = 3.0f;					// fireball의 꼬리 잔상 텍스처이다.
	m_fSpeed = 8.0f;
	m_fBoomSize = m_fTailSize * 10.0f;

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
}

CFxBeeBoom::~CFxBeeBoom()
{
	DeleteRes();
}

void CFxBeeBoom::LoadRes(IndexedTexture *pIndexedTexture, CShadowRect *pShadowRect)
{}
void CFxBeeBoom::DeleteRes()
{}

BOOL CFxBeeBoom::Render(D3DVECTOR &vPosFrom, D3DVECTOR &vPosTo, int nFrame)
{
	if( !GET_D3DDEVICE() )
		return TRUE;

	LPDIRECTDRAWSURFACE7 lpSurface = NULL;
	D3DVECTOR vViewVec;
	D3DMATRIX matRx, matRy, matWorld;
	int i;
	BOOL bFinished = FALSE;

	vViewVec = pCMyApp->GetEyeDirection();

	float ry = atan2f(vViewVec.x, vViewVec.z);
	float rx = atan2f(vViewVec.x*sinf(ry)+vViewVec.z*cosf(ry), vViewVec.y) - (g_PI / 2.0f) ;

	D3DUtil_SetRotateXMatrix(matRx, rx);
	D3DUtil_SetRotateYMatrix(matRy, ry);
	D3DMath_MatrixMultiply(matWorld, matRx, matRy);

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND,   D3DBLEND_SRCALPHA );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND,  D3DBLEND_INVSRCALPHA );

	///////////////////////////////////////////////////////////////////////
	// 잔상 그리기.

	// 방향벡터를 구한다.
	D3DVECTOR vDir;
	D3DVECTOR vDrawPos;
	BOOL bDrawTail = TRUE;
	int start_frame, end_frame;
	int nFrameDist;
	float fDist;

	// 출발 점과 타겟지점과의 거리가 7프레임 이상이 되는가 확인한다.
	// 왜 꼬리잔상 그림이 소스지점보다 더 그려지지 않을려고..
	fDist = DistVecToVec(vPosFrom, vPosTo);
	// 몇 프레임 거리인가 확인을 한다.
	nFrameDist = (int) (fDist / m_fSpeed);		// src와 dest와의 프레임간 거리.

	start_frame = 0;
	end_frame = 7 - nFrame;	// 7은 총잔상의 수에서 1을 뺀 것.
	if (end_frame < 0)
		bDrawTail = FALSE;

	if (nFrameDist < end_frame)
		end_frame = nFrameDist;

	get_normal_vector(vDir, vPosFrom, vPosTo);

	// 프레임당 스피드.
	vDir *= m_fSpeed;

	// 텍스처 택하기.
	if (bDrawTail)
	{
		// nate 2004 - 7 - image
		if( g_pCapsyongTexture )
			lpSurface = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_BEEBOOMTAIL ] );
		GET_D3DDEVICE()->SetTexture(0, lpSurface);

		// 여기서 start_frame은 화이어볼의 꼬리 부분의 앞쪽.
		int nTex = nFrame;
		for (i= start_frame; i < end_frame; i++)
		{
			vDrawPos.x = vPosTo.x - vDir.x * i;
			vDrawPos.y = vPosTo.y - vDir.y * i;
			vDrawPos.z = vPosTo.z - vDir.z * i;

			matWorld._41 = vDrawPos.x;
			matWorld._42 = vDrawPos.y + FX_FIREBALL_HEIGHT;
			matWorld._43 = vDrawPos.z;

			GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &matWorld );

			MakeLVertexRect(m_lverTail, m_fTailSize, m_fTailSize, TRUE);

			m_lverTail[0].tu = nTex * 0.125f;
			m_lverTail[1].tu = nTex * 0.125f;
			m_lverTail[2].tu = (nTex+1) * 0.125f;
			m_lverTail[3].tu = (nTex+1) * 0.125f;
			GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_LVERTEX,
											m_lverTail, 4, 0 );
			nTex++;
		}
	}

	///////////////////////////////////////////////////////////////////////
	// 폭파 그리기.

	matWorld._41 = vPosTo.x;
	matWorld._42 = vPosTo.y + FX_BEE_HEIGHT;
	matWorld._43 = vPosTo.z;

	MakeLVertexRect(m_lverBoom, m_fBoomSize, m_fBoomSize, TRUE);

	if (nFrame < FX_BEEBOOM_FRAMECOUNT)
	{
		m_lverBoom[0].tu = m_Texcode[m_nFrameIdx[nFrame]].tu;
		m_lverBoom[0].tv = m_Texcode[m_nFrameIdx[nFrame]].tv+0.25f;
		m_lverBoom[1].tu = m_Texcode[m_nFrameIdx[nFrame]].tu;
		m_lverBoom[1].tv = m_Texcode[m_nFrameIdx[nFrame]].tv;
		m_lverBoom[2].tu = m_Texcode[m_nFrameIdx[nFrame]].tu+0.25f;
		m_lverBoom[2].tv = m_Texcode[m_nFrameIdx[nFrame]].tv+0.25f;
		m_lverBoom[3].tu = m_Texcode[m_nFrameIdx[nFrame]].tu+0.25f;
		m_lverBoom[3].tv = m_Texcode[m_nFrameIdx[nFrame]].tv;
	}

	// nate 2004 - 7 - image
	if( g_pCapsyongTexture )
		lpSurface = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_BEEBOOM ] );
	GET_D3DDEVICE()->SetTexture(0, lpSurface);
	GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &matWorld );

	GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_LVERTEX,
									m_lverBoom, 4, 0 );

	if (nFrame >= FX_BEEBOOM_FRAMECOUNT - 1)
		bFinished = TRUE;

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE);

	return bFinished;
}