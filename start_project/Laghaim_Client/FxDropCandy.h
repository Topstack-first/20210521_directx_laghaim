#ifndef _FX_DROP_CANDY_H_
#define _FX_DROP_CANDY_H_

#include "Config.h"

#define FX_DROP_STARCANDY_COUNT	48

class CLolos;

class CFxDropCandy
{
	CRectPlane*				smoke;

public:
	CFxDropCandy();
	virtual ~CFxDropCandy();

	CLolos *m_pLolo;						// object 하나.

	void		LoadRes( int kind );

	void		DeleteRes();

	void		Render(EffectSort &effect_sort);
};


class CFxDropStarCandy
{
public:
	CFxDropStarCandy();
	virtual ~CFxDropStarCandy();

	CLolos *m_pLolo;						// object 하나.

	BOOL		IsActive(int nIndex, int nFrame);

	void		LoadRes();
	void		DeleteRes();
	void		Render(EffectSort &effect_sort);
};

#endif // _FX_DROP_CANDY_H_
