#include "stdafx.h"
#include "main.h"
#include "NkCharacter.h"
#include "IndexedTexture.h"
#include "FxRegenStart.h"
#include "ImageDefine.h"
#include "DirtSnd.h"



extern int g_EffectSound[];
extern float g_fTimeKey;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFxRegenStart::CFxRegenStart()
{
	m_rectRegen = NULL;
	m_rectRegen = new D3DTLVERTEX[4];

	m_rectRegen[0] = D3DTLVERTEX( D3DVECTOR( 0.0f, 0.0f, 0.0f ), 1.0f, 0xffffffff, 0, 0.0f, 1.0f );
	m_rectRegen[1] = D3DTLVERTEX( D3DVECTOR( 0.0f, 0.0f, 0.0f ), 1.0f, 0xffffffff, 0, 0.0f, 0.0f );
	m_rectRegen[2] = D3DTLVERTEX( D3DVECTOR( 0.0f, 0.0f, 0.0f ), 1.0f, 0xffffffff, 0, 1.0f, 1.0f );
	m_rectRegen[3] = D3DTLVERTEX( D3DVECTOR( 0.0f, 0.0f, 0.0f ), 1.0f, 0xffffffff, 0, 1.0f, 0.0f );

	m_CurFrame = 0;
}

CFxRegenStart::~CFxRegenStart()
{
	DeleteRes();
}

void CFxRegenStart::DeleteRes()
{
	SAFE_DELETE( m_rectRegen );
}

void CFxRegenStart::LoadRes()
{
}

void CFxRegenStart::Render()
{
	if( !GET_D3DDEVICE() )
		return;

	int i = 0;
	static float fSaveTime2 = 0.0f;
	int nAddFrame = 0;
	int drawTotalFrame = 35;

	fSaveTime2 += g_fTimeKey;

	while (fSaveTime2 > 0.05f)
	{
		nAddFrame++;
		fSaveTime2 -= 0.05f;
	}

	m_CurFrame += nAddFrame;

	if( m_CurFrame > drawTotalFrame )
	{
		m_CurFrame = 0;
		pCMyApp->m_bRegenStart = FALSE;
		return;
	}

//=============================================================
	DWORD dwlingting, dwAlphaTest, dwAlphaBlend, dwRenState[2];
	DWORD dwTextureState[3];

	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_LIGHTING, &dwlingting );
	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, &dwAlphaBlend);
	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_ALPHATESTENABLE, &dwAlphaTest);

	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_SRCBLEND, &dwRenState[0] );
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_DESTBLEND, &dwRenState[1] );

	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG1, &dwTextureState[0] );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG2, &dwTextureState[1] );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAOP,   &dwTextureState[2] );

	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_LIGHTING, FALSE);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE   , TRUE);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHATESTENABLE   , FALSE);

	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA);

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );

	D3DVIEWPORT7 vp;
	GET_D3DDEVICE()->GetViewport( &vp );

	//battle start

	if(m_CurFrame>8)
	{
		if(m_CurFrame<12)
		{
			m_rectRegen[0].sx = (FLOAT)vp.dwWidth/2 + 100 - (m_CurFrame-5)*60;
			m_rectRegen[1].sx = (FLOAT)vp.dwWidth/2 + 100 - (m_CurFrame-5)*60;
			m_rectRegen[2].sx = (FLOAT)vp.dwWidth/2 + 500 - (m_CurFrame-5)*60;
			m_rectRegen[3].sx = (FLOAT)vp.dwWidth/2 + 500 - (m_CurFrame-5)*60;
		}
		else
		{
			m_rectRegen[0].sx = (FLOAT)vp.dwWidth/2  + 100 - 300;
			m_rectRegen[1].sx = (FLOAT)vp.dwWidth/2  + 100 - 300;
			m_rectRegen[2].sx = (FLOAT)vp.dwWidth/2  + 500 - 300;
			m_rectRegen[3].sx = (FLOAT)vp.dwWidth/2  + 500 - 300;
		}

		m_rectRegen[1].sy = (FLOAT)vp.dwHeight/2 + 0;
		m_rectRegen[3].sy = (FLOAT)vp.dwHeight/2 + 0;
		m_rectRegen[0].sy = (FLOAT)vp.dwHeight/2 + 150;
		m_rectRegen[2].sy = (FLOAT)vp.dwHeight/2 + 150;

		float alpha = 1.0f;

		if( m_CurFrame < 20 )
			alpha = 1.0f;
		else
		{
			int curFrame = 35 - m_CurFrame;
			alpha = ((float)curFrame / 15);
		}

		m_rectRegen[0].color = D3DRGBA(1.0f, 1.0f, 1.0f, alpha);
		m_rectRegen[1].color = D3DRGBA(1.0f, 1.0f, 1.0f, alpha);
		m_rectRegen[2].color = D3DRGBA(1.0f, 1.0f, 1.0f, alpha);
		m_rectRegen[3].color = D3DRGBA(1.0f, 1.0f, 1.0f, alpha);

		GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_REGENSTART ] ) );

		GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_TLVERTEX,
										m_rectRegen, 4, 0 );
	}
//=============================================================
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState[2] );

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND, dwRenState[0] );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND, dwRenState[1] );

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, dwlingting );
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, dwAlphaBlend);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHATESTENABLE, dwAlphaTest);

	return;
}
