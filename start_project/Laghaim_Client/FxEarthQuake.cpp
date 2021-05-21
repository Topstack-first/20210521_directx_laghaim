#include "stdafx.h"
#include "sfx.h"
#include "main.h"
#include "NkMob.h"
#include "NkCharacter.h"
#include "ShadowRect.h"
#include "ImageDefine.h"


// nate 2004 - 7 글로벌 ShadowRect사용


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFxEarthQuake::CFxEarthQuake()
{
	eng_sphere = NULL;
	eng_sphere = new CSphere;
	if( eng_sphere )
		eng_sphere->Create();

	smoke = NULL;
	smoke = new CRectPlane;
	if( smoke )
		smoke->Create(15.0f, 15.0f, FALSE);
}

CFxEarthQuake::~CFxEarthQuake()
{
	SAFE_DELETE(eng_sphere);
	SAFE_DELETE(smoke);
}

void CFxEarthQuake::LoadRes(IndexedTexture *pIndexedTexture, CShadowRect *pShadowRect)
{}

void CFxEarthQuake::DeleteRes()
{}

BOOL CFxEarthQuake::Render(EffectSort &effect_sort)
{
	if( !GET_D3DDEVICE() )
		return true;

	if(!effect_sort.pNkChaTo
			&& !effect_sort.pMobTo)
		return TRUE;

	if(effect_sort.nCurFrame >= FX_EARTHQUAKE_FRAMECOUNT)
		return TRUE;

	LPDIRECTDRAWSURFACE7 lpSurface = NULL;
	DWORD dwLighting;

	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_LIGHTING, &dwLighting);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND,   D3DBLEND_SRCALPHA );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND,  D3DBLEND_INVSRCALPHA );

	D3DVECTOR vDrawPos;
	vDrawPos = effect_sort.vTargetPos;

	if (effect_sort.nCurFrame >= 6 && effect_sort.nCurFrame < 10)
	{
		if (effect_sort.pMobTo)
			effect_sort.pMobTo->Attacked();
		else if (effect_sort.pNkChaTo)
			effect_sort.pNkChaTo->Attacked();
	}

	// nate 2004 - 7 글로벌 ShadowRect사용
	if( g_pShadowRect )
	{
		// nate 2004 - 7 - image
		if( g_pCapsyongTexture )
			lpSurface = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_EARTHQUAKE_BASE ] ) ;
		// nate 2004 - 7 글로벌 ShadowRect사용
		g_pShadowRect->SetTileCoord(vDrawPos, 50, FALSE);		// 부울값은 시간인가 아닌가
		g_pShadowRect->Render(lpSurface, FALSE);	// 부울값은 리얼그림자인가 아닌가?
	}

	DWORD  dwTextureState[3];

	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG1, &dwTextureState[0] );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG2, &dwTextureState[1] );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAOP,   &dwTextureState[2] );

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );

	// nate 2004 - 7 - image
	GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_EARTHQUAKE_ENG ] ) );
	float smoke_fade = 1.0f - (effect_sort.nCurFrame / (float)FX_EARTHQUAKE_FRAMECOUNT);
	float scale = 10.0f + effect_sort.nCurFrame * 2.0f;

	if( eng_sphere )
	{
		if( effect_sort.Index == 1 )
			eng_sphere->SetColor(0.6f, 1.0f, 1.0f, smoke_fade * 1.0f);
		else
			eng_sphere->SetColor(1.0f, 1.0f, 1.0f, smoke_fade * 0.5f);

		eng_sphere->SetScaling(scale, scale, scale);
		eng_sphere->SetTranslationXYZ(vDrawPos.x, vDrawPos.y, vDrawPos.z);
		eng_sphere->Render();
	}

	// nate 2004 - 7 - image
	GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_EARTHQUAKE_SMOKE ] ) );

	D3DVECTOR	vViewVec;
	vViewVec = pCMyApp->GetEyeDirection();

	float up_height = effect_sort.nCurFrame * 0.5f + 7.0f;

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

	return FALSE;
}