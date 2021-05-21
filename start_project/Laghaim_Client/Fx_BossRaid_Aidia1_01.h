#ifndef _FX_BOSSRAID_AIDIA1_01_
#define _FX_BOSSRAID_AIDIA1_01_

#include "Config.h"

class CLolos;

class CFxBossRaidAidia1_01
{
public:
	CFxBossRaidAidia1_01();
	virtual ~CFxBossRaidAidia1_01();

	CLolos		*m_pLolo;
	void		LoadRes();
	void		DeleteRes();
	BOOL		Render(EffectSort &effect_sort);
};

#endif