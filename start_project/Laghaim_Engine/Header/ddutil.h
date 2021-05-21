//-----------------------------------------------------------------------------
// File: ddutil.cpp
//
// Desc: Routines for loading bitmap and palettes from resources
//
// Copyright (C) 1998-1999 Microsoft Corporation. All Rights Reserved.
//-----------------------------------------------------------------------------
#ifndef DDUTIL_H
#define DDUTIL_H

#include <ddraw.h>
#include <d3d.h>
#include <list>
#include "MemoryPoolBase.h"


//-----------------------------------------------------------------------------
// Classes defined in this header file
//-----------------------------------------------------------------------------
class CDisplay;
class CSurface;




//-----------------------------------------------------------------------------
// Flags for the CDisplay and CSurface methods
//-----------------------------------------------------------------------------
#define DSURFACELOCK_READ
#define DSURFACELOCK_WRITE




//-----------------------------------------------------------------------------
// Name: class CDisplay
// Desc: Class to handle all DDraw aspects of a display, including creation of
//       front and back buffers, creating offscreen surfaces and palettes,
//       and blitting surface and displaying bitmaps.
//-----------------------------------------------------------------------------
class CDisplay
{
protected:
	LPDIRECTDRAW7        m_pDD;
	LPDIRECTDRAWSURFACE7 m_pddsFrontBuffer;
	LPDIRECTDRAWSURFACE7 m_pddsBackBuffer;
	LPDIRECTDRAWSURFACE7 m_pddsBackBufferLeft; // For stereo modes

	HWND                 m_hWnd;
	RECT                 m_rcWindow;
	BOOL                 m_bWindowed;
	BOOL                 m_bStereo;

public:
	CDisplay();
	~CDisplay();

	// Access functions
	HWND                 GetHWnd()
	{
		return m_hWnd;
	}
	LPDIRECTDRAW7        GetDirectDraw()
	{
		return m_pDD;
	}
	LPDIRECTDRAWSURFACE7 GetFrontBuffer()
	{
		return m_pddsFrontBuffer;
	}
	LPDIRECTDRAWSURFACE7 GetBackBuffer()
	{
		return m_pddsBackBuffer;
	}
	LPDIRECTDRAWSURFACE7 GetBackBufferLEft()
	{
		return m_pddsBackBufferLeft;
	}
	// 정원이가 집어 넣은 함수
	void				 SetHwnd(HWND hwnd)
	{
		m_hWnd = hwnd;
	}
	void                 SetDirectDraw(LPDIRECTDRAW7 pDD)
	{
		m_pDD = pDD;
	}
	void				 SetBackBuffer(LPDIRECTDRAWSURFACE7 pddsBackBuffer)
	{
		m_pddsBackBuffer = pddsBackBuffer;
	}

	// Status functions
	BOOL    IsWindowed()
	{
		return m_bWindowed;
	}
	BOOL    IsStereo()
	{
		return m_bStereo;
	}

	// Creation/destruction methods
	HRESULT CreateFullScreenDisplay( HWND hWnd, DWORD dwWidth, DWORD dwHeight,
									 DWORD dwBPP );
	HRESULT CreateWindowedDisplay( HWND hWnd, DWORD dwWidth, DWORD dwHeight );
	HRESULT InitClipper();
	HRESULT UpdateBounds();
	virtual HRESULT DestroyObjects();

	// Methods to create child objects
	HRESULT CreateSurface( CSurface** ppSurface, DWORD dwWidth,
						   DWORD dwHeight );
	HRESULT CreateSurfaceFromBitmap( CSurface** ppSurface, TCHAR* strBMP,
									 DWORD dwDesiredWidth = 0,
									 DWORD dwDesiredHeight = 0 );
	HRESULT CreateSurfaceFromText( CSurface** ppSurface, HFONT hFont,
								   TCHAR* strText,
								   COLORREF crBackground,
								   COLORREF crForeground, BOOL bShadow = FALSE );
	HRESULT CreatePaletteFromBitmap( LPDIRECTDRAWPALETTE* ppPalette, const TCHAR* strBMP );
	HRESULT CreateSurfaceFromJPG( CSurface** ppSurface, TCHAR* strJPG);//, DWORD dwDesiredWidth, DWORD dwDesiredHeight )

	// Display methods
	HRESULT Clear( DWORD dwColor = 0L );
	HRESULT ColorKeyBlt( DWORD x, DWORD y, LPDIRECTDRAWSURFACE7 pdds,
						 RECT* prc = NULL );
	HRESULT OpacityBlt( DWORD left, DWORD top, DWORD right, DWORD bottom,
						LPDIRECTDRAWSURFACE7 pdds, RECT* srcRect = NULL, BYTE dwAlpha = 0);
	HRESULT Blt( DWORD x, DWORD y, LPDIRECTDRAWSURFACE7 pdds,
				 RECT* prc=NULL, DWORD dwFlags=0 );
	HRESULT Blt( DWORD x, DWORD y, CSurface* pSurface, RECT* prc = NULL );
	HRESULT Blt( DWORD left, DWORD top, DWORD right, DWORD bottom,
				 LPDIRECTDRAWSURFACE7 pdds, RECT* srcRect = NULL, DWORD dwFlags = NULL);
	HRESULT Blt( DWORD left, DWORD top, DWORD right, DWORD bottom,
				 CSurface* pSurface, RECT* srcRect = NULL );
	HRESULT ShowBitmap( HBITMAP hbm, LPDIRECTDRAWPALETTE pPalette=NULL );
	HRESULT SetPalette( LPDIRECTDRAWPALETTE pPalette );
	HRESULT Present();


public:
	CSurface * GetSurface(CSurface* surface);
	CSurface * GetSurface(DWORD w, DWORD h);
	CSurface * GetSurface(TCHAR* filename, BOOL jpg=FALSE, DWORD w=0, DWORD h=0);
	CSurface * GetSurface(HFONT hfont, TCHAR* text, COLORREF bg, COLORREF fore, BOOL shadow=FALSE);

private:
	typedef std::list<CSurface*>			surflist;
	typedef surflist::iterator				surflist_it;


public:
	surflist		m_surflist;

	void Add(CSurface* surface);
	void Del(CSurface* surface);
	void ClearAll();
};

extern CDisplay * g_pDisplay;




//-----------------------------------------------------------------------------
// Name: class CSurface
// Desc: Class to handle aspects of a DirectDrawSurface.
//-----------------------------------------------------------------------------
class CSurface : public MemoryPoolBase<CSurface>
{
	LPDIRECTDRAWSURFACE7 m_pdds;
	DDSURFACEDESC2       m_ddsd;
	BOOL                 m_bColorKeyed;
	DWORD				 m_dwWidth;
	DWORD				 m_dwHeight;



public:
	LPDIRECTDRAWSURFACE7 GetDDrawSurface()
	{
		return m_pdds;
	}
	BOOL                 IsColorKeyed()
	{
		return m_bColorKeyed;
	}

	HRESULT DrawBitmap( HBITMAP hBMP, DWORD dwBMPOriginX = 0, DWORD dwBMPOriginY = 0,
						DWORD dwBMPWidth = 0, DWORD dwBMPHeight = 0 );
	HRESULT DrawBitmap( TCHAR* strBMP, DWORD dwDesiredWidth, DWORD dwDesiredHeight );
	//-- Option 으로 통합
	//-- Lyul 수정중
	HRESULT DrawText( HFONT hFont, TCHAR* strText,
					  DWORD dwOriginX, DWORD dwOriginY,
					  COLORREF crBackground, COLORREF crForeground,
					  DWORD Option = 0, COLORREF crShadow = RGB(100,100,100));


	HRESULT SetColorKey( DWORD dwColorKey );
	DWORD   ConvertGDIColor( COLORREF dwGDIColor );
	static HRESULT GetBitMaskInfo( DWORD dwBitMask, DWORD* pdwShift, DWORD* pdwBits );

	HRESULT Create( LPDIRECTDRAW7 pDD, DDSURFACEDESC2* pddsd );
	HRESULT Create( LPDIRECTDRAWSURFACE7 pdds );
	HRESULT Destroy();

	void	SetWidth(DWORD dwWidth)
	{
		m_dwWidth = dwWidth;
	};
	void	SetHeight(DWORD dwHeight)
	{
		m_dwHeight = dwHeight;
	};
	DWORD	GetWidth()
	{
		return m_dwWidth;
	};
	DWORD   GetHeight()
	{
		return m_dwHeight;
	};
	// 상은이가 추가한 것...
	HRESULT Clear( DWORD dwColor = 0L );

	BOOL	IsLost();

	//	by evilkiki 2008.08.28
	bool IsIn(int x, int y);

	CSurface();
	~CSurface();

	//  길성이가 추가한거..
	int Xpos;
	int Ypos;


	void SetPos(int x, int y)
	{
		Xpos = x;
		Ypos = y;
	}
	void SetSize(DWORD w, DWORD h)
	{
		m_dwWidth = w ;
		m_dwHeight = h;
	}


public:
	CDisplay * m_display;

	CSurface(CDisplay * display) : m_display(display) , m_pdds(NULL) , m_bColorKeyed(false) {}
	void SetDisplay(CDisplay * display)
	{
		m_display = display;
	}

	HRESULT CreateSurface(DWORD w, DWORD h);
	HRESULT CreateSurface(TCHAR * filename, DWORD w, DWORD h);
	HRESULT CreateSurface(TCHAR * filename);
	HRESULT CreateSurface(HFONT hFont, TCHAR * text, COLORREF bg, COLORREF fore, BOOL shadow=false);

};

#ifndef SAFE_DELETE_RES
#define SAFE_DELETE_RES(p)		{ if(p) p->DeleteRes(); }
#endif
#ifndef SAFE_LOAD_RES
#define SAFE_LOAD_RES(p)		{ if(p) p->LoadRes(); }
#endif
#ifndef SAFE_SURFACE_DESTROY
#define SAFE_SURFACE_DESTROY(p) { if(p) p->Destroy(); }
#endif


#endif // DDUTIL_H

