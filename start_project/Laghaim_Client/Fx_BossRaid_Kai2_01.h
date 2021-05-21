#ifndef _FX_KAI2_01_H_
#define _FX_KAI2_01_H_

#include "Config.h"

class CFxBossRaidKai2_01
{
public:
	CFxBossRaidKai2_01();
	virtual ~CFxBossRaidKai2_01();

	CLolos		*m_pLolo;
	void		LoadRes();
	void		DeleteRes();
	BOOL		Render(EffectSort &effect_sort);
};

#endif