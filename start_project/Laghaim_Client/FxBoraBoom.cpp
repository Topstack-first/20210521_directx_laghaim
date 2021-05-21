#include "stdafx.h"
#define D3D_OVERLOADS
#include <d3d.h>
#include "FxSet.h"
#include "FxBora.h"
#include "FxBoraBoom.h"
#include "ShadowRect.h"
#include "main.h"
#include "Capsyong.h"
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
extern float g_fDSDistance;

extern int g_EffectSound[];

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFxBoraBoom::CFxBoraBoom()
{
	m_fTailSize = 3.0f;					// fireball�� ���� �ܻ� �ؽ�ó�̴�.
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

CFxBoraBoom::~CFxBoraBoom()
{
	DeleteRes();
}

void CFxBoraBoom::LoadRes(IndexedTexture *pIndexedTexture, CShadowRect *pShadowRect)
{}

void CFxBoraBoom::DeleteRes()
{}

BOOL CFxBoraBoom::Render(D3DVECTOR &vPosFrom, D3DVECTOR &vPosTo, int nFrame)
{
	if( !GET_D3DDEVICE() )
		return true;

	LPDIRECTDRAWSURFACE7 lpSurface = NULL;
	D3DVECTOR vViewVec;
	D3DMATRIX matRx, matRy, matWorld;
	int i = 0;
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

	// �ؽ�ó ���ϱ�.
	if (bDrawTail)
	{
		// nate 2004 - 7 - image
		if( g_pCapsyongTexture )
			lpSurface = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_BORABOOMTAIL ] );
		GET_D3DDEVICE()->SetTexture(0, lpSurface);

		// ���⼭ start_frame�� ȭ�̾�� ���� �κ��� ����.
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
	// ���� �׸���.

	matWorld._41 = vPosTo.x;
	matWorld._42 = vPosTo.y + FX_BORA_HEIGHT;
	matWorld._43 = vPosTo.z;

	MakeLVertexRect(m_lverBoom, m_fBoomSize, m_fBoomSize, TRUE);

	if (nFrame < FX_BORABOOM_FRAMECOUNT)
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
		lpSurface = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_BORABOOM ] );
	GET_D3DDEVICE()->SetTexture(0, lpSurface);

	GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &matWorld );

	GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_LVERTEX,
									m_lverBoom, 4, 0 );

	if (nFrame >= FX_BORABOOM_FRAMECOUNT - 1)
		bFinished = TRUE;

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE);

	return bFinished;
}
