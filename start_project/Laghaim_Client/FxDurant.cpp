#include "stdafx.h"
#define D3D_OVERLOADS

#include <d3d.h>
#include <time.h>
#include <assert.h>
#include "FxSet.h"
#include "main.h"
#include "DirtSnd.h"
#include "LoadEffectSound.h"
#include "ImageDefine.h"
#include "GUtil.h"
#include "NkCharacter.h"
#include "NkMob.h"
#include "FxDurant.h"



extern float g_fDSDistance;

extern int g_EffectSound[];

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFxDurant::CFxDurant()
{
	m_pLolo = NULL;
	m_fSpeed = 5.0f;
	smoke = NULL;
	smoke = new CRectPlane;
	if( smoke )
		smoke->Create(15.0f, 15.0f, FALSE);
}

CFxDurant::~CFxDurant()
{
	DeleteRes();
}

void CFxDurant::DeleteRes()
{
	SAFE_DELETE(m_pLolo);
	SAFE_DELETE(smoke);
}

void CFxDurant::LoadRes()
{
	if (!m_pLolo)
	{
		m_pLolo = new CLolos;
		if( m_pLolo )
		{
			m_pLolo->SetIndexedTexture( g_pCapsyongTexture );
			m_pLolo->Read("data/effect/lolos/sd_osmkdl.LOL");
		}
	}



}


BOOL CFxDurant::Render(EffectSort &effect_sort)
{
	if( !GET_D3DDEVICE() )
		return true;

	if(!effect_sort.pMobFrom)
		return TRUE;

	BOOL bFinished = FALSE;

	LPDIRECTDRAWSURFACE7 lpSurface = NULL;
	D3DVECTOR vViewVec;
	D3DMATRIX matTrans, matView;
	D3DMATRIX matRx, matRy, matWorld;
	DWORD dwLighting, dwSrc, dwDest;

	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_LIGHTING, &dwLighting);
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_SRCBLEND,   &dwSrc );
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_DESTBLEND,  &dwDest );
	vViewVec = pCMyApp->GetEyeDirection();

	int nFrame;
	nFrame = effect_sort.nCurFrame;

	// 방향벡터를 구한다.
	D3DVECTOR vPosTo;
	D3DVECTOR vDrawPos;

	// 타겟 좌표를 구한다.
	vPosTo = D3DVECTOR(effect_sort.pMobFrom->m_wx, effect_sort.pMobFrom->m_wy, effect_sort.pMobFrom->m_wz);
	vDrawPos.x = vPosTo.x;
	vDrawPos.y = vPosTo.y;
	vDrawPos.z = vPosTo.z;

	if (m_pLolo)
	{
		//lolo 스케일 조절
		D3DUtil_SetTranslateMatrix( matWorld, vDrawPos.x, vDrawPos.y, vDrawPos.z);

		GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_WORLD, &matWorld );
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE);
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND,   D3DBLEND_SRCALPHA );
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND,  D3DBLEND_INVSRCALPHA );

		m_pLolo->Render(GET_D3DDEVICE(), effect_sort.nCurFrame, FALSE, FALSE);
	}

	/*	if (g_pDSound)
		{
			float x, y, z;
			x = (vDrawPos.x - g_pRoh->m_wx) / g_fDSDistance;
			y = (vDrawPos.y - g_pRoh->m_wy) / g_fDSDistance;
			z = (vDrawPos.z - g_pRoh->m_wz) / g_fDSDistance;
			g_pDSound->Play(g_EffectSound[EFF_SND_TONADO], x, y, z);
		}
	*/
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, dwLighting);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND,   dwSrc );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND,  dwDest );
//========================================================

	return bFinished;
}
