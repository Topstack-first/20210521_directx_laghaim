#include "stdafx.h"
#define D3D_OVERLOADS

#include <d3d.h>
#include <time.h>
#include <assert.h>
#include "FxSet.h"
#include "main.h"
#include "D3DMath.h"
#include "D3DUtil.h"
#include "sfxUtil.h"
#include "SFXBaseShape.h"
#include "IndexedTexture.h"
#include "ShadowRect.h"
#include "WeatherMgr.h"
#include "DirtSnd.h"
#include "LoadEffectSound.h"
// nate 2004 - 7 - image
#include "ImageDefine.h"
#include "CapSyong.h"
#include "GUtil.h"
#include "Capsyong.h"
#include "NkCharacter.h"
#include "NkMob.h"
#include "FxStoneRance.h"


// nate 2004 - 7 글로벌 ShadowRect사용

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFxStoneRance2::CFxStoneRance2()
{
	m_pLolo = NULL;
}

CFxStoneRance2::~CFxStoneRance2()
{
	SAFE_DELETE(m_pLolo);
}

// nate 2004 - 8 ImageManager
void CFxStoneRance2::LoadRes()
{
	PFILE *fpLolo = NULL;
	if (!m_pLolo)
	{
		m_pLolo = new CLolos;
		m_pLolo->SetIndexedTexture( g_pCapsyongTexture );
		m_pLolo->Read("data/effect/lolos/nw_mop_007_att_fx.LOL");
	}



}

void CFxStoneRance2::DeleteRes()
{}

void CFxStoneRance2::Render(EffectSort &effect_sort)
{
	if( !GET_D3DDEVICE() )
		return;

	if(!effect_sort.pNkChaTo
			&& !effect_sort.pMobTo)
		return;

	LPDIRECTDRAWSURFACE7 lpSurface = NULL;
	D3DMATRIX matTrans, matView;
	DWORD dwLighting, dwZWrite;

	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_LIGHTING, &dwLighting);
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_ZWRITEENABLE, &dwZWrite);

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ZWRITEENABLE, TRUE);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE);

	D3DVECTOR vDrawPos;
	vDrawPos = effect_sort.vPos;

	if (m_pLolo)
	{
		D3DUtil_SetIdentityMatrix( matTrans );
		D3DUtil_SetTranslateMatrix( matTrans, effect_sort.vPos.x, effect_sort.vPos.y, effect_sort.vPos.z);
		pCMyApp->SetViewMatrix(matView);
		GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_WORLD, &matTrans );
		GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_VIEW, &matView);
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND,   D3DBLEND_SRCALPHA );
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND,  D3DBLEND_INVSRCALPHA );

		if (effect_sort.nCurFrame >= 6 && effect_sort.nCurFrame < 10)
		{
			if (effect_sort.pMobTo)
				effect_sort.pMobTo->Attacked();
			else if (effect_sort.pNkChaTo)
				effect_sort.pNkChaTo->Attacked();
		}
		m_pLolo->Render(GET_D3DDEVICE(), effect_sort.nCurFrame, FALSE, FALSE);

		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ZWRITEENABLE, FALSE);
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE);

		// nate 2004 - 7 글로벌 ShadowRect사용
		if( g_pShadowRect )
		{
			// nate 2004 - 7 - image
			lpSurface = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_STONERANCE2 ] ) ;
			// nate 2004 - 7 글로벌 ShadowRect사용
			g_pShadowRect->SetTileCoord(vDrawPos, 55 + (effect_sort.nCurFrame * 0.1f), FALSE);		// 부울값은 시간인가 아닌가
			g_pShadowRect->Render(lpSurface, FALSE);	// 부울값은 리얼그림자인가 아닌가?
		}
	}
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, dwLighting);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ZWRITEENABLE, dwZWrite);
}