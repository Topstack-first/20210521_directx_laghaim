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
#include "FxAttackMoveLolo.h"



extern int g_EffectSound[];
extern float g_fDSDistance;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFxAttack_MoveLolo::CFxAttack_MoveLolo()
{
	for( int i = 0 ; i < 2 ; i ++ )
		m_pLolo[i]	= NULL;
}

CFxAttack_MoveLolo::~CFxAttack_MoveLolo()
{
	DeleteRes();
}

void CFxAttack_MoveLolo::LoadRes()
{
	if( !m_pLolo[0] )
	{
		m_pLolo[0] = new CLolos;
		m_pLolo[0]->SetIndexedTexture( g_pCapsyongTexture );
		m_pLolo[0]->Read("data/effect/lolos/Thorn.LOL");
	}

	if( !m_pLolo[1] )
	{
		m_pLolo[1] = new CLolos;
		m_pLolo[1]->SetIndexedTexture( g_pCapsyongTexture );
		m_pLolo[1]->Read("data/effect/lolos/dandel.LOL");
	}
}

void CFxAttack_MoveLolo::DeleteRes()
{
	for( int i = 0 ; i < 2 ; i ++ )
		SAFE_DELETE(m_pLolo[i]);
}

BOOL CFxAttack_MoveLolo::Render(EffectSort &effect_sort)
{
	if( !GET_D3DDEVICE() )
		return true;

	if(!effect_sort.pNkChaTo
			&& !effect_sort.pMobTo)
		return TRUE;

	BOOL bFinished = FALSE;
	D3DMATRIX matTrans, matRotate, matWorld, matWorld_Old;
	DWORD dwLighting;

	GET_D3DDEVICE()->GetTransform( D3DTRANSFORMSTATE_WORLD, &matWorld_Old );
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_LIGHTING, &dwLighting);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE);

	// 방향벡터를 구한다.
	D3DVECTOR vDir, vPosFrom, vPosTo;
	D3DVECTOR vDrawPos;

	// 시작위치가 고정. 시전 객체가 움직일 수도 있기 때문에.
	vPosFrom = effect_sort.vPos;
	vPosFrom.y += 18.0f;

	// 타겟 좌표를 구한다.
	if (effect_sort.pMobTo)
		vPosTo = D3DVECTOR(effect_sort.pMobTo->m_wx, effect_sort.pMobTo->m_wy, effect_sort.pMobTo->m_wz);
	else if (effect_sort.pNkChaTo)
		vPosTo = D3DVECTOR(effect_sort.pNkChaTo->m_wx, effect_sort.pNkChaTo->m_wy + 12.0f, effect_sort.pNkChaTo->m_wz);
	else
		return false;

	// 방향 백터
	get_normal_vector(vDir, vPosFrom, vPosTo);

	// 프레임당 스피드.
	if( effect_sort.Index == 4004 )			// 캐크터스
		vDir *= 15.0f;
	else if( effect_sort.Index == 4005 )	// 댄더리온
		vDir *= 10.0f;

	vDrawPos.x = vPosFrom.x + vDir.x * ( effect_sort.nCurFrame + 1 );
	vDrawPos.y = vPosFrom.y + vDir.y * ( effect_sort.nCurFrame + 1 );
	vDrawPos.z = vPosFrom.z + vDir.z * ( effect_sort.nCurFrame + 1 );

	float fDist = DistPlToPlAbs(vPosTo.x, vPosTo.z, vPosFrom.x, vPosFrom.z) -
				  DistPlToPlAbs(vDrawPos.x, vDrawPos.z, vPosFrom.x, vPosFrom.z);

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND,   D3DBLEND_SRCALPHA );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND,  D3DBLEND_INVSRCALPHA );

	if ( fabsf(fDist) <= 10.0f )	// 목표지점 돋달
	{
		if( effect_sort.Index == 4004 )
			vDrawPos = vPosTo;
		else if( effect_sort.Index == 4005 )
			bFinished = true;
	}

	float fDir = atan2f(vDir.z, vDir.x);
	Nk3DUtil_SetRotateCCWYMatrix(matRotate, fDir);
	D3DUtil_SetTranslateMatrix(matTrans, vDrawPos);
	D3DMath_MatrixMultiply( matWorld, matRotate, matTrans );
	GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &matWorld );

	int nIndex = 0;
	switch( effect_sort.Index )
	{
	case 4004:
		nIndex = 0;
		break;	// 가시
	case 4005:
		nIndex = 1;
		break;	// 씨앗
	}
	if( m_pLolo[nIndex] )
		m_pLolo[nIndex]->Render( GET_D3DDEVICE() );

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, dwLighting);
	GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &matWorld_Old );
	return bFinished;
}
