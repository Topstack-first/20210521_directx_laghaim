#ifndef _FX_BOSSRAID_LASTBOSS_02_
#define _FX_BOSSRAID_LASTBOSS_02_

#include "Config.h"

class CLolos;

#define FX_BOSSRAID_LASTBOSS_02_MAX_PARTICLE 15
#define FX_BOSSRAID_LASTBOSS_02_FRAME 12

#define FX_BOSSRAID_LASTBOSS_02_STONE_WIDTH 12
#define FX_BOSSRAID_LASTBOSS_02_STONE_HEIGHT 16

#define FX_BOSSRAID_LASTBOSS_02_HEIGHT 10.0f
#define FX_BOSSRAID_LASTBOSS_02_SIZE 20.0f

#define FX_BOSSRAID_LASTBOSS_02_MAX_EVENT 35


class CFxBossRaidLastBoss_02
{
protected:
	CRectPlane*	m_Floor;		// 바닦 갈라지는 이펙트
	CRectPlane*	m_Plane;
	D3DLVERTEX	m_lverRect[16], m_lverRectSmoke[16];


	//연기 관련
	D3DLVERTEX	m_lverBoom[4];
	Texcode		m_Texcode[12];
	int			m_nFrameIdx[ FX_STONESHOTBOOM_FRAMECOUNT ];


public:
	CFxBossRaidLastBoss_02();
	virtual ~CFxBossRaidLastBoss_02();

	int			ActiveStone(int nFrame);

	void		LoadRes();
	void		DeleteRes();
	bool		Render(EffectSort &effect_sort);
	void		RenderSmoke(D3DMATRIX matDecal, D3DVECTOR vDrawPos, int nFrame);
};

#endif