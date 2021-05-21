#include "stdafx.h"
#include "wanted_register.h"

#include "d3dutil.h"
#include "headers.h"
#include "tcpipcon.h"
#include "Nk2DFrame.h"
#include "2DResMgr.h"
#include "window_resource.h"
#include "WindowMgr.h"

CWantedRegister::CWantedRegister()
	: CUITgaBaseWindow()
	, m_beforTiem(0)
{
	ZeroMemory(m_szMsgBuffer, sizeof(m_szMsgBuffer));
}

CWantedRegister::~CWantedRegister()
{
	DeleteRes();

	m_TextCharName.CloseIMC();
	m_TextWantedMoney.CloseIMC();
}

BOOL CWantedRegister::InitMode()
{
	if( g_SvrType != ST_ADULT_ONLY )
		return FALSE;

	/****** 무조건 해줘야 함 *************************************************************/
	SetIndex_ptr(g_pNk2DFrame->GetWindowRes()->GetWindowTgaUI());
	SetBackground(g_pNk2DFrame->GetWindowRes()->GetTgaWindwResNum(WBK_TGA_WANTED_REGISTER));

	SetWidth(529);
	SetHeight(358);
	/*************************************************************************************/

	m_btWantedStart.SetFileName("wanted/wanted");	// 수배
	m_btExistCheck.SetFileName("wanted/exist");		// 존재여부확인
	m_btBack.SetFileName("wanted/back");

	m_TextRace.Init(14,  RGB(255, 255, 255), 0, 0, 132, 26, TRUE);

	m_beforTiem = timeGetTime();

	return TRUE;
}

void CWantedRegister::LoadRes()
{
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

	m_btExistCheck.LoadRes();
	m_btWantedStart.LoadRes();
	m_btBack.LoadRes();

	m_TextCharName.Init(m_nPosX, m_nPosY, 20, RGB(255, 255, 255), RGB(255, 179, 0), FALSE, MSG_POPUP);
	m_TextWantedMoney.Init(m_nPosX, m_nPosY, 20, RGB(255, 255, 255), RGB(255, 179, 0), FALSE, MSG_POPUP);

	m_TextCharName.SetfontSize(12);
	m_TextCharName.SetMaxLen(20);
	//m_TextCharName.SetEnable(TRUE);

	m_TextWantedMoney.SetfontSize(12);
	m_TextWantedMoney.SetMaxLen(20);
	//m_TextWantedMoney.SetEnable(TRUE);

	m_ComboTerm.SetItemSize(148, 17, 4, 3);
	m_ComboTerm.LoadRes("wanted/combo_btn", "interface/wanted/combo_bk.bmp", "interface/wanted/wanted_register/period.bmp");
	m_ComboTerm.DeleteAllItem();

	m_ComboTerm.InsertItemByImage("interface/wanted/wanted_register/period_1");
	m_ComboTerm.InsertItemByImage("interface/wanted/wanted_register/period_3");
	m_ComboTerm.InsertItemByImage("interface/wanted/wanted_register/period_7");
	m_ComboTerm.InsertItemByImage("interface/wanted/wanted_register/period_10");

	SetWindomMoveBarRange(329, 525, 6, 34);

	m_beforTiem = timeGetTime();

	SetPosition();
}


void CWantedRegister::DeleteRes()
{
	m_btExistCheck.DeleteRes();
	m_btWantedStart.DeleteRes();
	m_btBack.DeleteRes();
	m_TextCharName.DeleteRes();
	m_TextWantedMoney.DeleteRes();
	m_ComboTerm.DeleteRes();

	m_TextRace.DeleteRes();
}

int CWantedRegister::Render()
{
	if( g_SvrType != ST_ADULT_ONLY )
		return -1;

	if(!g_pDisplay)	return 0;

	CUITgaBaseWindow::Render();

	m_btExistCheck.Draw();
	m_btWantedStart.Draw();
	m_btBack.Draw();
	m_TextCharName.Draw();
	/*
	{
		char temp[128] = {0,};
		char temp2[128] = {0,};
		int money = atoi( m_TextWantedMoney.GetStrBuf() );

		sprintf( temp, "%d", money);
		InsertPriceComma( temp2, temp, '-' );
		m_TextWantedMoney.se(temp2);
	}
	*/

	m_TextWantedMoney.Draw();

	m_TextRace.Draw();

	m_ComboTerm.DrawImage();

	return -1;
}

void CWantedRegister::SetPosition()
{
	SetExitBtnPostion(451, 323);

	m_btExistCheck.SetPosition(m_nPosX + 357, m_nPosY + 157);

	m_btWantedStart.SetPosition(m_pBtExit->PosX - m_btWantedStart.Width - 8 , m_nPosY + 323);
	m_btBack.SetPosition( m_btWantedStart.PosX - m_btBack.Width - 8 , m_nPosY + 323 );

	m_TextCharName.SetPos(m_nPosX + 207, m_nPosY + 163);
	m_TextWantedMoney.SetPos(m_nPosX + 207, m_nPosY + 265);

	m_ComboTerm.SetPosition(m_nPosX + 200, m_nPosY + 224);
	m_ComboTerm.BtnSetPosition(m_nPosX + 200 + 155, m_nPosY + 224 + 5);

	m_TextRace.SetPos_Center(m_nPosX + 207, m_nPosY + 196);
}

void CWantedRegister::SendMsg()
{
	g_pTcpIp->SendNetMessage(m_szMsgBuffer);
}

void CWantedRegister::SetRaceName(char* szRaceName)
{
	m_TextRace.SetString_Center_Arrange(szRaceName, RGB(0, 255, 64), RGB(255, 255, 255), TRUE);

}

int CWantedRegister::GetTerm(int nNum)
{
	switch(nNum)
	{
	case 0 :
		return 1;
	case 1 :
		return 3;
	case 2 :
		return 7;
	case 3 :
		return 10;
	}

	return 0;
}

BOOL CWantedRegister::MoveWindow(UINT msg, int x, int y)
{
	return CUITgaBaseWindow::MoveWindow(msg, x, y);
}

BOOL CWantedRegister::IsInside(int x, int y)
{
	return CUITgaBaseWindow::IsInside(x, y);
}


LRESULT CWantedRegister::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	if( g_SvrType != ST_ADULT_ONLY )
		return IM_TGA_WIN_NONE;

	m_ComboTerm.MsgProcImage(hWnd, msg, wParam, lParam);
	m_TextCharName.MsgProc(hWnd, msg, wParam, lParam);
	m_TextWantedMoney.MsgProc(hWnd, msg, wParam, lParam);

	//if(!IsInside(LOWORD(lParam), HIWORD(lParam)))
	//	return IM_TGA_WIN_NONE;

	m_btWantedStart.MsgProc(hWnd, msg, wParam, lParam);
	m_btExistCheck.MsgProc(hWnd, msg, wParam, lParam);
	m_btBack.MsgProc(hWnd, msg, wParam, lParam);

	switch(msg)
	{
	case WM_KEYDOWN:
		{
			if( int(wParam) == VK_ESCAPE )
				return IM_TGA_WIN_NONE;
		}
	case WM_CHAR :
		{
			//m_TextCharName.MsgProc(hWnd, msg, wParam, lParam);
			//m_TextWantedMoney.MsgProc(hWnd, msg, wParam, lParam);
		}
		return IM_TGA_WIN_INPUT;

	case WM_LBUTTONDOWN:

		if(!IsInside(LOWORD(lParam), HIWORD(lParam)))
			return IM_TGA_WIN_NONE;

		if( m_btWantedStart.GetState() == BTN_DOWN )
			return IM_TGA_WIN_INPUT;

		if( m_btExistCheck.GetState() == BTN_DOWN )
			return IM_TGA_WIN_INPUT;

		if( m_btBack.GetState() == BTN_DOWN)
			return IM_TGA_WIN_INPUT;

		break;

	case WM_LBUTTONUP :
		{
			if(!IsInside(LOWORD(lParam), HIWORD(lParam)))
				return IM_TGA_WIN_NONE;

			//GetTerm(m_ComboTerm.GetSelect()) - 1
			if(m_btWantedStart.GetState() == BTN_ACTION && timeGetTime() - m_beforTiem > 1500 )
			{
				sprintf(m_szMsgBuffer,"wanted reg r %s %d %s\n", m_TextCharName.GetStrBuf(), m_ComboTerm.GetSelect(), m_TextWantedMoney.GetStrBuf());

				m_beforTiem = timeGetTime();
				SendMsg();

				// 현상수배 이름 초기화
				m_TextCharName.ClearText();
				m_TextWantedMoney.ClearText();
				m_TextRace.SetString_Center_Arrange("", RGB(0, 255, 64), RGB(255, 255, 255), TRUE);

				return IM_TGA_WIN_INPUT;
			}

			if(m_btExistCheck.GetState() == BTN_ACTION)
			{
				sprintf(m_szMsgBuffer,"wanted reg t %s\n", m_TextCharName.GetStrBuf());
				SendMsg();

				return IM_TGA_WIN_INPUT;
			}

			if( m_btBack.GetState() == BTN_ACTION )
			{
				g_pNk2DFrame->GetTgaUIWindow()->OnOpenWindow(WINDOW_TGA_WANTED_PAGE);
				return IM_TGA_WIN_INPUT;
			}
		}
		break;
	}

	return CUITgaBaseWindow::MsgProc(hWnd, msg, wParam, lParam);
}