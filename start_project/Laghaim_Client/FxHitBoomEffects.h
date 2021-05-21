#ifndef _FX_HIT_BOOM_EFFECTS_H_
#define _FX_HIT_BOOM_EFFECTS_H_

#include "Config.h"

class CFxHitBoomEffects
{
	CRectPlane*				hit;
	CRectPlane*				hit_boom;

	// nate 2004 - 8 ImageManager
//	DWORD					m_hitTexId;
//	DWORD					m_hitboomTexId;
//	int						m_numTHit;

	float					m_fScale;
	float					m_fFade;

	// nate 2004 - 7 - image
//	IndexedTexture		*m_pIndexedTexture;
	int						m_nTotalFrame;

public:
	CFxHitBoomEffects();
	virtual ~CFxHitBoomEffects();

	void Create();

	void LoadRes(IndexedTexture *pIndexedTexture);
	void DeleteRes();

	void SetTextureNum();

	BOOL Render(EffectSort &effect_sort);

};

#endif // _FX_HIT_BOOM_EFFECTS_H_
