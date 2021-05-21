#ifndef _FX_DURANT_TARGET_H_
#define _FX_DURANT_TARGET_H_

#include "Config.h"

class CFxDurantTarget
{
	CSphere*				sphere;
	CRectPlane*				light1;
	CRectPlane*				light2;
	CRectPlane*				light3;
	int						m_numTLight;

public:
	CFxDurantTarget();
	virtual ~CFxDurantTarget();

	// ����Ÿ ����� ���
	float		m_fRadius, m_fCycleRadius;	// ������� ũ��� ��ȯ�˵��� ũ��.
	ShowupRing  m_ShowupRing[FX_SHOWUPRINGNUM];			// 6���� �����.

	float		m_fHeight;
	D3DLVERTEX	m_lverRect[4], m_lverRot[4], m_lverBoom[4];
	D3DLVERTEX  *m_pMesh;
	WORD		*m_pIndices;
	int			m_nStartRingSize[10];
	int			m_nTotalFrame;

	void		Create();
	void		LoadRes();
	void		DeleteRes();
	void		Render(EffectSort &effect_sort);
	void		RenderLight(EffectSort &effect_sort);
};

#endif // _FX_DURANT_TARGET_H_
