#ifndef _FX_MAGIC_ARROW_H_
#define _FX_MAGIC_ARROW_H_

#include "Config.h"

class CFxMagicArrow
{
	LPD3DLVERTEX			m_arrow;

	// nate 2004 - 8 ImageManager
//	int						max_arrow_vertexs;
//	DWORD					m_arrowTexId;

	float					m_fScale;
	float					m_fFade;

	// nate 2004 - 7 - image
//	IndexedTexture		*m_pIndexedTexture;
	int						m_nTotalFrame;


public:
	CFxMagicArrow();
	virtual ~CFxMagicArrow();

	void Create();

	void LoadRes(IndexedTexture *pIndexedTexture);
	void DeleteRes();

	BOOL Render(EffectSort &effect_sort);

};

#endif // _FX_MAGIC_ARROW_H_
