#ifndef _FX_MERCERNERY_SLASH_
#define _FX_MERCERNERY_SLASH_

#include "Config.h"

class CLolos;
class CFxMercernerySlash
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
	CFxMercernerySlash();
	virtual ~CFxMercernerySlash();

	CLolos		*m_pLolo;
	void		LoadRes();
	void		DeleteRes();
	void		Render(EffectSort &effect_sort);
};

#endif // _FX_MERCERNERY_SLASH_
