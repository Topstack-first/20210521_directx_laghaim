#ifndef _FX_BOSSRAID_LASTBOSS_04_
#define _FX_BOSSRAID_LASTBOSS_04_

#include "Config.h"

class CLolos;

#define FX_BOSSRAID_LASTBOSS_04_FLAME       (3)
#define FX_BOSSRAID_LASTBOSS_04_FLAME_FRAME (30)
#define FX_BOSSRAID_LASTBOSS_04_FLAME_COUNT (3*30)

#define FX_BOSSRAID_LASTBOSS_04_FRAME0 33
#define FX_BOSSRAID_LASTBOSS_04_FRAME1 37
#define FX_BOSSRAID_LASTBOSS_04_FRAME2 60
#define FX_BOSSRAID_LASTBOSS_04_FRAME3 120

class CFxBossRaidLastBoss_04
{

protected:
	unsigned int end_idx;

	CLolos*	m_pLolo;
	CRectPlane*	m_Plane;
	D3DVECTOR vFireDir[3];
	D3DVECTOR vFirePos[3][2];

	inline int ParticleOffset(int x, int y);

public:
	CFxBossRaidLastBoss_04();
	virtual ~CFxBossRaidLastBoss_04();


	int GetMovingRange(int nFrame);
	Deco_Particle* MakeParticle(float dir);
	D3DVECTOR GetDeltaVector(float rx, float ry, float dx, float dy);

	void		LoadRes();
	void		DeleteRes();
	bool		Render(EffectSort &effect_sort);
	void		RenderFireBall(D3DVECTOR vViewVec, D3DVECTOR pos,
							   float alpha, float gamma, int time);
};

inline int
CFxBossRaidLastBoss_04::ParticleOffset(int type, int frame)
{
	return frame * FX_BOSSRAID_LASTBOSS_04_FLAME + type;
}

#endif