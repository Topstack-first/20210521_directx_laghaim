#include "stdafx.h"
#include "PacketProc.h"
#include <boost/format.hpp>
#include "packet_enum.h"
#include "packet_define.h"
#include "info_define.h"

#include "Nk2DFrame.h"
#include "g_stringmanager.h"
#include "SetItemEffMgr.h"
#include "BingoManager.h"
#include "ControlBingo.h"
#include "ItemProto.h"
#include "LetterManager.h"
#include "ControlLetterWindow.h"
#include "ControlLetterRecv.h"
#include "ControlLetterSend.h"
#include "ControlBottom.h"
#include "ControlBaseball.h"
#include "ControlBaseballReward.h"
#include "BaseballManager.h"
#include "BaseballRewardManager.h"
#include "main.h"
#include "UIMgr.h"
#include "NkMob.h"
#include "NkPet.h"
#include "NkMyPet.h"
#include "AutoPlay_test.h"
#include "ControlLottery.h"
#include "ControlDiceGame.h"
#include "ControlDiceGameReward.h"
#include "DiceGameManager.h"
#include "DiceGameRewardManager.h"

//////////////////////////////////////////////////////////////////////////
// Response
void CmdResponse_Megaphone(rnPacket::SP packet);
void CmdResponse_ShopGetLaim(rnPacket::SP packet);
void CmdResponse_BingoInfo(rnPacket::SP packet);
void CmdResponse_BingoReg(rnPacket::SP packet);
void CmdResponse_BingoPresent(rnPacket::SP packet);
void CmdResponse_Promotion(rnPacket::SP packet);
void CmdResponse_PetSkillInit(rnPacket::SP packet);
void CmdResponse_GearUpgrade(rnPacket::SP packet);
void CmdResponse_ViewBox(rnPacket::SP packet);
void CmdResponse_NoteMessageSend(rnPacket::SP packet);
void CmdResponse_NoteMessageDel(rnPacket::SP packet);
void CmdResponse_NoteMessageRead(rnPacket::SP packet);
void CmdResponse_GiftTime(rnPacket::SP packet);
void CmdResponse_BaseBallHit(rnPacket::SP packet);
void CmdResponse_GetBaseBallGift(rnPacket::SP packet);
void CmdResponse_GetBaseBallGiftList(rnPacket::SP packet);
void CmdResponse_Lottery(rnPacket::SP packet);
void CmdResponse_SnakeDiceThrow(rnPacket::SP packet);
void CmdResponse_SnakeDicePresentList(rnPacket::SP packet);
void CmdResponse_SnakeDiceGetPresent(rnPacket::SP packet);

//////////////////////////////////////////////////////////////////////////
// Update
void CmdUpdate_UIClose(rnPacket::SP packet);
void CmdUpdate_SetItem(rnPacket::SP packet);
void CmdUpdate_BingoInfo(rnPacket::SP packet);
void CmdUpdate_ExtraDel(rnPacket::SP packet);
void CmdUpdate_GetItemCaru(rnPacket::SP packet);
void CmdUpdate_NewNoteMessage(rnPacket::SP packet);
void CmdUpdate_NoteMessageData(rnPacket::SP packet);
void CmdUpdate_NoteMessageContent(rnPacket::SP packet);
void CmdUpdate_GetBaseBallGiftList(rnPacket::SP packet);
void CmdUpdate_BaseBallInfoList(rnPacket::SP packet);
void CmdUpdate_BossUseSkill(rnPacket::SP packet);
void CmdUpdate_StopLagrush(rnPacket::SP packet);
void CmdUpdate_SnakeDice(rnPacket::SP packet);
void CmdUpdate_SnakeDicePresentList(rnPacket::SP packet);

//////////////////////////////////////////////////////////////////////////
// Notify
void CmdNotify_Megaphone(rnPacket::SP packet);

PacketProcMgr::PacketProcMgr()
{
	Init();
}

PacketProcMgr::~PacketProcMgr()
{

}

void PacketProcMgr::Init()
{
	// Response
	AddCmd(ResMegaPhone, &CmdResponse_Megaphone);
	AddCmd(ResShopGetLaim, &CmdResponse_ShopGetLaim);
	AddCmd(ResBingoInfo, &CmdResponse_BingoInfo);
	AddCmd(ResBingoReg, &CmdResponse_BingoReg);
	AddCmd(ResBingoPresent, &CmdResponse_BingoPresent);
	AddCmd(ResPromotion, &CmdResponse_Promotion);
	AddCmd(ResPetSkillInit, &CmdResponse_PetSkillInit);
	AddCmd(ResGearUpgrade, &CmdResponse_GearUpgrade);
	AddCmd(ResViewBox, &CmdResponse_ViewBox);
	AddCmd(ResNoteMessageSend, &CmdResponse_NoteMessageSend);
	AddCmd(ResNoteMessageDel, &CmdResponse_NoteMessageDel);
	AddCmd(ResNoteMessageRead, &CmdResponse_NoteMessageRead);
	AddCmd(ResGiftTime, &CmdResponse_GiftTime);
	AddCmd(ResBaseBallHit, &CmdResponse_BaseBallHit);
	AddCmd(ResGetBaseBallGift, &CmdResponse_GetBaseBallGift);
	AddCmd(ResGetBaseBallGiftList, &CmdResponse_GetBaseBallGiftList);
	AddCmd(ResLottery, &CmdResponse_Lottery);
	AddCmd(ResSnakeDiceThrow, &CmdResponse_SnakeDiceThrow);
	AddCmd(ResSnakeDicePresentList, &CmdResponse_SnakeDicePresentList);
	AddCmd(ResSnakeDiceGetPresent, &CmdResponse_SnakeDiceGetPresent);	

	// Update
	AddCmd(UpdUIClose, &CmdUpdate_UIClose);
	AddCmd(UpdSetItemStatus, &CmdUpdate_SetItem);
	AddCmd(UpdBingoInfo, &CmdUpdate_BingoInfo);
	AddCmd(UpdExtraDel, &CmdUpdate_ExtraDel);
	AddCmd(UpdGetItemCaru, &CmdUpdate_GetItemCaru);
	AddCmd(UpdNewNoteMessage, &CmdUpdate_NewNoteMessage);
	AddCmd(UpdNoteMessageData, &CmdUpdate_NoteMessageData);
	AddCmd(UpdNoteMessageContent, &CmdUpdate_NoteMessageContent);
	AddCmd(UpdGetBaseBallGiftList, &CmdUpdate_GetBaseBallGiftList);
	AddCmd(UpdBaseBallInfoList, &CmdUpdate_BaseBallInfoList);
	AddCmd(UpdBossUseSkill, &CmdUpdate_BossUseSkill);
	AddCmd(UpdStopLagrush, &CmdUpdate_StopLagrush);
	AddCmd(UpdSnakeDice, &CmdUpdate_SnakeDice);
	AddCmd(UpdSnakePresentList, &CmdUpdate_SnakeDicePresentList);

	// Notify
	AddCmd(NtfMegaPhone, &CmdNotify_Megaphone);
}

void PacketProcMgr::AddCmd(const unsigned int pack_index, void (*cmd_p)(rnPacket::SP packet))
{
	m_mapFunc.insert(std::make_pair(pack_index, cmd_p));
}

void PacketProcMgr::Excute(rnPacket::SP packet)
{
	stPacketSubHeader* pack = (stPacketSubHeader*)packet->body();

	cmd_f func = m_mapFunc[pack->msg.index_];
	if( func == NULL )
		return;

	func(packet);
}

//////////////////////////////////////////////////////////////////////////
// Response
void CmdResponse_Megaphone(rnPacket::SP packet)
{
	ResponseClient::MagaPhone* pack = (ResponseClient::MagaPhone*)(packet->body());
	if( pack->nErrorCode == 0 )
	{
		return;
	}

	g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_SHOUT_ERR_0 + pack->nErrorCode) , TYPE_NOR_OK, 1);
}

void CmdResponse_ShopGetLaim(rnPacket::SP packet)
{
	ResponseClient::ShopGetLaim* pack = (ResponseClient::ShopGetLaim*)(packet->body());
	if( pack->nErrorCode == 0 || pack->nErrorCode == 1 )
	{
		std::string str = boost::str(boost::format(G_STRING(IDS_SHOPGETLAIM_ERR_0 + pack->nErrorCode)) % pack->nGetLaim);
		g_pNk2DFrame->InsertPopup((char*)(str.c_str()), TYPE_NOR_OK, 1);
		return;
	}

	g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_SHOPGETLAIM_ERR_0 + pack->nErrorCode) , TYPE_NOR_OK, 1);
}

void CmdResponse_BingoInfo(rnPacket::SP packet)
{	
	ResponseClient::BingoInfo* pack = (ResponseClient::BingoInfo*)(packet->body());

	ControlBingo* pBingo = (ControlBingo*)g_pNk2DFrame->GetInterface(BINGO);	
	pBingo->SetState(eBingoState_Idle);

	if( pack->nErrorCode == eResponseErrorType_BingoInfo_Success )
	{
		return;
	}

	g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_BINGOINFO_ERR_0 + pack->nErrorCode) , TYPE_NOR_OK, 1);
}

void CmdResponse_BingoReg(rnPacket::SP packet)
{
	ResponseClient::BingoReg* pack = (ResponseClient::BingoReg*)(packet->body());

	ControlBingo* pBingo = (ControlBingo*)g_pNk2DFrame->GetInterface(BINGO);	
	
	if( pack->nErrorCode == eResponseErrorType_BingoReg_Success )	
	{
		pBingo->SetState(eBingoState_Result);
	}
	else if( pack->nErrorCode == eResponseErrorType_BingoReg_Clear )
	{
		pBingo->SetState(eBingoState_Bingo);
	}
	else if( pack->nErrorCode == eResponseErrorType_BingoReg_OverRap )
	{
		pBingo->SetState(eBingoState_Result);		
	}
	else if( pack->nErrorCode > eResponseErrorType_BingoReg_OverRap )
	{	
		pBingo->SetState(eBingoState_Idle);
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_BINGOREG_ERR_0 + pack->nErrorCode) , TYPE_NOR_OK, 1);
		return;
	}
	pBingo->SetLastNum(pack->nRegNumber);
}

void CmdResponse_BingoPresent(rnPacket::SP packet)
{
	ResponseClient::BingoPresent* pack = (ResponseClient::BingoPresent*)(packet->body());

	ControlBingo* pBingo = (ControlBingo*)g_pNk2DFrame->GetInterface(BINGO);	
	pBingo->SetState(eBingoState_Idle);

	g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_BINGOREWARD_ERR_0 + pack->nErrorCode) , TYPE_NOR_OK, 1);
}

void CmdResponse_Promotion(rnPacket::SP packet)
{
	ResponseClient::Promotion* pack = (ResponseClient::Promotion*)(packet->body());

	if( pack->nErrorCode == eResponseErrorType_Promotion_Success ||
		pack->nErrorCode == eResponseErrorType_Promotion_OverRap )
	{
		std::string strCoupon = (char*)G_STRING(IDS_BATTLELEAGUE_ERR_0 + pack->nErrorCode);
		strCoupon += "            ";
		strCoupon += pack->strCoupon;

		g_pNk2DFrame->InsertPopup((char*)(strCoupon.c_str()), TYPE_NOR_OK, 1);
	}	
	else
	{
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_BATTLELEAGUE_ERR_0 + pack->nErrorCode) , TYPE_NOR_OK, 1);
	}	
}

void CmdResponse_PetSkillInit(rnPacket::SP packet)
{
	ResponseClient::PetSkillInit* pack = (ResponseClient::PetSkillInit*)(packet->body());
	
	if( g_MyPetStatus.m_Class < 3 )
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_PET_SKILL_INIT_ERR_0 + pack->nErrorCode) , TYPE_NOR_OK, 1);
	else
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_NEWPET_SKILL_INIT_ERR_0 + pack->nErrorCode) , TYPE_NOR_OK, 1);
}

void CmdResponse_GearUpgrade(rnPacket::SP packet)
{
	ResponseClient::GearUpgrade* pack = (ResponseClient::GearUpgrade*)(packet->body());

	g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GEAR_UPGRADE_ERR_0 + pack->nErrorCode) , TYPE_NOR_OK, 1);

	if( pack->nErrorCode != 0 )
		return;

	if( g_pRoh->m_Wearing[pack->nWearingNum] == NULL )
		return;

	if( g_pRoh->m_Wearing[pack->nWearingNum]->m_Index != pack->nIndex )
		return;

	g_pRoh->m_Wearing[pack->nWearingNum]->m_PlusNum = pack->nPlusePoint;	
}

void CmdResponse_ViewBox(rnPacket::SP packet)
{
	ResponseClient::NoteMessageViewBox* pack = (ResponseClient::NoteMessageViewBox*)(packet->body());
	
	((ControlLetterWindow*)g_pNk2DFrame->GetInterface(LETTER_WINDOW))->Refresh();

	if( pack->nErrorCode == 0 || pack->nErrorCode == 2 )
	{
		((ControlLetterWindow*)g_pNk2DFrame->GetInterface(LETTER_WINDOW))->SetCurType(pack->nType);
		
		return;
	}

	g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_VIEWBOX_ERR_0 + pack->nErrorCode) , TYPE_NOR_OK, 1);
}

void CmdResponse_NoteMessageSend(rnPacket::SP packet)
{
	ResponseClient::NoteMessageSend* pack = (ResponseClient::NoteMessageSend*)(packet->body());
	
	((ControlLetterSend*)g_pNk2DFrame->GetInterface(LETTER_SEND))->SetLock(false);

	if( pack->nErrorCode == 0 )
	{	
		g_pNk2DFrame->SetVisibleInterface(false, LETTER_SEND);
		((ControlLetterWindow*)g_pNk2DFrame->GetInterface(LETTER_WINDOW))->SetLock(false);
		((ControlLetterWindow*)g_pNk2DFrame->GetInterface(LETTER_WINDOW))->ReqList(false);

		return;
	}
	
	g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_LETTER_SEND_ERR_0 + pack->nErrorCode) , TYPE_NOR_OK, 1);
}

void CmdResponse_NoteMessageDel(rnPacket::SP packet)
{
	ResponseClient::NoteMessageDel* pack = (ResponseClient::NoteMessageDel*)(packet->body());
	
	if( pack->nErrorCode == 0 )
	{	
		g_pNk2DFrame->SetVisibleInterface(false, LETTER_RECV);
		((ControlLetterWindow*)g_pNk2DFrame->GetInterface(LETTER_WINDOW))->SetLock(false);
		((ControlLetterWindow*)g_pNk2DFrame->GetInterface(LETTER_WINDOW))->ReqList(true);

		return;
	}

	((ControlLetterRecv*)g_pNk2DFrame->GetInterface(LETTER_RECV))->SetLock(false);
	g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_LETTER_DEL_ERR_0 + pack->nErrorCode) , TYPE_NOR_OK, 1);
}

void CmdResponse_NoteMessageRead(rnPacket::SP packet)
{
	ResponseClient::NoteMessageRead* pack = (ResponseClient::NoteMessageRead*)(packet->body());

	if( pack->nErrorCode == 0 )
	{
		g_pNk2DFrame->SetVisibleInterface(true, LETTER_RECV);		
		return;
	}	

	((ControlLetterWindow*)g_pNk2DFrame->GetInterface(LETTER_WINDOW))->SetLock(false);
	g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_LETTER_READ_ERR_0 + pack->nErrorCode) , TYPE_NOR_OK, 1);
}

void CmdResponse_GiftTime(rnPacket::SP packet)
{
	ResponseClient::GiftTime* pack = (ResponseClient::GiftTime*)(packet->body());

	g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GIFTTIME_ERR_0 + pack->nErrorCode) , TYPE_NOR_OK, 1);
}

void CmdResponse_BaseBallHit(rnPacket::SP packet)
{
	ResponseClient::BaseBallHit* pack = (ResponseClient::BaseBallHit*)(packet->body());

	if( pack->nResultCode >= eBaseBallHitResult_Type_NotBat )
	{
		ControlBaseball* board = (ControlBaseball*)g_pNk2DFrame->GetInterface(BASEBALL);
		board->Init();

		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_BASEBALL_HIT_ERR0 + pack->nResultCode) , TYPE_NOR_OK, 1);	

		return;
	}

	BaseballManager* ballMgr = BaseballManager::GetInstance();
	int count = ballMgr->GetBallCount(pack->nResultCode);
	++count;
	ballMgr->SetBallCount(pack->nResultCode, count);

	ControlBaseball* board = (ControlBaseball*)g_pNk2DFrame->GetInterface(BASEBALL);
	board->SetResult(pack->nResultCode);
}

void CmdResponse_GetBaseBallGift(rnPacket::SP packet)
{
	ResponseClient::GetBaseBallGift* pack = (ResponseClient::GetBaseBallGift*)(packet->body());

	g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_BASEBALL_GIFT_ERR0 + pack->nResultCode) , TYPE_NOR_OK, 1);	
}

void CmdResponse_GetBaseBallGiftList(rnPacket::SP packet)
{
	ResponseClient::GetBaseBallGiftList* pack = (ResponseClient::GetBaseBallGiftList*)(packet->body());

	if( pack->nResultCode == eGetBaseBallGiftListResult_Type_Success )
		return;
	
	g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_BASEBALL_GIFTLIST_ERR0 + pack->nResultCode) , TYPE_NOR_OK, 1);	
}

void CmdResponse_Lottery(rnPacket::SP packet)
{
	ResponseClient::Lottery* pack = (ResponseClient::Lottery*)(packet->body());
	
	if( pack->nResultCode >= eLotteryResult_Type_NoHaveItem )
	{
		ControlLottery* board = (ControlLottery*)g_pNk2DFrame->GetInterface(LOTTERY);
		board->Init();

		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_LOTTERY_ERR0 + pack->nResultCode) , TYPE_NOR_OK, 1);	

		return;
	}

	ControlLottery* board = (ControlLottery*)g_pNk2DFrame->GetInterface(LOTTERY);
	board->SetResult(pack->nResultCode);
}

void CmdResponse_SnakeDiceThrow(rnPacket::SP packet)
{
	ResponseClient::SnakeDiceThrow* pack = (ResponseClient::SnakeDiceThrow*)(packet->body());

	if( pack->nResultCode > eResponseErrorType_Snake_dice_ok )
	{
		ControlDiceGame* board = (ControlDiceGame*)g_pNk2DFrame->GetInterface(DICEGAME);
		board->Init();

		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_DICEGAME_ERR0 + pack->nResultCode) , TYPE_NOR_OK, 1);	

		return;
	}

	ControlDiceGame* board = (ControlDiceGame*)g_pNk2DFrame->GetInterface(DICEGAME);
	std::vector<tINT> vec;
	vec.assign(pack->nPath, pack->nPath + pack->nPathCount);
	board->SetPath(pack->nDiceCount, vec);
	board->SetState(eDiceGameState_ShowDice);
}

void CmdResponse_SnakeDicePresentList(rnPacket::SP packet)
{
	ResponseClient::SnakeDicePresentList* pack = (ResponseClient::SnakeDicePresentList*)(packet->body());

	if( pack->nResultCode > eResponseErrorType_Snake_dice_ok )
	{
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_DICEGAME_ERR0 + pack->nResultCode) , TYPE_NOR_OK, 1);	

		return;
	}	
}

void CmdResponse_SnakeDiceGetPresent(rnPacket::SP packet)
{
	ResponseClient::SnakeDiceGetPresent* pack = (ResponseClient::SnakeDiceGetPresent*)(packet->body());

	g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_DICEGAME_ERR0 + pack->nResultCode) , TYPE_NOR_OK, 1);
}

//////////////////////////////////////////////////////////////////////////
// Update
void CmdUpdate_UIClose(rnPacket::SP packet)
{
	UpdateClient::UIClose* pack = (UpdateClient::UIClose*)(packet->body());

	if( pack->nType == eUICloseType_All )
	{
		g_pNk2DFrame->CloseAllWindow();
	}
}

void CmdUpdate_SetItem(rnPacket::SP packet)
{
	UpdateClient::SetItem* pack = (UpdateClient::SetItem*)(packet->body());

	SetItemEffMgr* setItemEffMgr = SetItemEffMgr::GetInstance();
	
	if( pack->nON == true )
	{
		setItemEffMgr->AddEffect(pack->nItemIndex, pack->nCount, pack->stStatus);
	}
	else
	{
		setItemEffMgr->RemoveEffect(pack->nItemIndex);
	}

	g_pNk2DFrame->UpdateDrawInfo();
}

void CmdUpdate_BingoInfo(rnPacket::SP packet)
{
	UpdateClient::BingoInfo* pack = (UpdateClient::BingoInfo*)(packet->body());

	BingoManager* bingoMgr = BingoManager::GetInstance();

	bingoMgr->SetSuccessCount(pack->nSuccessCount);
	bingoMgr->SetPreview(pack->nPreNumber);
	bingoMgr->SetPresentState(pack->nPresentState);
	bingoMgr->SetBingoNums(pack->nCount, pack->nNumberList);

	ControlBingo* pBingo = (ControlBingo*)g_pNk2DFrame->GetInterface(BINGO);	
	pBingo->RefreshRewardText();
}

void CmdUpdate_ExtraDel(rnPacket::SP packet)
{
	UpdateClient::ExtraDel* pack = (UpdateClient::ExtraDel*)(packet->body());	

	if( g_pRoh->m_ExtraSlot == NULL )
		return;

	if( g_pRoh->m_ExtraSlot->m_Index != pack->nItemIndex )
		return;

	SAFE_DELETE(g_pRoh->m_ExtraSlot);
}

void CmdUpdate_GetItemCaru(rnPacket::SP packet)
{
	UpdateClient::GetItemCaru* pack = (UpdateClient::GetItemCaru*)(packet->body());

	std::string strMsg = boost::str(boost::format("Caru NPCs have been given %1% and %2%.") 
		% GET_ITEM_NAME_VNUM(pack->nItemVnum)
		% pack->nItemCount);

	g_pNk2DFrame->InsertPopup((char*)strMsg.c_str(), TYPE_NOR_OK, 1);
}

void CmdUpdate_NewNoteMessage(rnPacket::SP packet)
{
	UpdateClient::NewNoteMessage* pack = (UpdateClient::NewNoteMessage*)(packet->body());

	if( pack->nCount > 0 )
	{
		g_pNk2DFrame->GetControlBottom()->NewLetter(true);
	}
}

void CmdUpdate_NoteMessageData(rnPacket::SP packet)
{
	UpdateClient::NoteMessageData* pack = (UpdateClient::NoteMessageData*)(packet->body());

	LetterManager* letterMgr = LetterManager::GetInstance();
	letterMgr->SetCurPage(pack->nNowPage);
	letterMgr->SetMaxPage(pack->nMaxPage);

	letterMgr->ClearList();
	int count = pack->nCount;
	for(int i = 0; i < count; i++)
	{
		letterMgr->AddListData(pack->nType, pack->stNoteMessageData[i]);		
	}

	((ControlLetterWindow*)g_pNk2DFrame->GetInterface(LETTER_WINDOW))->Refresh();
}

void CmdUpdate_NoteMessageContent(rnPacket::SP packet)
{
	UpdateClient::NoteMessageContent* pack = (UpdateClient::NoteMessageContent*)(packet->body());
	
	LetterManager::GetInstance()->UpdateContent(pack->nType, pack->nIndex, pack->strContent);	

	((ControlLetterWindow*)g_pNk2DFrame->GetInterface(LETTER_WINDOW))->Refresh();
	((ControlLetterRecv*)g_pNk2DFrame->GetInterface(LETTER_RECV))->Refresh(pack->nIndex, pack->nType);
}

void CmdUpdate_GetBaseBallGiftList(rnPacket::SP packet)
{
	UpdateClient::GetBaseBallGiftList* pack = (UpdateClient::GetBaseBallGiftList*)(packet->body());

	BaseballRewardManager* rewardMgr = BaseballRewardManager::GetInstance();
	rewardMgr->Clear();

	int count = pack->nCount;

	for(int i = 0; i < count; i++)
	{
		if( pack->stGetBaseBallGiftData[i].enable == false )
		{
			rewardMgr->AddCanGet(pack->stGetBaseBallGiftData[i].rating, pack->stGetBaseBallGiftData[i].rating_count);
		}
		else
		{
			rewardMgr->AddAlready(pack->stGetBaseBallGiftData[i].rating, pack->stGetBaseBallGiftData[i].rating_count);
		}
	}

	ControlBaseballReward* board = (ControlBaseballReward*)g_pNk2DFrame->GetInterface(BASEBALL_REWARD);
	board->Refresh();
}

void CmdUpdate_BaseBallInfoList(rnPacket::SP packet)
{
	UpdateClient::BaseBallInfoList* pack = (UpdateClient::BaseBallInfoList*)(packet->body());

	int count = pack->nCount;

	BaseballManager* ballMgr = BaseballManager::GetInstance();
	for(int i = 0; i < count; i++)
	{
		ballMgr->SetBallCount(pack->stBaseBallInfoData[i].rating, pack->stBaseBallInfoData[i].rating_count);
	}
}

void CmdUpdate_BossUseSkill(rnPacket::SP packet)
{
	UpdateClient::BossUseSkill* pack = (UpdateClient::BossUseSkill*)(packet->body());

	CNkMob* pMob = pCMyApp->m_pUIMgr->FindMobById(pack->nMobIndex);
	if( pMob == NULL )
	{
		return;
	}

	pMob->ExecuteBossSkill(pack->nType);
}

void CmdUpdate_StopLagrush(rnPacket::SP packet)
{
	pCMyApp->m_pUIMgr->m_AutoPlay->_OnInterrupt(LagRush_Interrupt__CantZone);
}

void CmdUpdate_SnakeDice(rnPacket::SP packet)
{
	UpdateClient::SnakeDiceinfo* pack = (UpdateClient::SnakeDiceinfo*)(packet->body());

	DiceGameManager::GetInstance()->SetSuccessCount(pack->nTotalSuccessCount);
	ControlDiceGame* board = (ControlDiceGame*)g_pNk2DFrame->GetInterface(DICEGAME);
	board->SetPlayerPos(pack->nPos);
}

void CmdUpdate_SnakeDicePresentList(rnPacket::SP packet)
{
	UpdateClient::SnakeDicePresentList* pack = (UpdateClient::SnakeDicePresentList*)(packet->body());

	DiceGameRewardManager* rewardMgr = DiceGameRewardManager::GetInstance();
	rewardMgr->Clear();

	int count = pack->nCount;

	for(int i = 0; i < count; ++i)
	{
		if( pack->nPresent[i].enable == false )
		{
			rewardMgr->AddCanGet(pack->nPresent[i].success_count);
		}
		else
		{
			rewardMgr->AddAlready(pack->nPresent[i].success_count);
		}
	}

	ControlDiceGameReward* board = (ControlDiceGameReward*)g_pNk2DFrame->GetInterface(DICEGAME_REWARD);
	board->Refresh();
}

//////////////////////////////////////////////////////////////////////////
// Notify
void CmdNotify_Megaphone(rnPacket::SP packet)
{
	NotifyClient::MegaPhone* pack = (NotifyClient::MegaPhone*)(packet->body());
	g_pNk2DFrame->AddChatShoutStr(pack->nSay);
}
