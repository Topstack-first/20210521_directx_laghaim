#include "stdafx.h"
#include "sfx.h"
#include "main.h"
#include "NkCharacter.h"
#include "FxReflection.h"
#include "Land.h"
// nate 2004 - 7 - image
#include "ImageDefine.h"



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFxReflection::CFxReflection()
{
	m_rect_Shiled			= NULL;
	m_cyli_Shiled			= NULL;
	m_fScale				= 0.0f;
	m_fFade					= 0.0f;
	fMinScale				= 1.6f; //보호막의 최소 크기
	fMaxScale				= 2.05f;//보호막의 최대 크기
	fFlag					= 0.03f;
	nSpeed					= 150;  //보호막 회전 스피드
	m_nTotalFrame			= FX_REFLECTION_FRAMECOUNT;
	Create();
}

CFxReflection::~CFxReflection()
{
	SAFE_DELETE(m_rect_Shiled);
	SAFE_DELETE(m_cyli_Shiled);
}

void CFxReflection::Create()
{
	SAFE_DELETE(m_rect_Shiled);
	SAFE_DELETE(m_cyli_Shiled);

	m_rect_Shiled = new CRectPlane;
	if( m_rect_Shiled )
		m_rect_Shiled->Create( 17.0f, 17.0f, FALSE );

	m_cyli_Shiled = new CCylinderPlane;
	if( m_cyli_Shiled )
		m_cyli_Shiled->Create( 7.0f, 8.0f, 0.5f, 20.0f, TRUE );
}

void CFxReflection::LoadRes(IndexedTexture *pIndexedTexture)
{}
void CFxReflection::DeleteRes()
{}

BOOL CFxReflection::Render(EffectSort &effect_sort)
{
	if( !GET_D3DDEVICE() )
		return true;

	//-- 사용 캐릭이 사라질 경우
	if(!effect_sort.pNkChaFrom)
		return TRUE;
	if( !effect_sort.pNkChaFrom->m_ShowReflection )
		return TRUE;

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

	D3DVECTOR	vDrawVec;
	if( effect_sort.pNkChaFrom )
	{
		vDrawVec = D3DVECTOR(effect_sort.pNkChaFrom->m_wx,
							 effect_sort.pNkChaFrom->m_wy,
							 effect_sort.pNkChaFrom->m_wz);
	}

	float red, green, blue;

	m_fScale = effect_sort.nCurFrame % 30;
	GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_REFLECTION ] ) );

	//프레임에 따라 밝고 어둡게 하기 위해서 색깔을 바꿔주었다.
	if(m_fScale < 15)
		red = green = blue = 1.0f - (m_fScale * 0.06f);
	else
		red = green = blue = 0.1f + ((m_fScale-15) * 0.06f);

	m_rect_Shiled->SetColor(red, green, blue, 1.0f);
	m_rect_Shiled->SetBillBoard( vViewVec, FALSE, 0.0f );

	//프레임에 따라 스케일이 어느 정도 이상 커지면 크기를 점차 줄인다.
	if(m_fScale < 15)
	{
		m_rect_Shiled->Scale( fMinScale + fFlag * m_fScale,
							  fMinScale + fFlag * m_fScale, fMinScale + fFlag * m_fScale );
	}
	else
	{
		m_rect_Shiled->Scale( fMaxScale - fFlag * (m_fScale-15),
							  fMaxScale - fFlag * (m_fScale- 15), fMaxScale - fFlag * (m_fScale-15) );
	}

	//보호막을 천천히 돌리기 위해  % 150 로 설정.(fSpeed가 클 수록 속도 감소)
	m_fScale = effect_sort.nCurFrame % nSpeed;

	if( m_rect_Shiled )
	{
		m_rect_Shiled->SetTranslationXYZ(vDrawVec.x, vDrawVec.y + 13.5f, vDrawVec.z);
		m_rect_Shiled->RotateZ( (2*g_PI) * ( m_fScale / ( nSpeed - 1) ) );
		m_rect_Shiled->Render();
	}

	//요기서부터 케릭 주위를 돌아가는 3마리 지렁이 효과.
	red = green = blue = 0.8f;

	m_fScale = (3.0f * effect_sort.nCurFrame) / m_nTotalFrame;
	m_fFade = 1.0f - (m_fScale * 0.3f);

	if( m_cyli_Shiled )
	{
		GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_REFLECTION2 ] ) );

		m_cyli_Shiled->SetColor(red, green, blue, m_fFade);
		m_cyli_Shiled->SetRotationY(g_PI);
		m_cyli_Shiled->RotateZ( g_PI );
		m_cyli_Shiled->RotateY( -m_fScale * 80.0f );
		m_cyli_Shiled->Scale( 1.3f, 1.0f, 1.3f );
		m_cyli_Shiled->SetTranslationXYZ(vDrawVec.x, vDrawVec.y + 13.0f, vDrawVec.z);
		m_cyli_Shiled->Render();

		m_cyli_Shiled->SetColor(red, green, blue, m_fFade);
		m_cyli_Shiled->SetRotationY(g_PI);
		m_cyli_Shiled->RotateZ( g_PI * 0.75f );
		m_cyli_Shiled->RotateY( m_fScale * 90.0f );
		m_cyli_Shiled->Scale( 1.2f, 1.0f, 1.2f );
		m_cyli_Shiled->SetTranslationXYZ(vDrawVec.x, vDrawVec.y+ 13.0f, vDrawVec.z);
		m_cyli_Shiled->Render();

		m_cyli_Shiled->SetColor(red, green, blue, m_fFade);
		m_cyli_Shiled->SetRotationY(g_PI);
		m_cyli_Shiled->RotateZ( g_PI * 0.5f );
		m_cyli_Shiled->RotateY( m_fScale * 100.0f );
		m_cyli_Shiled->Scale( 1.1f, 1.0f, 1.1f );
		m_cyli_Shiled->SetTranslationXYZ(vDrawVec.x, vDrawVec.y+ 13.0f, vDrawVec.z);
		m_cyli_Shiled->Render();

		m_cyli_Shiled->SetColor(red, green, blue, m_fFade);
		m_cyli_Shiled->SetRotationY(g_PI);
		m_cyli_Shiled->RotateZ( g_PI * 0.25f );
		m_cyli_Shiled->RotateY( -m_fScale * 80.0f );
		m_cyli_Shiled->Scale( 1.25f, 1.0f, 1.25f );
		m_cyli_Shiled->SetTranslationXYZ(vDrawVec.x, vDrawVec.y+ 13.0f, vDrawVec.z);
		m_cyli_Shiled->Render();
	}

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState[2] );

	return FALSE;
}
