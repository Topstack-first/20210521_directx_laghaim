#ifndef _FX_PEBBLE_SHOT_H_
#define _FX_PEBBLE_SHOT_H_

#include "Config.h"

class IndexedTexture;
class CShadowRect;

#define FX_PEBBLESHOT_HEIGHT	10.0f

class CFxPebbleShot
{
public:
	CFxPebbleShot();
	virtual ~CFxPebbleShot();

	// nate 2004 - 7 - image
//	IndexedTexture		*m_pIndexedTexture;
	// nate 2004 - 7 글로벌 ShadowRect사용
//	CShadowRect			*m_pShadowRect;

	// nate 2004 - 8 ImageManager
//	int			m_nHeadTexIdx, m_nTailTexIdx, m_nBaseTexIdx;
//	float		m_fSize, m_fSpeed, m_fLWidth, m_fLHeight;
	float		m_fSpeed;
	D3DLVERTEX	m_lverRect[16], m_lverRectSmoke[16];

	void		LoadRes(IndexedTexture *pIndexedTexture, CShadowRect *pShadowRect = NULL);
	void		DeleteRes();

	BOOL		Render(EffectSort &effect_sort);

};


class CFxPebbleShotBoom
{
public:
	CFxPebbleShotBoom();
	virtual ~CFxPebbleShotBoom();

	// nate 2004 - 7 - image
//	IndexedTexture		*m_pIndexedTexture;
	// nate 2004 - 7 글로벌 ShadowRect사용
//	CShadowRect			*m_pShadowRect;

	// nate 2004 - 8 ImageManager
//	int			m_nBoomTexIdx;
//	float		m_fBoomSize;
//	D3DLVERTEX	m_lverRectSmoke[16];

	D3DLVERTEX	m_lverBoom[4];
	Texcode		m_Texcode[12];		// 한 이미지에 있는 구분되는 텍스쳐 수
	int			m_nFrameIdx[ FX_STONESHOTBOOM_FRAMECOUNT ];

	void		LoadRes(IndexedTexture *pIndexedTexture, CShadowRect *pShadowRect = NULL);
	void		DeleteRes();

	BOOL		Render(EffectSort &effect_sort);
};

#endif // _FX_PEBBLE_SHOT_H_
