#include "stdafx.h"
#include "main.h"
#include "NkCharacter.h"
#include "Land.h"
#include "skill.h"
// nate 2004 - 7 - image
#include "ImageDefine.h"
#include "FxStoneBrust.h"
#include "NkMob.h"



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFxStoneBrust::CFxStoneBrust()
{
	m_Rect_center	= NULL;
	m_Cyli_outside	= NULL;

	m_Rect_center = new CRectPlane;
	m_Cyli_outside = new CCylinderPlane;

	if( m_Rect_center )
		m_Rect_center->Create( 10.0f, 10.0f, FALSE );
	if( m_Cyli_outside )
		m_Cyli_outside->Create( 7.0f, 5.0f, 1.0f, 20, 0.0f, TRUE, 1.0f );

	m_fScale = 0.0f;
	m_fScale2 = 0.0f;
}

CFxStoneBrust::~CFxStoneBrust()
{
	SAFE_DELETE(m_Rect_center);
	SAFE_DELETE(m_Cyli_outside);
}

BOOL CFxStoneBrust::Render(EffectSort &effect_sort)
{
	if( !GET_D3DDEVICE() )
		return true;

	if( !effect_sort.pMobFrom )
		return TRUE;

	//-- Vector
	D3DVECTOR		vViewVec;
	D3DVECTOR		vDrawPos;

	if( effect_sort.pMobFrom )
		vDrawPos = D3DVECTOR( effect_sort.pMobFrom->m_wx, effect_sort.pMobFrom->m_wy + 4.0f, effect_sort.pMobFrom->m_wz  );

	// Set any appropiate state
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND,   D3DBLEND_SRCALPHA );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND,  D3DBLEND_ONE);

	DWORD  dwTextureState[3];

	//-- Store State
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG1, &dwTextureState[0] );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG2, &dwTextureState[1] );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAOP,   &dwTextureState[2] );

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );

	vViewVec = pCMyApp->GetEyeDirection();

	//스톤 가운데서 원모양의 빛나는 효과 .
	m_fScale = 2.0f * effect_sort.nCurFrame / 20;

	if( m_Rect_center )
	{
		//EFF_FX_AUTOLASER_LIGHT: 기존의 택스쳐 사용.
		GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_AUTOLASER_LIGHT ] ) );
		m_Rect_center->SetBillBoard(vViewVec, FALSE, 0.0f);
		m_Rect_center->Scale(12.0f * m_fScale, 12.0f * m_fScale, 12.0f * m_fScale );
		m_Rect_center->SetColor(1.0f, 1.0f, 1.0f, 1.0f);
		m_Rect_center->SetTranslationXYZ(vDrawPos.x, vDrawPos.y + 6.0f, vDrawPos.z);
		m_Rect_center->Render();
	}

	//스톤 중심에서 바깥으로 퍼져나가는 효과.
	//2번 퍼져나가는 효과를 주었다.
	if( effect_sort.nCurFrame <= 8 )
		m_fScale2 = 2.0f * effect_sort.nCurFrame / 20;
	else
		m_fScale2 = 3.0f * ( effect_sort.nCurFrame - 9 ) / 20;

	if( m_Cyli_outside )
	{
		//EFF_FX_DSTONEBRUST: 원래는 배쉬 스킬에 있는 텍스쳐를 따로 불러서 사용.
		GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_DSTONEBRUST ] ) );
		m_Cyli_outside->SetRotationY( g_PI );
		m_Cyli_outside->Scale(12.0f * m_fScale2, 12.0f * m_fScale2, 12.0f * m_fScale2 );
		m_Cyli_outside->SetColor(1.0f, 1.0f, 1.0f, 1.0f);
		m_Cyli_outside->SetTranslationXYZ(vDrawPos.x, vDrawPos.y, vDrawPos.z);
		m_Cyli_outside->Render();
	}

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState[2] );
	return false;
}
