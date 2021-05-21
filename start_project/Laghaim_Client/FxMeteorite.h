#ifndef _FX_METEORITE_H_
#define _FX_METEORITE_H_

#include "Config.h"

class IndexedTexture;
class CShadowRect;

class CFxMeteorite
{
public:
	CFxMeteorite();
	virtual ~CFxMeteorite();

	// nate 2004 - 7 - image
//	IndexedTexture		*m_pIndexedTexture;
	// nate 2004 - 7 ±Û·Î¹ú ShadowRect»ç¿ë
//	CShadowRect			*m_pShadowRect;

	// nate 2004 - 8 ImageManager
//	int			m_nHeadTexIdx, m_nTailTexIdx, m_nBaseTexIdx;
//	float		m_fSpeed, m_fLWidth, m_fLHeight;
	float		m_fSpeed;
	D3DLVERTEX	m_lverRect[16], m_lverRectSmoke[16];

	void		LoadRes(IndexedTexture *pIndexedTexture, CShadowRect *pShadowRect = NULL);
	void		DeleteRes();

	BOOL		Render(EffectSort &effect_sort);
};

#endif // FX_METEORITE_H


