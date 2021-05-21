#ifndef _FX_MOB_SHOW_UP_H_
#define _FX_MOB_SHOW_UP_H_

#include "Config.h"

class CFxMobShowUp
{
	CRectPlane*				ground_corona;

	CCylinderPlane*			shining_ring;
	CCylinderPlane*			shining_lignt;

	// nate 2004 - 8 ImageManager
//	DWORD					m_lifeTexId;
//	DWORD					m_coronaTexId;
//	DWORD					m_laserTexId;
//	DWORD					m_heal_roundTexId;

	float					m_fScale;
	float					m_fFade;

	// nate 2004 - 7 - image
//	IndexedTexture		*m_pIndexedTexture;
	int						m_nTotalFrame;

public:
	CFxMobShowUp();
	virtual ~CFxMobShowUp();

	void Create();

	void LoadRes(IndexedTexture *pIndexedTexture);
	void DeleteRes();

	BOOL Render(EffectSort &effect_sort);
};

#endif // _FX_MOB_SHOW_UP_H_


