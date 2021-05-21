#ifndef _FX_FLYING_CRASH_H_
#define _FX_FLYING_CRASH_H_

#include "Config.h"

class CFxFlyingCrash
{
	CRectPlane*				ground_ring;
	CRectPlane*				ground_corona;

	float					m_fScale;
	float					m_fFade;

	int						m_nTotalFrame;


public:
	CFxFlyingCrash();
	virtual ~CFxFlyingCrash();

	void LoadRes();
	void DeleteRes();

	BOOL Render(EffectSort &effect_sort);

};

#endif // _FX_FLYING_CRASH_H_
