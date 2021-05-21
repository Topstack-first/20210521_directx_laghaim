#ifndef _FX_HERO_EFFECT_
#define _FX_HERO_EFFECT_

#include "Config.h"

#define MAX_HEROEFFECT_PLANE	3 //

class CRectPlane;

class CFxHeroEffect
{
protected:

	CRectPlane*	m_Plane;

public:
	CFxHeroEffect();
	virtual ~CFxHeroEffect();

	void		LoadRes();
	void		DeleteRes();
	void		Create();
	void		FrameMove( EffectSort &ef_sort, int nAddFrame );
	BOOL		Render(EffectSort &effect_sort);
};

#endif

