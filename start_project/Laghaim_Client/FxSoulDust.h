#ifndef _FX_SOUL_DUST_H_
#define _FX_SOUL_DUST_H_

#include "Config.h"

class CFxSoulDust
{
	LPD3DLVERTEX			m_souldust;

	// nate 2004 - 8 ImageManager
//	int						max_souldust_vertexs;
//	DWORD					m_souldustTexId;

	float					m_fScale;
	float					m_fFade;

	// nate 2004 - 7 - image
//	IndexedTexture		*m_pIndexedTexture;
	int						m_nTotalFrame;

public:
	CFxSoulDust();
	virtual ~CFxSoulDust();

	void Create();

	void LoadRes(IndexedTexture *pIndexedTexture);
	void DeleteRes();

	BOOL Render(EffectSort &effect_sort);

};

#endif // _FX_SOUL_DUST_H_


