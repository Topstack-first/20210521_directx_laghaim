#ifndef _FX_CHAIN_H_
#define _FX_CHAIN_H_

#include "Config.h"

#define FX_CHAIN_LINE	3
#define FX_CHAIN_DIV	3

class CFxChain
{
	CRectPlane*				light;

	float					m_fScale;
	float					m_fFade;

public:
	CFxChain();
	~CFxChain();

	D3DLVERTEX			m_lverRect[FX_CHAIN_LINE*(FX_CHAIN_DIV*2+2)];
	unsigned short		m_indexRect[FX_CHAIN_LINE*FX_CHAIN_DIV*6];

	void		LoadRes(IndexedTexture *pIndexedTexture);
	void		DeleteRes();

	void		MakeChain(D3DVECTOR &vPosFrom, D3DVECTOR &vPosTo);
	void		Render(EffectSort &effect_sort);
};

#endif // _FX_CHAIN_H_