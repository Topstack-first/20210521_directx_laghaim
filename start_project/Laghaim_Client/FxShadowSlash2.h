#ifndef _FX_SHADOW_SLASH2_H_
#define _FX_SHADOW_SLASH2_H_

#include "Config.h"

class CFxShadowSlash2
{
	CCylinderPlane*			around_ring;

	float					m_fScale;
	float					m_fFade;
	int						m_nTotalFrame;

public:
	CFxShadowSlash2();
	virtual ~CFxShadowSlash2();

	void LoadRes();
	void DeleteRes();

	BOOL Render(EffectSort &effect_sort);

};

#endif // _FX_SHADOW_SLASH2_H_
