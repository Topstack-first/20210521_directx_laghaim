#include "stdafx.h"
#include <assert.h>
#include "sfx.h"
#include "main.h"
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

CFxRocket::CFxRocket()
{
	m_pLolo				= NULL;
}

CFxRocket::~CFxRocket()
{
	SAFE_DELETE(m_pLolo);
}

void CFxRocket::Create()
{}

// nate 2004 - 8 ImageManager
void CFxRocket::LoadRes()
{
	if (!m_pLolo)
	{
		m_pLolo = new CLolos;
		m_pLolo->SetIndexedTexture( g_pCapsyongTexture );
		m_pLolo->Read("data/effect/lolos/bb.LOL");

	}


}

void CFxRocket::DeleteRes()
{}

BOOL CFxRocket::Render(EffectSort &effect_sort)
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

	int nFrame, offset;
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

	// Ÿ�� ��ǥ�� ���Ѵ�.
	if (effect_sort.pMobTo)
		vPosTo = D3DVECTOR(effect_sort.pMobTo->m_wx, effect_sort.pMobTo->m_wy, effect_sort.pMobTo->m_wz);
	else if (effect_sort.pNkChaTo)
		vPosTo = D3DVECTOR(effect_sort.pNkChaTo->m_wx, effect_sort.pNkChaTo->m_wy + 15.0f, effect_sort.pNkChaTo->m_wz);
	else
		vPosTo = effect_sort.vTargetPos;

	if (effect_sort.pMobTo)
	{
		switch (effect_sort.pMobTo->m_Height)
		{
		case MOB_HEIGHT_HIGH_NUM:
			vPosTo.y += MOB_HEIGHT_HIGH_VAL;
			break;
		case MOB_HEIGHT_LOW_NUM:
			vPosTo.y += MOB_HEIGHT_LOW_VAL;
			break;
		default: // MOB_HEIGHT_MIDDLE_NUM:
			vPosTo.y += MOB_HEIGHT_MIDDLE_VAL;
			break;
		}
	}

	// ����.
	get_normal_vector(vDir, vPosFrom, vPosTo);

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

	if ( fDist <= 0.0f || fabsf(fDist) <= 10.0f)
	{
		// ���� ������ �� �¾Ҵٰ� �˷��ش�.
		// �׷����ν� ���� ���� ������ �ִ� effect_num�� �����ȴ�.
		if (effect_sort.pMobTo)
			effect_sort.pMobTo->Attacked();
		else if (effect_sort.pNkChaTo)
			effect_sort.pNkChaTo->Attacked();

		if (effect_sort.pMobTo)
		{
			switch (effect_sort.pMobTo->m_Height)
			{
			case MOB_HEIGHT_HIGH_NUM:
				vPosTo.y -= MOB_HEIGHT_HIGH_VAL;
				break;
			case MOB_HEIGHT_LOW_NUM:
				vPosTo.y -= MOB_HEIGHT_LOW_VAL;
				break;
			default: // MOB_HEIGHT_MIDDLE_NUM:
				vPosTo.y -= MOB_HEIGHT_MIDDLE_VAL;
				break;
			}
		}

		// ���� ȿ���� ȣ���Ѵ�.
		EffectSort ef_sort;
		ZeroMemory(&ef_sort, sizeof(EffectSort));
		ef_sort.nType = FX_FIREBALLBOOM;
		ef_sort.vPos = vPosFrom;
		ef_sort.vTargetPos = vPosTo;
		pCMyApp->m_pFxSet->InsertFxEffect(ef_sort);
		if (g_pDSound)
		{
			g_pDSound->Play(g_EffectSound[EFF_SND_BOOM1], (vPosTo.x- g_pRoh->m_wx)/g_fDSDistance, (vPosTo.y- g_pRoh->m_wy)/g_fDSDistance, (vPosTo.z-g_pRoh->m_wz)/g_fDSDistance);
		}

		bFinished = TRUE;
	}

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND,   D3DBLEND_SRCALPHA );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND,  D3DBLEND_INVSRCALPHA );

	float fDir = 0.0f;
	fDir = atan2f(vDir.z, vDir.x);
	Nk3DUtil_SetRotateCCWYMatrix(matRotate, fDir);

	D3DUtil_SetTranslateMatrix(matTrans, vDrawPos);
	D3DMath_MatrixMultiply( matWorld, matRotate, matTrans );

	D3DMATRIX		matS;
	float rand = 0.0f;

	if( effect_sort.Index == 1 )
		rand = 2.0f;
	else if( effect_sort.Index == 2 )
		rand = 3.0f;

	D3DUtil_SetScaleMatrix(matS, rand, rand, rand);

	D3DMath_MatrixMultiply(matWorld, matS, matWorld);
	GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &matWorld );

	m_pLolo->Render(GET_D3DDEVICE(), offset, FALSE, FALSE, FALSE, NULL, NULL, FALSE, FALSE);

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
	// �ؽ�ó ���ϱ�.
	// nate 2004 - 7 - image
	lpSurface = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_ROCKET_SMOKE ] ) ;
	GET_D3DDEVICE()->SetTexture(0, lpSurface);

	// ������ ������ ������ �׸��� �׸���.
	// ���⼭ start_frame�� ȭ�̾�� ������.
	int nTex = 0;
	float fire_scale = 3.5f;
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

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, dwLighting);
	return bFinished;
}
