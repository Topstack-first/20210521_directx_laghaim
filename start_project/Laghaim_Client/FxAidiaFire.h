#ifndef _FX_AIDIA_FIRE_H_
#define _FX_AIDIA_FIRE_H_

#include "Config.h"

class IndexedTexture;
class CShadowRect;

#define FX_AIDIAFIRE_HEIGHT	5.0f

class CFxAidiaFire
{
public:
	CFxAidiaFire();
	virtual ~CFxAidiaFire();

	float		m_fSize, m_fSpeed, m_fLWidth, m_fLHeight;
	D3DLVERTEX	m_lverRect[16], m_lverRectSmoke[16];

	void		LoadRes(IndexedTexture *pIndexedTexture, CShadowRect *pShadowRect = NULL);
	void		DeleteRes();

	BOOL		Render(EffectSort &effect_sort);
};

class CFxAidiaBallBoom
{
public:
	CFxAidiaBallBoom();
	virtual ~CFxAidiaBallBoom();

	int			m_nFrameIdx[FX_AIDIABALLBOOM_FRAMECOUNT];
	D3DLVERTEX	m_lverBoom[4];
	float		m_fBoomSize;

	void		LoadRes();

	void		DeleteRes();

	BOOL		Render(EffectSort &effect_sort);
};

#endif // _FX_AIDIA_FIRE_H_
