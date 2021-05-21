#ifndef _FX_EARTH_QUAKE_H_
#define _FX_EARTH_QUAKE_H_

#include "Config.h"

class CLolos;

class CFxEarthQuake
{
	CSphere*				eng_sphere;
	CRectPlane*				smoke;

	// nate 2004 - 8 ImageManager
//	DWORD					m_engTexId;
//	DWORD					m_smokeTexId;
//	int					m_nBaseTexIdx;

public:
	CFxEarthQuake();
	virtual ~CFxEarthQuake();

	// nate 2004 - 7 - image
//	IndexedTexture		*m_pIndexedTexture;
	// nate 2004 - 7 ±Û·Î¹ú ShadowRect»ç¿ë
//	CShadowRect			*m_pShadowRect;

	void		LoadRes(IndexedTexture *pIndexedTexture, CShadowRect *pShadowRect);
	void		DeleteRes();

	BOOL		Render(EffectSort &effect_sort);

};

#endif // _FX_EARTH_QUAKE_H_
