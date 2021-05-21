#include "stdafx.h"
//-----------------------------------------------------------------------------
// File: D3DApp.cpp
//
// Desc: Application class for the Direct3D samples framework library.
//
// Copyright (c) 1998-1999 Microsoft Corporation. All rights reserved.
//				 Additions by Peter Kovach and William Chin
//-----------------------------------------------------------------------------



#include <pbt.h>
#include <mmsystem.h>

#include <tchar.h>
#include "D3DApp.h"
#include "resource.h"
#include <imm.h>

//-----------------------------------------------------------------------------
// Internal function prototypes and variables
//-----------------------------------------------------------------------------
enum APPMSGTYPE { MSG_NONE, MSGERR_APPMUSTEXIT, MSGWARN_SWITCHEDTOSOFTWARE };

static INT     CALLBACK AboutProc( HWND, UINT, WPARAM, LPARAM );
LRESULT CALLBACK WndProc( HWND, UINT, WPARAM, LPARAM );

CD3DApplication* g_pD3DApp;
HINSTANCE g_hInst;
extern HCURSOR g_hCursor;
extern BOOL	   g_bFFullMode;
extern BOOL	   g_bAdmin;

//-----------------------------------------------------------------------------
// Name: CD3DApplication()
// Desc: This is the constructor for CD3DApplication
//-----------------------------------------------------------------------------
CD3DApplication::CD3DApplication()
{
	m_pFramework   = NULL;
	m_hWnd         = NULL;
	m_pDD          = NULL;
	m_pD3D         = NULL;
	m_pd3dDevice   = NULL;

	m_pddsRenderTarget     = NULL;
	m_pddsRenderTargetLeft = NULL;

	m_bActive         = FALSE;
	m_bReady          = FALSE;
	m_bFrameMoving    = TRUE;
	m_bSingleStep     = FALSE;

	m_strWindowTitle  = _T("Direct3D Application");
	m_bAppUseZBuffer  = FALSE;
	m_bAppUseStereo   = FALSE;
	m_bShowStats      = FALSE;
	m_fnConfirmDevice = NULL;

	m_bRGB565		  = TRUE;

	m_bInitLoadingScr	  = FALSE;

	g_pD3DApp = this;
}



//-----------------------------------------------------------------------------
// Name: Create()
// Desc: D3D 디바이스를 선택하고, 사용자 씬을 구성한다.
//-----------------------------------------------------------------------------
HRESULT CD3DApplication::Create( HINSTANCE hInst, TCHAR* strCmdLine )
{
	HRESULT hr;

	// 인스턴스 저장
	g_hInst = hInst;

	if (FAILED(hr = EnumerateAndSelect()))
		return hr;

	// Initialize the app's custom scene stuff
	if( FAILED( hr = OneTimeSceneInit() ) )
	{
		DisplayFrameworkError( hr, MSGERR_APPMUSTEXIT );
		return hr;
	}


	// Create a new CD3DFramework class. This class does all of our D3D
	// initialization and manages the common D3D objects.
	if( NULL == ( m_pFramework = new CD3DFramework7() ) )
	{
		DisplayFrameworkError( E_OUTOFMEMORY, MSGERR_APPMUSTEXIT );
		return E_OUTOFMEMORY;
	}

	// 윈도우 클래스 구성
	WNDCLASS wndClass = { CS_DBLCLKS, WndProc, 0, 0, hInst,
						  LoadIcon( hInst, MAKEINTRESOURCE(IDI_MAIN_ICON) ),
						  NULL,
						  //LoadCursor( hInst, MAKEINTRESOURCE(IDC_CURSOR1) ),
						  //g_hCursor,
						  (HBRUSH)GetStockObject(HOLLOW_BRUSH),
						  NULL, _T("D3D Window")
						};
	// wndClass.hCursor = NULL;
	RegisterClass( &wndClass );

	// 윈도우 생성
	DWORD dwStyle;
	//RECT rect = {0, 0, 640, 480};
	RECT rect = {0, 0, 800, 600};
	//RECT rect = {0, 0, 1024, 768};
	if (m_bWindowed)
	{
		if (!g_bFFullMode)
		{
			if (!g_bAdmin)
			{
				dwStyle = 0;
				dwStyle = WS_SYSMENU|WS_CAPTION|WS_BORDER|WS_POPUP|WS_VISIBLE;
				dwStyle ^= WS_MAXIMIZE;
				dwStyle ^= WS_MINIMIZE;

				AdjustWindowRect(&rect, dwStyle, FALSE);
				m_hWnd = CreateWindow( _T("D3D Window"), m_strWindowTitle,
									   dwStyle,
									   CW_USEDEFAULT, CW_USEDEFAULT, rect.right-rect.left, rect.bottom-rect.top, 0L,
									   NULL,
									   hInst, 0L );
			}
			else
			{
				dwStyle = WS_OVERLAPPEDWINDOW|WS_VISIBLE;
				AdjustWindowRect(&rect, dwStyle, TRUE);
				m_hWnd = CreateWindow( _T("D3D Window"), m_strWindowTitle,
									   dwStyle,
									   CW_USEDEFAULT, CW_USEDEFAULT, rect.right-rect.left, rect.bottom-rect.top, 0L,
									   LoadMenu( hInst, MAKEINTRESOURCE(IDR_MENU) ),
									   hInst, 0L );
			}
		}
		else
		{
			m_hWnd = CreateWindow( _T("D3D Window"), m_strWindowTitle,
								   WS_POPUP|WS_VISIBLE,
								   0, 0, rect.right-rect.left, rect.bottom-rect.top, 0L,
								   NULL,
								   hInst, 0L );
		}
	}
	else
	{
		// FullScreen Mode
		m_hWnd = CreateWindow( _T("D3D Window"), m_strWindowTitle,
							   WS_POPUP|WS_VISIBLE,
							   0, 0, rect.right-rect.left, rect.bottom-rect.top, 0L,
							   NULL,
							   hInst, 0L );
	}

	if (!m_hWnd)
	{
		LPVOID lpMsgBuf;
		FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			GetLastError(),
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
			(LPTSTR) &lpMsgBuf,
			0,
			NULL);
		//
		// Process any inserts in lpMsgBuf.
		// ...
		// Display the string.
		//
		MessageBox( NULL, (LPCTSTR)lpMsgBuf, "Error", MB_OK | MB_ICONINFORMATION );
		// Free the buffer.
		LocalFree( lpMsgBuf );
	}

	UpdateWindow( m_hWnd );

	// Initialize the 3D environment for the app
	if( FAILED( hr = Initialize3DEnvironment() ) )
	{
		DisplayFrameworkError( hr, MSGERR_APPMUSTEXIT );
		Cleanup3DEnvironment();
		return E_FAIL;
	}

	// Setup the app so it can support single-stepping
	m_dwBaseTime = timeGetTime();

	// 애플리케이션이 시작될 준비가 되었음.
	m_bReady = TRUE;

	return S_OK;
}

HRESULT CD3DApplication::EnumerateAndSelect()
{
	HRESULT hr;

	// Enumerate available D3D devices. The callback is used so the app can
	// confirm/reject each enumerated device depending on its capabilities.
	// m_fnConfirmDevice에 사용자 콜백 함수를 지정할 수 있다.
	// m_fnConfirmDevice가 자식에 상속되므로 CD3DApplication의 자식 클래스의
	// 멤버 함수를 가르킬수 있다.
	if( FAILED( hr = D3DEnum_EnumerateDevices( m_fnConfirmDevice ) ) )
	{
		DisplayFrameworkError( hr, MSGERR_APPMUSTEXIT );
		return hr;
	}

	// Select a device. Ask for a hardware device that renders in a window.
	// 여기서 윈도우 모드로 할 것인가 말것인가를 결정할 수 있다.
	if( FAILED( hr = D3DEnum_SelectDefaultDevice( &m_pDeviceInfo ) ) )
	{
		DisplayFrameworkError( hr, MSGERR_APPMUSTEXIT );
		return hr;
	}
	// 윈도우 모드 또는 풀스크린 모드 설정
	m_pDeviceInfo->bWindowed = m_bWindowed;

	return S_OK;
}

//-----------------------------------------------------------------------------
// Name: Run()
// Desc: Message-processing loop. Idle time is used to render the scene.
//----------------------------------------------------------------------------
INT CD3DApplication::Run()
{
	// Load keyboard accelerators
	HACCEL hAccel = LoadAccelerators( NULL, MAKEINTRESOURCE(IDR_MAIN_ACCEL) );

	// Now we're ready to recieve and process Windows messages.
	BOOL bGotMsg;
	MSG  msg;
	PeekMessage( &msg, NULL, 0U, 0U, PM_NOREMOVE );

	while( WM_QUIT != msg.message  )
	{
		// Use PeekMessage() if the app is active, so we can use idle time to
		// render the scene. Else, use GetMessage() to avoid eating CPU time.
		bGotMsg = PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE );		

		if( bGotMsg )
		{
			// Translate and dispatch the message
			if( 0 == TranslateAccelerator( m_hWnd, hAccel, &msg ) )
			{
				TranslateMessage( &msg );
				DispatchMessage( &msg );
			}
		}
		else
		{
			// Render a frame during idle time (no messages are waiting)
			if( m_bReady )
			{
				// 2D, 3D 렌더링 메인 루틴
				if( FAILED( ProcessNextFrame(m_hWnd) ) )
					DestroyWindow( m_hWnd );
			}
		}
	}

	return msg.wParam;
}

//-----------------------------------------------------------------------------
// Name: WndProc()
// Desc: Static msg handler which passes messages to the application class.
//  왜 정적으로 선언해야 할까요?
//-----------------------------------------------------------------------------
LRESULT CALLBACK WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if( g_pD3DApp )
		return g_pD3DApp->MsgProc( hWnd, uMsg, wParam, lParam );

	return DefWindowProc( hWnd, uMsg, wParam, lParam );
}

//-----------------------------------------------------------------------------
// Name: AboutProc()
// Desc: Minimal message proc function for the about box
//-----------------------------------------------------------------------------
BOOL CALLBACK AboutProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM )
{
	if( WM_COMMAND == uMsg )
		if( IDOK == LOWORD(wParam) || IDCANCEL == LOWORD(wParam) )
			EndDialog( hWnd, TRUE );

	return WM_INITDIALOG == uMsg ? TRUE : FALSE;
}

//-----------------------------------------------------------------------------
// Name: MsgProc()
// Desc: Message handling function.
//-----------------------------------------------------------------------------
LRESULT CD3DApplication::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam,
								  LPARAM lParam )
{
	HRESULT hr;

	switch( uMsg )
	{
	case WM_CREATE:
		break;
	case WM_PAINT:
		// Handle paint messages when the app is not ready
		if( m_pFramework && !m_bReady )
		{
			m_pFramework->ShowFrame();
			//CMyD3DApplication::InitLoadingScr();
			//InitLoadingScr();
		}
		break;

	case WM_MOVE:
		// If in windowed mode, move the Framework's window
		if( m_pFramework && m_bActive && m_bReady && m_pDeviceInfo->bWindowed )
			m_pFramework->Move( (SHORT)LOWORD(lParam), (SHORT)HIWORD(lParam) );
		break;

	case WM_SIZE:
		// Check to see if we are losing our window...
		if( SIZE_MAXHIDE==wParam || SIZE_MINIMIZED==wParam )
		{
			m_bActive = FALSE;
		}
		else
		{
			m_bActive = TRUE;
		}

		// A new window size will require a new backbuffer
		// size, so the 3D structures must be changed accordingly.
		if( m_bActive && m_bReady && m_pDeviceInfo->bWindowed )
		{
			m_bReady = FALSE;
			m_bGameMode = FALSE;

			if( FAILED( hr = Change3DEnvironment() ) )
				return 0;

			m_bReady = TRUE;
			m_bGameMode = TRUE;
		}
		GetClientRect(m_hWnd, &m_rcClient);
		break;

	case WM_SETCURSOR:

		//SetCursor(g_hCursor);
		SetCursor(NULL);
		// Prevent a cursor in fullscreen mode
		if( m_bActive && m_bReady && !m_pDeviceInfo->bWindowed )
		{
			//SetCursor(NULL);
			return 1;
		}
		break;

	case WM_ENTERMENULOOP:
		// Pause the app when menus are displayed
		Pause(TRUE);
		break;
	case WM_EXITMENULOOP:
		Pause(FALSE);
		break;

	case WM_ENTERSIZEMOVE:
		// Halt frame movement while the app is sizing or moving
		if( m_bFrameMoving )
			m_dwStopTime = timeGetTime();
		break;
	case WM_EXITSIZEMOVE:
		if( m_bFrameMoving )
			m_dwBaseTime += timeGetTime() - m_dwStopTime;
		break;

	case WM_CONTEXTMENU:
		// Handle the app's context menu (via right mouse click)
		/*
		TrackPopupMenuEx(
		        GetSubMenu( LoadMenu( 0, MAKEINTRESOURCE(IDR_POPUP) ), 0 ),
		        TPM_VERTICAL, LOWORD(lParam), HIWORD(lParam), hWnd, NULL );
		*/
		break;

	case WM_NCHITTEST:
		// Prevent the user from selecting the menu in fullscreen mode
		if( !m_pDeviceInfo->bWindowed )
			return HTCLIENT;
		break;

	case WM_POWERBROADCAST:
		switch( wParam )
		{
		case PBT_APMQUERYSUSPEND:
			// At this point, the app should save any data for open
			// network connections, files, etc.., and prepare to go into
			// a suspended mode.
			return OnQuerySuspend( (DWORD)lParam );

		case PBT_APMRESUMESUSPEND:
			// At this point, the app should recover any data, network
			// connections, files, etc.., and resume running from when
			// the app was suspended.
			return OnResumeSuspend( (DWORD)lParam );
		}
		break;

	case WM_SYSCOMMAND:
		// Prevent moving/sizing and power loss in fullscreen mode
		switch( wParam )
		{
		case SC_MOVE:
		case SC_SIZE:
		case SC_MAXIMIZE:
		case SC_MONITORPOWER:
			if( FALSE == m_pDeviceInfo->bWindowed )
				return 1;
			break;
		}
		break;



	case WM_COMMAND:
		switch( LOWORD(wParam) )
		{
		case IDM_TOGGLESTART:
			// Toggle frame movement
			m_bFrameMoving = !m_bFrameMoving;

			if( m_bFrameMoving )
				m_dwBaseTime += timeGetTime() - m_dwStopTime;
			else
				m_dwStopTime = timeGetTime();
			break;

		case IDM_SINGLESTEP:
			// Single-step frame movement
			if( FALSE == m_bFrameMoving )
				m_dwBaseTime += timeGetTime() - ( m_dwStopTime + 100 );

			m_dwStopTime   = timeGetTime();
			m_bFrameMoving = FALSE;
			m_bSingleStep  = TRUE;
			break;

		case IDM_CHANGEDEVICE:
			// Display the device-selection dialog box.
			if( m_bActive && m_bReady )
			{
				Pause(TRUE);

				if( SUCCEEDED( D3DEnum_UserChangeDevice( &m_pDeviceInfo ) ) )
				{
					m_bGameMode = FALSE;
					m_bWindowed = FALSE;
					if( FAILED( hr = Change3DEnvironment() ) )
						return 0;
					m_bGameMode = TRUE;
				}
				Pause(FALSE);
			}
			return 0;

		case IDM_TOGGLEFULLSCREEN:
			// Toggle the fullscreen/window mode
			if( m_bActive && m_bReady )
			{
				m_bReady = FALSE;
				m_bGameMode = FALSE;

				m_pDeviceInfo->bWindowed = !m_pDeviceInfo->bWindowed;

				if( FAILED( hr = Change3DEnvironment() ) )
					return 0;

				m_bReady = TRUE;
				m_bGameMode = TRUE;
			}
			return 0;
		case IDM_ABOUT:
			// Display the About box
			Pause(TRUE);
			DialogBox( (HINSTANCE)GetWindowLong( hWnd, GWL_HINSTANCE ),
					   MAKEINTRESOURCE(IDD_ABOUT), hWnd, AboutProc );
			Pause(FALSE);
			return 0;

		case IDM_EXIT:
			// Recieved key/menu command to exit app
			SendMessage( hWnd, WM_CLOSE, 0, 0 );
			return 0;
		}
		break;

	case WM_GETMINMAXINFO:
		((MINMAXINFO*)lParam)->ptMinTrackSize.x = 100;
		((MINMAXINFO*)lParam)->ptMinTrackSize.y = 100;
		break;

	case WM_DISPLAYCHANGE:
		/*bGameMode = FALSE;
		if( FAILED( hr = Change3DEnvironment() ) )
			return 0;
		m_bGameMode = TRUE;*/
		break;
	// Close the window
	case WM_CLOSE:
		DestroyWindow( hWnd );
		return 0;

		// The WM_DESTROY message is sent when a window is being
		// destroyed. It is sent to the window procedure of the window
		// being destroyed after the window is removed from the screen.
		//
		// This message is sent first to the window being destroyed and
		// then to the child windows (if any) as they are destroyed.
		// During the processing of the message, it can be assumed that
		// all child windows still exist.
		/*
		case WM_DESTROY:
			Cleanup3DEnvironment();
		    PostQuitMessage(0);
		    return 0;
		*/
	}
	//
	// The DefWindowProc function calls the default window procedure
	// to provide default processing for any window messages that an
	// application does not process. This function ensures that every
	// message is processed. DefWindowProc is called with the same
	// parameters received by the window procedure.

	return DefWindowProc( hWnd, uMsg, wParam, lParam );
}

//-----------------------------------------------------------------------------
// Name: Initialize3DEnvironment()
// Desc: Initializes the sample framework, then calls the app-specific function
//       to initialize device specific objects. This code is structured to
//       handle any errors that may occur duing initialization
//-----------------------------------------------------------------------------
HRESULT CD3DApplication::Initialize3DEnvironment()
{
	HRESULT hr;
	DWORD   dwFrameworkFlags = 0L;
	dwFrameworkFlags |= ( !m_pDeviceInfo->bWindowed ? D3DFW_FULLSCREEN : 0L );
	dwFrameworkFlags |= (  m_pDeviceInfo->bStereo   ? D3DFW_STEREO     : 0L );
	dwFrameworkFlags |= (  m_bAppUseZBuffer         ? D3DFW_ZBUFFER    : 0L );

	// Initialize the Direct3D framework
	// 프레임워크의 초기화가 성공을 하면
	// 관련된 멤버의 정보를 얻어온다.
	if( SUCCEEDED( hr = m_pFramework->Initialize( m_hWnd,
						m_pDeviceInfo->pDriverGUID, m_pDeviceInfo->pDeviceGUID,
						&m_pDeviceInfo->ddsdFullscreenMode, dwFrameworkFlags ) ) )
	{
		m_pDD        = m_pFramework->GetDirectDraw();
		m_pD3D       = m_pFramework->GetDirect3D();
		m_pd3dDevice = m_pFramework->GetD3DDevice();
		m_pd3dDevicePick = m_pFramework->GetD3DDevicePick();

		m_pddsRenderTarget     = m_pFramework->GetRenderSurface();
		m_pddsRenderTargetLeft = m_pFramework->GetRenderSurfaceLeft();
		m_ddsdRenderTarget.dwSize = sizeof(m_ddsdRenderTarget);
		m_pddsRenderTarget->GetSurfaceDesc( &m_ddsdRenderTarget );

		if( (m_ddsdRenderTarget.ddpfPixelFormat.dwRBitMask & 0xf800)
				&& (m_ddsdRenderTarget.ddpfPixelFormat.dwGBitMask&0x07e0)
				&& (m_ddsdRenderTarget.ddpfPixelFormat.dwBBitMask & 0x001f) )
			m_bRGB565 = TRUE;
		else
			m_bRGB565 = FALSE;

		// 24 or 32 bit 모드 일때에는 RGB565모드를 true로 놓는다.
		// SetResolution(16);
		if ( m_ddsdRenderTarget.ddpfPixelFormat.dwRGBBitCount == 32)
		{
			SetResolution (m_ddsdRenderTarget.ddpfPixelFormat.dwRGBBitCount);
			m_bRGB565 = TRUE;
		}
		else if (m_ddsdRenderTarget.ddpfPixelFormat.dwRGBBitCount == 16)
		{
			SetResolution (m_ddsdRenderTarget.ddpfPixelFormat.dwRGBBitCount);
		}

		// 로딩 부분을 위해서.
		if (m_bInitLoadingScr == FALSE)
		{
			InitLoadingScr();
		}

		// 3DFramework에서 생성된 오브젝트를 2D Frame에 연결한다.
		if( FAILED( hr = Init2DFrameObjects() ) )
		{
			DeleteDeviceObjects();
			m_pFramework->DestroyObjects();
		}

		// Let the app run its startup code which creates the 3d scene.
		if( SUCCEEDED( hr = InitDeviceObjects() ) )
			return S_OK;
		else
		{
			DeleteDeviceObjects();
			m_pFramework->DestroyObjects();
		}
	}

	// If we get here, the first initialization pass failed. If that was with a
	// hardware device, try again using a software rasterizer instead.
	if( m_pDeviceInfo->bHardware )
	{
		// Try again with a software rasterizer
		DisplayFrameworkError( hr, MSGWARN_SWITCHEDTOSOFTWARE );
		D3DEnum_SelectDefaultDevice( &m_pDeviceInfo, D3DENUM_SOFTWAREONLY );
		return Initialize3DEnvironment();
	}

	return hr;
}


//-----------------------------------------------------------------------------
// Name: Change3DEnvironment()
// Desc: Handles driver, device, and/or mode changes for the app.
//-----------------------------------------------------------------------------
HRESULT CD3DApplication::Change3DEnvironment()
{
	HRESULT hr;
	static BOOL  bOldWindowedState = TRUE;
	static DWORD dwSavedStyle;
	static RECT  rcSaved;

	// Release all scene objects that will be re-created for the new device
	DeleteDeviceObjects();

	// Release framework objects, so a new device can be created

	hr = m_pFramework->DestroyObjects();

	if( FAILED( hr ) )
	{
		DisplayFrameworkError( hr, MSGERR_APPMUSTEXIT );
		SendMessage( m_hWnd, WM_CLOSE, 0, 0 );
		return hr;
	}

	if( bOldWindowedState != m_pDeviceInfo->bWindowed )
	{
		if( m_pDeviceInfo->bWindowed )
		{
			SetWindowLong( m_hWnd, GWL_STYLE, dwSavedStyle );
			SetWindowPos( m_hWnd, HWND_NOTOPMOST, rcSaved.left, rcSaved.top,
						  ( rcSaved.right - rcSaved.left ),
						  ( rcSaved.bottom - rcSaved.top ), SWP_SHOWWINDOW );
		}
		else
		{
			SetMenu(m_hWnd, NULL);
			dwSavedStyle = GetWindowLong( m_hWnd, GWL_STYLE );
			GetWindowRect( m_hWnd, &rcSaved );
			SetWindowLong( m_hWnd, GWL_STYLE, WS_POPUP|WS_VISIBLE );
		}

		bOldWindowedState = m_pDeviceInfo->bWindowed;
	}

	// Inform the framework class of the driver change. It will internally
	// re-create valid surfaces, a d3ddevice, etc.
	if( FAILED( hr = Initialize3DEnvironment() ) )
	{
		DisplayFrameworkError( hr, MSGERR_APPMUSTEXIT );
		SendMessage( m_hWnd, WM_CLOSE, 0, 0 );
		return hr;
	}

	// If the app is paused, trigger the rendering of the current frame
	if( FALSE == m_bFrameMoving )
	{
		m_bSingleStep = TRUE;
		m_dwBaseTime += timeGetTime() - m_dwStopTime;
		m_dwStopTime  = timeGetTime();
	}

	return S_OK;
}

//-----------------------------------------------------------------------------
// Name: ProcessNextFrame()
// Desc:
//-----------------------------------------------------------------------------
HRESULT CD3DApplication::ProcessNextFrame(HWND hwnd)
{
	HRESULT hr;

	// Check the cooperative level before rendering
	if( FAILED( hr = m_pDD->TestCooperativeLevel() ) )
	{
		switch( hr )
		{
		case DDERR_EXCLUSIVEMODEALREADYSET:
		case DDERR_NOEXCLUSIVEMODE:
			// Do nothing because some other app has exclusive mode
			return S_OK;

		case DDERR_WRONGMODE:
			// The display mode changed on us. Resize accordingly
			if( m_pDeviceInfo->bWindowed )
				return Change3DEnvironment();
			break;
		}
		return hr;
	}

	// 3D Rendering First and then 2D display on it.
	Render3DEnvironment();
	Render2DEnvironment(hwnd);

	// Show the frame rate, etc.
	if( m_bShowStats )
		ShowStats();

	return S_OK;
}

//-----------------------------------------------------------------------------
// Name: Render2DEnvironment()
// Desc:
//-----------------------------------------------------------------------------
HRESULT CD3DApplication::Render2DEnvironment(HWND hwnd)
{
	HRESULT hr;

	// Display the sprites on the screen
	if( FAILED( hr = DisplayFrame() ) )
	{
		if( hr != DDERR_SURFACELOST )
			return hr;

		// The surfaces were lost so restore them
		//Restore2DSurfaces();
	}

	return S_OK;
}

//-----------------------------------------------------------------------------
// Name: Render3DEnvironment()
// Desc: Draws the scene in stereo or mono mode.  It shows the frame
//		 and displays the scoring information on the screen if the
//		 show stats flag is set.
//-----------------------------------------------------------------------------
HRESULT CD3DApplication::Render3DEnvironment()
{
	HRESULT hr;

	// Get the relative time, in seconds
	// 무얼까?
	FLOAT fTime = ( timeGetTime() - m_dwBaseTime ) * 0.001f;

	// FrameMove (animate) the scene
	if( m_bFrameMoving || m_bSingleStep )
	{
		if( FAILED( hr = FrameMove( fTime ) ) )
			return hr;

		m_bSingleStep = FALSE;
	}

	if( FAILED( hr = Render() ) )
		return hr;

	return S_OK;
}


//-----------------------------------------------------------------------------
// Name: Cleanup3DEnvironment()
// Desc: Cleanup scene objects
//-----------------------------------------------------------------------------
VOID CD3DApplication::Cleanup3DEnvironment()
{
	m_bActive = FALSE;
	m_bReady  = FALSE;

	if( m_pFramework )
	{
		DeleteDeviceObjects();
		SAFE_DELETE( m_pFramework );

		FinalCleanup();
	}

	D3DEnum_FreeResources();
}


//-----------------------------------------------------------------------------
// Name: Pause()
// Desc: Called in to toggle the pause state of the app. This function
//       brings the GDI surface to the front of the display, so drawing
//       output like message boxes and menus may be displayed.
//-----------------------------------------------------------------------------
VOID CD3DApplication::Pause( BOOL bPause )
{
	static DWORD dwAppPausedCount = 0L;

	dwAppPausedCount += ( bPause ? +1 : -1 );
	m_bReady          = ( dwAppPausedCount ? FALSE : TRUE );

	// Handle the first pause request (of many, nestable pause requests)
	if( bPause && ( 1 == dwAppPausedCount ) )
	{
		// Get a surface for the GDI
		if( m_pFramework )
			m_pFramework->FlipToGDISurface( TRUE );

		// Stop the scene from animating
		if( m_bFrameMoving )
			m_dwStopTime = timeGetTime();
	}

	if( 0 == dwAppPausedCount )
	{
		// Restart the scene
		if( m_bFrameMoving )
			m_dwBaseTime += timeGetTime() - m_dwStopTime;
	}
}


//-----------------------------------------------------------------------------
// Name: OnQuerySuspend()
// Desc: Called when the app receives a PBT_APMQUERYSUSPEND message, meaning
//       the computer is about to be suspended. At this point, the app should
//       save any data for open network connections, files, etc.., and prepare
//       to go into a suspended mode.
//-----------------------------------------------------------------------------
LRESULT CD3DApplication::OnQuerySuspend( DWORD dwFlags )
{
	Pause(TRUE);

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name: OnResumeSuspend()
// Desc: Called when the app receives a PBT_APMRESUMESUSPEND message, meaning
//       the computer has just resumed from a suspended state. At this point,
//       the app should recover any data, network connections, files, etc..,
//       and resume running from when the app was suspended.
//-----------------------------------------------------------------------------
LRESULT CD3DApplication::OnResumeSuspend( DWORD dwData )
{
	Pause(FALSE);

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name: ShowStats()
// Desc: Shows frame rate and dimensions of the rendering device.
//-----------------------------------------------------------------------------
VOID CD3DApplication::ShowStats()
{
	static FLOAT fFPS      = 0.0f;
	static FLOAT fLastTime = 0.0f;
	static DWORD dwFrames  = 0L;

	// Keep track of the time lapse and frame count
	FLOAT fTime = timeGetTime() * 0.001f; // Get current time in seconds
	++dwFrames;

	// Update the frame rate once per second
	if( fTime - fLastTime > 1.0f )
	{
		fFPS      = dwFrames / (fTime - fLastTime);
		fLastTime = fTime;
		dwFrames  = 0L;
	}

	// Setup the text buffer to write out dimensions
	TCHAR buffer[80];
	sprintf( buffer, _T("%7.02f fps (%dx%dx%d) is."), fFPS,
			 m_ddsdRenderTarget.dwWidth, m_ddsdRenderTarget.dwHeight,
			 m_ddsdRenderTarget.ddpfPixelFormat.dwRGBBitCount );
	OutputText( 0, 0, buffer );
}


//-----------------------------------------------------------------------------
// Name: OutputText()
// Desc: Draws text on the window.
//-----------------------------------------------------------------------------
VOID CD3DApplication::OutputText( DWORD x, DWORD y, TCHAR* str )
{
	HDC hDC;

	// Get a DC for the surface. Then, write out the buffer
	if( m_pddsRenderTarget )
	{
		if( SUCCEEDED( m_pddsRenderTarget->GetDC(&hDC) ) )
		{
			SetTextColor( hDC, RGB(255,255,0) );
			SetBkMode( hDC, TRANSPARENT );
			ExtTextOut( hDC, x, y, 0, NULL, str, lstrlen(str), NULL );
			m_pddsRenderTarget->ReleaseDC(hDC);
		}
	}

	// Do the same for the left surface (in case of stereoscopic viewing).
	if( m_pddsRenderTargetLeft )
	{
		if( SUCCEEDED( m_pddsRenderTargetLeft->GetDC( &hDC ) ) )
		{
			// Use a different color to help distinguish left eye view
			SetTextColor( hDC, RGB(255,0,255) );
			SetBkMode( hDC, TRANSPARENT );
			ExtTextOut( hDC, x, y, 0, NULL, str, lstrlen(str), NULL );
			m_pddsRenderTargetLeft->ReleaseDC(hDC);
		}
	}
}

//-----------------------------------------------------------------------------
// Name: DisplayFrameworkError()
// Desc: Displays error messages in a message box
//-----------------------------------------------------------------------------
VOID CD3DApplication::DisplayFrameworkError( HRESULT hr, DWORD dwType )
{
	TCHAR strMsg[512];

	switch( hr )
	{
	case D3DENUMERR_NODIRECTDRAW:
		lstrcpy( strMsg, _T("Could not create DirectDraw!") );
		break;
	case D3DENUMERR_NOCOMPATIBLEDEVICES:
		lstrcpy( strMsg, _T("Could not find any compatible Direct3D\n"
							"devices.") );
		break;
	case D3DENUMERR_SUGGESTREFRAST:
		lstrcpy( strMsg, _T("Could not find any compatible devices.\n\n"
							"Try enabling the reference rasterizer using\n"
							"EnableRefRast.reg.") );
		break;
	case D3DENUMERR_ENUMERATIONFAILED:
		lstrcpy( strMsg, _T("Enumeration failed. Your system may be in an\n"
							"unstable state and need to be rebooted") );
		break;
	case D3DFWERR_INITIALIZATIONFAILED:
		lstrcpy( strMsg, _T("Generic initialization error.\n\nEnable "
							"debug output for detailed information.") );
		break;
	case D3DFWERR_NODIRECTDRAW:
		lstrcpy( strMsg, _T("No DirectDraw") );
		break;
	case D3DFWERR_INVALIDMODE:
		lstrcpy( strMsg, _T("This sample requires a 16-bit (or higher) "
							"display mode\nto run in a window.\n\nPlease "
							"switch your desktop settings accordingly.") );
		break;
	case D3DFWERR_COULDNTSETCOOPLEVEL:
		lstrcpy( strMsg, _T("Could not set Cooperative Level") );
		break;
	case D3DFWERR_NOPRIMARY:
		lstrcpy( strMsg, _T("No primary") );
		break;
	case D3DFWERR_NOCLIPPER:
		lstrcpy( strMsg, _T("No Clipper") );
		break;
	case D3DFWERR_BADDISPLAYMODE:
		lstrcpy( strMsg, _T("Bad display mode") );
		break;
	case D3DFWERR_NOBACKBUFFER:
		lstrcpy( strMsg, _T("No backbuffer") );
		break;
	case D3DFWERR_NONZEROREFCOUNT:
		lstrcpy( strMsg, _T("A DDraw object has a non-zero reference\n"
							"count (meaning it was not properly cleaned up)." ) );
		break;
	case D3DFWERR_NORENDERTARGET:
		lstrcpy( strMsg, _T("No render target") );
		break;
	case E_OUTOFMEMORY:
		lstrcpy( strMsg, _T("Not enough memory!") );
		break;
	case DDERR_OUTOFVIDEOMEMORY:
		lstrcpy( strMsg, _T("There was insufficient video memory "
							"to use the\nhardware device.") );
		break;
	default:
		lstrcpy( strMsg, _T("Generic application error.\n\nEnable "
							"debug output for detailed information.") );
	}

	if( MSGERR_APPMUSTEXIT == dwType )
	{
		lstrcat( strMsg, _T("\n\nThis sample will now exit.") );
		MessageBox( NULL, strMsg, m_strWindowTitle, MB_ICONERROR|MB_OK );
	}
	else
	{
		if( MSGWARN_SWITCHEDTOSOFTWARE == dwType )
			lstrcat( strMsg, _T("\n\nSwitching to software rasterizer.") );
		MessageBox( NULL, strMsg, m_strWindowTitle, MB_ICONWARNING|MB_OK );
	}
}

//-----------------------------------------------------------------------------
// Name: SetViewParams()
// Desc: Sets the parameters to be used by the SetViewMatrix() function.  You
//       must call this function rather than setting the D3D view matrix
//       yourself in order for stereo modes to work properly.
//-----------------------------------------------------------------------------
VOID CD3DApplication::SetViewParams( D3DVECTOR* vEyePt, D3DVECTOR* vLookatPt,
									 D3DVECTOR* vUpVec, FLOAT fEyeDistance )
{
	// Adjust camera position for left or right eye along the axis
	// perpendicular to the view direction vector and the up vector.
	D3DVECTOR vView = (*vLookatPt) - (*vEyePt);
	vView = CrossProduct( vView, (*vUpVec) );
	vView = Normalize( vView ) * fEyeDistance;

	D3DVECTOR vLeftEyePt  = (*vEyePt) + vView;
	D3DVECTOR vRightEyePt = (*vEyePt) - vView;

	D3DUtil_SetViewMatrix( m_matView,      *vEyePt,     *vLookatPt, *vUpVec );
}


