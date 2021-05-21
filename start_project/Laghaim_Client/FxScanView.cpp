#include "stdafx.h"
#include "sfx.h"
#include "main.h"
#include "NkCharacter.h"
#include "NkMob.h"
#include "Land.h"
#include "ImageDefine.h"



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFxScanView::CFxScanView()
{
	scan					= NULL;
	scan_ring				= NULL;
	corona					= NULL;
	m_fScale				= 0.0f;
	m_fFade					= 0.0f;
	m_nTotalFrame			= FX_SCANVIEW_FRAMECOUNT;

	Create();
}

CFxScanView::~CFxScanView()
{
	SAFE_DELETE(scan);
	SAFE_DELETE(scan_ring);
	SAFE_DELETE(corona);
}

void CFxScanView::Create()
{
	SAFE_DELETE(scan);
	SAFE_DELETE(scan_ring);
	SAFE_DELETE(corona);

	corona = new CRectPlane;
	if( corona )
		corona->Create( 20.0f, 20.0f, FALSE );

	scan = new CCylinderPlane;
	if( scan )
		scan->Create( 8.0f, 5.0f, 1.0f, 20, TRUE );

	scan_ring = new CCylinderPlane;
	if( scan_ring )
		scan_ring->Create( 3.0f, 8.0f, 0.0f, 20, FALSE );
}

void CFxScanView::LoadRes(IndexedTexture *pIndexedTexture)
{}
void CFxScanView::DeleteRes()
{}

BOOL CFxScanView::Render(EffectSort &effect_sort)
{
	if( !GET_D3DDEVICE() )
		return true;

	if( !effect_sort.pNkChaFrom )
		return TRUE;

	if(effect_sort.nCurFrame >= m_nTotalFrame)
	{
		m_fScale = 0.0f;
		m_fFade = 1.0f;
		return TRUE;
	}

	float maxScale = 3.0f;

	m_fScale = (maxScale * effect_sort.nCurFrame) / m_nTotalFrame;
	m_fFade = 1.0f - (m_fScale * 0.3f);

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

	D3DVECTOR	vViewVec;
	vViewVec = pCMyApp->GetEyeDirection();

	D3DVECTOR	vDrawVecFrom;
	if( effect_sort.pNkChaFrom )
		vDrawVecFrom = D3DVECTOR(effect_sort.pNkChaFrom->m_wx, effect_sort.pNkChaFrom->m_wy, effect_sort.pNkChaFrom->m_wz);


	float red, green, blue;
	red = 0.4f - (0.4f * m_fScale * 0.3f);
	green = 0.9f - (0.95f * m_fScale * 0.3f);
	blue = 0.5f - (0.5f * m_fScale * 0.3f);

	if( corona )
	{
		// nate 2004 - 7 - image
		GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_SCANVIEW_LIGHT ] ) );
		corona->SetColor(red * 0.35f, green * 0.35f, blue * 0.35f, m_fFade * 0.35f);
		corona->SetRotationX( g_PI * 0.5f );
		corona->RotateZ( m_fScale * 5.0f );
		corona->Scale( m_fScale + 1.0f, m_fScale + 1.0f, 1.0f );
		corona->SetTranslationXYZ(vDrawVecFrom.x, vDrawVecFrom.y + 5.5f + m_fScale*2.0f, vDrawVecFrom.z);
		corona->Render();
	}

	if( scan )
	{
		// nate 2004 - 7 - image
		GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_SCANVIEW_SCAN ] ) );
		scan->SetColor(red * 0.5f, green * 0.5f, blue * 0.5f, m_fFade * 0.5f);
		scan->SetRotationY( g_PI * 0.5f );
		scan->RotateY( m_fScale * 5.0f );
		scan->Scale( m_fScale + 1.0f, 1.0f, m_fScale + 1.0f );
		scan->SetTranslationXYZ(vDrawVecFrom.x, vDrawVecFrom.y + 5.5f, vDrawVecFrom.z);
		scan->Render();
	}

	if( scan_ring )
	{
		scan_ring->SetColor(red * 0.35f, green * 0.35f, blue * 0.35f, m_fFade * 0.35f);
		scan_ring->SetRotationY( g_PI * 0.5f );
		scan_ring->Scale( m_fScale*1.5f + 1.0f, 1.0f, m_fScale*1.5f + 1.0f );
		scan_ring->SetTranslationXYZ(vDrawVecFrom.x, vDrawVecFrom.y + 5.5f, vDrawVecFrom.z);
		scan_ring->Render();
	}

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState[2] );

	return FALSE;

}


