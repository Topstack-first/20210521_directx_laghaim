#include "stdafx.h"

#include "D3DApp.h"
#include "D3DUtil.h"
#include "D3DMath.h"
#include "D3DTypes.h"
#include "IndexedTexture.h"
#include "HappyNewYear.h"
#include "main.h"


extern float g_fTimeKey;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHappyNewYear::CHappyNewYear()
{
	m_rect = NULL;
	m_rect = new D3DTLVERTEX[4];

	m_rect[0] = D3DTLVERTEX( D3DVECTOR( 0.0f, 0.0f, 0.0f ), 1.0f, 0xffffffff, 0, 0.0f, 1.0f );
	m_rect[1] = D3DTLVERTEX( D3DVECTOR( 0.0f, 0.0f, 0.0f ), 1.0f, 0xffffffff, 0, 0.0f, 0.0f );
	m_rect[2] = D3DTLVERTEX( D3DVECTOR( 0.0f, 0.0f, 0.0f ), 1.0f, 0xffffffff, 0, 1.0f, 1.0f );
	m_rect[3] = D3DTLVERTEX( D3DVECTOR( 0.0f, 0.0f, 0.0f ), 1.0f, 0xffffffff, 0, 1.0f, 0.0f );

	ZeroMemory(m_eventTexId, sizeof(m_eventTexId));

	m_pIndexedTexture = NULL;

	m_CurFrame = 0;
	m_TexNum = 0;

	m_pIndexedTexture = new IndexedTexture( 8 );
	if( m_pIndexedTexture )
		m_pIndexedTexture->SetTexturePath("interface/HappyNewYear/");

	m_bRender = true;
}

CHappyNewYear::~CHappyNewYear()
{
	DeleteRes();
}

void CHappyNewYear::LoadRes()
{
	int	i;
	char strTexName[100] = {0,};
	if( !GET_D3DDEVICE() )
		return;

	// [12/24/2007 반재승] 그림은 8장이니까 8개 할당 해주자.
	if( m_pIndexedTexture )
	{
		for( i = 0; i < 8; i++ )
		{
			sprintf( strTexName, "newyear%d.sek", i );
			m_eventTexId[i] = m_pIndexedTexture->CreateTextureFromFile( strTexName );
		}
		m_pIndexedTexture->RestoreAllTextures( GET_D3DDEVICE() );
	}
}

void CHappyNewYear::DeleteRes()
{
	if( m_rect )
	{
		delete[] m_rect;
		m_rect = NULL;
	}
	if( m_pIndexedTexture )
	{
		m_pIndexedTexture->InvalidateAllTextures();
		delete m_pIndexedTexture;
	}
}

void CHappyNewYear::Render()
{
	if( !GET_D3DDEVICE() )
		return;

	static float fSaveTime = 0.0f;
	int nAddFrame = 0;
	int drawTotalFrame = 200;

	fSaveTime += g_fTimeKey;

	while (fSaveTime > 0.05f)
	{
		nAddFrame++;
		fSaveTime -= 0.05f;
	}

	m_CurFrame += nAddFrame;
	m_TexNum += nAddFrame;

	if( m_CurFrame > drawTotalFrame )
	{
		m_bRender = false; // [12/24/2007 반재승] 이 시점에 메모리 해제하라고 알려주자
		m_CurFrame = 0;
		m_TexNum = 0;
		return;
	}

	if( m_TexNum >= 7 )
		m_TexNum = 0;

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

	m_rect[0].sx = (FLOAT)vp.dwWidth/2 - 160;
	m_rect[1].sx = (FLOAT)vp.dwWidth/2 - 160;
	m_rect[2].sx = (FLOAT)vp.dwWidth/2 + 160;
	m_rect[3].sx = (FLOAT)vp.dwWidth/2 + 160;

	m_rect[1].sy = (FLOAT)vp.dwHeight/2 - 120;
	m_rect[3].sy = (FLOAT)vp.dwHeight/2 - 120;
	m_rect[0].sy = (FLOAT)vp.dwHeight/2 + 120;
	m_rect[2].sy = (FLOAT)vp.dwHeight/2 + 120;

	float alpha = 1.0f;

	if( m_CurFrame < 50 )
	{
		alpha = ((float)m_CurFrame / 50);
	}
	else if( m_CurFrame > (drawTotalFrame-50) )
	{
		int curFrame = drawTotalFrame - m_CurFrame;
		alpha = ((float)curFrame / 50);
	}
	m_rect[0].color = D3DRGBA(1.0f, 1.0f, 1.0f, alpha);
	m_rect[1].color = D3DRGBA(1.0f, 1.0f, 1.0f, alpha);
	m_rect[2].color = D3DRGBA(1.0f, 1.0f, 1.0f, alpha);
	m_rect[3].color = D3DRGBA(1.0f, 1.0f, 1.0f, alpha);

	if( m_pIndexedTexture )
		GET_D3DDEVICE()->SetTexture(0, m_pIndexedTexture->GetSurfaceByIdx(m_eventTexId[m_TexNum]));

	GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_TLVERTEX,
									m_rect, 4, 0 );

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

