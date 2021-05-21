#ifndef _FX_HIT_EFFECTS_H_
#define _FX_HIT_EFFECTS_H_

#include "Config.h"

class CFxHitEffects
{
	CRectPlane*				hit;
//	CRoundPlane*			hitround;
//	CSphere*				hitsphere;

//	DWORD					m_hitTexId;
//	DWORD					m_hitroundTexId;
//	int						m_numTHit;

	float					m_fScale;
	float					m_fFade;

	// nate 2004 - 7 - image
//	IndexedTexture		*m_pIndexedTexture;
	int						m_nTotalFrame;

public:
	CFxHitEffects();
	virtual ~CFxHitEffects();

	void Create();

	void LoadRes(IndexedTexture *pIndexedTexture);
	void DeleteRes();

	void SetTextureNum();

	BOOL Render(EffectSort &effect_sort);
};

#endif // FX_HIT_EFFECTS_H
