#ifndef _FX_BOSSRAID_AIDIA1_02_
#define _FX_BOSSRAID_AIDIA1_02_

#include "Config.h"

#define FX_BOSSRAID_AIDIA1_02_MAXCOUNT_1 7  // �⸦ ���̴� ����Ʈ
#define FX_BOSSRAID_AIDIA1_02_MAXCOUNT_2 20  // �� �ִ� �� ����Ʈ

class CFxBossRaidAidia1_02
{
protected:

	CRectPlane*	m_Plane;

public:
	CFxBossRaidAidia1_02();
	virtual ~CFxBossRaidAidia1_02();

	void		LoadRes();
	void		DeleteRes();
	void		Create();
	void		FrameMove( EffectSort &ef_sort, int nAddFrame );
	BOOL		Render(EffectSort &effect_sort);
};

#endif