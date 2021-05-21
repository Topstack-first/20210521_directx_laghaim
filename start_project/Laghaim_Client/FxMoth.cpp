#include "stdafx.h"
#define D3D_OVERLOADS

#include <d3d.h>
#include "FxSet.h"
#include "FxMoth.h"
#include "ShadowRect.h"
#include "main.h"
#include "Capsyong.h"
#include "IndexedTexture.h"
#include "ShadowRect.h"
#include "GUtil.h"
#include "DirtSnd.h"
#include "LoadEffectSound.h"
#include "NkCharacter.h"
#include "NkMob.h"
#include "D3DMath.h"



extern float g_fTimeKey;
extern float g_fDSDistance;

extern int g_EffectSound[];

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFxMoth::CFxMoth()
{
	m_pLolo = NULL;
	m_fSpeed = 3.5f;
}

CFxMoth::~CFxMoth()
{}

void CFxMoth::LoadRes()
{
	SAFE_DELETE(m_pLolo);
	m_pLolo = new CLolos( g_pCapsyongTexture );
	m_pLolo->Read("data//effect//lolos//bfly-2.lol");



}

void CFxMoth::DeleteRes()
{
	SAFE_DELETE(m_pLolo);
}

BOOL CFxMoth::Render(EffectSort &effect_sort)
{
	if( !GET_D3DDEVICE() )
		return true;

	if(!effect_sort.pNkChaTo
			&& !effect_sort.pMobTo)
		return TRUE;

	DWORD dwLighting, dwZWrite;

	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_LIGHTING, &dwLighting);
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_ZWRITEENABLE, &dwZWrite);

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ZWRITEENABLE, TRUE);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE);

	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA);


	BOOL bFinished = FALSE;
	D3DMATRIX	matRotate, matTrans, matWorld;

	int nFrame, offset;
	nFrame = effect_sort.nCurFrame;

	if (nFrame == 0)
		offset = 0;
	else
		offset = nFrame % 11;			// 나비움직임의 프레임수.

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

	vDrawPos.y = vDrawPos.y;

	D3DUtil_SetTranslateMatrix(matTrans, vDrawPos);

	D3DMath_MatrixMultiply( matWorld, matRotate, matTrans );

	GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &matWorld );

	if( m_pLolo )
		m_pLolo->Render(GET_D3DDEVICE(), offset, FALSE, FALSE, FALSE, NULL, NULL, FALSE, FALSE);

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

		bFinished = TRUE;
	}

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, dwLighting);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ZWRITEENABLE, dwZWrite);

	return bFinished;
}