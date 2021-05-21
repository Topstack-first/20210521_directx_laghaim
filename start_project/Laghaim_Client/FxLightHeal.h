#ifndef _FX_LIGHT_HEAL_H_
#define _FX_LIGHT_HEAL_H_

#include "Config.h"

#define HEAL_MAX_FORCE		0.02f
#define HEAL_MAX_VELOCITY	0.8
#define HEAL_DAMP			0.998f
#define HEAL_MAX_DIST	   60.0f

#define HEAL_MAX_SPARKLES	150
#define HEAL_CUR_SPARKLES	20
// nate 2004 - 8 ImageManager
#define FX_LIGHTHEAL_LIGHTSIZE	2.0f

class CFxLightHeal
{
public:
	CFxLightHeal();
	virtual ~CFxLightHeal();

	// nate 2004 - 7 - image
//	IndexedTexture		*m_pIndexedTexture;

	// 데이타 멤버와 상수
	// nate 2004 - 8 ImageManager
//	WORD		m_wTexIdx;
	D3DLVERTEX	m_lverRect[4], m_lverRot[4];
//	float		m_fLightSize;
	int			m_nTotalFrame;

	void		InitSparkles();
	Sparkle		RandomSparkle();
	// nate 2004 - 8 ImageManager
//	int			m_nMaxSparkles;

	Sparkle		*m_pSparkles;
	D3DLVERTEX  *m_pMesh;
	WORD		*m_pIndices;
	D3DVECTOR	m_vecBound;


	void		LoadRes(IndexedTexture *pIndexedTexture);
	void		DeleteRes();

	void		FrameMove(float fTimeKey);
	BOOL		Render(EffectSort &effect_sort);
};

#endif // _FX_LIGHT_HEAL_H_
