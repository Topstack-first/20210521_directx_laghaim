#ifndef _FX_MOTH_H_
#define _FX_MOTH_H_

#include "Config.h"

#define FX_MOTH_HEIGHT	10.0f

class IndexedTexture;
class CLolos;

class CFxMoth
{
public:
	CFxMoth();
	virtual ~CFxMoth();

	// nate 2004 - 7 - image
//	IndexedTexture		*m_pIndexedTexture;

	float	m_fSpeed;
	CLolos	*m_pLolo;

	// nate 2004 - 8 ImageManager
//	void		LoadRes(IndexedTexture *pIndexedTexture, CShadowRect *pShadowRect = NULL);
	void		LoadRes();

	void		DeleteRes();

	BOOL		Render(EffectSort &effect_sort);
};

#endif // _FX_MOTH_H_
