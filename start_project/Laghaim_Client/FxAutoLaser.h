#ifndef _FX_AUTO_LASER_H_
#define _FX_AUTO_LASER_H_

#include "Config.h"

#define MAX_LASER_VERTEXS 4

class CFxAutoLaser
{
	LPD3DLVERTEX			m_laser;
	CRectPlane*				light;

	float					m_fScale;
	float					m_fFade;

	int						m_nTotalFrame;


public:
	CFxAutoLaser();
	virtual ~CFxAutoLaser();

	void Create();

	void LoadRes(IndexedTexture *pIndexedTexture);
	void DeleteRes();

	BOOL Render(EffectSort &effect_sort);

};

#endif // _FX_AUTO_LASER_H_

