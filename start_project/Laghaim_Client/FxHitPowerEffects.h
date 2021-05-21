#ifndef _FX_HIT_POWER_EFFECTS_H_
#define _FX_HIT_POWER_EFFECTS_H_

#include "Config.h"

class CFxHitPowerEffects
{
	CRectPlane*				hit;
	CRectPlane*				hit_boom;

	CCylinderPlane*			hit_ring;
	CCylinderPlane*			hit_cross;

	CSphere*				hit_sphere;

	// nate 2004 - 8 ImageManager
//	DWORD					m_hitTexId;
//	DWORD					m_hitboomTexId;
//	DWORD					m_hitcrossTexId;

	float					m_fScale;
	float					m_fFade;

	// nate 2004 - 7 - image
//	IndexedTexture		*m_pIndexedTexture;
	int						m_nTotalFrame;


public:
	CFxHitPowerEffects();
	virtual ~CFxHitPowerEffects();

	void Create();

	void LoadRes(IndexedTexture *pIndexedTexture);
	void DeleteRes();

	void SetTextureNum();

	BOOL Render(EffectSort &effect_sort);

};

#endif // _FX_HIT_POWER_EFFECTS_H_
