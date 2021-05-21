#include "stdafx.h"
// FxShieldBlock.cpp: implementation of the CFxShieldBlock class.
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
#include "FxShieldBlock.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFxShieldBlock::CFxShieldBlock()
{
	m_pLolo = NULL;
}

CFxShieldBlock::~CFxShieldBlock()
{
	SAFE_DELETE(m_pLolo);
}

void CFxShieldBlock::LoadRes()
{
	PFILE *fpLolo = NULL;

	if (!m_pLolo)
	{
		m_pLolo = new CLolos;
		m_pLolo->SetIndexedTexture( g_pCapsyongTexture );
		m_pLolo->Read("data/effect/lolos/b_protection_shield.LOL");
	}



}

void CFxShieldBlock::DeleteRes()
{}

BOOL CFxShieldBlock::Render(EffectSort &effect_sort)
{
	if( !GET_D3DDEVICE() )
		return TRUE;

	if( !effect_sort.pNkChaFrom )
		return TRUE;

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

	D3DMATRIX matTrans, matRotate, matWorld, matS;
	LPDIRECTDRAWSURFACE7 lpSurface = NULL;

	D3DUtil_SetRotateYMatrix(matRotate, -effect_sort.pNkChaFrom->m_dir);
	D3DUtil_SetTranslateMatrix( matTrans, effect_sort.pNkChaFrom->m_wx, effect_sort.pNkChaFrom->m_wy, effect_sort.pNkChaFrom->m_wz);
	D3DMath_MatrixMultiply( matWorld, matRotate, matTrans );
	GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_WORLD, &matWorld );
	if( m_pLolo )
		m_pLolo->Render(GET_D3DDEVICE(), effect_sort.nCurFrame, FALSE, FALSE);

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState[2] );
	return FALSE;
}

