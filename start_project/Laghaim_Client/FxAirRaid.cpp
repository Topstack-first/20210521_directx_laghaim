#include "stdafx.h"
#define D3D_OVERLOADS

#include <d3d.h>
#include <time.h>
#include <assert.h>
#include "FxSet.h"
#include "FxAirRaid.h"
#include "main.h"
#include "D3DMath.h"
#include "D3DUtil.h"
#include "IndexedTexture.h"
#include "WeatherMgr.h"
#include "DirtSnd.h"
#include "LoadEffectSound.h"
#include "CapSyong.h"
#include "GUtil.h"
#include "NkCharacter.h"
#include "NkMob.h"



// nate 2004 - 7 - image

extern float g_fTimeKey;
extern float g_fDSDistance;

extern int g_EffectSound[];
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFxAirRaid::CFxAirRaid()
{
	m_pLolo = NULL;
	m_pLolo2 = NULL;
	m_fSpeed = 10.0f;
}

CFxAirRaid::~CFxAirRaid()
{
	DeleteRes();
}

void CFxAirRaid::DeleteRes()
{
	SAFE_DELETE(m_pLolo);
	SAFE_DELETE(m_pLolo2);
}

// nate 2004 - 8 ImageManager
void CFxAirRaid::LoadRes()
{
	if (!m_pLolo)
	{
		m_pLolo = new CLolos;
		m_pLolo->SetIndexedTexture( g_pCapsyongTexture );
		m_pLolo->Read("data/effect/lolos/h_s_air.lol");
	}

	if (!m_pLolo2)
	{
		m_pLolo2 = new CLolos;
		m_pLolo2->SetIndexedTexture( g_pCapsyongTexture );
		m_pLolo2->Read("data/effect/lolos/h_s_air_bomb.LOL");
	}
}


BOOL CFxAirRaid::Render(EffectSort &effect_sort)
{
	if( !GET_D3DDEVICE() )
		return TRUE;

	if( !effect_sort.pNkChaTo
			&& !effect_sort.pMobTo)
		return TRUE;

	BOOL bFinished = FALSE;
	LPDIRECTDRAWSURFACE7 lpSurface = NULL;
	D3DMATRIX matTrans, matView;
	D3DMATRIX matRx, matRy, matWorld;
	D3DMATRIX matWorld1,matWorld2,matWorld3,matWorld4,matWorld5,matWorld6;
	D3DVECTOR vDir, vPosFrom, vPosTo,vDir2, vPosFrom2, vPosTo2;
	D3DVECTOR vViewVec, vDrawPos;
	D3DVECTOR vDrawPos1,vDrawPos2,vDrawPos3,vDrawPos4,vDrawPos5,vDrawPos6;
	vViewVec = pCMyApp->GetEyeDirection();

	int nFrame = 0;
	//비행기 이동 벡터
	float nX = 0,nZ = 0;
	nFrame = effect_sort.nCurFrame;

	float nPositionX,nPositionY,nPositionZ;
	float nPosMissleX,nPosMissleY,nPosMissleZ;

	// 메테오 마법은 시작위치가 고정. 시전 객체가 움직일 수도 있기 때문에.
	if (effect_sort.pMobTo)
	{
		nX=effect_sort.pMobTo->m_wx-effect_sort.vPos.x;
		nZ=effect_sort.pMobTo->m_wz-effect_sort.vPos.z;
	}
	else if (effect_sort.pNkChaTo)
	{
		nX=effect_sort.pNkChaTo->m_wx-effect_sort.vPos.x;
		nZ=effect_sort.pNkChaTo->m_wz-effect_sort.vPos.z;
	}

	vPosFrom.x = effect_sort.vPos.x-(nX*400.0f)/ sqrt( pow(nX,2)+pow(nZ,2) );
	vPosFrom.y = effect_sort.vPos.y+120.0f;
	vPosFrom.z =effect_sort.vPos.z-(nZ*400.0f)/ sqrt(pow(nX,2)+pow(nZ,2));

	// 타겟 좌표를 구한다.
	if (effect_sort.pMobTo)
	{
		vPosTo.x = effect_sort.pMobTo->m_wx+(nX*400.0f)/ sqrt( pow(nX,2)+pow(nZ,2) );
		vPosTo.y = effect_sort.pMobTo->m_wy;//-10.0f;
		vPosTo.z =effect_sort.pMobTo->m_wz+(nZ*400.0f)/ sqrt(pow(nX,2)+pow(nZ,2));
		vPosTo2 = D3DVECTOR(effect_sort.pMobTo->m_wx, effect_sort.pMobTo->m_wy, effect_sort.pMobTo->m_wz);
	}
	else if (effect_sort.pNkChaTo)
	{
		vPosTo.x = effect_sort.pNkChaTo->m_wx+(nX*400.0f)/ sqrt( pow(nX,2)+pow(nZ,2) );
		vPosTo.y = effect_sort.pNkChaTo->m_wy;//-10.0f;
		vPosTo.z =effect_sort.pNkChaTo->m_wz+(nZ*400.0f)/ sqrt(pow(nX,2)+pow(nZ,2));
		vPosTo2 = D3DVECTOR(effect_sort.pNkChaTo->m_wx, effect_sort.pNkChaTo->m_wy, effect_sort.pNkChaTo->m_wz);
	}
	else
	{
		vPosTo = effect_sort.vTargetPos;
		vPosTo2 = effect_sort.vTargetPos;
	}

	get_normal_vector(vDir, vPosFrom, vPosTo);
	// 프레임당 스피드.
	vDir *= m_fSpeed;
	// 현재 프레임에서 8프레임 전까지 그린다.
	int start_frame, end_frame;
	start_frame = nFrame ;		// 총 8프레임.
	end_frame = nFrame+50.0f;

	float ry = atan2f(nX, nZ);
	float rx = 0.0f ;

//--------------------------------------------------------------------------------------
// 비행기가 진행방향을 향하게
	D3DUtil_SetRotateXMatrix(matRx, rx);
	D3DUtil_SetRotateYMatrix(matRy, ry-1.57f);
	D3DMath_MatrixMultiply(matWorld, matRx, matRy);
//--------------------------------------------------------------------------------------
	D3DMATRIX		matS;
	D3DUtil_SetScaleMatrix(matS, 0.5f,0.5f, 0.5f);
	D3DMath_MatrixMultiply(matWorld, matS, matWorld);

//--------------------------------------------------------------------------------------
	nPositionX=vPosFrom.x +(vPosTo.x-vPosFrom.x) * (effect_sort.nCurFrame / (float)FX_AIRRAID_FRAMECOUNT);
	nPositionY=vPosFrom.y +(vPosTo.y-vPosFrom.y) * (effect_sort.nCurFrame / (float)FX_AIRRAID_FRAMECOUNT);
	nPositionZ=vPosFrom.z +(vPosTo.z-vPosFrom.z) * (effect_sort.nCurFrame / (float)FX_AIRRAID_FRAMECOUNT);
//--------------------------------------------------------------------------------------
	nPosMissleX = vPosFrom.x +(vPosTo.x-vPosFrom.x) *(0.45f +0.003f*(effect_sort.nCurFrame - (float)FX_AIRRAID_FRAMECOUNT/2) ) ;
	nPosMissleY = nPositionY - (effect_sort.nCurFrame - (float)FX_AIRRAID_FRAMECOUNT/2);
	nPosMissleZ = vPosFrom.z +(vPosTo.z-vPosFrom.z)*(0.45f +0.003f*(effect_sort.nCurFrame - (float)FX_AIRRAID_FRAMECOUNT/2) );
//--------------------------------------------------------------------------------------
//미샬................

	D3DUtil_SetRotateXMatrix(matRx, rx);
	D3DUtil_SetRotateYMatrix(matRy, ry-1.57f);
	D3DMath_MatrixMultiply(matWorld1, matRx, matRy);
	D3DMath_MatrixMultiply(matWorld2, matRx, matRy);
	D3DMath_MatrixMultiply(matWorld3, matRx, matRy);
	D3DMath_MatrixMultiply(matWorld4, matRx, matRy);
	D3DMath_MatrixMultiply(matWorld5, matRx, matRy);
	D3DMath_MatrixMultiply(matWorld6, matRx, matRy);
//--------------------------------------------------------------------------------------
	D3DMATRIX		matS2;
	D3DUtil_SetScaleMatrix(matS2, 0.15f,0.15f, 0.15f);
	D3DMath_MatrixMultiply(matWorld1, matS2, matWorld1);
	D3DMath_MatrixMultiply(matWorld2, matS2, matWorld2);
	D3DMath_MatrixMultiply(matWorld3, matS2, matWorld3);
	D3DMath_MatrixMultiply(matWorld4, matS2, matWorld4);
	D3DMath_MatrixMultiply(matWorld5, matS2, matWorld5);
	D3DMath_MatrixMultiply(matWorld6, matS2, matWorld6);
//--------------------------------------------------------------------------------------
	if(effect_sort.nCurFrame<(float)FX_AIRRAID_FRAMECOUNT/2)
	{
		vDrawPos.x = nPositionX;
		vDrawPos.y = nPositionY;
		vDrawPos.z = nPositionZ;

		matWorld1._41 = vDrawPos.x;
		matWorld1._42 = vDrawPos.y;
		matWorld1._43 = vDrawPos.z;
	}
	else //프레임의 절반부터 몹을 향해 수직 투하
	{
		vDrawPos1.x = nPosMissleX;
		vDrawPos1.y = nPosMissleY;
		vDrawPos1.z = nPosMissleZ;

		matWorld1._41 = vDrawPos1.x;
		matWorld1._42 = vDrawPos1.y;
		matWorld1._43 = vDrawPos1.z;
		if(effect_sort.nCurFrame>(float)FX_AIRRAID_FRAMECOUNT/2-3)
		{
			vDrawPos2.x = nPosMissleX+25.0f;
			vDrawPos2.y = nPosMissleY + 0.0f;
			vDrawPos2.z = nPosMissleZ+25.0f;
			matWorld2._41 = vDrawPos2.x;
			matWorld2._42 = vDrawPos2.y;
			matWorld2._43 = vDrawPos2.z;
		}
		if(effect_sort.nCurFrame>(float)FX_AIRRAID_FRAMECOUNT/2-1)
		{
			vDrawPos3.x = nPosMissleX-25.0f;
			vDrawPos3.y = nPosMissleY+ 5.0f;
			vDrawPos3.z = nPosMissleZ-25.0f;
			matWorld3._41 = vDrawPos3.x;
			matWorld3._42 = vDrawPos3.y;
			matWorld3._43 = vDrawPos3.z;
		}
		if(effect_sort.nCurFrame>(float)FX_AIRRAID_FRAMECOUNT/2+1)
		{
			vDrawPos4.x = nPosMissleX+10.0f;
			vDrawPos4.y = nPosMissleY;
			vDrawPos4.z = nPosMissleZ+10.0f;
			matWorld4._41 = vDrawPos4.x;
			matWorld4._42 = vDrawPos4.y;
			matWorld4._43 = vDrawPos4.z;
		}
		if(effect_sort.nCurFrame>(float)FX_AIRRAID_FRAMECOUNT/2+3)
		{
			vDrawPos5.x = nPosMissleX-25.0f;
			vDrawPos5.y = nPosMissleY+ 15.0f;
			vDrawPos5.z = nPosMissleZ+25.0f;
			matWorld5._41 = vDrawPos5.x;
			matWorld5._42 = vDrawPos5.y;
			matWorld5._43 = vDrawPos5.z;
		}
		if(effect_sort.nCurFrame>(float)FX_AIRRAID_FRAMECOUNT/2+5)
		{
			vDrawPos6.x = nPosMissleX+25.0f;
			vDrawPos6.y = nPosMissleY+ 20.0f;
			vDrawPos6.z = nPosMissleZ-25.0f;
			matWorld6._41 = vDrawPos6.x;
			matWorld6._42 = vDrawPos6.y;
			matWorld6._43 = vDrawPos6.z;
		}
		//미사일 투하 사운드
		if (g_pDSound)
		{
			float x, y, z;
			x = (vDrawPos.x - g_pRoh->m_wx) / g_fDSDistance;
			y = (vDrawPos.y - g_pRoh->m_wy) / g_fDSDistance;
			z = (vDrawPos.z - g_pRoh->m_wz) / g_fDSDistance;
			g_pDSound->Play(g_EffectSound[EFF_SND_BOMBDRP], x, 0, z);
		}
	}
	GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_WORLD, &matWorld1 );
//==================================================
	// nate2004 - 6
	// RenderState 버그 수정
	DWORD dwLighting, dwSrc, dwDest;

	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_LIGHTING, &dwLighting );
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_SRCBLEND,   &dwSrc );
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_DESTBLEND,  &dwDest );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA );
//==================================================

//--------------------------------------------------------------------------------------
	if (m_pLolo2)
	{
//========================================
		// nate2004 - 6
		// 랜더 스테이트는 밖에서 모드 끝낸다.
		if (effect_sort.nCurFrame >= 6 && effect_sort.nCurFrame < 10)
		{
			if (effect_sort.pMobTo)
			{
				effect_sort.pMobTo->Attacked();
			}
			else if (effect_sort.pNkChaTo)
			{
				effect_sort.pNkChaTo->Attacked();
			}
		}
		m_pLolo2->Render(GET_D3DDEVICE(), effect_sort.nCurFrame, FALSE, FALSE);

		GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_WORLD, &matWorld2 );
		m_pLolo2->Render(GET_D3DDEVICE(), effect_sort.nCurFrame, FALSE, FALSE);

		GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_WORLD, &matWorld3 );
		m_pLolo2->Render(GET_D3DDEVICE(), effect_sort.nCurFrame, FALSE, FALSE);

		GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_WORLD, &matWorld4 );
		m_pLolo2->Render(GET_D3DDEVICE(), effect_sort.nCurFrame, FALSE, FALSE);

		GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_WORLD, &matWorld5 );
		m_pLolo2->Render(GET_D3DDEVICE(), effect_sort.nCurFrame, FALSE, FALSE);

		GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_WORLD, &matWorld6 );
		m_pLolo2->Render(GET_D3DDEVICE(), effect_sort.nCurFrame, FALSE, FALSE);
	}
//--------------------------------------------------------------------------------------
	if(effect_sort.nCurFrame<(float)FX_AIRRAID_FRAMECOUNT/2)
	{
		vDrawPos.x = nPositionX;
		vDrawPos.y = nPositionY;
		vDrawPos.z = nPositionZ;
		matWorld._41 = vDrawPos.x;
		matWorld._42 = vDrawPos.y;
		matWorld._43 = vDrawPos.z;
	}
	else
	{
		vDrawPos.x = nPositionX;
		vDrawPos.y = nPositionY + (effect_sort.nCurFrame - ((float)FX_AIRRAID_FRAMECOUNT/2.0f)) * 5.0f;// + FlyingUp;
		vDrawPos.z = nPositionZ;
		matWorld._41 = vDrawPos.x;
		matWorld._42 = vDrawPos.y;
		matWorld._43 = vDrawPos.z;
	}
	GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_WORLD, &matWorld );

//--------------------------------------------------------------------------------------
	//날아가는 비행기................
	if (m_pLolo)
	{
		m_pLolo->Render(GET_D3DDEVICE(), effect_sort.nCurFrame, FALSE, FALSE);
	}
	//비행기 사운드
	if (g_pDSound&&effect_sort.nCurFrame<(float)FX_AIRRAID_FRAMECOUNT/0.7)
	{
		float x, y, z;
		x = (vDrawPos.x - g_pRoh->m_wx) / g_fDSDistance;
		y = (vDrawPos.y - g_pRoh->m_wy) / g_fDSDistance;
		z = (vDrawPos.z - g_pRoh->m_wz) / g_fDSDistance;
		g_pDSound->Play(g_EffectSound[EFF_SND_BOMBER], x, 0, z);
	}

//========================================================

//=====================================
	// nate 2004 - 6
	// RenderState 버그 수정
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, dwLighting );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND,   dwSrc );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND,  dwDest );
//=====================================
//--------------------------------------------------------------------------------------
	// 폭파 효과를 호출한다.
	EffectSort ef_sort;
	ZeroMemory(&ef_sort, sizeof(EffectSort));
	ef_sort.nType = FX_FIREBALLBOOM;
	if(effect_sort.nCurFrame>(float)FX_AIRRAID_FRAMECOUNT/2+6)
	{
		if(vDrawPos1.y-vPosTo2.y<5.0f)
		{
			ef_sort.vPos = vDrawPos1;
			ef_sort.vTargetPos = vDrawPos1;
			pCMyApp->m_pFxSet->InsertFxEffect(ef_sort);
			//미사일 폭파 사운드
			if (g_pDSound&&effect_sort.nCurFrame<(float)FX_AIRRAID_FRAMECOUNT/0.8)
			{
				float x, y, z;
				x = (vDrawPos.x - g_pRoh->m_wx) / g_fDSDistance;
				y = (vDrawPos.y - g_pRoh->m_wy) / g_fDSDistance;
				z = (vDrawPos.z - g_pRoh->m_wz) / g_fDSDistance;
				g_pDSound->Play(g_EffectSound[EFF_SND_MISSLEBOMB], x, y, z);
			}
		}
		if(vDrawPos2.y-vPosTo2.y<5.0f)
		{
			ef_sort.vPos = vDrawPos2;
			ef_sort.vTargetPos = vDrawPos2;
			pCMyApp->m_pFxSet->InsertFxEffect(ef_sort);
			//미사일 폭파 사운드
			if (g_pDSound&&effect_sort.nCurFrame<(float)FX_AIRRAID_FRAMECOUNT/0.8)
			{
				float x, y, z;
				x = (vDrawPos.x - g_pRoh->m_wx) / g_fDSDistance;
				y = (vDrawPos.y - g_pRoh->m_wy) / g_fDSDistance;
				z = (vDrawPos.z - g_pRoh->m_wz) / g_fDSDistance;
				g_pDSound->Play(g_EffectSound[EFF_SND_MISSLEBOMB], x, y, z);
			}
		}
		if(vDrawPos3.y-vPosTo2.y<5.0f)
		{
			ef_sort.vPos = vDrawPos3;
			ef_sort.vTargetPos = vDrawPos3;
			pCMyApp->m_pFxSet->InsertFxEffect(ef_sort);
			//미사일 폭파 사운드
			if (g_pDSound&&effect_sort.nCurFrame<(float)FX_AIRRAID_FRAMECOUNT/0.8)
			{
				float x, y, z;
				x = (vDrawPos.x - g_pRoh->m_wx) / g_fDSDistance;
				y = (vDrawPos.y - g_pRoh->m_wy) / g_fDSDistance;
				z = (vDrawPos.z - g_pRoh->m_wz) / g_fDSDistance;
				g_pDSound->Play(g_EffectSound[EFF_SND_MISSLEBOMB], x, y, z);
			}
		}
		if(vDrawPos4.y-vPosTo2.y<5.0f)
		{
			ef_sort.vPos = vDrawPos4;
			ef_sort.vTargetPos = vDrawPos4;
			pCMyApp->m_pFxSet->InsertFxEffect(ef_sort);
			//미사일 폭파 사운드
			if (g_pDSound&&effect_sort.nCurFrame<(float)FX_AIRRAID_FRAMECOUNT/0.8)
			{
				float x, y, z;
				x = (vDrawPos.x - g_pRoh->m_wx) / g_fDSDistance;
				y = (vDrawPos.y - g_pRoh->m_wy) / g_fDSDistance;
				z = (vDrawPos.z - g_pRoh->m_wz) / g_fDSDistance;
				g_pDSound->Play(g_EffectSound[EFF_SND_MISSLEBOMB], x, y, z);
			}
		}
		if(vDrawPos5.y-vPosTo2.y<5.0f)
		{
			ef_sort.vPos = vDrawPos5;
			ef_sort.vTargetPos = vDrawPos5;
			pCMyApp->m_pFxSet->InsertFxEffect(ef_sort);
			//미사일 폭파 사운드
			if (g_pDSound&&effect_sort.nCurFrame<(float)FX_AIRRAID_FRAMECOUNT/0.8)
			{
				float x, y, z;
				x = (vDrawPos.x - g_pRoh->m_wx) / g_fDSDistance;
				y = (vDrawPos.y - g_pRoh->m_wy) / g_fDSDistance;
				z = (vDrawPos.z - g_pRoh->m_wz) / g_fDSDistance;
				g_pDSound->Play(g_EffectSound[EFF_SND_MISSLEBOMB], x, y, z);
			}
		}
		if(vDrawPos6.y-vPosTo2.y<5.0f)
		{
			ef_sort.vPos = vDrawPos6;
			ef_sort.vTargetPos = vDrawPos6;
			pCMyApp->m_pFxSet->InsertFxEffect(ef_sort);
			//미사일 폭파 사운드
			if (g_pDSound&&effect_sort.nCurFrame<(float)FX_AIRRAID_FRAMECOUNT/0.8)
			{
				float x, y, z;
				x = (vDrawPos.x - g_pRoh->m_wx) / g_fDSDistance;
				y = (vDrawPos.y - g_pRoh->m_wy) / g_fDSDistance;
				z = (vDrawPos.z - g_pRoh->m_wz) / g_fDSDistance;
				g_pDSound->Play(g_EffectSound[EFF_SND_MISSLEBOMB], x, y, z);
			}
		}
	}
//--------------------------------------------------------------------------------------

	// 3차원 공간상에서 거리를 재야 한다.
	float fDist = Dist3PTo3P(vPosTo.x, vPosTo.y, vPosTo.z, vPosFrom.x, vPosFrom.y, vPosFrom.z) - \
				  Dist3PTo3P(vDrawPos.x, vDrawPos.y, vDrawPos.z, vPosFrom.x, vPosFrom.y, vPosFrom.z);

	if ( fDist <= 0.0f || fabsf(fDist) <= 10.0f)
	{
		// 맞은 놈한테 너 맞았다고 알려준다.
		// 그럼으로써 맞은 놈이 가지고 있는 effect_num이 삭제된다.
		if (effect_sort.pMobTo)
		{
			effect_sort.pMobTo->Attacked();
		}
		else if (effect_sort.pNkChaTo)
		{
			effect_sort.pNkChaTo->Attacked();
		}
		vDrawPos = vPosTo;

		matWorld1._41 = vDrawPos1.x;
		matWorld1._42 = vDrawPos1.y;
		matWorld1._43 = vDrawPos1.z;

	}

	return bFinished;
}

