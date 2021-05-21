// Nk3DMipmap.h: interface for the Nk3DMipmap class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NK3DMIPMAP_H__1F0E6470_38FC_4626_B44E_DEA4BB6688B2__INCLUDED_)
#define AFX_NK3DMIPMAP_H__1F0E6470_38FC_4626_B44E_DEA4BB6688B2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CNk3DMipMap
{
	HBITMAP              m_hbmBitmap[10];     // Bitmaps containing images
	DWORD                m_dwMipMapCount;     // Levels of mipmap
	LPDIRECTDRAWSURFACE7 m_pddsSurface;       // Surface of the mipmap
	char				 m_strFilePath[255];

public:
	CNk3DMipMap();
	virtual ~CNk3DMipMap();

	void	SetTexturePath(char *strFilePath);
	HRESULT Create( CHAR** strMipmapNames, WORD wNum );
	VOID    Delete();
	HRESULT Restore( LPDIRECT3DDEVICE7 pd3dDevice );
	VOID    Invalidate();

	LPDIRECTDRAWSURFACE7 GetSurface()
	{
		return m_pddsSurface;
	};
};
#endif // !defined(AFX_NK3DMIPMAP_H__1F0E6470_38FC_4626_B44E_DEA4BB6688B2__INCLUDED_)
