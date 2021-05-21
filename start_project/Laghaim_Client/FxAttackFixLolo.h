#ifndef _FX_ATTACK_FIX_LOLO_H_
#define _FX_ATTACK_FIX_LOLO_H_

#include "Config.h"

class CFxAttackFixLolo
{
	enum {SPORE, ELECTRICSHOT, SNAKEBOSS, SNAKEBOSS_MUL, FROZEN_SHOT, LEGKNOCK, LEGKNOCK_MUL, EFF_NUM};

	CLolos*			m_pLolo[EFF_NUM];
	CRectPlane		*light;
	int				m_numTLight;
	float			m_fScale;
	int				m_nOldFrame;

public:
	CFxAttackFixLolo();
	virtual ~CFxAttackFixLolo();

	void	LoadRes();
	void	DeleteRes();
	BOOL	Render(EffectSort &effect_sort);
	void	RenderLight(EffectSort &effect_sort);
};

#endif // _FX_ATTACK_FIX_LOLO_H_
