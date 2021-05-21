#include "stdafx.h"
#include "sfx.h"
#include "main.h"
#include "NkMob.h"
#include "NkCharacter.h"
#include "GUtil.h"
#include "DirtSnd.h"
#include "LoadEffectSound.h"
#include "ImageDefine.h"
#include "ShadowRect.h"



extern int g_EffectSound[];
extern float g_fDSDistance;
extern BOOL	g_bDungeon;
// nate 2004 - 7 �۷ι� ShadowRect���


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFxFlame::CFxFlame()
{
	m_fSpeed = 6.0f;
}

CFxFlame::~CFxFlame()
{
	DeleteRes();
}

void CFxFlame::LoadRes(IndexedTexture *pIndexedTexture, CShadowRect *pShadowRect)
{}

void CFxFlame::DeleteRes()
{}

BOOL CFxFlame::Render(EffectSort &effect_sort)
{
	if( !GET_D3DDEVICE() )
		return true;

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
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND,   D3DBLEND_ONE );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND,  D3DBLEND_ONE );

	// ���⺤�͸� ���Ѵ�.
	D3DVECTOR vDir, vPosFrom, vPosTo;
	D3DVECTOR vDrawPos;
	int nFrame = effect_sort.nCurFrame;

	// ȭ�̾ ������ ������ġ�� ����. ���� ��ü�� ������ ���� �ֱ� ������.
	vPosFrom = effect_sort.vPos;
	vPosFrom.y += 15.0f;

	// Ÿ�� ��ǥ�� ���Ѵ�.
	vPosTo = effect_sort.vTargetPos;

	vPosTo.y += 15.0f;
	get_normal_vector(vDir, vPosFrom, vPosTo);

	if( effect_sort.subNum )
		vDir *= 10.0f;
	else
		vDir *= m_fSpeed;	// �����Ӵ� ���ǵ�.

	// �ؽ�ó ���ϱ�.
	if( g_pCapsyongTexture )
		lpSurface = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_FLAME_TAIL ] ) ;
	GET_D3DDEVICE()->SetTexture(0, lpSurface);

	int	temp = nFrame / 8;
	int start_frame = temp * 8;
	int end_frame = nFrame - start_frame;
	if( nFrame > 8 )
		end_frame = 8;

	int	i = 0;
	float fire_scale = 4.5f;
	float fire_fade = 0.3f;

	for( i = 0; i < end_frame; i++ )
	{
		vDrawPos.x = vPosFrom.x + (vDir.x+RandomNum(-0.5f, 0.5f)) * (i+1);
		vDrawPos.y = vPosFrom.y + vDir.y * (i+1);
		vDrawPos.z = vPosFrom.z + (vDir.z+RandomNum(-0.5f, 0.5f)) * (i+1);

		matWorld._41 = vDrawPos.x;
		matWorld._42 = vDrawPos.y;
		matWorld._43 = vDrawPos.z;

		GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &matWorld );

		MakeLVertexRect(m_lverTail, fire_scale, fire_scale, TRUE);

		m_lverTail[0].tu = i * 0.125f;
		m_lverTail[1].tu = i * 0.125f;
		m_lverTail[2].tu = (i+1) * 0.125f;
		m_lverTail[3].tu = (i+1) * 0.125f;

		m_lverTail[0].color = D3DRGBA(fire_fade, fire_fade, fire_fade, fire_fade);
		m_lverTail[1].color = D3DRGBA(fire_fade, fire_fade, fire_fade, fire_fade);
		m_lverTail[2].color = D3DRGBA(fire_fade, fire_fade, fire_fade, fire_fade);
		m_lverTail[3].color = D3DRGBA(fire_fade, fire_fade, fire_fade, fire_fade);

		GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_LVERTEX,
										m_lverTail, 4, 0 );

		fire_scale += 0.375f;
		fire_fade += 0.04f;
	}

	if( effect_sort.subNum )
	{
		fire_scale = 3.5f;
		fire_fade = 0.5f;

		for( i = 0; i < end_frame; i++ )
		{
			vDrawPos.x = vPosFrom.x + (vDir.x+RandomNum(-0.5f, 0.5f)) * (i+1.5f);
			vDrawPos.y = vPosFrom.y + vDir.y * (i+1);
			vDrawPos.z = vPosFrom.z + (vDir.z+RandomNum(-0.5f, 0.5f)) * (i+1.5f);

			matWorld._41 = vDrawPos.x;
			matWorld._42 = vDrawPos.y;
			matWorld._43 = vDrawPos.z;

			GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &matWorld );

			MakeLVertexRect(m_lverTail, fire_scale, fire_scale, TRUE);

			m_lverTail[0].tu = i * 0.125f;
			m_lverTail[1].tu = i * 0.125f;
			m_lverTail[2].tu = (i+1) * 0.125f;
			m_lverTail[3].tu = (i+1) * 0.125f;

			m_lverTail[0].color = D3DRGBA(fire_fade, fire_fade, fire_fade, fire_fade);
			m_lverTail[1].color = D3DRGBA(fire_fade, fire_fade, fire_fade, fire_fade);
			m_lverTail[2].color = D3DRGBA(fire_fade, fire_fade, fire_fade, fire_fade);
			m_lverTail[3].color = D3DRGBA(fire_fade, fire_fade, fire_fade, fire_fade);

			GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_LVERTEX,
											m_lverTail, 4, 0 );

			fire_scale += 0.375f;
			fire_fade += 0.04f;
		}
	}

	BOOL bFinished = FALSE;

	if(effect_sort.nCurFrame >= (effect_sort.subNum?20:8))
		bFinished = TRUE;

	// �ٴ��� ��.
	// nate 2004 - 7 �۷ι� ShadowRect���
	if( g_pShadowRect && !effect_sort.subNum )
	{
		// nate 2004 - 7 - image
		if( g_pCapsyongTexture )
			lpSurface = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_FLAME_BASE ] ) ;

		if (g_bDungeon)
			// nate 2004 - 7 �۷ι� ShadowRect���
			g_pShadowRect->SetTileCoord(vDrawPos, 55, FALSE);		// �οﰪ�� �ð��ΰ� �ƴѰ�
		else
			// nate 2004 - 7 �۷ι� ShadowRect���
			g_pShadowRect->SetTileCoord(vDrawPos, 70, FALSE);		// �οﰪ�� �ð��ΰ� �ƴѰ�
		// nate 2004 - 7 �۷ι� ShadowRect���
		g_pShadowRect->Render(lpSurface, FALSE);	// �οﰪ�� ����׸����ΰ� �ƴѰ�?
	}

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE);

	return bFinished;
}

