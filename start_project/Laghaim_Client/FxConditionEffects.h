#ifndef _FX_CONDITION_EFFECTS_H_
#define _FX_CONDITION_EFFECTS_H_

#include "Config.h"

//조건 전위 이펙트
class CFxConditionEffects
{
	CRectPlane*				condition;
	CRectPlane*				corona;

	float					m_fScale;
	float					m_fFade;

	int						m_nTotalFrame;
	CLolos*					m_pSpiderWire;

public:
	CFxConditionEffects();
	virtual ~CFxConditionEffects();

	void Create();

	void LoadRes(IndexedTexture *pIndexedTexture);
	void DeleteRes();

	BOOL Render(EffectSort &effect_sort);


};

//조건 후위 이펙트
class CFxConditionEffects2
{

	float					m_fScale;
	float					m_fFade;

	int						m_nTotalFrame;
	CLolos*					m_pSpiderWire;

public:
	CFxConditionEffects2();
	virtual ~CFxConditionEffects2();

	void Create();

	void LoadRes(IndexedTexture *pIndexedTexture);
	void DeleteRes();

	BOOL Render(EffectSort &effect_sort);
};

#endif // _FX_CONDITION_EFFECTS_H_
