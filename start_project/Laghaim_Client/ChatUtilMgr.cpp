#include "stdafx.h"
#include "ChatUtilMgr.h"

ChatUtilMgr::ChatUtilMgr()
: m_curMsgIndex(-1)
{
	m_vecChatMsg.reserve(128);
}

ChatUtilMgr::~ChatUtilMgr()
{

}

void ChatUtilMgr::Clear()
{
	m_curMsgIndex = -1;
	m_vecChatMsg.clear();
}

void ChatUtilMgr::AddMsg(std::string strMsg)
{	
	m_vecChatMsg.push_back(strMsg);
	m_curMsgIndex = m_vecChatMsg.size();
}

std::string ChatUtilMgr::GetPrevMsg()
{
	--m_curMsgIndex;

	return getMsg();
}

std::string ChatUtilMgr::GetNextMsg()
{
	++m_curMsgIndex;

	return getMsg();
}

std::string ChatUtilMgr::getMsg()
{
	std::string strRet = "";

	int nVecSize = m_vecChatMsg.size();

	if( nVecSize == 0 )
	{
		m_curMsgIndex = -1;
		return strRet;
	}

	if( m_curMsgIndex < 0 )
	{
		m_curMsgIndex = nVecSize - 1;		
	}
	else if( m_curMsgIndex >= nVecSize )
	{
		m_curMsgIndex = 0;		
	}
	
	strRet = m_vecChatMsg[m_curMsgIndex];	
		
	return strRet;
}