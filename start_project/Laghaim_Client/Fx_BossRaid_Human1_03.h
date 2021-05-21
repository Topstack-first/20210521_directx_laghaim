#ifndef _FX_BOSSRAID_HUMAN1_03__
#define _FX_BOSSRAID_HUMAN1_03__

#include "Config.h"
#define FX_BOSSRAID_HUMAN1_03_MAX_COUNT 1
class CLolos;

class CFxBossRaidHuman1_03
{
protected:

	CLolos*	m_pLoloR;	// ������ ���ư��� �������� ����Ʈ
	CLolos*	m_pLoloL;	// ������ ���ư��� �������� ����Ʈ
	DWORD	m_BeforTime;

public:
	CFxBossRaidHuman1_03();
	virtual ~CFxBossRaidHuman1_03();

	void	LoadRes();
	void	FrameMove( EffectSort &ef_sort, int nAddFrame );
	void	DeleteRes();
	BOOL	Render(EffectSort &effect_sort);
};


#endif // _FX_BOSSRAID_BULKAN1_02


