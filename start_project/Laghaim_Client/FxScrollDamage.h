#ifndef _FX_SCROLL_DAMAGE_H_
#define _FX_SCROLL_DAMAGE_H_

#include "Config.h"

class CFxScrollDamage
{
	CRectPlane*				ring;

	// nate 2004 - 8 ImageManager
//	DWORD					m_ringTexId;

	float					m_fScale;
	float					m_fFade;

	// nate 2004 - 7 - image
//	IndexedTexture		*m_pIndexedTexture;
	int						m_nTotalFrame;



public:
	CFxScrollDamage();
	virtual ~CFxScrollDamage();

	void Create();

	void LoadRes(IndexedTexture *pIndexedTexture);
	void DeleteRes();

	BOOL Render(EffectSort &effect_sort);

};

#endif // _FX_SCROLL_DAMAGE_H_
