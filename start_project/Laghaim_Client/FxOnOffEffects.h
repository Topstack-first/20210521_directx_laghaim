#ifndef _FX_ONOFF_EFFECTS_H_
#define _FX_ONOFF_EFFECTS_H_

#include "Config.h"

class CFxOnOffEffects
{
	CRectPlane*				effects;

	// nate 2004 - 8 ImageManager
//	DWORD					m_onTexId;
//	DWORD					m_offTexId;
//	DWORD					m_lightTexId;

	float					m_fScale;
	float					m_fFade;

	// nate 2004 - 7 - image
//	IndexedTexture		*m_pIndexedTexture;
	int						m_nTotalFrame;


public:
	CFxOnOffEffects();
	virtual ~CFxOnOffEffects();

	void Create();
	void LoadRes(IndexedTexture *pIndexedTexture);
	void DeleteRes();

	BOOL Render(EffectSort &effect_sort);

};

#endif // _FX_ONOFF_EFFECTS_H_


