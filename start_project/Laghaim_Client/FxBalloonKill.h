#ifndef _FX_BALLOON_KILL_H_
#define _FX_BALLOON_KILL_H_

#include "Config.h"

class CLolos;
class CFxBalloonKill
{
public:
	CFxBalloonKill();
	~CFxBalloonKill();

public:
	void LoadRes();
	BOOL Render(EffectSort& effect_sort);

private:
	CLolos*		m_pLolo;
};

#endif // _FX_BALLOON_KILL_H_
