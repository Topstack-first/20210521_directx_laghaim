#include "stdafx.h"
///자유대련
#include "main.h"
#include "ImageDefine.h"
#include "NkCharacter.h"
#include "FxFreePvpRing.h"


extern float g_fTimeKey;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFxFreePvpRing::CFxFreePvpRing()
{
	m_nCurrFrame		= 0;
	m_fSaveTime			= 0;
	m_fScale			= 0.0f;

	m_CylinderId[0]		= 0;
	m_CylinderId[1]		= 0;

	m_CylinderPlane[0]	= NULL;
	m_CylinderPlane[1]	= NULL;

	m_CircleId[0]		= 0;
	m_CircleId[1]		= 0;
	m_CircleId[2]		= 0;

	m_CirclePlane[0]	= 0;
	m_CirclePlane[1]	= 0;
	m_CirclePlane[2]	= 0;

}

CFxFreePvpRing::~CFxFreePvpRing()
{
	DeleteRes();
}

void CFxFreePvpRing::LoadRes()
{
	//실린더 메쉬 생성
	m_CylinderPlane[0] = new CCylinderPlane;
	if( m_CylinderPlane[0] )
		m_CylinderPlane[0]->Create( 1.0f, 1.0f, 40.0f, 16, 0.0f, TRUE, 16.0f );

	m_CylinderPlane[1] = new CCylinderPlane;
	if( m_CylinderPlane[1] )
		m_CylinderPlane[1]->Create( 0.99f, 0.99f, 40.0f, 32, 0.0f, TRUE, 32.0f );

	//평면 메쉬 생성
	m_CirclePlane[0] = new CRectPlane;
	if( m_CirclePlane[0] )
		m_CirclePlane[0]->Create( 2.0f, 2.0f, FALSE );

	m_CirclePlane[1] = new CRectPlane;
	if( m_CirclePlane[1] )
		m_CirclePlane[1]->Create( 2.0f, 2.0f, FALSE );
}

void CFxFreePvpRing::DeleteRes()
{
	SAFE_DELETE(m_CylinderPlane[0]);
	SAFE_DELETE(m_CylinderPlane[1]);

	SAFE_DELETE(m_CirclePlane[0]);
	SAFE_DELETE(m_CirclePlane[1]);
}

void CFxFreePvpRing::Render(EffectSort &effect_sort)
{
	if( !GET_D3DDEVICE() )
		return;

	//변수 설정
	DWORD alphaTest, alphaBlend, srcBlendOld, destBlendOld, dwLighting, dwAmbient, dwFogEnable;
	DWORD ElapsedTick = timeGetTime() - effect_sort.dwColor;
//	m_fScale = (maxScale * effect_sort.nCurFrame) / effect_sort.subNum; // effect_sort.subNum을 토탈 프레임으로 사용.

	float maxScale = 0.08f;
	if( effect_sort.nCurFrame < effect_sort.subNum/2 )
		m_fScale = maxScale * effect_sort.nCurFrame; // effect_sort.subNum을 토탈 프레임으로 사용.
	else
		m_fScale = maxScale*(effect_sort.subNum/2) - (maxScale * (effect_sort.nCurFrame-(effect_sort.subNum/2))); // effect_sort.subNum을 토탈 프레임으로 사용.

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

	float red, green, blue;//, fade;
	red = green = blue = 0.0f;

	float fScale = (float)((ElapsedTick*ElapsedTick))*0.0002f;
	if (fScale > 150.0f) fScale = 150.0f;

	//외곽 실제 렌더링
	if( m_CylinderPlane[0] )
	{
		m_CylinderPlane[0]->SetColor(1.0f, 1.0f, 1.0f, 0.8f);
		m_CylinderPlane[0]->SetRotationY( g_PI );
		m_CylinderPlane[0]->RotateY( ElapsedTick * g_PI*0.00006f );

		if( ElapsedTick < 1000 ) // 배틀 시작 카운트 중
		{
			m_CylinderPlane[0]->Scale( fScale, 0.7f, fScale );
		}
		else
		{
			m_CylinderPlane[0]->Scale( 150.0f, 0.7f, 150.0f );
		}
		m_CylinderPlane[0]->SetTranslationXYZ(effect_sort.vPos.x,
											  effect_sort.vPos.y,//+(m_fScale*10.0f),
											  effect_sort.vPos.z);

		GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_PVP_RING_SIDE0 ] ) );
		m_CylinderPlane[0]->Render();
	}

	if( m_CylinderPlane[1] )
	{
		m_CylinderPlane[1]->SetColor(1.0f, 1.0f, 1.0f, 0.8f);
		m_CylinderPlane[1]->SetRotationY( g_PI );
		m_CylinderPlane[1]->RotateY( ElapsedTick * g_PI*0.00004f );

		if( ElapsedTick < 1000 ) // 배틀 시작 카운트 중
		{
			m_CylinderPlane[1]->Scale( fScale, 0.7f, fScale );
		}
		else
		{
			m_CylinderPlane[1]->Scale( 150.0f, 0.7f, 150.0f );
		}
		m_CylinderPlane[1]->SetTranslationXYZ(effect_sort.vPos.x,
											  effect_sort.vPos.y,//+(m_fScale*10.0f),
											  effect_sort.vPos.z);

		GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_PVP_RING_SIDE1 ] ));
		m_CylinderPlane[1]->Render();
	}

	D3DVECTOR vViewVec;
	vViewVec = pCMyApp->GetEyeDirection();

	D3DVECTOR vCirclePos;
	vCirclePos = D3DVECTOR( effect_sort.vPos.x, effect_sort.vPos.y+1.0f, effect_sort.vPos.z);

	//바닥 평면 그리기
	if( m_CirclePlane[0] )
	{
		m_CirclePlane[0]->SetColor(1.0f, 1.0f, 1.0f, 0.40f);
		m_CirclePlane[0]->SetRotationX( g_PI * 0.5f );
		m_CirclePlane[0]->RotateZ( ElapsedTick * g_PI*0.00006f );
		m_CirclePlane[0]->Scale(fScale, fScale, 10.0f);

		m_CirclePlane[0]->SetTranslationXYZ(vCirclePos.x, vCirclePos.y,  vCirclePos.z);
		GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_PVP_RING_BOTTOM0 ] ));
		m_CirclePlane[0]->Render();
	}

	vCirclePos = D3DVECTOR( effect_sort.vPos.x, effect_sort.vPos.y+2.0f, effect_sort.vPos.z);

	if( m_CirclePlane[1] )
	{
		m_CirclePlane[1]->SetColor(1.0f, 1.0f, 1.0f, 0.40f);
		m_CirclePlane[1]->SetRotationX( g_PI * 0.5f );
		m_CirclePlane[1]->RotateZ( -1.0 * ElapsedTick * g_PI*0.00006f );
		m_CirclePlane[1]->Scale(fScale, fScale, 10.0f);

		m_CirclePlane[1]->SetTranslationXYZ(vCirclePos.x, vCirclePos.y, vCirclePos.z);
		GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_PVP_RING_BOTTOM1 ] ));
		m_CirclePlane[1]->Render();
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

	return;
}

