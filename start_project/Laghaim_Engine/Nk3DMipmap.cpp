#include "stdafx.h"
// Nk3DMipmap.cpp: implementation of the Nk3DMipmap class.
//
//////////////////////////////////////////////////////////////////////

#include <d3d.h>
#include "d3dutil.h"
#include "IndexedTexture.h"
#include "Nk3DMipmap.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
// Name: CNk3DMipMap()
// Desc: Constructs a mipmap object
//-----------------------------------------------------------------------------
CNk3DMipMap::CNk3DMipMap()
{
	m_pddsSurface = NULL;
}

CNk3DMipMap::~CNk3DMipMap()
{
	Invalidate();
}

void CNk3DMipMap::SetTexturePath(char *strFilePath)
{
	if (strlen(strFilePath) > 254)
	{
		MessageBox(NULL, "The file path is longer than 255 characters..", "Warning", MB_OK);
		m_strFilePath[0] = NULL;
		return;
	}

	strcpy(m_strFilePath, strFilePath);
}


//-----------------------------------------------------------------------------
// Name: MipMap_Create()
// Desc: Creates the mipmap structure and loads image data from bitmaps.
//-----------------------------------------------------------------------------
HRESULT CNk3DMipMap::Create( CHAR** strMipMapNames , WORD wMipMapNum)
{
	char strFilePath[1024];

	m_dwMipMapCount = wMipMapNum;

	WORD wNum=0;
	for( wNum=0; wNum<wMipMapNum; wNum++ )
	{
		strcpy(strFilePath, m_strFilePath);
		strcat(strFilePath, strMipMapNames[wNum]);

		HBITMAP hbm = (HBITMAP)VLoadImage( GetModuleHandle(NULL),
										   strFilePath, IMAGE_BITMAP,
										   0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION );
		if( NULL == hbm )
		{
			if( 0 == wNum )
				return E_FAIL;

			m_dwMipMapCount = wNum;
			return S_OK;
		}

		m_hbmBitmap[wNum] = hbm;
	}

	if (wMipMapNum == wNum)
		return S_OK;
	else
		return E_FAIL;
}




//-----------------------------------------------------------------------------
// Name: TextureSearchCallback()
// Desc: Callback function used to enumerate texture formats.
//-----------------------------------------------------------------------------
HRESULT CALLBACK TextureSearchCallback( DDPIXELFORMAT* pddpf, VOID* param )
{
	DDSURFACEDESC2* pddsd = (DDSURFACEDESC2*)param;

	// Skip unwanted formats
	if( pddpf->dwRGBBitCount != pddsd->dwFlags )
		return DDENUMRET_OK;
	if( pddpf->dwFlags & (DDPF_LUMINANCE|DDPF_ALPHAPIXELS) )
		return DDENUMRET_OK;
	if( pddpf->dwFlags & (DDPF_BUMPLUMINANCE|DDPF_BUMPDUDV) )
		return DDENUMRET_OK;
	if( 0 != pddpf->dwFourCC )
		return DDENUMRET_OK;

	memcpy( &pddsd->ddpfPixelFormat, pddpf, sizeof(DDPIXELFORMAT) );
	return DDENUMRET_CANCEL;
}




//-----------------------------------------------------------------------------
// Name: MipMap_Restore()
// Desc: Creates the device-dependant surface and texture for the mipmap
//-----------------------------------------------------------------------------
HRESULT CNk3DMipMap::Restore( LPDIRECT3DDEVICE7 pd3dDevice )
{
	// Release any previously created objects
	Invalidate();

	// Get size info for top level bitmap
	BITMAP bm;
	GetObject( m_hbmBitmap[0], sizeof(BITMAP), &bm );

	// Set up and create the mipmap surface
	DDSURFACEDESC2 ddsd;
	ZeroMemory( &ddsd, sizeof(DDSURFACEDESC2) );
	ddsd.dwSize          = sizeof(DDSURFACEDESC2);
	ddsd.dwFlags         = DDSD_CAPS|DDSD_MIPMAPCOUNT|DDSD_WIDTH|DDSD_HEIGHT|
						   DDSD_PIXELFORMAT;
	ddsd.ddsCaps.dwCaps  = DDSCAPS_TEXTURE|DDSCAPS_MIPMAP|DDSCAPS_COMPLEX;
	ddsd.dwMipMapCount   = m_dwMipMapCount;
	ddsd.dwWidth         = bm.bmWidth;
	ddsd.dwHeight        = bm.bmHeight;

	// Get the device caps
	D3DDEVICEDESC7 ddDesc;
	DWORD         dwDeviceCaps;
	if( FAILED( pd3dDevice->GetCaps( &ddDesc ) ) )
		return E_FAIL;
	dwDeviceCaps = ddDesc.dpcTriCaps.dwTextureCaps;

	// Turn on texture management for hardware devices
	if( IsEqualIID(ddDesc.deviceGUID, IID_IDirect3DHALDevice) || IsEqualIID(ddDesc.deviceGUID, IID_IDirect3DTnLHalDevice) )
		ddsd.ddsCaps.dwCaps2 = DDSCAPS2_TEXTUREMANAGE;
	else
		ddsd.ddsCaps.dwCaps |= DDSCAPS_SYSTEMMEMORY;

	// Enumerate a good texture format. Search for a 16-bit format first
	DDSURFACEDESC2 ddsdSearch;
	ddsdSearch.dwFlags = 16;
	pd3dDevice->EnumTextureFormats( TextureSearchCallback, &ddsdSearch );

	// If that wasn't found, check for a 32-bit format
	if( 16 != ddsdSearch.ddpfPixelFormat.dwRGBBitCount )
	{
		ddsdSearch.dwFlags = 32;
		pd3dDevice->EnumTextureFormats( TextureSearchCallback, &ddsdSearch );
		if( 32 != ddsdSearch.ddpfPixelFormat.dwRGBBitCount )
			return E_FAIL;
	}

	// If we got a good texture format, use it to create the surface
	memcpy( &ddsd.ddpfPixelFormat, &ddsdSearch.ddpfPixelFormat,
			sizeof(DDPIXELFORMAT) );

	// Get a DDraw ptr (from the device's render target) for creating surfaces.
	// Note: the Release() calls just serve to decrement the ref count, but the
	// ptrs are still valid.
	LPDIRECTDRAWSURFACE7 pddsRender;
	LPDIRECTDRAW7        pDD  = NULL;
	pd3dDevice->GetRenderTarget( &pddsRender );
	pddsRender->GetDDInterface( (VOID**)&pDD );
	pddsRender->Release();

	// Create the mipmap surface
	if( FAILED( pDD->CreateSurface( &ddsd, &m_pddsSurface, NULL ) ) )
	{
		pDD->Release();
		return E_FAIL;
	}

	// Done with DDraw
	pDD->Release();

	// Loop through each surface in the mipmap, copying the bitmap to the temp
	// surface, and then blitting the temp surface to the real one.
	LPDIRECTDRAWSURFACE7 pddsDest = m_pddsSurface;

	for( WORD wNum=0; wNum < m_dwMipMapCount; wNum++ )
	{
		// Copy the bitmap image to the surface
		BITMAP bm;
		GetObject( m_hbmBitmap[wNum], sizeof(BITMAP), &bm );

		// Create a DC and setup the bitmap
		HDC hdcBitmap = CreateCompatibleDC( NULL );
		if( NULL == hdcBitmap )
			return E_FAIL;

		SelectObject( hdcBitmap, m_hbmBitmap[wNum] );

		HDC hdcSurface;
		if( SUCCEEDED( pddsDest->GetDC( &hdcSurface ) ) )
		{
			BitBlt( hdcSurface, 0, 0, bm.bmWidth, bm.bmHeight, hdcBitmap,
					0, 0, SRCCOPY );
			pddsDest->ReleaseDC( hdcSurface );
		}

		DeleteDC( hdcBitmap );

		// Get the next surface in the chain. Do a Release() call, though, to
		// avoid increasing the ref counts on the surfaces.
		DDSCAPS2 ddsCaps;
		ddsCaps.dwCaps  = DDSCAPS_TEXTURE | DDSCAPS_MIPMAP;
		ddsCaps.dwCaps2 = 0;
		ddsCaps.dwCaps3 = 0;
		ddsCaps.dwCaps4 = 0;
		if( SUCCEEDED( pddsDest->GetAttachedSurface( &ddsCaps, &pddsDest ) ) )
			pddsDest->Release();
	}

	// For palettized textures, use the bitmap and GetDIBColorTable() to build
	// and attach a palette to the surface here.

	return S_OK;
}


//-----------------------------------------------------------------------------
// Name: MipMap_Invalidate()
// Desc: Frees device dependant objects for the mipmap
//-----------------------------------------------------------------------------
VOID CNk3DMipMap::Invalidate()
{
	SAFE_RELEASE( m_pddsSurface );
}




//-----------------------------------------------------------------------------
// Name: Mipmap_Delete()
// Desc: Frees device dependant objects for the mipmap
//-----------------------------------------------------------------------------
VOID CNk3DMipMap::Delete()
{
	for( WORD wNum = 0; wNum < m_dwMipMapCount; wNum++ )
		DeleteObject( m_hbmBitmap[wNum] );
}


