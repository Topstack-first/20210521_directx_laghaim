#include "stdafx.h"
#include "ShoutChatMgr.h"

ShoutChatMgr::ShoutChatMgr()
	: m_slotNum(0)
	, m_itemType(0)
	, m_itemIndex(0)
{
}

ShoutChatMgr::~ShoutChatMgr()
{
}

void ShoutChatMgr::Clear()
{
	m_slotNum = 0;
	m_itemIndex = 0;
	m_itemType = 0;
	m_strMessage = "";
}

void ShoutChatMgr::SetSlotNum(const int slotNum)
{
	m_slotNum = slotNum;
}

const int ShoutChatMgr::GetSlotNum()
{
	return m_slotNum;
}

void ShoutChatMgr::SetItemType(const int itemType)
{
	m_itemType = 0;
}

const int ShoutChatMgr::GetItemType()
{
	return m_itemType;
}

void ShoutChatMgr::SetItemIndex(const int itemIndex)
{
	m_itemIndex = itemIndex;
}

const int ShoutChatMgr::GetItemIndex()
{
	return m_itemIndex;
}

void ShoutChatMgr::SetMessage(const std::string& msg)
{
	m_strMessage = msg;
}

const std::string& ShoutChatMgr::GetMessage()
{
	return m_strMessage;
}