#ifndef _FX_CHAOS_H_
#define _FX_CHAOS_H_

#include "Config.h"

#define FX_CHAIN_LINE	3
#define FX_CHAIN_DIV	3

class CFxChaos
{
	CCylinderPlane*			ring;
	CCylinderPlane*			wave;

	float					m_fScale;
	float					m_fFade;


public:
	CFxChaos();
	virtual ~CFxChaos();

	void		LoadRes(IndexedTexture *pIndexedTexture, CShadowRect *pShadowRect);
	void		DeleteRes();

	void		MakeChain(D3DVECTOR &vPosFrom, D3DVECTOR &vPosTo);
	void		Render(EffectSort &effect_sort);
};

#endif // _FX_CHAOS_H_


