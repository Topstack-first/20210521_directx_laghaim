#ifndef _FX_HYBIRBOSS3_1_H_
#define _FX_HYBIRBOSS3_1_H_

#define FX_BOSSRAID_HAYBRI3_01_ANIFRAME 48

#include "Config.h"

class CFxBossRaidHybri3_01
{
public:
	CRectPlane*			m_CirclePlane;

public:
	CFxBossRaidHybri3_01();
	virtual ~CFxBossRaidHybri3_01();
	void	LoadRes();
	void	DeleteRes();
	void	SetDevice();
	void	SetAlpha( bool startFrame, EffectSort &ef_sort, float &nAlpha, int time );
	bool	Render(EffectSort &effect_sort);

protected:
	bool	m_bStartFrame;
	float	m_nAlpha;
};

#endif