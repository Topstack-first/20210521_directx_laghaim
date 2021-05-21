#ifndef _FX_SUMMON_FIRE_H_
#define _FX_SUMMON_FIRE_H_

#include "Config.h"

class CLolos;
class CFxSummonFire
{
	int m_nTotalFrame;
	float m_fScale;
	CRectPlane*		m_rect_magic;

public:
	CFxSummonFire();
	virtual ~CFxSummonFire();

	CLolos		*m_pLolo;
	CLolos		*m_pLolo2;

	void		LoadRes();
	BOOL		Render(EffectSort &effect_sort);
	void		InsertFire( EffectSort &effect_sort, D3DVECTOR vDrawVec );

};

#endif // _FX_SUMMON_FIRE_H_
