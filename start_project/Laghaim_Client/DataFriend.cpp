#include "stdafx.h"
#include "DataFriend.h"

DataFriend::DataFriend()
{
	m_bDenyState = false;
}

DataFriend::~DataFriend()
{
}

void DataFriend::Clear()
{
	m_mapFriends.clear();
	m_vecFriends_Deny.clear();
	m_vecFriends_Wait.clear();
}

bool DataFriend::AddFriend(const int& index, const std::string& name, const bool& login, const int& channel, const int& zone)
{
	MAP_FRIEND::iterator itFind = m_mapFriends.find(index);
	if( itFind != m_mapFriends.end() )
		return false;

	stFriend newFriend;
	newFriend.index = index;
	newFriend.login = login;
	newFriend.name = name;
	newFriend.channel = channel;
	newFriend.zone = zone;

	m_mapFriends.insert(std::make_pair(index, newFriend));

	return true;
}

bool DataFriend::RemoveFriend(const int& index)
{
	MAP_FRIEND::iterator itFind = m_mapFriends.find(index);
	if( itFind == m_mapFriends.end() )
		return false;

	m_mapFriends.erase(index);

	return true;
}

bool DataFriend::RemoveFriend(const std::string& name)
{
	MAP_FRIEND::iterator itBeg = m_mapFriends.begin();
	MAP_FRIEND::iterator itEnd = m_mapFriends.end();
	std::string temp;
	for( ; itBeg != itEnd; itBeg++ )
	{
		if( itBeg->second.name == name )
		{
			m_mapFriends.erase(itBeg);
			return true;
		}
	}

	return false;
}

bool DataFriend::ChangeLogin(const int& index, const bool& login)
{
	MAP_FRIEND::iterator itFind = m_mapFriends.find(index);
	if( itFind == m_mapFriends.end() )
		return false;

	itFind->second.login = login;

	return true;
}

bool DataFriend::ChangeChannel(const int& index, const int& channel)
{
	MAP_FRIEND::iterator itFind = m_mapFriends.find(index);
	if( itFind == m_mapFriends.end() )
		return false;

	itFind->second.channel = channel;

	return true;
}

bool DataFriend::ChangeZone(const int& index, const int& zone)
{
	MAP_FRIEND::iterator itFind = m_mapFriends.find(index);
	if( itFind == m_mapFriends.end() )
		return false;

	itFind->second.zone = zone;

	return true;
}

stFriend DataFriend::GetFriendInfo(const int& index)
{
	stFriend friendVal;
	friendVal.index = 0;

	MAP_FRIEND::iterator itFind = m_mapFriends.find(index);
	if( itFind != m_mapFriends.end() )
	{
		friendVal = itFind->second;
	}

	return friendVal;
}

stFriend DataFriend::GetFriendInfo(const std::string& name)
{
	stFriend friendVal;
	friendVal.index = 0;

	MAP_FRIEND::iterator itBeg = m_mapFriends.begin();
	MAP_FRIEND::iterator itEnd = m_mapFriends.end();
	for( ; itBeg != itEnd; itBeg++ )
	{
		if( itBeg->second.name == name )
		{
			friendVal = itBeg->second;
			break;
		}
	}

	return friendVal;
}

const DataFriend::MAP_FRIEND& DataFriend::GetFriendList()
{
	return m_mapFriends;
}

bool DataFriend::AddDeny(const int& index, const std::string& name)
{
	VEC_FRIEND::iterator itBeg = m_vecFriends_Deny.begin();
	VEC_FRIEND::iterator itEnd = m_vecFriends_Deny.end();
	for( ; itBeg != itEnd; itBeg++ )
	{
		if( itBeg->index == index )
			return false;
	}

	stFriend newDeny;
	newDeny.index = index;
	newDeny.name = name;

	m_vecFriends_Deny.push_back(newDeny);

	return true;
}

bool DataFriend::AddDeny(const std::string& name)
{
	VEC_FRIEND::iterator itBeg = m_vecFriends_Deny.begin();
	VEC_FRIEND::iterator itEnd = m_vecFriends_Deny.end();
	for( ; itBeg != itEnd; itBeg++ )
	{
		if( itBeg->name == name )
			return false;
	}

	stFriend newDeny;
	newDeny.name = name;

	m_vecFriends_Deny.push_back(newDeny);

	return true;
}

bool DataFriend::RemoveDeny(const int& index)
{
	VEC_FRIEND::iterator itBeg = m_vecFriends_Deny.begin();
	VEC_FRIEND::iterator itEnd = m_vecFriends_Deny.end();
	for( ; itBeg != itEnd; itBeg++ )
	{
		if( itBeg->index == index )
		{
			m_vecFriends_Deny.erase(itBeg);
			return true;
		}
	}

	return false;
}

bool DataFriend::RemoveDeny(const std::string& name)
{
	VEC_FRIEND::iterator itBeg = m_vecFriends_Deny.begin();
	VEC_FRIEND::iterator itEnd = m_vecFriends_Deny.end();
	for( ; itBeg != itEnd; itBeg++ )
	{
		if( itBeg->name == name )
		{
			m_vecFriends_Deny.erase(itBeg);
			return true;
		}
	}

	return false;
}

stFriend DataFriend::GetDenyInfo(const int& index)
{
	stFriend friendVal;
	friendVal.index = 0;

	VEC_FRIEND::iterator itBeg = m_vecFriends_Deny.begin();
	VEC_FRIEND::iterator itEnd = m_vecFriends_Deny.end();
	for( ; itBeg != itEnd; itBeg++ )
	{
		if( itBeg->index == index )
		{
			friendVal = *itBeg;
		}
	}

	return friendVal;
}

stFriend DataFriend::GetDenyInfo(const std::string& name)
{
	stFriend friendVal;
	friendVal.index = 0;

	VEC_FRIEND::iterator itBeg = m_vecFriends_Deny.begin();
	VEC_FRIEND::iterator itEnd = m_vecFriends_Deny.end();
	for( ; itBeg != itEnd; itBeg++ )
	{
		if( itBeg->name == name )
		{
			friendVal = *itBeg;
		}
	}

	return friendVal;
}

const DataFriend::VEC_FRIEND& DataFriend::GetDenyList()
{
	return m_vecFriends_Deny;
}

bool DataFriend::IsDenyFriend(const int& index)
{
	VEC_FRIEND::iterator itBeg = m_vecFriends_Deny.begin();
	VEC_FRIEND::iterator itEnd = m_vecFriends_Deny.end();
	for( ; itBeg != itEnd; itBeg++ )
	{
		if( itBeg->index == index )
		{
			return true;
		}
	}

	return false;
}

bool DataFriend::AddWait(const int& index, const std::string& name)
{
	VEC_FRIEND::iterator itBeg = m_vecFriends_Wait.begin();
	VEC_FRIEND::iterator itEnd = m_vecFriends_Wait.end();
	for( ; itBeg != itEnd; itBeg++ )
	{
		if( itBeg->index == index )
			return false;
	}

	stFriend newWait;
	newWait.index = index;
	newWait.name = name;

	m_vecFriends_Wait.push_back(newWait);

	return true;
}

bool DataFriend::AddWait(const std::string& name)
{
	VEC_FRIEND::iterator itBeg = m_vecFriends_Wait.begin();
	VEC_FRIEND::iterator itEnd = m_vecFriends_Wait.end();
	for( ; itBeg != itEnd; itBeg++ )
	{
		if( itBeg->name == name )
			return false;
	}

	stFriend newWait;
	newWait.name = name;

	m_vecFriends_Wait.push_back(newWait);

	return true;
}

bool DataFriend::RemoveWait(const int& index)
{
	VEC_FRIEND::iterator itBeg = m_vecFriends_Wait.begin();
	VEC_FRIEND::iterator itEnd = m_vecFriends_Wait.end();
	for( ; itBeg != itEnd; itBeg++ )
	{
		if( itBeg->index == index )
		{
			m_vecFriends_Wait.erase(itBeg);
			return true;
		}
	}

	return false;
}

bool DataFriend::RemoveWait(const std::string& name)
{
	VEC_FRIEND::iterator itBeg = m_vecFriends_Wait.begin();
	VEC_FRIEND::iterator itEnd = m_vecFriends_Wait.end();
	for( ; itBeg != itEnd; itBeg++ )
	{
		if( itBeg->name == name )
		{
			m_vecFriends_Wait.erase(itBeg);
			return true;
		}
	}

	return true;
}

stFriend DataFriend::GetWaitInfo(const int& index)
{
	stFriend friendVal;
	friendVal.index = 0;

	VEC_FRIEND::iterator itBeg = m_vecFriends_Wait.begin();
	VEC_FRIEND::iterator itEnd = m_vecFriends_Wait.end();
	for( ; itBeg != itEnd; itBeg++ )
	{
		if( itBeg->index == index )
		{
			friendVal = *itBeg;
		}
	}

	return friendVal;
}

stFriend DataFriend::GetWaitInfo(const std::string& name)
{
	stFriend friendVal;
	friendVal.index = 0;

	VEC_FRIEND::iterator itBeg = m_vecFriends_Wait.begin();
	VEC_FRIEND::iterator itEnd = m_vecFriends_Wait.end();
	for( ; itBeg != itEnd; itBeg++ )
	{
		if( itBeg->name == name )
		{
			friendVal = *itBeg;
		}
	}

	return friendVal;
}

const DataFriend::VEC_FRIEND& DataFriend::GetWaitList()
{
	return m_vecFriends_Wait;
}