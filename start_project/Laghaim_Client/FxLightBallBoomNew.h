#ifndef _FX_LIGHT_BALL_BOOM_NEW_H_
#define _FX_LIGHT_BALL_BOOM_NEW_H_

#include "Config.h"

class CFxLightBallBoomNew
{
	// nate 2004 - 8 ImageManager
//	CSphere*				lightball;
//	DWORD					m_lightballTexId;

	CRectPlane*				light;

//	DWORD					m_lightTexId;
//	DWORD					m_lightTexId[4];

	int						m_numTLight;

	float					m_fScale;
	float					m_fFade;

	// nate 2004 - 7 - image
//	IndexedTexture		*m_pIndexedTexture;
	int						m_nTotalFrame;

public:
	CFxLightBallBoomNew();
	virtual ~CFxLightBallBoomNew();

	void Create();

	void LoadRes(IndexedTexture *pIndexedTexture);
	void DeleteRes();

	BOOL Render(EffectSort &effect_sort);
};

#endif // _FX_LIGHT_BALL_BOOM_NEW_H_
