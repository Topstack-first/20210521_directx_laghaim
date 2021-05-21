#include "stdafx.h"
#include "main.h"
#include "ImageDefine.h"
#include "FxFireTower.h"
#include "UIMgr.h"



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFxFireTower::CFxFireTower()
{
	inside_ring			= NULL;
	outside_ring		= NULL;
	m_fScale			= 0.0f;
	m_fFade				= 1.0f;
	m_fRotation			= 0.0f;
	m_nTotalFrame		= FX_FIRETOWER_FRAMECOUNT;

	Create();
}

CFxFireTower::~CFxFireTower()
{
	SAFE_DELETE(outside_ring);
	SAFE_DELETE(inside_ring);
}

void CFxFireTower::Create()
{
	SAFE_DELETE(outside_ring);
	SAFE_DELETE(inside_ring);

	inside_ring = new CCylinderPlane;
	if( inside_ring )
		inside_ring->Create( 25.0f, 30.0f, 1.0f, 20, 0.0f, TRUE, 1.0f );
	outside_ring = new CCylinderPlane;
	if( outside_ring )
		outside_ring->Create( 50.0f, 60.0f, 1.0f, 20, 0.0f, TRUE, 1.0f );
}

void CFxFireTower::LoadRes(IndexedTexture *pIndexedTexture)
{
}

void CFxFireTower::DeleteRes()
{
}

BOOL CFxFireTower::Render(EffectSort &effect_sort)
{
	if( !GET_D3DDEVICE() )
		return true;

	if( pCMyApp->GetCurWorld() == 19 ) // [2008/11/3 Theodoric] WORLD_MATRIX_2009
	{
		if( effect_sort.num == 2 && pCMyApp->m_pUIMgr->m_nStoneState[4] != 2 )
			return TRUE;
		else if( effect_sort.num == 3 && pCMyApp->m_pUIMgr->m_nStoneState[5] != 2 )
			return TRUE;
	}
	else
	{
		if( effect_sort.num == 1 && pCMyApp->m_pUIMgr->m_nStoneState[6] != 2 )
			return TRUE;
		else if( effect_sort.num == 2 && pCMyApp->m_pUIMgr->m_nStoneState[7] != 2 )
			return TRUE;
		else if( effect_sort.num == 3 && pCMyApp->m_pUIMgr->m_nStoneState[8] != 2 )
			return TRUE;
	}

	m_fRotation += 0.15f;

	m_fScale = (2.0f * effect_sort.nCurFrame) / m_nTotalFrame;

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND,   D3DBLEND_ONE );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND,  D3DBLEND_ONE );

	DWORD  dwTextureState[3];

	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG1, &dwTextureState[0] );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG2, &dwTextureState[1] );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAOP,   &dwTextureState[2] );

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );

	D3DVECTOR	vDrawPos;
	vDrawPos = D3DVECTOR( effect_sort.vTargetPos.x,
						  effect_sort.vTargetPos.y, effect_sort.vTargetPos.z);

	float red, green, blue;

	if( effect_sort.num == 1 )
	{
		red = 0.2f;
		green = 0.5f;
		blue = 0.5f;
	}

	if( effect_sort.num == 2 )
	{
		red = 0.2f;
		green = 1.0f;
		blue = 0.2f;
	}

	if( effect_sort.num == 3 )
	{
		red = 0.1f;
		green = 0.2f;
		blue = 0.8f;
	}

	if( g_pCapsyongTexture )
		GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_FIRETOWER2 ] ) );

	if( inside_ring )
	{
		inside_ring->SetColor(red, green, blue, 1.0f);
		inside_ring->SetRotationY( g_PI );
		inside_ring->RotateY( m_fRotation * 0.2f );
		inside_ring->Scale( 1.0f, m_fScale * 300.0f, 1.0f );
		inside_ring->SetTranslationXYZ(vDrawPos.x, vDrawPos.y, vDrawPos.z);
		inside_ring->Render();
	}

	if( g_pCapsyongTexture )
		GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_FIRETOWER ] ) );

	if( outside_ring )
	{
		outside_ring->SetColor(red, green, blue, 1.0f);
		outside_ring->SetRotationY( g_PI );
		outside_ring->RotateY( m_fRotation * 0.1f );
		outside_ring->Scale( 1.0f, m_fScale * 300.0f, 1.0f );
		outside_ring->SetTranslationXYZ(vDrawPos.x, vDrawPos.y, vDrawPos.z);
		outside_ring->Render();
	}

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState[2] );

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE);

	return FALSE;
}
