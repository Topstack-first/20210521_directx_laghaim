#ifndef _FX_BOSSRAID_HYBRIDER1_02
#define _FX_BOSSRAID_HYBRIDER1_02

#include "Config.h"

class CLolos;

class CFxBossRaidHybri1_02
{
protected:

	CLolos*			m_pLolo; //날아가는 구체, 바닥

public:
	CFxBossRaidHybri1_02 ();
	virtual ~CFxBossRaidHybri1_02 ();

	void	LoadRes();
	void	DeleteRes();
	bool	Render(EffectSort &effect_sort);
};

#endif // _FX_BOSSRAID_HYBRIDER1_02