#ifndef _FX_SHIELD_H_
#define _FX_SHIELD_H_

#include "Config.h"

class CFxShield
{
	CRectPlane*				shield_round1;
	CRectPlane*				shield_round2;

	// nate 2004 - 8 ImageManager
//	CRectPlane*				shield_round3;
//	CRectPlane*				shield_round4;
//	DWORD					m_shield1TexId;
//	DWORD					m_shield2TexId;

	float					m_fScale;
	float					m_fFade;

	// nate 2004 - 7 - image
//	IndexedTexture		*m_pIndexedTexture;
	int						m_nTotalFrame;


public:
	CFxShield();
	virtual ~CFxShield();

	void Create();

	void LoadRes(IndexedTexture *pIndexedTexture);
	void DeleteRes();

	BOOL Render(EffectSort &effect_sort);
};

#endif // _FX_SHIELD_H_
