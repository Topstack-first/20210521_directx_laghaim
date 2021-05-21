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
#include "LetterReject.h"
#include "ControlLetter.h"




extern HINSTANCE  g_hDllInst;
extern int	g_nServerIndex;		// 서버 인덱스 ( 쪽지 시스템 )

static DWORD back_color = 0x000000;
static DWORD fore_color = RGB(231, 203, 102);
static DWORD gray_color = RGB(155, 155, 155);
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLetterReject::CLetterReject(CControlLetter *Owner)
	: m_nPageNum(0)
	, m_nCurLineNum(0)
{
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
	g_pDisplay->CreateSurfaceFromBitmap(&m_pBack, "interface/LetterSystem/rejection_setting.bmp");
	int X = 0, Y = 0;
	if( m_pBack )
	{
		m_pBack->Xpos = m_ScreenWidth/2 - m_pBack->GetWidth()/2;
		m_pBack->Ypos = m_ScreenHeight/2 - m_pBack->GetHeight()/2;
		X = m_iMainX = m_pBack->Xpos;
		Y = m_iMainY =m_pBack->Ypos;
	}
	//------------------------------------------------------------
	//	TEXOUTBOX
	for(int i=0; i<5; i++)
	{
		m_pDelRejectText[i] = NULL;
		m_pDelRejectText[i] = new CTextOutBox();
		if( m_pDelRejectText[i] )
			m_pDelRejectText[i]->Init(13, RGB(255, 255, 255),X+145, Y+203+(i*20));
	}
	//------------------------------------------------------------
	//	TEXBOX
	m_pAddRejectText = new CTextBox();
	if( m_pAddRejectText )
	{
		m_pAddRejectText->Init(0, 0,100,back_color,_FCOLOR_YELLOW);
		m_pAddRejectText->SetPos(X+144, Y+110);
		m_pAddRejectText->DrawText();
		m_pAddRejectText->SetMaxLen(15*2);
		m_pAddRejectText->SetfontSize(10);
		m_pAddRejectText->SetEnable(false);
	}
	//------------------------------------------------------------
	//	BUTTON
	m_pBtn_Add = NULL;
	m_pBtn_Add = new CBasicButton();
	if( m_pBtn_Add )
	{
		m_pBtn_Add->SetPosition(X+393, Y+105);
		m_pBtn_Add->SetFileName("LetterSystem/button/btn_add");
		m_pBtn_Add->LoadRes();
	}
	m_pBtn_Del = NULL;
	m_pBtn_Del = new CBasicButton();
	if( m_pBtn_Del )
	{
		m_pBtn_Del->SetPosition(X+393, Y+199);
		m_pBtn_Del->SetFileName("LetterSystem/button/btn_delete");
		m_pBtn_Del->LoadRes();
	}
	m_pBtn_Up = NULL;
	m_pBtn_Up = new CBasicButton();
	if( m_pBtn_Up )
	{
		m_pBtn_Up->SetPosition(X+349, Y+203);
		m_pBtn_Up->SetFileName("LetterSystem/button/btn_arrowup");
		m_pBtn_Up->LoadRes();
	}
	m_pBtn_Down = NULL;
	m_pBtn_Down = new CBasicButton();
	if( m_pBtn_Down )
	{
		m_pBtn_Down->SetPosition(X+349, Y+274);
		m_pBtn_Down->SetFileName("LetterSystem/button/btn_arrowdown");
		m_pBtn_Down->LoadRes();
	}

	for(int j=0; j<50; j++)
	{
		m_pBlockList[j] = new char[20];
		ZeroMemory(m_pBlockList[j], sizeof(m_pBlockList[j]));
	}

	m_nRejectLineCount = 0;
	m_nSelectLine = 5;
	m_nStartLine = 0;
	m_nAllRejectNum = 0;		//수신거부 개수
	m_IsAllView = TRUE;
}

CLetterReject::~CLetterReject()
{
	SAFE_DELETE(m_pAddRejectText);
	for(int i=0; i<5; i++)
		SAFE_DELETE(m_pDelRejectText[i]);

	SAFE_DELETE(m_pBtn_Add);
	SAFE_DELETE(m_pBtn_Del);
	for(int j=0; j<50; j++)
		SAFE_DELETE(m_pBlockList[j]);

	SAFE_DELETE(m_pBtn_Up);
	SAFE_DELETE(m_pBtn_Down);
}
void CLetterReject::Draw()
{
	if (!m_bActive || !g_pDisplay)
		return;

	if( m_pBack )
		g_pDisplay->Blt(m_pBack->Xpos, m_pBack->Ypos, m_pBack);

	if( m_pBtn_Add )
		m_pBtn_Add->Draw();
	if( m_pBtn_Del )
		m_pBtn_Del->Draw();

	if( m_pAddRejectText )
		m_pAddRejectText->Draw();

	for(int i=0; i<m_nRejectLineCount; i++)
		if( m_pDelRejectText[i] )
			m_pDelRejectText[i]->Draw();

	if( m_pBtn_Up )
		m_pBtn_Up->Draw();
	if( m_pBtn_Down )
		m_pBtn_Down->Draw();
}
LRESULT CLetterReject::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
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
			if( m_pAddRejectText )
				m_pAddRejectText->MsgProc(hWnd, msg, wParam, lParam);
			if( m_pBtn_Add )
				m_pBtn_Add->MsgProc(hWnd, msg, wParam, lParam);
			if( m_pBtn_Del )
				m_pBtn_Del->MsgProc(hWnd, msg, wParam, lParam);
			if( m_pBtn_Up )
				m_pBtn_Up->MsgProc(hWnd, msg, wParam, lParam);
			if( m_pBtn_Down )
				m_pBtn_Down->MsgProc(hWnd, msg, wParam, lParam);
			return 1;
			break;
		}
		return 0;//빠져나옴
		break;
	case WM_LBUTTONUP:
		if(m_bActive)
		{
			if( m_pBtn_Add )
				m_pBtn_Add->MsgProc(hWnd, msg, wParam, lParam);
			if( m_pBtn_Del )
				m_pBtn_Del->MsgProc(hWnd, msg, wParam, lParam);
			if( m_pBtn_Up )
				m_pBtn_Up->MsgProc(hWnd, msg, wParam, lParam);
			if( m_pBtn_Down )
				m_pBtn_Down->MsgProc(hWnd, msg, wParam, lParam);
			if( m_pAddRejectText )
				m_pAddRejectText->MsgProc(hWnd, msg, wParam, lParam);

			if(m_pBtn_Add && m_pBtn_Add->m_iBtnFlg == BTN_ACTION)
			{
				if(m_pAddRejectText && strlen(m_pAddRejectText->GetString()))
				{
					if( Owner_CL )
					{
						Owner_CL->LetterBagBlockMessage(g_nServerIndex,g_pRoh->m_nUserIndex,g_pRoh->m_pstrName,"block",m_pAddRejectText->GetString());
						Owner_CL->LetterBlockListMessage(g_nServerIndex,g_pRoh->m_pstrName);
						Owner_CL->PutBlockList();
						Owner_CL->ViewBlockList(m_nStartLine,5);
					}
					if( m_pAddRejectText )
						m_pAddRejectText->ClearText();
				}
				if( m_pBtn_Add )
					m_pBtn_Add->m_iBtnFlg = BTN_NORMAL;
			}
			if(m_pBtn_Del && m_pBtn_Del->m_iBtnFlg == BTN_ACTION )
			{
				if(m_nSelectLine < m_nRejectLineCount)
				{
					if( Owner_CL )
					{
						Owner_CL->LetterBagBlockMessage(g_nServerIndex,g_pRoh->m_nUserIndex,g_pRoh->m_pstrName,"bdel",m_pDelRejectText[m_nSelectLine]->m_str);
						Owner_CL->LetterBlockListMessage(g_nServerIndex,g_pRoh->m_pstrName);
						Owner_CL->PutBlockList();
						Owner_CL->ViewBlockList(m_nStartLine,5);
					}
				}
				if( m_pBtn_Del )
					m_pBtn_Del->m_iBtnFlg = BTN_NORMAL;
			}
			if(m_pBtn_Up && m_pBtn_Up->m_iBtnFlg == BTN_ACTION )
			{
				if( m_nStartLine>0)
				{
					m_nStartLine--;
					if( Owner_CL )
					{
						Owner_CL->PutBlockList();
						Owner_CL->ViewBlockList(m_nStartLine,5);
					}
				}
				if( m_pBtn_Up )
					m_pBtn_Up->m_iBtnFlg = BTN_NORMAL;
			}
			if(m_pBtn_Down && m_pBtn_Down->m_iBtnFlg == BTN_ACTION )
			{
				if(m_nStartLine<m_nAllRejectNum)
				{
					m_nStartLine++;
					if( Owner_CL )
					{
						Owner_CL->PutBlockList();
						Owner_CL->ViewBlockList(m_nStartLine,5);
					}
				}
				if( m_pBtn_Down )
					m_pBtn_Down->m_iBtnFlg = BTN_NORMAL;
			}
		}
		if (IsInside(LOWORD (lParam), HIWORD (lParam)) == 0)
		{
			// enable 되었을 상태에서만 FALSE 시켜주고
			// Ime 상태는 영문상태로 바꾼다.
			if( m_pAddRejectText )
				m_pAddRejectText->SetEnable(FALSE);
			pCMyApp->ConvertImmStatus();

			return 0;
		}
		else
		{
			// 체팅박스 입력창 영역을 체크하고
			if (IsInside_Prompt(LOWORD (lParam), HIWORD (lParam)) == 1)
			{
				if( m_pAddRejectText && m_pAddRejectText->GetEnable() == FALSE )
				{
					// enable이 FALSE => TRUE로 간 상태에서만 ime 상태 복귀.
					m_pAddRejectText->SetEnable(TRUE);
					pCMyApp->RestoreImmStatus();
				}
			}
			else
			{
				if (m_pAddRejectText && m_pAddRejectText->GetEnable())
				{
					m_pAddRejectText->SetEnable(FALSE);
					pCMyApp->ConvertImmStatus();
				}
			}
			m_nSelectLine=IsLineInside(x,y);
			if(m_nSelectLine<m_nRejectLineCount)
			{
				//	m_nSelectLine=IsLineInside(x,y);
				for(int i=m_nStartLine; i<m_nRejectLineCount; i++)
				{
					if( m_pDelRejectText[i] )
						m_pDelRejectText[i]->ChangeColor(RGB(255, 255, 255));
				}
				if( m_pDelRejectText[m_nSelectLine] )
					m_pDelRejectText[m_nSelectLine]->ChangeColor(_FCOLOR_YELLOW);
			}

		}
		break;
	case WM_MOUSEMOVE:
		if(IsInside(x, y))
		{
			if( m_pBtn_Add )
				m_pBtn_Add->MsgProc(hWnd, msg, wParam, lParam);
			if( m_pBtn_Del )
				m_pBtn_Del->MsgProc(hWnd, msg, wParam, lParam);
			return 1;
		}
		return 0;
		break;

	case WM_KEYDOWN:
		if( m_pAddRejectText )
			m_pAddRejectText->MsgProc(hWnd, msg, wParam, lParam);
		return 1;
		if(wParam ==VK_TAB || wParam ==VK_RETURN )
			return 1;
		break;


	case WM_CHAR:
		if( m_pAddRejectText )
			m_pAddRejectText->MsgProc(hWnd, msg, wParam, lParam);
		return 1;
	case WM_IME_STARTCOMPOSITION:
	case WM_IME_COMPOSITION:
	case WM_IME_ENDCOMPOSITION:
		if (m_pAddRejectText && m_pAddRejectText->GetEnable())
		{
			m_pAddRejectText->MsgProc( hWnd, msg, wParam, lParam );
			return 1;
		}
		else
			return 0;
	case WM_IME_CHAR:
	case WM_IME_NOTIFY:
		if( m_pAddRejectText )
			m_pAddRejectText->MsgProc( hWnd, msg, wParam, lParam );
		return 1;
		break;
	}
	return 0;
}
BOOL CLetterReject::IsInside(int x, int y)
{
	if( !m_pBack )
		return FALSE;

	if (x >= (int)(m_pBack->Xpos)  && x < (int)(m_pBack->Xpos + m_pBack->GetWidth()) &&
			y >= (int)(m_pBack->Ypos) && y < (int)(m_pBack->Ypos + m_pBack->GetHeight()) )
	{
		return TRUE;
	}
	return FALSE;
}
BOOL CLetterReject::IsInside_Prompt(int x, int y)
{
	if( !m_pBack )
		return FALSE;

	int start_x, start_y;
	int check_cx, check_cy;

	start_y = m_pBack->Ypos+110;
	check_cy = 22;

	start_x = m_pBack->Xpos+144;
	check_cx = 200;

	if ( ( ( x > start_x) && ( x < (start_x + check_cx)) )
			&& ( ( y > start_y ) && ( y < (start_y + check_cy) ) ) )
		return true;
	else
		return false;
}
int CLetterReject::IsLineInside(int x, int y)
{
	if( !m_pBack )
		return FALSE;

	int i = 0;
	for(i=0; i<m_nRejectLineCount; i++)
	{
		if (x >= (int)(m_pBack->Xpos+145)  && x < (int)(m_pBack->Xpos + 245) &&
				y >= (int)(m_pBack->Ypos+203+(i*20)) && y < (int)(m_pBack->Ypos+223+(i*20)) )
		{
			return i;
		}
	}
	return i;
}
