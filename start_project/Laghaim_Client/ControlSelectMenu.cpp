#include "stdafx.h"


#include "headers.h"
#include "BaseInterface.h"
#include "main.h"
#include "ControlSocial.h"
#include "ddutil.h"
#include "Nk2DFrame.h"
#include "tcpipcon.h"
#include "ListBox.h"
#include "ControlSelectMenu.h"
#include "Land.h"
#include "UIMgr.h"
#include "g_stringmanager.h"
#include "ItemProto.h"
#include "TcpUtil.h"
#include "Mouse.h"







extern DWORD			g_dwClientCountry;			// 0: Korean 1: Japanese  2: English


#define BASE_X_SIZE		640
#define BASE_Y_SIZE		480
#define BASE_X_SIZE_F	(640.0f)
#define BASE_Y_SIZE_F	(480.0f)

#define GATE_WIDTH	(219)	// * m_ScreenWidth / BASE_X_SIZE)
#define GATE_HEIGHT	(344)	// * m_ScreenHeight / BASE_Y_SIZE)
#define GATE_START_X	((m_ScreenWidth/2) - (GATE_WIDTH/2))
#define GATE_START_Y	((m_ScreenHeight/2) - (GATE_HEIGHT/2))

#define GATE_LIST_X	(GATE_START_X+17)	// * m_ScreenWidth / BASE_X_SIZE)
#define GATE_LIST_Y	(GATE_START_Y+40)	// * m_ScreenHeight / BASE_Y_SIZE)
#define GATE_LIST_WIDTH	(192)			// * m_ScreenWidth / BASE_X_SIZE)
#define GATE_LIST_HEIGHT	(250)		// * m_ScreenHeight / BASE_Y_SIZE)
#define GATE_LIST_FCOLOR	(RGB(255, 255, 0))

#define GATE_CLOSE_BTN_X		(370)	// * m_ScreenWidth / BASE_X_SIZE)
#define GATE_CLOSE_BTN_Y		(353)	// * m_ScreenHeight / BASE_Y_SIZE)

#define GATE_NUM				100

#define GATE_TYPE_MATRIX_CENTER   1010  // 게이트 타입별로 인덱스. ///국가대항전 시 부터 추가.



CControlSelectMenu::CControlSelectMenu()
	: m_ScreenWidth(0)
	, m_ScreenHeight(0)
{
	m_pSurfaceBack = NULL;
	m_MenuList = NULL;
	m_MenuType = -1;

	Init();
}

CControlSelectMenu::~CControlSelectMenu()
{
	DeleteRes();
	SAFE_DELETE( m_MenuList );
	m_vecReturnMsg.clear();
}

void CControlSelectMenu::Init()
{
	m_MenuType = -1;
	m_NumCount = 0;
	m_MenuList = new CListBox(" ");
	if( m_MenuList )
	{
		m_MenuList->SetImage("common/scroll_dn_t01", "common/scroll_up_t01", "interface/common/scroll_thumb_01.bmp");
		m_MenuList->SetSelectedLine(-1);
		m_MenuList->SetTextColor(GATE_LIST_FCOLOR);
	}
}

void CControlSelectMenu::ResetMenuList()
{
	SAFE_DELETE( m_MenuList );
}


BOOL CControlSelectMenu::SetMenuList(int type, char * arg)
{
	if( type < 0 || type > 2)
		return FALSE;

	ResetMenuList();
	Init();
	m_MenuType = type;

	switch( type )
	{
	case 0: // 후견인
		m_MenuList->AddString((char*)G_STRING(IDS_LHSTRING1938));
		m_MenuList->AddString((char*)G_STRING(IDS_LHSTRING1939));
		m_MenuList->AddString((char*)G_STRING(IDS_LHSTRING1940));
		m_NumCount = 3;
		break;

	case 1: // 견습생
		{
			if( !arg )
				return FALSE;

			char buf[128] = {0,};
			arg = AnyOneArg(arg, buf);
			m_NumCount = atoi(buf);

			for( int i=0; i<m_NumCount; i++)
			{
				arg = AnyOneArg(arg, buf);
				int vnum = atoi(buf);

				int protoNum = g_ItemList.FindItem( vnum );
				if( protoNum == -1 ) return FALSE;	//  아이템을 못 찾았다.
				char str[256] = {0,};
				sprintf( str, "%s +5", GET_ITEM_NAME_VNUM(vnum) );
				m_MenuList->AddString(str);
			}
		}
		break;
	}
	m_MenuList->SetPosDim(m_pSurfaceBack->Xpos+25, m_pSurfaceBack->Ypos+81, 191, 226);
	m_MenuList->LoadRes();

	return TRUE;
}




void CControlSelectMenu::AddMenuStart()
{
	m_MenuList->SetImage("common/scroll_dn_t01", "common/scroll_up_t01", "interface/common/scroll_thumb_01.bmp");
	m_MenuList->SetSelectedLine(-1);
	m_MenuList->SetTextColor(GATE_LIST_FCOLOR);
	m_NumCount = 0;
	m_vecReturnMsg.clear();
}

//void CControlSelectMenu::AddMenuPush( char * strMenuList )
//{	m_MenuList->AddString(strMenuList);	}

void CControlSelectMenu::AddMenuPush( int vnum )
{
	char str[128] = {0,};
	m_NumCount ++;
	int protoNum = g_ItemList.FindItem( vnum );
	if( protoNum == -1 ) return;	//  아이템을 못 찾았다.
	printf(str, "%d Goods : %s", m_NumCount, GET_ITEM_NAME_VNUM(vnum));
	m_MenuList->AddString(str);
	m_vecReturnMsg.push_back(vnum);
}

void CControlSelectMenu::AddMenuEnd()
{
	m_MenuList->SetPosDim(m_pSurfaceBack->Xpos+25, m_pSurfaceBack->Ypos+81, 191, 226);
	m_MenuList->LoadRes();
}

void CControlSelectMenu::LoadRes()
{
	if( !g_pDisplay || !g_pNk2DFrame )
		return;
	m_ScreenWidth = g_pNk2DFrame->GetClientWidth();
	m_ScreenHeight = g_pNk2DFrame->GetClientHeight();

	SAFE_DELETE(m_pSurfaceBack);

	m_CloseBtn.SetFileName("common/btn_close_01");

	g_pDisplay->CreateSurfaceFromBitmap(&m_pSurfaceBack, "interface/social/social_back.bmp");

	m_pSurfaceBack->SetColorKey(RGB(0, 0, 0));
	m_pSurfaceBack->Xpos = (g_pNk2DFrame->GetClientWidth() - m_pSurfaceBack->GetWidth())>>1;
	m_pSurfaceBack->Ypos = (g_pNk2DFrame->GetClientHeight() - m_pSurfaceBack->GetHeight())>>1;
	m_CloseBtn.LoadRes();

	m_CloseBtn.PosX = m_pSurfaceBack->Xpos+219;
	m_CloseBtn.PosY = m_pSurfaceBack->Ypos+5;

	if (m_MenuList)
	{
		m_MenuList->SetPosDim(m_pSurfaceBack->Xpos+25, m_pSurfaceBack->Ypos+81, 191, 226);
		m_MenuList->LoadRes();
	}
}


void CControlSelectMenu::DeleteRes()
{
	SAFE_DELETE(m_pSurfaceBack);
	m_CloseBtn.DeleteRes();
	if (m_MenuList)
		m_MenuList->DeleteRes();
}

void CControlSelectMenu::Draw()
{
	if (g_pDisplay && m_pSurfaceBack)
		g_pDisplay->Blt(m_pSurfaceBack->Xpos,m_pSurfaceBack->Ypos, m_pSurfaceBack);

	m_CloseBtn.Draw(m_pSurfaceBack->Xpos + 219,m_pSurfaceBack->Ypos + 5);
	if( m_MenuList )
		m_MenuList->Draw();

	POINT point;
	GetCursorPos(&point);
	ScreenToClient(g_pDisplay->GetHWnd(), &point);

	if (pCMyApp->m_pMouse && IsInside(point.x, point.y))
		pCMyApp->m_pMouse->SetMouseType(M_NORMAL);
}

BOOL CControlSelectMenu::IsInside(int x, int y)
{
	if( !m_pSurfaceBack )
		return false;

	return m_pSurfaceBack->IsIn(x, y);
}

LRESULT CControlSelectMenu::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int x, y, sel;
	int i = 0;

	switch ( msg )
	{
	case WM_LBUTTONUP:
	case WM_MOUSEMOVE:
		x = LOWORD (lParam);
		y = HIWORD (lParam);
		m_CloseBtn.MsgProc(hWnd, msg, wParam, lParam);
		if (g_pNk2DFrame && m_CloseBtn.GetState() == BTN_ACTION)
		{
			g_pNk2DFrame->ShowControlSelMenu(FALSE);
			m_CloseBtn.SetState(BTN_NORMAL);
		}
		if( m_MenuList )
			m_MenuList->GetMessage(hWnd, msg, wParam, lParam );
		if (!IsInside(x, y))
			return 0;
		return 1;

	case WM_LBUTTONDOWN:
		x = LOWORD (lParam);
		y = HIWORD (lParam);

		if (IsInside(x, y))
		{
			// Msg Popup창이 떠있을때는 클릭이 안되게 리턴~
			if( g_pNk2DFrame->Check_All_MsgPopUp() )
				return 1;

			if( pCMyApp->m_pUIMgr && m_CloseBtn.IsInside(x, y) )
				m_CloseBtn.MsgProc(hWnd, msg, wParam, lParam);

			if (m_MenuList && (sel = m_MenuList->GetMessage(hWnd, msg, wParam, lParam )) >= 0)
			{
				g_pNk2DFrame->ShowControlSelMenu(FALSE);
				m_CloseBtn.SetState(BTN_NORMAL);
				switch( m_MenuType )
				{
				case 0: // 후견인
					if( sel >= 0 || sel < m_NumCount )
					{
						char str[128] = {0,};
						sprintf(str, "support_npc %d\n", sel + 1 );
						g_pTcpIp->SendNetMessage(str);
					}
					break;
				case 1: // 견습생
					if( sel >= 0 || sel < m_NumCount )
					{
						char str[128] = {0,};
						sprintf(str, "student_npc %d\n", sel + 1 );
						g_pTcpIp->SendNetMessage(str);
					}
					break;
				}
			}
		}
		return 1;
	}
	return 1;
}
