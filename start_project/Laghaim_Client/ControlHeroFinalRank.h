#ifndef _CONTROL_HERO_FINAL_RANK_H_
#define _CONTROL_HERO_FINAL_RANK_H_

#include "Config.h"

class CSurface;
class CNkCharacter;

#define MAX_HERO_FINAL_RANK 18


class CControlHeroFinalRank : public CBaseInterface
{
public:

	CControlHeroFinalRank();
	~CControlHeroFinalRank();

	struct HERORANK
	{
		BOOL		bEnable;
		CSurface	*pSur;
	};

	CSurface	*m_pBack;
	HERORANK	m_Rank[MAX_HERO_FINAL_RANK];
	RECT		m_ClientRc;
	HFONT		m_Font;

	void		LoadRes();
	void		DeleteRes();
	void		SetHeroRank(char* buf);
	void		SetFont (int f_w, int f_h, char *fontname);

	int			IsInside(int x, int y);
	LRESULT		MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	void		Draw();
};

#endif // _CONTROL_BOSSRAID_INFO_H_
