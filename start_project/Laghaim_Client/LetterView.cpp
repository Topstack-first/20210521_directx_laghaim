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
#include "LetterView.h"
#include "ControlLetter.h"




extern HINSTANCE  g_hDllInst;
extern int	g_nServerIndex;		// 서버 인덱스 ( 쪽지 시스템 )

static DWORD back_color = 0x000000;
static DWORD fore_color = RGB(231, 203, 102);
static DWORD gray_color = RGB(155, 155, 155);

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLetterView::CLetterView(CControlLetter *Owner)
	: m_pSecret(NULL)
	, m_nPageNum(0)
	, m_nCurLineNum(0)
{
	Owner_CL = Owner;
	m_bLetterIsNew = false;
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
	if( g_pDisplay )
		g_pDisplay->CreateSurfaceFromBitmap(&m_pBack, "interface/LetterSystem/tag_view.bmp");

	int X = 0, Y = 0;
	if( m_pBack )
	{
		m_pBack->Xpos = m_ScreenWidth/2 - m_pBack->GetWidth()/2;
		m_pBack->Ypos = m_ScreenHeight/2 - m_pBack->GetHeight()/2;
		X = m_iMainX = m_pBack->Xpos;
		Y = m_iMainY =m_pBack->Ypos;
	}

	m_pSenderText = NULL;
	m_pSenderText = new CTextOutBox();
	if( m_pSenderText )
		m_pSenderText->Init(12, RGB(255, 255, 255),X+423, Y+98);
	//------------------------------------------------------------
	//	LETTERBOX
	m_pLetterViewText = new CLetterBox();
	if( m_pLetterViewText )
	{
		m_pLetterViewText->Init_MEBox(true, 11 , 63);
		m_pLetterViewText->Init(0, 0,100,back_color,_FCOLOR_YELLOW);
		m_pLetterViewText->SetPos(X+60, Y+123);
		m_pLetterViewText->SetMaxLen(157);
		m_pLetterViewText->SetFontSize(12);
		m_pLetterViewText->SetEnable(false);
	}

	m_pLetterIndex = new char[10];
	ZeroMemory(m_pLetterIndex, sizeof(char) * 10);
	//------------------------------------------------------------
	//	BUTTON
	m_pBtn_List = NULL;
	m_pBtn_Report = NULL;
	m_pBtn_Reject = NULL;
	m_pBtn_friend = NULL;
	m_pBtn_Reply = NULL;
	m_pBtn_Keep = NULL;
	m_pBtn_Del = NULL;

	m_pBtn_List = new CBasicButton();
	m_pBtn_Report = new CBasicButton();
	m_pBtn_Reject = new CBasicButton();
	m_pBtn_friend = new CBasicButton();
	m_pBtn_Reply = new CBasicButton();
	m_pBtn_Keep = new CBasicButton();
	m_pBtn_Del = new CBasicButton();

	if( m_pBtn_List )
		m_pBtn_List->SetPosition(X+29, Y+352);
	if( m_pBtn_Report )
		m_pBtn_Report->SetPosition(X+119, Y+352);
	if( m_pBtn_Reject )
		m_pBtn_Reject->SetPosition(X+209, Y+352);
	if( m_pBtn_friend )
		m_pBtn_friend->SetPosition(X+300, Y+352);
	if( m_pBtn_Reply )
		m_pBtn_Reply->SetPosition(X+391, Y+352);
	if( m_pBtn_Keep )
		m_pBtn_Keep->SetPosition(X+482, Y+352);
	if( m_pBtn_Del )
		m_pBtn_Del->SetPosition(X+482, Y+352);

	if( m_pBtn_List )
		m_pBtn_List->SetFileName("LetterSystem/button/btn_list");
	if( m_pBtn_Report )
		m_pBtn_Report->SetFileName("LetterSystem/button/btn_report");
	if( m_pBtn_Reject )
		m_pBtn_Reject->SetFileName("LetterSystem/button/btn_m_rejection");
	if( m_pBtn_friend )
		m_pBtn_friend->SetFileName("LetterSystem/button/btn_friend_add");
	if( m_pBtn_Reply )
		m_pBtn_Reply->SetFileName("LetterSystem/button/btn_reply");
	if( m_pBtn_Keep )
		m_pBtn_Keep->SetFileName("LetterSystem/button/btn_keep");
	if( m_pBtn_Del )
		m_pBtn_Del->SetFileName("LetterSystem/button/btn_delete");

	if( m_pBtn_List )
		m_pBtn_List->LoadRes();
	if( m_pBtn_Report )
		m_pBtn_Report->LoadRes();
	if( m_pBtn_Reject )
		m_pBtn_Reject->LoadRes();
	if( m_pBtn_friend )
		m_pBtn_friend->LoadRes();
	if( m_pBtn_Reply )
		m_pBtn_Reply->LoadRes();
	if( m_pBtn_Keep )
		m_pBtn_Keep->LoadRes();
	if( m_pBtn_Del )
		m_pBtn_Del->LoadRes();

	m_pBtn_Up = NULL;
	m_pBtn_Up = new CBasicButton();
	if( m_pBtn_Up )
	{
		m_pBtn_Up->SetPosition(X+509, Y+123);
		m_pBtn_Up->SetFileName("LetterSystem/button/btn_arrowup");
		m_pBtn_Up->LoadRes();
	}

	m_pBtn_Down = NULL;
	m_pBtn_Down = new CBasicButton();
	if( m_pBtn_Down )
	{
		m_pBtn_Down->SetPosition(X+509, Y+283);
		m_pBtn_Down->SetFileName("LetterSystem/button/btn_arrowdown");
		m_pBtn_Down->LoadRes();
	}

	m_IsAllView = TRUE;
}

CLetterView::~CLetterView()
{
	SAFE_DELETE(m_pSenderText);
	SAFE_DELETE(m_pLetterViewText);
	SAFE_DELETE(m_pBtn_List);
	SAFE_DELETE(m_pBtn_Report);
	SAFE_DELETE(m_pBtn_Reject);
	SAFE_DELETE(m_pBtn_friend);
	SAFE_DELETE(m_pBtn_Reply);
	SAFE_DELETE(m_pBtn_Keep);
	SAFE_DELETE(m_pBtn_Del);
	SAFE_DELETE(m_pBtn_Up);
	SAFE_DELETE(m_pBtn_Down);
	SAFE_DELETE(m_pLetterIndex);
	// [12/22/2008 D.K ] : memory leak.
// 	SAFE_DELETE( m_pBack );
	//- end.
}
void CLetterView::Draw()
{
	if (!m_bActive || !g_pDisplay)
		return;

	if( m_pBack )
		g_pDisplay->Blt(m_pBack->Xpos, m_pBack->Ypos, m_pBack);

	if( m_pBtn_List )
		m_pBtn_List->Draw();
	if( m_pBtn_Report )
		m_pBtn_Report->Draw();
	if( m_pBtn_Reject )
		m_pBtn_Reject->Draw();
	if( m_pBtn_friend )
		m_pBtn_friend->Draw();
	if( m_pBtn_Reply )
		m_pBtn_Reply->Draw();
	if(m_pBtn_Keep && m_bLetterIsNew)
	{
		m_pBtn_Keep->Draw();
		m_pBtn_Keep->SetDisable(false);
		if( m_pBtn_Del )
			m_pBtn_Del->SetDisable(true);
	}
	else
	{
		if( m_pBtn_Del )
		{
			m_pBtn_Del->Draw();
			m_pBtn_Del->SetDisable(false);
		}
		if( m_pBtn_Keep )
			m_pBtn_Keep->SetDisable(true);
	}
	if( m_pSenderText )
		m_pSenderText->Draw();
	if( m_pLetterViewText )
		m_pLetterViewText->Draw();

	if( m_pBtn_Up )
		m_pBtn_Up->Draw();
	if( m_pBtn_Down )
		m_pBtn_Down->Draw();
}
LRESULT CLetterView::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int x, y;

	if(!m_bActive)
		return 0;

	x = LOWORD (lParam);
	y = HIWORD (lParam);


	switch ( msg )
	{
	case WM_LBUTTONDOWN:
		if(IsInside(x, y))
		{
			if( m_pBtn_List )
				m_pBtn_List->MsgProc(hWnd, msg, wParam, lParam);
			if( m_pBtn_Report )
				m_pBtn_Report->MsgProc(hWnd, msg, wParam, lParam);
			if( m_pBtn_Reject )
				m_pBtn_Reject->MsgProc(hWnd, msg, wParam, lParam);
			if( m_pBtn_friend )
				m_pBtn_friend->MsgProc(hWnd, msg, wParam, lParam);
			if( m_pBtn_Reply )
				m_pBtn_Reply->MsgProc(hWnd, msg, wParam, lParam);
			if( m_pBtn_Keep )
				m_pBtn_Keep->MsgProc(hWnd, msg, wParam, lParam);
			if( m_pBtn_Del )
				m_pBtn_Del->MsgProc(hWnd, msg, wParam, lParam);
			if( m_pBtn_Up )
				m_pBtn_Up->MsgProc(hWnd, msg, wParam, lParam);
			if( m_pBtn_Down )
				m_pBtn_Down->MsgProc(hWnd, msg, wParam, lParam);
			break;
		}
		else
			return 0;//빠져나옴

		break;

	case WM_LBUTTONUP:

		if(m_bActive)
		{
			if( m_pBtn_List )
				m_pBtn_List->MsgProc(hWnd, msg, wParam, lParam);
			if( m_pBtn_Report )
				m_pBtn_Report->MsgProc(hWnd, msg, wParam, lParam);
			if( m_pBtn_Reject )
				m_pBtn_Reject->MsgProc(hWnd, msg, wParam, lParam);
			if( m_pBtn_friend )
				m_pBtn_friend->MsgProc(hWnd, msg, wParam, lParam);
			if( m_pBtn_Reply )
				m_pBtn_Reply->MsgProc(hWnd, msg, wParam, lParam);
			if( m_pBtn_Keep )
				m_pBtn_Keep->MsgProc(hWnd, msg, wParam, lParam);
			if( m_pBtn_Del )
				m_pBtn_Del->MsgProc(hWnd, msg, wParam, lParam);
			if( m_pBtn_Up )
				m_pBtn_Up->MsgProc(hWnd, msg, wParam, lParam);
			if( m_pBtn_Down )
				m_pBtn_Down->MsgProc(hWnd, msg, wParam, lParam);

			if(m_pBtn_List && m_pBtn_List->m_iBtnFlg == BTN_ACTION)
			{
				if( Owner_CL )
				{
					Owner_CL->m_IsLetterList =true;
					Owner_CL->m_IsLetterView =false;
				}
				if( m_pBtn_List )
					m_pBtn_List->SetState(BTN_NORMAL);
				//------------
				//List Refresh
				if( Owner_CL )
				{
					Owner_CL->LetterListMessage(g_nServerIndex,g_pRoh->m_pstrName,g_pRoh->m_nUserIndex,Owner_CL->m_LetterList->m_nPageNum);
					Owner_CL->PutList();
				}
			}

			if(m_pBtn_Report && m_pBtn_Report->m_iBtnFlg == BTN_ACTION)
			{
				if( Owner_CL )
				{
					Owner_CL->m_IsLetterList =true;
					Owner_CL->m_IsLetterView =false;
					Owner_CL->LetterCrimeMessage(g_nServerIndex,m_pLetterIndex);
				}
				if( m_pBtn_Report )
					m_pBtn_Report->SetState(BTN_NORMAL);
				//------------
				//List Refresh
				if( Owner_CL )
				{
					Owner_CL->LetterListMessage(g_nServerIndex,g_pRoh->m_pstrName,g_pRoh->m_nUserIndex,Owner_CL->m_LetterList->m_nPageNum);
					Owner_CL->PutList();
				}
			}
			if(m_pBtn_Reject && m_pBtn_Reject->m_iBtnFlg == BTN_ACTION)
			{
				if( Owner_CL )
				{
					Owner_CL->m_IsLetterReject =true;
					Owner_CL->m_IsLetterView =false;
					Owner_CL->m_LetterReject->m_pAddRejectText->ClearText();
					Owner_CL->m_LetterReject->m_pAddRejectText->SetText(m_pSenderText->m_str);
				}
				if( m_pBtn_Reject )
					m_pBtn_Reject->SetState(BTN_NORMAL);
			}
			if(m_pBtn_friend && m_pBtn_friend->m_iBtnFlg == BTN_ACTION)
			{
				if( Owner_CL )
				{
					Owner_CL->m_IsLetterFriend =true;
					Owner_CL->m_IsLetterView =false;
					Owner_CL->m_LetterFriend->m_pAddFriendText->ClearText();
					Owner_CL->m_LetterFriend->m_pAddFriendText->SetText(m_pSenderText->m_str);
				}
				if( m_pBtn_friend )
					m_pBtn_friend->SetState(BTN_NORMAL);
			}
			if(m_pBtn_Reply && m_pBtn_Reply->m_iBtnFlg == BTN_ACTION)
			{
				if( Owner_CL )
				{
					Owner_CL->m_IsLetterSend =true;
					Owner_CL->m_IsLetterView =false;
					Owner_CL->m_LetterSend->m_pRecNameText->ClearText();
					Owner_CL->m_LetterSend->m_pRecNameText->SetText(m_pSenderText->m_str);
				}
				if( m_pBtn_Reply )
					m_pBtn_Reply->SetState(BTN_NORMAL);
			}
			if(m_pBtn_Keep && m_pBtn_Keep->m_iBtnFlg == BTN_ACTION)
			{
				if( Owner_CL )
				{
					Owner_CL->LetterKeepDelMessage(g_nServerIndex,m_pLetterIndex,g_pRoh->m_nUserIndex,"keep",g_pRoh->m_pstrName);
					//------------
					//List Refresh
					Owner_CL->LetterListMessage(g_nServerIndex,g_pRoh->m_pstrName,g_pRoh->m_nUserIndex,Owner_CL->m_LetterList->m_nPageNum);
					Owner_CL->PutList();
					Owner_CL->m_IsLetterList =true;
					Owner_CL->m_IsLetterView =false;
				}
				if( m_pBtn_Keep )
					m_pBtn_Keep->SetState(BTN_NORMAL);

			}
			if(m_pBtn_Del && m_pBtn_Del->m_iBtnFlg == BTN_ACTION)
			{
				if( Owner_CL )
				{
					Owner_CL->m_IsLetterList =true;
					Owner_CL->m_IsLetterView =false;
				}
				if( m_pBtn_Del )
					m_pBtn_Del->SetState(BTN_NORMAL);
				if( Owner_CL )
				{
					Owner_CL->LetterKeepDelMessage(g_nServerIndex,m_pLetterIndex,g_pRoh->m_nUserIndex,"del",g_pRoh->m_pstrName);
					//------------
					//List Refresh
					Owner_CL->LetterListMessage(g_nServerIndex,g_pRoh->m_pstrName,g_pRoh->m_nUserIndex,Owner_CL->m_LetterList->m_nPageNum);
					Owner_CL->PutList();
				}
			}
			if(m_pBtn_Up && m_pBtn_Up->m_iBtnFlg == BTN_ACTION )
			{
				if( m_pLetterViewText )
					m_pLetterViewText->m_MEBox.Dec_Start_row();
				if( m_pBtn_Up )
					m_pBtn_Up->m_iBtnFlg = BTN_NORMAL;
			}
			if(m_pBtn_Down && m_pBtn_Down->m_iBtnFlg == BTN_ACTION )
			{
				if( m_pLetterViewText )
					m_pLetterViewText->m_MEBox.Inc_Start_row();
				if( m_pBtn_Down )
					m_pBtn_Down->m_iBtnFlg = BTN_NORMAL;
			}
		}
		break;
	case WM_MOUSEMOVE:
		if(IsInside(x, y))
		{
			if( m_pBtn_List )
				m_pBtn_List->MsgProc(hWnd, msg, wParam, lParam);
			if( m_pBtn_Report )
				m_pBtn_Report->MsgProc(hWnd, msg, wParam, lParam);
			if( m_pBtn_Reject )
				m_pBtn_Reject->MsgProc(hWnd, msg, wParam, lParam);
			if( m_pBtn_friend )
				m_pBtn_friend->MsgProc(hWnd, msg, wParam, lParam);
			if( m_pBtn_Reply )
				m_pBtn_Reply->MsgProc(hWnd, msg, wParam, lParam);
			if( m_pBtn_Keep )
				m_pBtn_Keep->MsgProc(hWnd, msg, wParam, lParam);
			if( m_pBtn_Del )
				m_pBtn_Del->MsgProc(hWnd, msg, wParam, lParam);
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
BOOL CLetterView::IsInside(int x, int y)
{
	if( !m_pBack )
		return false;

	if (x >= (int)(m_pBack->Xpos)  && x < (int)(m_pBack->Xpos + m_pBack->GetWidth()) &&
			y >= (int)(m_pBack->Ypos) && y < (int)(m_pBack->Ypos + m_pBack->GetHeight()) )
	{
		return TRUE;
	}
	return FALSE;
}