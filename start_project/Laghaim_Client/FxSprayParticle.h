#ifndef _FX_SPRAY_PARTICLE_H_
#define _FX_SPRAY_PARTICLE_H_

#include "Config.h"

class CFxSprayParticle
{
	CRectPlane*				spray;

	// nate 2004 - 8 ImageManager
//	DWORD					m_sprayTexId;

	// nate 2004 - 7 - image
//	IndexedTexture		*m_pIndexedTexture;

public:
	CFxSprayParticle();
	virtual ~CFxSprayParticle();

	void Create();
	void Update_Spray_Particle(EffectSort &effect_sort);

	void LoadRes(IndexedTexture *pIndexedTexture);
	void DeleteRes();

	BOOL Render(EffectSort &effect_sort);
};

#endif // _FX_SPRAY_PARTICLE_H_