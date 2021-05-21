#ifndef _FX_STONE_RANCE_H_
#define _FX_STONE_RANCE_H_

#include "Config.h"

class CLolos;

class CFxStoneRance
{
	CRectPlane*	smoke;		// 바닦 갈라지는 이펙트
	int			m_numTEng;
	CLolos*		m_pLolo;	// 돌이 올라오는 롤로
	CLolos*		m_pLolo2;	// nate 2005-06-16 : Burning Crash - 돌이 튀는 롤로

public:
	CFxStoneRance();
	virtual ~CFxStoneRance();
	void		LoadRes();
	void		DeleteRes();
	void		Render(EffectSort &effect_sort);

	// [3/6/2008 반재승] 스톤랜스 플래그 랜더 추가(에이디아보스3 2번 스킬 어택에서 사용)
	bool		Render_EX(EffectSort &effect_sort);
};

#endif // _FX_STONE_RANCE_H_