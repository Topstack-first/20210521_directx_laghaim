#ifndef _CONTROL_BOSSRAID_INFO_H_
#define _CONTROL_BOSSRAID_INFO_H_

#include "Config.h"

class CSurface;
class CNkCharacter;

class CControlBossraidInfo : public CBaseInterface
{
public:

	struct bossinfo
	{
		BOOL	bKill;
		CSurface* pEna;  // Ȱ�� �̹���
	};

	CControlBossraidInfo();
	~CControlBossraidInfo();

	CSurface	*m_pBack;
	CSurface	*m_pFinalBossEna;	   // ������ ���� Ȱ�� �̹���

	bossinfo	m_bBossRaidInfo[5][3];  // ���� ų ����
	int			m_KillCount;
	RECT		m_ClientRc;

	void		Init();
	void		LoadRes();
	void		DeleteRes();
	void		SetBossraidKillFlag( int BossZoneNum, int BossNum, BOOL KillFlag );
	BOOL		IsBossAllKill();
	void		InitKillCount();

	int			IsInside(int x, int y);
	LRESULT		MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	void		Draw();
};

#endif // _CONTROL_BOSSRAID_INFO_H_
