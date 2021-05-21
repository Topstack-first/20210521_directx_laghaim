#ifndef _FX_STAR_FLARE_H_
#define _FX_STAR_FLARE_H_

#include "Config.h"

class IndexedTexture;
class CShadowRect;

class CFxStarFlare
{
public:
	CFxStarFlare();
	virtual ~CFxStarFlare();

	// nate 2004 - 7 - image
//	IndexedTexture		*m_pIndexedTexture;
	CShadowRect			*m_pShadowRect;

	int			m_nHeadTexIdx, m_nTailTexIdx, m_nBaseTexIdx;
	float		m_fSize, m_fSpeed, m_fLWidth, m_fLHeight;
	D3DLVERTEX	m_lverRect[16], m_lverRectSmoke[16];

	void		LoadRes(IndexedTexture *pIndexedTexture, CShadowRect *pShadowRect = NULL);
	void		DeleteRes();

	BOOL		Render(EffectSort &effect_sort);
};

class CFxStarFlareBoom
{
public:
	CFxStarFlareBoom();
	virtual ~CFxStarFlareBoom();

	// nate 2004 - 7 - image
//	IndexedTexture		*m_pIndexedTexture;
	CShadowRect			*m_pShadowRect;

	int			m_nBoomTexIdx, m_nHeadTexIdx, m_nTailTexIdx, m_nBaseTexIdx;
	float		m_fBoomSize, m_fFragSize;
	float		m_fSize, m_fSpeed, m_fLWidth, m_fLHeight;
	D3DLVERTEX	m_lverBoom[4];
	D3DLVERTEX  m_lverFrag[4];
	Texcode		m_Texcode[12];		// 한 이미지에 있는 구분되는 텍스쳐 수
	int			m_nFrameIdx[FX_STARFLAREBOOM_FRAMECOUNT];
	D3DLVERTEX	m_lverRect[16], m_lverRectSmoke[16];

	void		LoadRes(IndexedTexture *pIndexedTexture, CShadowRect *pShadowRect = NULL);
	void		DeleteRes();

	BOOL		Render(EffectSort &effect_sort);
};
/*

#define FLARE_CUR_SPARKLES  20
#define FLARE_MAX_SPARKLES	150

class CFxStarFlare
{
public:
	CFxStarFlare();
	virtual ~CFxStarFlare();

	IndexedTexture		*m_pIndexedTexture;

	// 데이타 멤버와 상수
	WORD		m_wTexIdx;
	WORD		m_wArrowIdx;

	D3DLVERTEX	m_lverRect[4], m_lverRot[4];
	D3DLVERTEX	m_lverArrow[4];
	float		m_fLightSize;
	int			m_nTotalFrame;

	void		InitSparkles();
	Sparkle		RandomSparkle();
	int			m_nMaxSparkles;
	Sparkle		*m_pSparkles;
	D3DLVERTEX  *m_pMesh;
	WORD		*m_pIndices;
	D3DVECTOR	m_vecBound;


	void		LoadRes(IndexedTexture *pIndexedTexture);
	void		DeleteRes();

	void		FrameMove(float fTimeKey);
	BOOL		Render(EffectSort &effect_sort);
	BOOL		Fly(EffectSort &effect_sort);
	BOOL		Hitted(EffectSort &effect_sort);
};
*/

#endif // _FX_STAR_FLARE_H_
