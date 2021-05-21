#ifndef _FX_FIRE_WORKS_H_
#define _FX_FIRE_WORKS_H_

#include "Config.h"

//----------------------------------------------------------------------------
//-- FireWork
//----------------------------------------------------------------------------
//-- 꽃불의 처리.
#define FIREWORKS_NUM 128
#define FIREWORKS_NUM_SM 27
#define FIREWORKS_NUM_FON 80 //-- 4 Color * 10개의 꼬리
class CFxFireWorks
{
	//-- Fireswork Big
	D3DLVERTEX		m_flame[4];
	D3DLVERTEX		m_position;
	D3DVECTOR			m_flamemove[FIREWORKS_NUM];

	//-- Fireswork Small
	D3DVECTOR			m_flamemove_small[FIREWORKS_NUM];
	D3DLVERTEX		m_position_sm;

	//-- Fireswork Screw
	//-- 실시간 계산

	//-- Fireswork Screw
	D3DVECTOR			m_flamemove_Fountain[FIREWORKS_NUM_FON];
	D3DLVERTEX		m_position_fo;
	int m_startframe[FIREWORKS_NUM_FON];

public:

	CFxFireWorks();
	virtual ~CFxFireWorks();

	void DeleteRes();

	BOOL Render(EffectSort &effect_sort);
	BOOL Render_Small(EffectSort &effect_sort);
	BOOL Render_Screw(EffectSort &effect_sort);
	BOOL Render_Fountain(EffectSort &effect_sort);

};

#endif // _FX_FIRE_WORKS_H_
