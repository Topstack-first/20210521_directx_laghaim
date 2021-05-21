#ifndef _FX_VAMPIRE_H_
#define _FX_VAMPIRE_H_

#include "Config.h"

class CFxVampire
{
	CTailPlane*				m_vampire;

	// nate 2004 - 8 ImageManager
//	DWORD					m_vampireTexId;
//	int						m_maxVampire;
//	int						m_maxTailPos;

	// nate 2004 - 7 - image
//	IndexedTexture		*m_pIndexedTexture;

	D3DXCOLOR*				m_ptailRGB;

public:
	CFxVampire();
	virtual ~CFxVampire();

	void Create();

	void LoadRes(IndexedTexture *pIndexedTexture);
	void DeleteRes();

	BOOL Render(EffectSort &effect_sort);

};


#endif // _FX_VAMPIRE_H_



