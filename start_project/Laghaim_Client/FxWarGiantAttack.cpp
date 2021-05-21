#include "stdafx.h"
#include "sfx.h"
#include "main.h"
// nate 2004 - 7 - image
#include "ImageDefine.h"
#include "NkMob.h" ///인던1 시 추가
#include "NkCharacter.h" ///인던1 시 추가



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CFxWarGiantAttack::CFxWarGiantAttack()
{
	hit					= NULL;
	hit_boom			= NULL;
	m_fScale			= 0.0f;
	m_fFade				= 1.0f;
	m_nTotalFrame		= FX_WARGIANTATTACK_FRAMECOUNT;

	Create();
}

CFxWarGiantAttack::~CFxWarGiantAttack()
{
	SAFE_DELETE(hit);
	SAFE_DELETE(hit_boom);
}

void CFxWarGiantAttack::Create()
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

void CFxWarGiantAttack::LoadRes(IndexedTexture *pIndexedTexture)
{}
void CFxWarGiantAttack::DeleteRes()
{}
void CFxWarGiantAttack::SetTextureNum()
{}

BOOL CFxWarGiantAttack::Render(EffectSort &effect_sort)
{
	if( !GET_D3DDEVICE() )
		return true;

	if(effect_sort.nCurFrame >= m_nTotalFrame)
	{
		m_fScale = 0.0f;
		m_fFade = 1.0f;
		return TRUE;
	}

	if( effect_sort.nCurFrame <= 0 )
		return FALSE;

	float maxScale = 2.0f;
	m_fScale = (maxScale * effect_sort.nCurFrame) / m_nTotalFrame;
	m_fFade = 1.0f - (m_fScale * 0.15f);

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

	D3DVECTOR		vAdj;
	vAdj = effect_sort.vTargetPos - effect_sort.vPos;
	vAdj = Normalize(vAdj);

	D3DVECTOR		vDrawPos;

	if( effect_sort.subNum == 1 ) //레그녹 타격이펙트인 경우 ///인던1
	{
		// 공격이 느리므로 타격이펙트가 목표를 제대로 따라가도록 설정.
		float TargetHeight = 0.0f;

		if( effect_sort.pNkChaTo )
		{
			TargetHeight = MOB_HEIGHT_MIDDLE_VAL;

			vDrawPos = D3DVECTOR( effect_sort.pNkChaTo->m_wx,
								  effect_sort.pNkChaTo->m_wy+TargetHeight,
								  effect_sort.pNkChaTo->m_wz );
		}
		else if( effect_sort.pMobTo )
		{
			switch (effect_sort.pMobTo->m_Height)
			{
			case MOB_HEIGHT_HIGH_NUM:
				TargetHeight = MOB_HEIGHT_HIGH_VAL;
				break;
			case MOB_HEIGHT_LOW_NUM:
				TargetHeight = MOB_HEIGHT_LOW_VAL;
				break;
			default: // MOB_HEIGHT_MIDDLE_NUM:
				TargetHeight = MOB_HEIGHT_MIDDLE_VAL;
				break;
			}

			vDrawPos = D3DVECTOR( effect_sort.pMobTo->m_wx,
								  effect_sort.pMobTo->m_wy+TargetHeight,
								  effect_sort.pMobTo->m_wz );
		}
		else
		{
			vDrawPos = D3DVECTOR( effect_sort.vTargetPos.x,
								  effect_sort.vTargetPos.y,
								  effect_sort.vTargetPos.z );
		}
	}
	else // 일반 워자.
	{
		vDrawPos = D3DVECTOR( effect_sort.vTargetPos.x - (vAdj.x*5),
							  effect_sort.vTargetPos.y,
							  effect_sort.vTargetPos.z - (vAdj.z*5) );
	}


	float red, green, blue;
	red = green = blue = 1.0f;

	float x_scale;
	x_scale = 1.5f;

	red = 0.75f - (0.75f * m_fScale * 0.3f);
	green = 0.75f - (0.75f * m_fScale * 0.3f);
	blue = 1.0f - (1.0f * m_fScale * 0.3f);

	x_scale = 1.2f;

	if( hit )
	{
		// nate 2004 - 7 - image
		GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_WARGIANTATTACK_HIT ] ) );
		hit->SetColor(red, green, blue, m_fFade);
		hit->ReCreate(m_fScale * 0.25f);
		hit->SetBillBoard(vViewVec, FALSE, 0.0f);
		hit->Scale(m_fScale * 1.5f, m_fScale * 1.5f, 0.0f);
		hit->SetTranslationXYZ(vDrawPos.x, vDrawPos.y, vDrawPos.z);
		hit->Render();
	}


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

	maxScale = 2.0f;
	m_fFade = 1.0f - (m_fScale * 0.4f);
	m_fScale = (maxScale * effect_sort.nCurFrame) / m_nTotalFrame;

	red *= 0.5f;
	green *= 0.5f;
	blue *= 0.5f;

	if( hit_boom )
	{
		// nate 2004 - 7 - image
		GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_WARGIANTATTACK_HITBOOM ] ) );

		hit_boom->SetColor(red, green, blue, m_fFade);
		hit_boom->SetRotationY(ry);
		hit_boom->RotateX(g_PI * 0.5f);
		hit_boom->Scale(x_scale, m_fScale * 1.3f, 0.0f);
		hit_boom->SetTranslationXYZ(vDrawPos2.x, vDrawPos2.y, vDrawPos2.z);
		hit_boom->Render();

		hit_boom->SetRotationY(ry+0.3f);
		hit_boom->RotateX(g_PI * 0.25f);
		hit_boom->Scale(x_scale, m_fScale * 1.2f, 0.0f);
		hit_boom->SetTranslationXYZ(vDrawPos2.x, vDrawPos2.y, vDrawPos2.z);
		hit_boom->Render();

		hit_boom->SetRotationY(ry+0.6f);
		hit_boom->RotateX(g_PI * 0.45f);
		hit_boom->Scale(x_scale, m_fScale * 1.25f, 0.0f);
		hit_boom->SetTranslationXYZ(vDrawPos2.x, vDrawPos2.y, vDrawPos2.z);
		hit_boom->Render();

		hit_boom->SetRotationY(ry-0.3f);
		hit_boom->RotateX(g_PI * 0.35f);
		hit_boom->Scale(x_scale, m_fScale * 1.35f, 0.0f);
		hit_boom->SetTranslationXYZ(vDrawPos2.x, vDrawPos2.y, vDrawPos2.z);
		hit_boom->Render();

		hit_boom->SetRotationY(ry-0.6f);
		hit_boom->RotateX(g_PI * 0.55f);
		hit_boom->Scale(x_scale, m_fScale * 1.15f, 0.0f);
		hit_boom->SetTranslationXYZ(vDrawPos2.x, vDrawPos2.y, vDrawPos2.z);
		hit_boom->Render();
	}

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE);

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState[2] );

	return FALSE;
}

