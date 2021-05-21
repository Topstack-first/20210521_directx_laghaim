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
		CSurface* pEna;  // 활성 이미지
	};

	CControlBossraidInfo();
	~CControlBossraidInfo();

	CSurface	*m_pBack;
	CSurface	*m_pFinalBossEna;	   // 마지막 보스 활성 이미지

	bossinfo	m_bBossRaidInfo[5][3];  // 보스 킬 정보
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
