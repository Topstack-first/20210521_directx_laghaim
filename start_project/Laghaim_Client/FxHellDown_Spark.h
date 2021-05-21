#ifndef _FX_HELL_DOWN_SPARK_H_
#define _FX_HELL_DOWN_SPARK_H_

#include "Config.h"

#define FX_SHOCKSPEARRINGNUM 6

class CFxHellDown_Spark
{
	CRectPlane*				light1;
	CRectPlane*				light2;
	CRectPlane*				light3;

public:
	CFxHellDown_Spark();
	virtual ~CFxHellDown_Spark();

	ShowupRing  m_ShowupRing[FX_SHOWUPRINGNUM];
	D3DLVERTEX	m_lverRect[4], m_lverRot[4], m_lverBoom[4];
	D3DLVERTEX  *m_pMesh;

	WORD		*m_pIndices;
	float		m_fHeight, m_fRadius, m_fCycleRadius;
	int			m_nStartRingSize[10];
	int			m_nTotalFrame;
	int			m_numTLight;

	void		Create();
	void		LoadRes(IndexedTexture *pIndexedTexture, CShadowRect *pShadowRect);
	void		DeleteRes();

	void		Render(EffectSort &effect_sort);
	void		RenderRing(EffectSort &effect_sort);
	void		RenderBoom(EffectSort &effect_sort);
	void		RenderLight(EffectSort &effect_sort);
};

#endif // _FX_HELL_DOWN_SPARK_H_
