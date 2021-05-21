#ifndef _FX_PROTECTION_SHIELD_H_
#define _FX_PROTECTION_SHIELD_H_

#include "Config.h"

class CLolos;
class CFxProtectionShield
{
	int			m_nTotalFrame;

public:
	CFxProtectionShield();
	virtual ~CFxProtectionShield();

	CLolos		*m_pLolo;
	CLolos		*m_pLolo2;
	void		LoadRes();
	void		DeleteRes();
	BOOL		Render(EffectSort &effect_sort);
};

#endif // _FX_PROTECTION_SHIELD_H_
