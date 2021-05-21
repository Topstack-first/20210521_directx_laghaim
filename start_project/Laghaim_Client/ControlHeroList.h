#ifndef _CONTROL_HERO_LIST_H_
#define _CONTROL_HERO_LIST_H_

#include "Config.h"

class CSurface;
class CBasicButton;
class CNkCharacter;

#define MAX_HERO_LIST_PAGE 10

class CControlHeroRankList : public CBaseInterface
{
public:

	struct HERORANKLIST
	{
		BOOL		bEnable;
		CSurface*	pSurRank;
		CSurface*	pSurName;
	};

	CControlHeroRankList();
	~CControlHeroRankList();

	CSurface		*m_pBack;
	HERORANKLIST	m_RankList[MAX_HERO_LIST_PAGE];
	RECT			m_ClientRc;
	HFONT			m_Font;
	CBasicButton	m_btnClose;
	CBasicButton	m_btnArrowR;
	CBasicButton	m_btnArrowL;
	int				m_Race;
	CSurface*		m_pSurRace[NUM_RACE];
	int				m_Page;
	int				m_TotalPage;
	CSurface*		m_pSurPage;

	void		LoadRes();
	void		DeleteRes();
	void		SetRace(int race = -1);
	void		SetHeroRankLsit(char* buf);
	void		SetFont (int f_w, int f_h, char *fontname);

	void		SendMsgPage(int page = 1);

	int			IsInside(int x, int y);
	LRESULT		MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	void		Draw();
};

#endif // _CONTROL_BOSSRAID_INFO_H_
