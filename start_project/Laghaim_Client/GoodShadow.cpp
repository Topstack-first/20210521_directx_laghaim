#include "stdafx.h"
#define D3D_OVERLOADS


#include <assert.h>
#include <ddraw.h>
#include <tchar.h>
#include "headers.h"
#include "ddutil.h"
#include "dxutil.h"
#include "main.h"
#include "D3DEnum.h"
#include "GoodShadow.h"
#include "SMRender.h"
#include "IndexedTexture.h"



extern float g_fTimeKey;

struct TEXTURESEARCHINFO
{
	DWORD dwDesiredBPP; // Input for texture format search
	BOOL  bUseAlpha;
	DWORD dwDesiredAlphaBPP;
	BOOL  bUsePalette;
	BOOL  bUseFourCC;
	BOOL  bUseBumpDUDV;
	BOOL  bFoundGoodFormat;

	DDPIXELFORMAT* pddpf; // Result of texture format search
};

static HRESULT CALLBACK TextureSearchCallback( DDPIXELFORMAT* pddpf, VOID* param )
{
	//if bad data in...
	if( !pddpf || !param )
		return DDENUMRET_OK;

	TEXTURESEARCHINFO* ptsi = (TEXTURESEARCHINFO*)param;
	if( !ptsi )
		return DD_FALSE;

	if (ptsi->bUseBumpDUDV)
	{
		//for now, I haven't supported the luminance flags, that's next
		if ( pddpf->dwFlags & (DDPF_BUMPLUMINANCE | DDPF_LUMINANCE))
			//this tex format is not the bump map we want
			return DDENUMRET_OK;
		// make sure we get a BUMPDUDV mode;
		if (!( pddpf->dwFlags & DDPF_BUMPDUDV ))
			//this tex format is not a bump map
			return DDENUMRET_OK;
		else
			OutputDebugString("desired bump map format found \n");
	}
	else
	{
		// Skip any bump mapping and/or luminance modes, since they werem't requested
		if( pddpf->dwFlags & (DDPF_LUMINANCE|DDPF_BUMPLUMINANCE|DDPF_BUMPDUDV) )
			return DDENUMRET_OK;
	}

	// Check for palettized formats
	if( ptsi->bUsePalette )
	{
		if( !( pddpf->dwFlags & DDPF_PALETTEINDEXED8 ) )
			return DDENUMRET_OK;

		// Accept the first 8-bit palettized format we get
		memcpy( ptsi->pddpf, pddpf, sizeof(DDPIXELFORMAT) );
		ptsi->bFoundGoodFormat = TRUE;
		return DDENUMRET_CANCEL;
	}

	// Else, skip any paletized formats (all modes under 16bpp)
	// NOTE, THAT MEANS WE ARE NOT CURRENTLY HANDLING PALETTIZED TEXTURES
	if( pddpf->dwRGBBitCount < 16 )
		return DDENUMRET_OK;

	// Check for FourCC formats
	if( ptsi->bUseFourCC )
	{
		if( pddpf->dwFourCC == 0 )
			return DDENUMRET_OK;

		return DDENUMRET_CANCEL;
	}

	// Else, skip any FourCC formats
	if( pddpf->dwFourCC != 0 )
		return DDENUMRET_OK;

	// Make sure current alpha format agrees with requested format type
	// (i.e. If the user asks for alpha, and the surface doesn't have an
	// alpha channel, or vice versa, then we want to get out)
	if( (ptsi->bUseAlpha==TRUE) && !(pddpf->dwFlags&DDPF_ALPHAPIXELS) )
		return DDENUMRET_OK;
	if( (ptsi->bUseAlpha==FALSE) && (pddpf->dwFlags&DDPF_ALPHAPIXELS) )
		return DDENUMRET_OK;

	// Check if we found a good match
	if( pddpf->dwRGBBitCount == ptsi->dwDesiredBPP )
	{
		//if requesting to use alpha, then format must have required alpha
		if ((ptsi->bUseAlpha) || (ptsi->dwDesiredAlphaBPP == 1))
		{
			// dwAlphaBitDepth member reflects the bit depth of an alpha-only
			// pixel format. For pixel formats with RGBA, we have to
			// do the following

			if ((pddpf->dwFlags && DDPF_ALPHAPIXELS)==0)
				//rejecting, format had no alpha
				return	DDENUMRET_OK;
			else
			{
				if	((ptsi->dwDesiredAlphaBPP == 4) && (pddpf->dwRGBAlphaBitMask != 0xf000))
					//rejecting. Format had alpha, but not 4 bits
					return	DDENUMRET_OK;
				if	((ptsi->dwDesiredAlphaBPP == 8) && (pddpf->dwRGBAlphaBitMask != 0xff000000))
					//rejecting. Format had alpha, but not 8 bits
					return	DDENUMRET_OK;
			}
		}

		//if we fell through to here, then we found what we wanted
		memcpy( ptsi->pddpf, pddpf, sizeof(DDPIXELFORMAT) );
		ptsi->bFoundGoodFormat = TRUE;
		return DDENUMRET_CANCEL;
	}
	//if we fell through to here, then the bpp didn't match what we wanted
	return DDENUMRET_OK;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGoodShadow::CGoodShadow()
	: m_Xpos(0)
	, m_Ypos(0)
	, m_Width(0)
	, m_Height(0)
	, m_dwWidth(0)
	, m_dwHeight(0)
	, m_dwColor(0)
	, m_lightingOld(0)
	, m_lightEnable0(FALSE)
	, m_lightEnable1(FALSE)
	, m_lightEnable2(FALSE)
{
	memset(m_fTimeElapsed, 0, sizeof(m_fTimeElapsed));
	memset(m_nCurFrame, 0, sizeof(m_nCurFrame));

	m_pddsSmRenderTarget = NULL;
	m_wasLpRenderTarget = NULL;
	m_hbmBitmap = NULL;
	m_dwStage = 0;
	m_dwFlags = 0;

	m_pSMRender = new CSMRender();
}

CGoodShadow::~CGoodShadow()
{
	if (m_hbmBitmap)
	{
		DeleteObject( m_hbmBitmap );
		m_hbmBitmap = NULL;
	}
	SAFE_DELETE(m_pSMRender);
	Invalidate();
	SAFE_DELETE(m_pddsSmRenderTarget);
}

void CGoodShadow::Init()
{
	if( m_pSMRender )
		m_pSMRender->Init(SHADOW_TARGET_WIDTH, SHADOW_TARGET_HEIGHT);

	m_dwWidth = SHADOW_TARGET_WIDTH;
	m_dwHeight = SHADOW_TARGET_HEIGHT;

	CreateTexture();
}

void CGoodShadow::DeleteRes()
{
	if( m_pSMRender )
		m_pSMRender->DeleteRes();
	Invalidate();
}

bool CGoodShadow::CheckRestore()
{
	if( m_pddsSmRenderTarget && m_pddsSmRenderTarget->IsLost() == DDERR_SURFACELOST )
	{
		return false;
	}
	if( m_pSMRender && m_pSMRender->CheckRestore() == false )
	{
		return false;
	}

	return true;
}

HRESULT CGoodShadow::Restore()
{
	HRESULT hr = S_OK;

	if( m_pddsSmRenderTarget )
	{
		m_pddsSmRenderTarget->Restore();
	}
	if( m_pSMRender )
	{
		m_pSMRender->Restore();
	}

	return hr;
}

HRESULT CGoodShadow::CreateTexture()
{
	// Setup the new surface desc
	DDSURFACEDESC2 ddsd;
	ZeroMemory( &ddsd, sizeof(DDSURFACEDESC2) );
	ddsd.dwSize                 = sizeof(DDSURFACEDESC2);
	ddsd.dwFlags                = 0;
	ddsd.ddsCaps.dwCaps         = 0;
	ddsd.ddpfPixelFormat.dwSize = sizeof(DDPIXELFORMAT);

	// Get info on the target surface, so we can create one similar, but in sys mem
	if( GET_DDSRT() )
		GET_DDSRT()->GetSurfaceDesc( &ddsd );
	D3DEnum_DeviceInfo* pDeviceInfo;
	pDeviceInfo = pCMyApp->GetDeviceInfo();

	ddsd.dwFlags          = DDSD_CAPS|DDSD_HEIGHT|DDSD_WIDTH|DDSD_PIXELFORMAT|
							DDSD_TEXTURESTAGE;
	ddsd.ddsCaps.dwCaps   = DDSCAPS_TEXTURE;
	ddsd.ddsCaps.dwCaps2  = 0L;

	// Turn on texture management for hardware devices
	if (pDeviceInfo->bHardware)
		ddsd.ddsCaps.dwCaps2 = DDSCAPS2_TEXTUREMANAGE;
	else
		ddsd.ddsCaps.dwCaps |= DDSCAPS_SYSTEMMEMORY;

	ddsd.dwTextureStage  = m_dwStage;
	ddsd.dwWidth          = m_dwWidth; // Since we are only using this to get a compatible DC,
	ddsd.dwHeight         = m_dwHeight; // there is no need to create anything very large

	HRESULT hr;


	if( !GET_DD7() || FAILED( hr = GET_DD7()->CreateSurface( &ddsd, &m_pddsSmRenderTarget, NULL ) ) )
	{
		OutputDebugString("Couldn't create a temp surface \n");
		return E_FAIL;
	}

	return hr;
}

// CreateSurface()는 m_hbmBitmap 정보를 얻어오기 위해서 만든 것이다.
HRESULT CGoodShadow::CreateSurface()
{
	m_dwStage = 0;

	m_dwFlags = DRGTEXTR_RENDERTARGET;

	// CREATE A TEMP SURFACE IN SYSMEM WITH SIMILAR FORMAT AS THE DEVICE

	// Setup the new surface desc
	DDSURFACEDESC2 ddsd;
	ZeroMemory( &ddsd, sizeof(DDSURFACEDESC2) );
	ddsd.dwSize                 = sizeof(DDSURFACEDESC2);
	ddsd.dwFlags                = 0;
	ddsd.ddsCaps.dwCaps         = 0;
	ddsd.ddpfPixelFormat.dwSize = sizeof(DDPIXELFORMAT);

	// Get info on the target surface, so we can create one similar, but in sys mem
	if( GET_DDSRT() )
		GET_DDSRT()->GetSurfaceDesc( &ddsd );
	ddsd.dwFlags          = DDSD_CAPS|DDSD_HEIGHT|DDSD_WIDTH|DDSD_PIXELFORMAT|
							DDSD_TEXTURESTAGE;
	//ddsd.ddsCaps.dwCaps   = DDSCAPS_TEXTURE|DDSCAPS_SYSTEMMEMORY;
	ddsd.ddsCaps.dwCaps   = DDSCAPS_TEXTURE|DDSCAPS_SYSTEMMEMORY;
	ddsd.ddsCaps.dwCaps2  = 0L;
	ddsd.dwWidth          = 16; // Since we are only using this to get a compatible DC,
	ddsd.dwHeight         = 16; // there is no need to create anything very large

	// Create the temp sysmem surface

	LPDIRECTDRAWSURFACE7 pddsTempSurface;
	HRESULT hr;
	if( !GET_DD7() || FAILED( hr = GET_DD7()->CreateSurface( &ddsd, &pddsTempSurface, NULL ) ) )
	{
		OutputDebugString("Couldn't create a temp surface \n");
		return E_FAIL;
	}


	HDC hdcBitmap;

	//get a DC from the temp surface
	if( pddsTempSurface )
		hr = pddsTempSurface->GetDC(&hdcBitmap);
	if (hr == DD_OK)
	{
		m_hbmBitmap = CreateCompatibleBitmap(  hdcBitmap, SHADOW_TARGET_WIDTH,  SHADOW_TARGET_HEIGHT);
		if( pddsTempSurface )
		{
			pddsTempSurface->ReleaseDC(hdcBitmap);
			pddsTempSurface->Release();
		}
	}

	return S_OK;
}

/*HRESULT CGoodShadow::Restore()
{
	Invalidate();
    // Get the device caps
	D3DDEVICEDESC7 devDesc;
    DWORD         dwDeviceCaps;

	if( FAILED( GET_D3DDEVICE()->GetCaps( &devDesc ) ) )
        return E_FAIL;

	bool bHardware = false;

	if( devDesc.dwDevCaps & D3DDEVCAPS_HWRASTERIZATION)
		bHardware = true;

	dwDeviceCaps = devDesc.dpcTriCaps.dwTextureCaps;

	// Get the bitmap structure (to extract width, height, and bpp)
    BITMAP bm;
    GetObject( m_hbmBitmap, sizeof(BITMAP), &bm );
    DWORD dwWidth  = (DWORD)bm.bmWidth;
    DWORD dwHeight = (DWORD)bm.bmHeight;

    // Setup the new surface desc
    //DDSURFACEDESC2 ddsd;
    ZeroMemory( &m_ddsd, sizeof(DDSURFACEDESC2) );
    m_ddsd.dwSize                 = sizeof(DDSURFACEDESC2);
    m_ddsd.dwFlags                = 0;
    m_ddsd.ddsCaps.dwCaps         = 0;
    m_ddsd.ddpfPixelFormat.dwSize = sizeof(DDPIXELFORMAT);
    m_ddsd.dwFlags         = DDSD_CAPS|DDSD_HEIGHT|DDSD_WIDTH|
                           DDSD_PIXELFORMAT|DDSD_TEXTURESTAGE;//|DDSD_ALPHABITDEPTH;

	//we can't use DX texture management if we are doing target textures, or trying to force in agp
    if (m_dwFlags & (DRGTEXTR_RENDERTARGET | DRGTEXTR_FORCEINAGP))
	{
		//if trying to create a texture you can render to on a 3Dfx (should handle this better at some point)
		if ((devDesc.dwDevCaps & D3DDEVCAPS_SEPARATETEXTUREMEMORIES) && (m_dwFlags & DRGTEXTR_RENDERTARGET))
			OutputDebugString("WARNING! DEV HAS SEPARATE TEXTURE MEMORIES. CREATING TARGET TEXTURE LIKELY WONT WORK\n");

		if (!(devDesc.dwDevCaps & D3DDEVCAPS_TEXTURENONLOCALVIDMEM))
			OutputDebugString("WARNING! TRYING TO FORCE A TEXTURE INTO AGP ON A DEVICE THAT CANT TEXTURE FROM THERE. WATCH OUT\n");

		if (bHardware)
			m_ddsd.ddsCaps.dwCaps  = (DDSCAPS_TEXTURE | DDSCAPS_VIDEOMEMORY);
		else
			m_ddsd.ddsCaps.dwCaps  = (DDSCAPS_TEXTURE | DDSCAPS_SYSTEMMEMORY);

		if (m_dwFlags & DRGTEXTR_RENDERTARGET)
			m_ddsd.ddsCaps.dwCaps |= DDSCAPS_3DDEVICE;

		if (m_dwFlags & DRGTEXTR_FORCEINAGP)
			m_ddsd.ddsCaps.dwCaps |= DDSCAPS_NONLOCALVIDMEM;
	}
	else
	{
		m_ddsd.ddsCaps.dwCaps  = DDSCAPS_TEXTURE;
		m_ddsd.ddsCaps.dwCaps2 = DDSCAPS2_TEXTUREMANAGE;

		if (bHardware == false)
		{
			m_ddsd.ddsCaps.dwCaps  = (DDSCAPS_TEXTURE | DDSCAPS_SYSTEMMEMORY);
			m_ddsd.ddsCaps.dwCaps2 = 0;
		}

	}

	//now handle creation of a bump map
	BOOL bUseBumpDUDV = false;
	if (m_dwFlags & DRGTEXTR_CREATEBUMPMAP_DUDV)
	{
		bUseBumpDUDV = true;
		m_ddsd.ddpfPixelFormat.dwFlags = (m_ddsd.ddpfPixelFormat.dwFlags | DDPF_BUMPDUDV);
		m_ddsd.ddpfPixelFormat.dwBumpBitCount          = 16;
		m_ddsd.ddpfPixelFormat.dwBumpDuBitMask         = 0x000000ff;
		m_ddsd.ddpfPixelFormat.dwBumpDvBitMask         = 0x0000ff00;
		m_ddsd.ddpfPixelFormat.dwBumpLuminanceBitMask  = 0x00000000;

		if (bHardware)
			m_ddsd.ddsCaps.dwCaps  |= DDSCAPS_VIDEOMEMORY;
		else
			m_ddsd.ddsCaps.dwCaps  |= DDSCAPS_SYSTEMMEMORY;

		m_ddsd.ddsCaps.dwCaps2 = 0;//DDSCAPS2_TEXTUREMANAGE;
	}

	//caution: The dwStage member doesn't matter on a lot of HW, but needs to be set explicitly
	// on HW where textures are stored in separate memory spaces depending on stage (3Dfx
	// Voodoo 2)for example)
    m_ddsd.dwTextureStage  = m_dwStage;
    m_ddsd.dwWidth         = dwWidth;
    m_ddsd.dwHeight        = dwHeight;

    // Adjust width and height, if the driver requires it...

	//   for 'requires power of 2'
    if( dwDeviceCaps & D3DPTEXTURECAPS_POW2 )
    {
        for( m_ddsd.dwWidth=1;  dwWidth>m_ddsd.dwWidth;   m_ddsd.dwWidth<<=1 );
        for( m_ddsd.dwHeight=1; dwHeight>m_ddsd.dwHeight; m_ddsd.dwHeight<<=1 );
    }

	//   for 'requires square texture'
    if( dwDeviceCaps & D3DPTEXTURECAPS_SQUAREONLY )
    {
        if( m_ddsd.dwWidth > m_ddsd.dwHeight ) m_ddsd.dwHeight = m_ddsd.dwWidth;
        else                               m_ddsd.dwWidth  = m_ddsd.dwHeight;
    }

	//   for 'I tried to create a texture bigger than the HW supports and I can't get up'
	if ((devDesc.dwMaxTextureWidth < dwWidth) || (devDesc.dwMaxTextureHeight < dwHeight))
	{
		while(devDesc.dwMaxTextureWidth < dwWidth)
			dwWidth>>=1;
		while(devDesc.dwMaxTextureHeight < dwHeight)
			dwHeight>>=1;
		m_ddsd.dwWidth         = dwWidth;
		m_ddsd.dwHeight        = dwHeight;
	}

	BOOL bUsePalette = ( bm.bmBitsPixel <= 8 );
	BOOL bUseAlpha   = FALSE;

	if( m_dwFlags & (DRGTEXTR_TRANSPARENTWHITE|DRGTEXTR_TRANSPARENTBLACK) )
	{
		if( bUsePalette )
		{
			if( dwDeviceCaps & D3DPTEXTURECAPS_ALPHAPALETTE )
			{
				bUseAlpha   = TRUE;
				bUsePalette = TRUE;
			}
		}
		else
		{
			bUseAlpha   = TRUE;
			bUsePalette = FALSE;
		}
	}

	// Setup the structure to be used for texture enumeration.
	TEXTURESEARCHINFO tsi;
	tsi.pddpf            = &m_ddsd.ddpfPixelFormat;
	tsi.bUseAlpha        = bUseAlpha;
	tsi.bUsePalette      = bUsePalette;
	tsi.bUseFourCC       = ( m_ddsd.ddpfPixelFormat.dwFlags & DDPF_FOURCC );
	tsi.bUseBumpDUDV	 = bUseBumpDUDV;
	tsi.dwDesiredBPP     = 16;
	tsi.bFoundGoodFormat = FALSE;
	tsi.dwDesiredAlphaBPP = 0;

	if( m_dwFlags & DRGTEXTR_32BITSPERPIXEL )
	{
		m_ddsd.dwAlphaBitDepth = 8;
		tsi.dwDesiredBPP = 32;
		tsi.dwDesiredAlphaBPP = 8;
	}

	// Enumerate the texture formats, and find the closest device-supported
	// texture pixel format
    if( GET_D3DDEVICE() )
		GET_D3DDEVICE()->EnumTextureFormats( TextureSearchCallback, &tsi );

	//if we tried to create a 32bpp and it didn't work, try 16... (3Dfx, PowerVR PCX2)
	if (( m_dwFlags & DRGTEXTR_32BITSPERPIXEL ) && (tsi.bFoundGoodFormat == false))
	{
		tsi.dwDesiredBPP = 16;
		if(tsi.dwDesiredAlphaBPP == 8)
			tsi.dwDesiredAlphaBPP = 4;
		m_dwFlags -= DRGTEXTR_32BITSPERPIXEL;
		if( GET_D3DDEVICE() )
			GET_D3DDEVICE()->EnumTextureFormats( TextureSearchCallback, &tsi );
	}

	// If a palettized format was requested, but not found, default to a
	// 16-bit texture format
	if( FALSE == tsi.bFoundGoodFormat && bUsePalette )
	{
		tsi.bUsePalette  = FALSE;
		tsi.dwDesiredBPP = 16;
	    if( GET_D3DDEVICE() )
			GET_D3DDEVICE()->EnumTextureFormats( TextureSearchCallback, &tsi );
		if( FALSE == tsi.bFoundGoodFormat )
			return E_FAIL;
	}

	// Create a new surface for the texture
    HRESULT hr;
    if( !GET_DD7() || FAILED( hr = GET_DD7()->CreateSurface( &m_ddsd, &m_pddsSmRenderTarget, NULL ) ) )
	{
	    if (m_dwFlags & DRGTEXTR_RENDERTARGET)
		{
			OutputDebugString("CreateSurface failed with RenderTarget flags\n");
			OutputDebugString("likely not supported on this device\n");
		}
        return E_FAIL;
	}

	//If we are rendering to a texture, and we are using a Z buffer with the main render
	// target, we need to attach a Zbuffer to the texture
	if (m_dwFlags & DRGTEXTR_RENDERTARGET)
	{
		LPDIRECTDRAWSURFACE7	pBackBuffer;	// ptr to back buffer
		LPDIRECTDRAWSURFACE7	pZBuffer;		// ptr to Z buffer
		LPDIRECTDRAWSURFACE7	pTextureZBuffer; // surface that will be created and attached
		DDSCAPS2				SurfCaps;
		DDSURFACEDESC2			SurfDesc;		//the desc of the Z buffer we will create
		DDSURFACEDESC2			ZbuffDesc;		//the desc of the Z buffer of the back buffer

		SurfCaps.dwCaps = DDSCAPS_ZBUFFER;
		SurfCaps.dwCaps2 = SurfCaps.dwCaps3 = SurfCaps.dwCaps4 = 0;

		if( GET_D3DDEVICE() )
			hr = GET_D3DDEVICE()->GetRenderTarget(&pBackBuffer);

		if( GET_DDSRT() )
			hr = GET_DDSRT()->GetAttachedSurface(&SurfCaps,&pZBuffer);

		ZeroMemory( &ZbuffDesc, sizeof( DDSURFACEDESC2 ) );
		ZbuffDesc.dwSize = sizeof( DDSURFACEDESC2 );

		if( pZBuffer )
			hr = pZBuffer->GetSurfaceDesc(&ZbuffDesc);

		ZeroMemory( &SurfDesc, sizeof( DDSURFACEDESC2 ) );
		SurfDesc.dwSize = sizeof( DDSURFACEDESC2 );

		//set the flags
		SurfDesc.dwFlags = DDSD_WIDTH | DDSD_HEIGHT | DDSD_CAPS | DDSD_PIXELFORMAT;

		//get the device info and z info, and pixel format from the current z buffer
		//SurfDesc.ddsCaps.dwCaps = ZbuffDesc.ddsCaps.dwCaps; //this will specify Z buffer, and sysmem or vidmem
		SurfDesc.ddsCaps.dwCaps = DDSCAPS_ZBUFFER | DDSCAPS_VIDEOMEMORY;
		memcpy( &SurfDesc.ddpfPixelFormat, &ZbuffDesc.ddpfPixelFormat, sizeof( DDPIXELFORMAT ) );

		//get the size from the texture surface desc
		SurfDesc.dwWidth = m_ddsd.dwWidth;
		SurfDesc.dwHeight = m_ddsd.dwHeight;

		//now we should have a z buffer description of the right size and format
		// and we can create the surface and attach it.

		if( GET_DD7() )
			hr = GET_DD7()->CreateSurface( &SurfDesc, &pTextureZBuffer, NULL );

		if (FAILED(hr))
			MessageBox(NULL, "GET_DD7()->CreateSurface( &SurfDesc, &pTextureZBuffer, NULL )", "GoodShadow", MB_OK);

		if( m_pddsSmRenderTarget )
			hr = m_pddsSmRenderTarget->AddAttachedSurface( pTextureZBuffer );

		if (FAILED(hr))
			MessageBox(NULL, "m_pddsSmRenderTarget->AddAttachedSurface( pTextureZBuffer )", "GoodShadow", MB_OK);

		OutputDebugString("!");
		if( pTextureZBuffer )
			pTextureZBuffer->Release();
		if( pBackBuffer )
			pBackBuffer->Release();
		if( pZBuffer )
			pZBuffer->Release();

	}

	return CopyBitmapToSurface( m_pddsSmRenderTarget, m_hbmBitmap, NULL, m_dwFlags );
}
*/

#define MyGetPixel( bits, x, y, width, height ) \
	(COLORREF)((*(bits + (height-y-1) * ((width*3+3)&~3) + x*3) ) << 16) | \
	((*(bits + (height-y-1) * ((width*3+3)&~3) + x*3+1) ) <<8) | \
	((*(bits + (height-y-1) * ((width*3+3)&~3) + x*3+2) ) )

HRESULT CGoodShadow::CopyBitmapToSurface( LPDIRECTDRAWSURFACE7 pddsTarget,
		HBITMAP hbmBitmap,
		HBITMAP hbmAlphaBitmap,
		DWORD dwFlags )
{
	LPDIRECTDRAW7 pDD;
	if( pddsTarget )
		pddsTarget->GetDDInterface( (VOID**)&pDD );

	// Get the bitmap structure (to extract width, height, and bpp)
	BITMAP bm;
	GetObject( hbmBitmap, sizeof(BITMAP), &bm );

	//do the same for the alpha bitmap, if there is one
	BITMAP bmAlpha;
	if (hbmAlphaBitmap)
	{
		GetObject( hbmAlphaBitmap, sizeof(BITMAP), &bmAlpha );
		if ((bmAlpha.bmWidth != bm.bmWidth) || (bmAlpha.bmHeight != bm.bmHeight))
		{
			OutputDebugString ("color and alpha bitmaps not the same size (this isn't supported yet \n");
			return NULL;
		} //end if not same size
	} //end if Alpha bitmap exists


	// Setup the new surface desc
	DDSURFACEDESC2 ddsd;
	ZeroMemory( &ddsd, sizeof(DDSURFACEDESC2) );
	ddsd.dwSize                 = sizeof(DDSURFACEDESC2);

	// get info on the target surface, so we can create one similar, but in sys mem
	pddsTarget->GetSurfaceDesc( &ddsd );
	ddsd.dwFlags          = DDSD_CAPS|DDSD_HEIGHT|DDSD_WIDTH|DDSD_PIXELFORMAT|
							DDSD_TEXTURESTAGE;
	ddsd.ddsCaps.dwCaps   = DDSCAPS_TEXTURE|DDSCAPS_SYSTEMMEMORY;
	ddsd.ddsCaps.dwCaps2  = 0L;
	ddsd.dwWidth          = bm.bmWidth;
	ddsd.dwHeight         = bm.bmHeight;

	// Create a new surface for the texture
	LPDIRECTDRAWSURFACE7 pddsTempSurface;
	HRESULT hr;
	if( !pDD || FAILED( hr = pDD->CreateSurface( &ddsd, &pddsTempSurface, NULL ) ) )
		return NULL;

	// Get a Device Context for the bitmap. We'll need it for blitting
	HDC hdcBitmap = CreateCompatibleDC( NULL );
	if( NULL == hdcBitmap )
	{
		pddsTempSurface->Release();
		return NULL;
	}
	//KP SelectObject( hdcBitmap, hbmBitmap );

	HDC hdcAlphaBitmap = CreateCompatibleDC( NULL );
	if (hbmAlphaBitmap)
	{
		// and get a DC for the alpha bitmap
		if( NULL == hdcAlphaBitmap )
		{
			if( pddsTempSurface )
				pddsTempSurface->Release();
			return NULL;
		}
		//KP SelectObject( hdcAlphaBitmap, hbmAlphaBitmap );
	}

	// Handle palettized textures. Need to attach a palette
	if( ddsd.ddpfPixelFormat.dwRGBBitCount == 8 )
	{
		// Create the color table and parse in the palette
		DWORD pe[256];
		WORD  wNumColors = GetDIBColorTable( hdcBitmap, 0, 256,
											 (RGBQUAD*)pe );
		for( WORD i=0; i<wNumColors; i++ )
		{
			pe[i] = 0xff000000 + RGB( GetBValue(pe[i]), GetGValue(pe[i]),
									  GetRValue(pe[i]) );

			// Set alpha for transparent pixels
			if( dwFlags & DRGTEXTR_TRANSPARENTBLACK )
			{
				if( (pe[i]&0x00ffffff) == 0x00000000 )
					pe[i] &= 0x00ffffff;
			}
			else if( dwFlags & DRGTEXTR_TRANSPARENTWHITE )
			{
				if( (pe[i]&0x00ffffff) == 0x00ffffff )
					pe[i] &= 0x00ffffff;
			}
		}
		// Create & attach a palette with the bitmap's colors
		LPDIRECTDRAWPALETTE  pPalette;
		if( pDD )
		{
			if( dwFlags & (DRGTEXTR_TRANSPARENTWHITE|DRGTEXTR_TRANSPARENTBLACK) )
				pDD->CreatePalette( DDPCAPS_8BIT|DDPCAPS_ALPHA, (PALETTEENTRY*)pe, &pPalette, NULL );
			else
				pDD->CreatePalette( DDPCAPS_8BIT, (PALETTEENTRY*)pe, &pPalette, NULL );
		}
		if( pddsTempSurface )
			pddsTempSurface->SetPalette( pPalette );
		if( pddsTarget )
			pddsTarget->SetPalette( pPalette );
		SAFE_RELEASE( pPalette );
	}

	//don't need this anymore
	if( pDD )
		pDD->Release();


	//For some reason, I can't ge a DC to the surface to do a GDI blit,
	// so I am going to copy the bits over. Right now I am using a
	// GDI GetPixel() to get the RGB's from the bitmap, but I should
	// replace that for obvious performance reasons

	DDSURFACEDESC2 lpSurfDesc;
	ZeroMemory( &lpSurfDesc, sizeof(DDSURFACEDESC2) );
	lpSurfDesc.dwSize                 = sizeof(DDSURFACEDESC2);
	lpSurfDesc.dwFlags                = 0;
	lpSurfDesc.ddsCaps.dwCaps         = 0;
	lpSurfDesc.ddpfPixelFormat.dwSize = sizeof(DDPIXELFORMAT);

	//lock the surface so we can start writing bits to it
	while( pddsTempSurface->Lock( NULL, &lpSurfDesc, 0, NULL ) ==
			DDERR_WASSTILLDRAWING );

	if ((lpSurfDesc.ddpfPixelFormat.dwRGBBitCount != 16 ) &&
			(lpSurfDesc.ddpfPixelFormat.dwRGBBitCount != 32 ))
	{
		if ((lpSurfDesc.ddpfPixelFormat.dwFlags & DDPF_BUMPLUMINANCE) &&
				(lpSurfDesc.ddpfPixelFormat.dwRGBBitCount != 24 ))
			// Yikes! not 16 or 32, or 24bit DuDvL, we shouldn't be here!
			OutputDebugString("Unknown format. proceed with caution\n");
	}

	//calculate shift increments (clever shift stuff courtesy of the MS dudes
	DWORD dwRMask = lpSurfDesc.ddpfPixelFormat.dwRBitMask;
	DWORD dwGMask = lpSurfDesc.ddpfPixelFormat.dwGBitMask;
	DWORD dwBMask = lpSurfDesc.ddpfPixelFormat.dwBBitMask;
	DWORD dwAMask = lpSurfDesc.ddpfPixelFormat.dwRGBAlphaBitMask;

	//we'll need these if this is a bump map
	DWORD dwDuMask = lpSurfDesc.ddpfPixelFormat.dwBumpDuBitMask;
	DWORD dwDvMask = lpSurfDesc.ddpfPixelFormat.dwBumpDvBitMask;
	DWORD dwLumMask = lpSurfDesc.ddpfPixelFormat.dwBumpLuminanceBitMask;


	DWORD dwRShiftL = 8, dwRShiftR = 0;
	DWORD dwGShiftL = 8, dwGShiftR = 0;
	DWORD dwBShiftL = 8, dwBShiftR = 0;
	DWORD dwAShiftL = 8, dwAShiftR = 0;

	//again, these are only needed for bump mapping
	DWORD dwDuShiftL = 8, dwDuShiftR = 0;
	DWORD dwDvShiftL = 8, dwDvShiftR = 0;
	DWORD dwLumShiftL = 8, dwLumShiftR = 0;

	DWORD dwMask;
	for( dwMask=dwRMask; dwMask && !(dwMask&0x1); dwMask>>=1 ) dwRShiftR++;
	for( ; dwMask; dwMask>>=1 ) dwRShiftL--;

	for( dwMask=dwGMask; dwMask && !(dwMask&0x1); dwMask>>=1 ) dwGShiftR++;
	for( ; dwMask; dwMask>>=1 ) dwGShiftL--;

	for( dwMask=dwBMask; dwMask && !(dwMask&0x1); dwMask>>=1 ) dwBShiftR++;
	for( ; dwMask; dwMask>>=1 ) dwBShiftL--;

	for( dwMask=dwAMask; dwMask && !(dwMask&0x1); dwMask>>=1 ) dwAShiftR++;
	for( ; dwMask; dwMask>>=1 ) dwAShiftL--;

	//again, some stuff for bump mapping
	DWORD dwBumpMask;
	for( dwBumpMask=dwDuMask; dwBumpMask && !(dwBumpMask&0x1); dwBumpMask>>=1 ) dwDuShiftR++;
	for( ; dwBumpMask; dwBumpMask>>=1 ) dwDuShiftL--;

	for( dwBumpMask=dwDvMask; dwBumpMask && !(dwBumpMask&0x1); dwBumpMask>>=1 ) dwDvShiftR++;
	for( ; dwBumpMask; dwBumpMask>>=1 ) dwDvShiftL--;

	for( dwBumpMask=dwLumMask; dwBumpMask && !(dwBumpMask&0x1); dwBumpMask>>=1 ) dwLumShiftR++;
	for( ; dwBumpMask; dwBumpMask>>=1 ) dwLumShiftL--;

	unsigned char *pBits, *pAlphaBits;
	BITMAPINFO bi;
	ZeroMemory( &bi, sizeof(  BITMAPINFO ) );
	bi.bmiHeader.biSize = sizeof( BITMAPINFOHEADER );
	bi.bmiHeader.biWidth = lpSurfDesc.dwWidth;
	bi.bmiHeader.biHeight = lpSurfDesc.dwHeight;
	bi.bmiHeader.biPlanes = 1;
	bi.bmiHeader.biBitCount = 24;
	bi.bmiHeader.biCompression = BI_RGB;
	bi.bmiHeader.biSizeImage = ((bi.bmiHeader.biWidth * bi.bmiHeader.biBitCount/8 + 3) & ~3) * bi.bmiHeader.biHeight;
	pBits = new unsigned char[ ((bi.bmiHeader.biWidth * bi.bmiHeader.biBitCount/8 + 3) & ~3) * bi.bmiHeader.biHeight ];
	GetDIBits( hdcBitmap, hbmBitmap, 0, lpSurfDesc.dwHeight, pBits, &bi, DIB_RGB_COLORS );
	if( hdcAlphaBitmap )
	{
		pAlphaBits = new unsigned char[ ((bi.bmiHeader.biWidth * bi.bmiHeader.biBitCount/8 + 3) & ~3) * bi.bmiHeader.biHeight ];
		GetDIBits( hdcAlphaBitmap, hbmAlphaBitmap, 0, lpSurfDesc.dwHeight, pAlphaBits, &bi, DIB_RGB_COLORS );
	}


	for( DWORD y=0; y<lpSurfDesc.dwHeight; y++ )
	{
		WORD*  p16 =  (WORD*)((BYTE*)lpSurfDesc.lpSurface + y*lpSurfDesc.lPitch);
		DWORD* p32 = (DWORD*)((BYTE*)lpSurfDesc.lpSurface + y*lpSurfDesc.lPitch);

		for( DWORD x=0; x<lpSurfDesc.dwWidth; x++ )
		{
			//Get the color info from the bitmap
			COLORREF tempcolor = MyGetPixel(  pBits, x, y, lpSurfDesc.dwWidth, lpSurfDesc.dwHeight);
			DWORD r = tempcolor       & 0x000000ff;
			DWORD g = (tempcolor>>8)  & 0x000000ff;
			DWORD b = (tempcolor>>16) & 0x000000ff;
			DWORD a = 0x000000ff;

			if (lpSurfDesc.ddpfPixelFormat.dwRGBAlphaBitMask) //if alpha
			{
				if( dwFlags & (DRGTEXTR_TRANSPARENTWHITE|DRGTEXTR_TRANSPARENTBLACK) )
				{
					if ((dwFlags & DRGTEXTR_TRANSPARENTWHITE) &&
							(tempcolor == 0x00ffffff))
						a = 0x00000000;
					else
					{
						if ((dwFlags & DRGTEXTR_TRANSPARENTBLACK) &&
								(tempcolor == 0x00000000))
							a = 0x00000000;
						else
							a = 0x000000ff;
					}
				} //end if white/black, which means there must have been
				// an alpha bitmap supplied, or only format avail. had alpha
				else
				{
					if (hdcAlphaBitmap)
					{
						COLORREF tempalpha = MyGetPixel(  pAlphaBits, x, y, lpSurfDesc.dwWidth, lpSurfDesc.dwHeight);
						a = tempalpha & 0x000000ff;
					}
					// else a remains 0x000000ff
				}
			} //end if alpha

			if (dwFlags & DRGTEXTR_CREATEINVHEIGHTMAP) //if doing embossing
			{
				r = 0x00000080 - r;
				g = 0x00000080 - g;
				b = 0x00000080 - b;
			}

			DWORD dr = ((r>>(dwRShiftL))<<dwRShiftR)&dwRMask;
			DWORD dg = ((g>>(dwGShiftL))<<dwGShiftR)&dwGMask;
			DWORD db = ((b>>(dwBShiftL))<<dwBShiftR)&dwBMask;
			DWORD da = ((a>>(dwAShiftL))<<dwAShiftR)&dwAMask;

			if( 32 == lpSurfDesc.ddpfPixelFormat.dwRGBBitCount )
				p32[x] = (DWORD)(dr+dg+db+da);
			else
				p16[x] = (WORD)(dr+dg+db+da);

			//have to override the above for any 'special' formats we want to support
			if (lpSurfDesc.ddpfPixelFormat.dwFlags & DDPF_BUMPDUDV)
			{
				DWORD du = ((r>>(dwDuShiftL))<<dwDuShiftR)&dwDuMask;
				DWORD dv = ((g>>(dwDvShiftL))<<dwDvShiftR)&dwDvMask;
				DWORD lum = ((b>>(dwLumShiftL))<<dwLumShiftR)&dwLumMask;
				if( 32 == lpSurfDesc.ddpfPixelFormat.dwRGBBitCount )
					p32[x] = (DWORD)(du+dv+lum);
				else
					p16[x] = (WORD)(du+dv+lum);
			} //end of override for bump mapping
		}//end for x=width
		p16++;
		p32++;
	} //end for y=height

	if( pddsTempSurface )
		pddsTempSurface->Unlock( NULL );

	//now blit the temp surface to the vidmem surface
	if( pddsTarget )
		pddsTarget->Blt( NULL, pddsTempSurface, NULL, DDBLT_WAIT, NULL );

	//be sure to clean up before you leave
	delete [] pBits;
	if (hdcAlphaBitmap)
		delete [] pAlphaBits;

	if( pddsTempSurface )
		pddsTempSurface->Release();

	DeleteDC( hdcBitmap );
	DeleteDC( hdcAlphaBitmap );

	return DD_OK;
}


//-----------------------------------------------------------------------
// DeleteRes()
// Desc : 지우고 싶은 것 대충 대충 지우자.
//-----------------------------------------------------------------------
void CGoodShadow::Invalidate()
{
	SAFE_RELEASE(m_pddsSmRenderTarget);
}

#define	NIGHT_X (20)
#define NIGHT_Y (58)
float light_hour_x[30] = { NIGHT_X,  NIGHT_X,  NIGHT_X,  NIGHT_X,  NIGHT_X,
						   //       0,        1,        2,        3,        4,
						   50, 45, 40, 35, 30, 25, 20, 15, 10,  5,
						   //   5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
						   -0.1f, -5, -10, -15, -20, -25, -30, -37, -44, -50,
						   //     15, 16,  17,  18,  19,  20,  21,  22,  23,  24,
						   -NIGHT_X,  -NIGHT_X,  -NIGHT_X,  -NIGHT_X,  -NIGHT_X
						 };
//        25,        26,        27,        28,        29
float light_hour_y[30] = {NIGHT_Y, NIGHT_Y, NIGHT_Y, NIGHT_Y, NIGHT_Y,
						  //      0,       1,       2,       3,       4,
						  45, 47, 49, 51, 54, 56, 58, 60, 63, 65,
						  //   5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
						  67, 65, 63, 60, 58, 56, 54, 51, 48, 45,
						  //  15, 16, 17, 18, 19, 20, 21, 22, 23, 24,
						  NIGHT_Y, NIGHT_Y, NIGHT_Y, NIGHT_Y, NIGHT_Y
						 };
//       25,      26,      27,      28,      29

BOOL CGoodShadow::Start(D3DMATRIX *matRotate, int cur_hour)
{
	if (!GET_D3DDEVICE() || !m_pddsSmRenderTarget)
		return FALSE;

	cur_hour %= 30;
	//cur_hour = 5;

	D3DMATRIX matWorld, matView, matProjNew;
	D3DVECTOR vEyePt, vLookatPt, vUpVec;
	//LPDIRECTDRAWSURFACE7 lpRenderTarget;
	D3DVIEWPORT7 vpNew;
	//float fTimeFrame;
	//DWORD	dwColor;
	D3DMATERIAL7 mtrl;

	DWORD dwPass = 0;
	HRESULT hr = GET_D3DDEVICE()->ValidateDevice(&dwPass);
	if( hr != S_OK )
	{
		char chkValid[MAX_PATH] = {0,};
		sprintf(chkValid, "DEVICE VALID ERROR : %X\n", hr);
		OutputDebugString(chkValid);
		Sleep(20);

		return FALSE;
	}

	// 정원 작업.
	if ( !m_pSMRender || FAILED(GET_D3DDEVICE()->SetRenderTarget(m_pSMRender->GetRenderTarget(), 0)))
	{
		GET_D3DDEVICE()->SetRenderTarget(m_wasLpRenderTarget, 0);
		if( m_wasLpRenderTarget )
			m_wasLpRenderTarget->Release();			// 중요함.. 중요하쥐..
		Invalidate();
		return FALSE;
	}
	// 정원 작업 긑

	// 머티리얼 설정
	D3DUtil_InitMaterial( mtrl, 1.0f, 1.0f, 1.0f );
	GET_D3DDEVICE()->SetMaterial(&mtrl);

	// 현재의 뷰포트를 저장하고 새롭게 뷰포트를 설정한다.
	GET_D3DDEVICE()->GetViewport(&m_vpOld);
	vpNew = m_vpOld;
	vpNew.dwWidth = SHADOW_TARGET_WIDTH;
	vpNew.dwHeight = SHADOW_TARGET_HEIGHT;
	GET_D3DDEVICE()->SetViewport(&vpNew);

	// 프로젝션 매트릭스 수정.
	float fAspect = (float) ((double) vpNew.dwHeight / (double) vpNew.dwWidth);
	GET_D3DDEVICE()->GetTransform(D3DTRANSFORMSTATE_PROJECTION, &matProjOld);
	D3DUtil_SetProjectionMatrix(matProjNew, 0.8f, fAspect, 1.0f, 500.0f );
	GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_PROJECTION, &matProjNew);

	// 월드 트랜스폼
	matWorld = *matRotate;
	matWorld._41 = matWorld._42 = matWorld._43 = 0.0f;
	//D3DUtil_SetIdentityMatrix(matWorld);
	GET_D3DDEVICE()->GetTransform(D3DTRANSFORMSTATE_WORLD, &m_matWorldOld );
	GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_WORLD, &matWorld );

	vEyePt = D3DVECTOR(light_hour_x[cur_hour], light_hour_y[cur_hour], 0);
	vLookatPt = D3DVECTOR(0, 0, 0);
	vUpVec = D3DVECTOR(0, 1, 0);

	D3DUtil_SetViewMatrix(matView,  vEyePt,     vLookatPt, vUpVec );
	GET_D3DDEVICE()->GetTransform(D3DTRANSFORMSTATE_VIEW, &m_matViewOld );
	GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_VIEW, &matView );
	// 라이팅
	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_AMBIENT, &m_dwColor );
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_AMBIENT, 0xff000000);
	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_LIGHTING, &m_lightingOld);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_LIGHTING, TRUE);
	GET_D3DDEVICE()->GetLightEnable(0, &m_lightEnable0);
	GET_D3DDEVICE()->GetLightEnable(1, &m_lightEnable1);
	GET_D3DDEVICE()->GetLightEnable(2, &m_lightEnable2);
	GET_D3DDEVICE()->LightEnable(0, FALSE);
	GET_D3DDEVICE()->LightEnable(1, FALSE);
	GET_D3DDEVICE()->LightEnable(2, FALSE);
	GET_D3DDEVICE()->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00FFFFFF, 1.0f, 0L );

	// 뷰포트를 더 작게 셋팅.. 그림자 늘어나는 현상 방지!
	vpNew.dwWidth = SHADOW_TARGET_WIDTH-2;
	vpNew.dwHeight = SHADOW_TARGET_HEIGHT-2;
	vpNew.dwX = 1;
	vpNew.dwY = 1;
	GET_D3DDEVICE()->SetViewport(&vpNew);

	return TRUE;
	//return FALSE;
}

void CGoodShadow::End(BOOL bStarted)
{
	DWORD dwPass = 0;
	HRESULT hr = GET_D3DDEVICE()->ValidateDevice(&dwPass);
	if( hr != S_OK )
	{
		char chkValid[MAX_PATH] = {0,};
		sprintf(chkValid, "DEVICE VALID ERROR : %X\n", hr);
		OutputDebugString(chkValid);
		Sleep(20);

		return;
	}

	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_LIGHTING, TRUE);

	// 원상복귀.
	GET_D3DDEVICE()->SetRenderTarget(pCMyApp->GetRenderTarget(), 0);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_AMBIENT, m_dwColor);
	//m_wasLpRenderTarget->Release();			// 중요함.. 중요하쥐..
	GET_D3DDEVICE()->SetViewport(&m_vpOld);
	GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_PROJECTION, &matProjOld);
	GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_WORLD, &m_matWorldOld );
	GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_VIEW, &m_matViewOld );
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_LIGHTING, m_lightingOld);
	GET_D3DDEVICE()->LightEnable(0, m_lightEnable0);
	GET_D3DDEVICE()->LightEnable(1, m_lightEnable1);
	GET_D3DDEVICE()->LightEnable(2, m_lightEnable2);

	if( m_pddsSmRenderTarget && m_pSMRender )
		HRESULT hr = m_pddsSmRenderTarget->BltFast(0, 0, m_pSMRender->GetRenderTarget(), NULL, 0);
}

// 화면에 보여 줄때에는 렌더링되는 서피스를 보여준다.
// 일부 비디오 카드에서는 텍스처 서피스를 백버퍼에 블릿할 수 없다.
void CGoodShadow::Render2D()
{
	g_pDisplay->Blt(0, 0, m_pSMRender->GetRenderTarget());
}

// 텍스처로 쓰일 서피스를 리턴한다.
LPDIRECTDRAWSURFACE7 CGoodShadow::GetSurface()
{
	return m_pddsSmRenderTarget;
}