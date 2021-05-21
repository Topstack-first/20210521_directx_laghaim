#ifndef _FX_BOSSRAID_BULKAN3_03
#define _FX_BOSSRAID_BULKAN3_03

#define FX_BOSSRAID_BULKAN3_02_FRAMELIMITE	61

#include "Config.h"

class IndexedTexture;
class CShadowRect;
class CLolos;
class CRectPlane;
class CFxBossRaidBulkan3_03
{
public:
	CFxBossRaidBulkan3_03();
	virtual ~CFxBossRaidBulkan3_03();

	CLolos *m_pLolo;

	CRectPlane	*smoke;

	float		m_fSpeed;

	void		LoadRes();

	void		DeleteRes();

	BOOL		Render(EffectSort &effect_sort);
};

#endif