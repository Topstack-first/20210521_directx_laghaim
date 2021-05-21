#ifndef _FX_KAI3_01_H_
#define _FX_KAI3_01_H_

#include "Config.h"

class CFxBossRaidKai3_01
{
public:
	CFxBossRaidKai3_01();
	virtual ~CFxBossRaidKai3_01();

	CLolos		*m_pLolo;
	void		LoadRes();
	void		DeleteRes();
	BOOL		Render(EffectSort &effect_sort);
};

#endif