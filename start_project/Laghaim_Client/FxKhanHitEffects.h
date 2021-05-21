#ifndef _FX_KHAN_HIT_EFFECTS_H_
#define _FX_KHAN_HIT_EFFECTS_H_

#include "Config.h"

class CFxKhanHitEffects
{
	CCylinderPlane*			ring;
	CCylinderPlane*			wave;

	// nate 2004 - 8 ImageManager
//	DWORD					m_ringTexIdx;
//	DWORD					m_waveTexIdx;

	float					m_fScale;
	float					m_fFade;

	// nate 2004 - 7 - image
//	IndexedTexture		*m_pIndexedTexture;
	int						m_nTotalFrame;

public:
	CFxKhanHitEffects();
	virtual ~CFxKhanHitEffects();

	void		Create();

	void		LoadRes(IndexedTexture *pIndexedTexture);
	void		DeleteRes();

	BOOL		Render(EffectSort &effect_sort);
};

#endif // _FX_KHAN_HIT_EFFECTS_H_
