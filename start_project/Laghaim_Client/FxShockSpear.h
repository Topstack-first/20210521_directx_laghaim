#ifndef _FX_SHOCK_SPEAR_H_
#define _FX_SHOCK_SPEAR_H_

#include "Config.h"

#define FX_SHOCKSPEARRINGNUM 6

class CFxShockSpear
{
	CSphere*				sphere;

	CRectPlane*				light1;
	CRectPlane*				light2;
	CRectPlane*				light3;

	// nate 2004 - 8 ImageManager
//	DWORD					m_sphereTexId;
//	DWORD					m_lightTexId[4];
//	WORD					m_wTexIdx, m_wStartIdx;

	int						m_numTLight;

public:
	CFxShockSpear();
	virtual ~CFxShockSpear();

	// nate 2004 - 7 - image
//	IndexedTexture		*m_pIndexedTexture;
	// nate 2004 - 7 ±Û·Î¹ú ShadowRect»ç¿ë
//	CShadowRect			*m_pShadowRect;

	// µ¥ÀÌÅ¸ ¸â¹ö¿Í »ó¼ö
	float		m_fRadius, m_fCycleRadius;	// ºûµ¢¾î¸®ÀÇ Å©±â¿Í ¼øÈ¯±ËµµÀÇ Å©±â.
	ShowupRing  m_ShowupRing[FX_SHOWUPRINGNUM];			// 6°³ÀÇ ºûµ¢¾î¸®.

	float		m_fHeight;
	D3DLVERTEX	m_lverRect[4], m_lverRot[4], m_lverBoom[4];
	D3DLVERTEX  *m_pMesh;
	WORD		*m_pIndices;
	int			m_nStartRingSize[10];
	int			m_nTotalFrame;

	void		Create();

	void		LoadRes(IndexedTexture *pIndexedTexture, CShadowRect *pShadowRect);
	void		DeleteRes();

	void		Render(EffectSort &effect_sort);

	void		RenderRing(EffectSort &effect_sort);
	void		RenderBoom(EffectSort &effect_sort);
	void		RenderLight(EffectSort &effect_sort);
};

#endif // _FX_SHOCK_SPEAR_H_
