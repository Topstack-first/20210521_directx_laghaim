#pragma once

class FxArupakaSpit
{
	LPD3DLVERTEX			m_souldust;
	float					m_fScale;
	float					m_fFade;
	int						m_nTotalFrame;

public:
	FxArupakaSpit(void);
	virtual ~FxArupakaSpit(void);

	void Create();

	void LoadRes(IndexedTexture *pIndexedTexture);
	void DeleteRes();

	BOOL Render(EffectSort &effect_sort);
};
