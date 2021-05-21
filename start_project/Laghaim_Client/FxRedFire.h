#ifndef _FX_RED_FIRE_H_
#define _FX_RED_FIRE_H_

#include "Config.h"

class IndexedTexture;

class CFxRedFire
{
public:
	CFxRedFire();
	virtual ~CFxRedFire();

	// nate 2004 - 7 - image
//	IndexedTexture		*m_pIndexedTexture;

	// 데이타 멤버와 상수.
	// nate 2004 - 8 ImageManager
//	WORD		m_wTexIdx;
//	int			m_nTotalFrame;

	D3DLVERTEX	m_lverRect[4];
	float		m_fLightSize[4];

	void		LoadRes(IndexedTexture *pIndexedTexture);
	void		DeleteRes();

	void		FrameMove();
	void		Render(EffectSort &effect_sort);
};

#endif // _FX_RED_FIRE_H_
