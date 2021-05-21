#include "stdafx.h"
///자유대련
#include "main.h"
#include "ImageDefine.h"
#include "NkCharacter.h"
#include "FxDmitronEffects.h"

extern float g_fTimeKey;

float BiModule(DWORD Time, int Modulo)
{
	int Family = Modulo >> 1;
	DWORD Val = Time % Modulo;
	if ( Val < Family) return Time % Modulo;
	else return 24 - (Time % Modulo);
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CFxDmitronPortal::CFxDmitronPortal()
{
	m_nCurrFrame		= 0;
	m_fSaveTime			= 0;

	m_CirclePlane[0]	= 0;
	m_CirclePlane[1]	= 0;
	m_CirclePlane[2]	= 0;

	m_Rotation			= 0;
}

CFxDmitronPortal::~CFxDmitronPortal()
{
	DeleteRes();
}

void CFxDmitronPortal::LoadRes()
{
	m_CirclePlane[0] = new CRectPlane;
	if( m_CirclePlane[0] )
		m_CirclePlane[0]->Create( 2.0f, 2.0f, FALSE);
	m_CirclePlane[1] = new CRectPlane;
	if( m_CirclePlane[1] )
		m_CirclePlane[1]->Create( 2.0f, 2.0f, FALSE);
	m_CirclePlane[2] = new CRectPlane;
	if( m_CirclePlane[2] )
		m_CirclePlane[2]->Create( 2.0f, 2.0f, FALSE);
}

void CFxDmitronPortal::DeleteRes()
{
	for(int i = 0; i < 3; ++i)
	{
		SAFE_DELETE(m_CirclePlane[i]);
	}
}

void CFxDmitronPortal::Render(EffectSort &effect_sort)
{
	if( !GET_D3DDEVICE() )
		return;

	//변수 설정
	DWORD alphaTest, alphaBlend, srcBlendOld, destBlendOld, dwLighting, dwAmbient, dwFogEnable;

	//5초를 주기로
	float fScale1 = 15.0f + BiModule(effect_sort.nCurFrame, FX_DMITRON_CRYSTAL_FRAMECOUNT);
	float fScale2 = 15.0f + BiModule(effect_sort.nCurFrame+8, FX_DMITRON_CRYSTAL_FRAMECOUNT);
	float fScale3 = 15.0f + BiModule(effect_sort.nCurFrame+16, FX_DMITRON_CRYSTAL_FRAMECOUNT);

	//회전 비 증가
	m_Rotation+=2;

	D3DVECTOR vViewVec;
	vViewVec = pCMyApp->GetEyeDirection();

	D3DVECTOR vCirclePos;
	vCirclePos = D3DVECTOR( effect_sort.vPos.x, effect_sort.vPos.y+1.0f, effect_sort.vPos.z);

	// 상태값 보존.
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_FOGENABLE, &dwFogEnable );
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, &alphaBlend );
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_ALPHATESTENABLE, &alphaTest );
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_SRCBLEND, &srcBlendOld );
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_DESTBLEND, &destBlendOld );
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_LIGHTING, &dwLighting );
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_AMBIENT, &dwAmbient);

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_AMBIENT, 0x00FFFFFF);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, TRUE);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND,   D3DBLEND_SRCALPHA );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND,  D3DBLEND_INVSRCALPHA );

	DWORD  dwTextureState[3];
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG1, &dwTextureState[0] );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG2, &dwTextureState[1] );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAOP,   &dwTextureState[2] );

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );

	//바닥 평면 그리기
	if( m_CirclePlane[0] )
	{
		GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_DMITRON_PORTAL1 ] ));

		m_CirclePlane[0]->SetColor(1.0f, 1.0f, 1.0f, 1.0f);
		m_CirclePlane[0]->SetBillBoard( vViewVec, FALSE, 0.0f);
		m_CirclePlane[0]->RotateZ( (float)m_Rotation * 2 * g_PI*0.00036f );
		m_CirclePlane[0]->Scale(fScale1, fScale1, 10.0f);
		m_CirclePlane[0]->SetTranslationXYZ(vCirclePos.x, vCirclePos.y+15,  vCirclePos.z);
		m_CirclePlane[0]->Render();
	}

	if( m_CirclePlane[1] )
	{
		GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_DMITRON_PORTAL2 ] ));

		m_CirclePlane[1]->SetColor(1.0f, 1.0f, 1.0f, fScale2*0.01f);
		m_CirclePlane[1]->SetBillBoard( vViewVec, FALSE, 0.0f);
		m_CirclePlane[1]->RotateZ( (float)m_Rotation * 2 * g_PI*0.00006f );
		m_CirclePlane[1]->Scale(fScale2, fScale2, 10.0f);
		m_CirclePlane[1]->SetTranslationXYZ(vCirclePos.x, vCirclePos.y+15,  vCirclePos.z);
		m_CirclePlane[1]->Render();
	}

	if( m_CirclePlane[2] )
	{
		GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_DMITRON_PORTAL3 ] ));

		m_CirclePlane[2]->SetColor(1.0f, 1.0f, 1.0f, 1.f);
		m_CirclePlane[2]->SetBillBoard( vViewVec, FALSE, 0.0f);
		//m_CirclePlane[2]->RotateZ( (float)m_Rotation * 2 * g_PI*0.00006f );
		m_CirclePlane[2]->Scale(10.f, 10.f, 10.0f);
		m_CirclePlane[2]->SetTranslationXYZ(vCirclePos.x, vCirclePos.y+15,  vCirclePos.z);
		m_CirclePlane[2]->Render();
	}

	//상태 복원
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE);

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState[2] );

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_FOGENABLE, dwFogEnable );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ALPHATESTENABLE, alphaTest );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, alphaBlend );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND, srcBlendOld);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND, destBlendOld);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_AMBIENT, dwAmbient);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, dwLighting );
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFxDmitronCrystal::CFxDmitronCrystal()
{
	m_Rotation			= 0;
	m_nCurrFrame		= 0;
	m_fSaveTime			= 0;

	m_CirclePlane[0]	= 0;
	m_CirclePlane[1]	= 0;
	m_CirclePlane[2]	= 0;

	m_Rotation			= 0;
}

CFxDmitronCrystal::~CFxDmitronCrystal()
{
	DeleteRes();
}

void CFxDmitronCrystal::LoadRes()
{
	//텍스처 로딩
	m_CirclePlane[0] = new CRectPlane;
	if( m_CirclePlane[0] )
		m_CirclePlane[0]->Create( 2.0f, 2.0f, FALSE);
	m_CirclePlane[1] = new CRectPlane;
	if( m_CirclePlane[1] )
		m_CirclePlane[1]->Create( 2.0f, 2.0f, FALSE);
	m_CirclePlane[2] = new CRectPlane;
	if( m_CirclePlane[2] )
		m_CirclePlane[2]->Create( 2.0f, 2.0f, FALSE);
}

void CFxDmitronCrystal::DeleteRes()
{
	SAFE_DELETE(m_CirclePlane[0]);
	SAFE_DELETE(m_CirclePlane[1]);
	SAFE_DELETE(m_CirclePlane[2]);
}


void CFxDmitronCrystal::Render(EffectSort &effect_sort)
{
	if( !GET_D3DDEVICE() )
		return;

	//변수 설정
	DWORD alphaTest, alphaBlend, srcBlendOld, destBlendOld, dwLighting, dwAmbient, dwFogEnable;

	//5초를 주기로
	float fScale1 = 15.0f + BiModule(effect_sort.nCurFrame, FX_DMITRON_CRYSTAL_FRAMECOUNT);
	float fScale2 = 15.0f + BiModule(effect_sort.nCurFrame+8, FX_DMITRON_CRYSTAL_FRAMECOUNT);
	float fScale3 = 15.0f + BiModule(effect_sort.nCurFrame+16, FX_DMITRON_CRYSTAL_FRAMECOUNT);

	//회전 비 증가
	m_Rotation+=2;

	D3DVECTOR vViewVec;
	vViewVec = pCMyApp->GetEyeDirection();

	D3DVECTOR vCirclePos;
	vCirclePos = D3DVECTOR( effect_sort.vPos.x, effect_sort.vPos.y+1.0f, effect_sort.vPos.z);

	// 상태값 보존.
	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_FOGENABLE, &dwFogEnable );
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, &alphaBlend );
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_ALPHATESTENABLE, &alphaTest );
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_SRCBLEND, &srcBlendOld );
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_DESTBLEND, &destBlendOld );
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_LIGHTING, &dwLighting );
	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_AMBIENT, &dwAmbient);

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, TRUE);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND,   D3DBLEND_SRCALPHA );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND,  D3DBLEND_INVSRCALPHA );

	DWORD  dwTextureState[3];
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG1, &dwTextureState[0] );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG2, &dwTextureState[1] );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAOP,   &dwTextureState[2] );

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );

	//바닥 평면 그리기
	if( m_CirclePlane[0] )
	{
		GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_DMITRON_CRYSTAL1 ] ));

		m_CirclePlane[0]->SetColor(1.0f, 1.0f, 1.0f, 1.0f);
		m_CirclePlane[0]->SetBillBoard( vViewVec, FALSE, 0.0f);
		m_CirclePlane[0]->RotateZ( (float)m_Rotation * 2 * g_PI * 0.001f );
		m_CirclePlane[0]->Scale(fScale1, fScale1, 10.0f);
		m_CirclePlane[0]->SetTranslationXYZ(vCirclePos.x, vCirclePos.y+30,  vCirclePos.z);
		m_CirclePlane[0]->Render();
	}

	if( m_CirclePlane[1] )
	{
		GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_DMITRON_CRYSTAL2 ] ));

		m_CirclePlane[1]->SetColor(1.0f, 1.0f, 1.0f, fScale2*0.05f);
		m_CirclePlane[1]->SetBillBoard( vViewVec, FALSE, 0.0f);
		m_CirclePlane[1]->RotateZ( (float)m_Rotation * 2 * g_PI * 0.006f );
		m_CirclePlane[1]->Scale(fScale2, fScale2, 10.0f);
		m_CirclePlane[1]->SetTranslationXYZ(vCirclePos.x, vCirclePos.y+30,  vCirclePos.z);
		m_CirclePlane[1]->Render();
	}

	if( m_CirclePlane[2] )
	{
		GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_DMITRON_CRYSTAL3 ] ));

		m_CirclePlane[2]->SetColor(1.0f, 1.0f, 1.0f, fScale3*0.01f);
		m_CirclePlane[2]->SetBillBoard( vViewVec, FALSE, 0.0f);
		m_CirclePlane[2]->RotateZ( (float)m_Rotation * 2 * g_PI * 0.001f );
		m_CirclePlane[2]->Scale(fScale3, fScale3, 10.0f);
		m_CirclePlane[2]->SetTranslationXYZ(vCirclePos.x, vCirclePos.y+30,  vCirclePos.z);
		m_CirclePlane[2]->Render();
	}

	vCirclePos = D3DVECTOR( effect_sort.vPos.x, effect_sort.vPos.y+2.0f, effect_sort.vPos.z);

	//상태 복원
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE);

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState[2] );

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_FOGENABLE, dwFogEnable );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ALPHATESTENABLE, alphaTest );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, alphaBlend );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND, srcBlendOld);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND, destBlendOld);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_AMBIENT, dwAmbient);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, dwLighting );
	return;
}

