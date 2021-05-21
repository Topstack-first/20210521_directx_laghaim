#include "stdafx.h"
#define D3D_OVERLOADS

#include <d3d.h>
#include <time.h>
#include <assert.h>
#include "FxSet.h"
#include "sfx.h"
#include "main.h"
#include "UIMgr.h"
#include "D3DMath.h"
#include "D3DUtil.h"
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
#include "ItemProto.h"


#define FX_CHAIN_NOISE		20
#define FX_CHAIN_WIDTH		(2.0f)
#define FX_CHAIN_TEXTR_COUNT	4


CFxChaos::CFxChaos()
	: m_fScale(0)
	, m_fFade(0)
{
	ring = NULL;
	ring = new CCylinderPlane;
	if(ring)
		ring->Create(5.0f, 2.0f, 12.0f, 10.0f, TRUE);

	wave = NULL;
	wave = new CCylinderPlane;
	if( wave )
		wave->Create(7.0f, 3.0f, 0.5f, 20.0f, TRUE);
}

CFxChaos::~CFxChaos()
{
	DeleteRes();

	SAFE_DELETE(wave);
	SAFE_DELETE(ring);
}

void CFxChaos::LoadRes(IndexedTexture *pIndexedTexture, CShadowRect *pShadowRect)
{}

void CFxChaos::DeleteRes()
{
}

void CFxChaos::Render(EffectSort &effect_sort)
{
	if( !GET_D3DDEVICE() )
		return;

	LPDIRECTDRAWSURFACE7 lpSurface = NULL;

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND,   D3DBLEND_SRCALPHA );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND,  D3DBLEND_INVSRCALPHA );

	// 방향벡터를 구한다.
	D3DVECTOR vPosFrom, vPosTo;

	// 시작위치가 고정. 시전 객체가 움직일 수도 있기 때문에.
	vPosFrom = D3DVECTOR(effect_sort.vTargetPos.x, effect_sort.vTargetPos.y + 50.0f, effect_sort.vTargetPos.z);
	vPosTo = D3DVECTOR(effect_sort.vTargetPos.x, effect_sort.vTargetPos.y - 10.0f, effect_sort.vTargetPos.z);

	float maxScale = 3.0f;

	m_fScale = (maxScale * effect_sort.nCurFrame) / FX_CHAOS_FRAMECOUNT;
	m_fFade = 1.0f - (m_fScale * 0.3f);

	D3DVECTOR	vViewVec;
	vViewVec = pCMyApp->GetEyeDirection();

	// nate 2004 - 7 - image
	if( g_pCapsyongTexture )
		GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_CHAOS_RING ] ) );

	if( ring )
	{
		ring->SetColor(1.0f, 1.0f, 1.0f, 0.35f);
		ring->SetRotationY( g_PI * 0.5f );
		ring->RotateY( m_fScale * 10.0f );
		ring->Scale( 3.5f, 10.0f, 3.5f );
		ring->SetTranslationXYZ(vPosFrom.x, vPosFrom.y - RandomNum(19.0f, 20.0f), vPosFrom.z);
		ring->Render();

		ring->SetColor(1.0f, 1.0f, 1.0f, 0.25f);
		ring->SetRotationY( g_PI * 0.5f );
		ring->RotateY( m_fScale * 20.0f );
		ring->Scale( 2.5f, 5.0f, 2.5f );
		ring->SetTranslationXYZ(vPosFrom.x, vPosFrom.y - RandomNum(34.0f, 35.0f), vPosFrom.z);
		ring->Render();

		ring->SetColor(1.0f, 1.0f, 1.0f, 0.15f);
		ring->SetRotationY( g_PI * 0.5f );
		ring->RotateY( m_fScale * 40.0f );
		ring->Scale( 1.5f, 2.5f, 1.5f );
		ring->SetTranslationXYZ(vPosFrom.x, vPosFrom.y - RandomNum(43.0f, 45.0f), vPosFrom.z);
		ring->Render();
	}

	if( wave )
	{
		wave->SetColor(1.0f, 1.0f, 1.0f, m_fFade);
		wave->SetRotationY( g_PI * 0.5f );
		wave->RotateY( m_fScale * 5.0f );
		wave->Scale( m_fScale + 1.0f, 1.0f, m_fScale + 1.0f );
		wave->SetTranslationXYZ(vPosFrom.x, vPosFrom.y - 50.0f + m_fScale, vPosFrom.z);
		wave->Render();

		wave->SetColor(1.0f, 1.0f, 1.0f, m_fFade * 0.35f);
		wave->SetRotationY( g_PI * 0.5f );
		wave->RotateY( m_fScale * 10.0f );
		wave->Scale( m_fScale*2.5f + 1.0f, 1.0f, m_fScale*2.5f + 1.0f );
		wave->SetTranslationXYZ(vPosFrom.x, vPosFrom.y - 50.0f + m_fScale + 1.0f, vPosFrom.z);
		wave->Render();
	}

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE);
}
