#ifndef _FX_KAI3_03_H_
#define _FX_KAI3_03_H_

#include "Config.h"

#define SMOKE_NUM 30

class CFxBossRaidKai3_03
{
protected :
	CRectPlane	*m_poison_Smoke[SMOKE_NUM];
	D3DTLVERTEX m_TLVec[4];
	DWORD m_Alpha;

	CRectPlane*				ground_ring;
	float	m_fadeValue;

public:
	CFxBossRaidKai3_03();
	~CFxBossRaidKai3_03();

	void Create();
	void LoadRes();
	void DeleteRes();
	BOOL Render( EffectSort &effect_sort );
	BOOL RenderRing(EffectSort &effect_sort);
};

#endif