#include "stdafx.h"
#define D3D_OVERLOADS



// nate 2004 - 4 : CastleWar
#include <time.h>
#include <math.h>
#include <d3d.h>
#include "d3dutil.h"
#include "d3dutil.h"
#include "main.h"
#include "IndexedTexture.h"
#include "basicbutton.h"
#include "textoutbox.h"
#include "nkcharacter.h"
#include "headers.h"
#include "tcpipcon.h"
#include "ControlLowGuildMaster.h"
#include "Nk2DFrame.h"
#include "Country.h"
#include "Compas.h"
// nate 2004 - 6 : 가입 안내 윈도우 깜빡임 제거
#include "UIMgr.h"






extern DWORD	g_dwLangType;
extern DWORD	g_dwClientCountry;

CControlLowGuildMaster::CControlLowGuildMaster()
{
	m_pBack			= NULL;
	m_pBack2		= NULL;	// nate 2004 - 4 : CastleWar
	m_byRandomNum	= 0;
	m_bActive		= TRUE;
	m_iMainX		= 0;
	m_iMainY		= 0;
	m_dwLastOpenTime	= timeGetTime();
	m_IsOpenWindow	= FALSE;
	m_IsVisible		= FALSE;
}

CControlLowGuildMaster::~CControlLowGuildMaster()
{
	DeleteRes();
}

void CControlLowGuildMaster::LoadRes()
{
	if(!g_pDisplay)
		return;

	int X = 0, Y = 0;
	g_pDisplay->CreateSurfaceFromBitmap(&m_pBack, "interface/LowGuild/guild_b_r_war1.bmp");
	if( m_pBack )
	{
		m_pBack->SetColorKey(RGB(0, 0, 0));
		m_pBack->Xpos = GET_CLIENT_WIDTH()/2 - m_pBack->GetWidth()/2;
		m_pBack->Ypos = GET_CLIENT_HEIGHT()/2 - m_pBack->GetHeight()/2;
		X = m_iMainX = m_pBack->Xpos;
		Y = m_iMainY =m_pBack->Ypos;
	}

	// nate 2004 - 4 : CastleWar
	g_pDisplay->CreateSurfaceFromBitmap( &m_pBack2, "interface/LowGuild/guild_b_r_war2.bmp" );
	if( m_pBack2 )
	{
		m_pBack2->SetColorKey( RGB( 0, 0, 0 ) );
		m_pBack2->Xpos = X;
		m_pBack2->Ypos = Y;
	}

	m_BlueBtn.SetFileName("LowGuild/btn_guild_b2");
	m_RedBtn.SetFileName("LowGuild/btn_guild_r2");
	m_OutBtn.SetFileName("LowGuild/btn_guild_e");
	m_CloseBtn.SetFileName("LowGuild/btn_G_n");

	m_BlueBtn.LoadRes();
	m_RedBtn.LoadRes();
	m_OutBtn.LoadRes();
	m_CloseBtn.LoadRes();

	m_BlueBtn.PosX	= X+70;
	m_BlueBtn.PosY	= Y+329;
	m_RedBtn.PosX	= X+204;
	m_RedBtn.PosY	= Y+329;
	m_OutBtn.PosX	= X+136;
	m_OutBtn.PosY	= Y+329;
	m_CloseBtn.PosX	= X+302;
	m_CloseBtn.PosY	= Y+360;

	// nate 2004 - 4 : CastleWar
	m_pSiegeDate.Init( 16, _FCOLOR_YELLOW, 0, 0 );
	m_pSiegeDate.SetString("\0");

	srand( (unsigned)time( NULL ) );
	m_byRandomNum = rand() % 2;
}

void CControlLowGuildMaster::DeleteRes()
{
	SAFE_DELETE(m_pBack);
	SAFE_DELETE( m_pBack2 );

	m_BlueBtn.DeleteRes();
	m_RedBtn.DeleteRes();
	m_OutBtn.DeleteRes();
	m_CloseBtn.DeleteRes();
	m_pSiegeDate.DeleteRes();
}
BOOL CControlLowGuildMaster::IsInside(int x, int y)
{
	if (x >= (int)(m_pBack->Xpos)
			&& x < (int)(m_pBack->Xpos + m_pBack->GetWidth())
			&& y >= (int)(m_pBack->Ypos)
			&& y < (int)(m_pBack->Ypos + m_pBack->GetHeight()) )
		return TRUE;

	return FALSE;
}
void CControlLowGuildMaster::SendGuildMsg(int index)
{
	char tmp[100];

	if( index == OUT_GUILD )
		sprintf(tmp, "mguild out\n");
	else
		sprintf(tmp, "mguild req %d\n", index);

	if (g_pTcpIp)
		g_pTcpIp->SendNetMessage(tmp);
}

void CControlLowGuildMaster::SetSiegeDateString(char *SiegeDate)
{
	m_pSiegeDate.SetString(SiegeDate);
}
void CControlLowGuildMaster::Draw()
{
	if(!m_bActive || !g_pDisplay)
		return;

	bool IsVisible = FALSE;

	if( g_pNk2DFrame
			&& ( g_pNk2DFrame->IsHelpVisible()
				 || g_pNk2DFrame->IsInvenVisible()
				 || g_pNk2DFrame->IsPartyVisible()
				 || g_pNk2DFrame->IsInterfaceVisible(SCORE)
				 || g_pNk2DFrame->IsSkillVisible()
				 || g_pNk2DFrame->IsInterfaceVisible(MAP)
				 || g_pNk2DFrame->IsPetStatusVisible()
				 || g_pNk2DFrame->IsInterfaceVisible(QUEST)
				 || g_pNk2DFrame->IsControlGuild()
				 || g_pNk2DFrame->IsControlGuildManage()
			   ))
		IsVisible = TRUE;
	else
		IsVisible = FALSE;

	// nate 2004 - 4 : CastleWar
	SetDrawPos();

	if( m_byRandomNum )
		g_pDisplay->Blt( m_pBack->Xpos, m_pBack->Ypos, m_pBack );
	else
		g_pDisplay->Blt( m_pBack->Xpos, m_pBack->Ypos, m_pBack2 );
//=======================================

	if( g_pRoh->m_GuildIndex )
		m_OutBtn.Draw();
	else
	{
		m_BlueBtn.Draw();
		m_RedBtn.Draw();
	}
	m_CloseBtn.Draw();
	// nate 2004 - 4 : CastleWar
	m_pSiegeDate.Draw();

	if(	m_IsOpenWindow == FALSE )
	{
		if ( (timeGetTime() - m_dwLastOpenTime) > 300 )
		{
			m_IsOpenWindow = TRUE;
			m_dwLastOpenTime = timeGetTime();
		}
	}
}

void CControlLowGuildMaster::SetDrawPos()
{
	if( !m_pBack )
		return;

	RECT rc;
	HWND hWnd = g_pDisplay->GetHWnd();
	GetClientRect(hWnd, &rc);

	if(g_pNk2DFrame
			&& ( g_pNk2DFrame->IsHelpVisible()
				 || g_pNk2DFrame->IsInvenVisible()
				 || g_pNk2DFrame->IsPartyVisible()
				 || g_pNk2DFrame->IsInterfaceVisible(SCORE)
				 || g_pNk2DFrame->IsSkillVisible()
				 || g_pNk2DFrame->IsInterfaceVisible(MAP)
				 || g_pNk2DFrame->IsPetStatusVisible()
				 || g_pNk2DFrame->IsInterfaceVisible(QUEST)
				 || g_pNk2DFrame->IsControlGuild()
				 || g_pNk2DFrame->IsControlGuildManage()
			   ))
		m_pBack->Xpos = GET_CLIENT_WIDTH()/2 - m_pBack->GetWidth()/2 - 274/2;
	else
		m_pBack->Xpos = GET_CLIENT_WIDTH()/2 - m_pBack->GetWidth()/2;

	m_pBack->Ypos = GET_CLIENT_HEIGHT()/2 - m_pBack->GetHeight()/2;

	int X = m_iMainX = m_pBack->Xpos;
	int Y = m_iMainY =m_pBack->Ypos;

	m_BlueBtn.PosX	= X+70;
	m_BlueBtn.PosY	= Y+329;
	m_RedBtn.PosX	= X+204;
	m_RedBtn.PosY	= Y+329;
	m_OutBtn.PosX	= X+136;
	m_OutBtn.PosY	= Y+329;
	m_CloseBtn.PosX	= X+302;
	m_CloseBtn.PosY	= Y+360;

	// nate 2004 - 4 : CastleWar
	m_pSiegeDate.SetPos( ( X + 170 ) - m_pSiegeDate.GetBoxWidth() / 2, Y + 275 );

	return;
}


LRESULT CControlLowGuildMaster::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int x, y;
	int result = 1;

	if(!m_bActive)
		return -1;

	x = LOWORD (lParam);
	y = HIWORD (lParam);

	switch (msg)
	{
	case WM_LBUTTONDOWN:
		x = LOWORD (lParam);
		y = HIWORD (lParam);

		if(!IsInside(x, y))
			return 2;
		if( !g_pRoh )
			return 0;

		// nate 2004 - 6 : 데카르디 공성관련 - 버튼 클릭시 타 버튼에 영향을 미치는 버그 수정
		if( !g_pRoh->m_GuildIndex )
		{
			m_BlueBtn.MsgProc(hWnd, msg, wParam, lParam);
			m_RedBtn.MsgProc(hWnd, msg, wParam, lParam);
		}
		else
			m_OutBtn.MsgProc(hWnd, msg, wParam, lParam);

		m_CloseBtn.MsgProc(hWnd, msg, wParam, lParam);
		return 1;

	case WM_LBUTTONUP:
		x = LOWORD (lParam);
		y = HIWORD (lParam);

		if (!IsInside(x, y))
			// nate 2004 - 6 : 가입 안내 윈도우 깜빡거림 제거
			if( !pCMyApp->m_pUIMgr->m_bIsLowGuildMaster )
				return 2;

		m_BlueBtn.MsgProc(hWnd, msg, wParam, lParam);
		m_RedBtn.MsgProc(hWnd, msg, wParam, lParam);
		m_OutBtn.MsgProc(hWnd, msg, wParam, lParam);
		m_CloseBtn.MsgProc(hWnd, msg, wParam, lParam);
		if(m_BlueBtn.m_iBtnFlg == BTN_ACTION)
		{
			SendGuildMsg(BLUE_GUILD);
			m_BlueBtn.m_iBtnFlg = BTN_NORMAL;
			return 2;
		}
		else if(m_RedBtn.m_iBtnFlg == BTN_ACTION)
		{
			SendGuildMsg(RED_GUILD);
			m_RedBtn.m_iBtnFlg = BTN_NORMAL;
			return 2;
		}
		else if(m_OutBtn.m_iBtnFlg == BTN_ACTION)
		{
			SendGuildMsg(OUT_GUILD);
			m_OutBtn.m_iBtnFlg = BTN_NORMAL;
			return 2;
		}
		else if(m_CloseBtn.m_iBtnFlg == BTN_ACTION)
		{
			m_CloseBtn.m_iBtnFlg = BTN_NORMAL;
			return 2;
		}
		break;

	case WM_MOUSEMOVE:
		x = LOWORD (lParam);
		y = HIWORD (lParam);

		m_BlueBtn.MsgProc(hWnd, msg, wParam, lParam);
		m_RedBtn.MsgProc(hWnd, msg, wParam, lParam);
		m_OutBtn.MsgProc(hWnd, msg, wParam, lParam);
		m_CloseBtn.MsgProc(hWnd, msg, wParam, lParam);
		return 1;
		break;
	}
	return 0;
}

