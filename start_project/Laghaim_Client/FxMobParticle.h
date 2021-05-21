#ifndef _FX_MOB_PARTICLE_H_
#define _FX_MOB_PARTICLE_H_

#include "Config.h"

class CFxMobParticle
{
	CRectPlane*				light;

	// nate 2004 - 8 ImageManager
//	DWORD					m_lightTexId;

	// nate 2004 - 7 - image
//	IndexedTexture		*m_pIndexedTexture;

public:
	CFxMobParticle();
	virtual ~CFxMobParticle();

	void LoadRes(IndexedTexture *pIndexedTexture);
	void DeleteRes();

	void Update_Mob_Particle(EffectSort &effect_sort);
	BOOL Render(EffectSort &effect_sort);
};

#endif // _FX_MOB_PARTICLE_H_


