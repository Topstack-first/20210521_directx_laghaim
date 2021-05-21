#ifndef _FX_FIRESMOKE_PARTICLE_
#define _FX_FIRESMOKE_PARTICLE_

#include "Config.h"

class CRectPlane;

class CFxFireSmokeParticle
{
protected:

	CRectPlane*	m_Plane;

public:
	CFxFireSmokeParticle();
	virtual ~CFxFireSmokeParticle();

	void		LoadRes();
	void		DeleteRes();
	void		Create();
	void		FrameMove( EffectSort &ef_sort, int nAddFrame );
	BOOL		Render(EffectSort &effect_sort);
};

#endif

