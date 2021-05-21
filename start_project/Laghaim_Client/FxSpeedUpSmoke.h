#ifndef _FX_SPEED_UP_SMOKE_H_
#define _FX_SPEED_UP_SMOKE_H_

#include "Config.h"

class CFxSpeedUpSmoke
{
	D3DLVERTEX				m_lsmoke[4];

public:
	CFxSpeedUpSmoke();
	virtual ~CFxSpeedUpSmoke();
	void		LoadRes();
	void		DeleteRes();
	BOOL		Render(EffectSort &effect_sort);
};

#endif // _FX_SPEED_UP_SMOKE_H_
