#ifndef _FX_VAMPIRE_ATTACK_H_
#define _FX_VAMPIRE_ATTACK_H_

#include "Config.h"

class CFxVampireAttack
{
	CTailPlane*				m_VampireBall;

	// nate 2004 - 8 ImageManager
//	DWORD					m_VampireBallTexId;
//	int						m_maxVampireBall;
//	int						m_maxTailPos;

	// nate 2004 - 7 - image
//	IndexedTexture		*m_pIndexedTexture;

	D3DXCOLOR*				m_ptailRGB;

public:
	CFxVampireAttack();
	virtual ~CFxVampireAttack();

	void Create();

	void LoadRes(IndexedTexture *pIndexedTexture);
	void DeleteRes();

	BOOL Render(EffectSort &effect_sort);

};

#endif // _FX_VAMPIRE_ATTACK_H_





