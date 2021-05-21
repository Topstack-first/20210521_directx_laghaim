#ifndef _FX_SPACE_SLASH_H_
#define _FX_SPACE_SLASH_H_

#include "Config.h"

class CLolos;

class CFxSpaceSlash
{
	CRectPlane*	m_rect_Shiled; //리플렉션 보호막
	CRectPlane*	light1;
	CRectPlane*	light2;
	CRectPlane*	light3;
	CRectPlane*	light4;
	float	m_fScale;
	float	m_fFade;

	float	fMinScale; //보호막의 최소 크기
	float	fMaxScale;//보호막의 최대 크기
	float	fFlag;

	int	m_nTotalFrame;
	int	m_numTLight;
public:
	CFxSpaceSlash();
	virtual ~CFxSpaceSlash();

	CLolos		*m_pLolo;
	CLolos		*m_pLolo2;
	void		LoadRes();
	void		DeleteRes();
	void		Render(EffectSort &effect_sort);
};

#endif // _FX_SPACE_SLASH_H_
