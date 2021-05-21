#ifndef _FX_ADRENALINE_H_
#define _FX_ADRENALINE_H_

#include "Config.h"

class CFxAdrenaline
{
	CRectPlane*				circle_up;
	CRectPlane*				circle_down;

	float					m_fScale;
	float					m_fFade;
	int						m_nTotalFrame;

public:
	CFxAdrenaline();
	virtual ~CFxAdrenaline();

	void LoadRes();
	void DeleteRes();

	BOOL Render(EffectSort &effect_sort);
};

#endif // _FX_ADRENALINE_H_
