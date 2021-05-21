#ifndef _FX_ATTACK_MOVE_LOLO_H_
#define _FX_ATTACK_MOVE_LOLO_H_

#include "Config.h"

class CFxAttack_MoveLolo
{
	CLolos	*m_pLolo[2];

public:
	CFxAttack_MoveLolo();
	virtual ~CFxAttack_MoveLolo();

	void	LoadRes();
	void	DeleteRes();
	BOOL	Render(EffectSort &effect_sort);
};

#endif // _FX_ATTACK_MOVE_LOLO_H_
