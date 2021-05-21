#ifndef _FX_TORNADO_HAMMER_H_
#define _FX_TORNADO_HAMMER_H_

#include "Config.h"

class CFxTornadoHammer
{
public:
	CFxTornadoHammer();
	virtual ~CFxTornadoHammer();

	CLolos		*m_pLolo;
	void		LoadRes();
	void		DeleteRes();
	BOOL		Render(EffectSort &effect_sort);
};

#endif // _FX_TORNADO_HAMMER_H_
