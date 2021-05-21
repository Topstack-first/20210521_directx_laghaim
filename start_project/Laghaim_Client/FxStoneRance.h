#ifndef _FX_STONE_RANCE_H_
#define _FX_STONE_RANCE_H_

#include "Config.h"

class CLolos;

class CFxStoneRance
{
	CRectPlane*	smoke;		// �ٴ� �������� ����Ʈ
	int			m_numTEng;
	CLolos*		m_pLolo;	// ���� �ö���� �ѷ�
	CLolos*		m_pLolo2;	// nate 2005-06-16 : Burning Crash - ���� Ƣ�� �ѷ�

public:
	CFxStoneRance();
	virtual ~CFxStoneRance();
	void		LoadRes();
	void		DeleteRes();
	void		Render(EffectSort &effect_sort);

	// [3/6/2008 �����] ���淣�� �÷��� ���� �߰�(���̵�ƺ���3 2�� ��ų ���ÿ��� ���)
	bool		Render_EX(EffectSort &effect_sort);
};

#endif // _FX_STONE_RANCE_H_