#include "stdafx.h"
#include "wanted_start_page.h"

#include "d3dutil.h"
#include "headers.h"
#include "tcpipcon.h"
#include "Nk2DFrame.h"
#include "2DResMgr.h"
#include "window_resource.h"
#include "WindowMgr.h"

CWantedStartPage::CWantedStartPage() : CUITgaBaseWindow()
{
	ZeroMemory(m_MsgBuf, sizeof(m_MsgBuf));
}

CWantedStartPage::~CWantedStartPage()
{
	DeleteRes();
}

BOOL CWantedStartPage::InitMode()
{
	if( g_SvrType != ST_ADULT_ONLY )
		return FALSE;

	/****** 무조건 해줘야 함 *************************************************************/
	SetIndex_ptr(g_pNk2DFrame->GetWindowRes()->GetWindowTgaUI());
	SetBackground(g_pNk2DFrame->GetWindowRes()->GetTgaWindwResNum(WBK_TGA_WANTED_PAGE));

	SetWidth(518);
	SetHeight(248);
	/*************************************************************************************/

	m_btWantedAdd.SetFileName("wanted/regist");	 // 수배등록
	m_btMyMenu.SetFileName("wanted/mymenu");	 // 나의메뉴
	m_btWantedlist.SetFileName("wanted/list");	 // 수배자 리스트
	m_btPrizeMoney.SetFileName("wanted/reward"); // 상금 지급


	return TRUE;
}

void CWantedStartPage::LoadRes()
{
	if( g_SvrType != ST_ADULT_ONLY )
		return;

	if(!g_pDisplay)	return;

	int nScreenWidth  = 0;
	int nScreenHeight = 0;
	if( g_pNk2DFrame )
	{
		nScreenWidth  = g_pNk2DFrame->GetClientWidth();
		nScreenHeight = g_pNk2DFrame->GetClientHeight();
	}

	MakeCloseBtn("wanted/cancel");

	int n_x = (nScreenWidth  / 2) - ( GetWidth()  / 2);
	int n_y = (nScreenHeight / 2) - ( GetHeight() / 2);

	m_nPosX = n_x;
	m_nPosY = n_y;

	m_btWantedAdd.LoadRes();
	m_btMyMenu.LoadRes();
	m_btWantedlist.LoadRes();
	m_btPrizeMoney.LoadRes();

	SetWindomMoveBarRange(318, 513, 6, 34);

	SetPosition();

}

void CWantedStartPage::DeleteRes()
{
	m_btWantedAdd.DeleteRes();
	m_btMyMenu.DeleteRes();
	m_btWantedlist.DeleteRes();
	m_btPrizeMoney.DeleteRes();
}

int CWantedStartPage::Render()
{
	if( g_SvrType != ST_ADULT_ONLY )
		return -1;

	if(!g_pDisplay)	return 0;

	CUITgaBaseWindow::Render();

	m_btWantedAdd.Draw();
	m_btMyMenu.Draw();
	m_btWantedlist.Draw();
	m_btPrizeMoney.Draw();

	return -1;
}

void CWantedStartPage::SendMsg()
{
	g_pTcpIp->SendNetMessage(m_MsgBuf);
}

void CWantedStartPage::SetPosition()
{
	SetExitBtnPostion(440, 212);

	m_btWantedAdd.SetPosition(m_nPosX + 34, m_nPosY + 212);
	m_btMyMenu.SetPosition(m_nPosX + 98, m_nPosY + 212);
	m_btWantedlist.SetPosition(m_nPosX + 161, m_nPosY + 212);
	m_btPrizeMoney.SetPosition(m_nPosX + 224, m_nPosY + 212);
}

BOOL CWantedStartPage::MoveWindow(UINT msg, int x, int y)
{
	return CUITgaBaseWindow::MoveWindow(msg, x, y);
}

BOOL CWantedStartPage::IsInside(int x, int y)
{
	return CUITgaBaseWindow::IsInside(x, y);
}

LRESULT CWantedStartPage::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	if( g_SvrType != ST_ADULT_ONLY )
		return IM_TGA_WIN_NONE;

	m_btMyMenu.MsgProc(hWnd, msg, wParam, lParam);
	m_btWantedlist.MsgProc(hWnd, msg, wParam, lParam);
	m_btWantedAdd.MsgProc(hWnd, msg, wParam, lParam);
	m_btPrizeMoney.MsgProc(hWnd, msg, wParam, lParam);

	switch(msg)
	{
	case WM_LBUTTONDOWN:
		if(!IsInside(LOWORD(lParam), HIWORD(lParam)))
			return IM_TGA_WIN_NONE;

		if( m_btMyMenu.GetState() == BTN_DOWN )
			return IM_TGA_WIN_INPUT;

		if( m_btWantedlist.GetState() == BTN_DOWN )
			return IM_TGA_WIN_INPUT;

		if( m_btWantedAdd.GetState() == BTN_DOWN )
			return IM_TGA_WIN_INPUT;

		if( m_btPrizeMoney.GetState() == BTN_DOWN )
			return IM_TGA_WIN_INPUT;

		break;

	case WM_LBUTTONUP :
		{
			if(m_btMyMenu.GetState() == BTN_ACTION)
			{
				sprintf(m_MsgBuf, "wanted my list\n");
				SendMsg();

				g_pNk2DFrame->GetTgaUIWindow()->OnOpenWindow(WINDOW_TGA_WANTED_MYMENU);

				return IM_TGA_WIN_INPUT;
			}

			if(m_btWantedlist.GetState() == BTN_ACTION)
			{

				sprintf(m_MsgBuf, "wanted list\n");
				SendMsg();

				g_pNk2DFrame->GetTgaUIWindow()->OnOpenWindow(WINDOW_TGA_WANTED_ALLLIST);

				return IM_TGA_WIN_INPUT;
			}

			if(m_btWantedAdd.GetState() == BTN_ACTION)
			{
				g_pNk2DFrame->GetTgaUIWindow()->OnOpenWindow(WINDOW_TGA_WANTED_REGISTER);

				return IM_TGA_WIN_INPUT;
			}

			if(m_btPrizeMoney.GetState() == BTN_ACTION)
			{
				sprintf(m_MsgBuf, "wanted reward\n");
				SendMsg();
				return IM_TGA_WIN_INPUT;
			}
		}
		break;
	}

	return CUITgaBaseWindow::MsgProc(hWnd, msg, wParam, lParam);
}