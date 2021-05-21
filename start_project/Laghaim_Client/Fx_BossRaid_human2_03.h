#ifndef _FX_BOSSRAID_HUMAN2_03
#define _FX_BOSSRAID_HUMAN2_03

#include "Config.h"

#define FX_BOSSRAID_HUMAN2_03_MAX_LASER		(8)
#define FX_BOSSRAID_HUMAN2_03_MAX_TAIL		(16)

#define FX_BOSSRAID_HUMAN2_03_LASER_SIZE1	(450.f)
#define FX_BOSSRAID_HUMAN2_03_LASER_SIZE2	(45.f)
#define FX_BOSSRAID_HUMAN2_03_LASER_SIZE3	(30.f)

#define FX_BOSSRAID_HUMAN2_03_EXPLOSION		(26.f)

class CLolos;

class CFxBossRaidHuman2_03
{
protected:
	D3DVECTOR vControl[4];

	CRectPlane	*m_CirclePlane;

public:
	CFxBossRaidHuman2_03();
	virtual ~CFxBossRaidHuman2_03();

	CLolos	*m_pLolo; //날아가는 구체
	CLolos	*m_pLolo1; //모이는 에너지
	CLolos	*m_pLolo2; //에너지 폭발 Lolo

	void	LoadRes();
	void	DeleteRes();
	bool	Render(EffectSort &effect_sort);

	void	RenderWave(D3DVECTOR& pos, D3DVECTOR& scale, float alpha);
	void	RenderLaser(D3DVECTOR& mat, D3DVECTOR& pos, D3DVECTOR& scale, float alpha, float rotate = 0);
};

#endif // _FX_BOSSRAID_HUMAN2_03