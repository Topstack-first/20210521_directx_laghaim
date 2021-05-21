#include "stdafx.h"

#include <assert.h>

#include "wanted_network_func.h"

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
#include "wanted_register.h"
#include "wanted_my_menu.h"
#include "wanted_list.h"
#include "g_stringmanager.h"

#include "window_resource.h"

char g_wantedbuf[256] = "";
char g_wantedbuf1[256] = "";
char g_wantedbuf2[256] = "";
char g_wantedbuf3[256] = "";

int  g_wantedii[20] = {0,};

void wanted_reg(char *arg, CUIMgr *pMgr);
void wanted_reg_t(char *arg, CUIMgr *pMgr);
void wanted_reg_r(char *arg, CUIMgr *pMgr);

void wanted_my(char *arg, CUIMgr *pMgr);
void wanted_my_list(char *arg, CUIMgr *pMgr);
void wanted_my_list_s(char *arg, CUIMgr *pMgr);
void wanted_my_list_r(char *arg, CUIMgr *pMgr);
void wanted_my_list_e(char *arg, CUIMgr *pMgr);
void wanted_my_list_cancel(char *arg, CUIMgr *pMgr);

void wanted_list(char *arg, CUIMgr *pMgr);
void wanted_list_s(char *arg, CUIMgr *pMgr);
void wanted_list_r(char *arg, CUIMgr *pMgr);
void wanted_list_e(char *arg, CUIMgr *pMgr);

void wanted_reward(char *arg, CUIMgr *pMgr);

void wanted_siren(char *arg, CUIMgr *pMgr);


CWantedCmd* CWantedCmd::Create(CUIMgr *pMgr)
{
	return new CWantedCmd(pMgr);
}

CWantedCmd::CWantedCmd(CUIMgr *pMgr)
{
	m_pUIMgr = pMgr;

	wanted_addcmd("reg"    , wanted_reg    );
	wanted_addcmd("my"     , wanted_my     );
	wanted_addcmd("list"   , wanted_list   );
	wanted_addcmd("reward" , wanted_reward );
	wanted_addcmd("mark"   , wanted_siren  );

}

CWantedCmd::~CWantedCmd()
{
}

BOOL CWantedCmd::wanted_addcmd(char *cmd, void cmd_p(char *, CUIMgr *))
{
	m_mapFunc.insert(std::make_pair(cmd, &cmd_p));
	return TRUE;
}

void CWantedCmd::wanted_run(char* cmd, char *arg)
{
	MAP_FUNC::iterator itFind = m_mapFunc.find(cmd);
	if( itFind == m_mapFunc.end() )
		return;

	itFind->second(arg, m_pUIMgr);
}

void wanted_reg(char *arg, CUIMgr *pMgr)
{
	arg = AnyOneArg(arg, g_wantedbuf);

	if(!strcmp("t", g_wantedbuf))
	{
		wanted_reg_t(arg, pMgr);
	}
	else if(!strcmp("r", g_wantedbuf))
	{
		wanted_reg_r(arg, pMgr);
	}
}

void wanted_reg_t(char *arg, CUIMgr *pMgr)
{
	arg = AnyOneArg(arg, g_wantedbuf);

	switch(atoi(g_wantedbuf))
	{
	case 0 :	// 실패 --> 그런 캐릭 없어..
		{
			if(g_pNk2DFrame->GetTgaUIWindow()->IsOpenWindow(WINDOW_TGA_WANTED_REGISTER))
			{
				CWantedRegister* pRegister =  (CWantedRegister*)g_pNk2DFrame->GetTgaUIWindow()->GetWindow(WINDOW_TGA_WANTED_REGISTER);

				pRegister->SetRaceName(" ");
			}
			// IDS_WANTED_REG_FAIL_SERCH_CHA "존재하지 않는 캐릭터입니다."
			g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_WANTED_REG_FAIL_SERCH_CHA), TYPE_WANTED_CONFIRM, 52);
		}
		break;
	case 1 :	// 성공
		{
			arg = AnyOneArg(arg, g_wantedbuf);	// 종 족

			int nRace = atoi(g_wantedbuf);

			if(g_pNk2DFrame->GetTgaUIWindow()->IsOpenWindow(WINDOW_TGA_WANTED_REGISTER))
			{
				CWantedRegister* pRegister =  (CWantedRegister*)g_pNk2DFrame->GetTgaUIWindow()->GetWindow(WINDOW_TGA_WANTED_REGISTER);

				pRegister->SetRaceName(g_pNk2DFrame->GetWindowRes()->GetRaceName(nRace));
			}
		}
		break;
	case 2 :	// 실패 --> 이미 등록이 되어 있어..
		{
			if(g_pNk2DFrame->GetTgaUIWindow()->IsOpenWindow(WINDOW_TGA_WANTED_REGISTER))
			{
				CWantedRegister* pRegister =  (CWantedRegister*)g_pNk2DFrame->GetTgaUIWindow()->GetWindow(WINDOW_TGA_WANTED_REGISTER);

				pRegister->SetRaceName(" ");
			}
			// IDS_WANTED_REG_FAIL_ING "이미 수배중인 캐릭터입니다."
			g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_WANTED_REG_FAIL_ING), TYPE_WANTED_CONFIRM, 52);
		}
		break;
	case 3 :	// 실패 --> 이미 등록이 되어 있어..
		{
			if(g_pNk2DFrame->GetTgaUIWindow()->IsOpenWindow(WINDOW_TGA_WANTED_REGISTER))
			{
				CWantedRegister* pRegister =  (CWantedRegister*)g_pNk2DFrame->GetTgaUIWindow()->GetWindow(WINDOW_TGA_WANTED_REGISTER);

				pRegister->SetRaceName(" ");
			}
			// IDS_WANTED_REG_FAIL_YOU_WANTED "자신을 수배할 수 없습니다."
			g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_WANTED_REG_FAIL_YOU_WANTED), TYPE_WANTED_CONFIRM, 52);
		}
		break;
	}
}
void wanted_reg_r(char *arg, CUIMgr *pMgr)
{
	arg = AnyOneArg(arg, g_wantedbuf);

	switch(atoi(g_wantedbuf))
	{
	case 0:	// IDS_WANTED_REG_FAIL "수배자 등록에 실패하였습니다."
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_WANTED_REG_FAIL), TYPE_WANTED_CONFIRM, 52);
		break;
	case 1:	// IDS_WANTED_REG_SUC "수배자 등록이 완료 되었습니다."
		g_pNk2DFrame->GetTgaUIWindow()->AllCloseWindow();
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_WANTED_REG_SUC), TYPE_WANTED_CONFIRM, 52);
		break;
	case 2: // IDS_WANTED_REG_FAIL_LAIM "라임이 부족합니다."
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_WANTED_REG_FAIL_LAIM), TYPE_WANTED_CONFIRM, 52);
		break;
	case 3: // IDS_WANTED_REG_FAIL_ING "이미 수배중인 캐릭터입니다."
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_WANTED_REG_FAIL_ING), TYPE_WANTED_CONFIRM, 52);
		break;
	case 4: // IDS_GUILD_MINIMUM_LAIM "최소 라임이 부족합니다."
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_MINIMUM_LAIM), TYPE_WANTED_CONFIRM, 52);
		break;
	}
}

void wanted_my(char *arg, CUIMgr *pMgr)
{
	arg = AnyOneArg(arg, g_wantedbuf);

	if(!strcmp("list", g_wantedbuf))
	{
		wanted_my_list(arg, pMgr);
	}
	else if(!strcmp("cancel", g_wantedbuf))
	{
		wanted_my_list_cancel(arg, pMgr);
	}
}

void wanted_my_list(char *arg, CUIMgr *pMgr)
{
	arg = AnyOneArg(arg, g_wantedbuf);

	if(!strcmp("s", g_wantedbuf))
	{
		wanted_my_list_s(arg, pMgr);
	}
	else if(!strcmp("r", g_wantedbuf))
	{
		wanted_my_list_r(arg, pMgr);
	}
	if(!strcmp("e", g_wantedbuf))
	{
		wanted_my_list_e(arg, pMgr);
	}
}

void wanted_my_list_s(char *arg, CUIMgr *pMgr)
{
	arg = AnyOneArg(arg, g_wantedbuf);

	g_pNk2DFrame->DeleteAllPopup();

	CWantedMyMenu* pTemp = (CWantedMyMenu*)g_pNk2DFrame->GetTgaUIWindow()->GetWindow(WINDOW_TGA_WANTED_MYMENU);

	pTemp->list_all_reset();

	// IDS_WANTED_LIST_LOADING "리스트 받기를 시작합니다,"
	g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_WANTED_LIST_LOADING), TYPE_WANTED_CONFIRM, 52);
}

void wanted_my_list_r(char *arg, CUIMgr *pMgr)
{
	arg = AnyOneArg(arg, g_wantedbuf);

	wanted_info	tmpInfo;

	strcpy(tmpInfo.wanted_name, g_wantedbuf);

	arg = AnyOneArg(arg, g_wantedbuf);

	tmpInfo.wanted_laim = atoi(g_wantedbuf);

	arg = AnyOneArg(arg, g_wantedbuf);

	tmpInfo.wanted_race = atoi(g_wantedbuf);

	arg = AnyOneArg(arg, g_wantedbuf);

	tmpInfo.wanted_zone = atoi(g_wantedbuf);

	arg = AnyOneArg(arg, g_wantedbuf);

	tmpInfo.wanted_remaintime.nDay = atoi(g_wantedbuf);

	arg = AnyOneArg(arg, g_wantedbuf);

	tmpInfo.wanted_remaintime.nHour = atoi(g_wantedbuf);

	arg = AnyOneArg(arg, g_wantedbuf);

	tmpInfo.wanted_remaintime.nMin = atoi(g_wantedbuf);

	arg = AnyOneArg(arg, g_wantedbuf);

	tmpInfo.wanted_remaintime.nSec = atoi(g_wantedbuf);


	CWantedMyMenu* pTemp = (CWantedMyMenu*)g_pNk2DFrame->GetTgaUIWindow()->GetWindow(WINDOW_TGA_WANTED_MYMENU);

	if(pTemp)	pTemp->Insert_list(	tmpInfo );
}

void wanted_my_list_e(char *arg, CUIMgr *pMgr)
{
	arg = AnyOneArg(arg, g_wantedbuf);

	g_pNk2DFrame->DeleteAllPopup();

	//g_pNk2DFrame->InsertPopup("리스트를 성공적으로 받았습니다.", TYPE_WANTED_CONFIRM, 52);
}

void wanted_my_list_cancel(char *arg, CUIMgr *pMgr)
{
	arg = AnyOneArg(arg, g_wantedbuf);

	switch(atoi(g_wantedbuf))
	{
	case 0 : // IDS_WANTED_LIST_LOAD_FAIL "실패하였습니다"
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_WANTED_LIST_LOAD_FAIL), TYPE_WANTED_CONFIRM, 52);
		break;

	case 1 : // IDS_WANTED_MYWANTED_CANCEL_SUC "수수료를 뺀 현상금 %d라임을 돌려 받았습니다."
		{
			arg = AnyOneArg(arg, g_wantedbuf);
			int nLaim1 = atoi(g_wantedbuf);		// 지급받은 라임

			char temp[512] = {0,};

			sprintf(temp, (char*)G_STRING(IDS_WANTED_MYWANTED_CANCEL_SUC), nLaim1 );

			g_pNk2DFrame->InsertPopup(temp, TYPE_WANTED_CONFIRM, 52);

			CWantedMyMenu* mymenu = (CWantedMyMenu*)g_pNk2DFrame->GetTgaUIWindow()->GetWindow(WINDOW_TGA_WANTED_MYMENU);

			mymenu->list_all_reset();	// 다시 다 지우고 새로 받는다 ㅠ.ㅠ
			g_pTcpIp->SendNetMessage("wanted my list\n");

		}
		break;

	case 2: // IDS_WANTED_MYWANTED_CANCEL_FAIL_LAIM "인벤에 더이상 라임을 넣을 수 없습니다."
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_WANTED_MYWANTED_CANCEL_FAIL_LAIM), TYPE_WANTED_CONFIRM, 52);
		break;
	}
}

void wanted_list(char *arg, CUIMgr *pMgr)
{
	arg = AnyOneArg(arg, g_wantedbuf);

	if(!strcmp("s", g_wantedbuf))
	{
		wanted_list_s(arg, pMgr);
	}
	else if(!strcmp("r", g_wantedbuf))
	{
		wanted_list_r(arg, pMgr);
	}
	if(!strcmp("e", g_wantedbuf))
	{
		wanted_list_e(arg, pMgr);
	}
}

void wanted_list_s(char *arg, CUIMgr *pMgr)
{
	arg = AnyOneArg(arg, g_wantedbuf);

	g_pNk2DFrame->DeleteAllPopup();

	CWantedList* ptemp = (CWantedList*)g_pNk2DFrame->GetTgaUIWindow()->GetWindow(WINDOW_TGA_WANTED_ALLLIST);
	ptemp->list_all_reset();

	// IDS_WANTED_LIST_LOADING "리스트 받기를 시작합니다,"
	g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_WANTED_LIST_LOADING), TYPE_WANTED_CONFIRM, 52);
}

void wanted_list_r(char *arg, CUIMgr *pMgr)
{
	arg = AnyOneArg(arg, g_wantedbuf);

	wanted_info tmpInfo;

	strcpy(tmpInfo.wanted_name, g_wantedbuf);

	arg = AnyOneArg(arg, g_wantedbuf);

	tmpInfo.wanted_laim = atoi(g_wantedbuf);

	arg = AnyOneArg(arg, g_wantedbuf);

	tmpInfo.wanted_race = atoi(g_wantedbuf);

	arg = AnyOneArg(arg, g_wantedbuf);

	tmpInfo.wanted_zone = atoi(g_wantedbuf);

	arg = AnyOneArg(arg, g_wantedbuf);

	tmpInfo.wanted_remaintime.nDay = atoi(g_wantedbuf);

	arg = AnyOneArg(arg, g_wantedbuf);

	tmpInfo.wanted_remaintime.nHour = atoi(g_wantedbuf);

	arg = AnyOneArg(arg, g_wantedbuf);

	tmpInfo.wanted_remaintime.nMin = atoi(g_wantedbuf);

	arg = AnyOneArg(arg, g_wantedbuf);

	tmpInfo.wanted_remaintime.nSec = atoi(g_wantedbuf);

	CWantedList* pTemp = (CWantedList*)g_pNk2DFrame->GetTgaUIWindow()->GetWindow(WINDOW_TGA_WANTED_ALLLIST);

	if(pTemp )	pTemp->Insert_list(tmpInfo);
}

void wanted_list_e(char *arg, CUIMgr *pMgr)
{
	arg = AnyOneArg(arg, g_wantedbuf);

	g_pNk2DFrame->DeleteAllPopup();

	//g_pNk2DFrame->InsertPopup("등록된 리스트를 모두 받았습니다.", TYPE_WANTED_CONFIRM, 52);
}

void wanted_reward(char *arg, CUIMgr *pMgr)
{
	arg = AnyOneArg(arg, g_wantedbuf);

	switch(atoi(g_wantedbuf))
	{
	case 0 : // IDS_WANTED_COMPLETE_FAIL_LAIM "지급받을 현상금이 없습니다."
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_WANTED_COMPLETE_FAIL_LAIM), TYPE_WANTED_CONFIRM, 52);
		break;

	case 1 :	// 전부받음
		{
			arg = AnyOneArg(arg, g_wantedbuf);
			int nLaim1 = atoi(g_wantedbuf);		// 지급받은 라임
			arg = AnyOneArg(arg, g_wantedbuf);
			int nLaim2 = atoi(g_wantedbuf);		// 지급받지 못한 라임

			char temp[512] = {0,};

			if( nLaim2 == 0) // IDS_WANTED_COMPLETE_SUC_LAIM01 "현상금 %d라임을 지급 받았습니다."
				sprintf(temp, (char*)G_STRING(IDS_WANTED_COMPLETE_SUC_LAIM01), nLaim1 );
			else // IDS_WANTED_COMPLETE_SUC_LAIM02 "현상금 %d라임을 지급 받았습니다. (남은 현상금 : %d라임)"
				sprintf(temp, (char*)G_STRING(IDS_WANTED_COMPLETE_SUC_LAIM02), nLaim1, nLaim2 );

			g_pNk2DFrame->InsertPopup(temp, TYPE_WANTED_CONFIRM, 52);
		}
		break;

	case 2: // IDS_WANTED_MYWANTED_CANCEL_FAIL_LAIM "인벤에 더이상 라임을 넣을 수 없습니다."
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_WANTED_MYWANTED_CANCEL_FAIL_LAIM), TYPE_WANTED_CONFIRM, 52);
		break;
	}
}

void wanted_siren(char *arg, CUIMgr *pMgr)
{
	arg = AnyOneArg(arg, g_wantedbuf);

	int flag = atoi(g_wantedbuf);

	arg = AnyOneArg(arg, g_wantedbuf);

	int idx = atoi(g_wantedbuf);

	pMgr->Siren_flag(flag, idx);
}