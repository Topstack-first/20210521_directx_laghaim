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
// nate 2004 - 7 글로벌 ShadowRect사용


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

	// 방향벡터를 구한다.
	D3DVECTOR vDir, vPosFrom, vPosTo;
	D3DVECTOR vDrawPos;
	int nFrame = effect_sort.nCurFrame;

	// 화이어볼 마법은 시작위치가 고정. 시전 객체가 움직일 수도 있기 때문에.
	vPosFrom = effect_sort.vPos;
	vPosFrom.y += 15.0f;

	// 타겟 좌표를 구한다.
	vPosTo = effect_sort.vTargetPos;

	vPosTo.y += 15.0f;
	get_normal_vector(vDir, vPosFrom, vPosTo);

	if( effect_sort.subNum )
		vDir *= 10.0f;
	else
		vDir *= m_fSpeed;	// 프레임당 스피드.

	// 텍스처 택하기.
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

	// 바닥의 빛.
	// nate 2004 - 7 글로벌 ShadowRect사용
	if( g_pShadowRect && !effect_sort.subNum )
	{
		// nate 2004 - 7 - image
		if( g_pCapsyongTexture )
			lpSurface = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_FLAME_BASE ] ) ;

		if (g_bDungeon)
			// nate 2004 - 7 글로벌 ShadowRect사용
			g_pShadowRect->SetTileCoord(vDrawPos, 55, FALSE);		// 부울값은 시간인가 아닌가
		else
			// nate 2004 - 7 글로벌 ShadowRect사용
			g_pShadowRect->SetTileCoord(vDrawPos, 70, FALSE);		// 부울값은 시간인가 아닌가
		// nate 2004 - 7 글로벌 ShadowRect사용
		g_pShadowRect->Render(lpSurface, FALSE);	// 부울값은 리얼그림자인가 아닌가?
	}

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE);

	return bFinished;
}

