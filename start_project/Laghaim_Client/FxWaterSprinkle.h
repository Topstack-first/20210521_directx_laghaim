// CFxWaterSprinkle.h: interface for the CFxBora class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CFxWaterSprinkle_H__F4579EF4_28ED_4F23_875E_6F6196CCB48D__INCLUDED_)
#define AFX_CFxWaterSprinkle_H__F4579EF4_28ED_4F23_875E_6F6196CCB48D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define FX_WTRSPRINKLE_HEIGHT 10

class CFxWaterSprinkle
{
public:
	CFxWaterSprinkle();
	virtual ~CFxWaterSprinkle();

	float		m_fSize, m_fSpeed;
	D3DLVERTEX	m_lverHead[4];
	D3DLVERTEX	m_lverTail[4];

	void		LoadRes(IndexedTexture *pIndexedTexture, CShadowRect *pShadowRect = NULL);
	void		DeleteRes();

	//BOOL		Render(D3DVECTOR &vPosFrom, D3DVECTOR &vPosTo, int nFrame);
	BOOL		Render(EffectSort &effect_sort);
};

#endif // !defined(AFX_FXBORA_H__F4579EF4_28ED_4F23_875E_6F6196CCB48D__INCLUDED_)
