#ifndef _FX_SHIELD_BLOCK_H_
#define _FX_SHIELD_BLOCK_H_

#include "Config.h"

class CFxShieldBlock
{
public:
	CFxShieldBlock();
	virtual ~CFxShieldBlock();

	CLolos		*m_pLolo;
	void		LoadRes();
	void		DeleteRes();
	BOOL		Render(EffectSort &effect_sort);
};

#endif // _FX_SHIELD_BLOCK_H_
