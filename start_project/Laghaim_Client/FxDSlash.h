#ifndef _FX_DSLASH_H_
#define _FX_DSLASH_H_

#include "Config.h"

class CFxDSlash
{
public:

	CFxDSlash();
	virtual ~CFxDSlash();

	CRectPlane*				hit; //-- ������
	CRectPlane*				hit2; //-- ������ Plane
	CRectPlane*				hit3; //-- Dev �� TEMP
	CRectPlane*				hit4; //-- Dev �� TEMP

	float					m_fScale;
	float					m_fFade;
	BOOL Render(EffectSort &effect_sort);

};

#endif // _FX_DSLASH_H_