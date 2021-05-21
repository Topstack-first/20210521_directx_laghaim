#ifndef _FX_LIFE_LIGHT_H_
#define _FX_LIFE_LIGHT_H_

#include "Config.h"

class CFxLifeLight
{
	CRectPlane*				life_light;
	// nate 2004 - 8 ImageManager
//	CRectPlane*				corona;
//	DWORD					m_lifeTexId;
//	DWORD					m_coronaTexId;

	float					m_fScale;
	float					m_fFade;

	// nate 2004 - 7 - image
//	IndexedTexture		*m_pIndexedTexture;
	int						m_nTotalFrame;


public:
	CFxLifeLight();
	virtual ~CFxLifeLight();

	void Create();

	void LoadRes(IndexedTexture *pIndexedTexture);
	void DeleteRes();

	BOOL Render(EffectSort &effect_sort);


};

#endif // _FX_LIFE_LIGHT_H_
