#ifndef _FX_WORLDCUP_BOOM_H_
#define _FX_WORLDCUP_BOOM_H_

#include "Config.h"

class CFxWorldCupBoom
{
	CRectPlane*				hit;

	// nate 2004 - 8 ImageManager
//	DWORD					m_hitTexId;

	float					m_fScale;
	float					m_fFade;

	// nate 2004 - 7 - image
//	IndexedTexture		*m_pIndexedTexture;
	int						m_nTotalFrame;

public:
	CFxWorldCupBoom();
	virtual ~CFxWorldCupBoom();

	void Create();

	void LoadRes(IndexedTexture *pIndexedTexture);
	void DeleteRes();

	BOOL Render(EffectSort &effect_sort);

};

#endif // _FX_WORLDCUP_BOOM_H_
