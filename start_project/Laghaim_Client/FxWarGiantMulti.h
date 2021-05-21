#ifndef _FX_WARGIANT_MULTI_H_
#define _FX_WARGIANT_MULTI_H_

#include "Config.h"

class CLolos;

class CFxWarGiantMulti
{
	CLolos					*m_pLolo;						// object ÇÏ³ª.

	D3DLVERTEX				m_lsmoke[4];

	// nate 2004 - 8 ImageManager
// 	DWORD					m_smokeTexId;
//	float					m_fScale;
//	float					m_fFade;
//	int						m_nTotalFrame;

	// nate 2004 - 7 - image
//	IndexedTexture		*m_pIndexedTexture;

public:
	CFxWarGiantMulti();
	virtual ~CFxWarGiantMulti();

	void		Create();

	// nate 2004 - 8 ImageManager
//	void		LoadRes(IndexedTexture *pIndexedTexture);
	void		LoadRes();

	void		DeleteRes();

	BOOL		Render(EffectSort &effect_sort);
};

#endif // _FX_WARGIANT_MULTI_H_


