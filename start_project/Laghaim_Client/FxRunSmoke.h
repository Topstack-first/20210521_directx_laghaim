#ifndef _FX_RUN_SMOKE_H_
#define _FX_RUN_SMOKE_H_

#include "Config.h"

class CFxRunSmoke
{
	CRectPlane*				smoke;

	// nate 2004 - 8 ImageManager
//	DWORD					m_smokeTexId;

	// nate 2004 - 7 - image
//	IndexedTexture		*m_pIndexedTexture;

public:
	CFxRunSmoke();
	virtual ~CFxRunSmoke();

	void Create();

	void LoadRes(IndexedTexture *pIndexedTexture);
	void DeleteRes();

	BOOL Render(EffectSort &effect_sort);

};

#endif // _FX_RUN_SMOKE_H_


