#ifndef _FX_BORA_H_
#define _FX_BORA_H_

#include "Config.h"

#define FX_BORA_HEIGHT 10

class CFxBora
{
public:
	CFxBora();
	virtual ~CFxBora();
	float		m_fSize, m_fSpeed;
	D3DLVERTEX	m_lverHead[4];
	D3DLVERTEX	m_lverTail[4];

	void		LoadRes(IndexedTexture *pIndexedTexture, CShadowRect *pShadowRect = NULL);
	void		DeleteRes();

	BOOL		Render(EffectSort &effect_sort);
};

#endif // _FX_BORA_H_
