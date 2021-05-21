#ifndef _CUIFRIENDLIST_H_
#define _CUIFRIENDLIST_H_

#include "UIFriendList_Base.h"

struct stFriend;
class CUIFriendList : public CUIFriendList_Base
{
public:
	CUIFriendList(int x, int y);
	~CUIFriendList(void);

	void LoadRes();
	void DeleteRes();
	void Draw();

public:
	bool AddFriend(stFriend newFriend);
	bool RemoveFriend(const int& index);
	bool ChangeLogin(const int& index, const bool& login);
	bool ChangeChannel(const int& index, const int& channel);
	bool ChangeZone(const int& index, const int& zone);

private:
	void ListLoadRes();
	void SortList();

private:
	CSurface* m_pOnlineIcon;
	CSurface* m_pOfflineIcon;
};

#endif // _CUIFRIENDLIST_H_
