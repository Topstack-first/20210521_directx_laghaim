#ifndef _FX_GHOST_RISE_H_
#define _FX_GHOST_RISE_H_

#include "Config.h"

class CFxGhostRise
{
	CTailPlane*				m_ghostrise;

	// nate 2004 - 8 ImageManager
//	DWORD					m_ghostriseTexId;
//	int						m_maxGhostRise;
//	int						m_maxTailPos;

	// nate 2004 - 7 - image
//	IndexedTexture		*m_pIndexedTexture;

	D3DXCOLOR*				m_ptailRGB;

public:
	CFxGhostRise();
	virtual ~CFxGhostRise();


	void Create();

	void LoadRes(IndexedTexture *pIndexedTexture);
	void DeleteRes();

	BOOL Render(EffectSort &effect_sort);

};

#endif // _FX_GHOST_RISE_H_


