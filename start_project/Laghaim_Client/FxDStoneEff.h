#ifndef _FX_DSTONE_EFF_H_
#define _FX_DSTONE_EFF_H_

#include "Config.h"

class CFxDStoneEff
{
public:
	CFxDStoneEff();
	virtual ~CFxDStoneEff();

	BOOL Render(EffectSort &effect_sort);

	CRectPlane*				corona1;
	CRectPlane*				corona2;
	CRectPlane*				corona3;  //-- Dummy
	float m_fScale0;
	float m_fScale1;
	float m_fFade0;
	float m_fFade1;
	float m_rotang;

};

#endif // _FX_DSTONE_EFF_H_
