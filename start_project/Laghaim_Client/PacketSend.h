#ifndef _PACKET_SEND_H_
#define _PACKET_SEND_H_

#include "info_define.h"
#include "sheet.h"

void SendAddFriend(const int& order, const std::string& name);
void SendDelFriend(const int& order, const int& index);
void SendRefusalFriend(const int& order, const std::string& name);
void SendFishThrow(const int& wormType, const bool& success);
void SendLogin(const int& type, const bool& web, const bool& flag, const bool& dev, const char* id, const char* pw);
void SendEventMadCharName(const std::string& charName);
void SendCharAttack(const tBYTE charType, const tINT targetCharIndex, const tBOOL haste);
void SendCharUseSkill(const tINT& skillIndex);
void SendCharUseSkill_Position(const tINT& skillIndex, const tFLOAT& x, const tFLOAT& z);
void SendCharUseSkill_Target(const tINT& skillIndex, const tBYTE& targetCharType, const tINT& targetCharIndex);
void SendCharUseSkill_TargetValue2(const tINT& skillIndex, const tBYTE& targetCharType, const tINT& targetCharIndex, const tINT& skillValue1, const tINT& skillValue2);
void SendCharUseSkill_TargetValue3(const tINT& skillIndex, const tBYTE& targetCharType, const tINT& targetCharIndex, const tINT& skillValue1, const tINT& skillValue2, const tINT& skillValue3);
void SendCharUseSkill_DStone(const tINT& skillIndex, const tINT& dir, const tFLOAT& x, const tFLOAT& z);
void SendCharUseSkill_Hide(const tINT& skillIndex, const tBOOL& on);
void SendCharUseSkill_StoneBash1(const tINT& skillIndex, const tINT& stoneNpcIndex);
void SendCharUseSkill_MultiTarget(const tINT& skillIndex, const tBYTE& targetCharType, const std::vector<tINT>& vecMobIndex);
void SendCharUseSkill_MultiTargetStep(const tINT& skillIndex, const tBYTE& targetCharType, const tINT& targetCharIndex, const int& step, const std::vector<tINT>& vecMobIndex);
void SendCharUseSkill_MultiTargetAll(const tINT& skillIndex, const std::vector<tINT>& vecCharIndex, const std::vector<tINT>& vecMobIndex);
void SendFishRewardListReq();
void SendFishReward(const int grade, const int index);
void SendMasterDelegation(const bool confirm);
void SendDecardiBattleReq();
void SendChatMutliTarget(const tBYTE btChatType, const std::string& strChat);
void SendChatTarget(const tBYTE btChatType, const std::string& strName, const std::string& strChat);
void SendLotto(const std::vector<tINT>& numbers);
void SendLottoList(const tINT page);
void SendShout(const tINT itemType, const tINT slotNum, const tINT itemIndex, const std::string& strShout);
void SendShopGetLaim();
void SendBingoInfo();
void SendBingoReg(const tINT type);
void SendBingoPresent(const tINT index);
void SendPromotion();
void SendPetSkillInit();
void SendGearUpgrade(const tINT where);
void SendViewNoteMessageBox(const tINT type, const tINT nPage);
void SendNoteMessageSend(const std::string& name, const std::string& title, const std::string& content);
void SendNoteMessageDel(const tINT type, const tINT index);
void SendNoteMessageRead(const tINT type, const tINT index);
void SendGetAdmin(const std::string& strPassword);
void SendGiftTime();
void SendBaseballHit(const int type, const bool success);
void SendBaseballRewardListReq();
void SendBaseballReward(const int grade, const int index);
void SendLottery();
void SendSnakeDiceThrow();
void SendSnakeDicePresentList();
void SendSnakeDiceGetPresent(const tINT index);
void SendPetEquip(const tINT pack, const tINT x, const tINT y);
	
#endif // _PACKET_SEND_H_
