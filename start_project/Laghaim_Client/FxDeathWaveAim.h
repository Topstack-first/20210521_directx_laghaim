#ifndef _FX_DEATH_WAVE_AIM_H_
#define _FX_DEATH_WAVE_AIM_H_

#include "Config.h"

class CFxDeathWaveAim
{
	CRectPlane*				sign;
	CRectPlane*				sign2;

	CCylinderPlane*			wave;
	CCylinderPlane*			wave2;

	float					m_fScale;
	float					m_fFade;

	int						m_nTotalFrame;

public:
	CFxDeathWaveAim();
	virtual ~CFxDeathWaveAim();


	void Create();

	void LoadRes(IndexedTexture *pIndexedTexture);
	void DeleteRes();

	BOOL Render(EffectSort &effect_sort);

};

#endif // _FX_DEATH_WAVE_AIM_H_
