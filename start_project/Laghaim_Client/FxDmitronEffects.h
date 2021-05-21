#ifndef _FX_DMITRON_EFFECTS_H
#define _FX_DMITRON_EFFECTS_H

#include "Config.h"


// ÀÌ³à¼®Àº InsertMagic µîÀ¸·Î FxSet¿¡¼­ ·»´õÇÏ´Â ³à¼®ÀÌ ¾Æ´Ô¿¡ À¯ÀÇ!
#include "BasicButton.h"

// Æ÷Å»
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

	void Render(EffectSort &effect_sort); // FxSet¿¡¼­ ·»´õÇÏ´Â ³à¼®ÀÌ ¾Æ´Ô¿¡ À¯ÀÇ!
};

// Å©¸®½ºÅ»
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

	void Render(EffectSort &effect_sort); // FxSet¿¡¼­ ·»´õÇÏ´Â ³à¼®ÀÌ ¾Æ´Ô¿¡ À¯ÀÇ!
};

#endif // _FX_DMITRON_EFFECTS_H


