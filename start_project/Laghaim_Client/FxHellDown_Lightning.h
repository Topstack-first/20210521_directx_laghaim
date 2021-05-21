#ifndef _FX_HELL_DOWN_LIGHTNING_H_
#define _FX_HELL_DOWN_LIGHTNING_H_

#include "Config.h"

class CFxHellDown_Lightning
{
public:
	CFxHellDown_Lightning();
	~CFxHellDown_Lightning();

	D3DLVERTEX			m_lverRect[16];
	D3DLVERTEX			m_lverFrag[4];

	int					m_nSparkSize[10];
	int					m_nTotalFrame;

	void		LoadRes(IndexedTexture *pIndexedTexture, CShadowRect *pShadowRect);
	void		DeleteRes();

	void		Render(EffectSort &effect_sort, int nFrame);
	void		Render2(EffectSort &effect_sort, int nFrame);
	void		Render3(EffectSort &effect_sort, int nFrame);
	void		Render4(EffectSort &effect_sort, int nFrame);
	void		Render5(EffectSort &effect_sort, int nFrame);
	void		RenderFrag(EffectSort &effect_sort, int nFrame);
	void		RenderFrag2(EffectSort &effect_sort, int nFrame);
};

#endif // _FX_HELL_DOWN_LIGHTNING_H_
