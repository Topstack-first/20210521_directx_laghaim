#ifndef _FX_GUARD_TOWER_H_
#define _FX_GUARD_TOWER_H_

#include "Config.h"

class CFxGuardTower
{
	CRectPlane*				head;

	D3DLVERTEX				m_lsmoke[4];

	// nate 2004 - 8 ImageManager
//	DWORD					m_headTexId;
//	DWORD					m_smokeTexId;
//	float					m_fScale;
//	float					m_fFade;

	// nate 2004 - 7 - image
//	IndexedTexture		*m_pIndexedTexture;
	int						m_nTotalFrame;

public:
	CFxGuardTower();
	virtual ~CFxGuardTower();

	void		Create();

	void		LoadRes(IndexedTexture *pIndexedTexture);
	void		DeleteRes();

	BOOL		Render(EffectSort &effect_sort);

};

#endif // _FX_GUARD_TOWER_H_
