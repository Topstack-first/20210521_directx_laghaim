#ifndef _FX_SCROLL_RING_H_
#define _FX_SCROLL_RING_H_

#include "Config.h"

class CFxScrollRing
{
	CRectPlane*				ring;

	// nate 2004 - 8 ImageManager
//	DWORD					m_ringTexId;

	float					m_fScale;
	float					m_fFade;

	// nate 2004 - 7 - image
//	IndexedTexture		*m_pIndexedTexture;
	int						m_nTotalFrame;


public:
	CFxScrollRing();
	virtual ~CFxScrollRing();

	void Create();

	void LoadRes(IndexedTexture *pIndexedTexture);
	void DeleteRes();

	BOOL Render(EffectSort &effect_sort);

};

#endif // _FX_SCROLL_RING_H_
