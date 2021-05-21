#ifndef _FX_SHILON_H_
#define _FX_SHILON_H_

#include "Config.h"

class IndexedTexture;
class CShadowRect;

class CFxShilon
{
public:
	CFxShilon();
	virtual ~CFxShilon();

	// nate 2004 - 7 - image
//	IndexedTexture		*m_pIndexedTexture;
	// nate 2004 - 7 �۷ι� ShadowRect���
//	CShadowRect			*m_pShadowRect;

	// nate 2004 - 8 ImageManager
//	int			m_nBoomTexIdx, m_nHeadTexIdx, m_nTailTexIdx, m_nBaseTexIdx;
//	float		m_fBoomSize, m_fFragSize;
//	float		m_fSize, m_fSpeed, m_fLWidth, m_fLHeight;
//	D3DLVERTEX  m_lverFrag[4];
//	D3DLVERTEX	m_lverRect[16], m_lverRectSmoke[16];

	D3DLVERTEX	m_lverBoom[4];
	Texcode		m_Texcode[12];		// �� �̹����� �ִ� ���еǴ� �ؽ��� ��
	int			m_nFrameIdx[FX_STARFLAREBOOM_FRAMECOUNT];
	float		m_fBaseSize[FX_SHILON_FRAMECOUNT];

	void		LoadRes(IndexedTexture *pIndexedTexture, CShadowRect *pShadowRect = NULL);
	void		DeleteRes();

	BOOL		Render(EffectSort &effect_sort);
};

#endif // _FX_SHILON_H_
