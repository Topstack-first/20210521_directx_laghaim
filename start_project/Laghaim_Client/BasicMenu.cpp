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


///�޴��������̽�
CControlMenu::CControlMenu()
{
	ZeroMemory( &m_BackSur, sizeof(sMenuSurface) ); // �������̽� ���
	Init(); // ������ �ʱ�ȭ.
}

CControlMenu::~CControlMenu()
{
	DeleteRes();
}

void CControlMenu::ClearList() // ����Ʈ �ʱ�ȭ.
{
	// �̹��� ����Ʈ.
	for( list<sMenuSurface *>::iterator iter = m_SurfaceList.begin() ; iter != m_SurfaceList.end() ; ++iter )
	{
		delete (*iter);
	}

	m_SurfaceList.clear();

	// ��ư ����Ʈ.
	for( list<sMenuButton *>::iterator iter2 = m_BtnList.begin() ; iter2 != m_BtnList.end() ; ++iter2 )
	{
		delete (*iter2);
	}

	m_BtnList.clear();


	// ��Ʈ��
	for( list<sMenuString *>::iterator iter3 = m_StringList.begin() ; iter3 != m_StringList.end() ; ++iter3 )
	{
		delete (*iter3);
	}

	m_StringList.clear();

	// ��ũ�� ��Ʈ��

	for( vector<sMenuMultiScrollText *>::iterator iter4 = m_ScrollTextList.begin() ; iter4 != m_ScrollTextList.end() ; ++iter4 )
	{
		delete (*iter4);
	}

	m_ScrollTextList.clear();
}


void CControlMenu::Init()
{
	// ���� ������ �ʱ�ȭ.
	m_TextOut.Init( 12, RGB(255,255,0), 0, 0 ); // �̰� ���⼭ ���ִ� ������ �⺻ ��Ʈ ������ ���ؼ�.

	m_ScrollTextList.reserve( 5 );
}

void CControlMenu::DeleteRes()
{
	ClearList();
	m_TextOut.DeleteRes();

	SAFE_DELETE( m_BackSur.m_pSurface );
	ZeroMemory( &m_BackSur, sizeof(sMenuSurface) ); // �̰������ ���� LoadRes�� ������ �ȵȴ�.(������ �ƴ� �ٸ� �޴�â�� ��ﶧ.)
}

HRESULT CControlMenu::RestoreSurfaces()
{
	sMenuSurface TempMenuSurface;
	ZeroMemory( &TempMenuSurface, sizeof(sMenuSurface) ); // �̰������ ���� LoadRes�� ������ �ȵȴ�.(������ �ƴ� �ٸ� �޴�â�� ��ﶧ.)


	if( m_BackSur.m_pSurface && m_BackSur.FileName ) // �����ؾ��ϴ� ��� �̹����� �ִٸ�...
	{
		// ���������� ������ ���.(DeleteRes���� �ʱ�ȭ�ϱ⶧��)
		strcpy( TempMenuSurface.FileName, m_BackSur.FileName );
		TempMenuSurface.x = m_BackSur.x;
		TempMenuSurface.y = m_BackSur.y;
		TempMenuSurface.Width = m_BackSur.Width;
		TempMenuSurface.Height = m_BackSur.Height;
	}

	DeleteRes(); // �����ϰ� �� �����ٰ� �ε�.

	if( TempMenuSurface.FileName ) // �����ؾ��ϴ� ��� �̹����� �ִٸ�...
	{
		// �ε��ϱ� ���� ������ �����ش�.
		strcpy( m_BackSur.FileName, TempMenuSurface.FileName );
		m_BackSur.x = TempMenuSurface.x;
		m_BackSur.y = TempMenuSurface.y;
		m_BackSur.Width = TempMenuSurface.Width;
		m_BackSur.Height = TempMenuSurface.Height;
	}

	LoadRes(); // �̳༮�� �� ���ϸ���� ����ص״ٰ� �ٽ� �ε��ؾ� �ҵ�.

	return S_OK;
}

void CControlMenu::LoadRes()
{
	Init();

	// �� �༮�� �ػ󵵸� �Ű澵 �ʿ䰡 ������. ���ϸ��� Add�Ǿ��ִ� �༮���� �ε��Ѵ�.
	if( strlen(m_BackSur.FileName) ) // ���ϸ��� ��ϵǾ��� ����..
	{
		if( m_BackSur.m_pSurface ) // ������ �ִٸ�...
			SAFE_DELETE( m_BackSur.m_pSurface );

		g_pDisplay->CreateSurfaceFromBitmap( &m_BackSur.m_pSurface, m_BackSur.FileName );

		if( m_BackSur.m_pSurface )
		{
			m_BackSur.m_pSurface->Xpos = m_BackSur.x;
			m_BackSur.m_pSurface->Ypos = m_BackSur.y;

			m_BackSur.m_pSurface->SetWidth( m_BackSur.Width ); // �̰� �� �����Ǿ�� �Ѵ�.
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
			if( (*iter)->Width != -1 )  // -1�̸� ����� �� �ʿ䰡 ���� ���.
				TempSurface->SetWidth( (*iter)->Width );
			if( (*iter)->Height != -1 )  // -1�̸� ����� �� �ʿ䰡 ���� ���.
				TempSurface->SetHeight( (*iter)->Height );
		}
	}

	for( list<sMenuButton *>::iterator iter2 = m_BtnList.begin() ; iter2 != m_BtnList.end() ; ++iter2 )
	{
		(*iter2)->m_Button.LoadRes();
		(*iter2)->m_Button.SetPosition( (*iter2)->x, (*iter2)->y );
	}

	// ��ũ�� ��Ʈ��
	for( vector<sMenuMultiScrollText *>::iterator iter3 = m_ScrollTextList.begin() ; iter3 != m_ScrollTextList.end() ; ++iter3 )
	{
		(*iter3)->ScrollText.SetScrollImage( (*iter3)->DecBtnName, (*iter3)->IncBtnName, (*iter3)->BarName ); // �̰ɾ��ϸ� ��ũ�� ���� �༮���� ���������� �ϴ� �޴����� �׳� ��ũ�� ������ �ִ´�.(���߿� ������ �÷��׶� �ε���...)
		(*iter3)->ScrollText.LoadRes( (*iter3)->OutRect.left, (*iter3)->OutRect.top, (*iter3)->OutRect.right, (*iter3)->OutRect.bottom );  // ��ũ�� ���ҽ���.
		(*iter3)->ScrollText.MakeSurface(); // �ؽ�Ʈ ���ҽ���. (�̰͵� LoadRes() �ڿ� ���ִ°� ����)
		(*iter3)->ScrollText.SetScrollPos( (*iter3)->ScrollPosX, (*iter3)->ScrollPosY, (*iter3)->ScrollHeight ); // �̰� LoadRes() �ڿ� ������Ѵ�.
	}
}

void CControlMenu::AddBackGround( const char *FileName, int PosX, int PosY, int Width, int Height )
{
	if( !FileName )
		return;

	m_hWnd =  g_pDisplay->GetHWnd();  // �̳༮�� �ػ󵵿� ������ �������ִ�. x,y�� -1�� ���ö�...
	GetClientRect( m_hWnd, &m_ClientRc );

	if( PosX == -1 && PosY == -1 ) // ��ǥ �Է��� �ȵǾ��� ��� Width,Height�� �������� ȭ���� �߾ӿ� ��ġ�ϰ� �Ѵ�.
	{
		PosX = ( m_ClientRc.right - Width ) / 2;
		PosY = ( m_ClientRc.bottom - Height ) / 2;
	}

	strcpy( m_BackSur.FileName, FileName ); // �̸��� ���.
	m_BackSur.x = PosX;
	m_BackSur.y = PosY;
	m_BackSur.Width = Width;
	m_BackSur.Height = Height;

	m_BackSur.m_pSurface = NULL; // �ʱ�ȭ.
}

void CControlMenu::AddSurface( const char *FileName, int PosX, int PosY, int Width, int Height ) // �̳༮�� �ʿ信 ���� ���� ���θ� �Է��صξ���Ѵ�.
{
	if( !FileName )
		return;

	sMenuSurface *TempMenuSurface = NULL;
	TempMenuSurface = new sMenuSurface; // �޸𸮸� ���� ����ش�.

	if( !TempMenuSurface )
		return;

	PosX += m_BackSur.x; // ��ǥ�� ����� �������� �ϴ� ��ǥ�谡 �Ǿ��Ѵ�.
	PosY += m_BackSur.y;

	strcpy( TempMenuSurface->FileName, FileName ); // �̸��� ���.
	TempMenuSurface->x = PosX;
	TempMenuSurface->y = PosY;
	TempMenuSurface->Width = Width;
	TempMenuSurface->Height = Height;

	TempMenuSurface->m_pSurface = NULL; // �ʱ�ȭ.

	m_SurfaceList.push_back( TempMenuSurface );
}

void CControlMenu::AddButton( const char *FileName, int PosX, int PosY, int ActionIndex )
{
	if( !FileName )
		return;

	sMenuButton *TempButton = NULL;
	TempButton = new sMenuButton; // �޸𸮸� ���� ����ش�.

	if( !TempButton )
		return;

	TempButton->ActionIndex = ActionIndex;

	PosX += m_BackSur.x; // ��ǥ�� ����� �������� �ϴ� ��ǥ�谡 �Ǿ��Ѵ�.
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

	if (!g_pDisplay || !m_TextOut.m_Font )	// m_Font �� �ڵ��̴�
		return;

	sMenuString *TempString = NULL;
	TempString = new sMenuString; // �޸𸮸� ���� ����ش�.

	if( !TempString )
		return;

	if( bCenter ) // ��ǥ�� ���� �߰��� �ǹ��ϴ� ����̸�..
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

		PosX -= size.cx/2; // ������ ������ ������ ����.

		SelectObject(hdc, hFont);
		ReleaseDC(pCMyApp->Get_hWnd(), hdc);
	}

	PosX += m_BackSur.x; // ��ǥ�� ����� �������� �ϴ� ��ǥ�谡 �Ǿ��Ѵ�.
	PosY += m_BackSur.y;

	TempString->x = PosX;
	TempString->y = PosY;

	strcpy( TempString->String, String );

	m_StringList.push_back( TempString );
}

CMultiScrollText* CControlMenu::AddScrollText( int PosX, int PosY, int Width, int Height, int ScrollPosX, int ScrollPosY, int ScrollHeight , const char *DecBtn, const char *IncBtn, const char *Bar)
{
	sMenuMultiScrollText *TempScrollText = NULL;
	TempScrollText = new sMenuMultiScrollText; // �޸𸮸� ���� ����ش�.

	if( !TempScrollText )
		return NULL;

	PosX += m_BackSur.x; // ��ǥ�� ����� �������� �ϴ� ��ǥ�谡 �Ǿ��Ѵ�.
	PosY += m_BackSur.y;

	TempScrollText->OutRect.left = PosX;
	TempScrollText->OutRect.top = PosY;
	TempScrollText->OutRect.right = PosX+Width;
	TempScrollText->OutRect.bottom = PosY+Height;

	ScrollPosX += m_BackSur.x; // ��ǥ�� ����� �������� �ϴ� ��ǥ�谡 �Ǿ��Ѵ�.
	ScrollPosY += m_BackSur.y;

	TempScrollText->ScrollPosX = ScrollPosX;
	TempScrollText->ScrollPosY = ScrollPosY;
	TempScrollText->ScrollHeight = ScrollHeight;

	if( DecBtn )
		strcpy( TempScrollText->DecBtnName, DecBtn );
	else // ���̾��ٸ�..
		TempScrollText->DecBtnName[0] = '\0';

	if( IncBtn )
		strcpy( TempScrollText->IncBtnName, IncBtn );
	else // ���̾��ٸ�..
		TempScrollText->IncBtnName[0] = '\0';

	if( Bar )
		strcpy( TempScrollText->BarName, Bar );
	else // ���̾��ٸ�..
		TempScrollText->BarName[0] = '\0';

	m_ScrollTextList.push_back( TempScrollText );

	return &(TempScrollText->ScrollText); // �����͸� ���Ͻ����༭ ���Ŀ��� �����Ҽ��ֵ��� ���ش�.
}

void CControlMenu::Draw()
{
	if( m_BackSur.m_pSurface )
		g_pDisplay->Blt( m_BackSur.m_pSurface->Xpos, m_BackSur.m_pSurface->Ypos, m_BackSur.m_pSurface ); // ���.

	for( list<sMenuSurface *>::iterator iter = m_SurfaceList.begin() ; iter != m_SurfaceList.end() ; ++iter )
	{
		g_pDisplay->Blt( (*iter)->m_pSurface->Xpos, (*iter)->m_pSurface->Ypos, (*iter)->m_pSurface );
	}

	// ��ư ����Ʈ.
	for( list<sMenuButton *>::iterator iter2 = m_BtnList.begin() ; iter2 != m_BtnList.end() ; ++iter2 )
	{
		(*iter2)->m_Button.Draw();
	}

	// ��Ʈ��
	for( list<sMenuString *>::iterator iter3 = m_StringList.begin() ; iter3 != m_StringList.end() ; ++iter3 )
	{
		m_TextOut.SetPos( (*iter3)->x, (*iter3)->y );
		m_TextOut.SetString( (*iter3)->String, FALSE );
		m_TextOut.Draw();
	}

	// ��ũ�� ��Ʈ��
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

	// ��ũ�� �ؽ�Ʈ�� ��ũ�� ������ �޽��� ���ν����� ��������Ѵ�.
	for( vector<sMenuMultiScrollText *>::iterator iter2 = m_ScrollTextList.begin() ; iter2 != m_ScrollTextList.end() ; ++iter2 )
	{
		if( (*iter2)->ScrollText.MsgProc(hWnd, msg, wParam, lParam) )
			return 1;
	}


	switch (msg)
	{
	case 0x020A: // �ٸ��콺 �޼���(����Ʈ �����ȿ����� �� �޼����� ������ ��ũ�ѿ� ���������ش�.)
		break; // �̰� �����ָ� ��ũ�� �ؽ�Ʈ�� �ȸԴ´�.(������ �𸣰ڽ�.)
	case WM_LBUTTONDOWN:

		if( IsInside(x,y) ) // �������̽��� ������ Ŭ������ ���.
		{
			for( iter = m_BtnList.begin() ; iter != m_BtnList.end() ; ++iter )
			{
				(*iter)->m_Button.MsgProc(hWnd, msg, wParam, lParam); // �� ��ư�� ���鼭 ���ֳ�.
			}

			return 1;
		}
		else // �������̽� �̿��� �κ��� Ŭ����.(�ݾƹ�����.)
		{
			g_pNk2DFrame->ShowControlMenuWindow( FALSE ); // ���� �����Ų��.
		}

		return 0; // �ٸ� �������̽� �κ��� ����� ���...(�κ�,Bottom��...)

	case WM_MOUSEMOVE:
	case WM_LBUTTONUP:

		if( IsInside(x,y) ) // �������̽��� ������ Ŭ������ ���.
		{
			for( iter = m_BtnList.begin() ; iter != m_BtnList.end() ; ++iter )
			{
				(*iter)->m_Button.MsgProc(hWnd, msg, wParam, lParam); // �� ��ư�� ���鼭 ���ֳ�.

				if( (*iter)->m_Button.GetState()==BTN_ACTION ) // ���� ��ư Ŭ����
				{
					(*iter)->m_Button.SetState(BTN_NORMAL);	// ��ư�� �������ͽ��� ���� Ǯ������ϳ�����.
					BtnAction( (*iter)->ActionIndex ); // �۵� ó��. // ���� ƨ��� ����.

					return 1; // BtnAction ���� �޸𸮸� �ǵ帮�� �ݺ��ڰ� ��ȿ�� �ǳ� ����. �׷��� ���� �ݺ��ڿ��� ƨ�����. ������ ��ư�� �ѹ��� �ϳ��� �۵��ϸ� �Ǵ� ���Ͻ�Ű��.
				}
			}

			return 1;
		}

		break;
	}
	return 0;
}

BOOL CControlMenu::IsInside( int x, int y ) // ��� ���� üũ.
{
	if( !m_BackSur.m_pSurface )
		return FALSE;

	return m_BackSur.m_pSurface->IsIn( x,y );
}

void CControlMenu::BtnAction( int ActionIndex ) // ��ư�� �������� ������ �ڵ�.
{
	char m_commOut[512];

	if( ActionIndex > 10000 ) // 10000�� �� �ε����� ������ ���� ���� NPC �ε����� �޾Ƽ� ó���ϴ� ����. �ش� �ε��� -10000 ���� shop ���� ��Ŷ�� ������.
	{
		g_pNk2DFrame->ShowControlMenuWindow( FALSE ); // �� �༮�� ����â ������ �̸� �ݾƾ��Ѵ�.

		g_pNk2DFrame->ToggleShopWindow();
		if (g_pNk2DFrame->IsShopVisible())
		{			
			g_pNk2DFrame->GetControlShop()->SetKeeper( ActionIndex-10000, locationX, locationZ );

			sprintf(m_commOut, "shop %d %d %d 1\n", ActionIndex-10000, (int)locationX, (int)locationZ ); // ��ǥ�� �ϵ� �ڵ�
			if( g_pTcpIp )
				g_pTcpIp->SendNetMessage(m_commOut);
		}

		return;
	}

	switch( ActionIndex )
	{
	case MENU_ACTION_USERSHOP_SEARCH: // ���λ��� ã��
		g_pNk2DFrame->ToggleUserShopSearchWindow( FALSE ); ///���λ����˻�.
		break;

	case MENU_ACTION_USERSHOP_RECEIVE: // ���λ��� ȸ��
		sprintf(m_commOut, "u_shop req\n" );
		if( g_pTcpIp )
			g_pTcpIp->SendNetMessage(m_commOut);

		g_pNk2DFrame->ShowControlMenuWindow( FALSE ); // ��Ŷ������ �ݾ��ش�.

		break;

	case MENU_ACTION_NPCUPGRADE_UPGRADE: // ��ȭ
		g_pNk2DFrame->ToggleUpgradeWindow( INTERFACE_TYPE_UPARMOR ); // ��ȭ �������̽� ���
		break;

	case MENU_ACTION_NPCUPGRADE_CONVERSION: // ������
		g_pNk2DFrame->ToggleUpgradeWindow( INTERFACE_TYPE_CONVERSION ); // ������ �������̽� ���
		break;

	case MENU_ACTION_NPCUPGRADE_CONFUSION: // ��ǻ��
		g_pNk2DFrame->ToggleUpgradeWindow( INTERFACE_TYPE_CONFUSION ); // ��ǻ�� �������̽� ���
		break;

	case MENU_ACTION_NPCUPGRADE_REFORM: // ����
		g_pNk2DFrame->ToggleUpgradeWindow( INTERFACE_TYPE_PROCESS ); // ���� �������̽� ���
		break;

	case MENU_ACTION_NPCUPGRADE_ARMOR400:
		g_pNk2DFrame->ToggleUpgradeWindow( INTERFACE_TYPE_UPARMOR_400 ); // ���� �������̽� ���
		break;

	case MENU_ACTION_CLOSE: // �ݱ�
		g_pNk2DFrame->ShowControlMenuWindow( FALSE );
		break;

	case MENU_ACTION_ENCHANT_UPGRADE:	// ��ȭ ��ư
		if( g_SvrType == ST_ADULT_ONLY ) // [2/27/2007 Theodoric] ��þƮ : ����â ��ư Ŭ���� �ൿ��
			g_pNk2DFrame->TogglePlatinumExchWindow(0);
		break;

	case MENU_ACTION_ENCHANT_MAKE:	// �÷�Ƽ�� ����
		if( g_SvrType == ST_ADULT_ONLY ) // [2/27/2007 Theodoric] ��þƮ : ����â ��ư Ŭ���� �ൿ��
			g_pNk2DFrame->TogglePlatinumExchWindow(1);
		break;

	case MENU_ACTION_ENCHANT_INIT:	// �ɼ� �ʱ�ȭ
		if( g_SvrType == ST_ADULT_ONLY ) // [2/27/2007 Theodoric] ��þƮ : ����â ��ư Ŭ���� �ൿ��
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
		// ���� ����Ʈ�� �ƹ��͵� ���Ѵ�.
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
	m_pScroll = NULL; // �ʱ�ȭ�� ���ؼ�.

	// ���������� �ʱ�ȭ
	m_Font		= NULL;

	ZeroMemory( m_DecBtnName, sizeof(char)*MENU_FILE_NAME_LEN ); // 0���� �ʱ�ȭ�� �س��ش�.(�׷��� ����Ʈ�� �ν��� �ɵ�.)
	ZeroMemory( m_IncBtnName, sizeof(char)*MENU_FILE_NAME_LEN ); // 0���� �ʱ�ȭ�� �س��ش�.(�׷��� ����Ʈ�� �ν��� �ɵ�.)
	ZeroMemory( m_BarName, sizeof(char)*MENU_FILE_NAME_LEN ); // 0���� �ʱ�ȭ�� �س��ش�.(�׷��� ����Ʈ�� �ν��� �ɵ�.)

	Init(); // ���⼭�� �ѹ� ȣ���� ������� ��.
}

CMultiScrollText::~CMultiScrollText()
{
	DeleteRes();
}

void CMultiScrollText::Init( int FontSize )
{
	// ���������� �ʱ�ȭ
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

	SetViewRect( 0, 0, 0, 0 ); // �䷺Ʈ�� �ʱ�ȭ.

	m_ViewHeightSize = 0;
}

void CMultiScrollText::DeleteRes()
{
	SAFE_DELETE( m_pScroll ); // ��ũ�� ���ҽ��� ������ �ȵǴ� ������ �ִ�.
	SAFE_DELETE( m_pTextSur );
}

void CMultiScrollText::LoadRes( int l, int t, int r,int b )
{
	Init();

	SetViewRect( l, t, r, b );

	if( m_pScroll ) // ���� ���� �ִ�.
		m_pScroll->LoadRes( m_DecBtnName, m_IncBtnName, m_BarName ); //��ũ�� ���ҽ��� �ε�.
}

void CMultiScrollText::SetViewRect( int l, int t, int r,int b ) //�ε� ���ҽ� �Ŀ� ����Ǿ��Ѵ�.
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

	m_pScroll = new CScroll(); // �޸𸮸� ���, �̸��� �����صδ¿�Ȱ.

	if( DecBtn[0] )
		strcpy( m_DecBtnName, DecBtn );
	else // ���̾��ٸ�..
		m_DecBtnName[0] = '\0';

	if( IncBtn[0] )
		strcpy( m_IncBtnName, IncBtn );
	else // ���̾��ٸ�..
		m_IncBtnName[0] = '\0';

	if( ScrollBar[0] )
		strcpy( m_BarName, ScrollBar );
	else // ���̾��ٸ�..
		m_BarName[0] = '\0';
}

void CMultiScrollText::SetScrollPos( int x, int y, int size ) // ��ũ�� ��ġ�� ũ�⸦ �����ϰ� ������ ���.
{
	if( !m_pScroll )
		return;

	if( x == -1 ) // ����Ʈ ��ġ.(�ڽ� ������ ��)
	{
		x =  m_ViewRect.right;
	}

	if( y == -1 ) // ����Ʈ ��ġ.(�ڽ� ������ ��)
	{
		y =  m_ViewRect.top;
	}

	if( size == -1 ) // ����Ʈ ��ġ.(�ڽ� ������ ��)
	{
		size =  m_ViewHeightSize;
	}

	m_pScroll->SetDrawPos( x, y, size ); // ��ũ���� ����.
}

void CMultiScrollText::AddString( char *String, int LineLen, DWORD color, DWORD back_color ) // ��Ʈ�� ���� �߰�. �׳� ��Ƽ�ؽ�Ʈ�� �ֱ� ���� ���ֱ� ��.
{
	if( LineLen == -1 )
		m_MultiText.AddString( String, color, back_color );
	else
		m_MultiText.AddStringMulti( String, LineLen, FALSE, color, back_color );
}

void CMultiScrollText::LoadString( char *FileName ) // �ؽ�Ʈ ���Ϸ� ���� ��Ʈ���� �о���� �Լ�.
{
	// by evilkiki 2009.09.03 �ؽ�Ʈ ���� �б� ���� ���� �ӽ� ó��
// 	PFILE *fp = NULL;
// 	char strTemp[512] = {0,};
// 	int LineCount = 0;
// 	int LineLen = 0;
//
// 	fp = pfopen( FileName, "rt");
//
// 	if( fp )
// 	{
// 		pfscanf( fp, "%d", &LineCount ); // �� �ټ�.
//
// 		for( int i = 0 ; i < LineCount ; ++i )
// 		{
// 			pfgets( strTemp, 512, fp ); // ������ �о����...(���ٿ� 512����Ʈ ����)
// 			LineLen = strlen( strTemp ); // ���๮���� ��ġ�� ������.
//
// 			strTemp[LineLen-1] = '\0'; // ���Ṯ�ڷ� �ٲ��ش�.
//
// 			m_MultiText.AddString( strTemp ); // ��°�� �ִ´�. (����� �� ������ �������� �ʴ´�.)
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
		fscanf( fp, "%d", &LineCount ); // �� �ټ�.

		for( int i = 0 ; i < LineCount ; ++i )
		{
			fgets( strTemp, 512, fp ); // ������ �о����...(���ٿ� 512����Ʈ ����)
			LineLen = strlen( strTemp ); // ���๮���� ��ġ�� ������.

			strTemp[LineLen-1] = '\0'; // ���Ṯ�ڷ� �ٲ��ش�.

			m_MultiText.AddString( strTemp ); // ��°�� �ִ´�. (����� �� ������ �������� �ʴ´�.)
		}

		fclose( fp );
	}

}

void CMultiScrollText::MakeSurface() // ��Ƽ �ؽ�Ʈ�� ������ ���ǽ��� ������ִ� �༮.
{
	if (!g_pDisplay || !m_Font )	// m_Font �� �ڵ��̴�
		return;

	int GreatestCount=0; // ����ū ���ڿ��� ������ ��´�
	int TextSurWidth = 0;
	SIZE size;

	HDC hdc;
	hdc = GetDC(pCMyApp->Get_hWnd());
	HFONT hFont = (HFONT) SelectObject( hdc, m_Font );

	// ����ū ���ڿ��� ������ ��´�
	int i = 0;
	for(i = 0; i < m_MultiText.m_LineCount; i++)
	{
		// �Է¹��� �ؽ�Ʈ�� ������ ���鼭 ����� ���ڿ� üũ
		int len = strlen(m_MultiText.m_Text[i]);
		// ���ڿ��� ���� �����Ǿ��°�?
		if (g_bFixedWidthFont)
		{
			if(GreatestCount < len)
				GreatestCount = len;
		}
		else
		{
			// ��Ʈ ������ ��ȯ
			GetTextExtentPoint32(hdc, m_MultiText.m_Text[i], len, &size);
			if (size.cx > TextSurWidth)
				TextSurWidth = size.cx;
		};
	}

	SAFE_DELETE(m_pTextSur);
	if (g_bFixedWidthFont)
	{
		// ����� ���ڿ��� ���μ��� ���ؼ� ���ǽ� ����
		m_TextSurWidth = GreatestCount*m_FontWidth;
		m_TextSurHeight = m_MultiText.m_LineCount*m_FontHeight;
	}
	else
	{
		m_TextSurWidth = TextSurWidth;
		m_TextSurHeight = m_MultiText.m_LineCount*m_FontHeight;

	}

	// ���� ũ��� ���ǽ� ����
	g_pDisplay->CreateSurface( &m_pTextSur, m_TextSurWidth, m_TextSurHeight );
	if( !m_pTextSur )
		return;
	// fillcolor�� �̿��� textsurface Ŭ�����ϱ�
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

	if( m_pScroll ) // ��ũ���� �ִٸ� ��ũ���� ������ �������ش�.
		m_pScroll->SetRange( m_TextSurHeight + 1 - m_ViewHeightSize ); // +1 ����� �´���.
}

void CMultiScrollText::Draw()
{
	// ���ǽ� �̹����� �κ��� ���.
	RECT TempRect;

	if( !m_pScroll || m_pScroll->RangeMax < 0 ) // ��ũ�� ���� ����ϴ� ���.
	{
		g_pDisplay->Blt( m_ViewRect.left, m_ViewRect.top, m_pTextSur ); // ���� ���.
	}
	else // ��ũ���� �ִ� ���.
	{
		int StrDrawHeight = m_pScroll->GetPos()+m_ViewHeightSize; // ���� ����� ������ ���� ũ�⺸�� ũ�� �ƿ� ����� ���ع�������. �׷��� ������ �ʿ��ϴ�.

		if( StrDrawHeight > m_TextSurHeight ) // m_TextSurHeight ���Ϸ� �ٿ����Ѵ�.
			StrDrawHeight = m_TextSurHeight;

		TempRect.left = 0;
		TempRect.right = m_TextSurWidth; // ���� ���δ� Ǯ��...
		TempRect.top = m_pScroll->GetPos(); // ��ũ���� �ȼ����� ��ũ�� �߱⶧���� �ٷ� GetPos()�� �ᵵ �ȴ�.
		TempRect.bottom = StrDrawHeight;

		g_pDisplay->Blt( m_ViewRect.left, m_ViewRect.top, m_pTextSur, &TempRect );
	}

	if( m_pScroll && m_pScroll->RangeMax > 0 ) // ��ũ�� ������ �� �ʿ��Ҷ��� ��� ����.
		m_pScroll->Draw();
}

LRESULT CMultiScrollText::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam ) // ��ũ�ѿ�.
{
	if( !m_pScroll || m_pScroll->RangeMax <= 0 ) // ��ũ�� ������ �� �ʿ��Ҷ��� ó�� ����.
		return 0;

	if( m_pScroll->MsgProc(hWnd, msg, wParam, lParam) == S_OK )
		return 1;

	int x, y;
	x = LOWORD( lParam );
	y = HIWORD( lParam );

	switch (msg)
	{
	case 0x020A: // �ٸ��콺 �޼���(����Ʈ �����ȿ����� �� �޼����� ������ ��ũ�ѿ� ���������ش�.)

		if (x >= m_ViewRect.left
				&& x < m_ViewRect.right
				&& y >= m_ViewRect.top
				&& y < m_ViewRect.bottom )
		{
			if( (short)HIWORD(wParam) < 0 )
			{
				m_pScroll->NowPos += 10; // �ȼ������ϱ� �� �����ϰ�.

				if( m_pScroll->NowPos > m_pScroll->RangeMax-1 )
				{
					m_pScroll->NowPos = m_pScroll->RangeMax-1;
				}
			}
			else if( (short)HIWORD(wParam) > 0 )
			{
				m_pScroll->NowPos -= 10; // �ȼ������ϱ� �� �����ϰ�.

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
	m_pScroll = NULL; // �ʱ�ȭ�� ���ؼ�.

	// ���������� �ʱ�ȭ
	m_Font		= NULL;

	ZeroMemory( m_DecBtnName, sizeof(char)*MENU_FILE_NAME_LEN ); // 0���� �ʱ�ȭ�� �س��ش�.(�׷��� ����Ʈ�� �ν��� �ɵ�.)
	ZeroMemory( m_IncBtnName, sizeof(char)*MENU_FILE_NAME_LEN ); // 0���� �ʱ�ȭ�� �س��ش�.(�׷��� ����Ʈ�� �ν��� �ɵ�.)
	ZeroMemory( m_BarName, sizeof(char)*MENU_FILE_NAME_LEN ); // 0���� �ʱ�ȭ�� �س��ش�.(�׷��� ����Ʈ�� �ν��� �ɵ�.)

	Init(); // ���⼭�� �ѹ� ȣ���� ������� ��.
}

CMultiScrollText2::~CMultiScrollText2()
{
	DeleteRes();
}

void CMultiScrollText2::Init( int FontSize )
{
	// ���������� �ʱ�ȭ
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

	SetViewRect( 0, 0, 0, 0 ); // �䷺Ʈ�� �ʱ�ȭ.

	m_ViewHeightSize = 0;
}

void CMultiScrollText2::DeleteRes()
{
	SAFE_DELETE( m_pScroll ); // ��ũ�� ���ҽ��� ������ �ȵǴ� ������ �ִ�.
	SAFE_DELETE( m_pTextSur );
}

void CMultiScrollText2::LoadRes( int l, int t, int r,int b )
{
	Init();

	SetViewRect( l, t, r, b );

	if( m_pScroll ) // ���� ���� �ִ�.
		m_pScroll->LoadRes( m_DecBtnName, m_IncBtnName, m_BarName ); //��ũ�� ���ҽ��� �ε�.
}

void CMultiScrollText2::SetViewRect( int l, int t, int r,int b ) //�ε� ���ҽ� �Ŀ� ����Ǿ��Ѵ�.
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

	m_pScroll = new CScroll(); // �޸𸮸� ���, �̸��� �����صδ¿�Ȱ.

	if( DecBtn[0] )
		strcpy( m_DecBtnName, DecBtn );
	else // ���̾��ٸ�..
		m_DecBtnName[0] = '\0';

	if( IncBtn[0] )
		strcpy( m_IncBtnName, IncBtn );
	else // ���̾��ٸ�..
		m_IncBtnName[0] = '\0';

	if( ScrollBar[0] )
		strcpy( m_BarName, ScrollBar );
	else // ���̾��ٸ�..
		m_BarName[0] = '\0';
}

void CMultiScrollText2::SetScrollPos( int x, int y, int size ) // ��ũ�� ��ġ�� ũ�⸦ �����ϰ� ������ ���.
{
	if( !m_pScroll )
		return;

	if( x == -1 ) // ����Ʈ ��ġ.(�ڽ� ������ ��)
	{
		x =  m_ViewRect.right;
	}

	if( y == -1 ) // ����Ʈ ��ġ.(�ڽ� ������ ��)
	{
		y =  m_ViewRect.top;
	}

	if( size == -1 ) // ����Ʈ ��ġ.(�ڽ� ������ ��)
	{
		size =  m_ViewHeightSize;
	}

	m_pScroll->SetDrawPos( x, y, size ); // ��ũ���� ����.
}

void CMultiScrollText2::AddString( char *String, int LineLen, DWORD color, DWORD back_color ) // ��Ʈ�� ���� �߰�. �׳� ��Ƽ�ؽ�Ʈ�� �ֱ� ���� ���ֱ� ��.
{
	if( LineLen == -1 )
		m_MultiText.AddString( String, color, back_color );
	else
		m_MultiText.AddStringMulti( String, LineLen, FALSE, color, back_color );
}

void CMultiScrollText2::LoadString( char *FileName ) // �ؽ�Ʈ ���Ϸ� ���� ��Ʈ���� �о���� �Լ�.
{
	FILE *fp = NULL;
	char strTemp[512];
	int LineCount = 0;
	int LineLen = 0;

	fp = fopen( FileName, "rt");

	if( fp )
	{
		fscanf( fp, "%d", &LineCount ); // �� �ټ�.

		for( int i = 0 ; i < LineCount ; ++i )
		{
			fgets( strTemp, 512, fp ); // ������ �о����...(���ٿ� 512����Ʈ ����)
			LineLen = strlen( strTemp ); // ���๮���� ��ġ�� ������.
			strTemp[LineLen-1] = '\0'; // ���Ṯ�ڷ� �ٲ��ش�.
			m_MultiText.AddString( strTemp ); // ��°�� �ִ´�. (����� �� ������ �������� �ʴ´�.)
		}

		fclose( fp );
	}
}

void CMultiScrollText2::MakeSurface() // ��Ƽ �ؽ�Ʈ�� ������ ���ǽ��� ������ִ� �༮.
{
	if (!g_pDisplay || !m_Font )	// m_Font �� �ڵ��̴�
		return;

	int GreatestCount=0; // ����ū ���ڿ��� ������ ��´�
	int TextSurWidth = 0;
	SIZE size;

	HDC hdc;
	hdc = GetDC(pCMyApp->Get_hWnd());
	HFONT hFont = (HFONT) SelectObject( hdc, m_Font );

	// ����ū ���ڿ��� ������ ��´�
	for(int i = 0; i < m_MultiText.m_LineCount; i++)
	{
		// �Է¹��� �ؽ�Ʈ�� ������ ���鼭 ����� ���ڿ� üũ
		int len = strlen(m_MultiText.m_Text[i]);
		// ���ڿ��� ���� �����Ǿ��°�?
		if (g_bFixedWidthFont)
		{
			if(GreatestCount < len)
				GreatestCount = len;
		}
		else
		{
			// ��Ʈ ������ ��ȯ
			GetTextExtentPoint32(hdc, m_MultiText.m_Text[i], len, &size);
			if (size.cx > TextSurWidth)
				TextSurWidth = size.cx;
		};
	}

	SAFE_DELETE(m_pTextSur);
	if (g_bFixedWidthFont)
	{
		// ����� ���ڿ��� ���μ��� ���ؼ� ���ǽ� ����
		m_TextSurWidth = GreatestCount*m_FontWidth;
		m_TextSurHeight = m_MultiText.m_LineCount*m_FontHeight;
	}
	else
	{
		m_TextSurWidth = TextSurWidth;
		m_TextSurHeight = m_MultiText.m_LineCount*m_FontHeight;

	}

	// ���� ũ��� ���ǽ� ����
	g_pDisplay->CreateSurface( &m_pTextSur, m_TextSurWidth, m_TextSurHeight );
	if( !m_pTextSur )
		return;
	// fillcolor�� �̿��� textsurface Ŭ�����ϱ�
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

	if( m_pScroll ) // ��ũ���� �ִٸ� ��ũ���� ������ �������ش�.
		m_pScroll->SetRange( m_TextSurHeight + 1 - m_ViewHeightSize ); // +1 ����� �´���.
}

void CMultiScrollText2::Draw()
{
	// ���ǽ� �̹����� �κ��� ���.
	RECT TempRect;

	if( !m_pScroll || m_pScroll->RangeMax < 0 ) // ��ũ�� ���� ����ϴ� ���.
	{
		g_pDisplay->Blt( m_ViewRect.left, m_ViewRect.top, m_pTextSur ); // ���� ���.
	}
	else // ��ũ���� �ִ� ���.
	{
		int StrDrawHeight = m_pScroll->GetPos()+m_ViewHeightSize; // ���� ����� ������ ���� ũ�⺸�� ũ�� �ƿ� ����� ���ع�������. �׷��� ������ �ʿ��ϴ�.

		if( StrDrawHeight > m_TextSurHeight ) // m_TextSurHeight ���Ϸ� �ٿ����Ѵ�.
			StrDrawHeight = m_TextSurHeight;

		TempRect.left = 0;
		TempRect.right = m_TextSurWidth; // ���� ���δ� Ǯ��...
		TempRect.top = m_pScroll->GetPos(); // ��ũ���� �ȼ����� ��ũ�� �߱⶧���� �ٷ� GetPos()�� �ᵵ �ȴ�.
		TempRect.bottom = StrDrawHeight;

		g_pDisplay->Blt( m_ViewRect.left, m_ViewRect.top, m_pTextSur, &TempRect );
	}

	if( m_pScroll && m_pScroll->RangeMax > 0 ) // ��ũ�� ������ �� �ʿ��Ҷ��� ��� ����.
		m_pScroll->Draw();
}

LRESULT CMultiScrollText2::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam ) // ��ũ�ѿ�.
{
	if( !m_pScroll || m_pScroll->RangeMax <= 0 ) // ��ũ�� ������ �� �ʿ��Ҷ��� ó�� ����.
		return 0;

	if( m_pScroll->MsgProc(hWnd, msg, wParam, lParam) == S_OK )
		return 1;

	int x, y;
	x = LOWORD( lParam );
	y = HIWORD( lParam );

	switch (msg)
	{
	case 0x020A: // �ٸ��콺 �޼���(����Ʈ �����ȿ����� �� �޼����� ������ ��ũ�ѿ� ���������ش�.)

		if (x >= m_ViewRect.left
				&& x < m_ViewRect.right
				&& y >= m_ViewRect.top
				&& y < m_ViewRect.bottom )
		{
			if( (short)HIWORD(wParam) < 0 )
			{
				m_pScroll->NowPos += 10; // �ȼ������ϱ� �� �����ϰ�.

				if( m_pScroll->NowPos > m_pScroll->RangeMax-1 )
				{
					m_pScroll->NowPos = m_pScroll->RangeMax-1;
				}
			}
			else if( (short)HIWORD(wParam) > 0 )
			{
				m_pScroll->NowPos -= 10; // �ȼ������ϱ� �� �����ϰ�.

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
