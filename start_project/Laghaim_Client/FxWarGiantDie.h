#ifndef _FX_WARGIANT_DIE_H_
#define _FX_WARGIANT_DIE_H_

#include "Config.h"

class CFxWarGiantDie
{
	// nate 2004 - 8 ImageManager
//	CRectPlane*				light;

	CRectPlane*				corona;

	CCylinderPlane*			round;
	CCylinderPlane*			round_light;

	// nate 2004 - 8 ImageManager
//	DWORD					m_ground1TexId;
//	DWORD					m_ground2TexId;
//	DWORD					m_lightTexId;
//	DWORD					m_coronaTexId;
//	DWORD					m_light_roundTexId;

	float					m_fScale;
	float					m_fFade;

	// nate 2004 - 7 - image
//	IndexedTexture		*m_pIndexedTexture;
	int						m_nTotalFrame;

public:
	CFxWarGiantDie();
	virtual ~CFxWarGiantDie();

	void Create();

	void LoadRes(IndexedTexture *pIndexedTexture);
	void DeleteRes();

	void SetTextureNum();

	BOOL Render(EffectSort &effect_sort);

};

#endif // _FX_WARGIANT_DIE_H_


