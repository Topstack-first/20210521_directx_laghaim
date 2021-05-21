#ifndef _FX_LIFE_COMMON_H_
#define _FX_LIFE_COMMON_H_

#include "Config.h"

class CFxLifeCommon
{
	CCylinderPlane*			life_ring;

	// nate 2004 - 8 ImageManager
//	DWORD					m_lifeTexId;

	float					m_fScale;
	float					m_fFade;

	// nate 2004 - 7 - image
//	IndexedTexture		*m_pIndexedTexture;
	int						m_nTotalFrame;

public:
	CFxLifeCommon();
	virtual ~CFxLifeCommon();

	void Create();

	void LoadRes(IndexedTexture *pIndexedTexture);
	void DeleteRes();

	BOOL Render(EffectSort &effect_sort);


};

#endif // _FX_LIFE_COMMON_H_
