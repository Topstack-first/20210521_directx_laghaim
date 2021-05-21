#ifndef _SM_RENDER_H_
#define _SM_RENDER_H_

#include "Config.h"

#include "ChaAct.h"

//#define RENDER_TARGET_WIDTH		150
//#define RENDER_TARGET_HEIGHT	250
#define RENDER_TARGET_WIDTH		204
#define RENDER_TARGET_HEIGHT	256

class CDisplay;
class CLolos;
class IndexedTexture;
class CNkCharacter;

class CSMRender
{
	LPDIRECTDRAWSURFACE7	m_pddsSmRenderTarget;
	LPDIRECTDRAWSURFACE7	m_pddsZBuffer;
	int m_Xpos, m_Ypos;
	int m_Width, m_Height;

	// µ•¿Ã≈∏.
	HRESULT					Load3DResources();
	int						m_nCurFrame[NUM_RACE];
	float					m_fTimeElapsed[NUM_RACE];

	BOOL					m_bWireframe;

public:
	CSMRender();
	virtual ~CSMRender();

	LPDIRECTDRAWSURFACE7	GetRenderTarget()
	{
		return m_pddsSmRenderTarget;
	}

	void					Init(int width = RENDER_TARGET_WIDTH, int height = RENDER_TARGET_HEIGHT);
	void					SetRenderWireframe(BOOL bWire)
	{
		m_bWireframe = bWire;
	};

	void					LoadResources();

	HRESULT					CreateDevices();

	void DeleteRes();

	void					RenderCha(int x, int y, int width, int height, CNkCharacter *pNkCha, int page_num, BOOL bSelected = 0);

	bool					CheckRestore();
	HRESULT					Restore();
};
#endif // _SM_RENDER_H_
