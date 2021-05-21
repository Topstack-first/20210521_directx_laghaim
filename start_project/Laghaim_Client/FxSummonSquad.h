#ifndef _FX_SUMMON_SQUAD_H_
#define _FX_SUMMON_SQUAD_H_

#include "Config.h"

class CFxSummonSquad
{
protected:

	CRectPlane*		m_pRect;

public:

	CFxSummonSquad();
	virtual ~CFxSummonSquad();

	void LoadRes();
	void DeleteRes();
	BOOL Render( EffectSort &effect_sort );
};

#endif // _FX_SUMMON_SQUAD_H_
