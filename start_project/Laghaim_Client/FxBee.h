#ifndef _FX_BEE_H_
#define _FX_BEE_H_

#include "Config.h"

#define FX_BEE_HEIGHT 10

class CFxBee
{
public:
	CFxBee();
	virtual ~CFxBee();

	float		m_fSize, m_fSpeed;
	D3DLVERTEX	m_lverHead[4];
	D3DLVERTEX	m_lverTail[4];

	void		LoadRes(IndexedTexture *pIndexedTexture, CShadowRect *pShadowRect = NULL);
	void		DeleteRes();
	BOOL		Render(EffectSort &effect_sort);
};

#endif // _FX_BEE_H_
