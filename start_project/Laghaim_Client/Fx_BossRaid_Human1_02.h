#ifndef _FX_BOSSRAID_HUMAN1_02
#define _FX_BOSSRAID_HUMAN1_02

#include "Config.h"

class CLolos;

class CFxBossRaidHuman1_02
{
	CLolos					*m_pLolo;						// object ÇÏ³ª.

	D3DLVERTEX				m_lsmoke[4];

	// nate 2004 - 8 ImageManager
// 	DWORD					m_smokeTexId;
//	float					m_fScale;
//	float					m_fFade;
//	int						m_nTotalFrame;

	// nate 2004 - 7 - image
//	IndexedTexture		*m_pIndexedTexture;

public:
	CFxBossRaidHuman1_02();
	virtual ~CFxBossRaidHuman1_02();

	void		Create();

	// nate 2004 - 8 ImageManager
	void		LoadRes();

	void		DeleteRes();

	BOOL		Render(EffectSort &effect_sort);
};

#endif // _FX_WARGIANT_MULTI_H_


