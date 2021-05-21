#include "stdafx.h"
// FxShadowSlash.cpp: implementation of the CFxShadowSlash class.
//
//////////////////////////////////////////////////////////////////////

#define D3D_OVERLOADS

#include <d3d.h>
#include "main.h"
#include "ImageDefine.h"
#include "NkCharacter.h"
#include "FxShadowSlash.h"




//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CFxShadowSlash::CFxShadowSlash()
{
	m_pSmoke = NULL;
}

CFxShadowSlash::~CFxShadowSlash()
{
	DeleteRes();
}

void CFxShadowSlash::LoadRes()
{
	m_pSmoke = new CRectPlane;
	if( m_pSmoke )
		m_pSmoke->Create( 15.0f, 10.0f, false );
}

void CFxShadowSlash::DeleteRes()
{
	SAFE_DELETE(m_pSmoke);
}

BOOL CFxShadowSlash::Render( EffectSort &effect_sort )
{
	if( !GET_D3DDEVICE() )
		return false;

	LPDIRECTDRAWSURFACE7 lpSurface = NULL;
	D3DMATRIX matTrans;
	D3DVECTOR vDrawPos;
	DWORD dwLighting;
	int i = 0;

	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_LIGHTING, &dwLighting);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND,   D3DBLEND_SRCALPHA );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND,  D3DBLEND_INVSRCALPHA );

	/* ///하이콤보 시 수정.
		if( effect_sort.pNkChaFrom )
		{
			vDrawPos = D3DVECTOR(effect_sort.pNkChaFrom->m_wx,
								 effect_sort.pNkChaFrom->m_wy,
								 effect_sort.pNkChaFrom->m_wz);
		}
	*/
	vDrawPos = effect_sort.vPos; ///하이콤보 시 수정.


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

	float fUpheight = (effect_sort.nCurFrame + 3.0f) / 2.0f;
	float fSmoke_fade = 1.0f - (effect_sort.nCurFrame / (float)FX_SHADOWSLASH_FRAMECOUNT);
	float fWide = 4.0f;
	float fWide2 = 3.0f;
	float fScale = 4.0f;

	if( m_pSmoke && effect_sort.nCurFrame >= 10 )
	{
		for( i = 0 ; i < 3 ; i ++ )
		{
			m_pSmoke->SetColor(1.0f, 1.0f, 1.0f, fSmoke_fade);
			m_pSmoke->SetBillBoard(vViewVec, FALSE, 0.0f);
			m_pSmoke->SetTranslationXYZ(vDrawPos.x + fWide, (vDrawPos.y + fUpheight * i)/1.0f, vDrawPos.z);
			m_pSmoke->Scale( fScale, fScale, fScale );
			m_pSmoke->Render();

			m_pSmoke->SetColor(1.0f, 1.0f, 1.0f, fSmoke_fade);
			m_pSmoke->SetBillBoard(vViewVec, FALSE, 0.0f);
			m_pSmoke->SetTranslationXYZ(vDrawPos.x, (vDrawPos.y + fUpheight * i)/0.95f, vDrawPos.z + fWide);
			m_pSmoke->Scale( fScale, fScale, fScale );
			m_pSmoke->Render();

			m_pSmoke->SetColor(1.0f, 1.0f, 1.0f, fSmoke_fade);
			m_pSmoke->SetBillBoard(vViewVec, FALSE, 0.0f);
			m_pSmoke->SetTranslationXYZ(vDrawPos.x - fWide, (vDrawPos.y + fUpheight * i)/1.0f, vDrawPos.z);
			m_pSmoke->Scale( fScale, fScale, fScale );
			m_pSmoke->Render();

			m_pSmoke->SetColor(1.0f, 1.0f, 1.0f, fSmoke_fade);
			m_pSmoke->SetBillBoard(vViewVec, FALSE, 0.0f);
			m_pSmoke->SetTranslationXYZ(vDrawPos.x, (vDrawPos.y + fUpheight * i)/0.97f, vDrawPos.z - fWide);
			m_pSmoke->Scale( fScale, fScale, fScale );
			m_pSmoke->Render();

			m_pSmoke->SetColor(1.0f, 1.0f, 1.0f, fSmoke_fade);
			m_pSmoke->SetBillBoard(vViewVec, FALSE, 0.0f);
			m_pSmoke->SetTranslationXYZ(vDrawPos.x + fWide2, (vDrawPos.y + fUpheight * i)/0.89f, vDrawPos.z + fWide2);
			m_pSmoke->Scale( fScale, fScale, fScale );
			m_pSmoke->Render();

			m_pSmoke->SetColor(1.0f, 1.0f, 1.0f, fSmoke_fade);
			m_pSmoke->SetBillBoard(vViewVec, FALSE, 0.0f);
			m_pSmoke->SetTranslationXYZ(vDrawPos.x - fWide2, (vDrawPos.y + fUpheight * i)/1.0f, vDrawPos.z - fWide2);
			m_pSmoke->Scale( fScale, fScale, fScale );
			m_pSmoke->Render();

			m_pSmoke->SetColor(1.0f, 1.0f, 1.0f, fSmoke_fade);
			m_pSmoke->SetBillBoard(vViewVec, FALSE, 0.0f);
			m_pSmoke->SetTranslationXYZ(vDrawPos.x + fWide2, (vDrawPos.y + fUpheight * i)/0.94f, vDrawPos.z - fWide2);
			m_pSmoke->Scale( fScale, fScale, fScale );
			m_pSmoke->Render();

			m_pSmoke->SetColor(1.0f, 1.0f, 1.0f, fSmoke_fade);
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

