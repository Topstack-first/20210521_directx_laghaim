#ifndef _FX_FIRE_BOOM_SHORT_H_
#define _FX_FIRE_BOOM_SHORT_H_

#include "Config.h"

class CFxFireBoomShort
{
public:
	CFxFireBoomShort();
	virtual ~CFxFireBoomShort();

	// nate 2004 - 7 - image
//	IndexedTexture		*m_pIndexedTexture;
	// nate 2004 - 7 글로벌 ShadowRect사용
//	CShadowRect			*m_pShadowRect;

	// nate 2004 - 8 ImageManager
//	int			m_nBoomTexIdx, m_nBaseTexIdx;

	float		m_fBoomSize;
	D3DLVERTEX	m_lverBoom[4];
	Texcode		m_Texcode[12];		// 한 이미지에 있는 구분되는 텍스쳐 수.
	float		m_fBaseSize[13];


	void		LoadRes(IndexedTexture *pIndexedTexture, CShadowRect *pShadowRect = NULL);
	void		DeleteRes();

	BOOL		Render(EffectSort &effect_sort);

};

#endif // _FX_FIRE_BOOM_SHORT_H_


