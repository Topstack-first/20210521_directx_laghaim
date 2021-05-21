#ifndef _FX_AIDIA3_01_H_
#define _FX_AIDIA3_01_H_

#include "Config.h"

class CFxBossRaidAidia3_01
{
public:
	CFxBossRaidAidia3_01();
	virtual ~CFxBossRaidAidia3_01();

	CLolos		*m_pLolo;
	void		LoadRes();
	void		DeleteRes();
	BOOL		Render(EffectSort &effect_sort);
};

#endif