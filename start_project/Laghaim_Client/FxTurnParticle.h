#ifndef _FX_TURN_PARTICLE_H_
#define _FX_TURN_PARTICLE_H_

#include "Config.h"

class CFxTurnParticle
{
	CRectPlane*				turn;

	// nate 2004 - 8 ImageManager
//	DWORD					m_turnTexId;

	// nate 2004 - 7 - image
//	IndexedTexture		*m_pIndexedTexture;

public:
	CFxTurnParticle();
	virtual ~CFxTurnParticle();

	void Create();
	void Update_Turn_Particle(EffectSort &effect_sort);

	void LoadRes(IndexedTexture *pIndexedTexture);
	void DeleteRes();

	BOOL Render(EffectSort &effect_sort);
};

#endif // _FX_TURN_PARTICLE_H_

