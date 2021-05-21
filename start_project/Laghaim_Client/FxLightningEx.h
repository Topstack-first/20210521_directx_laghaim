#ifndef _FX_LIGHTNINGEX_H_
#define _FX_LIGHTNINGEX_H_
// 보스레이드 휴먼성 중앙에 들어가는 지형 이펙트

#include "Config.h"

class CFxLightningEx
{
public:
	CFxLightningEx();
	~CFxLightningEx();

	D3DLVERTEX			m_lverRect[16];
	D3DLVERTEX			m_lverFrag[4];
	int					m_nSparkSize[10];
	int					m_nTotalFrame;
	void		SetRect(float width = 20.0f, float height = 80.0f);
	void		LoadRes(IndexedTexture *pIndexedTexture, CShadowRect *pShadowRect);
	void		LoadRes();
	void		DeleteRes();
	void		Render(D3DVECTOR &vPos, int nFrame);
	void		RenderFrag(D3DVECTOR &vPos, int nFrame);
	void		RenderFrag2(D3DVECTOR &vPos, int nFrame);
};


#endif