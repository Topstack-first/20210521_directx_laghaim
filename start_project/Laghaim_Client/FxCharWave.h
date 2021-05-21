#ifndef _FX_CHAR_WAVE_H_
#define _FX_CHAR_WAVE_H_

#include "Config.h"

class CFxCharWave
{
	CCylinderPlane*			wave;
	CCylinderPlane*			wave2;

	CRectPlane*				light;

	float					m_fScale;
	float					m_fFade;


	int						m_nTotalFrame;

public:
	CFxCharWave();
	virtual ~CFxCharWave();

	void Create();

	void LoadRes(IndexedTexture *pIndexedTexture);
	void DeleteRes();

	BOOL Render(EffectSort &effect_sort);

};

#endif // _FX_CHAR_WAVE_H_


