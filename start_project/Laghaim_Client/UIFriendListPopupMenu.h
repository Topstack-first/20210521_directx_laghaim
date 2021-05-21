#ifndef _UIFRIENDLIST_POPUP_MENU_H_
#define _UIFRIENDLIST_POPUP_MENU_H_

#include "ControlPopupMenu.h"

class CSurface;
class UIFriendListPopupMenu : public ControlPopupMenu
{
public:
	enum
	{
		eFRIEND_POPUP_MENU_WHISPER = 0,
		eFRIEND_POPUP_MENU_REMOVE,
		eFRIEND_POPUP_MENU_END,
	};

public:
	UIFriendListPopupMenu();
	~UIFriendListPopupMenu();

public:
	void LoadRes();
	void DeleteRes();
	LRESULT MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

private:
	void LoadRes_Btn();

private:
	void ProcWhisper();
	void ProcRemove();
};

#endif