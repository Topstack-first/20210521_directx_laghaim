#ifndef _FX_METEORITE_BOOM_H_
#define _FX_METEORITE_BOOM_H_

#include "Config.h"

class IndexedTexture;
class CShadowRect;

class CFxMeteoriteBoom
{
public:
	CFxMeteoriteBoom();
	virtual ~CFxMeteoriteBoom();


	// nate 2004 - 7 - image
//	IndexedTexture		*m_pIndexedTexture;
	// nate 2004 - 7 글로벌 ShadowRect사용
//	CShadowRect			*m_pShadowRect;

	// nate 2004 - 8 ImageManager
//	int			m_nBoomTexIdx, m_nBaseTexIdx;
//	float		m_fBoomSize, m_fFragSize;
//	float		m_fSize, m_fSpeed, m_fLWidth, m_fLHeight;
//	D3DLVERTEX  m_lverFrag[4];

	D3DLVERTEX	m_lverBoom[4];
	Texcode		m_Texcode[12];		// 한 이미지에 있는 구분되는 텍스쳐 수
	int			m_nFrameIdx[FX_METEORITEBOOM_FRAMECOUNT];

	void		LoadRes(IndexedTexture *pIndexedTexture, CShadowRect *pShadowRect = NULL);
	void		DeleteRes();

	BOOL		Render(EffectSort &effect_sort);
};

#endif // _FX_METEORITE_BOOM_H_

