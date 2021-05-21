#ifndef _FX_SCAN_VIEW_H_
#define _FX_SCAN_VIEW_H_

#include "Config.h"

class CFxScanView
{
	CCylinderPlane*			scan;
	CCylinderPlane*			scan_ring;

	CRectPlane*				corona;

	// nate 2004 - 8 ImageManager
//	DWORD					m_scanTexId;
//	DWORD					m_lightTexId;

	float					m_fScale;
	float					m_fFade;

	// nate 2004 - 7 - image
//	IndexedTexture		*m_pIndexedTexture;
	int						m_nTotalFrame;



public:
	CFxScanView();
	virtual ~CFxScanView();

	void Create();

	void LoadRes(IndexedTexture *pIndexedTexture);
	void DeleteRes();

	BOOL Render(EffectSort &effect_sort);


};

#endif // _FX_SCAN_VIEW_H_

