#ifndef _FX_WARGIANT_ATTACK_H_
#define _FX_WARGIANT_ATTACK_H_

#include "Config.h"

class CFxWarGiantAttack
{
	CRectPlane*				hit;
	CRectPlane*				hit_boom;

	// nate 2004 - 8 ImageManager
//	DWORD					m_hitTexId;
//	DWORD					m_hitboomTexId;
//	int						m_numTHit;

	float					m_fScale;
	float					m_fFade;

	// nate 2004 - 7 - image
//	IndexedTexture		*m_pIndexedTexture;
	int						m_nTotalFrame;


public:
	CFxWarGiantAttack();
	virtual ~CFxWarGiantAttack();

	void Create();

	void LoadRes(IndexedTexture *pIndexedTexture);
	void DeleteRes();

	void SetTextureNum();

	BOOL Render(EffectSort &effect_sort);

};

#endif // _FX_WARGIANT_ATTACK_H_


