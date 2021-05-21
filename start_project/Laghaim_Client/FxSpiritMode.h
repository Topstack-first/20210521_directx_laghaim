#ifndef _FX_SPIRIT_MODE_H_
#define _FX_SPIRIT_MODE_H_

#include "Config.h"

class CFxSpiritMode
{
	CCylinderPlane*			around_ring;
	CCylinderPlane*			around_ring2;

	float					m_fScale;
	float					m_fFade;
	int						m_nTotalFrame;

public:
	CFxSpiritMode();
	virtual ~CFxSpiritMode();

	void LoadRes();
	void DeleteRes();

	BOOL Render(EffectSort &effect_sort);

};

#endif // _FX_SPIRIT_MODE_H_
