#ifndef _FX_HIT_PET_EFFECTS_H_
#define _FX_HIT_PET_EFFECTS_H_

#include "Config.h"

class CFxHitPetEffects
{
	CRectPlane*				hit;
	CRectPlane*				hit_light;

//	CRoundPlane*			hitround;
//	CSphere*				hitsphere;

	// nate 2004 - 8 ImageManager
//	DWORD					m_hitTexId;
//	DWORD					m_hitlightTexId;

//	DWORD					m_hitroundTexId;

	float					m_fScale;
	float					m_fFade;

	// nate 2004 - 7 - image
//	IndexedTexture		*m_pIndexedTexture;
	int						m_nTotalFrame;

public:
	CFxHitPetEffects();
	virtual ~CFxHitPetEffects();

	void Create();

	void LoadRes(IndexedTexture *pIndexedTexture);
	void DeleteRes();

	void SetTextureNum();

	BOOL Render(EffectSort &effect_sort);

};

#endif // _FX_HIT_PET_EFFECTS_H_


