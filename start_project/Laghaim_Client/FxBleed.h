#ifndef _FX_BLEED_H_
#define _FX_BLEED_H_

#include "Config.h"

class CFxBleed
{
public:
	CFxBleed();
	virtual ~CFxBleed();

	D3DLVERTEX		m_flame[4];                 //-- Effect Billboard
	D3DVECTOR			vPosFrom;                   //-- Effect ÀÇ ÁÂÇ¥

	BOOL Render(EffectSort &effect_sort);

};

#endif // _FX_BLEED_H_
