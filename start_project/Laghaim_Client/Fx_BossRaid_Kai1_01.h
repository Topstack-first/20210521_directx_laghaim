#ifndef _FX_BOSSRAID_KAI1_01
#define _FX_BOSSRAID_KAI1_01

#include "Config.h"

class CLolos;

class CFxBossRaidKai1_01
{
protected:

	CSphere*				sphere;

	CRectPlane*				light1;
	CRectPlane*				light2;
	CRectPlane*				light3;

	int						m_numTLight;

	// µ¥ÀÌÅ¸ ¸â¹ö¿Í »ó¼ö
	float		m_fRadius, m_fCycleRadius;	// ºûµ¢¾î¸®ÀÇ Å©±â¿Í ¼øÈ¯±ËµµÀÇ Å©±â.
	ShowupRing  m_ShowupRing[FX_SHOWUPRINGNUM];			// 6°³ÀÇ ºûµ¢¾î¸®.

	float		m_fHeight;
	D3DLVERTEX	m_lverRect[4], m_lverRot[4], m_lverBoom[4];
	D3DLVERTEX  *m_pMesh;
	WORD		*m_pIndices;
	int			m_nStartRingSize[10];
	int			m_nTotalFrame;

public:
	CFxBossRaidKai1_01();
	virtual ~CFxBossRaidKai1_01();

	void	LoadRes();
	void	DeleteRes();

	bool	Render(EffectSort &effect_sort);
	void	RenderRing(EffectSort &effect_sort);
	void	RenderBoom( EffectSort &effect_sort);
	void	RenderLight(EffectSort &effect_sort);
};

#endif // _FX_BOSSRAID_KAI1_01