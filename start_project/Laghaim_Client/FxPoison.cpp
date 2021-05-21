#include "stdafx.h"
#define D3D_OVERLOADS

#include <d3d.h>
#include "main.h"
#include "ShadowRect.h"
#include "D3DUtil.h"
#include "D3DMath.h"
#include "sfxUtil.h"
#include "SFXBaseShape.h"
#include "IndexedTexture.h"
#include "ImageDefine.h"
#include "NkCharacter.h"
#include "NkMob.h"
#include "FxSet.h"
#include "FxPoison.h"



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CFxPoison::CFxPoison()
{
	m_pSmoke = NULL;
	m_pLolo = NULL;
}

CFxPoison::~CFxPoison()
{
	DeleteRes();
}

void CFxPoison::LoadRes()
{
	m_pSmoke = new CRectPlane;
	if( m_pSmoke )
		m_pSmoke->Create( 15.0f, 10.0f, false );

	if( !m_pLolo )
	{
		m_pLolo = new CLolos;
		m_pLolo->SetIndexedTexture( g_pCapsyongTexture );
		m_pLolo->Read( "data/effect/lolos/poison.lol" );
	}





}

void CFxPoison::DeleteRes()
{
	SAFE_DELETE(m_pSmoke);
	SAFE_DELETE(m_pLolo);
}

BOOL CFxPoison::Render( EffectSort &effect_sort )
{
	if( !m_pLolo || !GET_D3DDEVICE() )
		return false;

	LPDIRECTDRAWSURFACE7 lpSurface = NULL;
	D3DMATRIX matTrans;
	D3DVECTOR vDrawPos;
	DWORD dwLighting;
	int i = 0;

	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_LIGHTING, &dwLighting);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND,   D3DBLEND_SRCALPHA );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND,  D3DBLEND_INVSRCALPHA );

	vDrawPos = effect_sort.vTargetPos;

	D3DUtil_SetIdentityMatrix( matTrans );
	D3DUtil_SetTranslateMatrix( matTrans, effect_sort.vPos.x, effect_sort.vPos.y, effect_sort.vPos.z);
	GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_WORLD, &matTrans );

	if( g_pShadowRect && effect_sort.Index != 10082 ) // 라비용 포이즌은 이거 안그리게..
	{
		lpSurface = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_POISON ] ) ;
		if( effect_sort.nCurFrame >= 30 )
			g_pShadowRect->SetTileCoord(vDrawPos, 155 - effect_sort.nCurFrame * 2.5f, FALSE);
		else if(  effect_sort.nCurFrame <= 10 )
			g_pShadowRect->SetTileCoord(vDrawPos, effect_sort.nCurFrame * 8, FALSE);
		else
			g_pShadowRect->SetTileCoord(vDrawPos, 80, FALSE);
		g_pShadowRect->Render(lpSurface, FALSE);
	}

	if( effect_sort.nCurFrame < 5 && effect_sort.Index != 10082 ) // 라비용 포이즌은 이거 안그리게..
		m_pLolo->Render(GET_D3DDEVICE(), effect_sort.nCurFrame, FALSE, FALSE);

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE);

	DWORD  dwTextureState[3];
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG1, &dwTextureState[0] );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG2, &dwTextureState[1] );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAOP,   &dwTextureState[2] );

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );

	// nate 2004 - 7 - image
	lpSurface = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_DROPCANDY_SMOKE ] ) ;
	GET_D3DDEVICE()->SetTexture(0, lpSurface);
	D3DVECTOR	vViewVec = pCMyApp->GetEyeDirection();

	float fUpheight = (effect_sort.nCurFrame + 3.0f) / 4.0f;
	float fSmoke_fade = 1.0f - (effect_sort.nCurFrame / (float)FX_POISON_FRAMECOUNT);
	float fWide = 30.0f;
	float fWide2 = 20.0f;
	float fScale = 4.0f;
	float fRed   = 0.3f;
	float fGreen = 0.6f;
	float fBlue  = 0.1f;

	if( effect_sort.Index == 10082 ) // 라비용 스킬용 포이즌 이면...
	{
		fUpheight = (effect_sort.nCurFrame + 1.0f) / 6.0f;
		fWide = 15.0f;
		fWide2 = 10.0f;
		fScale = 3.0f; // 구름 크기 작게.

		fRed   = 0.1f;
		fGreen = 0.4f;
		fBlue  = 0.1f;
	}

	if( m_pSmoke && effect_sort.nCurFrame >= 10 )
	{
		for( i = 0 ; i < 3 ; i ++ )
		{
			m_pSmoke->SetColor(fRed, fGreen, fBlue, fSmoke_fade);
			m_pSmoke->SetBillBoard(vViewVec, FALSE, 0.0f);
			m_pSmoke->SetTranslationXYZ(vDrawPos.x + fWide, (vDrawPos.y + fUpheight * i)/1.0f, vDrawPos.z);
			m_pSmoke->Scale( fScale, fScale, fScale );
			m_pSmoke->Render();

			m_pSmoke->SetColor(fRed, fGreen, fBlue, fSmoke_fade);
			m_pSmoke->SetBillBoard(vViewVec, FALSE, 0.0f);
			m_pSmoke->SetTranslationXYZ(vDrawPos.x, (vDrawPos.y + fUpheight * i)/0.95f, vDrawPos.z + fWide);
			m_pSmoke->Scale( fScale, fScale, fScale );
			m_pSmoke->Render();

			m_pSmoke->SetColor(fRed, fGreen, fBlue, fSmoke_fade);
			m_pSmoke->SetBillBoard(vViewVec, FALSE, 0.0f);
			m_pSmoke->SetTranslationXYZ(vDrawPos.x - fWide, (vDrawPos.y + fUpheight * i)/1.0f, vDrawPos.z);
			m_pSmoke->Scale( fScale, fScale, fScale );
			m_pSmoke->Render();

			m_pSmoke->SetColor(fRed, fGreen, fBlue, fSmoke_fade);
			m_pSmoke->SetBillBoard(vViewVec, FALSE, 0.0f);
			m_pSmoke->SetTranslationXYZ(vDrawPos.x, (vDrawPos.y + fUpheight * i)/0.97f, vDrawPos.z - fWide);
			m_pSmoke->Scale( fScale, fScale, fScale );
			m_pSmoke->Render();

			m_pSmoke->SetColor(fRed, fGreen, fBlue, fSmoke_fade);
			m_pSmoke->SetBillBoard(vViewVec, FALSE, 0.0f);
			m_pSmoke->SetTranslationXYZ(vDrawPos.x + fWide2, (vDrawPos.y + fUpheight * i)/0.89f, vDrawPos.z + fWide2);
			m_pSmoke->Scale( fScale, fScale, fScale );
			m_pSmoke->Render();

			m_pSmoke->SetColor(fRed, fGreen, fBlue, fSmoke_fade);
			m_pSmoke->SetBillBoard(vViewVec, FALSE, 0.0f);
			m_pSmoke->SetTranslationXYZ(vDrawPos.x - fWide2, (vDrawPos.y + fUpheight * i)/1.0f, vDrawPos.z - fWide2);
			m_pSmoke->Scale( fScale, fScale, fScale );
			m_pSmoke->Render();

			m_pSmoke->SetColor(fRed, fGreen, fBlue, fSmoke_fade);
			m_pSmoke->SetBillBoard(vViewVec, FALSE, 0.0f);
			m_pSmoke->SetTranslationXYZ(vDrawPos.x + fWide2, (vDrawPos.y + fUpheight * i)/0.94f, vDrawPos.z - fWide2);
			m_pSmoke->Scale( fScale, fScale, fScale );
			m_pSmoke->Render();

			m_pSmoke->SetColor(fRed, fGreen, fBlue, fSmoke_fade);
			m_pSmoke->SetBillBoard(vViewVec, FALSE, 0.0f);
			m_pSmoke->SetTranslationXYZ(vDrawPos.x - fWide2, (vDrawPos.y + fUpheight * i)/0.97f, vDrawPos.z + fWide2);
			m_pSmoke->Scale( fScale, fScale, fScale );
			m_pSmoke->Render();
		}
	}

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState[2] );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, dwLighting);

	return true;
}