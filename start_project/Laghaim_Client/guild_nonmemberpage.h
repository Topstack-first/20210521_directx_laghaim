#if !defined(_GUILD_NONE_MEMBER_)
#define _GUILD_NONE_MEMBER_

#include "Config.h"

#include "basewindow.h"
#include "basicbutton.h"

class CGuildNoneMemberPage : public CUIBaseWindow
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

private:
	CBasicButton	m_btCreate;	// 창설
	CBasicButton	m_btSearch;	// 검색

	CSurface*		m_pTextSur;

public:
	CGuildNoneMemberPage();
	virtual ~CGuildNoneMemberPage();
};
#endif