#ifndef _FX_SMOKE_PARTICLE_H_
#define _FX_SMOKE_PARTICLE_H_

#include "Config.h"

class CFxSmokeParticle
{
	CRectPlane*				smoke;

	// nate 2004 - 8 ImageManager
//	DWORD					m_smokeTexId;

	// nate 2004 - 7 - image
//	IndexedTexture		*m_pIndexedTexture;

public:
	CFxSmokeParticle();
	virtual ~CFxSmokeParticle();

	void Create();
	void Update_Smoke_Particle(EffectSort &effect_sort);

	void LoadRes(IndexedTexture *pIndexedTexture);
	void DeleteRes();

	BOOL Render(EffectSort &effect_sort);
};

#endif // _FX_SMOKE_PARTICLE_H_
