#include "stdafx.h"

#define D3D_OVERLOADS

#include <d3d.h>
#include <ddraw.h>
#include <time.h>
#include "FxSet.h"
#include "ddutil.h"
#include "D3DUtil.h"
#include "D3DMath.h"
#include "sfxUtil.h"
#include "GUtil.h"
#include "SFXBaseShape.h"
#include "main.h"
#include "D3DMath.h"
#include "D3DUtil.h"
#include "IndexedTexture.h"
#include "ImageDefine.h"
#include "NkCharacter.h"
#include "NkMob.h"
#include "UIMgr.h"
#include "land.h"

#include "FxPoisonFog.h"






#define RUNNING_TIME 7000 // 이펙트가 표시되는 틱.
#define FADE_TIME    2550 // 이펙트 알파 조절 구간 틱.

CFxPoisonFog::CFxPoisonFog()
{
}

CFxPoisonFog::~CFxPoisonFog()
{

}

BOOL CFxPoisonFog::Render( EffectSort &effect_sort )
{
	DWORD dwNowTick = timeGetTime();
	DWORD dwElapsedTick = dwNowTick - effect_sort.dwColor; // dwColor를 이펙트가 생성된 틱으로 사용.
	effect_sort.nCurFrame = (dwElapsedTick) * 6 / 1000; // 프레임을 직접 계산한다. (프레임 레이트를 다르게 적용하기 위해서...)

	if( effect_sort.nCurFrame >= 20 )
		effect_sort.nCurFrame %= 20; // 루프시키기.

	// 상태값 보존.
	DWORD alphaTest, alphaBlend, srcBlendOld, destBlendOld, dwLighting, dwAmbient, dwFogEnable;
	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_FOGENABLE, &dwFogEnable );
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, &alphaBlend );
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_ALPHATESTENABLE, &alphaTest );
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_SRCBLEND, &srcBlendOld );
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_DESTBLEND, &destBlendOld );
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_LIGHTING, &dwLighting );
	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_AMBIENT, &dwAmbient);
	DWORD  dwTextureState[3];
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG1, &dwTextureState[0] );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG2, &dwTextureState[1] );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAOP,   &dwTextureState[2] );



	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_FOGENABLE, FALSE );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, TRUE );
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA);

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );

	D3DTLVERTEX TLVec[4];
	DWORD Alpha = 255;

	// 알파 조정.
	if( dwElapsedTick < FADE_TIME ) // 안보이다가 점점 보이게...
	{
		Alpha = 0;
		Alpha += (float)dwElapsedTick*0.1f;

		if( Alpha > 255 )
			Alpha = 255;
	}
	if( dwElapsedTick > RUNNING_TIME-FADE_TIME ) // 점점 사라지듯이
	{
		Alpha -= (float)( dwElapsedTick-(RUNNING_TIME-FADE_TIME) )*0.1f;

		if( Alpha > 255 )
			Alpha = 0;
	}



	TLVec[0] = D3DTLVERTEX( D3DVECTOR(0,g_pNk2DFrame->GetClientHeight(),0), 1, RGBA_MAKE(255,255,255,Alpha), 0, 0, 1.0f );
	TLVec[1] = D3DTLVERTEX( D3DVECTOR(0,0,0), 1, RGBA_MAKE(255,255,255,Alpha), 0, 0, 0 );
	TLVec[2] = D3DTLVERTEX( D3DVECTOR(g_pNk2DFrame->GetClientWidth(),0,0), 1, RGBA_MAKE(255,255,255,Alpha), 0, 1.0f, 0 );
	TLVec[3] = D3DTLVERTEX( D3DVECTOR(g_pNk2DFrame->GetClientWidth(),g_pNk2DFrame->GetClientHeight(),0), 1, RGBA_MAKE(255,255,255,Alpha), 0, 1.0f, 1.0f );

	GET_D3DDEVICE()->SetTexture( 0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_POISON_FOG+effect_sort.nCurFrame ] ) );
	GET_D3DDEVICE()->DrawPrimitive(D3DPT_TRIANGLEFAN    , D3DFVF_TLVERTEX, TLVec, 4, 0);




	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_FOGENABLE, dwFogEnable );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ALPHATESTENABLE, alphaTest );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, alphaBlend );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND, srcBlendOld);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND, destBlendOld);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_AMBIENT, dwAmbient);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, dwLighting );

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState[2] );


	if( dwElapsedTick > RUNNING_TIME ) // 일정 초가 지났으면 사라지게..
		return TRUE;

	return FALSE;
}
