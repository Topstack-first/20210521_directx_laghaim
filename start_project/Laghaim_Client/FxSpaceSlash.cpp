#include "stdafx.h"
// FxSpaceSlash.cpp: implementation of the CFxSpaceSlash class.
//
//////////////////////////////////////////////////////////////////////

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
#include "FxSpaceSlash.h"


// nate 2004 - 7 글로벌 ShadowRect사용
extern float g_fDSDistance;

extern int g_EffectSound[];
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFxSpaceSlash::CFxSpaceSlash()
{
	m_rect_Shiled			= NULL;
	m_fScale				= 0.0f;
	m_fFade					= 0.0f;
	fMinScale				= 2.0f; //막의 최소 크기
	fMaxScale				= 2.05f;//막의 최대 크기
	fFlag					= 0.1f;
	m_nTotalFrame			= FX_SPACESLASH_FRAMECOUNT;
	m_pLolo = NULL;
	m_pLolo2 = NULL;
	m_rect_Shiled = new CRectPlane;
	if( m_rect_Shiled )
		m_rect_Shiled->Create( 17.0f, 17.0f, FALSE );

	light1	= NULL;
	light2	= NULL;
	light3	= NULL;
	light4	= NULL;
	light1 = new CRectPlane;
	if( light1 )
		light1->Create( 10.0f, 10.0f, TRUE );
	light2 = new CRectPlane;
	if( light2 )
		light2->Create( 10.0f, 10.0f, TRUE );
	light3 = new CRectPlane;
	if( light3 )
		light3->Create( 10.0f, 10.0f, TRUE );
	light4 = new CRectPlane;
	if( light4 )
		light4->Create( 10.0f, 10.0f, TRUE );

	m_numTLight	= 0;
}

CFxSpaceSlash::~CFxSpaceSlash()
{
	// [12/22/2008 D.K ] : memory leak.
	SAFE_DELETE( light1 );
	SAFE_DELETE( light2 );
	SAFE_DELETE( light3 );
	SAFE_DELETE( light4 );
	SAFE_DELETE(m_rect_Shiled);
}

void CFxSpaceSlash::LoadRes()
{
	if (!m_pLolo)
	{
		m_pLolo = new CLolos;
		m_pLolo->SetIndexedTexture( g_pCapsyongTexture );
		m_pLolo->Read("data/effect/lolos/f_space_slash.LOL");
	}
	if (!m_pLolo2)
	{
		m_pLolo2 = new CLolos;
		m_pLolo2->SetIndexedTexture( g_pCapsyongTexture );
		m_pLolo2->Read("data/effect/lolos/f_space_slash_cutting.LOL");
	}



}

void CFxSpaceSlash::DeleteRes()
{
	SAFE_DELETE(m_pLolo);
	SAFE_DELETE(m_pLolo2);
}

void CFxSpaceSlash::Render(EffectSort &effect_sort)
{
	if( !GET_D3DDEVICE() )
		return;

	if( !effect_sort.pNkChaTo && !effect_sort.pMobTo)
		return;

	float maxScale = 0.5f;

	m_fScale = effect_sort.nCurFrame - 20;
	m_fFade = 1.0f - m_fScale;

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND,   D3DBLEND_SRCALPHA );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND,  D3DBLEND_ONE);

	DWORD  dwTextureState[3];

	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG1, &dwTextureState[0] );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG2, &dwTextureState[1] );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAOP,   &dwTextureState[2] );

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );

	D3DVECTOR	vViewVec, vDrawVec2;
	vViewVec = pCMyApp->GetEyeDirection();

	D3DMATRIX matTrans, matView,matRotate, matWorld,matWorld2,matTemp;
	if (m_pLolo2)
	{
		D3DUtil_SetRotateYMatrix(matRotate, -effect_sort.pNkChaFrom->m_dir);
		if (effect_sort.pMobTo)
			D3DUtil_SetTranslateMatrix( matTrans, effect_sort.pMobTo->m_wx, effect_sort.pMobTo->m_wy, effect_sort.pMobTo->m_wz);
		else if (effect_sort.pNkChaTo)
			D3DUtil_SetTranslateMatrix( matTrans, effect_sort.pNkChaTo->m_wx, effect_sort.pNkChaTo->m_wy, effect_sort.pNkChaTo->m_wz);
		else
			return;

		D3DMATRIX		matS;
		D3DUtil_SetScaleMatrix(matS, 8.5f,8.5f,8.5f);
		D3DMath_MatrixMultiply(matWorld, matS, matWorld);
		D3DMath_MatrixMultiply(matWorld, matRotate, matTrans);
		GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_WORLD, &matWorld );
		//	GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_VIEW, &matView);
		if( m_pLolo2 )
			m_pLolo2->Render(GET_D3DDEVICE(), effect_sort.nCurFrame, FALSE, FALSE);

		if(effect_sort.pMobTo)
			effect_sort.pMobTo->Attacked();
		else if(effect_sort.pNkChaTo)
			effect_sort.pNkChaTo->Attacked();
	}

	LPDIRECTDRAWSURFACE7 lpSurface = NULL;

	int nFrame;
	nFrame = effect_sort.nCurFrame;

	if (m_pLolo)
	{
		D3DUtil_SetRotateYMatrix(matRotate, -effect_sort.pNkChaFrom->m_dir);
		if (effect_sort.pMobTo)
			D3DUtil_SetTranslateMatrix( matTrans, effect_sort.pMobTo->m_wx, effect_sort.pMobTo->m_wy, effect_sort.pMobTo->m_wz);
		else if (effect_sort.pNkChaTo)
			D3DUtil_SetTranslateMatrix( matTrans, effect_sort.pNkChaTo->m_wx, effect_sort.pNkChaTo->m_wy, effect_sort.pNkChaTo->m_wz);
		else
			return;

		D3DMATRIX		matS2;
		D3DUtil_SetScaleMatrix(matS2, 2.5f,2.5f,2.5f);
		D3DMath_MatrixMultiply(matWorld, matS2, matWorld);
		D3DMath_MatrixMultiply(matWorld, matRotate, matTrans);
		GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_WORLD, &matWorld );
		if( m_pLolo )
			m_pLolo->Render(GET_D3DDEVICE(), effect_sort.nCurFrame, FALSE, FALSE);
	}
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState[2] );

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND,   D3DBLEND_ONE );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND,  D3DBLEND_ONE );

	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG1, &dwTextureState[0] );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG2, &dwTextureState[1] );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAOP,   &dwTextureState[2] );

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );

	if (effect_sort.pNkChaFrom)
		vDrawVec2 = D3DVECTOR(effect_sort.pNkChaFrom->m_wx, effect_sort.pNkChaFrom->m_wy, effect_sort.pNkChaFrom->m_wz);

	if( ++m_numTLight >= 4 )
		m_numTLight = 0;

	if ( nFrame <= 20 )
	{
		if( g_pCapsyongTexture )
			lpSurface = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_SHOCKSPEARLIGHT + m_numTLight ] );
		GET_D3DDEVICE()->SetTexture(0, lpSurface);

		if( light1 )
		{
			light1->SetColor(0.2f, 1.0f, 1.0f, 1.0f);
			light1->SetRotationY( g_PI * 0.5f );
			light1->RotateZ( -nFrame * 0.3f );
			light1->Scale(4.0f, 0.0f, 4.0f);
			light1->SetTranslationXYZ(vDrawVec2.x, vDrawVec2.y + 10.0f, vDrawVec2.z);
			light1->Render();
		}

		if( light2 )
		{
			light2->SetColor(0.2f, 1.0f, 1.0f, 1.0f);
			light2->SetRotationX( g_PI * 0.5f );
			light2->RotateZ( -nFrame * 0.3f );
			light2->Scale(4.0f, 0.0f, 4.0f);
			light2->SetTranslationXYZ(vDrawVec2.x, vDrawVec2.y + 10.0f, vDrawVec2.z);
			light2->Render();
		}

		if( light3 )
		{
			light3->SetColor(0.2f, 1.0f, 1.0f, 1.0f);
			light3->SetRotationY( g_PI * 0.5f );
			light3->RotateX( -nFrame * 0.3f );
			light3->Scale(4.0f, 0.0f, 4.0f);
			light3->SetTranslationXYZ(vDrawVec2.x, vDrawVec2.y + 10.0f, vDrawVec2.z);
			light3->Render();
		}

		if( light4 )
		{
			light4->SetColor(0.2f, 1.0f, 1.0f, 1.0f);
			light4->SetRotationY( g_PI * 0.5f );
			light4->RotateX( -nFrame * 0.3f );
			light4->Scale(4.0f, 0.0f, 4.0f);
			light4->SetTranslationXYZ(vDrawVec2.x, vDrawVec2.y + 10.0f, vDrawVec2.z);
			light4->Render();
		}
	}
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState[2] );
}
