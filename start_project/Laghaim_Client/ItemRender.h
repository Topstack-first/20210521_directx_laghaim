#ifndef _ITEM_RENDER_H_
#define _ITEM_RENDER_H_

#include "Config.h"
#include "ChaAct.h"

#define BASE_X_SIZE		640
#define BASE_Y_SIZE		480

//#define RENDER_TARGET_WIDTH		100
//#define RENDER_TARGET_HEIGHT	179
#define ITEM_RENDER_WIDTH		(54)//	* m_ScreenWidth / BASE_X_SIZE)	// (27*2)
#define ITEM_RENDER_HEIGHT		(108)// * m_ScreenHeight / BASE_Y_SIZE)	// (27*4)
#define ITEM_KEY_COLOR		(0x00ff00ff)
#define ITEM_KEY_COLOR_REF	RGB(0xff, 0, 0xff)
//#define ITEM_KEY_COLOR		(0x00ffffff)
//#define ITEM_KEY_COLOR_REF	RGB(0xff, 0xff, 0xff)

#define UPGRADE_ITEM_COLOR	(0x0058ff4c)
#define UPGRADE_ITEM_COLOR_REF	RGB(0x58, 0xff, 0x4c)
#define SPECIAL_ITEM_COLOR_REF	RGB(0xff, 0x58, 0x77)

class CLolos;
class IndexedTexture;
class CNkCharacter;

class CItemRender
{
	LPDIRECTDRAWSURFACE7	m_pddsRenderTarget, m_lpOldRenderTarget;
	LPDIRECTDRAWSURFACE7	m_pddsZBuffer;
	LPDIRECTDRAWSURFACE7	m_pddsBlt;
	int m_Xpos, m_Ypos;
	int m_Width, m_Height;
	DWORD m_lastTime;
	int m_Frame;

	D3DLIGHT7 m_d3dLight;

	// 데이타.
	HRESULT					Load3DResources();
	int						m_nCurFrame[NUM_RACE];
	float					m_fTimeElapsed[NUM_RACE];

	int		m_ScreenWidth;
	int		m_ScreenHeight;

	D3DVIEWPORT7			m_vpOld, m_vpNew;
	D3DMATRIX				m_matWorldOld, m_matView, m_matViewOld, m_matProjOld, m_matProjNew;
	DWORD					m_dwAlphaTest, m_dwAlphaBlend;
	D3DLIGHT7				m_lightOld;
	BOOL					m_lightEnable0, m_lightEnable1, m_lightEnable2;
	DWORD					m_dwColor;

public:
	CItemRender();
	virtual ~CItemRender();

	void					Init();
	DWORD					ConvertGDIColor( COLORREF dwGDIColor );
	// 생성
	void					LoadResources();

	HRESULT					CreateDevices();
	void					MakeLight();

	// 삭제.
	void DeleteRes();

	// 렌더링
	//void					Draw(int idx, int x, int y, BOOL bSelected);
	//void					Render(int idx, DWORD dwAmbiColor);
	void					BeginRenderLolo();
	void					RenderLolo(int x, int y, CLolos *pLolo, int TextureNum, DWORD dwAmbient = 0x00555555, BOOL bMove = FALSE,
									   int width = -1, int height = -1, BOOL bBeginEnd = FALSE, int sizeX = -1, int sizeY = -1, BOOL bIgronreSize = FALSE, float fScale = 1.0f ); ///롤로텍스쳐변경 을 위한 TextureNum 인지 추가.
	void					EndRenderLolo();

	bool					CheckRestore();
	HRESULT					Restore();
};

#endif // _ITEM_RENDER_H_
