#ifndef _FX_FLAME_H_
#define _FX_FLAME_H_

#include "Config.h"

class CFxFlame
{
public:
	CFxFlame();
	virtual ~CFxFlame();

	// nate 2004 - 7 - image
//	IndexedTexture		*m_pIndexedTexture;
	// nate 2004 - 7 ±Û·Î¹ú ShadowRect»ç¿ë
//	CShadowRect			*m_pShadowRect;

	// nate 2004 - 8 ImageManager
//	int				m_nTailTexIdx, m_nBaseTexIdx;
//	float			m_fSize, m_fSpeed;
	float			m_fSpeed;

	D3DLVERTEX		m_lverTail[4];

	void			LoadRes(IndexedTexture *pIndexedTexture, CShadowRect *pShadowRect = NULL);
	void			DeleteRes();

	BOOL			Render(EffectSort &effect_sort);
};


#endif // _FX_FLAME_H_

