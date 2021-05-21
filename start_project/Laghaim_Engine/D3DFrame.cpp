#include "stdafx.h"
//-----------------------------------------------------------------------------
// File: D3DFrame.cpp
//
// Desc: Class functions to implement a Direct3D app framework.
//
// Copyright (c) 1995-1999 by Microsoft, all rights reserved
//-----------------------------------------------------------------------------


#include <tchar.h>
#include "D3DFrame.h"
#include "D3DUtil.h"

extern LPDIRECTDRAWSURFACE7 g_pddsBackBuffer;
extern BOOL g_bFFullMode;
extern DWORD	g_dwLangType;				// 0: Korean 1: Japanese  2: English 3:Chinese Taiwan
extern DWORD	g_dwClientCountry;			// 0: Korean 1: Japanese  2: English 3:Chinese Taiwan

//-----------------------------------------------------------------------------
// Name: CD3DFramework7()
// Desc: The constructor. Clears static variables
//-----------------------------------------------------------------------------
CD3DFramework7::CD3DFramework7()
{
	m_hWnd           = NULL;
	m_bIsFullscreen  = TRUE;
	m_bIsStereo      = FALSE;
	m_dwRenderWidth  = 0L;
	m_dwRenderHeight = 0L;

	m_pddsFrontBuffer    = NULL;
	m_pddsBackBuffer     = NULL;
	m_pddsBackBufferLeft = NULL;

	m_pddsZBuffer     = NULL;
	m_pd3dDevice      = NULL;
	m_pDD             = NULL;
	m_pD3D            = NULL;
	m_dwDeviceMemType = NULL;

	// 픽버퍼를 보기 위해서.
	m_bPickView	   = FALSE;
	m_pd3dDevicePick = NULL;			// The D3D pick device
	m_pddsBackBufferPick = NULL;		// The backbuffer surface pick
	m_pddsZBufferPick = NULL;				// The zbuffer surface
}

//-----------------------------------------------------------------------------
// Name: ~CD3DFramework7()
// Desc: The destructor. Deletes all objects
//-----------------------------------------------------------------------------
CD3DFramework7::~CD3DFramework7()
{
	DestroyObjects();
}




//-----------------------------------------------------------------------------
// Name: DestroyObjects()
// Desc: Cleans everything up upon deletion. This code returns an error
//       if any of the objects have remaining reference counts.
//-----------------------------------------------------------------------------
HRESULT CD3DFramework7::DestroyObjects()
{
	LONG nDD  = 0L; // Number of outstanding DDraw references
	LONG nD3D = 0L; // Number of outstanding D3DDevice references

	if( m_pDD )
	{
		m_pDD->SetCooperativeLevel( m_hWnd, DDSCL_NORMAL );
	}

	// Do a safe check for releasing the D3DDEVICE. RefCount must be zero.
	if( m_pd3dDevice )
		if( 0 < ( nD3D = m_pd3dDevice->Release() ) )
			DEBUG_MSG( _T("Error: D3DDevice object is still referenced!") );
	m_pd3dDevice = NULL;

	SAFE_RELEASE( m_pddsBackBuffer );
	SAFE_RELEASE( m_pddsBackBufferLeft );
	SAFE_RELEASE( m_pddsZBuffer );
	SAFE_RELEASE( m_pddsFrontBuffer );

	// Picking device and buffer 해제
	nD3D = 0L;
	if ( m_pd3dDevicePick )
		if ( 0 < (nD3D = m_pd3dDevicePick->Release()) )
			DEBUG_MSG(_T("Error : D3DDevice objce is still referenced") );
	m_pd3dDevicePick = NULL;

	SAFE_RELEASE (m_pddsBackBufferPick);
	SAFE_RELEASE (m_pddsZBufferPick);

	SAFE_RELEASE( m_pD3D );

	if( m_pDD )
	{
		// Do a safe check for releasing DDRAW. RefCount must be zero.
		if( 0 < ( nDD = m_pDD->Release() ) )
		{
			TCHAR err_buf[128];
			_stprintf( err_buf , _T("Error: DDraw object is still referenced!(%d)") , nDD );
			DEBUG_MSG( err_buf );
		}
		//nDD = 0;
		//m_pDD->Release();
	}
	m_pDD = NULL;

	// Return successful, unless there are outstanding DD or D3DDevice refs.
	return ( nDD==0 && nD3D==0 ) ? S_OK : D3DFWERR_NONZEROREFCOUNT;
}


//-----------------------------------------------------------------------------
// Name: Initialize()
// Desc: Creates the internal objects for the framework
//-----------------------------------------------------------------------------
HRESULT CD3DFramework7::Initialize( HWND hWnd, GUID* pDriverGUID,
									GUID* pDeviceGUID, DDSURFACEDESC2* pMode,
									DWORD dwFlags )
{
	HRESULT hr;

	// Check params. Note: A NULL mode is valid for windowed modes only.
	if( ( NULL==hWnd ) || ( NULL==pDeviceGUID ) ||
			( NULL==pMode && (dwFlags&D3DFW_FULLSCREEN) ) )
		return E_INVALIDARG;

	// Setup state for windowed/fullscreen mode
	m_hWnd          = hWnd;
	m_bIsFullscreen = ( dwFlags & D3DFW_FULLSCREEN ) ? TRUE : FALSE;

	// Create the D3D rendering environment (surfaces, device, viewport, and so forth.)
	if( FAILED( hr = CreateEnvironment( pDriverGUID, pDeviceGUID, pMode,
										dwFlags ) ) )
	{
		DestroyObjects();
		return hr;
	}

	return S_OK;
}



//-----------------------------------------------------------------------------
// Name: CreateEnvironment()
// Desc: Creates the internal objects for the framework
//-----------------------------------------------------------------------------
HRESULT CD3DFramework7::CreateEnvironment( GUID* pDriverGUID, GUID* pDeviceGUID,
		DDSURFACEDESC2* pMode, DWORD dwFlags )
{
	HRESULT hr;

	// Select the default memory type, for whether the device is HW or SW
	if( IsEqualIID( *pDeviceGUID, IID_IDirect3DHALDevice) )
		m_dwDeviceMemType = DDSCAPS_VIDEOMEMORY;
	else if( IsEqualIID( *pDeviceGUID, IID_IDirect3DTnLHalDevice) )
		m_dwDeviceMemType = DDSCAPS_VIDEOMEMORY;
	else
		m_dwDeviceMemType = DDSCAPS_SYSTEMMEMORY;

	// Create the DDraw object
	hr = CreateDirectDraw( pDriverGUID, dwFlags );
	if( FAILED( hr ) )
		return hr;

	// Create the front and back buffers, and attach a clipper
	if( dwFlags & D3DFW_FULLSCREEN )
		hr = CreateFullscreenBuffers( pMode );
	else
		hr = CreateWindowedBuffers( pMode );
	if( FAILED( hr ) )
		return hr;

	// Create the Direct3D object and the Direct3DDevice object
	hr = CreateDirect3D( pDeviceGUID );
	if( FAILED( hr ) )
		return hr;

	// Create and attach the zbuffer
	if( dwFlags & D3DFW_ZBUFFER )
		hr = CreateZBuffer( pDeviceGUID );
	if( FAILED( hr ) )
		return hr;



	return S_OK;
}


//-----------------------------------------------------------------------------
// Name: EnumZBufferFormatsCallback()
// Desc: Simply returns the first matching enumerated z-buffer format
//-----------------------------------------------------------------------------
static HRESULT WINAPI EnumZBufferFormatsCallback( DDPIXELFORMAT* pddpf,
		VOID* pContext )
{
	DDPIXELFORMAT* pddpfOut = (DDPIXELFORMAT*)pContext;

	if( pddpfOut->dwRGBBitCount == pddpf->dwRGBBitCount )
	{
		(*pddpfOut) = (*pddpf);
		return D3DENUMRET_CANCEL;
	}

	return D3DENUMRET_OK;
}



//-----------------------------------------------------------------------------
// Name: CreateDirectDraw()
// Desc: Create the DirectDraw interface
//-----------------------------------------------------------------------------
HRESULT CD3DFramework7::CreateDirectDraw( GUID* pDriverGUID, DWORD dwFlags )
{
	// Create the DirectDraw interface
	if( FAILED( DirectDrawCreateEx( pDriverGUID, (VOID**)&m_pDD,
									IID_IDirectDraw7, NULL ) ) )
	{
		DEBUG_MSG( _T("Could not create DirectDraw") );
		return D3DFWERR_NODIRECTDRAW;
	}

	// Set the Windows cooperative level
	DWORD dwCoopFlags = DDSCL_NORMAL;
	if( m_bIsFullscreen )
		dwCoopFlags = DDSCL_ALLOWREBOOT|DDSCL_EXCLUSIVE|DDSCL_FULLSCREEN;
	//if( m_bIsFullscreen )
	//    dwCoopFlags = DDSCL_NORMAL;

	// By defualt, set the flag to allow D3D to optimize floating point calcs
	if( 0L == ( dwFlags & D3DFW_NO_FPUSETUP ) )
		dwCoopFlags |= DDSCL_FPUSETUP;

	if( FAILED( m_pDD->SetCooperativeLevel( m_hWnd, dwCoopFlags ) ) )
	{
		DEBUG_MSG( _T("Couldn't set coop level") );
		return D3DFWERR_COULDNTSETCOOPLEVEL;
	}

	// Check that we are NOT in a palettized display. That case will fail,
	// since the Direct3D framework doesn't use palettes.
	DDSURFACEDESC2 ddsd;
	ddsd.dwSize = sizeof(ddsd);
	m_pDD->GetDisplayMode( &ddsd );
	if( ddsd.ddpfPixelFormat.dwRGBBitCount <= 8 )
		return D3DFWERR_INVALIDMODE;

	return S_OK;
}






//-----------------------------------------------------------------------------
// Name: CreateFullscreenBuffers()
// Desc: Creates the primary and (optional) backbuffer for rendering.
//       Windowed mode and fullscreen mode are handled differently.
//-----------------------------------------------------------------------------
HRESULT CD3DFramework7::CreateFullscreenBuffers( DDSURFACEDESC2* pddsd )
{
	HRESULT hr;

	// Get the dimensions of the screen bounds
	// Store the rectangle which contains the renderer
	SetRect( &m_rcScreenRect, 0, 0, pddsd->dwWidth, pddsd->dwHeight );
	m_dwRenderWidth  = m_rcScreenRect.right  - m_rcScreenRect.left;
	m_dwRenderHeight = m_rcScreenRect.bottom - m_rcScreenRect.top;

	// Set the display mode to the requested dimensions. Check for
	// 320x200x8 modes, and set flag to avoid using ModeX
	DWORD dwModeFlags = 0;

	if( (320==m_dwRenderWidth) && (200==m_dwRenderHeight) &&
			(8==pddsd->ddpfPixelFormat.dwRGBBitCount) )
		dwModeFlags |= DDSDM_STANDARDVGAMODE;

	if( FAILED( m_pDD->SetDisplayMode( m_dwRenderWidth, m_dwRenderHeight,
									   pddsd->ddpfPixelFormat.dwRGBBitCount,
									   pddsd->dwRefreshRate, dwModeFlags ) ) )
	{
		DEBUG_MSG( _T("Can't set display mode") );
		return D3DFWERR_BADDISPLAYMODE;
	}

	// Setup to create the primary surface w/backbuffer
	DDSURFACEDESC2 ddsd;
	ZeroMemory( &ddsd, sizeof(ddsd) );
	ddsd.dwSize            = sizeof(ddsd);
	ddsd.dwFlags           = DDSD_CAPS|DDSD_BACKBUFFERCOUNT;
	ddsd.ddsCaps.dwCaps    = DDSCAPS_PRIMARYSURFACE | DDSCAPS_3DDEVICE |
							 DDSCAPS_FLIP | DDSCAPS_COMPLEX;
	ddsd.dwBackBufferCount = 1;

	// Support for stereoscopic viewing
	if( m_bIsStereo )
	{
		ddsd.ddsCaps.dwCaps  |= DDSCAPS_VIDEOMEMORY;
		ddsd.ddsCaps.dwCaps2 |= DDSCAPS2_STEREOSURFACELEFT;
	}

	// Create the primary surface
	if( FAILED( hr = m_pDD->CreateSurface( &ddsd, &m_pddsFrontBuffer, NULL ) ) )
	{
		DEBUG_MSG( _T("Error: Can't create primary surface") );
		if( hr != DDERR_OUTOFVIDEOMEMORY )
			return D3DFWERR_NOPRIMARY;
		DEBUG_MSG( _T("Error: Out of video memory") );
		return DDERR_OUTOFVIDEOMEMORY;
	}

	// Get the backbuffer, which was created along with the primary.
	DDSCAPS2 ddscaps = { DDSCAPS_BACKBUFFER, 0, 0, 0 };
	if( FAILED( hr = m_pddsFrontBuffer->GetAttachedSurface( &ddscaps,
					 &m_pddsBackBuffer ) ) )
	{
		DEBUG_ERR( hr, _T("Error: Can't get the backbuffer") );
		return D3DFWERR_NOBACKBUFFER;
	}

	LPDIRECTDRAWCLIPPER		pClipper = NULL;

	// Try to get a clipper is already exist
	m_pddsFrontBuffer->GetClipper( &pClipper );

	// create a new one if not exist
	if( pClipper == NULL ) m_pDD->CreateClipper( 0, &pClipper, NULL ) ;

	// If still fail ... exit
	if( pClipper  )
	{
		// assign clipper to  Main Window
		pClipper->SetHWnd( 0, m_hWnd );

		m_pddsFrontBuffer->SetClipper( pClipper );

		// The Primary surface have add a ref , no need to keep this
		pClipper->Release();
	}

	// Increment the backbuffer count (for consistency with windowed mode)
	m_pddsBackBuffer->AddRef();

	// Support for stereoscopic viewing
	if( m_bIsStereo )
	{
		// Get the left back buffer, which was created along with the primary.
		DDSCAPS2 ddscaps = { 0, DDSCAPS2_STEREOSURFACELEFT, 0, 0 };
		if( FAILED( hr = m_pddsBackBuffer->GetAttachedSurface( &ddscaps,
						 &m_pddsBackBufferLeft ) ) )
		{
			DEBUG_ERR( hr, _T("Error: Can't get the left back buffer") );
			return D3DFWERR_NOBACKBUFFER;
		}
		m_pddsBackBufferLeft->AddRef();
	}

	// 픽버퍼 생성
	// 픽버퍼용 백버퍼 생성 : backbuffer의 1/2 크기로 생성

	ddsd.dwFlags        = DDSD_WIDTH | DDSD_HEIGHT | DDSD_CAPS;
	ddsd.dwWidth        = m_dwRenderWidth / 2;
	ddsd.dwHeight       = m_dwRenderHeight / 2;
	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_3DDEVICE;

	if( FAILED( hr = m_pDD->CreateSurface( &ddsd, &m_pddsBackBufferPick, NULL ) ) )
	{
		DEBUG_ERR( hr, _T("Error: Couldn't create the backbufferpick") );
		if( hr != DDERR_OUTOFVIDEOMEMORY )
			return D3DFWERR_NOBACKBUFFER;
		DEBUG_MSG( _T("Error: Out of video memory") );
		return DDERR_OUTOFVIDEOMEMORY;
	}
	//

	/*
	FILE *fplog = fopen("rrlogfile.txt","a");

	ZeroMemory(&m_ddpfBackBufferPixelFormat, sizeof(DDPIXELFORMAT));
	m_ddpfBackBufferPixelFormat.dwSize = sizeof(DDPIXELFORMAT);
	hr = m_pddsBackBuffer->GetPixelFormat(&m_ddpfBackBufferPixelFormat);

	if(hr == DD_OK)
	{
		fprintf( fplog, "Backbuffer Pixelformat RGB bits  = : ");
		fprintf( fplog, "%d\n",m_ddpfBackBufferPixelFormat.dwRGBBitCount);

		fprintf( fplog, "Backbuffer Pixelformat RGB masks = : ");
		fprintf( fplog, "%d %d %d\n\n",
		m_ddpfBackBufferPixelFormat.dwRBitMask,
		m_ddpfBackBufferPixelFormat.dwGBitMask,
		m_ddpfBackBufferPixelFormat.dwBBitMask);
	}
	else
		fprintf( fplog, "Check on backbuffer pixel format FAILED ");
	*/


	hr = m_pddsBackBuffer->GetSurfaceDesc( &ddsd );

	//fclose(fplog);

	return S_OK;
}


//-----------------------------------------------------------------------------
// Name: CreateWindowedBuffers()
// Desc: Creates the primary and (optional) backbuffer for rendering.
//       Windowed mode and fullscreen mode are handled differently.
//-----------------------------------------------------------------------------
HRESULT CD3DFramework7::CreateWindowedBuffers(  DDSURFACEDESC2* pddsd )
{
	HRESULT hr;


	// Get the dimensions of the viewport and screen bounds
	GetClientRect( m_hWnd, &m_rcScreenRect );
	ClientToScreen( m_hWnd, (POINT*)&m_rcScreenRect.left );
	ClientToScreen( m_hWnd, (POINT*)&m_rcScreenRect.right );
	m_dwRenderWidth  = m_rcScreenRect.right  - m_rcScreenRect.left;
	m_dwRenderHeight = m_rcScreenRect.bottom - m_rcScreenRect.top;

	// 임시 테스트
	// Set the display mode to the requested dimensions. Check for
	// 320x200x8 modes, and set flag to avoid using ModeX
	if (g_bFFullMode)
	{
		DWORD dwModeFlags = 0;

		if( (320==m_dwRenderWidth) && (200==m_dwRenderHeight) &&
				(8==pddsd->ddpfPixelFormat.dwRGBBitCount) )
			dwModeFlags |= DDSDM_STANDARDVGAMODE;

		if( FAILED( m_pDD->SetDisplayMode( m_dwRenderWidth, m_dwRenderHeight,
										   //16,
										   pddsd->ddpfPixelFormat.dwRGBBitCount,
										   pddsd->dwRefreshRate, dwModeFlags ) ) )
		{
			DEBUG_MSG( _T("Can't set display mode") );
			return D3DFWERR_BADDISPLAYMODE;
		}
	}
	// 임시테스트 끝.

	// Create the primary surface
	DDSURFACEDESC2 ddsd;
	ZeroMemory( &ddsd, sizeof(ddsd) );
	ddsd.dwSize         = sizeof(ddsd);
	ddsd.dwFlags        = DDSD_CAPS;
	ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | DDSCAPS_3DDEVICE | m_dwDeviceMemType;

	if( FAILED( hr = m_pDD->CreateSurface( &ddsd, &m_pddsFrontBuffer, NULL ) ) )
	{
		DEBUG_MSG( _T("Error: Can't create primary surface") );
		if( hr != DDERR_OUTOFVIDEOMEMORY )
			return D3DFWERR_NOPRIMARY;
		DEBUG_MSG( _T("Error: Out of video memory") );
		return DDERR_OUTOFVIDEOMEMORY;
	}

	// If in windowed-mode, create a clipper object
	LPDIRECTDRAWCLIPPER pcClipper;
	if( FAILED( hr = m_pDD->CreateClipper( 0, &pcClipper, NULL ) ) )
	{
		DEBUG_MSG( _T("Error: Couldn't create clipper") );
		return D3DFWERR_NOCLIPPER;
	}

	// Associate the clipper with the window
	pcClipper->SetHWnd( 0, m_hWnd );
	m_pddsFrontBuffer->SetClipper( pcClipper );
	SAFE_RELEASE( pcClipper );

	// Create a backbuffer
	ddsd.dwFlags        = DDSD_WIDTH | DDSD_HEIGHT | DDSD_CAPS;
	ddsd.dwWidth        = m_dwRenderWidth;
	ddsd.dwHeight       = m_dwRenderHeight;
	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_3DDEVICE | m_dwDeviceMemType;

	if( FAILED( hr = m_pDD->CreateSurface( &ddsd, &m_pddsBackBuffer, NULL ) ) )
	{
		DEBUG_ERR( hr, _T("Error: Couldn't create the backbuffer") );
		if( hr != DDERR_OUTOFVIDEOMEMORY )
			return D3DFWERR_NOBACKBUFFER;
		DEBUG_MSG( _T("Error: Out of video memory") );
		return DDERR_OUTOFVIDEOMEMORY;
	}

	// 픽버퍼용 백버퍼 생성
	// 픽버퍼용 백버퍼 생성 : backbuffer의 1/2 크기로 생성
	ZeroMemory( &ddsd, sizeof(ddsd) );
	ddsd.dwSize			= sizeof(ddsd);
	ddsd.dwFlags        = DDSD_WIDTH | DDSD_HEIGHT | DDSD_CAPS;
	ddsd.dwWidth        = m_dwRenderWidth / 2;
	ddsd.dwHeight       = m_dwRenderHeight / 2;
	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_3DDEVICE | m_dwDeviceMemType;

	if( FAILED( hr = m_pDD->CreateSurface( &ddsd, &m_pddsBackBufferPick, NULL ) ) )
	{
		DEBUG_ERR( hr, _T("Error: Couldn't create the backbufferpick") );
		if( hr != DDERR_OUTOFVIDEOMEMORY )
			return D3DFWERR_NOBACKBUFFER;
		DEBUG_MSG( _T("Error: Out of video memory") );
		return DDERR_OUTOFVIDEOMEMORY;
	}

	ZeroMemory(&m_ddpfBackBufferPixelFormat, sizeof(DDPIXELFORMAT));
	m_ddpfBackBufferPixelFormat.dwSize = sizeof(DDPIXELFORMAT);
	hr = m_pddsBackBuffer->GetPixelFormat(&m_ddpfBackBufferPixelFormat);
	hr = m_pddsBackBuffer->GetSurfaceDesc( &ddsd );

	return S_OK;
}






//-----------------------------------------------------------------------------
// Name: CreateDirect3D()
// Desc: Create the Direct3D interface
//-----------------------------------------------------------------------------
HRESULT CD3DFramework7::CreateDirect3D( GUID* pDeviceGUID )
{
	// Query DirectDraw for access to Direct3D.
	if( FAILED( m_pDD->QueryInterface( IID_IDirect3D7, (VOID**)&m_pD3D ) ) )
	{
		DEBUG_MSG( _T("Couldn't get the Direct3D interface") );
		return D3DFWERR_NODIRECT3D;
	}

	// Create the device
	if( FAILED( m_pD3D->CreateDevice( *pDeviceGUID, m_pddsBackBuffer,
									  &m_pd3dDevice) ) )
	{
		DEBUG_MSG( _T("Couldn't create the D3DDevice") );
		return D3DFWERR_NO3DDEVICE;
	}

	// 픽용 디바이스 생성
	if( FAILED( m_pD3D->CreateDevice( *pDeviceGUID, m_pddsBackBufferPick,
									  &m_pd3dDevicePick) ) )
	{
		DEBUG_MSG( _T("Couldn't create the D3DDevice Pick") );
		return D3DFWERR_NO3DDEVICE;
	}

	// Finally, set the viewport for the newly created device
	D3DVIEWPORT7 vp = { 0, 0, m_dwRenderWidth, m_dwRenderHeight, 0.0f, 1.0f };

	if( FAILED( m_pd3dDevice->SetViewport( &vp ) ) )
	{
		DEBUG_MSG( _T("Error: Couldn't set current viewport to device") );
		return D3DFWERR_NOVIEWPORT;
	}

	// 픽용 설정
	vp.dwWidth = m_dwRenderWidth / 2;
	vp.dwHeight = m_dwRenderHeight / 2;
	if( FAILED( m_pd3dDevicePick->SetViewport( &vp ) ) )
	{
		DEBUG_MSG( _T("Error: Couldn't set current viewport to device") );
		return D3DFWERR_NOVIEWPORT;
	}

	return S_OK;
}


//-----------------------------------------------------------------------------
// Name: CreateZBuffer()
// Desc: Internal function called by Create() to make and attach a zbuffer
//       to the renderer
//-----------------------------------------------------------------------------
HRESULT CD3DFramework7::CreateZBuffer( GUID* pDeviceGUID )
{
	HRESULT hr;

	// Check if the device supports z-bufferless hidden surface removal. If so,
	// we don't really need a z-buffer
	D3DDEVICEDESC7 ddDesc;
	m_pd3dDevice->GetCaps( &ddDesc );
	if( ddDesc.dpcTriCaps.dwRasterCaps & D3DPRASTERCAPS_ZBUFFERLESSHSR )
		return S_OK;

	// Get z-buffer dimensions from the render target
	DDSURFACEDESC2 ddsd;
	ddsd.dwSize = sizeof(ddsd);
	m_pddsBackBuffer->GetSurfaceDesc( &ddsd );

	// Setup the surface desc for the z-buffer.
	ddsd.dwFlags        = DDSD_WIDTH | DDSD_HEIGHT | DDSD_CAPS | DDSD_PIXELFORMAT;
	ddsd.ddsCaps.dwCaps = DDSCAPS_ZBUFFER | m_dwDeviceMemType;
	ddsd.ddpfPixelFormat.dwSize = 0;  // Tag the pixel format as unitialized.

	// Get an appropiate pixel format from enumeration of the formats. On the
	// first pass, we look for a zbuffer dpeth which is equal to the frame
	// buffer depth (as some cards unfornately require this).
	m_pD3D->EnumZBufferFormats( *pDeviceGUID, EnumZBufferFormatsCallback,
								(VOID*)&ddsd.ddpfPixelFormat );
	if( 0 == ddsd.ddpfPixelFormat.dwSize )
	{
		// Try again, just accepting any 16-bit zbuffer
		ddsd.ddpfPixelFormat.dwRGBBitCount = 16;
		m_pD3D->EnumZBufferFormats( *pDeviceGUID, EnumZBufferFormatsCallback,
									(VOID*)&ddsd.ddpfPixelFormat );

		if( 0 == ddsd.ddpfPixelFormat.dwSize )
		{
			DEBUG_MSG( _T("Device doesn't support requested zbuffer format") );
			return D3DFWERR_NOZBUFFER;
		}
	}

	// Create and attach a z-buffer
	if( FAILED( hr = m_pDD->CreateSurface( &ddsd, &m_pddsZBuffer, NULL ) ) )
	{
		DEBUG_MSG( _T("Error: Couldn't create a ZBuffer surface") );
		if( hr != DDERR_OUTOFVIDEOMEMORY )
			return D3DFWERR_NOZBUFFER;
		DEBUG_MSG( _T("Error: Out of video memory") );
		return DDERR_OUTOFVIDEOMEMORY;
	}

	if( FAILED( m_pddsBackBuffer->AddAttachedSurface( m_pddsZBuffer ) ) )
	{
		DEBUG_MSG( _T("Error: Couldn't attach zbuffer to render surface") );
		return D3DFWERR_NOZBUFFER;
	}

	//////////////////////////////////////////////////////////////////////////////
	// z-buffer pick의 생성과 연결.
	// 여기서도 z-buffer의 크기를 1/2로 생성을 해야 한다.
	ddsd.dwSize = sizeof(ddsd);
	m_pddsBackBufferPick->GetSurfaceDesc( &ddsd );

	// Setup the surface desc for the z-buffer.
	ddsd.dwFlags        = DDSD_WIDTH | DDSD_HEIGHT | DDSD_CAPS | DDSD_PIXELFORMAT;
	ddsd.ddsCaps.dwCaps = DDSCAPS_ZBUFFER | m_dwDeviceMemType;
	ddsd.ddpfPixelFormat.dwSize = 0;  // Tag the pixel format as unitialized.

	m_pD3D->EnumZBufferFormats( *pDeviceGUID, EnumZBufferFormatsCallback,
								(VOID*)&ddsd.ddpfPixelFormat );
	if( 0 == ddsd.ddpfPixelFormat.dwSize )
	{
		// Try again, just accepting any 16-bit zbuffer
		ddsd.ddpfPixelFormat.dwRGBBitCount = 16;
		m_pD3D->EnumZBufferFormats( *pDeviceGUID, EnumZBufferFormatsCallback,
									(VOID*)&ddsd.ddpfPixelFormat );

		if( 0 == ddsd.ddpfPixelFormat.dwSize )
		{
			DEBUG_MSG( _T("Device doesn't support requested zbuffer format") );
			return D3DFWERR_NOZBUFFER;
		}
	}

	if( FAILED( hr = m_pDD->CreateSurface( &ddsd, &m_pddsZBufferPick, NULL ) ) )
	{
		DEBUG_MSG( _T("Error: Couldn't create a ZBuffer pick surface") );
		if( hr != DDERR_OUTOFVIDEOMEMORY )
			return D3DFWERR_NOZBUFFER;
		DEBUG_MSG( _T("Error: Out of video memory") );
		return DDERR_OUTOFVIDEOMEMORY;
	}

	if( FAILED( m_pddsBackBufferPick->AddAttachedSurface( m_pddsZBufferPick ) ) )
	{
		DEBUG_MSG( _T("Error: Couldn't attach zbuffer to render surface") );
		return D3DFWERR_NOZBUFFER;
	}
	// z-buffer pick 끝
	////////////////////

	// For stereoscopic viewing, attach zbuffer to left surface as well
	if( m_bIsStereo )
	{
		if( FAILED( m_pddsBackBufferLeft->AddAttachedSurface( m_pddsZBuffer ) ) )
		{
			DEBUG_MSG( _T("Error: Couldn't attach zbuffer to left render surface") );
			return D3DFWERR_NOZBUFFER;
		}
	}

	// Finally, this call rebuilds internal structures
	if( FAILED( m_pd3dDevice->SetRenderTarget( m_pddsBackBuffer, 0L ) ) )
	{
		DEBUG_MSG( _T("Error: SetRenderTarget() failed after attaching zbuffer!") );
		return D3DFWERR_NOZBUFFER;
	}

	if( FAILED( m_pd3dDevicePick->SetRenderTarget( m_pddsBackBufferPick, 0L ) ) )
	{
		DEBUG_MSG( _T("Error: SetRenderTarget() failed after attaching zbuffer!") );
		return D3DFWERR_NOZBUFFER;
	}

	return S_OK;
}



//-----------------------------------------------------------------------------
// Name: RestoreSurfaces()
// Desc: Checks for lost surfaces and restores them if lost. Note: Don't
//       restore render surface, since it's just a duplicate ptr.
//-----------------------------------------------------------------------------
HRESULT CD3DFramework7::RestoreSurfaces()
{
	// Restore all surfaces (including video memory vertex buffers)
	m_pDD->RestoreAllSurfaces();

	return S_OK;
}



//-----------------------------------------------------------------------------
// Name: Move()
// Desc: Moves the screen rect for windowed renderers
//-----------------------------------------------------------------------------
VOID CD3DFramework7::Move( INT x, INT y )
{
	if( TRUE == m_bIsFullscreen )
		return;

	SetRect( &m_rcScreenRect, x, y, x + m_dwRenderWidth, y + m_dwRenderHeight );
}




//-----------------------------------------------------------------------------
// Name: FlipToGDISurface()
// Desc: Puts the GDI surface in front of the primary, so that dialog
//       boxes and other windows drawing funcs may happen.
//-----------------------------------------------------------------------------
HRESULT CD3DFramework7::FlipToGDISurface( BOOL bDrawFrame )
{
	if( m_pDD && m_bIsFullscreen )
	{
		m_pDD->FlipToGDISurface();

		if( bDrawFrame )
		{
			DrawMenuBar( m_hWnd );
			RedrawWindow( m_hWnd, NULL, NULL, RDW_FRAME );
		}
	}

	return S_OK;
}




//-----------------------------------------------------------------------------
// Name: ShowFrame()
// Desc: Show the frame on the primary surface, via a blt or a flip.
//-----------------------------------------------------------------------------
HRESULT CD3DFramework7::ShowFrame()
{
	RECT rcVp = {0, 0, 0, 0};
	D3DVIEWPORT7 vp;

	if (m_pd3dDevicePick && m_bPickView)
	{
		m_pd3dDevicePick->GetViewport(&vp);
		rcVp.left = 0;
		rcVp.top = 0;
		rcVp.right = vp.dwWidth;
		rcVp.bottom = vp.dwHeight;
	}

	if( NULL == m_pddsFrontBuffer )
		return D3DFWERR_NOTINITIALIZED;

	if (m_bPickView)
	{
		m_pddsBackBuffer->BltFast (0, 0, m_pddsBackBufferPick, &rcVp, DDBLTFAST_NOCOLORKEY );
		return m_pddsFrontBuffer->Blt( &m_rcScreenRect, m_pddsBackBuffer, NULL, DDBLT_WAIT, NULL );
	}
	else
	{
		return m_pddsFrontBuffer->Blt( &m_rcScreenRect, m_pddsBackBuffer, NULL, DDBLT_WAIT, NULL );
	}
}



