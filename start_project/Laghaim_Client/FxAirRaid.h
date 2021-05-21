#ifndef _FX_AIR_RAID_H_
#define _FX_AIR_RAID_H_

#include "Config.h"

class IndexedTexture;
class CShadowRect;
class CLolos;

class CFxAirRaid
{
public:
	CFxAirRaid();
	virtual ~CFxAirRaid();

	CLolos *m_pLolo;
	CLolos *m_pLolo2;
	float		m_fSpeed;
	void		LoadRes();

	void		DeleteRes();

	BOOL		Render(EffectSort &effect_sort);
};

#endif // _FX_AIR_RAID_H_