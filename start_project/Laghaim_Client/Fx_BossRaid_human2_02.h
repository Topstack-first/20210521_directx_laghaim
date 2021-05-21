#ifndef _FX_BOSSRAID_HUMAN2_02
#define _FX_BOSSRAID_HUMAN2_02

#include "Config.h"

class CLolos;

class CFxBossRaidHuman2_02
{
protected:
	D3DVECTOR vPos[4];

public:
	CFxBossRaidHuman2_02();
	virtual ~CFxBossRaidHuman2_02();

	CLolos		*m_pLolo[2]; //날아가는 구체, 바닥

	void	LoadRes();
	void	DeleteRes();
	bool	Render(EffectSort &effect_sort);
};

#endif // _FX_BOSSRAID_HUMAN2_02