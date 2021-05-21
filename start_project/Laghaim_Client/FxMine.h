#ifndef _FX_MINE_H_
#define _FX_MINE_H_

#include "Config.h"

class CLolos;

class CFxMine
{
	CCapsyong	*m_pCapsyong;	// ���� ĸ��
	CBiped		*m_pBpd[ 3 ];	// 0:�̵�, 1:����, 2:����

	BYTE		m_byState[ 4 ];	// 0:�̵�, 1:����, 2:����, 3:����
	int			m_nArrivalFrame;

public:
	CFxMine();
	virtual ~CFxMine();
	void	Create();
	void	LoadRes();
	void	DeleteRes();
	BOOL	Render(EffectSort &effect_sort);
	void	SetArrivalMine( int byState )
	{
		memset( m_byState, byState, sizeof(m_byState) );
	}
};

#endif // _FX_MINE_H_
