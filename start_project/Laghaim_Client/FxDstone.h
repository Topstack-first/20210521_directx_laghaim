#ifndef _FX_DSTONE_H_
#define _FX_DSTONE_H_

#include "Config.h"

class CFxDstone
{
public:
	CFxDstone();
	virtual ~CFxDstone();

	CRectPlane*				hit; //-- ºôº¸µå
	CRectPlane*				hit2; //-- ¶¥À§ÀÇ Plane
	CRectPlane*				hit3; //-- Dev ¿ë TEMP
	CRectPlane*				hit4; //-- Dev ¿ë TEMP

	float					m_fScale;
	float					m_fFade;

public:

	BOOL Render(EffectSort &effect_sort);

};

#endif // _FX_DSTONE_H_
