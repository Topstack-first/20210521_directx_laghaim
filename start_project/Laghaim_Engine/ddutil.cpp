#include "stdafx.h"
//-----------------------------------------------------------------------------
// File: ddutil.cpp
//
// Desc: DirectDraw framewark classes. Feel free to use this class as a
//       starting point for adding extra functionality.
//
//
// Copyright (c) 1995-1999 Microsoft Corporation. All rights reserved.
//-----------------------------------------------------------------------------

#include <tchar.h>
#include <windowsx.h>
#include <ddraw.h>
#include "ddutil.h"
#include "dxutil.h"
#include <fcntl.h>
#include <io.h>

#include <ocidl.h>
#include <olectl.h>
#include "Def.h"

CDisplay * g_pDisplay = NULL;

extern void LogBugTrap(std::string strLog);

// [12/12/2008 D.K ] : test용 bitmap 함수.
HBITMAP LoadBitmapFile( char* szFileName )
{
	LPVOID lpBits;
	PFILE* fp = pfopen( szFileName, "rb" );

	if( !fp )
		return NULL;

	BITMAPFILEHEADER bmfHeader;

	// Read file header
	pfread( (LPSTR)&bmfHeader, sizeof( BITMAPFILEHEADER  ), 1, fp );

	// File type should be 'BM'
	if (bmfHeader.bfType != ((WORD) ('M' << 8) | 'B'))
	{
		pfclose( fp );
		return NULL;
	}

	DWORD dwInfoSize = bmfHeader.bfOffBits - sizeof( BITMAPFILEHEADER  );

	BITMAPINFO *pbmInfo;

	pbmInfo = (BITMAPINFO *) new BYTE[dwInfoSize];

	if (pbmInfo == NULL)
	{
		pfclose( fp );
		return NULL;
	}

	pfread( pbmInfo, dwInfoSize , 1, fp );

	BITMAPINFO &bmInfo = *pbmInfo ;
	HBITMAP hBitmap = NULL;

	DWORD dwSize = bmfHeader.bfSize - bmfHeader.bfOffBits;

	hBitmap = CreateDIBSection( NULL, pbmInfo, DIB_RGB_COLORS, &lpBits, NULL, 0 );

	if( hBitmap == NULL )
	{
		LPVOID lpMsgBuf;

		FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
			NULL, GetLastError(),
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
			(LPTSTR) &lpMsgBuf, 0, NULL
		);

		LogBugTrap((const char*)lpMsgBuf);

		LocalFree( lpMsgBuf );
	}

	if( lpBits != NULL )
	{
		pfread(lpBits, dwSize-2, 1, fp);
	}

	delete [] pbmInfo;

	// release buffer.
	pfclose( fp );

	return hBitmap;
}

//-----------------------------------------------------------------------------
// Name: CDisplay()
// Desc:
//-----------------------------------------------------------------------------
CDisplay::CDisplay()
{
	m_pDD                = NULL;
	m_pddsFrontBuffer    = NULL;
	m_pddsBackBuffer     = NULL;
	m_pddsBackBufferLeft = NULL;
}




//-----------------------------------------------------------------------------
// Name: ~CDisplay()
// Desc:
//-----------------------------------------------------------------------------
CDisplay::~CDisplay()
{
	ClearAll();
	DestroyObjects();
}




//-----------------------------------------------------------------------------
// Name: DestroyObjects()
// Desc:
//-----------------------------------------------------------------------------
HRESULT CDisplay::DestroyObjects()
{
	SAFE_RELEASE( m_pddsBackBufferLeft );
	SAFE_RELEASE( m_pddsFrontBuffer );

	m_pddsBackBuffer = NULL;
	m_pDD = NULL;

	return S_OK;
}




//-----------------------------------------------------------------------------
// Name: CreateFullScreenDisplay()
// Desc:
//-----------------------------------------------------------------------------
HRESULT CDisplay::CreateFullScreenDisplay( HWND hWnd, DWORD dwWidth,
		DWORD dwHeight, DWORD dwBPP )
{
	// by evilkiki 2009.12.03 사용안하는 함수
	return S_OK;

	HRESULT hr;

	// Cleanup anything from a previous call
	DestroyObjects();

	// DDraw stuff begins here
	if( FAILED( hr = DirectDrawCreateEx( NULL, (VOID**)&m_pDD,
										 IID_IDirectDraw7, NULL ) ) )
		return E_FAIL;

	// Set cooperative level
	hr = m_pDD->SetCooperativeLevel( hWnd, DDSCL_EXCLUSIVE|DDSCL_FULLSCREEN );
	if( FAILED(hr) )
		return E_FAIL;

	// Set the display mode
	if( FAILED( m_pDD->SetDisplayMode( dwWidth, dwHeight, dwBPP, 0, 0 ) ) )
		return E_FAIL;

	// Create primary surface (with backbuffer attached)
	DDSURFACEDESC2 ddsd;
	ZeroMemory( &ddsd, sizeof( ddsd ) );
	ddsd.dwSize            = sizeof( ddsd );
	ddsd.dwFlags           = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
	ddsd.ddsCaps.dwCaps    = DDSCAPS_PRIMARYSURFACE | DDSCAPS_FLIP |
							 DDSCAPS_COMPLEX | DDSCAPS_3DDEVICE;
	ddsd.dwBackBufferCount = 1;

	if( FAILED( hr = m_pDD->CreateSurface( &ddsd, &m_pddsFrontBuffer,
										   NULL ) ) )
		return E_FAIL;

	// Get a pointer to the back buffer
	DDSCAPS2 ddscaps;
	ZeroMemory( &ddscaps, sizeof( ddscaps ) );
	ddscaps.dwCaps = DDSCAPS_BACKBUFFER;

	if( FAILED( hr = m_pddsFrontBuffer->GetAttachedSurface( &ddscaps,
					 &m_pddsBackBuffer ) ) )
		return E_FAIL;

	m_pddsBackBuffer->AddRef();

	m_hWnd      = hWnd;
	m_bWindowed = FALSE;
	UpdateBounds();

	return S_OK;
}




//-----------------------------------------------------------------------------
// Name: CreateWindowedDisplay()
// Desc:
//-----------------------------------------------------------------------------
HRESULT CDisplay::CreateWindowedDisplay( HWND hWnd, DWORD dwWidth, DWORD dwHeight )
{
	// by evilkiki 2009.12.03 사용안하는 함수
	return S_OK;

	HRESULT hr;

	// Cleanup anything from a previous call
	DestroyObjects();

	// DDraw stuff begins here
	if( FAILED( hr = DirectDrawCreateEx( NULL, (VOID**)&m_pDD,
										 IID_IDirectDraw7, NULL ) ) )
		return E_FAIL;

	// Set cooperative level
	hr = m_pDD->SetCooperativeLevel( hWnd, DDSCL_NORMAL );
	if( FAILED(hr) )
		return E_FAIL;

	RECT  rcWork;
	RECT  rc;
	DWORD dwStyle;

	// If we are still a WS_POPUP window we should convert to a normal app
	// window so we look like a windows app.
	dwStyle  = GetWindowStyle( hWnd );
	dwStyle &= ~WS_POPUP;
	dwStyle |= WS_OVERLAPPED | WS_CAPTION | WS_THICKFRAME | WS_MINIMIZEBOX;
	SetWindowLong( hWnd, GWL_STYLE, dwStyle );

	// Aet window size
	SetRect( &rc, 0, 0, dwWidth, dwHeight );

	AdjustWindowRectEx( &rc, GetWindowStyle(hWnd), GetMenu(hWnd) != NULL,
						GetWindowExStyle(hWnd) );

	SetWindowPos( hWnd, NULL, 0, 0, rc.right-rc.left, rc.bottom-rc.top,
				  SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE );

	SetWindowPos( hWnd, HWND_NOTOPMOST, 0, 0, 0, 0,
				  SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE );

	//  Make sure our window does not hang outside of the work area
	SystemParametersInfo( SPI_GETWORKAREA, 0, &rcWork, 0 );
	GetWindowRect( hWnd, &rc );
	if( rc.left < rcWork.left ) rc.left = rcWork.left;
	if( rc.top  < rcWork.top )  rc.top  = rcWork.top;
	SetWindowPos( hWnd, NULL, rc.left, rc.top, 0, 0,
				  SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE );

	LPDIRECTDRAWCLIPPER pcClipper;

	// Create the primary surface
	DDSURFACEDESC2 ddsd;
	ZeroMemory( &ddsd, sizeof( ddsd ) );
	ddsd.dwSize         = sizeof( ddsd );
	ddsd.dwFlags        = DDSD_CAPS;
	ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;

	if( FAILED( m_pDD->CreateSurface( &ddsd, &m_pddsFrontBuffer, NULL ) ) )
		return E_FAIL;

	// Create the backbuffer surface
	ddsd.dwFlags        = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;
	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_3DDEVICE;
	ddsd.dwWidth        = dwWidth;
	ddsd.dwHeight       = dwHeight;

	if( FAILED( hr = m_pDD->CreateSurface( &ddsd, &m_pddsBackBuffer, NULL ) ) )
		return E_FAIL;

	if( FAILED( hr = m_pDD->CreateClipper( 0, &pcClipper, NULL ) ) )
		return E_FAIL;

	if( FAILED( hr = pcClipper->SetHWnd( 0, hWnd ) ) )
	{
		pcClipper->Release();
		return E_FAIL;
	}

	if( FAILED( hr = m_pddsFrontBuffer->SetClipper( pcClipper ) ) )
	{
		pcClipper->Release();
		return E_FAIL;
	}

	// Done with clipper
	pcClipper->Release();

	m_hWnd      = hWnd;
	m_bWindowed = TRUE;
	UpdateBounds();

	return S_OK;
}




//-----------------------------------------------------------------------------
// Name:
// Desc:
//-----------------------------------------------------------------------------
HRESULT CDisplay::CreateSurface( CSurface** ppSurface,
								 DWORD dwWidth, DWORD dwHeight )
{
	if( NULL == m_pDD )
		return E_POINTER;
	if( NULL == ppSurface )
		return E_INVALIDARG;

	HRESULT        hr;
	DDSURFACEDESC2 ddsd;
	ZeroMemory( &ddsd, sizeof( ddsd ) );
	ddsd.dwSize         = sizeof( ddsd );
	ddsd.dwFlags        = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;
	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
	ddsd.dwWidth        = dwWidth;
	ddsd.dwHeight       = dwHeight;

	(*ppSurface) = new CSurface();
	if( FAILED( hr = (*ppSurface)->Create( m_pDD, &ddsd ) ) )
	{
		delete (*ppSurface);
		*ppSurface = NULL;

		return hr;
	}

	return S_OK;
}




//-----------------------------------------------------------------------------
// Name: CDisplay::CreateSurfaceFromBitmap()
// Desc: Create a DirectDrawSurface from a bitmap resource or bitmap file.
//       Use MAKEINTRESOURCE() to pass a constant into strBMP.
//-----------------------------------------------------------------------------
HRESULT CDisplay::CreateSurfaceFromBitmap( CSurface** ppSurface,
		TCHAR* strBMP,
		DWORD dwDesiredWidth,
		DWORD dwDesiredHeight )
{
	HRESULT        hr;
	HBITMAP        hBMP = NULL;
	BITMAP         bmp;
	DDSURFACEDESC2 ddsd;

	if( m_pDD == NULL || strBMP == NULL || ppSurface == NULL )
		return E_INVALIDARG;

	*ppSurface = NULL;

	//  Try to load the bitmap as a resource, if that fails, try it as a file
	hBMP = (HBITMAP) LoadImage( GetModuleHandle(NULL), strBMP,
								IMAGE_BITMAP, dwDesiredWidth, dwDesiredHeight,
								LR_CREATEDIBSECTION );
	if( hBMP == NULL )
	{
		hBMP = LoadBitmapFile( strBMP );
		
		if( hBMP == NULL )
			return E_FAIL;
	}

	// Get size of the bitmap
	GetObject( hBMP, sizeof(bmp), &bmp );

	// Create a DirectDrawSurface for this bitmap
	ZeroMemory( &ddsd, sizeof(ddsd) );
	ddsd.dwSize         = sizeof(ddsd);
	ddsd.dwFlags        = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;
	//ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
	ddsd.dwWidth        = bmp.bmWidth;
	ddsd.dwHeight       = bmp.bmHeight;

	(*ppSurface) = new CSurface();
	if( FAILED( hr = (*ppSurface)->Create( m_pDD, &ddsd ) ) )
	{
		delete (*ppSurface);
		*ppSurface = NULL;

		return hr;
	}

	// Draw the bitmap on this surface
	if( FAILED( hr = (*ppSurface)->DrawBitmap( hBMP, 0, 0, 0, 0 ) ) )
	{
		DeleteObject( hBMP );
		return hr;
	}

	(*ppSurface)->SetWidth(bmp.bmWidth);
	(*ppSurface)->SetHeight(bmp.bmHeight);

	DeleteObject( hBMP );

	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

// [5/4/2007 Theodoric] JPG load Test
HRESULT CDisplay::CreateSurfaceFromJPG( CSurface** ppSurface, TCHAR* strJPG)//, DWORD dwDesiredWidth, DWORD dwDesiredHeight )
{
	// Use IPicture stuff to use JPG / GIF files
	IPicture* picture = NULL;
	IStream* stream;
	HGLOBAL hGlobal;
	void* pp;
	char* orgBuf = NULL;
	char* buf = NULL;
	long orgBufSize = 0;
	CVirtualFile* pVF = CVirtualFile::GetInstance();
	if( !pVF->OpenFile( strJPG ) )
		return NULL;

	pVF->Read( &orgBuf, orgBufSize );
	buf = orgBuf;
	pVF->CloseFile();

	hGlobal = GlobalAlloc(GPTR, orgBufSize);
	if( !hGlobal )
	{
		SAFE_DELETE_ARRAY( orgBuf );
		return NULL;
	}
	pp = (void*)hGlobal;
	SAFE_READ_BUFFER( pp, buf, 1 * orgBufSize );
	SAFE_DELETE_ARRAY( orgBuf );

	// Create an IStream so IPicture can
	CreateStreamOnHGlobal (hGlobal, false, &stream);

	if( !stream )
	{
		GlobalFree(picture);
		return NULL;
	}

	OleLoadPicture(stream, 0, false, IID_IPicture, (void**)&picture);

	if (!picture)
	{
		stream->Release();
		GlobalFree(hGlobal);
		return NULL;
	}

	stream->Release();
	GlobalFree(hGlobal);

	HBITMAP hBitmap = 0;
	picture->get_Handle((unsigned int*)&hBitmap);

	// Copy the image. Necessary, because upon p's release,
	// the handle is destroyed.
	HBITMAP tempBitmap = (HBITMAP)CopyImage(hBitmap,IMAGE_BITMAP,0,0,LR_COPYRETURNORG);

	picture->Release();
	//return tempBitmap;

	//////////////////////////////////////////////////////////////////////////
	DDSURFACEDESC2 ddsd;
	BITMAP         bmp;

	GetObject( tempBitmap, sizeof(bmp), &bmp );

	// Create a DirectDrawSurface for this bitmap
	ZeroMemory( &ddsd, sizeof(ddsd) );
	ddsd.dwSize         = sizeof(ddsd);
	ddsd.dwFlags        = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;
	//ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
	ddsd.dwWidth        = bmp.bmWidth;
	ddsd.dwHeight       = bmp.bmHeight;
	HRESULT hr;

	(*ppSurface) = new CSurface();
	if( FAILED( hr = (*ppSurface)->Create( m_pDD, &ddsd ) ) )
	{
		delete (*ppSurface);
		*ppSurface = NULL;

		return hr;
	}

	// Draw the bitmap on this surface
	if( FAILED( hr = (*ppSurface)->DrawBitmap( tempBitmap, 0, 0, 0, 0 ) ) )
	{
		DeleteObject( tempBitmap );
		return hr;
	}

	(*ppSurface)->SetWidth(bmp.bmWidth);
	(*ppSurface)->SetHeight(bmp.bmHeight);

	DeleteObject( tempBitmap );

	return S_OK;

	//////////////////////////////////////////////////////////////////////////
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////




//-----------------------------------------------------------------------------
// Name: CDisplay::CreateSurfaceFromText()
// Desc: Creates a DirectDrawSurface from a text string using hFont or the default
//       GDI font if hFont is NULL.
//-----------------------------------------------------------------------------
HRESULT CDisplay::CreateSurfaceFromText( CSurface** ppSurface,
		HFONT hFont, TCHAR* strText,
		COLORREF crBackground, COLORREF crForeground,
		BOOL bShadow)
{
	HDC                  hDC  = NULL;
	LPDIRECTDRAWSURFACE7 pDDS = NULL;
	HRESULT              hr;
	DDSURFACEDESC2       ddsd;
	SIZE                 sizeText;
	HFONT *				 hOldFont;

	if( m_pDD == NULL || strText == NULL || ppSurface == NULL )
		return E_INVALIDARG;

	*ppSurface = NULL;

	hDC = GetDC( NULL );

	if( hFont )
	{
		hOldFont = (HFONT*)SelectObject( hDC, hFont );
	}

	GetTextExtentPoint32( hDC, strText, _tcslen(strText), &sizeText );

	if( hFont )
		SelectObject(hDC , hOldFont);

	ReleaseDC( NULL, hDC );

	// Create a DirectDrawSurface for this bitmap
	ZeroMemory( &ddsd, sizeof(ddsd) );
	ddsd.dwSize         = sizeof(ddsd);
	ddsd.dwFlags        = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
	ddsd.dwWidth        = sizeText.cx;
	ddsd.dwHeight       = sizeText.cy;
	if (bShadow)
	{
		ddsd.dwWidth        += 1;
		ddsd.dwHeight       += 1;
	}

	(*ppSurface) = new CSurface();
	if( FAILED( hr = (*ppSurface)->Create( m_pDD, &ddsd ) ) )
	{
		delete (*ppSurface);
		*ppSurface = NULL;

		return hr;
	}

	(*ppSurface)->SetWidth(ddsd.dwWidth);
	(*ppSurface)->SetHeight(ddsd.dwHeight);

	//-- Lyul 수정 --

	if (bShadow) //-- 아마 배경의 Alpha 가 없는 듯
	{
		if( FAILED( hr = (*ppSurface)->DrawText( hFont, strText, 0, 0,
						 crBackground, RGB(127, 127, 127),0 ) ) )
			return hr;
		if( FAILED( hr = (*ppSurface)->DrawText( hFont, strText, 1, 1,
						 crBackground, crForeground ,0) ) )
			return hr;
	}
	else
	{
		if( FAILED( hr = (*ppSurface)->DrawText( hFont, strText, 0, 0,
						 crBackground, crForeground , 0 ) ) )
			return hr;
	}

	return S_OK;
}




//-----------------------------------------------------------------------------
// Name:
// Desc:
//-----------------------------------------------------------------------------
HRESULT CDisplay::Present()
{
	HRESULT hr;

	if( NULL == m_pddsFrontBuffer && NULL == m_pddsBackBuffer )
		return E_POINTER;

	while( 1 )
	{
		if( m_bWindowed )
			hr = m_pddsFrontBuffer->Blt( &m_rcWindow, m_pddsBackBuffer,
										 NULL, DDBLT_WAIT, NULL );
		else
			hr = m_pddsFrontBuffer->Flip( NULL, 0 );

		if( hr == DDERR_SURFACELOST )
		{
			m_pddsFrontBuffer->Restore();
			m_pddsBackBuffer->Restore();
		}

		if( hr != DDERR_WASSTILLDRAWING )
			return hr;
	}
}




//-----------------------------------------------------------------------------
// Name:
// Desc:
//-----------------------------------------------------------------------------
HRESULT CDisplay::ShowBitmap( HBITMAP hbm, LPDIRECTDRAWPALETTE pPalette )
{
	if( NULL == m_pddsFrontBuffer ||  NULL == m_pddsBackBuffer )
		return E_POINTER;

	// Set the palette before loading the bitmap
	if( pPalette )
		m_pddsFrontBuffer->SetPalette( pPalette );

	CSurface backBuffer;
	backBuffer.Create( m_pddsBackBuffer );

	if( FAILED( backBuffer.DrawBitmap( hbm, 0, 0, 0, 0 ) ) )
		return E_FAIL;

	return Present();
}



HRESULT CDisplay::OpacityBlt( DWORD left, DWORD top, DWORD right, DWORD bottom,
							  LPDIRECTDRAWSURFACE7 pdds, RECT* srcRect, BYTE dwAlpha)
{
	if ( NULL == m_pddsBackBuffer )
		return E_POINTER;

	DDBLTFX  ddbltfx;
	ZeroMemory(&ddbltfx, sizeof(ddbltfx));
	ddbltfx.dwSize = sizeof(ddbltfx);
	//ddbltfx.dwAlphaSrcConstBitDepth = 8;
	ddbltfx.dwAlphaSrcConst = dwAlpha;

	RECT destRect = {left, top, right, bottom};

	if(pdds->IsLost() == DDERR_SURFACELOST)
		pdds->Restore();

	return m_pddsBackBuffer->Blt(&destRect, pdds, srcRect, DDBLT_WAIT | DDBLT_ALPHASRCCONSTOVERRIDE, &ddbltfx);
}

//-----------------------------------------------------------------------------
// Name:
// Desc:
//-----------------------------------------------------------------------------
HRESULT CDisplay::ColorKeyBlt( DWORD x, DWORD y, LPDIRECTDRAWSURFACE7 pdds,
							   RECT* prc )
{
	if( NULL == m_pddsBackBuffer )
		return E_POINTER;

	return m_pddsBackBuffer->BltFast( x, y, pdds, prc, DDBLTFAST_SRCCOLORKEY );
}




//-----------------------------------------------------------------------------
// Name:
// Desc:
//-----------------------------------------------------------------------------
HRESULT CDisplay::Blt( DWORD x, DWORD y, LPDIRECTDRAWSURFACE7 pdds, RECT* prc,
					   DWORD dwFlags )
{
	if( NULL == m_pddsBackBuffer )
		return E_POINTER;

	if(pdds->IsLost() == DDERR_SURFACELOST)
	{
		pdds->Restore();
	}

	return m_pddsBackBuffer->BltFast( x, y, pdds, prc, dwFlags );
}




//-----------------------------------------------------------------------------
// Name:
// Desc:
//-----------------------------------------------------------------------------
HRESULT CDisplay::Blt( DWORD x, DWORD y, CSurface* pSurface, RECT* prc )
{
	if( NULL == pSurface )
		return E_INVALIDARG;

	if( NULL == pSurface->GetDDrawSurface() )
		return E_INVALIDARG;

	if(pSurface->GetDDrawSurface()->IsLost() == DDERR_SURFACELOST)
	{
		pSurface->GetDDrawSurface()->Restore();
	}

	if( pSurface->IsColorKeyed() )
		return Blt( x, y, pSurface->GetDDrawSurface(), prc, DDBLTFAST_SRCCOLORKEY );
	else
		return Blt( x, y, pSurface->GetDDrawSurface(), prc, 0L );
}

// By Sehan !! Mu Chi Da! Sehan!! Jang Ha Da!! Sehan!!!
HRESULT CDisplay::Blt( DWORD left, DWORD top, DWORD right, DWORD bottom,
					   LPDIRECTDRAWSURFACE7 pdds, RECT* srcRect, DWORD dwFlags )
{
	if( NULL == m_pddsBackBuffer )
		return E_POINTER;

	DDBLTFX  ddbltfx;
	ZeroMemory(&ddbltfx, sizeof(ddbltfx));
	ddbltfx.dwSize      = sizeof(ddbltfx);
	ddbltfx.ddckSrcColorkey ;

	RECT destRect;
	destRect.left = left;
	destRect.top = top;
	destRect.right = right;
	destRect.bottom = bottom;

	if(pdds->IsLost() == DDERR_SURFACELOST)
	{
		pdds->Restore();
	}

	return m_pddsBackBuffer->Blt( &destRect, pdds, srcRect, dwFlags | DDBLT_WAIT, &ddbltfx );
}

HRESULT CDisplay::Blt( DWORD left, DWORD top, DWORD right, DWORD bottom,
					   CSurface* pSurface, RECT* srcRect)
{
	if( NULL == pSurface )
		return E_INVALIDARG;

	if( NULL == pSurface->GetDDrawSurface() )
		return E_INVALIDARG;

	if(pSurface->GetDDrawSurface()->IsLost() == DDERR_SURFACELOST)
	{
		pSurface->GetDDrawSurface()->Restore();
	}

	if( pSurface->IsColorKeyed() )
		return Blt( left, top, right, bottom, pSurface->GetDDrawSurface(), srcRect, DDBLT_KEYSRC );
	else
		return Blt( left, top, right, bottom, pSurface->GetDDrawSurface(), srcRect, 0L );
}


//-----------------------------------------------------------------------------
// Name:
// Desc:
//-----------------------------------------------------------------------------
HRESULT CDisplay::Clear( DWORD dwColor )
{
	if( NULL == m_pddsBackBuffer )
		return E_POINTER;

	// Erase the background
	DDBLTFX ddbltfx;
	ZeroMemory( &ddbltfx, sizeof(ddbltfx) );
	ddbltfx.dwSize      = sizeof(ddbltfx);
	ddbltfx.dwFillColor = dwColor;

	return m_pddsBackBuffer->Blt( NULL, NULL, NULL, DDBLT_COLORFILL, &ddbltfx );
}




//-----------------------------------------------------------------------------
// Name:
// Desc:
//-----------------------------------------------------------------------------
HRESULT CDisplay::SetPalette( LPDIRECTDRAWPALETTE pPalette )
{
	if( NULL == m_pddsFrontBuffer )
		return E_POINTER;

	return m_pddsFrontBuffer->SetPalette( pPalette );
}




//-----------------------------------------------------------------------------
// Name:
// Desc:
//-----------------------------------------------------------------------------
HRESULT CDisplay::CreatePaletteFromBitmap( LPDIRECTDRAWPALETTE* ppPalette,
		const TCHAR* strBMP )
{
	HRSRC             hResource      = NULL;
	RGBQUAD*          pRGB           = NULL;
	BITMAPINFOHEADER* pbi = NULL;
	PALETTEENTRY      aPalette[256];
	HANDLE            hFile = NULL;
	DWORD             iColor;
	DWORD             dwColors;
	BITMAPFILEHEADER  bf;
	BITMAPINFOHEADER  bi;
	DWORD             dwBytesRead;

	if( m_pDD == NULL || strBMP == NULL || ppPalette == NULL )
		return E_INVALIDARG;

	*ppPalette = NULL;

	//  Try to load the bitmap as a resource, if that fails, try it as a file
	hResource = FindResource( NULL, strBMP, RT_BITMAP );
	if( hResource )
	{
		pbi = (LPBITMAPINFOHEADER) LockResource( LoadResource( NULL, hResource ) );
		if( NULL == pbi )
			return E_FAIL;

		pRGB = (RGBQUAD*) ( (BYTE*) pbi + pbi->biSize );

		// Figure out how many colors there are
		if( pbi->biSize < sizeof(BITMAPINFOHEADER) )
			dwColors = 0;
		else if( pbi->biBitCount > 8 )
			dwColors = 0;
		else if( pbi->biClrUsed == 0 )
			dwColors = 1 << pbi->biBitCount;
		else
			dwColors = pbi->biClrUsed;

		//  A DIB color table has its colors stored BGR not RGB
		//  so flip them around.
		for( iColor = 0; iColor < dwColors; iColor++ )
		{
			aPalette[iColor].peRed   = pRGB[iColor].rgbRed;
			aPalette[iColor].peGreen = pRGB[iColor].rgbGreen;
			aPalette[iColor].peBlue  = pRGB[iColor].rgbBlue;
			aPalette[iColor].peFlags = 0;
		}

		return m_pDD->CreatePalette( DDPCAPS_8BIT, aPalette, ppPalette, NULL );
	}

	// Attempt to load bitmap as a file
	hFile = CreateFile( strBMP, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL );
	if( NULL == hFile )
		return E_FAIL;

	// Read the BITMAPFILEHEADER
	ReadFile( hFile, &bf, sizeof(bf), &dwBytesRead, NULL );
	if( dwBytesRead != sizeof(bf) )
	{
		CloseHandle( hFile );
		return E_FAIL;
	}

	// Read the BITMAPINFOHEADER
	ReadFile( hFile, &bi, sizeof(bi), &dwBytesRead, NULL );
	if( dwBytesRead != sizeof(bi) )
	{
		CloseHandle( hFile );
		return E_FAIL;
	}

	// Read the PALETTEENTRY
	ReadFile( hFile, aPalette, sizeof(aPalette), &dwBytesRead, NULL );
	if( dwBytesRead != sizeof(aPalette) )
	{
		CloseHandle( hFile );
		return E_FAIL;
	}

	CloseHandle( hFile );

	// Figure out how many colors there are
	if( bi.biSize != sizeof(BITMAPINFOHEADER) )
		dwColors = 0;
	else if (bi.biBitCount > 8)
		dwColors = 0;
	else if (bi.biClrUsed == 0)
		dwColors = 1 << bi.biBitCount;
	else
		dwColors = bi.biClrUsed;

	//  A DIB color table has its colors stored BGR not RGB
	//  so flip them around since DirectDraw uses RGB
	for( iColor = 0; iColor < dwColors; iColor++ )
	{
		BYTE r = aPalette[iColor].peRed;
		aPalette[iColor].peRed  = aPalette[iColor].peBlue;
		aPalette[iColor].peBlue = r;
	}

	return m_pDD->CreatePalette( DDPCAPS_8BIT, aPalette, ppPalette, NULL );
}




//-----------------------------------------------------------------------------
// Name:
// Desc:
//-----------------------------------------------------------------------------
HRESULT CDisplay::UpdateBounds()
{
	if( m_bWindowed )
	{
		GetClientRect( m_hWnd, &m_rcWindow );
		ClientToScreen( m_hWnd, (POINT*)&m_rcWindow );
		ClientToScreen( m_hWnd, (POINT*)&m_rcWindow+1 );
	}
	else
	{
		SetRect( &m_rcWindow, 0, 0, GetSystemMetrics(SM_CXSCREEN),
				 GetSystemMetrics(SM_CYSCREEN) );
	}

	return S_OK;
}





//-----------------------------------------------------------------------------
// Name: CDisplay::InitClipper
// Desc:
//-----------------------------------------------------------------------------
HRESULT CDisplay::InitClipper()
{
	LPDIRECTDRAWCLIPPER pClipper;
	HRESULT hr;

	// Create a clipper when using GDI to draw on the primary surface
	if( FAILED( hr = m_pDD->CreateClipper( 0, &pClipper, NULL ) ) )
		return hr;

	pClipper->SetHWnd( 0, m_hWnd );

	if( FAILED( hr = m_pddsFrontBuffer->SetClipper( pClipper ) ) )
		return hr;

	// We can release the clipper now since g_pDDSPrimary
	// now maintains a ref count on the clipper
	SAFE_RELEASE( pClipper );

	return S_OK;
}


CSurface * CDisplay::GetSurface(CSurface* surface)
{
	// 찾을 방법이 없다?
	return NULL;
}

CSurface * CDisplay::GetSurface(DWORD w, DWORD h)
{
	CSurface * surface = new CSurface(this);

	if( FAILED(surface->CreateSurface(w, h)) )
	{
		SAFE_DELETE(surface);
		return NULL;
	}

	Add(surface);

	return surface;
}

CSurface * CDisplay::GetSurface(TCHAR* filename, BOOL jpg, DWORD w, DWORD h)
{
	CSurface * surface = new CSurface(this);

	if( jpg )
	{
		if( FAILED(surface->CreateSurface(filename)) )
		{
			SAFE_DELETE(surface);
			return NULL;
		}
	}
	else
	{
		if( FAILED(surface->CreateSurface(filename, w, h)) )
		{
			SAFE_DELETE(surface);
			return NULL;
		}
	}

	Add(surface);

	return surface;
}

CSurface * CDisplay::GetSurface(HFONT hfont, TCHAR* text, COLORREF bg, COLORREF fore, BOOL shadow)
{
	CSurface * surface = new CSurface(this);

	if( FAILED(surface->CreateSurface(hfont , text , bg , fore, shadow)) )
	{
		SAFE_DELETE(surface);
		return NULL;
	}

	Add(surface);

	return surface;
}


void CDisplay::Add(CSurface* surface)
{
	if( !surface )
		return;
	m_surflist.push_back(surface);
}

void CDisplay::Del(CSurface* surface)
{
	if( !surface )
		return;

	for( surflist_it it = m_surflist.begin() ; it!=m_surflist.end() ; )
	{
		if( (*it) == surface )
		{
			delete (*it);
			it = m_surflist.erase(it);
			return;
		}
		else
		{
			++it;
		}
	}

	// 찾을 수 없습니다.
}

void CDisplay::ClearAll()
{
	if( m_surflist.empty() )
		return;

	surflist_it it = m_surflist.begin();

	for( ; it!=m_surflist.end() ; ++it )
		if(*it) delete (*it);

	m_surflist.clear();
}






int num_created_surface = 0;

//-----------------------------------------------------------------------------
// Name:
// Desc:
//-----------------------------------------------------------------------------
CSurface::CSurface()
{
	m_display = NULL;
	m_pdds = NULL;
	m_bColorKeyed = NULL;

	num_created_surface++;
	_asm nop;
}


//-----------------------------------------------------------------------------
// Name:
// Desc:
//-----------------------------------------------------------------------------
CSurface::~CSurface()
{
	SAFE_RELEASE( m_pdds );
	num_created_surface--;
	_asm nop;
}

BOOL CSurface::IsLost()
{
	if (m_pdds)
	{
		if (m_pdds->IsLost())
		{
			m_pdds->Restore();
			return TRUE;
		}
		else
			return FALSE;
	}

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name:
// Desc:
//-----------------------------------------------------------------------------
HRESULT CSurface::Create( LPDIRECTDRAWSURFACE7 pdds )
{
	m_pdds = pdds;

	if( m_pdds )
	{
		m_pdds->AddRef();

		// Get the DDSURFACEDESC structure for this surface
		m_ddsd.dwSize = sizeof(m_ddsd);
		m_pdds->GetSurfaceDesc( &m_ddsd );
	}

	return S_OK;
}




//-----------------------------------------------------------------------------
// Name:
// Desc:
//-----------------------------------------------------------------------------
HRESULT CSurface::Create( LPDIRECTDRAW7 pDD, DDSURFACEDESC2* pddsd )
{
	HRESULT hr;

	// Create the DDraw surface
	if( FAILED( hr = pDD->CreateSurface( pddsd, &m_pdds, NULL ) ) )
		return hr;

	// Prepare the DDSURFACEDESC structure
	m_ddsd.dwSize = sizeof(m_ddsd);

	// Get the DDSURFACEDESC structure for this surface
	m_pdds->GetSurfaceDesc( &m_ddsd );

	return S_OK;
}




//-----------------------------------------------------------------------------
// Name:
// Desc:
//-----------------------------------------------------------------------------
HRESULT CSurface::Destroy()
{
	SAFE_RELEASE( m_pdds );
	return S_OK;
}




//-----------------------------------------------------------------------------
// Name: CSurface::DrawBitmap()
// Desc: Draws a bitmap over an entire DirectDrawSurface, stretching the
//       bitmap if nessasary
//-----------------------------------------------------------------------------
HRESULT CSurface::DrawBitmap( HBITMAP hBMP,
							  DWORD dwBMPOriginX, DWORD dwBMPOriginY,
							  DWORD dwBMPWidth, DWORD dwBMPHeight )
{
	HDC            hDCImage;
	HDC            hDC;
	BITMAP         bmp;
	HBITMAP *	   hOldBmp;
	DDSURFACEDESC2 ddsd;
	HRESULT        hr;

	if( hBMP == NULL || m_pdds == NULL )
		return E_INVALIDARG;

	// Make sure this surface is restored.
	if( FAILED( hr = m_pdds->Restore() ) )
		return hr;

	// Get the surface.description
	ddsd.dwSize  = sizeof(ddsd);
	m_pdds->GetSurfaceDesc( &ddsd );

	if( ddsd.ddpfPixelFormat.dwFlags == DDPF_FOURCC )
		return E_NOTIMPL;

	// Select bitmap into a memoryDC so we can use it.
	hDCImage = CreateCompatibleDC( NULL );
	if( NULL == hDCImage )
		return E_FAIL;

	hOldBmp = (HBITMAP*)SelectObject( hDCImage, hBMP );

	// Get size of the bitmap
	GetObject( hBMP, sizeof(bmp), &bmp );

	// Use the passed size, unless zero
	dwBMPWidth  = ( dwBMPWidth  == 0 ) ? bmp.bmWidth  : dwBMPWidth;
	dwBMPHeight = ( dwBMPHeight == 0 ) ? bmp.bmHeight : dwBMPHeight;

	// Stretch the bitmap to cover this surface
	if( FAILED( hr = m_pdds->GetDC( &hDC ) ) )
		return hr;

	StretchBlt( hDC, 0, 0,
				ddsd.dwWidth, ddsd.dwHeight,
				hDCImage, dwBMPOriginX, dwBMPOriginY,
				dwBMPWidth, dwBMPHeight, SRCCOPY );

	if( FAILED( hr = m_pdds->ReleaseDC( hDC ) ) )
		return hr;

	SelectObject( hDCImage, hOldBmp );
	DeleteDC( hDCImage );

	return S_OK;
}




//----------------------------------------------------------------------------
//-- Draw Font New
//----------------------------------------------------------------------------
//-- Option 을 가진 Draw
//-- Bilt 를 사용하는 TEXT 를 제외한 TEXT 는 여기서 출력 된다.

//-- Option : 0 : 그림자 - X      Fo = 글자색  Back = 배경색 Shad = Don't care
//--          1 : 그림자 - 우하   Fo = 글자색  Back = 배경색 Shad = 그림자 색
//--          2 : 그림자 - 외곽선 Fo = 글자색  Back = 배경색 Shad = 외곽선색

//-- DEFALUT :
//-- crShadow = RGB(100,100,100)
//-- option   = 0

//-- 04/03/15
//-- Lyul / 수정중
//-- 옵션을 주며 외부에서 그림자를 직접 생성해야 되었던 이전 루틴을
//-- 자체 해결 하도록 변경
HRESULT CSurface::DrawText( HFONT hFont, TCHAR* strText,
							DWORD dwOriginX, DWORD dwOriginY,
							COLORREF crBackground, COLORREF crForeground, DWORD Option , COLORREF crShadow)
{
	HDC     hDC = NULL;
	HRESULT hr;
	HFONT	oldFont;

	if( m_pdds == NULL || strText == NULL )
		return E_INVALIDARG;

// Make sure this surface is restored.
	if( FAILED( hr = m_pdds->Restore() ) )
		return hr;

	if( FAILED( hr = m_pdds->GetDC( &hDC ) ) )
		return hr;

	//-- Font 설정
	if( hFont )
	{
		oldFont = (HFONT) SelectObject( hDC, hFont );
	}
//---------------------------------------------------------------------------
//-- 각 옵션에 따라 다른 표현
	switch(Option)
	{
	//-- 그림자 - X      Fo = 글자색       Back = 배경색      Shad = Don't care
	case 0:
		SetBkMode(hDC, OPAQUE);
		SetBkColor( hDC, crBackground );
		SetTextColor( hDC, crForeground );
		TextOut( hDC, dwOriginX, dwOriginY, strText, _tcslen(strText) );
		break;
	//-- 그림자 - 우하   Fo = 글자색       Back = 배경색      Shad = 그림자 색
	case 1:
		SetBkMode(hDC, OPAQUE);
		SetBkColor( hDC, crBackground );
		SetTextColor( hDC, crShadow );
		TextOut( hDC, dwOriginX+1, dwOriginY+1, strText, _tcslen(strText) );

		SetBkMode(hDC, TRANSPARENT);
		SetTextColor( hDC, crForeground );
		TextOut( hDC, dwOriginX, dwOriginY, strText, _tcslen(strText) );
		break;
	//-- 그림자 - 외곽선 Fo = 글자색       Back = 배경색      Shad = 외곽선 색
	case 2:
		SetBkMode(hDC, OPAQUE);
		SetBkColor( hDC, crBackground );
		SetTextColor( hDC, crShadow );
		TextOut( hDC, dwOriginX-1, dwOriginY+1, strText, _tcslen(strText) );

		SetBkMode(hDC, TRANSPARENT);
		TextOut( hDC, dwOriginX+1, dwOriginY+1, strText, _tcslen(strText) );
		TextOut( hDC, dwOriginX, dwOriginY, strText, _tcslen(strText) );
		TextOut( hDC, dwOriginX, dwOriginY+2, strText, _tcslen(strText) );

		SetTextColor( hDC, crForeground );
		TextOut( hDC, dwOriginX, dwOriginY+1, strText, _tcslen(strText) );
		break;
	//-- 투명 문자.
	case 3:
		SetBkMode(hDC, TRANSPARENT);
		SetTextColor( hDC, crForeground );
		TextOut( hDC, dwOriginX, dwOriginY, strText, _tcslen(strText) );
		break;
	}

	if (hFont)
	{
		SelectObject( hDC, oldFont);
	}

	if( FAILED( hr = m_pdds->ReleaseDC( hDC ) ) )   return hr;

	return S_OK;
}




//-----------------------------------------------------------------------------
// Name: CSurface::ReDrawBitmapOnSurface()
// Desc: Load a bitmap from a file or resource into a DirectDraw surface.
//       normaly used to re-load a surface after a restore.
//-----------------------------------------------------------------------------
HRESULT CSurface::DrawBitmap( TCHAR* strBMP,
							  DWORD dwDesiredWidth, DWORD dwDesiredHeight  )
{
	HBITMAP hBMP;
	HRESULT hr;

	if( m_pdds == NULL || strBMP == NULL )
		return E_INVALIDARG;

	//  Try to load the bitmap as a resource, if that fails, try it as a file
	hBMP = (HBITMAP) LoadImage( GetModuleHandle(NULL), strBMP,
								IMAGE_BITMAP, dwDesiredWidth, dwDesiredHeight,
								LR_CREATEDIBSECTION );
	if( hBMP == NULL )
	{
		hBMP = (HBITMAP) LoadImage( NULL, strBMP, IMAGE_BITMAP,
									dwDesiredWidth, dwDesiredHeight,
									LR_LOADFROMFILE | LR_CREATEDIBSECTION );
		if( hBMP == NULL )
			return E_FAIL;
	}

	// Draw the bitmap on this surface
	if( FAILED( hr = DrawBitmap( hBMP, 0, 0, 0, 0 ) ) )
	{
		DeleteObject( hBMP );
		return hr;
	}

	DeleteObject( hBMP );

	return S_OK;
}




//-----------------------------------------------------------------------------
// Name:
// Desc:
//-----------------------------------------------------------------------------
HRESULT CSurface::SetColorKey( DWORD dwColorKey )
{
	if( NULL == m_pdds )
		return E_POINTER;

	m_bColorKeyed = TRUE;

	DDCOLORKEY ddck;
	ddck.dwColorSpaceLowValue  = ConvertGDIColor( dwColorKey );
	ddck.dwColorSpaceHighValue = ConvertGDIColor( dwColorKey );

	return m_pdds->SetColorKey( DDCKEY_SRCBLT, &ddck );
}




//-----------------------------------------------------------------------------
// Name: CSurface::ConvertGDIColor()
// Desc: Converts a GDI color (0x00bbggrr) into the equivalent color on a
//       DirectDrawSurface using its pixel format.
//-----------------------------------------------------------------------------
DWORD CSurface::ConvertGDIColor( COLORREF dwGDIColor )
{
	if( m_pdds == NULL )
		return 0x00000000;

	COLORREF       rgbT;
	HDC            hdc;
	DWORD          dw = CLR_INVALID;
	DDSURFACEDESC2 ddsd;
	HRESULT        hr;

	//  Use GDI SetPixel to color match for us
	if( dwGDIColor != CLR_INVALID && m_pdds->GetDC(&hdc) == DD_OK)
	{
		rgbT = GetPixel(hdc, 0, 0);     // Save current pixel value
		SetPixel(hdc, 0, 0, dwGDIColor);       // Set our value
		m_pdds->ReleaseDC(hdc);
	}

	// Now lock the surface so we can read back the converted color
	if (m_pdds->IsLost() == DDERR_SURFACELOST)
	{
		m_pdds->Restore();
	}

	ddsd.dwSize = sizeof(ddsd);
	hr = m_pdds->Lock( NULL, &ddsd, DDLOCK_WAIT, NULL );
	if( hr == DD_OK)
	{
		dw = *(DWORD *) ddsd.lpSurface;
		if( ddsd.ddpfPixelFormat.dwRGBBitCount < 32 ) // Mask it to bpp
			dw &= ( 1 << ddsd.ddpfPixelFormat.dwRGBBitCount ) - 1;
		m_pdds->Unlock(NULL);
	}

	//  Now put the color that was there back.
	if( dwGDIColor != CLR_INVALID && m_pdds->GetDC(&hdc) == DD_OK )
	{
		SetPixel( hdc, 0, 0, rgbT );
		m_pdds->ReleaseDC(hdc);
	}

	return dw;
}




//-----------------------------------------------------------------------------
// Name: CSurface::GetBitMaskInfo()
// Desc: Returns the number of bits and the shift in the bit mask
//-----------------------------------------------------------------------------
HRESULT CSurface::GetBitMaskInfo( DWORD dwBitMask, DWORD* pdwShift, DWORD* pdwBits )
{
	DWORD dwShift = 0;
	DWORD dwBits  = 0;

	if( pdwShift == NULL || pdwBits == NULL )
		return E_INVALIDARG;

	if( dwBitMask )
	{
		while( (dwBitMask & 1) == 0 )
		{
			dwShift++;
			dwBitMask >>= 1;
		}
	}

	while( (dwBitMask & 1) != 0 )
	{
		dwBits++;
		dwBitMask >>= 1;
	}

	*pdwShift = dwShift;
	*pdwBits  = dwBits;

	return S_OK;
}

HRESULT CSurface::Clear( DWORD dwColor )
{
	if( NULL == m_pdds )
		return E_POINTER;

	// Erase the background
	DDBLTFX ddbltfx;
	ZeroMemory( &ddbltfx, sizeof(ddbltfx) );
	ddbltfx.dwSize      = sizeof(ddbltfx);
	ddbltfx.dwFillColor = dwColor;

	return m_pdds->Blt( NULL, NULL, NULL, DDBLT_COLORFILL, &ddbltfx );
}

bool CSurface::IsIn(int x, int y)
{
	RECT rc;
	SetRect( &rc , Xpos , Ypos , Xpos+m_dwWidth , Ypos+m_dwHeight );

	POINT pt = {x,y};

	return PtInRect(&rc , pt)?true:false;
}

HRESULT CSurface::CreateSurface(DWORD w, DWORD h)
{
	if( !m_display || !m_display->GetDirectDraw() )
		return E_FAIL;

	ZeroMemory( &m_ddsd, sizeof( m_ddsd ) );
	m_ddsd.dwSize         = sizeof( m_ddsd );
	m_ddsd.dwFlags        = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;
	m_ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
	m_ddsd.dwWidth        = w;
	m_ddsd.dwHeight       = h;

	SAFE_RELEASE(m_pdds);

	HRESULT hr;
	if( FAILED( hr = Create( m_display->GetDirectDraw(), &m_ddsd ) ) )
		return hr;

	return S_OK;
}

HRESULT CSurface::CreateSurface(TCHAR * filename, DWORD w, DWORD h)
{
	if( !m_display || !m_display->GetDirectDraw() )
		return E_FAIL;

	HBITMAP        hBMP = NULL;
	BITMAP         bmp;

	hBMP = (HBITMAP) LoadImage( GetModuleHandle(NULL), filename,  IMAGE_BITMAP, w, h, LR_CREATEDIBSECTION );
	if( hBMP == NULL )
	{
		hBMP = LoadBitmapFile( filename );
		if( hBMP == NULL )
			return E_FAIL;
	}

	GetObject( hBMP, sizeof(bmp), &bmp );

	ZeroMemory( &m_ddsd, sizeof(m_ddsd) );
	m_ddsd.dwSize         = sizeof(m_ddsd);
	m_ddsd.dwFlags        = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
	m_ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;
	m_ddsd.dwWidth        = bmp.bmWidth;
	m_ddsd.dwHeight       = bmp.bmHeight;

	SAFE_RELEASE(m_pdds);

	HRESULT hr;
	if( FAILED( hr = Create( m_display->GetDirectDraw(), &m_ddsd ) ) )
		return hr;

	if( FAILED( hr = DrawBitmap( hBMP, 0, 0, 0, 0 ) ) )
	{
		DeleteObject( hBMP );
		return hr;
	}

	SetSize(bmp.bmWidth , bmp.bmHeight);

	DeleteObject( hBMP );

	return S_OK;
}

HRESULT CSurface::CreateSurface(TCHAR * filename)
{
	if( !m_display || !m_display->GetDirectDraw() )
		return E_FAIL;

	CVirtualFile* pVF = CVirtualFile::GetInstance();
	if( !pVF->OpenFile( filename ) )
		return NULL;

	IPicture* picture = NULL;
	IStream* stream;
	HGLOBAL hGlobal;
	void* pp;
	char* orgBuf = NULL;
	char* buf = NULL;
	long orgBufSize = 0;

	pVF->Read( &orgBuf, orgBufSize );
	buf = orgBuf;
	pVF->CloseFile();

	hGlobal = GlobalAlloc(GPTR, orgBufSize);
	if( !hGlobal )
	{
		SAFE_DELETE_ARRAY( orgBuf );
		return NULL;
	}
	pp = (void*)hGlobal;
	SAFE_READ_BUFFER( pp, buf, 1 * orgBufSize );
	SAFE_DELETE_ARRAY( orgBuf );

	CreateStreamOnHGlobal (hGlobal, false, &stream);

	if( !stream )
	{
		GlobalFree(picture);
		return NULL;
	}

	OleLoadPicture(stream, 0, false, IID_IPicture, (void**)&picture);

	if (!picture)
	{
		stream->Release();
		GlobalFree(hGlobal);
		return NULL;
	}

	stream->Release();
	GlobalFree(hGlobal);

	HBITMAP hBitmap = 0;
	picture->get_Handle((unsigned int*)&hBitmap);
	HBITMAP tempBitmap = (HBITMAP)CopyImage(hBitmap,IMAGE_BITMAP,0,0,LR_COPYRETURNORG);
	picture->Release();

	//////////////////////////////////////////////////////////////////////////	
	BITMAP         bmp;

	GetObject( tempBitmap, sizeof(bmp), &bmp );

	ZeroMemory( &m_ddsd, sizeof(m_ddsd) );
	m_ddsd.dwSize         = sizeof(DDSURFACEDESC2);
	m_ddsd.dwFlags        = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
	m_ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;
	m_ddsd.dwWidth        = bmp.bmWidth;
	m_ddsd.dwHeight       = bmp.bmHeight;

	SAFE_RELEASE(m_pdds);

	HRESULT hr;
	if( FAILED( hr = Create( m_display->GetDirectDraw(), &m_ddsd ) ) )
		return hr;

	if( FAILED( hr = DrawBitmap( tempBitmap, 0, 0, 0, 0 ) ) )
	{
		DeleteObject( tempBitmap );
		return hr;
	}

	SetSize(m_ddsd.dwWidth , m_ddsd.dwHeight);

	DeleteObject( tempBitmap );

	return S_OK;
}

HRESULT CSurface::CreateSurface(HFONT hFont, TCHAR * text, COLORREF bg, COLORREF fore, BOOL shadow)
{
	if( !m_display || !m_display->GetDirectDraw() )
		return E_FAIL;

	HDC                  hDC  = NULL;
	HRESULT              hr;
	SIZE                 sizeText;
	HFONT *				 hOldFont;

	hDC = GetDC( NULL );

	if( hFont )
		hOldFont = (HFONT*)SelectObject( hDC, hFont );

	GetTextExtentPoint32( hDC, text, _tcslen(text), &sizeText );

	if( hFont )
		SelectObject(hDC , hOldFont);

	ReleaseDC( NULL, hDC );

	ZeroMemory( &m_ddsd, sizeof(m_ddsd) );
	m_ddsd.dwSize         = sizeof(m_ddsd);
	m_ddsd.dwFlags        = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
	m_ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
	m_ddsd.dwWidth        = sizeText.cx;
	m_ddsd.dwHeight       = sizeText.cy;

	if(shadow)
	{
		m_ddsd.dwWidth	+= 1;
		m_ddsd.dwHeight	+= 1;
	}

	SAFE_RELEASE(m_pdds);

	if( FAILED( hr = Create( m_display->GetDirectDraw(), &m_ddsd ) ) )
		return hr;

	SetSize(m_ddsd.dwWidth , m_ddsd.dwHeight);

	if (shadow)
	{
		if( FAILED( hr = DrawText( hFont, text, 0, 0, bg, RGB(127, 127, 127),0 ) ) )
			return hr;
		if( FAILED( hr = DrawText( hFont, text, 1, 1, bg, fore ,0) ) )
			return hr;
	}
	else
	{
		if( FAILED( hr = DrawText( hFont, text, 0, 0, bg, fore, 0 ) ) )
			return hr;
	}

	return S_OK;
}
