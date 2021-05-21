#include "stdafx.h"
#define D3D_OVERLOADS

#include <d3d.h>
#include <time.h>
#include "FxSet.h"
#include "FxLightHeal.h"
#include "main.h"
#include "D3DMath.h"
#include "D3DUtil.h"
#include "IndexedTexture.h"
#include "WeatherMgr.h"
#include "DirtSnd.h"
#include "LoadEffectSound.h"
#include "ImageDefine.h"
#include "CapSyong.h"
#include "GUtil.h"
#include "NkCharacter.h"
#include "NkMob.h"



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFxLightHeal::CFxLightHeal()
{
	m_nTotalFrame = 20;
	m_lverRect[0] = D3DLVERTEX( D3DVECTOR(FX_LIGHTHEAL_LIGHTSIZE, -FX_LIGHTHEAL_LIGHTSIZE, 0), 0x00ffffff, 0, 1.0f, 1.0f );
	m_lverRect[1] = D3DLVERTEX( D3DVECTOR(-FX_LIGHTHEAL_LIGHTSIZE, -FX_LIGHTHEAL_LIGHTSIZE, 0), 0x00ffffff, 0, 0.0f, 1.0f );
	m_lverRect[2] = D3DLVERTEX( D3DVECTOR(-FX_LIGHTHEAL_LIGHTSIZE, FX_LIGHTHEAL_LIGHTSIZE, 0), 0x00ffffff, 0, 0.0f, 0.0f );
	m_lverRect[3] = D3DLVERTEX( D3DVECTOR( FX_LIGHTHEAL_LIGHTSIZE, FX_LIGHTHEAL_LIGHTSIZE, 0), 0x00ffffff, 0, 1.0f, 0.0f );

	m_lverRot[0] = m_lverRect[0];
	m_lverRot[1] = m_lverRect[1];
	m_lverRot[2] = m_lverRect[2];
	m_lverRot[3] = m_lverRect[3];

	m_pSparkles = new Sparkle[HEAL_MAX_SPARKLES];
	m_pIndices = new WORD[HEAL_MAX_SPARKLES*6];
	m_pMesh = new D3DLVERTEX[HEAL_MAX_SPARKLES*4];

	srand(time(0));

	// 입자가 생기는 바운드 영역 맥스값.(축의 반절값만..)
	m_vecBound = D3DVECTOR(8.0f, 15.0f, 8.0f);
}

CFxLightHeal::~CFxLightHeal()
{
	DeleteRes();
}

void CFxLightHeal::LoadRes(IndexedTexture *pIndexedTexture)
{
	InitSparkles();
}

void CFxLightHeal::InitSparkles()
{
	int i = 0;

	for (i=0; i < HEAL_MAX_SPARKLES; i++)
		m_pSparkles[i] = RandomSparkle();

	for (i=0; i < HEAL_MAX_SPARKLES; i++)
	{
		m_pIndices[i*6+0] = 4*i + 0;
		m_pIndices[i*6+1] = 4*i + 1;
		m_pIndices[i*6+2] = 4*i + 2;
		m_pIndices[i*6+3] = 4*i + 0;
		m_pIndices[i*6+4] = 4*i + 2;
		m_pIndices[i*6+5] = 4*i + 3;
	}
}

Sparkle CFxLightHeal::RandomSparkle()
{
	Sparkle ret;

	ret.position = D3DVECTOR(m_vecBound.x * (rnd()-rnd()),
							 m_vecBound.y * (rnd()-rnd()),
							 m_vecBound.z * (rnd()-rnd()));
	ret.color = D3DVECTOR(rnd(), rnd(), rnd());
	ret.velocity = D3DVECTOR(0,0,0);

	return ret;
}

void CFxLightHeal::DeleteRes()
{
	SAFE_DELETE_ARRAY(m_pSparkles);
	SAFE_DELETE_ARRAY(m_pIndices);
	SAFE_DELETE_ARRAY(m_pMesh);
}

void CFxLightHeal::FrameMove(float fTimeKey)
{}

BOOL CFxLightHeal::Render(EffectSort &effect_sort)
{
	if( !GET_D3DDEVICE() )
		return true;

	if(!effect_sort.pNkChaTo
			&& !effect_sort.pMobTo)
		return TRUE;

	BOOL bFinished = TRUE;

	static int nFrameCount = 0;
	static float g_fSaveTime = 0.0f;

	int i;

	D3DVECTOR vViewVec;
	D3DMATRIX matRx, matRy, matRyRev, matRySet, matRz, matWorld, matRot;
	LPDIRECTDRAWSURFACE7 lpSurface = NULL;

	// 회전 및 월드 트랜스폼.
	nFrameCount = effect_sort.nCurFrame;
	D3DUtil_SetRotateYMatrix(matRySet, g_2_PI * (float) nFrameCount / m_nTotalFrame);
	D3DUtil_SetRotateYMatrix(matRyRev, -g_2_PI * (float) nFrameCount / m_nTotalFrame);

	// 스파크의 갯수를 계산한다.
	int nCurSparkles = HEAL_CUR_SPARKLES;

	for (i=0; i < nFrameCount; i++)
	{
		if (nFrameCount < m_nTotalFrame/2)
			nCurSparkles = nCurSparkles + i * 2;
		else
			nCurSparkles = nCurSparkles - i * 2;

		if (nCurSparkles < 0)
			nCurSparkles = 0;
	}

	D3DUtil_SetIdentityMatrix(matWorld);

	if (effect_sort.pNkChaTo)
	{
		matWorld._41 = effect_sort.pNkChaTo->m_wx;
		matWorld._42 = effect_sort.pNkChaTo->m_wy + 8.0f;	// 높이 보정
		matWorld._43 = effect_sort.pNkChaTo->m_wz;
	}
	else if (effect_sort.pMobTo)
	{
		matWorld._41 = effect_sort.pMobTo->m_wx;
		matWorld._42 = effect_sort.pMobTo->m_wy + 8.0f;	// 높이 보정
		matWorld._43 = effect_sort.pMobTo->m_wz;
	}

	D3DMath_MatrixMultiply(matWorld, matRySet, matWorld);

	vViewVec = pCMyApp->GetEyeDirection();
	float ry = atan2f(vViewVec.x, vViewVec.z);
	float rx = atan2f(vViewVec.x*sinf(ry)+vViewVec.z*cosf(ry), vViewVec.y) - (g_PI / 2.0f) ;

	D3DUtil_SetRotateXMatrix(matRx, rx);
	D3DMath_MatrixMultiply(matRx, matRx, matRyRev);
	D3DUtil_SetRotateYMatrix(matRy, ry);
	D3DMath_MatrixMultiply(matRot, matRx, matRy);

	// 기본 사각형을 카메라를 향하게 회전.
	NKMath_LVertexMatrixMultiply(m_lverRot[0], m_lverRect[0], matRot);
	NKMath_LVertexMatrixMultiply(m_lverRot[1], m_lverRect[1], matRot);
	NKMath_LVertexMatrixMultiply(m_lverRot[2], m_lverRect[2], matRot);
	NKMath_LVertexMatrixMultiply(m_lverRot[3], m_lverRect[3], matRot);

	// 렌더링 단계 시작.
	int num = 0;
	for (i=0; i<nCurSparkles; i++)
	{
		D3DCOLOR	color = D3DRGB(m_pSparkles[i].color.x, m_pSparkles[i].color.y, m_pSparkles[i].color.z);

		D3DVECTOR position = m_pSparkles[i].position;

		for (int j=0; j < 4; j++)
		{
			m_pMesh[num*4+j].x = position.x + m_lverRot[j].x;
			m_pMesh[num*4+j].y = position.y + m_lverRot[j].y;
			m_pMesh[num*4+j].z = position.z + m_lverRot[j].z;

			m_pMesh[num*4+j].color = color;
			m_pMesh[num*4+j].specular = 0;
			m_pMesh[num*4+j].tu = m_lverRot[j].tu;
			m_pMesh[num*4+j].tv = m_lverRot[j].tv;
		}
		num++;
	}

	// 월드 트랜스폼.
	GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &matWorld );

	// nate 2004 - 7 - image
	if( g_pCapsyongTexture )
		lpSurface = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_LIGHTHEAL ] );
	GET_D3DDEVICE()->SetTexture(0, lpSurface);

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND,   D3DBLEND_ONE );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND,  D3DBLEND_ONE );

	GET_D3DDEVICE()->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,D3DFVF_LVERTEX,
										  (LPVOID)m_pMesh, nCurSparkles*4 , m_pIndices, nCurSparkles*6, 0);

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE);

	return bFinished;
}