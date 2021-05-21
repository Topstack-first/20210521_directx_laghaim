#include "stdafx.h"



#include <string.h>
#include <TCHAR.h>
#include <direct.h>
#include "main.h"
#include "basicbutton.h"
#include "headers.h"
#include "Item.h"
#include "ItemRender.h"
#include "ItemProto.h"
#include "SpecialItem.h"
#include "Nk2DFrame.h"
#include "UIMgr.h"
#include "tcpipcon.h"
#include "AlphaPopUp.h"
#include "textoutbox.h"
#include "controlbottom.h"
#include "controlinven.h"
#include "controltop.h"
#include "controlmap.h"
#include "ControlShop.h"
#include "ControlUpgrade.h"
#include "msgpopup.h"
#include "g_stringmanager.h"
#include "NkCharacter.h"
#include "DirtSnd.h"
#include "Mouse.h"
#include "Scroll.h"
#include "BasicMenu.h"

extern char g_strFontName[];




extern BOOL					g_bFixedWidthFont;


extern HFONT G_GetFont(int h, int w,const char  * const fontname);


///메뉴인터페이스
CControlMenu::CControlMenu()
{
	ZeroMemory( &m_BackSur, sizeof(sMenuSurface) ); // 인터페이스 배경
	Init(); // 변수들 초기화.
}

CControlMenu::~CControlMenu()
{
	DeleteRes();
}

void CControlMenu::ClearList() // 리스트 초기화.
{
	// 이미지 리스트.
	for( list<sMenuSurface *>::iterator iter = m_SurfaceList.begin() ; iter != m_SurfaceList.end() ; ++iter )
	{
		delete (*iter);
	}

	m_SurfaceList.clear();

	// 버튼 리스트.
	for( list<sMenuButton *>::iterator iter2 = m_BtnList.begin() ; iter2 != m_BtnList.end() ; ++iter2 )
	{
		delete (*iter2);
	}

	m_BtnList.clear();


	// 스트링
	for( list<sMenuString *>::iterator iter3 = m_StringList.begin() ; iter3 != m_StringList.end() ; ++iter3 )
	{
		delete (*iter3);
	}

	m_StringList.clear();

	// 스크롤 스트링

	for( vector<sMenuMultiScrollText *>::iterator iter4 = m_ScrollTextList.begin() ; iter4 != m_ScrollTextList.end() ; ++iter4 )
	{
		delete (*iter4);
	}

	m_ScrollTextList.clear();
}


void CControlMenu::Init()
{
	// 참조 포인터 초기화.
	m_TextOut.Init( 12, RGB(255,255,0), 0, 0 ); // 이걸 여기서 해주는 이유는 기본 폰트 설정을 위해서.

	m_ScrollTextList.reserve( 5 );
}

void CControlMenu::DeleteRes()
{
	ClearList();
	m_TextOut.DeleteRes();

	SAFE_DELETE( m_BackSur.m_pSurface );
	ZeroMemory( &m_BackSur, sizeof(sMenuSurface) ); // 이걸해줘야 다음 LoadRes때 문제가 안된다.(리스토어가 아닌 다른 메뉴창을 띄울때.)
}

HRESULT CControlMenu::RestoreSurfaces()
{
	sMenuSurface TempMenuSurface;
	ZeroMemory( &TempMenuSurface, sizeof(sMenuSurface) ); // 이걸해줘야 다음 LoadRes때 문제가 안된다.(리스토어가 아닌 다른 메뉴창을 띄울때.)


	if( m_BackSur.m_pSurface && m_BackSur.FileName ) // 복구해야하는 배경 이미지가 있다면...
	{
		// 예외적으로 정보를 기억.(DeleteRes에서 초기화하기때문)
		strcpy( TempMenuSurface.FileName, m_BackSur.FileName );
		TempMenuSurface.x = m_BackSur.x;
		TempMenuSurface.y = m_BackSur.y;
		TempMenuSurface.Width = m_BackSur.Width;
		TempMenuSurface.Height = m_BackSur.Height;
	}

	DeleteRes(); // 무식하게 다 지웠다가 로딩.

	if( TempMenuSurface.FileName ) // 복구해야하는 배경 이미지가 있다면...
	{
		// 로드하기 전에 정보를 돌려준다.
		strcpy( m_BackSur.FileName, TempMenuSurface.FileName );
		m_BackSur.x = TempMenuSurface.x;
		m_BackSur.y = TempMenuSurface.y;
		m_BackSur.Width = TempMenuSurface.Width;
		m_BackSur.Height = TempMenuSurface.Height;
	}

	LoadRes(); // 이녀석은 각 파일명들을 기억해뒀다가 다시 로드해야 할듯.

	return S_OK;
}

void CControlMenu::LoadRes()
{
	Init();

	// 이 녀석은 해상도를 신경쓸 필요가 없을듯. 파일명이 Add되어있는 녀석들을 로드한다.
	if( strlen(m_BackSur.FileName) ) // 파일명이 등록되었을 때만..
	{
		if( m_BackSur.m_pSurface ) // 기존께 있다면...
			SAFE_DELETE( m_BackSur.m_pSurface );

		g_pDisplay->CreateSurfaceFromBitmap( &m_BackSur.m_pSurface, m_BackSur.FileName );

		if( m_BackSur.m_pSurface )
		{
			m_BackSur.m_pSurface->Xpos = m_BackSur.x;
			m_BackSur.m_pSurface->Ypos = m_BackSur.y;

			m_BackSur.m_pSurface->SetWidth( m_BackSur.Width ); // 이건 꼭 지정되어야 한다.
			m_BackSur.m_pSurface->SetHeight( m_BackSur.Height );

			m_BackSur.m_pSurface->SetColorKey( RGB(0, 0, 0) );
		}
	}

	for( list<sMenuSurface *>::iterator iter = m_SurfaceList.begin() ; iter != m_SurfaceList.end() ; ++iter )
	{
		CSurface *TempSurface = (*iter)->m_pSurface;

		g_pDisplay->CreateSurfaceFromBitmap( &TempSurface, (*iter)->FileName );

		if( TempSurface )
		{
			if( (*iter)->Width != -1 )  // -1이면 기억을 할 필요가 없는 경우.
				TempSurface->SetWidth( (*iter)->Width );
			if( (*iter)->Height != -1 )  // -1이면 기억을 할 필요가 없는 경우.
				TempSurface->SetHeight( (*iter)->Height );
		}
	}

	for( list<sMenuButton *>::iterator iter2 = m_BtnList.begin() ; iter2 != m_BtnList.end() ; ++iter2 )
	{
		(*iter2)->m_Button.LoadRes();
		(*iter2)->m_Button.SetPosition( (*iter2)->x, (*iter2)->y );
	}

	// 스크롤 스트링
	for( vector<sMenuMultiScrollText *>::iterator iter3 = m_ScrollTextList.begin() ; iter3 != m_ScrollTextList.end() ; ++iter3 )
	{
		(*iter3)->ScrollText.SetScrollImage( (*iter3)->DecBtnName, (*iter3)->IncBtnName, (*iter3)->BarName ); // 이걸안하면 스크롤 없는 녀석으로 쓸수있지만 일단 메뉴용은 그냥 스크롤 무조건 넣는다.(나중에 싫으면 플래그라도 두도록...)
		(*iter3)->ScrollText.LoadRes( (*iter3)->OutRect.left, (*iter3)->OutRect.top, (*iter3)->OutRect.right, (*iter3)->OutRect.bottom );  // 스크롤 리소스용.
		(*iter3)->ScrollText.MakeSurface(); // 텍스트 리소스용. (이것도 LoadRes() 뒤에 해주는게 좋다)
		(*iter3)->ScrollText.SetScrollPos( (*iter3)->ScrollPosX, (*iter3)->ScrollPosY, (*iter3)->ScrollHeight ); // 이건 LoadRes() 뒤에 해줘야한다.
	}
}

void CControlMenu::AddBackGround( const char *FileName, int PosX, int PosY, int Width, int Height )
{
	if( !FileName )
		return;

	m_hWnd =  g_pDisplay->GetHWnd();  // 이녀석은 해상도에 영향을 받을수있다. x,y가 -1로 들어올때...
	GetClientRect( m_hWnd, &m_ClientRc );

	if( PosX == -1 && PosY == -1 ) // 좌표 입력이 안되었을 경우 Width,Height를 기준으로 화면의 중앙에 위치하게 한다.
	{
		PosX = ( m_ClientRc.right - Width ) / 2;
		PosY = ( m_ClientRc.bottom - Height ) / 2;
	}

	strcpy( m_BackSur.FileName, FileName ); // 이름을 기억.
	m_BackSur.x = PosX;
	m_BackSur.y = PosY;
	m_BackSur.Width = Width;
	m_BackSur.Height = Height;

	m_BackSur.m_pSurface = NULL; // 초기화.
}

void CControlMenu::AddSurface( const char *FileName, int PosX, int PosY, int Width, int Height ) // 이녀석은 필요에 따라 가로 세로를 입력해두어야한다.
{
	if( !FileName )
		return;

	sMenuSurface *TempMenuSurface = NULL;
	TempMenuSurface = new sMenuSurface; // 메모리를 새로 잡아준다.

	if( !TempMenuSurface )
		return;

	PosX += m_BackSur.x; // 좌표는 배경을 기준으로 하는 좌표계가 되야한다.
	PosY += m_BackSur.y;

	strcpy( TempMenuSurface->FileName, FileName ); // 이름을 기억.
	TempMenuSurface->x = PosX;
	TempMenuSurface->y = PosY;
	TempMenuSurface->Width = Width;
	TempMenuSurface->Height = Height;

	TempMenuSurface->m_pSurface = NULL; // 초기화.

	m_SurfaceList.push_back( TempMenuSurface );
}

void CControlMenu::AddButton( const char *FileName, int PosX, int PosY, int ActionIndex )
{
	if( !FileName )
		return;

	sMenuButton *TempButton = NULL;
	TempButton = new sMenuButton; // 메모리를 새로 잡아준다.

	if( !TempButton )
		return;

	TempButton->ActionIndex = ActionIndex;

	PosX += m_BackSur.x; // 좌표는 배경을 기준으로 하는 좌표계가 되야한다.
	PosY += m_BackSur.y;

	TempButton->x = PosX;
	TempButton->y = PosY;

	TempButton->m_Button.SetFileName( (char*)FileName );

	m_BtnList.push_back( TempButton );
}

void CControlMenu::AddString( const char *String, int PosX, int PosY, BOOL bCenter )
{
	if( !String )
		return;

	if (!g_pDisplay || !m_TextOut.m_Font )	// m_Font 는 핸들이다
		return;

	sMenuString *TempString = NULL;
	TempString = new sMenuString; // 메모리를 새로 잡아준다.

	if( !TempString )
		return;

	if( bCenter ) // 좌표가 글자 중간을 의미하는 경우이면..
	{
		SIZE size;

		HDC hdc;
		hdc = GetDC(pCMyApp->Get_hWnd());
		HFONT hFont = (HFONT) SelectObject( hdc, m_TextOut.m_Font );

		int len = strlen( String );

		if (g_bFixedWidthFont)
			size.cx = len * m_TextOut.m_FontWidth;
		else
			GetTextExtentPoint32( hdc, String, len, &size );

		PosX -= size.cx/2; // 길이의 절반을 앞으로 간다.

		SelectObject(hdc, hFont);
		ReleaseDC(pCMyApp->Get_hWnd(), hdc);
	}

	PosX += m_BackSur.x; // 좌표는 배경을 기준으로 하는 좌표계가 되야한다.
	PosY += m_BackSur.y;

	TempString->x = PosX;
	TempString->y = PosY;

	strcpy( TempString->String, String );

	m_StringList.push_back( TempString );
}

CMultiScrollText* CControlMenu::AddScrollText( int PosX, int PosY, int Width, int Height, int ScrollPosX, int ScrollPosY, int ScrollHeight , const char *DecBtn, const char *IncBtn, const char *Bar)
{
	sMenuMultiScrollText *TempScrollText = NULL;
	TempScrollText = new sMenuMultiScrollText; // 메모리를 새로 잡아준다.

	if( !TempScrollText )
		return NULL;

	PosX += m_BackSur.x; // 좌표는 배경을 기준으로 하는 좌표계가 되야한다.
	PosY += m_BackSur.y;

	TempScrollText->OutRect.left = PosX;
	TempScrollText->OutRect.top = PosY;
	TempScrollText->OutRect.right = PosX+Width;
	TempScrollText->OutRect.bottom = PosY+Height;

	ScrollPosX += m_BackSur.x; // 좌표는 배경을 기준으로 하는 좌표계가 되야한다.
	ScrollPosY += m_BackSur.y;

	TempScrollText->ScrollPosX = ScrollPosX;
	TempScrollText->ScrollPosY = ScrollPosY;
	TempScrollText->ScrollHeight = ScrollHeight;

	if( DecBtn )
		strcpy( TempScrollText->DecBtnName, DecBtn );
	else // 널이었다면..
		TempScrollText->DecBtnName[0] = '\0';

	if( IncBtn )
		strcpy( TempScrollText->IncBtnName, IncBtn );
	else // 널이었다면..
		TempScrollText->IncBtnName[0] = '\0';

	if( Bar )
		strcpy( TempScrollText->BarName, Bar );
	else // 널이었다면..
		TempScrollText->BarName[0] = '\0';

	m_ScrollTextList.push_back( TempScrollText );

	return &(TempScrollText->ScrollText); // 포인터를 리턴시켜줘서 추후에도 접근할수있도록 해준다.
}

void CControlMenu::Draw()
{
	if( m_BackSur.m_pSurface )
		g_pDisplay->Blt( m_BackSur.m_pSurface->Xpos, m_BackSur.m_pSurface->Ypos, m_BackSur.m_pSurface ); // 배경.

	for( list<sMenuSurface *>::iterator iter = m_SurfaceList.begin() ; iter != m_SurfaceList.end() ; ++iter )
	{
		g_pDisplay->Blt( (*iter)->m_pSurface->Xpos, (*iter)->m_pSurface->Ypos, (*iter)->m_pSurface );
	}

	// 버튼 리스트.
	for( list<sMenuButton *>::iterator iter2 = m_BtnList.begin() ; iter2 != m_BtnList.end() ; ++iter2 )
	{
		(*iter2)->m_Button.Draw();
	}

	// 스트링
	for( list<sMenuString *>::iterator iter3 = m_StringList.begin() ; iter3 != m_StringList.end() ; ++iter3 )
	{
		m_TextOut.SetPos( (*iter3)->x, (*iter3)->y );
		m_TextOut.SetString( (*iter3)->String, FALSE );
		m_TextOut.Draw();
	}

	// 스크롤 스트링
	for( vector<sMenuMultiScrollText *>::iterator iter4 = m_ScrollTextList.begin() ; iter4 != m_ScrollTextList.end() ; ++iter4 )
	{
		(*iter4)->ScrollText.Draw();
	}

	POINT point;
	GetCursorPos(&point);
	ScreenToClient(g_pDisplay->GetHWnd(), &point);
	if (pCMyApp->m_pMouse && IsInside(point.x, point.y))
		pCMyApp->m_pMouse->SetMouseType(M_NORMAL);
}

LRESULT CControlMenu::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	static char commOutBuf[512];
	int x, y;
	x = LOWORD( lParam );
	y = HIWORD( lParam );

	list<sMenuButton *>::iterator iter;

	// 스크롤 텍스트는 스크롤 때문에 메시지 프로시져를 돌아줘야한다.
	for( vector<sMenuMultiScrollText *>::iterator iter2 = m_ScrollTextList.begin() ; iter2 != m_ScrollTextList.end() ; ++iter2 )
	{
		if( (*iter2)->ScrollText.MsgProc(hWnd, msg, wParam, lParam) )
			return 1;
	}


	switch (msg)
	{
	case 0x020A: // 휠마우스 메세지(리스트 영역안에서의 휠 메세지를 강제로 스크롤에 연동시켜준다.)
		break; // 이걸 안해주면 스크롤 텍스트께 안먹는다.(원인은 모르겠슴.)
	case WM_LBUTTONDOWN:

		if( IsInside(x,y) ) // 인터페이스의 안쪽을 클릭했을 경우.
		{
			for( iter = m_BtnList.begin() ; iter != m_BtnList.end() ; ++iter )
			{
				(*iter)->m_Button.MsgProc(hWnd, msg, wParam, lParam); // 전 버튼을 돌면서 해주낟.
			}

			return 1;
		}
		else // 인터페이스 이외의 부분을 클릭시.(닫아버리자.)
		{
			g_pNk2DFrame->ShowControlMenuWindow( FALSE ); // 강제 종료시킨다.
		}

		return 0; // 다른 인터페이스 부분을 찍었을 경우...(인벤,Bottom등...)

	case WM_MOUSEMOVE:
	case WM_LBUTTONUP:

		if( IsInside(x,y) ) // 인터페이스의 안쪽을 클릭했을 경우.
		{
			for( iter = m_BtnList.begin() ; iter != m_BtnList.end() ; ++iter )
			{
				(*iter)->m_Button.MsgProc(hWnd, msg, wParam, lParam); // 전 버튼을 돌면서 해주낟.

				if( (*iter)->m_Button.GetState()==BTN_ACTION ) // 수정 버튼 클릭시
				{
					(*iter)->m_Button.SetState(BTN_NORMAL);	// 버튼의 스테이터스를 직접 풀어줘야하나보다.
					BtnAction( (*iter)->ActionIndex ); // 작동 처리. // 여기 튕기는 버그.

					return 1; // BtnAction 에서 메모리를 건드리면 반복자가 무효가 되나 보다. 그래서 다음 반복자에서 튕기더라. 어차피 버튼은 한번에 하나만 작동하면 되니 리턴시키자.
				}
			}

			return 1;
		}

		break;
	}
	return 0;
}

BOOL CControlMenu::IsInside( int x, int y ) // 배경 영역 체크.
{
	if( !m_BackSur.m_pSurface )
		return FALSE;

	return m_BackSur.m_pSurface->IsIn( x,y );
}

void CControlMenu::BtnAction( int ActionIndex ) // 버튼을 눌렀을때 동작할 코드.
{
	char m_commOut[512];

	if( ActionIndex > 10000 ) // 10000번 대 인덱스는 서버로 부터 상점 NPC 인덱스를 받아서 처리하는 경우다. 해당 인덱스 -10000 으로 shop 오픈 패킷을 보낸다.
	{
		g_pNk2DFrame->ShowControlMenuWindow( FALSE ); // 이 녀석은 상점창 때문에 미리 닫아야한다.

		g_pNk2DFrame->ToggleShopWindow();
		if (g_pNk2DFrame->IsShopVisible())
		{			
			g_pNk2DFrame->GetControlShop()->SetKeeper( ActionIndex-10000, locationX, locationZ );

			sprintf(m_commOut, "shop %d %d %d 1\n", ActionIndex-10000, (int)locationX, (int)locationZ ); // 좌표는 하드 코딩
			if( g_pTcpIp )
				g_pTcpIp->SendNetMessage(m_commOut);
		}

		return;
	}

	switch( ActionIndex )
	{
	case MENU_ACTION_USERSHOP_SEARCH: // 개인상점 찾기
		g_pNk2DFrame->ToggleUserShopSearchWindow( FALSE ); ///개인상점검색.
		break;

	case MENU_ACTION_USERSHOP_RECEIVE: // 개인상점 회수
		sprintf(m_commOut, "u_shop req\n" );
		if( g_pTcpIp )
			g_pTcpIp->SendNetMessage(m_commOut);

		g_pNk2DFrame->ShowControlMenuWindow( FALSE ); // 패킷보낸후 닫아준다.

		break;

	case MENU_ACTION_NPCUPGRADE_UPGRADE: // 강화
		g_pNk2DFrame->ToggleUpgradeWindow( INTERFACE_TYPE_UPARMOR ); // 강화 인터페이스 출력
		break;

	case MENU_ACTION_NPCUPGRADE_CONVERSION: // 컨버젼
		g_pNk2DFrame->ToggleUpgradeWindow( INTERFACE_TYPE_CONVERSION ); // 컨버젼 인터페이스 출력
		break;

	case MENU_ACTION_NPCUPGRADE_CONFUSION: // 컨퓨젼
		g_pNk2DFrame->ToggleUpgradeWindow( INTERFACE_TYPE_CONFUSION ); // 컨퓨젼 인터페이스 출력
		break;

	case MENU_ACTION_NPCUPGRADE_REFORM: // 가공
		g_pNk2DFrame->ToggleUpgradeWindow( INTERFACE_TYPE_PROCESS ); // 가공 인터페이스 출력
		break;

	case MENU_ACTION_NPCUPGRADE_ARMOR400:
		g_pNk2DFrame->ToggleUpgradeWindow( INTERFACE_TYPE_UPARMOR_400 ); // 가공 인터페이스 출력
		break;

	case MENU_ACTION_CLOSE: // 닫기
		g_pNk2DFrame->ShowControlMenuWindow( FALSE );
		break;

	case MENU_ACTION_ENCHANT_UPGRADE:	// 강화 버튼
		if( g_SvrType == ST_ADULT_ONLY ) // [2/27/2007 Theodoric] 인첸트 : 선택창 버튼 클릭시 행동들
			g_pNk2DFrame->TogglePlatinumExchWindow(0);
		break;

	case MENU_ACTION_ENCHANT_MAKE:	// 플래티늄 제작
		if( g_SvrType == ST_ADULT_ONLY ) // [2/27/2007 Theodoric] 인첸트 : 선택창 버튼 클릭시 행동들
			g_pNk2DFrame->TogglePlatinumExchWindow(1);
		break;

	case MENU_ACTION_ENCHANT_INIT:	// 옵션 초기화
		if( g_SvrType == ST_ADULT_ONLY ) // [2/27/2007 Theodoric] 인첸트 : 선택창 버튼 클릭시 행동들
			g_pNk2DFrame->TogglePlatinumExchWindow(2);
		break;
	case MENU_ACTION_NEW_NPCUPGRADE_CONVERSION:
		g_pNk2DFrame->ToggleUpgradeWindow(INTERFACE_TYPE_320LV_WEAPON_CONVERSION);
		break;

	case MENU_ACTION_NEW_NPCUPGRADE_UPGRADE:
		g_pNk2DFrame->ToggleUpgradeWindow(INTERFACE_TYPE_320LV_WEAPON_UPGRADE);
		break;

	case MENU_ACTION_GODWEAPON_UP:
		g_pNk2DFrame->ToggleUpgradeWindow(INTERFACE_TYPE_GOD_WEAPON_UPGRADE);
		break;

	case MENU_ACTION_NEW_NPCUPGRADE_EXCHANGE:
		g_pNk2DFrame->ToggleUpgradeWindow(INTERFACE_TYPE_320LV_WEAPON_GODWPOWER);
		break;

	case MENU_ACTION_ITEMMAKE:
		g_pNk2DFrame->ToggleUpgradeWindow(INTERFACE_TYPE_ITEMMAKE);
		break;

	case MENU_ACTION_ITEMSEPARATE:
		g_pNk2DFrame->ToggleUpgradeWindow(INTERFACE_TYPE_ITEMSEPARATE);
		break;

	case MENU_ACTION_GMAGICSTONE:
		g_pNk2DFrame->ToggleUpgradeWindow(INTERFACE_TYPE_GMAGICSTONE);
		break;

	case MENU_ACTION_BUFFMAKE_MAKE:
		{
			g_pNk2DFrame->ToggleUpgradeWindow(INTERFACE_TYPE_BUFFMAKE_MAKE);
		}
		break;

	case MENU_ACTION_BUFFMAKE_GATCHA:
		{
			g_pNk2DFrame->ToggleUpgradeWindow(INTERFACE_TYPE_BUFFMAKE_GATCHA);
		}
		break;

	case MENU_ACTION_BUFFMAKE_MEDAL:
		{
			g_pNk2DFrame->ToggleUpgradeWindow(INTERFACE_TYPE_BUFFMAKE_MEDAL);
		}
		break;

	default:
		// 현재 디폴트론 아무것도 안한다.
		break;
	}
}



CMultiScrollText::CMultiScrollText()
	: m_Xpos(0)
	, m_Ypos(0)
	, m_TextSurWidth(0)
	, m_TextSurHeight(0)
{
	m_pTextSur	= NULL;
	m_pScroll = NULL; // 초기화를 위해서.

	// 참조포인터 초기화
	m_Font		= NULL;

	ZeroMemory( m_DecBtnName, sizeof(char)*MENU_FILE_NAME_LEN ); // 0으로 초기화를 해놔준다.(그래야 디폴트로 인식이 될듯.)
	ZeroMemory( m_IncBtnName, sizeof(char)*MENU_FILE_NAME_LEN ); // 0으로 초기화를 해놔준다.(그래야 디폴트로 인식이 될듯.)
	ZeroMemory( m_BarName, sizeof(char)*MENU_FILE_NAME_LEN ); // 0으로 초기화를 해놔준다.(그래야 디폴트로 인식이 될듯.)

	Init(); // 여기서도 한번 호출을 해줘야할 듯.
}

CMultiScrollText::~CMultiScrollText()
{
	DeleteRes();
}

void CMultiScrollText::Init( int FontSize )
{
	// 참조포인터 초기화
	m_Font		= NULL;

	SetFont(FontSize/2, FontSize, g_strFontName);

	SIZE	sizeFont;
	getFontSize(&sizeFont, m_Font, "A", 1);

	if( pCMyApp->Font_AddPosY == 0 )
	{
		m_FontWidth  = sizeFont.cx;
		m_FontHeight = sizeFont.cy+4;
	}
	else
	{
		m_FontWidth  = sizeFont.cx;
		m_FontHeight = sizeFont.cy+1;
	}

	SetViewRect( 0, 0, 0, 0 ); // 뷰렉트도 초기화.

	m_ViewHeightSize = 0;
}

void CMultiScrollText::DeleteRes()
{
	SAFE_DELETE( m_pScroll ); // 스크롤 리소스의 리스토어가 안되는 문제가 있다.
	SAFE_DELETE( m_pTextSur );
}

void CMultiScrollText::LoadRes( int l, int t, int r,int b )
{
	Init();

	SetViewRect( l, t, r, b );

	if( m_pScroll ) // 없을 수도 있다.
		m_pScroll->LoadRes( m_DecBtnName, m_IncBtnName, m_BarName ); //스크롤 리소스를 로드.
}

void CMultiScrollText::SetViewRect( int l, int t, int r,int b ) //로드 리소스 후에 실행되야한다.
{
	m_ViewRect.left = l;
	m_ViewRect.top = t;
	m_ViewRect.right = r;
	m_ViewRect.bottom = b;

	m_ViewHeightSize = m_ViewRect.bottom - m_ViewRect.top;
}

void CMultiScrollText::SetFont (int f_w, int f_h, char *fontname)
{
	m_Font = G_GetFont(f_h,f_w,fontname);
}

void CMultiScrollText::SetScrollImage( char *DecBtn, char *IncBtn, char *ScrollBar )
{
	SAFE_DELETE( m_pScroll );

	m_pScroll = new CScroll(); // 메모리를 잡고, 이름을 셋팅해두는역활.

	if( DecBtn[0] )
		strcpy( m_DecBtnName, DecBtn );
	else // 널이었다면..
		m_DecBtnName[0] = '\0';

	if( IncBtn[0] )
		strcpy( m_IncBtnName, IncBtn );
	else // 널이었다면..
		m_IncBtnName[0] = '\0';

	if( ScrollBar[0] )
		strcpy( m_BarName, ScrollBar );
	else // 널이었다면..
		m_BarName[0] = '\0';
}

void CMultiScrollText::SetScrollPos( int x, int y, int size ) // 스크롤 위치및 크기를 변경하고 싶을때 사용.
{
	if( !m_pScroll )
		return;

	if( x == -1 ) // 디폴트 위치.(박스 오른쪽 옆)
	{
		x =  m_ViewRect.right;
	}

	if( y == -1 ) // 디폴트 위치.(박스 오른쪽 옆)
	{
		y =  m_ViewRect.top;
	}

	if( size == -1 ) // 디폴트 위치.(박스 오른쪽 옆)
	{
		size =  m_ViewHeightSize;
	}

	m_pScroll->SetDrawPos( x, y, size ); // 스크롤을 조정.
}

void CMultiScrollText::AddString( char *String, int LineLen, DWORD color, DWORD back_color ) // 스트링 라인 추가. 그냥 멀티텍스트에 넣기 쉽게 해주기 용.
{
	if( LineLen == -1 )
		m_MultiText.AddString( String, color, back_color );
	else
		m_MultiText.AddStringMulti( String, LineLen, FALSE, color, back_color );
}

void CMultiScrollText::LoadString( char *FileName ) // 텍스트 파일로 부터 스트링을 읽어오는 함수.
{
	// by evilkiki 2009.09.03 텍스트 파일 읽기 오류 관련 임시 처리
// 	PFILE *fp = NULL;
// 	char strTemp[512] = {0,};
// 	int LineCount = 0;
// 	int LineLen = 0;
//
// 	fp = pfopen( FileName, "rt");
//
// 	if( fp )
// 	{
// 		pfscanf( fp, "%d", &LineCount ); // 줄 겟수.
//
// 		for( int i = 0 ; i < LineCount ; ++i )
// 		{
// 			pfgets( strTemp, 512, fp ); // 한줄을 읽어오고...(한줄에 512바이트 제한)
// 			LineLen = strlen( strTemp ); // 개행문자의 위치를 얻어오고.
//
// 			strTemp[LineLen-1] = '\0'; // 종료문자로 바꿔준다.
//
// 			m_MultiText.AddString( strTemp ); // 통째로 넣는다. (현재는 색 같은건 지원하지 않는다.)
// 		}
//
// 		pfclose( fp );
// 	}

	FILE *fp = NULL;
	char strTemp[512] = {0,};
	int LineCount = 0;
	int LineLen = 0;

	fp = fopen( FileName, "rt");

	m_MultiText.Reset();

	if( fp )
	{
		fscanf( fp, "%d", &LineCount ); // 줄 겟수.

		for( int i = 0 ; i < LineCount ; ++i )
		{
			fgets( strTemp, 512, fp ); // 한줄을 읽어오고...(한줄에 512바이트 제한)
			LineLen = strlen( strTemp ); // 개행문자의 위치를 얻어오고.

			strTemp[LineLen-1] = '\0'; // 종료문자로 바꿔준다.

			m_MultiText.AddString( strTemp ); // 통째로 넣는다. (현재는 색 같은건 지원하지 않는다.)
		}

		fclose( fp );
	}

}

void CMultiScrollText::MakeSurface() // 멀티 텍스트의 내용을 서피스로 만들어주는 녀석.
{
	if (!g_pDisplay || !m_Font )	// m_Font 는 핸들이다
		return;

	int GreatestCount=0; // 가장큰 문자열의 개수를 얻는다
	int TextSurWidth = 0;
	SIZE size;

	HDC hdc;
	hdc = GetDC(pCMyApp->Get_hWnd());
	HFONT hFont = (HFONT) SelectObject( hdc, m_Font );

	// 가장큰 문자열의 개수를 얻는다
	int i = 0;
	for(i = 0; i < m_MultiText.m_LineCount; i++)
	{
		// 입력받은 텍스트를 루프를 돌면서 가장긴 문자열 체크
		int len = strlen(m_MultiText.m_Text[i]);
		// 문자열의 폭이 고정되었는가?
		if (g_bFixedWidthFont)
		{
			if(GreatestCount < len)
				GreatestCount = len;
		}
		else
		{
			// 폰트 사이즈 반환
			GetTextExtentPoint32(hdc, m_MultiText.m_Text[i], len, &size);
			if (size.cx > TextSurWidth)
				TextSurWidth = size.cx;
		};
	}

	SAFE_DELETE(m_pTextSur);
	if (g_bFixedWidthFont)
	{
		// 가장긴 문자열과 라인수에 의해서 서피스 생성
		m_TextSurWidth = GreatestCount*m_FontWidth;
		m_TextSurHeight = m_MultiText.m_LineCount*m_FontHeight;
	}
	else
	{
		m_TextSurWidth = TextSurWidth;
		m_TextSurHeight = m_MultiText.m_LineCount*m_FontHeight;

	}

	// 계산된 크기로 서피스 생성
	g_pDisplay->CreateSurface( &m_pTextSur, m_TextSurWidth, m_TextSurHeight );
	if( !m_pTextSur )
		return;
	// fillcolor를 이용한 textsurface 클리어하기
	m_pTextSur->Clear();

	m_Xpos = 0;
	m_Ypos = 0;
	for(i=0; i<m_MultiText.m_LineCount; i++)
	{
		m_pTextSur->DrawText(m_Font, m_MultiText.m_Text[i], m_Xpos, m_Ypos,
							 m_MultiText.m_TextBackColor[i], m_MultiText.m_TextColor[i], 0);
		m_Ypos+=m_FontHeight;
	}
	m_pTextSur->SetColorKey(TEXT_BACK_COLOR);

	SelectObject(hdc, hFont);
	ReleaseDC(pCMyApp->Get_hWnd(), hdc);

	if( m_pScroll ) // 스크롤이 있다면 스크롤의 범위를 갱신해준다.
		m_pScroll->SetRange( m_TextSurHeight + 1 - m_ViewHeightSize ); // +1 해줘야 맞더라.
}

void CMultiScrollText::Draw()
{
	// 서피스 이미지의 부분을 출력.
	RECT TempRect;

	if( !m_pScroll || m_pScroll->RangeMax < 0 ) // 스크롤 없이 출력하는 경우.
	{
		g_pDisplay->Blt( m_ViewRect.left, m_ViewRect.top, m_pTextSur ); // 만땅 출력.
	}
	else // 스크롤이 있는 경우.
	{
		int StrDrawHeight = m_pScroll->GetPos()+m_ViewHeightSize; // 영역 출력은 범위가 원래 크기보다 크면 아예 출력을 안해버리더라. 그래서 보정이 필요하다.

		if( StrDrawHeight > m_TextSurHeight ) // m_TextSurHeight 이하로 줄여야한다.
			StrDrawHeight = m_TextSurHeight;

		TempRect.left = 0;
		TempRect.right = m_TextSurWidth; // 가로 세로는 풀로...
		TempRect.top = m_pScroll->GetPos(); // 스크롤을 픽셀단위 스크롤 했기때문에 바로 GetPos()를 써도 된다.
		TempRect.bottom = StrDrawHeight;

		g_pDisplay->Blt( m_ViewRect.left, m_ViewRect.top, m_pTextSur, &TempRect );
	}

	if( m_pScroll && m_pScroll->RangeMax > 0 ) // 스크롤 범위가 안 필요할때는 출력 안함.
		m_pScroll->Draw();
}

LRESULT CMultiScrollText::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam ) // 스크롤용.
{
	if( !m_pScroll || m_pScroll->RangeMax <= 0 ) // 스크롤 범위가 안 필요할때는 처리 안함.
		return 0;

	if( m_pScroll->MsgProc(hWnd, msg, wParam, lParam) == S_OK )
		return 1;

	int x, y;
	x = LOWORD( lParam );
	y = HIWORD( lParam );

	switch (msg)
	{
	case 0x020A: // 휠마우스 메세지(리스트 영역안에서의 휠 메세지를 강제로 스크롤에 연동시켜준다.)

		if (x >= m_ViewRect.left
				&& x < m_ViewRect.right
				&& y >= m_ViewRect.top
				&& y < m_ViewRect.bottom )
		{
			if( (short)HIWORD(wParam) < 0 )
			{
				m_pScroll->NowPos += 10; // 픽셀단위니까 좀 넓직하게.

				if( m_pScroll->NowPos > m_pScroll->RangeMax-1 )
				{
					m_pScroll->NowPos = m_pScroll->RangeMax-1;
				}
			}
			else if( (short)HIWORD(wParam) > 0 )
			{
				m_pScroll->NowPos -= 10; // 픽셀단위니까 좀 넓직하게.

				if( m_pScroll->NowPos < 0 )
				{
					m_pScroll->NowPos = 0;
				}
			}
			return 1;
		}

		break;
	}

	return 0;
}



//////////////////////////////////////////////////////////////////////////




CMultiScrollText2::CMultiScrollText2()
	: m_Xpos(0)
	, m_Ypos(0)
	, m_TextSurWidth(0)
	, m_TextSurHeight(0)
{
	m_pTextSur	= NULL;
	m_pScroll = NULL; // 초기화를 위해서.

	// 참조포인터 초기화
	m_Font		= NULL;

	ZeroMemory( m_DecBtnName, sizeof(char)*MENU_FILE_NAME_LEN ); // 0으로 초기화를 해놔준다.(그래야 디폴트로 인식이 될듯.)
	ZeroMemory( m_IncBtnName, sizeof(char)*MENU_FILE_NAME_LEN ); // 0으로 초기화를 해놔준다.(그래야 디폴트로 인식이 될듯.)
	ZeroMemory( m_BarName, sizeof(char)*MENU_FILE_NAME_LEN ); // 0으로 초기화를 해놔준다.(그래야 디폴트로 인식이 될듯.)

	Init(); // 여기서도 한번 호출을 해줘야할 듯.
}

CMultiScrollText2::~CMultiScrollText2()
{
	DeleteRes();
}

void CMultiScrollText2::Init( int FontSize )
{
	// 참조포인터 초기화
	m_Font		= NULL;

	SetFont(FontSize/2, FontSize, g_strFontName);

	SIZE	sizeFont;
	getFontSize(&sizeFont, m_Font, "A", 1);

	if( pCMyApp->Font_AddPosY == 0 )
	{
		m_FontWidth  = sizeFont.cx;
		m_FontHeight = sizeFont.cy+4;
	}
	else
	{
		m_FontWidth  = sizeFont.cx;
		m_FontHeight = sizeFont.cy+1;
	}

	SetViewRect( 0, 0, 0, 0 ); // 뷰렉트도 초기화.

	m_ViewHeightSize = 0;
}

void CMultiScrollText2::DeleteRes()
{
	SAFE_DELETE( m_pScroll ); // 스크롤 리소스의 리스토어가 안되는 문제가 있다.
	SAFE_DELETE( m_pTextSur );
}

void CMultiScrollText2::LoadRes( int l, int t, int r,int b )
{
	Init();

	SetViewRect( l, t, r, b );

	if( m_pScroll ) // 없을 수도 있다.
		m_pScroll->LoadRes( m_DecBtnName, m_IncBtnName, m_BarName ); //스크롤 리소스를 로드.
}

void CMultiScrollText2::SetViewRect( int l, int t, int r,int b ) //로드 리소스 후에 실행되야한다.
{
	m_ViewRect.left = l;
	m_ViewRect.top = t;
	m_ViewRect.right = r;
	m_ViewRect.bottom = b;

	m_ViewHeightSize = m_ViewRect.bottom - m_ViewRect.top;
}

void CMultiScrollText2::SetFont (int f_w, int f_h, char *fontname)
{
	m_Font = G_GetFont(f_h,f_w,fontname);
}

void CMultiScrollText2::SetScrollImage( char *DecBtn, char *IncBtn, char *ScrollBar )
{
	SAFE_DELETE( m_pScroll );

	m_pScroll = new CScroll(); // 메모리를 잡고, 이름을 셋팅해두는역활.

	if( DecBtn[0] )
		strcpy( m_DecBtnName, DecBtn );
	else // 널이었다면..
		m_DecBtnName[0] = '\0';

	if( IncBtn[0] )
		strcpy( m_IncBtnName, IncBtn );
	else // 널이었다면..
		m_IncBtnName[0] = '\0';

	if( ScrollBar[0] )
		strcpy( m_BarName, ScrollBar );
	else // 널이었다면..
		m_BarName[0] = '\0';
}

void CMultiScrollText2::SetScrollPos( int x, int y, int size ) // 스크롤 위치및 크기를 변경하고 싶을때 사용.
{
	if( !m_pScroll )
		return;

	if( x == -1 ) // 디폴트 위치.(박스 오른쪽 옆)
	{
		x =  m_ViewRect.right;
	}

	if( y == -1 ) // 디폴트 위치.(박스 오른쪽 옆)
	{
		y =  m_ViewRect.top;
	}

	if( size == -1 ) // 디폴트 위치.(박스 오른쪽 옆)
	{
		size =  m_ViewHeightSize;
	}

	m_pScroll->SetDrawPos( x, y, size ); // 스크롤을 조정.
}

void CMultiScrollText2::AddString( char *String, int LineLen, DWORD color, DWORD back_color ) // 스트링 라인 추가. 그냥 멀티텍스트에 넣기 쉽게 해주기 용.
{
	if( LineLen == -1 )
		m_MultiText.AddString( String, color, back_color );
	else
		m_MultiText.AddStringMulti( String, LineLen, FALSE, color, back_color );
}

void CMultiScrollText2::LoadString( char *FileName ) // 텍스트 파일로 부터 스트링을 읽어오는 함수.
{
	FILE *fp = NULL;
	char strTemp[512];
	int LineCount = 0;
	int LineLen = 0;

	fp = fopen( FileName, "rt");

	if( fp )
	{
		fscanf( fp, "%d", &LineCount ); // 줄 겟수.

		for( int i = 0 ; i < LineCount ; ++i )
		{
			fgets( strTemp, 512, fp ); // 한줄을 읽어오고...(한줄에 512바이트 제한)
			LineLen = strlen( strTemp ); // 개행문자의 위치를 얻어오고.
			strTemp[LineLen-1] = '\0'; // 종료문자로 바꿔준다.
			m_MultiText.AddString( strTemp ); // 통째로 넣는다. (현재는 색 같은건 지원하지 않는다.)
		}

		fclose( fp );
	}
}

void CMultiScrollText2::MakeSurface() // 멀티 텍스트의 내용을 서피스로 만들어주는 녀석.
{
	if (!g_pDisplay || !m_Font )	// m_Font 는 핸들이다
		return;

	int GreatestCount=0; // 가장큰 문자열의 개수를 얻는다
	int TextSurWidth = 0;
	SIZE size;

	HDC hdc;
	hdc = GetDC(pCMyApp->Get_hWnd());
	HFONT hFont = (HFONT) SelectObject( hdc, m_Font );

	// 가장큰 문자열의 개수를 얻는다
	for(int i = 0; i < m_MultiText.m_LineCount; i++)
	{
		// 입력받은 텍스트를 루프를 돌면서 가장긴 문자열 체크
		int len = strlen(m_MultiText.m_Text[i]);
		// 문자열의 폭이 고정되었는가?
		if (g_bFixedWidthFont)
		{
			if(GreatestCount < len)
				GreatestCount = len;
		}
		else
		{
			// 폰트 사이즈 반환
			GetTextExtentPoint32(hdc, m_MultiText.m_Text[i], len, &size);
			if (size.cx > TextSurWidth)
				TextSurWidth = size.cx;
		};
	}

	SAFE_DELETE(m_pTextSur);
	if (g_bFixedWidthFont)
	{
		// 가장긴 문자열과 라인수에 의해서 서피스 생성
		m_TextSurWidth = GreatestCount*m_FontWidth;
		m_TextSurHeight = m_MultiText.m_LineCount*m_FontHeight;
	}
	else
	{
		m_TextSurWidth = TextSurWidth;
		m_TextSurHeight = m_MultiText.m_LineCount*m_FontHeight;

	}

	// 계산된 크기로 서피스 생성
	g_pDisplay->CreateSurface( &m_pTextSur, m_TextSurWidth, m_TextSurHeight );
	if( !m_pTextSur )
		return;
	// fillcolor를 이용한 textsurface 클리어하기
	m_pTextSur->Clear();

	m_Xpos = 0;
	m_Ypos = 0;
	for(int i=0; i<m_MultiText.m_LineCount; i++)
	{
		m_pTextSur->DrawText(m_Font, m_MultiText.m_Text[i], m_Xpos, m_Ypos,
							 m_MultiText.m_TextBackColor[i], m_MultiText.m_TextColor[i], 0);
		m_Ypos+=m_FontHeight;
	}
	m_pTextSur->SetColorKey(TEXT_BACK_COLOR);

	SelectObject(hdc, hFont);
	ReleaseDC(pCMyApp->Get_hWnd(), hdc);

	if( m_pScroll ) // 스크롤이 있다면 스크롤의 범위를 갱신해준다.
		m_pScroll->SetRange( m_TextSurHeight + 1 - m_ViewHeightSize ); // +1 해줘야 맞더라.
}

void CMultiScrollText2::Draw()
{
	// 서피스 이미지의 부분을 출력.
	RECT TempRect;

	if( !m_pScroll || m_pScroll->RangeMax < 0 ) // 스크롤 없이 출력하는 경우.
	{
		g_pDisplay->Blt( m_ViewRect.left, m_ViewRect.top, m_pTextSur ); // 만땅 출력.
	}
	else // 스크롤이 있는 경우.
	{
		int StrDrawHeight = m_pScroll->GetPos()+m_ViewHeightSize; // 영역 출력은 범위가 원래 크기보다 크면 아예 출력을 안해버리더라. 그래서 보정이 필요하다.

		if( StrDrawHeight > m_TextSurHeight ) // m_TextSurHeight 이하로 줄여야한다.
			StrDrawHeight = m_TextSurHeight;

		TempRect.left = 0;
		TempRect.right = m_TextSurWidth; // 가로 세로는 풀로...
		TempRect.top = m_pScroll->GetPos(); // 스크롤을 픽셀단위 스크롤 했기때문에 바로 GetPos()를 써도 된다.
		TempRect.bottom = StrDrawHeight;

		g_pDisplay->Blt( m_ViewRect.left, m_ViewRect.top, m_pTextSur, &TempRect );
	}

	if( m_pScroll && m_pScroll->RangeMax > 0 ) // 스크롤 범위가 안 필요할때는 출력 안함.
		m_pScroll->Draw();
}

LRESULT CMultiScrollText2::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam ) // 스크롤용.
{
	if( !m_pScroll || m_pScroll->RangeMax <= 0 ) // 스크롤 범위가 안 필요할때는 처리 안함.
		return 0;

	if( m_pScroll->MsgProc(hWnd, msg, wParam, lParam) == S_OK )
		return 1;

	int x, y;
	x = LOWORD( lParam );
	y = HIWORD( lParam );

	switch (msg)
	{
	case 0x020A: // 휠마우스 메세지(리스트 영역안에서의 휠 메세지를 강제로 스크롤에 연동시켜준다.)

		if (x >= m_ViewRect.left
				&& x < m_ViewRect.right
				&& y >= m_ViewRect.top
				&& y < m_ViewRect.bottom )
		{
			if( (short)HIWORD(wParam) < 0 )
			{
				m_pScroll->NowPos += 10; // 픽셀단위니까 좀 넓직하게.

				if( m_pScroll->NowPos > m_pScroll->RangeMax-1 )
				{
					m_pScroll->NowPos = m_pScroll->RangeMax-1;
				}
			}
			else if( (short)HIWORD(wParam) > 0 )
			{
				m_pScroll->NowPos -= 10; // 픽셀단위니까 좀 넓직하게.

				if( m_pScroll->NowPos < 0 )
				{
					m_pScroll->NowPos = 0;
				}
			}
			return 1;
		}

		break;
	}

	return 0;
}
