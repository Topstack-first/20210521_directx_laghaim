#ifndef _FX_EAT_EFFECTS_H_
#define _FX_EAT_EFFECTS_H_

#include "Config.h"

class CFxEatEffects
{
	CRectPlane*				eat_smile;

//	DWORD					m_smileTexId;
//	DWORD					m_lightTexId;

	float					m_fScale;
	float					m_fFade;

	// nate 2004 - 7 - image
//	IndexedTexture		*m_pIndexedTexture;
	int						m_nTotalFrame;


public:
	CFxEatEffects();
	virtual ~CFxEatEffects();

	void Create();

	void LoadRes(IndexedTexture *pIndexedTexture);
	void DeleteRes();

	BOOL Render(EffectSort &effect_sort);

};

#endif // _FX_EAT_EFFECTS_H_
