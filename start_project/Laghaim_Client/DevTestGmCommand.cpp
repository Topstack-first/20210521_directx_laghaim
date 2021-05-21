#include "stdafx.h"
#include "DevTestGmCommand.h"
#include <iostream>
#include <string.h>
#include <string>
#include "Def.h"
#include "TcpUtil.h"
#include "main.h"
#include "NkMob.h"
#include "BuffItemMgr.h"
#include "ItemProto.h"
#include "Nk2DFrame.h"
#include "ControlUpgrade.h"
#include "UIMgr.h"
#include "ControlYutboard.h"
#include "Skill.h"
#include "Tcpipcon.h"
#include "LHGlobal_String.h"
#include "LHUtil_Text.h"
#include "ItemOutlineMgr.h"
#include "ControlFishing.h"
#include "CommonConfig.h"
#include "info_define.h"
#include "packet_define.h"
#include "PacketProc.h"
#include "LetterManager.h"
#include "ControlLetterWindow.h"

extern BOOL g_bAdmin;
extern BOOL g_bFFullMode;

DevTestGmCommand::DevTestGmCommand()
{
	registCmd();
}

DevTestGmCommand::~DevTestGmCommand()
{

}

bool DevTestGmCommand::run(const char* buf)
{
	char* msg = new char[MAX_PATH];
	char* msgOrg = msg;	// 지울 때 이 것으로 지운다
	char firstArg[MAX_PATH] = {0,};
	strcpy(msg, buf);
	msg[MAX_PATH-1] = '\0';

	msg = AnyOneArg(msg, firstArg);

	std::string funcname = firstArg;

	MAP_FUNC_NICK::iterator itFindNick = m_mapFuncNicks.find(funcname);
	if( itFindNick != m_mapFuncNicks.end() )
	{
		funcname = (*itFindNick).second;
	}

	MAP_FUNC::iterator itFind = m_mapFuncs.find(funcname);
	if( itFind != m_mapFuncs.end() )
	{
		bool bResult = (this->*((*itFind).second))(msg);

		delete [] msgOrg;

		return bResult;
	}

	delete [] msgOrg;

	return false;
}

bool DevTestGmCommand::registCmd()
{
	REGIST_CMD(res);
	REGIST_CMD(fullscr);
	REGIST_CMD(msrl);
	REGIST_CMD(addbuff);
	REGIST_CMD(delbuff);
	REGIST_CMD(item);
	REGIST_CMD_NICK(item, 아이템);
	REGIST_CMD(crash);
	REGIST_CMD(lock_upgrade);
	REGIST_CMD(open_friend);
	REGIST_CMD(spack);
	REGIST_CMD(thy);
	REGIST_CMD(yut);
	REGIST_CMD(gmark);
	REGIST_CMD(skillupall);
	REGIST_CMD(nocool);
	REGIST_CMD(index);
	REGIST_CMD(vnum);
	REGIST_CMD(moblist);
	REGIST_CMD_NICK(moblist, 몹번호);
	REGIST_CMD(gomob);
	REGIST_CMD(gonpc);
	REGIST_CMD(moblistall);
	REGIST_CMD_NICK(moblistall, mla);
	REGIST_CMD(wfr);
	REGIST_CMD(fish);
	REGIST_CMD(fishball);
	REGIST_CMD(rlcfg);
	REGIST_CMD(pr);
	REGIST_CMD(testpopup);
	REGIST_CMD(ac);
	REGIST_CMD(shout_test);
	REGIST_CMD(bl);
	REGIST_CMD(cboss);
	REGIST_CMD(cbe);
	REGIST_CMD(invpack);
	REGIST_CMD(invpack2);

	return true;
}

bool DevTestGmCommand::res(char* arg)
{
	char buf[MAX_PATH] = {0,};
	arg = AnyOneArg(arg, buf);

	int iRes = atoi(buf);
	RESOLUTION res = R800;

	if( iRes == 800 )
	{
		res = R800;
	}
	else if( iRes == 1024 )
	{
		res = R1024;
	}

	pCMyApp->ChangeResolution(res);

	return true;
}

bool DevTestGmCommand::fullscr(char* arg)
{
	char buf[MAX_PATH] = {0,};
	arg = AnyOneArg(arg, buf);

	int iOnOff = atoi(buf);
	if( iOnOff == 0 )
		pCMyApp->ChangeFullMode(FALSE);
	else
		pCMyApp->ChangeFullMode(TRUE);

	return true;
}

bool DevTestGmCommand::msrl(char* arg)
{
	MobScaleData::GetInstance()->LoadScript();

	return true;
}

bool DevTestGmCommand::addbuff(char* arg)
{
	char buf[MAX_PATH] = {0,};
	arg = AnyOneArg(arg, buf);
	int type = atoi(buf);
	arg = AnyOneArg(arg, buf);
	int time = atoi(buf);
	time *= 1000;
	arg = AnyOneArg(arg, buf);
	int val = atoi(buf);

	BuffItemMgr::GetInstance()->AddBuff(type, time, val);

	return true;
}

bool DevTestGmCommand::delbuff(char* arg)
{
	char buf[MAX_PATH] = {0,};
	arg = AnyOneArg(arg, buf);
	int type = atoi(buf);

	BuffItemMgr::GetInstance()->RemoveBuff(type);

	return true;
}

bool DevTestGmCommand::item(char* arg)
{
	char buf[MAX_PATH] = {0,};
	arg = AnyOneArg(arg, buf);
	if( strlen(buf) == 0 )
		return false;

	int size = g_ItemList.m_ItemCount;

	std::string findName = buf;
	std::string itemName;
	int findCount = 0;

	for(int i = 0; i < size; i++)
	{
		itemName = GET_ITEM_NAME_VNUM(g_ItemList.m_ItemArray[i].m_Vnum);

		if( std::string::npos == itemName.find(findName) )
			continue;

		sprintf(buf, "[%d] %s", g_ItemList.m_ItemArray[i].m_Vnum, itemName.c_str());
		g_pNk2DFrame->AddChatStr_ExtraInfo(buf, 0, 1, g_ItemList.m_ItemArray[i].m_Vnum);

		++findCount;
	}

	sprintf(buf, "==== [%d] item found ====", findCount);
	g_pNk2DFrame->AddChatStr(buf);
	g_pNk2DFrame->AddChatStr("");

	return true;
}

bool DevTestGmCommand::crash(char* arg)
{
	int* i = 0;
	*i = 0;

	return true;
}

bool DevTestGmCommand::lock_upgrade(char* arg)
{
	g_pNk2DFrame->GetUpgrade()->m_bSlotLock = !(g_pNk2DFrame->GetUpgrade()->m_bSlotLock);

	return true;
}

bool DevTestGmCommand::open_friend(char* arg)
{
	g_pNk2DFrame->ToggleFriend();

	return true;
}

bool DevTestGmCommand::spack(char* arg)
{
	if( pCMyApp && pCMyApp->m_pUIMgr )
	{
		pCMyApp->m_pUIMgr->ProcessOneSentence(arg);
	}

	return true;
}

bool DevTestGmCommand::thy(char* arg)
{
	char buf[MAX_PATH] = {0,};
	arg = AnyOneArg(arg, buf);
	int result = atoi(buf);

	if( g_pNk2DFrame )
	{
		((ControlYutboard*)g_pNk2DFrame->GetInterface(YUTBOARD))->SetAniYutStart(result);
	}

	return true;
}

bool DevTestGmCommand::yut(char* arg)
{
	char buf[MAX_PATH] = {0,};
	arg = AnyOneArg(arg, buf);
	int index = atoi(buf);

	YutManager::GetInstance()->SetPosIndex(index);

	return true;
}

bool DevTestGmCommand::gmark(char* arg)
{
	if( g_pNk2DFrame && pCMyApp && pCMyApp->m_pUIMgr )
	{
		int serverIndex = pCMyApp->m_pUIMgr->GetServerIndex();
		int guildIndex = g_pRoh->m_GuildIndex;

		char buf[MAX_PATH] = {0,};

		if( guildIndex == 0 )
		{
			sprintf(buf, "no guild");
		}
		else
		{
			sprintf(buf, "Server Idx : [%d]  Guild Idx : [%d]", serverIndex, guildIndex);
		}
		g_pNk2DFrame->AddChatStr(buf);
	}

	return true;
}

bool DevTestGmCommand::skillupall(char* arg)
{
	if( g_pTcpIp == NULL )
		return false;

	char buf[MAX_PATH] = {0,};
	int nCharSkillNum = g_ChaSkill.m_SkillNum;
	int nSkillIndex = -1;

	g_pTcpIp->SendNetMessage("sp_point 5000\n");

	for(int i = 0; i < nCharSkillNum; i++)
	{
		nSkillIndex = g_ChaSkill.m_SkillIndex[i];
		int jStart = g_ChaSkill.m_SkillLevel[i];
		int jEnd = g_Skills[nSkillIndex].m_MaxLevel;
		for(int j = jStart; j < jEnd; j++)
		{
			sprintf(buf, "skillup %d\n", nSkillIndex);
			g_pTcpIp->SendNetMessage(buf);
		}
	}

	return true;
}

bool g_bNoCool = false;
bool DevTestGmCommand::nocool(char* arg)
{
	g_bNoCool = !g_bNoCool;

	char buf[MAX_PATH] = {0,};
	sprintf(buf, "No Cool : %s", g_bNoCool == true ? "On" : "Off");
	g_pNk2DFrame->AddChatStr(buf);

	return true;
}

bool g_bShowIndex = false;
bool DevTestGmCommand::index(char* arg)
{
	g_bShowIndex = !g_bShowIndex;

	char buf[MAX_PATH] = {0,};
	sprintf(buf, "Show Index : %s", g_bShowIndex == true ? "On" : "Off");
	g_pNk2DFrame->AddChatStr(buf);

	return true;
}

bool g_bShowVnum = false;
bool DevTestGmCommand::vnum(char* arg)
{
	g_bShowVnum = !g_bShowVnum;

	char buf[MAX_PATH] = {0,};
	sprintf(buf, "Show Vnum : %s", g_bShowVnum == true ? "On" : "Off");
	g_pNk2DFrame->AddChatStr(buf);

	return true;
}

bool DevTestGmCommand::moblist(char* arg)
{
	SkipSpaces(&arg);
	if( strlen(arg) == 0 )
		return false;

	char buf[MAX_PATH] = {0,};
	LHGlobal_String* pGS = LHGlobal_String::GetSingletonPtr();
	LHSTRMAP_it it = pGS->m_npc_string->begin();
	LHSTRMAP_it itEnd = pGS->m_npc_string->end();

	std::string findName = arg;
	std::string mobName;
	int mobIndex = 0;
	int findCount = 0;

	for( ; it != itEnd; it++)
	{
		mobName = (*it).second;
		mobIndex = (*it).first;

		if( std::string::npos == mobName.find(findName) )
			continue;

		sprintf(buf, "[%d] %s", mobIndex, mobName.c_str());
		g_pNk2DFrame->AddChatStr_ExtraInfo(buf, 0, 2, mobIndex);

		++findCount;
	}

	sprintf(buf, "==== [%d] mob found ====", findCount);
	g_pNk2DFrame->AddChatStr(buf);
	g_pNk2DFrame->AddChatStr("");

	return true;
}

bool DevTestGmCommand::gomob(char* arg)
{
	SkipSpaces(&arg);
	if( strlen(arg) == 0 )
		return false;

	if( LHUtil_Text::IsDigit(arg) == false )
		return false;

	int mobIndex = atoi(arg);
	std::string mobName = LHGET_NPC_NAME(mobIndex);

	char buf[MAX_PATH] = {0,};
	sprintf(buf, "gomob %s\n", mobName.c_str());
	g_pTcpIp->SendNetMessage(buf);

	return true;
}

bool DevTestGmCommand::gonpc(char* arg)
{
	SkipSpaces(&arg);
	if( strlen(arg) == 0 )
		return false;

	if( LHUtil_Text::IsDigit(arg) == false )
		return false;

	int mobIndex = atoi(arg);
	std::string mobName = LHGET_NPC_NAME(mobIndex);

	char buf[MAX_PATH] = {0,};
	sprintf(buf, "gonpc %s\n", mobName.c_str());
	g_pTcpIp->SendNetMessage(buf);

	return true;
}

bool DevTestGmCommand::moblistall(char* arg)
{
	char buf[MAX_PATH] = {0,};
	std::string mobName;
	int mobIndex = 0;

	for( int i = 0; i < g_pMobDataLoading->m_nNpcNum; i++ )
	{
		mobName = GET_NPC_NAME_VNUM(g_pMobDataLoading->m_pMobData[i].nVnum);
		mobIndex = g_pMobDataLoading->m_pMobData[i].nVnum;

		sprintf(buf, "[%d] %s", mobIndex, mobName.c_str());
		g_pNk2DFrame->AddChatStr_ExtraInfo(buf, 0, 2, mobIndex);
	}

	sprintf(buf, "==== [%d] mob found ====", g_pMobDataLoading->m_nNpcNum);
	g_pNk2DFrame->AddChatStr(buf);
	g_pNk2DFrame->AddChatStr("");

	return true;
}

bool DevTestGmCommand::wfr(char* arg)
{
	ItemOutlineMgr* outlineMgr = ItemOutlineMgr::GetInstance();

	outlineMgr->Reload();

	return true;
}

bool DevTestGmCommand::fish(char* arg)
{
	g_pNk2DFrame->ShowInterfaceWindow(TRUE, FISHING);

	return true;
}

bool DevTestGmCommand::fishball(char* arg)
{
	char buf[MAX_PATH] = {0,};
	arg = AnyOneArg(arg, buf);
	int speed = atoi(buf);

	ControlFishing* board = (ControlFishing*)g_pNk2DFrame->GetInterface(FISHING);
	board->SetBallSpeed(speed);

	return true;
}

bool DevTestGmCommand::rlcfg(char* arg)
{
	CommonConfig::Instance()->Init();

	return true;
}

// Packet Repeat
bool DevTestGmCommand::pr(char* arg)
{
	char buf[MAX_PATH] = {0,};
	arg = AnyOneArg(arg, buf);
	int count = atoi(buf);	

	SkipSpaces(&arg);
	sprintf(buf, "%s\n", arg);

	for( int i = 0; i < count; i++ )
	{		
		g_pTcpIp->SendNetMessage(buf);
	}	

	return true;
}

bool DevTestGmCommand::testpopup(char* arg)
{
	g_pNk2DFrame->InsertPopup("test popup", TYPE_NOR_OKCANCELCHECK, POPUP_EVENT_MAD_CONFIRM);
	CMsgPopUp* p = g_pNk2DFrame->FindPopup(POPUP_EVENT_MAD_CONFIRM);
	p->SetCheckBoxText("오늘 다시 안보기");

	return true;
}

bool DevTestGmCommand::ac(char* arg)
{
	char buf[MAX_PATH] = {0,};
	arg = AnyOneArg(arg, buf);
	int race = atoi(buf);
	arg = AnyOneArg(arg, buf);
	int actnum = atoi(buf);

	CBiped* pBiped = g_ChaBpdList[g_ChaBpdIndex[race][actnum]].m_pBiped;

	sprintf(buf, "== AN:[%d] RACE:[%d] FR CNT:[%d] HALF:[%d]", actnum, race, pBiped->m_AnimCount, pBiped->m_bHalfFrame);
	g_pNk2DFrame->AddChatStr(buf);

	return true;
}

bool DevTestGmCommand::shout_test(char* arg)
{
	char buf[MAX_PATH] = {0,};	
	g_pNk2DFrame->AddChatShoutStr(arg);

	return true;
}

bool DevTestGmCommand::bl(char* arg)
{
	char buf[MAX_PATH] = {0,};
	arg = AnyOneArg(arg, buf);
	int err = atoi(buf);

	ResponseClient::Promotion msg;
	msg.msg.index_ = ResPromotion;
	msg.nErrorCode = err;
	memset(msg.strCoupon, 0, sizeof(msg.strCoupon));
	sprintf(msg.strCoupon, "asdbwersaseeabejm");

	rnPacket::SP packet(new rnPacket(sizeof(msg)));
	memcpy(packet->body(), &msg, sizeof(msg));
	PacketProcMgr* procMgr = PacketProcMgr::GetInstance();
	procMgr->Excute(packet);
	
	return true;
}

bool DevTestGmCommand::cboss(char* arg)
{
	int id = 1000000;
	int vnum = 879;
	float x = g_pRoh->m_wx;
	float y = g_pRoh->m_wy;
	float z = g_pRoh->m_wz;
	float dir = g_pRoh->m_dir;
	int vital = 6300000;
	pCMyApp->m_pUIMgr->AddMob(vnum, id, x, z, dir, vital);

	return true;
}

bool DevTestGmCommand::cbe(char* arg)
{
	CNkMob* pMob = pCMyApp->m_pUIMgr->FindMobById(1000000);

	if( pMob == NULL )
		return true;

	char buf[MAX_PATH] = {0,};
	arg = AnyOneArg(arg, buf);
	int type = atoi(buf);
	arg = AnyOneArg(arg, buf);
	int start_frame = atoi(buf);
	arg = AnyOneArg(arg, buf);
	float scale = atof(buf);
	if( scale == 0 )
	{
		scale = 1.0f;
	}

	arg = AnyOneArg(arg, buf);
	float fX = atof(buf);
	arg = AnyOneArg(arg, buf);
	float fY = atof(buf);
	arg = AnyOneArg(arg, buf);
	float fZ = atof(buf);

	int state = 0;
	int eff_type = 0;
	if( type == 0 )
	{
		state = MOB_ATTACK;
		eff_type = FX_BOSS_ATTACK_01;
	}
	else if( type == 1 )
	{
		state = MOB_EMPTY1;
		eff_type = FX_BOSS_ATTACK_02;
	}
	else if( type == 2 )
	{
		state = MOB_EMPTY2;
		eff_type = FX_BOSS_DEBUFF;
	}
	else if( type == 3 )
	{
		state = MOB_EMPTY3;
		eff_type = FX_BOSS_MAGIC;
	}
	
	pMob->SetMotionState(state);

	EffectSort ef_sort;
	ZeroMemory(&ef_sort, sizeof(EffectSort));
	
	ef_sort.nType = eff_type;
	ef_sort.pMobFrom = pMob;
	ef_sort.nCurFrame = start_frame;
	ef_sort.vSortPos.x = scale;
	ef_sort.vPos = D3DVECTOR(fX, fY, fZ);
	int eff_num = pCMyApp->m_pFxSet->InsertFxEffect(ef_sort);
	EffectSort* p = pCMyApp->m_pFxSet->GetEffect(eff_num);
	p->nCurFrame = start_frame;
	g_pRoh->InsertEffectNum(eff_num);	


	return true;
}

bool DevTestGmCommand::invpack(char* arg)
{
	RequestClient::CharAttack msg;
	msg.msg.type_ = 2;
	msg.msg.index_ = ReqCharAttack;
	msg.btCharType = 0;
	msg.nTargetCharIndex = 0;
	msg.bHaste = 0;

	rnPacket::SP packet(new rnPacket(sizeof(msg)));
	memcpy(packet->body(), &msg, sizeof(msg));
	g_pTcpIp->SendPacket(packet);

	return true;
}

bool DevTestGmCommand::invpack2(char* arg)
{
	g_pTcpIp->SendNetMessage("   ");

	return true;
}