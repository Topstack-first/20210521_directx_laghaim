#ifndef _FX_WATERFALL_PARTICLE_H_
#define _FX_WATERFALL_PARTICLE_H_
// �������̵� �޸ռ� �߾ӿ� ���� ���� ����Ʈ

#include "Config.h"
class CLolos;

// �޸� ������ �߾� ��� ������Ʈ ����Ʈ
class CFxWaterfallParticle
{
	CRectPlane*	m_Plane;

public:
	CFxWaterfallParticle();
	virtual ~CFxWaterfallParticle();

	void Create();
	void Update_Particle(EffectSort &ef_sort);

	void LoadRes();
	void DeleteRes();

	BOOL Render(EffectSort &ef_sort);
};

#endif