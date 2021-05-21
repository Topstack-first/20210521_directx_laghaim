#include "stdafx.h"

#include "D3DApp.h"
#include "D3DUtil.h"
#include "D3DMath.h"
#include "D3DTypes.h"
#include "IndexedTexture.h"
#include "DirtSnd.h"
#include "main.h"
#include "ControlGuildBattle.h"
#include "Nk2DFrame.h"
#include "TextOutBox.h"

#define BACKIMG_W		470
#define BACKIMG_H		44
#define NUMBERIC_W		21
#define NUMBERIC_H		28

#define IMGREADY_W		353/2
#define IMGREADY_H		155/2
#define IMGNUMBERRIC_W	120/2
#define IMGNUMBERRIC_H	120/2
#define IMGSTART_W		353/2
#define IMGSTART_H		155/2







CGuildRankWarBattleCount::CGuildRankWarBattleCount()
	: m_befortime(0)
	, m_nCount(0)
	, m_nFrameCount(0)
{
	m_CountState = IT_NONE;
	m_bActive = false;

	m_scale = 1.0f;
	m_alpha = 1.0f;

	m_Idx[0] = 0;
	m_Idx[1] = 2;
	m_Idx[2] = 1;
	m_Idx[3] = 1;
	m_Idx[4] = 2;
	m_Idx[5] = 3;

	m_pVertex = NULL;

	int i = 0;
	for(i=0; i< 12; i++)
		m_tID_Numberic[i] = -1;

	m_pIDTexture = NULL;
	m_pIDTexture = new IndexedTexture( 12 );

	if( m_pIDTexture )
		m_pIDTexture->SetTexturePath("interface/guild/guild_battle/");
}

CGuildRankWarBattleCount::~CGuildRankWarBattleCount()
{
	DeleteRes();
}

void CGuildRankWarBattleCount::DeleteRes()
{
	SAFE_DELETE(m_pVertex);
	SAFE_DELETE(m_pIDTexture);
}

void CGuildRankWarBattleCount::LoadResource()
{
// 	if( !m_pIDTexture )
// 	{
// 		m_pIDTexture = new IndexedTexture( 12 );
// 		m_pIDTexture->SetTexturePath("interface/guild/guild_battle/");
// 	}

	m_pVertex = new D3DTLVERTEX[4];

	m_pVertex[0] = D3DTLVERTEX( D3DVECTOR( 0.0f, 0.0f, 0.0f ), 1.0f, 0xffffffff, 0, 0.0f, 0.0f );
	m_pVertex[1] = D3DTLVERTEX( D3DVECTOR( 0.0f, 0.0f, 0.0f ), 1.0f, 0xffffffff, 0, 1.0f, 0.0f );
	m_pVertex[2] = D3DTLVERTEX( D3DVECTOR( 0.0f, 0.0f, 0.0f ), 1.0f, 0xffffffff, 0, 0.0f, 1.0f );
	m_pVertex[3] = D3DTLVERTEX( D3DVECTOR( 0.0f, 0.0f, 0.0f ), 1.0f, 0xffffffff, 0, 1.0f, 1.0f );

	m_Idx[0] = 2;
	m_Idx[1] = 1;
	m_Idx[2] = 0;
	m_Idx[3] = 2;
	m_Idx[4] = 3;
	m_Idx[5] = 1;

	char strDir[64] = {0,};

	for(int i=0; i<10; i++)
	{
		sprintf( strDir, "CountDown%d.tga", i);
		m_tID_Numberic[i] = m_pIDTexture->CreateTextureFromFile( strDir );
	}

	m_tID_Numberic[10] = m_pIDTexture->CreateTextureFromFile( "rankwar_ready.tga" );
	m_tID_Numberic[11] = m_pIDTexture->CreateTextureFromFile( "rankwar_start.tga" );

	m_pIDTexture->RestoreAllTextures( GET_D3DDEVICE() );
}

void CGuildRankWarBattleCount::SetActive(bool active)
{
	m_bActive = active;

	if( m_bActive )
	{
		m_CountState = IT_READY;
		m_befortime = timeGetTime();
		m_alpha = 1.0f;
		m_scale = 1.0f;
	}
}

void CGuildRankWarBattleCount::StartCount(int count)
{
	if(count > 9)
		count = 9;

	m_bActive		= true;
	m_nCount		= count;
	m_CountState	= IT_NUM;
	m_nFrameCount	= 0.0f;
	m_befortime		= timeGetTime();
}

void CGuildRankWarBattleCount::EndCount()
{
	if( !m_bActive )
		return;

	m_bActive		= false;
	m_CountState	= IT_NONE;
	m_alpha			= 1.0f;
	m_scale			= 1.0f;
	m_nFrameCount	= 0.0f;

	// by evilkiki 2010.06.04 리소스 해제오류
	// DeleteRes();
}

void CGuildRankWarBattleCount::StartBattle()
{
	m_bActive		= true;
	m_CountState	= IT_START;
	m_alpha			= 1.0f;
	m_scale			= 1.0f;
	m_nFrameCount	= 0.0f;
	m_befortime		= timeGetTime();
}

void CGuildRankWarBattleCount::SetVertex(IMGTYPE imgType ,float alpha, float scale )
{
	FLOAT	w = 0;
	FLOAT	h = 0;

	switch( imgType )
	{
	case IT_NUM: //Back Img
		w = (FLOAT)IMGNUMBERRIC_W;
		h = (FLOAT)IMGNUMBERRIC_H;
		break;

	case IT_READY: // Number img
		w = (FLOAT)IMGREADY_W;
		h = (FLOAT)IMGREADY_H;
		break;

	case IT_START:
		w = (FLOAT)IMGSTART_W;
		h = (FLOAT)IMGSTART_H;
		break;
	}

	FLOAT	x = (FLOAT)g_pNk2DFrame->GetClientWidth()  / 2.0f;
	FLOAT	y = 65.0f + h/2.0f ;//(FLOAT)g_pNk2DFrame->GetClientHeight() / 2.0f;

	m_pVertex[0].sx = x - (w*scale) / 2 - 0.5f;
	m_pVertex[1].sx = x + (w*scale) / 2 - 0.5f;
	m_pVertex[2].sx = x - (w*scale) / 2 - 0.5f;
	m_pVertex[3].sx = x + (w*scale) / 2 - 0.5f;

	m_pVertex[0].sy = y - (h*scale) / 2	- 0.5f;
	m_pVertex[1].sy = y - (h*scale) / 2	- 0.5f;
	m_pVertex[2].sy = y + (h*scale)	/ 2 - 0.5f;
	m_pVertex[3].sy = y + (h*scale) / 2	- 0.5f;

	m_pVertex[0].color = D3DRGBA(1.0f, 1.0f, 1.0f, alpha);
	m_pVertex[1].color = D3DRGBA(1.0f, 1.0f, 1.0f, alpha);
	m_pVertex[2].color = D3DRGBA(1.0f, 1.0f, 1.0f, alpha);
	m_pVertex[3].color = D3DRGBA(1.0f, 1.0f, 1.0f, alpha);
}


void CGuildRankWarBattleCount::Render()
{
	if( !m_bActive)
		return;

	switch(m_CountState)
	{
	case IT_READY:
		{
			m_alpha = 0.5f + (float)( (timeGetTime()/100) % 10 ) * 0.03f;
			SetVertex(IT_READY, m_alpha);
			Render( IT_READY );
		}
		break;

	case IT_NUM:
		{
			DWORD curTime = timeGetTime();
			if( curTime - m_befortime >= 1000 )
			{
				m_befortime = curTime;
				m_nCount--;
				m_nFrameCount = 0.0f;

				if( m_nCount == -1 )
				{
					EndCount();
					break;
				}
			}
			m_nFrameCount += 0.1f;
			m_alpha = 1.0f - m_nFrameCount;
			m_scale = 1.0f + m_nFrameCount*2.5f;

			if( m_alpha < 0.0f) m_alpha = 0.0f;

			SetVertex( IT_NUM, m_alpha, m_scale);
			Render( m_nCount );
		}
		break;

	case IT_START:
		{
			if( timeGetTime() - m_befortime >= 5000 )
			{
				EndCount();
				break;
			}
			else
//			if( timeGetTime() - m_befortime >= 1000 )
			{
				m_nFrameCount += 0.05f;
				m_alpha = 1.0f - m_nFrameCount;
				m_scale = 1.0f + m_nFrameCount*2.5f;
				if( m_alpha < 0.0f)	m_alpha = 0.0f;
			}

			SetVertex(IT_START, m_alpha, m_scale);
			Render( IT_START );
		}
		break;
	}
}


void CGuildRankWarBattleCount::Render( int TexID  )
{

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

	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////

	GET_D3DDEVICE()->SetTexture(0, m_pIDTexture->GetSurfaceByIdx( TexID ) );
	GET_D3DDEVICE()->DrawIndexedPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_TLVERTEX, m_pVertex, 4, m_Idx, 6, NULL);

	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState[2] );

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND, dwRenState[0] );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND, dwRenState[1] );

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, dwlingting );
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, dwAlphaBlend);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHATESTENABLE, dwAlphaTest);

	/*
	DWORD dwAlphaBlend;
	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, &dwAlphaBlend);

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, TRUE);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA);
	// 필터링
	DWORD dwMag;
	GET_D3DDEVICE()->GetTextureStageState(0, D3DTSS_MAGFILTER, &dwMag);
	GET_D3DDEVICE()->SetTextureStageState(0, D3DTSS_MAGFILTER, D3DTFG_LINEAR);

	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////

	GET_D3DDEVICE()->SetTexture(0, m_pIDTexture->GetSurfaceByIdx( TexID ) );
	GET_D3DDEVICE()->DrawIndexedPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_TLVERTEX, m_pVertex, 4, m_Idx, 6, NULL);
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////

	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, FALSE);
	GET_D3DDEVICE()->SetTextureStageState(0, D3DTSS_MAGFILTER, dwMag);

	*/
}

CControlGuildBattle::CControlGuildBattle()
	: m_wX(0)
	, m_wY(0)
{
	m_bActive	= false;
	for(int i=0 ; i<_MAX_NUMBER; i++)
	{
		m_ATeamNum[i].Num  = m_BTeamNum[i].Num  = 0;
		m_ATeamNum[i].XPos = m_BTeamNum[i].XPos = 0;
		m_ATeamNum[i].YPos = m_BTeamNum[i].YPos = 0;
	}

	m_texID_Back = 0;

	for(int i=0; i< 10; i++)
		m_texID_Numberic[i] = -1;

	m_pIndexedTexture = NULL;
	m_pIndexedTexture = new IndexedTexture( 11 );

	if( m_pIndexedTexture )
		m_pIndexedTexture->SetTexturePath("interface/guild/guild_battle/");

	m_pVertex = NULL;

	m_Idx[0] = 0;
	m_Idx[1] = 2;
	m_Idx[2] = 1;
	m_Idx[3] = 1;
	m_Idx[4] = 2;
	m_Idx[5] = 3;

	m_ATeamName.Init(  12, RGB(250, 235, 0), 0, 0);
	m_BTeamName.Init(  12, RGB(250, 235, 0), 0, 0);
}


CControlGuildBattle::~CControlGuildBattle()
{
	DeleteRes();
}


void CControlGuildBattle::DeleteRes()
{
	SAFE_DELETE(m_pVertex);
	SAFE_DELETE(m_pIndexedTexture);
}

void CControlGuildBattle::LoadResource()
{
	for(int i=0 ; i<_MAX_NUMBER; i++)
	{
		m_ATeamNum[i].Num  = m_BTeamNum[i].Num  = 0;
		m_ATeamNum[i].XPos = m_BTeamNum[i].XPos = 0;
		m_ATeamNum[i].YPos = m_BTeamNum[i].YPos = 0;
	}

	m_pVertex = new D3DTLVERTEX[4];

	m_pVertex[0] = D3DTLVERTEX( D3DVECTOR( 0.0f, 0.0f, 0.0f ), 1.0f, 0xffffffff, 0, 0.0f, 0.0f );
	m_pVertex[1] = D3DTLVERTEX( D3DVECTOR( 0.0f, 0.0f, 0.0f ), 1.0f, 0xffffffff, 0, 1.0f, 0.0f );
	m_pVertex[2] = D3DTLVERTEX( D3DVECTOR( 0.0f, 0.0f, 0.0f ), 1.0f, 0xffffffff, 0, 0.0f, 1.0f );
	m_pVertex[3] = D3DTLVERTEX( D3DVECTOR( 0.0f, 0.0f, 0.0f ), 1.0f, 0xffffffff, 0, 1.0f, 1.0f );

	m_Idx[0] = 2;
	m_Idx[1] = 1;
	m_Idx[2] = 0;
	m_Idx[3] = 2;
	m_Idx[4] = 3;
	m_Idx[5] = 1;

	if( !m_pIndexedTexture )
	{
		m_pIndexedTexture = new IndexedTexture( 11 );
		m_pIndexedTexture->SetTexturePath("interface/guild/guild_battle/");
	}

	char strDir[64] = {0,};

	for( int i=0; i<10; i++)
	{
		sprintf( strDir, "number_%01d.tga", i);
		m_texID_Numberic[i] = m_pIndexedTexture->CreateTextureFromFile( strDir );
	}

	m_texID_Back = m_pIndexedTexture->CreateTextureFromFile( "score_bk.tga" );

	m_pIndexedTexture->RestoreAllTextures( GET_D3DDEVICE() );

	SetPosition();
}


void CControlGuildBattle::SetPosition()
{
	m_wX = ( g_pNk2DFrame->GetClientWidth() - BACKIMG_W ) / 2;
	m_wY = 30;

	for(int i=0; i<_MAX_NUMBER; i++)
	{
		m_ATeamNum[i].XPos = m_wX + 144 + NUMBERIC_W * i;
		m_BTeamNum[i].XPos = m_wX + ( BACKIMG_W / 2 ) + 27 + NUMBERIC_W * i;

		m_ATeamNum[i].YPos = m_BTeamNum[i].YPos = m_wY + 8;
	}
}

void CControlGuildBattle::SetVertex( int x, int y, IMGTYP imgType )
{
	int w = 0;
	int h = 0;
	switch( imgType )
	{
	case BACKIMG: //Back Img
		w = BACKIMG_W;
		h = BACKIMG_H;
		break;

	case NUMBERIMG: // Number img
		w = NUMBERIC_W;
		h = NUMBERIC_H;
		break;
	}

	m_pVertex[0].sx = (FLOAT)x			- 0.5f;
	m_pVertex[1].sx = (FLOAT)(x + w)	- 0.5f;
	m_pVertex[2].sx = (FLOAT)x			- 0.5f;
	m_pVertex[3].sx = (FLOAT)(x + w)	- 0.5f;

	m_pVertex[0].sy = (FLOAT)y			- 0.5f;
	m_pVertex[1].sy = (FLOAT)y			- 0.5f;
	m_pVertex[2].sy = (FLOAT)(y + h)	- 0.5f;
	m_pVertex[3].sy = (FLOAT)(y + h)	- 0.5f;

//	m_pVertex[0].color = D3DRGBA(1.0f, 1.0f, 1.0f, alpha);
//	m_pVertex[1].color = D3DRGBA(1.0f, 1.0f, 1.0f, alpha);
//	m_pVertex[2].color = D3DRGBA(1.0f, 1.0f, 1.0f, alpha);
//	m_pVertex[3].color = D3DRGBA(1.0f, 1.0f, 1.0f, alpha);

}
void CControlGuildBattle::Render( int TexID )
{
	GET_D3DDEVICE()->SetTexture(0, m_pIndexedTexture->GetSurfaceByIdx( TexID ) );

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, TRUE);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA);
	// 필터링
	DWORD dwMag;
	GET_D3DDEVICE()->GetTextureStageState(0, D3DTSS_MAGFILTER, &dwMag);
	GET_D3DDEVICE()->SetTextureStageState(0, D3DTSS_MAGFILTER, D3DTFG_LINEAR);

	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////

	GET_D3DDEVICE()->DrawIndexedPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_TLVERTEX, m_pVertex, 4, m_Idx, 6, NULL);
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////

	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, FALSE);
	GET_D3DDEVICE()->SetTextureStageState(0, D3DTSS_MAGFILTER, dwMag);
}
void CControlGuildBattle::Render()
{
	if( !m_bActive )
		return;

	if( !GET_D3DDEVICE())
		return;

	SetVertex(m_wX, m_wY, BACKIMG );
	Render( m_texID_Back );

//	bool	bPrevNumZeroCheck = true;

	for(int i=0; i<_MAX_NUMBER; i++)
	{
		// 울팀
//		if( m_ATeamNum[i].Num > 0 )
//		{	bPrevNumZeroCheck = false; }

//		if( !bPrevNumZeroCheck )
		{
			SetVertex(m_ATeamNum[i].XPos, m_ATeamNum[i].YPos, NUMBERIMG );
			Render( m_texID_Numberic[m_ATeamNum[i].Num] );
		}
	}

//	bPrevNumZeroCheck = true;

	for( int i=0; i<_MAX_NUMBER; i++)
	{
//		if( m_BTeamNum[i].Num > 0 )
//		{	bPrevNumZeroCheck = false; }

//		if( !bPrevNumZeroCheck )
		{
			SetVertex(m_BTeamNum[i].XPos, m_BTeamNum[i].YPos, NUMBERIMG );
			Render( m_texID_Numberic[m_BTeamNum[i].Num] );
		}
	}

	m_ATeamName.Draw();
	m_BTeamName.Draw();
}

void CControlGuildBattle::SetTeamKillcount(int ATeamKCount, int BTeamKCount)
{
	m_ATeamNum[0].Num = ATeamKCount / 100;
	m_ATeamNum[1].Num = (ATeamKCount % 100) / 10;
	m_ATeamNum[2].Num = ATeamKCount % 10;

	m_BTeamNum[0].Num = BTeamKCount / 100;
	m_BTeamNum[1].Num = (BTeamKCount % 100) / 10;
	m_BTeamNum[2].Num = BTeamKCount % 10;
}

void CControlGuildBattle::SetTeamName(char* ATeamName, char* BTeamName )
{
	m_ATeamName.SetString( ATeamName );
	int xPos = m_wX + 72 - m_ATeamName.GetBoxWidth()/2 ;
	m_ATeamName.SetPos(xPos, m_wY + 17);

	m_BTeamName.SetString( BTeamName );
	xPos = m_wX + 399 - m_BTeamName.GetBoxWidth()/2 ;
	m_BTeamName.SetPos(xPos, m_wY + 17);
}

bool CControlGuildBattle::IsInside(int x, int y)
{
//	if( x > m_pBack->Xpos && x < m_pBack->Xpos + m_pBack->GetWidth() &&
//		y > m_pBack->Ypos && y < m_pBack->Ypos + m_pBack->GetHeight())
//		return true;

	return false;
}


HRESULT CControlGuildBattle::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	switch( msg )
	{
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_MOUSEMOVE:
		if( IsInside( HIWORD(wParam), LOWORD(wParam) ) )
			return S_OK;
		break;
	}
	return E_FAIL;
}