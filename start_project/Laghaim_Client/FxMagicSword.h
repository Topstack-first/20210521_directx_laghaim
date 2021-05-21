#ifndef _FX_MAGIC_SWORD_H_
#define _FX_MAGIC_SWORD_H_

#include "Config.h"

class CFxMagicSword
{
	CTailPlane*				m_MagicSword;

	// nate 2004 - 8 ImageManager
//	DWORD					m_MagicSwordTexId;
//	int						m_maxMagicSword;
//	int						m_maxTailPos;

	// nate 2004 - 7 - image
//	IndexedTexture		*m_pIndexedTexture;


	D3DXCOLOR*				m_ptailRGB;

public:
	CFxMagicSword();
	virtual ~CFxMagicSword();

	void Create();

	void LoadRes(IndexedTexture *pIndexedTexture);
	void DeleteRes();

	BOOL Render(EffectSort &effect_sort);

};

#endif // _FX_MAGIC_SWORD_H_



