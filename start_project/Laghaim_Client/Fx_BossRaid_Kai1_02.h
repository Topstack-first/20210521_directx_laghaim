#ifndef _FX_BOSSRAID_KAI1_02
#define _FX_BOSSRAID_KAI1_02

#include "Config.h"

class CLolos;

class CFxBossRaidKai1_02
{
protected:

	CLolos*			m_pLolo; //���ư��� ��ü, �ٴ�

public:
	CFxBossRaidKai1_02  ();
	virtual ~CFxBossRaidKai1_02  ();

	void	LoadRes();
	void	DeleteRes();
	bool	Render(EffectSort &effect_sort);
};

#endif // _FX_BOSSRAID_KAI1_02