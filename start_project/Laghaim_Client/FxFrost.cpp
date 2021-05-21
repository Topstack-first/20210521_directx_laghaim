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

CFxFrost::CFxFrost()
{
	m_pLolo = NULL;
}

CFxFrost::~CFxFrost()
{
	SAFE_DELETE(m_pLolo);
}

// nate 2004 - 8 ImageManager
void CFxFrost::LoadRes()
{
	PFILE *fpLolo = NULL;
	if (!m_pLolo)
	{
		m_pLolo = new CLolos;
		m_pLolo->SetIndexedTexture( g_pCapsyongTexture );
		m_pLolo->Read("data/effect/lolos/skill_ice.lol");
	}



}

void CFxFrost::DeleteRes()
{}

void CFxFrost::Render(EffectSort &effect_sort)
{
	if( !GET_D3DDEVICE() )
		return;

	LPDIRECTDRAWSURFACE7 lpSurface = NULL;
	D3DMATRIX matTrans, matView;
	DWORD dwLighting, dwZWrite;

	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_LIGHTING, &dwLighting);
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_ZWRITEENABLE, &dwZWrite);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ZWRITEENABLE, TRUE);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE);

	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA );
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHAFUNC, D3DCMP_GREATER);

	D3DVECTOR vDrawPos;
	vDrawPos = effect_sort.vPos;

	if (m_pLolo)
	{
		D3DUtil_SetIdentityMatrix( matTrans );
		D3DUtil_SetTranslateMatrix( matTrans, effect_sort.vPos.x, effect_sort.vPos.y, effect_sort.vPos.z);
		pCMyApp->SetViewMatrix(matView);
		GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_WORLD, &matTrans );
		GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_VIEW, &matView);

		DWORD alpha;
		alpha = (DWORD)(0xff * ((FX_FROST_FRAMECOUNT - (effect_sort.nCurFrame*0.75f))*0.05f) ) << 24;

		DWORD alphaArg1, alphaArg2, alphaRef, alphaOp;
		GET_D3DDEVICE()->GetTextureStageState(0, D3DTSS_ALPHAARG1, &alphaArg1);
		GET_D3DDEVICE()->GetTextureStageState(0, D3DTSS_ALPHAARG2, &alphaArg2);
		GET_D3DDEVICE()->GetTextureStageState(0, D3DTSS_ALPHAOP,  &alphaOp);
		GET_D3DDEVICE()->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		GET_D3DDEVICE()->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
		GET_D3DDEVICE()->SetTextureStageState(0, D3DTSS_ALPHAOP,  D3DTOP_MODULATE);
		GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_ALPHAREF   , &alphaRef);
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHAREF   , 0x00000010);

		m_pLolo->Render(GET_D3DDEVICE(), 12, FALSE, FALSE, FALSE,
						NULL, NULL, FALSE, FALSE, alpha);

		GET_D3DDEVICE()->SetTextureStageState(0, D3DTSS_ALPHAARG1, alphaArg1);
		GET_D3DDEVICE()->SetTextureStageState(0, D3DTSS_ALPHAARG2, alphaArg2);
		GET_D3DDEVICE()->SetTextureStageState(0, D3DTSS_ALPHAOP, alphaOp);
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHAREF  , alphaRef);

		// nate 2004 - 7 글로벌 ShadowRect사용
		if( g_pShadowRect )
		{
			// nate 2004 - 7 - image
			if( g_pCapsyongTexture )
				lpSurface = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_FROST ] ) ;
			// nate 2004 - 7 글로벌 ShadowRect사용
			g_pShadowRect->SetTileCoord(vDrawPos, 35, FALSE);		// 부울값은 시간인가 아닌가
			g_pShadowRect->Render(lpSurface, FALSE);	// 부울값은 리얼그림자인가 아닌가?
		}
	}

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, dwLighting);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ZWRITEENABLE, dwZWrite);
}