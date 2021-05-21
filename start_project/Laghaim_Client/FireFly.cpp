#include "stdafx.h"
#define D3D_OVERLOADS
#include "d3d.h"
#include "D3DMath.h"
#include "D3DUtil.h"
#include "FireFly.h"
#include "IndexedTexture.h"
#include "LoadEffectSound.h"
// nate 2004 - 7 : Image Manager
#include "ImageDefine.h"
#include "main.h"

extern int g_EffectTextr[];



extern float FloatRandom(float from, float to);

#ifndef ABS
#define ABS(a)	((a) < 0 ? (-(a)) : (a))
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CFfObj::CFfObj(float x, float z, float h, float r)
{
	m_FfCenter = D3DVECTOR(x, h, z);
	m_dx = sinf(r) * FF_SPEED;
	m_dz = cosf(r) * FF_SPEED;
	m_Frame = 0;
	m_Next = NULL;
	m_Width = 0;
	m_Color = 0x00ffff77;
}


void CFfObj::Move()
{
	m_FfCenter.x += m_dx;
	m_FfCenter.z += m_dz;
	m_FfCenter.y += FloatRandom(-1, 1) / 10.0f;

	int time = m_Frame - (FF_LIFE/2);
	time = ABS(time);
	time = FF_LIFE / 2 - time;
	m_Width = FF_WIDTH * time / FF_LIFE / 2;

	m_Frame++;
}

CFireFly::CFireFly()
{
	m_FfList = NULL;
	m_LastTime = timeGetTime();
	D3DUtil_SetIdentityMatrix(m_matId);
}

CFireFly::~CFireFly()
{
	CFfObj *pFfObj = m_FfList;
	while (pFfObj)
	{
		CFfObj *pNextFfObj = pFfObj->m_Next;
		delete pFfObj;
		pFfObj = pNextFfObj;
	}
}

void CFireFly::AddFf()
{
	float x = FloatRandom(-FF_XZ_MAX, FF_XZ_MAX);
	float z = FloatRandom(-FF_XZ_MAX, FF_XZ_MAX);
	float h = FloatRandom(FF_H_MIN, FF_H_MAX);
	float r = FloatRandom(0, g_PI*2);

	CFfObj *pFfObj = new CFfObj(x, z, h, r);
	if( pFfObj )
		pFfObj->m_Next = m_FfList;
	m_FfList = pFfObj;
}

CFfObj* CFireFly::DelFf(CFfObj *pPrevFfObj, CFfObj *pFfObj)
{
	if (pPrevFfObj)
	{
		pPrevFfObj->m_Next = pFfObj->m_Next;
		delete pFfObj;
		return pPrevFfObj->m_Next;
	}
	else
	{
		// Root Node ÀÏ °æ¿ì...
		if( pFfObj )
		{
			m_FfList = pFfObj->m_Next;
			delete pFfObj;
		}
		return m_FfList;
	}
}

void CFireFly::Render()
{
	if( !GET_D3DDEVICE() )
		return;

	D3DLVERTEX lvertex[4];
	D3DVECTOR centerVector;
	CFfObj *pFfObj = m_FfList, *pPrevFfObj = NULL;
	DWORD curTime = timeGetTime();
	if (curTime - m_LastTime >= FF_RATE)
	{
		m_LastTime = curTime;
		while (pFfObj)
		{
			if (pFfObj->m_Frame > FF_LIFE)
			{
				pFfObj = DelFf(pPrevFfObj, pFfObj);
				continue;
			}
			if( pFfObj )
				pFfObj->Move();

			pPrevFfObj = pFfObj;
			if( pFfObj )
				pFfObj = pFfObj->m_Next;
		}
		AddFf();
	}
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, TRUE);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHATESTENABLE, FALSE);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA);

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE );
	GET_D3DDEVICE()->GetTransform( D3DTRANSFORMSTATE_WORLD, &m_matWorld);
	GET_D3DDEVICE()->GetTransform( D3DTRANSFORMSTATE_VIEW, &m_matView);
	GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &m_matId);
	GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_VIEW, &m_matId);

	D3DMATRIX matTrans;
	D3DMath_MatrixMultiply(matTrans, m_matWorld, m_matView);

	LPDIRECTDRAWSURFACE7 lpTexture = NULL;
	if( g_pCapsyongTexture )
		lpTexture = g_pCapsyongTexture->GetSurfaceByIdx(g_EffectTextr[EFF_FIREFLY]);

	GET_D3DDEVICE()->SetTexture(0, lpTexture);
	pFfObj = m_FfList;
	while (pFfObj)
	{
		D3DMath_VectorMatrixMultiply(centerVector, pFfObj->m_FfCenter, matTrans);
		lvertex[0] = D3DLVERTEX(D3DVECTOR(centerVector.x - pFfObj->m_Width,
										  centerVector.y - pFfObj->m_Width,
										  centerVector.z),
								pFfObj->m_Color, 0L, 0, 1);
		lvertex[1] = D3DLVERTEX(D3DVECTOR(centerVector.x - pFfObj->m_Width,
										  centerVector.y + pFfObj->m_Width,
										  centerVector.z),
								pFfObj->m_Color, 0L, 0, 0);
		lvertex[2] = D3DLVERTEX(D3DVECTOR(centerVector.x + pFfObj->m_Width,
										  centerVector.y - pFfObj->m_Width,
										  centerVector.z),
								pFfObj->m_Color, 0L, 1, 1);
		lvertex[3] = D3DLVERTEX(D3DVECTOR(centerVector.x + pFfObj->m_Width,
										  centerVector.y + pFfObj->m_Width,
										  centerVector.z),
								pFfObj->m_Color, 0L, 1, 0);
		GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP , D3DFVF_LVERTEX ,
										lvertex, 4, NULL );

		pFfObj = pFfObj->m_Next;
	}
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, FALSE);
	GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &m_matWorld);
	GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_VIEW, &m_matView);
}
