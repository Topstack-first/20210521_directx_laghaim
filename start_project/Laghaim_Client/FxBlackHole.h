#ifndef _FX_BLACK_HOLE_H_
#define _FX_BLACK_HOLE_H_

#include "Config.h"

class CFxBlackHole
{
	CCylinderPlane*			blackhole;

	float					m_fScale;
	float					m_fFade;

	int						m_nTotalFrame;


public:
	CFxBlackHole();
	virtual ~CFxBlackHole();

	void Create();

	void LoadRes(IndexedTexture *pIndexedTexture, CShadowRect *pShadowRect);
	void DeleteRes();

	BOOL Render(EffectSort &effect_sort);

};

#endif // _FX_BLACK_HOLE_H_
