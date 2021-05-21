#include "stdafx.h"
#include "sfx.h"
#include "main.h"
#include "skill.h"
#include "ImageDefine.h"



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFxHitPowerEffects::CFxHitPowerEffects()
{
	hit					= NULL;
	hit_boom			= NULL;
	hit_ring			= NULL;
	hit_cross			= NULL;
	hit_sphere			= NULL;
	m_fScale			= 0.0f;
	m_fFade				= 1.0f;
	m_nTotalFrame		= FX_HITPOWEREFFECTS_FRAMECOUNT;

	Create();
}

CFxHitPowerEffects::~CFxHitPowerEffects()
{
	SAFE_DELETE(hit);
	SAFE_DELETE(hit_boom);
	SAFE_DELETE(hit_ring);
	SAFE_DELETE(hit_cross);
	SAFE_DELETE(hit_sphere);
}

void CFxHitPowerEffects::Create()
{
	SAFE_DELETE(hit);
	SAFE_DELETE(hit_boom);
	SAFE_DELETE(hit_ring);
	SAFE_DELETE(hit_cross);
	SAFE_DELETE(hit_sphere);

	hit = new CRectPlane;
	if( hit )
		hit->Create( 2.5f, 2.5f, FALSE );

	hit_boom = new CRectPlane;
	if( hit_boom )
		hit_boom->Create( 0.7f, 7.0f, 0.0f, TRUE );

	hit_ring = new CCylinderPlane;
	if( hit_ring )
		hit_ring->Create( 5.0f, 2.0f, 2.0f, 20.0f, TRUE );

	hit_cross = new CCylinderPlane;
	if( hit_cross )
		hit_cross->Create( 5.0f, 2.0f, 0.5f, 20.0f, TRUE );

	hit_sphere = new CSphere;
	if( hit_sphere )
		hit_sphere->Create();
}

void CFxHitPowerEffects::LoadRes(IndexedTexture *pIndexedTexture)
{}
void CFxHitPowerEffects::DeleteRes()
{}
void CFxHitPowerEffects::SetTextureNum()
{}

BOOL CFxHitPowerEffects::Render(EffectSort &effect_sort)
{
	if( !GET_D3DDEVICE() )
		return true;

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
	// Set any appropiate state
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND,   D3DBLEND_ONE );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND,  D3DBLEND_ONE);

	DWORD  dwTextureState[3];

	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG1, &dwTextureState[0] );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG2, &dwTextureState[1] );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_COLOROP,   &dwTextureState[2] );

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE2X );

	D3DVECTOR	vViewVec;
	vViewVec = pCMyApp->GetEyeDirection();

	D3DVECTOR		vAdj;
	vAdj = effect_sort.vTargetPos - effect_sort.vPos;
	vAdj = Normalize(vAdj);

	D3DVECTOR		vDrawPos;
	vDrawPos = D3DVECTOR( effect_sort.vTargetPos.x - (vAdj.x*5),
						  effect_sort.vTargetPos.y,
						  effect_sort.vTargetPos.z - (vAdj.z*5) );

	float red, green, blue;
	red = 1.0f - (0.2f * m_fScale * 0.3f);
	green = 0.6f - (0.4f * m_fScale * 0.3f);
	blue = 0.8f - (0.5f * m_fScale * 0.3f);

	// nate 2004 - 7 - image
	if( g_pCapsyongTexture )
		GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_HITPOWER_HIT ] ) );

	if( hit )
	{
		hit->SetColor(red, green, blue, m_fFade);
		hit->SetBillBoard(vViewVec, FALSE, 0.0f);
		hit->Scale(m_fScale*12.0f + 3.0f, m_fScale*12.0f + 3.0f, 0.0f);
		hit->SetTranslationXYZ(vDrawPos.x, vDrawPos.y, vDrawPos.z);
		hit->Render();
	}

	// nate 2004 - 7 - image
	if( g_pCapsyongTexture )
		GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_HITPOWER_HITBOOM ] ) );

	float angle = -1.0f;
	float scale = 5.0f;

	if( hit_boom )
	{
		for( int i = 0; i < 10; i++ )
		{
			red = 0.6f - (0.4f * m_fScale * 0.3f);
			green = 0.6f - (0.4f * m_fScale * 0.3f);
			blue = 1.0f - (0.5f * m_fScale * 0.3f);

			hit_boom->SetColor(red, green, blue, m_fFade);
			hit_boom->SetRotationY(g_PI * angle);
			hit_boom->RotateX(-g_PI*0.15f);
			hit_boom->Scale(1.5f, 1.5f, m_fScale*scale + 1.5f);
			hit_boom->SetTranslationXYZ(vDrawPos.x, vDrawPos.y, vDrawPos.z);
			hit_boom->Render();

			red = 0.5f - (0.3f * m_fScale * 0.3f);
			green = 0.5f - (0.3f * m_fScale * 0.3f);
			blue = 0.8f - (0.5f * m_fScale * 0.3f);

			hit_boom->SetColor(red, green, blue, m_fFade);
			hit_boom->SetRotationY(g_PI * angle);
			hit_boom->RotateX(g_PI*0.15f);
			hit_boom->Scale(0.5f, 0.5f, m_fScale*scale + 0.5f);
			hit_boom->SetTranslationXYZ(vDrawPos.x, vDrawPos.y, vDrawPos.z);
			hit_boom->Render();

			angle += 0.2f;
			scale += 0.05f;
			scale *= -1.0f;
		}
	}

	red = 0.6f - (0.3f * m_fScale * 0.3f);
	green = 0.6f - (0.5f * m_fScale * 0.3f);
	blue = 1.0f - (0.3f * m_fScale * 0.3f);

	// nate 2004 - 7 - image
	if( g_pCapsyongTexture )
		GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_HITPOWER_HIT ] ) );

	if( hit_ring && effect_sort.Index >= ATTACK_CRITICAL_HIT )
	{
		hit_ring->SetColor(red, green, blue, m_fFade);
		hit_ring->SetRotationY(g_PI);
		hit_ring->RotateX(-g_PI*0.15f);
		hit_ring->Scale(m_fScale*3.0f + 2.0f, 1.0f, m_fScale*3.0f + 2.0f);
		hit_ring->SetTranslationXYZ(vDrawPos.x, vDrawPos.y, vDrawPos.z);
		hit_ring->Render();

		hit_ring->SetColor(red, green, blue, m_fFade);
		hit_ring->SetRotationY(g_PI);
		hit_ring->RotateX(-g_PI*0.10f);
		hit_ring->Scale(m_fScale*3.0f + 2.0f, 1.0f, m_fScale*3.0f + 2.0f);
		hit_ring->SetTranslationXYZ(vDrawPos.x, vDrawPos.y, vDrawPos.z);
		hit_ring->Render();

		hit_ring->SetColor(red, green, blue, m_fFade);
		hit_ring->SetRotationY(g_PI);
		hit_ring->RotateX(g_PI*0.10f);
		hit_ring->Scale(m_fScale*3.0f + 2.0f, 1.0f, m_fScale*3.0f + 2.0f);
		hit_ring->SetTranslationXYZ(vDrawPos.x, vDrawPos.y, vDrawPos.z);
		hit_ring->Render();
	}

	red = 0.6f - (0.5f * m_fScale * 0.3f);
	green = 0.5f - (0.5f * m_fScale * 0.3f);
	blue = 0.6f - (0.5f * m_fScale * 0.3f);

	if( hit_sphere && effect_sort.Index >= ATTACK_DEMOLITION_HIT )
	{
		hit_sphere->SetColor(red, green, blue, m_fFade);
		hit_sphere->SetScaling(-m_fScale*17.0f + 2.0f, m_fScale*17.0f + 1.0f, - m_fScale*17.0f - 2.0f);
		hit_sphere->SetTranslationXYZ(vDrawPos.x, vDrawPos.y, vDrawPos.z);
		hit_sphere->Render();

		hit_sphere->SetColor(red, green, blue, m_fFade);
		hit_sphere->SetScaling(m_fScale*17.0f + 2.0f, m_fScale*17.0f + 1.0f, - m_fScale*17.0f - 2.0f);
		hit_sphere->SetTranslationXYZ(vDrawPos.x, vDrawPos.y, vDrawPos.z);
		hit_sphere->Render();

		hit_sphere->SetColor(red, green, blue, m_fFade);
		hit_sphere->SetScaling( m_fScale*17.0f + 1.0f, m_fScale*17.0f + 1.0f, m_fScale*17.0f + 2.0f);
		hit_sphere->SetTranslationXYZ(vDrawPos.x, vDrawPos.y, vDrawPos.z);
		hit_sphere->Render();
	}

//	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND,   D3DBLEND_SRCALPHA );
//	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND,  D3DBLEND_DESTALPHA );

	red = 1.0f;// - (0.9f * m_fScale * 0.1f);
	green = 0.4f;// - (0.4f * m_fScale * 0.1f);
	blue = 0.5f;// - (0.5f * m_fScale * 0.1f);

	if( hit_cross && effect_sort.Index >= ATTACK_CROSS_HIT )
	{
		// nate 2004 - 7 - image
		if( g_pCapsyongTexture )
			GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_HITPOWER_HITCROSS ] ) );

		float		ry = 0.0f;
		vAdj = effect_sort.vTargetPos - effect_sort.vPos;
		ry = atan2f(vAdj.x, vAdj.z);

		vDrawPos = D3DVECTOR( effect_sort.vTargetPos.x, effect_sort.vTargetPos.y, effect_sort.vTargetPos.z);

		hit_cross->SetColor(red, green, blue, m_fFade);
		hit_cross->SetRotationY(ry+(g_PI*0.5f));
		hit_cross->RotateX(-g_PI*0.15f);
		hit_cross->RotateY( -m_fScale * 10.0f );
		hit_cross->Scale(3.5f, 1.0f, 3.5f);
		hit_cross->SetTranslationXYZ(vDrawPos.x, vDrawPos.y, vDrawPos.z);
		hit_cross->Render();

		hit_cross->SetColor(red, green, blue, m_fFade);
		hit_cross->SetRotationY(ry+(g_PI*0.5f));
		hit_cross->RotateX(g_PI*0.15f);
		hit_cross->RotateY( -m_fScale * 10.0f );
		hit_cross->Scale(3.5f, 1.0f, 3.5f);
		hit_cross->SetTranslationXYZ(vDrawPos.x, vDrawPos.y, vDrawPos.z);
		hit_cross->Render();
	}

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE);

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_COLOROP,   dwTextureState[2] );

	return FALSE;
}

