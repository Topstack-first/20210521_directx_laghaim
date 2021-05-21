#ifndef _FX_HELL_DOWN_CIRCLE2_H_
#define _FX_HELL_DOWN_CIRCLE2_H_

#include "Config.h"

class CFxHellDown_Circle2
{
	CRectPlane*				m_pMasic_Circle;

	float					m_fScale;
	float					m_fFade;
	int						m_nTotalFrame;

public:
	CFxHellDown_Circle2();
	virtual ~CFxHellDown_Circle2();

	void Create();

	void LoadRes(IndexedTexture *pIndexedTexture);
	void DeleteRes();

	void Render(EffectSort &effect_sort);
};

#endif // _FX_HELL_DOWN_CIRCLE2_H_
