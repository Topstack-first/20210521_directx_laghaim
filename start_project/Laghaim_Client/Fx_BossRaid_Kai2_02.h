#ifndef _FX_BOSSRADID_KAI2_02_H_
#define _FX_BOSSRADID_KAI2_02_H_

#include "Config.h"

class CFxBossRaidKai2_02
{
public:
	CFxBossRaidKai2_02();
	virtual ~CFxBossRaidKai2_02();

	CLolos		*m_pLolo;
	void		LoadRes();
	void		DeleteRes();
	BOOL		Render(EffectSort &effect_sort);
};

#endif