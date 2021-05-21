#include "stdafx.h"
#include "sfx.h"
#include "main.h"
#include "ImageDefine.h"



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFxHitBoomEffects::CFxHitBoomEffects()
{
	hit					= NULL;
	hit_boom			= NULL;
	m_fScale			= 0.0f;
	m_fFade				= 1.0f;
	m_nTotalFrame		= FX_WEAPONEFF_FRAMECOUNT;

	Create();
}

CFxHitBoomEffects::~CFxHitBoomEffects()
{
	SAFE_DELETE(hit);
	SAFE_DELETE(hit_boom);
}

void CFxHitBoomEffects::Create()
{
	SAFE_DELETE(hit);
	SAFE_DELETE(hit_boom);

	hit = new CRectPlane;
	if( hit )
		hit->Create( 10.0f, 10.0f, FALSE );
	hit_boom = new CRectPlane;
	if( hit_boom )
		hit_boom->Create( 1.0f, 30.0f, 1.5f, FALSE );
}

void CFxHitBoomEffects::LoadRes(IndexedTexture *pIndexedTexture)
{}
void CFxHitBoomEffects::DeleteRes()
{}
void CFxHitBoomEffects::SetTextureNum()
{}

//-- 검의 타격 Effect 를 render 한다.
//-- 2005.01.28
//-- Lyul / 이도류 이펙트 추가.
//-- 미안하다 하드코딩이다.
BOOL CFxHitBoomEffects::Render(EffectSort &effect_sort)
{
	if( !GET_D3DDEVICE() )
		return true;

	float rot = 0.0f;
	if(effect_sort.nCurFrame >= m_nTotalFrame && effect_sort.num == 1)
	{
		m_fScale = 0.0f;
		m_fFade = 1.0f;
		return TRUE;
	}
	else if(effect_sort.nCurFrame >= 6 && effect_sort.num == 0)
	{
		m_fScale = 0.0f;
		m_fFade = 1.0f;
		return TRUE;
	}

	float maxScale = 2.0f;

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
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );

	D3DVECTOR	vViewVec;
	vViewVec = pCMyApp->GetEyeDirection();

	D3DVECTOR		vAdj;
	vAdj = effect_sort.vTargetPos - effect_sort.vPos;
	vAdj = Normalize(vAdj);

	D3DVECTOR		vDrawPos;
	vDrawPos = D3DVECTOR( effect_sort.vTargetPos.x - (vAdj.x*5),
						  effect_sort.vTargetPos.y,
						  effect_sort.vTargetPos.z - (vAdj.z*5) );
	//--------------------------------------------------------------------------
	//-- 이도류일 경우 라인 Effect 를 출력한다.
	if(effect_sort.num == 1)
	{
		if(effect_sort.nCurFrame < 6)
		{
			m_fScale = ((effect_sort.nCurFrame) * (effect_sort.nCurFrame)) / 20.0f;
			m_fFade = 1.0f;
			rot = effect_sort.dwColor;
		}
		else if(effect_sort.nCurFrame > 8 && effect_sort.nCurFrame < 15)
		{
			m_fScale = ((effect_sort.nCurFrame-8) * (effect_sort.nCurFrame-10)) / 20.0f;
			m_fFade = 1.0f;
			rot = effect_sort.subNum;
		}
		else
		{
			m_fFade = 0.0f;
		}


		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND,  D3DBLEND_ONE);
		if( g_pCapsyongTexture )
			GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_DSWORD_HIT ] ) );
		if( hit )
		{
			hit->SetColor(1.0f, 1.0f, 1.0f, m_fFade);
			hit->SetBillBoard(vViewVec, FALSE, 0.0f);
			hit->Scale(m_fScale*2, m_fScale*2, 0.0f);
			hit->SetTranslationXYZ(vDrawPos.x, vDrawPos.y, vDrawPos.z);
			hit->RotateZ((float)rot /50.0f);
			hit->Render();
		}
	}
	//--------------------------------------------------------------------------
	//-- 일반 Effect
	else
	{

		m_fScale = (maxScale * effect_sort.nCurFrame) / 6;
		m_fFade = 1.0f - (m_fScale * 0.15f);
		// nate 2004 - 7 - image
		if( g_pCapsyongTexture )
			GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_HITBOOM ] ) );
		if( hit )
		{
			hit->SetColor(1.0f, 1.0f, 1.0f, m_fFade);
			hit->SetBillBoard(vViewVec, FALSE, 0.0f);
			hit->Scale(m_fScale, m_fScale, 0.0f);
			hit->SetTranslationXYZ(vDrawPos.x, vDrawPos.y, vDrawPos.z);
			hit->Render();
		}

		// nate 2004 - 7 - image
		GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_HITBOOMEFFECTS ] ) );

		float		ry = 0.0f;

		vAdj = effect_sort.vTargetPos - effect_sort.vPos;
		ry = atan2f(vAdj.x, vAdj.z);

		vAdj = Normalize(vAdj);

		D3DVECTOR		vDrawPos2;
		vDrawPos2 = D3DVECTOR(  vDrawPos.x + (vAdj.x * effect_sort.nCurFrame * 0.15f),
								vDrawPos.y,
								vDrawPos.z + (vAdj.z * effect_sort.nCurFrame * 0.15f) );

		D3DVECTOR		vDir;
		D3DVECTOR		test;

		test.x = vDrawPos.x * (float)sin(g_PI * 0.5f);
		test.z = vDrawPos.z * (float)cos(g_PI * 0.5f);
		test.y = vDrawPos.y;

		vDir = vDrawPos - test;

		if( (Hit_Index)effect_sort.Index == HIT_SOWRD_ROTATE_ATTACK ||
				(Hit_Index)effect_sort.Index == HIT_AXE_JUMP_ATTACK )
		{
			maxScale = 2.0f;			// 돌려베기, 내려찍기 거리정도...
			m_fFade = 1.0f - (m_fScale * 0.4f);
		}
		else
		{
			maxScale = 1.5f;
			m_fFade = 1.0f - (m_fScale * 0.5f);
		}

		m_fScale = (maxScale * effect_sort.nCurFrame) / 6;

		if( hit_boom )
		{
			hit_boom->SetColor(1.0f, 1.0f, 1.0f, m_fFade);
			hit_boom->SetRotationY(ry);
			hit_boom->RotateX(g_PI * 0.5f);
			hit_boom->Scale(1.5f, m_fScale * 1.3f, 0.0f);
			hit_boom->SetTranslationXYZ(vDrawPos2.x, vDrawPos2.y, vDrawPos2.z);
			hit_boom->Render();

			//	hit_boom->SetColor(1.0f, 1.0f, 1.0f, m_fFade);
			hit_boom->SetRotationY(ry+0.3f);
			hit_boom->RotateX(g_PI * 0.25f);
			hit_boom->Scale(1.5f, m_fScale * 1.2f, 0.0f);
			hit_boom->SetTranslationXYZ(vDrawPos2.x, vDrawPos2.y, vDrawPos2.z);
			hit_boom->Render();

			//	hit_boom->SetColor(1.0f, 1.0f, 1.0f, m_fFade);
			hit_boom->SetRotationY(ry+0.6f);
			hit_boom->RotateX(g_PI * 0.45f);
			hit_boom->Scale(1.5f, m_fScale * 1.25f, 0.0f);
			hit_boom->SetTranslationXYZ(vDrawPos2.x, vDrawPos2.y, vDrawPos2.z);
			hit_boom->Render();

			//	hit_boom->SetColor(1.0f, 1.0f, 1.0f, m_fFade);
			hit_boom->SetRotationY(ry-0.3f);
			hit_boom->RotateX(g_PI * 0.35f);
			hit_boom->Scale(1.5f, m_fScale * 1.35f, 0.0f);
			hit_boom->SetTranslationXYZ(vDrawPos2.x, vDrawPos2.y, vDrawPos2.z);
			hit_boom->Render();

			//	hit_boom->SetColor(1.0f, 1.0f, 1.0f, m_fFade);
			hit_boom->SetRotationY(ry-0.6f);
			hit_boom->RotateX(g_PI * 0.55f);
			hit_boom->Scale(1.5f, m_fScale * 1.15f, 0.0f);
			hit_boom->SetTranslationXYZ(vDrawPos2.x, vDrawPos2.y, vDrawPos2.z);
			hit_boom->Render();
		}
	}
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE);

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState[2] );

	return FALSE;
}
