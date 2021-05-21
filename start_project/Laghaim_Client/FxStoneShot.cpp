#include "stdafx.h"
#define D3D_OVERLOADS

#include <d3d.h>
#include "FxSet.h"
#include "FxStoneShot.h"
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

#define FX_STONESHOT_LHEIGHT	14.0f
#define FX_STONESHOT_LWIDTH		7.0f

CFxStoneShot::CFxStoneShot()
{
	m_fSpeed = 5.0f;

	m_lverRect[0] = D3DLVERTEX( D3DVECTOR(-FX_STONESHOT_LWIDTH, 0, 0), 0xffffffff, 0, 0.0f, 1.0f );
	m_lverRect[1] = D3DLVERTEX( D3DVECTOR(-FX_STONESHOT_LWIDTH, FX_STONESHOT_LHEIGHT, 0), 0xffffffff, 0, 0.0f, 0.0f );
	m_lverRect[2] = D3DLVERTEX( D3DVECTOR( FX_STONESHOT_LWIDTH, 0, 0), 0xffffffff, 0, 0.25f, 1.0f );
	m_lverRect[3] = D3DLVERTEX( D3DVECTOR( FX_STONESHOT_LWIDTH, FX_STONESHOT_LHEIGHT, 0), 0xffffffff, 0, 0.25f, 0.0f );

	m_lverRect[4] = D3DLVERTEX( D3DVECTOR(-FX_STONESHOT_LWIDTH, 0, 0), 0xffffffff, 0, 0.25f, 1.0f );
	m_lverRect[5] = D3DLVERTEX( D3DVECTOR(-FX_STONESHOT_LWIDTH, FX_STONESHOT_LHEIGHT, 0), 0xffffffff, 0, 0.25f, 0.0f );
	m_lverRect[6] = D3DLVERTEX( D3DVECTOR( FX_STONESHOT_LWIDTH, 0, 0), 0xffffffff, 0, 0.5f, 1.0f );
	m_lverRect[7] = D3DLVERTEX( D3DVECTOR( FX_STONESHOT_LWIDTH, FX_STONESHOT_LHEIGHT, 0), 0xffffffff, 0, 0.5f, 0.0f );

	m_lverRect[8] = D3DLVERTEX( D3DVECTOR(-FX_STONESHOT_LWIDTH, 0, 0), 0xffffffff, 0, 0.5f, 1.0f );
	m_lverRect[9] = D3DLVERTEX( D3DVECTOR(-FX_STONESHOT_LWIDTH, FX_STONESHOT_LHEIGHT, 0), 0xffffffff, 0, 0.5f, 0.0f );
	m_lverRect[10] = D3DLVERTEX( D3DVECTOR( FX_STONESHOT_LWIDTH, 0, 0), 0xffffffff, 0, 0.75f, 1.0f );
	m_lverRect[11] = D3DLVERTEX( D3DVECTOR( FX_STONESHOT_LWIDTH, FX_STONESHOT_LHEIGHT, 0), 0xffffffff, 0, 0.75f, 0.0f );

	m_lverRect[12] = D3DLVERTEX( D3DVECTOR(-FX_STONESHOT_LWIDTH, 0, 0), 0xffffffff, 0, 0.75f, 1.0f );
	m_lverRect[13] = D3DLVERTEX( D3DVECTOR(-FX_STONESHOT_LWIDTH, FX_STONESHOT_LHEIGHT, 0), 0xffffffff, 0, 0.75f, 0.0f );
	m_lverRect[14] = D3DLVERTEX( D3DVECTOR( FX_STONESHOT_LWIDTH, 0, 0), 0xffffffff, 0, 1.0f, 1.0f );
	m_lverRect[15] = D3DLVERTEX( D3DVECTOR( FX_STONESHOT_LWIDTH, FX_STONESHOT_LHEIGHT, 0), 0xffffffff, 0, 1.0f, 0.0f );
	// ������ ���� ����.

	m_lverRectSmoke[0] = D3DLVERTEX( D3DVECTOR(-FX_STONESHOT_LWIDTH*1.5, 0, 0), 0xffffd29f, 0, 0.0f, 1.0f );
	m_lverRectSmoke[1] = D3DLVERTEX( D3DVECTOR(-FX_STONESHOT_LWIDTH*1.5, FX_STONESHOT_LHEIGHT*1.5, 0), 0xffffd29f, 0, 0.0f, 0.0f );
	m_lverRectSmoke[2] = D3DLVERTEX( D3DVECTOR( FX_STONESHOT_LWIDTH*1.5, 0, 0), 0xffffd29f, 0, 0.25f, 1.0f );
	m_lverRectSmoke[3] = D3DLVERTEX( D3DVECTOR( FX_STONESHOT_LWIDTH*1.5, FX_STONESHOT_LHEIGHT*1.5, 0), 0xffffd29f, 0, 0.25f, 0.0f );

	m_lverRectSmoke[4] = D3DLVERTEX( D3DVECTOR(-FX_STONESHOT_LWIDTH*1.5, 0, 0), 0xffffd29f, 0, 0.25f, 1.0f );
	m_lverRectSmoke[5] = D3DLVERTEX( D3DVECTOR(-FX_STONESHOT_LWIDTH*1.5, FX_STONESHOT_LHEIGHT*1.5, 0), 0xffffd29f, 0, 0.25f, 0.0f );
	m_lverRectSmoke[6] = D3DLVERTEX( D3DVECTOR( FX_STONESHOT_LWIDTH*1.5, 0, 0), 0xffffd29f, 0, 0.5f, 1.0f );
	m_lverRectSmoke[7] = D3DLVERTEX( D3DVECTOR( FX_STONESHOT_LWIDTH*1.5, FX_STONESHOT_LHEIGHT*1.5, 0), 0xffffd29f, 0, 0.5f, 0.0f );

	m_lverRectSmoke[8] = D3DLVERTEX( D3DVECTOR(-FX_STONESHOT_LWIDTH*1.5, 0, 0), 0xffffd29f, 0, 0.5f, 1.0f );
	m_lverRectSmoke[9] = D3DLVERTEX( D3DVECTOR(-FX_STONESHOT_LWIDTH*1.5, FX_STONESHOT_LHEIGHT*1.5, 0), 0xffffd29f, 0, 0.5f, 0.0f );
	m_lverRectSmoke[10] = D3DLVERTEX( D3DVECTOR( FX_STONESHOT_LWIDTH*1.5, 0, 0), 0xffffd29f, 0, 0.75f, 1.0f );
	m_lverRectSmoke[11] = D3DLVERTEX( D3DVECTOR( FX_STONESHOT_LWIDTH*1.5, FX_STONESHOT_LHEIGHT*1.5, 0), 0xffffd29f, 0, 0.75f, 0.0f );

	m_lverRectSmoke[12] = D3DLVERTEX( D3DVECTOR(-FX_STONESHOT_LWIDTH*1.5, 0, 0), 0xffffd29f, 0, 0.75f, 1.0f );
	m_lverRectSmoke[13] = D3DLVERTEX( D3DVECTOR(-FX_STONESHOT_LWIDTH*1.5, FX_STONESHOT_LHEIGHT*1.5, 0), 0xffffd29f, 0, 0.75f, 0.0f );
	m_lverRectSmoke[14] = D3DLVERTEX( D3DVECTOR( FX_STONESHOT_LWIDTH*1.5, 0, 0), 0xffffd29f, 0, 1.0f, 1.0f );
	m_lverRectSmoke[15] = D3DLVERTEX( D3DVECTOR( FX_STONESHOT_LWIDTH*1.5, FX_STONESHOT_LHEIGHT*1.5, 0), 0xffffd29f, 0, 1.0f, 0.0f );
}

CFxStoneShot::~CFxStoneShot()
{
	DeleteRes();
}

void CFxStoneShot::DeleteRes()
{}
void CFxStoneShot::LoadRes(IndexedTexture *pIndexedTexture, CShadowRect *pShadowRect)
{}


BOOL CFxStoneShot::Render(EffectSort &effect_sort)
{
	if( !GET_D3DDEVICE() )
		return true;

	if(!effect_sort.pNkChaTo
			&& !effect_sort.pMobTo)
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

	if (nFrame == 0)
		offset = 0;
	else
		offset = nFrame % 4;

	// ���⺤�͸� ���Ѵ�.
	D3DVECTOR vDir, vPosFrom, vPosTo;
	D3DVECTOR vDrawPos;

	// ���׿� ������ ������ġ�� ����. ���� ��ü�� ������ ���� �ֱ� ������.
	vPosFrom = effect_sort.vPos;

	// Ÿ�� ��ǥ�� ���Ѵ�.
	if (effect_sort.pMobTo)
		vPosTo = D3DVECTOR(effect_sort.pMobTo->m_wx, effect_sort.pMobTo->m_wy, effect_sort.pMobTo->m_wz);
	else if (effect_sort.pNkChaTo)
		vPosTo = D3DVECTOR(effect_sort.pNkChaTo->m_wx, effect_sort.pNkChaTo->m_wy, effect_sort.pNkChaTo->m_wz);
	else
		vPosTo = effect_sort.vTargetPos;

	get_normal_vector(vDir, vPosFrom, vPosTo);

	// �����Ӵ� ���ǵ�.
	vDir *= m_fSpeed;

	// ���� �����ӿ��� 8������ ������ �׸���.
	int start_frame, end_frame;

	start_frame = nFrame - 4;		// �� 8������.
	if (start_frame < 0)
		start_frame = 0;
	end_frame = nFrame;

	// ������ ������ ������ �׸��� �׸���.
	// ���⼭ start_frame�� ȭ�̾�� ������.
	int nTex = 0;
	int nFrameNum = 0;

	// nate 2004 - 7 - image
	lpSurface = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_STONESHOTBASE ] );
	GET_D3DDEVICE()->SetTexture(0, lpSurface);

	for (i= end_frame - 1; i >= start_frame; i--)
	{
		vDrawPos.x = vPosFrom.x + vDir.x * (i+1);
		vDrawPos.y = vPosFrom.y + vDir.y * (i+1);
		vDrawPos.z = vPosFrom.z + vDir.z * (i+1);

		matWorld._41 = vDrawPos.x;
		matWorld._42 = vDrawPos.y;
		matWorld._43 = vDrawPos.z;

		GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &matWorld );

		GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_LVERTEX,
										&m_lverRectSmoke[nFrameNum*4], 4, 0 );
		nFrameNum++;
		if (nFrameNum == 4)
			nFrameNum = 0;
	}

	// ������ �������� �������� �����ߴ°� Ȯ��? - �Ÿ��� �����ؼ� �Ѵ�.
	vDrawPos.x = vPosFrom.x + vDir.x * (end_frame+1);
	vDrawPos.y = vPosFrom.y + vDir.y * (end_frame+1);
	vDrawPos.z = vPosFrom.z + vDir.z * (end_frame+1);

	// 3���� �����󿡼� �Ÿ��� ��� �Ѵ�.
	float fDist = Dist3PTo3P(vPosTo.x, vPosTo.y, vPosTo.z, vPosFrom.x, vPosFrom.y, vPosFrom.z) - \
				  Dist3PTo3P(vDrawPos.x, vDrawPos.y, vDrawPos.z, vPosFrom.x, vPosFrom.y, vPosFrom.z);

	if ( fDist <= 0.0f || fabsf(fDist) <= 1.0f)
	{
		// ���� ������ �� �¾Ҵٰ� �˷��ش�.
		// �׷����ν� ���� ���� ������ �ִ� effect_num�� �����ȴ�.
		if (effect_sort.pMobTo)
			effect_sort.pMobTo->Attacked();
		else if (effect_sort.pNkChaTo)
			effect_sort.pNkChaTo->Attacked();

		vDrawPos = vPosTo;

		matWorld._41 = vDrawPos.x;
		matWorld._42 = vDrawPos.y;
		matWorld._43 = vDrawPos.z;

		// ����Ÿ ����.
		GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &matWorld );

		// nate 2004 - 7 - image
		lpSurface = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_STONESHOTBASE ] );
		GET_D3DDEVICE()->SetTexture(0, lpSurface);

		GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_LVERTEX,
										&m_lverRectSmoke[offset*4], 4, 0 );

		// nate 2004 - 7 - image
		lpSurface = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_STONESHOTHEAD ] );
		GET_D3DDEVICE()->SetTexture(0, lpSurface);

		GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_LVERTEX,
										&m_lverRect[offset*4], 4, 0 );

		bFinished = TRUE;

		// ���� ȿ���� ȣ���Ѵ�.
		EffectSort ef_sort;
		ef_sort.nType = FX_STONESHOTBOOM;
		ef_sort.vPos = vDrawPos;
		ef_sort.vTargetPos = vDrawPos;
		pCMyApp->m_pFxSet->InsertFxEffect(ef_sort);
	}
	else
	{
		// ����.
		matWorld._41 = vDrawPos.x;
		matWorld._42 = vDrawPos.y;
		matWorld._43 = vDrawPos.z;

		GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &matWorld );
		// nate 2004 - 7 - image
		lpSurface = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_STONESHOTBASE ] );
		GET_D3DDEVICE()->SetTexture(0, lpSurface);

		GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_LVERTEX,
										&m_lverRectSmoke[offset*4], 4, 0 );

		// nate 2004 - 7 - image
		lpSurface = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_STONESHOTHEAD ] );
		GET_D3DDEVICE()->SetTexture(0, lpSurface);

		GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_LVERTEX,
										&m_lverRect[offset*4], 4, 0 );
	}

	// �ٴ��� ��.
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE);

	return bFinished;
}

/////////////////////////////////////////////////////////////////////////////////////
// CFxStoneShotBoom

#define FX_STONESHOTBOOM_BOOMSIZE 25.0f
#define FX_STONESHOTBOOM_FRAGSIZE 3.0f

CFxStoneShotBoom::CFxStoneShotBoom()
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

	MakeLVertexRect(m_lverBoom, FX_STONESHOTBOOM_BOOMSIZE, FX_STONESHOTBOOM_BOOMSIZE, TRUE);
	m_lverBoom[0].color = 0xffffd29f;
	m_lverBoom[1].color = 0xffffd29f;
	m_lverBoom[2].color = 0xffffd29f;
	m_lverBoom[3].color = 0xffffd29f;
}

CFxStoneShotBoom::~CFxStoneShotBoom()
{
	DeleteRes();
}

void CFxStoneShotBoom::LoadRes(IndexedTexture *pIndexedTexture, CShadowRect *pShadowRect)
{}

void CFxStoneShotBoom::DeleteRes()
{}

BOOL CFxStoneShotBoom::Render(EffectSort &effect_sort)
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
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND,   D3DBLEND_SRCALPHA );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND,  D3DBLEND_INVSRCALPHA );

	///////////////////////////////////////////////////////////////////////
	// �ܻ� �׸���.

	// ���⺤�͸� ���Ѵ�.
	D3DVECTOR vDir;
	D3DVECTOR vDrawPos;
	BOOL bDrawTail = TRUE;

	///////////////////////////////////////////////////////////////////////
	// ���� �׸���.

	matWorld._41 = effect_sort.vPos.x;
	matWorld._42 = effect_sort.vPos.y + FX_STONESHOT_HEIGHT;
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
	lpSurface = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_STONESHOTBOOM ] );
	GET_D3DDEVICE()->SetTexture(0, lpSurface);

	GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &matWorld );

	GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_LVERTEX,
									m_lverBoom, 4, 0 );



	if (effect_sort.nCurFrame >= FX_FIREBALLBOOM_FRAMECOUNT - 1)
		bFinished = TRUE;

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE);

	return bFinished;
}