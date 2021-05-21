#include "stdafx.h"
// PlaceInfo.cpp: implementation of the CPlaceInfo class.
//
//////////////////////////////////////////////////////////////////////


#include "D3DApp.h"
#include "D3DUtil.h"
#include "D3DMath.h"
#include "D3DTypes.h"

#include "IndexedTexture.h"
#include "PlaceInfo.h"
#include "main.h"
#include "DirtSnd.h"


extern float g_fTimeKey;
extern int	g_MatrixSvr;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPlaceInfo::CPlaceInfo()
{
	m_rectPlaceInfo = NULL;
	m_rectPlaceInfo = new D3DTLVERTEX[4];

	m_rectPlaceInfo[0] = D3DTLVERTEX( D3DVECTOR( 0.0f, 0.0f, 0.0f ), 1.0f, 0xffffffff, 0, 0.0f, 1.0f );
	m_rectPlaceInfo[1] = D3DTLVERTEX( D3DVECTOR( 0.0f, 0.0f, 0.0f ), 1.0f, 0xffffffff, 0, 0.0f, 0.0f );
	m_rectPlaceInfo[2] = D3DTLVERTEX( D3DVECTOR( 0.0f, 0.0f, 0.0f ), 1.0f, 0xffffffff, 0, 1.0f, 1.0f );
	m_rectPlaceInfo[3] = D3DTLVERTEX( D3DVECTOR( 0.0f, 0.0f, 0.0f ), 1.0f, 0xffffffff, 0, 1.0f, 0.0f );

	m_PlaceInfoTexId=0;
	m_CurFrame = 0;
	m_TexNum = 0;

	m_pIndexedTexture = NULL;
	m_pIndexedTexture = new IndexedTexture( 1 );
	if( m_pIndexedTexture )
		m_pIndexedTexture->SetTexturePath("interface/BattleStart/");

}

CPlaceInfo::~CPlaceInfo()
{
	DeleteRes();
	SAFE_DELETE(m_pIndexedTexture);
}
void CPlaceInfo::LoadRes( int texNum )
{
	char strTexName[100] = {0,};
	if( !GET_D3DDEVICE() )
		return;

	if( m_pIndexedTexture )
	{
		switch( g_MatrixSvr )
		{
		case 1:
			sprintf( strTexName, "MatPlace%d.tga", texNum);
			break;
		case 2:
			sprintf( strTexName, "2009MatPlace%d.tga", texNum);
			break;
		}

		m_PlaceInfoTexId = m_pIndexedTexture->CreateTextureFromFile( strTexName );
		m_pIndexedTexture->RestoreAllTextures( GET_D3DDEVICE() );
	}
}

void CPlaceInfo::DeleteRes()
{
	if( m_rectPlaceInfo )
	{
		delete m_rectPlaceInfo;
		m_rectPlaceInfo = NULL;
	}
	m_pIndexedTexture->InvalidateAllTextures();
}

void CPlaceInfo::Render()
{
	int i = 0;
	if( !GET_D3DDEVICE() )
		return;

	static float fSaveTime = 0.0f;
	int nAddFrame = 0;
	int drawTotalFrame = 100;

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
		m_CurFrame = 0;
		pCMyApp->DrawPlaceInfo = FALSE;
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
//=============================================================


	i = m_CurFrame/20;
	float alpha = 0;

	FLOAT flagY1 = 260.0f;
	FLOAT flagY2 = 200.0f;

	if( g_MatrixSvr == 2 ) // [2008/11/24 Theodoric] WORLD_MATRIX_2009 :: 위치 보정
	{
		flagY1 = 230.0f;
		flagY2 = 170.0f;
	}

	m_rectPlaceInfo[0].sx = (FLOAT)vp.dwWidth/2 - 109.0f ;
	m_rectPlaceInfo[1].sx = (FLOAT)vp.dwWidth/2 - 109.0f ;
	m_rectPlaceInfo[2].sx = (FLOAT)vp.dwWidth/2 + 108.0f ;
	m_rectPlaceInfo[3].sx = (FLOAT)vp.dwWidth/2 + 108.0f ;

	m_rectPlaceInfo[1].sy = (FLOAT)vp.dwHeight/2 - flagY1;
	m_rectPlaceInfo[3].sy = (FLOAT)vp.dwHeight/2 - flagY1;
	m_rectPlaceInfo[0].sy = (FLOAT)vp.dwHeight/2 - flagY2;
	m_rectPlaceInfo[2].sy = (FLOAT)vp.dwHeight/2 - flagY2;

	if(m_CurFrame < 20)
		alpha = (float)m_CurFrame/20;
	else
		alpha = (float)( 1.0f -((float)m_CurFrame-20)/80 );

	m_rectPlaceInfo[0].color = D3DRGBA(1.0f, 1.0f, 1.0f, alpha);
	m_rectPlaceInfo[1].color = D3DRGBA(1.0f, 1.0f, 1.0f, alpha);
	m_rectPlaceInfo[2].color = D3DRGBA(1.0f, 1.0f, 1.0f, alpha);
	m_rectPlaceInfo[3].color = D3DRGBA(1.0f, 1.0f, 1.0f, alpha);

	GET_D3DDEVICE()->SetTexture(0, m_pIndexedTexture->GetSurfaceByIdx(m_PlaceInfoTexId));

	GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_TLVERTEX, m_rectPlaceInfo, 4, 0 );


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

