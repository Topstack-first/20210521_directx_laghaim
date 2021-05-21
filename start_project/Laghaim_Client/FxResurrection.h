#ifndef _FX_RESURRECTION_H_
#define _FX_RESURRECTION_H_

#include "Config.h"

class CFxResurrection
{
	CRectPlane*				ground_ring;
	CRectPlane*				ground_corona;

	CCylinderPlane*			shining_ring;
	CCylinderPlane*			shining_lignt;

	CRectPlane*				m_rect_magic;

	float					m_fScale;
	float					m_fFade;
	int						m_nTotalFrame;


public:
	CFxResurrection();
	virtual ~CFxResurrection();

	void Create();

	void LoadRes();
	void DeleteRes();
	BOOL Render(EffectSort &effect_sort);

};

#endif // _FX_RESURRECTION_H_
