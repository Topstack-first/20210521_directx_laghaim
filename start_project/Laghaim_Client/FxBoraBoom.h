#ifndef _FX_BORA_BOOM_H_
#define _FX_BORA_BOOM_H_

#include "Config.h"

class CFxBoraBoom
{
public:
	CFxBoraBoom();
	virtual ~CFxBoraBoom();

	float		m_fTailSize, m_fSpeed, m_fBoomSize;
	D3DLVERTEX	m_lverBoom[4];
	D3DLVERTEX	m_lverTail[4];
	Texcode		m_Texcode[12];		// 한 이미지에 있는 구분되는 텍스쳐 수.
	int			m_nFrameIdx[FX_BORABOOM_FRAMECOUNT];


	void		LoadRes(IndexedTexture *pIndexedTexture, CShadowRect *pShadowRect = NULL);
	void		DeleteRes();

	BOOL		Render(D3DVECTOR &vPosFrom, D3DVECTOR &vPosTo, int nFrame);
};

#endif // _FX_BORA_BOOM_H_
