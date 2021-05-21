#ifndef _FX_DSTONE_H_
#define _FX_DSTONE_H_

#include "Config.h"

class CFxDstone
{
public:
	CFxDstone();
	virtual ~CFxDstone();

	CRectPlane*				hit; //-- ������
	CRectPlane*				hit2; //-- ������ Plane
	CRectPlane*				hit3; //-- Dev �� TEMP
	CRectPlane*				hit4; //-- Dev �� TEMP

	float					m_fScale;
	float					m_fFade;

public:

	BOOL Render(EffectSort &effect_sort);

};

#endif // _FX_DSTONE_H_
