#ifndef _FX_TELEPORT_H_
#define _FX_TELEPORT_H_

#include "Config.h"

class CFxTeleport
{
	CRectPlane*				ground_ring;
	CRectPlane*				ground_corona;

	CCylinderPlane*			shining_ring;
	CCylinderPlane*			shining_lignt;

	// nate 2004 - 8 ImageManager
//	DWORD					m_shieldTexId;
//	DWORD					m_coronaTexId;
//	DWORD					m_levelupTexId;
//	DWORD					m_heal_roundTexId;

	float					m_fScale;
	float					m_fFade;

	// nate 2004 - 7 - image
//	IndexedTexture		*m_pIndexedTexture;
	int						m_nTotalFrame;

public:
	CFxTeleport();
	virtual ~CFxTeleport();

	void Create();

	void LoadRes(IndexedTexture *pIndexedTexture);
	void DeleteRes();

	BOOL Render(EffectSort &effect_sort);
};

#endif // _FX_TELEPORT_H_


