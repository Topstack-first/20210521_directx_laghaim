#ifndef _FX_ELECTRIC_HAMMER_H_
#define _FX_ELECTRIC_HAMMER_H_

#include "Config.h"

class CLolos;
class CFxElectricHammer
{

	CRectPlane*				corona1;
	CRectPlane*				light1;
	CRectPlane*				light2;
	CRectPlane*				light3;
	CRectPlane*				light4;
	CSphere*				eng_sphere;

	float m_fScale0;
	float m_fScale1;
	float m_fFade;
	float m_rotang;
	int	m_nTotalFrame;
	int	m_numTLight;

public:
	CFxElectricHammer();
	virtual ~CFxElectricHammer();

	CLolos		*m_pLolo;
	void		LoadRes();
	void		DeleteRes();
	void		Render(EffectSort &effect_sort);
};

#endif // _FX_ELECTRIC_HAMMER_H_
