#include "stdafx.h"

#include <assert.h>

#include "s_guild_func.h"

#include "main.h"
#include "TcpipCon.h"
#include "TcpUtil.h"
#include "Nk2DFrame.h"
#include "headers.h"
#include "UIMgr.h"
#include "guild_search.h"
#include "ControlGuildM.h"
#include "s_notice_control.h"
#include "ControlGuildT.h"
#include "guild_data.h"
#include "WindowMgr.h"
#include "s_Job_control.h"
#include "s_member_control.h"
#include "s_joinrequest_control.h"
#include "guild_battle.h"
#include "s_guild_manager.h"
#include "time.h"
#include "land.h"
#include "g_stringmanager.h"






#define GUILD_INFO_UPDATE  1
#define GUILD_INFO_ADD	   0

char g_guildbuf[256] = "";
char g_guildbuf1[256] = "";
char g_guildbuf2[256] = "";
char g_guildbuf3[256] = "";

int  g_guildii[20] = {0,};

BOOL g_bSearch = FALSE;

// Guild set up
void guild_create(char *arg, CUIMgr *pMgr);		// 0
void guild_create_t(char *arg, CUIMgr *pMgr);
void guild_create_c(char *arg, CUIMgr *pMgr);

// Guild search
void guild_search(char *arg, CUIMgr *pMgr);		// 1
void guild_search_s(char *arg, CUIMgr *pMgr);
void guild_search_r(char *arg, CUIMgr *pMgr);
void guild_search_e(char *arg, CUIMgr *pMgr);

// Guild dissolved
void guild_disband(char *arg, CUIMgr *pMgr);	// 2

// Guild Store
void guild_stash(char *arg, CUIMgr *pMgr);		// 3
void guild_stash_n(char *arg, CUIMgr *pMgr);
void guild_stash_s(char *arg, CUIMgr *pMgr);
void guild_stash_r(char *arg, CUIMgr *pMgr);
void guild_stash_e(char *arg, CUIMgr *pMgr);
void guild_stash_t(char *arg, CUIMgr *pMgr);
void guild_stash_f(char *arg, CUIMgr *pMgr);
void guild_stash_p(char *arg, CUIMgr *pMgr);
void guild_stash_g(char *arg, CUIMgr *pMgr);

// Break with Guild
void guild_leave(char *arg, CUIMgr *pMgr);		// 4

// Guild upgrade
void guild_upgrade(char *arg, CUIMgr *pMgr);	// 5

// Guild battle

// Guild recommend joining
void guild_recom(char *arg, CUIMgr *pMgr);		// 6
void guild_recom_r(char *arg);
void guild_recom_a(char *arg);

// Guild general joining
void guild_request(char *arg, CUIMgr *pMgr);	// 7

// Guild Management
void guild_nick(char *arg, CUIMgr *pMgr);		// 8
void guild_notice(char *arg, CUIMgr *pMgr);		// 9
void guild_allow(char *arg, CUIMgr *pMgr);		// 10
void guild_appoint(char *arg, CUIMgr *pMgr);	// 11
void guild_fire(char *arg, CUIMgr *pMgr);		// 12
void guild_fired(char *arg, CUIMgr *pMgr);		// 13

// Guild Info �α��νÿ� ��� �Ϳ�~~~
void guild_ginfo(char *arg, CUIMgr *pMgr);		// 14
void guild_info(char *arg, CUIMgr *pMgr);		// 15
void guild_ninfo(char *arg, CUIMgr *pMgr);		// 16
void guild_ainfo(char *arg, CUIMgr *pMgr);		// 17
void guild_up(char *arg, CUIMgr *pMgr);			// 18
void guild_down(char *arg, CUIMgr *pMgr);		// 19

void guild_info_update(char *arg, CUIMgr *pMgr);
void guild_info_add(char *arg, CUIMgr *pMgr);

void guild_warning(char *arg, CUIMgr *pMgr);

/************* RANKING WAR ********************/
void guild_msg(char* arg, CUIMgr *pMgr);

/************* RANKING WAR ********************/
void guild_rankwar(char *arg, CUIMgr *pMgr);
void guild_rankwar_info(char *arg, CUIMgr *pMgr);
void guild_rankwar_ready(char *arg, CUIMgr *pMgr);
void guild_rankwar_count(char *arg, CUIMgr *pMgr);
void guild_rankwar_time(char *arg, CUIMgr *pMgr);
void guild_rankwar_open(char *arg, CUIMgr *pMgr);
void guild_rankwar_req(char *arg, CUIMgr *pMgr);
void guild_rankwar_list(char *arg, CUIMgr *pMgr);
void guild_rankwar_invite(char *arg, CUIMgr *pMgr);
void guild_rankwar_kcount(char *arg, CUIMgr *pMgr);
void guild_rankwar_start(char *arg, CUIMgr *pMgr);
void guild_rankwar_end(char *arg, CUIMgr *pMgr);
void guild_rankwar_goworld(char *arg, CUIMgr *pMgr);
void guild_rankwar_cancel(char *arg, CUIMgr *pMgr);
void guild_rankwar_get(char *arg, CUIMgr *pMgr);
void guild_rankwar_comp(char *arg, CUIMgr *pMgr);

/************* INVASION WAR ********************/
void guild_invasion_info(char *arg, CUIMgr *pMgr);
void guild_invasion_war(char *arg, CUIMgr *pMgr);
void guild_invasion_war_req(char *arg, CUIMgr *pMgr);
void guild_invasion_war_list(char *arg, CUIMgr *pMgr);
void guild_invasion_war_cancel(char *arg, CUIMgr *pMgr);

void guild_recom_info(char *arg, CUIMgr *pMgr);;

void guild_master_delegation(char *arg, CUIMgr *pMgr);

CGuildCmd* CGuildCmd::Create(CUIMgr *pMgr)
{
	return new CGuildCmd(pMgr);
}

CGuildCmd::CGuildCmd(CUIMgr *pMgr)
{
	m_pUIMgr = pMgr;

	guild_addcmd("create" , guild_create );
	guild_addcmd("search" , guild_search );
	guild_addcmd("disband", guild_disband);
	guild_addcmd("stash"  , guild_stash  );
	guild_addcmd("leave"  , guild_leave  );
	guild_addcmd("upgrade", guild_upgrade);
	guild_addcmd("recom"  , guild_recom  );
	guild_addcmd("nick"   , guild_nick	 );
	guild_addcmd("notice" , guild_notice );
	guild_addcmd("allow"  , guild_allow  );
	guild_addcmd("appoint", guild_appoint);
	guild_addcmd("fire"	  , guild_fire   );
	guild_addcmd("fired"  , guild_fired  );
	guild_addcmd("ginfo"  , guild_ginfo  );
	guild_addcmd("info"   , guild_info	 );
	guild_addcmd("ninfo"  , guild_ninfo  );
	guild_addcmd("ainfo"  , guild_ainfo  );
	guild_addcmd("up"	  , guild_up     );
	guild_addcmd("down"   , guild_down   );
	guild_addcmd("request", guild_request);
	guild_addcmd("warning", guild_warning);
	guild_addcmd("winfo"  , guild_invasion_info);
	guild_addcmd("war"	  , guild_invasion_war);
	guild_addcmd("rankwar", guild_rankwar);
	guild_addcmd("msg",		guild_msg);
	guild_addcmd("rinfo",	guild_recom_info);
	guild_addcmd("master_dg", guild_master_delegation);
}

CGuildCmd::~CGuildCmd()
{
}

BOOL CGuildCmd::guild_addcmd(char *cmd, void cmd_p(char *, CUIMgr *))
{
	m_mapFunc.insert(std::make_pair(cmd, &cmd_p));
	return TRUE;
}

void CGuildCmd::guild_run(char* cmd, char *arg)
{
	MAP_FUNC::iterator itFind = m_mapFunc.find(cmd);
	if( itFind == m_mapFunc.end() )
		return;

	itFind->second(arg, m_pUIMgr);
}

// ��� â��
void guild_create(char *arg, CUIMgr *pMgr)
{
	arg = AnyOneArg(arg, g_guildbuf);

	if(!strcmp(g_guildbuf, "t"))
	{
		// ��� â�� ���� success or fail
		arg = AnyOneArg(arg, g_guildbuf);

		int nResult = atoi(g_guildbuf);

		switch( nResult)
		{
		case -2: // IDS_GUILD_CREATE_GUILD_DONT "��带 â���� �� �����ϴ�."
			g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_CREATE_GUILD_DONT), TYPE_GUILD_CONFIRM, 42 );
			break;
		case -1: // IDS_GUILD_CREATE_GUILD_FAIL_NEED_LVL "������ �����մϴ�."
			g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_CREATE_GUILD_FAIL_NEED_LVL), TYPE_GUILD_CONFIRM, 42 );
			break;
		case 0: // 	IDS_GUILD_CREATE_GUILD_FAIL_NEED_LAIM "������ �����մϴ�."
			g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_CREATE_GUILD_FAIL_NEED_LAIM), TYPE_GUILD_CONFIRM, 42 );
			break;
		case 1:
			g_pNk2DFrame->GetUIWindow()->OnOpenWindow(WINDOW_GUILD_CREATE);
			break;
		}
	}
	else if(!strcmp(g_guildbuf, "c"))
	{
		// ��� â�� ���� success or fail
		arg = AnyOneArg(arg, g_guildbuf);

		int nResult = atoi(g_guildbuf);

		switch( nResult )
		{
		case -3: // IDS_GUILD_CREATE_GUILD_FAIL_NAME_DELETE "��ü�� ����̸��� ����� �� �����ϴ�."
			g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_CREATE_GUILD_FAIL_NAME_DELETE), TYPE_GUILD_CONFIRM, 42 );
			break;

		case -2: // IDS_GUILD_CREATE_GUILD_FAIL "��� â���� ���� �Ͽ����ϴ�."
			g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_CREATE_GUILD_FAIL), TYPE_GUILD_CONFIRM, 42 );
			break;

		case -1: // IDS_CHECK_ITEM "�������� Ȯ���� �ּ���!"
			g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_CHECK_ITEM), TYPE_GUILD_CONFIRM, 42 );
			break;

		case  0: // IDS_GUILD_CREATE_GUILD_FAIL_NAME "���� �̸��� ��尡 �����մϴ�."
			g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_CREATE_GUILD_FAIL_NAME), TYPE_GUILD_CONFIRM, 42 );
			break;

		case  1: // IDS_GUILD_CREATE_GUILD_SUC "��尡 â�� �Ǿ����ϴ�."
			g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_CREATE_GUILD_SUC), TYPE_GUILD_CONFIRM, 42 );
			break;
		}
	}
}

// ��� �˻�
void guild_search(char *arg, CUIMgr *pMgr)
{
	arg = AnyOneArg(arg, g_guildbuf);

	if(!strcmp(g_guildbuf, "s"))
	{
		// �˻� ���� �˸�
		CGuildSearch* pSearch = NULL;

		if(g_pNk2DFrame->GetUIWindow()->IsOpenWindow(WINDOW_GUILD_SEARCH))
		{
			pSearch = (CGuildSearch*)g_pNk2DFrame->GetUIWindow()->GetWindow(WINDOW_GUILD_SEARCH);

			if(pSearch)
			{
				pSearch->all_Reset();
				pSearch->set_start(TRUE);

				g_bSearch = FALSE;
			}
		}

	}
	else if(!strcmp(g_guildbuf, "r"))
	{
		g_listInfo list_tmp;
		// �˻������� ����.
		arg = AnyOneArg(arg, g_guildbuf );				// ����
		strcpy(list_tmp.guild_name, g_guildbuf);

		arg = AnyOneArg(arg, g_guildbuf);				// ��帶���͸�
		strcpy(list_tmp.guild_master_name, g_guildbuf);

		arg = AnyOneArg(arg, g_guildbuf);	// ��巩ũ
		g_guildii[0] = atoi(g_guildbuf);
		list_tmp.guild_rank = g_guildii[0];

		arg = AnyOneArg(arg, g_guildbuf);	// ��巹��
		g_guildii[0] = atoi(g_guildbuf);
		list_tmp.guild_glevel = g_guildii[0];

		arg = AnyOneArg(arg, g_guildbuf3);  // �������
		g_guildii[0] = atoi(g_guildbuf3);
		list_tmp.guild_point = g_guildii[0];

		list_tmp.guild_pageNum = 0;

		CGuildSearch* pSearch = NULL;

		if(g_pNk2DFrame->GetUIWindow()->IsOpenWindow(WINDOW_GUILD_SEARCH))
		{
			pSearch = (CGuildSearch*)g_pNk2DFrame->GetUIWindow()->GetWindow(WINDOW_GUILD_SEARCH);

			if(pSearch)
			{
				pSearch->guilds_insertlist(list_tmp);

				g_bSearch = TRUE;
			}
		}
	}
	else if(!strcmp(g_guildbuf, "e"))
	{
		// �˻� ��
		CGuildSearch* pSearch = NULL;

		if(g_pNk2DFrame->GetUIWindow()->IsOpenWindow(WINDOW_GUILD_SEARCH))
		{
			pSearch = (CGuildSearch*)g_pNk2DFrame->GetUIWindow()->GetWindow(WINDOW_GUILD_SEARCH);
			if(pSearch)
			{
				pSearch->set_start(FALSE);
			}

			if(!g_bSearch) // IDS_GUILD_SERCH_FAIL_GUILD "�˻��� ��尡 �����ϴ�"
				g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_SERCH_FAIL_GUILD), TYPE_GUILD_CONFIRM, 42);

		}


	}
}

// ��� �ػ�
void guild_disband(char *arg, CUIMgr *pMgr)
{
	arg = AnyOneArg(arg, g_guildbuf);

	int nResult = atoi(g_guildbuf);

	switch(nResult)
	{
	case 0 : // ���� // IDS_GUILD_DISERSE_FAIL "��带 �ػ��� �� �����ϴ�. ������ Ȯ�� �ٶ��ϴ�."
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_DISERSE_FAIL), TYPE_GUILD_CONFIRM, 42);
		break;
	case 1 : // ����  /IDS_GUILD_DISERSE_SUC "��尡 �ػ�Ǿ����ϴ�."
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_DISERSE_SUC), TYPE_GUILD_CONFIRM, 42);
		g_pNk2DFrame->GetUIWindow()->CloseTopWindow();
		g_pRoh->DeleteMyGuildData();
		break;
//		case 2 : // �ػ� �˸�
//			g_pNk2DFrame->InsertPopup("��尡 �ػ�˴ϴ�.", TYPE_GUILD_CONFIRM,);
//			break;
	}
}

// ��� â��
void guild_stash(char *arg, CUIMgr *pMgr)
{
	arg = AnyOneArg(arg, g_guildbuf);

	if(!strcmp(g_guildbuf, "n"))
	{
		// ���� ����
		guild_stash_n(arg, pMgr);
	}
	else if(!strcmp(g_guildbuf, "s"))
	{
		// â�� ����
		guild_stash_s(arg, pMgr);
	}
	else if(!strcmp(g_guildbuf, "r"))
	{
		// â�� ���� ����
		guild_stash_r(arg, pMgr);
	}
	else if(!strcmp(g_guildbuf, "e"))
	{
		// â�� ��
		guild_stash_e(arg, pMgr);
	}
	else if(!strcmp(g_guildbuf, "t"))
	{
		// ������ �ֱ� (������ ����) - ����, ���� �ֱ�
		guild_stash_t(arg, pMgr);
	}
	else if(!strcmp(g_guildbuf, "f"))
	{
		// ������ ���� (������ ����) - ����, ���� ����
		guild_stash_f(arg, pMgr);
	}
	else if(!strcmp(g_guildbuf, "p"))
	{
		// ������ �ֱ� (������ ����) - ���� �ֱ�
		guild_stash_p(arg, pMgr);
	}
	else if(!strcmp(g_guildbuf, "g"))
	{
		// ������ ���� (������ ����) - ���� ����
		guild_stash_g(arg, pMgr);
	}
}

void guild_stash_n(char *arg, CUIMgr *pMgr)
{
	// IDS_GUILD_MSG_01 â�� �����...
	g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_GUILD_MSG_01), TYPE_NOR_OK, 1 );
}

void guild_stash_s(char *arg, CUIMgr *pMgr)
{
	// â�� ���� �˸�

}

void guild_stash_r(char *arg, CUIMgr *pMgr)
{
	if( !pMgr )	return;

	arg = AnyOneArg(arg, g_guildbuf);
	g_guildii[0] = atoi(g_guildbuf);	// ���Թ�ȣ

	arg = AnyOneArg(arg, g_guildbuf);	// vnum
	g_guildii[1] = atoi(g_guildbuf);

	arg = AnyOneArg(arg, g_guildbuf);	// item_plus
	g_guildii[2] = atoi(g_guildbuf);

	arg = AnyOneArg(arg, g_guildbuf);	// item_flag
	g_guildii[3] = atoi(g_guildbuf);

	///�Ź���ȭ�ý���
	arg = AnyOneArg(arg, g_guildbuf);
	g_guildii[6] = atoi(g_guildbuf);	// �߰� �ɼ� m_Special2.
	arg = AnyOneArg(arg, g_guildbuf);
	g_guildii[7] = atoi(g_guildbuf);	// ���� ��ȭ ������ ��ġ.
	arg = AnyOneArg(arg, g_guildbuf);
	g_guildii[8] = atoi(g_guildbuf);	// �ƽ� ��ȭ ������.

	if( g_SvrType == ST_ADULT_ONLY)
	{
		arg = AnyOneArg(arg, g_guildbuf);
		g_guildii[9] = atoi(g_guildbuf);	// ���� ��ȭ ������ ��ġ.
		arg = AnyOneArg(arg, g_guildbuf);
		g_guildii[10] = atoi(g_guildbuf);	// �ƽ� ��ȭ ������.
	}
	arg = AnyOneArg(arg, g_guildbuf);	// count
	g_guildii[4] = atoi(g_guildbuf);

	//arg = AnyOneArg(arg, g_guildbuf);
	//g_guildii[5] = atoi(g_guildbuf);
	g_guildii[5] = -1;

	if( g_SvrType == ST_ADULT_ONLY)
	{
		if( pMgr )
			pMgr->guild_Stash(g_guildii[0], g_guildii[1], g_guildii[2], g_guildii[3], g_guildii[6],
							  g_guildii[7], g_guildii[8], g_guildii[9], g_guildii[10],g_guildii[4], g_guildii[5] );
	}
	else
	{
		if( pMgr )
			pMgr->guild_Stash(g_guildii[0], g_guildii[1], g_guildii[2], g_guildii[3], g_guildii[6],
							  g_guildii[7], g_guildii[8], 0, 0,g_guildii[4], g_guildii[5] );
	}
}

void guild_stash_e(char *arg, CUIMgr *pMgr) // end
{
	pMgr->guild_StashEnd();
}

void guild_stash_t(char *arg, CUIMgr *pMgr) // to
{
	if( !pMgr )	return;

	arg = AnyOneArg(arg, g_guildbuf);
	g_guildii[0] = atoi(g_guildbuf);	// slot

	arg = AnyOneArg(arg, g_guildbuf);
	g_guildii[1] = atoi(g_guildbuf);	// vnum

	arg = AnyOneArg(arg, g_guildbuf);
	g_guildii[2] = atoi(g_guildbuf);	// plus

	arg = AnyOneArg(arg, g_guildbuf);
	g_guildii[3] = atoi(g_guildbuf);	// flag1

	arg = AnyOneArg(arg, g_guildbuf);
	g_guildii[4] = atoi(g_guildbuf);	// flag2

	arg = AnyOneArg(arg, g_guildbuf);
	g_guildii[5] = atoi(g_guildbuf);	// endu1

	arg = AnyOneArg(arg, g_guildbuf);
	g_guildii[6] = atoi(g_guildbuf);	// endu2

	if( g_SvrType == ST_ADULT_ONLY)
	{
		arg = AnyOneArg(arg, g_guildbuf);
		g_guildii[9] = atoi(g_guildbuf);	// �Ϲ� ������

		arg = AnyOneArg(arg, g_guildbuf);
		g_guildii[10] = atoi(g_guildbuf);	// �Ϲ� ������
	}

	arg = AnyOneArg(arg, g_guildbuf);
	g_guildii[7] = atoi(g_guildbuf);	// count

	arg = AnyOneArg(arg, g_guildbuf);
	g_guildii[8] = atoi(g_guildbuf);	// swap

	if( g_SvrType == ST_ADULT_ONLY)
	{
		pMgr->guild_StashTo(g_guildii[0], g_guildii[1], g_guildii[2], g_guildii[3], g_guildii[4],
							g_guildii[5], g_guildii[6], g_guildii[9], g_guildii[10], g_guildii[7], g_guildii[8] );
	}
	else
	{
		pMgr->guild_StashTo(g_guildii[0], g_guildii[1], g_guildii[2], g_guildii[3], g_guildii[4],
							g_guildii[5], g_guildii[6], 0, 0, g_guildii[7], g_guildii[8] );
	}
}

void guild_stash_f(char *arg, CUIMgr *pMgr) // from
{
	if( !pMgr )	return;

	arg = AnyOneArg(arg, g_guildbuf);
	g_guildii[0] = atoi(g_guildbuf);	// slot

	arg = AnyOneArg(arg, g_guildbuf);
	g_guildii[1] = atoi(g_guildbuf);	// index

	arg = AnyOneArg(arg, g_guildbuf);
	g_guildii[2] = atoi(g_guildbuf);	// vnum

	arg = AnyOneArg(arg, g_guildbuf);
	g_guildii[3] = atoi(g_guildbuf);	// plus

	arg = AnyOneArg(arg, g_guildbuf);
	g_guildii[4] = atoi(g_guildbuf);	// flag1

	arg = AnyOneArg(arg, g_guildbuf);
	g_guildii[5] = atoi(g_guildbuf);	// flag2

	arg = AnyOneArg(arg, g_guildbuf);
	g_guildii[6] = atoi(g_guildbuf);	// endu1

	arg = AnyOneArg(arg, g_guildbuf);
	g_guildii[7] = atoi(g_guildbuf);	// endu2

	if( g_SvrType == ST_ADULT_ONLY)
	{
		arg = AnyOneArg(arg, g_guildbuf);
		g_guildii[9] = atoi(g_guildbuf);	// endu1
		arg = AnyOneArg(arg, g_guildbuf);
		g_guildii[10] = atoi(g_guildbuf);	// endu2
	}

	arg = AnyOneArg(arg, g_guildbuf);
	g_guildii[8] = atoi(g_guildbuf);	// count

	if( g_SvrType == ST_ADULT_ONLY)
	{
		pMgr->guild_StashFrom(g_guildii[0], g_guildii[1], g_guildii[2], g_guildii[3], g_guildii[4],
							  g_guildii[5], g_guildii[6], g_guildii[7], g_guildii[9], g_guildii[10], g_guildii[8] );
	}
	else
	{
		pMgr->guild_StashFrom(g_guildii[0], g_guildii[1], g_guildii[2], g_guildii[3], g_guildii[4],
							  g_guildii[5], g_guildii[6], g_guildii[7], 0, 0, g_guildii[8] );
	}
}

void guild_stash_p(char *arg, CUIMgr *pMgr) // put
{
	if( !pMgr ) return;

	arg = AnyOneArg( arg, g_guildbuf );

	g_guildii[0] = atoi( g_guildbuf );	// ���� ���� Ȯ�� �޼���

	if( g_guildii[0] == 1 )
	{
		arg = AnyOneArg( arg,g_guildbuf );
		g_guildii[ 1 ] = atoi( g_guildbuf );		// ������ ��ȣ
		arg = AnyOneArg( arg, g_guildbuf );
		g_guildii[ 2 ] = atoi( g_guildbuf );		// �κ� �� ��ȣ
		arg = AnyOneArg( arg, g_guildbuf );
		g_guildii[ 3 ] = atoi( g_guildbuf );		// â�� �� ��ȣ
		arg = AnyOneArg( arg, g_guildbuf );
		g_guildii[ 4 ] = atoi( g_guildbuf );		// â�� ���� ��ȣ
		arg = AnyOneArg( arg, g_guildbuf );
		g_guildii[ 5 ] = atoi( g_guildbuf );		// ������ ����

		for( int i = 0 ; i < g_guildii[ 5 ]  ; i ++ )
		{
			arg = AnyOneArg( arg, g_guildbuf );		// X ��ǥ
			g_guildii[ 6 ] = atoi( g_guildbuf );
			arg = AnyOneArg( arg, g_guildbuf );		// Y ��ǥ
			g_guildii[ 7 ] = atoi( g_guildbuf );

			pMgr->guild_StashPut( g_guildii[ 1 ], g_guildii[ 2 ], g_guildii[ 3 ], g_guildii[ 4 ], g_guildii[ 6 ], g_guildii[ 7 ] );
		}
	}

}

void guild_stash_g(char *arg, CUIMgr *pMgr) // get
{
	if( !pMgr )
		return;

	arg = AnyOneArg( arg, g_guildbuf );

	g_guildii[ 0 ] = atoi( g_guildbuf );	// ���� ���� Ȯ�� �޼���
	if( g_guildii[ 0 ] == 1 )
	{
		arg = AnyOneArg( arg, g_guildbuf );
		g_guildii[ 1 ] = atoi( g_guildbuf );		// ������ ��ȣ
		arg = AnyOneArg( arg, g_guildbuf );
		g_guildii[ 2 ] = atoi( g_guildbuf );		// �κ� �� ��ȣ
		arg = AnyOneArg( arg, g_guildbuf );
		g_guildii[ 3 ] = atoi( g_guildbuf );		// â�� �� ��ȣ
		arg = AnyOneArg( arg, g_guildbuf );
		g_guildii[ 4 ] = atoi( g_guildbuf );		// â�� ���� ��ȣ
		arg = AnyOneArg( arg, g_guildbuf );
		g_guildii[ 5 ] = atoi( g_guildbuf );		// ������ ī��Ʈ

		for( int i = 0 ; i < g_guildii[ 5 ]  ; i ++ )
		{
			arg = AnyOneArg( arg, g_guildbuf );		// Item Index
			g_guildii[ 6 ] = atoi( g_guildbuf );
			arg = AnyOneArg( arg, g_guildbuf );		// X ��ǥ
			g_guildii[ 7 ] = atoi( g_guildbuf );
			arg = AnyOneArg( arg, g_guildbuf );		// Y ��ǥ
			g_guildii[ 8 ] = atoi( g_guildbuf );
			pMgr->guild_StashGet( g_guildii[ 1 ], g_guildii[ 2 ], g_guildii[ 3 ], g_guildii[ 4 ],
								  g_guildii[ 6 ], g_guildii[ 7 ], g_guildii[ 8 ] );
		}
	}
}

// ��� Ż��
void guild_leave(char *arg, CUIMgr *pMgr)
{
	arg = AnyOneArg(arg, g_guildbuf);

	// ��带 Ż�� �Ѵ�. success or fail
	int nResult = atoi(g_guildbuf);

	switch( nResult )
	{
	case -3:		
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_MG_LEAVE_ERR_3), TYPE_GUILD_CONFIRM, 42);
		break;
	case -2: // ���� �Һи�
		//g_pNk2DFrame->InsertPopup("Ż���ϴµ� �����Ͽ����ϴ�.", TYPE_GUILD_CONFIRM, 42);
		break;

	case -1: // IDS_GUILD_MEMBER_OUT_DONT_MASTER "��� �����ʹ� Ż���� �� �����ϴ�."
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_MEMBER_OUT_DONT_MASTER), TYPE_GUILD_CONFIRM, 42);
		break;

	case 0: // IDS_GUILD_MEMBER_OUT_FAIL_LAIM "������ �����մϴ�. ������ Ȯ���Ͻñ� �ٶ��ϴ�."
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_MEMBER_OUT_FAIL_LAIM), TYPE_GUILD_CONFIRM, 42);
		break;

	case 1: // IDS_GUILD_MEMBER_OUT_SUC "��� Ż�� ó���� �Ǿ����ϴ�."
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_MEMBER_OUT_SUC), TYPE_GUILD_CONFIRM, 42);
		g_pRoh->DeleteMyGuildData();
		g_pNk2DFrame->GetUIWindow()->CloseTopWindow();
		break;
	}
}

// ��� �±�
void guild_upgrade(char *arg, CUIMgr *pMgr)
{
	arg = AnyOneArg(arg, g_guildbuf);

	// �±� success or fail
	int nResult = atoi(g_guildbuf);

	switch( nResult )
	{
	case 0:	// IDS_GUILD_LVLUP_FAIL "�±� ���ǿ� ���� �ʽ��ϴ�. �±� ������ Ȯ�� �ٶ��ϴ�."
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_LVLUP_FAIL), TYPE_GUILD_CONFIRM);
		break;

	case 1: // IDS_GUILD_LVLUP_SUC "��� ������ �Ѵܰ� �±޵Ǿ����ϴ�."
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_LVLUP_SUC), TYPE_GUILD_CONFIRM);
		g_pNk2DFrame->GetUIWindow()->OnOpenWindow(WINDOW_GUILD_MEMBER);
		break;
	}
}

// ��� ����

// ��õ����
void guild_recom(char *arg, CUIMgr *pMgr)
{
	arg = AnyOneArg(arg, g_guildbuf);

	if(!strcmp(g_guildbuf , "r"))
	{
		// ��� ��õ ��û ���
		guild_recom_r(arg);
	}
	else if(!strcmp(g_guildbuf, "a"))
	{
		// ��� ��û �¶� ���
		guild_recom_a(arg);
	}
}

void guild_recom_r(char *arg)
{
	arg = AnyOneArg(arg, g_guildbuf);

	int nResult = atoi(g_guildbuf);

	switch(nResult)
	{
	case 0 : // IDS_GUILD_RECOM_FAIL_SERCH_CHA "��õ�� ĳ���͸� ã�� �� �����ϴ�."
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_RECOM_FAIL_SERCH_CHA), TYPE_GUILD_CONFIRM, POPUP_GUILD_NORMAL);
		break;
	case 1 : // IDS_GUILD_RECOM_SUC "��� ���� ��õ�� �Ǿ����ϴ�."
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_RECOM_SUC), TYPE_GUILD_CONFIRM, POPUP_GUILD_NORMAL);
		break;
	case 2 : // IDS_GUILD_RECOM_FAIL_GMEMBER "��忡 ���Ե� ĳ�����Դϴ�."
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_RECOM_FAIL_GMEMBER), TYPE_GUILD_CONFIRM, POPUP_GUILD_NORMAL);
		break;
	case 3 : // IDS_GUILD_RECOM_FAIL_MEMBER_COUNT "��� ���� ��� �ο��� �ʰ��Ǿ� ��õ�� �� �����ϴ�."
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_RECOM_FAIL_MEMBER_COUNT), TYPE_GUILD_CONFIRM, POPUP_GUILD_NORMAL);
		break;
	case 4 : // IDS_GUILD_RECOM_FAIL_LVL "50���� �̸��� ��尡�� ��õ�� �� �� �����ϴ�."
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_RECOM_FAIL_LVL), TYPE_GUILD_CONFIRM, POPUP_GUILD_NORMAL);
		break;
	case 5 : // IDS_GUILD_RECOM_FAIL "��� ������ �Ұ��� �մϴ�."
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_RECOM_FAIL), TYPE_GUILD_CONFIRM, POPUP_GUILD_NORMAL);
		break;
	}
}

void guild_recom_a(char *arg)
{
	// ��õ�� ����, ĳ���͸� ������ �޴´�..
	arg  = AnyOneArg(arg, g_guildbuf);
	arg  = AnyOneArg(arg, g_guildbuf1);

	char szTmp[256] = {0,};

	// IDS_GUILD_RECOM_TO "%'%s%' ���� %s%���� ��õ �Ͽ����ϴ�."
	sprintf(szTmp, (char*)G_STRING(IDS_GUILD_RECOM_TO), g_guildbuf, g_guildbuf1);

	g_pNk2DFrame->GetGuildSystemManager()->set_guilds_name(g_guildbuf);
	g_pNk2DFrame->GetGuildSystemManager()->set_guilds_recomname(g_guildbuf1);

	g_pNk2DFrame->InsertPopup(szTmp, TYPE_GUILD_CONFIRM_CANCEL, POPUP_GUILD_RECOMMAND);
}

// �Ϲݰ���
void guild_request(char *arg, CUIMgr *pMgr)
{
	arg = AnyOneArg(arg, g_guildbuf);

	// ���Խ�û ���
	int nResult = atoi(g_guildbuf);

	switch(nResult)
	{
	case 0 : // ã�� ����
		break;
	case 1 : // IDS_GUILD_RECOM_REG_SUC			"��� ���� ��û�� �����Ǿ����ϴ�."
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_RECOM_REG_SUC), TYPE_GUILD_CONFIRM, 42 );
		break;
	case 2 : // IDS_GUILD_RECOM_REG_FAIL		"��� ���� ��û�� �Ǵ� �����ڴ� ��û�� �� �����ϴ�."
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_RECOM_REG_FAIL), TYPE_GUILD_CONFIRM, 42 );
		break;
	case 3 : // IDS_GUILD_JOIN_FAIL_MEMBER "����ο� �ʰ��� ���� ��û�� �� �� �����ϴ�."
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_RECOM_FAIL_MEMBER), TYPE_GUILD_CONFIRM, 42 );
		break;
	case 4 : // IDS_GUILD_RECOM_FAIL_LVL "50���� �̸��� ��尡�� ��õ�� �� �� �����ϴ�."	
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_RECOM_FAIL_LVL), TYPE_GUILD_CONFIRM, 42);
		break;
	case 5 : // IDS_GUILD_RECOM_FAIL "��� ������ �Ұ��� �մϴ�."
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_RECOM_FAIL), TYPE_GUILD_CONFIRM, 42);
		break;
	}
}

// ��� ���� >> ��޸� ����
void guild_nick(char *arg, CUIMgr *pMgr)
{
	arg = AnyOneArg(arg, g_guildbuf);

	// ���� ���� �� ���� ����

	switch(atoi(g_guildbuf))
	{
	case 0 : // // IDS_GUILD_JOBNAME_FAIL "��޸� ���濡 �����Ͽ����ϴ�."
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_JOBNAME_FAIL), TYPE_GUILD_CONFIRM);
		break;
	case 1 : // ����
		{
			CGuildData* pGuildData = g_pRoh->GetMyGuildData();

			arg = AnyOneArg(arg, g_guildbuf1); // ����
			int nlevel = atoi(g_guildbuf1);

			arg = AnyOneArg(arg, g_guildbuf2); // ��޸�

			char oldGNmae[128]= {0};
			char msg[256]= {0};
			memcpy(oldGNmae,pGuildData->GetMyGuildInfo().s_guildjobname[nlevel], strlen(pGuildData->GetMyGuildInfo().s_guildjobname[nlevel]) );

			// ���� ��� ������ ����
			pGuildData->Change_guildgradename(g_guildbuf2, nlevel);

			// ����â�� ������ ����
			g_pNk2DFrame->GetControlGuildManage()->ResetGrade(g_guildbuf2, nlevel);
			// IDS_GUILD_JOBNAME_CHANGE_SUC "%'%s%'��޸��� %'%s%'(��)�� ����Ǿ����ϴ�."
			sprintf( msg, (char*)G_STRING(IDS_GUILD_JOBNAME_CHANGE_SUC),oldGNmae, g_guildbuf2 );
			g_pNk2DFrame->InsertPopup(msg, TYPE_GUILD_CONFIRM);
		}
		break;
	}

}

// ��� ���� >> ���� ����
void guild_notice(char *arg, CUIMgr *pMgr)
{
	arg = AnyOneArg(arg, g_guildbuf);

	switch(atoi(g_guildbuf))
	{
	case 0 :	// IDS_GUILD_NOTICE_MODIFY_FAIL "���� ������ ���� �߽��ϴ�."
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_NOTICE_MODIFY_FAIL), TYPE_GUILD_CONFIRM, 42);
		break;
	case 1 :	// ����
		{
			// IDS_GUILD_NOTICE_MODIFY_SUC "��� ������ ���� �Ǿ����ϴ�."
			g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_NOTICE_MODIFY_SUC), TYPE_GUILD_CONFIRM, 42);
		}
		break;
	}
}

// ��� ���� >> ���� �¶�
void guild_allow(char *arg, CUIMgr *pMgr)
{
	arg = AnyOneArg(arg, g_guildbuf);

	switch(atoi(g_guildbuf))
	{
	case 0 : // IDS_GUILD_JOIN_FAIL	"��� ���� ��û�� ���ε��� �ʾҽ��ϴ�"
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_JOIN_FAIL), TYPE_GUILD_CONFIRM, 42);
		break;
	case 1 : // IDS_GUILD_JOIN_SUC "��� ���� ��û�� ���εǾ����ϴ�."
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_JOIN_SUC), TYPE_GUILD_CONFIRM, 42);
		break;
	case 2 : // IDS_GUILD_JOIN_FAIL	"��� ���� ��û�� ���ε��� �ʾҽ��ϴ�"
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_JOIN_FAIL), TYPE_GUILD_CONFIRM, 42);
		break;
	}
}

// ��� ���� >> ��å����
void guild_appoint(char *arg, CUIMgr *pMgr)
{
	arg = AnyOneArg(arg, g_guildbuf);

	switch(atoi(g_guildbuf))
	{
	case 0 : // IDS_GUILD_JOB_FAIL "�Ӹ� �����Ͽ����ϴ�."
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_JOB_FAIL), TYPE_GUILD_CONFIRM, 42);
		break;
	case 1 :  // IDS_GUILD_JOB_SUC �Ӹ�Ǿ����ϴ�."
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_JOB_SUC), TYPE_GUILD_CONFIRM, 42);
		break;
	}
}

// ��� ���� >> �������
void guild_fire(char *arg, CUIMgr *pMgr)
{
	// success or fail
	arg = AnyOneArg(arg, g_guildbuf);

	switch(atoi(g_guildbuf))
	{
	case 0 :  // IDS_GUILD_MEMBER_FIRE_FAIL	 "��� ���⿡ �����Ͽ����ϴ�."
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_MEMBER_FIRE_FAIL), TYPE_GUILD_CONFIRM, 42);
		break;
	case 1 :
		{
			// IDS_GUILD_MEMBER_FIRE_SUC "��� ���⿡ �����߽��ϴ�."
			g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_MEMBER_FIRE_SUC), TYPE_GUILD_CONFIRM, 42);
			g_pNk2DFrame->GetControlGuildManage()->ResetMember();
		}
		break;
	case 2 :
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_MG_FIRE_ERR_2), TYPE_GUILD_CONFIRM, 42);
		break;
	}
}

// ��� ���� >> �������
void guild_fired(char *arg, CUIMgr *pMgr)
{
	// ����� ĳ�� ����
	// IDS_GUILD_MEMBER_FIRE "��忡�� ���� �Ǿ����ϴ�. "
	g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_MEMBER_FIRE), TYPE_GUILD_CONFIRM, 42);
	g_pRoh->DeleteMyGuildData();
	if( pCMyApp->GetCurWorld() == WORLD_DMITRON_BATTLE )
	{
		LH_SEND_NET_MSG( "go_world 9 0\n" );
	}
}

// �������� >> ���
void guild_ginfo(char *arg, CUIMgr *pMgr)
{
	if(g_pRoh == NULL) return;

	arg = AnyOneArg(arg, g_guildbuf);

	int add_or_update = atoi(g_guildbuf);		// 0:�߰�, 1:����

	if(add_or_update == 0)
	{
		g_pRoh->MakeMyGuildData();
	}

	CGuildData* pGuildData = g_pRoh->GetMyGuildData();

	if(pGuildData == NULL)	return;

	guild_Info	tmpInfo;

	arg = AnyOneArg(arg, g_guildbuf);

	tmpInfo.s_guildIndex = atoi(g_guildbuf);	// ��� �ε���

	arg = AnyOneArg(arg, g_guildbuf);

	strcpy(tmpInfo.s_guildName, g_guildbuf);	// ����

	arg = AnyOneArg(arg, g_guildbuf);

	tmpInfo.s_rank = atoi(g_guildbuf);			// ��巩ŷ

	arg = AnyOneArg(arg, g_guildbuf);

	tmpInfo.s_glevel = atoi(g_guildbuf);		// ��巹��  ( ���� ������ ȥ�� ���� ���� )

	arg = AnyOneArg(arg, g_guildbuf);

	tmpInfo.s_gPoint = atoi(g_guildbuf);		// �������

	arg = AnyOneArg(arg, g_guildbuf);

	strcpy(tmpInfo.s_guildjobname[JOB_MASTER], g_guildbuf);	// ��� ������

	arg = AnyOneArg(arg, g_guildbuf);

	strcpy(tmpInfo.s_guildjobname[JOB_ASSISTANTMASTER], g_guildbuf);	// ��� ������

	arg = AnyOneArg(arg, g_guildbuf);

	strcpy(tmpInfo.s_guildjobname[JOB_VULCAN], g_guildbuf);	// ��ĭ����

	arg = AnyOneArg(arg, g_guildbuf);

	strcpy(tmpInfo.s_guildjobname[JOB_KAI], g_guildbuf);	// ī�̺���

	arg = AnyOneArg(arg, g_guildbuf);

	strcpy(tmpInfo.s_guildjobname[JOB_EIDA], g_guildbuf);	// ���̴ٺ���

	arg = AnyOneArg(arg, g_guildbuf);;

	strcpy(tmpInfo.s_guildjobname[JOB_HUMAN], g_guildbuf);	// �޸պ���

	arg = AnyOneArg(arg, g_guildbuf);

	strcpy(tmpInfo.s_guildjobname[JOB_HIV], g_guildbuf);	// ���̺���

	arg = AnyOneArg(arg, g_guildbuf);

	strcpy(tmpInfo.s_guildjobname[JOB_GENERIC], g_guildbuf); // �Ϲݱ�� ��Ī

	arg = AnyOneArg(arg, g_guildbuf);

	strcpy(tmpInfo.s_guildjobname[JOB_PEROM], g_guildbuf); // �Ϲݱ�� ��Ī

	arg = AnyOneArg(arg, g_guildbuf);

	tmpInfo.s_revisiontime.nYear = (time_t)atol(g_guildbuf); // ����϶��� �����ð�

	arg = AnyOneArg(arg, g_guildbuf);

	tmpInfo.s_revisiontime.nMon = (time_t)atol(g_guildbuf);

	arg = AnyOneArg(arg, g_guildbuf);

	tmpInfo.s_revisiontime.nDay = (time_t)atol(g_guildbuf);

	arg = AnyOneArg(arg, g_guildbuf);

	tmpInfo.s_revisiontime.nHour = (time_t)atol(g_guildbuf);

	arg = AnyOneArg(arg, g_guildbuf);

	tmpInfo.s_revisiontime.nMin = (time_t)atol(g_guildbuf);

	arg = AnyOneArg(arg, g_guildbuf);

	tmpInfo.s_revisiontime.nSec = (time_t)atol(g_guildbuf);

	if(pGuildData)
		pGuildData->Insert_guildInfo(tmpInfo);

	if( g_pNk2DFrame->GetControlGuild() )
		g_pNk2DFrame->GetControlGuild()->SetGuildInfo();
}

// �������� >> ����
void guild_info(char *arg, CUIMgr *pMgr)
{

	guild_member tmpMember;

	arg = AnyOneArg(arg, g_guildbuf);

	int nRefresh = atoi(g_guildbuf);		// 0 : �߰�(�α���) 1 : ����

	if(nRefresh == GUILD_INFO_ADD)
	{
		guild_info_add(arg, pMgr);
	}
	else if(nRefresh == GUILD_INFO_UPDATE)
	{
		guild_info_update(arg, pMgr);
	}

}

void guild_info_add(char *arg, CUIMgr *pMgr)
{
	CGuildData* pGuildData = g_pRoh->GetMyGuildData();

	if(pGuildData == NULL)	return;

	guild_member tmpMember;

	arg = AnyOneArg(arg, g_guildbuf);

	tmpMember.s_index = atoi(g_guildbuf);	// ĳ�� �ε���

	arg = AnyOneArg(arg, g_guildbuf);

	tmpMember.s_glevel = atoi(g_guildbuf);  // ��å ����

	arg = AnyOneArg(arg, g_guildbuf);

	strcpy(tmpMember.s_name, g_guildbuf);   // ĳ����

	arg = AnyOneArg(arg, g_guildbuf);

	tmpMember.s_race = atoi(g_guildbuf);	// ����

	arg = AnyOneArg(arg, g_guildbuf);

	tmpMember.s_connect = atoi(g_guildbuf); // ���� ����

	arg = AnyOneArg(arg, g_guildbuf);

	tmpMember.s_jtype = atoi(g_guildbuf);	// ��������

	arg = AnyOneArg(arg, g_guildbuf);

	tmpMember.s_waittime.nYear = atol(g_guildbuf); // ���Դ���� �ð�

	arg = AnyOneArg(arg, g_guildbuf);

	tmpMember.s_waittime.nMon = atol(g_guildbuf);

	arg = AnyOneArg(arg, g_guildbuf);

	tmpMember.s_waittime.nDay = atol(g_guildbuf);

	arg = AnyOneArg(arg, g_guildbuf);

	tmpMember.s_waittime.nHour = atol(g_guildbuf);

	arg = AnyOneArg(arg, g_guildbuf);

	tmpMember.s_waittime.nMin = atol(g_guildbuf);

	arg = AnyOneArg(arg, g_guildbuf);

	tmpMember.s_waittime.nSec = atol(g_guildbuf);


	if(g_pRoh->m_nCharIndex == tmpMember.s_index)
		pGuildData->Insert_guildMyInfo(tmpMember);

	if(tmpMember.s_glevel == 0 && tmpMember.s_jtype == 1)	// ��õ ���� �����
		tmpMember.s_check = CHECK_RECOMMAND_WAIT_ADD;
	else if(tmpMember.s_glevel == 0 && tmpMember.s_jtype == 2)	// �Ϲ� ���� �����
		tmpMember.s_check = CHECK_GENERAL_WAIT_ADD;
	else
		tmpMember.s_check = CHECK_ADD;						// �����....

	if(pGuildData)
		pGuildData->Insert_guilddata(tmpMember);

}

void guild_info_update(char *arg, CUIMgr *pMgr)
{
	CGuildData* pGuildData = g_pRoh->GetMyGuildData();

	if(pGuildData == NULL)	return;

	guild_member tmpMember;

	arg = AnyOneArg(arg, g_guildbuf);

	tmpMember.s_index = atoi(g_guildbuf);	// ĳ�� �ε���

	arg = AnyOneArg(arg, g_guildbuf);

	tmpMember.s_glevel = atoi(g_guildbuf);  // ��å ����

	arg = AnyOneArg(arg, g_guildbuf);

	strcpy(tmpMember.s_name, g_guildbuf);   // ĳ����

	arg = AnyOneArg(arg, g_guildbuf);

	tmpMember.s_race = atoi(g_guildbuf);	// ����

	arg = AnyOneArg(arg, g_guildbuf);

	tmpMember.s_connect = atoi(g_guildbuf); // ���� ����

	arg = AnyOneArg(arg, g_guildbuf);

	tmpMember.s_jtype = atoi(g_guildbuf);	// ��������

	arg = AnyOneArg(arg, g_guildbuf);

	tmpMember.s_waittime.nYear = atoi(g_guildbuf); // ���Դ���� �ð�

	arg = AnyOneArg(arg, g_guildbuf);

	tmpMember.s_waittime.nMon = atoi(g_guildbuf);

	arg = AnyOneArg(arg, g_guildbuf);

	tmpMember.s_waittime.nDay = atoi(g_guildbuf);

	arg = AnyOneArg(arg, g_guildbuf);

	tmpMember.s_waittime.nHour = atoi(g_guildbuf);

	arg = AnyOneArg(arg, g_guildbuf);

	tmpMember.s_waittime.nMin = atoi(g_guildbuf);

	arg = AnyOneArg(arg, g_guildbuf);

	tmpMember.s_waittime.nSec = atoi(g_guildbuf);

	int gLevelBack =  g_pRoh->GetMyGuildData()->GetGLevel( tmpMember.s_index );

	if(g_pRoh->m_nCharIndex == tmpMember.s_index)
	{
		int glev = g_pRoh->GetMyGuildLevel();

		pGuildData->Insert_guildMyInfo(tmpMember);
//
//		if( (glev == 10 || glev == 9) && tmpMember.s_glevel < 9 )
		{
			if( g_pNk2DFrame->IsControlGuild() ||
					g_pNk2DFrame->IsControlGuildManage() )
			{
				g_pNk2DFrame->ShowControlGuild(true);
			}
			else if( g_pNk2DFrame->IsControlGuildManage() )
			{
				if( tmpMember.s_glevel < 9 )
				{
					g_pNk2DFrame->ShowControlGuildManage(false);
					g_pNk2DFrame->ShowControlGuild(true);
				}
				if( tmpMember.s_glevel == 10 || tmpMember.s_glevel == 9 )
					g_pNk2DFrame->ShowControlGuildManage(true);
			}
		}
	}

	if(tmpMember.s_glevel == -1)
	{
		tmpMember.s_check = CHECK_DELETE;
	}
	else if(tmpMember.s_glevel != -1 && tmpMember.s_glevel != 0)
	{
		if(pGuildData->IsWaitlist(tmpMember, 1))	// ��õ
		{
			pGuildData->Delete_RecommandWait(tmpMember);
			tmpMember.s_check = CHECK_ADD;
		}
		else if(pGuildData->IsWaitlist(tmpMember, 2))	// �Ϲ�
		{
			pGuildData->Delete_GeneralWait(tmpMember);
			tmpMember.s_check = CHECK_ADD;
		}
		else
		{
			tmpMember.s_check = CHECK_UPDATE;
		}
	}

	if(pGuildData)
	{
		pGuildData->Insert_guilddata(tmpMember);
//		if(tmpMember.s_glevel == -1 )
//		{
//			g_pNk2DFrame->GetControlGuildManage()->m_pJoinWait->Insert_list();
//		}
	}


	if( gLevelBack == -1 )
		return;

	if( gLevelBack == 0  && tmpMember.s_glevel > 0 )
		g_pNk2DFrame->GetControlGuildManage()->ResetJoinRequest(tmpMember.s_index );

}
// �������� >> ����
void guild_ninfo(char *arg, CUIMgr *pMgr)
{
	CGuildData* pGuildData = g_pRoh->GetMyGuildData();

	if(pGuildData == NULL)	return;

	guild_alert tmpNotice;

	//arg = AnyOneArg(arg, g_guildbuf);

	SkipSpaces(&arg);

	strcpy(tmpNotice.s_guildnotic, arg);

	if(pGuildData)
	{
		if( strlen(tmpNotice.s_guildnotic) > 0 )
		{
			pGuildData->Insert_guildnotice(tmpNotice);
			g_pNk2DFrame->GetControlGuildManage()->m_pNoticeInfo->SetNotice(tmpNotice.s_guildnotic);
		}
	}
}

// �������� >> ��å����
void guild_ainfo(char *arg, CUIMgr *pMgr)
{
	CGuildData* pGuildData = g_pRoh->GetMyGuildData();

	if(pGuildData == NULL)	return;

	arg = AnyOneArg(arg, g_guildbuf);

	int nLevel = atoi(g_guildbuf);		// ��å

	arg = AnyOneArg(arg, g_guildbuf);

	int nCharIdx = atoi(g_guildbuf1);	// ĳ�� �ε���

	CJobChangeControl* pJob =  g_pNk2DFrame->GetControlGuildManage()->m_pJob;

	if(pJob == 0)	return;

	guild_member* mem_ber = pGuildData->SearchMember_charIdx(nCharIdx);

	if(mem_ber)
	{
		mem_ber->s_glevel = nLevel;
		pJob->Insert_list();
	}

}

// �������� >> ��� ��
void guild_up(char *arg, CUIMgr *pMgr)
{
	/*
	arg = AnyOneArg(arg, g_guildbuf);

	int g_level = atoi(g_guildbuf);

	CGuildData* pGuildData = g_pRoh->GetMyGuildData();

	if(pGuildData == NULL)	return;

	pGuildData->Change_guildLevel(g_level);

	g_pNk2DFrame->InsertPopup("��� �� �Ǿ����ϴ�.", TYPE_GUILD_CONFIRM, 42);
	*/
}

// �������� >> ��� �ٿ�
void guild_down(char *arg, CUIMgr *pMgr)
{
	/*
		arg = AnyOneArg(arg, g_guildbuf);

		int g_level = atoi(g_guildbuf);

		CGuildData* pGuildData = g_pRoh->GetMyGuildData();

		if(pGuildData == NULL)	return;

		pGuildData->Change_guildLevel(g_level);

		g_pNk2DFrame->InsertPopup("����� �϶� �Ǿ����ϴ�.", TYPE_GUILD_CONFIRM, 42);
	*/
}

//ħ�� ����
void guild_warning(char *arg, CUIMgr *pMgr)
{
	t_Info	time;

	arg = AnyOneArg(arg, g_guildbuf);
	time.nYear = atol(g_guildbuf);

	arg = AnyOneArg(arg, g_guildbuf);
	time.nMon = atol(g_guildbuf);

	arg = AnyOneArg(arg, g_guildbuf);
	time.nDay = atol(g_guildbuf);

	arg = AnyOneArg(arg, g_guildbuf);
	time.nHour = atol(g_guildbuf);

	arg = AnyOneArg(arg, g_guildbuf);
	time.nMin = atol(g_guildbuf);

	arg = AnyOneArg(arg, g_guildbuf);
	time.nSec = atol(g_guildbuf);

	if( time.nYear	== -1 &&
			time.nMon	== -1 &&
			time.nDay	== -1 &&
			time.nHour	== -1 &&
			time.nMin	== -1 &&
			time.nSec	== -1 )
		return;

	char szTmp[256] = {0,};

	if( time.nYear != -1 )
	{
		// IDS_GUILD_LEVELDOWN_D_DAY_YEAR	"%02d/%02d/%02d %02d:%02d���� ��� ����� �������� ���� ��� ��� ����� �϶��˴ϴ�."
		sprintf(szTmp, (char*)G_STRING(IDS_GUILD_LEVELDOWN_D_DAY_YEAR), time.nYear, time.nMon, time.nDay, time.nHour, time.nMin);
	}
	else if( time.nMon != -1 )
	{
		// IDS_GUILD_LEVELDOWN_D_DAY_MONTH	"%02d/%02d %02d:%02d���� ��� ����� �������� ���� ��� ��� ����� �϶��˴ϴ�."
		sprintf(szTmp, (char*)G_STRING(IDS_GUILD_LEVELDOWN_D_DAY_MONTH), time.nMon, time.nDay, time.nHour, time.nMin);
	}

	g_pNk2DFrame->InsertPopup(szTmp, TYPE_GUILD_CONFIRM, 42);

	/*
	arg = AnyOneArg(arg, g_guildbuf);

	time_t	time_g = atol(g_guildbuf);

	tm* pTm = NULL;

	pTm = localtime(&time_g);

	if(pTm == NULL)	return;

	char szTmp[256];

	int	nMonth  = 0;
	int nDay    = 0;
	int nHour   = 0;
	int nMinute = 0;

	ZeroMemory(szTmp, sizeof(szTmp));

	nMonth  = pTm->tm_mon + 1;
	nDay    = pTm->tm_mday;
	nHour   = pTm->tm_hour;
	nMinute = pTm->tm_min;

	sprintf(szTmp, "��� ����� �������� ���� ��� %d�� %d�� %d�� %d�п� ��� ����� �϶��˴ϴ�.", nMonth, nDay, nHour, nMinute);

	g_pNk2DFrame->InsertPopup(szTmp, TYPE_GUILD_CONFIRM, 42);
	*/
}

void guild_rankwar(char *arg, CUIMgr *pMgr)
{
	arg = AnyOneArg(arg, g_guildbuf);

	if(!strcmp(g_guildbuf, "info"))
	{
		guild_rankwar_info(arg, pMgr);
	}
	else if(!strcmp(g_guildbuf, "ready"))
	{
		guild_rankwar_ready(arg, pMgr);
	}
	else if(!strcmp(g_guildbuf, "count"))
	{
		guild_rankwar_count(arg, pMgr);
	}
	else if(!strcmp(g_guildbuf, "time"))
	{
		guild_rankwar_time(arg, pMgr);
	}
	else if(!strcmp(g_guildbuf, "open"))
	{
		guild_rankwar_open(arg, pMgr);
	}
	else if(!strcmp(g_guildbuf, "req"))
	{
		guild_rankwar_req(arg, pMgr);
	}
	else if(!strcmp(g_guildbuf, "list"))
	{
		guild_rankwar_list(arg, pMgr);
	}
	else if(!strcmp(g_guildbuf, "invite"))
	{
		guild_rankwar_invite(arg, pMgr);
	}
	else if(!strcmp(g_guildbuf, "start"))
	{
		guild_rankwar_start(arg, pMgr);
	}
	else if(!strcmp(g_guildbuf, "end"))
	{
		guild_rankwar_end(arg, pMgr);
	}
	else if(!strcmp(g_guildbuf, "kcount"))
	{
		guild_rankwar_kcount(arg, pMgr);
	}
	else if(!strcmp(g_guildbuf, "goworld"))
	{
		guild_rankwar_goworld(arg, pMgr);
	}
	else if(!strcmp(g_guildbuf, "cancel"))
	{
		guild_rankwar_cancel(arg, pMgr);
	}
	else if(!strcmp(g_guildbuf, "get"))
	{
		guild_rankwar_get(arg, pMgr);
	}
	else if(!strcmp(g_guildbuf, "comp"))
	{
		guild_rankwar_comp(arg, pMgr);
	}
}

void guild_msg(char *arg, CUIMgr *pMgr)
{
	arg = AnyOneArg(arg, g_guildbuf);

	int nMsg = atoi(g_guildbuf);		// 1:1, �����, ��ֹ�

	switch(nMsg)
	{
	case 1: // IDS_GUILD_JOIN_FAIL_SORRY "��� ���� ��û�� �źεǾ����ϴ�."
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_JOIN_FAIL_SORRY), TYPE_GUILD_CONFIRM, POPUP_OK);
		break;
	case 2: // IDS_GUILD_JOIN_FAIL_TIMEOUT_SORRY "���νð� �ʰ��� ���� ��� ������ ��ҵǾ����ϴ�."
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_JOIN_FAIL_TIMEOUT_SORRY), TYPE_GUILD_CONFIRM, POPUP_OK);
		break;
	case 3: // IDS_GUILD_JOIN_SUC "��� ���� ��û�� ���εǾ����ϴ�."
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_JOIN_SUC), TYPE_GUILD_CONFIRM, POPUP_OK);
		break;
	case 4:
		{
			char name[256]= {0,};
			arg = AnyOneArg(arg, name);

			char buffer[256]= {0,};
			// IDS_GUILD_RCOM_RE_SUC "%s���� ������õ�� �¶��ϼ̽��ϴ�."
			sprintf(buffer, (char*)G_STRING(IDS_GUILD_RCOM_RE_SUC), name);
			g_pNk2DFrame->InsertPopup(buffer, TYPE_GUILD_CONFIRM, POPUP_OK);
		}
		break;
	case 5:
		{
			char name[256]= {0,};
			arg = AnyOneArg(arg, name);

			char buffer[256]= {0,};
			// IDS_GUILD_RECOM_RE_FAIL "%s���� ������õ�� �ź��ϼ̽��ϴ�."
			sprintf(buffer, (char*)G_STRING(IDS_GUILD_RECOM_RE_FAIL), name);
			g_pNk2DFrame->InsertPopup(buffer, TYPE_GUILD_CONFIRM, POPUP_OK);
		}
		break;
	case 6: // IDS_GUILD_MEMBER_FIRE_SORRY "��忡�� ����Ǿ����ϴ�."
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_MEMBER_FIRE_SORRY), TYPE_GUILD_CONFIRM, POPUP_OK);
		break;
	case 7: // IDS_GUILD_DISSOLVE "��尡 ��ü�Ǿ����ϴ�."
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_DISSOLVE), TYPE_GUILD_CONFIRM, POPUP_OK);
		break;
	case 8: // IDS_GUILD_RANKWAR_JOIN_SEND_MSG "��� ��ŷ�� ���� ��û�� ��� �Խ��ϴ�."
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_RANKWAR_JOIN_SEND_MSG), TYPE_GUILD_CONFIRM, POPUP_OK);
		break;
	case 9: // IDS_GUILD_RANKWAR_CANCEL "��� ��ŷ���� ��� �Ǿ����ϴ�."
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_RANKWAR_CANCEL), TYPE_GUILD_CONFIRM, POPUP_OK);
		break;
	case 10:
		{
			char level[256]= {0,};
			arg = AnyOneArg(arg, level);

			char buffer[256]= {0,};
			// IDS_GUILD_LVLUP "��� ������ %s(��)�� ��� �Ǿ����ϴ�."
			sprintf(buffer, (char*)G_STRING(IDS_GUILD_LVLUP), level);
			g_pNk2DFrame->InsertPopup(buffer, TYPE_GUILD_CONFIRM, POPUP_OK);
		}
		break;
	case 11:
		{
			char level[256]= {0,};
			arg = AnyOneArg(arg, level);

			char buffer[256]= {0,};
			// IDS_GUILD_LVLDOWN "��� ������ %s(��)�� ��� �Ǿ����ϴ�."
			sprintf(buffer, (char*)G_STRING(IDS_GUILD_LVLDOWN), level);
			g_pNk2DFrame->InsertPopup(buffer, TYPE_GUILD_CONFIRM, POPUP_OK);
		}
		break;
	case 12:
		{
			char name[256]= {0,};
			char jobname[256]= {0,};
			arg = AnyOneArg(arg, name);
			arg = AnyOneArg(arg, jobname);

			char buffer[256]= {0,};
			// IDS_GUILD_JOB_CAPTAIN "%s���� %'%s%'(��)�� �Ӹ� �Ǿ����ϴ�."
			sprintf(buffer, (char*)G_STRING(IDS_GUILD_JOB_CAPTAIN), name, jobname);
			g_pNk2DFrame->InsertPopup(buffer, TYPE_GUILD_CONFIRM, POPUP_OK);
		}
		break;
	case 13:
		{
			char name[256]= {0,};
			arg = AnyOneArg(arg, name);

			char buffer[256]= {0,};
			// IDS_GUILD_JOB_NORMAL "%s���� �Ϲ� �������� ���� �Ǿ����ϴ�."
			sprintf(buffer, (char*)G_STRING(IDS_GUILD_JOB_NORMAL), name);
			g_pNk2DFrame->InsertPopup(buffer, TYPE_GUILD_CONFIRM, POPUP_OK);
		}
		break;
	case 14:
		{
			char name[256]= {0,};
			arg = AnyOneArg(arg, name);

			char buffer[256]= {0,};
			// IDS_GUILD_JOB_MATER  "%s���� '��� ������'�� ���� �Ǿ����ϴ�."
			sprintf(buffer, (char*)G_STRING(IDS_GUILD_JOB_MATER), name);
			g_pNk2DFrame->InsertPopup(buffer, TYPE_GUILD_CONFIRM, POPUP_OK);
		}
		break;
	case 15:
		{
			char point[256]= {0,};
			arg = AnyOneArg(arg, point);

			char buffer[256]= {0,};
			// IDS_GUILD_RANKWAR_RESULT_WIN	"��� ��ŷ������ �¸��Ͽ� %spoint ȹ���Ͽ����ϴ�."
			sprintf(buffer, (char*)G_STRING(IDS_GUILD_RANKWAR_RESULT_WIN), point);
			g_pNk2DFrame->InsertPopup(buffer, TYPE_GUILD_CONFIRM, POPUP_OK);
		}
		break;
	case 16:
		{
			char point[256];
			arg = AnyOneArg(arg, point);

			char buffer[256];
			// IDS_GUILD_RANKWAR_RESULT_LOSE	"��� ��ŷ������ ���Ͽ� %spoint ȹ���Ͽ����ϴ�."
			sprintf(buffer, (char*)G_STRING(IDS_GUILD_RANKWAR_RESULT_LOSE), point);
			g_pNk2DFrame->InsertPopup(buffer, TYPE_GUILD_CONFIRM, POPUP_OK);
		}
		break;
	case 17:
		{
			char point[256];
			arg = AnyOneArg(arg, point);

			char buffer[256];
			// IDS_GUILD_RANKWAR_RESULT_TIE	"��� ��ŷ������ ���ºη� %spoint ȹ���Ͽ����ϴ�."
			sprintf(buffer, (char*)G_STRING(IDS_GUILD_RANKWAR_RESULT_TIE), point);
			g_pNk2DFrame->InsertPopup(buffer, TYPE_GUILD_CONFIRM, POPUP_OK);
		}
		break;

	}
}

void guild_rankwar_info(char *arg, CUIMgr *pMgr)
{
	arg  = AnyOneArg(arg, g_guildbuf);
	arg  = AnyOneArg(arg, g_guildbuf1);

	g_pNk2DFrame->ShowControlGuildBattle(true);
	g_pNk2DFrame->GetControlGuildBattle()->SetTeamName( g_guildbuf, g_guildbuf1);
}

void guild_rankwar_ready(char *arg, CUIMgr *pMgr)
{
	if( !g_pNk2DFrame->GetGuildRankWarBattleCount()->IsActive() )
		g_pNk2DFrame->GetGuildRankWarBattleCount()->LoadResource();

	g_pNk2DFrame->GetGuildRankWarBattleCount()->ReadyCount();
}

void guild_rankwar_count(char *arg, CUIMgr *pMgr)
{
// 	arg  = AnyOneArg(arg, g_guildbuf);
// 	int count = atoi(g_guildbuf);

	if( !g_pNk2DFrame->GetGuildRankWarBattleCount()->IsActive() )
		g_pNk2DFrame->GetGuildRankWarBattleCount()->LoadResource();

	g_pNk2DFrame->GetGuildRankWarBattleCount()->StartCount();
}

void guild_rankwar_time(char *arg, CUIMgr *pMgr)
{
	arg  = AnyOneArg(arg, g_guildbuf);
	int time = atoi(g_guildbuf);
}

void guild_rankwar_open(char *arg, CUIMgr *pMgr) ///071102 ��巩ŷ�� ��Ŷ �ޱ� ���� ����.
{
	arg = AnyOneArg(arg, g_guildbuf);

	int nMain = atoi(g_guildbuf);		// 1:1, �����, ��ֹ�

	int i = 0;
	int nCount[CBattleRank::BT_TOTAL]; // ���� ����

	switch(nMain)
	{
	case 0 :	// 1:1
		{
			arg = AnyOneArg(arg, g_guildbuf);

			int nSlotNum = atoi(g_guildbuf);

			arg = AnyOneArg(arg, g_guildbuf);
			nCount[0] = atoi(g_guildbuf);

			arg = AnyOneArg(arg, g_guildbuf);
			nCount[1] = atoi(g_guildbuf);

			arg = AnyOneArg(arg, g_guildbuf);
			nCount[2] = atoi(g_guildbuf);

			arg = AnyOneArg(arg, g_guildbuf);
			nCount[3] = atoi(g_guildbuf);

			arg = AnyOneArg(arg, g_guildbuf);
			nCount[4] = atoi(g_guildbuf);

			if(g_pNk2DFrame->GetUIWindow()->IsOpenWindow(WINDOW_GUILD_BATTLE_1VS1))
			{
				CBattleRank* pRank = (CBattleRank*)g_pNk2DFrame->GetUIWindow()->GetWindow(WINDOW_GUILD_BATTLE_1VS1);
				pRank->SetSlotNum(nSlotNum);
				for( i = 0 ; i < CBattleRank::BT_TOTAL ; ++i )
				{
					arg = AnyOneArg(arg, g_guildbuf);
					pRank->SetText( i, itoa(nCount[i],g_guildbuf,10) );
				}
			}
		}
		break;
	case 1 :	// �����
		{

		}
		break;
	case 2 :	// ��ֹ�
		break;
	}
}

void guild_rankwar_req(char *arg, CUIMgr *pMgr)
{
	arg = AnyOneArg(arg, g_guildbuf);

	switch(atoi(g_guildbuf))
	{
	case 0 : // IDS_GUILD_RANKWAR_JOIN_FAIL_GLEVEL "��ŷ���� ��û �Ͻ� �� �����ϴ�."
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_RANKWAR_JOIN_FAIL_GLEVEL), TYPE_GUILD_CONFIRM, 42);
		break;
	case 1 : // ��û�㰡
		{
			g_pNk2DFrame->GetUIWindow()->OnOpenWindow(WINDOW_GUILD_BATTLE_ACCEPTMEMBER);

			CBattleAcceptMember* pTemp = (CBattleAcceptMember*)g_pNk2DFrame->GetUIWindow()->GetWindow(WINDOW_GUILD_BATTLE_ACCEPTMEMBER);

			if(pTemp)	pTemp->SetSubType(g_pNk2DFrame->GetGuildSystemManager()->GetSubType());

		}
		break;
	case 2 : // IDS_GUILD_INVASION_FAIL_GULVL "��å�� ���� �ʾ� ��û�� �Ұ����մϴ�."
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_INVASION_FAIL_GULVL), TYPE_GUILD_CONFIRM, 42);
		break;
	}
}

void guild_rankwar_list(char *arg, CUIMgr *pMgr)
{
	arg = AnyOneArg(arg, g_guildbuf);

	switch(atoi(g_guildbuf))
	{
	case 0 : // IDS_GUILD_RANKWAR_JOIN_FAIL "��� ��ŷ���� ���������߽��ϴ�."
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_RANKWAR_JOIN_FAIL), TYPE_GUILD_CONFIRM, 42);
		break;
	case 1 : // IDS_GUILD_RANKWAR_JOIN_SUC "��� ��ŷ���� �����߽��ϴ�."
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_RANKWAR_JOIN_SUC), TYPE_GUILD_CONFIRM, 42);
		break;
	}
}

void guild_rankwar_invite(char *arg, CUIMgr *pMgr)
{
	arg = AnyOneArg(arg, g_guildbuf);
	int nResult = atoi(g_guildbuf);

	switch( nResult)
	{
	case 0: // IDS_GUILD_RANKWAR_ZONE_GOTO_FAIL "��� ��ŷ������ ������ �� �����ϴ�."
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_RANKWAR_ZONE_GOTO_FAIL), TYPE_GUILD_CONFIRM, POPUP_OK );
		break;

	case 1: // IDS_GUILD_RANKWAR_ZONE_GOTO_SUC "��� ��ŷ������ �̵��մϴ�."
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_RANKWAR_ZONE_GOTO_SUC), TYPE_GUILD_CONFIRM, POPUP_GUILD_RANKWAR_INVITE);
		// pCMyApp->m_pUIMgr->PrepareChangeWorld(TRUE, WORLD_GUILD_WAR, 0);
		break;
	case 2: // IDS_GUILD_RANKWAR_ZONE_GOTO_FAIL "��� ��ŷ������ ������ �� �����ϴ�."
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_RANKWAR_ZONE_GOTO_FAIL), TYPE_GUILD_CONFIRM);
		break;
	}

}

void guild_rankwar_start(char *arg, CUIMgr *pMgr)
{
	//char ATeamName[16] = {0,};
	//char BTeamName[16] = {0,};

	if( !g_pNk2DFrame->GetGuildRankWarBattleCount()->IsActive() )
		g_pNk2DFrame->GetGuildRankWarBattleCount()->LoadResource();
	else
	{
		g_pNk2DFrame->GetGuildRankWarBattleCount()->EndCount();
		g_pNk2DFrame->GetGuildRankWarBattleCount()->LoadResource();
	}

	g_pNk2DFrame->GetGuildRankWarBattleCount()->StartBattle();

	//g_pNk2DFrame->InsertPopup("��ŷ���� ���� �Ǿ����ϴ�.", TYPE_GUILD_CONFIRM, 52);
}

void guild_rankwar_end(char *arg, CUIMgr *pMgr)
{
	g_pNk2DFrame->ShowControlGuildBattle(false);
	// IDS_GUILD_RANKWAR_END "��� ��ŷ���� ����Ǿ����ϴ�."
	g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_RANKWAR_END), TYPE_GUILD_CONFIRM, 52);
}

void guild_rankwar_kcount(char *arg, CUIMgr *pMgr)
{
	arg = AnyOneArg(arg, g_guildbuf);	// ù��° ��� ����

	int AKillCount = atoi(g_guildbuf);

	arg = AnyOneArg(arg, g_guildbuf);	// �ι�° ��� ����

	int BKillCount = atoi(g_guildbuf);

	g_pNk2DFrame->GetControlGuildBattle()->SetTeamKillcount( AKillCount, BKillCount);
}

void guild_rankwar_goworld(char *arg, CUIMgr *pMgr)
{
	arg = AnyOneArg(arg, g_guildbuf);	// ����ȣ

	int Zone = atoi(g_guildbuf);

	arg = AnyOneArg(arg, g_guildbuf);	// �����ȣ

	int Sub = atoi(g_guildbuf);

	if(pMgr) pMgr->PrepareChangeWorld(TRUE, Zone, 0, Sub);
}

void guild_rankwar_cancel(char *arg, CUIMgr *pMgr)
{
	arg = AnyOneArg(arg, g_guildbuf);

	int nResult = atoi(g_guildbuf);	// 0 : ��ҽ���, 1: ��Ҽ���
}

void guild_rankwar_get(char *arg, CUIMgr *pMgr)
{
	arg = AnyOneArg(arg, g_guildbuf);

	int nResult = atoi(g_guildbuf); // 0 : ������ �������� ���� ���, 1: ���� ����, 2: �κ��丮�� �� �� ���

	switch(nResult)
	{
	case 0: // IDS_GUILD_GIFT_ITEM_FAIL "���� ���� �������� �����ϴ�."
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_GIFT_ITEM_FAIL), TYPE_GUILD_CONFIRM, 55);
		break;
	case 1: // IDS_GUILD_GIFT_ITEM_SUC "�±� ������ ���޹޾ҽ��ϴ�."
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_GIFT_ITEM_SUC), TYPE_GUILD_CONFIRM, 55);
		break;
	case 2: // IDS_CASHSHOP_ERROR3 "�κ��丮�� �� ������ �����մϴ�."
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_CASHSHOP_ERROR3), TYPE_GUILD_CONFIRM, 55);
		break;
	}
}

void guild_rankwar_comp(char *arg, CUIMgr *pMgr)
{
	arg = AnyOneArg(arg, g_guildbuf);

	int nResult = atoi(g_guildbuf);

	switch(nResult)
	{
	case 0: // IDS_GUILD_ITEM_COMPOSITION_FAIL "�������� �ռ��� �� �����ϴ�"
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_ITEM_COMPOSITION_FAIL), TYPE_GUILD_CONFIRM, 55);
		break;
	case 1: // IDS_GUILD_ITEM_COMPOSITION_SUC "�������� �ռ��߽��ϴ�."
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_ITEM_COMPOSITION_SUC), TYPE_GUILD_CONFIRM, 55);
		g_pNk2DFrame->GetUIWindow()->CloseTopWindow(); // �ռ��� �Ϸ�Ǹ� �ݾ��ش�.
		g_pNk2DFrame->GetUIWindow()->OnOpenWindow(WINDOW_GUILD_MEMBER);
		break;
	}
}

/******************************************************************************/

void guild_invasion_info(char *arg, CUIMgr *pMgr)
{
	CGuildData* pGuildData = g_pRoh->GetMyGuildData();

	if(pGuildData == NULL)	return;

	arg = AnyOneArg(arg, g_guildbuf);

	int add_or_delete = atoi(g_guildbuf);		// �߰��� ������ 0 : ����, 1 : �߰�

	arg = AnyOneArg(arg, g_guildbuf);

	int attack_or_defense = atoi(g_guildbuf);

	arg = AnyOneArg(arg, g_guildbuf);

	int guild_index = atoi(g_guildbuf);			// ��� �ε���

	arg = AnyOneArg(arg, g_guildbuf);

	char szTmp[128] = {0,};

	strcpy(szTmp, g_guildbuf);					// ����

	t_Info time_invasion;

	arg = AnyOneArg(arg, g_guildbuf);

	time_invasion.nYear = atoi(g_guildbuf);	// �� ��

	arg = AnyOneArg(arg, g_guildbuf);

	time_invasion.nMon = atoi(g_guildbuf);

	arg = AnyOneArg(arg, g_guildbuf);

	time_invasion.nDay = atoi(g_guildbuf);

	arg = AnyOneArg(arg, g_guildbuf);

	time_invasion.nHour = atoi(g_guildbuf);

	arg = AnyOneArg(arg, g_guildbuf);

	time_invasion.nMin = atoi(g_guildbuf);

	arg = AnyOneArg(arg, g_guildbuf);

	time_invasion.nSec = atoi(g_guildbuf);

	guild_Invasion Info_Invasion;

	if(attack_or_defense == 0)
		Info_Invasion.s_Invasion_flag = INVASION_DEFENSE;	// ���ϴ°Ŵ� �׷��� ��� �ؾ� �ȴ�.
	else if(attack_or_defense == 1)
		Info_Invasion.s_Invasion_flag = INVASION_ATTACK;	// �����Ѵ�...�� �׿�...

	Info_Invasion.s_guild_Index = guild_index;

	strcpy(Info_Invasion.s_guild_Name, szTmp);

	Info_Invasion.s_time = time_invasion;

	if(add_or_delete == 0)	// ����
		pGuildData->Delete_Invasion(Info_Invasion);

	if(add_or_delete == 1)  // �߰�
		pGuildData->Insert_guildInvasion(Info_Invasion);

	if( g_pNk2DFrame->GetUIWindow()->IsOpenWindow(WINDOW_GUILD_BATTLE_INVASION_INFO) )
	{
		CBattleInvasionInfo* pWidnow = (CBattleInvasionInfo*)g_pNk2DFrame->GetUIWindow()->GetWindow(WINDOW_GUILD_BATTLE_INVASION_INFO);
		pWidnow->resetlist();
		pWidnow->InsertList();
	}
}

void guild_invasion_war(char *arg, CUIMgr *pMgr)
{
	arg = AnyOneArg(arg, g_guildbuf);

	if(!strcmp(g_guildbuf, "req"))
	{
		guild_invasion_war_req(arg, pMgr);
	}
	else if(!strcmp(g_guildbuf, "list"))
	{
		guild_invasion_war_list(arg, pMgr);
	}
	else if(!strcmp(g_guildbuf, "cancel"))
	{
		guild_invasion_war_cancel(arg, pMgr);
	}
}

void guild_invasion_war_req(char *arg, CUIMgr *pMgr)
{
	arg = AnyOneArg(arg, g_guildbuf);

	switch(atoi(g_guildbuf))
	{
	case 0 :	// IDS_GUILD_INVASION_FAIL "������ ���� �ʽ��ϴ�."
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_INVASION_FAIL), TYPE_GUILD_CONFIRM, 42);
		break;
	case 1 :	// IDS_GUILD_INVASION_START "��� ħ���� ���۵˴ϴ�."
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_INVASION_START), TYPE_GUILD_CONFIRM, POPUP_GUILD_INVASION_FINISH_RES_OK);
		break;
	case 2 :	// IDS_GUILD_INVASION_ING "�̹� ħ������ ����Դϴ�."
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_INVASION_ING), TYPE_GUILD_CONFIRM, 42);
		break;
	case 3 :	// IDS_GUILD_INVASION_FAIL_LAIM "������ �����մϴ�."
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_INVASION_FAIL_LAIM), TYPE_GUILD_CONFIRM, 42);
		break;
	case 4 :	// IDS_GUILD_INVASION_FAIL_GULVL "��å�� ���� �ʾ� ��û�� �Ұ����մϴ�."
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_INVASION_FAIL_GULVL), TYPE_GUILD_CONFIRM, 42);
		break;
	case 5 :	// IDS_GUILD_INVASION_FAIL_GULVL "��å�� ���� �ʾ� ��û�� �Ұ����մϴ�."
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_LHSTRING1827), TYPE_GUILD_CONFIRM, 42);
		break;
	}
}

void guild_invasion_war_list(char *arg, CUIMgr *pMgr)
{

}

void guild_invasion_war_cancel(char *arg, CUIMgr *pMgr)
{
	arg = AnyOneArg(arg, g_guildbuf);

	if(!strcmp(g_guildbuf, "req"))
	{
		arg = AnyOneArg(arg, g_guildbuf);	// ����
		arg = AnyOneArg(arg, g_guildbuf1);  // ��帶���͸�

		char szTmp[256] = {0,};

		CGuildSystemManager* pSystem = g_pNk2DFrame->GetGuildSystemManager();

		switch( g_pRoh->CheckInvasionGNameType(g_guildbuf) )
		{
		case INVASION_ATTACK :	// ȭ��
			pSystem->set_tempbuffer1(g_guildbuf);
			pSystem->set_tempbuffer2(g_guildbuf1);
			// IDS_GUILD_INVASION_PLZ_PEASE	"%s ��忡 ȭ���� ��û�Ͽ����ϴ�. ȭ�ظ� �����Ͻðڽ��ϱ�?"
			sprintf(szTmp, (char*)G_STRING(IDS_GUILD_INVASION_PLZ_PEASE), g_guildbuf);
			g_pNk2DFrame->InsertPopup(szTmp, TYPE_GUILD_INVASION_CONFIRM_CANCEL, 53);
			break;
		case INVASION_DEFENSE : // ����
			pSystem->set_tempbuffer1(g_guildbuf);
			pSystem->set_tempbuffer2(g_guildbuf1);
			// IDS_GUILD_INVASION_PLZ_PEASE_1 "%s ��忡 ������ ��û�Ͽ����ϴ�. ȭ�ظ� �����Ͻðڽ��ϱ�?"
			sprintf(szTmp, (char*)G_STRING(IDS_GUILD_INVASION_PLZ_PEASE_1), g_guildbuf);
			g_pNk2DFrame->InsertPopup(szTmp, TYPE_GUILD_INVASION_CONFIRM_CANCEL, 53);
			break;
		case INVASION_NONE :
			break;
		}
	}
	else if(!strcmp(g_guildbuf, "res"))
	{
		char szTmp[256];

		arg = AnyOneArg(arg, g_guildbuf2);  // result

		if( atoi(g_guildbuf2) == 3 ) // ���� ����ó��
			return;

		arg = AnyOneArg(arg, g_guildbuf);	// ����
		arg = AnyOneArg(arg, g_guildbuf1);  // ��帶���͸�


		switch(atoi(g_guildbuf2))
		{
		case 0 : // IDS_GUILD_INVASION_PLZ_PEASE_FAIL "%s ��忡�� ȭ�� ��û�� ���� �߽��ϴ�."
			sprintf( szTmp, (char*)G_STRING(IDS_GUILD_INVASION_PLZ_PEASE_FAIL), g_guildbuf);
			g_pNk2DFrame->InsertPopup(szTmp, TYPE_GUILD_CONFIRM, 42);
			break;

		case 1 :		// ����
			{
				CBattleInvasionInfo* pInvasion = (CBattleInvasionInfo*)g_pNk2DFrame->GetUIWindow()->GetWindow(WINDOW_GUILD_BATTLE_INVASION_INFO);

				//if(pInvasion)
				//	pInvasion->EraseData();
				// IDS_GUILD_INVASION_PLZ_PEASE_SUC "%s ��忡�� ȭ�� ��û�� ���� �߽��ϴ�"
				sprintf( szTmp, (char*)G_STRING(IDS_GUILD_INVASION_PLZ_PEASE_SUC), g_guildbuf);
				g_pNk2DFrame->InsertPopup(szTmp, TYPE_GUILD_CONFIRM, 42);

			}
			break;

		case 2 : // IDS_GUILD_INVASION_PLZ_PEASE_FAIL_MASTER_OFFLINE "%s ����� ��� �����Ͱ� �������� �����Դϴ�."
			sprintf( szTmp, (char*)G_STRING(IDS_GUILD_INVASION_PLZ_PEASE_FAIL_MASTER_OFFLINE), g_guildbuf);
			g_pNk2DFrame->InsertPopup(szTmp, TYPE_GUILD_CONFIRM, 42);
			break;
		}
	}
}

void guild_recom_info(char *arg, CUIMgr *pMgr)
{
	arg = AnyOneArg(arg, g_guildbuf);
	int nResult = atoi(g_guildbuf);

	switch( nResult )
	{
	case 0: // ���� ��û ���
		g_pNk2DFrame->GetControlGuild()->SetRecomInfo(FALSE);
		break;

	case 1: // ���� ��û ����
		arg = AnyOneArg(arg, g_guildbuf2);
		g_pNk2DFrame->GetControlGuild()->SetRecomInfo(TRUE, g_guildbuf2);
		break;

	}

}

void guild_master_delegation(char *arg, CUIMgr *pMgr)
{
	g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_MASTER_DELEGATION),TYPE_NOR_OKCANCLE, POPUP_MASTER_DELEGATION);
}
