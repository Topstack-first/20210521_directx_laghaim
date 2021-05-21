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

// Guild Info 로그인시에 들어 와여~~~
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

// 길드 창설
void guild_create(char *arg, CUIMgr *pMgr)
{
	arg = AnyOneArg(arg, g_guildbuf);

	if(!strcmp(g_guildbuf, "t"))
	{
		// 길드 창설 조건 success or fail
		arg = AnyOneArg(arg, g_guildbuf);

		int nResult = atoi(g_guildbuf);

		switch( nResult)
		{
		case -2: // IDS_GUILD_CREATE_GUILD_DONT "길드를 창설할 수 없습니다."
			g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_CREATE_GUILD_DONT), TYPE_GUILD_CONFIRM, 42 );
			break;
		case -1: // IDS_GUILD_CREATE_GUILD_FAIL_NEED_LVL "레벨이 부족합니다."
			g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_CREATE_GUILD_FAIL_NEED_LVL), TYPE_GUILD_CONFIRM, 42 );
			break;
		case 0: // 	IDS_GUILD_CREATE_GUILD_FAIL_NEED_LAIM "라임이 부족합니다."
			g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_CREATE_GUILD_FAIL_NEED_LAIM), TYPE_GUILD_CONFIRM, 42 );
			break;
		case 1:
			g_pNk2DFrame->GetUIWindow()->OnOpenWindow(WINDOW_GUILD_CREATE);
			break;
		}
	}
	else if(!strcmp(g_guildbuf, "c"))
	{
		// 길드 창설 생성 success or fail
		arg = AnyOneArg(arg, g_guildbuf);

		int nResult = atoi(g_guildbuf);

		switch( nResult )
		{
		case -3: // IDS_GUILD_CREATE_GUILD_FAIL_NAME_DELETE "해체된 길드이름은 사용할 수 없습니다."
			g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_CREATE_GUILD_FAIL_NAME_DELETE), TYPE_GUILD_CONFIRM, 42 );
			break;

		case -2: // IDS_GUILD_CREATE_GUILD_FAIL "길드 창설에 실패 하였습니다."
			g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_CREATE_GUILD_FAIL), TYPE_GUILD_CONFIRM, 42 );
			break;

		case -1: // IDS_CHECK_ITEM "아이템을 확인해 주세요!"
			g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_CHECK_ITEM), TYPE_GUILD_CONFIRM, 42 );
			break;

		case  0: // IDS_GUILD_CREATE_GUILD_FAIL_NAME "같은 이름의 길드가 존재합니다."
			g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_CREATE_GUILD_FAIL_NAME), TYPE_GUILD_CONFIRM, 42 );
			break;

		case  1: // IDS_GUILD_CREATE_GUILD_SUC "길드가 창설 되었습니다."
			g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_CREATE_GUILD_SUC), TYPE_GUILD_CONFIRM, 42 );
			break;
		}
	}
}

// 길드 검색
void guild_search(char *arg, CUIMgr *pMgr)
{
	arg = AnyOneArg(arg, g_guildbuf);

	if(!strcmp(g_guildbuf, "s"))
	{
		// 검색 시작 알림
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
		// 검색내용을 받음.
		arg = AnyOneArg(arg, g_guildbuf );				// 길드명
		strcpy(list_tmp.guild_name, g_guildbuf);

		arg = AnyOneArg(arg, g_guildbuf);				// 길드마스터명
		strcpy(list_tmp.guild_master_name, g_guildbuf);

		arg = AnyOneArg(arg, g_guildbuf);	// 길드랭크
		g_guildii[0] = atoi(g_guildbuf);
		list_tmp.guild_rank = g_guildii[0];

		arg = AnyOneArg(arg, g_guildbuf);	// 길드레벨
		g_guildii[0] = atoi(g_guildbuf);
		list_tmp.guild_glevel = g_guildii[0];

		arg = AnyOneArg(arg, g_guildbuf3);  // 길드점수
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
		// 검색 끝
		CGuildSearch* pSearch = NULL;

		if(g_pNk2DFrame->GetUIWindow()->IsOpenWindow(WINDOW_GUILD_SEARCH))
		{
			pSearch = (CGuildSearch*)g_pNk2DFrame->GetUIWindow()->GetWindow(WINDOW_GUILD_SEARCH);
			if(pSearch)
			{
				pSearch->set_start(FALSE);
			}

			if(!g_bSearch) // IDS_GUILD_SERCH_FAIL_GUILD "검색된 길드가 없습니다"
				g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_SERCH_FAIL_GUILD), TYPE_GUILD_CONFIRM, 42);

		}


	}
}

// 길드 해산
void guild_disband(char *arg, CUIMgr *pMgr)
{
	arg = AnyOneArg(arg, g_guildbuf);

	int nResult = atoi(g_guildbuf);

	switch(nResult)
	{
	case 0 : // 실패 // IDS_GUILD_DISERSE_FAIL "길드를 해산할 수 없습니다. 조건을 확인 바랍니다."
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_DISERSE_FAIL), TYPE_GUILD_CONFIRM, 42);
		break;
	case 1 : // 성공  /IDS_GUILD_DISERSE_SUC "길드가 해산되었습니다."
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_DISERSE_SUC), TYPE_GUILD_CONFIRM, 42);
		g_pNk2DFrame->GetUIWindow()->CloseTopWindow();
		g_pRoh->DeleteMyGuildData();
		break;
//		case 2 : // 해산 알림
//			g_pNk2DFrame->InsertPopup("길드가 해산됩니다.", TYPE_GUILD_CONFIRM,);
//			break;
	}
}

// 길드 창고
void guild_stash(char *arg, CUIMgr *pMgr)
{
	arg = AnyOneArg(arg, g_guildbuf);

	if(!strcmp(g_guildbuf, "n"))
	{
		// 열기 실패
		guild_stash_n(arg, pMgr);
	}
	else if(!strcmp(g_guildbuf, "s"))
	{
		// 창고 시작
		guild_stash_s(arg, pMgr);
	}
	else if(!strcmp(g_guildbuf, "r"))
	{
		// 창고 내용 받음
		guild_stash_r(arg, pMgr);
	}
	else if(!strcmp(g_guildbuf, "e"))
	{
		// 창고 끝
		guild_stash_e(arg, pMgr);
	}
	else if(!strcmp(g_guildbuf, "t"))
	{
		// 아이템 넣기 (아이템 정보) - 단일, 라임 넣기
		guild_stash_t(arg, pMgr);
	}
	else if(!strcmp(g_guildbuf, "f"))
	{
		// 아이템 빼기 (아이템 정보) - 단일, 라인 빼기
		guild_stash_f(arg, pMgr);
	}
	else if(!strcmp(g_guildbuf, "p"))
	{
		// 아이템 넣기 (아이템 정보) - 다중 넣기
		guild_stash_p(arg, pMgr);
	}
	else if(!strcmp(g_guildbuf, "g"))
	{
		// 아이템 빼기 (아이템 정보) - 다중 빼기
		guild_stash_g(arg, pMgr);
	}
}

void guild_stash_n(char *arg, CUIMgr *pMgr)
{
	// IDS_GUILD_MSG_01 창고 사용중...
	g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_GUILD_MSG_01), TYPE_NOR_OK, 1 );
}

void guild_stash_s(char *arg, CUIMgr *pMgr)
{
	// 창고 시작 알림

}

void guild_stash_r(char *arg, CUIMgr *pMgr)
{
	if( !pMgr )	return;

	arg = AnyOneArg(arg, g_guildbuf);
	g_guildii[0] = atoi(g_guildbuf);	// 슬롯번호

	arg = AnyOneArg(arg, g_guildbuf);	// vnum
	g_guildii[1] = atoi(g_guildbuf);

	arg = AnyOneArg(arg, g_guildbuf);	// item_plus
	g_guildii[2] = atoi(g_guildbuf);

	arg = AnyOneArg(arg, g_guildbuf);	// item_flag
	g_guildii[3] = atoi(g_guildbuf);

	///신방어구강화시스템
	arg = AnyOneArg(arg, g_guildbuf);
	g_guildii[6] = atoi(g_guildbuf);	// 추가 옵션 m_Special2.
	arg = AnyOneArg(arg, g_guildbuf);
	g_guildii[7] = atoi(g_guildbuf);	// 현재 강화 내구도 수치.
	arg = AnyOneArg(arg, g_guildbuf);
	g_guildii[8] = atoi(g_guildbuf);	// 맥스 강화 내구도.

	if( g_SvrType == ST_ADULT_ONLY)
	{
		arg = AnyOneArg(arg, g_guildbuf);
		g_guildii[9] = atoi(g_guildbuf);	// 현재 강화 내구도 수치.
		arg = AnyOneArg(arg, g_guildbuf);
		g_guildii[10] = atoi(g_guildbuf);	// 맥스 강화 내구도.
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
		g_guildii[9] = atoi(g_guildbuf);	// 일반 내구도

		arg = AnyOneArg(arg, g_guildbuf);
		g_guildii[10] = atoi(g_guildbuf);	// 일반 내구도
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

	g_guildii[0] = atoi( g_guildbuf );	// 성공 여부 확인 메세지

	if( g_guildii[0] == 1 )
	{
		arg = AnyOneArg( arg,g_guildbuf );
		g_guildii[ 1 ] = atoi( g_guildbuf );		// 아이템 번호
		arg = AnyOneArg( arg, g_guildbuf );
		g_guildii[ 2 ] = atoi( g_guildbuf );		// 인벤 팩 번호
		arg = AnyOneArg( arg, g_guildbuf );
		g_guildii[ 3 ] = atoi( g_guildbuf );		// 창고 팩 번호
		arg = AnyOneArg( arg, g_guildbuf );
		g_guildii[ 4 ] = atoi( g_guildbuf );		// 창고 슬롯 번호
		arg = AnyOneArg( arg, g_guildbuf );
		g_guildii[ 5 ] = atoi( g_guildbuf );		// 아이템 갯수

		for( int i = 0 ; i < g_guildii[ 5 ]  ; i ++ )
		{
			arg = AnyOneArg( arg, g_guildbuf );		// X 좌표
			g_guildii[ 6 ] = atoi( g_guildbuf );
			arg = AnyOneArg( arg, g_guildbuf );		// Y 좌표
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

	g_guildii[ 0 ] = atoi( g_guildbuf );	// 성공 여부 확인 메세지
	if( g_guildii[ 0 ] == 1 )
	{
		arg = AnyOneArg( arg, g_guildbuf );
		g_guildii[ 1 ] = atoi( g_guildbuf );		// 아이템 번호
		arg = AnyOneArg( arg, g_guildbuf );
		g_guildii[ 2 ] = atoi( g_guildbuf );		// 인벤 팩 번호
		arg = AnyOneArg( arg, g_guildbuf );
		g_guildii[ 3 ] = atoi( g_guildbuf );		// 창고 팩 번호
		arg = AnyOneArg( arg, g_guildbuf );
		g_guildii[ 4 ] = atoi( g_guildbuf );		// 창고 슬롯 번호
		arg = AnyOneArg( arg, g_guildbuf );
		g_guildii[ 5 ] = atoi( g_guildbuf );		// 아이템 카운트

		for( int i = 0 ; i < g_guildii[ 5 ]  ; i ++ )
		{
			arg = AnyOneArg( arg, g_guildbuf );		// Item Index
			g_guildii[ 6 ] = atoi( g_guildbuf );
			arg = AnyOneArg( arg, g_guildbuf );		// X 좌표
			g_guildii[ 7 ] = atoi( g_guildbuf );
			arg = AnyOneArg( arg, g_guildbuf );		// Y 좌표
			g_guildii[ 8 ] = atoi( g_guildbuf );
			pMgr->guild_StashGet( g_guildii[ 1 ], g_guildii[ 2 ], g_guildii[ 3 ], g_guildii[ 4 ],
								  g_guildii[ 6 ], g_guildii[ 7 ], g_guildii[ 8 ] );
		}
	}
}

// 길드 탈퇴
void guild_leave(char *arg, CUIMgr *pMgr)
{
	arg = AnyOneArg(arg, g_guildbuf);

	// 길드를 탈퇴 한다. success or fail
	int nResult = atoi(g_guildbuf);

	switch( nResult )
	{
	case -3:		
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_MG_LEAVE_ERR_3), TYPE_GUILD_CONFIRM, 42);
		break;
	case -2: // 이유 불분명
		//g_pNk2DFrame->InsertPopup("탈퇴하는데 실패하였습니다.", TYPE_GUILD_CONFIRM, 42);
		break;

	case -1: // IDS_GUILD_MEMBER_OUT_DONT_MASTER "길드 마스터는 탈퇴할 수 없습니다."
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_MEMBER_OUT_DONT_MASTER), TYPE_GUILD_CONFIRM, 42);
		break;

	case 0: // IDS_GUILD_MEMBER_OUT_FAIL_LAIM "라임이 부족합니다. 라임을 확인하시기 바랍니다."
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_MEMBER_OUT_FAIL_LAIM), TYPE_GUILD_CONFIRM, 42);
		break;

	case 1: // IDS_GUILD_MEMBER_OUT_SUC "길드 탈퇴 처리가 되었습니다."
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_MEMBER_OUT_SUC), TYPE_GUILD_CONFIRM, 42);
		g_pRoh->DeleteMyGuildData();
		g_pNk2DFrame->GetUIWindow()->CloseTopWindow();
		break;
	}
}

// 길드 승급
void guild_upgrade(char *arg, CUIMgr *pMgr)
{
	arg = AnyOneArg(arg, g_guildbuf);

	// 승급 success or fail
	int nResult = atoi(g_guildbuf);

	switch( nResult )
	{
	case 0:	// IDS_GUILD_LVLUP_FAIL "승급 조건에 맞지 않습니다. 승급 조건을 확인 바랍니다."
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_LVLUP_FAIL), TYPE_GUILD_CONFIRM);
		break;

	case 1: // IDS_GUILD_LVLUP_SUC "길드 레벨이 한단계 승급되었습니다."
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_LVLUP_SUC), TYPE_GUILD_CONFIRM);
		g_pNk2DFrame->GetUIWindow()->OnOpenWindow(WINDOW_GUILD_MEMBER);
		break;
	}
}

// 길드 전투

// 추천가입
void guild_recom(char *arg, CUIMgr *pMgr)
{
	arg = AnyOneArg(arg, g_guildbuf);

	if(!strcmp(g_guildbuf , "r"))
	{
		// 길드 추천 신청 결과
		guild_recom_r(arg);
	}
	else if(!strcmp(g_guildbuf, "a"))
	{
		// 길드 신청 승락 결과
		guild_recom_a(arg);
	}
}

void guild_recom_r(char *arg)
{
	arg = AnyOneArg(arg, g_guildbuf);

	int nResult = atoi(g_guildbuf);

	switch(nResult)
	{
	case 0 : // IDS_GUILD_RECOM_FAIL_SERCH_CHA "추천한 캐릭터를 찾을 수 없습니다."
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_RECOM_FAIL_SERCH_CHA), TYPE_GUILD_CONFIRM, POPUP_GUILD_NORMAL);
		break;
	case 1 : // IDS_GUILD_RECOM_SUC "길드 가입 추천이 되었습니다."
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_RECOM_SUC), TYPE_GUILD_CONFIRM, POPUP_GUILD_NORMAL);
		break;
	case 2 : // IDS_GUILD_RECOM_FAIL_GMEMBER "길드에 가입된 캐릭터입니다."
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_RECOM_FAIL_GMEMBER), TYPE_GUILD_CONFIRM, POPUP_GUILD_NORMAL);
		break;
	case 3 : // IDS_GUILD_RECOM_FAIL_MEMBER_COUNT "길드 가입 허용 인원이 초과되어 추천할 수 없습니다."
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_RECOM_FAIL_MEMBER_COUNT), TYPE_GUILD_CONFIRM, POPUP_GUILD_NORMAL);
		break;
	case 4 : // IDS_GUILD_RECOM_FAIL_LVL "50레벨 미만은 길드가입 추천을 할 수 없습니다."
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_RECOM_FAIL_LVL), TYPE_GUILD_CONFIRM, POPUP_GUILD_NORMAL);
		break;
	case 5 : // IDS_GUILD_RECOM_FAIL "길드 가입이 불가능 합니다."
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_RECOM_FAIL), TYPE_GUILD_CONFIRM, POPUP_GUILD_NORMAL);
		break;
	}
}

void guild_recom_a(char *arg)
{
	// 추천된 길드명, 캐릭터명 정보를 받는다..
	arg  = AnyOneArg(arg, g_guildbuf);
	arg  = AnyOneArg(arg, g_guildbuf1);

	char szTmp[256] = {0,};

	// IDS_GUILD_RECOM_TO "%'%s%' 길드로 %s%님이 추천 하였습니다."
	sprintf(szTmp, (char*)G_STRING(IDS_GUILD_RECOM_TO), g_guildbuf, g_guildbuf1);

	g_pNk2DFrame->GetGuildSystemManager()->set_guilds_name(g_guildbuf);
	g_pNk2DFrame->GetGuildSystemManager()->set_guilds_recomname(g_guildbuf1);

	g_pNk2DFrame->InsertPopup(szTmp, TYPE_GUILD_CONFIRM_CANCEL, POPUP_GUILD_RECOMMAND);
}

// 일반가입
void guild_request(char *arg, CUIMgr *pMgr)
{
	arg = AnyOneArg(arg, g_guildbuf);

	// 가입신청 결과
	int nResult = atoi(g_guildbuf);

	switch(nResult)
	{
	case 0 : // 찾기 실패
		break;
	case 1 : // IDS_GUILD_RECOM_REG_SUC			"길드 가입 신청이 접수되었습니다."
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_RECOM_REG_SUC), TYPE_GUILD_CONFIRM, 42 );
		break;
	case 2 : // IDS_GUILD_RECOM_REG_FAIL		"길드 가입 요청자 또는 가입자는 요청할 수 없습니다."
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_RECOM_REG_FAIL), TYPE_GUILD_CONFIRM, 42 );
		break;
	case 3 : // IDS_GUILD_JOIN_FAIL_MEMBER "길드인원 초과로 가입 요청을 할 수 없습니다."
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_RECOM_FAIL_MEMBER), TYPE_GUILD_CONFIRM, 42 );
		break;
	case 4 : // IDS_GUILD_RECOM_FAIL_LVL "50레벨 미만은 길드가입 추천을 할 수 없습니다."	
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_RECOM_FAIL_LVL), TYPE_GUILD_CONFIRM, 42);
		break;
	case 5 : // IDS_GUILD_RECOM_FAIL "길드 가입이 불가능 합니다."
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_RECOM_FAIL), TYPE_GUILD_CONFIRM, 42);
		break;
	}
}

// 길드 관리 >> 계급명 변경
void guild_nick(char *arg, CUIMgr *pMgr)
{
	arg = AnyOneArg(arg, g_guildbuf);

	// 변경 정보 및 성공 여부

	switch(atoi(g_guildbuf))
	{
	case 0 : // // IDS_GUILD_JOBNAME_FAIL "계급명 변경에 실패하였습니다."
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_JOBNAME_FAIL), TYPE_GUILD_CONFIRM);
		break;
	case 1 : // 성공
		{
			CGuildData* pGuildData = g_pRoh->GetMyGuildData();

			arg = AnyOneArg(arg, g_guildbuf1); // 레벨
			int nlevel = atoi(g_guildbuf1);

			arg = AnyOneArg(arg, g_guildbuf2); // 계급명

			char oldGNmae[128]= {0};
			char msg[256]= {0};
			memcpy(oldGNmae,pGuildData->GetMyGuildInfo().s_guildjobname[nlevel], strlen(pGuildData->GetMyGuildInfo().s_guildjobname[nlevel]) );

			// 나의 길드 정보를 갱신
			pGuildData->Change_guildgradename(g_guildbuf2, nlevel);

			// 정보창에 내용을 갱신
			g_pNk2DFrame->GetControlGuildManage()->ResetGrade(g_guildbuf2, nlevel);
			// IDS_GUILD_JOBNAME_CHANGE_SUC "%'%s%'계급명이 %'%s%'(으)로 변경되었습니다."
			sprintf( msg, (char*)G_STRING(IDS_GUILD_JOBNAME_CHANGE_SUC),oldGNmae, g_guildbuf2 );
			g_pNk2DFrame->InsertPopup(msg, TYPE_GUILD_CONFIRM);
		}
		break;
	}

}

// 길드 관리 >> 공지 변경
void guild_notice(char *arg, CUIMgr *pMgr)
{
	arg = AnyOneArg(arg, g_guildbuf);

	switch(atoi(g_guildbuf))
	{
	case 0 :	// IDS_GUILD_NOTICE_MODIFY_FAIL "공지 수정에 실패 했습니다."
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_NOTICE_MODIFY_FAIL), TYPE_GUILD_CONFIRM, 42);
		break;
	case 1 :	// 성공
		{
			// IDS_GUILD_NOTICE_MODIFY_SUC "길드 공지가 변경 되었습니다."
			g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_NOTICE_MODIFY_SUC), TYPE_GUILD_CONFIRM, 42);
		}
		break;
	}
}

// 길드 관리 >> 가입 승락
void guild_allow(char *arg, CUIMgr *pMgr)
{
	arg = AnyOneArg(arg, g_guildbuf);

	switch(atoi(g_guildbuf))
	{
	case 0 : // IDS_GUILD_JOIN_FAIL	"길드 가입 요청이 승인되지 않았습니다"
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_JOIN_FAIL), TYPE_GUILD_CONFIRM, 42);
		break;
	case 1 : // IDS_GUILD_JOIN_SUC "길드 가입 요청이 승인되었습니다."
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_JOIN_SUC), TYPE_GUILD_CONFIRM, 42);
		break;
	case 2 : // IDS_GUILD_JOIN_FAIL	"길드 가입 요청이 승인되지 않았습니다"
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_JOIN_FAIL), TYPE_GUILD_CONFIRM, 42);
		break;
	}
}

// 길드 관리 >> 직책관리
void guild_appoint(char *arg, CUIMgr *pMgr)
{
	arg = AnyOneArg(arg, g_guildbuf);

	switch(atoi(g_guildbuf))
	{
	case 0 : // IDS_GUILD_JOB_FAIL "임명에 실패하였습니다."
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_JOB_FAIL), TYPE_GUILD_CONFIRM, 42);
		break;
	case 1 :  // IDS_GUILD_JOB_SUC 임명되었습니다."
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_JOB_SUC), TYPE_GUILD_CONFIRM, 42);
		break;
	}
}

// 길드 관리 >> 길원강퇴
void guild_fire(char *arg, CUIMgr *pMgr)
{
	// success or fail
	arg = AnyOneArg(arg, g_guildbuf);

	switch(atoi(g_guildbuf))
	{
	case 0 :  // IDS_GUILD_MEMBER_FIRE_FAIL	 "길드 축출에 실패하였습니다."
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_MEMBER_FIRE_FAIL), TYPE_GUILD_CONFIRM, 42);
		break;
	case 1 :
		{
			// IDS_GUILD_MEMBER_FIRE_SUC "길드 축출에 성공했습니다."
			g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_MEMBER_FIRE_SUC), TYPE_GUILD_CONFIRM, 42);
			g_pNk2DFrame->GetControlGuildManage()->ResetMember();
		}
		break;
	case 2 :
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_MG_FIRE_ERR_2), TYPE_GUILD_CONFIRM, 42);
		break;
	}
}

// 길드 관리 >> 길원강퇴
void guild_fired(char *arg, CUIMgr *pMgr)
{
	// 강퇴된 캐릭 전달
	// IDS_GUILD_MEMBER_FIRE "길드에서 축출 되었습니다. "
	g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_MEMBER_FIRE), TYPE_GUILD_CONFIRM, 42);
	g_pRoh->DeleteMyGuildData();
	if( pCMyApp->GetCurWorld() == WORLD_DMITRON_BATTLE )
	{
		LH_SEND_NET_MSG( "go_world 9 0\n" );
	}
}

// 정보갱신 >> 길드
void guild_ginfo(char *arg, CUIMgr *pMgr)
{
	if(g_pRoh == NULL) return;

	arg = AnyOneArg(arg, g_guildbuf);

	int add_or_update = atoi(g_guildbuf);		// 0:추가, 1:갱신

	if(add_or_update == 0)
	{
		g_pRoh->MakeMyGuildData();
	}

	CGuildData* pGuildData = g_pRoh->GetMyGuildData();

	if(pGuildData == NULL)	return;

	guild_Info	tmpInfo;

	arg = AnyOneArg(arg, g_guildbuf);

	tmpInfo.s_guildIndex = atoi(g_guildbuf);	// 길드 인덱스

	arg = AnyOneArg(arg, g_guildbuf);

	strcpy(tmpInfo.s_guildName, g_guildbuf);	// 길드명

	arg = AnyOneArg(arg, g_guildbuf);

	tmpInfo.s_rank = atoi(g_guildbuf);			// 길드랭킹

	arg = AnyOneArg(arg, g_guildbuf);

	tmpInfo.s_glevel = atoi(g_guildbuf);		// 길드레벨  ( 길드원 레벨과 혼동 하지 마삼 )

	arg = AnyOneArg(arg, g_guildbuf);

	tmpInfo.s_gPoint = atoi(g_guildbuf);		// 길드점수

	arg = AnyOneArg(arg, g_guildbuf);

	strcpy(tmpInfo.s_guildjobname[JOB_MASTER], g_guildbuf);	// 길드 마스터

	arg = AnyOneArg(arg, g_guildbuf);

	strcpy(tmpInfo.s_guildjobname[JOB_ASSISTANTMASTER], g_guildbuf);	// 길드 마스터

	arg = AnyOneArg(arg, g_guildbuf);

	strcpy(tmpInfo.s_guildjobname[JOB_VULCAN], g_guildbuf);	// 불칸부장

	arg = AnyOneArg(arg, g_guildbuf);

	strcpy(tmpInfo.s_guildjobname[JOB_KAI], g_guildbuf);	// 카이부장

	arg = AnyOneArg(arg, g_guildbuf);

	strcpy(tmpInfo.s_guildjobname[JOB_EIDA], g_guildbuf);	// 에이다부장

	arg = AnyOneArg(arg, g_guildbuf);;

	strcpy(tmpInfo.s_guildjobname[JOB_HUMAN], g_guildbuf);	// 휴먼부장

	arg = AnyOneArg(arg, g_guildbuf);

	strcpy(tmpInfo.s_guildjobname[JOB_HIV], g_guildbuf);	// 하이부장

	arg = AnyOneArg(arg, g_guildbuf);

	strcpy(tmpInfo.s_guildjobname[JOB_GENERIC], g_guildbuf); // 일반길원 명칭

	arg = AnyOneArg(arg, g_guildbuf);

	strcpy(tmpInfo.s_guildjobname[JOB_PEROM], g_guildbuf); // 일반길원 명칭

	arg = AnyOneArg(arg, g_guildbuf);

	tmpInfo.s_revisiontime.nYear = (time_t)atol(g_guildbuf); // 등급하락시 보정시간

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

// 정보갱신 >> 길드원
void guild_info(char *arg, CUIMgr *pMgr)
{

	guild_member tmpMember;

	arg = AnyOneArg(arg, g_guildbuf);

	int nRefresh = atoi(g_guildbuf);		// 0 : 추가(로그인) 1 : 갱신

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

	tmpMember.s_index = atoi(g_guildbuf);	// 캐릭 인덱스

	arg = AnyOneArg(arg, g_guildbuf);

	tmpMember.s_glevel = atoi(g_guildbuf);  // 직책 레벨

	arg = AnyOneArg(arg, g_guildbuf);

	strcpy(tmpMember.s_name, g_guildbuf);   // 캐릭명

	arg = AnyOneArg(arg, g_guildbuf);

	tmpMember.s_race = atoi(g_guildbuf);	// 종족

	arg = AnyOneArg(arg, g_guildbuf);

	tmpMember.s_connect = atoi(g_guildbuf); // 접속 상태

	arg = AnyOneArg(arg, g_guildbuf);

	tmpMember.s_jtype = atoi(g_guildbuf);	// 가입형태

	arg = AnyOneArg(arg, g_guildbuf);

	tmpMember.s_waittime.nYear = atol(g_guildbuf); // 가입대기중 시간

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

	if(tmpMember.s_glevel == 0 && tmpMember.s_jtype == 1)	// 추천 가입 대기자
		tmpMember.s_check = CHECK_RECOMMAND_WAIT_ADD;
	else if(tmpMember.s_glevel == 0 && tmpMember.s_jtype == 2)	// 일반 가입 대기자
		tmpMember.s_check = CHECK_GENERAL_WAIT_ADD;
	else
		tmpMember.s_check = CHECK_ADD;						// 멤버들....

	if(pGuildData)
		pGuildData->Insert_guilddata(tmpMember);

}

void guild_info_update(char *arg, CUIMgr *pMgr)
{
	CGuildData* pGuildData = g_pRoh->GetMyGuildData();

	if(pGuildData == NULL)	return;

	guild_member tmpMember;

	arg = AnyOneArg(arg, g_guildbuf);

	tmpMember.s_index = atoi(g_guildbuf);	// 캐릭 인덱스

	arg = AnyOneArg(arg, g_guildbuf);

	tmpMember.s_glevel = atoi(g_guildbuf);  // 직책 레벨

	arg = AnyOneArg(arg, g_guildbuf);

	strcpy(tmpMember.s_name, g_guildbuf);   // 캐릭명

	arg = AnyOneArg(arg, g_guildbuf);

	tmpMember.s_race = atoi(g_guildbuf);	// 종족

	arg = AnyOneArg(arg, g_guildbuf);

	tmpMember.s_connect = atoi(g_guildbuf); // 접속 상태

	arg = AnyOneArg(arg, g_guildbuf);

	tmpMember.s_jtype = atoi(g_guildbuf);	// 가입형태

	arg = AnyOneArg(arg, g_guildbuf);

	tmpMember.s_waittime.nYear = atoi(g_guildbuf); // 가입대기중 시간

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
		if(pGuildData->IsWaitlist(tmpMember, 1))	// 추천
		{
			pGuildData->Delete_RecommandWait(tmpMember);
			tmpMember.s_check = CHECK_ADD;
		}
		else if(pGuildData->IsWaitlist(tmpMember, 2))	// 일반
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
// 정보갱신 >> 공지
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

// 정보갱신 >> 직책변경
void guild_ainfo(char *arg, CUIMgr *pMgr)
{
	CGuildData* pGuildData = g_pRoh->GetMyGuildData();

	if(pGuildData == NULL)	return;

	arg = AnyOneArg(arg, g_guildbuf);

	int nLevel = atoi(g_guildbuf);		// 직책

	arg = AnyOneArg(arg, g_guildbuf);

	int nCharIdx = atoi(g_guildbuf1);	// 캐릭 인덱스

	CJobChangeControl* pJob =  g_pNk2DFrame->GetControlGuildManage()->m_pJob;

	if(pJob == 0)	return;

	guild_member* mem_ber = pGuildData->SearchMember_charIdx(nCharIdx);

	if(mem_ber)
	{
		mem_ber->s_glevel = nLevel;
		pJob->Insert_list();
	}

}

// 정보갱신 >> 등급 업
void guild_up(char *arg, CUIMgr *pMgr)
{
	/*
	arg = AnyOneArg(arg, g_guildbuf);

	int g_level = atoi(g_guildbuf);

	CGuildData* pGuildData = g_pRoh->GetMyGuildData();

	if(pGuildData == NULL)	return;

	pGuildData->Change_guildLevel(g_level);

	g_pNk2DFrame->InsertPopup("등급 업 되었습니다.", TYPE_GUILD_CONFIRM, 42);
	*/
}

// 정보갱신 >> 등급 다운
void guild_down(char *arg, CUIMgr *pMgr)
{
	/*
		arg = AnyOneArg(arg, g_guildbuf);

		int g_level = atoi(g_guildbuf);

		CGuildData* pGuildData = g_pRoh->GetMyGuildData();

		if(pGuildData == NULL)	return;

		pGuildData->Change_guildLevel(g_level);

		g_pNk2DFrame->InsertPopup("등급이 하락 되었습니다.", TYPE_GUILD_CONFIRM, 42);
	*/
}

//침략 정보
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
		// IDS_GUILD_LEVELDOWN_D_DAY_YEAR	"%02d/%02d/%02d %02d:%02d까지 길드 등급을 충족하지 못할 경우 길드 등급이 하락됩니다."
		sprintf(szTmp, (char*)G_STRING(IDS_GUILD_LEVELDOWN_D_DAY_YEAR), time.nYear, time.nMon, time.nDay, time.nHour, time.nMin);
	}
	else if( time.nMon != -1 )
	{
		// IDS_GUILD_LEVELDOWN_D_DAY_MONTH	"%02d/%02d %02d:%02d까지 길드 등급을 충족하지 못할 경우 길드 등급이 하락됩니다."
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

	sprintf(szTmp, "길드 등급을 충족하지 못할 경우 %d월 %d일 %d시 %d분에 길드 등급이 하락됩니다.", nMonth, nDay, nHour, nMinute);

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

	int nMsg = atoi(g_guildbuf);		// 1:1, 깃발전, 장애물

	switch(nMsg)
	{
	case 1: // IDS_GUILD_JOIN_FAIL_SORRY "길드 가입 요청이 거부되었습니다."
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_JOIN_FAIL_SORRY), TYPE_GUILD_CONFIRM, POPUP_OK);
		break;
	case 2: // IDS_GUILD_JOIN_FAIL_TIMEOUT_SORRY "승인시간 초과로 인해 길드 가입이 취소되었습니다."
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_JOIN_FAIL_TIMEOUT_SORRY), TYPE_GUILD_CONFIRM, POPUP_OK);
		break;
	case 3: // IDS_GUILD_JOIN_SUC "길드 가입 요청이 승인되었습니다."
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_JOIN_SUC), TYPE_GUILD_CONFIRM, POPUP_OK);
		break;
	case 4:
		{
			char name[256]= {0,};
			arg = AnyOneArg(arg, name);

			char buffer[256]= {0,};
			// IDS_GUILD_RCOM_RE_SUC "%s님이 가입추천을 승락하셨습니다."
			sprintf(buffer, (char*)G_STRING(IDS_GUILD_RCOM_RE_SUC), name);
			g_pNk2DFrame->InsertPopup(buffer, TYPE_GUILD_CONFIRM, POPUP_OK);
		}
		break;
	case 5:
		{
			char name[256]= {0,};
			arg = AnyOneArg(arg, name);

			char buffer[256]= {0,};
			// IDS_GUILD_RECOM_RE_FAIL "%s님이 가입추천을 거부하셨습니다."
			sprintf(buffer, (char*)G_STRING(IDS_GUILD_RECOM_RE_FAIL), name);
			g_pNk2DFrame->InsertPopup(buffer, TYPE_GUILD_CONFIRM, POPUP_OK);
		}
		break;
	case 6: // IDS_GUILD_MEMBER_FIRE_SORRY "길드에서 축출되었습니다."
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_MEMBER_FIRE_SORRY), TYPE_GUILD_CONFIRM, POPUP_OK);
		break;
	case 7: // IDS_GUILD_DISSOLVE "길드가 해체되었습니다."
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_DISSOLVE), TYPE_GUILD_CONFIRM, POPUP_OK);
		break;
	case 8: // IDS_GUILD_RANKWAR_JOIN_SEND_MSG "길드 랭킹전 참가 요청이 들어 왔습니다."
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_RANKWAR_JOIN_SEND_MSG), TYPE_GUILD_CONFIRM, POPUP_OK);
		break;
	case 9: // IDS_GUILD_RANKWAR_CANCEL "길드 랭킹전이 취소 되었습니다."
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_RANKWAR_CANCEL), TYPE_GUILD_CONFIRM, POPUP_OK);
		break;
	case 10:
		{
			char level[256]= {0,};
			arg = AnyOneArg(arg, level);

			char buffer[256]= {0,};
			// IDS_GUILD_LVLUP "길드 레벨이 %s(으)로 상승 되었습니다."
			sprintf(buffer, (char*)G_STRING(IDS_GUILD_LVLUP), level);
			g_pNk2DFrame->InsertPopup(buffer, TYPE_GUILD_CONFIRM, POPUP_OK);
		}
		break;
	case 11:
		{
			char level[256]= {0,};
			arg = AnyOneArg(arg, level);

			char buffer[256]= {0,};
			// IDS_GUILD_LVLDOWN "길드 레벨이 %s(으)로 상등 되었습니다."
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
			// IDS_GUILD_JOB_CAPTAIN "%s님이 %'%s%'(으)로 임명 되었습니다."
			sprintf(buffer, (char*)G_STRING(IDS_GUILD_JOB_CAPTAIN), name, jobname);
			g_pNk2DFrame->InsertPopup(buffer, TYPE_GUILD_CONFIRM, POPUP_OK);
		}
		break;
	case 13:
		{
			char name[256]= {0,};
			arg = AnyOneArg(arg, name);

			char buffer[256]= {0,};
			// IDS_GUILD_JOB_NORMAL "%s님이 일반 길드원으로 강등 되었습니다."
			sprintf(buffer, (char*)G_STRING(IDS_GUILD_JOB_NORMAL), name);
			g_pNk2DFrame->InsertPopup(buffer, TYPE_GUILD_CONFIRM, POPUP_OK);
		}
		break;
	case 14:
		{
			char name[256]= {0,};
			arg = AnyOneArg(arg, name);

			char buffer[256]= {0,};
			// IDS_GUILD_JOB_MATER  "%s님이 '길드 마스터'로 위임 되었습니다."
			sprintf(buffer, (char*)G_STRING(IDS_GUILD_JOB_MATER), name);
			g_pNk2DFrame->InsertPopup(buffer, TYPE_GUILD_CONFIRM, POPUP_OK);
		}
		break;
	case 15:
		{
			char point[256]= {0,};
			arg = AnyOneArg(arg, point);

			char buffer[256]= {0,};
			// IDS_GUILD_RANKWAR_RESULT_WIN	"길드 랭킹전에서 승리하여 %spoint 획득하였습니다."
			sprintf(buffer, (char*)G_STRING(IDS_GUILD_RANKWAR_RESULT_WIN), point);
			g_pNk2DFrame->InsertPopup(buffer, TYPE_GUILD_CONFIRM, POPUP_OK);
		}
		break;
	case 16:
		{
			char point[256];
			arg = AnyOneArg(arg, point);

			char buffer[256];
			// IDS_GUILD_RANKWAR_RESULT_LOSE	"길드 랭킹전에서 패하여 %spoint 획득하였습니다."
			sprintf(buffer, (char*)G_STRING(IDS_GUILD_RANKWAR_RESULT_LOSE), point);
			g_pNk2DFrame->InsertPopup(buffer, TYPE_GUILD_CONFIRM, POPUP_OK);
		}
		break;
	case 17:
		{
			char point[256];
			arg = AnyOneArg(arg, point);

			char buffer[256];
			// IDS_GUILD_RANKWAR_RESULT_TIE	"길드 랭킹전에서 무승부로 %spoint 획득하였습니다."
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

void guild_rankwar_open(char *arg, CUIMgr *pMgr) ///071102 길드랭킹전 패킷 받기 버그 수정.
{
	arg = AnyOneArg(arg, g_guildbuf);

	int nMain = atoi(g_guildbuf);		// 1:1, 깃발전, 장애물

	int i = 0;
	int nCount[CBattleRank::BT_TOTAL]; // 참가 길드수

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
	case 1 :	// 깃발전
		{

		}
		break;
	case 2 :	// 장애물
		break;
	}
}

void guild_rankwar_req(char *arg, CUIMgr *pMgr)
{
	arg = AnyOneArg(arg, g_guildbuf);

	switch(atoi(g_guildbuf))
	{
	case 0 : // IDS_GUILD_RANKWAR_JOIN_FAIL_GLEVEL "랭킹전을 신청 하실 수 없습니다."
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_RANKWAR_JOIN_FAIL_GLEVEL), TYPE_GUILD_CONFIRM, 42);
		break;
	case 1 : // 신청허가
		{
			g_pNk2DFrame->GetUIWindow()->OnOpenWindow(WINDOW_GUILD_BATTLE_ACCEPTMEMBER);

			CBattleAcceptMember* pTemp = (CBattleAcceptMember*)g_pNk2DFrame->GetUIWindow()->GetWindow(WINDOW_GUILD_BATTLE_ACCEPTMEMBER);

			if(pTemp)	pTemp->SetSubType(g_pNk2DFrame->GetGuildSystemManager()->GetSubType());

		}
		break;
	case 2 : // IDS_GUILD_INVASION_FAIL_GULVL "직책이 맞지 않아 신청이 불가능합니다."
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_INVASION_FAIL_GULVL), TYPE_GUILD_CONFIRM, 42);
		break;
	}
}

void guild_rankwar_list(char *arg, CUIMgr *pMgr)
{
	arg = AnyOneArg(arg, g_guildbuf);

	switch(atoi(g_guildbuf))
	{
	case 0 : // IDS_GUILD_RANKWAR_JOIN_FAIL "길드 랭킹전에 참가실패했습니다."
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_RANKWAR_JOIN_FAIL), TYPE_GUILD_CONFIRM, 42);
		break;
	case 1 : // IDS_GUILD_RANKWAR_JOIN_SUC "길드 랭킹전에 참가했습니다."
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
	case 0: // IDS_GUILD_RANKWAR_ZONE_GOTO_FAIL "길드 랭킹존으로 입장할 수 없습니다."
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_RANKWAR_ZONE_GOTO_FAIL), TYPE_GUILD_CONFIRM, POPUP_OK );
		break;

	case 1: // IDS_GUILD_RANKWAR_ZONE_GOTO_SUC "길드 랭킹존으로 이동합니다."
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_RANKWAR_ZONE_GOTO_SUC), TYPE_GUILD_CONFIRM, POPUP_GUILD_RANKWAR_INVITE);
		// pCMyApp->m_pUIMgr->PrepareChangeWorld(TRUE, WORLD_GUILD_WAR, 0);
		break;
	case 2: // IDS_GUILD_RANKWAR_ZONE_GOTO_FAIL "길드 랭킹존으로 입장할 수 없습니다."
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

	//g_pNk2DFrame->InsertPopup("랭킹전이 시작 되었습니다.", TYPE_GUILD_CONFIRM, 52);
}

void guild_rankwar_end(char *arg, CUIMgr *pMgr)
{
	g_pNk2DFrame->ShowControlGuildBattle(false);
	// IDS_GUILD_RANKWAR_END "길드 랭킹전이 종료되었습니다."
	g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_RANKWAR_END), TYPE_GUILD_CONFIRM, 52);
}

void guild_rankwar_kcount(char *arg, CUIMgr *pMgr)
{
	arg = AnyOneArg(arg, g_guildbuf);	// 첫번째 길드 점수

	int AKillCount = atoi(g_guildbuf);

	arg = AnyOneArg(arg, g_guildbuf);	// 두번째 길드 점수

	int BKillCount = atoi(g_guildbuf);

	g_pNk2DFrame->GetControlGuildBattle()->SetTeamKillcount( AKillCount, BKillCount);
}

void guild_rankwar_goworld(char *arg, CUIMgr *pMgr)
{
	arg = AnyOneArg(arg, g_guildbuf);	// 존번호

	int Zone = atoi(g_guildbuf);

	arg = AnyOneArg(arg, g_guildbuf);	// 서브번호

	int Sub = atoi(g_guildbuf);

	if(pMgr) pMgr->PrepareChangeWorld(TRUE, Zone, 0, Sub);
}

void guild_rankwar_cancel(char *arg, CUIMgr *pMgr)
{
	arg = AnyOneArg(arg, g_guildbuf);

	int nResult = atoi(g_guildbuf);	// 0 : 취소실패, 1: 취소성공
}

void guild_rankwar_get(char *arg, CUIMgr *pMgr)
{
	arg = AnyOneArg(arg, g_guildbuf);

	int nResult = atoi(g_guildbuf); // 0 : 지급할 아이템이 없는 경우, 1: 지급 성공, 2: 인벤토리가 꽉 찬 경우

	switch(nResult)
	{
	case 0: // IDS_GUILD_GIFT_ITEM_FAIL "지급 받을 아이템이 없습니다."
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_GIFT_ITEM_FAIL), TYPE_GUILD_CONFIRM, 55);
		break;
	case 1: // IDS_GUILD_GIFT_ITEM_SUC "승급 문장을 지급받았습니다."
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_GIFT_ITEM_SUC), TYPE_GUILD_CONFIRM, 55);
		break;
	case 2: // IDS_CASHSHOP_ERROR3 "인벤토리에 빈 공간이 부족합니다."
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
	case 0: // IDS_GUILD_ITEM_COMPOSITION_FAIL "아이템을 합성할 수 없습니다"
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_ITEM_COMPOSITION_FAIL), TYPE_GUILD_CONFIRM, 55);
		break;
	case 1: // IDS_GUILD_ITEM_COMPOSITION_SUC "아이템을 합성했습니다."
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_ITEM_COMPOSITION_SUC), TYPE_GUILD_CONFIRM, 55);
		g_pNk2DFrame->GetUIWindow()->CloseTopWindow(); // 합성이 완료되면 닫아준다.
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

	int add_or_delete = atoi(g_guildbuf);		// 추가냐 삭제냐 0 : 삭제, 1 : 추가

	arg = AnyOneArg(arg, g_guildbuf);

	int attack_or_defense = atoi(g_guildbuf);

	arg = AnyOneArg(arg, g_guildbuf);

	int guild_index = atoi(g_guildbuf);			// 길드 인덱스

	arg = AnyOneArg(arg, g_guildbuf);

	char szTmp[128] = {0,};

	strcpy(szTmp, g_guildbuf);					// 길드명

	t_Info time_invasion;

	arg = AnyOneArg(arg, g_guildbuf);

	time_invasion.nYear = atoi(g_guildbuf);	// 시 간

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
		Info_Invasion.s_Invasion_flag = INVASION_DEFENSE;	// 당하는거다 그래서 방어 해야 된다.
	else if(attack_or_defense == 1)
		Info_Invasion.s_Invasion_flag = INVASION_ATTACK;	// 공격한다...다 죽여...

	Info_Invasion.s_guild_Index = guild_index;

	strcpy(Info_Invasion.s_guild_Name, szTmp);

	Info_Invasion.s_time = time_invasion;

	if(add_or_delete == 0)	// 삭제
		pGuildData->Delete_Invasion(Info_Invasion);

	if(add_or_delete == 1)  // 추가
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
	case 0 :	// IDS_GUILD_INVASION_FAIL "조건이 맞지 않습니다."
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_INVASION_FAIL), TYPE_GUILD_CONFIRM, 42);
		break;
	case 1 :	// IDS_GUILD_INVASION_START "길드 침략이 시작됩니다."
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_INVASION_START), TYPE_GUILD_CONFIRM, POPUP_GUILD_INVASION_FINISH_RES_OK);
		break;
	case 2 :	// IDS_GUILD_INVASION_ING "이미 침략중인 길드입니다."
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_INVASION_ING), TYPE_GUILD_CONFIRM, 42);
		break;
	case 3 :	// IDS_GUILD_INVASION_FAIL_LAIM "라임이 부족합니다."
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_INVASION_FAIL_LAIM), TYPE_GUILD_CONFIRM, 42);
		break;
	case 4 :	// IDS_GUILD_INVASION_FAIL_GULVL "직책이 맞지 않아 신청이 불가능합니다."
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_INVASION_FAIL_GULVL), TYPE_GUILD_CONFIRM, 42);
		break;
	case 5 :	// IDS_GUILD_INVASION_FAIL_GULVL "직책이 맞지 않아 신청이 불가능합니다."
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
		arg = AnyOneArg(arg, g_guildbuf);	// 길드명
		arg = AnyOneArg(arg, g_guildbuf1);  // 길드마스터명

		char szTmp[256] = {0,};

		CGuildSystemManager* pSystem = g_pNk2DFrame->GetGuildSystemManager();

		switch( g_pRoh->CheckInvasionGNameType(g_guildbuf) )
		{
		case INVASION_ATTACK :	// 화해
			pSystem->set_tempbuffer1(g_guildbuf);
			pSystem->set_tempbuffer2(g_guildbuf1);
			// IDS_GUILD_INVASION_PLZ_PEASE	"%s 길드에 화해을 요청하였습니다. 화해를 수락하시겠습니까?"
			sprintf(szTmp, (char*)G_STRING(IDS_GUILD_INVASION_PLZ_PEASE), g_guildbuf);
			g_pNk2DFrame->InsertPopup(szTmp, TYPE_GUILD_INVASION_CONFIRM_CANCEL, 53);
			break;
		case INVASION_DEFENSE : // 종결
			pSystem->set_tempbuffer1(g_guildbuf);
			pSystem->set_tempbuffer2(g_guildbuf1);
			// IDS_GUILD_INVASION_PLZ_PEASE_1 "%s 길드에 종결을 요청하였습니다. 화해를 수락하시겠습니까?"
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

		if( atoi(g_guildbuf2) == 3 ) // 오류 예외처리
			return;

		arg = AnyOneArg(arg, g_guildbuf);	// 길드명
		arg = AnyOneArg(arg, g_guildbuf1);  // 길드마스터명


		switch(atoi(g_guildbuf2))
		{
		case 0 : // IDS_GUILD_INVASION_PLZ_PEASE_FAIL "%s 길드에서 화해 요청을 거절 했습니다."
			sprintf( szTmp, (char*)G_STRING(IDS_GUILD_INVASION_PLZ_PEASE_FAIL), g_guildbuf);
			g_pNk2DFrame->InsertPopup(szTmp, TYPE_GUILD_CONFIRM, 42);
			break;

		case 1 :		// 수락
			{
				CBattleInvasionInfo* pInvasion = (CBattleInvasionInfo*)g_pNk2DFrame->GetUIWindow()->GetWindow(WINDOW_GUILD_BATTLE_INVASION_INFO);

				//if(pInvasion)
				//	pInvasion->EraseData();
				// IDS_GUILD_INVASION_PLZ_PEASE_SUC "%s 길드에서 화해 요청을 수락 했습니다"
				sprintf( szTmp, (char*)G_STRING(IDS_GUILD_INVASION_PLZ_PEASE_SUC), g_guildbuf);
				g_pNk2DFrame->InsertPopup(szTmp, TYPE_GUILD_CONFIRM, 42);

			}
			break;

		case 2 : // IDS_GUILD_INVASION_PLZ_PEASE_FAIL_MASTER_OFFLINE "%s 길드의 길드 마스터가 오프라인 상태입니다."
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
	case 0: // 가입 요청 취소
		g_pNk2DFrame->GetControlGuild()->SetRecomInfo(FALSE);
		break;

	case 1: // 가입 요청 상태
		arg = AnyOneArg(arg, g_guildbuf2);
		g_pNk2DFrame->GetControlGuild()->SetRecomInfo(TRUE, g_guildbuf2);
		break;

	}

}

void guild_master_delegation(char *arg, CUIMgr *pMgr)
{
	g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_MASTER_DELEGATION),TYPE_NOR_OKCANCLE, POPUP_MASTER_DELEGATION);
}
