#ifndef _FX_BOSSRAID_AIDIA1_01_Shockwave
#define _FX_BOSSRAID_AIDIA1_02_Shockwave

#include "Config.h"

class CLolos;

class CFxBossRaidAidia1_02_Shockwave
{
public:
	CFxBossRaidAidia1_02_Shockwave();
	virtual ~CFxBossRaidAidia1_02_Shockwave();

	CLolos		*m_pLolo;
	void		LoadRes();
	void		DeleteRes();
	BOOL		Render(EffectSort &effect_sort);
};

#endif