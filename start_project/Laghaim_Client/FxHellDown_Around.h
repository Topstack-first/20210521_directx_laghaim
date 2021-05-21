#ifndef _FX_HELL_DOWN_AROUND_H_
#define _FX_HELL_DOWN_AROUND_H_

#include "Config.h"

class CFxHellDown_Around
{
	CCylinderPlane*			m_pAround_Ring;
	float					m_fScale;
	int						m_nTotalFrame;

public:
	CFxHellDown_Around();
	virtual ~CFxHellDown_Around();

	void Create();
	void LoadRes(IndexedTexture *pIndexedTexture);
	void DeleteRes();
	void Render(EffectSort &effect_sort);
};

#endif // _FX_HELL_DOWN_AROUND_H_
