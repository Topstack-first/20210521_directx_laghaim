#ifndef _FX_BASE_H_
#define _FX_BASE_H_

#include "Config.h"

class CLolos;
class CFxBash
{
	CSphere*				eng_sphere;

public:
	CLolos *m_pLolo;

	CFxBash();
	virtual ~CFxBash();

	void		LoadRes();
	void		Render(EffectSort &effect_sort);

};

#endif // _FX_BASE_H_