#ifndef _FX_SHOW_UP2_H_
#define _FX_SHOW_UP2_H_

#include "Config.h"

class CFxShowUp2
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
//	int						m_numTField;

	float					m_fScale;
	float					m_fFade;

	// nate 2004 - 7 - image
//	IndexedTexture		*m_pIndexedTexture;
	int						m_nTotalFrame;


public:
	CFxShowUp2();
	virtual ~CFxShowUp2();

	void Create();

	void LoadRes(IndexedTexture *pIndexedTexture);
	void DeleteRes();

	void SetTextureNum();

	BOOL Render(EffectSort &effect_sort);

};

#endif // _FX_SHOW_UP2_H_
