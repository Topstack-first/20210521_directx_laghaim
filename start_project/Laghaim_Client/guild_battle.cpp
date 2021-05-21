#include "stdafx.h"
#include <time.h>
#include "guild_battle.h"
#include "d3dutil.h"
#include "headers.h"
#include "Nk2DFrame.h"
#include "2DResMgr.h"
#include "main.h"
#include "window_resource.h"
#include "WindowMgr.h"
#include "Tcpipcon.h"
#include "NkCharacter.h"
#include "s_guild_manager.h"
#include "g_stringmanager.h"
#include "ControlBottom.h"
#include "CommonConfig.h"

CBattleSelect::CBattleSelect() : CUIBaseWindow()
{
	m_pTextSur = NULL;

	ZeroMemory(m_MsgBuf, sizeof(m_MsgBuf));
}

CBattleSelect::~CBattleSelect()
{
	DeleteRes();

	SAFE_DELETE(m_pTextSur);
}

BOOL CBattleSelect::InitMode()
{
	m_btRankBattle.SetFileName("guild/rankbattle");
	m_btInvasion.SetFileName("guild/invasion");
	m_btCancel.SetFileName("guild/msg_no");

	return TRUE;
}

void CBattleSelect::LoadRes()
{
	int nScreenWidth  = 0;
	int nScreenHeight = 0;

	if( g_pNk2DFrame )
	{
		nScreenWidth  = g_pNk2DFrame->GetClientWidth();
		nScreenHeight = g_pNk2DFrame->GetClientHeight();
	}

	SetBackground(WBK_BATTLE);
	MakeCloseBtn("guild/exit");

	SAFE_DELETE(m_pTextSur);

	g_pDisplay->CreateSurfaceFromBitmap(&m_pTextSur, "interface/guild/guild_text/NPC_guild.bmp");

	if(m_pBackScene)
	{
		int n_x = (nScreenWidth  / 2) - (m_pBackScene->GetWidth()  / 2);
		int n_y = (nScreenHeight / 2) - (m_pBackScene->GetHeight() / 2) - 30;

		if( n_y <= 0)
			n_y = 1;

		m_nPosX = n_x;
		m_nPosY = n_y;

		if(m_pTextSur)
		{
			m_pTextSur->Xpos = m_nPosX + 23;
			m_pTextSur->Ypos = m_nPosY + 51;
		}

		m_btRankBattle.LoadRes();
		m_btInvasion.LoadRes();
		m_btCancel.LoadRes();

		SetPosition();

		if( CommonConfig::Instance()->GetGuildInvasionEnable() == false )
		{
			m_btInvasion.SetDisable(TRUE);
			m_btInvasion.SetVisible(FALSE);
		}
	}
}


void CBattleSelect::SetPosition()
{
	m_pBackScene->Xpos = m_nPosX;
	m_pBackScene->Ypos = m_nPosY;

	m_btRankBattle.SetPosition(m_nPosX + 86 , m_nPosY + 253);
	m_btInvasion.SetPosition(m_nPosX + 86, m_nPosY + 310);

	m_btCancel.SetPosition( m_nPosX + 60 ,
							m_nPosY + m_pBackScene->GetHeight() - m_btCancel.Height - 30 );
	m_pBtExit->SetPosition( m_nPosX + m_pBackScene->GetWidth() -  m_pBtExit->Width   - 60 ,
							m_nPosY + m_pBackScene->GetHeight() - m_pBtExit->Height  - 30 );
	m_pTextSur->Xpos = m_nPosX + 23;
	m_pTextSur->Ypos = m_nPosY + 51;
}

void CBattleSelect::DeleteRes()
{
	m_btRankBattle.DeleteRes();
	m_btInvasion.DeleteRes();
	m_btCancel.DeleteRes();
}

void CBattleSelect::RestoreSurfaces()
{

}

int CBattleSelect::Draw()
{
	if( g_pDisplay && m_pBackScene)
	{
		if(CUIBaseWindow::Draw())
		{
			m_btRankBattle.Draw();
			m_btInvasion.Draw();
			m_btCancel.Draw();

			if(m_pTextSur)	g_pDisplay->Blt(m_pTextSur->Xpos, m_pTextSur->Ypos, m_pTextSur);
		}
	}
	return -1;
}

BOOL CBattleSelect::MoveWindow(UINT msg, int x, int y)
{
	return CUIBaseWindow::MoveWindow(msg, x, y);
}


BOOL CBattleSelect::IsInside(int x, int y)
{
	return CUIBaseWindow::IsInside(x, y);
}

void CBattleSelect::SendMsg()
{
	g_pTcpIp->SendNetMessage(m_MsgBuf);
}

LRESULT CBattleSelect::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	m_btRankBattle.MsgProc(hWnd, msg, wParam, lParam);
	m_btInvasion.MsgProc(hWnd, msg, wParam, lParam);
	m_btCancel.MsgProc(hWnd, msg, wParam, lParam);

	switch( msg )
	{
	case WM_LBUTTONDOWN:

		if(!IsInside(LOWORD(lParam), HIWORD(lParam)))
			return IM_NONE;

		if( m_btRankBattle.GetState() == BTN_DOWN )
			return IM_INPUT;

		if( m_btInvasion.GetState() == BTN_DOWN )
			return IM_INPUT;

		if( m_btCancel.GetState() == BTN_DOWN )
			return IM_INPUT;


		break;

	case WM_LBUTTONUP :

		if(!IsInside(LOWORD(lParam), HIWORD(lParam)))
			return IM_NONE;

		if(m_btRankBattle.GetState() == BTN_ACTION)
		{
			g_pNk2DFrame->GetGuildSystemManager()->SetMainType(RANK_BATTLE_1VS1);
			g_pNk2DFrame->GetUIWindow()->OnOpenWindow(WINDOW_GUILD_BATTLE_RANKING_MENU);

			return IM_INPUT;
		}

		if(m_btInvasion.GetState() == BTN_ACTION)
		{
			g_pNk2DFrame->GetUIWindow()->OnOpenWindow(WINDOW_GUILD_BATTLE_INVASION_REQUEST);

			return IM_INPUT;
		}

		if( m_btCancel.GetState() == BTN_ACTION )
		{
			g_pNk2DFrame->GetUIWindow()->OnOpenWindow(WINDOW_GUILD_MEMBER);
			return IM_INPUT;
		}

		break;
	}

	return CUIBaseWindow::MsgProc(hWnd, msg, wParam, lParam);
}

/*******************************************************************************************************/
// 길드 침략 신청 창
CBattleInvasion::CBattleInvasion() : CUIBaseWindow()
{
	m_pTextSur = NULL;
}

CBattleInvasion::~CBattleInvasion()
{
	DeleteRes();

	SAFE_DELETE(m_pTextSur);
}

BOOL CBattleInvasion::InitMode()
{
	m_btInvasionRequest.SetFileName("guild/invasion_request");
	m_btRequestInfo.SetFileName("guild/invasion_status");
	m_btCancel.SetFileName("guild/msg_no");

	return TRUE;
}


void CBattleInvasion::LoadRes()
{
	int nScreenWidth  = 0;
	int nScreenHeight = 0;

	if( g_pNk2DFrame )
	{
		nScreenWidth  = g_pNk2DFrame->GetClientWidth();
		nScreenHeight = g_pNk2DFrame->GetClientHeight();
	}

	SetBackground(WBK_BATTLE);
	MakeCloseBtn("guild/exit");

	SAFE_DELETE(m_pTextSur);

	g_pDisplay->CreateSurfaceFromBitmap(&m_pTextSur, "interface/guild/guild_text/NPC_guild.bmp");

	if(m_pBackScene)
	{
		int n_x = (nScreenWidth  / 2) - (m_pBackScene->GetWidth()  / 2);
		int n_y = (nScreenHeight / 2) - (m_pBackScene->GetHeight() / 2) - 30;

		if( n_y <= 0)
			n_y = 1;

		m_nPosX = n_x;
		m_nPosY = n_y;

		if(m_pTextSur)
		{
			m_pTextSur->Xpos = m_nPosX + 40;
			m_pTextSur->Ypos = m_nPosY + 70;
		}

		m_btInvasionRequest.LoadRes();
		m_btRequestInfo.LoadRes();
		m_btCancel.LoadRes();

		SetPosition();

	}
}

void CBattleInvasion::SetPosition()
{
	m_btInvasionRequest.SetPosition(m_nPosX + 86 , m_nPosY + 253);
	m_btRequestInfo.SetPosition(m_nPosX + 86, m_nPosY + 310);

	//SetExitBtnPostion(255, 408);
	m_btCancel.SetPosition( m_nPosX + 60 ,
							m_nPosY + m_pBackScene->GetHeight() - m_btCancel.Height - 30 );
	m_pBtExit->SetPosition( m_nPosX + m_pBackScene->GetWidth() -  m_pBtExit->Width  - 60 ,
							m_nPosY + m_pBackScene->GetHeight() - m_pBtExit->Height - 30 );

	m_pTextSur->Xpos = m_nPosX + 23;
	m_pTextSur->Ypos = m_nPosY + 51;

}

void CBattleInvasion::DeleteRes()
{
	m_btInvasionRequest.DeleteRes();
	m_btRequestInfo.DeleteRes();
	m_btCancel.DeleteRes();
}

void CBattleInvasion::RestoreSurfaces()
{

}

int CBattleInvasion::Draw()
{
	if( g_pDisplay && m_pBackScene)
	{
		if(CUIBaseWindow::Draw())
		{
			m_btInvasionRequest.Draw();
			m_btRequestInfo.Draw();
			m_btCancel.Draw();

			if(m_pTextSur)	g_pDisplay->Blt(m_pTextSur->Xpos, m_pTextSur->Ypos, m_pTextSur);
		}
	}
	return -1;
}

BOOL CBattleInvasion::MoveWindow(UINT msg, int x, int y)
{
	return CUIBaseWindow::MoveWindow(msg, x, y);
}


BOOL CBattleInvasion::IsInside(int x, int y)
{
	return CUIBaseWindow::IsInside(x, y);
}

LRESULT CBattleInvasion::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	m_btInvasionRequest.MsgProc(hWnd, msg, wParam, lParam);
	m_btRequestInfo.MsgProc(hWnd, msg, wParam, lParam);
	m_btCancel.MsgProc(hWnd, msg, wParam, lParam);

	switch( msg )
	{
	case WM_LBUTTONDOWN :

		if(!IsInside(LOWORD(lParam), HIWORD(lParam)))
			return IM_NONE;

		if( m_btInvasionRequest.GetState() == BTN_DOWN )
			return IM_INPUT;

		if( m_btRequestInfo.GetState() == BTN_DOWN )
			return IM_INPUT;

		if( m_btCancel.GetState() == BTN_DOWN )
			return IM_INPUT;

		break;

	case WM_LBUTTONUP :

		if(!IsInside(LOWORD(lParam), HIWORD(lParam)))
			return IM_NONE;

		if(m_btInvasionRequest.GetState() == BTN_ACTION)
		{
			g_pNk2DFrame->GetUIWindow()->OnOpenWindow(WINDOW_GUILD_BATTLE_INVASION);
			return IM_INPUT;
		}

		if(m_btRequestInfo.GetState() == BTN_ACTION)
		{
			//if(g_pRoh->GetMyGuildData()->IsInvasion_Attack())
			//	g_pNk2DFrame->GetUIWindow()->OnOpenWindow(WINDOW_GUILD_BATTLE_INVASION_INFO);
			//else
			g_pNk2DFrame->GetUIWindow()->OnOpenWindow(WINDOW_GUILD_BATTLE_INVASION_INFO);

			return IM_INPUT;
		}

		if( m_btCancel.GetState() == BTN_ACTION )
		{
			g_pNk2DFrame->GetUIWindow()->OnOpenWindow(WINDOW_GUILD_BATTLE_KIND);
			return IM_INPUT;
		}

		break;
	}

	return CUIBaseWindow::MsgProc(hWnd, msg, wParam, lParam);
}
/*******************************************************************************************************/

CBattleRankSelect::CBattleRankSelect() : CUIBaseWindow()
{
	ZeroMemory(m_MsgBuf, sizeof(m_MsgBuf));

	m_pTextSur	= NULL;
}

CBattleRankSelect::~CBattleRankSelect()
{
	DeleteRes();

	SAFE_DELETE(m_pTextSur);
}

BOOL CBattleRankSelect::InitMode()
{
	m_bt1vs1.SetFileName("guild/1vs1_battle");
	m_btFlag.SetFileName("guild/flag_battle");
	m_btobstacl.SetFileName("guild/hurdle_battle");
	m_btCancel.SetFileName("guild/msg_no");
	return TRUE;
}


void CBattleRankSelect::LoadRes()
{
	int nScreenWidth  = 0;
	int nScreenHeight = 0;

	if( g_pNk2DFrame )
	{
		nScreenWidth  = g_pNk2DFrame->GetClientWidth();
		nScreenHeight = g_pNk2DFrame->GetClientHeight();
	}

	SetBackground(WBK_BATTLE);
	MakeCloseBtn("guild/exit");

	SAFE_DELETE(m_pTextSur);

	g_pDisplay->CreateSurfaceFromBitmap(&m_pTextSur, "interface/guild/guild_text/NPC_guild.bmp");

	if(m_pBackScene)
	{

		int n_x = (nScreenWidth  / 2) - (m_pBackScene->GetWidth()  / 2);
		int n_y = (nScreenHeight / 2) - (m_pBackScene->GetHeight() / 2);

		m_nPosX = n_x;
		m_nPosY = n_y;

		if(m_pTextSur)
		{
			m_pTextSur->Xpos = m_nPosX + 40;
			m_pTextSur->Ypos = m_nPosY + 70;
		}

		m_bt1vs1.LoadRes();
		m_btFlag.LoadRes();
		m_btobstacl.LoadRes();

		m_btCancel.LoadRes();


		SetPosition();

	}
}

void CBattleRankSelect::SetPosition()
{
	if(m_pTextSur)
	{
		m_pTextSur->Xpos = m_nPosX + 40;
		m_pTextSur->Ypos = m_nPosY + 70;
	}

	m_bt1vs1.SetPosition(m_nPosX + 86, m_nPosY + 253);
	m_btFlag.SetPosition(m_nPosX + 86, m_nPosY + 289);
	m_btobstacl.SetPosition(m_nPosX + 86, m_nPosY + 325);

	//SetExitBtnPostion(255, 408);
	m_btCancel.SetPosition( m_nPosX + 60 ,
							m_nPosY + m_pBackScene->GetHeight() - m_btCancel.Height - 30 );
	m_pBtExit->SetPosition( m_nPosX + m_pBackScene->GetWidth() -  m_pBtExit->Width  - 60 ,
							m_nPosY + m_pBackScene->GetHeight() - m_pBtExit->Height - 30 );


}

void CBattleRankSelect::DeleteRes()
{
	m_bt1vs1.DeleteRes();
	m_btFlag.DeleteRes();
	m_btobstacl.DeleteRes();
	m_btCancel.DeleteRes();
}

void CBattleRankSelect::RestoreSurfaces()
{

}

int CBattleRankSelect::Draw()
{
	if( g_pDisplay && m_pBackScene)
	{
		if(CUIBaseWindow::Draw())
		{
			m_bt1vs1.Draw();
			m_btFlag.Draw();
			m_btobstacl.Draw();
			m_btCancel.Draw();

			if(m_pTextSur)	g_pDisplay->Blt(m_pTextSur->Xpos, m_pTextSur->Ypos, m_pTextSur);
		}
	}

	return -1;
}

void CBattleRankSelect::SendMsg()
{
	g_pTcpIp->SendNetMessage(m_MsgBuf);
}

BOOL CBattleRankSelect::MoveWindow(UINT msg, int x, int y)
{
	return CUIBaseWindow::MoveWindow(msg, x, y);
}

BOOL CBattleRankSelect::IsInside(int x, int y)
{
	return CUIBaseWindow::IsInside(x, y);
}

LRESULT CBattleRankSelect::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if(!IsInside(LOWORD(lParam), HIWORD(lParam)))
		return IM_NONE;

	m_bt1vs1.MsgProc(hWnd, msg, wParam, lParam);
	m_btFlag.MsgProc(hWnd, msg, wParam, lParam);
	m_btobstacl.MsgProc(hWnd, msg, wParam, lParam);
	m_btCancel.MsgProc(hWnd, msg, wParam, lParam);

	switch( msg )
	{
	case WM_LBUTTONDOWN :

		if(!IsInside(LOWORD(lParam), HIWORD(lParam)))
			return IM_NONE;

		if( m_bt1vs1.GetState() == BTN_DOWN )
			return IM_INPUT;

		if( m_btFlag.GetState() == BTN_DOWN )
			return IM_INPUT;

		if( m_btobstacl.GetState() == BTN_DOWN )
			return IM_INPUT;

		if( m_btCancel.GetState() == BTN_DOWN )
			return IM_INPUT;

		break;

	case WM_LBUTTONUP :
		if(!IsInside(LOWORD(lParam), HIWORD(lParam)))
			return IM_NONE;

		if(m_bt1vs1.GetState() == BTN_ACTION)
		{
			g_pNk2DFrame->GetGuildSystemManager()->SetMainType(RANK_BATTLE_1VS1);

			sprintf(m_MsgBuf, "mg rankwar open %d\n", RANK_BATTLE_1VS1);
			SendMsg();

			g_pNk2DFrame->GetUIWindow()->OnOpenWindow(WINDOW_GUILD_BATTLE_1VS1);
			return IM_INPUT;
		}

		if(m_btFlag.GetState() == BTN_ACTION)
		{
			g_pNk2DFrame->GetGuildSystemManager()->SetMainType(RANK_BATTLE_FLAG);

			return IM_INPUT;
		}

		if(m_btobstacl.GetState() == BTN_ACTION)
		{
			g_pNk2DFrame->GetGuildSystemManager()->SetMainType(RANK_BATTLE_OBSTACLE);
			return IM_INPUT;
		}

		if( m_btCancel.GetState() == BTN_ACTION )
		{
			g_pNk2DFrame->GetUIWindow()->OnOpenPrvWindow();
			return IM_INPUT;
		}

		break;
	}

	return CUIBaseWindow::MsgProc(hWnd, msg, wParam, lParam);
}

/*******************************************************************************************************/

CBattleRankMenu::CBattleRankMenu() : CUIBaseWindow()
{
	ZeroMemory(m_MsgBuf, sizeof(m_MsgBuf));

	m_pTextSur	= NULL;
}

CBattleRankMenu::~CBattleRankMenu()
{
	DeleteRes();

	SAFE_DELETE(m_pTextSur);
}


BOOL CBattleRankMenu::InitMode()
{
	m_btRequest.SetFileName("guild/request_rankbattle");
	m_btWatch.SetFileName("guild/watch_rankbattle");
	m_btTakeItem.SetFileName("guild/takeitem_rankbattle");
	m_btCancel.SetFileName("guild/msg_no");

	return TRUE;
}

void CBattleRankMenu::LoadRes()
{
	int nScreenWidth  = 0;
	int nScreenHeight = 0;

	if( g_pNk2DFrame )
	{
		nScreenWidth  = g_pNk2DFrame->GetClientWidth();
		nScreenHeight = g_pNk2DFrame->GetClientHeight();
	}

	SetBackground(WBK_BATTLE);
	MakeCloseBtn("guild/exit");

	SAFE_DELETE(m_pTextSur);

	g_pDisplay->CreateSurfaceFromBitmap(&m_pTextSur, "interface/guild/guild_text/NPC_guild.bmp");

	if(m_pBackScene)
	{

		int n_x = (nScreenWidth  / 2) - (m_pBackScene->GetWidth()  / 2);
		int n_y = (nScreenHeight / 2) - (m_pBackScene->GetHeight() / 2) - 30 ;

		m_nPosX = n_x;
		m_nPosY = n_y;

		if(m_pTextSur)
		{
			m_pTextSur->Xpos = m_nPosX + 40;
			m_pTextSur->Ypos = m_nPosY + 70;
		}

		m_btRequest.LoadRes();
		m_btWatch.LoadRes();
		m_btTakeItem.LoadRes();

		m_btCancel.LoadRes();

		SetPosition();
	}
}


void CBattleRankMenu::SetPosition()
{
	if(m_pTextSur)
	{
		m_pTextSur->Xpos = m_nPosX + 23;
		m_pTextSur->Ypos = m_nPosY + 51;
	}

	m_btRequest.SetPosition(m_nPosX + 86, m_nPosY + 253);
	m_btWatch.SetPosition(m_nPosX + 86, m_nPosY + 289);
	m_btTakeItem.SetPosition(m_nPosX + 86, m_nPosY + 325);

	//SetExitBtnPostion(255, 408);
	m_btCancel.SetPosition( m_nPosX + 60 ,
							m_nPosY + m_pBackScene->GetHeight() - m_btCancel.Height - 30 );
	m_pBtExit->SetPosition( m_nPosX + m_pBackScene->GetWidth() -  m_pBtExit->Width  - 60 ,
							m_nPosY + m_pBackScene->GetHeight() - m_pBtExit->Height - 30 );

}
void CBattleRankMenu::DeleteRes()
{
	m_btRequest.DeleteRes();
	m_btWatch.DeleteRes();
	m_btTakeItem.DeleteRes();
	m_btCancel.DeleteRes();
}

void CBattleRankMenu::RestoreSurfaces()
{

}

int CBattleRankMenu::Draw()
{
	if( g_pDisplay && m_pBackScene)
	{
		if(CUIBaseWindow::Draw())
		{
			m_btRequest.Draw();
			m_btWatch.Draw();
			m_btTakeItem.Draw();
			m_btCancel.Draw();

			if(m_pTextSur)	g_pDisplay->Blt(m_pTextSur->Xpos, m_pTextSur->Ypos, m_pTextSur);
		}
	}


	return -1;
}

void CBattleRankMenu::SendMsg()
{
	g_pTcpIp->SendNetMessage(m_MsgBuf);
}


BOOL CBattleRankMenu::MoveWindow(UINT msg, int x, int y)
{
	return CUIBaseWindow::MoveWindow(msg, x, y);
}

BOOL CBattleRankMenu::IsInside(int x, int y)
{
	return CUIBaseWindow::IsInside(x, y);
}

LRESULT CBattleRankMenu::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	m_btRequest.MsgProc(hWnd, msg, wParam, lParam);
	m_btWatch.MsgProc(hWnd, msg, wParam, lParam);
	m_btTakeItem.MsgProc(hWnd, msg, wParam, lParam);
	m_btCancel.MsgProc(hWnd, msg, wParam, lParam);

	switch( msg )
	{
	case WM_LBUTTONDOWN :

		if(!IsInside(LOWORD(lParam), HIWORD(lParam)))
			return IM_NONE;

		if( m_btRequest.GetState() == BTN_DOWN )
			return IM_INPUT;

		if( m_btWatch.GetState() == BTN_DOWN )
			return IM_INPUT;

		if( m_btTakeItem.GetState() == BTN_DOWN )
			return IM_INPUT;

		if( m_btCancel.GetState() == BTN_DOWN )
			return IM_INPUT;

		break;

	case WM_LBUTTONUP :

		if(!IsInside(LOWORD(lParam), HIWORD(lParam)))
			return IM_NONE;

		if(m_btRequest.GetState() == BTN_ACTION)
		{
			int MainType = g_pNk2DFrame->GetGuildSystemManager()->GetMainType();
			sprintf(m_MsgBuf, "mg rankwar open %d\n", MainType);
			SendMsg();

			switch(MainType)
			{
			case 0:
				g_pNk2DFrame->GetUIWindow()->OnOpenWindow(WINDOW_GUILD_BATTLE_1VS1);
				return IM_INPUT;
			case 1:
				//g_pNk2DFrame->GetUIWindow()->OnOpenWindow(WINDOW_GUILD_BATTLE_FLAG);

				//아마 수정
				//_pNk2DFrame->GetGuildSystemManager()->SetMainType(RANK_BATTLE_FLAG);
				return IM_INPUT;
			case 2:
				return IM_INPUT;
			default:
				return IM_INPUT;
			}
		}

		if(m_btWatch.GetState() == BTN_ACTION)
		{
			// IDS_CANT_USE_ITEM_NOW 현재는 사용하실 수 없습니다.
			g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_CANT_USE_ITEM_NOW), TYPE_GUILD_CONFIRM );

			///패킷 요망
//			sprintf(m_MsgBuf, "mg rankwar invite\n");
//			SendMsg();

			return IM_INPUT;
		}


		if(m_btTakeItem.GetState() == BTN_ACTION)
		{
			//아이템 지급
			sprintf(m_MsgBuf, "mg rankwar get\n");
			SendMsg();

			return IM_INPUT;
		}

		if( m_btCancel.GetState() == BTN_ACTION )
		{
			g_pNk2DFrame->GetUIWindow()->OnOpenWindow(WINDOW_GUILD_BATTLE_KIND);
			return IM_INPUT;
		}
		break;
	}

	return CUIBaseWindow::MsgProc(hWnd, msg, wParam, lParam);
}

/****************************************************************************************************/

CBattleRank::CBattleRank()
	: CUIBaseWindow()
	, m_nSlotNum(0)
{
	ZeroMemory(m_MsgBuf, sizeof(m_MsgBuf));

	m_nSubType = -1;


}

CBattleRank::~CBattleRank()
{
	DeleteRes();
}


BOOL CBattleRank::InitMode()
{
	int i;
	for(i = 0; i < BT_TOTAL; i++)
		m_btJoin[i].SetFileName("guild/join");

	for(i = 0; i < BT_TOTAL; i++)
		m_btJoinCancel[i].SetFileName("guild/join_cancel");

	m_btCancel.SetFileName("guild/msg_no");

	return TRUE;
}


void CBattleRank::LoadRes()
{
	int nScreenWidth  = 0;
	int nScreenHeight = 0;

	if( g_pNk2DFrame )
	{
		nScreenWidth  = g_pNk2DFrame->GetClientWidth();
		nScreenHeight = g_pNk2DFrame->GetClientHeight();
	}

	SetBackground(WBK_RANKSYSTEM);
	MakeCloseBtn("guild/exit");

	if(m_pBackScene)
	{
		int n_x = (nScreenWidth  / 2) - (m_pBackScene->GetWidth()  / 2);
		int n_y = (nScreenHeight / 2) - (m_pBackScene->GetHeight() / 2);

		m_nPosX = n_x;
		m_nPosY = n_y;
		int i = 0;
		for(i = 0; i < BT_TOTAL; i++)
			m_btJoin[i].LoadRes();

		for(i = 0; i < BT_TOTAL; i++)
			m_btJoinCancel[i].LoadRes();

		for(i = 0; i <BT_TOTAL; i++)
		{
			m_text[i].Init(14,  RGB(255, 255, 255), m_nPosX + 183 , m_nPosY + 89 + (41*i), 30, 21, TRUE);
		}


		m_btCancel.LoadRes();
		SetPosition();

	}
}

void CBattleRank::SetPosition()
{
	for(int i = 0, nHeight = 0; i < BT_TOTAL; i++, nHeight += 41)
	{
		m_btJoin[i].SetPosition(m_nPosX + 313, m_nPosY + 84 + nHeight);
		m_btJoinCancel[i].SetPosition(m_nPosX + 313, m_nPosY + 84 + nHeight);
		m_text[i].SetPos_Center(m_nPosX + 183 , m_nPosY + 89 + (44*i));
	}

	m_btCancel.SetPosition( m_nPosX + 100 ,
							m_nPosY + m_pBackScene->GetHeight() - m_btCancel.Height - 15 );
	m_pBtExit->SetPosition( m_nPosX + m_pBackScene->GetWidth() -  m_pBtExit->Width  - 100 ,
							m_nPosY + m_pBackScene->GetHeight() - m_pBtExit->Height - 15 );

}

void CBattleRank::DeleteRes()
{
	for(int i = 0; i < BT_TOTAL; i++)
	{
		m_btJoin[i].DeleteRes();
		m_text[i].DeleteRes();
		m_btJoinCancel[i].DeleteRes();
	}

	m_btCancel.DeleteRes();
}

void CBattleRank::RestoreSurfaces()
{

}

int CBattleRank::Draw()
{
	if( g_pDisplay && m_pBackScene)
	{
		if(CUIBaseWindow::Draw())
		{
			for(int i = 0; i < BT_TOTAL; i++)
			{
				if (m_nSlotNum == i)
				{
					m_btJoinCancel[i].Draw();
				}
				else
				{
					m_btJoin[i].Draw();
				}
				m_text[i].Draw();
			}

			m_btCancel.Draw();
		}
	}
	return -1;
}

void CBattleRank::SetText(int nNum, char* szNum)
{
	/*	///071102 길드랭킹전 패킷 받기 버그 수정.
		for(int i = 0; i < BT_TOTAL; i++)
		{
			m_text[i].SetString_Center_Arrange("0", RGB(0, 255, 64), RGB(255, 255, 255), TRUE);
		}
	*/

	if(strlen(szNum))
		m_text[nNum].SetString_Center_Arrange(szNum, RGB(0, 255, 64), RGB(255, 255, 255), TRUE);
}

void CBattleRank::SetSlotNum(int nSlotNum)
{
	m_nSlotNum = nSlotNum;
}

BOOL CBattleRank::MoveWindow(UINT msg, int x, int y)
{
	return CUIBaseWindow::MoveWindow(msg, x, y);
}


BOOL CBattleRank::IsInside(int x, int y)
{
	return CUIBaseWindow::IsInside(x, y);
}

void CBattleRank::SendMsg()
{
	g_pTcpIp->SendNetMessage(m_MsgBuf);
}

LRESULT CBattleRank::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	for(int i = 0 ; i < BT_TOTAL; i++)
	{
//		if (i != m_nSlotNum)
		{
			m_btJoin[i].MsgProc(hWnd, msg, wParam, lParam);
			m_btJoinCancel[i].MsgProc(hWnd, msg, wParam, lParam);
		}
	}

	m_btCancel.MsgProc(hWnd, msg, wParam, lParam);

	switch( msg )
	{
	case WM_LBUTTONDOWN:

		if(!IsInside(LOWORD(lParam), HIWORD(lParam)))
			return IM_NONE;

		if( m_nSlotNum == BT_5_10 )
		{
			if(m_btJoinCancel[BT_5_10].GetState() == BTN_DOWN)		return IM_INPUT;
		}
		else
		{
			if(m_btJoin[BT_5_10].GetState() == BTN_DOWN)			return IM_INPUT;
		}

		if( m_nSlotNum == BT_11_20 )
		{
			if(m_btJoinCancel[BT_11_20].GetState() == BTN_DOWN)		return IM_INPUT;
		}
		else
		{
			if(m_btJoin[BT_11_20].GetState() == BTN_DOWN)			return IM_INPUT;
		}

		if( m_nSlotNum == BT_21_30 )
		{
			if(m_btJoinCancel[BT_21_30].GetState() == BTN_DOWN)		return IM_INPUT;
		}
		else
		{
			if(m_btJoin[BT_21_30].GetState() == BTN_DOWN)			return IM_INPUT;
		}

		if( m_nSlotNum == BT_31_40 )
		{
			if(m_btJoinCancel[BT_31_40].GetState() == BTN_DOWN)		return IM_INPUT;
		}
		else
		{
			if(m_btJoin[BT_31_40].GetState() == BTN_DOWN)			return IM_INPUT;
		}

		if( m_nSlotNum == BT_UNLIMITED )
		{
			if(m_btJoinCancel[BT_UNLIMITED].GetState() == BTN_DOWN)	return IM_INPUT;
		}
		else
		{
			if(m_btJoin[BT_UNLIMITED].GetState() == BTN_DOWN)		return IM_INPUT;
		}

		if(m_btCancel.GetState() == BTN_DOWN )
			return IM_INPUT;

		break;

	case WM_LBUTTONUP :

		if(!IsInside(LOWORD(lParam), HIWORD(lParam)))
			return IM_NONE;

		if( m_btCancel.GetState() == BTN_ACTION)
		{
			g_pNk2DFrame->GetUIWindow()->OnOpenWindow(WINDOW_GUILD_BATTLE_RANKING_MENU);
			return IM_INPUT;
		}

		if (m_nSlotNum != -1)
		{
			if(m_btJoinCancel[BT_5_10].GetState() == BTN_ACTION)
			{
				if (m_nSlotNum == BT_5_10)
				{
					sprintf(m_MsgBuf, "mg rankwar cancel\n");
					SendMsg();
					m_nSlotNum = -1;
					return IM_CLOSE;
				}
				else
				{
					//IDS_GUILD_JOIN_DONT_REQ "이미 신청중입니다."
					g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_JOIN_DONT_REQ), TYPE_GUILD_CONFIRM, 55);
					return IM_CLOSE;
				}
			}

			if(m_btJoinCancel[BT_11_20].GetState() == BTN_ACTION)
			{
				if (m_nSlotNum == BT_11_20)
				{
					sprintf(m_MsgBuf, "mg rankwar cancel\n");
					SendMsg();
					m_nSlotNum = -1;
					return IM_CLOSE;
				}
				else
				{
					//IDS_GUILD_JOIN_DONT_REQ "이미 신청중입니다."
					g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_JOIN_DONT_REQ), TYPE_GUILD_CONFIRM, 55);
					return IM_CLOSE;
				}
			}

			if(m_btJoinCancel[BT_21_30].GetState() == BTN_ACTION)
			{
				if (m_nSlotNum == BT_21_30)
				{
					sprintf(m_MsgBuf, "mg rankwar cancel\n");
					SendMsg();
					m_nSlotNum = -1;
					return IM_CLOSE;
				}
				else
				{
					//IDS_GUILD_JOIN_DONT_REQ "이미 신청중입니다."
					g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_JOIN_DONT_REQ), TYPE_GUILD_CONFIRM, 55);
					return IM_CLOSE;
				}
			}

			if(m_btJoinCancel[BT_31_40].GetState() == BTN_ACTION)
			{
				if (m_nSlotNum == BT_31_40)
				{
					sprintf(m_MsgBuf, "mg rankwar cancel\n");
					SendMsg();
					m_nSlotNum = -1;
					return IM_CLOSE;
				}
				else
				{
					//IDS_GUILD_JOIN_DONT_REQ "이미 신청중입니다."
					g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_JOIN_DONT_REQ), TYPE_GUILD_CONFIRM, 55);
					return IM_CLOSE;

				}
			}

			if(m_btJoinCancel[BT_UNLIMITED].GetState() == BTN_ACTION)
			{
				if (m_nSlotNum == BT_UNLIMITED)
				{
					sprintf(m_MsgBuf, "mg rankwar cancel\n");
					SendMsg();
					m_nSlotNum = -1;
					return IM_CLOSE;
				}
				else
				{
					//IDS_GUILD_JOIN_DONT_REQ "이미 신청중입니다."
					g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_JOIN_DONT_REQ), TYPE_GUILD_CONFIRM, 55);
					return IM_CLOSE;
				}
			}
		}
		else
		{
			if(m_btJoin[BT_5_10].GetState() == BTN_ACTION)
			{
				sprintf(m_MsgBuf, "mg rankwar req\n");
				SendMsg();

				m_nSubType = BT_5_10;

				g_pNk2DFrame->GetGuildSystemManager()->SetSubType(m_nSubType);

				return IM_INPUT;
			}

			if(m_btJoin[BT_11_20].GetState() == BTN_ACTION)
			{
				sprintf(m_MsgBuf, "mg rankwar req\n");
				SendMsg();

				m_nSubType = BT_11_20;

				g_pNk2DFrame->GetGuildSystemManager()->SetSubType(m_nSubType);

				return IM_INPUT;
			}

			if(m_btJoin[BT_21_30].GetState() == BTN_ACTION)
			{
				sprintf(m_MsgBuf, "mg rankwar req\n");
				SendMsg();

				m_nSubType = BT_21_30;

				g_pNk2DFrame->GetGuildSystemManager()->SetSubType(m_nSubType);

				return IM_INPUT;
			}

			if(m_btJoin[BT_31_40].GetState() == BTN_ACTION)
			{
				sprintf(m_MsgBuf, "mg rankwar req\n");
				SendMsg();

				m_nSubType = BT_31_40;

				g_pNk2DFrame->GetGuildSystemManager()->SetSubType(m_nSubType);

				return IM_INPUT;
			}

			if(m_btJoin[BT_UNLIMITED].GetState() == BTN_ACTION)
			{
				sprintf(m_MsgBuf, "mg rankwar req\n");
				SendMsg();

				m_nSubType = BT_UNLIMITED;

				g_pNk2DFrame->GetGuildSystemManager()->SetSubType(m_nSubType);

				return IM_INPUT;
			}
		}
		break;
	}

	return CUIBaseWindow::MsgProc(hWnd, msg, wParam, lParam);
}

/********************************************************************************************************************/

#define CLASS_BATTLE_LIST				CBattleAcceptMember::CDataList
#define BATTLE_LIST_LINE_POSY_TOP		(m_nBaseY + 68)
#define BATTLE_LIST_LINE_POSY_BOTTOM	(m_nBaseY + 280)

CBattleAcceptMember::CBattleAcceptMember() : CUIBaseWindow()
{
	m_listMember.clear();
	m_listComein.clear();

	m_nMemberCnt = 0;
	m_nComeinCnt = 0;

	m_nMemberSelect = -1;
	m_nComeInSelect = -1;

	m_nSubType = -1;
	ZeroMemory(m_MsgBuf, sizeof(m_MsgBuf));

	for(int i = 0; i < BATTLE_NUMBER_TOTAL; i++)
	{
		m_pSelectSur[i] = NULL;
	}
}

CBattleAcceptMember::~CBattleAcceptMember()
{
	DeleteRes();

	list<CDataList*>::iterator _iter_;

	for(_iter_ = m_listMember.begin(); _iter_ != m_listMember.end(); _iter_++)
	{
		CDataList* pTemp = *_iter_;

		SAFE_DELETE(pTemp);
	}

	for(_iter_ = m_listComein.begin(); _iter_ != m_listComein.end(); _iter_++)
	{
		CDataList* pTemp = *_iter_;

		SAFE_DELETE(pTemp);
	}

	m_listMember.clear();
	m_listComein.clear();

	for(int i = 0; i < BATTLE_NUMBER_TOTAL; i++)
	{
		SAFE_DELETE(m_pSelectSur[i]);
	}

}


BOOL CBattleAcceptMember::InitMode()
{
	m_btOk.SetFileName("guild/msg_ok");
	//m_btCancel.SetFileName("guild/msg_no");

	m_btLeftArrow.SetFileName("guild/battle_left");
	m_btRightArrow.SetFileName("guild/battle_right");
	m_btCancel.SetFileName("guild/msg_no");

	return TRUE;
}

void CBattleAcceptMember::LoadRes()
{
	int nScreenWidth  = 0;
	int nScreenHeight = 0;

	if( g_pNk2DFrame )
	{
		nScreenWidth  = g_pNk2DFrame->GetClientWidth();
		nScreenHeight = g_pNk2DFrame->GetClientHeight();
	}

	for(int i = 0; i < BATTLE_NUMBER_TOTAL; i++)
	{
		SAFE_DELETE(m_pSelectSur[i]);
	}

	SetBackground(WBK_RANKPARICIPATE);
	MakeCloseBtn("guild/exit");

	if(m_pBackScene)
	{
		int n_x = (nScreenWidth  / 2) - (m_pBackScene->GetWidth()  / 2);
		int n_y = (nScreenHeight / 2) - (m_pBackScene->GetHeight() / 2);

		m_nPosX = n_x;
		m_nPosY = n_y;

		g_pDisplay->CreateSurfaceFromBitmap(&m_pSelectSur[BATTLE_NUMBER_5_10 ]	  , "interface/guild/guild_battle/battle_5_10p.bmp");
		g_pDisplay->CreateSurfaceFromBitmap(&m_pSelectSur[BATTLE_NUMBER_11_20]	  , "interface/guild/guild_battle/battle_11_20p.bmp");
		g_pDisplay->CreateSurfaceFromBitmap(&m_pSelectSur[BATTLE_NUMBER_21_30]	  , "interface/guild/guild_battle/battle_21_30p.bmp");
		g_pDisplay->CreateSurfaceFromBitmap(&m_pSelectSur[BATTLE_NUMBER_31_40]	  , "interface/guild/guild_battle/battle_31_40p.bmp");
		g_pDisplay->CreateSurfaceFromBitmap(&m_pSelectSur[BATTLE_NUMBER_UNLIMITED], "interface/guild/guild_battle/battle_unlimited.bmp");

		for(int i = 0; i < BATTLE_NUMBER_TOTAL; i++)
		{
			m_pSelectSur[i]->Xpos = m_nPosX + 256;
			m_pSelectSur[i]->Ypos = m_nPosY + 45;
		}

		m_btOk.LoadRes();

		m_btLeftArrow.LoadRes();
		m_btRightArrow.LoadRes();
		m_btCancel.LoadRes();

		m_ScrollMember.LoadRes("guild/scr_up", "guild/scr_down", "interface/guild/scr_bar.bmp");
		m_ScrollMember.SetRange(1);

		m_ScrollComein.LoadRes("guild/scr_up", "guild/scr_down", "interface/guild/scr_bar.bmp");
		m_ScrollComein.SetRange(1);

		SetPosition();

		Insert_list_Member();

	}
}


void CBattleAcceptMember::SetPosition()
{
	m_btLeftArrow.SetPosition(m_nPosX + 200, m_nPosY + 126);
	m_btRightArrow.SetPosition(m_nPosX + 200, m_nPosY + 180);

	m_ScrollMember.SetDrawPos(m_nPosX+173, m_nPosY+56, 226);
	m_ScrollComein.SetDrawPos(m_nPosX+408, m_nPosY+56, 226);

	int i = 0;
	for(i = 0; i < BATTLE_NUMBER_TOTAL; i++)
	{
		m_pSelectSur[i]->Xpos = m_nPosX + 257;
		m_pSelectSur[i]->Ypos = m_nPosY + 44;
	}

	list<CLASS_BATTLE_LIST*>::iterator _iter;

	i = 0;

	for(_iter = m_listMember.begin(); _iter != m_listMember.end(); _iter++, i++)
	{
		CLASS_BATTLE_LIST* pTemp = *_iter;

		if(pTemp)
			pTemp->SetRePosition(m_nPosX, m_nPosY, 27, 70, i);
	}

	i = 0;
	for(_iter = m_listComein.begin(); _iter != m_listComein.end(); _iter++, i++)
	{
		CLASS_BATTLE_LIST* pTemp = *_iter;

		if(pTemp)
			pTemp->SetRePosition(m_nPosX, m_nPosY, 263, 70, i);
	}

	//SetExitBtnPostion(236, 295);

	m_btOk.SetPosition(		m_nPosX + 60,
							m_nPosY + m_pBackScene->GetHeight()     - m_pBtExit->Height - 20 );
	m_btCancel.SetPosition( m_nPosX + m_pBackScene->GetWidth()  / 2 - m_btCancel.Width  / 2 ,
							m_nPosY + m_pBackScene->GetHeight()     - m_btCancel.Height - 20 );
	m_pBtExit->SetPosition( m_nPosX + m_pBackScene->GetWidth()      - m_pBtExit->Width  - 60 ,
							m_nPosY + m_pBackScene->GetHeight()     - m_pBtExit->Height - 20 );

}


void CBattleAcceptMember::Insert_list_Member()
{
	if(!g_pRoh)	return;

	CGuildData* pData = g_pRoh->GetMyGuildData();

	if(pData == NULL)	return;

	list_guildmember::iterator _iter = pData->GetMemberInfo().begin();


	for(; _iter != pData->GetMemberInfo().end(); _iter++)
	{
		Insert_list_Member(*_iter);
	}

}

void CBattleAcceptMember::Insert_list_Member(guild_member& g_member)
{
	if(g_member.s_glevel == -1 || g_member.s_glevel == 0 /* || g_member.s_glevel == JOB_MASTER*/)
		return;

	if( g_member.s_connect == -1) // offline 상태이면 리스트에 넣지 말자.
		return;

	CLASS_BATTLE_LIST* pTemp  = CLASS_BATTLE_LIST::Create(m_nPosX, m_nPosY);

	pTemp->Init();
	pTemp->LoadRes();
	pTemp->SetPosition(27, 70, m_nMemberCnt);

	pTemp->SetConnect( g_member.s_connect );

	if( g_member.s_connect == -1 )
	{
		pTemp->SetCharName(g_member.s_name, RGB(125,125,125));
	}
	else
	{
		pTemp->SetCharName(g_member.s_name, RGB(0, 255, 64) );
	}

	pTemp->SetCharIdx(g_member.s_index);

	m_ScrollMember.SetRange(m_nMemberCnt*28);

	m_nMemberCnt++;

	m_listMember.push_back(pTemp);
}

void CBattleAcceptMember::Insert_list_Comein(guild_member& g_member)
{
	if(g_member.s_glevel == -1 || g_member.s_glevel == 0 || g_member.s_glevel == JOB_MASTER)
		return;

	CLASS_BATTLE_LIST* pTemp  = CLASS_BATTLE_LIST::Create(m_nPosX, m_nPosY);

	pTemp->Init();
	pTemp->LoadRes();
	pTemp->SetPosition(263, 70, m_nComeinCnt);

	pTemp->SetConnect( g_member.s_connect );

	if( g_member.s_connect == -1)
	{
		pTemp->SetCharName(g_member.s_name, RGB(125,125,125));
	}
	else
	{
		pTemp->SetCharName(g_member.s_name, RGB(0, 255, 64));
	}

	pTemp->SetCharIdx(g_member.s_index);

	m_ScrollComein.SetRange(m_nComeinCnt*28);

	m_nComeinCnt++;

	m_listComein.push_back(pTemp);
}

BOOL CBattleAcceptMember::Exchage_Right_Left(int nSelect)
{
	if(nSelect < 0)	return FALSE;

	list<CLASS_BATTLE_LIST*>::iterator _iter;

	for(_iter = m_listComein.begin(); _iter != m_listComein.end(); _iter++)
	{
		CLASS_BATTLE_LIST *pTemp = *_iter;

		if(pTemp->m_nSequence == nSelect)
		{
			guild_member ComeIn;

			ComeIn.s_index  = pTemp->m_nCharidx;
			ComeIn.s_glevel = 1;
			ComeIn.s_connect = pTemp->m_Connect;
			strcpy(ComeIn.s_name, pTemp->m_TCharName.m_str);

			Insert_list_Member(ComeIn);

			m_nComeInSelect = -1;

			SAFE_DELETE(pTemp);
			m_listComein.erase(_iter);

			return TRUE;
		}
	}

	return FALSE;
}



BOOL CBattleAcceptMember::Exchage_Left_Right(int nSelect)
{
	if(nSelect < 0)	return FALSE;

	//if(!IsSelectLimited())	return FALSE;

	list<CLASS_BATTLE_LIST*>::iterator _iter;

	for(_iter = m_listMember.begin(); _iter != m_listMember.end(); _iter++)
	{
		CLASS_BATTLE_LIST *pTemp = *_iter;

		if(pTemp->m_nSequence == nSelect)
		{
			guild_member ComeIn;

			ComeIn.s_index  = pTemp->m_nCharidx;
			ComeIn.s_glevel = 1;
			ComeIn.s_connect = pTemp->m_Connect;
			strcpy(ComeIn.s_name, pTemp->m_TCharName.m_str);

			Insert_list_Comein(ComeIn);

			m_nMemberSelect = -1;

			SAFE_DELETE(pTemp);
			m_listMember.erase(_iter);

			return TRUE;
		}
	}

	return FALSE;
}

void CBattleAcceptMember::list_Member_sort()
{
	list<CLASS_BATTLE_LIST*>::iterator _iter;

	m_nMemberCnt = 0;

	for(_iter = m_listMember.begin(); _iter != m_listMember.end(); _iter++, m_nMemberCnt++)
	{
		CLASS_BATTLE_LIST* pTemp = *_iter;

		if(pTemp)
		{
			pTemp->SetRePosition(m_nPosX, m_nPosY, 27, 70, m_nMemberCnt);

		}
	}

}

void CBattleAcceptMember::list_ComeIn_sort()
{
	list<CLASS_BATTLE_LIST*>::iterator _iter;

	m_nComeinCnt = 0;

	for(_iter = m_listComein.begin(); _iter != m_listComein.end(); _iter++, m_nComeinCnt++)
	{
		CLASS_BATTLE_LIST* pTemp = *_iter;

		if(pTemp)
		{
			pTemp->SetRePosition(m_nPosX, m_nPosY, 263, 70, m_nComeinCnt);

		}
	}
}

BOOL CBattleAcceptMember::IsSelectLimited()
{
	if(m_nSubType < 0) return FALSE;

	BOOL result = TRUE;
	switch(m_nSubType)
	{
	case 0:	// 5  ~ 10
		if(m_nComeinCnt < 5 || m_nComeinCnt > 10)
			result = FALSE;
		break;
	case 1:	// 11 ~ 20
		if(m_nComeinCnt < 11 || m_nComeinCnt > 20)
			result = FALSE;
		break;
	case 2: // 21 ~ 30
		if(m_nComeinCnt < 21 || m_nComeinCnt > 30)
			result = FALSE;
		break;
	case 3: // 31 ~ 40
		if(m_nComeinCnt < 31 || m_nComeinCnt > 40)
			result = FALSE;
		break;
	case 4: // unlimited
		{

		}
		break;
	}

	if( result == FALSE)
	{
		// IDS_GUILD_JOIN_DONT_MEMBER_COUNT "참가인원이 맞지 않습니다."
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_JOIN_DONT_MEMBER_COUNT), TYPE_GUILD_CONFIRM, 42);
	}

	return result;
}

void CBattleAcceptMember::DeleteRes()
{
	m_btOk.DeleteRes();
	m_btLeftArrow.DeleteRes();
	m_btRightArrow.DeleteRes();
	m_btCancel.DeleteRes();

	list<CLASS_BATTLE_LIST*>::iterator _iter;

	for(_iter = m_listMember.begin(); _iter != m_listMember.end(); _iter++)
	{
		CLASS_BATTLE_LIST *pTemp = *_iter;

		if(pTemp)
			pTemp->DeleteRes();
	}

	for(_iter = m_listComein.begin(); _iter != m_listComein.end(); _iter++)
	{
		CLASS_BATTLE_LIST *pTemp = *_iter;

		if(pTemp)
			pTemp->DeleteRes();
	}

	m_ScrollMember.DeleteRes();
	m_ScrollComein.DeleteRes();
}

void CBattleAcceptMember::RestoreSurfaces()
{

}

int CBattleAcceptMember::Draw()
{
	if( g_pDisplay && m_pBackScene)
	{
		if(CUIBaseWindow::Draw())
		{
			m_btOk.Draw();
			m_btLeftArrow.Draw();
			m_btRightArrow.Draw();
			m_btCancel.Draw();

			list<CLASS_BATTLE_LIST*>::iterator _iter;

			for(_iter = m_listMember.begin(); _iter != m_listMember.end(); _iter++)
			{
				CLASS_BATTLE_LIST* pTemp = *_iter;

				if(pTemp)
					pTemp->Draw(m_ScrollMember.GetPos());
			}

			m_ScrollMember.Draw();

			for(_iter = m_listComein.begin(); _iter != m_listComein.end(); _iter++)
			{
				CLASS_BATTLE_LIST* pTemp = *_iter;

				if(pTemp)
					pTemp->Draw(m_ScrollComein.GetPos());
			}

			if(m_nSubType >= 0 && m_pSelectSur[m_nSubType])
				g_pDisplay->Blt(m_pSelectSur[m_nSubType]->Xpos, m_pSelectSur[m_nSubType]->Ypos, m_pSelectSur[m_nSubType]);


			m_ScrollComein.Draw();
		}
	}
	return -1;
}

BOOL CBattleAcceptMember::MoveWindow(UINT msg, int x, int y)
{
	return CUIBaseWindow::MoveWindow(msg, x, y);
}



BOOL CBattleAcceptMember::IsInside(int x, int y)
{
	return CUIBaseWindow::IsInside(x, y);
}

void CBattleAcceptMember::SetChangeColor(int nLeft, int nRight)
{
	list<CLASS_BATTLE_LIST*>::iterator _iter;

	for(_iter = m_listMember.begin(); _iter != m_listMember.end(); _iter++)
	{
		CLASS_BATTLE_LIST *pTemp = *_iter;

		if(pTemp->m_nSequence == nLeft )
		{
			if( pTemp->m_Connect == -1 )
			{
				pTemp->m_TCharName.ChangeColor(RGB(200,0,200));
			}
			else
			{
				pTemp->m_TCharName.ChangeColor(RGB(255,0,255));
			}

		}
		else
		{
			if( pTemp->m_Connect == -1 )
			{
				pTemp->m_TCharName.ChangeColor( RGB(125,125,125));
			}
			else
			{
				pTemp->m_TCharName.ChangeColor( RGB(0, 255, 64) );
			}
		}
	}

	for(_iter = m_listComein.begin(); _iter != m_listComein.end(); _iter++)
	{
		CLASS_BATTLE_LIST *pTemp = *_iter;

		if(pTemp->m_nSequence == nRight)
		{
			if( pTemp->m_Connect == -1 )
			{
				pTemp->m_TCharName.ChangeColor(RGB(200,0,200));
			}
			else
			{
				pTemp->m_TCharName.ChangeColor(RGB(255,0,255));
			}
		}
		else
		{
			if( pTemp->m_Connect == -1)
			{
				pTemp->m_TCharName.ChangeColor( RGB(125,125,125) );
			}
			else
			{
				pTemp->m_TCharName.ChangeColor( RGB(0, 255, 64) );
			}
		}
	}
}

void CBattleAcceptMember::SendMsg()
{
	if(m_listComein.empty())	return;

	char szTmp[512];
	char szOri[512];

	ZeroMemory(szTmp, sizeof(szTmp));
	ZeroMemory(szOri, sizeof(szOri));

	list<CLASS_BATTLE_LIST*>::iterator _iter;

	for(_iter = m_listComein.begin(); _iter != m_listComein.end(); _iter++)
	{
		CLASS_BATTLE_LIST *pTemp = *_iter;

		sprintf(szTmp, "%d ", pTemp->m_nCharidx);
		strcat(szOri, szTmp);
	}
	int nMainType	= g_pNk2DFrame->GetGuildSystemManager()->GetMainType();
	int nSubType	= g_pNk2DFrame->GetGuildSystemManager()->GetSubType();
	sprintf(m_MsgBuf, "mg rankwar list %d %d %d %s\n", nMainType, nSubType, m_nComeinCnt, szOri);

	g_pTcpIp->SendNetMessage(m_MsgBuf);
}

LRESULT CBattleAcceptMember::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	m_btOk.MsgProc(hWnd, msg, wParam, lParam);
	m_btLeftArrow.MsgProc(hWnd, msg, wParam, lParam);
	m_btRightArrow.MsgProc(hWnd, msg, wParam, lParam);
	m_btCancel.MsgProc(hWnd, msg, wParam, lParam);

	switch( msg )
	{
	case WM_LBUTTONDOWN:

		if(!IsInside(LOWORD(lParam), HIWORD(lParam)))
			return IM_NONE;

		if( m_ScrollMember.MsgProc(hWnd, msg, wParam, lParam)  == S_OK)
			return IM_INPUT;

		if( m_ScrollComein.MsgProc(hWnd, msg, wParam, lParam) == S_OK )
			return IM_INPUT;

		if( m_btOk.GetState() == BTN_DOWN )
			return IM_INPUT;

		if( m_btLeftArrow.GetState() == BTN_DOWN )
			return IM_INPUT;

		if( m_btRightArrow.GetState() == BTN_DOWN )
			return IM_INPUT;

		if( m_btCancel.GetState() == BTN_DOWN )
			return IM_INPUT;

		break;
	case WM_MOUSEMOVE:

		if(!IsInside(LOWORD(lParam), HIWORD(lParam)))
			return IM_NONE;

		if( m_ScrollMember.MsgProc(hWnd, msg, wParam, lParam) == S_OK)
			return IM_INPUT;

		if( m_ScrollComein.MsgProc(hWnd, msg, wParam, lParam) == S_OK)
			return IM_INPUT;

		break;

	case WM_LBUTTONUP :
		{
			if(!IsInside(LOWORD(lParam), HIWORD(lParam)))
				return IM_NONE;

			if( m_ScrollMember.MsgProc(hWnd, msg, wParam, lParam) == S_OK)
				return IM_INPUT;

			if( m_ScrollComein.MsgProc(hWnd, msg, wParam, lParam) == S_OK)
				return IM_INPUT;

			/**************************************************************************/
			list<CLASS_BATTLE_LIST*>::iterator _iter;

			int	member_select = -1;
			int comein_select = -1;

			for(_iter = m_listMember.begin(); _iter != m_listMember.end(); _iter++)
			{
				CLASS_BATTLE_LIST *pTemp = *_iter;

				if(pTemp && (pTemp->MsgProc(hWnd, msg, wParam, lParam, m_ScrollMember.GetPos()) != -1) )
				{
					member_select = pTemp->m_nSequence;

					m_nMemberSelect = pTemp->m_nSequence;
				}
			}

			for(_iter = m_listComein.begin(); _iter != m_listComein.end(); _iter++)
			{
				CLASS_BATTLE_LIST *pTemp = *_iter;

				if(pTemp && (pTemp->MsgProc(hWnd, msg, wParam, lParam, m_ScrollComein.GetPos()) != -1) )
				{
					comein_select = pTemp->m_nSequence;

					m_nComeInSelect = pTemp->m_nSequence;
				}
			}

			if(member_select != -1 || comein_select != -1)
				SetChangeColor(member_select, comein_select);

			if(m_btOk.GetState() == BTN_ACTION)
			{
				//if(IsSelectLimited())
				// IDS_GUILD_SEND_JOIN_MSG "참가 요청 메시지를 보내겠습니까?"
				g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_SEND_JOIN_MSG), TYPE_GUILD_CONFIRM_CANCEL, 51);
			}

			if(m_btLeftArrow.GetState() == BTN_ACTION)
			{
				if(Exchage_Right_Left(m_nComeInSelect))
					list_ComeIn_sort();

				return IM_INPUT;
			}

			if(m_btRightArrow.GetState() == BTN_ACTION)
			{
				if(Exchage_Left_Right(m_nMemberSelect))
					list_Member_sort();

				return IM_INPUT;
			}

			if( m_btCancel.GetState() == BTN_ACTION)
			{
				g_pNk2DFrame->GetUIWindow()->OnOpenWindow(WINDOW_GUILD_BATTLE_RANKING_MENU);
				return IM_INPUT;
			}
		}
		break;
	}

	return CUIBaseWindow::MsgProc(hWnd, msg, wParam, lParam);
}


/*****************************************************************************************************************/
CLASS_BATTLE_LIST*	CLASS_BATTLE_LIST::Create(int nBaseX, int nBaseY)
{
	return new CDataList(nBaseX, nBaseY);
}

CLASS_BATTLE_LIST::CDataList(int nBaseX, int nBaseY)
{
	m_hWnd		= NULL;

	m_nBaseX = nBaseX;
	m_nBaseY = nBaseY;

	m_nSequence		= 0;
	m_Connect		= -1;

	ZeroMemory(m_MsgBuf, sizeof(m_MsgBuf));
}

CLASS_BATTLE_LIST::~CDataList()
{
	DeleteRes();

}

void CLASS_BATTLE_LIST::Init()
{
}

void CLASS_BATTLE_LIST::LoadRes()
{
	if(!g_pDisplay)	return;

	m_hWnd =  g_pDisplay->GetHWnd();
	GetClientRect(m_hWnd, &m_ClientRc);
}

void CLASS_BATTLE_LIST::Draw(int nScrollPos)
{
	if(!g_pDisplay)	return;

	m_TCharName.DrawClipText(m_TCharName.m_StringPosY, BATTLE_LIST_LINE_POSY_TOP, BATTLE_LIST_LINE_POSY_BOTTOM, nScrollPos);

}

void CLASS_BATTLE_LIST::DeleteRes()
{
	m_TCharName.DeleteRes();
}

void CLASS_BATTLE_LIST::SetCharName(char* pName, COLORREF fontColor)
{
	m_TCharName.SetString_Center_Arrange(pName, fontColor, RGB(255, 255, 255), TRUE);
}

void CLASS_BATTLE_LIST::SetPosition(int nPosX, int nPosY, int nNum)
{
	m_hWnd =  g_pDisplay->GetHWnd();
	GetClientRect(m_hWnd, &m_ClientRc);

	m_nSequence = nNum;

	m_TCharName.Init(14, RGB(255, 255, 255), m_nBaseX + nPosX , m_nBaseY + nPosY + (24*nNum), 160, 20, TRUE);
}

void CLASS_BATTLE_LIST::SetRePosition(int nBaseX, int nBaseY, int nPosX, int nPosY, int nNum)
{
	m_hWnd =  g_pDisplay->GetHWnd();
	GetClientRect(m_hWnd, &m_ClientRc);

	m_nSequence = nNum;

	m_nBaseX = nBaseX;
	m_nBaseY = nBaseY;

	m_TCharName.SetPos_Center(m_nBaseX + nPosX, m_nBaseY + nPosY + (24*nNum));
}

void CLASS_BATTLE_LIST::Restore()
{

}

void CLASS_BATTLE_LIST::SendMsg()
{
	g_pTcpIp->SendNetMessage(m_MsgBuf);
}

BOOL CLASS_BATTLE_LIST::IsInside(int x, int y, int nScroll)
{
	if(m_TCharName.m_StringPosX < x && m_TCharName.m_StringPosX + m_TCharName.GetBoxWidth()  > x &&
			m_TCharName.m_StringPosY - nScroll < y && m_TCharName.m_StringPosY + m_TCharName.GetBoxHeight() -nScroll > y)
		return TRUE;

	return FALSE;
}

LRESULT CLASS_BATTLE_LIST::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, int nScroll)
{
	switch(msg)
	{
	case WM_LBUTTONUP :
		{
			if( IsInside( LOWORD(lParam), HIWORD(lParam), nScroll ) )
				return 3;
		}
		break;
	}
	return -1;
}

/*****************************************************************************************************************************/
// 침략 할 길드 선택 창
CBattleInvasionAttack::CBattleInvasionAttack()
	: CUIBaseWindow()
	, m_pInvasionInfo(NULL)
{
	ZeroMemory(m_MsgBuf, sizeof(m_MsgBuf));

	m_pTextSur	= NULL;
	m_pTyping	= NULL;
}

CBattleInvasionAttack::~CBattleInvasionAttack()
{
	DeleteRes();

	SAFE_DELETE(m_pTextSur);
	SAFE_DELETE(m_pTyping);

	m_TextGuildName.CloseIMC();
}


BOOL CBattleInvasionAttack::InitMode()
{
	m_btConfirm.SetFileName("guild/msg_ok");

	return TRUE;
}


void CBattleInvasionAttack::LoadRes()
{
	int nScreenWidth  = 0;
	int nScreenHeight = 0;

	if( g_pNk2DFrame )
	{
		nScreenWidth  = g_pNk2DFrame->GetClientWidth();
		nScreenHeight = g_pNk2DFrame->GetClientHeight();
	}

	SetBackground(WBK_BATTLE);
	MakeCloseBtn("guild/msg_no");

	SAFE_DELETE(m_pTextSur);
	SAFE_DELETE(m_pTyping );

	g_pDisplay->CreateSurfaceFromBitmap(&m_pTextSur, "interface/guild/guild_battle/select_period.bmp");
	g_pDisplay->CreateSurfaceFromBitmap(&m_pTyping , "interface/guild/guild_battle/battle_typing.bmp");
	g_pDisplay->CreateSurfaceFromBitmap(&m_pInvasionInfo, "interface/guild/guild_text/NPC_invasion.bmp");

	if(m_pBackScene)
	{

		int n_x = (nScreenWidth  / 2) - (m_pBackScene->GetWidth()  / 2);
		int n_y = (nScreenHeight / 2) - (m_pBackScene->GetHeight() / 2) - 30;

		if( n_y <= 0 )
			n_y = 1;

		m_nPosX = n_x;
		m_nPosY = n_y;

		if(m_pTextSur)
		{
			m_pTextSur->Xpos = m_nPosX + 36;
			m_pTextSur->Ypos = m_nPosY + 245;
		}

		if(m_pTyping)
		{
			m_pTyping->Xpos = m_nPosX + 132;
			m_pTyping->Ypos = m_nPosY + 244;
		}

		m_btConfirm.LoadRes();

		m_ComboBox.SetItemSize(75, 21, 4, 3);
		m_ComboBox.LoadRes("guild/down", "interface/guild/scr_box.bmp", "interface/guild/guild_battle/period.bmp");
		m_ComboBox.DeleteAllItem();

		m_ComboBox.InsertItemByImage("interface/guild/guild_battle/period_1");
		m_ComboBox.InsertItemByImage("interface/guild/guild_battle/period_2");
		m_ComboBox.InsertItemByImage("interface/guild/guild_battle/period_3");
		m_ComboBox.InsertItemByImage("interface/guild/guild_battle/period_4");

		m_TextGuildName.Init(m_nPosX, m_nPosY, 20, RGB(255,255,255), RGB(255, 179, 0), FALSE, MSG_POPUP);
		m_TextGuildName.SetfontSize(9);

		SetPosition();
	}

	if( g_pNk2DFrame && g_pNk2DFrame->GetControlBottom() )
		g_pNk2DFrame->GetControlBottom()->LostFocus();
	m_TextGuildName.SetEnable(TRUE);
}

void CBattleInvasionAttack::DeleteRes()
{
	m_btConfirm.DeleteRes();
	m_ComboBox.DeleteRes();
	m_TextGuildName.DeleteRes();
}

void CBattleInvasionAttack::RestoreSurfaces()
{

}

int CBattleInvasionAttack::Draw()
{
	if( g_pDisplay && m_pBackScene)
	{
		if(CUIBaseWindow::Draw())
		{
			if(m_pTyping )  g_pDisplay->Blt(m_pTyping->Xpos, m_pTyping->Ypos, m_pTyping);
			if(m_pTextSur)	g_pDisplay->Blt(m_pTextSur->Xpos, m_pTextSur->Ypos, m_pTextSur);
			if(m_pInvasionInfo) g_pDisplay->Blt(m_pInvasionInfo->Xpos, m_pInvasionInfo->Ypos, m_pInvasionInfo);

			m_btConfirm.Draw();


			m_TextGuildName.Draw();
			m_ComboBox.DrawImage();
		}
	}


	return -1;
}

void CBattleInvasionAttack::SetPosition()
{
	//m_btConfirm.SetPosition(	m_nPosX + 75,		m_nPosY + 388);
	m_TextGuildName.SetPos(		m_nPosX + 135,		m_nPosY + 250);
	m_ComboBox.SetPosition(		m_nPosX + 132,		m_nPosY + 274);
	m_ComboBox.BtnSetPosition(	m_nPosX + 132 + 78, m_nPosY + 274 + 3);

	m_btConfirm.SetPosition( m_nPosX + 60 ,
							 m_nPosY + m_pBackScene->GetHeight() - m_btConfirm.Height - 30 );
	m_pBtExit->SetPosition( m_nPosX + m_pBackScene->GetWidth() -  m_pBtExit->Width   - 60 ,
							m_nPosY + m_pBackScene->GetHeight() - m_pBtExit->Height  - 30 );

	if(m_pTextSur)
	{
		m_pTextSur->Xpos = m_nPosX + 36;
		m_pTextSur->Ypos = m_nPosY + 245;
	}

	if(m_pTyping)
	{
		m_pTyping->Xpos = m_nPosX + 132;
		m_pTyping->Ypos = m_nPosY + 244;
	}

	if(m_pInvasionInfo)
	{
		m_pInvasionInfo->Xpos = m_nPosX + 23;
		m_pInvasionInfo->Ypos = m_nPosY + 51;
	}
}

void CBattleInvasionAttack::SendMsg()
{
	g_pTcpIp->SendNetMessage(m_MsgBuf);
}

BOOL CBattleInvasionAttack::MoveWindow(UINT msg, int x, int y)
{
	return CUIBaseWindow::MoveWindow(msg, x, y);
}

BOOL CBattleInvasionAttack::IsInside(int x, int y)
{
	return CUIBaseWindow::IsInside(x, y);
}

LRESULT CBattleInvasionAttack::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	m_btConfirm.MsgProc(hWnd, msg, wParam, lParam);
	m_ComboBox.MsgProcImage(hWnd, msg, wParam, lParam);

	switch( msg )
	{
	case WM_CHAR:
	case WM_IME_STARTCOMPOSITION:
	case WM_IME_COMPOSITION:
	case WM_IME_ENDCOMPOSITION:

		if (m_TextGuildName.GetEnable())
			m_TextGuildName.MsgProc(hWnd, msg, wParam, lParam);

		if (m_TextGuildName.GetEnable())
			return 1;
		else
			return 0;

	case WM_LBUTTONDOWN:

		if(!IsInside(LOWORD(lParam), HIWORD(lParam)))
			return IM_NONE;

		if( m_btConfirm.GetState() == BTN_DOWN )
			return IM_INPUT;

		break;

	//case WM_KEYDOWN:
	//case WM_CHAR :
	//	return IM_INPUT;
	case WM_LBUTTONUP :
		{
			if(!IsInside(LOWORD(lParam), HIWORD(lParam)))
				return IM_NONE;

			if(m_btConfirm.GetState() == BTN_ACTION)
			{
				if( strlen( m_TextGuildName.GetStrBuf() ) == 0 )
				{
					g_pNk2DFrame->InsertPopup((char*)G_STRING( IDS_LHSTRING1824 ) , TYPE_GUILD_CONFIRM , 42 );
					return IM_NONE;
				}
				if( strlen( m_TextGuildName.GetStrBuf() ) < 4 )
				{
					g_pNk2DFrame->InsertPopup((char*)G_STRING( IDS_LHSTRING1825 ) , TYPE_GUILD_CONFIRM , 42 );
					return IM_NONE;
				}

				double needmoney = 0;
				switch( m_ComboBox.GetSelect() )
				{
				case 0:
					needmoney = 50000000;
					break;
				case 1:
					needmoney = 100000000;
					break;
				case 2:
					needmoney = 150000000;
					break;
				case 3:
					needmoney = 200000000;
					break;
				}

				if( g_pRoh->m_Money < needmoney )
				{
					g_pNk2DFrame->InsertPopup((char*)G_STRING( IDS_LHSTRING1826) , TYPE_GUILD_CONFIRM , 42 );
					return IM_NONE;
				}

				sprintf(m_MsgBuf, "mg war req %d %s\n", m_ComboBox.GetSelect() , m_TextGuildName.GetStrBuf());
				SendMsg();
				return IM_INPUT;
			}

			m_pBtExit->MsgProc(hWnd, msg, wParam, lParam);

			if( m_pBtExit->GetState() == BTN_ACTION )
			{
				g_pNk2DFrame->GetUIWindow()->OnOpenWindow(WINDOW_GUILD_BATTLE_INVASION_REQUEST);
				return IM_INPUT;
			}
		}
		break;
	}

	return CUIBaseWindow::MsgProc(hWnd, msg, wParam, lParam);
}

/*****************************************************************************************************************************/
// 길드 침략 현황 인포 창

#define		CLASS_INVASION_IN_NAME	CBattleInvasionInfo::CBunch

#define	BOUNDARY_ATTACK_LINE_POSY_TOP	  (86)
#define	BOUNDARY_ATTACK_LINE_POSY_BOTTOM  (190)
#define	BOUNDARY_DEFENSE_LINE_POSY_TOP	  (254)
#define	BOUNDARY_DEFENSE_LINE_POSY_BOTTOM (360)

#define SCROLL_BOX_WIDTH	326
#define SCROLL_BOX_HEIGHT	122

CBattleInvasionInfo::CBattleInvasionInfo() : CUIBaseWindow()
{

	m_AttackList.clear();
	m_DefenseList.clear();

	m_nAttackCnt  = 0;
	m_nDefenseCnt = 0;

	m_nSelectType = -1;

	m_nAttackSelectNum  = -1;
	m_nDefenseSelectNum = -1;

	ZeroMemory(m_MsgBuf, sizeof(m_MsgBuf));
}

CBattleInvasionInfo::~CBattleInvasionInfo()
{
	DeleteRes();

	resetlist();
}

BOOL CBattleInvasionInfo::InitMode()
{
	m_btOk.SetFileName("guild/ok");

	return TRUE;
}

void CBattleInvasionInfo::LoadRes()
{
	int nScreenWidth  = 0;
	int nScreenHeight = 0;

	if( g_pNk2DFrame )
	{
		nScreenWidth  = g_pNk2DFrame->GetClientWidth();
		nScreenHeight = g_pNk2DFrame->GetClientHeight();
	}

	SetBackground(WBK_INVASIONINFO);
	MakeCloseBtn("guild/cancel");

	if(m_pBackScene)
	{

		int n_x = (nScreenWidth  / 2) - (m_pBackScene->GetWidth()  / 2);
		int n_y = (nScreenHeight / 2) - (m_pBackScene->GetHeight() / 2);

		m_nPosX = n_x;
		m_nPosY = n_y;

		m_AttackScroll.LoadRes("guild/scr_up", "guild/scr_down", "interface/guild/scr_bar.bmp");
		m_AttackScroll.SetRange(1);

		m_DefenseScroll.LoadRes("guild/scr_up", "guild/scr_down", "interface/guild/scr_bar.bmp");
		m_DefenseScroll.SetRange(1);

		if(g_pRoh->GetMyGuildData())
		{
			InsertList();
		}

		m_btOk.LoadRes();

		SetPosition();
	}
}

void CBattleInvasionInfo::DeleteRes()
{
	m_btOk.DeleteRes();

	m_AttackScroll.DeleteRes();
	m_DefenseScroll.DeleteRes();

	vector<CBunch*>::iterator _iter;

	for(_iter = m_AttackList.begin(); _iter != m_AttackList.end(); _iter++)
	{
		CBunch *pTemp = *_iter;

		if(pTemp)
			pTemp->DeleteRes();
	}

	for(_iter = m_DefenseList.begin(); _iter != m_DefenseList.end(); _iter++)
	{
		CBunch *pTemp = *_iter;

		if(pTemp)
			pTemp->DeleteRes();
	}
}

void CBattleInvasionInfo::RestoreSurfaces()
{

}

int CBattleInvasionInfo::Draw()
{
	if( g_pDisplay && m_pBackScene)
	{
		if(CUIBaseWindow::Draw())
		{
			m_AttackScroll.Draw();
			m_DefenseScroll.Draw();

			m_btOk.Draw();

			vector<CBunch*>::iterator _iter;

			for(_iter = m_AttackList.begin(); _iter != m_AttackList.end(); _iter++)
			{
				CBunch *pTemp = *_iter;

				if(pTemp)
					pTemp->Draw(m_AttackScroll.GetPos());
			}

			for(_iter = m_DefenseList.begin(); _iter != m_DefenseList.end(); _iter++)
			{
				CBunch *pTemp = *_iter;

				if(pTemp)
					pTemp->Draw(m_DefenseScroll.GetPos());
			}
		}
	}


	return -1;
}

void CBattleInvasionInfo::InsertList()
{
	CGuildData* pData = g_pRoh->GetMyGuildData();

	list_guildInvasion::iterator _iter_ = pData->GetInvasionInfo().begin();

	for(; _iter_ != pData->GetInvasionInfo().end(); _iter_++)
	{
		InsertList(*_iter_);
	}

}

void CBattleInvasionInfo::InsertList(guild_Invasion& Info_Invasion)
{
	switch(Info_Invasion.s_Invasion_flag)
	{
	case INVASION_ATTACK :
		{
			CLASS_INVASION_IN_NAME* pTemp = CLASS_INVASION_IN_NAME::Create(m_nPosX, m_nPosY,
											BOUNDARY_ATTACK_LINE_POSY_TOP,
											BOUNDARY_ATTACK_LINE_POSY_BOTTOM);

			pTemp->Init("guild/end");
			pTemp->LoadRes();
			pTemp->SetPosition(0, 0, m_nAttackCnt);
			pTemp->SetGuildName(Info_Invasion.s_guild_Name);
			pTemp->SetTime(Info_Invasion.s_time);
			pTemp->SetGuildIdx(Info_Invasion.s_guild_Index);
			pTemp->SetInvasionType(Info_Invasion.s_Invasion_flag);

			m_AttackScroll.SetRange(m_nAttackCnt*28);

			m_nAttackCnt++;

			m_AttackList.push_back(pTemp);
		}
		break;

	case INVASION_DEFENSE :
		{
			CLASS_INVASION_IN_NAME* pTemp = CLASS_INVASION_IN_NAME::Create(m_nPosX, m_nPosY,
											BOUNDARY_DEFENSE_LINE_POSY_TOP,
											BOUNDARY_DEFENSE_LINE_POSY_BOTTOM);

			pTemp->Init("guild/reconcile");
			pTemp->LoadRes();
			pTemp->SetPosition(0, 0, m_nDefenseCnt);
			pTemp->SetGuildName(Info_Invasion.s_guild_Name);
			pTemp->SetTime(Info_Invasion.s_time);
			pTemp->SetGuildIdx(Info_Invasion.s_guild_Index);
			pTemp->SetInvasionType(Info_Invasion.s_Invasion_flag);

			m_DefenseScroll.SetRange(m_nDefenseCnt*28);

			m_nDefenseCnt++;

			m_DefenseList.push_back(pTemp);
		}
		break;
	}

}

void CBattleInvasionInfo::EraseData()
{
	if(m_nSelectType == -1)
		return;

	if(m_nAttackSelectNum == -1 && m_nDefenseSelectNum == -1)
		return;

	if(m_nSelectType == 1)
	{
		vector<CBunch*>::iterator _iter_;

		int i = 0;

		//////////////////////////////////////////////////
		_iter_ = m_AttackList.begin() + m_nAttackSelectNum;

		CBunch* pTemp = (*_iter_);

		SAFE_DELETE(pTemp);
		//////////////////////////////////////////////////

		m_AttackList.erase(m_AttackList.begin() + m_nAttackSelectNum);

		for(_iter_ = m_AttackList.begin(); _iter_ != m_AttackList.end(); _iter_++, i++)
		{
			CBunch* pTemp  = (*_iter_);

			if(pTemp)
				pTemp->SetRePosition(i, 0);

		}

		m_AttackScroll.SetRange(m_AttackList.size()*28);

		m_nAttackSelectNum = -1;

		m_nSelectType = -1;

		m_nAttackCnt--;
	}
	else if(m_nSelectType == 2)		// 일반
	{
		vector<CBunch*>::iterator _iter_;

		int i = 0;

		_iter_ = m_DefenseList.begin() + m_nDefenseSelectNum;

		CBunch* pTemp = (*_iter_);

		SAFE_DELETE(pTemp);

		m_DefenseList.erase(m_DefenseList.begin() + m_nDefenseSelectNum);

		for(_iter_ = m_DefenseList.begin(); _iter_ != m_DefenseList.end(); _iter_++, i++)
		{
			CBunch* pTemp  = (*_iter_);

			if(pTemp)
				pTemp->SetRePosition(i, 135);

		}

		m_DefenseScroll.SetRange(m_DefenseList.size()*55);

		m_nDefenseSelectNum = -1;

		m_nSelectType = -1;

		m_nDefenseCnt--;
	}
}

void CBattleInvasionInfo::deleteList(guild_Invasion& Info_Invasion)
{
}


void CBattleInvasionInfo::resetlist()
{
	vector<CBunch*>::iterator _iter_;

	for(_iter_ = m_AttackList.begin(); _iter_ != m_AttackList.end(); _iter_++)
	{
		CLASS_INVASION_IN_NAME* pTemp = *_iter_;

		SAFE_DELETE(pTemp);
	}

	m_AttackList.clear();

	for(_iter_ = m_DefenseList.begin(); _iter_ != m_DefenseList.end(); _iter_++)
	{
		CLASS_INVASION_IN_NAME* pTemp = *_iter_;

		SAFE_DELETE(pTemp);
	}

	m_DefenseList.clear();
}

void CBattleInvasionInfo::SendMsg()
{
	g_pTcpIp->SendNetMessage(m_MsgBuf);
}

void CBattleInvasionInfo::SetPosition()
{
	m_AttackScroll.SetDrawPos(m_nPosX + 347 , m_nPosY + 68 , 131);
	m_DefenseScroll.SetDrawPos(m_nPosX + 347, m_nPosY + 236, 131);

	m_btOk.SetPosition( m_nPosX + 90 ,
						m_nPosY + m_pBackScene->GetHeight() - m_btOk.Height - 20 );
	m_pBtExit->SetPosition( m_nPosX + m_pBackScene->GetWidth() -  m_pBtExit->Width  - 90 ,
							m_nPosY + m_pBackScene->GetHeight() - m_pBtExit->Height - 20 );

	vector<CBunch*>::iterator _iter_;

	for(_iter_ = m_AttackList.begin(); _iter_ != m_AttackList.end(); _iter_++)
	{
		CLASS_INVASION_IN_NAME* pTemp = *_iter_;

		if(pTemp)
			pTemp->SetBasePosition(m_nPosX, m_nPosY);
	}

	m_AttackScroll.SetUseWheelArea(true);
	m_AttackScroll.SetWheelArea(m_nPosX + 21, m_nPosY + BOUNDARY_ATTACK_LINE_POSY_TOP, SCROLL_BOX_WIDTH, BOUNDARY_ATTACK_LINE_POSY_BOTTOM - 27);

	for(_iter_ = m_DefenseList.begin(); _iter_ != m_DefenseList.end(); _iter_++)
	{
		CLASS_INVASION_IN_NAME* pTemp = *_iter_;

		if(pTemp)
			pTemp->SetBasePosition(m_nPosX, m_nPosY);
	}

	m_DefenseScroll.SetUseWheelArea(true);
	m_DefenseScroll.SetWheelArea(m_nPosX + 21, m_nPosY + BOUNDARY_DEFENSE_LINE_POSY_TOP, SCROLL_BOX_WIDTH, BOUNDARY_DEFENSE_LINE_POSY_BOTTOM - 27);
}

BOOL CBattleInvasionInfo::MoveWindow(UINT msg, int x, int y)
{
	return CUIBaseWindow::MoveWindow(msg, x, y);
}

BOOL CBattleInvasionInfo::IsInside(int x, int y)
{
	return CUIBaseWindow::IsInside(x, y);
}

LRESULT CBattleInvasionInfo::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	m_btOk.MsgProc(hWnd, msg, wParam, lParam);

	if( msg == 0x020A )
	{
		if(!IsInside(LOWORD(lParam), HIWORD(lParam)))
			return IM_NONE;

		if( m_AttackScroll.MsgProc(hWnd, msg, wParam, lParam) == S_OK )
			return IM_INPUT;

		if( m_DefenseScroll.MsgProc(hWnd, msg, wParam, lParam) == S_OK)
			return IM_INPUT;
	}		

	vector<CBunch*>::iterator _iter;


	bool nResult = false;
	for(_iter = m_AttackList.begin(); _iter != m_AttackList.end(); _iter++)
	{
		CBunch *pTemp = *_iter;

		if(pTemp && (pTemp->MsgProc(hWnd, msg, wParam, lParam, m_AttackScroll.GetPos()) != -1))
		{

			strcpy(m_MsgBuf, pTemp->m_MsgBuf);
			m_nAttackSelectNum = pTemp->m_nSequence;
			m_nSelectType = 1;
			nResult = true;
		}
	}
	if( nResult )
		return IM_INPUT;


	nResult = false;
	for(_iter = m_DefenseList.begin(); _iter != m_DefenseList.end(); _iter++)
	{
		CBunch *pTemp = *_iter;

		if(pTemp && (pTemp->MsgProc(hWnd, msg, wParam, lParam, m_DefenseScroll.GetPos()) != -1))
		{
			strcpy(m_MsgBuf, pTemp->m_MsgBuf);
			m_nDefenseSelectNum = pTemp->m_nSequence;
			m_nSelectType = 2;
			nResult = true;
		}
	}
	if( nResult )
		return IM_INPUT;



	switch( msg )
	{
	case WM_LBUTTONDOWN:

		if(!IsInside(LOWORD(lParam), HIWORD(lParam)))
			return IM_NONE;

		if( m_AttackScroll.MsgProc(hWnd, msg, wParam, lParam) == S_OK )
			return IM_INPUT;

		if( m_DefenseScroll.MsgProc(hWnd, msg, wParam, lParam) == S_OK)
			return IM_INPUT;

		if( m_btOk.GetState() == BTN_DOWN )
			return IM_INPUT;

		break;

	case WM_LBUTTONUP :
		{
			if(!IsInside(LOWORD(lParam), HIWORD(lParam)))
				return IM_NONE;

			if( m_AttackScroll.MsgProc(hWnd, msg, wParam, lParam) == S_OK)
				return IM_INPUT;

			if( m_DefenseScroll.MsgProc(hWnd, msg, wParam, lParam) == S_OK)
				return IM_INPUT;

			if(m_btOk.GetState() == BTN_ACTION )
			{
				g_pNk2DFrame->GetUIWindow()->OnOpenWindow(WINDOW_GUILD_MEMBER);
				return IM_INPUT;
			}

			m_pBtExit->MsgProc(hWnd, msg, wParam, lParam);

			if( m_pBtExit->GetState() == BTN_ACTION )
			{
				g_pNk2DFrame->GetUIWindow()->OnOpenWindow(WINDOW_GUILD_BATTLE_INVASION_REQUEST);
				return IM_INPUT;
			}

		}
		break;
	}

	return CUIBaseWindow::MsgProc(hWnd, msg, wParam, lParam);
}

CLASS_INVASION_IN_NAME*	CLASS_INVASION_IN_NAME::Create(int nBaseX, int nBaseY, int nTopLimit, int nBottomLimit)
{
	return new CBunch(nBaseX, nBaseY, nTopLimit, nBottomLimit );
}

CLASS_INVASION_IN_NAME::CBunch(int nBaseX, int nBaseY, int nTopLimit, int nBottomLimit)
{
	m_hWnd		= NULL;

	m_nBaseX = nBaseX;
	m_nBaseY = nBaseY;

	m_nTopLimit	   = nTopLimit;
	m_nBottomLimit = nBottomLimit;

	m_nSequence   = 0;
	m_nGuildIdx   = 0;

	m_InvasionType = INVASION_NONE;

	ZeroMemory(m_MsgBuf, sizeof(m_MsgBuf));
}

CLASS_INVASION_IN_NAME::~CBunch()
{
	DeleteRes();

}

void CLASS_INVASION_IN_NAME::Init(char *pBtFilename)
{
	m_BtFire.SetFileName(pBtFilename);
}

void CLASS_INVASION_IN_NAME::LoadRes()
{
	if(!g_pDisplay)	return;

	m_hWnd =  g_pDisplay->GetHWnd();
	GetClientRect(m_hWnd, &m_ClientRc);
	m_BtFire.LoadRes();
}

void CLASS_INVASION_IN_NAME::Draw(int nScrollPos)
{
	if(!g_pDisplay)	return;

	m_tGuildName.DrawClipText(m_tGuildName.m_StringPosY,
							  m_nBaseY + m_nTopLimit,
							  m_nBaseY + m_nBottomLimit,
							  nScrollPos);

	m_tTime.DrawClipText(m_tTime.m_StringPosY,
						 m_nBaseY + m_nTopLimit,
						 m_nBaseY + m_nBottomLimit,
						 nScrollPos);

	m_BtFire.DrawClip(m_nBaseY + m_nTopLimit,
					  m_nBaseY + m_nBottomLimit, nScrollPos);

}

void CLASS_INVASION_IN_NAME::DeleteRes()
{
	m_BtFire.DeleteRes();
	m_tGuildName.DeleteRes();
	m_tTime.DeleteRes();
}

void CLASS_INVASION_IN_NAME::SetGuildName(char* pName)
{
	m_tGuildName.SetString_Center_Arrange(pName, RGB(0, 255, 64), RGB(255, 255, 255), TRUE);
}

void CLASS_INVASION_IN_NAME::SetTime(t_Info time)
{
	if( time.nYear	== -1 &&
			time.nMon	== -1 &&
			time.nDay	== -1 &&
			time.nHour	== -1 &&
			time.nMin	== -1 &&
			time.nSec	== -1 )
		return;

	char szTmp[256] = {0,};

	if( time.nMon != -1) // IDS_GUILD_DAY_MONTH	"%02d/%02d %02d:%02d"
	{
		sprintf( szTmp, (char*)G_STRING(IDS_GUILD_DAY_MONTH), time.nMon, time.nDay, time.nHour, time.nMin );
	}
	else if( time.nDay != -1) // IDS_GUILD_DAY_DAY "%d일 %02d:%02d"
	{
		sprintf( szTmp, (char*)G_STRING(IDS_GUILD_DAY_DAY), time.nDay, time.nHour, time.nMin );
	}
	else if( time.nHour != -1) // IDS_GUILD_DAY_HOUR	"%d시 %d분"
	{
		sprintf( szTmp, (char*)G_STRING(IDS_GUILD_DAY_HOUR), time.nHour, time.nMin );
	}
	else if( time.nMin != -1) // IDS_GUILD_DAY_MINUTE "%d분"
	{
		sprintf( szTmp, (char*)G_STRING(IDS_GUILD_DAY_MINUTE), time.nMin );
	}
	else if( time.nSec != -1) // IDS_GUILD_DAT_SECOND "%d초"
	{
		sprintf( szTmp, (char*)G_STRING(IDS_GUILD_DAT_SECOND), time.nSec );
	}

	m_tTime.SetString_Center_Arrange(szTmp, RGB(0, 255, 64), RGB(255, 255, 255), TRUE);

	/*
	if(timeT <= 0)	return;

	tm *pTm = NULL;

	pTm = localtime(&timeT);

	if(pTm == NULL)	return;

	int	nMonth  = 0;
	int nDay    = 0;
	int nHour   = 0;
	int nMinute = 0;

	char szTmp[256];

	nMonth  = pTm->tm_mon + 1;
	nDay    = pTm->tm_mday;
	nHour   = pTm->tm_hour;
	nMinute = pTm->tm_min;

	ZeroMemory(szTmp, sizeof(szTmp));

	sprintf(szTmp, "%d월 %d일 %d시 %d분", nMonth, nDay, nHour, nMinute);

	m_tTime.SetString_Center_Arrange(szTmp, RGB(0, 255, 64), RGB(255, 255, 255), TRUE);
	*/
}

void CLASS_INVASION_IN_NAME::SetPosition(int nPosX, int nPosY, int nNum)
{
	m_hWnd =  g_pDisplay->GetHWnd();
	GetClientRect(m_hWnd, &m_ClientRc);

	m_nSequence = nNum;

	m_tGuildName.Init(12, RGB(255, 255, 255),
					  m_nBaseX + 27 ,
					  m_nBaseY + m_nTopLimit + nPosY + (28*m_nSequence),
					  122, 14, TRUE);

	m_tTime.Init(12, RGB(255, 255, 255),
				 m_nBaseX + 162,
				 m_nBaseY + m_nTopLimit + nPosY + (28*m_nSequence),
				 118, 14, TRUE);

	m_BtFire.SetPosition(m_nBaseX + 290, m_nBaseY + m_nTopLimit + nPosY + (28*m_nSequence) );
}

void CLASS_INVASION_IN_NAME::SetBasePosition(int nBaseX, int nBaseY)
{
	m_nBaseX = nBaseX;
	m_nBaseY = nBaseY;

	m_tGuildName.SetPos_Center(	m_nBaseX +  27, m_nBaseY + m_nTopLimit + (28*m_nSequence));
	m_tTime.SetPos_Center(		m_nBaseX + 162, m_nBaseY + m_nTopLimit + (28*m_nSequence));
	m_BtFire.SetPosition(		m_nBaseX + 290, m_nBaseY + m_nTopLimit + (28*m_nSequence));

}
void CLASS_INVASION_IN_NAME::SetRePosition(int nNum, int nPosY)
{
	m_hWnd =  g_pDisplay->GetHWnd();
	GetClientRect(m_hWnd, &m_ClientRc);

	m_nSequence = nNum;

	m_tGuildName.SetPos_Center(m_nBaseX + 27, m_nBaseY + 93 + (28*nNum));
	m_tTime.SetPos_Center(m_nBaseX + 162, m_nBaseY + 93 + (28*nNum));
	m_BtFire.SetPosition(m_nBaseX + 290, m_nBaseY + 90 + (28*nNum));
}

void CLASS_INVASION_IN_NAME::Restore()
{

}

void CLASS_INVASION_IN_NAME::SendMsg()
{
	g_pTcpIp->SendNetMessage(m_MsgBuf);
}


BOOL CLASS_INVASION_IN_NAME::IsInside(int x, int y)
{
	if( x > m_nBaseX+21 && x < m_nBaseX+21+SCROLL_BOX_WIDTH &&
			y > m_nBaseY + m_nTopLimit && y <  m_nBaseY + m_nBottomLimit)
		return TRUE;

	return FALSE;
}

LRESULT CLASS_INVASION_IN_NAME::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, int nScroll)
{
	m_BtFire.MsgProcClip(hWnd, msg, wParam, lParam, nScroll, m_nBaseY + m_nTopLimit, m_nBaseY + m_nBottomLimit);

	switch(msg)
	{
	case WM_LBUTTONDOWN:

		if( IsInside( LOWORD(lParam), HIWORD(lParam) ) )
			return IM_INPUT;

		break;

	case WM_LBUTTONUP :
		{
			if(m_BtFire.GetState() == BTN_ACTION)
			{
				char szTmp[256] = {0,};

				if(m_InvasionType == INVASION_ATTACK)
				{
					// IDS_GUILD_PLZ_INVASION_END01 "%s 길드에 종결을 요청합니다."
					sprintf(szTmp, (char*)G_STRING(IDS_GUILD_PLZ_INVASION_END01), m_tGuildName.GetString());
					sprintf(m_MsgBuf, "mg war cancel req %d\n", m_nGuildIdx);

					g_pNk2DFrame->InsertPopup(szTmp, TYPE_GUILD_CONFIRM, 52);
				}

				if(m_InvasionType == INVASION_DEFENSE )
				{
					// IDS_GUILD_PLZ_INVASION_END02 "%s 길드에 화해를 요청합니다."
					sprintf(szTmp, (char*)G_STRING(IDS_GUILD_PLZ_INVASION_END02), m_tGuildName.GetString());
					sprintf(m_MsgBuf, "mg war cancel req %d\n", m_nGuildIdx);

					g_pNk2DFrame->InsertPopup(szTmp, TYPE_GUILD_CONFIRM, 52);
				}

				return 3;
			}
		}
		break;
	}

	return -1;
}