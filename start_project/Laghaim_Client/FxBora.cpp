#include "stdafx.h"
#define D3D_OVERLOADS
#include <d3d.h>
#include "FxSet.h"
#include "FxBora.h"
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

CFxBora::CFxBora()
{
	m_fSize = 3.0f;
	m_fSpeed = 4.5f;
}

CFxBora::~CFxBora()
{
	DeleteRes();
}

void CFxBora::LoadRes(IndexedTexture *pIndexedTexture, CShadowRect *pShadowRect)
{}
void CFxBora::DeleteRes()
{}

BOOL CFxBora::Render(EffectSort &effect_sort)
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
		offset = nFrame % 11;

	// ���⺤�͸� ���Ѵ�.
	D3DVECTOR vDir, vPosFrom, vPosTo;
	D3DVECTOR vDrawPos;

	// ȭ�̾ ������ ������ġ�� ����. ���� ��ü�� ������ ���� �ֱ� ������.
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

	start_frame = nFrame - 8;		// �� 8������.
	if (start_frame < 0)
		start_frame = 0;
	end_frame = nFrame;

	// �ؽ�ó ���ϱ�.
	// nate 2004 - 7 - image
	if( g_pCapsyongTexture )
		lpSurface = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_BORATAIL ] );
	GET_D3DDEVICE()->SetTexture(0, lpSurface);

	// ������ ������ ������ �׸��� �׸���.
	// ���⼭ start_frame�� ȭ�̾�� ������.
	int nTex = 0;
	for (i= end_frame - 1; i >= start_frame; i--)
	{
		vDrawPos.x = vPosFrom.x + vDir.x * (i+1);
		vDrawPos.y = vPosFrom.y + vDir.y * (i+1);
		vDrawPos.z = vPosFrom.z + vDir.z * (i+1);

		matWorld._41 = vDrawPos.x;
		matWorld._42 = vDrawPos.y + FX_BORA_HEIGHT;
		matWorld._43 = vDrawPos.z;

		GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &matWorld );

		MakeLVertexRect(m_lverTail, m_fSize, m_fSize, TRUE);

		m_lverTail[0].tu = nTex * 0.125f;
		m_lverTail[1].tu = nTex * 0.125f;
		m_lverTail[2].tu = (nTex+1) * 0.125f;
		m_lverTail[3].tu = (nTex+1) * 0.125f;
		GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_LVERTEX,
										m_lverTail, 4, 0 );
		nTex++;
	}

	// ������ �������� �������� �����ߴ°� Ȯ��? - �Ÿ��� �����ؼ� �Ѵ�.
	vDrawPos.x = vPosFrom.x + vDir.x * (end_frame+1);
	vDrawPos.y = vPosFrom.y + vDir.y * (end_frame+1);
	vDrawPos.z = vPosFrom.z + vDir.z * (end_frame+1);

	float fDist = DistPlToPlAbs(vPosTo.x, vPosTo.z, vPosFrom.x, vPosFrom.z) - \
				  DistPlToPlAbs(vDrawPos.x, vDrawPos.z, vPosFrom.x, vPosFrom.z);

	if ( fDist <= 0.0f || fabsf(fDist) <= 10.0f)
	{
		// ���� ������ �� �¾Ҵٰ� �˷��ش�.
		// �׷����ν� ���� ���� ������ �ִ� effect_num�� �����ȴ�.
		if (effect_sort.pMobTo)
			effect_sort.pMobTo->Attacked();
		else if (effect_sort.pNkChaTo)
			effect_sort.pNkChaTo->Attacked();

		vDrawPos = vPosTo;
		// nate 2004 - 7 - image
		lpSurface = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_BORAHEAD ] );
		GET_D3DDEVICE()->SetTexture(0, lpSurface);

		matWorld._41 = vDrawPos.x;
		matWorld._42 = vDrawPos.y + FX_BORA_HEIGHT;
		matWorld._43 = vDrawPos.z;

		// ����Ÿ ����.
		MakeLVertexRect(m_lverHead, m_fSize*2.5f, m_fSize*2.5f, TRUE);

		GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &matWorld );

		GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_LVERTEX,
										m_lverHead, 4, 0 );
		bFinished = TRUE;

		// ���� ȿ���� ȣ���Ѵ�.
		EffectSort ef_sort;
		ef_sort.nType = FX_BORABOOM;
		ef_sort.vPos = vPosFrom;
		ef_sort.vTargetPos = vPosTo;
		if( pCMyApp->m_pFxSet )
			pCMyApp->m_pFxSet->InsertFxEffect(ef_sort);
	}
	else
	{
		// ����.
		matWorld._41 = vDrawPos.x;
		matWorld._42 = vDrawPos.y + FX_BORA_HEIGHT;
		matWorld._43 = vDrawPos.z;

		GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &matWorld );

		m_lverHead[0] = D3DLVERTEX( D3DVECTOR(-m_fSize*1.5f, -m_fSize*1.5f, 0), 0xffffffff, 0, 0.0f, 1.0f );
		m_lverHead[1] = D3DLVERTEX( D3DVECTOR(-m_fSize*1.5f, m_fSize*1.5f, 0), 0xffffffff, 0, 0.0f, 0.0f );
		m_lverHead[2] = D3DLVERTEX( D3DVECTOR( m_fSize*1.5f, -m_fSize*1.5f, 0), 0xffffffff, 0, 1.0f, 1.0f );
		m_lverHead[3] = D3DLVERTEX( D3DVECTOR( m_fSize*1.5f, m_fSize*1.5f, 0), 0xffffffff, 0, 1.0f, 0.0f );

		// nate 2004 - 7 - image
		if( g_pCapsyongTexture )
			lpSurface = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_BORAHEAD ] );
		GET_D3DDEVICE()->SetTexture(0, lpSurface);

		GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_LVERTEX,
										m_lverHead, 4, 0 );
	}

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE);

	return bFinished;
}

