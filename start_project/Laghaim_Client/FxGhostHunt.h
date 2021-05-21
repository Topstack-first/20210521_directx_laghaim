#ifndef _FX_GHOST_HUNT_H_
#define _FX_GHOST_HUNT_H_

#include "Config.h"

class CFxGhostHunt
{
	CTailPlane*				m_ghosthunt;
//	CSphere*				sphere;

	// nate 2004 - 8 ImageManager
//	DWORD					m_ghosthuntTexId;
//	int						m_maxGhostHunt;
//	int						m_maxTailPos;

	// nate 2004 - 7 - image
//	IndexedTexture		*m_pIndexedTexture;

	D3DXCOLOR*				m_ptailRGB;

public:
	CFxGhostHunt();
	virtual ~CFxGhostHunt();

	void Create();

	void LoadRes(IndexedTexture *pIndexedTexture);
	void DeleteRes();

	BOOL Render(EffectSort &effect_sort);

};

#endif // _FX_GHOST_HUNT_H_


