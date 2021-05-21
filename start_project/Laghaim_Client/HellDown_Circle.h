#ifndef _HELL_DOWN_CIRCLE_H_
#define _HELL_DOWN_CIRCLE_H_

#include "Config.h"

class CFxHellDown_Circle
{
	CRectPlane*				m_pMasic_Circle;

	float					m_fScale;
	float					m_fFade;
	int						m_nTotalFrame;

public:
	CFxHellDown_Circle();
	virtual ~CFxHellDown_Circle();

	void Create();
	void LoadRes(IndexedTexture *pIndexedTexture);
	void DeleteRes();
	void Render(EffectSort &effect_sort);
};

#endif // _HELL_DOWN_CIRCLE_H_
