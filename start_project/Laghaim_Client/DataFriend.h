#ifndef _DATAFRIEND_H_
#define _DATAFRIEND_H_

#include "singleton.h"
#include <vector>
#include <map>
#include <string>

struct stFriend
{
	stFriend() : index(0), login(false), channel(-1), zone(0), deny(false)
	{

	}

	int index;	// ģ�� �ε���
	std::string name;	// ģ�� ĳ���͸�
	bool login;			// ģ���� ���� ����
	int channel;		// ģ���� �����ϰ� �ִ� ä��
	int zone;			// ģ���� �����ϰ� �ִ� �� �ѹ�
	bool deny;			// ���� ��� �ΰ�
};

class DataFriend : public CSingleton<DataFriend>
{
public:
	typedef std::map<int, stFriend> MAP_FRIEND;
	typedef std::vector<stFriend> VEC_FRIEND;
	DataFriend();
	~DataFriend();

public:
	void Clear();

	bool AddFriend(const int& index, const std::string& name, const bool& login, const int& channel, const int& zone);
	bool RemoveFriend(const int& index);
	bool RemoveFriend(const std::string& name);
	bool ChangeLogin(const int& index, const bool& login);
	bool ChangeChannel(const int& index, const int& channel);
	bool ChangeZone(const int& index, const int& zone);
	stFriend GetFriendInfo(const int& index);
	stFriend GetFriendInfo(const std::string& name);
	const MAP_FRIEND& GetFriendList();

	bool AddDeny(const int& index, const std::string& name);
	bool AddDeny(const std::string& name);
	bool RemoveDeny(const int& index);
	bool RemoveDeny(const std::string& name);
	stFriend GetDenyInfo(const int& index);
	stFriend GetDenyInfo(const std::string& name);
	const VEC_FRIEND& GetDenyList();
	bool IsDenyFriend(const int& index);

	bool AddWait(const int& index, const std::string& name);
	bool AddWait(const std::string& name);
	bool RemoveWait(const int& index);
	bool RemoveWait(const std::string& name);
	stFriend GetWaitInfo(const int& index);
	stFriend GetWaitInfo(const std::string& name);
	const VEC_FRIEND& GetWaitList();

	void SetDenyState(const bool& denyState)
	{
		m_bDenyState = denyState;
	}
	const bool& GetDenyState()
	{
		return m_bDenyState;
	}

private:
	MAP_FRIEND m_mapFriends;		// ģ�� ����Ʈ
	VEC_FRIEND m_vecFriends_Deny;	// ���� ����Ʈ
	VEC_FRIEND m_vecFriends_Wait;	// ��� ����Ʈ

	bool m_bDenyState;				// ���� ģ�� �ź� ����
};

#endif