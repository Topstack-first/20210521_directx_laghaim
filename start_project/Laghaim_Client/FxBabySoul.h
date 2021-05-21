#ifndef _FX_BABY_SOUL_H_
#define _FX_BABY_SOUL_H_

#include "Config.h"

//----------------------------------------------------------------------------
//-- FireWork
//----------------------------------------------------------------------------

#define BABYSOUL_NUM 128
#define BABYSOUL_NUM_SM 27
#define BABYSOUL_NUM_FON 80 //-- 4 Color * 10°³ÀÇ ²¿¸®

class CFxBabySoul
{
	D3DLVERTEX		m_flame[4];
	D3DLVERTEX		m_position;
	D3DVECTOR		m_flamemove[BABYSOUL_NUM];
	D3DVECTOR		m_flamemove_Fountain[BABYSOUL_NUM_FON];
	D3DLVERTEX		m_position_fo;
	int				m_startframe[BABYSOUL_NUM_FON];

	//-- Fireswork Small
	D3DVECTOR		m_flamemove_small[BABYSOUL_NUM];
//	D3DLVERTEX		m_position_sm;

public:

	CFxBabySoul();
	virtual ~CFxBabySoul();

	void DeleteRes();

	BOOL Render(EffectSort &effect_sort);
	BOOL Render_Screw(EffectSort &effect_sort);

};

#endif // _FX_FIRE_WORKS_H_
