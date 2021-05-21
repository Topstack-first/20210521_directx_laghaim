#ifndef _FX_ROCKET_H_
#define _FX_ROCKET_H_

#include "Config.h"

class CLolos;

class CFxRocket
{
	CLolos					*m_pLolo;						// object ÇÏ³ª.

	// nate 2004 - 8 ImageManager
//	CSphere*				fire_sphere;
//	DWORD					m_smokeTexId;
//	DWORD					m_fireTexId;
//	float					m_fScale;
//	float					m_fFade;
//	int						m_nTotalFrame;

	D3DLVERTEX				m_lsmoke[4];


	// nate 2004 - 7 - image
//	IndexedTexture		*m_pIndexedTexture;

public:
	CFxRocket();
	virtual ~CFxRocket();

	void		Create();

	// nate 2004 - 8 ImageManager
//	void		LoadRes(IndexedTexture *pIndexedTexture);
	void		LoadRes();

	void		DeleteRes();

	BOOL		Render(EffectSort &effect_sort);
};

#endif // _FX_ROCKET_H_


