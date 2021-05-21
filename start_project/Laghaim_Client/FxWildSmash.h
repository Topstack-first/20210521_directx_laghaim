#ifndef _FX_WILD_SMASH_H_
#define _FX_WILD_SMASH_H_

#include "Config.h"

class CLolos;
class CFxWildSmash
{
	CRectPlane*	m_rect_Shiled; //���÷��� ��ȣ��
	CRectPlane*	light1;
	CRectPlane*	light2;
	CRectPlane*	light3;
	CRectPlane*	light4;
	float	m_fScale;
	float	m_fFade;

	float	fMinScale; //��ȣ���� �ּ� ũ��
	float	fMaxScale;//��ȣ���� �ִ� ũ��
	float	fFlag;

	int	m_nTotalFrame;
	int	m_numTLight;
public:
	CFxWildSmash();
	virtual ~CFxWildSmash();

	CLolos		*m_pLolo;
	CLolos		*m_pLolo2;
	void		LoadRes();
	void		DeleteRes();
	void		Render(EffectSort &effect_sort);
};

#endif // _FX_WILD_SMASH_H_
