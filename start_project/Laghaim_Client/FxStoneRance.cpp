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

CFxStoneRance::CFxStoneRance()
{
	m_pLolo = NULL;
	m_pLolo2 = NULL;	// nate 2005-06-16 : Burning Crash - 돌튀는 이펙트 롤로 초기화
	m_numTEng = 0;

	smoke = NULL;
	smoke = new CRectPlane;
	if( smoke )
		smoke->Create(15.0f, 15.0f, FALSE);
}

CFxStoneRance::~CFxStoneRance()
{
	DeleteRes();
}

void CFxStoneRance::LoadRes()
{
	PFILE *fpLolo = NULL;

	// nate 2005-06-16 : Burning Crash - 돌 솢구치는 롤로
	if (!m_pLolo)
	{
		m_pLolo = new CLolos;
		m_pLolo->SetIndexedTexture( g_pCapsyongTexture );
		m_pLolo->Read("data/effect/lolos/stone.lol");
	}
	if( !m_pLolo2 )
	{
		m_pLolo2 = new CLolos;
		m_pLolo2->SetIndexedTexture( g_pCapsyongTexture );
		m_pLolo2->Read( "data/effect/lolos/b_skill_st_00.LOL" );
	}



}

void CFxStoneRance::DeleteRes()
{
	SAFE_DELETE( smoke );
	SAFE_DELETE( m_pLolo );
	SAFE_DELETE( m_pLolo2 );
}

void CFxStoneRance::Render(EffectSort &effect_sort)
{
	if( !GET_D3DDEVICE() )
		return;

	if( effect_sort.Index != 2 ) // 인던 보스 레그녹 용 이펙트일 경우 예외처리 ///인던1
	{
		if( !effect_sort.pNkChaTo
				&& !effect_sort.pMobTo )
			return;
	}

	LPDIRECTDRAWSURFACE7 lpSurface = NULL;
	D3DMATRIX matTrans, matView;
	DWORD dwLighting, dwZWrite, dwAlpha;

	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_LIGHTING, &dwLighting);
	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, &dwAlpha);
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_ZWRITEENABLE, &dwZWrite);

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ZWRITEENABLE, TRUE);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, FALSE);

	D3DVECTOR vDrawPos;
	vDrawPos = effect_sort.vPos;

	if( ++m_numTEng >= 4 )
		m_numTEng = 3;

	D3DUtil_SetIdentityMatrix( matTrans );
	D3DUtil_SetTranslateMatrix( matTrans, effect_sort.vPos.x, effect_sort.vPos.y, effect_sort.vPos.z);
	pCMyApp->SetViewMatrix(matView);
	GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_WORLD, &matTrans );
	GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_VIEW, &matView);

	if( effect_sort.Index != 2 ) // 인던 보스 레그녹 용 이펙트일 경우 예외처리 ///인던1
	{
		if (effect_sort.nCurFrame >= 6 && effect_sort.nCurFrame < 10)
		{
			if (effect_sort.pMobTo)
				effect_sort.pMobTo->Attacked();
			else if (effect_sort.pNkChaTo)
				effect_sort.pNkChaTo->Attacked();
		}
	}

	if( effect_sort.Index == 0 && m_pLolo )
		m_pLolo->Render(GET_D3DDEVICE(), effect_sort.nCurFrame, FALSE, FALSE);
	if( effect_sort.Index == 1 && m_pLolo2 )
		m_pLolo2->Render(GET_D3DDEVICE(), effect_sort.nCurFrame, FALSE, FALSE);
	if( effect_sort.Index == 2 && m_pLolo2 ) // 인던 보스 레그녹 용 이펙트 ///인던1
		m_pLolo2->Render(GET_D3DDEVICE(), effect_sort.nCurFrame, FALSE, FALSE);

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ZWRITEENABLE, FALSE);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, TRUE);

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND,   D3DBLEND_SRCALPHA );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND,  D3DBLEND_INVSRCALPHA );

	if( g_pShadowRect )
	{
		lpSurface = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_STONERANGEBASE ] );
		g_pShadowRect->SetTileCoord(vDrawPos, 55 + (effect_sort.nCurFrame * 0.1f), FALSE);		// 부울값은 시간인가 아닌가
		g_pShadowRect->Render(lpSurface, FALSE);	// 부울값은 리얼그림자인가 아닌가?
	}

	DWORD  dwTextureState[3];

	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG1, &dwTextureState[0] );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG2, &dwTextureState[1] );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAOP,   &dwTextureState[2] );

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );

	lpSurface = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_STONERANGESMOKE ] );
	GET_D3DDEVICE()->SetTexture(0, lpSurface);

	D3DVECTOR	vViewVec;
	vViewVec = pCMyApp->GetEyeDirection();

	float up_height = effect_sort.nCurFrame * 0.5f + 7.0f;
	float smoke_fade = 1.0f - (effect_sort.nCurFrame / (float)FX_STONERANCE_FRAMECOUNT);

	if( smoke )
	{
		smoke->SetColor(1.0f, 1.0f, 1.0f, smoke_fade);
		smoke->SetBillBoard(vViewVec, FALSE, 0.0f);
		smoke->SetTranslationXYZ(vDrawPos.x + 10.0f, vDrawPos.y + up_height, vDrawPos.z);
		smoke->Render();

		smoke->SetColor(1.0f, 1.0f, 1.0f, smoke_fade);
		smoke->SetBillBoard(vViewVec, FALSE, 0.0f);
		smoke->SetTranslationXYZ(vDrawPos.x, vDrawPos.y + up_height, vDrawPos.z + 10.0f);
		smoke->Render();

		smoke->SetColor(1.0f, 1.0f, 1.0f, smoke_fade);
		smoke->SetBillBoard(vViewVec, FALSE, 0.0f);
		smoke->SetTranslationXYZ(vDrawPos.x - 10.0f, vDrawPos.y + up_height, vDrawPos.z);
		smoke->Render();

		smoke->SetColor(1.0f, 1.0f, 1.0f, smoke_fade);
		smoke->SetBillBoard(vViewVec, FALSE, 0.0f);
		smoke->SetTranslationXYZ(vDrawPos.x, vDrawPos.y + up_height, vDrawPos.z - 10.0f);
		smoke->Render();

		smoke->SetColor(1.0f, 1.0f, 1.0f, smoke_fade);
		smoke->SetBillBoard(vViewVec, FALSE, 0.0f);
		smoke->SetTranslationXYZ(vDrawPos.x + 10.0f, vDrawPos.y + up_height, vDrawPos.z + 10.0f);
		smoke->Render();

		smoke->SetColor(1.0f, 1.0f, 1.0f, smoke_fade);
		smoke->SetBillBoard(vViewVec, FALSE, 0.0f);
		smoke->SetTranslationXYZ(vDrawPos.x - 10.0f, vDrawPos.y + up_height, vDrawPos.z - 10.0f);
		smoke->Render();
	}

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState[2] );

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, dwLighting);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, dwAlpha);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ZWRITEENABLE, dwZWrite);
}

bool CFxStoneRance::Render_EX(EffectSort &effect_sort)
{
	if( !GET_D3DDEVICE() )
		return TRUE;

	if( effect_sort.nCurFrame >= FX_STONERANCE_FRAMECOUNT)
		return TRUE;

	LPDIRECTDRAWSURFACE7 lpSurface = NULL;
	D3DMATRIX matTrans, matView;
	DWORD dwLighting, dwZWrite, dwAlpha;

	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_LIGHTING, &dwLighting);
	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, &dwAlpha);
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_ZWRITEENABLE, &dwZWrite);

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ZWRITEENABLE, TRUE);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, FALSE);

	D3DVECTOR vDrawPos;
	vDrawPos = effect_sort.vPos;

	if( ++m_numTEng >= 4 )
		m_numTEng = 3;

	D3DUtil_SetIdentityMatrix( matTrans );
	D3DUtil_SetTranslateMatrix( matTrans, vDrawPos.x, vDrawPos.y, vDrawPos.z);
	pCMyApp->SetViewMatrix(matView);
	GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_WORLD, &matTrans );
	GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_VIEW, &matView);

	if( m_pLolo )
		m_pLolo->Render(GET_D3DDEVICE(), effect_sort.nCurFrame, FALSE, FALSE);
	if( m_pLolo2 )
		m_pLolo2->Render(GET_D3DDEVICE(), effect_sort.nCurFrame, FALSE, FALSE);

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ZWRITEENABLE, FALSE);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, TRUE);

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND,   D3DBLEND_SRCALPHA );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND,  D3DBLEND_INVSRCALPHA );

	if( g_pShadowRect )
	{
		lpSurface = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_STONERANGEBASE ] );
		g_pShadowRect->SetTileCoord(vDrawPos, 55 + (effect_sort.nCurFrame * 0.1f), FALSE);		// 부울값은 시간인가 아닌가
		g_pShadowRect->Render(lpSurface, FALSE);	// 부울값은 리얼그림자인가 아닌가?
	}

	DWORD  dwTextureState[3];

	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG1, &dwTextureState[0] );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG2, &dwTextureState[1] );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAOP,   &dwTextureState[2] );

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );

	lpSurface = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_STONERANGESMOKE ] );
	GET_D3DDEVICE()->SetTexture(0, lpSurface);

	D3DVECTOR	vViewVec;
	vViewVec = pCMyApp->GetEyeDirection();

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState[2] );

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, dwLighting);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, dwAlpha);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ZWRITEENABLE, dwZWrite);

	return FALSE;
}