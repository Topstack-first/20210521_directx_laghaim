#ifndef _FX_ELECTRIC_PARTICLE_H_
#define _FX_ELECTRIC_PARTICLE_H_
// �������̵� �޸ռ� �߾ӿ� ���� ���� ����Ʈ

#include "Config.h"

// �޸� ������ �߾� ��� ������Ʈ ����Ʈ
class CFxElectricParticle
{
	CRectPlane*	electric;

public:
	CFxElectricParticle();
	virtual ~CFxElectricParticle();

	void Create();
	void Update_Particle(EffectSort &ef_sort);

	void LoadRes();
	void DeleteRes();

	BOOL Render(EffectSort &ef_sort);
};

#endif