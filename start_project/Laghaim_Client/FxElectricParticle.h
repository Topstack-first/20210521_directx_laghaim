#ifndef _FX_ELECTRIC_PARTICLE_H_
#define _FX_ELECTRIC_PARTICLE_H_
// 보스레이드 휴먼성 중앙에 들어가는 지형 이펙트

#include "Config.h"

// 휴먼 보스성 중앙 기둥 오브젝트 이펙트
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