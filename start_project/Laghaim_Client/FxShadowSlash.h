#ifndef _FX_SHADOW_SLASH_H_
#define _FX_SHADOW_SLASH_H_

#include "Config.h"

class CFxShadowSlash
{
protected:
	CRectPlane*	m_pSmoke;

public:
	CFxShadowSlash();
	virtual ~CFxShadowSlash();

	void LoadRes();
	void DeleteRes();
	BOOL Render( EffectSort &effect_sort );
};

#endif // _X_SHADOW_SLASH_H_
