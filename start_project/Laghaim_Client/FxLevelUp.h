#ifndef _FX_LEVEL_UP_H_
#define _FX_LEVEL_UP_H_

#include "Config.h"

class CFxLevelUp
{
	CRectPlane*				light;
	CRectPlane*				corona;

	// nate 2004 - 8 ImageManager
//	DWORD					m_lightTexId;
//	DWORD					m_coronaTexId;
//	DWORD					m_light_roundTexId;
//	CCylinderPlane*			round;

	CCylinderPlane*			round_light;

	float					m_fScale;
	float					m_fFade;

	// nate 2004 - 7 - image
//	IndexedTexture		*m_pIndexedTexture;
	int						m_nTotalFrame;

public:
	CFxLevelUp();
	virtual ~CFxLevelUp();

	void Create();

	void LoadRes(IndexedTexture *pIndexedTexture);
	void DeleteRes();

	void SetTextureNum();

	BOOL Render(EffectSort &effect_sort);
};

#endif // _FX_LEVEL_UP_H_


