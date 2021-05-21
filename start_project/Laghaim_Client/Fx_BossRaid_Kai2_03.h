#ifndef _FX_BOSSRAID_KAILIPTON2_03_
#define _FX_BOSSRAID_KAILIPTON2_03_
// 보스레이드 카이립톤 2번 보스 3번째 광역 공격 이펙트
// 다크 소울 공격 이팩트를 이용한다.

#include "Config.h"
class CLolos;

#define FX_BOSSRAID_KAI2_03_MAX_PARTICLE 50

class CFxBossRaidKai2_03
{
	CRectPlane*	m_Plane;

public:
	CFxBossRaidKai2_03();
	virtual ~CFxBossRaidKai2_03();

	void Create();
	void Update_Particle(EffectSort &ef_sort);

	void LoadRes();
	void DeleteRes();

	BOOL Render(EffectSort &ef_sort);
	BOOL RenderLightningBall(EffectSort &ef_sort);
	BOOL RenderMagicCycle(EffectSort &ef_sort);
};

#endif