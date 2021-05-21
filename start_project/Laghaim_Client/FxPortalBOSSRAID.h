#ifndef _FX_PORTAL_BOSSRAID_
#define _FX_PORTAL_BOSSRAID_

#include "Config.h"

class CRectPlane;

class CFxPortalBossraid
{
protected:

	CRectPlane*	m_Plane;

public:
	CFxPortalBossraid();
	virtual ~CFxPortalBossraid();

	void		LoadRes();
	void		DeleteRes();
	void		Create();
	void		FrameMove( EffectSort &ef_sort, int nAddFrame );
	BOOL		Render(EffectSort &effect_sort);
};

#endif

