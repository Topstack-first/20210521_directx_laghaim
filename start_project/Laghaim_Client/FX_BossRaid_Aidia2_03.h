#ifndef _FX_BOSSRAID_ADIA2_03
#define _FX_BOSSRAID_ADIA2_03

#include "Config.h"

#define FX_BOSSRAID_ADIA2_03_MAX_EVENT  35
#define FX_BOSSRAID_ADIA2_03_MAX_PARTICLE 40
#define FX_BOSSRAID_ADIA2_03_MAX_LASER_VERTEXS 4

class CLolos;
class CRectPlane;

class CFxBossRaidAidia2_03
{
protected:
	LPD3DLVERTEX			m_laser;
	CRectPlane*				light;
	CLolos*					m_pLolo;
	float					m_fScale;
	float					m_fFade;

public:
	CFxBossRaidAidia2_03();
	virtual ~CFxBossRaidAidia2_03();

	void	LoadRes();
	void	DeleteRes();
	bool	Render(EffectSort &effect_sort);
};

#endif // _FX_BOSSRAID_ADIA2_03