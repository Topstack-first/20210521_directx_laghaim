#ifndef _CUIFRIEND_H_
#define _CUIFRIEND_H_

// MsgProc
#include "DataFriend.h"

class CSurface;
class CBasicButton;
class CToggleButton;
class CUIFriendList;
class CUIFriendDenyList;

class CUIFriend
{
public:
	CUIFriend();
	~CUIFriend();

	void LoadRes();
	void DeleteRes();

	LRESULT MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	void Draw();

public:
	bool IsFocus();
	BOOL IsVisible()
	{
		return m_bVisible;
	}
	void SetVisible(BOOL visible);

public:
	bool AddFriend(stFriend newFriend);
	bool RemoveFriend(const int& index);
	bool ChangeLogin(const int& index, const bool& login);
	bool ChangeChannel(const int& index, const int& channel);
	bool ChangeZone(const int& index, const int& zone);

	bool AddDeny(stFriend newDeny);
	bool RemoveDeny(const int& index);

	bool SetRefusal(BOOL bRefusal);

	HRESULT RestoreSurfaces();

private:
	void BtnLoadRes(int posX, int posY);
	void SendAddFriendReq();
	void SendAddDenyReq();
	void SendRefusalReq(BOOL bRefusal);

	enum { eBtnListFriend = 0, eBtnListBlock, eBtnAddFriend, eBtnAddBlock, eBtnMax };

	int m_iPosX;
	int m_iPosY;

	CSurface* m_pBack;
	CBasicButton* m_pBtnClose;
	CBasicButton* m_pBtn[eBtnMax];
	CToggleButton* m_pTglRefusal;

	CTextBox m_tbAddFriend;

	CUIFriendList* m_pFriendList;
	CUIFriendDenyList* m_pFriendDenyList;

	BOOL m_bVisible;
};

#endif // _CUIFRIEND_H_