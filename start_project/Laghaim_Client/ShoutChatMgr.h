#ifndef _SHOUT_CHAT_MGR_H_
#define _SHOUT_CHAT_MGR_H_

#include "singleton.h"

class ShoutChatMgr : public CSingleton<ShoutChatMgr>
{
public:
	ShoutChatMgr();
	~ShoutChatMgr();

public:
	void Clear();

	void SetSlotNum(const int slotNum);
	const int GetSlotNum();

	void SetItemType(const int itemType);
	const int GetItemType();

	void SetItemIndex(const int itemIndex);
	const int GetItemIndex();

	void SetMessage(const std::string& msg);
	const std::string& GetMessage();

private:
	int m_slotNum;
	int m_itemType;
	int m_itemIndex;
	std::string	m_strMessage;
};

#endif // _SHOUT_CHAT_MGR_H_
