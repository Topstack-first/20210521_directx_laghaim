#ifndef _CHAT_UTIL_MGR_H_
#define _CHAT_UTIL_MGR_H_

#include "singleton.h"

class ChatUtilMgr : public CSingleton<ChatUtilMgr>
{
private:
	typedef std::vector<std::string> VEC_CHATMSG;

public:
	ChatUtilMgr();
	~ChatUtilMgr();

public:
	void Clear();
	void AddMsg(std::string strMsg);
	std::string GetPrevMsg();
	std::string GetNextMsg();

private:
	std::string getMsg();

private:
	int m_curMsgIndex;
	VEC_CHATMSG m_vecChatMsg;
};

#endif