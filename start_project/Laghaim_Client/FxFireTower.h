#ifndef _FX_FIRE_TOWER_H_
#define _FX_FIRE_TOWER_H_

#include "Config.h"

//메트릭스 그랜드 스톤 깨졌을 때의 효과(mungmae-03/21)
class CFxFireTower
{
	CCylinderPlane*			inside_ring;
	CCylinderPlane*			outside_ring;

	float					m_fScale;
	float					m_fRotation;
	float					m_fFade;
	int						m_nTotalFrame;

public:
	CFxFireTower();
	virtual ~CFxFireTower();

	void Create();

	void LoadRes(IndexedTexture *pIndexedTexture);
	void DeleteRes();

	BOOL Render(EffectSort &effect_sort);
};
#endif // _FX_FIRE_TOWER_H
