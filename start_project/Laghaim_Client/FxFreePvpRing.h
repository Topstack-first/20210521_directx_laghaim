#ifndef _FX_FREE_PVP_RING_H_ ///자유대련
#define _FX_FREE_PVP_RING_H_

#include "Config.h"

// 이녀석은 InsertMagic 등으로 FxSet에서 렌더하는 녀석이 아님에 유의!

#include "BasicButton.h"

class CFxFreePvpRing
{
	float					m_fSaveTime;
	unsigned				m_nCurrFrame;

	// 0 : Smaller Cylinder
	// 1 : Larger Cylinder
	DWORD					m_CylinderId[2];
	CCylinderPlane*			m_CylinderPlane[2];

	// 0 : Inner Plane,
	// 1 : Outer Plane,
	// 2 : Outer Inverse Plane
	DWORD					m_CircleId[3];
	CRectPlane*				m_CirclePlane[3];

	float					m_fScale;

public:
	CFxFreePvpRing();
	virtual ~CFxFreePvpRing();

	void LoadRes();
	void DeleteRes();

	void Render(EffectSort &effect_sort); // FxSet에서 렌더하는 녀석이 아님에 유의!
};

#endif // _FX_FREE_PVP_RING_H_