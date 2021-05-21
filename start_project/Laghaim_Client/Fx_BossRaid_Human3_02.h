#ifndef _FX_HUMANBOSS3_1_H_
#define _FX_HUMANBOSS3_1_H_

#include "Config.h"

class CFxBossRaidHuman3_02
{
public:
	CRectPlane*			m_CirclePlane[2];

public:
	CFxBossRaidHuman3_02();
	virtual ~CFxBossRaidHuman3_02();
	void	LoadRes();
	void	DeleteRes();
	void	SetDevice();
	void	SetAlpha( bool startFrame, EffectSort &ef_sort, float &nAlpha, int time );
	bool	Render(EffectSort &effect_sort);
	void	CameraShakeFrame( int currentFrame );

protected:
	bool	m_bStartFrame;
	float	m_nAlpha;

	D3DVECTOR m_oldEye;	// [12/18/2007 반재승] 이전 카메라 좌표를 저장하기 위해
	D3DVECTOR m_oldAt;
};

#endif