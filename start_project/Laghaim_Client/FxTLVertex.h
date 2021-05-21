#ifndef _FX_TLVERTEX_H_
#define _FX_TLVERTEX_H_

#include "Config.h"

class CFxTLVertex
{
public:

	D3DTLVERTEX			m_TLVec[4];

public:

	CFxTLVertex();
	virtual ~CFxTLVertex();
	void	LoadRes();
	void	DeleteRes();
	void	SetDevice();
	bool	Render(EffectSort &effect_sort);
};

#endif // _FX_TLVERTEX_H_
