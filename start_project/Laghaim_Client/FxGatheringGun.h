#ifndef _FX_GATHERING_GUN_H_
#define _FX_GATHERING_GUN_H_

#include "Config.h"

class CFxGatheringGun
{
	LPD3DLVERTEX			m_gather;
	// nate 2004 - 8 ImageManager
//	CRectPlane*				light;
//	int						max_gather_vertexs;
//	DWORD					m_gatherTexId;
//	DWORD					m_lightTexId;

	float					m_fScale;
	float					m_fFade;

	// nate 2004 - 7 - image
//	IndexedTexture		*m_pIndexedTexture;
	int						m_nTotalFrame;


public:
	CFxGatheringGun();
	virtual ~CFxGatheringGun();

	void Create();

	void LoadRes(IndexedTexture *pIndexedTexture);
	void DeleteRes();

	BOOL Render(EffectSort &effect_sort);

};

#endif // _FX_GATHERING_GUN_H_

