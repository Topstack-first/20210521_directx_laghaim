#ifndef _FX_SUM_PART_H_
#define _FX_SUM_PART_H_

#include "Config.h"

class CLolos;

class CFxSumPart
{
	// // nate 2004 - 8 ImageManager
//	CSphere*				eng_sphere;
	CRectPlane*				bottem;

//	DWORD					m_engTexId[4];
//	DWORD					m_bottemTexId;

//	int						m_numTEng;
//	int						m_nBaseTexIdx;

public:
	CFxSumPart();
	virtual ~CFxSumPart();

	CLolos *m_pLolo;// object 하나.

	// nate 2004 - 7 - image
//	IndexedTexture		*m_pIndexedTexture;
	// nate 2004 - 7 글로벌 ShadowRect사용
//	CShadowRect			*m_pShadowRect;

	// // nate 2004 - 8 ImageManager
//	void		LoadRes(IndexedTexture *pIndexedTexture, CShadowRect *pShadowRect);
	void		LoadRes();

	void		DeleteRes();

	void		Render(EffectSort &effect_sort);
};

#endif // _FX_SUM_PART_H_
