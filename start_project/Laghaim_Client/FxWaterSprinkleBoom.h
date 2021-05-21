// FxWaterSprinkleBoom.h: interface for the CFxBoraBoom class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WTRSPRINKLEBOOM_H__635923B0_B882_4F72_88D8_766C5E956DF1__INCLUDED_)
#define AFX_WTRSPRINKLEBOOM_H__635923B0_B882_4F72_88D8_766C5E956DF1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CFxWaterSprinkleBoom
{
public:
	CFxWaterSprinkleBoom();
	virtual ~CFxWaterSprinkleBoom();

	float		m_fTailSize, m_fSpeed, m_fBoomSize;
	D3DLVERTEX	m_lverBoom[4];
	D3DLVERTEX	m_lverTail[4];
	Texcode		m_Texcode[12];		// 한 이미지에 있는 구분되는 텍스쳐 수.
	int			m_nFrameIdx[FX_WTRSPRINKLEBOOM_FRAMECOUNT];


	void		LoadRes(IndexedTexture *pIndexedTexture, CShadowRect *pShadowRect = NULL);
	void		DeleteRes();

	BOOL		Render(D3DVECTOR &vPosFrom, D3DVECTOR &vPosTo, int nFrame);
};

#endif // !defined(AFX_FXBORABOOM_H__635923B0_B882_4F72_88D8_766C5E956DF1__INCLUDED_)
