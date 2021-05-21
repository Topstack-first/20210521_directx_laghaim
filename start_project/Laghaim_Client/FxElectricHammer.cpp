#include "stdafx.h"
// FxElectricHammer.cpp: implementation of the CFxElectricHammer class.
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
#include "FxElectricHammer.h"


// nate 2004 - 7 ±Û·Î¹ú ShadowRect»ç¿ë
extern float g_fDSDistance;

extern int g_EffectSound[];
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFxElectricHammer::CFxElectricHammer()
{
	corona1 = new CRectPlane;

	if( corona1 )
		corona1->Create( 10.0f, 10.0f, FALSE );

	eng_sphere = new CSphere;

	if( eng_sphere )
		eng_sphere->Create();

	m_fScale0 = 0.0f;
	m_fScale1 = 0.0f;
	m_fFade = 0.0f;
	m_rotang = 0.0f;

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
	m_nTotalFrame = FX_ELECTRICHAMMER_FRAMECOUNT;

	m_pLolo = NULL;

}

CFxElectricHammer::~CFxElectricHammer()
{
	SAFE_DELETE( light1 );
	SAFE_DELETE( light2 );
	SAFE_DELETE( light3 );
	SAFE_DELETE( light4 );
	SAFE_DELETE(corona1);
	SAFE_DELETE(eng_sphere);
}

// nate 2004 - 8 ImageManager
void CFxElectricHammer::LoadRes()
{
	PFILE *fpLolo = NULL;

	if (!m_pLolo)
	{
		m_pLolo = new CLolos;
		if( m_pLolo )
		{
			m_pLolo->SetIndexedTexture( g_pCapsyongTexture );
			m_pLolo->Read("data/effect/lolos/f_electric_hammer.LOL");
		}
	}



}

void CFxElectricHammer::DeleteRes()
{
	SAFE_DELETE(m_pLolo);
}

void CFxElectricHammer::Render(EffectSort &effect_sort)
{
	if( !GET_D3DDEVICE() )
		return;

	if( !effect_sort.pNkChaTo && !effect_sort.pMobTo)
		return;

	int y = 0.0f;
	float angle = 0.0f;
	int nCurFrame = effect_sort.nCurFrame - 30;
	LPDIRECTDRAWSURFACE7	lpSurface = NULL;

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND,   D3DBLEND_SRCALPHA );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND,  D3DBLEND_ONE);

	DWORD  dwTextureState[3];
	//-- Store State
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG1, &dwTextureState[0] );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG2, &dwTextureState[1] );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAOP,   &dwTextureState[2] );

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );

	D3DVECTOR	vViewVec;
	vViewVec = pCMyApp->GetEyeDirection();

	D3DVECTOR	vDrawVec;
	if (effect_sort.pMobTo)
		vDrawVec = D3DVECTOR(effect_sort.pMobTo->m_wx, effect_sort.pMobTo->m_wy + 3.0f, effect_sort.pMobTo->m_wz);
	else if (effect_sort.pNkChaTo)
		vDrawVec = D3DVECTOR(effect_sort.pNkChaTo->m_wx, effect_sort.pNkChaTo->m_wy + 3.0f, effect_sort.pNkChaTo->m_wz);
	else
		return;

	m_fScale0 = ( 2.0f * effect_sort.nCurFrame ) / m_nTotalFrame;
	m_fScale1 = nCurFrame * 2; //-- scale
	m_fFade = (40-(nCurFrame % 40 )) / 50.0f;   //-- scale fade

	D3DVECTOR	vtestVec;
	D3DVECTOR	vAdj;

	vtestVec.dvX=0;
	vtestVec.dvY =-10;
	vtestVec.dvZ = 0;
	// °Å¸®¸¦ ¶³¾î¶ß¸°´Ù.
	vAdj = effect_sort.vTargetPos - effect_sort.vPos;
	vAdj = Normalize(vAdj);

	//------------------------------------------------------------------------
	//-- Rotation Plane

	angle = float(effect_sort.nCurFrame - 20)*3.1416f / 62.0f ;

	//-- Set Texture _0	:rot

	if( corona1 && effect_sort.nCurFrame > 30 )
	{
		if( g_pCapsyongTexture )
			GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_ELECTRIC_SPARK ] ) );

		corona1->SetBillBoard(vtestVec, FALSE, 0.0f);	//-- Make Billboard
		corona1->Scale(m_fScale1, m_fScale1, 0.0f);	    //-- Scale
		corona1->SetColor(1.0f, 1.0f, 1.0f, m_fFade);	//-- color
		corona1->SetTranslationXYZ(vDrawVec.x, vDrawVec.y, vDrawVec.z);
		corona1->Render();

		if( g_pCapsyongTexture )
			GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_FROST ] ) );
		//	float smoke_fade = 1.0f - ( ( effect_sort.nCurFrame - 30.0f) / 5.0f);
		float scale =  ( effect_sort.nCurFrame - 30.0f ) * 5.0f;
		eng_sphere->SetColor(1.0f, 1.0f, 1.0f, 1.0f );
		eng_sphere->SetScaling( scale, scale, scale );
		eng_sphere->SetTranslationXYZ(vDrawVec.x, vDrawVec.y, vDrawVec.z);
		eng_sphere->RotateY( m_fScale0 * 5.0f);
		eng_sphere->Render();
	}

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState[2] );

	D3DMATRIX matTrans, matView,matRotate, matWorld,matWorld2,matTemp;
	D3DMATRIX matRx, matRy;

	if (m_pLolo)
	{
		D3DUtil_SetRotateYMatrix(matRotate, -effect_sort.pNkChaFrom->m_dir);
		D3DUtil_SetTranslateMatrix( matTrans, effect_sort.pNkChaFrom->m_wx, effect_sort.pNkChaFrom->m_wy, effect_sort.pNkChaFrom->m_wz);
		D3DMath_MatrixMultiply(matWorld, matRotate, matTrans);
		GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_WORLD, &matWorld );
		if( m_pLolo )
			m_pLolo->Render(GET_D3DDEVICE(), effect_sort.nCurFrame, FALSE, FALSE);

		if(effect_sort.pMobTo)
			effect_sort.pMobTo->Attacked();
		else if(effect_sort.pNkChaTo)
			effect_sort.pNkChaTo->Attacked();
	}

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
		vDrawVec = D3DVECTOR(effect_sort.pNkChaFrom->m_wx - 3.0f, effect_sort.pNkChaFrom->m_wy + 23.0f, effect_sort.pNkChaFrom->m_wz + 3.0f);

	if( ++m_numTLight >= 4 )
		m_numTLight = 0;

	int nFrame = effect_sort.nCurFrame;

	if ( nFrame >= 4 && nFrame <= 13 )
	{
		if( g_pCapsyongTexture )
			lpSurface = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_SPARK + m_numTLight ] );
		GET_D3DDEVICE()->SetTexture(0, lpSurface);

		if( light1 )
		{
			light1->SetColor(0.2f, 1.0f, 1.0f, 1.0f);
			light1->SetRotationY( g_PI * 0.5f );
			light1->RotateZ( -nFrame * 0.3f );
			light1->Scale(4.0f, 0.0f, 4.0f);
			light1->SetTranslationXYZ(vDrawVec.x, vDrawVec.y + 10.0f, vDrawVec.z);
			light1->Render();
		}

		if( light2 )
		{
			light2->SetColor(0.2f, 1.0f, 1.0f, 1.0f);
			light2->SetRotationX( g_PI * 0.5f );
			light2->RotateZ( -nFrame * 0.3f );
			light2->Scale(4.0f, 0.0f, 4.0f);
			light2->SetTranslationXYZ(vDrawVec.x, vDrawVec.y + 10.0f, vDrawVec.z);
			light2->Render();
		}

		if( light3 )
		{
			light3->SetColor(0.2f, 1.0f, 1.0f, 1.0f);
			light3->SetRotationY( g_PI * 0.5f );
			light3->RotateX( -nFrame * 0.3f );
			light3->Scale(4.0f, 0.0f, 4.0f);
			light3->SetTranslationXYZ(vDrawVec.x, vDrawVec.y + 10.0f, vDrawVec.z);
			light3->Render();
		}

		if( light4 )
		{
			light4->SetColor(0.2f, 1.0f, 1.0f, 1.0f);
			light4->SetRotationY( g_PI * 0.5f );
			light4->RotateX( -nFrame * 0.3f );
			light4->Scale(4.0f, 0.0f, 4.0f);
			light4->SetTranslationXYZ(vDrawVec.x, vDrawVec.y + 10.0f, vDrawVec.z);
			light4->Render();
		}
	}
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState[2] );
}
