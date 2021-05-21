#ifndef _FX_ICE_BLOT_H_
#define _FX_ICE_BLOT_H_

#include "Config.h"

class CLolos;
class CFxIceBlot
{
public:
	CFxIceBlot();
	virtual ~CFxIceBlot();

	CLolos		*m_pLolo;
	void		LoadRes();
	void		DeleteRes();
	void		Render(EffectSort &effect_sort);
};

#endif // _FX_ICE_BLOT_H_
