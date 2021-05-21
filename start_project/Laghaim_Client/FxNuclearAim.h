#ifndef _FX_NUCLEAR_AIM_H_
#define _FX_NUCLEAR_AIM_H_

#include "Config.h"

class CFxNuclearAim
{
	CRectPlane*				sign;
	CRectPlane*				sign2;

	CCylinderPlane*			wave;
	CCylinderPlane*			wave2;

	// nate 2004 - 8 ImageManager
//	DWORD					m_signTexId;
//	DWORD					m_waveTexId;
//	DWORD					m_lightTexId;

	float					m_fScale;
	float					m_fFade;

	// nate 2004 - 7 - image
//	IndexedTexture		*m_pIndexedTexture;
	int						m_nTotalFrame;

public:
	CFxNuclearAim();
	virtual ~CFxNuclearAim();

	void Create();

	void LoadRes(IndexedTexture *pIndexedTexture);
	void DeleteRes();

	BOOL Render(EffectSort &effect_sort);

};

#endif // _FX_NUCLEAR_AIM_H_


