#ifndef _FX_SHOOT_ATTACKED_H_
#define _FX_SHOOT_ATTACKED_H_

#include "Config.h"

class IndexedTexture;

class CFxShootAttacked
{
public:
	CFxShootAttacked();
	virtual ~CFxShootAttacked();

	// nate 2004 - 7 - image
//	IndexedTexture		*m_pIndexedTexture;

	// nate 2004 - 8 ImageManager
//	int			m_nTexIdx;
//	float		m_fSize;

	D3DLVERTEX	m_lverRect[16];

	void		LoadRes(IndexedTexture *pIndexedTexture);
	void		DeleteRes();

	void		Render(EffectSort &effect_sort);
};

#endif // _FX_SHOOT_ATTACKED_H_
