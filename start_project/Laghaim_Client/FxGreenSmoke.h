#ifndef _FX_GREEN_SMOKE_H_
#define _FX_GREEN_SMOKE_H_

#include "Config.h"

class IndexedTexture;
class CShadowRect;

#define FX_GREENSMOKE_HEIGHT	10.0f

class CFxGreenSmoke
{
public:
	CFxGreenSmoke();
	virtual ~CFxGreenSmoke();

	// nate 2004 - 7 - image
//	IndexedTexture		*m_pIndexedTexture;

	// nate 2004 - 8 ImageManager
//	int			m_nTailTexIdx;
//	float		m_fSize, m_fSpeed, m_fLWidth, m_fLHeight;
//	float		m_fSpeed, m_fLWidth, m_fLHeight;
	float		m_fSpeed;

	D3DLVERTEX	m_lverRectSmoke[16];

	void		LoadRes(IndexedTexture *pIndexedTexture);
	void		DeleteRes();

	BOOL		Render(EffectSort &effect_sort);

};

#endif // _FX_GREEN_SMOKE_H_

