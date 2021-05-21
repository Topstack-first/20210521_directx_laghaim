#ifndef _FX_MAGIC_LASER_H_
#define _FX_MAGIC_LASER_H_

#include "Config.h"

class CFxMagicLaser
{
	LPD3DLVERTEX			m_laser;
	CRectPlane*				light;
	float					m_fScale;
	float					m_fFade;
	int						m_nTotalFrame;

public:
	CFxMagicLaser();
	virtual ~CFxMagicLaser();

	void Create();
	void DeleteRes();
	BOOL Render(EffectSort &effect_sort);
};

#endif // _FX_MAGIC_LASER_H_


