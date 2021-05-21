#ifndef _CONTROL_HERO_PRIVILEGE_H_
#define _CONTROL_HERO_PRIVILEGE_H_

#include "Config.h"

class CSurface;
class CNkCharacter;
class CBasicButton;

class CControlHeroPrivilege : public CBaseInterface
{
public:
	CControlHeroPrivilege();
	~CControlHeroPrivilege();

	CSurface		*m_pSurBack;
	CSurface		*m_pSurInfo;
	RECT			m_ClientRc;
	CBasicButton	m_btnPrivilege;
	CBasicButton	m_btnClose;

	int				m_HeroPrivilege;

	void		LoadRes();
	void		DeleteRes();

	void		SetHeroPrivilege(int heroPrivilege );
	int			IsInside(int x, int y);
	LRESULT		MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	void		Draw();
};

#endif // _CONTROL_BOSSRAID_INFO_H_
