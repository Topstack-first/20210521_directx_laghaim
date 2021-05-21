#ifndef _FX_RANDOM_FIRE_H_
#define _FX_RANDOM_FIRE_H_

#include "Config.h"

class CLolos;
class CFxRandomFire
{
	int m_nTotalFrame;
	float m_fScale;
	CRectPlane*		m_rect_magic;

public:
	CFxRandomFire();
	virtual ~CFxRandomFire();

	CLolos		*m_pLolo;
	CLolos		*m_pLolo2;
	CLolos		*m_pLolo3;
	CLolos		*m_pLolo4;

	void		LoadRes();
	void		DeleteRes();
	BOOL		Render(EffectSort &effect_sort);
};

#endif // _FX_RANDOM_FIRE_H_
