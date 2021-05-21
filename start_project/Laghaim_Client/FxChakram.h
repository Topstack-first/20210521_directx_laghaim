#ifndef _FX_CHAKRAM_H_
#define _FX_CHAKRAM_H_

#include "Config.h"

class CLolos;

class CFxChakram
{
public:
	CFxChakram();
	virtual ~CFxChakram();

	float		m_fSpeed;

	void		LoadRes(IndexedTexture *pIndexedTexture, CShadowRect *pShadowRect);
	void		DeleteRes();

	BOOL		Render(EffectSort &effect_sort);
};

#endif // _FX_CHAKRAM_H_