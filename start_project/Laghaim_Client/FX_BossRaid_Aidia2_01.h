#ifndef _FX_BOSSRAID_ADIA2_01
#define _FX_BOSSRAID_ADIA2_01

#include "Config.h"

#define FX_BOSSRAID_ADIA2_01_MAX_COUNT	3
#define FX_BOSSRAID_ADIA2_01_MAX_PARTICLE 40

class CLolos;
class CRectPlane;

class CFxBossRaidAidia2_01
{
protected:
	CLolos* m_pLolo;

public:
	CFxBossRaidAidia2_01();
	virtual ~CFxBossRaidAidia2_01();

	void	LoadRes();
	void	DeleteRes();
	bool	Render(EffectSort &effect_sort);
};

#endif // _FX_BOSSRAID_ADIA2_01