#include "stdafx.h"
#include "StartPage.h"
#include "headers.h"
#include "Sheet.h"

#define GOLOGIN 1
#define GOMAKEACNT 2
#define GOSETOPT 3
#define GOTUTOR 4
#define GOEXIT 5

#define BACKGROUND_STARTPAGE		"interface/back_startpage.bmp"

extern BOOL g_bDungeon;

//-----------------------------------------------------------------------------
// CStartPage

CStartPage::CStartPage()
{
	m_pSheet = NULL;
	m_pBack = NULL;
	Init();
}

CStartPage::CStartPage( CSheet *pSheet )
{
	m_pSheet = NULL;
	m_pBack = NULL;
	SetSheetPtr(pSheet);
	Init();
}

CStartPage::~CStartPage()
{
	DeleteRes();
}

void CStartPage::SetSheetPtr( CSheet *pSheet )
{
	m_pSheet = pSheet;
}

void CStartPage::Init()
{
	m_LoginBtn.SetFileName( "connect" );
	m_LoginBtn.SetSize( START_BTN_WIDTH, START_BTN_HEIGHT );
	m_LoginBtn.SetDisable( false );

	m_MakeAcntBtn.SetFileName( "makeid" );
	m_MakeAcntBtn.SetSize( START_BTN_WIDTH, START_BTN_HEIGHT );
	m_MakeAcntBtn.SetDisable( false );

	m_SetOptBtn.SetFileName( "setting" );
	m_SetOptBtn.SetSize( START_BTN_WIDTH, START_BTN_HEIGHT );
	m_SetOptBtn.SetDisable( false );

	m_ExitBtn.SetFileName( "exit" );
	m_ExitBtn.SetSize( START_BTN_WIDTH, START_BTN_HEIGHT );
	m_ExitBtn.SetDisable( false );

	Read_URL_Add("explorer.txt");
}

void CStartPage::LoadRes()
{
	if( !g_pDisplay )
		return;

	g_pDisplay->CreateSurfaceFromBitmap( &m_pBack, BACKGROUND_STARTPAGE);
	m_pBack->SetColorKey(RGB(0, 0, 0));

	m_LoginBtn.LoadRes(  );
	m_MakeAcntBtn.LoadRes(  );
	m_SetOptBtn.LoadRes(  );
	m_ExitBtn.LoadRes(  );
}

void CStartPage::DeleteRes()
{
	SAFE_DELETE(m_pBack);
	m_LoginBtn.DeleteRes();
	m_MakeAcntBtn.DeleteRes();
	m_SetOptBtn.DeleteRes();
	m_ExitBtn.DeleteRes();
}


void CStartPage::Draw()
{
	//m_pSheet->TestSceneRender(0);
	if( g_pDisplay )
		g_pDisplay->Blt( 0, 0, m_pBack, NULL );

	m_LoginBtn.Draw( START_BTN_X, START_BTN_Y + (START_BTN_GAP*0));
	m_MakeAcntBtn.Draw( START_BTN_X, START_BTN_Y + (START_BTN_GAP*1) );
	m_SetOptBtn.Draw( START_BTN_X, START_BTN_Y + (START_BTN_GAP*2) );
	m_ExitBtn.Draw( START_BTN_X, START_BTN_Y + (START_BTN_GAP*3) );

	Sleep(1);

}

void CStartPage::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	switch ( msg )
	{
	case WM_LBUTTONDOWN:

	case WM_MOUSEMOVE:
		m_LoginBtn.MsgProc( hWnd, msg, wParam, lParam );
		m_MakeAcntBtn.MsgProc( hWnd, msg, wParam, lParam );
		m_SetOptBtn.MsgProc( hWnd, msg, wParam, lParam );
		m_ExitBtn.MsgProc( hWnd, msg, wParam, lParam );
		break;

	case WM_LBUTTONUP:
		m_LoginBtn.MsgProc( hWnd, msg, wParam, lParam );
		m_MakeAcntBtn.MsgProc( hWnd, msg, wParam, lParam );
		m_SetOptBtn.MsgProc( hWnd, msg, wParam, lParam );
		m_ExitBtn.MsgProc( hWnd, msg, wParam, lParam );

		if ( m_LoginBtn.GetState() == BTN_ACTION )
		{
			m_LoginBtn.m_iBtnFlg = BTN_NORMAL;
			Action( GOLOGIN );
		}
		else if ( m_MakeAcntBtn.GetState() == BTN_ACTION )
		{
			if (g_bDungeon)
			{
				m_MakeAcntBtn.m_iBtnFlg = BTN_NORMAL;
				Action(GOTUTOR);
			}
			else
			{
				ShellExecute(hWnd, "open" , m_AddRes, NULL, NULL, SW_SHOWNORMAL);
				m_MakeAcntBtn.m_iBtnFlg = BTN_NORMAL;
				Action( GOEXIT );
			}
		}
		else if ( m_SetOptBtn.GetState() == BTN_ACTION )
		{
			m_SetOptBtn.m_iBtnFlg = BTN_NORMAL;
			Action( GOSETOPT );
		}
		else if ( m_ExitBtn.GetState() == BTN_ACTION )
		{
			m_ExitBtn.m_iBtnFlg = BTN_NORMAL;
			Action( GOEXIT );
		}

		break;
	}

}

void CStartPage::Action(int i)
{
	switch (i)
	{
	case GOLOGIN:
		m_pSheet->GoPage( LOGINPAGE );		
		return;

	case GOSETOPT:
		m_pSheet->GoPage( OPTIONPAGE );
		return;

	case GOEXIT:
		exit(1);
		return ;
	}
	return ;
}


void CStartPage::Read_URL_Add(char *filename)
{
	if( filename == NULL )
		return;

	FILE *stream;
	char line[100] = {0,};

	if( (stream = fopen( filename, "rt" )) != NULL )
	{
		if( fgets( line, 100, stream ) == NULL)
			return;
		else
			printf( "%s", line);
		fclose( stream );
	}
	strcpy(m_AddRes, line);
}