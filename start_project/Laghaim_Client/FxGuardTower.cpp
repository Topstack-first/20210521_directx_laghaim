#include "stdafx.h"
#include "sfx.h"
#include "main.h"
#include "Land.h"
#include "NkCharacter.h"
#include "NkMob.h"
#include "GUtil.h"
#include "DirtSnd.h"
#include "LoadEffectSound.h"
#include "ImageDefine.h"


extern int g_EffectSound[];
extern float g_fDSDistance;



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFxGuardTower::CFxGuardTower()
	: m_nTotalFrame(0)
{
	head				= NULL;
	Create();
}

CFxGuardTower::~CFxGuardTower()
{
	SAFE_DELETE(head);
}


void CFxGuardTower::Create()
{
	SAFE_DELETE(head);

	head = new CRectPlane;
	if( head )
		head->Create( 10.0f, 10.0f, FALSE );
}


void CFxGuardTower::LoadRes(IndexedTexture *pIndexedTexture)
{}
void CFxGuardTower::DeleteRes()
{}

BOOL CFxGuardTower::Render(EffectSort &effect_sort)
{
	if( !GET_D3DDEVICE() )
		return true;

	if(!effect_sort.pNkChaTo
			&& !effect_sort.pMobTo)
		return TRUE;

	BOOL bFinished = FALSE;
	D3DMATRIX matTrans, matView, matRotate, matWorld;
	D3DMATRIX matRx, matRy;
	DWORD dwLighting;

	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_LIGHTING, &dwLighting);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_ONE);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_ONE);

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
	vPosFrom = effect_sort.vPos;
	vPosFrom.y += 85.0f;

	// 타겟 좌표를 구한다.
	if (effect_sort.pMobTo)
		vPosTo = D3DVECTOR(effect_sort.pMobTo->m_wx, effect_sort.pMobTo->m_wy, effect_sort.pMobTo->m_wz);
	else if (effect_sort.pNkChaTo)
		vPosTo = D3DVECTOR(effect_sort.pNkChaTo->m_wx, effect_sort.pNkChaTo->m_wy, effect_sort.pNkChaTo->m_wz);
	else
		vPosTo = effect_sort.vTargetPos;

	// 방향.
	get_normal_vector(vDir, vPosFrom, vPosTo);

	// 프레임당 스피드.
	vDir *= 6.0f;

	// 현재 프레임에서 8프레임 전까지 그린다.
	int start_frame, end_frame;

	start_frame = nFrame - 8;		// 총 8프레임.
	if (start_frame < 0)
		start_frame = 0;
	end_frame = nFrame;

	D3DVECTOR	vViewVec;
	vViewVec = pCMyApp->GetEyeDirection();

	float ry = atan2f(vViewVec.x, vViewVec.z);
	float rx = atan2f(vViewVec.x*sinf(ry)+vViewVec.z*cosf(ry), vViewVec.y) - (g_PI / 2.0f) ;

	D3DUtil_SetRotateXMatrix(matRx, rx);
	D3DUtil_SetRotateYMatrix(matRy, ry);
	D3DMath_MatrixMultiply(matWorld, matRx, matRy);

	LPDIRECTDRAWSURFACE7 lpSurface = NULL;
	// nate 2004 - 7 - image
	if( g_pCapsyongTexture )
		lpSurface = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_GUARDTOWER_SMOKE ] );
	GET_D3DDEVICE()->SetTexture(0, lpSurface);

	// 마지막 프레임 전까지 그림을 그린다.
	// 여기서 start_frame은 화이어볼의 꼬리쪽.
	int nTex = 0;
	float fire_scale = 4.5f;
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
		fire_scale -= 0.3f;
		fire_fade -= 0.05f;
	}

	vDrawPos.x = vPosFrom.x + vDir.x * (end_frame+1);
	vDrawPos.y = vPosFrom.y + vDir.y * (end_frame+1);
	vDrawPos.z = vPosFrom.z + vDir.z * (end_frame+1);

	float fDist = DistPlToPlAbs(vPosTo.x, vPosTo.z, vPosFrom.x, vPosFrom.z) -
				  DistPlToPlAbs(vDrawPos.x, vDrawPos.z, vPosFrom.x, vPosFrom.z);

	float fDist2 = vDrawPos.y - (pCMyApp->GetLandPtr()->GetHFHeight(vPosTo.x, vPosTo.z));

	if ( (fDist <= 0.0f || fabsf(fDist) <= 10.0f) &&
			(fDist2 <= 0.0f || fabsf(fDist2) <= 10.0f))
	{
		// nate 2004 - 7 - image
		if( g_pCapsyongTexture )
			lpSurface = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_QUARDTOWER_HEAD ] );
		GET_D3DDEVICE()->SetTexture(0, lpSurface);
		vDrawPos = vPosTo;

		if( head )
		{
			head->SetColor(1.0f, 0.55f, 0.55f, 1.0f);
			head->SetBillBoard(vViewVec, FALSE, 0.0f);
			head->SetTranslationXYZ(vDrawPos.x, vDrawPos.y, vDrawPos.z);
			head->Render();
		}

		// 맞은 놈한테 너 맞았다고 알려준다.
		// 그럼으로써 맞은 놈이 가지고 있는 effect_num이 삭제된다.
		if (effect_sort.pMobTo)
			effect_sort.pMobTo->Attacked();
		else if (effect_sort.pNkChaTo)
			effect_sort.pNkChaTo->Attacked();

		bFinished = TRUE;

		EffectSort ef_sort;
		ZeroMemory(&ef_sort, sizeof(EffectSort));

		ef_sort.nType = FX_FIREBALLBOOM2;
		ef_sort.vPos = vPosFrom;
		ef_sort.vTargetPos = vPosTo;
		if( pCMyApp->m_pFxSet )
			pCMyApp->m_pFxSet->InsertFxEffect(ef_sort);
		if (g_pDSound)
			g_pDSound->Play(g_EffectSound[EFF_SND_BOOM1], (vPosTo.x- g_pRoh->m_wx)/g_fDSDistance, (vPosTo.y- g_pRoh->m_wy)/g_fDSDistance, (vPosTo.z-g_pRoh->m_wz)/g_fDSDistance);
	}
	else
	{
		// nate 2004 - 7 - image
		if( g_pCapsyongTexture )
			lpSurface = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_QUARDTOWER_HEAD ] );
		GET_D3DDEVICE()->SetTexture(0, lpSurface);

		if( head )
		{
			head->SetColor(1.0f, 0.55f, 0.55f, 1.0f);
			head->SetBillBoard(vViewVec, FALSE, 0.0f);
			head->SetTranslationXYZ(vDrawPos.x, vDrawPos.y, vDrawPos.z);
			head->Render();
		}
	}

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, dwLighting);

	return bFinished;
}

