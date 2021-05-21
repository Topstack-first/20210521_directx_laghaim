#ifndef _FX_HORN_BOOM_H_
#define _FX_HORN_BOOM_H_

#include "Config.h"

class CFxHornBoom
{
	CRectPlane*				hit;
//	CShpere*				hitsphere;

	// nate 2004 - 8 ImageManager
//	DWORD					m_hitTexId;

//	DWORD					m_hitsphereTexId;

	float					m_fScale;
	float					m_fFade;

	// nate 2004 - 7 - image
//	IndexedTexture		*m_pIndexedTexture;
	int						m_nTotalFrame;

public:
	CFxHornBoom();
	virtual ~CFxHornBoom();

	void Create();

	void LoadRes(IndexedTexture *pIndexedTexture);
	void DeleteRes();

	BOOL Render(EffectSort &effect_sort);

};

#endif // _FX_HORN_BOOM_H_
