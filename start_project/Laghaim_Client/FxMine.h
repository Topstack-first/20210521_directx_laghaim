#ifndef _FX_MINE_H_
#define _FX_MINE_H_

#include "Config.h"

class CLolos;

class CFxMine
{
	CCapsyong	*m_pCapsyong;	// ¸¶ÀÎ Ä¸¼õ
	CBiped		*m_pBpd[ 3 ];	// 0:ÀÌµ¿, 1:µµÂø, 2:½ÇÆÐ

	BYTE		m_byState[ 4 ];	// 0:ÀÌµ¿, 1:µµÂø, 2:ÆøÆÄ, 3:½ÇÆÐ
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
