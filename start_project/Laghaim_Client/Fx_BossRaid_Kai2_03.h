#ifndef _FX_BOSSRAID_KAILIPTON2_03_
#define _FX_BOSSRAID_KAILIPTON2_03_
// �������̵� ī�̸��� 2�� ���� 3��° ���� ���� ����Ʈ
// ��ũ �ҿ� ���� ����Ʈ�� �̿��Ѵ�.

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