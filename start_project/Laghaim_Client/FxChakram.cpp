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
//-- Lyul / 수정완료
//-- 마법곰 완드 제거
//-- g_ItemLolos[g_ItemList.m_ItemArray[effect_sort.subNum].m_LoloNum]; 에서
//-- 아무 효과도 없어야될 마법검 lolos 가 곰돌이가 들어간 상태로 로딩된다.
//-- v_num 이 773 일 경우 (마법검완드) lolos 를 막아버림.
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

	// 방향.
	get_normal_vector(vDir, vPosFrom, vPosTo);
	float fDir = atan2f(vDir.z, vDir.x);

	Nk3DUtil_SetRotateCCWYMatrix(matRotate, fDir);

	// 프레임당 스피드.
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
		// 혹시 번호가 잘못 넘어올 수도 있다.
		CLolos *pLolo = GET_LOLOC(effect_sort.subNum);
		if(pLolo && effect_sort.subNum != 773)
			pLolo->Render(GET_D3DDEVICE()); //-- lyul 04.03.30 마법곰(Bear) 완드
	}

	float fDist = DistPlToPlAbs(vPosTo.x, vPosTo.z, vPosFrom.x, vPosFrom.z) - \
				  DistPlToPlAbs(vDrawPos.x, vDrawPos.z, vPosFrom.x, vPosFrom.z);

	if ( fDist <= 0.0f || fabsf(fDist) <= 10.0f)
	{
		// 맞은 놈한테 너 맞았다고 알려준다.
		// 그럼으로써 맞은 놈이 가지고 있는 effect_num이 삭제된다.
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
