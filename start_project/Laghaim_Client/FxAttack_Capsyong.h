#ifndef _FX_ATTACK_CAPSYONG_H_
#define _FX_ATTACK_CAPSYONG_H_

#include "Config.h"

class CFxAttack_Capsyong
{
	CCapsyong				*m_pCapsyong;
	CBiped					*m_pBpd;

public:
	CFxAttack_Capsyong();
	virtual ~CFxAttack_Capsyong();

	void	LoadRes();
	void	DeleteRes();
	void	Render(EffectSort &effect_sort);
};

#endif //FX_ATTACK_CAPSYONG_H
