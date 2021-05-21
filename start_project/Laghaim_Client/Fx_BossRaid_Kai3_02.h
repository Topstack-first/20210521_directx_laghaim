#ifndef _FX_KAI3_02_H_
#define _FX_KAI3_02_H_

#include "Config.h"

class CFxBossRaidKai3_02
{
public:
	CFxBossRaidKai3_02();
	virtual ~CFxBossRaidKai3_02();

	CLolos		*m_pLolo;
	void		LoadRes();
	void		DeleteRes();
	BOOL		Render(EffectSort &effect_sort);
};

#endif