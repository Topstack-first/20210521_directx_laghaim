#ifndef _FX_WORLDCUP_H_
#define _FX_WORLDCUP_H_

#include "Config.h"

class CLolos;

class CFxWorldCup
{
	CLolos					*m_pLolo;						// object ÇÏ³ª.

	CSphere*				fire_sphere;

	// nate 2004 - 8 ImageManager
//	int						m_nTotalFrame;
//	CRectPlane*				light;
	D3DLVERTEX				m_lsmoke[4];

//	DWORD					m_lightTexId;
//	DWORD					m_smokeTexId;
//	DWORD					m_fireTexId[4];
//	float					m_fScale;
//	float					m_fFade;

	int						m_numTFire;

	// nate 2004 - 7 - image
//	IndexedTexture		*m_pIndexedTexture;

public:
	CFxWorldCup();
	virtual ~CFxWorldCup();

	void		Create();

	// nate 2004 - 8 ImageManager
//	void		LoadRes(IndexedTexture *pIndexedTexture);
	void		LoadRes();

	void		DeleteRes();

	BOOL		Render(EffectSort &effect_sort);
};

#endif // _FX_WORLDCUP_H_
