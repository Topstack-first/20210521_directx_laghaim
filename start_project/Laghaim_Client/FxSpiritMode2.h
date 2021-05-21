#ifndef _FX_SPIRIT_MODE2_H_
#define _FX_SPIRIT_MODE2_H_

#include "Config.h"

class CFxSpiritMode2
{
	CCylinderPlane*			around_ring;
	CCylinderPlane*			around_ring2;

	float					m_fScale;
	float					m_fFade;
	int						m_nTotalFrame;

public:
	CFxSpiritMode2();
	virtual ~CFxSpiritMode2();

	void LoadRes();
	void DeleteRes();

	BOOL Render(EffectSort &effect_sort);

};

#endif // _FX_SPIRIT_MODE2_H_
