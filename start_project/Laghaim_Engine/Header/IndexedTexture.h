#ifndef D3DTEXTR_H
#define D3DTEXTR_H

#include <ddraw.h>
#include <d3d.h>
#include <map>
#include <string>
#include "MemoryPoolBase.h"

HBITMAP VLoadImage(HINSTANCE hinst, LPCTSTR lpszName, UINT uType, int cxDesired, int cyDesired, UINT fuLoad);

//-----------------------------------------------------------------------------
// Texture creation and deletion functions
//-----------------------------------------------------------------------------
#define D3DTEXTR_TRANSPARENTWHITE 0x00000001
#define D3DTEXTR_TRANSPARENTBLACK 0x00000002
#define D3DTEXTR_TRANSPARENTALPHA 0x00000020

#define D3DTEXTR_32BITSPERPIXEL   0x00000004
#define D3DTEXTR_16BITSPERPIXEL   0x00000008
#define D3DTEXTR_CREATEWITHALPHA  0x00000010

#define CREATE_ADD		(-1)
#define CREATE_INSERT	(-2)

struct TargaHeader
{
	BYTE IDLength;
	BYTE ColormapType;
	BYTE ImageType;
	BYTE ColormapSpecification[5];
	WORD XOrigin;
	WORD YOrigin;
	WORD ImageWidth;
	WORD ImageHeight;
	BYTE PixelDepth;
	BYTE ImageDescriptor;
};

class TextureContainer : public MemoryPoolBase<TextureContainer>
{
public:
	TextureContainer(std::string strName, DWORD dwStage, DWORD dwFlags, DWORD transAlpha , BOOL packed=true );
	~TextureContainer();

	HRESULT LoadImageData(const TCHAR* strTexturePath, const TCHAR* strTexturePath2, const TCHAR* strTexturePath3);
	HRESULT LoadImageData();
	HRESULT LoadBitmapFile2( TCHAR* strPathname );
	HRESULT LoadBitmapFile( TCHAR* strPathname, bool bDummy = false );
	HRESULT LoadWxyFile( TCHAR* strPathname );
	HRESULT LoadTargaFile( TCHAR* strPathname, bool bDummy = false );
	HRESULT LoadSekFile( TCHAR* strPathname );
	HRESULT Restore( LPDIRECT3DDEVICE7 pd3dDevice );

	HRESULT CopyToSurface(BOOL& bRet);

	HRESULT CopyBitmapToSurface();
	HRESULT CopyRGBADataToSurface();

	HRESULT SetAlpha(DWORD transAlpha);

	HRESULT LoadNTXFile( TCHAR* strPathname );
	HRESULT CopyNTXDataToSurface();

public:
	TCHAR   m_strName[80];
	TCHAR	m_strPathName[_MAX_FNAME + _MAX_EXT];
	DWORD   m_dwWidth;
	DWORD   m_dwHeight;
	DWORD   m_dwStage;
	DWORD   m_dwBPP;
	DWORD   m_dwFlags;
	BOOL    m_bHasAlpha;

	LPDIRECTDRAWSURFACE7 m_pddsSurface;
	HBITMAP m_hbmBitmap;
	DWORD*  m_pRGBAData;
	DWORD	m_transAlpha;

	HWND	m_hWnd;
	DWORD	m_dwMipmapCount;

	BYTE**	m_ppNTXData;

	bool	m_packed;
};

//////////////////////////////////////////////////////////////////////////

class IndexedTexture
{
public:
	IndexedTexture();
	IndexedTexture(int size);
	~IndexedTexture();	

	LPDIRECTDRAWSURFACE7 GetSurface( TCHAR* strName );
	LPDIRECTDRAWSURFACE7 GetSurfaceByIdx( int Idx );

	HRESULT Invalidate( TCHAR* strName );
	HRESULT Restore( TCHAR* strName, LPDIRECT3DDEVICE7 pd3dDevice );
	HRESULT RestoreByIdx( int Idx, LPDIRECT3DDEVICE7 pd3dDevice );
	HRESULT InvalidateAllTextures();
	HRESULT RestoreAllTextures( LPDIRECT3DDEVICE7 pd3dDevice );
	
	int CreateTextureFromFile( TCHAR* strName, int Idx = CREATE_ADD, DWORD dwStage=0L, DWORD dwFlags=0L, DWORD transAlpha = 0xff );
	int CreateTextureFromFileNonePack( TCHAR* strName, int Idx = CREATE_ADD, DWORD dwStage=0L, DWORD dwFlags=0L, DWORD transAlpha = 0xff );
	int CreateTextureFromFile_Imp(std::string strFilename, int index, DWORD dwStage, DWORD dwFlags, DWORD transAlpha, bool bPack);
	int CreateEmptyTexture( TCHAR* strName, DWORD dwWidth, DWORD dwHeight, int Idx, DWORD dwStage, DWORD dwFlags, DWORD transAlpha = 0xff );

	HRESULT DestroyTexture( TCHAR* strName );
	HRESULT DestroyTextureByIdx( int Idx );
	HRESULT DestroyTextureAll(void);
	VOID    SetTexturePath( LPCTSTR strTexturePath );
	VOID    SetTexturePath2( LPCTSTR strTexturePath );
	VOID    SetTexturePath3( LPCTSTR strTexturePath );

	int FindTexture(std::string strTextureName);
	void ReleaseTextureList();

	void SetAlphaAll(DWORD transAlpha = 0xff);
	BOOL SetAlpha(int Idx, DWORD transAlpha);

	BOOL SetSize(int size);

	int FindTextureWXY(TCHAR* strTextureName);

private:
	void addTextureIndex(std::string textureName, int idx);
	void delTextureIndex(std::string textureName);
	int findTextureIndex(std::string textureName);
	std::string getXYZFormatFileName(std::string textureName);

public:
	int m_Size;
	int m_Idx;

	std::string m_TexturePath;
	std::string m_TexturePath2;
	std::string m_TexturePath3;
	TextureContainer** m_TextureList;

private:
	std::map<std::string, int> m_mapTextureIndex;
};

extern IndexedTexture * g_pCapsyongTexture;

//////////////////////////////////////////////////////////////////////////

// 타일용 서피스를 위한 클래스
// tga를 읽는 루틴은 생략하였다.
class TileTextures
{
protected:
	TCHAR   m_strName[80];		// Name of texture (doubles as image filename)
	DWORD   m_dwWidth;			// bitmap의 Width
	DWORD   m_dwHeight;			// bitmap의 height
	DWORD	m_dwTileWidth;		// tile 하나의 width
	DWORD	m_dwTileHeight;		// tile 하나의 height
	DWORD	m_dwWidthNum;		// 서피스의 가로 갯수
	DWORD	m_dwHeightNum;		// 서피스의 세로 갯수
	DWORD   m_dwStage;			// Texture stage (for multitexture devices)
	DWORD   m_dwBPP;
	DWORD   m_dwFlags;
	BOOL    m_bHasAlpha;

	LPDIRECTDRAWSURFACE7	*m_ppddsSurface; // Surface of the texture
	HBITMAP m_hbmBitmap[3];       // Bitmap containing texture image
	DWORD	m_transAlpha;

	DWORD	m_dwMipmapCount;

public:
	TCHAR  m_TexturePath[512]; // Path for files

protected:
	HRESULT LoadImageData(TCHAR* strTexturePath);
	HRESULT LoadBitmapFile( TCHAR* strPathname );
	HRESULT LoadWxyFile( TCHAR* strPathname );

	HRESULT CopyBitmapToMultipleSurface();

public:
	VOID    SetTexturePath( LPCTSTR strTexturePath );
	HRESULT CreateTextureFromFile(TCHAR* strName, DWORD dwTileWidth, DWORD dwTileHeight,
								  DWORD dwStage=0L, DWORD dwFlags=0L, DWORD transAlpha = 0xff );

	//-----------------------------------------------------------------------------
	// Texture invalidation and restoration functions
	//-----------------------------------------------------------------------------
	HRESULT Invalidate();
	HRESULT Restore(LPDIRECT3DDEVICE7 pd3dDevice);

	// Texture Access functions
	LPDIRECTDRAWSURFACE7 GetSurfaceByIdx( int Idx);

	TileTextures(DWORD dwStage=0L, DWORD dwFlags=0L, DWORD transAlpha = 0xff );
	~TileTextures();

	void DeleteTextureSurfaces();
};




#endif // D3DTEXTR_H
