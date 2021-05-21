#include "stdafx.h"
#include <tchar.h>
#include <stdlib.h>
#include <algorithm>
#include "IndexedTexture.h"
#include "D3DUtil.h"
#include "VirtualFile.h"
#include "Def.h"

#define WXY_MIP_SCALE 2

extern BOOL g_bWXY_FORMAT;
extern BOOL g_bCsyMipmap;
extern BOOL g_bMipmap;
extern BOOL g_bDungeon;
extern CVirtualFile *g_pvfTextr;

IndexedTexture* g_pCapsyongTexture = NULL;

struct TEXTURESEARCHINFO
{
	DWORD dwDesiredBPP;
	BOOL  bSetAlpha;
	BOOL  bUseAlpha;
	BOOL  bUsePalette;
	BOOL  bFoundGoodFormat;

	DDPIXELFORMAT* pddpf;
};

static HRESULT CALLBACK TextureSearchCallback(DDPIXELFORMAT* pddpf, VOID* param)
{
	if( NULL==pddpf || NULL==param )
		return DDENUMRET_OK;

	TEXTURESEARCHINFO* ptsi = (TEXTURESEARCHINFO*)param;

	// Skip any funky modes
	if( pddpf->dwFlags & (DDPF_LUMINANCE|DDPF_BUMPLUMINANCE|DDPF_BUMPDUDV) )
	{
		return DDENUMRET_OK;
	}

	// Check for palettized formats
	if( ptsi->bUsePalette )
	{
		if( !( pddpf->dwFlags & DDPF_PALETTEINDEXED8 ) )
		{
			return DDENUMRET_OK;
		}

		// Accept the first 8-bit palettized format we get
		memcpy( ptsi->pddpf, pddpf, sizeof(DDPIXELFORMAT) );
		ptsi->bFoundGoodFormat = TRUE;

		return DDENUMRET_CANCEL;
	}

	// Else, skip any paletized formats (all modes under 16bpp)
	if( pddpf->dwRGBBitCount < 16 )
	{
		return DDENUMRET_OK;
	}

	// Skip any FourCC formats
	if( pddpf->dwFourCC != 0 )
	{
		return DDENUMRET_OK;
	}

	// Skip any ARGB 4444 formats (which are best used for pre-authored
	// content designed speciafically for an ARGB 4444 format).		
	if( pddpf->dwRGBAlphaBitMask == 0x0000f000 )
	{
		return DDENUMRET_OK;
	}

	if( ( (ptsi->bUseAlpha) && (pddpf->dwRGBBitCount == 16) ) )
	{
		pddpf->dwRGBAlphaBitMask = 0x0000f000;
		pddpf->dwRBitMask = 0x00000f00;
		pddpf->dwGBitMask = 0x000000f0;
		pddpf->dwBBitMask = 0x0000000f;
		pddpf->dwFlags |= DDPF_ALPHAPIXELS;
	}

	// Make sure current alpha format agrees with requested format type
	if( (ptsi->bUseAlpha==TRUE) && !(pddpf->dwFlags&DDPF_ALPHAPIXELS) )
	{
		return DDENUMRET_OK;
	}

	if( (ptsi->bUseAlpha==FALSE) && (pddpf->dwFlags&DDPF_ALPHAPIXELS) )
	{
		return DDENUMRET_OK;
	}

	// Check if we found a good match
	if( pddpf->dwRGBBitCount == ptsi->dwDesiredBPP )
	{
		memcpy( ptsi->pddpf, pddpf, sizeof(DDPIXELFORMAT) );
		ptsi->bFoundGoodFormat = TRUE;

		return DDENUMRET_CANCEL;
	}

	return DDENUMRET_OK;
}

HBITMAP VLoadImage(HINSTANCE hinst, LPCTSTR lpszName, UINT uType, int cxDesired, int cyDesired, UINT fuLoad)
{
	LPVOID lpBits;

	if( strlen(lpszName) == 0 )
	{
		return NULL;
	}

	PFILE* fp = pfopen(const_cast<char*>(lpszName), "rb");

	if( fp == NULL )
	{
		return NULL;
	}

	BITMAPFILEHEADER bmfHeader;	
	pfread((LPSTR)&bmfHeader, sizeof( BITMAPFILEHEADER  ), 1, fp);
		
	if( bmfHeader.bfType != ((WORD) ('M' << 8) | 'B') )
	{
		pfclose(fp);

		return NULL;
	}

	BITMAPINFO *pbmInfo;
	pbmInfo = (BITMAPINFO *)::GlobalAlloc(GMEM_FIXED, sizeof(BITMAPINFO) + (sizeof(RGBQUAD) * 256));
	
	if (pbmInfo == NULL)
	{
		pfclose( fp );

		return NULL;
	}

	pfread(pbmInfo, sizeof(BITMAPINFO) + (sizeof(RGBQUAD) * 256), 1, fp);

	BITMAPINFO &bmInfo = *pbmInfo;
	HBITMAP hBitmap = NULL;

	hBitmap = CreateDIBSection(NULL, pbmInfo, DIB_RGB_COLORS, &lpBits, NULL, 0);
		
	pfseek(fp, bmfHeader.bfOffBits, SEEK_SET);

	int bytes = (bmInfo.bmiHeader.biWidth * bmInfo.bmiHeader.biBitCount + 15) / 16 * 2;
	bytes *= bmInfo.bmiHeader.biHeight;

	pfread(lpBits, bytes, 1, fp);

	::GlobalFree(pbmInfo);

	pfclose( fp );

	return hBitmap;
}

//////////////////////////////////////////////////////////////////////////

TextureContainer::TextureContainer(std::string strName, DWORD dwStage, DWORD dwFlags, DWORD transAlpha , BOOL packed)
{
	lstrcpy( m_strName, strName.c_str() );
	m_dwWidth     = 0;
	m_dwHeight    = 0;
	m_dwStage     = dwStage;
	m_dwBPP       = 0;
	m_dwFlags     = dwFlags;
	m_bHasAlpha   = 0;

	m_pddsSurface = NULL;
	m_hbmBitmap   = NULL;
	m_pRGBAData   = NULL;

	m_transAlpha  = transAlpha;
	m_hWnd = NULL;
	m_dwMipmapCount = 4;

	m_ppNTXData = NULL;
	m_packed = packed == FALSE ? false : true;
}

TextureContainer::~TextureContainer()
{
	SAFE_RELEASE( m_pddsSurface );
	SAFE_DELETE_ARRAY( m_pRGBAData );
	DeleteObject( m_hbmBitmap );

	if( m_ppNTXData )
	{
		for( DWORD i = 0; i < m_dwMipmapCount; i++ )
		{
			SAFE_DELETE_ARRAY(m_ppNTXData[i]);
		}

		delete [] m_ppNTXData;
	}
}

HRESULT TextureContainer::LoadImageData(const TCHAR* strTexturePath, const TCHAR* strTexturePath2, const TCHAR* strTexturePath3)
{
	m_hbmBitmap = (HBITMAP)LoadImage( GetModuleHandle(NULL), m_strName,	IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION );
	
	if( m_hbmBitmap )
		return S_OK;

	TCHAR* strExtension;
	TCHAR  strPathname[256];
	
	lstrcpy( strPathname, strTexturePath );
	lstrcat( strPathname, m_strName );

	PFILE*  file;

	bool forceStd = ! m_packed;

	if( NULL == ( file = pfopen( strPathname, "rb", forceStd ) ) )
	{
		if (strTexturePath2)
		{
			lstrcpy( strPathname, strTexturePath2 );
			lstrcat( strPathname, m_strName );
			if( NULL == ( file = pfopen( strPathname, "rb", forceStd ) ) )
			{
				if (strTexturePath3)
				{
					lstrcpy( strPathname, strTexturePath3 );
					lstrcat( strPathname, m_strName );
					if( NULL == ( file = pfopen( strPathname, "rb", forceStd ) ) )
					{
						return DDERR_NOTFOUND;
					}
				}
				else
				{
					return DDERR_NOTFOUND;
				}
			}
		}
		else
		{
			return DDERR_NOTFOUND;
		}
	}
	pfclose( file );
	
	
	if( NULL == ( strExtension = _tcsrchr( m_strName, _T('.') ) ) )
		return DDERR_UNSUPPORTED;
	
	lstrcpy(m_strPathName, strPathname);

	return LoadImageData();
}

HRESULT TextureContainer::LoadImageData()
{
	TCHAR* strExtension;
	
	if( NULL == ( strExtension = _tcsrchr( m_strPathName, _T('.') ) ) )
		return DDERR_UNSUPPORTED;
	
	if( !lstrcmpi( strExtension, _T(".bmp") ) )
	{
		return LoadBitmapFile( m_strPathName );
	}
	else if( !lstrcmpi( strExtension, _T(".wxy") ) )
	{
		return LoadWxyFile( m_strPathName );
	}	
	else if( !lstrcmpi( strExtension, _T(".tga") ) )
	{	
		m_dwMipmapCount = 1;
		return LoadTargaFile( m_strPathName );
	}
	else if( !lstrcmpi( strExtension, _T(".sek") ) )
	{
		m_dwMipmapCount = 1;
		return LoadSekFile( m_strPathName );
	}
	else if( !lstrcmpi( strExtension, _T(".ntx") ) )
	{
		return LoadNTXFile( m_strPathName );
	}

	return DDERR_UNSUPPORTED;
}

HRESULT TextureContainer::LoadBitmapFile2( TCHAR* strPathname )
{	
	m_hbmBitmap = (HBITMAP)LoadImage( NULL, strPathname, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE|LR_CREATEDIBSECTION );
	if( m_hbmBitmap )
		return S_OK;

	return DDERR_NOTFOUND;
}

HRESULT TextureContainer::LoadBitmapFile( TCHAR* strPathname, bool bDummy )
{
	LPVOID lpBits;

	bool forceStd = ! m_packed;
	int offsetDummy = 0;
	
	PFILE *fp;
	PFILE* fptemp = new PFILE_NFS();
	bool decode = true;
	
	if(!fptemp->Open(strPathname, "rb"))
	{
		decode = false;
	}
	else
		fptemp->Close();

	if( (fp = pfopen(strPathname, "rb", forceStd)) == NULL )
		return DDERR_NOTFOUND;

	long fileSize = getFileSize(fp);
	unsigned char* stream = new unsigned char[fileSize];
	unsigned char* streamOrg = stream;
	pfread(stream, fileSize, 1, fp);
	if(decode)
		pDecode(stream, fileSize);

	BITMAPFILEHEADER bmfHeader;

	if( bDummy == true )
	{	
		char dummy[10];	
		/*if (*/readData(&dummy, 6, 1, &stream);// <= 0)
		//{
		//	pfclose(fp);
		//	return E_FAIL;
		//}

		offsetDummy = 6;
	}

	/*if (*/readData((LPSTR)&bmfHeader, sizeof(bmfHeader), 1, &stream);// <= 0)
	/*{
		pfclose(fp);
		return E_FAIL;
	}*/

	BITMAPINFO *pbmInfo;
	pbmInfo = (BITMAPINFO *)::GlobalAlloc(GMEM_FIXED, sizeof(BITMAPINFO) + sizeof(RGBQUAD)*256);
	if (pbmInfo == NULL)
	{
		pfclose(fp);
		return E_FAIL;
	}

	readData(pbmInfo, sizeof(BITMAPINFO) + sizeof(RGBQUAD)*256, 1, &stream);

	BITMAPINFO &bmInfo = *pbmInfo;

	m_hbmBitmap = CreateDIBSection(NULL, pbmInfo, DIB_RGB_COLORS, &lpBits, NULL, 0);

	if( lpBits == NULL )
	{
		pfclose(fp);
		return E_FAIL;
	}

	
//	pfseek(fp, bmfHeader.bfOffBits + offsetDummy, SEEK_SET);
	//unsigned char* tempstream = new unsigned char[bmfHeader.bfOffBits + offsetDummy];
	stream = streamOrg + (bmfHeader.bfOffBits + offsetDummy);
	//readData(&tempstream, bmfHeader.bfOffBits + offsetDummy, 1, &stream);

	int bytes = (bmInfo.bmiHeader.biWidth * bmInfo.bmiHeader.biBitCount + 15) / 16 * 2;
	bytes *= bmInfo.bmiHeader.biHeight;

	readData(lpBits, bytes, 1, &stream);

	::GlobalFree(pbmInfo);

	pfclose(fp);
	SAFE_DELETE_ARRAY(streamOrg);

	return S_OK;
}

HRESULT TextureContainer::LoadWxyFile( TCHAR* strPathname )
{
	return LoadBitmapFile(strPathname, true);
}

HRESULT TextureContainer::LoadTargaFile( TCHAR* strPathname, bool bDummy )
{
	bool forceStd = ! m_packed;
	int offsetDummy = 0;

	PFILE* file = pfopen(strPathname, "rb", forceStd);
	if( NULL == file )
		return E_FAIL;

	PFILE* fptemp = new PFILE_NFS();
	bool decode = true;
	
	if(!fptemp->Open(strPathname, "rb"))
	{
		decode = false;
	}
	else
		fptemp->Close();


	long fileSize = getFileSize(file);
	unsigned char* stream = new unsigned char[fileSize];
	unsigned char* streamOrg = stream;
	pfread(stream, fileSize, 1, file);
	if(decode)
		pDecode(stream, fileSize);

	if( bDummy == true )
	{	
		char dummy[10];
		readData( &dummy, 8, 1, &stream );

		offsetDummy = 8;
	}

	TargaHeader tga;
	readData(&tga, sizeof(TargaHeader), 1, &stream );
		
	if( ( 0 != tga.ColormapType ) || ( tga.ImageType != 10 && tga.ImageType != 2 ) )
	{
		pfclose( file );
		return E_FAIL;
	}

	// Skip the ID field. The first byte of the header is the length of this field
	if( tga.IDLength )
	{
		//pfseek( file, tga.IDLength + offsetDummy, SEEK_CUR );
		stream = streamOrg + (tga.IDLength + offsetDummy);
	}

	m_dwWidth   = tga.ImageWidth;
	m_dwHeight  = tga.ImageHeight;
	m_dwBPP     = tga.PixelDepth;
	m_pRGBAData = new DWORD[m_dwWidth * m_dwHeight];	

	//long curPos = pftell(file);
	//pfseek(file, 0, SEEK_END);
	//long endPos = pftell(file);
	//pfseek(file, curPos, SEEK_SET);
	//long remain = endPos - curPos;
	long remain = fileSize - (tga.IDLength + offsetDummy);

	unsigned char* buf = new unsigned char[remain];
	unsigned char* bufOrg = buf;
	readData(buf, remain, 1, &stream);

	pfclose( file );

	for(DWORD y = 0; y < m_dwHeight; ++y)
	{
		DWORD dwOffset = y * m_dwWidth;

		if( 0 == ( tga.ImageDescriptor & 0x0010 ) )
		{
			dwOffset = (m_dwHeight- y - 1) * m_dwWidth;
		}

		for(DWORD x = 0; x < m_dwWidth; )
		{
			if( tga.ImageType == 10 )
			{
				BYTE PacketInfo = *(buf++);
				WORD PacketType = 0x80 & PacketInfo;
				WORD PixelCount = ( 0x007f & PacketInfo ) + 1;

				if( PacketType )
				{
					DWORD b = *(buf++);
					DWORD g = *(buf++);
					DWORD r = *(buf++);
					DWORD a = 0xff;

					if( m_dwBPP == 32 )
					{
						a = *(buf++);
					}

					while( PixelCount-- )
					{
						m_pRGBAData[dwOffset + x] = (r << 24L) + (g << 16L) + (b << 8L) + (a);
						++x;
					}
				}
				else
				{
					while( PixelCount-- )
					{
						BYTE b = *(buf++);
						BYTE g = *(buf++);
						BYTE r = *(buf++);
						BYTE a = 0xff;

						if( m_dwBPP == 32 )
						{
							a = *(buf++);
						}

						m_pRGBAData[dwOffset + x] = (r << 24L) + (g << 16L) + (b << 8L) + (a);
						++x;
					}
				}
			}
			else
			{
				BYTE b = *(buf++);
				BYTE g = *(buf++);
				BYTE r = *(buf++);
				BYTE a = 0xff;

				if( m_dwBPP == 32 )
				{
					a = *(buf++);
				}

				m_pRGBAData[dwOffset + x] = (r << 24L) + (g << 16L) + (b << 8L) + (a);
				x++;
			}
		}
	}

	SAFE_DELETE_ARRAY(bufOrg);
	SAFE_DELETE_ARRAY(streamOrg);

	m_bHasAlpha = TRUE;

	return S_OK;
}

HRESULT TextureContainer::LoadSekFile( TCHAR* strPathname )
{
	return LoadTargaFile(strPathname, true);
}

HRESULT TextureContainer::LoadNTXFile( TCHAR* strPathname )
{
	bool forceStd = ! m_packed;
	
	PFILE*	fp = pfopen(strPathname, "rb", forceStd);
	if( fp == NULL )
		return E_FAIL;
	PFILE* fptemp = new PFILE_NFS();
	bool decode = true;
	
	if(!fptemp->Open(strPathname, "rb"))
	{
		decode = false;
	}
	else
		fptemp->Close();

	if( (fp = pfopen(strPathname, "rb", forceStd)) == NULL )
		return DDERR_NOTFOUND;

	long fileSize = getFileSize(fp);
	unsigned char* stream = new unsigned char[fileSize];
	unsigned char* streamOrg = stream;
	pfread(stream, fileSize, 1, fp);
	if(decode)
		pDecode(stream, fileSize);

	DWORD	dwMagic;
	readData( &dwMagic, sizeof(DWORD), 1, &stream );
	if( dwMagic != ( 'N' << 24 | 'T' << 16 | 'X' << 8 | ' ' ) )
	{
		pfclose( fp );
		return E_FAIL;
	}
		
	readData( &m_dwWidth, sizeof(DWORD), 1, &stream );
	readData( &m_dwHeight, sizeof(DWORD), 1, &stream );
	readData( &m_dwBPP, sizeof(DWORD), 1, &stream );
	readData( &m_dwMipmapCount, sizeof(DWORD), 1, &stream );
	
	DWORD	dwWidth, dwHeight;

	// Allocate memory
	m_ppNTXData = new BYTE*[m_dwMipmapCount];
	if( m_ppNTXData == NULL )
	{
		pfclose( fp );
		return E_FAIL;
	}	

	// Bit data
	for(DWORD i = 0; i < m_dwMipmapCount; ++i)
	{
		dwWidth = m_dwWidth >> i;
		dwHeight = m_dwHeight >> i;
		if( dwWidth < 1 ) dwWidth = 1;
		if( dwHeight < 1 ) dwHeight = 1;

		m_ppNTXData[i] = new BYTE[sizeof(BYTE) * 3 * dwWidth * dwHeight];
		readData( m_ppNTXData[i], sizeof(BYTE), 3 * dwWidth * dwHeight, &stream )/* < 3 * dwWidth * dwHeight )*/;
		//{
		//	for(DWORD j = 0; j < m_dwMipmapCount; ++j)
		//	{
		//		delete [] m_ppNTXData[j];
		//	}

		//	delete [] m_ppNTXData;

		//	pfclose( fp );
		//	return E_FAIL;
		//}
	}

	pfclose( fp );
	SAFE_DELETE_ARRAY(streamOrg);

	return S_OK;
}

HRESULT TextureContainer::Restore( LPDIRECT3DDEVICE7 pd3dDevice)
{	
	SAFE_RELEASE( m_pddsSurface );
	
	if( NULL == pd3dDevice )
		return DDERR_INVALIDPARAMS;
	
	D3DDEVICEDESC7 ddDesc;
	if( FAILED( pd3dDevice->GetCaps( &ddDesc) ) )
		return E_FAIL;
	
	// GeForce 6XXX 계열 대에선 텍스쳐 사이즈가 2의 배수가 아니면 아래쪽의 CreateSurface 에서 생성에 실패하더라. 그래서 텍스쳐가 하얗게 나온다.
	// 아마도 밉맵 서피스 생성시의 문제인듯 아래의 밉맵 속성을 안주면 일단 해결은 되기 때문에 사이즈를 검사해서 2의 배수가 아니면 밉맵으로 생성안하게 한다.(bmp만해당, tga쪽은 원래 밉맵 안썼다. 그리고 사이즈가 꼭 정사각형일 필요는 없더라..)
	if(
		( m_dwWidth != 16 && m_dwWidth != 32 && m_dwWidth != 64 && m_dwWidth != 128 && m_dwWidth != 256 && m_dwWidth != 512 && m_dwWidth != 1024 ) // 현재 라그는 여기 해상도 까지만 지원하자.
		|| ( m_dwHeight != 16 && m_dwHeight != 32 && m_dwHeight != 64 && m_dwHeight != 128 && m_dwHeight != 256 && m_dwHeight != 512 && m_dwHeight != 1024 ) // 현재 라그는 여기 해상도 까지만 지원하자.
		) // 가로나 세로중 2의 배수가 아닌 사이즈가 있으면...
		m_dwMipmapCount = 1; // 밉맵 생성을 시키지 않는다.
			
	DDSURFACEDESC2 ddsd;
	if (g_bCsyMipmap && m_dwMipmapCount > 1)
	{
		D3DUtil_InitSurfaceDesc( ddsd );
		ddsd.dwFlags         = DDSD_CAPS|DDSD_MIPMAPCOUNT|DDSD_HEIGHT|DDSD_WIDTH|
			DDSD_PIXELFORMAT|DDSD_TEXTURESTAGE;
		ddsd.ddsCaps.dwCaps  = DDSCAPS_TEXTURE|DDSCAPS_MIPMAP|DDSCAPS_COMPLEX;
		ddsd.dwMipMapCount   = m_dwMipmapCount;
		ddsd.dwTextureStage  = m_dwStage;
		ddsd.dwWidth         = m_dwWidth;
		ddsd.dwHeight        = m_dwHeight;
	}
	else
	{
		D3DUtil_InitSurfaceDesc( ddsd );
		ddsd.dwFlags         = DDSD_CAPS|DDSD_HEIGHT|DDSD_WIDTH|
			DDSD_PIXELFORMAT|DDSD_TEXTURESTAGE;
		ddsd.ddsCaps.dwCaps  = DDSCAPS_TEXTURE;
		ddsd.dwTextureStage  = m_dwStage;
		ddsd.dwWidth         = m_dwWidth;
		ddsd.dwHeight        = m_dwHeight;
	}
		
	if( ddDesc.deviceGUID == IID_IDirect3DHALDevice )
	{
		ddsd.ddsCaps.dwCaps2 = DDSCAPS2_TEXTUREMANAGE;
	}
	else if( ddDesc.deviceGUID == IID_IDirect3DTnLHalDevice )
	{
		ddsd.ddsCaps.dwCaps2 = DDSCAPS2_TEXTUREMANAGE;
	}
	else
	{
		ddsd.ddsCaps.dwCaps |= DDSCAPS_SYSTEMMEMORY;
	}
	
	if( ddDesc.dpcTriCaps.dwTextureCaps & D3DPTEXTURECAPS_POW2 )
	{
		for( ddsd.dwWidth=1;  m_dwWidth>ddsd.dwWidth;   ddsd.dwWidth<<=1 );
		for( ddsd.dwHeight=1; m_dwHeight>ddsd.dwHeight; ddsd.dwHeight<<=1 );
	}
		
	DWORD dwMaxWidth  = ddDesc.dwMaxTextureWidth;
	DWORD dwMaxHeight = ddDesc.dwMaxTextureHeight;
	ddsd.dwWidth  = min( ddsd.dwWidth,  ( dwMaxWidth  ? dwMaxWidth  : 256 ) );
	ddsd.dwHeight = min( ddsd.dwHeight, ( dwMaxHeight ? dwMaxHeight : 256 ) );
		
	if( ddDesc.dpcTriCaps.dwTextureCaps & D3DPTEXTURECAPS_SQUAREONLY )
	{
		if( ddsd.dwWidth > ddsd.dwHeight )
		{
			ddsd.dwHeight = ddsd.dwWidth;
		}
		else
		{
			ddsd.dwWidth  = ddsd.dwHeight;
		}
	}
	
	TEXTURESEARCHINFO tsi;
	tsi.bFoundGoodFormat = FALSE;
	tsi.pddpf            = &ddsd.ddpfPixelFormat;
	tsi.dwDesiredBPP     = m_dwBPP;	
	tsi.bUsePalette      = FALSE;
	tsi.bUseAlpha        = m_bHasAlpha;
	tsi.bSetAlpha		 = (m_dwFlags & D3DTEXTR_TRANSPARENTALPHA);
	
	if (tsi.bSetAlpha)
	{
		tsi.bUseAlpha =	TRUE;
	}
	if( m_dwFlags & D3DTEXTR_16BITSPERPIXEL )
	{
		tsi.dwDesiredBPP = 16;
	}
	else if( m_dwFlags & D3DTEXTR_32BITSPERPIXEL )
	{
		tsi.dwDesiredBPP = 32;
	}

	if( m_dwFlags & (D3DTEXTR_TRANSPARENTWHITE|D3DTEXTR_TRANSPARENTBLACK|D3DTEXTR_TRANSPARENTALPHA) )
	{
		if( tsi.bUsePalette )
		{
			if( ddDesc.dpcTriCaps.dwTextureCaps & D3DPTEXTURECAPS_ALPHAPALETTE )
			{
				tsi.bUseAlpha   = TRUE;
				tsi.bUsePalette = TRUE;
			}
			else
			{
				tsi.bUseAlpha   = TRUE;
				tsi.bUsePalette = FALSE;
			}
		}
	}

	pd3dDevice->EnumTextureFormats( TextureSearchCallback, &tsi );
		
	if( FALSE == tsi.bFoundGoodFormat )
	{
		tsi.bUsePalette  = FALSE;
		tsi.dwDesiredBPP = 16;
		pd3dDevice->EnumTextureFormats( TextureSearchCallback, &tsi );
		
		if( FALSE == tsi.bFoundGoodFormat )
			return E_FAIL;
	}
		
	LPDIRECTDRAW7        pDD;
	LPDIRECTDRAWSURFACE7 pddsRender;
	pd3dDevice->GetRenderTarget( &pddsRender );
	pddsRender->GetDDInterface( (VOID**)&pDD );
	pddsRender->Release();
	
	HRESULT hr = pDD->CreateSurface( &ddsd, &m_pddsSurface, NULL );
	
	pDD->Release();

	if( FAILED(hr) )
		return hr;
		
	if( !m_hbmBitmap && !m_pRGBAData && !m_ppNTXData )
	{
		LoadImageData();
	}

	BOOL bRet = FALSE;
	hr = CopyToSurface(bRet);
	
	if( bRet == TRUE )
		return hr;

	return S_OK;
}


HRESULT TextureContainer::SetAlpha(DWORD transAlpha)
{
	if ( m_dwFlags & D3DTEXTR_TRANSPARENTALPHA )
	{
		DDSURFACEDESC2 ddsd;
		ddsd.dwSize = sizeof(ddsd);
		while( m_pddsSurface->Lock( NULL, &ddsd, 0, NULL ) ==
			DDERR_WASSTILLDRAWING );

		DWORD dwAlphaMask = ddsd.ddpfPixelFormat.dwRGBAlphaBitMask;
		DWORD dwRGBMask   = ( ddsd.ddpfPixelFormat.dwRBitMask |
			ddsd.ddpfPixelFormat.dwGBitMask |
			ddsd.ddpfPixelFormat.dwBBitMask );

		if ( ddsd.ddpfPixelFormat.dwRGBBitCount == 16 &&
			ddsd.ddpfPixelFormat.dwRGBAlphaBitMask == 0xf000 )
		{
			dwAlphaMask = (m_transAlpha >> 4) << 12;
		}
		else if ( ddsd.ddpfPixelFormat.dwRGBBitCount == 32 &&
			ddsd.ddpfPixelFormat.dwRGBAlphaBitMask == 0xff000000 )
		{
			dwAlphaMask = m_transAlpha << 24;
		}
		else
		{
			return E_FAIL;
		}

		m_transAlpha = transAlpha;

		// Add an opaque alpha value to each non-colorkeyed pixel
		for( DWORD y=0; y<ddsd.dwHeight; y++ )
		{
			WORD*  p16 =  (WORD*)((BYTE*)ddsd.lpSurface + y*ddsd.lPitch);
			DWORD* p32 = (DWORD*)((BYTE*)ddsd.lpSurface + y*ddsd.lPitch);

			for( DWORD x=0; x<ddsd.dwWidth; x++ )
			{
				if( ddsd.ddpfPixelFormat.dwRGBBitCount == 16 )
				{
					*p16 &= dwRGBMask;
					*p16 |= dwAlphaMask;

					p16++;
				}
				if( ddsd.ddpfPixelFormat.dwRGBBitCount == 32 )
				{
					*p32 &= dwRGBMask;
					*p32 |= dwAlphaMask;

					p32++;
				}
			}
		}
		m_pddsSurface->Unlock( NULL );
	}
	return S_OK;
}

HRESULT TextureContainer::CopyBitmapToSurface()
{
	DWORD i;
	// Get a DDraw object to create a temporary surface
	LPDIRECTDRAW7 pDD;
	m_pddsSurface->GetDDInterface( (VOID**)&pDD );

	// Get the bitmap structure (to extract width, height, and bpp)
	BITMAP bm;
	GetObject( m_hbmBitmap, sizeof(BITMAP), &bm );

	// Setup the new surface desc
	DDSURFACEDESC2 ddsd;
	ddsd.dwSize = sizeof(ddsd);
	m_pddsSurface->GetSurfaceDesc( &ddsd );
	ddsd.dwFlags          = DDSD_CAPS|DDSD_HEIGHT|DDSD_WIDTH|DDSD_PIXELFORMAT|
		DDSD_TEXTURESTAGE;
	ddsd.ddsCaps.dwCaps   = DDSCAPS_TEXTURE|DDSCAPS_SYSTEMMEMORY;
	ddsd.ddsCaps.dwCaps2  = 0L;
	ddsd.dwWidth          = bm.bmWidth;
	ddsd.dwHeight         = bm.bmHeight;

	// 일단 1555 포맷으로 바꿔서 Temp 서피스를 만든다.
	BOOL bSet4444 = FALSE;
	if (ddsd.ddpfPixelFormat.dwRGBBitCount == 16 &&
		ddsd.ddpfPixelFormat.dwRGBAlphaBitMask == 0xf000 &&
		(m_dwFlags & D3DTEXTR_TRANSPARENTALPHA) )
	{
		ddsd.ddpfPixelFormat.dwRGBAlphaBitMask = 0x8000;
		ddsd.ddpfPixelFormat.dwRBitMask = 0x00007c00;
		ddsd.ddpfPixelFormat.dwGBitMask = 0x000003e0;
		ddsd.ddpfPixelFormat.dwBBitMask = 0x0000001f;
		bSet4444 = TRUE;
	}

	// Create a new surface for the texture
	LPDIRECTDRAWSURFACE7 pddsTempSurface[4];
	HRESULT hr;
	if (g_bCsyMipmap)
	{
		for (i=0; i < m_dwMipmapCount; i++)
		{
			if( FAILED( hr = pDD->CreateSurface( &ddsd, &pddsTempSurface[i], NULL ) ) )
			{
				pDD->Release();
				return hr;
			}

			ddsd.dwWidth /= WXY_MIP_SCALE;			// dwWidth와 dwHeight가 0이 되지 않게 한다.
			ddsd.dwHeight /= WXY_MIP_SCALE;
			if (ddsd.dwWidth < 1)
				ddsd.dwWidth = 1;
			if (ddsd.dwHeight < 1)
				ddsd.dwHeight = 1;
		}
	}
	else
	{
		if( FAILED( hr = pDD->CreateSurface( &ddsd, &pddsTempSurface[0], NULL ) ) )
		{
			pDD->Release();
			return hr;
		}
		pddsTempSurface[1] = NULL;
		pddsTempSurface[2] = NULL;
	}

	// Get a DC for the bitmap
	HDC hdcBitmap = CreateCompatibleDC( NULL );
	// test
	HDC hdcBitmap1 = CreateCompatibleDC( NULL);
	HDC hdcBitmap2 = CreateCompatibleDC (NULL);

	if( NULL == hdcBitmap )
	{
		if (g_bCsyMipmap)
		{
			for (i=0; i < m_dwMipmapCount; i++)
			{
				pddsTempSurface[i]->Release();
			}
		}
		else
		{
			pddsTempSurface[0]->Release();
		}
		pDD->Release();
		return hr;
	}

	HBITMAP* hOldBmp;
	hOldBmp = (HBITMAP*)SelectObject( hdcBitmap, m_hbmBitmap );

	// Handle palettized textures. Need to attach a palette
	if( ddsd.ddpfPixelFormat.dwRGBBitCount == 8 )
	{
		LPDIRECTDRAWPALETTE  pPalette;
		DWORD dwPaletteFlags = DDPCAPS_8BIT|DDPCAPS_ALLOW256;
		DWORD pe[256];
		WORD  wNumColors     = GetDIBColorTable( hdcBitmap, 0, 256, (RGBQUAD*)pe );

		// Create the color table
		for( WORD i=0; i<wNumColors; i++ )
		{
			pe[i] = RGB( GetBValue(pe[i]), GetGValue(pe[i]), GetRValue(pe[i]) );

			// Handle textures with transparent pixels
			if( m_dwFlags & (D3DTEXTR_TRANSPARENTWHITE|D3DTEXTR_TRANSPARENTBLACK|D3DTEXTR_TRANSPARENTALPHA) )
			{
				// Set alpha for opaque pixels
				if( m_dwFlags & D3DTEXTR_TRANSPARENTBLACK )
				{
					if( pe[i] != 0x00000000 )
						pe[i] |= 0xff000000;
				}
				else if( m_dwFlags & D3DTEXTR_TRANSPARENTWHITE )
				{
					if( pe[i] != 0x00ffffff )
						pe[i] |= 0xff000000;
				}
				else if( m_dwFlags & D3DTEXTR_TRANSPARENTALPHA )
				{
					pe[i] |= (m_transAlpha << 24);
				}
			}
		}
		// Add DDPCAPS_ALPHA flag for textures with transparent pixels
		if( m_dwFlags & (D3DTEXTR_TRANSPARENTWHITE|D3DTEXTR_TRANSPARENTBLACK|D3DTEXTR_TRANSPARENTALPHA) )
			dwPaletteFlags |= DDPCAPS_ALPHA;

		// Create & attach a palette
		pDD->CreatePalette( dwPaletteFlags, (PALETTEENTRY*)pe, &pPalette, NULL );
		if (g_bCsyMipmap)
		{
			for (unsigned int i=0; i < m_dwMipmapCount; i++)
			{
				pddsTempSurface[i]->SetPalette( pPalette );
			}
		}
		else
		{
			pddsTempSurface[0]->SetPalette( pPalette );
		}
		m_pddsSurface->SetPalette( pPalette );
		SAFE_RELEASE( pPalette );
	}

	// Copy the bitmap image to the surface.
	HDC hdcSurface[4];
	if (g_bCsyMipmap)
	{
		int width = bm.bmWidth;
		int height = bm.bmHeight;
		for (i=0; i < m_dwMipmapCount; i++)
		{
			if( SUCCEEDED( pddsTempSurface[i]->GetDC( &hdcSurface[i] ) ) )
			{				
				SetStretchBltMode(hdcSurface[i], COLORONCOLOR);
				StretchBlt( hdcSurface[i], 0, 0, width, height, hdcBitmap, 0, 0, bm.bmWidth, bm.bmHeight, SRCPAINT );
				pddsTempSurface[i]->ReleaseDC( hdcSurface[i] );
			}
			width /= WXY_MIP_SCALE;
			height /= WXY_MIP_SCALE;
			if (width < 1)
				width = 1;
			if (height < 1)
				height = 1;
		}
	}
	else
	{
		if( SUCCEEDED( pddsTempSurface[0]->GetDC( &hdcSurface[0] ) ) )
		{
			BitBlt( hdcSurface[0], 0, 0, bm.bmWidth, bm.bmHeight, hdcBitmap, 0, 0, SRCCOPY );			
			pddsTempSurface[0]->ReleaseDC( hdcSurface[0] );
		}
	}

	SelectObject(hdcBitmap, hOldBmp);

	DeleteDC( hdcBitmap );
	// test
	DeleteDC( hdcBitmap1 );
	DeleteDC( hdcBitmap2 );
	// end test

	// Copy the temp surface to the real texture surface
	if (!bSet4444)
	{
		if (g_bCsyMipmap)
		{
			LPDIRECTDRAWSURFACE7 pddsDest = m_pddsSurface;
			for (i=0; i < m_dwMipmapCount; i++)
			{
				pddsDest->Blt( NULL, pddsTempSurface[i], NULL, DDBLT_WAIT, NULL );

				DDSCAPS2 ddsCaps;
				ddsCaps.dwCaps  = DDSCAPS_TEXTURE | DDSCAPS_MIPMAP;
				ddsCaps.dwCaps2 = 0;
				ddsCaps.dwCaps3 = 0;
				ddsCaps.dwCaps4 = 0;
				if( SUCCEEDED( pddsDest->GetAttachedSurface( &ddsCaps, &pddsDest ) ) )
					pddsDest->Release();
			}
		}
		else
		{
			m_pddsSurface->Blt( NULL, pddsTempSurface[0], NULL, DDBLT_WAIT, NULL );
		}
	}

	// For textures with real alpha (not palettized), set transparent bits
	if( ddsd.ddpfPixelFormat.dwRGBAlphaBitMask )
	{
		if( m_dwFlags & (D3DTEXTR_TRANSPARENTWHITE|D3DTEXTR_TRANSPARENTBLACK|D3DTEXTR_TRANSPARENTALPHA) )
		{
			DWORD	mipmapCount;
			if (g_bCsyMipmap)
			{
				mipmapCount = m_dwMipmapCount;
			}
			else
			{
				mipmapCount = 1;
			}
			LPDIRECTDRAWSURFACE7 pddsDest = m_pddsSurface;
			for (i=0; i < mipmapCount; i++)
			{
				// Lock the texture surface
				DDSURFACEDESC2 ddsd, ddsd_t;
				ddsd.dwSize = sizeof(ddsd);
				ddsd_t.dwSize = sizeof(ddsd_t);
				while( pddsDest->Lock( NULL, &ddsd, 0, NULL ) ==
					DDERR_WASSTILLDRAWING );
				while( pddsTempSurface[i]->Lock( NULL, &ddsd_t, 0, NULL ) ==
					DDERR_WASSTILLDRAWING );

				DWORD dwAlphaMask = ddsd.ddpfPixelFormat.dwRGBAlphaBitMask;
				DWORD dwRGBMask   = ( ddsd.ddpfPixelFormat.dwRBitMask |
					ddsd.ddpfPixelFormat.dwGBitMask |
					ddsd.ddpfPixelFormat.dwBBitMask );
				DWORD dwColorkey  = 0x00000000; // Colorkey on black
				if( m_dwFlags & D3DTEXTR_TRANSPARENTWHITE )
					dwColorkey = dwRGBMask;     // Colorkey on white

				if ( m_dwFlags & D3DTEXTR_TRANSPARENTALPHA )
				{
					dwColorkey = 0xffffffff;
					if ( ddsd.ddpfPixelFormat.dwRGBBitCount == 16 &&
						ddsd.ddpfPixelFormat.dwRGBAlphaBitMask == 0xf000 )
					{
						dwAlphaMask = (m_transAlpha >> 4) << 12;
					}
					else if ( ddsd.ddpfPixelFormat.dwRGBBitCount == 32 &&
						ddsd.ddpfPixelFormat.dwRGBAlphaBitMask == 0xff000000 )
					{
						dwAlphaMask = m_transAlpha << 24;
					}
				}

				// Add an opaque alpha value to each non-colorkeyed pixel
				for( DWORD y=0; y<ddsd.dwHeight; y++ )
				{
					WORD*  p16 =  (WORD*)((BYTE*)ddsd.lpSurface + y*ddsd.lPitch);
					WORD*  p16_t =  (WORD*)((BYTE*)ddsd_t.lpSurface + y*ddsd_t.lPitch);
					DWORD* p32 = (DWORD*)((BYTE*)ddsd.lpSurface + y*ddsd.lPitch);

					for( DWORD x=0; x<ddsd.dwWidth; x++ )
					{
						if( ddsd.ddpfPixelFormat.dwRGBBitCount == 16 )
						{
							// 1555 로 저장했던 것을 4444 로 바꿔서 재저장한다.
							if (bSet4444)
							{
								WORD r = ((*p16_t & 0x7c00) >> 11) << 8;
								WORD g = ((*p16_t & 0x03e0) >> 6) << 4;
								WORD b = ((*p16_t & 0x001f) >> 1);
								*p16 = r | g | b;
							}

							if( ( *p16 &= dwRGBMask ) != dwColorkey )
								*p16 |= dwAlphaMask;
							p16++;
							p16_t++;
						}
						if( ddsd.ddpfPixelFormat.dwRGBBitCount == 32 )
						{
							if( ( *p32 &= dwRGBMask ) != dwColorkey )
								*p32 |= dwAlphaMask;
							p32++;
						}
					}
				}
				pddsDest->Unlock( NULL );
				pddsTempSurface[i]->Unlock( NULL );
				if (g_bCsyMipmap)
				{
					DDSCAPS2 ddsCaps;
					ddsCaps.dwCaps  = DDSCAPS_TEXTURE | DDSCAPS_MIPMAP;
					ddsCaps.dwCaps2 = 0;
					ddsCaps.dwCaps3 = 0;
					ddsCaps.dwCaps4 = 0;
					if( SUCCEEDED( pddsDest->GetAttachedSurface( &ddsCaps, &pddsDest ) ) )
						pddsDest->Release();
				}
			}
		}
	}

	// Done with the temp surface
	if (g_bCsyMipmap)
	{
		for (i=0; i < m_dwMipmapCount; i++)
		{
			pddsTempSurface[i]->Release();
		}
	}
	else
	{
		pddsTempSurface[0]->Release();
	}

	pDD->Release();

	return S_OK;;
}

HRESULT TextureContainer::CopyRGBADataToSurface()
{
	// Get a DDraw object to create a temporary surface
	LPDIRECTDRAW7 pDD;
	m_pddsSurface->GetDDInterface( (VOID**)&pDD );

	// Setup the new surface desc
	DDSURFACEDESC2 ddsd;
	ddsd.dwSize = sizeof(ddsd);
	m_pddsSurface->GetSurfaceDesc( &ddsd );
	ddsd.dwFlags         = DDSD_CAPS|DDSD_HEIGHT|DDSD_WIDTH|DDSD_PIXELFORMAT|
		DDSD_TEXTURESTAGE;
	ddsd.ddsCaps.dwCaps  = DDSCAPS_TEXTURE|DDSCAPS_SYSTEMMEMORY;
	ddsd.ddsCaps.dwCaps2 = 0L;
	ddsd.dwWidth         = m_dwWidth;
	ddsd.dwHeight        = m_dwHeight;

	// Create a new surface for the texture
	LPDIRECTDRAWSURFACE7 pddsTempSurface;
	HRESULT hr;
	if( FAILED( hr = pDD->CreateSurface( &ddsd, &pddsTempSurface, NULL ) ) )
	{
		pDD->Release();
		return NULL;
	}

	while( pddsTempSurface->Lock( NULL, &ddsd, 0, 0 ) == DDERR_WASSTILLDRAWING );
	DWORD lPitch = ddsd.lPitch;
	BYTE* pBytes = (BYTE*)ddsd.lpSurface;

	DWORD dwRMask = ddsd.ddpfPixelFormat.dwRBitMask;
	DWORD dwGMask = ddsd.ddpfPixelFormat.dwGBitMask;
	DWORD dwBMask = ddsd.ddpfPixelFormat.dwBBitMask;
	DWORD dwAMask = ddsd.ddpfPixelFormat.dwRGBAlphaBitMask;

	DWORD dwRShiftL = 8, dwRShiftR = 0;
	DWORD dwGShiftL = 8, dwGShiftR = 0;
	DWORD dwBShiftL = 8, dwBShiftR = 0;
	DWORD dwAShiftL = 8, dwAShiftR = 0;

	DWORD dwMask;
	for( dwMask=dwRMask; dwMask && !(dwMask&0x1); dwMask>>=1 ) dwRShiftR++;
	for( ; dwMask; dwMask>>=1 ) dwRShiftL--;

	for( dwMask=dwGMask; dwMask && !(dwMask&0x1); dwMask>>=1 ) dwGShiftR++;
	for( ; dwMask; dwMask>>=1 ) dwGShiftL--;

	for( dwMask=dwBMask; dwMask && !(dwMask&0x1); dwMask>>=1 ) dwBShiftR++;
	for( ; dwMask; dwMask>>=1 ) dwBShiftL--;

	for( dwMask=dwAMask; dwMask && !(dwMask&0x1); dwMask>>=1 ) dwAShiftR++;
	for( ; dwMask; dwMask>>=1 ) dwAShiftL--;

	for( DWORD y=0; y<ddsd.dwHeight; y++ )
	{
		DWORD* pDstData32 = (DWORD*)pBytes;
		WORD*  pDstData16 = (WORD*)pBytes;

		for( DWORD x=0; x<ddsd.dwWidth; x++ )
		{
			DWORD dwPixel = m_pRGBAData[y*ddsd.dwWidth+x];

			BYTE r = (BYTE)((dwPixel>>24)&0x000000ff);
			BYTE g = (BYTE)((dwPixel>>16)&0x000000ff);
			BYTE b = (BYTE)((dwPixel>> 8)&0x000000ff);
			BYTE a = (BYTE)((dwPixel>> 0)&0x000000ff);

			DWORD dr = ((r>>(dwRShiftL))<<dwRShiftR)&dwRMask;
			DWORD dg = ((g>>(dwGShiftL))<<dwGShiftR)&dwGMask;
			DWORD db = ((b>>(dwBShiftL))<<dwBShiftR)&dwBMask;
			DWORD da = ((a>>(dwAShiftL))<<dwAShiftR)&dwAMask;

			if( 32 == ddsd.ddpfPixelFormat.dwRGBBitCount )
				pDstData32[x] = (DWORD)(dr+dg+db+da);
			else
				pDstData16[x] = (WORD)(dr+dg+db+da);
		}

		pBytes += ddsd.lPitch;
	}

	pddsTempSurface->Unlock(0);

	// Copy the temp surface to the real texture surface
	m_pddsSurface->Blt( NULL, pddsTempSurface, NULL, DDBLT_WAIT, NULL );

	// Done with the temp objects
	pddsTempSurface->Release();
	pDD->Release();

	return S_OK;
}

HRESULT TextureContainer::CopyNTXDataToSurface()
{
	// Get a DDraw object to create a temporary surface
	LPDIRECTDRAW7	pDD;
	m_pddsSurface->GetDDInterface( (VOID**)&pDD );

	// Setup the new surface desc
	DDSURFACEDESC2	ddsd;
	ddsd.dwSize = sizeof(ddsd);
	m_pddsSurface->GetSurfaceDesc( &ddsd );
	ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT | DDSD_TEXTURESTAGE;
	ddsd.ddsCaps.dwCaps = DDSCAPS_TEXTURE | DDSCAPS_SYSTEMMEMORY;
	ddsd.ddsCaps.dwCaps2 = 0L;
	ddsd.dwWidth = m_dwWidth;
	ddsd.dwHeight = m_dwHeight;

	// Create a new surface for the texture
	LPDIRECTDRAWSURFACE7 pddsTempSurface[9];
	HRESULT hr;
	DWORD i = 0;
	for( i = 0; i < m_dwMipmapCount; i++ )
	{
		if( FAILED( hr = pDD->CreateSurface( &ddsd, &pddsTempSurface[i], NULL ) ) )
		{
			pDD->Release();
			return hr;
		}

		ddsd.dwWidth >>= 1;
		ddsd.dwHeight >>= 1;
		if( ddsd.dwWidth < 1 ) ddsd.dwWidth = 1;
		if( ddsd.dwHeight < 1 ) ddsd.dwHeight = 1;
	}

	DWORD	dwX, dwY, dwWidth, dwHeight;
	DWORD	r, g, b;

	pddsTempSurface[0]->GetSurfaceDesc( &ddsd );
	if( 16 == ddsd.ddpfPixelFormat.dwRGBBitCount )		// 16 bit
	{
		DWORD	dwRMask = ddsd.ddpfPixelFormat.dwRBitMask;
		DWORD	dwGMask = ddsd.ddpfPixelFormat.dwGBitMask;
		DWORD	dwBMask = ddsd.ddpfPixelFormat.dwBBitMask;
		DWORD	dwRShiftL = 8, dwRShiftR = 0;
		DWORD	dwGShiftL = 8, dwGShiftR = 0;
		DWORD	dwBShiftL = 8, dwBShiftR = 0;
		DWORD	dwMask;

		for( dwMask = dwRMask; dwMask && !(dwMask & 0x1); dwMask >>= 1 ) dwRShiftR++;
		for( ; dwMask; dwMask >>= 1 ) dwRShiftL--;
		for( dwMask = dwGMask; dwMask && !(dwMask & 0x1); dwMask >>= 1 ) dwGShiftR++;
		for( ; dwMask; dwMask >>= 1 ) dwGShiftL--;
		for( dwMask = dwBMask; dwMask && !(dwMask & 0x1); dwMask >>= 1 ) dwBShiftR++;
		for( ; dwMask; dwMask >>= 1 ) dwBShiftL--;

		for( i = 0; i < m_dwMipmapCount; i++ )
		{
			dwWidth = m_dwWidth >> i;
			dwHeight = m_dwHeight >> i;
			if( dwWidth < 1 ) dwWidth = 1;
			if( dwHeight < 1 ) dwHeight = 1;

			while( pddsTempSurface[i]->Lock( NULL, &ddsd, 0, 0 ) == DDERR_WASSTILLDRAWING );
			BYTE*	pBytes = (BYTE*)ddsd.lpSurface;

			WORD*	pDest;
			BYTE*	pSrc = m_ppNTXData[i];
			BYTE*	pSrcRow = pSrc;

			for( dwY = 0; dwY < ddsd.dwHeight; dwY++ )
			{
				pDest = (WORD*)pBytes;
				for( dwX = 0; dwX < ddsd.dwWidth; dwX++ )
				{
					r = *( pSrc );
					g = *( pSrc + 1 );
					b = *( pSrc + 2 );
					r = ( ( r >> dwRShiftL ) << dwRShiftR ) & dwRMask;
					g = ( ( g >> dwGShiftL ) << dwGShiftR ) & dwGMask;
					b = ( ( b >> dwBShiftL ) << dwBShiftR ) & dwBMask;

					pDest[dwX] = (WORD)( r + g + b );
					pSrc += 3;
				}

				pBytes += ddsd.lPitch;
				pSrcRow += dwWidth * 3;
				pSrc = pSrcRow;
			}

			pddsTempSurface[i]->Unlock( 0 );
		}
	}
	else		// 32bit
	{
		for( i = 0; i < m_dwMipmapCount; i++ )
		{
			dwWidth = m_dwWidth >> i;
			dwHeight = m_dwHeight >> i;
			if( dwWidth < 1 ) dwWidth = 1;
			if( dwHeight < 1 ) dwHeight = 1;

			while( pddsTempSurface[i]->Lock( NULL, &ddsd, 0, 0 ) == DDERR_WASSTILLDRAWING );
			BYTE*	pBytes = (BYTE*)ddsd.lpSurface;

			DWORD*	pDest;
			BYTE*	pSrc = m_ppNTXData[i];
			BYTE*	pSrcRow = pSrc;

			for( dwY = 0; dwY < ddsd.dwHeight; dwY++ )
			{
				pDest = (DWORD*)pBytes;
				for( dwX = 0; dwX < ddsd.dwWidth; dwX++ )
				{
					r = *( pSrc );
					g = *( pSrc + 1 );
					b = *( pSrc + 2 );

					pDest[dwX] = ( r << 16 ) + ( g << 8 ) + b;
					pSrc += 3;
				}

				pBytes += ddsd.lPitch;
				pSrcRow += dwWidth * 3;
				pSrc = pSrcRow;
			}

			pddsTempSurface[i]->Unlock( 0 );
		}
	}

	// Copy the temp surface to the real texture surface
	LPDIRECTDRAWSURFACE7	pddsDest = m_pddsSurface;
	for( i = 0; i < m_dwMipmapCount; i++ )
	{
		pddsDest->Blt( NULL, pddsTempSurface[i], NULL, DDBLT_WAIT, NULL );

		DDSCAPS2	ddsCaps;
		ddsCaps.dwCaps = DDSCAPS_TEXTURE | DDSCAPS_MIPMAP;
		ddsCaps.dwCaps2 = 0;
		ddsCaps.dwCaps3 = 0;
		ddsCaps.dwCaps4 = 0;
		if( SUCCEEDED( pddsDest->GetAttachedSurface( &ddsCaps, &pddsDest ) ) )
			pddsDest->Release();
	}

	// Done with the temp objects
	for( i = 0; i < m_dwMipmapCount; i++ )
		pddsTempSurface[i]->Release();

	pDD->Release();

	return S_OK;
}

HRESULT TextureContainer::CopyToSurface(BOOL& bRet)
{
	HRESULT hr = S_FALSE;
	bRet = TRUE;
	
	if( m_hbmBitmap )
	{
		hr = CopyBitmapToSurface();
		DeleteObject(m_hbmBitmap);
		m_hbmBitmap = NULL;
		return hr;
	}

	if( m_pRGBAData )
	{
		hr = CopyRGBADataToSurface();
		SAFE_DELETE_ARRAY( m_pRGBAData );
		return hr;
	}
	
	if( m_ppNTXData )
	{
		hr = CopyNTXDataToSurface();
		for( DWORD i = 0; i < m_dwMipmapCount; i++ )
			SAFE_DELETE_ARRAY( m_ppNTXData[i] );
		SAFE_DELETE_ARRAY( m_ppNTXData );
		return hr;
	}

	bRet = FALSE;
	return hr;
}

//////////////////////////////////////////////////////////////////////////

IndexedTexture::IndexedTexture()
{	
	m_TextureList = NULL;

	m_Size = 0;
	m_Idx = 0;
}

IndexedTexture::IndexedTexture(int size)
{	
	m_TextureList = NULL;

	SetSize(size);
}

IndexedTexture::~IndexedTexture()
{	
	ReleaseTextureList();
}

VOID IndexedTexture::SetTexturePath( LPCTSTR strTexturePath )
{
	m_TexturePath = strTexturePath;	
}

VOID IndexedTexture::SetTexturePath2( LPCTSTR strTexturePath )
{
	m_TexturePath2 = strTexturePath;	
}

VOID IndexedTexture::SetTexturePath3( LPCTSTR strTexturePath )
{
	m_TexturePath3 = strTexturePath;	
}

int IndexedTexture::FindTextureWXY(TCHAR* strTextureName)
{
	if( NULL == strTextureName )
		return -1;

	int len = strlen(strTextureName);
	if (len < 3 || len > 255)
	{
		return -2;
	}

	std::string strFileName = strTextureName;	

	if (g_bWXY_FORMAT)
	{
		strFileName = getXYZFormatFileName(strTextureName);
	}

	return FindTexture(strFileName);
}

int IndexedTexture::CreateTextureFromFile( TCHAR* strName, int Idx, DWORD dwStage, DWORD dwFlags, DWORD transAlpha )
{
	int len = strlen(strName);
	if( len <= 0 )
		return -1;

	if( strcmp(strName, "NULL") == 0 )
		return -1;

	int TextureNum = FindTextureWXY( strName );
	if( TextureNum >= 0)
	{
		return TextureNum;
	}

	std::string strFileName;
	if (g_bWXY_FORMAT)
	{
		strFileName = getXYZFormatFileName(strName);
	}

	return CreateTextureFromFile_Imp(strFileName, Idx, dwStage, dwFlags, transAlpha, true);
}

int IndexedTexture::CreateTextureFromFileNonePack( TCHAR* strName, int Idx, DWORD dwStage, DWORD dwFlags, DWORD transAlpha )
{
	int TextureNum = FindTextureWXY( strName );
	if( TextureNum >= 0)
	{
		return TextureNum;
	}

	std::string strFileName;
	if (g_bWXY_FORMAT)
	{
		strFileName = getXYZFormatFileName(strName);
	}

	return CreateTextureFromFile_Imp(strFileName, Idx, dwStage, dwFlags, transAlpha, false);
}

int IndexedTexture::CreateTextureFromFile_Imp(std::string strFilename, int index, DWORD dwStage, DWORD dwFlags, DWORD transAlpha, bool bPack)
{
	int Idx = index;

	if (Idx >= m_Size || (Idx == CREATE_ADD && m_Idx >= m_Size))
		return -1;

	if (Idx == CREATE_INSERT)
	{
		for (int i = 0; i < m_Size; i++)
		{
			if( m_TextureList[i] == NULL )
			{
				Idx = i;
				break;
			}
		}
	}

	if (Idx == CREATE_INSERT)
		return -1;

	TextureContainer* ptcTexture = new TextureContainer(strFilename.c_str(), dwStage, dwFlags, transAlpha , bPack );

	if( FAILED( ptcTexture->LoadImageData(m_TexturePath.c_str(), m_TexturePath2.c_str(), m_TexturePath3.c_str()) ) )
	{
		delete ptcTexture;
		return -1;
	}

	if( ptcTexture->m_hbmBitmap )
	{
		BITMAP bm;
		GetObject( ptcTexture->m_hbmBitmap, sizeof(BITMAP), &bm );
		ptcTexture->m_dwWidth  = (DWORD)bm.bmWidth;
		ptcTexture->m_dwHeight = (DWORD)bm.bmHeight;
		ptcTexture->m_dwBPP    = (DWORD)bm.bmBitsPixel;
	}

	if (Idx >= 0)
	{
		if (m_TextureList[Idx])
		{
			delTextureIndex(m_TextureList[Idx]->m_strName);
			delete m_TextureList[Idx];
		}

		m_TextureList[Idx] = ptcTexture;
		addTextureIndex(ptcTexture->m_strName, Idx);

		if (Idx >= m_Idx)
		{
			m_Idx = Idx + 1;
		}

		return Idx;
	}
	else
	{
		m_TextureList[m_Idx++] = ptcTexture;
		addTextureIndex(ptcTexture->m_strName, m_Idx-1);

		return (m_Idx - 1);
	}
}

int IndexedTexture::CreateEmptyTexture( TCHAR* strName, DWORD dwWidth, DWORD dwHeight, int Idx, DWORD dwStage, DWORD dwFlags, DWORD transAlpha )
{	
	if( NULL == strName )		
		return -1;
	
	if( FindTexture( strName ) >= 0 )		
		return -1;

	if (Idx >= m_Size || (Idx < 0 && m_Idx >= m_Size))		
		return -1;
	
	TextureContainer* ptcTexture = new TextureContainer( strName, dwStage, dwFlags, transAlpha );	
		
	ptcTexture->m_dwWidth  = dwWidth;
	ptcTexture->m_dwHeight = dwHeight;
	ptcTexture->m_dwBPP    = 16;

	if( ptcTexture->m_dwFlags & D3DTEXTR_32BITSPERPIXEL )
	{
		ptcTexture->m_dwBPP = 32;
	}
	
	if( dwFlags & D3DTEXTR_CREATEWITHALPHA )
	{
		ptcTexture->m_bHasAlpha = TRUE;
	}

	if (Idx >= 0)
	{
		if (m_TextureList[Idx])
		{
			delTextureIndex(m_TextureList[Idx]->m_strName);
			delete m_TextureList[Idx];
		}

		m_TextureList[Idx] = ptcTexture;
		addTextureIndex(ptcTexture->m_strName, Idx);

		if( Idx >= m_Idx )
		{
			m_Idx = Idx + 1;
		}

		return Idx;
	}
	else
	{
		m_TextureList[m_Idx++] = ptcTexture;
		addTextureIndex(ptcTexture->m_strName, m_Idx-1);

		return (m_Idx - 1);
	}	
}

HRESULT IndexedTexture::Restore( TCHAR* strName, LPDIRECT3DDEVICE7 pd3dDevice )
{	
	int TextureNum = FindTexture( strName );
	if( TextureNum < 0 )
	{
		return DDERR_NOTFOUND;
	}
	
	return m_TextureList[TextureNum]->Restore( pd3dDevice );
}


HRESULT IndexedTexture::RestoreByIdx( int Idx, LPDIRECT3DDEVICE7 pd3dDevice )
{
	if( Idx < 0 || Idx >= m_Idx )
	{
		return DDERR_NOTFOUND;
	}
	
	return m_TextureList[Idx]->Restore( pd3dDevice );
}

HRESULT IndexedTexture::RestoreAllTextures( LPDIRECT3DDEVICE7 pd3dDevice )
{
	for (int i = 0; i < m_Idx; i++)
	{
		if( m_TextureList[i] )
		{			
			RestoreByIdx(i, pd3dDevice );
		}
	}

	return S_OK;
}

HRESULT IndexedTexture::Invalidate( TCHAR* strName )
{
	int TextureNum = FindTexture( strName );
	if( TextureNum < 0 )
	{
		return DDERR_NOTFOUND;
	}

	SAFE_RELEASE( m_TextureList[TextureNum]->m_pddsSurface );

	return S_OK;
}

HRESULT IndexedTexture::InvalidateAllTextures()
{
	for (int i = 0; i < m_Idx; i++)
	{
		if( m_TextureList[i] )
		{
			SAFE_RELEASE( m_TextureList[i]->m_pddsSurface );
		}
	}
	
	return S_OK;
}

HRESULT IndexedTexture::DestroyTexture( TCHAR* strName )
{
	int TextureNum = FindTexture( strName );

	if (TextureNum >=0)
	{
		delTextureIndex(m_TextureList[TextureNum]->m_strName);
		SAFE_DELETE( m_TextureList[TextureNum] );
		m_TextureList[TextureNum] = NULL;
	}

	return S_OK;
}

HRESULT IndexedTexture::DestroyTextureByIdx( int Idx )
{
	if (Idx >= m_Idx)
		return E_FAIL;

	if( m_TextureList[Idx] )
		delTextureIndex(m_TextureList[Idx]->m_strName);

	SAFE_DELETE( m_TextureList[Idx] );
	m_TextureList[Idx] = NULL;

	return S_OK;
}

HRESULT IndexedTexture::DestroyTextureAll()
{
	for (int i = 0; i < m_Idx; i++)
	{
		SAFE_DELETE( m_TextureList[i] );
		m_TextureList[i] = NULL;
	}

	m_mapTextureIndex.clear();

	return S_OK;
}

LPDIRECTDRAWSURFACE7 IndexedTexture::GetSurface( TCHAR* strName )
{
	int TextureNum = FindTexture( strName );

	return (TextureNum >= 0) ? m_TextureList[TextureNum]->m_pddsSurface : NULL;
}

LPDIRECTDRAWSURFACE7 IndexedTexture::GetSurfaceByIdx( int Idx )
{
	if (Idx >= 0 && Idx < m_Size && m_TextureList[Idx])
	{
		return m_TextureList[Idx]->m_pddsSurface;
	}

	return NULL;
}

BOOL IndexedTexture::SetSize(int size)
{
	ReleaseTextureList();

	m_TextureList = new TextureContainer*[size];
	ZeroMemory(m_TextureList, sizeof(TextureContainer*) * size);

	m_Size = size;
	m_Idx = 0;

	return TRUE;
}

void IndexedTexture::ReleaseTextureList()
{
	if (m_TextureList)
	{
		for (int i = 0; i < m_Idx; i++)
		{
			if (m_TextureList[i])
			{
				delete m_TextureList[i];
			}
		}

		delete[] m_TextureList;
	}
}

void IndexedTexture::SetAlphaAll(DWORD transAlpha)
{
	for (int i = 0; i < m_Size; i++)
	{
		if (m_TextureList[i])
		{
			m_TextureList[i]->SetAlpha(transAlpha);
		}
	}
}

BOOL IndexedTexture::SetAlpha(int Idx, DWORD transAlpha)
{
	if (Idx < m_Idx && m_TextureList[Idx])
	{
		m_TextureList[Idx]->SetAlpha(transAlpha);

		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

int IndexedTexture::FindTexture(std::string strTextureName)
{
	std::string textureName = strTextureName;
	std::transform(textureName.begin(), textureName.end(), textureName.begin(), toupper);

	return findTextureIndex(textureName);
}

void IndexedTexture::addTextureIndex(std::string textureName, int idx)
{
	std::string nameUpper = textureName;
	std::transform(nameUpper.begin(), nameUpper.end(), nameUpper.begin(), toupper);

	std::map<std::string, int>::iterator itFind = m_mapTextureIndex.find(nameUpper);
	if( itFind != m_mapTextureIndex.end() )
	{
		(*itFind).second = idx;
		return;
	}

	m_mapTextureIndex.insert(std::make_pair(nameUpper, idx));
}

void IndexedTexture::delTextureIndex(std::string textureName)
{
	std::string nameUpper = textureName;
	std::transform(nameUpper.begin(), nameUpper.end(), nameUpper.begin(), toupper);

	std::map<std::string, int>::iterator itFind = m_mapTextureIndex.find(nameUpper);
	if( itFind != m_mapTextureIndex.end() )
	{
		m_mapTextureIndex.erase(itFind);
	}
}

int IndexedTexture::findTextureIndex(std::string textureName)
{
	std::map<std::string, int>::iterator itFind = m_mapTextureIndex.find(textureName);
	if( itFind != m_mapTextureIndex.end() )
	{
		return (*itFind).second;
	}

	return -1;
}

std::string IndexedTexture::getXYZFormatFileName(std::string textureName)
{
	std::string strRet = textureName;
	
	// bmp -> wxy
	std::string findString = ".bmp";
	std::string replaceString = ".wxy";	
	std::string::size_type offset = 0;
	
	offset = strRet.find(findString, offset);

	if( std::string::npos != offset )
	{
		strRet.replace(offset, findString.length(), replaceString);
		return strRet;
	}

	// BMP -> wxy
	findString = ".BMP";
	replaceString = ".wxy";
	offset = 0;

	offset = strRet.find(findString, offset);

	if( std::string::npos != offset )
	{
		strRet.replace(offset, findString.length(), replaceString);
		return strRet;
	}
	
	// tga -> sek
	findString = ".tga";
	replaceString = ".sek";
	offset = 0;

	offset = strRet.find(findString, offset);

	if( std::string::npos != offset )
	{
		strRet.replace(offset, findString.length(), replaceString);
		return strRet;
	}

	// TGA -> sek
	findString = ".TGA";
	replaceString = ".sek";
	offset = 0;

	offset = strRet.find(findString, offset);

	if( std::string::npos != offset )
	{
		strRet.replace(offset, findString.length(), replaceString);
		return strRet;
	}

	return strRet;	
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Name : TileTextures Class
// Desc : 하나의 비트맵에서 여러 장의 텍스처 서피스를 생성한 후에 인덱스를 이용하여 접근을 한다.

TileTextures::TileTextures(DWORD dwStage, DWORD dwFlags, DWORD transAlpha)
{
	m_TexturePath[0] = '\0';

	m_dwWidth     = 0;
	m_dwHeight    = 0;
	m_dwStage     = dwStage;
	m_dwBPP       = 0;
	m_dwFlags     = dwFlags;
	m_bHasAlpha   = 0;

	m_ppddsSurface = NULL;
	m_hbmBitmap[0]   = NULL;
	m_hbmBitmap[1]   = NULL;
	m_hbmBitmap[2]   = NULL;

	m_transAlpha  = transAlpha;

	m_dwMipmapCount = 3;
}

TileTextures::~TileTextures()
{
	m_TexturePath[0] = '\0';

	Invalidate();								// 여러개의 서피스를 invalidate 시킨다.
	DeleteTextureSurfaces();

	for (DWORD i=0; i < m_dwMipmapCount; i++)
	{
		if (m_hbmBitmap[i])
			DeleteObject( m_hbmBitmap[i] );				// 원본 비트맵을 제거.
	}
}

VOID TileTextures::SetTexturePath( LPCTSTR strTexturePath )
{
	if( NULL == strTexturePath )
		strTexturePath = _T("");

	lstrcpy( m_TexturePath, strTexturePath );
}

HRESULT TileTextures::CreateTextureFromFile(TCHAR* strName, DWORD dwTileWidth, DWORD dwTileHeight, DWORD dwStage, DWORD dwFlags, DWORD transAlpha)
{
	// Check parameters
	if( NULL == strName )
		return E_INVALIDARG;

	// 로딩할 텍스처 이름을 보관
	lstrcpy( m_strName, strName );

	if (g_bWXY_FORMAT)
	{
		TCHAR* strExtension;
		if( NULL != ( strExtension = _tcsrchr( m_strName, _T('.') ) ) )
		{
			if( !lstrcmpi( strExtension, _T(".bmp") ) )
			{
				int len = strlen(m_strName);
				strcpy(m_strName+len-3, "wxy");				
			}
		}
	}

	// Create a bitmap and load the texture file into it,
	if (FAILED( LoadImageData(m_TexturePath) ))
		return E_FAIL;

	// Save the image's dimensions
	if ( m_hbmBitmap[0] )
	{
		BITMAP bm;
		GetObject( m_hbmBitmap[0], sizeof(BITMAP), &bm );
		m_dwWidth  = (DWORD)bm.bmWidth;
		m_dwHeight = (DWORD)bm.bmHeight;
		m_dwBPP    = (DWORD)bm.bmBitsPixel;
	}

	m_dwTileWidth = dwTileWidth;
	m_dwTileHeight = dwTileHeight;

	m_dwWidthNum = m_dwWidth / m_dwTileWidth;
	m_dwHeightNum = m_dwHeight / m_dwTileHeight;

	return S_OK;
}

HRESULT TileTextures::LoadImageData(TCHAR* strTexturePath)
{
	TCHAR* strExtension;
	TCHAR  strPathname[256];

	m_hbmBitmap[0] = (HBITMAP)LoadImage( GetModuleHandle(NULL), m_strName,
										 IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION );

	if( m_hbmBitmap[0] )
		return S_OK;

	// First check if the file exists in the global texture path
	lstrcpy( strPathname, strTexturePath );
	lstrcat( strPathname, m_strName );

	PFILE* file;
	if( NULL == ( file = pfopen( strPathname, "rb" ) ) )
		return DDERR_NOTFOUND;
	pfclose(file);

	// Get the filename extension
	if( NULL == ( strExtension = _tcsrchr( m_strName, _T('.') ) ) )
		return DDERR_UNSUPPORTED;

	// Load bitmap files
	if( !lstrcmpi( strExtension, _T(".bmp") ) )
		return LoadBitmapFile( strPathname );
	else if( !lstrcmpi( strExtension, _T(".wxy") ) )
		return LoadWxyFile( strPathname );

	// Can add code here to check for other file formats before failing
	return DDERR_UNSUPPORTED;
}

// -------------------------------------------------------------------------------
//
//
// -------------------------------------------------------------------------------
HRESULT TileTextures::LoadBitmapFile( TCHAR* strPathname )
{
	// Try to load the bitmap as a file
	m_hbmBitmap[0] = (HBITMAP)VLoadImage( NULL, strPathname, IMAGE_BITMAP, 0, 0,
										  LR_LOADFROMFILE|LR_CREATEDIBSECTION );

	if (g_bMipmap)
	{
		char strMipmapName1[_MAX_FNAME + _MAX_EXT], strMipmapName2[_MAX_FNAME + _MAX_EXT];

		int len = strlen(strPathname);

		strncpy(strMipmapName1, strPathname, len-4);
		strncpy(strMipmapName2, strPathname, len-4);
		strMipmapName1[len-4] = NULL;
		strMipmapName2[len-4] = NULL;
		strcat(strMipmapName1, "1.bmp");
		strcat(strMipmapName2, "2.bmp");

		m_hbmBitmap[1] = (HBITMAP)VLoadImage( NULL, strMipmapName1, IMAGE_BITMAP, 0, 0,
											  LR_LOADFROMFILE|LR_CREATEDIBSECTION );
		m_hbmBitmap[2] = (HBITMAP)VLoadImage( NULL, strMipmapName2, IMAGE_BITMAP, 0, 0,
											  LR_LOADFROMFILE|LR_CREATEDIBSECTION );
	}

	if( m_hbmBitmap[0] )
		return S_OK;

	return DDERR_NOTFOUND;
}

HRESULT TileTextures::LoadWxyFile( TCHAR* strPathname )
{
	LPVOID lpBits;
	PFILE *fp;
	BITMAPFILEHEADER bmfHeader;
	BITMAPINFO *pbmInfo;
	char dummy[10];
	PFILE* fptemp = new PFILE_NFS();
	bool decode = true;
	
	if(!fptemp->Open(strPathname, "rb"))
	{
		decode = false;
	}
	else
		fptemp->Close();

	if( (fp = pfopen(strPathname, "rb")) == NULL )
		return DDERR_NOTFOUND;

	long fileSize = getFileSize(fp);
	unsigned char* stream = new unsigned char[fileSize];
	unsigned char* streamOrg = stream;
	pfread(stream, fileSize, 1, fp);
	if(decode)
		pDecode(stream, fileSize);

	readData(&dummy, 6, 1, &stream);
	
	
	readData((LPSTR)&bmfHeader, sizeof(bmfHeader), 1, &stream);

	pbmInfo = (BITMAPINFO *)::GlobalAlloc(GMEM_FIXED, sizeof(BITMAPINFO) + sizeof(RGBQUAD)*256);
	if (pbmInfo == NULL)
	{
		pfclose(fp);
		return E_FAIL;
	}

	readData(pbmInfo, sizeof(BITMAPINFO) + sizeof(RGBQUAD)*256, 1, &stream);

	BITMAPINFO &bmInfo = *pbmInfo ;

	m_hbmBitmap[0] = CreateDIBSection(NULL, pbmInfo, DIB_RGB_COLORS, &lpBits, NULL, 0);

	stream = streamOrg + (bmfHeader.bfOffBits + 6);
	int  bytes = (bmInfo.bmiHeader.biWidth * bmInfo.bmiHeader.biBitCount + 15) / 16 * 2;
	bytes *= bmInfo.bmiHeader.biHeight;
	readData(lpBits, bytes, 1, &stream);

	::GlobalFree(pbmInfo);
	pfclose(fp);
	fp = NULL;
	SAFE_DELETE_ARRAY(streamOrg);

	if (g_bMipmap && g_bDungeon != TRUE)
	{
		//////////////////////////////////////////////////////////////////////////
		char strMipmapName1[_MAX_FNAME + _MAX_EXT], strMipmapName2[_MAX_FNAME + _MAX_EXT];

		int len = strlen(strPathname);

		strncpy(strMipmapName1, strPathname, len-4);
		strncpy(strMipmapName2, strPathname, len-4);
		strMipmapName1[len-4] = NULL;
		strMipmapName2[len-4] = NULL;
		strcat(strMipmapName1, "1.wxy");
		strcat(strMipmapName2, "2.wxy");
		
		if( (fp = pfopen(strMipmapName1, "rb")) == NULL )
			return DDERR_NOTFOUND;

		fileSize = getFileSize(fp);
		stream = new unsigned char[fileSize];
		streamOrg = stream;
		pfread(stream, fileSize, 1, fp);
		if(decode)
			pDecode(stream, fileSize);

		// Read Dummy header
		readData(&dummy, 6, 1, &stream);

		// Read file header
		readData((LPSTR)&bmfHeader, sizeof(bmfHeader), 1, &stream);

		pbmInfo = (BITMAPINFO *)::GlobalAlloc(GMEM_FIXED, sizeof(BITMAPINFO) + sizeof(RGBQUAD)*256);
		if (pbmInfo == NULL)
		{
			pfclose( fp );
			return E_FAIL;
		}

		readData(pbmInfo, sizeof(BITMAPINFO) + sizeof(RGBQUAD)*256, 1, &stream);

		BITMAPINFO &bmInfo1 = *pbmInfo;

		m_hbmBitmap[1] = CreateDIBSection(NULL, pbmInfo, DIB_RGB_COLORS, &lpBits, NULL, 0);

		stream = streamOrg + (bmfHeader.bfOffBits + 6);
		bytes = (bmInfo1.bmiHeader.biWidth * bmInfo1.bmiHeader.biBitCount + 15) / 16 * 2;
		bytes *= bmInfo1.bmiHeader.biHeight;
		readData(lpBits, bytes, 1, &stream);

		::GlobalFree(pbmInfo);

		pfclose(fp);
		SAFE_DELETE_ARRAY(streamOrg);

		//////////////////////////////////////////////////////////////////////////
		fp = NULL;
		
		if( (fp = pfopen(strMipmapName2, "rb")) == NULL )
			return DDERR_NOTFOUND;

		fileSize = getFileSize(fp);
		stream = new unsigned char[fileSize];
		streamOrg = stream;
		pfread(stream, fileSize, 1, fp);
		if(decode)
			pDecode(stream, fileSize);

		// Read Dummy header
		readData(&dummy, 6, 1, &stream);

		// Read file header
		readData((LPSTR)&bmfHeader, sizeof(bmfHeader), 1, &stream);

		pbmInfo = (BITMAPINFO *)::GlobalAlloc(GMEM_FIXED, sizeof(BITMAPINFO) + sizeof(RGBQUAD)*256);
		if (pbmInfo == NULL)
		{
			pfclose( fp );
			return NULL;
		}

		readData(pbmInfo, sizeof(BITMAPINFO) + sizeof(RGBQUAD)*256, 1, &stream);

		BITMAPINFO &bmInfo2 = *pbmInfo ;

		m_hbmBitmap[2] = CreateDIBSection(NULL, pbmInfo, DIB_RGB_COLORS, &lpBits, NULL, 0);

		stream = streamOrg + (bmfHeader.bfOffBits + 6);
		bytes = (bmInfo2.bmiHeader.biWidth * bmInfo2.bmiHeader.biBitCount + 15) / 16 * 2;
		bytes *= bmInfo2.bmiHeader.biHeight;
		readData(lpBits, bytes, 1, &stream);

		::GlobalFree(pbmInfo);

		pfclose(fp);
		SAFE_DELETE_ARRAY(streamOrg);
	}

	return S_OK;
}

HRESULT TileTextures::Restore(LPDIRECT3DDEVICE7 pd3dDevice)
{
	// Release any previously created objects
	if (m_ppddsSurface != NULL)
	{
		Invalidate();
		DeleteTextureSurfaces();
	}

	// Check params
	if( NULL == pd3dDevice )
		return DDERR_INVALIDPARAMS;

	// Get the device caps
	D3DDEVICEDESC7 ddDesc;
	if( FAILED( pd3dDevice->GetCaps( &ddDesc) ) )
		return E_FAIL;

	// Setup the new surface desc  : 타일 크기의 서피스에 대한 설정
	// 타일 텍스처는 비디오 메모리에 생성을 한다.
	DDSURFACEDESC2 ddsd;
	if (g_bMipmap)
	{
		D3DUtil_InitSurfaceDesc( ddsd );
		ddsd.dwFlags         = DDSD_CAPS|DDSD_MIPMAPCOUNT|DDSD_HEIGHT|DDSD_WIDTH|
							   DDSD_PIXELFORMAT|DDSD_TEXTURESTAGE;
		ddsd.ddsCaps.dwCaps  = DDSCAPS_TEXTURE|DDSCAPS_MIPMAP|DDSCAPS_COMPLEX;
		ddsd.dwMipMapCount   = m_dwMipmapCount;
		ddsd.dwTextureStage  = m_dwStage;
		ddsd.dwWidth         = m_dwTileWidth;
		ddsd.dwHeight        = m_dwTileHeight;
	}
	else
	{
		D3DUtil_InitSurfaceDesc( ddsd );
		ddsd.dwFlags         = DDSD_CAPS|DDSD_HEIGHT|DDSD_WIDTH|
							   DDSD_PIXELFORMAT|DDSD_TEXTURESTAGE;
		ddsd.ddsCaps.dwCaps  = DDSCAPS_TEXTURE;
		ddsd.dwTextureStage  = m_dwStage;
		ddsd.dwWidth         = m_dwTileWidth;
		ddsd.dwHeight        = m_dwTileHeight;
	}

	// Turn on texture management for hardware devices
	if( ddDesc.deviceGUID == IID_IDirect3DHALDevice )
		ddsd.ddsCaps.dwCaps2 = DDSCAPS2_TEXTUREMANAGE;
	else if( ddDesc.deviceGUID == IID_IDirect3DTnLHalDevice )
		ddsd.ddsCaps.dwCaps2 = DDSCAPS2_TEXTUREMANAGE;
	else
		ddsd.ddsCaps.dwCaps |= DDSCAPS_SYSTEMMEMORY;

	// Adjust width and height to be powers of 2, if the device requires it
	if( ddDesc.dpcTriCaps.dwTextureCaps & D3DPTEXTURECAPS_POW2 )
	{
		for( ddsd.dwWidth=1;  m_dwTileWidth>ddsd.dwWidth;   ddsd.dwWidth<<=1 );
		for( ddsd.dwHeight=1; m_dwTileHeight>ddsd.dwHeight; ddsd.dwHeight<<=1 );
	}

	// Limit max texture sizes, if the driver can't handle large textures
	DWORD dwMaxWidth  = ddDesc.dwMaxTextureWidth;
	DWORD dwMaxHeight = ddDesc.dwMaxTextureHeight;
	ddsd.dwWidth  = min( ddsd.dwWidth,  ( dwMaxWidth  ? dwMaxWidth  : 256 ) );
	ddsd.dwHeight = min( ddsd.dwHeight, ( dwMaxHeight ? dwMaxHeight : 256 ) );

	// Make the texture square, if the driver requires it
	if( ddDesc.dpcTriCaps.dwTextureCaps & D3DPTEXTURECAPS_SQUAREONLY )
	{
		if( ddsd.dwWidth > ddsd.dwHeight ) ddsd.dwHeight = ddsd.dwWidth;
		else                               ddsd.dwWidth  = ddsd.dwHeight;
	}

	// Setup the structure to be used for texture enumration.
	TEXTURESEARCHINFO tsi;
	tsi.bFoundGoodFormat = FALSE;
	tsi.pddpf            = &ddsd.ddpfPixelFormat;
	tsi.dwDesiredBPP     = m_dwBPP;
	//tsi.bUsePalette      = ( m_dwBPP <= 8 );
	tsi.bUsePalette      = FALSE;
	tsi.bUseAlpha        = m_bHasAlpha;
	tsi.bSetAlpha		 = FALSE;
	if( m_dwFlags & D3DTEXTR_16BITSPERPIXEL )
		tsi.dwDesiredBPP = 16;
	else if( m_dwFlags & D3DTEXTR_32BITSPERPIXEL )
		tsi.dwDesiredBPP = 32;

	if( m_dwFlags & (D3DTEXTR_TRANSPARENTWHITE|D3DTEXTR_TRANSPARENTBLACK|D3DTEXTR_TRANSPARENTALPHA) )
	{
		if( tsi.bUsePalette )
		{
			if( ddDesc.dpcTriCaps.dwTextureCaps & D3DPTEXTURECAPS_ALPHAPALETTE )
			{
				tsi.bUseAlpha   = TRUE;
				tsi.bUsePalette = TRUE;
			}
			else
			{
				tsi.bUseAlpha   = TRUE;
				tsi.bUsePalette = FALSE;
			}
		}
	}

	// Enumerate the texture formats, and find the closest device-supported
	// texture pixel format
	pd3dDevice->EnumTextureFormats( TextureSearchCallback, &tsi );

	// If we couldn't find a format, let's try a default format
	if( FALSE == tsi.bFoundGoodFormat )
	{
		tsi.bUsePalette  = FALSE;
		tsi.dwDesiredBPP = 16;
		pd3dDevice->EnumTextureFormats( TextureSearchCallback, &tsi );

		// If we still fail, we cannot create this texture
		if( FALSE == tsi.bFoundGoodFormat )
			return E_FAIL;
	}

	// Get the DirectDraw interface for creating surfaces
	LPDIRECTDRAW7        pDD;
	LPDIRECTDRAWSURFACE7 pddsRender;
	pd3dDevice->GetRenderTarget( &pddsRender );
	pddsRender->GetDDInterface( (VOID**)&pDD );
	pddsRender->Release();

	// 타일의 갯수만큼 서피스를 생성.
	m_ppddsSurface = new LPDIRECTDRAWSURFACE7[m_dwWidthNum*m_dwHeightNum];

	for (int i=0; i < (int) (m_dwWidthNum*m_dwHeightNum); i++)
	{
		HRESULT hr = pDD->CreateSurface(&ddsd, &m_ppddsSurface[i], NULL);
		if( FAILED(hr) )
			return hr;
	}

	// Done with DDraw
	pDD->Release();

	// For bitmap-based textures, copy the bitmap image.
	if( m_hbmBitmap[0] )
		return CopyBitmapToMultipleSurface();

	return S_OK;
}

void TileTextures::DeleteTextureSurfaces()
{
	for (unsigned int i = 0; i < m_dwWidthNum*m_dwHeightNum; i++)
	{
		SAFE_RELEASE( m_ppddsSurface[i] );
	}

	SAFE_DELETE_ARRAY( m_ppddsSurface );
}

HRESULT TileTextures::CopyBitmapToMultipleSurface()
{
	unsigned int i;

	// Get a DDraw object to create a temporary surface
	LPDIRECTDRAW7 pDD;
	m_ppddsSurface[0]->GetDDInterface( (VOID**)&pDD );

	// Get the bitmap structure (to extract width, height, and bpp)
	BITMAP bm;
	GetObject( m_hbmBitmap[0], sizeof(BITMAP), &bm );

	// Setup the new surface desc
	DDSURFACEDESC2 ddsd;
	ddsd.dwSize = sizeof(ddsd);
	m_ppddsSurface[0]->GetSurfaceDesc( &ddsd );
	ddsd.dwFlags          = DDSD_CAPS|DDSD_HEIGHT|DDSD_WIDTH|DDSD_PIXELFORMAT|
							DDSD_TEXTURESTAGE;
	ddsd.ddsCaps.dwCaps   = DDSCAPS_TEXTURE|DDSCAPS_SYSTEMMEMORY;
	ddsd.ddsCaps.dwCaps2  = 0L;
	ddsd.dwWidth          = m_dwTileWidth;
	ddsd.dwHeight         = m_dwTileHeight;

	// Create a new surface for the texture
	// 밉맵 체인은 3레벨까지 만든다.
	LPDIRECTDRAWSURFACE7 pddsTempSurface[3];
	HRESULT hr;
	if (g_bMipmap)
	{
		for (i=0; i < m_dwMipmapCount; i++)
		{
			ddsd.dwWidth = m_dwTileWidth >> i;			// dwWidth와 dwHeight가 0이 되지 않게 한다.
			ddsd.dwHeight = m_dwTileHeight >> i;

			if( FAILED( hr = pDD->CreateSurface( &ddsd, &pddsTempSurface[i], NULL ) ) )
			{
				pDD->Release();
				return hr;
			}
		}
	}
	else
	{
		if( FAILED( hr = pDD->CreateSurface( &ddsd, &pddsTempSurface[0], NULL ) ) )
		{
			pDD->Release();
			return hr;
		}
		pddsTempSurface[1] = NULL;
		pddsTempSurface[2] = NULL;
	}

	// Get a DC for the bitmap
	HDC hdcBitmap[3];
	HBITMAP * hOldBmp[3];

	hdcBitmap[0] = CreateCompatibleDC( NULL );
	if (g_bMipmap)
	{
		hdcBitmap[1] = CreateCompatibleDC( NULL );
		hdcBitmap[2] = CreateCompatibleDC( NULL );
	}
	if( NULL == hdcBitmap[0] )
	{
		pddsTempSurface[0]->Release();
		pDD->Release();
		return hr;
	}



	hOldBmp[0] = (HBITMAP*)SelectObject( hdcBitmap[0], m_hbmBitmap[0] );

	for (i=1; i < m_dwMipmapCount; i++)
	{
		hOldBmp[i] = (HBITMAP*)SelectObject( hdcBitmap[i], m_hbmBitmap[i]);
	}	

	// Copy the bitmap image to the surface.
	HDC hdcSurface[3];

	for (i=0; i < m_dwWidthNum*m_dwHeightNum; i++)
	{
		if (g_bMipmap)
		{
			// chain의 갯수만큼 서피스를 만들어 준다.
			DWORD dwTileWidth, dwTileHeight;

			LPDIRECTDRAWSURFACE7 pddsDest = m_ppddsSurface[i];
			for (DWORD j=0; j < m_dwMipmapCount; j++)
			{
				dwTileWidth = m_dwTileWidth >> j;
				dwTileHeight = m_dwTileHeight >> j;

				if( SUCCEEDED( pddsTempSurface[j]->GetDC( &hdcSurface[j] ) ) )
				{
					BitBlt( hdcSurface[j], 0, 0, dwTileWidth, dwTileHeight, hdcBitmap[j],
							(i % m_dwWidthNum)*dwTileWidth, (i/m_dwWidthNum)*dwTileHeight,
							SRCCOPY );
					pddsTempSurface[j]->ReleaseDC( hdcSurface[j] );
				}
				// Copy the temp surface to the real texture surface
				hr = pddsDest->Blt( NULL, pddsTempSurface[j], NULL, DDBLT_WAIT, NULL );

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
		}
		else
		{
			if( SUCCEEDED( pddsTempSurface[0]->GetDC( &hdcSurface[0] ) ) )
			{
				BitBlt( hdcSurface[0], 0, 0, m_dwTileWidth, m_dwTileHeight, hdcBitmap[0],
						(i % m_dwWidthNum)*m_dwTileWidth, (i/m_dwWidthNum)*m_dwTileHeight,
						SRCCOPY );
				pddsTempSurface[0]->ReleaseDC( hdcSurface[0] );
			}
			// Copy the temp surface to the real texture surface
			m_ppddsSurface[i]->Blt( NULL, pddsTempSurface[0], NULL, DDBLT_WAIT, NULL );
		}
	}


	if (g_bMipmap)
	{
		for (i=0; i < m_dwMipmapCount; i++)
		{
			SelectObject( hdcBitmap[i] , hOldBmp[i] );
			DeleteDC (hdcBitmap[i]);
			pddsTempSurface[i]->Release();
		}
	}
	else
	{
		SelectObject( hdcBitmap[0] , hOldBmp[0] );
		DeleteDC( hdcBitmap[0] );
		pddsTempSurface[0]->Release();
	}

	pDD->Release();

	return S_OK;
}

HRESULT TileTextures::Invalidate()
{
	if (m_ppddsSurface != NULL)
	{
		// m_dwTileWidth * m_dwTileHeight 갯수만큼의 서피스를 release 한다.
		for (unsigned int i = 0; i < m_dwWidthNum*m_dwHeightNum; i++)
		{
			SAFE_RELEASE( m_ppddsSurface[i] );
		}
	}

	return S_OK;
}

LPDIRECTDRAWSURFACE7 TileTextures::GetSurfaceByIdx( int Idx)
{
	if (m_ppddsSurface != NULL)
		return m_ppddsSurface[Idx];

	return NULL;
}
