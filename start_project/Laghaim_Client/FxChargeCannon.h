#ifndef _FX_CHARGE_CANNON_H_
#define _FX_CHARGE_CANNON_H_

#include "Config.h"

class CFxChargeCannon
{
	LPD3DLVERTEX			m_cannon;

	float					m_fScale;
	float					m_fFade;

	int						m_nTotalFrame;


public:
	CFxChargeCannon();
	virtual ~CFxChargeCannon();

	void Create();

	void LoadRes(IndexedTexture *pIndexedTexture);
	void DeleteRes();

	BOOL Render(EffectSort &effect_sort);

};

#endif // _FX_CHARGE_CANNON_H_

