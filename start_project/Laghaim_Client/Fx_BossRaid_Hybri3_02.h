#ifndef _FX_HYBIRBOSS3_2_H_
#define _FX_HYBIRBOSS3_2_H_

#include "Config.h"

class CFxBossRaidHybri3_02
{
public:
	CFxBossRaidHybri3_02();
	virtual ~CFxBossRaidHybri3_02();

	CLolos		*m_pLolo;
	void		LoadRes();
	void		DeleteRes();
	BOOL		Render(EffectSort &effect_sort);
};

#endif