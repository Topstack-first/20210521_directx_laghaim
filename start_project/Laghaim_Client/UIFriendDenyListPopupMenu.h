#ifndef _UIFRIENDDENYLIST_POPUP_MENU_H_
#define _UIFRIENDDENYLIST_POPUP_MENU_H_

#include "ControlPopupMenu.h"

class CSurface;
class UIFriendDenyListPopupMenu : public ControlPopupMenu
{
public:
	enum
	{
		eFRIEND_DENY_POPUP_MENU_DENY = 0,
		eFRIEND_DENY_POPUP_MENU_END,
	};

public:
	UIFriendDenyListPopupMenu();
	~UIFriendDenyListPopupMenu();

public:
	void LoadRes();
	void DeleteRes();
	LRESULT MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

private:
	void LoadRes_Btn();

private:
	void ProcUndeny();
};

#endif