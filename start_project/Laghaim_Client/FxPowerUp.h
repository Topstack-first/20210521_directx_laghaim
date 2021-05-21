#ifndef _FX_POWER_UP_H_
#define _FX_POWER_UP_H_

#include "Config.h"

class CFxPowerUp
{
	CCylinderPlane*			wave;
	CCylinderPlane*			wave2;

	CRectPlane*				light;

	// nate 2004 - 8 ImageManager
//	DWORD					m_waveTexId;
//	DWORD					m_wave2TexId;
//	DWORD					m_lightTexId;

	float					m_fScale;
	float					m_fFade;

	// nate 2004 - 7 - image
//	IndexedTexture		*m_pIndexedTexture;
	int						m_nTotalFrame;

public:
	CFxPowerUp();
	virtual ~CFxPowerUp();

	void Create();

	void LoadRes(IndexedTexture *pIndexedTexture);
	void DeleteRes();

	BOOL Render(EffectSort &effect_sort);

};

#endif // _FX_POWER_UP_H_



