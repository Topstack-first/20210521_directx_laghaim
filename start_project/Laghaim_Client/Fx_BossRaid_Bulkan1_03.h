#ifndef _FX_BOSSRAID_BULKAN1_03__
#define _FX_BOSSRAID_BULKAN1_03__

#include "Config.h"

class CLolos;

class CFxBossRaidBulkan1_03
{
protected:

	CLolos*	m_pLolo;	// ������ ���ư��� �������� ����Ʈ
	DWORD	m_BeforTime;

public:
	CFxBossRaidBulkan1_03();
	virtual ~CFxBossRaidBulkan1_03();

	void	LoadRes();
	void	FrameMove( EffectSort &ef_sort, int nAddFrame );
	void	DeleteRes();
	void	Render(EffectSort &effect_sort);
};


#endif // _FX_BOSSRAID_BULKAN1_02


