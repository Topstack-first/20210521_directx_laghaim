#ifndef _FX_BOSSRAID_ADIA2_02
#define _FX_BOSSRAID_ADIA2_02

#include "Config.h"

#define FX_BOSSRAID_ADIA2_02_MAX_LASER_VERTEXS 4

class CLolos;
class CRectPlane;

class CFxBossRaidAidia2_02
{
protected:
	LPD3DLVERTEX			m_laser;
	CRectPlane*				light;
	float					m_fScale;
	float					m_fFade;

public:
	CFxBossRaidAidia2_02();
	virtual ~CFxBossRaidAidia2_02();

	void	LoadRes();
	void	DeleteRes();
	bool	Render(EffectSort &effect_sort);
};

#endif // _FX_BOSSRAID_ADIA2_02