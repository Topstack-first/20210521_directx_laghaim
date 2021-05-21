#ifndef _FX_OPTION_SKILL_EFFECTS_H_
#define _FX_OPTION_SKILL_EFFECTS_H_

#include "Config.h"

class CFxOptionSkillEffects
{
	CRectPlane*				skill;
	CRectPlane*				corona;

	float					m_fScale;
	float					m_fFade;
	float					fFlag;
	// nate 2004 - 7 - image
//	IndexedTexture		*m_pIndexedTexture;
	int						m_nTotalFrame;


public:
	CFxOptionSkillEffects();
	virtual ~CFxOptionSkillEffects();

	void Create();

	void LoadRes(IndexedTexture *pIndexedTexture);
	void DeleteRes();

	BOOL Render(EffectSort &effect_sort);

};

#endif // _FX_OPTION_SKILL_EFFECTS_H_
