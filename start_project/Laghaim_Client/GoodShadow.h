#ifndef _GOOD_SHADOW_H_
#define _GOOD_SHADOW_H_

#include "Config.h"

#include "ChaAct.h"

#define DRGTEXTR_TRANSPARENTWHITE 0x00000002
#define	DRGTEXTR_TRANSPARENTBLACK 0x00000004
#define DRGTEXTR_32BITSPERPIXEL   0x00000008
#define DRGTEXTR_RENDERTARGET		0x00000010 //create texture w. DDSCAPS_3DDEVICE, to render to it.  		
#define DRGTEXTR_FORCEINAGP			0x00000020 //Specifies to create the texture in agp memory
//some flags added for embossing and bump mapping
#define DRGTEXTR_CREATEINVHEIGHTMAP 0x00000040 //will create a texture which is (0.5 - src texture)
#define DRGTEXTR_CREATEBUMPMAP_DUDV 0x00000080 //will create with DUDV caps bits set


//#define RENDER_TARGET_WIDTH		100
//#define RENDER_TARGET_HEIGHT	179
//#define SHADOW_TARGET_WIDTH		150
//#define SHADOW_TARGET_HEIGHT	250
#define SHADOW_TARGET_WIDTH		128
#define SHADOW_TARGET_HEIGHT	128

class CDisplay;
class CLolos;
class IndexedTexture;
class CNkCharacter;
class CSMRender;

class CGoodShadow
{
	LPDIRECTDRAWSURFACE7	m_pddsSmRenderTarget;

	int m_Xpos, m_Ypos;
	int m_Width, m_Height;

	int						m_nCurFrame[NUM_RACE];
	float					m_fTimeElapsed[NUM_RACE];

	DDSURFACEDESC2       m_ddsd;
	HBITMAP              m_hbmBitmap;
	DWORD m_dwStage;
	DWORD m_dwFlags;
	DWORD m_dwWidth, m_dwHeight;

	LPDIRECTDRAWSURFACE7 m_wasLpRenderTarget;
	D3DVIEWPORT7 m_vpOld;
	DWORD	m_dwColor;
	D3DMATRIX matProjOld, m_matWorldOld, m_matViewOld;
	DWORD m_lightingOld;
	BOOL m_lightEnable0, m_lightEnable1, m_lightEnable2;


public:
	CGoodShadow();
	virtual ~CGoodShadow();

	void Init();
	HRESULT CreateSurface();
	HRESULT CreateTexture();
	HRESULT CreateTexture2();
	HRESULT Restore();
	HRESULT CopyBitmapToSurface( LPDIRECTDRAWSURFACE7 pddsTarget,
								 HBITMAP hbmBitmap,
								 HBITMAP hbmAlphaBitmap,
								 DWORD dwFlags );
	// 삭제.
	void Invalidate();

	LPDIRECTDRAWSURFACE7 GetSurface();

	BOOL					Start(D3DMATRIX *matRotate, int cur_hour);
	void					End(BOOL bStarted);
	void					Render2D();

	// 테스트 - 정원
	CSMRender		*m_pSMRender;
	void			DeleteRes();
	// 테스트 - 끝.

	bool			CheckRestore();
};

#endif // _GOOD_SHADOW_H_
