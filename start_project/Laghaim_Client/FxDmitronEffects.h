#ifndef _FX_DMITRON_EFFECTS_H
#define _FX_DMITRON_EFFECTS_H

#include "Config.h"


// �̳༮�� InsertMagic ������ FxSet���� �����ϴ� �༮�� �ƴԿ� ����!
#include "BasicButton.h"

// ��Ż
class CFxDmitronPortal
{
	float					m_fSaveTime;
	unsigned				m_nCurrFrame;

	// 0 : Inner Plane,
	// 1 : Outer Plane,
	// 2 : Outer Inverse Plane
	CRectPlane*				m_CirclePlane[3];
	unsigned				m_Rotation;

public:
	CFxDmitronPortal();
	virtual ~CFxDmitronPortal();

	void LoadRes();
	void DeleteRes();

	void Render(EffectSort &effect_sort); // FxSet���� �����ϴ� �༮�� �ƴԿ� ����!
};

// ũ����Ż
class CFxDmitronCrystal
{
	float					m_fSaveTime;
	unsigned				m_nCurrFrame;

	// 0 : Inner Plane,
	// 1 : Outer Plane,
	// 2 : Outer Inverse Plane
	CRectPlane*				m_CirclePlane[3];

	unsigned				m_Rotation;

public:
	CFxDmitronCrystal();
	virtual ~CFxDmitronCrystal();

	void LoadRes();
	void DeleteRes();

	void Render(EffectSort &effect_sort); // FxSet���� �����ϴ� �༮�� �ƴԿ� ����!
};

#endif // _FX_DMITRON_EFFECTS_H


