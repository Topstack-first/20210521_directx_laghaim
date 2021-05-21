#ifndef _FX_LIFE_EXPANSION_H_
#define _FX_LIFE_EXPANSION_H_

#include "Config.h"

class CRectPlane;

class CFxLifeExpansion
{
private:
	CRectPlane*	m_pPlane;
	int m_nOldFrame;
	float m_fCurHeight[ 8 ];
	float m_fPlaneFade[ 8 ];

public:
	CFxLifeExpansion();
	virtual ~CFxLifeExpansion();

	void LoadRes();
	void DeleteRes();
	BOOL Render(EffectSort &effect_sort);
};

#endif // _FX_LIFE_EXPANSION_H_
