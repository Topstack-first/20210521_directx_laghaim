#ifndef _FX_WEAPON_EFFECTS_H_
#define _FX_WEAPON_EFFECTS_H_

#include "Config.h"

class CFxWeaponEffects
{
	// nate 2004 - 8 ImageManager
//	CRectPlane*				light;
//	DWORD					m_lightTexId;
//	DWORD					m_coronaTexId;
//	DWORD					m_traceTexId;
//	DWORD					m_shootTexId;
//	DWORD					m_spareTexId;

	// nate 2004 - 7 - image
//	IndexedTexture		*m_pIndexedTexture;

public:
	CFxWeaponEffects();
	virtual ~CFxWeaponEffects();

	void LoadRes(IndexedTexture *pIndexedTexture);
	void DeleteRes();

	void Update_Weapon_Particle(EffectSort &effect_sort, int linetype);
	BOOL Render(EffectSort &effect_sort);

};

#endif // _FX_WEAPON_EFFECTS_H_
