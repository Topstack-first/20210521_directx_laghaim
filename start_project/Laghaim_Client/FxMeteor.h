#ifndef _FX_METEOR_H_
#define _FX_METEOR_H_

#include "Config.h"

class IndexedTexture;
class CShadowRect;

class CFxMeteor
{
	/*
	public:
		CFxMeteor();
		virtual ~CFxMeteor();

		IndexedTexture		*m_pIndexedTexture;
		CShadowRect			*m_pShadowRect;

		int			m_nHeadTexIdx, m_nTailTexIdx, m_nBaseTexIdx;
		float		m_fSize, m_fSpeed;
		D3DLVERTEX	m_lverRect[4];

		void		LoadRes(IndexedTexture *pIndexedTexture, CShadowRect *pShadowRect = NULL);
		void		DeleteRes();

		BOOL		Render(EffectSort &effect_sort);
	*/
public:
	CFxMeteor();
	virtual ~CFxMeteor();

	// nate 2004 - 7 - image
//	IndexedTexture		*m_pIndexedTexture;
	// nate 2004 - 7 글로벌 ShadowRect사용
//	CShadowRect			*m_pShadowRect;

	// 데이타 멤버와 상수
	// nate 2004 - 8 ImageManager
//	WORD				m_wLightningIdx, m_wSparkIdx;
//	float				m_fLWidth, m_fLHeight;	// 위에서 떨어지는 사각형 크기
//	float				m_fFWidth, m_fFHeight;	// 옆으로 떨어지는 파편.
//	int					m_nTotalFrame;

	D3DLVERTEX			m_lverRect[16];
	D3DLVERTEX			m_lverFrag[4];

	int					m_nSparkSize[10];

	void		LoadRes(IndexedTexture *pIndexedTexture, CShadowRect *pShadowRect);
	void		DeleteRes();

	void		Render(D3DVECTOR &vPos, int nFrame);
	void		RenderFrag(D3DVECTOR &vPos, int nFrame);
	void		RenderFrag2(D3DVECTOR &vPos, int nFrame);
};

#endif // _FX_METEOR_H_
