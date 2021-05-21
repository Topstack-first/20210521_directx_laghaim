#ifndef _FX_FLYING_CRASH2_H_
#define _FX_FLYING_CRASH2_H_

#include "Config.h"

class CLolos;

class CFxFlyingCrash2
{
	CSphere*				eng_sphere;

public:
	CLolos *m_pLolo;

	CFxFlyingCrash2();
	virtual ~CFxFlyingCrash2();

	void		LoadRes();
	void		DeleteRes();
	void		Render(EffectSort &effect_sort);
};

#endif // _FX_FLYING_CRASH2_H_
