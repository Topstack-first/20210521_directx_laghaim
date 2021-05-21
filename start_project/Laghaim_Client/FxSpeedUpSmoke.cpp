#include "stdafx.h"
// FxSpeedUpSmoke.cpp: implementation of the CFxSpeedUpSmoke class.
//
//////////////////////////////////////////////////////////////////////

#include <assert.h>
#include "sfx.h"
#include "main.h"
#include "NkCharacter.h"
#include "NkMob.h"
#include "GUtil.h"
#include "DirtSnd.h"
#include "LoadEffectSound.h"
#include "ImageDefine.h"
#include "FxSpeedUpSmoke.h"




//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFxSpeedUpSmoke::CFxSpeedUpSmoke()
{

}

CFxSpeedUpSmoke::~CFxSpeedUpSmoke()
{

}

void CFxSpeedUpSmoke::LoadRes()
{

}

void CFxSpeedUpSmoke::DeleteRes()
{

}


BOOL CFxSpeedUpSmoke::Render(EffectSort &effect_sort)
{

	if( !GET_D3DDEVICE() )
		return true;

	BOOL bFinished = FALSE;
	D3DMATRIX matTrans, matView, matRotate, matWorld;
	D3DMATRIX matRx, matRy;
	DWORD dwLighting;

	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_LIGHTING, &dwLighting);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE);
//===========================================================================
	int nFrame = 0, offset = 0;
	nFrame = effect_sort.nCurFrame;

	if (nFrame == 0)
		offset = 0;
	else
		offset = nFrame % 11;

	// ���⺤�͸� ���Ѵ�.
	D3DVECTOR vDir, vPosFrom, vPosTo;
	D3DVECTOR vDrawPos;

	// ������ġ�� ����. ���� ��ü�� ������ ���� �ֱ� ������.
	vPosFrom = effect_sort.vPos;
	vPosFrom.y += 18.0f;

	if (effect_sort.pNkChaTo)
		vPosTo = D3DVECTOR(effect_sort.pNkChaTo->m_wx, effect_sort.pNkChaTo->m_wy - 6.0f, effect_sort.pNkChaTo->m_wz);
	else
		vPosTo = effect_sort.vTargetPos;

	// ����.
	get_normal_vector(vDir, vPosFrom, vPosTo);

	vPosFrom.x+=vDir.x*10;
	vPosFrom.z+=vDir.z*10;

	// �����Ӵ� ���ǵ�.
	vDir *= 6.0f;

	// ���� �����ӿ��� 8������ ������ �׸���.
	int start_frame, end_frame;

	start_frame = nFrame - 8;		// �� 8������.
	if (start_frame < 0)
		start_frame = 0;
	end_frame = nFrame;

	vDrawPos.x = vPosFrom.x + vDir.x * (end_frame+1);
	vDrawPos.y = vPosFrom.y + vDir.y * (end_frame+1);
	vDrawPos.z = vPosFrom.z + vDir.z * (end_frame+1);

	float fDist = DistPlToPlAbs(vPosTo.x, vPosTo.z, vPosFrom.x, vPosFrom.z) -
				  DistPlToPlAbs(vDrawPos.x, vDrawPos.z, vPosFrom.x, vPosFrom.z);

//===========================================================================
	//�̻���
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND,   D3DBLEND_SRCALPHA );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND,  D3DBLEND_INVSRCALPHA );

	float fDir = 0.0f;
	fDir = atan2f(vDir.z, vDir.x);
	Nk3DUtil_SetRotateCCWYMatrix(matRotate, fDir);

	D3DUtil_SetTranslateMatrix(matTrans, vDrawPos);
	D3DMath_MatrixMultiply( matWorld, matRotate, matTrans );

	D3DMATRIX		matS;
	float rand = 0.0f;

	rand = 0.07f;

	D3DUtil_SetScaleMatrix(matS, rand, rand, rand);

	D3DMath_MatrixMultiply(matWorld, matS, matWorld);
	GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &matWorld );

	//�̻��� ����
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_ONE);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_ONE);

	vDrawPos = vPosFrom + (vDir * (end_frame-1));

	D3DVECTOR	vViewVec;
	vViewVec = pCMyApp->GetEyeDirection();

	float ry = atan2f(vViewVec.x, vViewVec.z);
	float rx = atan2f(vViewVec.x*sinf(ry)+vViewVec.z*cosf(ry), vViewVec.y) - (g_PI/2.0f);

	D3DUtil_SetRotateXMatrix(matRx, rx);
	D3DUtil_SetRotateYMatrix(matRy, ry);
	D3DMath_MatrixMultiply(matWorld, matRx, matRy);

	LPDIRECTDRAWSURFACE7 lpSurface = NULL;
	if( g_pCapsyongTexture )
		lpSurface = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_SMOKE_TAIL ] ) ;

	GET_D3DDEVICE()->SetTexture(0, lpSurface);

	// ������ ������ ������ �׸��� �׸���.
	// ���⼭ start_frame�� ȭ�̾�� ������.
	int nTex = 0;
	float fire_scale = 4.0f;
	float fire_dir = 0.7f;
	float fire_fade = 0.5f;
	for (int i= end_frame - 1; i >= start_frame; i--)
	{
		if( i == (end_frame - 1) )
		{
			vDrawPos.x = vPosFrom.x + vDir.x * (i+1);
			vDrawPos.y = vPosFrom.y + vDir.y * (i+1);
			vDrawPos.z = vPosFrom.z + vDir.z * (i+1);
		}
		else
		{
			vDrawPos.x = vPosFrom.x + vDir.x * (end_frame - fire_dir);
			vDrawPos.y = vPosFrom.y + vDir.y * (end_frame - fire_dir);
			vDrawPos.z = vPosFrom.z + vDir.z * (end_frame - fire_dir);
		}

		if( end_frame <= 5 )
			fire_dir *= 1.25f;
		else
			fire_dir *= 1.5f;

		matWorld._41 = vDrawPos.x;
		matWorld._42 = vDrawPos.y;
		matWorld._43 = vDrawPos.z;

		GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &matWorld );

		MakeLVertexRect(m_lsmoke, fire_scale, fire_scale, TRUE);

		m_lsmoke[0].tu = nTex * 0.125f;
		m_lsmoke[1].tu = nTex * 0.125f;
		m_lsmoke[2].tu = (nTex+1) * 0.125f;
		m_lsmoke[3].tu = (nTex+1) * 0.125f;

		m_lsmoke[0].color = D3DRGBA(fire_fade, fire_fade, fire_fade, fire_fade);
		m_lsmoke[1].color = D3DRGBA(fire_fade, fire_fade, fire_fade, fire_fade);
		m_lsmoke[2].color = D3DRGBA(fire_fade, fire_fade, fire_fade, fire_fade);
		m_lsmoke[3].color = D3DRGBA(fire_fade, fire_fade, fire_fade, fire_fade);

		GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_LVERTEX,
										m_lsmoke, 4, 0 );
		nTex++;
		fire_scale += 0.5f;
		fire_fade -= 0.05f;
	}

	// ���� ȿ���� ȣ���Ѵ�.
	if( pCMyApp->m_pFxSet )
	{
		EffectSort ef_sort;
		ZeroMemory(&ef_sort, sizeof(EffectSort));
		ef_sort.nType = FX_STONESHOTBOOM;
		ef_sort.vPos = vPosTo;//vPosFrom;
		ef_sort.vTargetPos = vPosTo;
		pCMyApp->m_pFxSet->InsertFxEffect(ef_sort);
	}

	bFinished = TRUE;
//===========================================================================
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, dwLighting);

	return bFinished;
}