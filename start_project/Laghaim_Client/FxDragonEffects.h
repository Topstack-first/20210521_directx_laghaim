#ifndef _FX_DRAGON_EFFECTS_H_
#define _FX_DRAGON_EFFECTS_H_

#include "Config.h"

class CFxDragonEffects
{
public:
	CFxDragonEffects();
	virtual ~CFxDragonEffects();

	float			m_fSpeed;

	D3DLVERTEX		m_lverTail[4];

	void			LoadRes(IndexedTexture *pIndexedTexture, CShadowRect *pShadowRect = NULL);
	void			DeleteRes();

	BOOL			Render(EffectSort &effect_sort);
};

#endif // _FX_DRAGON_EFFECTS_H_




