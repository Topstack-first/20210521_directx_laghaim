#ifndef _FX_BOSSRAID_HYNRIDER2_02__
#define _FX_BOSSRAID_HYNRIDER2_02__

#include "Config.h"
#define FX_BOSSRAID_HYBRIDER2_02_MAX_PARTICLE 40
class CLolos;

class CFxBossRaidHybri2_02
{
protected:

	CLolos*	m_pLolo;	// 앞으로 나아가는 기형태의 이펙트

public:
	CFxBossRaidHybri2_02();
	virtual ~CFxBossRaidHybri2_02();

	void	LoadRes();
	void	FrameMove( EffectSort &ef_sort, int nAddFrame );
	void	DeleteRes();
	BOOL	Render(EffectSort &effect_sort);
};


#endif // _FX_BOSSRAID_BULKAN1_02


