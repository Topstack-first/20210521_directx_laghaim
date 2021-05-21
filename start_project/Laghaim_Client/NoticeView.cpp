#include "stdafx.h"


#include <list>
#include "main.h"
#include "basicbutton.h"
#include "textoutbox.h"
#include "msgpopup.h"
#include "nkcharacter.h"
#include "headers.h"
#include "msgpopup.h"
#include "tcpipcon.h"
#include "nk2dframe.h"
#include "controlbottom.h"
#include "UIMgr.h"
#include "resource.h"
#include "NoticeView.h"
#include "ControlLetter.h"



extern HINSTANCE  g_hDllInst;
extern int	g_nServerIndex;		// 서버 인덱스 ( 쪽지 시스템 )

static DWORD back_color = 0x000000;
static DWORD fore_color = RGB(231, 203, 102);
static DWORD gray_color = RGB(155, 155, 155);
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNoticeView::CNoticeView(CControlLetter *Owner)
	: m_pTextOut(NULL)
	, m_nPageNum(0)
	, m_nCurLineNum(0)
{
	if( !g_pDisplay )
		return;

	Owner_CL = Owner;
	m_bActive = TRUE;
	// ----------------------------------------------------------
	// GET SCREEN SIZE
	RECT rc;
	HWND hWnd =  g_pDisplay->GetHWnd();
	GetClientRect(hWnd, &rc);
	m_ScreenWidth = rc.right;
	m_ScreenHeight = rc.bottom;
	//------------------------------------------------------------
	//	BACKGROUND
	m_pBack		=  NULL;
	g_pDisplay->CreateSurfaceFromBitmap(&m_pBack, "interface/LetterSystem/info_view.bmp");
	if( !m_pBack )
		return;
	m_pBack->Xpos = m_ScreenWidth/2 - m_pBack->GetWidth()/2;
	m_pBack->Ypos = m_ScreenHeight/2 - m_pBack->GetHeight()/2;
	int X = m_iMainX = m_pBack->Xpos;
	int Y = m_iMainY =m_pBack->Ypos;

	//------------------------------------------------------------
	//	TEXT BOX
	m_pTitleText = NULL;
	m_pTitleText = new CTextOutBox();
	if( m_pTitleText )
		m_pTitleText->Init(12, RGB(255, 255, 255),X+186, Y+108);
	//------------------------------------------------------------
	//	LETTER BOX
	m_pNoticeViewText = new CLetterBox();
	if( m_pNoticeViewText )
	{
		m_pNoticeViewText->Init_MEBox(true, 11, 63);
		m_pNoticeViewText->Init(2048, 0,100,back_color,RGB(255, 255, 255));
		m_pNoticeViewText->SetPos(X+60, Y+131);
		m_pNoticeViewText->SetMaxLen(157);
		m_pNoticeViewText->SetFontSize(12);
		m_pNoticeViewText->SetEnable(false);
	}
	//------------------------------------------------------------
	//	BUTTON
	m_pBtn_List = NULL;
	m_pBtn_Prev = NULL;
	m_pBtn_Next = NULL;
	m_pBtn_List = new CBasicButton();
	m_pBtn_Prev = new CBasicButton();
	m_pBtn_Next = new CBasicButton();
	if( m_pBtn_List )		m_pBtn_List->SetPosition(X+42, Y+356);
	if( m_pBtn_Prev )		m_pBtn_Prev->SetPosition(X+381, Y+356);
	if( m_pBtn_Next )		m_pBtn_Next->SetPosition(X+473, Y+356);
	if( m_pBtn_List )		m_pBtn_List->SetFileName("LetterSystem/button/btn_list");
	if( m_pBtn_Prev )		m_pBtn_Prev->SetFileName("LetterSystem/button/btn_prev");
	if( m_pBtn_Next )		m_pBtn_Next->SetFileName("LetterSystem/button/btn_next");
	if( m_pBtn_List )		m_pBtn_List->LoadRes();
	if( m_pBtn_Prev )		m_pBtn_Prev->LoadRes();
	if( m_pBtn_Next )		m_pBtn_Next->LoadRes();

	m_bIsViewPrev = false;
	m_bIsViewNext = false;


	m_pBtn_Up = NULL;
	m_pBtn_Up = new CBasicButton();
	if( m_pBtn_Up )
	{
		m_pBtn_Up->SetPosition(X+509, Y+131);
		m_pBtn_Up->SetFileName("LetterSystem/button/btn_arrowup");
		m_pBtn_Up->LoadRes();
	}

	m_pBtn_Down = NULL;
	m_pBtn_Down = new CBasicButton();
	if( m_pBtn_Down )
	{
		m_pBtn_Down->SetPosition(X+509, Y+288);
		m_pBtn_Down->SetFileName("LetterSystem/button/btn_arrowdown");
		m_pBtn_Down->LoadRes();
	}

	m_nNoticeIndex=0;
	m_pNoticeIndex = new char[10];
	ZeroMemory(m_pNoticeIndex, sizeof(char) * 10);

	m_IsAllView = TRUE;
}

CNoticeView::~CNoticeView()
{
	SAFE_DELETE(m_pBack);
	SAFE_DELETE(m_pTitleText);
	SAFE_DELETE(m_pNoticeViewText);
	SAFE_DELETE(m_pBtn_List);
	SAFE_DELETE(m_pBtn_Prev);
	SAFE_DELETE(m_pBtn_Next);
	SAFE_DELETE(m_pBtn_Up);
	SAFE_DELETE(m_pBtn_Down);
	SAFE_DELETE(m_pTextOut);
}

void CNoticeView::Draw()
{
	if (!m_bActive || !g_pDisplay || !m_pBack)
		return;

	g_pDisplay->Blt(m_pBack->Xpos, m_pBack->Ypos, m_pBack);

	if( m_pBtn_List )
		m_pBtn_List->Draw();

	if( m_pBtn_Prev )
	{
		if(m_bIsViewPrev)
		{
			m_pBtn_Prev->Draw();
			m_pBtn_Prev->SetDisable(false);
		}
		else
			m_pBtn_Prev->SetDisable(true);
	}

	if( m_pBtn_Next )
	{
		if(m_bIsViewNext)
		{
			m_pBtn_Next->Draw();
			m_pBtn_Next->SetDisable(false);
		}
		else
			m_pBtn_Next->SetDisable(true);
	}

	if( m_pTitleText )
		m_pTitleText->Draw();
	if( m_pNoticeViewText )
		m_pNoticeViewText->Draw();
	if( m_pBtn_Up )
		m_pBtn_Up->Draw();
	if( m_pBtn_Down )
		m_pBtn_Down->Draw();
}

LRESULT CNoticeView::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (!m_bActive || !g_pDisplay || !m_pBack)
		return 0;

	int x, y;
	x = LOWORD (lParam);
	y = HIWORD (lParam);

	switch ( msg )
	{
	case WM_LBUTTONDOWN:

		if(IsInside(x, y))
		{
			if( m_pBtn_List )	m_pBtn_List->MsgProc(hWnd, msg, wParam, lParam);
			if( m_pBtn_Prev )	m_pBtn_Prev->MsgProc(hWnd, msg, wParam, lParam);
			if( m_pBtn_Next )	m_pBtn_Next->MsgProc(hWnd, msg, wParam, lParam);
			if( m_pBtn_Up )		m_pBtn_Up->MsgProc(hWnd, msg, wParam, lParam);
			if( m_pBtn_Down )	m_pBtn_Down->MsgProc(hWnd, msg, wParam, lParam);
			break;
		}
		else
			return 0;//빠져나옴

		break;

	case WM_LBUTTONUP:

		if(m_bActive)
		{
			if( m_pBtn_List )	m_pBtn_List->MsgProc(hWnd, msg, wParam, lParam);
			if( m_pBtn_Prev )	m_pBtn_Prev->MsgProc(hWnd, msg, wParam, lParam);
			if( m_pBtn_Next )	m_pBtn_Next->MsgProc(hWnd, msg, wParam, lParam);
			if( m_pBtn_Up )		m_pBtn_Up->MsgProc(hWnd, msg, wParam, lParam);
			if( m_pBtn_Down )	m_pBtn_Down->MsgProc(hWnd, msg, wParam, lParam);

			if(m_pBtn_List && m_pBtn_List->m_iBtnFlg == BTN_ACTION)
			{
				if( Owner_CL )
				{
					Owner_CL->m_IsNoticeList =true;
					Owner_CL->m_IsNoticeView =false;
				}
				m_pBtn_List->SetState(BTN_NORMAL);
			}

			if(m_pBtn_Prev && m_pBtn_Prev->m_iBtnFlg == BTN_ACTION )
			{
				if(m_nNoticeIndex>0)
					m_nNoticeIndex--;
				if( m_pNoticeViewText )
					m_pNoticeViewText->m_MEBox.Set_Start_row(0);
				itoa(m_nNoticeIndex+1,m_pNoticeIndex,10);
				if( Owner_CL )
				{
					Owner_CL->LetterEventViewMessage(m_pNoticeIndex);
					Owner_CL->PutNoticeView();
				}
				m_pBtn_Prev->m_iBtnFlg = BTN_NORMAL;

			}

			if(m_pBtn_Next && m_pBtn_Next->m_iBtnFlg == BTN_ACTION )
			{
				if(m_nNoticeIndex< atoi(Owner_CL->m_NoticeList->m_pNoticeText->m_str))
					m_nNoticeIndex++;
				if( m_pNoticeViewText )
					m_pNoticeViewText->m_MEBox.Set_Start_row(0);
				itoa(m_nNoticeIndex+1,m_pNoticeIndex,10);
				Owner_CL->LetterEventViewMessage(m_pNoticeIndex);
				Owner_CL->PutNoticeView();
				m_pBtn_Next->m_iBtnFlg = BTN_NORMAL;

			}

			if(m_pBtn_Up && m_pBtn_Up->m_iBtnFlg == BTN_ACTION )
			{
				if( m_pNoticeViewText )
					m_pNoticeViewText->m_MEBox.Dec_Start_row();
				m_pBtn_Up->m_iBtnFlg = BTN_NORMAL;
			}

			if(m_pBtn_Down && m_pBtn_Down->m_iBtnFlg == BTN_ACTION )
			{
				if( m_pNoticeViewText )
					m_pNoticeViewText->m_MEBox.Inc_Start_row();
				m_pBtn_Down->m_iBtnFlg = BTN_NORMAL;
			}
		}
		break;

	case WM_MOUSEMOVE:

		if(IsInside(x, y))
		{
			if( m_pBtn_List )
				m_pBtn_List->MsgProc(hWnd, msg, wParam, lParam);
			if( m_pBtn_Prev )
				m_pBtn_Prev->MsgProc(hWnd, msg, wParam, lParam);
			if( m_pBtn_Next )
				m_pBtn_Next->MsgProc(hWnd, msg, wParam, lParam);
			return 1;
		}
		break;

	case WM_KEYDOWN:
		if(wParam ==VK_TAB || wParam ==VK_RETURN )
			return 1;
		break;

	case WM_CHAR:
	case WM_IME_STARTCOMPOSITION:
	case WM_IME_COMPOSITION:
	case WM_IME_ENDCOMPOSITION:
	case WM_IME_CHAR:
	case WM_IME_NOTIFY:
		return 1;
	}
	return 0;
}
BOOL CNoticeView::IsInside(int x, int y)
{
	if( m_pBack )
	{
		if (x >= (int)(m_pBack->Xpos)  && x < (int)(m_pBack->Xpos + m_pBack->GetWidth()) &&
				y >= (int)(m_pBack->Ypos) && y < (int)(m_pBack->Ypos + m_pBack->GetHeight()) )
			return TRUE;
	}
	return FALSE;
}