#include "stdafx.h"
#include "main.h"
#include "NkCharacter.h"
#include "FxHellDown_Lightly.h"
#include "ImageDefine.h"



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFxHellDown_Lightly::CFxHellDown_Lightly()
{
	m_pLightly			= NULL;
	m_fScale			= 0.0f;
	m_fFade				= 1.0f;
	m_nTotalFrame		= FX_HELLDOWN_FRAMECOUNT;

	Create();
}

CFxHellDown_Lightly::~CFxHellDown_Lightly()
{
	DeleteRes();
}

void CFxHellDown_Lightly::Create()
{
	DeleteRes();

	m_pLightly = new CRectPlane;
	if( m_pLightly )
		m_pLightly->Create( 0.35f, 30.0f, 5.5f, TRUE );
}


void CFxHellDown_Lightly::LoadRes(IndexedTexture *pIndexedTexture)
{
}

void CFxHellDown_Lightly::DeleteRes()
{
	SAFE_DELETE(m_pLightly);
}

void CFxHellDown_Lightly::Render(EffectSort &effect_sort)
{
	if( !GET_D3DDEVICE() || !effect_sort.pNkChaFrom)
		return;

	float maxScale = 2.0f;

	m_fScale = (maxScale * effect_sort.nCurFrame) / m_nTotalFrame;
	m_fFade = 1.0f - (m_fScale * 0.45f);
	if( effect_sort.nCurFrame > m_nTotalFrame )
		m_fScale = 2.0f;

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

	D3DVECTOR	vDrawPos;
	if (effect_sort.pNkChaFrom)
		vDrawPos = D3DVECTOR(effect_sort.pNkChaFrom->m_wx, effect_sort.pNkChaFrom->m_wy, effect_sort.pNkChaFrom->m_wz);

	float red, green, blue;
	red = green = blue = 0.0f;

	red = 0.38f - (0.38f * m_fScale * 0.15f);
	green = 0.38f - (0.38f * m_fScale * 0.15f);
	blue = 1.0f - (1.0f * m_fScale * 0.15f);

	// nate 2004 - 7 - image
	GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_LEVELUP_LIGHT ] ) );

	if( m_pLightly )
	{
		for( int i = 0; i < 30; i++ )
		{
			m_pLightly->SetColor(red, green, blue, m_fFade);
			m_pLightly->SetRotationY(g_PI * RandomNum(-1.0f, 1.0f));
			m_pLightly->RotateX(g_PI * RandomNum(-1.0f, 1.0f));
			m_pLightly->Scale(1.5f, 0.0f, RandomNum(-1.5f, 1.75f));
			m_pLightly->SetTranslationXYZ(vDrawPos.x, vDrawPos.y + 12.0f, vDrawPos.z);
			m_pLightly->Render();
		}
	}

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE);

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState[2] );
}

