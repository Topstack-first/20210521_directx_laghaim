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
	// nate 2004 - 7 �۷ι� ShadowRect���
//	CShadowRect			*m_pShadowRect;

	// nate 2004 - 8 ImageManager
//	int			m_nBoomTexIdx, m_nBaseTexIdx;

	float		m_fBoomSize;
	D3DLVERTEX	m_lverBoom[4];
	Texcode		m_Texcode[12];		// �� �̹����� �ִ� ���еǴ� �ؽ��� ��.
	float		m_fBaseSize[13];


	void		LoadRes(IndexedTexture *pIndexedTexture, CShadowRect *pShadowRect = NULL);
	void		DeleteRes();

	BOOL		Render(EffectSort &effect_sort);

};

#endif // _FX_FIRE_BOOM_SHORT_H_


