#include "stdafx.h"
// WebCtrl.cpp: implementation of the CWebCtrl class.
//
//////////////////////////////////////////////////////////////////////

//#include "AFXPRIV.H"
#include "COMDEF.H"
#include "WebCtrl.h"

#include "main.h"



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWebCtrl::CWebCtrl()
	: m_bComInitSuccess(false)
{
	m_pWebBrowser = NULL;
	m_hWnd_Parent = NULL;
	m_hWnd_Explorer = NULL;
}

CWebCtrl::~CWebCtrl()
{
	CloseWeb();
	m_hWnd_Parent = NULL;
}

void CWebCtrl::SetParentHwnd(HWND hWnd)
{
	m_hWnd_Parent = hWnd;
}

BOOL CWebCtrl::OpenWeb()
{
	if (FAILED(CoInitialize(NULL)))
		return FALSE;
	m_bComInitSuccess = true;

	// Instantiate a browser
	if (FAILED(CoCreateInstance(CLSID_InternetExplorer,NULL, CLSCTX_LOCAL_SERVER, IID_IWebBrowser2,(LPVOID*)&m_pWebBrowser)))
		return FALSE;

	// Capture Explorer HWND
	if((m_hWnd_Explorer =	FindWindow("IEFrame", NULL)) == NULL)
		return FALSE;

	// Set Explorer
	::SetParent(m_hWnd_Explorer,m_hWnd_Parent);

	// Set Browser Style
	SetBrowserStyle();

	return TRUE;
}

void CWebCtrl::CloseWeb()
{
	m_hWnd_Explorer = NULL;
	if(m_pWebBrowser)
	{
		m_pWebBrowser->Quit();
		m_pWebBrowser->Release();
	}
	m_pWebBrowser = NULL;

	if( m_bComInitSuccess == true )
	{
		CoUninitialize();
	}
}

BOOL CWebCtrl::ShowWeb()
{
	if(!m_pWebBrowser)	return FALSE;
	if(!m_hWnd_Parent)	return FALSE;
	if(FAILED(m_pWebBrowser->put_Visible(VARIANT_TRUE))) return FALSE;
	return TRUE;
}

HWND CWebCtrl::GetWebHwnd()
{
	return m_hWnd_Explorer;
}

void CWebCtrl::SetBrowserStyle()
{
	if(!m_pWebBrowser)	return;

	// 임의로 조정해서 쓰자!!!
	m_pWebBrowser->put_AddressBar(VARIANT_FALSE);
	m_pWebBrowser->put_MenuBar(VARIANT_FALSE);
	m_pWebBrowser->put_StatusBar(VARIANT_FALSE);
	m_pWebBrowser->put_ToolBar(FALSE);
	m_pWebBrowser->put_FullScreen(VARIANT_TRUE);
	m_pWebBrowser->put_RegisterAsDropTarget(VARIANT_FALSE);
	m_pWebBrowser->put_RegisterAsBrowser(VARIANT_FALSE);
}

void CWebCtrl::SetBrowserSize( int Width, int Height )
{
	if( m_pWebBrowser )
	{
		m_pWebBrowser->put_Width( Width ); // 임시 테스트. 이걸 잘 조절해주면 웹 페이지에 스크롤 바가 나오게 할수있다.
		m_pWebBrowser->put_Height( Height ); // 임시 테스트. 이걸 잘 조절해주면 웹 페이지에 스크롤 바가 나오게 할수있다.
	}
}

BOOL CWebCtrl::Navigate(char *szAddr)
{
//	USES_CONVERSION;
	VARIANT pvarEmpty = {0};
	if(!szAddr)	return FALSE;
	if(!m_pWebBrowser)	return FALSE;

	_bstr_t bstrText = szAddr;
	BSTR bstrAddr;
	bstrAddr = bstrText.copy();

	if(FAILED(m_pWebBrowser->Navigate( bstrAddr,&pvarEmpty,&pvarEmpty,&pvarEmpty,&pvarEmpty))) return FALSE;
//	if(FAILED(m_pWebBrowser->Navigate(A2BSTR(szAddr),&pvarEmpty,&pvarEmpty,&pvarEmpty,&pvarEmpty))) return FALSE;
//	::SetFocus(m_hWnd_Explorer);
	return TRUE;
}

BOOL CWebCtrl::GoHome()
{
	if(!m_pWebBrowser)	return FALSE;
	if(FAILED(m_pWebBrowser->GoHome()))	return FALSE;
	return TRUE;
}

void CWebCtrl::Refresh()
{
	if(!m_pWebBrowser)	return;
	m_pWebBrowser->Refresh();
}




CWebWnd::CWebWnd()
{
	m_WebHwnd = NULL;
//	m_DummyHwnd = NULL;

	Init();
}

CWebWnd::~CWebWnd()
{
	DeleteRes();
}

void CWebWnd::Init()
{
	m_WebRect.left = 0;
	m_WebRect.top = 0;
	m_WebRect.right = 0;
	m_WebRect.bottom = 0;

	oldLaghaimPT.x = -1;
	oldLaghaimPT.y = -1;

}
void CWebWnd::LoadRes(int type)
{
	if( m_WebHwnd )
	{
		DestroyWindow( m_WebHwnd );
		m_WebHwnd = NULL;
	}
	/*
		if( m_DummyHwnd )
		{
			DestroyWindow( m_DummyHwnd );
			m_DummyHwnd = NULL;
		}
	*/
	switch( type )
	{
	case 0:
		m_WebHwnd = CreateWindow("static","",WS_CLIPCHILDREN|WS_VISIBLE|WS_POPUP,0,0,10,10,pCMyApp->Get_hWnd(),NULL,pCMyApp->hInstApp,NULL );
		break;
	case 1:
		m_WebHwnd = CreateWindow("static","",WS_CLIPCHILDREN|WS_VISIBLE|WS_POPUP|WS_VSCROLL,0,0,10,10,pCMyApp->Get_hWnd(),NULL,pCMyApp->hInstApp,NULL );
		break;

	}

//	m_DummyHwnd = CreateWindow("static","",WS_CLIPCHILDREN|WS_VISIBLE|WS_POPUP,0,0,10,10,pCMyApp->Get_hWnd(),NULL,pCMyApp->hInstApp,NULL );

//	DWORD dwStyle = GetWindowLong( m_DummyHwnd, GWL_EXSTYLE );
//	SetWindowLong( m_DummyHwnd, GWL_EXSTYLE, dwStyle|WS_EX_TOPMOST|WS_EX_TRANSPARENT );

	ShowWindow( m_WebHwnd, SW_HIDE );
//	ShowWindow( m_DummyHwnd, SW_HIDE );

	m_WebCtrl.SetParentHwnd( m_WebHwnd );
	m_WebCtrl.OpenWeb();
	m_WebCtrl.ShowWeb();
//	m_WebCtrl.GoHome();
}

void CWebWnd::DeleteRes()
{
	m_WebCtrl.CloseWeb();

	if( m_WebHwnd )
	{
		DestroyWindow( m_WebHwnd );
		m_WebHwnd = NULL;
	}
	/*
		if( m_DummyHwnd )
		{
			DestroyWindow( m_DummyHwnd );
			m_DummyHwnd = NULL;
		}
	*/
}

void CWebWnd::Open( char *Url, int x, int y, int Width, int Height )
{
	// 보내온 좌표는 클라이언트 좌표이므로 스크린 좌표로 변환해준다.
	POINT TempPoint;

	TempPoint.x = x;
	TempPoint.y = y;
//	ClientToScreen( pCMyApp->Get_hWnd(), &TempPoint );
//	ScreenToClient( pCMyApp->Get_hWnd(), &TempPoint );
	m_WebRect.left = TempPoint.x;
	m_WebRect.top = TempPoint.y;

	m_WebRect.right = Width;
	m_WebRect.bottom = Height;

	/*
		TempPoint.x = Width;
		TempPoint.y = Height;
		ClientToScreen( pCMyApp->Get_hWnd(), &TempPoint );
		m_WebRect.right = TempPoint.x;
		m_WebRect.bottom = TempPoint.y;
	*/

	// 크기조절해줘야한다.
	RECT LaghaimRect;
	GetWindowRect( pCMyApp->Get_hWnd(), &LaghaimRect );
	MoveWindow( m_WebHwnd, LaghaimRect.left+m_WebRect.left, LaghaimRect.top+m_WebRect.top, m_WebRect.right, m_WebRect.bottom, TRUE );
//	MoveWindow( m_DummyHwnd, LaghaimRect.left+m_WebRect.left, LaghaimRect.top+m_WebRect.top, m_WebRect.right, m_WebRect.bottom, TRUE );

	ShowWindow( m_WebHwnd, SW_SHOW );
//	ShowWindow( m_DummyHwnd, SW_SHOW );

	SetForegroundWindow( pCMyApp->Get_hWnd() );// 이걸 안해주면 화면이 아래로 내려간 후에 돌아오질 못하더라...(일반적인 풀스크린 모드시..구라 풀모드는 상관없다.)

	m_WebCtrl.SetBrowserSize( Width, Height );
	m_WebCtrl.Navigate( Url );
}

void CWebWnd::Draw()
{
	RECT LaghaimRect;
	GetWindowRect( pCMyApp->Get_hWnd(), &LaghaimRect );

	if( LaghaimRect.left != oldLaghaimPT.x || LaghaimRect.top != oldLaghaimPT.y ) // 라그하임 창의 위치가 바뀌었으면..
	{
		MoveWindow( m_WebHwnd, LaghaimRect.left+m_WebRect.left, LaghaimRect.top+m_WebRect.top, m_WebRect.right, m_WebRect.bottom, TRUE );
//		MoveWindow( m_DummyHwnd, LaghaimRect.left+m_WebRect.left, LaghaimRect.top+m_WebRect.top, m_WebRect.right, m_WebRect.bottom, TRUE );

		m_WebCtrl.Refresh(); // 이걸해줘야 브라우져 창이 제대로 갱신되더라.

		oldLaghaimPT.x = LaghaimRect.left;
		oldLaghaimPT.y = LaghaimRect.top;
	}
}

void CWebWnd::Close()
{
	ShowWindow( m_WebHwnd, SW_HIDE );
//	ShowWindow( m_DummyHwnd, SW_HIDE );
}
