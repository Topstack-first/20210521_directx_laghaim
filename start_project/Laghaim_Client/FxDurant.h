#ifndef _FX_DURANT_H_
#define _FX_DURANT_H_

#include "Config.h"

class CFxDurant
{
public:
	CFxDurant();
	virtual ~CFxDurant();

	void		LoadRes();
	void		DeleteRes();
	BOOL		Render(EffectSort &effect_sort);

	CLolos		*m_pLolo;
	CRectPlane	*smoke;
	float		m_fSpeed;
};

#endif // _FX_DURANT_H_
