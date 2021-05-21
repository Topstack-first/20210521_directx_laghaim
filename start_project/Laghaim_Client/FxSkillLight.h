#ifndef _FX_SKILL_LIGHT_H_
#define _FX_SKILL_LIGHT_H_

#include "Config.h"

class CFxSkillLight
{
	CRectPlane*				skill_light;

	// nate 2004 - 8 ImageManager
//	DWORD					m_lightTexId;

	float					m_fScale;
	float					m_fFade;

	// nate 2004 - 7 - image
//	IndexedTexture		*m_pIndexedTexture;
	int						m_nTotalFrame;

public:
	CFxSkillLight();
	virtual ~CFxSkillLight();

	void Create();

	void LoadRes(IndexedTexture *pIndexedTexture);
	void DeleteRes();

	BOOL Render(EffectSort &effect_sort);

};

#endif // _FX_SKILL_LIGHT_H_
