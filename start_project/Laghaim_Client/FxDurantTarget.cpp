#include "stdafx.h"
#define D3D_OVERLOADS

#include <d3d.h>
#include <time.h>
#include "FxSet.h"
#include "D3DUtil.h"
#include "D3DMath.h"
#include "sfxUtil.h"
#include "SFXBaseShape.h"
#include "FxDurantTarget.h"
#include "main.h"
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
#include "NkCharacter.h"
#include "NkMob.h"



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFxDurantTarget::CFxDurantTarget()
	: sphere(NULL)
	, m_fRadius(0)
	, m_fCycleRadius(0)
	, m_fHeight(0)
	, m_pMesh(NULL)
	, m_pIndices(NULL)
	, m_nTotalFrame(0)
{
	memset(m_ShowupRing, 0, sizeof(m_ShowupRing));
	memset(m_nStartRingSize, 0, sizeof(m_nStartRingSize));

	light1		= NULL;
	light2		= NULL;
	light3		= NULL;
	m_numTLight	= 0;

	Create();
}

CFxDurantTarget::~CFxDurantTarget()
{
	DeleteRes();
}

void CFxDurantTarget::Create()
{
	DeleteRes();

	light1 = new CRectPlane;
	if( light1 )
		light1->Create( 10.0f, 10.0f, TRUE );
	light2 = new CRectPlane;
	if( light2 )
		light2->Create( 10.0f, 10.0f, TRUE );
	light3 = new CRectPlane;
	if( light3 )
		light3->Create( 10.0f, 10.0f, TRUE );
}

void CFxDurantTarget::LoadRes()
{}

void CFxDurantTarget::DeleteRes()
{
	SAFE_DELETE(light1);
	SAFE_DELETE(light2);
	SAFE_DELETE(light3);
}

void CFxDurantTarget::RenderLight(EffectSort &effect_sort)
{
	LPDIRECTDRAWSURFACE7	lpSurface = NULL;
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND,   D3DBLEND_ONE );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND,  D3DBLEND_ONE );

	DWORD  dwTextureState[3];

	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG1, &dwTextureState[0] );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG2, &dwTextureState[1] );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAOP,   &dwTextureState[2] );

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE2X );

	int nFrame = effect_sort.nCurFrame;
	D3DVECTOR vPos = D3DVECTOR(effect_sort.pNkChaTo->m_wx, effect_sort.pNkChaTo->m_wy, effect_sort.pNkChaTo->m_wz );

	if( ++m_numTLight >= 4 )
		m_numTLight = 0;

	if (nFrame > 20)
	{
		// nate 2004 - 7 - image
		lpSurface = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_ELECT + m_numTLight ] );
		GET_D3DDEVICE()->SetTexture(0, lpSurface);

		if( light1 )
		{
			light1->SetColor(0.8f, 0.8f, 1.0f, 1.0f);
			light1->SetRotationY( g_PI * 0.5f );
			light1->RotateZ( -nFrame * 0.3f );
			light1->Scale(3.0f, 0.0f, 3.0f);
			light1->SetTranslationXYZ(vPos.x, vPos.y + (nFrame - 10) * 0.35, vPos.z);
			light1->Render();
		}

		if( light2 )
		{
			light2->SetColor(0.8f, 0.8f, 1.0f, 1.0f);
			light2->SetRotationX( g_PI * 0.5f );
			light2->RotateZ( -nFrame * 0.3f );
			light2->Scale(3.0f, 0.0f, 3.0f);
			light2->SetTranslationXYZ(vPos.x, vPos.y + (nFrame - 10) * 0.35, vPos.z);
			light2->Render();
		}

		if( light3 )
		{
			light3->SetColor(0.8f, 0.8f, 1.0f, 1.0f);
			light3->SetRotationY( g_PI * 0.5f );
			light3->RotateX( -nFrame * 0.3f );
			light3->Scale(3.0f, 0.0f, 3.0f);
			light3->SetTranslationXYZ(vPos.x, vPos.y + (nFrame - 10) * 0.35, vPos.z);
			light3->Render();
		}
	}

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState[2] );
}

void CFxDurantTarget::Render(EffectSort &effect_sort)
{
	if( !GET_D3DDEVICE() )
		return;

	if( !effect_sort.pNkChaTo )
		return;

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE);

	RenderLight(effect_sort);

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE);
}