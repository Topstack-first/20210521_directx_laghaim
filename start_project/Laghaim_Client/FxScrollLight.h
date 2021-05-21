#ifndef _FX_SCROLL_LIGHT_H_
#define _FX_SCROLL_LIGHT_H_

#include "Config.h"

class CFxScrollLight
{
	CRectPlane*				scroll_light;

	// nate 2004 - 8 ImageManager
//	DWORD					m_lightTexId;

	float					m_fScale;
	float					m_fFade;

	// nate 2004 - 7 - image
//	IndexedTexture		*m_pIndexedTexture;
	int						m_nTotalFrame;


public:
	CFxScrollLight();
	virtual ~CFxScrollLight();

	void Create();

	void LoadRes(IndexedTexture *pIndexedTexture);
	void DeleteRes();

	BOOL Render(EffectSort &effect_sort);

};

#endif // _FX_SCROLL_LIGHT_H_
