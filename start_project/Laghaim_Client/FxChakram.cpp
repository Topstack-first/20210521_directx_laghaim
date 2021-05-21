#include "stdafx.h"
#define D3D_OVERLOADS

#include <d3d.h>
#include <time.h>
#include <assert.h>
#include "FxSet.h"
#include "FxChakram.h"
#include "main.h"
#include "UIMgr.h"
#include "D3DMath.h"
#include "D3DUtil.h"
#include "IndexedTexture.h"
#include "ShadowRect.h"
#include "WeatherMgr.h"
#include "DirtSnd.h"
#include "LoadEffectSound.h"
#include "CapSyong.h"
#include "GUtil.h"
#include "Capsyong.h"
#include "NkCharacter.h"
#include "NkMob.h"
#include "ItemProto.h"

CFxChakram::CFxChakram()
{
	m_fSpeed = 10.5f;
}

CFxChakram::~CFxChakram()
{}

void CFxChakram::LoadRes(IndexedTexture *pIndexedTexture, CShadowRect *pShadowRect)
{}

void CFxChakram::DeleteRes()
{}

//-- 2004.03.30
//-- Lyul / �����Ϸ�
//-- ������ �ϵ� ����
//-- g_ItemLolos[g_ItemList.m_ItemArray[effect_sort.subNum].m_LoloNum]; ����
//-- �ƹ� ȿ���� ����ߵ� ������ lolos �� �����̰� �� ���·� �ε��ȴ�.
//-- v_num �� 773 �� ��� (�����˿ϵ�) lolos �� ���ƹ���.
BOOL CFxChakram::Render(EffectSort &effect_sort)
{
	if( !GET_D3DDEVICE() )
		return TRUE;

	if(!effect_sort.pNkChaTo
			&& !effect_sort.pMobTo)
		return TRUE;

	DWORD dwLighting, dwZWrite;

	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_LIGHTING, &dwLighting);
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_ZWRITEENABLE, &dwZWrite);

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ZWRITEENABLE, TRUE);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_ONE);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_ONE);

	BOOL bFinished = FALSE;
	D3DMATRIX	matRotate, matTrans, matWorld;

	int nFrame = effect_sort.nCurFrame;

	// ���⺤�͸� ���Ѵ�.
	D3DVECTOR vDir, vPosFrom, vPosTo;
	D3DVECTOR vDrawPos;

	// ������ġ�� ����. ���� ��ü�� ������ ���� �ֱ� ������.
	vPosFrom = effect_sort.vPos;

	// Ÿ�� ��ǥ�� ���Ѵ�.
	if (effect_sort.pMobTo)
		vPosTo = D3DVECTOR(effect_sort.pMobTo->m_wx, effect_sort.pMobTo->m_wy, effect_sort.pMobTo->m_wz);
	else if (effect_sort.pNkChaTo)
		vPosTo = D3DVECTOR(effect_sort.pNkChaTo->m_wx, effect_sort.pNkChaTo->m_wy, effect_sort.pNkChaTo->m_wz);
	else
		vPosTo = effect_sort.vTargetPos;

	// ����.
	get_normal_vector(vDir, vPosFrom, vPosTo);
	float fDir = atan2f(vDir.z, vDir.x);

	Nk3DUtil_SetRotateCCWYMatrix(matRotate, fDir);

	// �����Ӵ� ���ǵ�.
	vDir *= m_fSpeed;
	vDrawPos.x = vPosFrom.x + vDir.x * nFrame;
	vDrawPos.y = vPosFrom.y + vDir.y * nFrame;
	vDrawPos.z = vPosFrom.z + vDir.z * nFrame;

	vDrawPos.y = vDrawPos.y + 10.0f;

	D3DUtil_SetTranslateMatrix(matTrans, vDrawPos);

	D3DMath_MatrixMultiply( matWorld, matRotate, matTrans );

	D3DMATRIX		matS;
	D3DUtil_SetScaleMatrix(matS, 3.0f, 3.0f, 3.0f);
	D3DMath_MatrixMultiply(matWorld, matS, matWorld);

	GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &matWorld );

	if (effect_sort.subNum >= 0
			&& effect_sort.subNum < g_ItemList.m_ItemCount )
	{
		// Ȥ�� ��ȣ�� �߸� �Ѿ�� ���� �ִ�.
		CLolos *pLolo = GET_LOLOC(effect_sort.subNum);
		if(pLolo && effect_sort.subNum != 773)
			pLolo->Render(GET_D3DDEVICE()); //-- lyul 04.03.30 ������(Bear) �ϵ�
	}

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

		if( pCMyApp->m_pUIMgr )
			pCMyApp->m_pUIMgr->InsertHitEffects(effect_sort.pNkChaFrom, effect_sort.pMobFrom, effect_sort.pNkChaTo, effect_sort.pMobTo, HIT_AXE_JUMP_ATTACK);
		bFinished = TRUE;
	}

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, dwLighting);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ZWRITEENABLE, dwZWrite);

	return bFinished;
}
