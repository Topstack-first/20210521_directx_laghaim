#ifndef _FX_FLY_H_
#define _FX_FLY_H_

#include "Config.h"

#define FX_FLY_HEIGHT	10.0f

class IndexedTexture;
class CLolos;

class CFxFly
{
public:
	CFxFly();
	virtual ~CFxFly();

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

#endif // _FX_FLY_H_
