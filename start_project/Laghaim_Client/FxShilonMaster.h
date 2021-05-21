#ifndef _FX_SHILON_MASTER_H_
#define _FX_SHILON_MASTER_H_

#include "Config.h"

class CLolos;

class CFxShilonMaster
{
	D3DLVERTEX				m_lhead[6];
	D3DLVERTEX				m_lsmoke[4];

	// nate 2004 - 8 ImageManager
//	DWORD					m_headTexId;
//	DWORD					m_smokeTexId;
//	float					m_fScale;
//	float					m_fFade;

	// nate 2004 - 7 - image
//	IndexedTexture		*m_pIndexedTexture;
	int						m_nTotalFrame;

public:
	CFxShilonMaster();
	virtual ~CFxShilonMaster();

	void		Create();

	void		LoadRes(IndexedTexture *pIndexedTexture);
	void		DeleteRes();

	BOOL		Render(EffectSort &effect_sort);

};

#endif // _FX_SHILON_MASTER_H_
