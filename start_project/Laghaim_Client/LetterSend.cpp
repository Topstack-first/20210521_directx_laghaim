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
#include "LetterSend.h"
#include "ControlLetter.h"




extern HINSTANCE  g_hDllInst;
extern int	g_nServerIndex;		// 서버 인덱스 ( 쪽지 시스템 )

static DWORD back_color = 0x000000;
static DWORD fore_color = RGB(231, 203, 102);
static DWORD gray_color = RGB(155, 155, 155);
static DWORD white_color = RGB(255, 255, 255);
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLetterSend::CLetterSend(CControlLetter *Owner)
	: m_pSecret(NULL)
	, m_pTextOut(NULL)
	, m_nPageNum(0)
	, m_nCurLineNum(0)
{
	Owner_CL=Owner;
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
		g_pDisplay->CreateSurfaceFromBitmap(&m_pBack, "interface/LetterSystem/tag_Write.bmp");
	int X = 0, Y = 0;
	if( m_pBack )
	{
		m_pBack->Xpos = m_ScreenWidth/2 - m_pBack->GetWidth()/2;
		m_pBack->Ypos = m_ScreenHeight/2 - m_pBack->GetHeight()/2;
		X = m_iMainX = m_pBack->Xpos;
		Y = m_iMainY =m_pBack->Ypos;
	}
	//------------------------------------------------------------
	//	TEXTOUTBOX
	m_pCharNameText = NULL;
	m_pCharNameText = new CTextOutBox();
	if( m_pCharNameText )
		m_pCharNameText->Init(12,white_color,X+108, Y+105);

	//------------------------------------------------------------
	//	TEXBOX
	m_pRecNameText = NULL;
	m_pRecNameText = new CTextBox();
	if( m_pRecNameText )
	{
		m_pRecNameText->Init(0, 0,100,back_color,_FCOLOR_YELLOW);
		m_pRecNameText->SetPos(X+108, Y+145);
		m_pRecNameText->DrawText();
		m_pRecNameText->SetMaxLen(7*2);
		m_pRecNameText->SetfontSize(10);
		m_pRecNameText->SetEnable(false);
	}


	//------------------------------------------------------------
	//	LETTERBOX
	m_pSendLetterText = NULL;
	m_pSendLetterText = new CLetterBox();
	if( m_pSendLetterText )
	{
		m_pSendLetterText->Init_MEBox(false, 9, 53);
		m_pSendLetterText->Init(0, 0,100,back_color,_FCOLOR_YELLOW);
		m_pSendLetterText->SetPos(X+108, Y+180);
		m_pSendLetterText->DrawText();
		m_pSendLetterText->SetMaxLen(127);
		m_pSendLetterText->SetFontSize(12);
		m_pSendLetterText->SetEnable(false);
	}

	//------------------------------------------------------------
	//	BUTTON
	m_pBtn_Send = NULL;
	m_pBtn_Send = new CBasicButton();
	if( m_pBtn_Send )
	{
		m_pBtn_Send->SetPosition(X+487, Y+348);
		m_pBtn_Send->SetFileName("LetterSystem/button/btn_send");
		m_pBtn_Send->LoadRes();
	}

	m_pBtn_List = NULL;
	m_pBtn_List = new CBasicButton();
	if( m_pBtn_List )
	{
		m_pBtn_List->SetPosition(X+385, Y+143);
		m_pBtn_List->SetFileName("LetterSystem/button/btn_friend_list");
		m_pBtn_List->LoadRes();
	}

	m_pBtn_Up = NULL;
	m_pBtn_Up = new CBasicButton();
	if( m_pBtn_Up )
	{
		m_pBtn_Up->SetPosition(X+365, Y+160);
		m_pBtn_Up->SetFileName("LetterSystem/button/btn_arrowup");
		m_pBtn_Up->LoadRes();
	}

	m_pBtn_Down = NULL;
	m_pBtn_Down = new CBasicButton();
	if( m_pBtn_Down )
	{
		m_pBtn_Down->SetPosition(X+365, Y+283);
		m_pBtn_Down->SetFileName("LetterSystem/button/btn_arrowdown");
		m_pBtn_Down->LoadRes();
	}
	//------------------------------------------------------------
	//	LIST BACK
	m_pListBack	= NULL;
	if( g_pDisplay )
		g_pDisplay->CreateSurfaceFromBitmap(&m_pListBack, "interface/LetterSystem/friend_list_back.bmp");

	int ListX = 0, ListY = 0;
	if( m_pListBack )
	{
		m_pListBack->Xpos =X+279;
		m_pListBack->Ypos =Y+158;
		ListX = m_pListBack->Xpos;
		ListY = m_pListBack->Ypos;
	}
	//------------------------------------------------------------
	//	TEXBOX(LIST)
	for(int i=0; i<8; i++)
	{
		m_pViewFriendList[i] = NULL;
		m_pViewFriendList[i] = new CTextOutBox();
		if( m_pViewFriendList[i] )
			m_pViewFriendList[i]->Init(12, white_color,ListX, ListY+(i*18));
	}

	for(int j=0; j<50; j++)
	{
		m_pFriendList[j] = new char[20];
		ZeroMemory(m_pFriendList[j], sizeof(m_pFriendList[j]));
	}

	m_nSelectLine=5;
	m_nFriendLineCount = 0;
	m_nStartLine=0;
	m_nAllFriendNum=0;
	m_bViewList=false;
	m_IsAllView = TRUE;
}

CLetterSend::~CLetterSend()
{
	SAFE_DELETE(m_pCharNameText);
	SAFE_DELETE(m_pRecNameText);
	SAFE_DELETE(m_pBtn_Send);
	SAFE_DELETE(m_pBtn_List);

	for(int i=0; i<8; i++)
		SAFE_DELETE(m_pViewFriendList[i]);

	SAFE_DELETE(m_pBtn_Up);
	SAFE_DELETE(m_pBtn_Down);
	// [12/22/2008 D.K ] : memory leak.
//	SAFE_DELETE( m_pSendLetterText );
// 	SAFE_DELETE( m_pListBack );
//	SAFE_DELETE( m_pBack );
	// end.
}
void CLetterSend::Draw()
{
	if (!m_bActive || !g_pDisplay)
		return;

	if( m_pCharNameText )
		m_pCharNameText->SetString(g_pRoh->m_pstrName,fore_color,back_color);
	if( m_pBack )
		g_pDisplay->Blt(m_pBack->Xpos, m_pBack->Ypos, m_pBack);
	if( m_pBtn_Send )
		m_pBtn_Send->Draw();
	if( m_pBtn_List )
		m_pBtn_List->Draw();
	if( m_pCharNameText )
		m_pCharNameText->Draw();
	if( m_pRecNameText )
		m_pRecNameText->Draw();
	if( m_pSendLetterText )
		m_pSendLetterText->Draw();
	if(m_bViewList && m_pListBack)
	{
		g_pDisplay->Blt(m_pListBack->Xpos,m_pListBack->Ypos,m_pListBack);
		if( m_pBtn_Up )
			m_pBtn_Up->Draw();
		if( m_pBtn_Down )
			m_pBtn_Down->Draw();
		if( m_pBtn_Up )
			m_pBtn_Up->SetDisable(false);
		if( m_pBtn_Down )
			m_pBtn_Down->SetDisable(false);
		for(int i=0; i<8; i++)
		{
			if(strcmp(m_pViewFriendList[i]->m_str,""))
				if( m_pViewFriendList[i] )
					m_pViewFriendList[i]->Draw();
		}
	}
	else
	{
		if( m_pBtn_Up )
			m_pBtn_Up->SetDisable(true);
		if( m_pBtn_Down )
			m_pBtn_Down->SetDisable(true);
	}
}
LRESULT CLetterSend::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
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
			if( m_pRecNameText )
				m_pRecNameText->MsgProc( hWnd, msg, wParam, lParam );
			if( m_pSendLetterText )
				m_pSendLetterText->MsgProc( hWnd, msg, wParam, lParam );
			if( m_pBtn_Send )
				m_pBtn_Send->MsgProc(hWnd, msg, wParam, lParam);
			if( m_pBtn_List )
				m_pBtn_List->MsgProc(hWnd, msg, wParam, lParam);
			if( m_pBtn_Up )
				m_pBtn_Up->MsgProc(hWnd, msg, wParam, lParam);
			if( m_pBtn_Down )
				m_pBtn_Down->MsgProc(hWnd, msg, wParam, lParam);
		}
		break;

	case WM_LBUTTONUP:

		if(m_bActive)
		{
			if( m_pBtn_Send )
				m_pBtn_Send->MsgProc(hWnd, msg, wParam, lParam);
			if( m_pBtn_List )
				m_pBtn_List->MsgProc(hWnd, msg, wParam, lParam);
			if( m_pRecNameText )
				m_pRecNameText->MsgProc(hWnd, msg, wParam, lParam);
			if( m_pSendLetterText )
				m_pSendLetterText->MsgProc( hWnd, msg, wParam, lParam );
			if( m_pBtn_Up )
				m_pBtn_Up->MsgProc(hWnd, msg, wParam, lParam);
			if( m_pBtn_Down )
				m_pBtn_Down->MsgProc(hWnd, msg, wParam, lParam);
			if(m_pBtn_Send && m_pBtn_Send->m_iBtnFlg == BTN_ACTION )
			{
				if( m_pSendLetterText && strlen(m_pSendLetterText->GetString())<255)
				{
					if( Owner_CL )
					{
						Owner_CL->LetterSendMessage(g_nServerIndex,g_pRoh->m_nUserIndex,pCMyApp->m_strLetterUserId,g_pRoh->m_pstrName,m_pRecNameText->GetString(),m_pSendLetterText->m_MEBox.Get_CoreString());
						Owner_CL->m_IsLetterList=true;
						Owner_CL->m_IsLetterSend=false;

						// List Refresh 추가. // 07-05-22
						Owner_CL->LetterListMessage(g_nServerIndex,g_pRoh->m_pstrName,g_pRoh->m_nUserIndex,Owner_CL->m_LetterList->m_nPageNum);
						Owner_CL->PutList();
					}
					if( m_pSendLetterText )
						m_pSendLetterText->m_MEBox.Reset();
				}
				if( m_pBtn_Send )
					m_pBtn_Send->m_iBtnFlg =BTN_NORMAL;
				return 1;
			}
			if(m_pBtn_List && m_pBtn_List->m_iBtnFlg == BTN_ACTION)
			{
				if(m_bViewList)
					m_bViewList=false;
				else
					m_bViewList=true;

				if( Owner_CL )
				{
					Owner_CL->LetterFriendListMessage(g_nServerIndex,pCMyApp->m_strLetterUserId,g_pRoh->m_nUserIndex);
					Owner_CL->PutFriendList();
					Owner_CL->ViewSendFriendList(m_nStartLine,8);
				}
				if( m_pBtn_List )
					m_pBtn_List->m_iBtnFlg =BTN_NORMAL;
			}
			if(m_pBtn_Up && m_pBtn_Up->m_iBtnFlg == BTN_ACTION )
			{
				if( m_nStartLine>0)
				{
					m_nStartLine--;
					if( Owner_CL )
					{
						Owner_CL->PutFriendList();
						Owner_CL->ViewFriendList(m_nStartLine,5);
					}
				}
				if( m_pBtn_Up )
					m_pBtn_Up->m_iBtnFlg = BTN_NORMAL;
			}
			if(m_pBtn_Down && m_pBtn_Down->m_iBtnFlg == BTN_ACTION )
			{
				if(m_nStartLine<m_nAllFriendNum)
				{
					m_nStartLine++;
					if( Owner_CL )
					{
						Owner_CL->PutFriendList();
						Owner_CL->ViewFriendList(m_nStartLine,5);
					}
				}
				if( m_pBtn_Down )
					m_pBtn_Down->m_iBtnFlg = BTN_NORMAL;
			}
		}

		if (IsInside(LOWORD (lParam), HIWORD (lParam)) == 0)
		{
			if( m_pRecNameText )
				m_pRecNameText->SetEnable(FALSE);
			if( m_pSendLetterText )
				m_pSendLetterText->SetEnable(FALSE);
			pCMyApp->ConvertImmStatus();
			return 0;
		}
		else
		{
			m_nSelectLine=IsLineInside(x,y);
			if(m_nSelectLine<m_nFriendLineCount && m_bViewList == TRUE)
			{
				if( m_pRecNameText )
				{
					m_pRecNameText->ClearText();
					m_pRecNameText->SetText(m_pViewFriendList[m_nSelectLine]->m_str);
				}
				m_bViewList=false;
			}
			// 채팅박스 입력창 영역을 체크하고
			else if (IsInside_RecName_Prompt(LOWORD (lParam), HIWORD (lParam)) == 1)
			{
				if( m_pSendLetterText )
					m_pSendLetterText->SetEnable(FALSE);
				if( m_pRecNameText )
					m_pRecNameText->SetEnable(TRUE);
				m_bViewList=false;
			}
			else if (IsInside_Letter_Prompt(LOWORD (lParam), HIWORD (lParam)) == 1)
			{
				if( m_pRecNameText )
					m_pRecNameText->SetEnable(FALSE);
				if( m_pSendLetterText )
					m_pSendLetterText->SetEnable(TRUE);
				m_bViewList=false;
			}
			else
			{
				if( m_pRecNameText )
					m_pRecNameText->SetEnable(FALSE);
				if( m_pSendLetterText )
					m_pSendLetterText->SetEnable(FALSE);
				pCMyApp->ConvertImmStatus();
			}

			return 1;
		}
		return 0;
		break;
	case WM_MOUSEMOVE:
		if(IsInside(x, y))
		{
			if( m_pBtn_Send )
				m_pBtn_Send->MsgProc(hWnd, msg, wParam, lParam);
			return 1;
		}
		return 0;
		break;

	case WM_KEYDOWN:
		if (m_pRecNameText && m_pRecNameText->GetEnable())
		{
			m_pRecNameText->MsgProc( hWnd, msg, wParam, lParam );
			return 1;
		}
		if (m_pSendLetterText && m_pSendLetterText->GetEnable())
		{
			m_pSendLetterText->MsgProc( hWnd, msg, wParam, lParam );
			return 1;
		}
		return 0;

		break;

	case WM_CHAR:
		if (m_pRecNameText && m_pRecNameText->GetEnable())
		{
			m_pRecNameText->MsgProc( hWnd, msg, wParam, lParam );
			return 1;
		}
		if (m_pSendLetterText && m_pSendLetterText->GetEnable())
		{
			m_pSendLetterText->MsgProc( hWnd, msg, wParam, lParam );
			return 1;
		}
		return 0;
		break;
	case WM_IME_STARTCOMPOSITION:
	case WM_IME_COMPOSITION:
	case WM_IME_ENDCOMPOSITION:
	case WM_IME_CHAR:
	case WM_IME_NOTIFY:
		if( m_pRecNameText )
			m_pRecNameText->MsgProc( hWnd, msg, wParam, lParam );
		if( m_pSendLetterText )
			m_pSendLetterText->MsgProc( hWnd, msg, wParam, lParam );
		return 1;
		break;

	}
	return 0;
}
BOOL CLetterSend::IsInside(int x, int y)
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
BOOL CLetterSend::IsInside_RecName_Prompt(int x, int y)
{
	if( !m_pBack )
		return false;

	int start_x, start_y;
	int check_cx, check_cy;

	start_y = m_pBack->Ypos+143;
	check_cy = 25;

	start_x = m_pBack->Xpos+108;
	check_cx = 110;

	if ( ( ( x > start_x) && ( x < (start_x + check_cx)) )
			&& ( ( y > start_y ) && ( y < (start_y + check_cy) ) ) )
		return true;
	else
		return false;
}

BOOL CLetterSend::IsInside_Letter_Prompt(int x, int y)
{
	if( !m_pBack )
		return false;

	int start_x, start_y;
	int check_cx, check_cy;

	start_y = m_pBack->Ypos+178;
	check_cy = 200;

	start_x = m_pBack->Xpos+108;
	check_cx = 170;

	if ( ( ( x > start_x) && ( x < (start_x + check_cx)) )
			&& ( ( y > start_y ) && ( y < (start_y + check_cy) ) ) )
		return true;
	else
		return false;
}
int CLetterSend::IsLineInside(int x, int y)
{
	if( !m_pListBack )
		return 0;

	int i = 0;
	for(i=0; i<m_nFriendLineCount; i++)
	{
		if (x >= (int)(m_pListBack->Xpos)  && x < (int)(m_pListBack->Xpos + 60) &&
				y >= (int)(m_pListBack->Ypos) && y < (int)(m_pListBack->Ypos+18+(i*18)) )
		{
			return i;
		}
	}
	return i;
}