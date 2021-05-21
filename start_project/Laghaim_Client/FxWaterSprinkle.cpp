#include "stdafx.h"
#define D3D_OVERLOADS
#include <d3d.h>
#include "FxSet.h"
#include "FxWaterSprinkle.h"
#include "ShadowRect.h"
#include "main.h"
#include "Capsyong.h"
#include "IndexedTexture.h"
#include "ShadowRect.h"
#include "GUtil.h"
#include "DirtSnd.h"
#include "LoadEffectSound.h"
#include "ImageDefine.h"
#include "NkCharacter.h"
#include "NkMob.h"
#include "D3DMath.h"


extern float g_fTimeKey;
extern float g_fDSDistance;

extern int g_EffectSound[];

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFxWaterSprinkle::CFxWaterSprinkle()
{
	m_fSize = 3.0f;
	m_fSpeed = 4.5f;
}

CFxWaterSprinkle::~CFxWaterSprinkle()
{
	DeleteRes();
}

void CFxWaterSprinkle::LoadRes(IndexedTexture *pIndexedTexture, CShadowRect *pShadowRect)
{}
void CFxWaterSprinkle::DeleteRes()
{}

BOOL CFxWaterSprinkle::Render(EffectSort &effect_sort)
{
	if( !GET_D3DDEVICE() )
		return true;

	if(!effect_sort.pNkChaTo
			&& !effect_sort.pMobTo)
		return TRUE;

	LPDIRECTDRAWSURFACE7 lpSurface = NULL;
	D3DVECTOR vViewVec;
	D3DMATRIX matRx, matRy, matWorld;
	unsigned int offset;
	int i;
	BOOL bFinished = FALSE;

	vViewVec = pCMyApp->GetEyeDirection();

	float ry = atan2f(vViewVec.x, vViewVec.z);
	float rx = atan2f(vViewVec.x*sinf(ry)+vViewVec.z*cosf(ry), vViewVec.y) - (g_PI / 2.0f) ;

	D3DUtil_SetRotateXMatrix(matRx, rx);
	D3DUtil_SetRotateYMatrix(matRy, ry);
	D3DMath_MatrixMultiply(matWorld, matRx, matRy);

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND,   D3DBLEND_SRCALPHA );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND,  D3DBLEND_INVSRCALPHA );

	int nFrame;
	nFrame = effect_sort.nCurFrame;

	if (nFrame == 0)
		offset = 0;
	else
		offset = nFrame % 11;

	D3DVECTOR vDir, vPosFrom, vPosTo;
	D3DVECTOR vDrawPos;

	vPosFrom = effect_sort.vPos;

	if (effect_sort.pMobTo)
		vPosTo = D3DVECTOR(effect_sort.pMobTo->m_wx, effect_sort.pMobTo->m_wy, effect_sort.pMobTo->m_wz);
	else if (effect_sort.pNkChaTo)
		vPosTo = D3DVECTOR(effect_sort.pNkChaTo->m_wx, effect_sort.pNkChaTo->m_wy, effect_sort.pNkChaTo->m_wz);
	else
		vPosTo = effect_sort.vTargetPos;

	get_normal_vector(vDir, vPosFrom, vPosTo);

	vDir *= m_fSpeed;

	int start_frame, end_frame;

	start_frame = nFrame - 8;		// 총 8프레임.
	if (start_frame < 0)
		start_frame = 0;
	end_frame = nFrame;

	if( g_pCapsyongTexture )
		lpSurface = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_WTRSPRINKLETAIL ] );
	GET_D3DDEVICE()->SetTexture(0, lpSurface);

	float WTRSprinkleHeight = 0.0f;

	if(effect_sort.pNkChaFrom )
	{
		switch( effect_sort.pNkChaFrom->m_Race )
		{
		case RACE_BULKAN:
			WTRSprinkleHeight = (effect_sort.pNkChaFrom->m_Sex == 0)? 17.0f : 14.0f ;
			break;
		case RACE_KAILIPTON:
			WTRSprinkleHeight = (effect_sort.pNkChaFrom->m_Sex == 0)? 17.0f : 14.0f ;
			break;
		case RACE_AIDIA:
			WTRSprinkleHeight = (effect_sort.pNkChaFrom->m_Sex == 0)? 10.0f : 10.0f ;
			break;
		case RACE_HUMAN:
			WTRSprinkleHeight = (effect_sort.pNkChaFrom->m_Sex == 0)? 17.0f : 14.0f ;
			break;
		case RACE_FREAK:
			WTRSprinkleHeight = (effect_sort.pNkChaFrom->m_Sex == 0)? 19.0f : 16.0f ;
			break;
		case RACE_PEROM:
			WTRSprinkleHeight = (effect_sort.pNkChaFrom->m_Sex == 0)? 19.0f : 16.0f ;
			break;
		}
	}

	int nTex = 0;
	for (i= end_frame - 1; i >= start_frame; i--)
	{
		vDrawPos.x = vPosFrom.x + vDir.x * (i+1);
		vDrawPos.y = vPosFrom.y + vDir.y * (i+1);
		vDrawPos.z = vPosFrom.z + vDir.z * (i+1);

		matWorld._41 = vDrawPos.x;
		matWorld._42 = vDrawPos.y + WTRSprinkleHeight;
		matWorld._43 = vDrawPos.z;

		GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &matWorld );

		MakeLVertexRect(m_lverTail, m_fSize, m_fSize, TRUE);

		m_lverTail[0].tu = nTex * 0.125f;
		m_lverTail[1].tu = nTex * 0.125f;
		m_lverTail[2].tu = (nTex+1) * 0.125f;
		m_lverTail[3].tu = (nTex+1) * 0.125f;
		GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_LVERTEX,
										m_lverTail, 4, 0 );
		nTex++;
	}

	// 마지막 프레임이 목적지에 도착했는가 확인? - 거리를 측정해서 한다.
	vDrawPos.x = vPosFrom.x + vDir.x * (end_frame+1);
	vDrawPos.y = vPosFrom.y + vDir.y * (end_frame+1);
	vDrawPos.z = vPosFrom.z + vDir.z * (end_frame+1);

	float fDist = DistPlToPlAbs(vPosTo.x, vPosTo.z, vPosFrom.x, vPosFrom.z) - \
				  DistPlToPlAbs(vDrawPos.x, vDrawPos.z, vPosFrom.x, vPosFrom.z);

	if ( fDist <= 0.0f || fabsf(fDist) <= 10.0f)
	{
		if (effect_sort.pMobTo)
			effect_sort.pMobTo->Attacked();
		else if (effect_sort.pNkChaTo)
			effect_sort.pNkChaTo->Attacked();

		vDrawPos = vPosTo;
		lpSurface = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_WTRSPRINKLEHEAD ] );
		GET_D3DDEVICE()->SetTexture(0, lpSurface);

		matWorld._41 = vDrawPos.x;
		matWorld._42 = vDrawPos.y + WTRSprinkleHeight;
		matWorld._43 = vDrawPos.z;

		MakeLVertexRect(m_lverHead, m_fSize*2.5f, m_fSize*2.5f, TRUE);

		GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &matWorld );

		GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_LVERTEX,
										m_lverHead, 4, 0 );
		bFinished = TRUE;

		EffectSort ef_sort;
		ef_sort.nType = FX_WTRSPRINKLEBOOM;
		ef_sort.vPos = vPosFrom;
		ef_sort.vTargetPos = vPosTo;
		if( pCMyApp->m_pFxSet )
			pCMyApp->m_pFxSet->InsertFxEffect(ef_sort);
	}
	else
	{
		// 선두.
		matWorld._41 = vDrawPos.x;
		matWorld._42 = vDrawPos.y + WTRSprinkleHeight;
		matWorld._43 = vDrawPos.z;

		GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &matWorld );

		m_lverHead[0] = D3DLVERTEX( D3DVECTOR(-m_fSize*1.5f, -m_fSize*1.5f, 0), 0xffffffff, 0, 0.0f, 1.0f );
		m_lverHead[1] = D3DLVERTEX( D3DVECTOR(-m_fSize*1.5f, m_fSize*1.5f, 0), 0xffffffff, 0, 0.0f, 0.0f );
		m_lverHead[2] = D3DLVERTEX( D3DVECTOR( m_fSize*1.5f, -m_fSize*1.5f, 0), 0xffffffff, 0, 1.0f, 1.0f );
		m_lverHead[3] = D3DLVERTEX( D3DVECTOR( m_fSize*1.5f, m_fSize*1.5f, 0), 0xffffffff, 0, 1.0f, 0.0f );

		// nate 2004 - 7 - image
		if( g_pCapsyongTexture )
			lpSurface = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_WTRSPRINKLEHEAD ] );
		GET_D3DDEVICE()->SetTexture(0, lpSurface);

		GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_LVERTEX,
										m_lverHead, 4, 0 );
	}

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE);

	return bFinished;
}

