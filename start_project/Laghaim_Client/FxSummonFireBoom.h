#ifndef _FX_SUMMON_FIRE_BOOM_H_
#define _FX_SUMMON_FIRE_BOOM_H_

#include "Config.h"

class CFxSummonFireBoom
{
public:
	CFxSummonFireBoom();
	virtual ~CFxSummonFireBoom();

	float		m_fSpeed, m_fBoomSize;
	D3DLVERTEX	m_lverBoom[4];
	Texcode		m_Texcode[12];		// 한 이미지에 있는 구분되는 텍스쳐 수.
	int			m_nFrameIdx[FX_FIREBALLBOOM2_FRAMECOUNT];
	float		m_fBaseSize[FX_FIREBALLBOOM2_FRAMECOUNT];
	void		LoadRes(IndexedTexture *pIndexedTexture, CShadowRect *pShadowRect = NULL);
	void		DeleteRes();
	BOOL		Render(D3DVECTOR &vPosFrom, D3DVECTOR &vPosTo, int nFrame);
};

#endif // _FX_SUMMON_FIRE_BOOM_H_
