#include "stdafx.h"
#include "sfx.h"
#include "main.h"
#include "NkCharacter.h"
#include "Land.h"
#include "ShadowRect.h"
#include "ImageDefine.h"




//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFxBlackHole::CFxBlackHole()
{
	blackhole				= NULL;
	m_fScale				= 0.0f;
	m_fFade					= 0.0f;
	m_nTotalFrame			= FX_BLACKHOLE_FRAMECOUNT;

	Create();
}

CFxBlackHole::~CFxBlackHole()
{
	SAFE_DELETE(blackhole);
}

void CFxBlackHole::Create()
{
	SAFE_DELETE(blackhole);

	blackhole = new CCylinderPlane;
	if( blackhole )
		blackhole->Create2( 9.0f, 5.0f, 1.5f, 20, 7, 0.05f, 0.08f, 0.0f );
}

void CFxBlackHole::LoadRes(IndexedTexture *pIndexedTexture, CShadowRect *pShadowRect)
{}
void CFxBlackHole::DeleteRes()
{}

BOOL CFxBlackHole::Render(EffectSort &effect_sort)
{
	if( !GET_D3DDEVICE() || !blackhole )
		return TRUE;

	if(effect_sort.nCurFrame >= m_nTotalFrame)
	{
		m_fScale = 0.0f;
		m_fFade = 1.0f;
		return TRUE;
	}

	float maxScale = 3.0f;

	m_fScale = (maxScale * effect_sort.nCurFrame) / m_nTotalFrame;
	m_fFade = 1.0f - (m_fScale * 0.1f);

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE);
	// Set any appropiate state
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND,   D3DBLEND_SRCALPHA );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND,  D3DBLEND_INVSRCALPHA );

	DWORD  dwTextureState[3];

	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG1, &dwTextureState[0] );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG2, &dwTextureState[1] );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAOP,   &dwTextureState[2] );

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE2X );

	D3DVECTOR	vViewVec;
	vViewVec = pCMyApp->GetEyeDirection();

	D3DVECTOR	vDrawVec;
	vDrawVec = effect_sort.vTargetPos;
	// nate 2004 - 7 - image
	if( g_pCapsyongTexture )
		GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_BLACKHOLE ] ) );

	blackhole->SetColor(1.0f, 0.8f, 0.8f, m_fFade);
	blackhole->SetRotationY(g_PI);
	blackhole->RotateY( m_fScale * 20.0f );
	blackhole->Scale( 1.0f - (m_fScale*0.20f), 1.0f, 1.0f - (m_fScale*0.20f));
	blackhole->SetTranslationXYZ(vDrawVec.x, vDrawVec.y-m_fScale, vDrawVec.z);
	blackhole->Render2();

	blackhole->SetColor(0.8f, 1.0f, 0.8f, m_fFade);
	blackhole->SetRotationY(g_PI*0.5f);
	blackhole->RotateY( m_fScale * 5.0f );
	blackhole->Scale( 1.0f - (m_fScale*0.70f), 1.0f, 1.0f - (m_fScale*0.70f));
	blackhole->SetTranslationXYZ(vDrawVec.x, vDrawVec.y-m_fScale, vDrawVec.z);
	blackhole->Render2();

	LPDIRECTDRAWSURFACE7 lpSurface = NULL;

	if( g_pShadowRect )
	{
		// nate 2004 - 7 - image
		if( g_pCapsyongTexture )
			lpSurface = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_BLACKHOLE_EARTH + effect_sort.Index ] );

		// nate 2004 - 7 글로벌 ShadowRect사용
		g_pShadowRect->SetTileCoord(vDrawVec, 30, FALSE);		// 부울값은 시간인가 아닌가
		g_pShadowRect->Render(lpSurface, FALSE);	// 부울값은 리얼그림자인가 아닌가?
	}

	if( effect_sort.num >= 2 )
	{
		effect_sort.num = 0;
		effect_sort.Index++;
		if( effect_sort.Index >= 4 )
			effect_sort.Index = 0;
	}

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState[2] );

	return FALSE;
}

