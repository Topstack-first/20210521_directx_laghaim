#include "stdafx.h"
#include <string.h>
#include "Nk2DFrame.h"
#include "TcpUtil.h"
#include "DataFriend.h"
#include "UIFriend.h"
#include "headers.h"
#include "window_resource.h"
#include "g_stringmanager.h"

void RecvFriendList(char* pack)
{
	char buf[MAX_PATH] = {0,};

	DataFriend* friendMgr = DataFriend::GetInstance();

	pack = AnyOneArg(pack, buf);
	int listCount = atoi(buf);

	int index = 0;
	std::string name = "";
	bool login = false;
	int channel = 0;
	int zone = 0;

	stFriend f;

	for( int i = 0; i < listCount; i++ )
	{
		pack = AnyOneArg(pack, buf);
		index = atoi(buf);
		pack = AnyOneArg(pack, buf);
		name = buf;
		pack = AnyOneArg(pack, buf);
		login = atoi(buf) == 0 ? false : true;
		pack = AnyOneArg(pack, buf);
		channel = atoi(buf);
		pack = AnyOneArg(pack, buf);
		zone = atoi(buf);

		if( friendMgr->AddFriend(index, name, login, channel, zone) )
		{
			f = friendMgr->GetFriendInfo(index);

			CUIFriend* pUIFriend = g_pNk2DFrame->GetFriend();
			if( pUIFriend )
				pUIFriend->AddFriend(f);
		}
	}
}

void RecvFriendListRefusal(char* pack)
{
	char buf[MAX_PATH] = {0,};

	DataFriend* friendMgr = DataFriend::GetInstance();

	pack = AnyOneArg(pack, buf);
	int listCount = atoi(buf);

	int index = 0;
	std::string name = "";

	stFriend f;

	for( int i = 0; i < listCount; i++ )
	{
		pack = AnyOneArg(pack, buf);
		index = atoi(buf);
		pack = AnyOneArg(pack, buf);
		name = buf;

		if( friendMgr->AddDeny(index, name) )
		{
			f = friendMgr->GetDenyInfo(index);

			CUIFriend* pUIFriend = g_pNk2DFrame->GetFriend();
			if( pUIFriend )
				pUIFriend->AddDeny(f);
		}
	}
}

void RecvFriendReq(char* pack)
{
	char buf[MAX_PATH] = {0,};

	pack = AnyOneArg(pack, buf);
	int index = atoi(buf);
	pack = AnyOneArg(pack, buf);
	std::string name = buf;

	char* szName = new char[MAX_PATH];
	strcpy(szName, name.c_str());

	char msg[MAX_PATH] = {0,};

	sprintf(msg, G_STRING(IDS_FRIEND_REQ), name.c_str());
	g_pNk2DFrame->InsertPopup(msg, TYPE_NOR_OKCANCLE, POPUP_FRIEND_ADD_REQ, 30, 0, (void*)szName);
}

void RecvFriendAdd(char* pack)
{
	char buf[MAX_PATH] = {0,};

	pack = AnyOneArg(pack, buf);
	int index = atoi(buf);
	pack = AnyOneArg(pack, buf);
	std::string name = buf;
	pack = AnyOneArg(pack, buf);
	int channel = atoi(buf);
	pack = AnyOneArg(pack, buf);
	int zone = atoi(buf);

	DataFriend* friendMgr = DataFriend::GetInstance();

	if( friendMgr->AddFriend(index, name, true, channel, zone) )
	{
		stFriend f = friendMgr->GetFriendInfo(index);
		CUIFriend* pUIFriend = g_pNk2DFrame->GetFriend();
		if( pUIFriend )
		{
			pUIFriend->AddFriend(f);
		}
	}
}

void RecvFriendDel(char* pack)
{
	char buf[MAX_PATH] = {0,};

	pack = AnyOneArg(pack, buf);
	int index = atoi(buf);

	if( DataFriend::GetInstance()->RemoveFriend(index) )
	{
		CUIFriend* pUIFriend = g_pNk2DFrame->GetFriend();
		if( pUIFriend )
		{
			pUIFriend->RemoveFriend(index);
		}
	}
}

void RecvFriendRefusal(char* pack)
{
	char buf[MAX_PATH] = {0,};

	pack = AnyOneArg(pack, buf);
	int order = atoi(buf);

	DataFriend* friendMgr = DataFriend::GetInstance();

	if( order == 1 )
	{
		friendMgr->SetDenyState(true);

		CUIFriend* pUIFriend = g_pNk2DFrame->GetFriend();
		if( pUIFriend )
			pUIFriend->SetRefusal(TRUE);
	}
	else if( order == 2)
	{
		friendMgr->SetDenyState(false);

		CUIFriend* pUIFriend = g_pNk2DFrame->GetFriend();
		if( pUIFriend )
			pUIFriend->SetRefusal(FALSE);
	}
	else if( order == 3 )
	{
		pack = AnyOneArg(pack, buf);
		int index = atoi(buf);
		pack = AnyOneArg(pack, buf);
		std::string name = buf;

		stFriend f;

		if( friendMgr->AddDeny(index, name) )
		{
			f = friendMgr->GetDenyInfo(index);

			CUIFriend* pUIFriend = g_pNk2DFrame->GetFriend();
			if( pUIFriend )
				pUIFriend->AddDeny(f);
		}
	}
	else if( order == 4 )
	{
		pack = AnyOneArg(pack, buf);
		int index = atoi(buf);

		if( friendMgr->RemoveDeny(index) )
		{
			CUIFriend* pUIFriend = g_pNk2DFrame->GetFriend();
			if( pUIFriend )
				pUIFriend->RemoveDeny(index);
		}
	}
}

void RecvFriendLogin(char* pack)
{
	char buf[MAX_PATH] = {0,};

	pack = AnyOneArg(pack, buf);
	int index = atoi(buf);
	pack = AnyOneArg(pack, buf);
	int channel = atoi(buf);
	pack = AnyOneArg(pack, buf);
	int zone = atoi(buf);

	DataFriend* friendMgr = DataFriend::GetInstance();
	friendMgr->ChangeLogin(index, true);
	friendMgr->ChangeChannel(index, channel);
	friendMgr->ChangeZone(index, zone);

	CUIFriend* pUIFriend = g_pNk2DFrame->GetFriend();
	if( pUIFriend )
	{
		pUIFriend->ChangeLogin(index, true);
		pUIFriend->ChangeChannel(index, channel);
		pUIFriend->ChangeZone(index, zone);
	}
}

void RecvFriendLogout(char* pack)
{
	char buf[MAX_PATH] = {0,};

	pack = AnyOneArg(pack, buf);
	int index = atoi(buf);

	DataFriend* friendMgr = DataFriend::GetInstance();
	friendMgr->ChangeLogin(index, false);
	friendMgr->ChangeChannel(index, -1);
	friendMgr->ChangeZone(index, 0);

	CUIFriend* pUIFriend = g_pNk2DFrame->GetFriend();
	if( pUIFriend )
	{
		pUIFriend->ChangeLogin(index, false);
		pUIFriend->ChangeChannel(index, -1);
		pUIFriend->ChangeZone(index, 0);
	}
}

void RecvFriendZoneMove(char* pack)
{
	char buf[MAX_PATH] = {0,};

	pack = AnyOneArg(pack, buf);
	int index = atoi(buf);
	pack = AnyOneArg(pack, buf);
	int zone = atoi(buf);

	DataFriend::GetInstance()->ChangeZone(index, zone);

	CUIFriend* pUIFriend = g_pNk2DFrame->GetFriend();
	if( pUIFriend )
	{
		pUIFriend->ChangeZone(index, zone);
	}
}

void RecvFriend(char* arg)
{
	char buf[MAX_PATH] = {0,};
	arg = AnyOneArg(arg, buf);

	if( 0 == strcmp(buf, "list") )
	{
		RecvFriendList(arg);
	}
	else if( 0 == strcmp(buf, "list_refusal") )
	{
		RecvFriendListRefusal(arg);
	}
	else if( 0 == strcmp(buf, "req") )
	{
		RecvFriendReq(arg);
	}
	else if( 0 == strcmp(buf, "add") )
	{
		RecvFriendAdd(arg);
	}
	else if( 0 == strcmp(buf, "del") )
	{
		RecvFriendDel(arg);
	}
	else if( 0 == strcmp(buf, "refusal") )
	{
		RecvFriendRefusal(arg);
	}
	else if( 0 == strcmp(buf, "login") )
	{
		RecvFriendLogin(arg);
	}
	else if( 0 == strcmp(buf, "logout") )
	{
		RecvFriendLogout(arg);
	}
	else if( 0 == strcmp(buf, "zonemove") )
	{
		RecvFriendZoneMove(arg);
	}
}
