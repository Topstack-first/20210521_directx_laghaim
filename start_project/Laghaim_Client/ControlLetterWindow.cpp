#include "stdafx.h"
#include <boost/format.hpp>
#include "ControlLetterWindow.h"
#include "Nk2DFrame.h"
#include "PacketSend.h"
#include "LetterManager.h"
#include "ControlLetterRecv.h"
#include "ControlLetterSend.h"

ControlLetterWindow::ControlLetterWindow()
	: m_curType(0)	
	, m_lstLetter(NULL)
	, m_isLock(false)
{
}

ControlLetterWindow::~ControlLetterWindow()
{
	DeleteRes();
}

HRESULT ControlLetterWindow::RestoreSurfaces()
{
	DeleteRes();
	LoadRes();

	return S_OK;
}

void ControlLetterWindow::LoadRes()
{
	m_background.Init("LETTER/BG_LETTER");
	m_background2.Init("LETTER/BG_LETTER_02");
	m_btnClose.Init("common/btn_close_01");
	m_btnTabSend.Init("LETTER/TAB_SEND");
	m_btnTabRecv.Init("LETTER/TAB_RECV");
	m_btnTabGM.Init("LETTER/TAB_MASTER");
	m_btnWrite.Init("LETTER/BTN_WRITE_LETTER");
	m_btnPageFirst.Init("LETTER/BTN_AR_LEFT_END");
	m_btnPagePrev.Init("LETTER/BTN_AR_LEFT");
	m_btnPageNext.Init("LETTER/BTN_AR_RIGHT");
	m_btnPageLast.Init("LETTER/BTN_AR_RIGHT_END");	

	m_background.Align(6, g_pNk2DFrame->GetClientWidth(), g_pNk2DFrame->GetClientHeight());
	m_background2.Align(6, g_pNk2DFrame->GetClientWidth(), g_pNk2DFrame->GetClientHeight());
	int bgx = m_background.m_x;
	int bgy = m_background.m_y;

	for(int i = 0; i < MAX_LETTER_LIST; i++)
	{
		m_signNew[i].Init("LETTER/SIGN_NEW");
		m_signNew[i].SetPos(bgx + 470, bgy + 126 + (i * 19));
		m_signNew[i].SetVisible(false);
	}

	m_btnClose.SetPosition(bgx + 636 + 15, bgy + 22 - 16);
	m_btnTabRecv.SetPosition(bgx + 34, bgy + 126 - 67);
	m_btnTabSend.SetPosition(bgx + 34 + 78, bgy + 126 - 67);
	m_btnTabGM.SetPosition(bgx + 34 + 78 + 78, bgy + 126 - 67);
	m_btnWrite.SetPosition(bgx + 480 + 45, bgy + 340);

	m_btnPageFirst.SetPosition(bgx + 269 - 20, bgy + 343);
	m_btnPagePrev.SetPosition(bgx + 269, bgy + 343);
	m_btnPageNext.SetPosition(bgx + 383, bgy + 343);
	m_btnPageLast.SetPosition(bgx + 383 + 20, bgy + 343);

	m_lstLetter = new CMutiListBox(" ", 0, 0, 0, 10, 3, 12);
	m_lstLetter->AddLine(0, 100.0f);
	m_lstLetter->AddLine(1, 350.0f);
	m_lstLetter->AddLine(2, 180.0f);
	m_lstLetter->SetImage("guild/scr_down", "guild/scr_up", "interface/guild/scr_bar.bmp");
	m_lstLetter->SetSelectedLine(-1);
	m_lstLetter->LoadRes();
	m_lstLetter->LineHeight = 19;
	m_lstLetter->SetPosDim(bgx + 30, bgy + 126, 625, 220);
	m_lstLetter->m_DecBtn.SetVisible(false);
	m_lstLetter->m_IncBtn.SetVisible(false);

	m_textPage.Init(12, RGB(255,255,255), bgx + 325, bgy + 348);

	SetCurType(m_curType);
}

void ControlLetterWindow::DeleteRes()
{
	m_background.DeleteRes();
	m_background2.DeleteRes();
	m_btnClose.DeleteRes();
	m_btnTabRecv.DeleteRes();
	m_btnTabSend.DeleteRes();
	m_btnTabGM.DeleteRes();
	m_btnWrite.DeleteRes();
	m_btnPageFirst.DeleteRes();
	m_btnPagePrev.DeleteRes();
	m_btnPageNext.DeleteRes();
	m_btnPageLast.DeleteRes();
	m_textPage.DeleteRes();

	for(int i = 0; i < MAX_LETTER_LIST; i++)
	{
		m_signNew[i].DeleteRes();
	}

	SAFE_DELETE(m_lstLetter);
}

float ControlLetterWindow::Update()
{
	DWORD timeCur = GetTickCount();
	DWORD timeElapsed = timeCur - m_time;
	m_time = timeCur;

	float fElapsed = (float)(timeElapsed) / 1000.0f;
	m_timeAccum += fElapsed;

	m_btnTabRecv.SetState(BTN_NORMAL);
	m_btnTabSend.SetState(BTN_NORMAL);
	m_btnTabGM.SetState(BTN_NORMAL);

	if( m_curType == 0 )
		m_btnTabRecv.SetState(BTN_DOWN);
	else if( m_curType == 1 )
		m_btnTabSend.SetState(BTN_DOWN);
	else if( m_curType == 2 )
		m_btnTabGM.SetState(BTN_DOWN);


	return 0;
}

void ControlLetterWindow::Draw()
{
	if( g_pDisplay == NULL )
		return;

	m_background.Draw();
	m_background2.Draw();
	m_btnClose.Draw();
	m_btnTabRecv.Draw();
	m_btnTabSend.Draw();
	m_btnTabGM.Draw();
	m_btnWrite.Draw();
	m_btnPageFirst.Draw();
	m_btnPagePrev.Draw();
	m_btnPageNext.Draw();
	m_btnPageLast.Draw();
	m_textPage.Draw();

	for(int i = 0; i < MAX_LETTER_LIST; i++)
	{
		m_signNew[i].Draw();
	}

	if( m_lstLetter )
		m_lstLetter->Draw();
}

void ControlLetterWindow::ActionTab(int type)
{	
	SetLock(true);	

	SendViewNoteMessageBox(type, 0);
}

void ControlLetterWindow::ActionWrite()
{
	SetLock(true);

	((ControlLetterSend*)g_pNk2DFrame->GetInterface(LETTER_SEND))->ClearText();
	((ControlLetterSend*)g_pNk2DFrame->GetInterface(LETTER_SEND))->SetLock(false);
	g_pNk2DFrame->SetVisibleInterface(true, LETTER_SEND);
}

void ControlLetterWindow::ActionPage(int type)
{
	// 0 : first , 1 : prev
	// 2 : next, 3 : last

	LetterManager* mgr = LetterManager::GetInstance();
	int curPage = mgr->GetCurPage();
	int maxPage = mgr->GetMaxPage();

	if( type == 0 )
	{
		if( curPage <= 0 )
			return;

		SetLock(true);
		SendViewNoteMessageBox(m_curType, 0);
	}
	else if( type == 1 )
	{
		if( curPage <= 0 )
			return;

		SetLock(true);
		SendViewNoteMessageBox(m_curType, (curPage - 1));
	}
	else if( type == 2 )
	{
		if( maxPage <= (curPage + 1) )
			return;

		SetLock(true);
		SendViewNoteMessageBox(m_curType, (curPage + 1));
	}
	else if( type == 3 )
	{
		if( maxPage <= (curPage + 1) )
			return;

		SetLock(true);
		SendViewNoteMessageBox(m_curType, (maxPage - 1));
	}
}

void ControlLetterWindow::ActionRead(int index)
{
	int letterIndex = LetterManager::GetInstance()->GetLetterIndex(index);

	if( letterIndex == -1 )
		return;

	SetLock(true);

	LetterContent cont = LetterManager::GetInstance()->GetContentByArrayIndex(m_curType, index);

	if( cont.index != -1 )
	{
		g_pNk2DFrame->SetVisibleInterface(true, LETTER_RECV);
		((ControlLetterRecv*)g_pNk2DFrame->GetInterface(LETTER_RECV))->Refresh(cont.index, m_curType);
		return;
	}

	SendNoteMessageRead(m_curType, letterIndex);
}


void ControlLetterWindow::Refresh()
{
	SetLock(false);

	if( m_lstLetter == NULL )
		return;

	m_lstLetter->all_Reset();

	LetterManager* letterMgr = LetterManager::GetInstance();
	auto vecList = letterMgr->GetList();

	for(int i = 0; i < MAX_LETTER_LIST; i++)
	{
		m_signNew[i].SetVisible(false);
	}

	int count = vecList.size();
	for(int i = 0; i < count; i++)
	{
		m_lstLetter->AddString(i, 0, (char*)vecList[i].name.c_str());
		m_lstLetter->AddString(i, 1, (char*)vecList[i].title.c_str());

		time_t t = vecList[i].send_time;
		struct tm* tm = localtime(&t);
		std::string str = boost::str(boost::format("%04d-%02d-%02d[%02d:%02d]") 
			% (tm->tm_year + 1900)
			% (tm->tm_mon + 1)
			% tm->tm_mday
			% tm->tm_hour
			% tm->tm_min);

		m_lstLetter->AddString(i, 2, (char*)str.c_str());

		m_lstLetter->SetIncLine();
		
		if( vecList[i].state != 0 )
			m_signNew[i].SetVisible(true);
	}

	int curPage = letterMgr->GetCurPage();
	int maxPage = letterMgr->GetMaxPage();
	if( maxPage < 1 )
		maxPage = 1;

	char buf[32];
	sprintf(buf, "%d/%d", (curPage+1), maxPage);
	m_textPage.SetString(buf);
}

void ControlLetterWindow::ReqList(bool byDel)
{
	SetLock(true);

	LetterManager* letterMgr = LetterManager::GetInstance();
	int curPage = letterMgr->GetCurPage();
	if( byDel == true )
	{
		auto vecList = letterMgr->GetList();
		int count = vecList.size();
		if( count == 1 )
		{
			--curPage;
		}
	}

	if( curPage < 0 )
		curPage = 0;

	SendViewNoteMessageBox(m_curType, curPage);
}

void ControlLetterWindow::Open()
{
	ActionTab(0);
}

LRESULT ControlLetterWindow::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	if( m_isLock == true )
		return 0;

	int x, y;

	switch( msg )
	{
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_MOUSEMOVE:
		{
			x = LOWORD (lParam);
			y = HIWORD (lParam);

			if( m_background.IsIn(x, y) == false )
				return 0;

			if( E_FAIL != m_lstLetter->GetMessage(hWnd, msg, wParam, lParam) )
				return 1;

			m_btnClose.MsgProc(hWnd, msg, wParam, lParam);
			m_btnWrite.MsgProc(hWnd, msg, wParam, lParam);
			m_btnPageFirst.MsgProc(hWnd, msg, wParam, lParam);
			m_btnPagePrev.MsgProc(hWnd, msg, wParam, lParam);
			m_btnPageNext.MsgProc(hWnd, msg, wParam, lParam);
			m_btnPageLast.MsgProc(hWnd, msg, wParam, lParam);
			if( m_btnClose.GetState() == BTN_ACTION )
			{
				if( g_pNk2DFrame )
					g_pNk2DFrame->ShowInterfaceWindow(FALSE, LETTER_WINDOW);
				m_btnClose.SetState(BTN_NORMAL);
				return 1;
			}

			if( m_btnTabRecv.IsInside(x, y) )
				m_btnTabRecv.MsgProc(hWnd, msg, wParam, lParam);
			if( m_btnTabSend.IsInside(x, y) )
				m_btnTabSend.MsgProc(hWnd, msg, wParam, lParam);
			if( m_btnTabGM.IsInside(x, y) )
				m_btnTabGM.MsgProc(hWnd, msg, wParam, lParam);

			if( m_btnTabRecv.GetState() == BTN_ACTION )
			{
				m_btnTabRecv.SetState(BTN_NORMAL);				
				ActionTab(0);

				return 1;
			}

			if( m_btnTabSend.GetState() == BTN_ACTION )
			{
				m_btnTabSend.SetState(BTN_NORMAL);				
				ActionTab(1);

				return 1;
			}

			if( m_btnTabGM.GetState() == BTN_ACTION )
			{
				m_btnTabGM.SetState(BTN_NORMAL);				
				ActionTab(2);

				return 1;
			}

			if( m_btnWrite.GetState() == BTN_ACTION )
			{
				m_btnWrite.SetState(BTN_NORMAL);
				ActionWrite();

				return 1;
			}

			if( m_btnPageFirst.GetState() == BTN_ACTION )
			{
				m_btnPageFirst.SetState(BTN_NORMAL);
				ActionPage(0);

				return 1;
			}

			if( m_btnPagePrev.GetState() == BTN_ACTION )
			{
				m_btnPagePrev.SetState(BTN_NORMAL);
				ActionPage(1);

				return 1;
			}

			if( m_btnPageNext.GetState() == BTN_ACTION )
			{
				m_btnPageNext.SetState(BTN_NORMAL);
				ActionPage(2);

				return 1;
			}

			if( m_btnPageLast.GetState() == BTN_ACTION )
			{
				m_btnPageLast.SetState(BTN_NORMAL);
				ActionPage(3);

				return 1;
			}

			return 1;
		}
		break;
	case WM_LBUTTONDBLCLK:
		{
			x = LOWORD (lParam);
			y = HIWORD (lParam);

			if( m_background.IsIn(x, y) == false )
				return 0;

			if( m_lstLetter->IsInside(x, y) == false )
				return 1;

			int index = (y - m_lstLetter->PosY) / 19;
			ActionRead(index);

			return 1;
		}
		break;
	}

	return 0;
}

void ControlLetterWindow::SetCurType(int type)
{ 
	m_curType = type;

	if( type == 1 )
	{
		m_background.SetVisible(false);
		m_background2.SetVisible(true);
	}
	else
	{
		m_background.SetVisible(true);
		m_background2.SetVisible(false);
	}
}
