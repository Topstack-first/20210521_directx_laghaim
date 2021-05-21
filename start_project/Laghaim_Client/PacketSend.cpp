#include "stdafx.h"
#include "PacketSend.h"
#include <boost/format.hpp>
#include "Tcpipcon.h"
#include "Nk2DFrame.h"
#include "g_stringmanager.h"
#include "packet_define.h"

#define SEND_PACKET(msg) \
	rnPacket::SP packet(new rnPacket(sizeof(msg))); \
	memcpy(packet->body(), &msg, sizeof(msg)); \
	g_pTcpIp->SendPacket(packet);

#define SEND_PACKET_SIZE(pData, size) \
	rnPacket::SP packet(new rnPacket(size)); \
	memcpy(packet->body(), msg, size); \
	g_pTcpIp->SendPacket(packet);

void SendAddFriend(const int& order, const std::string& name)
{
	char buf[MAX_PATH] = {0,};
	sprintf(buf, "addfriend %d %s\n", order, name.c_str());
	g_pTcpIp->SendNetMessage(buf);
}

void SendDelFriend(const int& order, const int& index)
{
	char buf[MAX_PATH] = {0,};
	sprintf(buf, "delfriend %d %d\n", order, index);
	g_pTcpIp->SendNetMessage(buf);
}

void SendRefusalFriend(const int& order, const std::string& name)
{
	char buf[MAX_PATH] = {0,};
	sprintf(buf, "refusalfriend %d %s\n", order, name.c_str());
	g_pTcpIp->SendNetMessage(buf);
}

void SendFishThrow(const int& wormType, const bool& success)
{
	char buf[MAX_PATH] = {0,};
	sprintf(buf, "fish_throw %d %d\n", wormType, success == true ? 1 : 0);
	g_pTcpIp->SendNetMessage(buf);
}

void SendLogin(const int& type, const bool& web, const bool& flag, const bool& dev, const char* id, const char* pw)
{
	char buf[MAX_PATH] = {0,};
	if( type == 0 )
	{
		sprintf(buf, "login\n");
		g_pTcpIp->SendNetMessage(buf, TRUE);
	}
	else
	{
		sprintf(buf, "play\n");
		g_pTcpIp->SendNetMessage(buf, TRUE);
	}

	sprintf(buf, "%s\n", id);
	g_pTcpIp->SendNetMessage(buf, TRUE);

	char chWeb = 'd';
	char chFlag = 'n';
	char chDev = '0';

	if( web == true )
	{
		chWeb = 'w';
	}
	if( flag == false )
	{
		chFlag = 'f';
	}
	if( dev == true )
	{
		chDev = 'c';
		chFlag = 'f';
	}

	sprintf(buf, "%s %c %c %c\n", pw, chWeb, chFlag, chDev);
	g_pTcpIp->SendNetMessage(buf, TRUE);
}

void SendEventMadCharName(const std::string& charName)
{
	int len = charName.length();
	if( len < 2 || len > 16 )
	{
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_EVENT_MAD_ERROR1), TYPE_NOR_OK, POPUP_EVENT_MAD_ERROR);
		return;
	}

	if( charName.find(" ") != std::string::npos )
	{
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_EVENT_MAD_ERROR2), TYPE_NOR_OK, POPUP_EVENT_MAD_ERROR);
		return;
	}

	char buf[MAX_PATH] = {0};
	sprintf(buf, "event_mad %s\n", charName.c_str());
	g_pTcpIp->SendNetMessage(buf);
}

void SendCharAttack(const tBYTE charType, const tINT targetCharIndex, const tBOOL haste)
{
	RequestClient::CharAttack msg;
	msg.msg.type_ = 1;
	msg.msg.index_ = ReqCharAttack;
	msg.btCharType = charType;
	msg.nTargetCharIndex = targetCharIndex;
	msg.bHaste = haste;

	SEND_PACKET(msg);	
}

void SendCharUseSkill(const tINT& skillIndex)
{
	RequestClient::CharUseSkill msg;
	memset(&msg, 0, sizeof(msg));
	msg.msg.type_ = 1;
	msg.msg.index_ = ReqCharUseSkill;
	msg.nSkillIndex = skillIndex;

	SEND_PACKET(msg);
}

void SendCharUseSkill_Position(const tINT& skillIndex, const tFLOAT& x, const tFLOAT& z)
{
	RequestClient::CharUseSkill msg;
	memset(&msg, 0, sizeof(msg));
	msg.msg.type_ = 1;
	msg.msg.index_ = ReqCharUseSkill;
	msg.nSkillIndex = skillIndex;
	msg.nX = x;
	msg.nZ = z;

	SEND_PACKET(msg);
}

void SendCharUseSkill_Target(const tINT& skillIndex, const tBYTE& targetCharType, const tINT& targetCharIndex)
{
	tBYTE targetType = targetCharType == eCharType_Char ? eTargetType_Char : eTargetType_Npc;

	tINT size = sizeof(RequestClient::CharUseSkill);
	size += sizeof(tINT);
	tBYTE* pData = new tBYTE[size];
	memset(pData, 0, size);

	RequestClient::CharUseSkill* msg = (RequestClient::CharUseSkill*)pData;
	msg->msg.type_ = 1;
	msg->msg.index_ = ReqCharUseSkill;
	msg->nSkillIndex = skillIndex;
	msg->nTargetType = targetType;
	msg->nTargetCount = 1;
	msg->nTargetIndex[0] = targetCharIndex;

	SEND_PACKET_SIZE(msg, size);

	delete [] pData;
}

void SendCharUseSkill_TargetValue2(const tINT& skillIndex, const tBYTE& targetCharType, const tINT& targetCharIndex, const tINT& skillValue1, const tINT& skillValue2)
{
	tBYTE targetType = targetCharType == eCharType_Char ? eTargetType_Char : eTargetType_Npc;

	tINT size = sizeof(RequestClient::CharUseSkill);
	size += sizeof(tINT);
	tBYTE* pData = new tBYTE[size];
	memset(pData, 0, size);

	RequestClient::CharUseSkill* msg = (RequestClient::CharUseSkill*)pData;
	msg->msg.type_ = 1;
	msg->msg.index_ = ReqCharUseSkill;
	msg->nSkillIndex = skillIndex;	
	msg->nSkillvalue1 = skillValue1;	
	msg->nSkillvalue2 = skillValue2;
	msg->nStep = skillValue1;
	msg->nStepStart = skillValue2;
	msg->nTargetType = targetType;
	msg->nTargetCount = 1;
	msg->nTargetIndex[0] = targetCharIndex;

	SEND_PACKET_SIZE(msg, size);

	delete [] pData;
}

void SendCharUseSkill_TargetValue3(const tINT& skillIndex, const tBYTE& targetCharType, const tINT& targetCharIndex, const tINT& skillValue1, const tINT& skillValue2, const tINT& skillValue3)
{
	tBYTE targetType = targetCharType == eCharType_Char ? eTargetType_Char : eTargetType_Npc;

	tINT size = sizeof(RequestClient::CharUseSkill);
	size += sizeof(tINT);
	tBYTE* pData = new tBYTE[size];
	memset(pData, 0, size);

	RequestClient::CharUseSkill* msg = (RequestClient::CharUseSkill*)pData;
	msg->msg.type_ = 1;
	msg->msg.index_ = ReqCharUseSkill;
	msg->nSkillIndex = skillIndex;	
	msg->nSkillvalue1 = skillValue1;	
	msg->nSkillvalue2 = skillValue2;
	msg->nSkillvalue3 = skillValue3;
	msg->nX = (tFLOAT)skillValue2;
	msg->nZ = (tFLOAT)skillValue3;
	msg->nTargetType = targetType;
	msg->nTargetCount = 1;
	msg->nTargetIndex[0] = targetCharIndex;

	SEND_PACKET_SIZE(msg, size);

	delete [] pData;
}

void SendCharUseSkill_DStone(const tINT& skillIndex, const tINT& dir, const tFLOAT& x, const tFLOAT& z)
{
	RequestClient::CharUseSkill msg;
	memset(&msg, 0, sizeof(msg));
	msg.msg.type_ = 1;
	msg.msg.index_ = ReqCharUseSkill;
	msg.nSkillIndex = skillIndex;
	msg.nDir = dir;
	msg.nX = x;
	msg.nZ = z;

	SEND_PACKET(msg);
}

void SendCharUseSkill_Hide(const tINT& skillIndex, const tBOOL& on)
{
	RequestClient::CharUseSkill msg;
	memset(&msg, 0, sizeof(msg));
	msg.msg.type_ = 1;
	msg.msg.index_ = ReqCharUseSkill;
	msg.nSkillIndex = skillIndex;
	msg.nHideFlag = on;

	SEND_PACKET(msg);
}

void SendCharUseSkill_StoneBash1(const tINT& skillIndex, const tINT& stoneNpcIndex)
{
	RequestClient::CharUseSkill msg;
	memset(&msg, 0, sizeof(msg));
	msg.msg.type_ = 1;
	msg.msg.index_ = ReqCharUseSkill;
	msg.nSkillIndex = skillIndex;
	msg.nDStoneIndex = stoneNpcIndex;

	SEND_PACKET(msg);
}

void SendCharUseSkill_MultiTarget(const tINT& skillIndex, const tBYTE& targetCharType, const std::vector<tINT>& vecMobIndex)
{
	tBYTE targetType = targetCharType == eCharType_Char ? eTargetType_Char : eTargetType_Npc;
	tINT mobCount = vecMobIndex.size();

	tINT size = sizeof(RequestClient::CharUseSkill);
	size += (sizeof(tINT) * mobCount);
	tBYTE* pData = new tBYTE[size];
	memset(pData, 0, size);	

	RequestClient::CharUseSkill* msg = (RequestClient::CharUseSkill*)pData;
	msg->msg.type_ = 1;
	msg->msg.index_ = ReqCharUseSkill;
	msg->nSkillIndex = skillIndex;	
	msg->nTargetType = targetType;
	msg->nTargetCount = mobCount;
	for(int i = 0; i < mobCount; i++)
	{
		msg->nTargetIndex[i] = vecMobIndex[i];
	}	

	SEND_PACKET_SIZE(msg, size);

	delete [] pData;
}

void SendCharUseSkill_MultiTargetStep(const tINT& skillIndex, const tBYTE& targetCharType, const tINT& targetCharIndex, const int& step, const std::vector<tINT>& vecMobIndex)
{
	tBYTE targetType = targetCharType == eCharType_Char ? eTargetType_Char : eTargetType_Npc;
	tINT mobCount = vecMobIndex.size();

	tINT size = sizeof(RequestClient::CharUseSkill);
	size += (sizeof(tINT) * mobCount);
	tBYTE* pData = new tBYTE[size];
	memset(pData, 0, size);	

	RequestClient::CharUseSkill* msg = (RequestClient::CharUseSkill*)pData;
	msg->msg.type_ = 1;
	msg->msg.index_ = ReqCharUseSkill;
	msg->nSkillIndex = skillIndex;
	msg->nStep = step;
	msg->nStepStart = 10;
	msg->nTargetType = targetType;
	msg->nTargetCount = mobCount;
	for(int i = 0; i < mobCount; i++)
	{
		msg->nTargetIndex[i] = vecMobIndex[i];
	}	

	SEND_PACKET_SIZE(msg, size);

	delete [] pData;
}

void SendCharUseSkill_MultiTargetAll(const tINT& skillIndex, const std::vector<tINT>& vecCharIndex, const std::vector<tINT>& vecMobIndex)
{
	{
		tBYTE targetType = eTargetType_Char;
		tINT mobCount = vecCharIndex.size();

		tINT size = sizeof(RequestClient::CharUseSkill);
		size += (sizeof(tINT) * mobCount);
		tBYTE* pData = new tBYTE[size];
		memset(pData, 0, size);	

		RequestClient::CharUseSkill* msg = (RequestClient::CharUseSkill*)pData;
		msg->msg.type_ = 1;
		msg->msg.index_ = ReqCharUseSkill;
		msg->nSkillIndex = skillIndex;	
		msg->nTargetType = targetType;
		msg->nTargetCount = mobCount;
		for(int i = 0; i < mobCount; i++)
		{
			msg->nTargetIndex[i] = vecCharIndex[i];
		}	

		SEND_PACKET_SIZE(msg, size);

		delete [] pData;
	}

	{
		tBYTE targetType = eTargetType_Npc;
		tINT mobCount = vecMobIndex.size();

		tINT size = sizeof(RequestClient::CharUseSkill);
		size += (sizeof(tINT) * mobCount);
		tBYTE* pData = new tBYTE[size];
		memset(pData, 0, size);	

		RequestClient::CharUseSkill* msg = (RequestClient::CharUseSkill*)pData;
		msg->msg.type_ = 1;
		msg->msg.index_ = ReqCharUseSkill;
		msg->nSkillIndex = skillIndex;	
		msg->nTargetType = targetType;
		msg->nTargetCount = mobCount;
		for(int i = 0; i < mobCount; i++)
		{
			msg->nTargetIndex[i] = vecMobIndex[i];
		}	

		SEND_PACKET_SIZE(msg, size);

		delete [] pData;
	}	
}

void SendFishRewardListReq()
{
	g_pTcpIp->SendNetMessage("fish_gift_list\n");
}

void SendFishReward(const int grade, const int index)
{
	char buf[MAX_PATH] = {0};
	sprintf(buf, "fish_gift_item %d %d\n", grade, index);
	g_pTcpIp->SendNetMessage(buf);
}

void SendMasterDelegation(const bool confirm)
{
	char buf[MAX_PATH] = {0};
	sprintf(buf, "mg master_dg %d\n", confirm == true ? 0 : 1);
	g_pTcpIp->SendNetMessage(buf);
}

void SendDecardiBattleReq()
{
	g_pTcpIp->SendNetMessage("dcb_req\n");
}

void SendChatMutliTarget(const tBYTE btChatType, const std::string& strChat)
{
	if( strChat.length() > CHAT_LENGTH )
		return;

	RequestClient::ChatMultiTarget msg;
	msg.msg.type_ = 1;
	msg.msg.index_ = ReqChatMultiTarget;
	msg.btType = btChatType;
	sprintf(msg.strChatMsg, "%s", strChat.c_str());	

	SEND_PACKET(msg);	
}

void SendChatTarget(const tBYTE btChatType, const std::string& strName, const std::string& strChat)
{
	if( strName.length() > USER_ID_LENGTH )
		return;

	if( strChat.length() > CHAT_LENGTH )
		return;

	RequestClient::ChatTarget msg;
	msg.msg.type_ = 1;
	msg.msg.index_ = ReqChatTarget;
	msg.btType = btChatType;
	sprintf(msg.strCharName, "%s", strName.c_str());
	sprintf(msg.strChatMsg, "%s", strChat.c_str());

	SEND_PACKET(msg);
}

void SendLotto(const std::vector<tINT>& numbers)
{
	int size = (int)numbers.size();
	if( size != 6 )
		return;		

	RequestClient::EventLottoReg msg;
	msg.msg.type_ = 1;
	msg.msg.index_ = ReqEventLottoReg;
	msg.nReg_Num0 = numbers[0];
	msg.nReg_Num1 = numbers[1];
	msg.nReg_Num2 = numbers[2];
	msg.nReg_Num3 = numbers[3];
	msg.nReg_Num4 = numbers[4];
	msg.nReg_Num5 = numbers[5];

	SEND_PACKET(msg);
}

void SendLottoList(const tINT page)
{
	RequestClient::EventLottoList msg;
	msg.msg.type_ = 1;
	msg.msg.index_ = ReqEventLottoList;
	msg.nPage = page;
	
	SEND_PACKET(msg);
}

void SendShout(const tINT /*itemType*/, const tINT slotNum, const tINT itemIndex, const std::string& strShout)
{
	if( strShout.length() > CHAT_LENGTH )
		return;

	RequestClient::MagaPhone msg;
	msg.msg.type_ = 1;
	msg.msg.index_ = ReqMagaPhone;
	msg.nQuickNum = slotNum;
	msg.nItemIndex = itemIndex;
	sprintf(msg.nSay, "%s", strShout.c_str());

	SEND_PACKET(msg);
}

void SendShopGetLaim()
{
	stPacketSubHeader msg;
	msg.msg.type_ = 1;
	msg.msg.index_ = ReqShopGetLaim;

	SEND_PACKET(msg);
}

void SendBingoInfo()
{
	stPacketSubHeader msg;
	msg.msg.type_ = 1;
	msg.msg.index_ = ReqBingoInfo;

	SEND_PACKET(msg);
}

void SendBingoReg(const tINT type)
{
	RequestClient::BingoReg msg;
	msg.msg.type_ = 1;
	msg.msg.index_ = ReqBingoReg;
	msg.nType = type;

	SEND_PACKET(msg);
}

void SendBingoPresent(const tINT index)
{
	RequestClient::BingoPresent msg;
	msg.msg.type_ = 1;
	msg.msg.index_ = ReqBingoPresent;
	msg.nPresentIndex = index;

	SEND_PACKET(msg);
}

void SendPromotion()
{
	stPacketSubHeader msg;
	msg.msg.type_ = 1;
	msg.msg.index_ = ReqPromotion;

	SEND_PACKET(msg);
}

void SendPetSkillInit()
{
	stPacketSubHeader msg;
	msg.msg.type_ = 1;
	msg.msg.index_ = ReqPetSkillInit;

	SEND_PACKET(msg);
}

void SendGearUpgrade(const tINT where)
{
	RequestClient::GearUpgrade msg;
	msg.msg.type_ = 1;
	msg.msg.index_ = ReqGearUpgrade;
	msg.nWearingNum = where;

	SEND_PACKET(msg);
}

void SendViewNoteMessageBox(const tINT type, const tINT nPage)
{
	RequestClient::ViewNoteMessageBox msg;
	msg.msg.type_ = 1;
	msg.msg.index_ = ReqNoteMesaageViewBox;
	msg.nType = type;
	msg.nPage = nPage;

	SEND_PACKET(msg);
}

void SendNoteMessageSend(const std::string& name, const std::string& title, const std::string& content)
{
	RequestClient::NoteMessageSend msg;
	msg.msg.type_ = 1;
	msg.msg.index_ = ReqNoteMessageSend;
	sprintf(msg.strReceive_name, "%s", name.c_str());
	sprintf(msg.strTitle, "%s", title.c_str());
	sprintf(msg.strContent, "%s", content.c_str());
	
	SEND_PACKET(msg);
}

void SendNoteMessageDel(const tINT type, const tINT index)
{
	RequestClient::NoteMessageDel msg;
	msg.msg.type_ = 1;
	msg.msg.index_ = ReqNoteMessageDel;
	msg.nType = type;
	msg.nIndex = index;

	SEND_PACKET(msg);
}

void SendNoteMessageRead(const tINT type, const tINT index)
{
	RequestClient::NoteMessageRead msg;
	msg.msg.type_ = 1;
	msg.msg.index_ = ReqNoteMessageRead;
	msg.nType = type;
	msg.nIndex = index;

	SEND_PACKET(msg);
}

void SendGetAdmin(const std::string& strPassword)
{
	RequestClient::GetAdmin msg;
	msg.msg.type_ = 1;
	msg.msg.index_ = ReqGetAdmin;
	sprintf(msg.strPassword, "%s", strPassword.c_str());

	SEND_PACKET(msg);
}

void SendGiftTime()
{
	stPacketSubHeader msg;
	msg.msg.type_ = 1;
	msg.msg.index_ = ReqGiftTime;

	SEND_PACKET(msg);
}

void SendBaseballHit(const int type, const bool success)
{
	RequestClient::BaseBallHit msg;
	msg.msg.type_ = 1;
	msg.msg.index_ = ReqBaseBallHit;
	msg.nBallType = type;
	msg.nHitType = success;

	SEND_PACKET(msg);
}

void SendBaseballRewardListReq()
{
	stPacketSubHeader msg;
	msg.msg.type_ = 1;
	msg.msg.index_ = ReqGetBaseBallGiftList;

	SEND_PACKET(msg);
}

void SendBaseballReward(const int grade, const int index)
{
	RequestClient::GetBaseBallGift msg;
	msg.msg.type_ = 1;
	msg.msg.index_ = ReqGetBaseBallGift;
	msg.nRatingType = grade;
	msg.nRatingCount = index;

	SEND_PACKET(msg);
}

void SendLottery()
{
	stPacketSubHeader msg;
	msg.msg.type_ = 1;
	msg.msg.index_ = ReqLottery;

	SEND_PACKET(msg);
}

void SendSnakeDiceThrow()
{
	stPacketSubHeader msg;
	msg.msg.type_ = 1;
	msg.msg.index_ = ReqSnakeDiceThrow;

	SEND_PACKET(msg);
}

void SendSnakeDicePresentList()
{
	stPacketSubHeader msg;
	msg.msg.type_ = 1;
	msg.msg.index_ = ReqSnakeDicePresentList;

	SEND_PACKET(msg);
}

void SendSnakeDiceGetPresent(const tINT index)
{
	RequestClient::SnakeDiceGetPresent msg;
	msg.msg.type_ = 1;
	msg.msg.index_ = ReqSnakeDiceGetPresent;
	msg.nSuccessCount = index;

	SEND_PACKET(msg);
}

void SendPetEquip(const tINT pack, const tINT x, const tINT y)
{
	char buf[MAX_PATH] = {0};
	sprintf(buf, "p_wear_drake %d %d %d\n", pack, x, y);
	g_pTcpIp->SendNetMessage(buf);
}
