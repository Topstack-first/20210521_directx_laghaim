#include "stdafx.h"
#define D3D_OVERLOADS
#include <d3d.h>
#include "FxSet.h"
#include "FxAidiaFire.h"
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
#include "D3DMath.h"



extern float g_fTimeKey;
extern float g_fDSDistance;

extern int g_EffectSound[];

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFxAidiaFire::CFxAidiaFire()
{
	m_fSize = 10.0f;
	m_fSpeed = 6.0f;

	m_fLHeight = 16.0f;
	m_fLWidth = m_fLHeight / 2.0f;

	// 바위.
	m_lverRect[0] = D3DLVERTEX( D3DVECTOR(-m_fLWidth, 0, 0), 0xffffffff, 0, 0.0f, 1.0f );
	m_lverRect[1] = D3DLVERTEX( D3DVECTOR(-m_fLWidth, m_fLHeight, 0), 0xffffffff, 0, 0.0f, 0.0f );
	m_lverRect[2] = D3DLVERTEX( D3DVECTOR( m_fLWidth, 0, 0), 0xffffffff, 0, 0.25f, 1.0f );
	m_lverRect[3] = D3DLVERTEX( D3DVECTOR( m_fLWidth, m_fLHeight, 0), 0xffffffff, 0, 0.25f, 0.0f );

	m_lverRect[4] = D3DLVERTEX( D3DVECTOR(-m_fLWidth, 0, 0), 0xffffffff, 0, 0.25f, 1.0f );
	m_lverRect[5] = D3DLVERTEX( D3DVECTOR(-m_fLWidth, m_fLHeight, 0), 0xffffffff, 0, 0.25f, 0.0f );
	m_lverRect[6] = D3DLVERTEX( D3DVECTOR( m_fLWidth, 0, 0), 0xffffffff, 0, 0.5f, 1.0f );
	m_lverRect[7] = D3DLVERTEX( D3DVECTOR( m_fLWidth, m_fLHeight, 0), 0xffffffff, 0, 0.5f, 0.0f );

	m_lverRect[8] = D3DLVERTEX( D3DVECTOR(-m_fLWidth, 0, 0), 0xffffffff, 0, 0.5f, 1.0f );
	m_lverRect[9] = D3DLVERTEX( D3DVECTOR(-m_fLWidth, m_fLHeight, 0), 0xffffffff, 0, 0.5f, 0.0f );
	m_lverRect[10] = D3DLVERTEX( D3DVECTOR( m_fLWidth, 0, 0), 0xffffffff, 0, 0.75f, 1.0f );
	m_lverRect[11] = D3DLVERTEX( D3DVECTOR( m_fLWidth, m_fLHeight, 0), 0xffffffff, 0, 0.75f, 0.0f );

	m_lverRect[12] = D3DLVERTEX( D3DVECTOR(-m_fLWidth, 0, 0), 0xffffffff, 0, 0.75f, 1.0f );
	m_lverRect[13] = D3DLVERTEX( D3DVECTOR(-m_fLWidth, m_fLHeight, 0), 0xffffffff, 0, 0.75f, 0.0f );
	m_lverRect[14] = D3DLVERTEX( D3DVECTOR( m_fLWidth, 0, 0), 0xffffffff, 0, 1.0f, 1.0f );
	m_lverRect[15] = D3DLVERTEX( D3DVECTOR( m_fLWidth, m_fLHeight, 0), 0xffffffff, 0, 1.0f, 0.0f );

	float fWidth = 0.0f, fHeight = 0.0f;
	fWidth = m_fLWidth;
	fHeight = m_fLHeight;

	m_lverRectSmoke[0] = D3DLVERTEX( D3DVECTOR(-fWidth, 0, 0), 0xffffffff, 0, 0.0f, 1.0f );
	m_lverRectSmoke[1] = D3DLVERTEX( D3DVECTOR(-fWidth, fHeight, 0), 0xffffffff, 0, 0.0f, 0.0f );
	m_lverRectSmoke[2] = D3DLVERTEX( D3DVECTOR( fWidth, 0, 0), 0xffffffff, 0, 0.25f, 1.0f );
	m_lverRectSmoke[3] = D3DLVERTEX( D3DVECTOR( fWidth, fHeight, 0), 0xffffffff, 0, 0.25f, 0.0f );

	fWidth = m_fLWidth - 1.0f;
	fHeight = m_fLHeight - 1.0f;
	m_lverRectSmoke[4] = D3DLVERTEX( D3DVECTOR(-fWidth, 0, 0), 0xffffffff, 0, 0.25f, 1.0f );
	m_lverRectSmoke[5] = D3DLVERTEX( D3DVECTOR(-fWidth, fHeight, 0), 0xffffffff, 0, 0.25f, 0.0f );
	m_lverRectSmoke[6] = D3DLVERTEX( D3DVECTOR( fWidth, 0, 0), 0xffffffff, 0, 0.5f, 1.0f );
	m_lverRectSmoke[7] = D3DLVERTEX( D3DVECTOR( fWidth, fHeight, 0), 0xffffffff, 0, 0.5f, 0.0f );

	fWidth = m_fLWidth - 2.0f;
	fHeight = m_fLHeight - 2.0f;
	m_lverRectSmoke[8] = D3DLVERTEX( D3DVECTOR(-fWidth, 0, 0), 0xffffffff, 0, 0.5f, 1.0f );
	m_lverRectSmoke[9] = D3DLVERTEX( D3DVECTOR(-fWidth, fHeight, 0), 0xffffffff, 0, 0.5f, 0.0f );
	m_lverRectSmoke[10] = D3DLVERTEX( D3DVECTOR( fWidth, 0, 0), 0xffffffff, 0, 0.75f, 1.0f );
	m_lverRectSmoke[11] = D3DLVERTEX( D3DVECTOR( fWidth, fHeight, 0), 0xffffffff, 0, 0.75f, 0.0f );

	fWidth = m_fLWidth - 3.0f;
	fHeight = m_fLHeight - 3.0f;
	m_lverRectSmoke[12] = D3DLVERTEX( D3DVECTOR(-fWidth, 0, 0), 0xffffffff, 0, 0.75f, 1.0f );
	m_lverRectSmoke[13] = D3DLVERTEX( D3DVECTOR(-fWidth, fHeight, 0), 0xffffffff, 0, 0.75f, 0.0f );
	m_lverRectSmoke[14] = D3DLVERTEX( D3DVECTOR( fWidth, 0, 0), 0xffffffff, 0, 1.0f, 1.0f );
	m_lverRectSmoke[15] = D3DLVERTEX( D3DVECTOR( fWidth, fHeight, 0), 0xffffffff, 0, 1.0f, 0.0f );
}

CFxAidiaFire::~CFxAidiaFire()
{
	DeleteRes();
}

void CFxAidiaFire::DeleteRes()
{}

void CFxAidiaFire::LoadRes(IndexedTexture *pIndexedTexture, CShadowRect *pShadowRect)
{}

BOOL CFxAidiaFire::Render(EffectSort &effect_sort)
{
	if( !GET_D3DDEVICE() )
		return TRUE;

	LPDIRECTDRAWSURFACE7 lpSurface = NULL;
	D3DVECTOR vViewVec;
	D3DMATRIX matRx, matRy, matWorld;
	unsigned int offset;
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

	int nFrame;
	nFrame = effect_sort.nCurFrame;

	offset = nFrame % 4;


	// 방향벡터를 구한다.
	D3DVECTOR vDir, vPosFrom, vPosTo;
	D3DVECTOR vDrawPos;

	// 시작위치가 고정. 시전 객체가 움직일 수도 있기 때문에.
	vPosFrom = effect_sort.vPos;

	// 타겟 좌표를 구한다.
	if (effect_sort.pMobTo)
		vPosTo = D3DVECTOR(effect_sort.pMobTo->m_wx, effect_sort.pMobTo->m_wy, effect_sort.pMobTo->m_wz);
	else if (effect_sort.pNkChaTo)
		vPosTo = D3DVECTOR(effect_sort.pNkChaTo->m_wx, effect_sort.pNkChaTo->m_wy, effect_sort.pNkChaTo->m_wz);
	else
		vPosTo = effect_sort.vTargetPos;

	get_normal_vector(vDir, vPosFrom, vPosTo);

	// 프레임당 스피드.
	vDir *= m_fSpeed;

	// 현재 프레임에서 8프레임 전까지 그린다.
	int start_frame, end_frame;

	start_frame = nFrame - 4;		// 총 8프레임.
	if (start_frame < 0)
		start_frame = 0;
	end_frame = nFrame;

	// 텍스처 택하기.
	// nate 2004 - 7 - image
	if( g_pCapsyongTexture )
		lpSurface = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_AIDIAFIREHEAD ] );

	GET_D3DDEVICE()->SetTexture(0, lpSurface);

	// 마지막 프레임 전까지 그림을 그린다.
	// 여기서 start_frame은 화이어볼의 꼬리쪽.
	int nTex = 0;
	int nFrameNum = 0;

	for (i= end_frame - 1; i >= start_frame; i--)
	{
		vDrawPos.x = vPosFrom.x + vDir.x * (i+1);
		vDrawPos.y = vPosFrom.y + vDir.y * (i+1) + FX_AIDIAFIRE_HEIGHT - m_fLHeight / 2;
		vDrawPos.z = vPosFrom.z + vDir.z * (i+1);

		matWorld._41 = vDrawPos.x;
		matWorld._42 = vDrawPos.y;
		matWorld._43 = vDrawPos.z;

		GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &matWorld );

		MakeLVertexRect(m_lverRect, m_fSize, m_fSize, TRUE);

		// imsi
		GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_LVERTEX, &m_lverRectSmoke[nFrameNum*4], 4, 0 );
		nFrameNum++;
		if (nFrameNum == 4)
			nFrameNum = 0;
	}

	// 마지막 프레임이 목적지에 도착했는가 확인? - 거리를 측정해서 한다.
	vDrawPos.x = vPosFrom.x + vDir.x * (end_frame+1);
	vDrawPos.y = vPosFrom.y + vDir.y * (end_frame+1) + FX_AIDIAFIRE_HEIGHT - m_fLHeight / 2;
	vDrawPos.z = vPosFrom.z + vDir.z * (end_frame+1);

	// 3차원 공간상에서 거리를 재야 한다.
	float fDist = Dist3PTo3P(vPosTo.x, vPosTo.y, vPosTo.z, vPosFrom.x, vPosFrom.y, vPosFrom.z) - \
				  Dist3PTo3P(vDrawPos.x, vDrawPos.y, vDrawPos.z, vPosFrom.x, vPosFrom.y, vPosFrom.z);

	if ( fDist <= 0.0f || fabsf(fDist) <= 1.0f)
	{
		// 맞은 놈한테 너 맞았다고 알려준다.
		// 그럼으로써 맞은 놈이 가지고 있는 effect_num이 삭제된다.
		if (effect_sort.pMobTo)
			effect_sort.pMobTo->Attacked();
		else if (effect_sort.pNkChaTo)
			effect_sort.pNkChaTo->Attacked();

		vDrawPos = vPosTo;

		matWorld._41 = vDrawPos.x;
		matWorld._42 = vDrawPos.y;
		matWorld._43 = vDrawPos.z;


		GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &matWorld );

		// 돌부분.
		// nate 2004 - 7 - image
		if( g_pCapsyongTexture )
			lpSurface = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_AIDIAFIREHEAD ] );
		GET_D3DDEVICE()->SetTexture(0, lpSurface);

		GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_LVERTEX,
										&m_lverRect[offset*4], 4, 0 );

		// 폭파 효과를 호출한다.
		EffectSort ef_sort;
		ef_sort.nType = FX_AIDIABALLBOOM;
		ef_sort.vPos = vDrawPos;
		ef_sort.vTargetPos = vDrawPos;
		pCMyApp->m_pFxSet->InsertFxEffect(ef_sort);
		if (g_pDSound)
		{
			float x, y, z;
			x = (vDrawPos.x - g_pRoh->m_wx) / g_fDSDistance;
			y = (vDrawPos.y - g_pRoh->m_wy) / g_fDSDistance;
			z = (vDrawPos.z - g_pRoh->m_wz) / g_fDSDistance;
			g_pDSound->Play(g_EffectSound[EFF_SND_BOOM3], x, y, z);
		}
		bFinished = TRUE;
	}
	else  													// 선두.
	{
		matWorld._41 = vDrawPos.x;
		matWorld._42 = vDrawPos.y;
		matWorld._43 = vDrawPos.z;

		GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &matWorld );

		// 돌 부분.
		// nate 2004 - 7 - image
		if( g_pCapsyongTexture )
			lpSurface = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_AIDIAFIREHEAD ] );
		GET_D3DDEVICE()->SetTexture(0, lpSurface);

		GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_LVERTEX,
										&m_lverRect[offset*4], 4, 0 );
	}

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE);

	return bFinished;
}

CFxAidiaBallBoom::CFxAidiaBallBoom()
	: m_fBoomSize(0)
{
	memset(m_nFrameIdx, 0, sizeof(m_nFrameIdx));
}

CFxAidiaBallBoom::~CFxAidiaBallBoom()
{
	DeleteRes();
}

// nate 2004 - 8 ImageManager
void CFxAidiaBallBoom::LoadRes()
{
	m_fBoomSize = 6.0f;

	m_nFrameIdx[0] = 0;
	m_nFrameIdx[1] = 0;
	m_nFrameIdx[2] = 1;
	m_nFrameIdx[3] = 1;
	m_nFrameIdx[4] = 1;

	m_nFrameIdx[5] = 2;
	m_nFrameIdx[6] = 2;
	m_nFrameIdx[7] = 3;
	m_nFrameIdx[8] = 3;
	m_nFrameIdx[9] = 3;

	m_nFrameIdx[10] = 4;
	m_nFrameIdx[11] = 4;
	m_nFrameIdx[12] = 4;
	m_nFrameIdx[13] = 5;

	m_nFrameIdx[14] = 5;

	m_nFrameIdx[15] = 6;
	m_nFrameIdx[16] = 6;
	m_nFrameIdx[17] = 6;
	m_nFrameIdx[18] = 7;
	m_nFrameIdx[19] = 7;

	MakeLVertexRect(m_lverBoom, m_fBoomSize, m_fBoomSize, TRUE);
}

void CFxAidiaBallBoom::DeleteRes()
{}

BOOL CFxAidiaBallBoom::Render(EffectSort &effect_sort)
{
	if( !GET_D3DDEVICE() )
		return TRUE;

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
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND,   D3DBLEND_SRCALPHA );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND,  D3DBLEND_INVSRCALPHA );

	///////////////////////////////////////////////////////////////////////
	// 잔상 그리기.
	// 방향벡터를 구한다.
	D3DVECTOR vDir;
	D3DVECTOR vDrawPos;
	BOOL bDrawTail = TRUE;
	///////////////////////////////////////////////////////////////////////
	// 폭파 그리기.
	matWorld._41 = effect_sort.vPos.x;
	matWorld._42 = effect_sort.vPos.y + m_fBoomSize + FX_AIDIAFIRE_HEIGHT;
	matWorld._43 = effect_sort.vPos.z;

	if (effect_sort.nCurFrame < FX_AIDIABALLBOOM_FRAMECOUNT)
	{
		// nate 2004 - 7 - image
		if( g_pCapsyongTexture )
			lpSurface = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ m_nFrameIdx[ effect_sort.nCurFrame ] + EFF_FX_SHADOWRING ] );
		GET_D3DDEVICE()->SetTexture(0, lpSurface);
		GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &matWorld );

		GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_LVERTEX,
										m_lverBoom, 4, 0 );
	}

	if (effect_sort.nCurFrame >= FX_FIREBALLBOOM_FRAMECOUNT - 1)
		bFinished = TRUE;

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE);

	return bFinished;
}