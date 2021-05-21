#include "stdafx.h"
#include "D3DApp.h"
#include "D3DUtil.h"
#include "D3DMath.h"
#include "D3DTypes.h"
#include "IndexedTexture.h"
#include "ImageDefine.h"
#include "main.h"
#include "DirtSnd.h"
#include "FxRegenBar.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFxRegenBar::CFxRegenBar()
{
	m_rectRegenBar = NULL;
	m_rectRegenBar = new D3DTLVERTEX[4];

	m_rectRegenBar[0] = D3DTLVERTEX( D3DVECTOR( 0.0f, 0.0f, 0.0f ), 1.0f, 0xffffffff, 0, 0.0f, 1.0f );
	m_rectRegenBar[1] = D3DTLVERTEX( D3DVECTOR( 0.0f, 0.0f, 0.0f ), 1.0f, 0xffffffff, 0, 0.0f, 0.0f );
	m_rectRegenBar[2] = D3DTLVERTEX( D3DVECTOR( 0.0f, 0.0f, 0.0f ), 1.0f, 0xffffffff, 0, 1.0f, 1.0f );
	m_rectRegenBar[3] = D3DTLVERTEX( D3DVECTOR( 0.0f, 0.0f, 0.0f ), 1.0f, 0xffffffff, 0, 1.0f, 0.0f );
}

CFxRegenBar::~CFxRegenBar()
{
	SAFE_DELETE( m_rectRegenBar );
}

void CFxRegenBar::Render(DWORD dwCurtime, int nRegenIndex)
{
	if( !GET_D3DDEVICE() )
		return;

	DWORD dwFogEnable, dwlingting, dwAlphaTest, dwAlphaBlend, dwRenState[2];
	DWORD dwTextureState[3];

	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_FOGENABLE, &dwFogEnable );
	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_LIGHTING, &dwlingting );
	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, &dwAlphaBlend);
	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_ALPHATESTENABLE, &dwAlphaTest);

	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_SRCBLEND, &dwRenState[0] );
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_DESTBLEND, &dwRenState[1] );

	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG1, &dwTextureState[0] );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG2, &dwTextureState[1] );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAOP,   &dwTextureState[2] );

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_FOGENABLE, FALSE );
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_LIGHTING, FALSE);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE   , TRUE);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHATESTENABLE   , TRUE);

	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA);

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );

	D3DVIEWPORT7 vp;
	GET_D3DDEVICE()->GetViewport( &vp );
//=============================================================
	float alpha = 1.0f;
	m_rectRegenBar[0].sx = (FLOAT)vp.dwWidth/2 - 120;
	m_rectRegenBar[1].sx = (FLOAT)vp.dwWidth/2 - 120;
	m_rectRegenBar[2].sx = (FLOAT)vp.dwWidth/2 + 130;
	m_rectRegenBar[3].sx = (FLOAT)vp.dwWidth/2 + 130;

	m_rectRegenBar[1].sy = (FLOAT)vp.dwHeight - 200;
	m_rectRegenBar[3].sy = (FLOAT)vp.dwHeight - 200;
	m_rectRegenBar[0].sy = (FLOAT)vp.dwHeight - 148;
	m_rectRegenBar[2].sy = (FLOAT)vp.dwHeight - 148;

	m_rectRegenBar[0].color = D3DRGBA(1.0f, 1.0f, 1.0f, alpha);
	m_rectRegenBar[1].color = D3DRGBA(1.0f, 1.0f, 1.0f, alpha);
	m_rectRegenBar[2].color = D3DRGBA(1.0f, 1.0f, 1.0f, alpha);
	m_rectRegenBar[3].color = D3DRGBA(1.0f, 1.0f, 1.0f, alpha);

	GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_REGENBAR ] ) );
	GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_TLVERTEX, m_rectRegenBar, 4, 0 );

	m_rectRegenBar[0].sx = (FLOAT)vp.dwWidth/2 - 82;
	m_rectRegenBar[1].sx = (FLOAT)vp.dwWidth/2 - 82;
	m_rectRegenBar[2].sx = (FLOAT)vp.dwWidth/2 - 108 + ( dwCurtime * 2 ) / ( nRegenIndex * 10 );
	m_rectRegenBar[3].sx = (FLOAT)vp.dwWidth/2 - 108 + ( dwCurtime * 2 ) / ( nRegenIndex * 10 );

	m_rectRegenBar[1].sy = (FLOAT)vp.dwHeight - 160;
	m_rectRegenBar[3].sy = (FLOAT)vp.dwHeight - 160;
	m_rectRegenBar[0].sy = (FLOAT)vp.dwHeight - 153;
	m_rectRegenBar[2].sy = (FLOAT)vp.dwHeight - 153;

	m_rectRegenBar[0].color = D3DRGBA(1.0f, 1.0f, 1.0f, alpha);
	m_rectRegenBar[1].color = D3DRGBA(1.0f, 1.0f, 1.0f, alpha);
	m_rectRegenBar[2].color = D3DRGBA(1.0f, 1.0f, 1.0f, alpha);
	m_rectRegenBar[3].color = D3DRGBA(1.0f, 1.0f, 1.0f, alpha);

	GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_REGENBAR2 ] ) );
	GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_TLVERTEX, m_rectRegenBar, 4, 0 );
//=============================================================
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState[2] );

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND, dwRenState[0] );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND, dwRenState[1] );

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, dwlingting );
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, dwAlphaBlend);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHATESTENABLE, dwAlphaTest);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_FOGENABLE, dwFogEnable );
}

void CFxRegenBar::RenderAnother( int Type ) // 캐릭터에 귀속되어있는 CFxRegenBar 객체를 이용해서 화면에 그려지는 여러가지 이펙트를 그려준다.(꼭지점 댄스시의 스페이스바 푸쉬 이펙트를 그려준다.)
{
	if( !GET_D3DDEVICE() )
		return;

	DWORD dwFogEnable, dwlingting, dwAlphaTest, dwAlphaBlend, dwRenState[2];
	DWORD dwTextureState[3];

	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_FOGENABLE, &dwFogEnable );
	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_LIGHTING, &dwlingting );
	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, &dwAlphaBlend);
	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_ALPHATESTENABLE, &dwAlphaTest);

	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_SRCBLEND, &dwRenState[0] );
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_DESTBLEND, &dwRenState[1] );

	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG1, &dwTextureState[0] );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG2, &dwTextureState[1] );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAOP,   &dwTextureState[2] );

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_FOGENABLE, FALSE );
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_LIGHTING, FALSE);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE   , TRUE);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHATESTENABLE   , TRUE);

	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA);

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );

	D3DVIEWPORT7 vp;
	GET_D3DDEVICE()->GetViewport( &vp );

	switch( Type )
	{
	case POINT_DANCE_SPACE: // 꼭지점 댄스 스페이스바 표시.

		m_rectRegenBar[0].sx = (FLOAT)vp.dwWidth/2 - 50;
		m_rectRegenBar[1].sx = (FLOAT)vp.dwWidth/2 - 50;
		m_rectRegenBar[2].sx = (FLOAT)vp.dwWidth/2 + 50;
		m_rectRegenBar[3].sx = (FLOAT)vp.dwWidth/2 + 50;

		m_rectRegenBar[1].sy = (FLOAT)vp.dwHeight/2 - 130;
		m_rectRegenBar[3].sy = (FLOAT)vp.dwHeight/2 - 130;
		m_rectRegenBar[1].tv = 0.01f; // 위쪽에 검은줄이 생기는 현상 방지.
		m_rectRegenBar[3].tv = 0.01f;
		m_rectRegenBar[0].sy = (FLOAT)vp.dwHeight/2 - 50;
		m_rectRegenBar[2].sy = (FLOAT)vp.dwHeight/2 - 50;

		m_rectRegenBar[0].color = D3DRGBA(1.0f, 1.0f, 1.0f, 1.0f);
		m_rectRegenBar[1].color = D3DRGBA(1.0f, 1.0f, 1.0f, 1.0f);
		m_rectRegenBar[2].color = D3DRGBA(1.0f, 1.0f, 1.0f, 1.0f);
		m_rectRegenBar[3].color = D3DRGBA(1.0f, 1.0f, 1.0f, 1.0f);

		if( (timeGetTime()%300) > 150 ) // 번갈아 가며 출력되도록.
			GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_PUSHSPACE_UP ] ) );
		else
			GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_PUSHSPACE_DOWN ] ) );

		GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_TLVERTEX, m_rectRegenBar, 4, 0 );

		break;
	}
	/*
	//=============================================================
		float alpha = 1.0f;
		m_rectRegenBar[0].sx = (FLOAT)vp.dwWidth/2 - 120;
		m_rectRegenBar[1].sx = (FLOAT)vp.dwWidth/2 - 120;
		m_rectRegenBar[2].sx = (FLOAT)vp.dwWidth/2 + 130;
		m_rectRegenBar[3].sx = (FLOAT)vp.dwWidth/2 + 130;

		m_rectRegenBar[1].sy = (FLOAT)vp.dwHeight - 200;
		m_rectRegenBar[3].sy = (FLOAT)vp.dwHeight - 200;
		m_rectRegenBar[0].sy = (FLOAT)vp.dwHeight - 148;
		m_rectRegenBar[2].sy = (FLOAT)vp.dwHeight - 148;

		m_rectRegenBar[0].color = D3DRGBA(1.0f, 1.0f, 1.0f, alpha);
		m_rectRegenBar[1].color = D3DRGBA(1.0f, 1.0f, 1.0f, alpha);
		m_rectRegenBar[2].color = D3DRGBA(1.0f, 1.0f, 1.0f, alpha);
		m_rectRegenBar[3].color = D3DRGBA(1.0f, 1.0f, 1.0f, alpha);

		GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_REGENBAR ] ) );
		GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_TLVERTEX, m_rectRegenBar, 4, 0 );

		m_rectRegenBar[0].sx = (FLOAT)vp.dwWidth/2 - 82;
		m_rectRegenBar[1].sx = (FLOAT)vp.dwWidth/2 - 82;
		m_rectRegenBar[2].sx = (FLOAT)vp.dwWidth/2 - 108 + ( dwCurtime * 2 ) / ( nRegenIndex * 10 );
		m_rectRegenBar[3].sx = (FLOAT)vp.dwWidth/2 - 108 + ( dwCurtime * 2 ) / ( nRegenIndex * 10 );

		m_rectRegenBar[1].sy = (FLOAT)vp.dwHeight - 160;
		m_rectRegenBar[3].sy = (FLOAT)vp.dwHeight - 160;
		m_rectRegenBar[0].sy = (FLOAT)vp.dwHeight - 153;
		m_rectRegenBar[2].sy = (FLOAT)vp.dwHeight - 153;

		m_rectRegenBar[0].color = D3DRGBA(1.0f, 1.0f, 1.0f, alpha);
		m_rectRegenBar[1].color = D3DRGBA(1.0f, 1.0f, 1.0f, alpha);
		m_rectRegenBar[2].color = D3DRGBA(1.0f, 1.0f, 1.0f, alpha);
		m_rectRegenBar[3].color = D3DRGBA(1.0f, 1.0f, 1.0f, alpha);

		GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_REGENBAR2 ] ) );
		GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_TLVERTEX, m_rectRegenBar, 4, 0 );
	*/
//=============================================================
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState[2] );

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND, dwRenState[0] );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND, dwRenState[1] );

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, dwlingting );
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, dwAlphaBlend);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHATESTENABLE, dwAlphaTest);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_FOGENABLE, dwFogEnable );
}
