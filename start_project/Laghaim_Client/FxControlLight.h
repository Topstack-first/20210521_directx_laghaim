#ifndef _FX_CONTROL_LIGHT_H_
#define _FX_CONTROL_LIGHT_H_

#include "Config.h"

class CFxControlLight
{
	CCylinderPlane*			gauge;
	CCylinderPlane*			gauge_real;

	float					m_fScale;
	float					m_fFade;

	int						m_nTotalFrame;

public:
	CFxControlLight();
	virtual ~CFxControlLight();

	void Create();

	void LoadRes(IndexedTexture *pIndexedTexture);
	void DeleteRes();

	BOOL Render(EffectSort &effect_sort);

};

#endif // _FX_CONTROL_LIGHT_H_
