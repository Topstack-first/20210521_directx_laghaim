#ifndef _FX_DEMI_CRUSHER_H_
#define _FX_DEMI_CRUSHER_H_

#include "Config.h"

class CLolos;

class CFxDemiCrusher
{
	CLolos					*m_pLolo;
	D3DLVERTEX				m_lsmoke[4];

public:
	CFxDemiCrusher();
	virtual ~CFxDemiCrusher();
	void		Create();
	void		LoadRes();
	void		DeleteRes();
	BOOL		Render(EffectSort &effect_sort);
};

#endif // _FX_DEMI_CRUSHER_H_
