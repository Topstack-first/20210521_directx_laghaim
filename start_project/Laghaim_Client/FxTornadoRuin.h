#ifndef _FX_TORNADO_RUIN_H_
#define _FX_TORNADO_RUIN_H_

#include "Config.h"

class IndexedTexture;
class CShadowRect;
class CLolos;
class CRectPlane;
class CFxTornadoRuin
{
public:
	CFxTornadoRuin();
	virtual ~CFxTornadoRuin();

	CLolos *m_pLolo;

	// nate 2004 - 7 - image
//	IndexedTexture		*m_pIndexedTexture;
	// nate 2004 - 7 ±Û·Î¹ú ShadowRect»ç¿ë
//	CShadowRect			*m_pShadowRect;
	CRectPlane	*smoke;

	// nate 2004 - 8 ImageManager
//	int			m_nHeadTexIdx, m_nTailTexIdx, m_nBaseTexIdx;
//	float		m_fSize, m_fSpeed, m_fLWidth, m_fLHeight;
//	D3DLVERTEX	m_lverRect[16], m_lverRectSmoke[16];
//	DWORD					m_smokeTexId;
	float		m_fSpeed;

	// nate 2004 - 8 ImageManager
//	void		LoadRes(IndexedTexture *pIndexedTexture, CShadowRect *pShadowRect = NULL);
	void		LoadRes();

	void		DeleteRes();

	BOOL		Render(EffectSort &effect_sort);
};

#endif // _FX_TORNADO_RUIN_H_
