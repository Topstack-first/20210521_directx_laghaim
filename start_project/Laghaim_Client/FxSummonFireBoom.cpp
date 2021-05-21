#include "stdafx.h"
// FxSummonFireBoom.cpp: implementation of the CFxSummonFireBoom class.
//
//////////////////////////////////////////////////////////////////////
#define D3D_OVERLOADS

#include <d3d.h>
#include <time.h>
#include <assert.h>
#include "FxSet.h"
#include "FxAirRaid.h"
#include "main.h"
#include "D3DMath.h"
#include "D3DUtil.h"
#include "IndexedTexture.h"
#include "WeatherMgr.h"
#include "DirtSnd.h"
#include "LoadEffectSound.h"
#include "CapSyong.h"
#include "GUtil.h"
#include "NkCharacter.h"
#include "NkMob.h"
#include "ImageDefine.h"
#include "FxSet.h"
#include "ShadowRect.h"
#include "FxSummonFireBoom.h"


extern int g_EffectSound[];


extern BOOL	g_bDungeon;
extern float g_fTimeKey;
extern float g_fDSDistance;
// nate 2004 - 7 �۷ι� ShadowRect���

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFxSummonFireBoom::CFxSummonFireBoom()
{
	m_fSpeed = 8.0f;
	m_fBoomSize = 50.0f;

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
}

CFxSummonFireBoom::~CFxSummonFireBoom()
{
	DeleteRes();
}

void CFxSummonFireBoom::LoadRes(IndexedTexture *pIndexedTexture, CShadowRect *pShadowRect)
{}

void CFxSummonFireBoom::DeleteRes()
{}

BOOL CFxSummonFireBoom::Render(D3DVECTOR &vPosFrom, D3DVECTOR &vPosTo, int nFrame)
{
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

	///////////////////////////////////////////////////////////////////////
	// �ܻ� �׸���.
	// ���⺤�͸� ���Ѵ�.
	D3DVECTOR vDir;
	D3DVECTOR vDrawPos;
	BOOL bDrawTail = TRUE;
	int start_frame, end_frame;
	int nFrameDist;
	float fDist;

	// ��� ���� Ÿ���������� �Ÿ��� 7������ �̻��� �Ǵ°� Ȯ���Ѵ�.
	// �� �����ܻ� �׸��� �ҽ��������� �� �׷����� ��������..
	fDist = DistVecToVec(vPosFrom, vPosTo);

	// �� ������ �Ÿ��ΰ� Ȯ���� �Ѵ�.
	assert(m_fSpeed != 0.0f);
	nFrameDist = (int) (fDist / m_fSpeed);		// src�� dest���� �����Ӱ� �Ÿ�.

	start_frame = 0;
	end_frame = 7 - nFrame;	// 7�� ���ܻ��� ������ 1�� �� ��.

	if (end_frame < 0)
		bDrawTail = FALSE;

	if (nFrameDist < end_frame)
		end_frame = nFrameDist;

	get_normal_vector(vDir, vPosFrom, vPosTo);

	// �����Ӵ� ���ǵ�.
	vDir *= m_fSpeed;

	///////////////////////////////////////////////////////////////////////
	// ���� �׸���.
	matWorld._41 = vPosTo.x;
	matWorld._42 = vPosTo.y + FX_FIREBALL_HEIGHT;
	matWorld._43 = vPosTo.z;

	MakeLVertexRect(m_lverBoom, m_fBoomSize, m_fBoomSize, TRUE);

	if (nFrame < FX_FIREBALLBOOM_FRAMECOUNT)
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
	lpSurface = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_FIREBALLBOOM2 ] );
	GET_D3DDEVICE()->SetTexture(0, lpSurface);

	GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &matWorld );

	GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_LVERTEX, m_lverBoom, 4, 0 );

	if (nFrame >= FX_FIREBALLBOOM_FRAMECOUNT - 1)
		bFinished = TRUE;

	// nate 2004 - 7 �۷ι� ShadowRect���
	if( g_pShadowRect )
	{
		// nate 2004 - 7 - image
		lpSurface = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_FIREBALLBOOM2BASE ] );

		if (g_bDungeon)
			// nate 2004 - 7 �۷ι� ShadowRect���
			g_pShadowRect->SetTileCoord(vPosTo, m_fBaseSize[nFrame]*0.6, FALSE);		// �οﰪ�� �ð��ΰ� �ƴѰ�
		else
			// nate 2004 - 7 �۷ι� ShadowRect���
			g_pShadowRect->SetTileCoord(vPosTo, m_fBaseSize[nFrame], FALSE);		// �οﰪ�� �ð��ΰ� �ƴѰ�

		// nate 2004 - 7 �۷ι� ShadowRect���
		g_pShadowRect->Render(lpSurface, FALSE);	// �οﰪ�� ����׸����ΰ� �ƴѰ�?
	}

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE);

	return bFinished;
}
