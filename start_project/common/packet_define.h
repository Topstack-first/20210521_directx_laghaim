#ifndef _PACKET_DEFINE_H_
#define _PACKET_DEFINE_H_

#include "info_define.h"
#include "packet_enum.h"
#include "packet_header.h"

#pragma pack(push, 1)

//////////////////////////////////////////////////////////////////////////
// Sniffing client
namespace SniffingClient
{
	struct ToolStart : public stPacketSubHeader
	{
		tINT nCharIndex;
		tBOOL bStart;
	};
}

//////////////////////////////////////////////////////////////////////////
// Client request
namespace RequestClient
{
	struct CharAttack : public stPacketSubHeader
	{
		tBYTE btCharType;		// eCharType
		tINT nTargetCharIndex;
		tBOOL bHaste;
	};

	struct ChatMultiTarget : public stPacketSubHeader
	{
		tBYTE btType;
		tCHAR strChatMsg[CHAT_LENGTH + 1];
	};

	struct ChatTarget : public stPacketSubHeader
	{
		tCHAR strCharName[USER_ID_LENGTH + 1];
		tBYTE btType;
		tCHAR strChatMsg[CHAT_LENGTH + 1];
	};
	
	struct CharUseSkill : public stPacketSubHeader
	{
		tINT nSkillIndex;
		tINT nDir;
		tFLOAT nX;
		tFLOAT nZ;
		tINT  nSkillvalue1;
		tINT  nSkillvalue2;
		tINT  nSkillvalue3;
		tBOOL nHideFlag;
		tINT  nDStoneIndex;
		tINT  nStep;
		tINT  nStepStart;
		tBYTE nTargetType;
		tINT  nTargetCount;
		tINT  nTargetIndex[0];
	};

	struct EventLottoReg : public stPacketSubHeader
	{
		tINT nReg_Num0;
		tINT nReg_Num1;
		tINT nReg_Num2;
		tINT nReg_Num3;
		tINT nReg_Num4;
		tINT nReg_Num5;
	};

	struct EventLottoList : public stPacketSubHeader
	{
		tINT nPage;
	};

	struct MagaPhone : public stPacketSubHeader
	{
		tINT	nQuickNum;
		tINT	nItemIndex;
		tCHAR	nSay[CHAT_LENGTH + 1];
	};

	struct BingoPresent : public stPacketSubHeader
	{
		tINT nPresentIndex;
	};

	struct BingoReg : public stPacketSubHeader
	{
		tINT nType;
	};

	struct GearUpgrade : public stPacketSubHeader
	{
		tINT nWearingNum;
	};

	struct ViewNoteMessageBox : public stPacketSubHeader
	{
		tINT nType;
		tINT nPage;
	};

	struct NoteMessageSend : public stPacketSubHeader
	{
		tCHAR strReceive_name[NOTE_MESSAGE_NAME_MAX];
		tCHAR strTitle[NOTE_MESSAGE_TITLE_MAX];
		tCHAR strContent[NOTE_MESSAGE_CONTENT];
	};

	struct NoteMessageDel : public stPacketSubHeader
	{
		tINT nType;
		tINT nIndex;
	};

	struct NoteMessageRead : public stPacketSubHeader
	{
		tINT nType;
		tINT nIndex;
	};

	struct GetAdmin : public stPacketSubHeader
	{
		tCHAR strPassword[256];
	};

	struct BaseBallHit : public stPacketSubHeader
	{
		tINT  nBallType;
		tBOOL nHitType;
	};

	struct GetBaseBallGift : public stPacketSubHeader
	{
		tINT nRatingType;
		tINT nRatingCount;
	};

	struct SnakeDiceGetPresent : public stPacketSubHeader
	{
		tINT nSuccessCount;
	};
}

//////////////////////////////////////////////////////////////////////////
// Response
namespace ResponseClient
{
	struct MagaPhone : public stPacketSubHeader
	{
		tINT	nErrorCode;
	};

	struct ShopGetLaim : public stPacketSubHeader
	{
		tINT	nErrorCode;
		tINT64	nGetLaim;
	};

	struct BingoInfo : public stPacketSubHeader
	{
		tINT nErrorCode;
	};

	struct BingoReg : public stPacketSubHeader
	{
		tINT nErrorCode;
		tINT nRegNumber;
	};

	struct BingoPresent : public stPacketSubHeader
	{
		tINT nErrorCode;
	};

	struct Promotion : public stPacketSubHeader
	{
		tINT nErrorCode;
		tCHAR strCoupon[BATTLE_LEAGUE_COUPON + 1];
	};

	struct PetSkillInit : public stPacketSubHeader
	{
		tINT nErrorCode;
	};

	struct GearUpgrade : public stPacketSubHeader
	{
		tINT nErrorCode;
		tINT nWearingNum;
		tINT nIndex;
		tINT nPlusePoint;
	};

	struct NoteMessageViewBox : public stPacketSubHeader
	{
		tINT nType;
		tINT nErrorCode;
	};

	struct NoteMessageSend : public stPacketSubHeader
	{
		tINT nErrorCode;
	};

	struct NoteMessageDel : public stPacketSubHeader
	{
		tINT nType;
		tINT nErrorCode;
		tINT nIndex;
	};

	struct NoteMessageRead : public stPacketSubHeader
	{
		tINT nType;
		tINT nErrorCode;
	};

	struct GiftTime : public stPacketSubHeader
	{
		tINT nErrorCode;
	};

	struct BaseBallHit : public stPacketSubHeader
	{
		tINT nResultCode;
	};

	struct GetBaseBallGift : public stPacketSubHeader
	{
		tINT nResultCode;
	};

	struct GetBaseBallGiftList : public stPacketSubHeader
	{
		tINT nResultCode;
	};

	struct Lottery : public stPacketSubHeader
	{
		tINT nResultCode;
	};

	struct SnakeDiceThrow : public stPacketSubHeader
	{
		tINT nResultCode;
		tINT nDiceCount;
		tINT nPathCount;
		tINT nPath[SNAKE_DICE_PATH_MAX];
	};

	struct SnakeDicePresentList : public stPacketSubHeader
	{
		tINT nResultCode;
	};

	struct SnakeDiceGetPresent : public stPacketSubHeader
	{
		tINT nResultCode;
	};
}

//////////////////////////////////////////////////////////////////////////
// Update
namespace UpdateClient
{
	struct UIClose : public stPacketSubHeader
	{
		tINT nType;
	};

	struct SetItem : public stPacketSubHeader
	{
		tBOOL nON;
		tINT nItemIndex[WEARING_NUM];
		tINT nCount;
		SetItemStatus stStatus[0];
	};

	struct BingoInfo : public stPacketSubHeader
	{
		tINT nSuccessCount;
		tINT nPreNumber[BINGO_READY_MAX];
		tINT nPresentState[BINGO_PRESENT_MAX];
		tINT nCount;
		tINT nNumberList[0];
	};

	struct ExtraDel : public stPacketSubHeader
	{
		tINT nItemIndex;
	};

	struct GetItemCaru : public stPacketSubHeader
	{
		tINT nItemVnum;
		tINT nItemCount;		
	};

	struct NewNoteMessage : public stPacketSubHeader
	{
		tINT nCount;
	};

	struct NoteMessageData : public stPacketSubHeader
	{
		tINT nType;
		tINT nNowPage;
		tINT nMaxPage;
		tINT nCount;
		NoteMessage stNoteMessageData[0];
	};

	struct NoteMessageContent : public stPacketSubHeader
	{
		tINT nIndex;
		tINT nType;
		tINT nKeepEndTime;
		tCHAR strContent[NOTE_MESSAGE_CONTENT];
	};

	struct GetBaseBallGiftList : public stPacketSubHeader
	{
		tINT nCount;
		GetBaseBallGiftData stGetBaseBallGiftData[0];
	};

	struct BaseBallInfoList : public stPacketSubHeader
	{
		tINT nCount;
		BaseBallInfo stBaseBallInfoData[0];
	};

	struct BossUseSkill : public stPacketSubHeader
	{
		tINT nMobIndex;
		tINT nType;
	};

	struct SnakeDiceinfo : public stPacketSubHeader
	{
		tINT nPos;
		tINT nTotalSuccessCount;
	};

	struct SnakeDicePresentList : public stPacketSubHeader
	{
		tINT nResultCode;
		tINT nCount;
		SnakeDicePresent  nPresent[0];
	};
}

//////////////////////////////////////////////////////////////////////////
// notify
namespace NotifyClient
{
	struct MegaPhone : public stPacketSubHeader
	{
		tCHAR nSay[CHAT_LENGTH + 1];
	};
}

struct ReqeustLogin
{
	tINT nPackVersion;				// Packet version
	tCHAR szId[USER_ID_LENGTH + 1];	// User ID
	tCHAR szPw[USER_PW_LENGTH + 1];	// User PW
	tBOOL bWeb;						// Is web connection
	tBOOL bFirm;					// 
	tBOOL bInternalConnect;			// Is internal connection
};

struct RequestCharNew
{
	PreCharInfo stInfo;
};

struct RequestCharDel
{
	tINT nSlotIndex;
	tINT nCharIndex;
	tINT nGuildIndex;
	tBOOL bWeb;
	tCHAR szPw_length;
	tCHAR szPw[0];
};

struct RequestGameStart
{
	tINT nSlotIndex;
	tINT nCharCount;
	tBOOL bSlotEmpty[CHAR_SLOT_MAX];
};

struct RequestWear
{
	tINT nWearWhere;
};

struct RequestEvWear
{
	tINT nWearWhere;
};

struct RequestInven
{
	tINT nInvenPack;
	tINT nSlotX;
	tINT nSlotY;
};

struct RequestQuick
{
	tINT nQuickSlotIndex;
};

struct RequestPulse
{
	tINT nClientTime;
	tINT nItemVnum;
	tINT nItemIndex;
	tINT nItemSpeed;
	tINT nItemAttackRange;
};

struct RequestCharWalk
{
	tFLOAT fPosX;
	tFLOAT fPosZ;
	tBOOL bRun;
};

struct RequestCharPlace
{
	tBYTE btCharType;		// eCharType
	tFLOAT fPosX;
	tFLOAT fPosZ;
	tFLOAT fDir;
	tINT nRemainFrame;
	tBOOL bRun;
};

struct RequestCharStop
{
	tBYTE btCharType;		// eCharType
	tFLOAT fPosX;
	tFLOAT fPosZ;
	tFLOAT fDir;
};

struct RequestCharGoto
{
	tBYTE btCharType;		// eCharType
	tFLOAT fPosX;
	tFLOAT fPosZ;
};

struct RequestCharPkState
{
	tBOOL bPkOn;
};

struct RequestCharStatusPointReset
{
	tBOOL bByNpc;	// NPC 통한 리셋
	tBOOL bManual;	// 모두 리셋
	tINT nStr;		// 힘
	tINT nInt;		// 지능
	tINT nDex;		// 민첩
	tINT nCon;		// 체질
	tINT nCha;		// 카리스마
};

struct RequestGoWorld
{
	tINT nZoneNumber;
	tINT nSubNumber;
};

struct ReqeustHeroList
{
	tINT nRace;		// 종족번호
	tINT nPage;
};

struct RequestPartyRequest
{
	tCHAR strCharName[USER_ID_LENGTH];
	tBYTE btExpType;
	tBYTE btItemType;
};

struct RequestPartyConsist
{
	tBYTE btMode;
	tINT nRequestCharIndex;
};

struct RequestPartyDissmiss
{
	tINT nDismissCharIndex;
	tBOOL bAutoRefuse;
};

struct RequestPartySecession
{
	tINT nSecessionCharIndex;
	tBOOL bAutoRefuse;
};

struct RequestPartyEntrust
{
	tINT nTargetCharIndex;
};

struct RequestCharSubAttack
{
	tBYTE btCharType;		// eCharType
	tINT nTargetCharIndex;
	tINT nSkillIndex;
	tBOOL bIsCombo;
};

struct RequestCharUseSubItem
{
	tINT nQuickSlotIndex;
	tBYTE btCharType;		// eCharType
	tINT nTargetCharIndex;
};

struct RequestCharProduct
{
	tINT nItemVnum;
};

struct RequestGuildHuntOpen
{
	tINT nPage;
};

struct RequestGuildHuntRequest
{
	tINT nPage;
	tBOOL bEvolution;
	tINT nEvolutionBall;
	tINT nItemIndex;
	tINT nGuildHuntIndex;
};

struct RequestGuildHuntCancel
{
	tINT nPage;
	tINT nGuildHuntIndex;
};

struct RequestCharSocialAction
{
	tINT nSocialIndex;
};

struct RequestExchangeRequest
{
	tCHAR strCharName[USER_ID_LENGTH];
};

struct RequestExchangeResponse
{
	tBYTE btExchangeResType;	// eExchangeResponseType
};

struct RequestExchangeInven
{
	tINT nSlotX;
	tINT nSlotY;
};

struct RequestExchangeMoney
{
	tINT nMoneyAmount;
};

struct RequestItemPick
{
	tINT nItemIndex;
};

struct RequestItemQuick
{
	tINT nQuickSlotIndex;
};

struct RequestItemUpgrade
{
	tBYTE btEquipIndex;
};

struct RequestItemupgradeMs
{
	tBYTE btEquipIndex;
	tBYTE btUpgradeType;
};

struct RequestCharStamina
{
	tINT nStamina;
};

struct RequestItemUseQuickItem
{
	tINT nQuickSlotIndex;
	tINT nItemIndex;
	tINT nExtraValue;
};

struct RequestItemusePremiumItem
{
	tINT nQuickSlotIndex;
	tINT nItemIndex;
	tCHAR strChangeName[USER_ID_LENGTH];
};

struct RequestItemInvenToQuick
{
	tINT nInvenPack;
	tINT nItemIndex;
	tINT nQuickSlotIndex;
};

struct RequestItemInvenToPetInven
{
	tINT nInvenPack;
	tINT nItemIndex;
};

struct RequestItemSpShopBuy
{
	tINT nShopIndex;
	tINT nItemIndex;
	tINT nInvenPack;
};

struct RequestItemMultiSell
{
	tINT nShopIndex;
	tINT nInvenPack;
	tINT nItemCount;
	ItemMultiInfo pInfo[0];
};

struct RequestItemShopList
{
	tINT nNpcIndex;
	tFLOAT fPosX;
	tFLOAT fPosZ;
};

struct RequestItemShopBuy
{
	tINT nNpcIndex;
	tFLOAT fPosX;
	tFLOAT fPosZ;
	tINT nItemVnum;
	tINT nInvenPack;
};

struct RequestItemShopSell
{
	tINT nNpcIndex;
};

struct RequestItemPotionShopBuy
{
	tINT nNpcIndex;
	tINT nItemVnum;
	tINT nInvenPack;
};

struct RequestItemChipExchange
{
	tINT nItemVnum;
};

struct RequestItemGambleExchange
{
	tBYTE btGambleType;
	tINT nItemVnum;
	tBYTE btSelectedNumber;
};

struct RequestCharChangeHair
{
	tBYTE btHair;
};

struct RequestEventItemExchange
{
	tBYTE btEventType;
	tINT nExtraValue;
};

struct RequestStashList
{
	tBOOL bCash;
};

struct RequestStashClick
{
	tINT nStashSlotIndex;
	tINT nItemVnum;
	tINT nPlusPoint;
	tINT nSpecialFlag1;
};

struct RequestStashPut
{
	tINT nItemVnum;
	tINT nInvenPack;
	tINT nStashPack;
	tINT nItemCount;
	StashItemMultiInfo pInfo[0];
};

struct RequestStashGet
{
	tINT nItemVnum;
	tINT nInvenPack;
	tINT nStashPack;
	tINT nStashSlotIndex;
};

struct RequestStashPuts
{
	tINT nItemVnum;
	tINT nInvenPack;
	tINT nStashSlotIndex;
	tINT nItemCount;
	StashItemMultiInfo pInfo[0];
};

struct RequestStashGets
{
	tINT nItemVnum;
	tINT nInvenPack;
	tINT nStashSlotIndex;
	tINT nItemCount;
};

struct RequestExchangeMulti
{
	tINT nInvenPack;
	tINT nItemCount;
	ItemMultiInfo pInfo[0];
};

struct RequestCharCombatState
{
	tINT nCombatState;
};

struct RequestCharStatusUp
{
	tBYTE btStatusType;			// eCharStatusType
};

struct RequestCharSkillUp
{
	tBYTE nSkillIndex;
};

struct RequestCharFastMoveReport
{
	tCHAR strCharName[USER_ID_LENGTH];
};

struct RequestChatSales
{
	tCHAR strChat[CHAT_LENGTH];
};

struct RequestChatSalesView
{
	tBOOL bView;
};

struct RequestCastleTaxAccount
{
	tINT nAccountLaim;			// 찾을 금액
};

struct RequestCastleTaxRate
{
	tINT nRate;
};

struct RequestCastleTowerChange
{
	tINT nNpcIndex;
	tINT nNpcVnum;
	tINT nDefenceValue;
};

struct RequestCastleTowerControl
{
	tBYTE btCastleTowerControlType;		// eCastleTowerControlType
};

struct RequestCastleBattleTime
{
	tINT nBattleTime;
};

struct RequestCastleEnergyUp
{
	tINT nNpcIndex;
};

struct RequestPetAttack
{
	tBYTE btCharType;			// eCharType
	tINT nCharIndex;
};

struct RequestPetComposite
{
	tINT nItemIndex;
};

struct RequestPetUseSkill
{
	tINT nSkillIndex;
	tBYTE btTargetCharType;		// eCharType
	tINT nTargetCount;
	tINT nTargetCharIndex[0];	
};

struct RequestPetChangePiece
{
	tINT nItemVnum;
};

struct RequestPetKeep
{
	tBYTE btPetKeepType;		// ePetKeepType
};

struct RequestPetSummon
{
	tINT nQuickSlotIndex;
};

struct RequestPetShop
{
	tINT nItemVnum;
};

struct RequestPetBag
{
	tBYTE btSubCmd;				// ePetBagSubCmd
};

struct RequestPetMode
{
	tBYTE btPetMode;			// ePetMode
};

struct RequestPetSocial
{
	tINT nPetSocial;
	tFLOAT fPosX;
	tFLOAT fPosZ;
};

struct RequestPetPick
{
	tINT nItemIndex;
};

struct RequestPetDrop
{
	tINT nInvenPack;
};

struct RequestPetPeragonDrop
{
	tINT nInvenPack;
};

struct RequestWarZone
{
	tBYTE btWarZone;			// eWarZone
};

struct RequestWarRequest
{
	tCHAR strGuildName[GUILD_NAME_LENGTH];
	tCHAR strGuildMasterName[USER_ID_LENGTH];
	tINT nLaim;
	tINT nMaxCount;
};

struct RequestWarChar
{
	tCHAR strCharName[USER_ID_LENGTH];
};

struct RequestWarAccept
{
	tCHAR strGuildName[GUILD_NAME_LENGTH];
};

struct RequestChatGuildWar
{
	tCHAR strChat[CHAT_LENGTH];
};

struct RequestGpkRequest
{
	tCHAR strGuildName[GUILD_NAME_LENGTH];
};

struct RequestGpkAccept
{
	tCHAR strGuildName[GUILD_NAME_LENGTH];
};

struct RequestGpkCancel
{
	tCHAR strGuildName[GUILD_NAME_LENGTH];
};

struct RequestItemBraceletUpgrade
{
	tINT nWhich;				// -1, 0, 1
};

struct RequestItemNecklaceUpgrade
{
	tINT nWhich;				// -1, 0, 1
};

struct RequestSupportRequest
{
	tCHAR strCharName[USER_ID_LENGTH];
};

struct RequestSupportAccept
{
	tBYTE btSupportAcceptType;	// eSupportAcceptType
};

struct RequestSupportGiveup
{
	tCHAR strCharName[USER_ID_LENGTH];
};

struct RequestMinimap
{
	tINT nMinimapPosX;
	tINT nMinimapPosY;
};

struct RequestChatShout
{
	tCHAR nChat[CHAT_LENGTH];
};

struct RequestQuestClick
{
	tINT nNpcVnum;
};

struct ResponseQuestSpecial
{
	tBOOL bComplete;
};

struct RequestItemRingCombine
{
	tBYTE btItemRingCombineType;	// eItemRingCombineType
};

struct RequestMapCheckGhostChar
{
	tBYTE btCharType;				// eCharType
	tINT nCharIndex;
};

struct RequestItemNpcUpgrade
{
	tBYTE btItemNpcUpgradeCmd;		// eItemNpcUpgradeCmd
};

struct ReqeustItemEndurance
{
	tINT nWearWhere;
};

struct RequestUsershopSearch
{
	tINT nItemVnum;
};

struct RequestUsershopRegistry
{
	tINT nUsershopSlotIndex;
	tINT nInvenPack;
	tINT nGold;
	tINT nSilver;
	tINT nBronze;
	tINT nLaim;
	tINT nInvenSlotX;
	tINT nInvenSlotY;
};

struct RequestUsershopUnregistry
{
	tINT nUsershopSlotIndex;
	tINT nInvenPack;
	tINT nUnregCount;
};

struct RequestUsershopStart
{
	tCHAR strShopTitle[USERSHOP_TITLE_LENGTH];
};

struct RequestUsershopBuy
{
	tINT nUsershopSlotIndex;
	tINT nInvenPack;
	tINT nBuyCount;
};

struct RequestUsershopOpen
{
	tBYTE btCharType;			// eCharType
	tINT nCharIndex;
};

struct RequestCharRotate
{
	tINT nCharIndex;
	tFLOAT fDir;
};

struct RequestSoccerIn
{
	tCHAR strOutCharName[USER_ID_LENGTH];
	tCHAR strInCharName[USER_ID_LENGTH];
};

struct RequestCharUseEffect
{
	tINT nEffectIndex;
	tBOOL bOn;
};

struct RequestPetUpgrade
{
	tINT nPetClass;
};

struct RequestPetSkillUp
{
	tINT nPetSkillIndex;
};

struct RequestDungeonFloor
{
	tINT nMapFloor;
};

struct RequestDungeonUseKey
{
	tINT nInvenPack;
	tINT nItemIndex;
	tINT nNpcIndex;
};

struct RequestItemRepair
{
	tINT nInvenPack;
	tINT nSlotX;
	tINT nSlotY;
};

struct RequestItemEnchant
{
	tBYTE btEnchantType;		// 0, 1
};

struct RequestCharStatusUpMulti
{
	tINT nStr;
	tINT nInt;
	tINT nDex;
	tINT nCon;
	tINT nCha;
};

struct RequestMinorRequest
{
	tBYTE btTeam;				// 0, 1
};

struct RequestGuildCreate
{
	tCHAR strGuildName[GUILD_NAME_LENGTH];
};

struct RequestGuildSearch
{
	tBYTE btGuildSearchType;	// eGuildSearchType
	tCHAR strName[GUILD_NAME_LENGTH];
};

struct RequestGuildStashPut
{
	tINT nGuildStashSlotIndex;
	tINT nItemVnum;
	tINT nLaim;
};

struct RequestGuildStashGet
{
	tINT nGuildStashSlotIndex;
	tINT nItemVnum;
	tINT64 nLaim;
};

struct RequestGuildStashPutMulti
{
	tINT nItemVnum;
	tINT nInvenPack;
	tINT nGuildStashPack;
	tINT nItemCount;
	ItemMultiInfo pInfo[0];
};

struct RequestGuildStashGetMulti
{
	tINT nItemVnum;
	tINT nInvenPack;
	tINT nGuildStashPack;
	tINT nGuildStashSlotIndex;
};

struct RequestGuildRankwarOpen
{
	tINT nGuildRankwarType;
};

struct RequestGuildRankwarList
{
	tINT nMainType;
	tINT nSubType;
	tINT nListCount;
	tINT nCharIndex[0];
};

struct RequestGuildInvasionRequest
{
	tINT nType;
	tCHAR strGuildName[GUILD_NAME_LENGTH];
};

struct RequestGuildInvasionCancelReq
{
	tINT nGuildIndex;
};

struct RequestGuildInvasionCancelRes
{
	tBOOL bAccept;
	tCHAR strGuildName[GUILD_NAME_LENGTH];
	tCHAR strMasterName[USER_ID_LENGTH];
};

struct RequestGuildRecommandReq
{
	tCHAR strCharName[USER_ID_LENGTH];
};

struct RequestGuildRecommandRes
{
	tCHAR strGuildName[GUILD_NAME_LENGTH];
	tCHAR strCharName[USER_ID_LENGTH];
	tBOOL bAllow;
};

struct RequestGuildJoinReq
{
	tCHAR strGuildName[GUILD_NAME_LENGTH];
};

struct RequestGuildJoinRes
{	
	tCHAR strCharName[GUILD_NAME_LENGTH];
	tBOOL bAllow;
};

struct RequestGuildNick
{
	tINT nGuildGrade;
	tCHAR strGuildGrade[GUILD_GRADE_LENGTH];
};

struct RequestGuildNotice
{
	tCHAR strGuildNotice[GUILD_NOTICE_LENGTH];
};

struct RequestGuildAppoint
{
	tINT nGuildGrade;
	tCHAR strCharName[USER_ID_LENGTH];
};

struct RequestGuildFire
{
	tCHAR strCharName[USER_ID_LENGTH];
};

struct RequestWantedRegistTest
{
	tCHAR strCharName[USER_ID_LENGTH];
};

struct RequestWantedRegist
{
	tCHAR strCharName[USER_ID_LENGTH];
	tINT nWantedType;
	tINT nWantedLaim;
};

struct RequestWantedMyCancel
{
	tCHAR strCharName[USER_ID_LENGTH];
};

struct RequestFreepvpReq
{
	tCHAR strCharName[USER_ID_LENGTH];
};

struct RequestFreepvpRes
{
	tBOOL bAllow;
};

struct RequestOccpTrans
{
	tINT nLocation;
};

struct RequestOccpTransPortal
{
	tINT nPrevLocation;
	tINT nDestLocation;
	tINT nPrice;
};

struct RequestOccpTransGate
{
	tINT nLocation;
};

struct RequestOccpBattleReq
{
	tINT nLocation;
};

struct RequestOccpBattleInfo
{
	tINT nLocation;
};

struct RequestOccpBattleStart
{
	tINT nLocation;
};

struct RequestOccpBattleCancel
{
	tINT nLocation;
};

struct RequestOccpCashOpen
{
	tINT nLocation;
};

struct RequestOccpCashAccount
{
	tINT nLaim;
};

struct RequestOccpCashRate
{
	tINT nRate;
};

struct RequestBossraidKey
{
	tINT nInvenPack;
	tINT nItemIndex;
	tINT nRoomNum;
};

struct RequestChipRoulette
{
	tINT nItemVnum;
};

struct RequestSupportRewardSupproter
{
	tINT nRewardType;
};

struct RequestSupportRewardApprentice
{
	tINT nRewardType;
};

struct RequestEventGiftList
{
	tBYTE btType;				// 0 : goods, 1 : present
	tINT nCurPage;
};

struct RequestEventGiftGet
{
	tBYTE btType;				// 0 : goods, 1 : present
	tINT nItemIndex;
};

struct RequestLagrushBuy
{
	tINT nNpcIndex;
	tINT nItemVnum;
	tINT nItemCount;
};

struct RequestFriendReq
{
	tCHAR nCharName[USER_ID_LENGTH];
};

struct RequestFriendRes
{
	tCHAR nCharName[USER_ID_LENGTH];
	tBOOL bAllow;
};

struct RequestFriendDel
{
	tINT nCharIndex;
};

struct RequestFriendRefuseAll
{
	tBOOL bRefuseAll;
};

struct RequestFriendRefuse
{
	tBOOL bRefuse;
	tCHAR strCharName[USER_ID_LENGTH];
};

struct RequestChatTell
{
	tCHAR strCharName[USER_ID_LENGTH];
	tCHAR strChatMsg[CHAT_LENGTH];
};

struct RequestChatGuild
{
	tCHAR strChatMsg[CHAT_LENGTH];
};

struct RequestChatParty
{
	tCHAR strChatMsg[CHAT_LENGTH];
};

struct RequestChatTellRefuse
{
	tBYTE btTypeAllOrOne;
	tCHAR strCharName[USER_ID_LENGTH];
};

struct RequestProcessReq
{
	tCHAR strCharName[USER_ID_LENGTH];
};

struct RequestProcessMoney
{
	tINT64 nLaim;
};

struct RequestProcessInven
{
	tINT nExchangeSlotX;
	tINT nExchangeSlotY;
};

struct RequestRefineReq
{
	tCHAR strCharName[USER_ID_LENGTH];
};

struct RequestRefineMoney
{
	tINT64 nLaim;
};

struct RequestRefineInven
{
	tINT nExchangeSlotX;
	tINT nExchangeSlotY;
};

struct RequestPremiumCharMoveStep1
{
	tCHAR strCharName[USER_ID_LENGTH];
};

struct RequestPremiumCharMoveStep3
{
	tCHAR strCharNameFrom[USER_ID_LENGTH];
	tCHAR strCharNameTo[USER_ID_LENGTH];
};

struct ResponseCharNew
{
	tINT nResult;	
};

struct ResponseCharDel
{
	tINT nResult;	
};

struct ResponseGameStart
{
	tINT nResult;
};

struct ResponseGamePlayReady
{
	tINT nResult;
};

struct ResponseCharGoto
{
	tFLOAT fPosX;
	tFLOAT fPosZ;
	tFLOAT fDir;
};

struct ResponseGoWorld
{
	tINT nResult;
	tCHAR strIp[IP_ADDR_LENGTH];
	tINT nPort;
	tINT nZoneNumber;
	tINT nSubNumber;
};

struct ResponseHeroList
{
	tINT nRace;		// 종족번호
	tINT nPage;
	tINT nTotalNum;
	tCHAR strCharName[HERO_LIST_NUM][USER_ID_LENGTH];
};

struct ResponsePartyRequest
{
	tBYTE btMode;
	tINT nRequestCharIndex;
	tCHAR strRequestCharName[USER_ID_LENGTH];
	tINT nMasterCharIndex;
	tCHAR strMasterCharName[USER_ID_LENGTH];
	tBYTE btExpType;
	tBYTE btItemType;
};

struct ResponsePartyDismiss
{
	tINT nDismissCharIndex;
};

struct ResponsePartySecession
{
	tINT nSecessionCharIndex;
};

struct ResponseCharUseSubItem
{
	tINT nResult;
	tBOOL bDelete;
	tINT nQuickSlotIndex;
	tINT nItemPlusPoint;
	tINT nItemSpecialFlag1;
};

struct ResponseCharCombine
{
	tINT nResult;
	tINT nItemSpecialFlag1;
};

struct ResponseCharSplit
{
	tINT nResult;
	tINT nItemSpecialFlag1;
};

struct ResponseCharProduct
{
	tINT nResult;
	tINT nItemVnum;
	tINT nItemIndex;
	tINT nItemPlusPoint;
	tINT nItemSpecialFlag1;
};

struct ResponseGuildHuntOpen
{
	tCHAR strStartDay[GUILD_HUNT_START_DAY_LENGTH];
	GuildHuntInfo pInfo[GUILD_HUNT_INFO_COUNT];
};

struct ResponseGuildHuntClose
{
	tINT nResult;
};

struct ResponseGuildHuntRequest
{
	tINT nResult;
};

struct ResponseGuildHuntCancel
{
	tINT nResult;
};

struct ResponseCharPlatinumItem
{
	tINT nResult;
	tBYTE btType;
	tINT nItemVnum;
	tINT nItemIndex;
	tINT nItemPlusPoint;
};

struct ResponseCharInterChange
{
	tINT nOrigItemIndex;
	ItemInfo stInfo;
};

struct ResponseExchangeRequest
{
	tCHAR strCharName[USER_ID_LENGTH];
};

struct ResponseExchangeResponse
{
	tBYTE btExchangeResType;	// eExchangeResponseType
};

struct ResponseExchangeStart
{
	tCHAR strCharName[USER_ID_LENGTH];
	tINT nCharLevel;
};

struct ResponseExchangeInvenTo
{
	int nSlotX;
	int nSlotY;
	ItemInfo stInfo;
};

struct ResponseExchangeInvenFrom
{
	int nSlotX;
	int nSlotY;
};

struct ResponseExchangeMoney
{
	tINT nMoenyAmount;
};

struct ResponsePetStatus
{
	tCHAR strPetName[PET_NAME_LENGTH];
	tINT nPetLevel;
	tINT nPetVital;
	tINT nPetAttackPoint;
	tINT nPetDefendPoint;
	tBYTE btPetType;
	tINT nPetSkillLevel[PET_SKILL_TOTAL];
};

struct ResponseItemPick
{
	tINT nInvenPack;
	tINT nSlotX;
	tINT nSlotY;
	ItemInfo stInfo;
};

struct ResponseItemPickExtra
{
	tINT nResult;
	ItemInfo stInfo;
};

struct ResponseItemQuick
{
	tINT nQuickSlotIndex;
	ItemInfo stInfo;
};

struct ResponseItemUpgrade
{
	tINT nResult;
	tBYTE btEquipIndex;
	tINT nItemIndex;
	tINT nPlusPoint;
};

struct ResponseItemupgradeMs
{
	tINT nResult;
	tBYTE btEquipIndex;
	tINT nItemIndex;
	tINT nSpecialFlag1;
};

struct ResponseItemInvenToQuick
{
	tINT nResult;
	tINT nItemIndex;
	tINT nQuickSlotIndex;
};

struct ResponseItemInvenToPetInven
{
	tINT nResult;
	tINT nItemIndex;
	tINT nPetInvenPack;
};

struct ResponseItemShopList
{
	tINT nItemCount;
	tINT nItemVnum[0];
};

struct ResponseItemShopRate
{
	tINT nBuyRate;
	tINT nSellRate;
	tBYTE btShopType;
};

struct ResponseItemSpShopList
{
	tINT nItemCount;
	SpShopItemInfo pInfo[0];
};

struct ResponseItemChipExchange
{
	tINT nResult;
	tINT nItemIndex;
	tINT nItemVnum;	
};

struct ResponseItemGambleExchange
{
	tINT nResult;
	tBYTE btGambleType;
	tBOOL bWinLose;
	tBYTE btWinNumber;		// 당첨 번호
	tINT nItemCount;
	tINT nItemIndex[0];
};

struct ResponseEventParentDay
{
	tINT nResult;
	tINT nItemIndex;
	tINT nItemVnum;
};

struct ResponseEventSummer
{
	tINT nResult;
	tBYTE btType;			// 0 : 아이템, 1 : 라임
	tINT nItemIndex;		// 라임의 경우 라임 수량
	tINT nItemVnum;
};

struct ResponseItemJewelExchange
{
	tINT nResult;
	tINT nItemIndex;
	tINT nItemVnum;
};

struct ResponseItemSelfRefine
{
	tINT nResult;
	tBOOL bClear;
	tINT nItemIndex;
	tINT nItemVnum;
	tINT nSlotX;
	tINT nSlotY;
};

struct ResponseStashList
{
	tINT nItemCount;
	StashItemInfo pInfo[0];
};

struct ResponseStashInfo
{
	tINT nStashPackMax;
	tINT nRemainTime;		// 단위 초
};

struct ResponseStashTo
{
	tBOOL bSwap;
	StashItemInfo stInfo;
};

struct ResponseStashFrom
{
	StashItemInfo stInfo;
};

struct ResponseStashPut
{
	tINT nResult;
	tINT nItemVnum;
	tINT nInvenPack;
	tINT nStashPack;
	tINT nStashSlotIndex;
	tINT nItemCount;
	StashItemMultiInfo pInfo[0];
};

struct ResponseStashGet
{
	tINT nResult;
	tINT nItemVnum;
	tINT nInvenPack;
	tINT nStashPack;
	tINT nStashSlotIndex;
	tINT nItemCount;
	StashItemMultiInfo pInfo[0];
};

struct ResponseStashPuts
{
	tINT nResult;
	tINT nItemVnum;
	tINT nInvenPack;
	tINT nStashSlotIndex;
	tINT nItemCount;
	StashItemMultiInfo pInfo[0];
};

struct ResponseStashGets
{
	tINT nResult;
	tINT nItemVnum;
	tINT nInvenPack;
	tINT nStashSlotIndex;
	tINT nItemCount;
	StashItemMultiInfo pInfo[0];
};

struct ResponseExchangeMulti
{
	tBOOL bIsMyInven;
	ItemInfo stItemInfo;
	tINT nInvenPack;
	tINT nItemCount;
	ItemWithIndexMultiInfo pInfo[0];	// toInfo, fromInfo nItemCount 개씩
};

struct ResponseCharSkillLevel
{
	tINT nSkillIndex;
	tINT nSkillLevel;
};

struct ResponseCastleBattleTime
{
	tINT nResult;
	tCHAR strTime[TIME_STRING_LENGTH];
	tINT nStartTime;
	tBYTE btType;
};

struct ResponseCastleEnergyUp
{
	tINT nResult;
	tINT nItemIndex;
};

struct ResponsePetComposite
{
	tINT nResult;
	tINT nItemIndex;
	tINT nItemVnum;
	tINT nSlotX;
	tINT nSlotY;
};

struct ResponsePetChangePiece
{
	tINT nResult;
};

struct ResponsePetBuyEgg
{
	tINT nResult;
};

struct ResponsePetLevelUp
{
	tINT nResult;
};

struct ResponsePetKeep
{
	tINT nResult;
	tINT nRemainTime;		// 단위 초
	tCHAR strPetName[PET_NAME_LENGTH];
	tBOOL bKeepOut;			// true : 찾기 / false : 넣기
	tINT nPetLevel;
	tINT nPetVital;
	tINT nPetRoyalty;
	tINT nPetHunger;
	tINT64 nLaim;				// 찾는 금액
};

struct ResponsePetSummon
{
	tINT nResult;
	tINT nQuickSlotIndex;
};

struct ResponsePetShop
{
	tINT nResult;
};

struct ResponsePetDrop
{
	tINT nResult;
};

struct ResponsePetPeragonDrop
{
	tINT nResult;
};

struct ResponseWarInfo
{	
	tINT nWarState;
	tCHAR strGuildName[GUILD_NAME_LENGTH];
	tCHAR strGuildMasterName[USER_ID_LENGTH];
	tINT nLaim;
	tINT nMaxCount;
};

struct ResponseWarRequest
{
	tINT nResult;
};

struct ResponseWarHead
{
	tCHAR strTeamA_MasterName[USER_ID_LENGTH];
	tCHAR strTeamB_MasterName[USER_ID_LENGTH];
};

struct ResponseWarCancel
{
	tINT nReason;
};

struct ResponseGpkRequest
{
	tCHAR strGuildName[GUILD_NAME_LENGTH];
};

struct ResponseGpkSurrender
{
	tCHAR strGuildName[GUILD_NAME_LENGTH];
	tCHAR strGuildMasterName[USER_ID_LENGTH];
};

struct ResponseItemBraceletUpgrade
{
	tINT nResult;
	tINT nSpecialFlag1;
};

struct ResponseItemNecklaceUpgrade
{
	tINT nResult;
	tINT nSpecialFlag1;
	tINT nPlusPoint;
};

struct ResponseSupportRequest
{
	tCHAR strCharName[USER_ID_LENGTH];
};

struct ResponseSupportAccept
{
	tINT nResult;
	tINT nRemainTime;		// 단위 초
};

struct ResponseSupportDisband
{
	tINT nCharIndex;
	tCHAR nCharName[USER_ID_LENGTH];
};

struct ResponseMinimap
{
	tINT nMinimapPosX;
	tINT nMinimapPosY;
	tCHAR strCharName[USER_ID_LENGTH];
	tCHAR strChat[CHAT_LENGTH];
};

struct ResponseItemRingCombine
{
	tINT nResult;
	tINT nSpecialFlag1;
	tINT nPlusPoint;
};

struct ResponseQuestExchange
{
	tINT nResult;
	tINT nLaim;
};

struct ResponseItemNpcUpgrade
{
	tBYTE btItemNpcUpgradeCmd;		// eItemNpcUpgradeCmd
	tBYTE btResult;					// eItemNpcUpgradeResult
	tBYTE btCheckType;
};

struct ResponseItemEndurance
{
	tINT nResult;
};

struct ResponseGuildFindPos
{
	tINT nMemberCount;
	GuildPosInfo pInfo[0];
};


struct ResponseUsershopRegistry
{
	tINT nResult;
	tINT nUsershopSlotIndex;
	tINT nGold;
	tINT nSilver;
	tINT nBronze;
	tINT64 nLaim;
	tINT nRegCount;
	ItemMultiInfo pInfo[0];		// Inven slots
};

struct ResponseUsershopUnregistry
{
	tINT nResult;
	tINT nUsershopSlotIndex;
	tINT nUnregCount;
};

struct ResponseUsershopStart
{
	tINT nResult;
};

struct ResponseUsershopModify
{
	tINT nResult;
};

struct ResponseUsershopBuy
{
	tINT nResult;
	tINT nUsershopSlotIndex;
	tINT nBuyCount;
};

struct ResponseUsershopOpen
{
	tINT nResult;
	tBYTE btCharType;			// eCharType
	tINT nCharIndex;			// 판매자 인덱스
	tINT nUsershopGrade;
};

struct ResponseUsershopSell
{
	tINT nResult;
	tINT nUsershopSlotIndex;		// 팔린 아이템 상점 슬롯 번호
	tINT nTotalGold;
	tINT nTotalSilver;
	tINT nTotalBronze;
	tINT64 nTotalLaim;
	tINT nSellCount;
};

struct ResponseUsershopLeave
{
	tINT nResult;
};

struct ResponseUsershopClose
{
	tINT nResult;	// 0 : 성공, 1 : 구매자 일 때 판매자가 닫음, 2 : 덜회수
};

struct ResponseUsershopRecall
{
	tINT nResult;	// 0 : 모두회수, 1 : 회수할 물건 없음, 2 : 덜회수
};

struct ResponseItemRepair
{
	tINT nResult;
};

struct ResponseItemEnchant
{
	tINT nResult;
};

struct ResponseMinorDate
{
	tCHAR strDate[MINOR_DATE_LENGTH];
};

struct ResponseGuildCreateTest
{
	tINT nResult;
};

struct ResponseGuildCreate
{
	tINT nResult;
};

struct ResponseGuildSearch
{
	tINT nGuildCount;
	GuildSearchInfo pInfo[0];
};

struct ResponseGuildDisband
{
	tINT nResult;
};

struct ResponseGuildStashOpen
{
	tINT nResult;
	tINT nCount;
	GuildStashItemInfo pInfo[0];
};

struct ResponseGuildStashPut
{
	GuildStashItemInfo stInfo;
	tBOOL bSwap;
};

struct ResponseGuildStashGet
{
	GuildStashItemInfo stInfo;
};

struct ResponseGuildStashPutMulti
{
	tINT nResult;
	tINT nItemVnum;
	tINT nInvenPack;
	tINT nGuildStashPack;
	tINT nGuildStashSlotIndex;
	tINT nItemCount;
	ItemMultiInfo pInfo[0];
};

struct ResponseGuildStashGetMulti
{
	tINT nResult;
	tINT nItemVnum;
	tINT nInvenPack;
	tINT nGuildStashPack;
	tINT nGuildStashSlotIndex;
	tINT nItemCount;
	ItemWithIndexMultiInfo pInfo[0];
};

struct ResponseGuildLeave
{
	tINT nResult;
};

struct ResponseGuildUpgrade
{
	tINT nResult;
};

struct ResponseGuildRankwarInvite
{
	tINT nResult;
};

struct ResponseGuildRankwarOpen
{
	tINT nGuildRankwarType;
};

struct ResponseGuildRankwarList
{
	tINT nResult;
};

struct ResponseGuildRankwarRequest
{
	tINT nResult;
};

struct ResponseGuildRankwarCancel
{
	tINT nResult;
};

struct ResponseGuildRankwarGet
{
	tINT nResult;
};

struct ResponseGuildRankwarComp
{
	tINT nResult;
};

struct ResponseGuildInvasionRequest
{
	tINT nResult;
};

struct ResponseGuildInvasionCancelReq
{
	tCHAR strGuildName[GUILD_NAME_LENGTH];
	tCHAR strMasterName[USER_ID_LENGTH];
};

struct ResponseGuildInvasionCancelRes
{
	tINT nResult;
	tCHAR strGuildName[GUILD_NAME_LENGTH];
	tCHAR strMasterName[USER_ID_LENGTH];
};

struct ResponseGuildRecommandReq
{
	tINT nResult;
};

struct ResponseGuildRecommandRes
{
	tCHAR strGuildName[GUILD_NAME_LENGTH];
	tCHAR strCharName[USER_ID_LENGTH];	
};

struct ResponseGuildRecommandInfo
{
	tINT nResult;
	tCHAR strGuildName[GUILD_NAME_LENGTH];
};

struct ResponseGuildJoinReq
{
	tINT nResult;
};

struct ResponseGuildJoinRes
{
	tINT nResult;
};

struct ResponseGuildNick
{
	tINT nResult;
	tINT nGuildGrade;
	tCHAR strGuildGrade[GUILD_GRADE_LENGTH];
};

struct ResponseGuildNotice
{
	tINT nResult;
};

struct ResponseGuildAppoint
{
	tINT nResult;
};

struct ResponseGuildFire
{
	tINT nResult;
};

struct ResponseWantedRegistTest
{
	tINT nResult;
};

struct ResponseWantedRegist
{
	tINT nResult;
};

struct ResponseWantedMyList
{
	tINT nCount;
	WantedListInfo pInfo[0];
};

struct ResponseWantedMyCancel
{
	tINT nResult;
	tINT nLaim;
};

struct ResponseWantedList
{
	tINT nCount;
	WantedListInfo pInfo[0];
};

struct ResponseWantedReward
{
	tINT nResult;
};

struct ResponseFreepvpReq
{
	tCHAR strCharName[USER_ID_LENGTH];
};

struct ResponseFreepvpRes
{
	tCHAR strCharName[USER_ID_LENGTH];
	tBOOL bAllow;
};

struct ResponseOccpTrans
{
	tINT nCount;
	OccpTransInfo pInfo[0];
};

struct ResponseOccpTransPortal
{
	tINT nResult;
};

struct ResponseOccpTransGate
{
	tINT nResult;
	tINT nLocation;
};

struct ResponseOccpList
{
	OccpBattleInfo pInfo[OCCP_LIST_COUNT];
};

struct ResponseOccpBattleInfo
{
	tINT timeBattle;
	tCHAR strCharName[USER_ID_LENGTH];
};

struct ResponseOccpCashOpen
{
	tINT nTaxTotal;			// 세금 징수액
	tINT nTexRate;			// 세율
};

struct ResponseEventGiftList
{
	tBYTE btType;				// 0 : goods, 1 : present
	tINT nItemTotalCount;
	tINT nCurPage;
	tINT nItemListCount;
	EventGiftInfo pInfo[0];
};

struct ResponseEventGiftGet
{
	tBYTE btType;				// 0 : goods, 1 : present
	tINT nResult;
	tINT nItemIndex;
	tINT nItemRemainCount;
};

struct ResponseGuildAttendCheck
{
	tINT nResult;
};

struct ResponseLagrushBuy
{
	tINT nResult;
};

struct ResponseItemSeparate
{
	tINT nResult;
};

struct ResponseItemMakes
{
	tINT nResult;
};

struct ResponseYutThrow
{
	tINT nResult;
	tINT nDice;
	tINT nPos;
};

struct ResponseCharMultiShot
{
	tINT nMultiShotCount;
};

struct ResponseShopMultiBuy
{
	tINT nResult;
};

struct ResponsePetResult
{
	tBYTE btResultType;
	tINT nResult;
};

struct ResponseItemUse
{
	tINT nResult;
	tINT nQuickSlotIndex;
	tINT nItemIndex;
	tINT nValue1;
	tINT nValue2;
	tINT nValue3;
};

struct ResponseProcessReq
{
	tCHAR strCharName[USER_ID_LENGTH];
};

struct ResponseProcessAccept
{
	tBYTE btTargetType;		// 0 : 요청자, 1 : 응답자
	tCHAR strCharName[USER_ID_LENGTH];
};

struct ResponseProcessMoney
{
	tINT64 nLaim;
};

struct ResponseProcessInvenTo
{
	tINT nSlotX;
	tINT nSlotY;
	ItemInfo stInfo;
};

struct ResponseProcessInvenFrom
{
	tINT nSlotX;
	tINT nSlotY;
};

struct ResponseProcessResult
{
	tBOOL bSuccess;
};

struct ResponseItemMix
{
	tINT nSlotX;
	tINT nSlotY;
	ItemInfo stInfo;
};

struct ResponseRefineReq
{
	tCHAR strCharName[USER_ID_LENGTH];
};

struct ResponseRefineAccept
{
	tBYTE btTargetType;		// 0 : 요청자, 1 : 응답자
	tCHAR strCharName[USER_ID_LENGTH];
};

struct ResponseRefineMoney
{
	tINT64 nLaim;
};

struct ResponseRefineInvenTo
{
	tINT nSlotX;
	tINT nSlotY;
	ItemInfo stInfo;
};

struct ResponseRefineInvenFrom
{
	tINT nSlotX;
	tINT nSlotY;
};

struct ResponseRefineResult
{
	tBOOL bSuccess;
	tBOOL bClearFlag;
	tINT nItemIndex;
	tINT nItemVnum;
	tINT nSlotX;
	tINT nSlotY;
};

struct ResponseItemUsePremiumItemChar
{
	tINT nResult;
	tINT nCharIndex;
	tCHAR strCharName[USER_ID_LENGTH];
};

struct ResponseItemUsePremiumItemGuild
{
	tINT nResult;
	tINT nGuildIndex;
	tCHAR strGuildName[GUILD_NAME_LENGTH];
};

struct ResponsePremiumCharMoveStep1
{
	tINT nResult;
};

struct ResponsePremiumCharMoveStep2
{
	tINT nResult;
	tCHAR strCharNameFrom[USER_ID_LENGTH];
	tCHAR strCharNameTo[USER_ID_LENGTH];
	tINT nCharLevel;
	tINT nCharRace;
	ItemInfo pInfo[CHAR_WEAR_TOTAL_COUNT];
};

//////////////////////////////////////////////////////////////////////////
// Server Update
struct UpdateCharExist
{
	tINT nCharCount;
	PreCharInfo pInfo[0];
};

struct UpdateUserExpired
{
	tINT nUserIndex;
};

struct UpdateCharStatus
{
	tBYTE btStatusType;		// eCharStatusType
	tINT nValue;
};

struct UpdateCharSkillLevel
{
	SkillLevel stInfo;
};

struct UpdateCharSkillLevelAll
{
	tINT nCount;
	SkillLevel pInfo[0];
};

struct UpdateMyCharIndex
{
	tINT nIndex;
};

struct UpdateMyCharName
{
	tCHAR szId[USER_ID_LENGTH];
};

struct UpdateMyCharAdminLevel
{
	tINT nAdminLevel;
};

struct UpdateMyCharWearing
{
	tINT nCount;
	ItemInfo pInfo[0];
};

struct UpdateMyCharEvWearing
{
	tINT nCount;
	ItemInfo pInfo[0];
};

struct UpdateMyCharEvWearTime
{
	tINT nRemainMinute;		// -1 : No Limit
};

struct UpdateCharWear
{
	tINT nCharIndex;
	tINT nWearWhere;
	tINT nItemVnum;
	tINT nItemPlusPoint;
};

struct UpdateCharEvWear
{
	tINT nCharIndex;
	tINT nWearWhere;
	tINT nItemVnum;
	tINT nItemPlusPoint;
};

struct UpdateCharEvShowFlag
{
	tINT nCharIndex;
	tBOOL bShow;
};

struct UpdateMyCharInven
{
	tINT nInvenPack;
	tINT nSlotX;
	tINT nSlotY;
	ItemInfo stInfo;
};

struct UpdateMyCharExtraInven
{
	ItemInfo stInfo;
};

struct UpdateMyCharQuickSlot
{
	tINT nQuickSlotIndex;
	ItemInfo stInfo;
};

struct UpdateMyCharLaghaimPoint
{
	tINT nLaghaimPoint;
};

struct UpdateServerNumber
{
	tINT nServerNumber;
	tINT nSubNumber;
};

struct UpdateMapAttribute
{
	tBYTE nAttribute;
	tFLOAT fMinX;
	tFLOAT fMinZ;
	tFLOAT fMaxX;
	tFLOAT fMaxZ;
};

struct UpdateCharAt
{
	tINT nCharIndex;
	tFLOAT fPosX;
	tFLOAT fPosZ;
	tFLOAT fDir;
};

struct UpdateMapHour
{
	tBYTE nHour;
};

struct UpdateMapWeather
{
	tBYTE nWeather;
};

struct UpdateMyCharLocateLP
{
	tBYTE btLocateType;
	tINT nLaghaimPoint;
};

struct UpdateMyCharFreeUser
{
	tBOOL bFreeUser;
};

struct UpdateMyCharPrizeAlram
{
	tBOOL bHasPresent;
};

struct UpdateInfoMessage
{
	tCHAR strInfoMessage[INFO_MSG_LENGTH];
};

struct UpdateCharMoveSpeed
{
	tBYTE btSpeed;	// 0 : normal, 1 : fast, 2 : very fast
};

struct UpdateChannelNumber
{
	tINT nChannelNumber;
};

struct UpdateEventFeverTime
{
	tINT nEventValue;
};

struct UpdateCharWalk
{
	tBYTE btCharType;		// eCharType
	tINT nTargetIndex;
	tFLOAT fPosX;
	tFLOAT fPosZ;
	tBOOL bRun;
};

struct UpdateCharPlace
{
	tBYTE btCharType;		// eCharType
	tINT nTargetIndex;
	tFLOAT fPosX;
	tFLOAT fPosZ;
	tFLOAT fDir;
	tINT nRemainFrame;
	tBOOL bRun;
};

struct UpdateCharStop
{
	tBYTE btCharType;		// eCharType
	tINT nTargetIndex;
	tFLOAT fPosX;
	tFLOAT fPosZ;
	tFLOAT fDir;
};

struct UpdateCharSkill
{
	tINT nSkillTime;
	tBYTE btCharType;		// eCharType
	tINT nTargetIndex;
	tINT nSkillIndex;
};

struct UpdateCharPkState
{
	tINT nCharIndex;
	tBOOL bPkOn;
};

struct UpdatePetStatus
{
	tBYTE btPetStatusType;
	tINT nValue;
	tINT nValueMax;
	tINT nMasterCharIndex;
};

struct UpdateLagrushEnable
{
	tBOOL bEnable;
};

struct UpdateLagrushRemainTime
{
	tINT nRemainTime;		// 단위 분
};

struct UpdateHeroRewardEnable
{
	tBOOL bEnable;
};

struct UpdateHeroInfoMessage
{
	tCHAR strInfoMessage[INFO_MSG_LENGTH];
};

struct UpdatePartyList
{
	tINT nPartyMemberCount;
	PartyInfo pInfo[0];
};

struct UpdatePartyTypeChange
{
	tBYTE btCategory;		// 경험치 or 아이템
	tBYTE btType;			// 분배 방식
};

struct UpdatePartyVital
{
	tINT nCharIndex;
	tINT nCharVitalPercent;
	tFLOAT fPosX;
	tFLOAT fPosZ;
};

struct UpdateCharAttack
{
	tBYTE btAttackerType;		// eCharType
	tINT nAttackerIndex;
	tBYTE btDefenderType;		// eCharType
	tINT nDefenderIndex;
	tINT nDefenderVitalPercent;
	tBYTE btAttackType;
	tINT nMobClass;
	tBOOL bHaste;
	tINT nSkillIndex;
};

struct UpdateCharAttackVital
{
	tBYTE btDefenderType;		// eCharType
	tINT nDefenderIndex;
	tINT nDefenderVitalPercent;
	tINT nMobClass;
	tINT nSkillIndex;
};

struct UpdateCharSubAttack
{
	tBYTE btAttackerType;		// eCharType
	tINT nAttackerIndex;
	tBYTE btDefenderType;		// eCharType
	tINT nDefenderIndex;
	tINT nDefenderVitalPercent;
	tBYTE btAttackType;
	tINT nMobClass;
	tBOOL bChakram;
	tINT nSkillIndex;
};

struct UpdateCharSubAttackVital
{
	tBYTE btDefenderType;		// eCharType
	tINT nDefenderIndex;
	tINT nDefenderVitalPercent;
	tBYTE btAttackType;
	tINT nMobClass;
	tINT nSubItemInfo;
	tINT nWearWhere;
};

struct UpdateCharSky
{
	tINT nCharIndex;
	tINT nSkyLevel;
};

struct UpdateGuildHuntDoor
{
	tBOOL bOpen;
};

struct UpdateCharSocialAction
{
	tBYTE btCharType;			// eCharType;
	tINT nCharIndex;
	tINT nSocialIndex;
};

struct UpdateItemDrop
{
	tFLOAT fPosX;
	tFLOAT fPosZ;
	tFLOAT fDir;
	ItemInfo stInfo;
};

struct UpdateItemPick
{
	tINT nCharIndex;
};

struct UpdateCharChangeSex
{
	tINT nCharIndex;
	tBYTE btSex;
};

struct UpdateCharChangeHair
{
	tINT nCharIndex;
	tBYTE btHair;
};

struct UpdateCharLevelUp
{
	tINT nCharIndex;
};

struct UpdateCharCombatState
{
	tINT nCharIndex;
	tINT nCombatState;
};

struct UpdateCharRevive
{
	tINT nCharIndex;
};

struct UpdateChatSales
{
	tINT nCharIndex;
	tCHAR strCharName[USER_ID_LENGTH];
	tCHAR strChat[CHAT_LENGTH];
};

struct UpdateCastleTaxInfo
{
	tINT nLaim;					// 세금 징수액
	tINT nRate;					// 세율
};

struct UpdatePetSocial
{
	tINT nPetIndex;
	tINT nPetSocial;
	tFLOAT fPosX;
	tFLOAT fPosZ;
};

struct UpdateWarChar
{
	tCHAR strCharName[USER_ID_LENGTH];
	tINT nTeamMemGrade;
};

struct UpdateWarPoint
{
	tCHAR strTeamA_MasterName[USER_ID_LENGTH];
	tCHAR strTeamB_MasterName[USER_ID_LENGTH];
	tINT nTeamA_Point;
	tINT nTeamB_Point;
	tINT nCharIndex;
};

struct UpdateWarNotify
{
	tBYTE btWarNotifyType;		// eWarNotifyType
};

struct UpdateGpkPlay
{
	tCHAR strTeamA_GuildName[GUILD_NAME_LENGTH];
	tCHAR strTeamB_GuildName[GUILD_NAME_LENGTH];
};

struct UpdateGpkStop
{
	tCHAR strGuildName[GUILD_NAME_LENGTH];
};

struct UpdateSupportSuccessFail
{
	tBOOL bSuccess;
	tBYTE btSupportType;		// eSupportType
	tINT nCharIndex;
	tCHAR nCharName[USER_ID_LENGTH];
};

struct UpdateSupportEnable
{
	tBYTE btSupportEnableType;	// eSupportEnableType
};

struct UpdateSupportInfo
{
	tBYTE btSupportType;		// eSupportType
	tINT nCount;
	SupportApprenticeInfo pInfo[0];
};

struct UpdateSupportConnect
{
	tINT nCharIndex;
	tINT nZone;					// -1 : 미접속
};

struct UpdateQuestGet
{
	tINT nQuestIndex;
	tBOOL bCancel;
};

struct UpdateQuestEnd
{
	tINT nQuestIndex;	
};

struct UpdateQuestKing
{
	tINT nNpcIndex;
	tBOOL bIsKing;
};

struct UpdateQuestKill
{
	tINT nQuestSlotIndex;
};

struct UpdateQuestQuick
{
	tINT nQuickSlotIndex;
	tINT nSpecialFlag1;
};

struct UpdateQuestFail
{
	tINT nReason;
};

struct UpdateQuestEffect
{
	tFLOAT fPosX;
	tFLOAT fPosZ;
	tINT nCharIndex;
};

struct UpdateQuestTotalPoint
{
	tINT nTotalPoint;
};

struct UpdateQuestPoint
{
	tINT nRemainPoint;		// 남은 포인트
	tINT nAddPoint;			// 누적 포인트
};

struct UpdateQuestBonus
{
	tBOOL bIsBonus;
};

struct UpdatePrize
{
	tBOOL bHasPrize;
};

struct UpdateUsershopCharIn
{
	tINT nCharIndex;
	tINT nUsershopLevel;
	tCHAR strUsershopTitle[USERSHOP_TITLE_LENGTH];
};

struct UpdateUsershopCharOut
{
	tINT nCharIndex;
};
	
struct UpdateUsershopNpcIn
{
	tINT nCharIndex;					// 상점 주인 인덱스
	tINT nNpcIndex;
	tINT nNpcVnum;
	tFLOAT fPosX;
	tFLOAT fPosZ;
	tFLOAT fDir;
	tCHAR strCharName[USER_ID_LENGTH];	// 주인 캐릭터 이름
	tCHAR strUsershopTitle[USERSHOP_TITLE_LENGTH];
};
	
struct UpdateUsershopNpcOut
{
	tINT nNpcIndex;
};

struct UpdateCharRotate
{
	tBYTE btCharType;
	tINT nCharIndex;
	tFLOAT fDir;
};

struct UpdateSoccerTime
{
	tINT nRemainTime;
};

struct UpdateSoccerSocre
{
	tINT nTeamA_Score;
	tINT nTeamB_Score;
};

struct UpdateSoccerName
{
	tINT nTeamA_GuildName[GUILD_NAME_LENGTH];
	tINT nTeamB_GuildName[GUILD_NAME_LENGTH];
};

struct UpdateCharUseEffect
{
	tINT nCharIndex;
	tINT nEffectIndex;
	tBOOL bOn;
};

struct UpdateMinorGuildName
{
	tCHAR strTeamA_GuildName[GUILD_NAME_LENGTH];
	tCHAR strTeamB_GuildName[GUILD_NAME_LENGTH];
};

struct UpdateGuildRankwarInfo
{
	tCHAR strTeamA_GuildName[GUILD_NAME_LENGTH];
	tCHAR strTeamB_GuildName[GUILD_NAME_LENGTH];
};

struct UpdateGuildRankwarTime
{
	tINT nRemainTime;
};

struct UpdateGuildRankwarKillCount
{
	tINT nTeamA_KillCount;
	tINT nTeamB_KillCount;
};

struct UpdateGuildRankwarGoworld
{
	tINT nZone;
	tINT nTown;
};

struct UpdateGuildInvasionInfo
{
	tBYTE btAddRemove;		// 0 : remove, 1 : add
	tBYTE btAttDef;			// 0 : def, 1 : att
	tINT nGuildIndex;
	tCHAR strGuildName[GUILD_NAME_LENGTH];
	tINT nTime;
};

struct UpdateGuildNotice
{
	tCHAR strGuildNotice[GUILD_NOTICE_LENGTH];
};

struct UpdateGuildInfo
{
	tBYTE btAddModify;		// 0 : add, 1 : modify
	tINT nGuildIndex;
	tCHAR strGuildName[GUILD_NAME_LENGTH];
	tINT nGuildRank;
	tINT nGuildLevel;
	tINT nGuildPoint;
	tCHAR strGuildGrade[GUILD_GRADE_COUNT][GUILD_GRADE_LENGTH];
	tINT timeGuildGradeDown;	// 길드 등급 하락 보정 시간
};

struct UpdateGuildMemberInfo
{
	tINT btAddModify;		// 0 : add, 1 : modify
	tINT nCharIndex;
	tINT nGuildGrade;
	tCHAR strCharName[USER_ID_LENGTH];
	tINT nCharRace;
	tINT nZone;
	tINT nJoinType;
	tINT timeJoinWait;
};

struct UpdateGuildWarning
{
	tINT timeInvasion;
};

struct UpdateGuildMsg
{
	tINT nMsgType;
	tCHAR strValue1[USER_ID_LENGTH];
	tCHAR strValue2[GUILD_GRADE_LENGTH];
};

struct UpdateWantedMark
{
	tINT nCharIndex;
	tBOOL bMarkOn;
};

struct UpdateFreepvpRing
{
	tFLOAT fPosX;
	tFLOAT fPosZ;
};

struct UpdateFreepvpStart
{
	tINT nCharIndex1;
	tINT nCharIndex2;
};

struct UpdateFreepvpEnd
{
	tINT nCharIndex;	
};

struct UpdateFreepvpResult
{
	tINT nCharIndex;
	tBOOL bWin;
};

struct UpdateFreepvpIn
{
	tINT nCharIndex;
};

struct UpdateFreepvpVital
{
	tINT nMyVital;
	tINT nTargetVital;
};

struct UpdateFreepvpRecord
{
	tINT nMyWinCount;
	tINT nMyLoseCount;
	tINT nTargetWinCount;
	tINT nTargetLoseCount;
};

struct UpdateFreepvpError
{
	tINT nErrorCode;
};

struct UpdateOccpBattleEnd
{
	tINT nLocation;
};

struct UpdateOccpBattleFail
{
	tINT nErrorCode;
};

struct UpdateOccpInfo
{
	OccpBattleInfo pInfo[OCCP_LIST_COUNT];
};

struct UpdateOccpBuff
{
	tINT nOccpBuffType;
	tINT nOccpBuffState;
};

struct UpdateOccpCount
{
	OccpBattleCountInfo pInfo[OCCP_LIST_COUNT];
};

struct UpdateFriendReq
{
	tINT nCharIndex;
	tCHAR strCharName[USER_ID_LENGTH];
};

struct UpdateFriendAdd
{
	tINT nCharIndex;
	tCHAR strCharName[USER_ID_LENGTH];
};

struct UpdateFriendDel
{
	tINT nCharIndex;
};

struct UpdateFriendRefuseAll
{
	tBOOL bRefuseAll;
};

struct UpdateFriendRefuse
{
	tBOOL bRefuse;
	tCHAR strCharName[USER_ID_LENGTH];
};

struct UpdateFriendList
{
	tINT nFriendCount;
	FriendInfo pInfo[0];
};

struct UpdateFriendRefuseList
{
	tINT nRefuseCount;
	FriendInfo pInfo[0];
};

struct UpdateFriendLogin
{
	tINT nCharIndex;
	tINT nChannel;
	tINT nZone;
};

struct UpdateFriendLogout
{
	tINT nCharIndex;
};

struct UpdateFriendZone
{
	tINT nCharIndex;
	tINT nZone;
};

struct UpdateYutInfo
{
	tINT nCompleteCount;
	tINT nPos;
	tBOOL bMapIn;
};

struct UpdateMapInChar
{
	tBYTE btType;			// 0 : in, 1 : appear
	tINT nCharIndex;
	tCHAR nCharName[USER_ID_LENGTH];
	tINT nCharRace;
	tINT nCharSex;
	tINT nCharHair;
	tFLOAT fPosX;
	tFLOAT fPosZ;
	tFLOAT fDir;
	tINT nItemVnum[CHAR_MAPIN_WEAR_COUNT];
	tINT nVital;
	tINT nCombatState;
	tINT nSkillIndex;
	tINT nState;
	tINT nSubSkill;
	tINT nChaoGrade;
	tINT nPK;
	tINT nSky;
	tCHAR strGuildName[GUILD_NAME_LENGTH];
	tCHAR strGuildGrade[GUILD_GRADE_LENGTH];
	tINT nGuildLevel;
	tINT nGuildIndex;
	tINT nGuildType;
};

struct UpdateMapInNpc
{
	tINT nNpcIndex;
	tINT nNpcVnum;
	tFLOAT fPosX;
	tFLOAT fPosZ;
	tFLOAT fDir;
	tINT nMobEventFlag;
	tINT nVital;
	tINT nMutant;
	tINT nAttribute;
	tINT nMobClass;
	tINT nState;
	tINT nQuestLevel;
	tINT nRegen;		// 워자이언트 리젠
};

struct UpdateMapInPet
{
	tBYTE btType;			// 0 : in, 1 : appear
	tINT nPetIndex;
	tCHAR strPetName[PET_NAME_LENGTH];
	tINT nPetClass;
	tFLOAT fPosX;
	tFLOAT fPosZ;
	tFLOAT fDir;
	tINT nItemVnum[6];
	tINT nVital;
	tINT nItemVnum2[PET_MAPIN_WEAR_COUNT];
	tINT nPetMode;
	tCHAR strCharName[USER_ID_LENGTH];		// 펫 주인 이름
	tINT nMeta;
};

struct UpdateMapInItem
{
	ItemInfo stInfo;
	tBOOL bTimeItem;
	tFLOAT fPosX;
	tFLOAT fPosZ;
	tFLOAT fDir;
};

struct UpdateMapOut
{
	tBYTE btCharType;		// eCharType
	tINT nCharIndex;
};

struct UpdateCharKill
{
	tBYTE btCharType;
	tINT nCharIndex;
};

struct UpdateCharWearRemove
{
	tINT nCharIndex;
	tINT nWearWhere;
};

struct UpdateCharEvWearRemove
{
	tINT nCharIndex;
	tINT nWearWhere;
};

struct UpdateNotice
{
	tCHAR strNoticeMsg[INFO_MSG_LENGTH];
};

struct UpdateEffect
{
	tINT nSkillIndex;
	tBYTE btCharType;
	tINT nCharIndex;
	tBYTE btTargetCharType;
	tINT nTargetCharIndex;
	tINT nTargetVital;
	tINT nSpecialAttack;
	tINT nAttackType;
	tINT nMobClass;
	tINT nSkillStep;
	tINT nSkillLevel;
};

struct UpdateGuildName
{
	tINT nCharIndex;
	tCHAR strGuildName[GUILD_NAME_LENGTH];
};

struct UpdateGuildGrade
{
	tINT nCharIndex;
	tINT nGuildIndex;
	tCHAR strGuildGrade[GUILD_GRADE_LENGTH];
	tINT nGuildGrade;
};

struct UpdateCharMultiShot
{
	tINT nCharIndex;
	tINT nMultiShotCount;
};

struct UpdateEvent
{
	tBYTE btType;
	tINT nValue1;
};

struct UpdateYouAre
{
	tINT nAdminLevel;
};

struct UpdateMapOpenCastle
{
	tINT nCastleDoorIndex;
};

struct UpdateMapOpenDoor
{
	tINT nZone;
	tBOOL bOpen[DOOR_MAX_COUNT];
};

struct UpdateMapBattleOn
{
	tBOOL bBattleOn;
	BattleInfo pInfo[DOOR_MAX_COUNT];
};

struct UpdateMapBattleCount
{
	tINT nRemainTime;		// 단위 초
};

struct UpdateMapBattleInfo
{
	tCHAR strGuildName[GUILD_NAME_LENGTH];
	tCHAR strCharName[USER_ID_LENGTH];		// 수정 소유자
	tINT nTime;								// 수정 소유 시간
	tCHAR strBestGuildName[GUILD_NAME_LENGTH];
	tINT nBestTime;
	tFLOAT fPosX;
	tFLOAT fPosZ;
	tINT nMyGuildPoint;
	BattleFlagInfo pInfo[BATTLE_FLAG_COUNT];
};

struct UpdatePopupMsg
{
	tCHAR strMsg[INFO_MSG_LENGTH];
};

struct UpdatePopupMsg2
{
	tINT nClientStringIndex;
};

struct UpdateGuildLevel
{
	tINT nGuildLevel;
};

struct UpdateMyPet
{
	tINT nPetIndex;
	tCHAR strPetName[PET_NAME_LENGTH];
	tINT nPetClass;
	tFLOAT fPosX;
	tFLOAT fPosZ;
	tFLOAT fDir;
	tINT nItemVnum[PET_MAPIN_WEAR_COUNT];
};

struct UpdateCharCondition
{
	tBYTE btCharType;
	tINT nCharIndex;
	tINT nState;
	tBOOL bOn;
};

struct UpdateNpcConDistruction
{
	tINT nNpcIndex;
	tINT nClassType;
};

struct UpdateCharSubEffect
{
	tINT nCharIndex;
	tBYTE btTargetCharType;
	tINT nTargetCharIndex;
	tBOOL bOn;
};

struct UpdateCharSubEffectMe
{
	tINT nCharIndex;
	tBOOL bOn;
};

struct UpdateCharSummon
{
	tBYTE btMasterCharType;		// eCharType	// 소환자
	tINT nMasterCharIndex;
	tBYTE btSummonCharType;		// eCharType	// 소환물
	tINT nSummonCharIndex;
	tFLOAT fPosX;
	tFLOAT fPosZ;
	tINT nSummonFlag;			// 소환 : 1, 11 / 소멸 : 0, 10
};

struct UpdateCharChao
{
	tINT nCharIndex;
	tINT nChaoGrade;
};

struct UpdateCharJump
{
	tINT nCharIndex;
	tFLOAT fPosX;
	tFLOAT fPosZ;
};

struct UpdateCharMultiAttack
{
	tINT nNpcIndex;
	tINT nCount;
	MultiAttackInfo pInfo[0];
};

struct UpdateEventAncient
{
	tINT nResult;
	tBYTE btType;
	tINT nCount;
	EventAncientInfo pInfo[0];
};

struct UpdateEventArborDay
{
	tINT nResult;
	tINT nItemIndex;
	tINT nItemVnum;
};

struct UpdateItemTeleport
{
	TeleportItemInfo pInfo[TELEPORT_LIST_COUNT];
};

struct UpdateItemCandy
{
	tINT nCharIndex;
};

struct UpdateItemApple
{
	tINT nCharIndex;
};

struct UpdateItemStarCandy
{
	tINT nCharIndex;
};

struct UpdateCharAttackMiss
{
	tBYTE btCharType;
	tINT nCharIndex;
	tINT nSkillIndex;
};

struct UpdateCharAttackBlock
{
	tBYTE btBlockedCharType;
	tINT nBlockedCharIndex;
	tBYTE btAttackCharType;
	tINT nAttackCharIndex;
};

struct UpdateEventPointCheck
{
	tINT nCharIndex;
};

struct UpdateCharPkItemDrop
{
	tINT nInvenPack;
	tINT nSlotX;
	tINT nSlotY;
	tINT nWidth;
	tINT nHeight;
};

struct UpdateEffectFirework
{
	tINT nCharIndex;
	tBYTE btFireType;
};

struct UpdateCharDStone
{
	tBOOL bCreate;
	tINT nCharIndex;
	tINT nNpcIndex;
	tINT nNpcVnum;
	tFLOAT fPosX;
	tFLOAT fPosZ;
	tFLOAT fDir;
};

struct UpdateCharCombo
{
	tINT nCharIndex;
	tINT nComboFrame;
};

struct UpdateItemExtraInfoTime
{
	tINT nItemIndex;
	tINT nRemainTime;		// 단위 초
};

struct UpdateItemExtraInfoDel
{
	tINT nItemIndex;
};

struct UpdateItemExtraInfoPrice
{
	tINT nItemIndex;
	tBYTE btItemPriceType;
	tINT nItemPrice;
};

struct UpdateItemExtraInfoAll
{
	tINT nItemIndex;
	tINT nRemainTime;		// 단위 초
	tBYTE btItemPriceType;
	tINT nItemPrice;
};

struct UpdateCharSummonSquad
{
	tINT nCharIndex;
};

struct UpdateMyCharUpdateItem
{
	ItemInfo stInfo;
};

struct UpdateCharMovingSkill
{	
	tINT nSkillIndex;
	tINT nCharIndex;
	tINT nSkillStep;
	tINT nTargetCount;
	MovingSkillInfo pInfo[0];
};

struct UpdateNpcWear
{
	tINT nNpcIndex;
	tINT nItemVnum;
};

struct UpdateBpShopList
{
	tINT nListCount;
	BpShopListInfo pInfo[0];
};

struct UpdatePetEffect
{
	tINT nPetIndex;
	tINT nSkillIndex;
	tBYTE btTargetCharType;
	tINT nTargetCharIndex;
	tINT nTargetVital;
	tINT nSpecialAttack;
	tINT nMobClass;
	tINT nSkillStep;
};

struct UpdateSpiritStart
{
	tINT nCharIndex;
	tINT nTargetPoint;
	tINT nRemainTime;			// 단위 분
};

struct UpdateSpiritPoint
{
	tINT nGetPoint;
	tINT nTargetPoint;
};

struct UpdateSpiritTime
{
	tINT nRemainTime;			// 단위 분
};

struct UpdateSpiritEnd
{
	tINT nResult;
};

struct UpdateMapChangeAttribute
{
	tINT nAttribute;
	tFLOAT fMinX;
	tFLOAT fMinZ;
	tFLOAT fMaxX;
	tFLOAT fMaxZ;
};

struct UpdateEventAction
{
	tINT nActionIndex;
	tFLOAT fPosX;
	tFLOAT fPosZ;
	tFLOAT fDir;
};

struct UpdateSkillCooltime
{
	tINT nSkillIndex;
	tBYTE btCoolType;		// 0 : disable, 1 : able, 2 : cool start
};

struct UpdateBossraidAttack
{
	tINT nSkillIndex;
	tINT nBossNpcIndex;
	tINT nTargetCharIndex;
	tFLOAT fPosX;
	tFLOAT fPosZ;
};

struct UpdateBossraidKillFlag
{
	tBOOL bKilled[15];
};

struct UpdateBossraidRankList
{
	tCHAR nCharName[15][USER_ID_LENGTH];
};

struct UpdateLpShopList
{
	tINT nListCount;
	BpShopListInfo pInfo[0];
};

struct UpdateCharFullsetFlag
{
	tBYTE btFlagType;
	tBOOL bOn;
};

struct UpdateInvenInit
{
	tINT nInvenPack;
};

struct UpdateMyCharRemoveItem
{
	tINT nItemIndex;
};

struct UpdateSupportRewardListApprentice
{
	tINT nListCount;
	tINT nItemVnum[0];
};

struct UpdateItemBuff
{
	tBYTE btBuffType;
	tINT nValue;
};

struct UpdateSkillContinue
{
	tINT nCharIndex;
	tINT nField;
	tINT nFieldEx;
	tINT nDSystem;
};

struct UpdateCharSize
{
	tINT nCharIndex;
	tFLOAT fScale;
};

struct UpdateNpcVisible
{
	tINT nNpcVnum;
	tBOOL bVisible;
};

struct UpdateChatNormal
{
	tINT nCharIndex;
	tINT nAdminLevel;
	tCHAR strChatMsg[CHAT_LENGTH];
};

struct UpdateChatTell
{
	tINT nCharIndex;
	tINT nAdminLevel;
	tCHAR strCharName[USER_ID_LENGTH];
	tCHAR strChatMsg[CHAT_LENGTH];
};

struct UpdateChatGuild
{
	tINT nCharIndex;
	tINT nAdminLevel;
	tCHAR strCharName[USER_ID_LENGTH];
	tCHAR strChatMsg[CHAT_LENGTH];
};

struct UpdateChatParty
{
	tINT nCharIndex;
	tINT nAdminLevel;
	tCHAR strCharName[USER_ID_LENGTH];
	tCHAR strChatMsg[CHAT_LENGTH];
};

#pragma pack(pop)

#endif // _PACKET_DEFINE_H_
