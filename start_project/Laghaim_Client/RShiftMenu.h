#ifndef _RSHIFT_MENU_H_
#define _RSHIFT_MENU_H_

#include "Config.h"

class CSurface;
class CBasicButton;
class CTextOutBox;

class CRShiftMenu
{
public:
	CRShiftMenu();
	~CRShiftMenu();

	int GetWidth();
	int GetHeight();
	void SetXPos(int x);
	void SetYPos(int y);
	void SetCurRace(int race);
	void SetUserName(char* name);
	void CloseMenu();

	void LoadRes();
	void DeleteRes();
	LRESULT MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	void Draw();

	BOOL IsInside(int x, int y);

private:
	void ProcAddFriend();

public:
	enum { eExchange = 0, eParty, eRefine, eWhisper, eCustodian, eFriend, ePvP , eBtnEnd };

	CSurface *m_pSurfaceBack;
	CBasicButton* m_pBtn[eBtnEnd];

	CTextOutBox *m_pNameText;

	char m_UserName[100];

	int m_nRace;
};

#endif // _R_SHIFT_MENU_H_