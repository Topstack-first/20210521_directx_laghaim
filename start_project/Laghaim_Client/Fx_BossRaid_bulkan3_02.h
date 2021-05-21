#ifndef _FX_VULKANBOSS3_1_H_
#define _FX_VULKANBOSS3_1_H_

#define _FX_BULKANBOSS3_02_1_START	13
#define _FX_BULKANBOSS3_02_2_START  22
#define _FX_BULKANBOSS3_02_3_START  29

#define _FX_BULKANBOSS3_VERTEX_COLOR		0
#define _FX_BULKANBOSS3_EFFECT_ENDTIME		1000
#define _FX_BULKANBOSS3_EFFECT_ALPHATIME	40

#include "Config.h"

class CFxBossRaidBulkan3_02
{
public:

	D3DTLVERTEX			m_TLVec[4];

public:

	CFxBossRaidBulkan3_02();
	virtual ~CFxBossRaidBulkan3_02();
	void	LoadRes();
	void	DeleteRes();
	void	SetDevice();
	bool	Render(EffectSort &effect_sort);
	void    SetAlpha(D3DTLVERTEX * in_TLVec, EffectSort &in_effect_sort, float in_Time);
	void	CameraShakeFrame( int currentFrame, int startFrame, int endFrame );
	void	SetRenderFrame( int num );

protected:
	bool	m_bStartFrame[3];
	DWORD   m_nStartTime[3];
};

#endif
