#ifndef _FX_NUCLEAR_BOOM_H_
#define _FX_NUCLEAR_BOOM_H_

#include "Config.h"

class IndexedTexture;
class CShadowRect;

class CFxNuclearBoom
{
	CSphere*				sphere;

	CRectPlane*				light;

	CCylinderPlane*			wave;
	CCylinderPlane*			wave2;

	// nate 2004 - 8 ImageManager
//	DWORD					m_dustTexId;
//	DWORD					m_waveTexId;
//	DWORD					m_lightTexId;
//	int						m_nBoomTexIdx, m_nBaseTexIdx;
//	float					m_fBoomSize;
//	int						m_nTotalFrame;

	D3DLVERTEX				m_lverBoom[4];
	Texcode					m_Texcode[12];		// 한 이미지에 있는 구분되는 텍스쳐 수.
	int						m_nFrameIdx[FX_NUCLEARBOOM_FRAMECOUNT];
	float					m_fBaseSize[FX_NUCLEARBOOM_FRAMECOUNT];

	float					m_fScale;
	float					m_fFade;

	// nate 2004 - 7 - image
//	IndexedTexture		*m_pIndexedTexture;
	// nate 2004 - 7 글로벌 ShadowRect사용
//	CShadowRect			*m_pShadowRect;


public:
	CFxNuclearBoom();
	virtual ~CFxNuclearBoom();

	void		Create();

	void		LoadRes(IndexedTexture *pIndexedTexture, CShadowRect *pShadowRect = NULL);
	void		DeleteRes();

	BOOL		Render(EffectSort &effect_sort);
};

#endif // _FX_NUCLEAR_BOOM_H_



