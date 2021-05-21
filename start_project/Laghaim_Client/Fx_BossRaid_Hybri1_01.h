#ifndef _FX_BOSSRAID_HYBRI1_01
#define _FX_BOSSRAID_HYBRI1_01

#include "Config.h"

class CLolos;

class CFxBossRaidHybri1_01
{
protected:
	D3DVECTOR		vPos[4];
	bool			mb_sound;
	D3DLVERTEX		m_flame[4];                 //-- Effect Billboard
	D3DVECTOR		vPosFrom;                   //-- Effect ÀÇ ÁÂÇ¥
	CRectPlane*		m_board;					//-- ºôº¸µå

public:
	CFxBossRaidHybri1_01();
	virtual ~CFxBossRaidHybri1_01();

	void	LoadRes();
	void	DeleteRes();
	bool	Render(EffectSort &effect_sort);

	void	RenderFire(float dx[2], float dy[2], float dz[2],
					   float fFrame);

	void	RenderExplosion(D3DVECTOR vEyeVec, D3DVECTOR vPos,
							float fIntesity, float fScale);

};

#endif // _FX_BOSSRAID_HYBRI1_01