#ifndef _FX_RANDOM_SHOOT_H_
#define _FX_RANDOM_SHOOT_H_

#include "Config.h"

class IndexedTexture;

class CFxRandomShoot
{
public:
	CFxRandomShoot();
	virtual ~CFxRandomShoot();

	D3DLVERTEX	m_lverRect[4];
	float		m_fLightSize[3];

	void		LoadRes(IndexedTexture *pIndexedTexture);
	void		DeleteRes();
	void		Render(EffectSort &effect_sort);
};

#endif // _FX_RANDOM_SHOOT_H_
