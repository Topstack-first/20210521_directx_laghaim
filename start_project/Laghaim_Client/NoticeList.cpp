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
#include "NoticeList.h"
#include "nk2dframe.h"
#include "controlbottom.h"
#include "UIMgr.h"
#include "resource.h"
#include "ControlLetter.h"

#define MATLIST_ROOMNUM_WIDTH 28
#define MATLIST_TITLE_WIDTH 290
#define MATLIST_ROOMTYPE_WIDTH 150
#define MATLIST_CURMAXCNT_WIDTH 51
#define MATLIST_MASTER_WIDTH 139
#define MATLIST_STATE_WIDTH 52
#define MATLIST_MINMAXLEVEL_WIDTH 62
#define MATLIST_PAY_WIDTH 80



extern HINSTANCE  g_hDllInst;
extern int	g_nServerIndex;		// 서버 인덱스 ( 쪽지 시스템 )

static DWORD back_color = 0x000000;
static DWORD fore_color = RGB(231, 203, 102);
static DWORD gray_color = RGB(155, 155, 155);

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNoticeList::CNoticeList(CControlLetter *Owner)
	: m_pSecret(NULL)
	, m_pTextOut(NULL)
	, m_iLineCount(0)
{
	if (!g_pDisplay)
		return;

	Owner_CL = Owner;
	m_bActive = TRUE;
	m_bIsViewPrev=false;
	m_bIsViewNext=false;
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
	g_pDisplay->CreateSurfaceFromBitmap(&m_pBack, "interface/LetterSystem/info_list.bmp");
	m_pBack->Xpos = m_ScreenWidth/2 - m_pBack->GetWidth()/2;
	m_pBack->Ypos = m_ScreenHeight/2 - m_pBack->GetHeight()/2;
	int X = m_iMainX = m_pBack->Xpos;
	int Y = m_iMainY =m_pBack->Ypos;
	//-------------------------------------------------------------
	//  Notice Up INFO
	m_pNoticeText = NULL;
	m_pNoticeText = new CTextOutBox();
	m_pNoticeText->Init(13, RGB(255, 255, 255),X+484, Y+115);
	//-------------------------------------------------------------
	//  NoticeLine INFO
	for(int i=0; i<7; i++)
	{
		m_pLineNumText[i] = NULL;
		m_pTitleText[i] = NULL;
		m_pDateText[i] = NULL;
		m_pLineNumText[i] = new CTextOutBox();
		m_pTitleText[i] = new CTextOutBox();
		m_pDateText[i] = new CTextOutBox();
		m_pLineNumText[i]->Init(13, RGB(255, 255, 255),X+62, Y+166+(i*25));
		m_pTitleText[i]->Init(13, RGB(255, 255, 255),X+100, Y+166+(i*25));
		m_pDateText[i]->Init(13, RGB(255, 255, 255),X+420, Y+166+(i*25));
		m_pNoticeLineIndex[i] = new char[10];
		ZeroMemory(m_pNoticeLineIndex[i],sizeof(m_pNoticeLineIndex[i]));
	}
	//------------------------------------------------------------
	//	BUTTON
	m_pBtn_Prev = NULL;
	m_pBtn_Next = NULL;

	m_pBtn_Prev = new CBasicButton();
	m_pBtn_Next = new CBasicButton();

	if( m_pBtn_Prev )
		m_pBtn_Prev->SetFileName("LetterSystem/button/btn_prev");
	if( m_pBtn_Next )
		m_pBtn_Next->SetFileName("LetterSystem/button/btn_next");

	if( m_pBtn_Prev )
		m_pBtn_Prev->LoadRes();
	if( m_pBtn_Next )
		m_pBtn_Next->LoadRes();

	if( m_pBtn_Prev )
		m_pBtn_Prev->SetPosition(X+190, Y+356);
	if( m_pBtn_Next )
		m_pBtn_Next->SetPosition(X+317, Y+356);

	m_strPageNum = new char[10];
	ZeroMemory(m_strPageNum, sizeof(char) * 10);
	m_nPageNum =1;
	m_IsAllView = TRUE;
}

CNoticeList::~CNoticeList()
{
	SAFE_DELETE(m_pBtn_Prev);
	SAFE_DELETE(m_pBtn_Next);
	SAFE_DELETE(m_pNoticeText);

	for(int i=0; i<7; i++)
	{
		SAFE_DELETE(m_pLineNumText[i]);
		SAFE_DELETE(m_pTitleText[i]);
		SAFE_DELETE(m_pDateText[i]);
		SAFE_DELETE(m_pNoticeLineIndex[i]);
	}

	SAFE_DELETE(m_strPageNum);
	// [12/22/2008 D.K ] : memory leak.
//	SAFE_DELETE( m_pBack );
}

void CNoticeList::Draw()
{
	if (!m_bActive)
		return;

	g_pDisplay->Blt(m_pBack->Xpos, m_pBack->Ypos, m_pBack);

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

	if( m_pNoticeText )
		m_pNoticeText->Draw();

	for(int i=0; i<m_iLineCount; i++)
	{
		if( m_pLineNumText[i] )
			m_pLineNumText[i]->Draw();
		if( m_pTitleText[i] )
			m_pTitleText[i]->Draw();
		if( m_pDateText[i] )
			m_pDateText[i]->Draw();
	}
}

LRESULT CNoticeList::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if(!m_bActive)
		return 0;

	int x, y;
	x = LOWORD (lParam);
	y = HIWORD (lParam);

	switch ( msg )
	{
	case WM_LBUTTONDOWN:

		if(IsNoticeLineInside(x,y)<m_iLineCount)
		{
			if( Owner_CL )
			{
				Owner_CL->LetterEventViewMessage(m_pNoticeLineIndex[IsNoticeLineInside(x, y)]);
				Owner_CL->PutNoticeView();
				Owner_CL->m_IsNoticeView=true;
				Owner_CL->m_IsNoticeList=false;
				Owner_CL->m_NoticeView->m_pNoticeViewText->m_MEBox.Set_Start_row(0);
			}
			return 1;
		}

		if(IsInside(x, y))
		{
			if( m_pBtn_Prev )
				m_pBtn_Prev->MsgProc(hWnd, msg, wParam, lParam);
			if( m_pBtn_Next )
				m_pBtn_Next->MsgProc(hWnd, msg, wParam, lParam);

			return 4;
		}
		else
			return 0;//빠져나옴

		break;

	case WM_LBUTTONUP:

		if(m_bActive)
		{
			if( m_pBtn_Prev )
				m_pBtn_Prev->MsgProc(hWnd, msg, wParam, lParam);
			if( m_pBtn_Next )
				m_pBtn_Next->MsgProc(hWnd, msg, wParam, lParam);
		}

		if(m_pBtn_Prev->m_iBtnFlg == BTN_ACTION)
		{
			if (m_nPageNum > 1)
				m_nPageNum--;

			if( Owner_CL )
			{
				Owner_CL->LetterEventListMessage(m_nPageNum);
				Owner_CL->PutNoticeList();
			}

			if( m_pBtn_Prev )
				m_pBtn_Prev->m_iBtnFlg = BTN_NORMAL;

			return 1;
		}
		else if(m_pBtn_Next->m_iBtnFlg == BTN_ACTION)
		{
			m_nPageNum++;
			if( Owner_CL )
			{
				Owner_CL->LetterEventListMessage(m_nPageNum);
				Owner_CL->PutNoticeList();
			}

			if( m_pBtn_Next )
				m_pBtn_Next->m_iBtnFlg = BTN_NORMAL;

			return 1;
		}

		break;

	case WM_MOUSEMOVE:

		if(IsInside(x, y))
		{
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
		break;
	}
	return 0;
}

void CNoticeLineList::AddData(int nNoticeCount, int nLineNum, char *strTitle, char *strDate)
{}

BOOL CNoticeList::IsInside(int x, int y)
{
	if( m_pBack )
	{
		if (x >= (int)(m_pBack->Xpos)  && x < (int)(m_pBack->Xpos + m_pBack->GetWidth()) &&
				y >= (int)(m_pBack->Ypos) && y < (int)(m_pBack->Ypos + m_pBack->GetHeight()) )
			return TRUE;
	}
	return FALSE;
}
int CNoticeList::IsNoticeLineInside(int x, int y)
{
	int i = 0;
	if( m_pBack )
	{
		for(i=0; i<m_iLineCount; i++)
		{
			if (x >= (int)(m_pBack->Xpos+62)  && x < (int)(m_pBack->Xpos + m_pBack->GetWidth()) &&
					y >= (int)(m_pBack->Ypos+166+(i*25)) && y < (int)(m_pBack->Ypos+186+(i*25)) )
				return i;
		}
	}
	else
		return -1;

	return i;
}

CNoticeLineList::CNoticeLineList()
{
	ZeroMemory( m_strTitle, sizeof(m_strTitle) );
	ZeroMemory( m_strDate, sizeof(m_strDate) );
	m_nNoticeCount = 0;
	m_nLineNum = 0;
}

CNoticeLineList::~CNoticeLineList()
{}
