#ifndef _FX_HELL_DOWN_LIGHTLY_H_
#define _FX_HELL_DOWN_LIGHTLY_H_

#include "Config.h"

class CFxHellDown_Lightly
{
	CRectPlane*				m_pLightly;

	float					m_fScale;
	float					m_fFade;
	int						m_nTotalFrame;

public:
	CFxHellDown_Lightly();
	virtual ~CFxHellDown_Lightly();

	void Create();

	void LoadRes(IndexedTexture *pIndexedTexture);
	void DeleteRes();
	void Render(EffectSort &effect_sort);
};

#endif // _FX_HELL_DOWN_LIGHTLY_H_
