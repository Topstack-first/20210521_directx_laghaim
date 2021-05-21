#ifndef _FX_STONE_BRUST_H_
#define _FX_STONE_BRUST_H_

#include "Config.h"

class CFxStoneBrust
{

public:
	CFxStoneBrust();
	virtual ~CFxStoneBrust();

	BOOL Render(EffectSort &effect_sort);

	CRectPlane*				m_Rect_center;
	CCylinderPlane*			m_Cyli_outside;

	float m_fScale;
	float m_fScale2;

};

#endif // _FX_STONE_BRUST_H_
