#ifndef _FX_BOSSRAID_BULKAN2_02
#define _FX_BOSSRAID_BULKAN2_02

#include "Config.h"

#define FX_BOSSRAID_BULKAN2_02_MAX_SWORD	50
#define FX_BOSSRAID_BULKAN2_02_MAX_EVENT	35

class CLolos;

class CFxBossRaidBulkan2_02
{
protected:
	CLolos*				m_pLolo;	// 앞으로 나아가는 기형태의 이펙트
	CRectPlane*			m_CirclePlane[2];
	static int			m_Event[FX_BOSSRAID_BULKAN2_02_MAX_EVENT];

public:
	CFxBossRaidBulkan2_02();
	virtual ~CFxBossRaidBulkan2_02();

	void	LoadRes();
	void	DeleteRes();
	bool	Render(EffectSort &effect_sort);
};


#endif // _FX_BOSSRAID_BULKAN2_02


