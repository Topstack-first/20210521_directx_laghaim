#ifndef _FX_HELL_CRY_H_
#define _FX_HELL_CRY_H_

#include "Config.h"

class CLolos;
class CFxHellCry
{
	// nate 2004 - 8 ImageManager
// 	CSphere*				eng_sphere;
	CRectPlane*				smoke;

	// nate 2004 - 8 ImageManager
//	DWORD					m_engTexId[4];
//	DWORD					m_smokeTexId;
//	DWORD					m_swordTexId;

	D3DLVERTEX				m_lsword[6];

//	int						m_numTEng;

public:
	CFxHellCry();
	virtual ~CFxHellCry();

	CLolos *m_pLolo;						// object 하나.
	// nate 2004 - 7 - image
//	IndexedTexture		*m_pIndexedTexture;
	// nate 2004 - 7 글로벌 ShadowRect사용
//	CShadowRect			*m_pShadowRect;
//	int					m_nBaseTexIdx;
//	int					m_nBaseTexIdx1;
//	int					m_nBaseTexIdx2;
//	int					m_nBaseTexIdx3;
//	int					m_nBaseTexIdx4;
//	int					m_nHeadTexIdx;

	// nate 2004 - 8 ImageManager
//	void		LoadRes(IndexedTexture *pIndexedTexture, CShadowRect *pShadowRect);
	void		LoadRes();

	void		DeleteRes();

	void		Render(EffectSort &effect_sort);

};
#endif // _FX_HELL_CRY_H_
