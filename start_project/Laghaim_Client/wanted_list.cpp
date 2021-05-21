#include "stdafx.h"
#include "wanted_list.h"

#include <time.h>
#include "d3dutil.h"
#include "headers.h"
#include "tcpipcon.h"
#include "Nk2DFrame.h"
#include "2DResMgr.h"
#include "window_resource.h"
#include "WindowMgr.h"

#define		CLASS_WANTED_LIST_NAME	CWantedList::CBunch

#define		BOUNDARY_WANTED_LIST_LINE_POSY_TOP	   (m_nBackPosY + 90)
#define		BOUNDARY_WANTED_LIST_LINE_POSY_BOTTOM  (m_nBackPosY + 295)

extern	int InsertPriceComma( char *DestString, char *SrcString, char FindChar ); // 가격 스트링들의 숫자 부에 콤마를 넣어주는 함수. ///가격표시개선

CWantedList::CWantedList() : CUITgaBaseWindow()
{
	m_nTotalCnt = 0;
	m_nSelect   = -1;
	ZeroMemory(m_szMsg, sizeof(m_szMsg));
}

CWantedList::~CWantedList()
{
	DeleteRes();

	list_all_reset();
}

BOOL CWantedList::InitMode()
{
	/****** 무조건 해줘야 함 *************************************************************/
	SetIndex_ptr(g_pNk2DFrame->GetWindowRes()->GetWindowTgaUI());
	SetBackground(g_pNk2DFrame->GetWindowRes()->GetTgaWindwResNum(WBK_TGA_WANTED_REGISTER_LIST));

	SetWidth(545);
	SetHeight(358);
	/*************************************************************************************/

	m_btRefrsh.SetFileName("wanted/newly");
	m_btBack.SetFileName("wanted/back");

	return TRUE;
}

void CWantedList::LoadRes()
{
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

	m_btRefrsh.LoadRes();
	m_btBack.LoadRes();

	m_Scroll.LoadRes("wanted/scr_up", "wanted/scr_down", "interface/wanted/scr_bar_normal.bmp");
	m_Scroll.SetRange(1);

	SetWindomMoveBarRange(326, 525, 6, 34);

	SetPosition();
}

void CWantedList::SetPosition()
{
	SetExitBtnPostion(451, 323);

	m_Scroll.SetDrawPos(m_nPosX + 532, m_nPosY + 75, 219);
	m_btRefrsh.SetPosition(m_pBtExit->PosX - m_btRefrsh.Width - 8 , m_nPosY + 323);
	m_btBack.SetPosition( m_btRefrsh.PosX - m_btBack.Width - 8 , m_nPosY + 323 );

	vector<CBunch*>::iterator _iter_;
	for(_iter_ = m_register_list.begin(); _iter_ != m_register_list.end(); _iter_++)
	{
		CLASS_WANTED_LIST_NAME* pTemp = *_iter_;

		if(pTemp)
			pTemp->SetBasePosition(m_nPosX, m_nPosY);
	}
}


void CWantedList::SendMsg()
{
	g_pTcpIp->SendNetMessage(m_szMsg);
}

void CWantedList::Insert_list(wanted_info& info)
{
	CLASS_WANTED_LIST_NAME* pTemp = CLASS_WANTED_LIST_NAME::Create(m_nPosX, m_nPosY);

	char szTmp[20];

	pTemp->Init();
	pTemp->LoadRes();
	pTemp->SetPosition(0, 0, m_nTotalCnt);
	pTemp->SetRegisterNum(itoa(m_nTotalCnt+1, szTmp, 10));
	pTemp->SetWantedName(info.wanted_name);
	pTemp->SetWantedRace(g_pNk2DFrame->GetWindowRes()->GetRaceName(info.wanted_race));
	pTemp->SetConnect(g_pNk2DFrame->GetWindowRes()->GetZoneName(info.wanted_zone));
	pTemp->SetRemainTime(info.wanted_remaintime);

	char temp[128] = {0,};
	sprintf( szTmp, "%d", info.wanted_laim);
	InsertPriceComma( temp, szTmp, '-' );
	pTemp->SetWantedMoney(temp);

	m_Scroll.SetRange(m_nTotalCnt*17);
	m_nTotalCnt++;

	m_register_list.push_back(pTemp);

}

void CWantedList::list_all_reset()
{
	vector<CBunch*>::iterator _iter_;

	for(_iter_ = m_register_list.begin(); _iter_ != m_register_list.end(); _iter_++)
	{
		CLASS_WANTED_LIST_NAME* pTemp = *_iter_;

		SAFE_DELETE(pTemp);
	}

	m_register_list.clear();

	m_nTotalCnt = 0;

	m_Scroll.SetRange(1);
}

void CWantedList::DeleteRes()
{
	m_Scroll.DeleteRes();
	m_btBack.DeleteRes();
	m_btRefrsh.DeleteRes();

	vector<CBunch*>::iterator _iter_;

	for(_iter_ = m_register_list.begin(); _iter_ != m_register_list.end(); _iter_++)
	{
		CLASS_WANTED_LIST_NAME *pTemp = *_iter_;

		if(pTemp) pTemp->DeleteRes();
	}

}

int CWantedList::Render()
{
	if( g_SvrType != ST_ADULT_ONLY )
		return -1;

	CUITgaBaseWindow::Render();

	vector<CBunch*>::iterator _iter_;

	for(_iter_ = m_register_list.begin(); _iter_ != m_register_list.end(); _iter_++)
	{
		CLASS_WANTED_LIST_NAME* pTemp = *_iter_;

		if(pTemp)
			pTemp->Draw(m_Scroll.GetPos());
	}

	m_Scroll.Draw();

	m_btRefrsh.Draw();
	m_btBack.Draw();

	return -1;
}

void CWantedList::Delete_list()
{
	vector<CLASS_WANTED_LIST_NAME*>::iterator _iter_;

	_iter_ = m_register_list.begin() + m_nSelect;

	CLASS_WANTED_LIST_NAME* pTemp = *_iter_;

	SAFE_DELETE(pTemp);

	m_register_list.erase(_iter_);

	int i = 0;

	for(_iter_ = m_register_list.begin(); _iter_ != m_register_list.end(); _iter_++, i++)
	{
		CLASS_WANTED_LIST_NAME* pTemp = *_iter_;

		if(pTemp)	pTemp->SetRePosition(i);

	}

	m_nTotalCnt = m_register_list.size();

	m_Scroll.SetRange(m_nTotalCnt*17);

	m_nSelect = -1;
}



BOOL CWantedList::MoveWindow(UINT msg, int x, int y)
{
	return CUITgaBaseWindow::MoveWindow(msg, x, y);
}


BOOL CWantedList::IsInside(int x, int y)
{
	return CUITgaBaseWindow::IsInside(x, y);
}


LRESULT CWantedList::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	if( g_SvrType != ST_ADULT_ONLY )
		return IM_TGA_WIN_NONE;

	m_Scroll.MsgProc(hWnd, msg, wParam, lParam);
	m_btRefrsh.MsgProc(hWnd, msg, wParam, lParam);
	m_btBack.MsgProc(hWnd, msg, wParam, lParam);

// 	vector<CLASS_WANTED_LIST_NAME*>::iterator _iter_;
//
// 	for(_iter_ = m_register_list.begin(); _iter_ != m_register_list.end(); _iter_++)
// 	{
// 		CLASS_WANTED_LIST_NAME* pTemp = *_iter_;
//
// 		if(pTemp && pTemp->MsgProc(hWnd, msg, wParam, lParam, m_Scroll.GetPos()) != 1)
// 		{
// 			m_nSelect = pTemp->m_nSequence;
// 			return IM_TGA_WIN_INPUT;
// 		}
// 	}

	switch(msg)
	{
	case WM_LBUTTONDOWN:

		if(!IsInside(LOWORD(lParam), HIWORD(lParam)))
			return IM_TGA_WIN_NONE;

		if( m_btRefrsh.GetState() == BTN_DOWN )
			return IM_TGA_WIN_INPUT;

		if( m_btBack.GetState() == BTN_DOWN )
			return IM_TGA_WIN_INPUT;
		break;

	case WM_LBUTTONUP :
		{
			if(!IsInside(LOWORD(lParam), HIWORD(lParam)))
				return IM_TGA_WIN_NONE;

			if(m_btRefrsh.GetState() == BTN_ACTION)
			{
				list_all_reset();	// 다시 다 지우고 새로 받는다 ㅠ.ㅠ

				sprintf(m_szMsg, "wanted list\n");
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

/*****************************************************************************************************************/
CLASS_WANTED_LIST_NAME*	CLASS_WANTED_LIST_NAME::Create(int nBaseX, int nBaseY)
{
	return new CBunch(nBaseX, nBaseY);
}

CLASS_WANTED_LIST_NAME::CBunch(int nBaseX, int nBaseY)
{
	m_hWnd		= NULL;

	m_nBackPosX = nBaseX;
	m_nBackPosY = nBaseY;

	m_nSequence   = 0;

	ZeroMemory(m_MsgBuf, sizeof(m_MsgBuf));
}

CLASS_WANTED_LIST_NAME::~CBunch()
{
	DeleteRes();
}

void CLASS_WANTED_LIST_NAME::Init()
{
}

void CLASS_WANTED_LIST_NAME::LoadRes()
{
	if( g_SvrType != ST_ADULT_ONLY )
		return;

	if(!g_pDisplay)	return;

	m_hWnd =  g_pDisplay->GetHWnd();
	GetClientRect(m_hWnd, &m_ClientRc);
}

void CLASS_WANTED_LIST_NAME::Draw(int nScrollPos)
{
	if( g_SvrType != ST_ADULT_ONLY )
		return;

	if(!g_pDisplay)	return;

	for(int i = 0; i < CWantedList::TEXT_TOTAL; i++)
	{
		m_WantedText[i].DrawClipText(m_WantedText[i].m_StringPosY, BOUNDARY_WANTED_LIST_LINE_POSY_TOP,
									 BOUNDARY_WANTED_LIST_LINE_POSY_BOTTOM, nScrollPos);
	}
}

void CLASS_WANTED_LIST_NAME::DeleteRes()
{
	for(int i = 0; i < CWantedList::TEXT_TOTAL; i++)
	{
		m_WantedText[i].DeleteRes();
	}
}

void CLASS_WANTED_LIST_NAME::SetRegisterNum(char* pNum)
{
	m_WantedText[CWantedList::TEXT_REG_NUM].SetString_Center_Arrange(pNum, RGB(0, 255, 64), RGB(255, 255, 255), TRUE);
}
void CLASS_WANTED_LIST_NAME::SetWantedName(char* pName)
{
	m_WantedText[CWantedList::TEXT_WANTED_NAME].SetString_Center_Arrange(pName, RGB(0, 255, 64), RGB(255, 255, 255), TRUE);
}
void CLASS_WANTED_LIST_NAME::SetWantedMoney(char* pName)
{
	m_WantedText[CWantedList::TEXT_WANTED_MONEY].SetString_Center_Arrange(pName, RGB(0, 255, 64), RGB(255, 255, 255), TRUE);
}
void CLASS_WANTED_LIST_NAME::SetWantedRace(char* pName)
{
	m_WantedText[CWantedList::TEXT_WANTED_RACE].SetString_Center_Arrange(pName, RGB(0, 255, 64), RGB(255, 255, 255), TRUE);
}
void CLASS_WANTED_LIST_NAME::SetConnect(char* pName)
{
	m_WantedText[CWantedList::TEXT_WANTED_CONNECT].SetString_Center_Arrange(pName, RGB(0, 255, 64), RGB(255, 255, 255), TRUE);
}

void CLASS_WANTED_LIST_NAME::SetRemainTime(t_Info remainTime)
{
	char szTmp[256] = {0,};

	if( remainTime.nDay != -1 )
		sprintf(szTmp, "%01d일 %02d:%02d", remainTime.nDay, remainTime.nHour, remainTime.nMin);
	else
		sprintf(szTmp, "0일 00:00");

	m_WantedText[CWantedList::TEXT_WANTED_REMAINTIME].SetString_Center_Arrange(szTmp, RGB(0, 255, 64), RGB(255, 255, 255), TRUE);
}

void CLASS_WANTED_LIST_NAME::SetPosition(int nPosX, int nPosY, int nNum)
{
	m_hWnd =  g_pDisplay->GetHWnd();
	GetClientRect(m_hWnd, &m_ClientRc);

	m_nSequence = nNum;

	m_WantedText[CWantedList::TEXT_REG_NUM].Init(12, RGB(255, 255, 255), m_nBackPosX + 22 , m_nBackPosY + 95 + (17*nNum), 43, 17, TRUE);
	m_WantedText[CWantedList::TEXT_WANTED_NAME].Init(12, RGB(255, 255, 255), m_nBackPosX + 65 , m_nBackPosY + 95 + (17*nNum), 117, 17, TRUE);
	m_WantedText[CWantedList::TEXT_WANTED_MONEY].Init(12, RGB(255, 255, 255), m_nBackPosX + 185 , m_nBackPosY + 95 + (17*nNum), 110, 17, TRUE);
	m_WantedText[CWantedList::TEXT_WANTED_RACE].Init(12, RGB(255, 255, 255), m_nBackPosX + 297 , m_nBackPosY + 95 + (17*nNum), 58, 17, TRUE);
	m_WantedText[CWantedList::TEXT_WANTED_CONNECT].Init(12, RGB(255, 255, 255), m_nBackPosX + 358 , m_nBackPosY + 95 + (17*nNum), 100, 17, TRUE);
	m_WantedText[CWantedList::TEXT_WANTED_REMAINTIME].Init(12, RGB(255, 255, 255), m_nBackPosX + 447 , m_nBackPosY + 95 + (17*nNum), 72, 17, TRUE);
}

void CLASS_WANTED_LIST_NAME::SetBasePosition(int nPosX, int nPosY)
{
	m_nBackPosX = nPosX;
	m_nBackPosY = nPosY;

	m_WantedText[CWantedList::TEXT_REG_NUM].SetPos_Center(			 m_nBackPosX +  22 , m_nBackPosY + 95 + (17*m_nSequence) );
	m_WantedText[CWantedList::TEXT_WANTED_NAME].SetPos_Center(		 m_nBackPosX +  65 , m_nBackPosY + 95 + (17*m_nSequence) );
	m_WantedText[CWantedList::TEXT_WANTED_MONEY].SetPos_Center(	 m_nBackPosX + 185 , m_nBackPosY + 95 + (17*m_nSequence) );
	m_WantedText[CWantedList::TEXT_WANTED_RACE].SetPos_Center(		 m_nBackPosX + 297 , m_nBackPosY + 95 + (17*m_nSequence) );
	m_WantedText[CWantedList::TEXT_WANTED_CONNECT].SetPos_Center(	 m_nBackPosX + 358 , m_nBackPosY + 95 + (17*m_nSequence) );
	m_WantedText[CWantedList::TEXT_WANTED_REMAINTIME].SetPos_Center(m_nBackPosX + 447 , m_nBackPosY + 95 + (17*m_nSequence) );
}
void CLASS_WANTED_LIST_NAME::SetRePosition(int nNum)
{
	m_hWnd =  g_pDisplay->GetHWnd();
	GetClientRect(m_hWnd, &m_ClientRc);

	m_nSequence = nNum;

	m_WantedText[CWantedList::TEXT_REG_NUM].SetPos_Center(m_nBackPosX + 22			 , m_nBackPosY + 95 + (17*nNum));
	m_WantedText[CWantedList::TEXT_WANTED_NAME].SetPos_Center(m_nBackPosX + 65		 , m_nBackPosY + 95 + (17*nNum));
	m_WantedText[CWantedList::TEXT_WANTED_MONEY].SetPos_Center(m_nBackPosX + 185	 , m_nBackPosY + 95 + (17*nNum));
	m_WantedText[CWantedList::TEXT_WANTED_RACE].SetPos_Center(m_nBackPosX + 297		 , m_nBackPosY + 95 + (17*nNum));
	m_WantedText[CWantedList::TEXT_WANTED_CONNECT].SetPos_Center(m_nBackPosX + 358	 , m_nBackPosY + 95 + (17*nNum));
	m_WantedText[CWantedList::TEXT_WANTED_REMAINTIME].SetPos_Center(m_nBackPosX + 447, m_nBackPosY + 95 + (17*nNum));
}

void CLASS_WANTED_LIST_NAME::Restore()
{

}

void CLASS_WANTED_LIST_NAME::SendMsg()
{
	g_pTcpIp->SendNetMessage(m_MsgBuf);
}

LRESULT CLASS_WANTED_LIST_NAME::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, int nScroll)
{
	return -1;
}