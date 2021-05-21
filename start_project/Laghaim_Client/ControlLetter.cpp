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
#include "ControlLetter.h"
#include "nk2dframe.h"
#include "controlbottom.h"
#include "UIMgr.h"
#include "Mouse.h"
#include "WebWorld.h"

#define WEB_ALL_MESSAGE_SIZE 768
#define WEB_TITLE_MESSAGE_SIZE 256
#define WEB_CONTENTS_MESSAGE_SIZE 512
#define WEB_Notice_MESSAGE_SIZE 2048






extern int	g_nServerIndex;		// 서버 인덱스 ( 쪽지 시스템 )

static DWORD back_color = 0x000000;
static DWORD fore_color = RGB(231, 203, 102);
static DWORD gray_color = RGB(155, 155, 155);


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CControlLetter::CControlLetter()
	: m_pBack(NULL)
	, m_pSecret(NULL)
	, m_pTextOut(NULL)
	, m_nPageNum(0)
	, m_iMainX(0)
	, m_iMainY(0)
	, m_ScreenWidth(0)
	, m_ScreenHeight(0)
	, m_IsAllView(FALSE)
{
//------------------------------------------------------------
	//	BUTTON
	m_pBtn_List = NULL;
	m_pBtn_Notice = NULL;
	m_pBtn_Send = NULL;
	m_pBtn_Friend = NULL;
	m_pBtn_Refuse = NULL;
	m_pBtn_Close = NULL;
	m_LetterList  = NULL;
	m_NoticeList = NULL;
	m_LetterSend = NULL;
	m_LetterFriend = NULL;
	m_LetterReject = NULL;
	m_LetterView = NULL;
	m_NoticeView = NULL;

	m_IsLetterList = true;
	m_IsNoticeList = false;
	m_IsLetterSend = false;
	m_IsLetterFriend = false;
	m_IsLetterReject = false;
	m_IsLetterView = false;
	m_IsNoticeView = false;
	m_bActive = false;

	m_strLetterMessage = new char[WEB_Notice_MESSAGE_SIZE];
	ZeroMemory(m_strLetterMessage, WEB_Notice_MESSAGE_SIZE);

	m_strUpInfo = new char[WEB_TITLE_MESSAGE_SIZE];
	ZeroMemory(m_strUpInfo, WEB_TITLE_MESSAGE_SIZE);

	m_strLineInfo = new char[WEB_Notice_MESSAGE_SIZE];
	ZeroMemory(m_strLineInfo, WEB_Notice_MESSAGE_SIZE);

	m_strFriendList = new char[WEB_TITLE_MESSAGE_SIZE];
	ZeroMemory(m_strFriendList, WEB_TITLE_MESSAGE_SIZE);
}

CControlLetter::~CControlLetter()
{
	DeleteRes();

	SAFE_DELETE(m_LetterList );
	SAFE_DELETE(m_NoticeList );
	SAFE_DELETE(m_LetterSend );
	SAFE_DELETE(m_LetterFriend );
	SAFE_DELETE(m_LetterReject);
	SAFE_DELETE(m_LetterView);
	SAFE_DELETE(m_NoticeView );
	SAFE_DELETE(m_strLetterMessage);
	SAFE_DELETE(m_strUpInfo);
	SAFE_DELETE(m_strLineInfo);
	SAFE_DELETE(m_strFriendList);
}

void CControlLetter::LoadRes()
{
	if (!g_pDisplay)
		return;

	// ----------------------------------------------------------
	// GET SCREEN SIZE
	RECT rc;
	HWND hWnd = g_pDisplay->GetHWnd();
	GetClientRect(hWnd, &rc);
	m_ScreenWidth = rc.right;
	m_ScreenHeight = rc.bottom;

	//------------------------------------------------------------
	//	BACKGROUND
	m_pBack		=  NULL;
	int X = 0, Y = 0;
	g_pDisplay->CreateSurfaceFromBitmap(&m_pBack, "interface/LetterSystem/tag_list.bmp");
	if( m_pBack )
	{
		m_pBack->Xpos = m_ScreenWidth/2 - m_pBack->GetWidth()/2;
		m_pBack->Ypos = m_ScreenHeight/2 - m_pBack->GetHeight()/2;
		X = m_iMainX = m_pBack->Xpos;
		Y = m_iMainY =m_pBack->Ypos;
	}

	//-- Contents Xlass
	m_LetterList  = new CLetterList (this);
	m_NoticeList = new CNoticeList(this);
	m_LetterSend = new CLetterSend(this);
	m_LetterFriend = new CLetterFriend(this);
	m_LetterReject = new CLetterReject(this);
	m_LetterView = new CLetterView(this);
	m_NoticeView = new CNoticeView(this);

	m_pBtn_List = new CBasicButton();
	m_pBtn_Notice = new CBasicButton();
	m_pBtn_Send = new CBasicButton();
	m_pBtn_Friend = new CBasicButton();
	m_pBtn_Refuse = new CBasicButton();
	m_pBtn_Close = new CBasicButton();

	if( m_pBtn_List )
	{
		m_pBtn_List->SetFileName("LetterSystem/button/btn_rank_list");
		m_pBtn_List->LoadRes();
		m_pBtn_List->SetPosition(X+36, Y+37);
	}

	if( m_pBtn_Notice )
	{
		m_pBtn_Notice->SetFileName("LetterSystem/button/btn_rank_info");
		m_pBtn_Notice->LoadRes();
		m_pBtn_Notice->SetPosition(X+139, Y+37);
	}

	if( m_pBtn_Send )
	{
		m_pBtn_Send->SetFileName("LetterSystem/button/btn_rank_send");
		m_pBtn_Send->LoadRes();
		m_pBtn_Send->SetPosition(X+243, Y+37);
	}

	if( m_pBtn_Friend )
	{
		m_pBtn_Friend->SetFileName("LetterSystem/button/btn_rank_friend");
		m_pBtn_Friend->LoadRes();
		m_pBtn_Friend->SetPosition(X+347, Y+37);
	}

	if( m_pBtn_Refuse )
	{
		m_pBtn_Refuse->SetFileName("LetterSystem/button/btn_rank_rejection");
		m_pBtn_Refuse->LoadRes();
		m_pBtn_Refuse->SetPosition(X+452, Y+37);
	}

	if( m_pBtn_Close )
	{
		m_pBtn_Close->SetFileName("btn_close");
		m_pBtn_Close->LoadRes();
		m_pBtn_Close->SetPosition(X+548, Y+18);
	}

	m_IsAllView = TRUE;
}
void CControlLetter::DeleteRes()
{
	SAFE_DELETE(m_pBtn_List);
	SAFE_DELETE(m_pBtn_Notice);
	SAFE_DELETE(m_pBtn_Send);
	SAFE_DELETE(m_pBtn_Friend);
	SAFE_DELETE(m_pBtn_Refuse);
	SAFE_DELETE(m_pBtn_Close);
	// [12/22/2008 D.K ] : memory leak.
//	SAFE_DELETE( m_pBack );
	// end.
}
void CControlLetter::Draw()
{
	if (!m_bActive)
		return;

	m_pBtn_List->Draw();
	m_pBtn_Notice->Draw();
	m_pBtn_Send->Draw();
	m_pBtn_Friend->Draw();
	m_pBtn_Refuse->Draw();

	if(m_IsLetterList) 		m_LetterList->Draw();
	if(m_IsNoticeList)		m_NoticeList->Draw();
	if(m_IsLetterSend)		m_LetterSend->Draw();
	if(m_IsLetterFriend)	m_LetterFriend->Draw();
	if(m_IsLetterReject)	m_LetterReject->Draw();
	if(m_IsLetterView)		m_LetterView->Draw();
	if(m_IsNoticeView)		m_NoticeView->Draw();
	if( m_pBtn_Close )		m_pBtn_Close->Draw();
}
LRESULT CControlLetter::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
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
			if( m_pBtn_List )	m_pBtn_List->MsgProc(hWnd, msg, wParam, lParam);
			if( m_pBtn_Notice )	m_pBtn_Notice->MsgProc(hWnd, msg, wParam, lParam);
			if( m_pBtn_Send )	m_pBtn_Send->MsgProc(hWnd, msg, wParam, lParam);
			if( m_pBtn_Friend )	m_pBtn_Friend->MsgProc(hWnd, msg, wParam, lParam);
			if( m_pBtn_Refuse )	m_pBtn_Refuse->MsgProc(hWnd, msg, wParam, lParam);
			if( m_pBtn_Close )	m_pBtn_Close->MsgProc(hWnd, msg, wParam, lParam);
		}
		//-- 상위 버튼에 대한 Contents 화면 설정
		if(m_pBtn_List && m_pBtn_List->m_iBtnFlg == BTN_DOWN)
		{
			m_IsLetterList =true;
			m_IsNoticeList = false;
			m_IsLetterSend= false;
			m_IsLetterFriend= false;
			m_IsLetterReject= false;
			m_IsLetterView= false;
			m_IsNoticeView= false;
			m_LetterList->m_nPageNum=1;
			if( g_pRoh )
				LetterListMessage(g_nServerIndex,g_pRoh->m_pstrName,g_pRoh->m_nUserIndex,m_LetterList->m_nPageNum);
			PutList();
			m_LetterList->MsgProc(hWnd, msg, wParam, lParam);
		}

		if(m_pBtn_Notice && m_pBtn_Notice->m_iBtnFlg == BTN_DOWN)
		{

			m_IsLetterList = false;
			m_IsNoticeList = true;
			m_IsLetterSend = false;
			m_IsLetterFriend= false;
			m_IsLetterReject= false;
			m_IsLetterView= false;
			m_IsNoticeView= false;
			LetterEventListMessage(m_NoticeList->m_nPageNum);
			PutNoticeList();
			if( m_NoticeList )	m_NoticeList->MsgProc(hWnd, msg, wParam, lParam);
		}

		if(m_pBtn_Send && m_pBtn_Send->m_iBtnFlg == BTN_DOWN)
		{
			m_IsLetterList = false;
			m_IsNoticeList = false;
			m_IsLetterSend= true;
			m_IsLetterFriend= false;
			m_IsLetterReject= false;
			m_IsLetterView= false;
			m_IsNoticeView= false;
			if( m_LetterSend )
			{
				m_LetterSend->m_bViewList=false;
				m_LetterSend->MsgProc(hWnd, msg, wParam, lParam);
			}
		}
		if(m_pBtn_Friend && m_pBtn_Friend->m_iBtnFlg == BTN_DOWN)
		{
			m_IsLetterList = false;
			m_IsNoticeList = false;
			m_IsLetterSend= false;
			m_IsLetterFriend= true;
			m_IsLetterReject= false;
			m_IsLetterView= false;
			m_IsNoticeView= false;
			if( g_pRoh )
				LetterFriendListMessage(g_nServerIndex,pCMyApp->m_strLetterUserId,g_pRoh->m_nUserIndex);
			PutFriendList();
			ViewFriendList(0,5);
			if( m_LetterFriend )	m_LetterFriend->MsgProc(hWnd, msg, wParam, lParam);

		}
		if(m_pBtn_Refuse && m_pBtn_Refuse->m_iBtnFlg == BTN_DOWN)
		{
			m_IsLetterList = false;
			m_IsNoticeList = false;
			m_IsLetterSend= false;
			m_IsLetterFriend= false;
			m_IsLetterReject= true;
			m_IsLetterView= false;
			m_IsNoticeView= false;
			if( g_pRoh )
				LetterBlockListMessage(g_nServerIndex,g_pRoh->m_pstrName);
			if( m_LetterReject )	m_LetterReject->m_nStartLine=0;
			PutBlockList();
			ViewBlockList(m_LetterReject->m_nStartLine,5);
			if( m_LetterReject )	m_LetterReject->MsgProc(hWnd, msg, wParam, lParam);
		}

		if(m_IsLetterList)			m_LetterList->MsgProc(hWnd, msg, wParam, lParam);
		else if(m_IsNoticeList)		m_NoticeList->MsgProc(hWnd, msg, wParam, lParam);
		else if(m_IsLetterSend)		m_LetterSend->MsgProc(hWnd, msg, wParam, lParam);
		else if(m_IsLetterFriend)	m_LetterFriend->MsgProc(hWnd, msg, wParam, lParam);
		else if(m_IsLetterReject)	m_LetterReject->MsgProc(hWnd, msg, wParam, lParam);
		else if(m_IsLetterView)		m_LetterView->MsgProc(hWnd, msg, wParam, lParam);
		else if(m_IsNoticeView)		m_NoticeView->MsgProc(hWnd, msg, wParam, lParam);

		return 1;

	case WM_LBUTTONUP:
		if(m_bActive)
		{
			if( m_pBtn_Close )
			{
				m_pBtn_Close->MsgProc(hWnd, msg, wParam, lParam);

				if(m_pBtn_Close->m_iBtnFlg == BTN_ACTION)
				{
					if( g_pNk2DFrame )	g_pNk2DFrame->ShowLetterWindow(FALSE);
					m_pBtn_Close->SetState(BTN_NORMAL);
					m_bActive = false;
					if( pCMyApp->m_pUIMgr )	pCMyApp->m_pUIMgr->isNewLetter();//새쪽지가 있는가 체크
					return 0;
				}
			}

			//-- Contents 화면의 Meg 호출
			if(m_IsLetterList)			m_LetterList->MsgProc(hWnd, msg, wParam, lParam);
			else if(m_IsNoticeList)		m_NoticeList->MsgProc(hWnd, msg, wParam, lParam);
			else if(m_IsLetterSend)		m_LetterSend->MsgProc(hWnd, msg, wParam, lParam);
			else if(m_IsLetterFriend)	m_LetterFriend->MsgProc(hWnd, msg, wParam, lParam);
			else if(m_IsLetterReject)	m_LetterReject->MsgProc(hWnd, msg, wParam, lParam);
			else if(m_IsLetterView)		m_LetterView->MsgProc(hWnd, msg, wParam, lParam);
			else if(m_IsNoticeView)		m_NoticeView->MsgProc(hWnd, msg, wParam, lParam);
		}
		return 0;

	case WM_MOUSEMOVE:
		if(IsInside(x, y))
		{
			if( pCMyApp->m_pMouse )	pCMyApp->m_pMouse->SetMouseType(M_NORMAL);
			if( m_pBtn_List )		m_pBtn_List->MsgProc(hWnd, msg, wParam, lParam);
			if( m_pBtn_Notice )		m_pBtn_Notice->MsgProc(hWnd, msg, wParam, lParam);
			if( m_pBtn_Send )		m_pBtn_Send->MsgProc(hWnd, msg, wParam, lParam);
			if( m_pBtn_Friend )		m_pBtn_Friend->MsgProc(hWnd, msg, wParam, lParam);
			if( m_pBtn_Refuse )		m_pBtn_Refuse->MsgProc(hWnd, msg, wParam, lParam);
			if( m_pBtn_Close )		m_pBtn_Close->MsgProc(hWnd, msg, wParam, lParam);
			return 1;
		}
		else
			return 0;

	case WM_KEYDOWN:
		if(wParam == VK_ESCAPE)
		{
			if( g_pNk2DFrame )	g_pNk2DFrame->ShowLetterWindow(FALSE);
			if( m_pBtn_Close )	m_pBtn_Close->SetState(BTN_NORMAL);
			m_bActive = false;
			if( pCMyApp->m_pUIMgr )	pCMyApp->m_pUIMgr->isNewLetter();//새쪽지가 있는가 체크
			return 0;
		}
		if(m_IsLetterSend && m_LetterSend)
		{
			m_LetterSend->MsgProc(hWnd, msg, wParam, lParam);
			return 1;
		}
		if(m_IsLetterFriend && m_LetterFriend)
		{
			m_LetterFriend->MsgProc(hWnd, msg, wParam, lParam);
			return 1;
		}
		if(m_IsLetterReject && m_LetterReject)
		{
			m_LetterReject->MsgProc(hWnd, msg, wParam, lParam);
			return 1;
		}
		if(wParam ==VK_TAB || wParam ==VK_RETURN )
			return 1;
		break;

	case WM_KEYUP:
		switch( (int) wParam)
		{
		case VK_SNAPSHOT:
			pCMyApp->m_pUIMgr->Save_Screen_Shot(hWnd);
		}
		break;

	case WM_CHAR:
		if(m_IsLetterSend && m_LetterSend)
		{
			m_LetterSend->MsgProc(hWnd, msg, wParam, lParam);
			return 1;
		}
		if(m_IsLetterFriend && m_LetterFriend)
		{
			m_LetterFriend->MsgProc(hWnd, msg, wParam, lParam);
			return 1;
		}
		if(m_IsLetterReject && m_LetterReject)
		{
			m_LetterReject->MsgProc(hWnd, msg, wParam, lParam);
			return 1;
		}
		break;

	case WM_IME_STARTCOMPOSITION:
	case WM_IME_COMPOSITION:
	case WM_IME_ENDCOMPOSITION:
		if(m_IsLetterSend && m_LetterSend)
		{
			m_LetterSend->MsgProc(hWnd, msg, wParam, lParam);
			return 1;
		}
		if(m_IsLetterFriend && m_LetterFriend)
		{
			m_LetterFriend->MsgProc(hWnd, msg, wParam, lParam);
			return 1;
		}
		if(m_IsLetterReject && m_LetterReject)
		{
			m_LetterReject->MsgProc(hWnd, msg, wParam, lParam);
			return 1;
		}
		break;

	case WM_IME_CHAR:
	case WM_IME_NOTIFY:
		return 2;
	}
	return 1;
}
BOOL CControlLetter::IsInside(int x, int y)
{
	if( !m_pBack )
		return false;

	return m_pBack->IsIn(x, y);
}
BOOL CControlLetter::LetterListMessage(int severNum,char *charName,int userIndex,int page)
{
	ResetWebData();

	CWebWorld	webworld;

	char address[WEB_ALL_MESSAGE_SIZE] = "";
	char index[WEB_TITLE_MESSAGE_SIZE] = "";
	char szNameTemp[ 256 ] = "";

	sprintf(address, "http://laghaim.barunsongames.com/game/notes/Letter_list.asp?");
	sprintf(index, "server=%d", severNum);
	strcat( address, index );

	CharChange(charName, szNameTemp);
	//strcpy( szNameTemp, CharChange(charName) );
	sprintf(index, "&charName=%s", szNameTemp);
	strcat( address, index );
	sprintf(index, "&userIndex=%d", userIndex);
	strcat( address, index );
	sprintf(index, "&page=%d", page);
	strcat( address, index );

	if( webworld.GetWebPage(address) )
	{
		int nTemp = 0;

		sscanf( webworld.m_UrlData, "%d", &nTemp ); // 제일 앞의 숫자를 받아온다.(쪽지가 제대로 받아졌는지의 플래그.)

		if( nTemp == 1 ) // 쪽지가 제대로 받아졌을때만 처리.
			strcpy(m_strLetterMessage,&webworld.m_UrlData[ 2 ]);
		else
			ZeroMemory(m_strLetterMessage, WEB_Notice_MESSAGE_SIZE );

		{
			int temp[5];

			sscanf(m_strLetterMessage,"%d %d %d %d %d", &temp[0], &temp[1], &temp[2], &temp[3], &temp[4] );
			int nCount = temp[0]+temp[1]+temp[2]+temp[3]+temp[4];

			if( nCount > 0 )
				return FALSE;
		}
	}
	return 1;
}

BOOL CControlLetter::LetterViewMessage(int severNum,char* letterIndex,char *charName,int userIndex)
{
	ResetWebData();

	CWebWorld	webworld;

	char address[WEB_ALL_MESSAGE_SIZE] = "";
	char temp[WEB_TITLE_MESSAGE_SIZE] = "";
	char szNameTemp[ 256 ] = "";

	sprintf(address, "http://laghaim.barunsongames.com/game/notes/Letter_view.asp?");
	sprintf(temp, "server=%d", severNum);
	strcat( address, temp );
	sprintf(temp, "&letterIndex=%s", letterIndex);
	strcat( address, temp );

	CharChange(charName, szNameTemp);
	//strcpy( szNameTemp, CharChange(charName) );
	sprintf(temp, "&charName=%s", szNameTemp);
	strcat( address, temp );
	sprintf(temp, "&userIndex=%d", userIndex);
	strcat( address, temp );

	if( webworld.GetWebPage(address) )
		strcpy(m_strLetterMessage,webworld.m_UrlData);

	return 1;
}
BOOL CControlLetter::LetterCrimeMessage(int severNum, char* letterIndex)
{
	ResetWebData();

	CWebWorld	webworld;
	char address[WEB_ALL_MESSAGE_SIZE] = "";
	char temp[WEB_TITLE_MESSAGE_SIZE] = "";

	sprintf(address, "http://laghaim.barunsongames.com/game/notes/Letter_crime.asp?");
	sprintf(temp, "server=%d", severNum);
	strcat( address, temp );
	sprintf(temp, "&letterIndex=%s", letterIndex);
	strcat( address, temp );


	if( webworld.GetWebPage(address) )
		strcpy(m_strLetterMessage,webworld.m_UrlData);

	return 1;
}
BOOL CControlLetter::LetterSendMessage(int severNum,int userIndex,char *userId,char *charName,char *toChar,char *contents)
{
	ResetWebData();

	CWebWorld	webworld;
	char tete[256*3] = "";
	char address[WEB_Notice_MESSAGE_SIZE] = "";
	char temp[WEB_Notice_MESSAGE_SIZE]= {0,};
	char szNameTemp[ 256 ] = "";

	sprintf(address, "http://laghaim.barunsongames.com/game/notes/Letter_write.asp?");
	sprintf(temp, "server=%d", severNum);
	strcat( address, temp );
	sprintf(temp, "&userIndex=%d", userIndex);
	strcat( address, temp );
	sprintf(temp, "&userId=%s", userId);
	strcat( address, temp );

	CharChange(charName, szNameTemp);
	//strcpy( szNameTemp, CharChange(charName) );
	sprintf(temp, "&charName=%s", szNameTemp);
	strcat( address, temp );
	CharChange(toChar, szNameTemp);
	//strcpy( szNameTemp, CharChange(toChar) );
	sprintf(temp, "&toChar=%s", szNameTemp);

	strcat( address, temp );

	int len = strlen(contents);
	int j = 0;
	for (int i = 0; i < len; i++)
	{
		if (contents[i] == '\n')
		{
			tete[j++] = '%';
			tete[j++] = '0';
			tete[j++] = 'a';
		}
		else if (contents[i] == '\r')
		{
			tete[j++] = '%';
			tete[j++] = '0';
			tete[j++] = 'd';
		}
		else if (contents[i] == '+')
		{
			tete[j++] = '%';
			tete[j++] = '2';
			tete[j++] = 'b';
		}
		else if (contents[i] == '#')
		{
			tete[j++] = '%';
			tete[j++] = '2';
			tete[j++] = '3';
		}
		else if (contents[i] == '%')
		{
			tete[j++] = '%';
			tete[j++] = '2';
			tete[j++] = '5';
		}
		else if (contents[i] == '&')
		{
			tete[j++] = '%';
			tete[j++] = '2';
			tete[j++] = '6';
		}
		else
		{
			tete[j++] = contents[i];
		}
	}
	tete[j] = '\0';
	sprintf(temp, "&contents=%s", tete);
	strcat( address, temp );

	if( webworld.GetWebPage(address) )
		strcpy(m_strLetterMessage,webworld.m_UrlData);
	// 쪽지 알림 위해 서버로 메시지 보냄
	// post 받는캐릭명
	if(m_strLetterMessage[0]=='1')
	{
		sprintf(temp, "post %s\n", toChar);

		if (g_pTcpIp)
			g_pTcpIp->SendNetMessage(temp);
	}
	else if(m_strLetterMessage[0]=='0' )
		g_pNk2DFrame->InsertPopup(&m_strLetterMessage[1], TYPE_NOR_OK);

	return 1;
}
BOOL CControlLetter::LetterFriendListMessage(int severNum,char *userId,int userIndex)
{
	ResetWebData();

	CWebWorld	webworld;

	char address[WEB_ALL_MESSAGE_SIZE] = "";
	char temp[WEB_TITLE_MESSAGE_SIZE] = "";

	sprintf(address, "http://laghaim.barunsongames.com/game/notes/Letter_friend_list.asp?");
	sprintf(temp, "server=%d", severNum);
	strcat( address, temp );
	sprintf(temp, "&userId=%s", userId);
	strcat( address, temp );
	sprintf(temp, "&&userIndex=%d", userIndex);
	strcat( address, temp );

	if( webworld.GetWebPage(address) )
	{
		int len = strlen(webworld.m_UrlData);
		if( len >= WEB_Notice_MESSAGE_SIZE )
			ZeroMemory(m_strLetterMessage, WEB_Notice_MESSAGE_SIZE);
		else
			strcpy(m_strLetterMessage,webworld.m_UrlData);
	}

	if( m_strLetterMessage[0]  != '0' )
		return FALSE;

	return 1;
}
BOOL CControlLetter::LetterBlockListMessage(int severNum,char *charName)
{
	ResetWebData();

	CWebWorld	webworld;

	char address[WEB_ALL_MESSAGE_SIZE] = "";
	char temp[WEB_TITLE_MESSAGE_SIZE] = "";
	char szNameTemp[ 256 ] = "";

	sprintf(address, "http://laghaim.barunsongames.com/game/notes/Letter_block_list.asp?");
	sprintf(temp, "server=%d", severNum);
	strcat( address, temp );

	CharChange(charName, szNameTemp);
	//strcpy( szNameTemp, CharChange(charName) );
	sprintf(temp, "&charName=%s", szNameTemp);

	strcat( address, temp );

	if( webworld.GetWebPage(address) )
	{
		int len = strlen(webworld.m_UrlData);
		if( len >= WEB_Notice_MESSAGE_SIZE )
			ZeroMemory(m_strLetterMessage, WEB_Notice_MESSAGE_SIZE);
		else
			strcpy(m_strLetterMessage,webworld.m_UrlData);
	}

	if( m_strLetterMessage[0] != '0' )
		return FALSE;

	return 1;
}
BOOL CControlLetter::LetterBagFriendMessage(int severNum,char *userId,int userIndex,char *charName,char* mode,char* friendName)
{
	ResetWebData();

	CWebWorld	webworld;

	char address[WEB_ALL_MESSAGE_SIZE] = "";
	char temp[WEB_TITLE_MESSAGE_SIZE] = "";
	char tete[32] = "";
	char szNameTemp[ 256 ] = "";

	sprintf(address, "http://laghaim.barunsongames.com/game/notes/Letter_bag_person.asp?");
	sprintf(temp, "server=%d", severNum);
	strcat( address, temp );
	sprintf(temp, "&userId=%s", userId);
	strcat( address, temp );
	sprintf(temp, "&userIndex=%d", userIndex);
	strcat( address, temp );

	CharChange(charName, szNameTemp);
	//strcpy( szNameTemp, CharChange(charName) );

	sprintf(temp, "&charName=%s", szNameTemp);
	strcat( address, temp );
	sprintf(temp, "&mode=%s", mode);
	strcat( address, temp );

	int len = strlen(friendName);
	if(len>30)
		len = 30;

	int j = 0;
	for (int i = 0; i < len; i++)
	{
		if (friendName[i] == '+')
		{
			tete[j++] = '%';
			tete[j++] = '2';
			tete[j++] = 'b';
		}
		else
			tete[j++] = friendName[i];
	}
	tete[j] = '\0';
	CharChange(tete, szNameTemp);
	sprintf(temp, "&friendName=%s", szNameTemp);
	strcat( address, temp );

	if( webworld.GetWebPage(address) )
	{
		int len = strlen(webworld.m_UrlData);
		if( len >= WEB_Notice_MESSAGE_SIZE )
			ZeroMemory(m_strLetterMessage, WEB_Notice_MESSAGE_SIZE);
		else
			strcpy(m_strLetterMessage,webworld.m_UrlData);
	}

	return 1;
}
BOOL CControlLetter::LetterBagBlockMessage(int severNum,int userIndex,char *charName,char* mode,char* blockName)
{
	ResetWebData();

	CWebWorld	webworld;

	char address[WEB_ALL_MESSAGE_SIZE] = "";
	char temp[WEB_TITLE_MESSAGE_SIZE] = "";
	char tete[32] = "";
	char szNameTemp[ 256 ] = "";

	sprintf(address, "http://laghaim.barunsongames.com/game/notes/Letter_bag_person.asp?");
	sprintf(temp, "server=%d", severNum);
	strcat( address, temp );
	sprintf(temp, "&userIndex=%d", userIndex);
	strcat( address, temp );

	sprintf(temp, "&charName=%s", charName);
	strcat( address, temp );
	sprintf(temp, "&mode=%s", mode);
	strcat( address, temp );

	int len = strlen(blockName);
	if(len>30)
		len = 30;

	int j = 0;
	for (int i = 0; i < len; i++)
	{
		if (blockName[i] == '+')
		{
			tete[j++] = '%';
			tete[j++] = '2';
			tete[j++] = 'b';
		}
		else
		{
			tete[j++] = blockName[i];
		}
	}
	tete[j] = '\0';
	CharChange(tete, szNameTemp);
	sprintf(temp, "&blockName=%s", szNameTemp);
	strcat( address, temp );
	if( webworld.GetWebPage(address) )
	{
		int len = strlen(webworld.m_UrlData);
		if( len >= WEB_Notice_MESSAGE_SIZE )
			ZeroMemory(m_strLetterMessage, WEB_Notice_MESSAGE_SIZE);
		else
			strcpy(m_strLetterMessage,webworld.m_UrlData);
	}

	return 1;
}
BOOL CControlLetter::LetterEventListMessage(int pageNum)
{
	ResetWebData();

	CWebWorld	webworld;

	char address[WEB_ALL_MESSAGE_SIZE] = "";
	char temp[WEB_TITLE_MESSAGE_SIZE] = "";

	sprintf(address, "http://laghaim.barunsongames.com/game/notes/Letter_event_list.asp?");
	sprintf(temp, "page=%d", pageNum);
	strcat( address, temp );

	if( webworld.GetWebPage(address) )
	{
		if(strlen(webworld.m_UrlData)<512)
			strcpy(m_strLetterMessage,webworld.m_UrlData);
		else
			strncpy(m_strLetterMessage,webworld.m_UrlData,512);
	}
	return 1;
}
BOOL CControlLetter::LetterEventViewMessage(char* noticeNo)
{
	ResetWebData();

	CWebWorld	webworld;

	char address[WEB_ALL_MESSAGE_SIZE] = "";
	char temp[WEB_TITLE_MESSAGE_SIZE ] = "";

	sprintf(address, "http://laghaim.barunsongames.com/game/notes/Letter_event_view.asp?");
	sprintf(temp, "noticeNo=%s", noticeNo);
	strcat( address, temp );

	if( webworld.GetWebPage(address) )
	{
		if(strlen(webworld.m_UrlData)>WEB_Notice_MESSAGE_SIZE)
			strncpy(m_strLetterMessage,webworld.m_UrlData,strlen(webworld.m_UrlData));
		else
			strcpy(m_strLetterMessage,webworld.m_UrlData);
	}

	return 1;
}
BOOL CControlLetter::LetterKeepDelMessage(int severNum,char* letterIndex,int userIndex,char *mode,char *charName)
{
	ResetWebData();

	CWebWorld	webworld;
	char address[WEB_ALL_MESSAGE_SIZE] = "";
	char temp[WEB_TITLE_MESSAGE_SIZE] = "";
	char szNameTemp[ 256 ] = "";

	sprintf(address, "http://laghaim.barunsongames.com/game/notes/Letter_bag.asp?");
	sprintf(temp, "server=%d", severNum);
	strcat( address, temp );
	sprintf(temp, "&letterIndex=%s", letterIndex);
	strcat( address, temp );
	sprintf(temp, "&userIndex=%d", userIndex);
	strcat( address, temp );
	sprintf(temp, "&Mode=%s", mode);
	strcat( address, temp );

	CharChange(charName, szNameTemp);
	//strcpy( szNameTemp, CharChange(charName) );

	sprintf(temp, "&charName=%s", szNameTemp);
	strcat( address, temp );

	if( webworld.GetWebPage(address) )
		strcpy(m_strLetterMessage,webworld.m_UrlData);

	return 1;
}
void CControlLetter::PutList()
{
	if( !m_LetterList )
		return;

	int i=0;
	int start=0;
	int len=0;
	int spaceCount=0;
	int lineCount=0;
	char temp[WEB_CONTENTS_MESSAGE_SIZE ]= {0,};
	int a=strlen(m_strLetterMessage);

	for(i=0; i<strlen(m_strLetterMessage)+1; i++)
	{
		if(m_strLetterMessage[i]==13||m_strLetterMessage[i]==NULL)
		{
			//엔터이면
			strncpy(m_strUpInfo,m_strLetterMessage,i);
			strcpy(m_strLineInfo,m_strLetterMessage+(i+2));
			i=strlen(m_strLetterMessage);
		}
	}
	for(i=0; i<strlen(m_strUpInfo)+1; i++)
	{
		if(m_strUpInfo[i]==32 || m_strUpInfo[i]==NULL)
		{
			//공백이면
			len=i-start;
			strset(temp,0);
			strncpy(temp,m_strUpInfo+start,len);
			start=i+1;
			if(spaceCount==0)
			{
				if( m_LetterList->m_pNewText )
					m_LetterList->m_pNewText->SetString(temp, RGB(255, 255, 255));
			}
			else if(spaceCount==1)
			{
				if( m_LetterList->m_pKeepText )
					m_LetterList->m_pKeepText->SetString(temp, RGB(255, 255, 255));
			}
			else if(spaceCount==2)
			{
				if( m_LetterList->m_pNoticeText )
					m_LetterList->m_pNoticeText->SetString(temp, RGB(255, 255, 255));
			}
			else if(spaceCount==3)
			{
				if(strcmp(temp,"0"))
					m_LetterList->m_bIsViewPrev=true;
				else
					m_LetterList->m_bIsViewPrev=false;
			}
			else if(spaceCount==4)
			{
				if(strcmp(temp,"0"))
					m_LetterList->m_bIsViewNext=true;
				else
					m_LetterList->m_bIsViewNext=false;
			}
			spaceCount++;
		}
	}

	start=0;
	len=0;
	spaceCount=0;

	for(i=0; i<strlen(m_strLineInfo); i++)
	{
		char z=m_strLineInfo[i];

		if(m_strLineInfo[i]==32||m_strLineInfo[i]==NULL)
		{
			//공백이면
			len=i-start;
			strset(temp,0);
			strncpy(temp,m_strLineInfo+start,len);
			start=i+1;
			if(spaceCount==0)
				strcpy(m_LetterList->m_pLineIndex[lineCount],temp);
			else if(m_LetterList->m_pLineNumText[lineCount] && spaceCount==1)
				m_LetterList->m_pLineNumText[lineCount]->SetString(temp, RGB(255, 255, 255));
			else if(m_LetterList->m_pChNameText[lineCount] && spaceCount==2)
				m_LetterList->m_pChNameText[lineCount]->SetString(temp, RGB(255, 255, 255));
			else if(m_LetterList->m_pDateText[lineCount] && spaceCount==3)
				m_LetterList->m_pDateText[lineCount]->SetString(temp, RGB(255, 255, 255));

			spaceCount++;
		}
		else if(m_strLineInfo[i]==13)
		{
			len=i-start;
			strset(temp,0);
			strncpy(temp,m_strLineInfo+start,len);
			start=i+2;
			if( m_LetterList->m_pStateText[lineCount] )
				m_LetterList->m_pStateText[lineCount]->SetString(temp, RGB(255, 255, 255));
			spaceCount=0;
			lineCount++;
			i++;
		}
	}
	m_LetterList->m_iLineCount=lineCount;

}
void CControlLetter::PutNoticeList()
{
	if( !m_NoticeList )
		return;

	int i=0;
	int start=0;
	int len=0;
	int spaceCount=0;
	int lineCount=0;
	char temp[WEB_CONTENTS_MESSAGE_SIZE ]= {0,};
	int a=strlen(m_strLetterMessage);

	for(i=0; i<strlen(m_strLetterMessage)+1; i++)
	{
		char t=m_strLetterMessage[i];
		if(m_strLetterMessage[i]==13||m_strLetterMessage[i]==NULL)
		{
			//엔터이면
			strncpy(m_strUpInfo,m_strLetterMessage,i);
			strcpy(m_strLineInfo,m_strLetterMessage+(i+2));
			i=strlen(m_strLetterMessage);
		}
	}
	for(i=0; i<strlen(m_strUpInfo)+1; i++)
	{
		if(m_strUpInfo[i]==32||m_strUpInfo[i]==NULL)
		{
			//공백이면
			len=i-start;
			strset(temp,0);
			strncpy(temp,m_strUpInfo+start,len);
			start=i+1;
			if(m_NoticeList->m_pNoticeText && spaceCount==0)
			{
				m_NoticeList->m_pNoticeText->SetString(temp, RGB(255, 255, 255));
			}
			else if(spaceCount==1)
			{
				if(strcmp(temp,"0"))
					m_NoticeList->m_bIsViewPrev=true;
				else
					m_NoticeList->m_bIsViewPrev=false;
			}
			else if(spaceCount==2)
			{
				if(strcmp(temp,"0"))
					m_NoticeList->m_bIsViewNext=true;
				else
					m_NoticeList->m_bIsViewNext=false;
			}
			else if(spaceCount==3)
			{
				m_NoticeList->m_nPageNum = atoi( temp );
				strcpy(m_NoticeList->m_strPageNum,temp);
			}
			spaceCount++;
		}
	}

	start=0;
	len=0;
	spaceCount=0;

	for(i=0; i<strlen(m_strLineInfo); i++)
	{
		if(m_strLineInfo[i]==32 && spaceCount<2)
		{
			//공백이면
			len=i-start;
			strset(temp,0);
			strncpy(temp,m_strLineInfo+start,len);
			start=i+1;
			if(m_NoticeList->m_pLineNumText[lineCount] && spaceCount==0)
			{
				m_NoticeList->m_pLineNumText[lineCount]->SetString(temp, RGB(255, 255, 255));
				strcpy(m_NoticeList->m_pNoticeLineIndex[lineCount],temp);
			}
			else if(m_NoticeList->m_pDateText[lineCount] && spaceCount==1)
			{
				m_NoticeList->m_pDateText[lineCount]->SetString(temp, RGB(255, 255, 255));
			}
			spaceCount++;
		}
		else if(m_strLineInfo[i]==13||m_strLineInfo[i]==NULL)
		{
			len=i-start;
			strset(temp,0);
			strncpy(temp,m_strLineInfo+start,len);
			start=i+2;
			if( m_NoticeList->m_pTitleText[lineCount] )
				m_NoticeList->m_pTitleText[lineCount]->SetString(temp, RGB(255, 255, 255));
			spaceCount=0;
			lineCount++;
			i++;
		}
	}
	m_NoticeList->m_iLineCount=lineCount;
}
void CControlLetter::PutLetterView()
{
	if( !m_LetterView )
		return;

	int i=0;
	int start=0;
	int len=0;
	int spaceCount=0;
	int lineCount=0;
	char temp[WEB_CONTENTS_MESSAGE_SIZE ]= {0,};
	int a=strlen(m_strLetterMessage);
	for(i=0; i<strlen(m_strLetterMessage)+1; i++)
	{
		char t=m_strLetterMessage[i];
		if(m_strLetterMessage[i]==13||m_strLetterMessage[i]==NULL)
		{
			//엔터이면
			strncpy(m_strUpInfo,m_strLetterMessage,i);
			strcpy(m_strLineInfo,m_strLetterMessage+(i+2));
			i=strlen(m_strLetterMessage);
		}
	}
	for(i=0; i<strlen(m_strUpInfo)+1; i++)
	{
		if(m_strUpInfo[i]==32||m_strUpInfo[i]==NULL)
		{
			//공백이면
			len=i-start;
			strset(temp,0);
			strncpy(temp,m_strUpInfo+start,len);
			start=i+1;
			if(spaceCount==0)
			{
				strcpy(m_LetterView->m_pLetterIndex,temp);
			}
			else if(spaceCount==1)
			{
				if(strcmp(temp,"0"))//0이아니면->보관
					m_LetterView->m_bLetterIsNew=false;
				else
					m_LetterView->m_bLetterIsNew=true;
			}
			else if(m_LetterView->m_pSenderText && spaceCount==2)
			{
				m_LetterView->m_pSenderText->SetString(temp,RGB(255, 255, 255));
			}
			spaceCount++;
		}
	}
	if( m_LetterView->m_pLetterViewText )
		m_LetterView->m_pLetterViewText->m_MEBox.Set_CoreString(m_strLineInfo);
}
void CControlLetter::PutNoticeView()
{
	if( !m_NoticeView )
		return;

	int i=0;
	int start=0;
	int len=0;
	int spaceCount=0;
	int lineCount=0;
	char temp[WEB_Notice_MESSAGE_SIZE  ]= {0,};
	int a=strlen(m_strLetterMessage);
	for(i=0; i<strlen(m_strLetterMessage)+1; i++)
	{
		char t=m_strLetterMessage[i];
		if(m_strLetterMessage[i]==13||m_strLetterMessage[i]==NULL)
		{
			//엔터이면
			strncpy(m_strUpInfo,m_strLetterMessage,i);
			if((strlen(m_strLetterMessage)-(i+2))>WEB_Notice_MESSAGE_SIZE)
				strncpy(m_strLineInfo,m_strLetterMessage+(i+2),(WEB_Notice_MESSAGE_SIZE-(i+2)));
			else
				strcpy(m_strLineInfo,m_strLetterMessage+(i+2));

			i=strlen(m_strLetterMessage)+1;
		}
	}

	for(i=0; i<strlen(m_strUpInfo)+1; i++)
	{
		if(m_strUpInfo[i]==32||m_strUpInfo[i]==NULL)
		{
			//공백이면
			len=i-start;
			strset(temp,0);
			strncpy(temp,m_strUpInfo+start,len);
			start=i+1;
			if(spaceCount==0)
			{
				strcpy(m_NoticeView->m_pNoticeIndex,temp);
			}
			else if(spaceCount==1)
			{}
			else if(spaceCount==2)
			{
				if(strcmp(temp,"0"))
					m_NoticeView->m_bIsViewPrev=true;
				else
					m_NoticeView->m_bIsViewPrev=false;
			}
			else if(spaceCount==3)
			{
				if(strcmp(temp,"0"))
					m_NoticeView->m_bIsViewNext=true;
				else
					m_NoticeView->m_bIsViewNext=false;

				strset(temp,0);
				strcpy(temp,m_strUpInfo+i);
				if( m_NoticeView->m_pTitleText )
					m_NoticeView->m_pTitleText->SetString(temp,RGB(255, 255, 255));

				i=strlen(m_strUpInfo)+1;
			}
			spaceCount++;
		}
	}
	if( m_NoticeView->m_pNoticeViewText )
		m_NoticeView->m_pNoticeViewText->m_MEBox.Set_CoreString(m_strLineInfo);

}
void CControlLetter::PutBlockList()
{
	if( !m_LetterReject )
		return;

	int i=0;
	int start=0;
	int len=0;
	int spaceCount=0;
	int lineCount=0;
	char temp[WEB_CONTENTS_MESSAGE_SIZE ]= {0,};
	for(i=0; i<50; i++)
		strset(m_LetterReject->m_pBlockList[i],0);

	if( strcmp(m_strLetterMessage,"0") )
	{
		//블록리스트가 없을때
		for(i=0; i<strlen(m_strLetterMessage)+1; i++)
		{
			if(m_strLetterMessage[i]==13||m_strLetterMessage[i]==NULL)
			{
				//엔터이면
				strncpy(m_LetterReject->m_pBlockList[lineCount],m_strLetterMessage+start,i-start);
				lineCount++;
				start=i+2;
				if(lineCount>50)
					i=strlen(m_strLetterMessage)+1;
			}
		}
	}
	m_LetterReject->m_nAllRejectNum=lineCount;
}
void CControlLetter::ViewBlockList(int startIndex,int lineNum)
{
	if( !m_LetterReject )
		return;

	int i=0;
	int lineCount=0;
	for(i=0; i<5; i++)
		strset(m_LetterReject->m_pDelRejectText[i]->m_str,0);

	for(i=startIndex; i<startIndex+lineNum; i++)
	{
		if(i<m_LetterReject->m_nAllRejectNum)
		{
			m_LetterReject->m_pDelRejectText[lineCount]->SetString(m_LetterReject->m_pBlockList[i], RGB(255, 255, 255));
			lineCount++;
		}
	}
	m_LetterReject->m_nRejectLineCount=lineCount;
}
void CControlLetter::PutFriendList()
{
	if( !m_LetterFriend || !m_LetterSend )
		return;

	int i=0;
	int start=0;
	int len=0;
	int spaceCount=0;
	int lineCount=0;
	char temp[WEB_CONTENTS_MESSAGE_SIZE ]= {0,};

	for(i=0; i<50; i++)
	{
		strset(m_LetterFriend->m_pFriendList[i],0);
		strset(m_LetterSend->m_pFriendList[i],0);
	}

	if( strcmp(m_strLetterMessage,"0") )
	{
		for(i=0; i<strlen(m_strLetterMessage)+1; i++)
		{
			if(m_strLetterMessage[i]==13||m_strLetterMessage[i]==NULL)
			{
				//엔터이면
				strncpy(m_LetterFriend->m_pFriendList[lineCount],m_strLetterMessage+start,i-start);
				strncpy(m_LetterSend->m_pFriendList[lineCount],m_strLetterMessage+start,i-start);
				lineCount++;
				start=i+2;
				if(lineCount>=50)
					i=strlen(m_strLetterMessage)+1;
			}
		}
	}

	m_LetterFriend->m_nAllFriendNum=lineCount;
	m_LetterSend->m_nAllFriendNum=lineCount;
}
void CControlLetter::ViewFriendList(int startIndex,int lineNum)
{
	if( !m_LetterFriend )
		return;

	int i=0;
	int lineCount=0;
	for(i=0; i<5; i++)	//TextBox에 남아있던 버퍼지움
		strset(m_LetterFriend->m_pDelFriendText[i]->m_str,0);

	for(i=startIndex; i<startIndex+lineNum; i++)
	{
		if(i<m_LetterFriend->m_nAllFriendNum)
		{
			m_LetterFriend->m_pDelFriendText[lineCount]->SetString(m_LetterFriend->m_pFriendList[i], RGB(255, 255, 255));
			lineCount++;
		}
	}
	m_LetterFriend->m_nFriendLineCount=lineCount;
}
void CControlLetter::ViewSendFriendList(int startIndex,int lineNum)
{
	if( !m_LetterSend )
		return;

	int i=0;
	int lineCount=0;
	for(i=0; i<8; i++)	//TextBox에 남아있던 버퍼지움
		strset(m_LetterSend->m_pViewFriendList[i]->m_str,0);

	for(i=startIndex; i<startIndex+lineNum; i++)
	{
		if(i<m_LetterSend->m_nAllFriendNum)
		{
			m_LetterSend->m_pViewFriendList[lineCount]->SetString(m_LetterSend->m_pFriendList[i], RGB(255, 255, 255));
			lineCount++;
		}
	}
	m_LetterSend->m_nFriendLineCount=lineCount;
}

void CControlLetter::ResetWebData()
{
	SAFE_DELETE(m_strLetterMessage);
	m_strLetterMessage = new char[WEB_Notice_MESSAGE_SIZE ];
	ZeroMemory(m_strLetterMessage, WEB_Notice_MESSAGE_SIZE );

	SAFE_DELETE(m_strUpInfo);
	m_strUpInfo = new char[WEB_TITLE_MESSAGE_SIZE ];
	ZeroMemory(m_strUpInfo, WEB_TITLE_MESSAGE_SIZE );

	SAFE_DELETE(m_strLineInfo);
	m_strLineInfo = new char[WEB_Notice_MESSAGE_SIZE ];
	ZeroMemory(m_strLineInfo, WEB_Notice_MESSAGE_SIZE );
}

void CControlLetter::CharChange(char IN *strIn, char OUT *strOut)
{
	strOut[0] = '\0';
	int lenStr = strlen(strIn);
	int a=0;
	int b=0;

	for(a =0; a <lenStr; a++ )
	{
		if(strIn[a]=='&')
		{
			//strcat(strOut, "error_andchar");
			//b+=13;
			strcat(strOut, "%26"); // [2008/8/14 Theodoric] 오늘 부로 error 처리 하지 않는다.
			b+=3;
		}
		else if(strIn[a]=='#')
		{
			strcat(strOut, "%23");
			b+=3;
		}
		else if(strIn[a]=='%')
		{
			strcat(strOut, "%25");
			b+=3;
		}
		else
		{
			strOut[b]=strIn[a];
			b++;
		}
	}
	strOut[b] = '\0';
}
