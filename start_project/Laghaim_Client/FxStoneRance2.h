#ifndef _FX_STONE_RANCE2_H_
#define _FX_STONE_RANCE2_H_

#include "Config.h"

class CLolos;

class CFxStoneRance2
{
public:
	CFxStoneRance2();
	virtual ~CFxStoneRance2();

	CLolos				*m_pLolo;
	// nate 2004 - 7 - image
//	IndexedTexture		*m_pIndexedTexture;
	// nate 2004 - 7 ±Û·Î¹ú ShadowRect»ç¿ë
//	CShadowRect			*m_pShadowRect;

	// nate 2004 - 8 ImageManager
//	int					m_nBaseTexIdx;

	// nate 2004 - 8 ImageManager
//	void		LoadRes(IndexedTexture *pIndexedTexture, CShadowRect *pShadowRect);
	void		LoadRes();

	void		DeleteRes();

	void		Render(EffectSort &effect_sort);
};


#endif // _FX_STONE_RANCE2_H_


