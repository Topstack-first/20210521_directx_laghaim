#ifndef _FX_FROST_H_
#define _FX_FROST_H_

#include "Config.h"

class CLolos;

class CFxFrost
{
	CLolos				*m_pLolo;						// object �ϳ�.

	// nate 2004 - 7 - image
//	IndexedTexture		*m_pIndexedTexture;
	// nate 2004 - 7 �۷ι� ShadowRect���
//	CShadowRect			*m_pShadowRect;

	// nate 2004 - 8 ImageManager
//	int					m_nBaseTexIdx;

public:
	CFxFrost();
	virtual ~CFxFrost();

	// nate 2004 - 8 ImageManager
//	void		LoadRes(IndexedTexture *pIndexedTexture, CShadowRect *pShadowRect);
	void		LoadRes();

	void		DeleteRes();

	void		Render(EffectSort &effect_sort);
};

#endif // _FX_FROST_H_
