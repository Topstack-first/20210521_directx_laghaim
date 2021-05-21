#ifndef _FX_AIDIA3_02_H_
#define _FX_AIDIA3_02_H_

#include "Config.h"

class CFxBossRaidAidia3_02
{
public:
	CFxBossRaidAidia3_02();
	virtual ~CFxBossRaidAidia3_02();

	CLolos		*m_pLolo;
	void		LoadRes();
	void		DeleteRes();
	BOOL		Render(EffectSort &effect_sort);
};

#endif