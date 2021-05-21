#ifndef _CUIFRIENDDENYLIST_H_
#define _CUIFRIENDDENYLIST_H_

#include "UIFriendList_Base.h"

struct stFriend;
class CUIFriendDenyList : public CUIFriendList_Base
{
public:
	CUIFriendDenyList(int x, int y);
	~CUIFriendDenyList();

	void LoadRes();
	void DeleteRes();
	void Draw();

public:
	bool AddFriend(stFriend newDeny);
	bool RemoveFriend(const int& index);

private:
	void SortList();
};

#endif