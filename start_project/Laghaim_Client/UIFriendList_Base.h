#ifndef _CUIFRIENDLIST_BASE_H_
#define _CUIFRIENDLIST_BASE_H_

#include <Windows.h>
#include <vector>
#include "Scroll.h"

class CUIFriendLine;
class ControlPopupMenu;
class CUIFriendList_Base
{
protected:
	typedef std::vector<CUIFriendLine*> VEC_FRIEND_INFO;

	enum { eLineHeight = 19 };
	enum { eFriendListMax = 50, eFriendListLineMax = 12};
	enum { eEmpty = -1, eOffLine = 0, eOnLine };
	enum { eTextListGapX = 1, eTextListGapY = 4, };

public:
	CUIFriendList_Base(int x, int y);
	virtual ~CUIFriendList_Base();

	virtual void LoadRes();
	virtual void DeleteRes();
	virtual LRESULT MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	virtual void Draw();

public:
	void SetVisible(const bool& visible);
	const bool& GetVisible()
	{
		return m_bVisible;
	}
	HRESULT RestoreSurfaces();

protected:
	bool isExistFriend(const int& index);
	void DrawScroll();
	bool IsInside(const int pointX, const int pointY);
	void OpenPopupMenu(int posX, int posY);
	void ClosePopupMenu();

protected:
	int m_iPosX;
	int m_iPosY;
	bool m_bVisible;

	CScroll m_Scroll;

	ControlPopupMenu* m_pPopupMenu;
	VEC_FRIEND_INFO	m_vecFriend;
};

#endif