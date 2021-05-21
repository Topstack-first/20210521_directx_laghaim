#include "stdafx.h"
#include <assert.h>
#include "sfx.h"
#include "main.h"
#include "Land.h"
#include "NkCharacter.h"
#include "NkMob.h"
#include "GUtil.h"
#include "DirtSnd.h"
#include "LoadEffectSound.h"
// nate 2004 - 7 - image
#include "ImageDefine.h"




extern int g_EffectSound[];
extern float g_fDSDistance;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFxWarGiantMulti::CFxWarGiantMulti()
{
	m_pLolo				= NULL;
}

CFxWarGiantMulti::~CFxWarGiantMulti()
{
	SAFE_DELETE(m_pLolo);
}

void CFxWarGiantMulti::Create()
{}

// nate 2004 - 8 ImageManager
void CFxWarGiantMulti::LoadRes()
{
	if (!m_pLolo)
	{
		m_pLolo = new CLolos;
		m_pLolo->SetIndexedTexture( g_pCapsyongTexture );
		m_pLolo->Read("data/effect/lolos/bb.LOL");
	}



}

void CFxWarGiantMulti::DeleteRes()
{}

BOOL CFxWarGiantMulti::Render(EffectSort &effect_sort)
{
	if( !GET_D3DDEVICE() )
		return true;

	BOOL bFinished = FALSE;
	D3DMATRIX matTrans, matView, matRotate, matWorld;
	DWORD dwLighting;

	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_LIGHTING, &dwLighting);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE);

	int nFrame, offset;
	nFrame = effect_sort.nCurFrame;

	if (nFrame == 0)
		offset = 0;
	else
		offset = nFrame % 11;

	// 방향벡터를 구한다.
	D3DVECTOR vDir, vPosFrom, vPosTo;
	D3DVECTOR vDrawPos;

	// 시작위치가 고정. 시전 객체가 움직일 수도 있기 때문에.
	if( nFrame <= 12 )
	{
		vPosFrom = effect_sort.vPos;
		vPosFrom.y += 20.0f;
		vPosTo = effect_sort.vTargetPos;
	}
	else
	{
		if( effect_sort.pNkChaTo )
		{
			vPosTo = D3DVECTOR(effect_sort.pNkChaTo->m_wx, effect_sort.pNkChaTo->m_wy, effect_sort.pNkChaTo->m_wz);
			vPosFrom = vPosTo;
			vPosFrom.y += 150.0f;
		}
		else if( effect_sort.pMobTo )
		{
			vPosTo = D3DVECTOR(effect_sort.pMobTo->m_wx, effect_sort.pMobTo->m_wy, effect_sort.pMobTo->m_wz);
			vPosFrom = vPosTo;
			vPosFrom.y += 150.0f;
		}
		else
		{
			vPosTo = effect_sort.vTargetPos;
			vPosFrom = vPosTo;
			vPosFrom.y += 150.0f;
		}
	}

	// 방향.
	get_normal_vector(vDir, vPosFrom, vPosTo);

	if( nFrame <= 12 )
		vDir *= 5 + (nFrame*0.5f);
	else
	{
		nFrame -= 12;
		vDir *= 13.5f;
	}

	// 현재 프레임에서 8프레임 전까지 그린다.
	int start_frame, end_frame;

	start_frame = nFrame - 8;		// 총 8프레임.
	if (start_frame < 0)
		start_frame = 0;
	end_frame = nFrame;

	vDrawPos.x = vPosFrom.x + vDir.x * (end_frame+1);
	vDrawPos.y = vPosFrom.y + vDir.y * (end_frame+1);
	vDrawPos.z = vPosFrom.z + vDir.z * (end_frame+1);

	if ( vDrawPos.y <= vPosTo.y &&  nFrame > 12 )
	{
		EffectSort ef_sort;
		ZeroMemory(&ef_sort, sizeof(EffectSort));
		ef_sort.nType = FX_FIREBALLBOOM;
		ef_sort.vPos = vPosFrom;
		ef_sort.vTargetPos = vPosTo;
		pCMyApp->m_pFxSet->InsertFxEffect(ef_sort);
		if (g_pDSound)
			g_pDSound->Play(g_EffectSound[EFF_SND_BOOM1], (vPosTo.x- g_pRoh->m_wx)/g_fDSDistance, (vPosTo.y- g_pRoh->m_wy)/g_fDSDistance, (vPosTo.z-g_pRoh->m_wz)/g_fDSDistance);

		bFinished = TRUE;
	}

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND,   D3DBLEND_SRCALPHA );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND,  D3DBLEND_INVSRCALPHA );

	D3DMATRIX		matRx, matRy, matS;
	D3DVECTOR		vDirection;
	float			rx = 0.0f, ry = 0.0f;

	vDirection = vPosFrom - vPosTo;

	ry = atan2f(vDirection.x, vDirection.z);
	rx = atan2f(vDirection.x*sinf(ry) + vDirection.z*cosf(ry), vDirection.y) + (g_PI / 2.0f);

	D3DUtil_SetRotateZMatrix(matRx, rx);
	D3DUtil_SetRotateYMatrix(matRy, ry);

	D3DMath_MatrixMultiply(matRotate, matRx, matRy);
	D3DUtil_SetTranslateMatrix(matTrans, vDrawPos);
	D3DMath_MatrixMultiply( matWorld, matRotate, matTrans );

	D3DUtil_SetScaleMatrix(matS, 3.0f, 2.0f, 2.0f);
	D3DMath_MatrixMultiply(matWorld, matS, matWorld);
	GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &matWorld );

	if( m_pLolo )
		m_pLolo->Render(GET_D3DDEVICE(), offset, FALSE, FALSE, FALSE, NULL, NULL, FALSE, FALSE);

	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_ONE);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_ONE);

	D3DVECTOR	vViewVec;
	vViewVec = pCMyApp->GetEyeDirection();

	ry = atan2f(vViewVec.x, vViewVec.z);
	rx = atan2f(vViewVec.x*sinf(ry)+vViewVec.z*cosf(ry), vViewVec.y) - (g_PI/2.0f);

	D3DUtil_SetRotateXMatrix(matRx, rx);
	D3DUtil_SetRotateYMatrix(matRy, ry);
	D3DMath_MatrixMultiply(matWorld, matRx, matRy);

	LPDIRECTDRAWSURFACE7 lpSurface = NULL;
	// 텍스처 택하기.
	// nate 2004 - 7 - image
	lpSurface = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_WARGIANTMULTI_SMOKE ] ) ;
	GET_D3DDEVICE()->SetTexture(0, lpSurface);

	// 마지막 프레임 전까지 그림을 그린다.
	// 여기서 start_frame은 화이어볼의 꼬리쪽.
	int nTex = 0;
	float fire_scale = 5.0f;
	float fire_dir = 0.5f;
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
			fire_dir *= 1.35f;
		else
			fire_dir *= 1.55f;

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
		fire_scale -= 0.35f;
		fire_fade -= 0.05f;
	}

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, dwLighting);

	return bFinished;
}



