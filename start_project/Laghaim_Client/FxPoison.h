#ifndef _FX_POISON_H_
#define _FX_POISON_H_

#include "Config.h"

class CFxPoison
{
protected:
	CRectPlane*	m_pSmoke;
	CLolos*		m_pLolo;

public:
	CFxPoison();
	virtual ~CFxPoison();

	void LoadRes();
	void DeleteRes();
	BOOL Render( EffectSort &effect_sort );
};

#endif // _FX_POISON_H_