#ifndef _GUILD_MEMBER_PAGE_H_
#define _GUILD_MEMBER_PAGE_H_

#include "Config.h"

#include "basewindow.h"
#include "basicbutton.h"

class CGuildMemberPage : public CUIBaseWindow
{

	/*******************************************************************/
public:
	int	 Draw();

	BOOL	InitMode();
	void LoadRes();

	void DeleteRes();
	void RestoreSurfaces();
	void SetPosition();

	BOOL MoveWindow(UINT msg, int x, int y);
	BOOL IsInside(int x, int y);
	BOOL IsMouseUpperWindow(int x, int y)
	{
		return TRUE;
	}

	LRESULT MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	/*******************************************************************/

	void	guilds_SetEnable(BOOL bEnable);
private:
	CBasicButton	m_btPromotion;	// ½Â±Þ
	CBasicButton	m_btDissolve;	// ÇØ»ê
	CBasicButton	m_btStore;		// Ã¢°í
	CBasicButton	m_btSearch;		// °Ë»ö
	CBasicButton	m_btBattle;		// ÀüÅõ
	CBasicButton	m_btMix;		// Á¶ÇÕ
	CBasicButton	m_btOut;		// Å»Åð

	CSurface*		m_pTextSur;

public:
	CGuildMemberPage();
	virtual ~CGuildMemberPage();
};
#endif // _GUILD_MEMBER_PAGE_H_