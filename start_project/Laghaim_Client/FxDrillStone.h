#ifndef _FX_DRILL_STONE_H_
#define _FX_DRILL_STONE_H_

#include "Config.h"

class CLolos;

class CFxDrillStone
{
public:
	CFxDrillStone();
	virtual ~CFxDrillStone();

	CLolos				*m_pLolo;
	void		LoadRes();

	void		DeleteRes();

	BOOL		Render(EffectSort &effect_sort);
};


#endif // FX_DRILL_STONE_H
