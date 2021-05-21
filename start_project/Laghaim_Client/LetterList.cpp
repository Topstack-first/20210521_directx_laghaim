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
#include "LetterList.h"
#include "nk2dframe.h"
#include "controlbottom.h"
#include "UIMgr.h"
#include "resource.h"
#include "ControlLetter.h"
#include "Country.h" // 쪽지 수신확인 때문에 추가.
#include "g_stringmanager.h"

#define MATLIST_ROOMNUM_WIDTH 28
#define MATLIST_TITLE_WIDTH 290
#define MATLIST_ROOMTYPE_WIDTH 150
#define MATLIST_CURMAXCNT_WIDTH 51
#define MATLIST_MASTER_WIDTH 139
#define MATLIST_STATE_WIDTH 52
#define MATLIST_MINMAXLEVEL_WIDTH 62
#define MATLIST_PAY_WIDTH 80




extern DWORD	g_dwClientCountry; // 쪽지 수신확인 때문에 추가.
extern HINSTANCE  g_hDllInst;
extern int	g_nServerIndex;		// 서버 인덱스 ( 쪽지 시스템 )
static DWORD back_color = 0x000000;
static DWORD fore_color = RGB(231, 203, 102);
static DWORD gray_color = RGB(155, 155, 155);

char g_StateType[2][10] = { "신규", "보관" };
int m_nLinePosX[4] = {164,190,215,240};
int m_nLinePosY[7] = {164,190,215,240,265,290,315};

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLetterList::CLetterList(CControlLetter *Owner)
	: m_pSecret(NULL)
	, m_nCurLineNum(0)
{
	memset(m_ListNum, 0, sizeof(m_ListNum));

	Owner_CL = Owner;
	m_nPageNum	=1;
	m_bActive = TRUE;
	m_iLineCount=0;
	// ----------------------------------------------------------
	// GET SCREEN SIZE
	RECT rc;
	HWND hWnd = NULL;
	if( g_pDisplay )
		hWnd = g_pDisplay->GetHWnd();
	GetClientRect(hWnd, &rc);
	m_ScreenWidth = rc.right;
	m_ScreenHeight = rc.bottom;
	//------------------------------------------------------------
	//	BACKGROUND
	m_pBack		=  NULL;
	if( g_pDisplay )
		g_pDisplay->CreateSurfaceFromBitmap(&m_pBack, "interface/LetterSystem/tag_list.bmp");
	int X = 0, Y = 0;
	if( m_pBack )
	{
		m_pBack->Xpos = m_ScreenWidth/2 - m_pBack->GetWidth()/2;
		m_pBack->Ypos = m_ScreenHeight/2 - m_pBack->GetHeight()/2;
		X = m_iMainX = m_pBack->Xpos;
		Y = m_iMainY = m_pBack->Ypos;
	}
	//-------------------------------------------------------------
	//  UP INFO
	m_pNewText = NULL;
	m_pKeepText = NULL;
	m_pNoticeText = NULL;
	m_pNewText = new CTextOutBox();
	m_pKeepText = new CTextOutBox();
	m_pNoticeText = new CTextOutBox();
	if( m_pNewText )
		m_pNewText->Init(13, RGB(255, 255, 255),X+408, Y+89);
	if( m_pKeepText )
		m_pKeepText->Init(13, RGB(255, 255, 255),X+488, Y+89);
	if( m_pNoticeText )
		m_pNoticeText->Init(13, RGB(255, 255, 255),X+488, Y+111);
	m_bIsViewPrev = false;
	m_bIsViewNext = false;
	//-------------------------------------------------------------
	//  LIST INFO
	for(int i=0; i<7; i++)
	{
		m_pLineNumText[i] = NULL;
		m_pChNameText[i] = NULL;
		m_pDateText[i] = NULL;
		m_pStateText[i] = NULL;
		m_pLineNumText[i] = new CTextOutBox();
		m_pChNameText[i] = new CTextOutBox();
		m_pDateText[i] = new CTextOutBox();
		m_pStateText[i] = new CTextOutBox();
		if( m_pLineNumText[i] )
			m_pLineNumText[i]->Init(13, RGB(255, 255, 255),X+62, Y+166+(i*25));
		if( m_pChNameText[i] )
			m_pChNameText[i]->Init(13, RGB(255, 255, 255),X+120, Y+166+(i*25));
		if( m_pDateText[i] )
			m_pDateText[i]->Init(13, RGB(255, 255, 255),X+280, Y+166+(i*25));
		if( m_pStateText[i] )
			m_pStateText[i]->Init(13, RGB(255, 255, 255),X+490, Y+166+(i*25));

		m_pLineIndex[i] = new char[10];
		ZeroMemory(m_pLineIndex[i], sizeof(m_pLineIndex[i]));
	}

	//------------------------------------------------------------
	//	BUTTON
	m_pBtn_Prev = NULL;
	m_pBtn_Next = NULL;
	m_pBtn_Prev = new CBasicButton();
	m_pBtn_Next = new CBasicButton();
	if( m_pBtn_Prev )
		m_pBtn_Prev->SetPosition(X+190, Y+356);
	if( m_pBtn_Next )
		m_pBtn_Next->SetPosition(X+317, Y+356);
	if( m_pBtn_Prev )
		m_pBtn_Prev->SetFileName("LetterSystem/button/btn_prev");
	if( m_pBtn_Next )
		m_pBtn_Next->SetFileName("LetterSystem/button/btn_next");
	if( m_pBtn_Prev )
		m_pBtn_Prev->LoadRes();
	if( m_pBtn_Next )
		m_pBtn_Next->LoadRes();
	m_IsAllView = TRUE;
}

CLetterList::~CLetterList()
{
	SAFE_DELETE(m_pNewText);
	SAFE_DELETE(m_pKeepText);
	SAFE_DELETE(m_pNoticeText);
	SAFE_DELETE(m_pBtn_Prev);
	SAFE_DELETE(m_pBtn_Next);

	for(int i=0; i<7; i++)
	{
		SAFE_DELETE(m_pLineNumText[i]);
		SAFE_DELETE(m_pChNameText[i]);
		SAFE_DELETE(m_pDateText[i]);
		SAFE_DELETE(m_pStateText[i]);
		SAFE_DELETE(m_pLineIndex[i]);
	}
	// [12/22/2008 D.K ] : memory leak.
// 	SAFE_DELETE( m_pBack );
	// end.
}
void CLetterList::Draw()
{
	if (!m_bActive || !g_pDisplay)
		return;

	if( m_pBack )
		g_pDisplay->Blt(m_pBack->Xpos, m_pBack->Ypos, m_pBack);

	if(m_pBtn_Prev && m_bIsViewPrev)
	{
		m_pBtn_Prev->Draw();
		m_pBtn_Prev->SetDisable(false);
	}
	else if( m_pBtn_Prev )
		m_pBtn_Prev->SetDisable(true);

	if(m_pBtn_Next && m_bIsViewNext)
	{
		m_pBtn_Next->Draw();
		m_pBtn_Next->SetDisable(false);
	}
	else if( m_pBtn_Next )
		m_pBtn_Next->SetDisable(true);

	if( m_pNewText )
		m_pNewText->Draw();
	if( m_pKeepText )
		m_pKeepText->Draw();
	if( m_pNoticeText )
		m_pNoticeText->Draw();

	for(int i=0; i<m_iLineCount; i++)
	{
		if( m_pLineNumText[i] )
			m_pLineNumText[i]->Draw();
		if( m_pChNameText[i] )
			m_pChNameText[i]->Draw();
		if( m_pDateText[i] )
			m_pDateText[i]->Draw();
		if( m_pStateText[i] )
			m_pStateText[i]->Draw();
	}

	int start_posY = 48;
}
LRESULT CLetterList::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int x, y;
	int l = 0; // 쪽지 수신확인 때문에 추가.

	if(!m_bActive)
		return 0;

	x = LOWORD (lParam);
	y = HIWORD (lParam);

	int curtime = 0;

	switch ( msg )
	{
	case WM_LBUTTONDOWN:
		if(IsInside(x, y))
		{
			if( m_pBtn_Prev )
				m_pBtn_Prev->MsgProc(hWnd, msg, wParam, lParam);
			if( m_pBtn_Next )
				m_pBtn_Next->MsgProc(hWnd, msg, wParam, lParam);
		}
		if(Owner_CL && IsLineInside(x,y)<m_iLineCount)
		{
			Owner_CL->LetterViewMessage(g_nServerIndex,m_pLineIndex[IsLineInside(x, y)],g_pRoh->m_pstrName,g_pRoh->m_nUserIndex);
			Owner_CL->PutLetterView();
			Owner_CL->m_IsLetterList=false;
			Owner_CL->m_IsLetterView=true;

			// 쪽지 수신 확인 기능 추가
			if(  g_dwClientCountry == CTRY_KOR && m_pStateText[l] // 상태 글자로 체크해야하는데 이게 웹에서 보내주는 언어라 체크를 한글로 직접하므로 한국만 수신 확인하도록 처리.
					&& m_pStateText[l]->m_str && (strcmp(m_pStateText[l]->m_str,g_StateType[0])==0) // 새로 받은 메세지를 볼때만 처리.
					&& Owner_CL->m_strLineInfo && strncmp( Owner_CL->m_strLineInfo,"[수신확인]",10 ) != 0
			  )
			{
				if( m_pChNameText[l] && m_pChNameText[l]->m_str && m_pDateText[l] && m_pDateText[l]->m_str )
				{
					char *strTemp = NULL;
					int Len = strlen(Owner_CL->m_strLineInfo) + 200;
					strTemp = new char[Len];

					if( !strTemp )
						return 1;

					sprintf( strTemp, G_STRING(IDS_LHSTRING1746) , g_pRoh->m_pstrName, m_pDateText[l]->m_str, Owner_CL->m_strLineInfo );

					Owner_CL->LetterSendMessage( g_nServerIndex,g_pRoh->m_nUserIndex
												 , pCMyApp->m_strLetterUserId, g_pRoh->m_pstrName
												 , m_pChNameText[l]->m_str, strTemp );

					SAFE_DELETE_ARRAY( strTemp );
				}
			}

			return 1;
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

			if(m_pBtn_Prev && m_pBtn_Prev->m_iBtnFlg == BTN_ACTION)
			{
				curtime = timeGetTime();
				if (m_nPageNum > 1)
					m_nPageNum--;

				if( Owner_CL )
				{
					Owner_CL->LetterListMessage(g_nServerIndex,g_pRoh->m_pstrName,g_pRoh->m_nUserIndex,m_nPageNum);
					Owner_CL->PutList();
				}
				if( m_pBtn_Prev )
					m_pBtn_Prev->m_iBtnFlg = BTN_NORMAL;
				return 1;
			}
			else if(m_pBtn_Next && m_pBtn_Next->m_iBtnFlg == BTN_ACTION)
			{
				m_nPageNum++;
				if( Owner_CL )
				{
					Owner_CL->LetterListMessage(g_nServerIndex,g_pRoh->m_pstrName,g_pRoh->m_nUserIndex,m_nPageNum);
					Owner_CL->PutList();
				}
				if( m_pBtn_Next )
					m_pBtn_Next->m_iBtnFlg = BTN_NORMAL;
				return 1;
			}

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
BOOL CLetterList::IsInside(int x, int y)
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
int CLetterList::IsLineInside(int x, int y)
{
	if( !m_pBack )
		return false;

	int i = 0;
	for(i=0; i<m_iLineCount; i++)
	{
		if (x >= (int)(m_pBack->Xpos+62)  && x < (int)(m_pBack->Xpos + m_pBack->GetWidth()) &&
				y >= (int)(m_pBack->Ypos+166+(i*25)) && y < (int)(m_pBack->Ypos+186+(i*25)) )
		{
			return i;
		}
	}
	return i;
}
