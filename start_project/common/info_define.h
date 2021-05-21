#ifndef _INFO_DEFINE_H_
#define _INFO_DEFINE_H_

#include "rntype.h"

#define USER_ID_LENGTH		50
#define USER_PW_LENGTH		20
#define GUILD_NAME_LENGTH	30
#define WEARING_SHOW		6
#define CHAR_SLOT_MAX		5
#define INFO_MSG_LENGTH		512
#define IP_ADDR_LENGTH		32
#define HERO_LIST_NUM		10
#define GUILD_HUNT_START_DAY_LENGTH	30
#define GUILD_HUNT_INFO_COUNT		5
#define PET_SKILL_TOTAL		8
#define PET_NAME_LENGTH		50
#define CHAT_LENGTH			512
#define TIME_STRING_LENGTH	30
#define USERSHOP_TITLE_LENGTH	50
#define MINOR_DATE_LENGTH	100
#define GUILD_GRADE_LENGTH	30
#define GUILD_NOTICE_LENGTH	1024
#define GUILD_GRADE_COUNT	7
#define OCCP_LIST_COUNT		5
#define DATE_STR_LENGTH		10
#define CHAR_MAPIN_WEAR_COUNT	7
#define PET_MAPIN_WEAR_COUNT	6
#define DOOR_MAX_COUNT			8
#define BATTLE_FLAG_COUNT		4
#define TELEPORT_LIST_COUNT		10
#define CHAR_WEAR_TOTAL_COUNT	12
#define BATTLE_LEAGUE_COUPON	64
#define ITEM_NAME_LENGTH 50

#define USE_SKILL_ONE_TARGET 1

#define WEARING_HELMET	0
#define WEARING_ARMOR	1
#define WEARING_PANTS	2
#define WEARING_CLOAK	3
#define WEARING_SHOES	4
#define WEARING_SHIELD	5

#define WEARING_SHOW_NUM	8
#define WEARING_NECKLACE	6

#define WEARING_RING		7
#define WEARING_BRACELET	8
#define WEARING_WEAPON		9
#define WEARING_PET1		10
#define WEARING_PET2		11
#define WEARING_PET3		12
#define WEARING_PET4		13
#define WEARING_NUM			14

#define BINGO_READY_MAX	5
#define BINGO_PRESENT_MAX 8

#define PETSKILL_INIT_MONEY 10000000

#define NOTE_MESSAGE_NAME_MAX 24
#define NOTE_MESSAGE_TITLE_MAX 80
#define NOTE_MESSAGE_CONTENT 800

#define CH0 0
#define CH1 1

#define SNAKE_DICE_PATH_MAX (7)

enum eCharType
{
	eCharType_Char = 0,
	eCharType_Npc,
	eCharType_Pet,
	eCharType_Item,

	eCharType_Max,
};

enum eChatMultiType
{
	eChatMultiType_Normal = 0,
	eChatMultiType_Guild,
	eChatMultiType_Party,
	eChatMultiType_Shout,
	eChatMultiType_Sales,
	eChatMultiType_Gwtalk,

	eChatMultiType_Max,
};

enum eChatType
{
	eChatType_Tell = 0,
	eChatType_Post,

	eChatType_Max,
};

enum eTargetType
{
	eTargetType_Not  = 0,
	eTargetType_Char,
	eTargetType_Npc,

	eTargetType_Max,
};

enum eUICloseType
{
	eUICloseType_All = 0,

	eUICloseType_Max,
};

enum eSetItemType
{
	eStatusType_MaxVital = 0,
	eStatusType_MaxMana,
	eStatusType_MaxStamina,
	eStatusType_MaxEnerge,
	eStatusType_PVE_DecraseDam,
	eStatusType_PVP_DecraseDam,
	eStatusType_Max
};

enum eBingoRegType
{
	eBingoRegType_One = 0,
	eBingoRegType_Many,
};
enum eBingoPresentStateType
{
	eBingoPresentStateType_Not= 0,
	eBingoPresentStateType_Can,
	eBingoPresentStateType_Already,
};

enum eResponseErrorType_BingoInfo
{
	eResponseErrorType_BingoInfo_Success = 0,
	eResponseErrorType_BingoInfo_NotReady,
};

enum eResponseErrorType_BingoReg
{
	eResponseErrorType_BingoReg_Success = 0,
	eResponseErrorType_BingoReg_Clear,
	eResponseErrorType_BingoReg_OverRap,
	eResponseErrorType_BingoReg_NotReady,
	eResponseErrorType_BingoReg_NotItem,
	eResponseErrorType_BingoReg_NotItemCount,
	eResponseErrorType_BingoReg_NotLp,
};

enum eResponseErrorType_BingoPresent
{
	eResponseErrorType_BingoPresent_Success = 0,
	eResponseErrorType_BingoPresent_NotReady,
	eResponseErrorType_BingoPresent_OverRap,	
	eResponseErrorType_BingoPresent_Fail,	
};

enum eResponseErrorType_Promotion
{
	eResponseErrorType_Promotion_Success = 0,
	eResponseErrorType_Promotion_OverRap,
	eResponseErrorType_Promotion_CouponNot,
	eResponseErrorType_Promotion_LevelNot,
	eResponseErrorType_Promotion_Fail,
};

enum eResponseErrorType_PetSkillInit
{
	eResponseErrorType_PetSkillInit_Success = 0,
	eResponseErrorType_PetSkillInit_Fail,
	eResponseErrorType_PetSkillInit_NotLaim,
	eResponseErrorType_PetSkillInit_NotPet,
	eResponseErrorType_PetSkillInit_NotClass,
};

enum eResponseErrorType_GearUpgrade
{
	eResponseErrorType_GearUpgrade_Success = 0,
	eResponseErrorType_GearUpgrade_Fail,
	eResponseErrorType_GearUpgrade_NotGem,
	eResponseErrorType_GearUpgrade_NotGear,
	eResponseErrorType_GearUpgrade_NotUpgrade,
};

enum eNoteMessageState
{
	eNoteMessageState_Read = 0,
	eNoteMessageState_Not_Read,
};

enum eNoteMessageBoxType
{
	eNoteMessageBoxType_Receive = 0,
	eNoteMessageBoxType_Send,
	eNoteMessageBoxType_Admin,
};

enum eResponseErrorType_ViewNoteMessage
{
	eResponseErrorType_ViewNoteMessage_Success = 0,
	eResponseErrorType_ViewNoteMessage_Fail,
	eResponseErrorType_ViewNoteMessage_Empty,
};

enum eResponseErrorType_NoteMessageSend
{
	eResponseErrorType_NoteMessageSend_Success = 0,
	eResponseErrorType_NoteMessageSend_Fail,
	eResponseErrorType_NoteMessageSend_NameLength,
	eResponseErrorType_NoteMessageSend_TitleLength,
	eResponseErrorType_NoteMessageSend_ConTentLength,
	eResponseErrorType_NoteMessageSend_NotChar,
	eResponseErrorType_NoteMessageSend_NotToday,
	eResponseErrorType_NoteMessageSend_MessageFull,
	eResponseErrorType_NoteMessageSend_MessageMy,
	eResponseErrorType_NoteMessageSend_TitleLengthMin,
};

enum eResponseErrorType_NoteMessageDel
{
	eResponseErrorType_NoteMessageDel_Success = 0,
	eResponseErrorType_NoteMessageDel_Fail,
};

enum eResponseErrorType_NoteMessageRead
{
	eResponseErrorType_NoteMessageRead_Success = 0,
	eResponseErrorType_NoteMessageRead_Fail,
};

enum eNoteMessageGmSend_Type
{
	eNoteMessageGmSend_Char = 0,
	eNoteMessageGmSend_all,
	eNoteMessageGmSend_Max,
};

enum eNoteMessageGmSend_Enable
{
	eNoteMessageGmSend_EnableDel = 0,
	eNoteMessageGmSend_EnableNotDel,
};

enum eGetGiftTimeErrorType
{
	eGetGiftTimeErrorType_Success = 0,
	eGetGiftTimeErrorType_InvenFull,
	eGetGiftTimeErrorType_OverRap,
	eGetGiftTimeErrorType_NotEvent,
};

enum eGetGiftTimeEnableType
{
	eGetGiftTimeEnableType_NotGet = 0,
	eGetGiftTimeEnableType_Get,
};

enum eBaseBallHitResult_Type
{
	eBaseBallHitResult_Type_Foul = 0,
	eBaseBallHitResult_Type_OneHit,
	eBaseBallHitResult_Type_TwoHit,
	eBaseBallHitResult_Type_ThreeHit,
	eBaseBallHitResult_Type_HomeRun,
	eBaseBallHitResult_Type_NotBat,
	eBaseBallHitResult_Type_NotBall,
	eBaseBallHitResult_Type_NotGoldBall,
	eBaseBallHitResult_Type_WaitTryAgain,
};

enum eGetBaseBallGiftResult_Type
{
	eGetBaseBallGiftResult_Type_Success = 0,
	eGetBaseBallGiftResult_Type_Fail,
	eGetBaseBallGiftResult_Type_Hack,
};

enum eGetBaseBallGiftListResult_Type
{
	eGetBaseBallGiftListResult_Type_Success = 0,
	eGetBaseBallGiftListResult_Type_Fail,
};

enum eBossUseSkill_Type
{
	eBossUseSkill_Type_Skill01 = 0,
	eBossUseSkill_Type_Skill02,
	eBossUseSkill_Type_Skill03,
};

enum eLotteryResult_Type
{
	eLotteryResult_Type_Fail = 0,
	eLotteryResult_Type_Grade1,
	eLotteryResult_Type_Grade2,
	eLotteryResult_Type_Grade3,
	eLotteryResult_Type_Grade4,
	eLotteryResult_Type_Grade5,
	eLotteryResult_Type_Grade6,
	eLotteryResult_Type_Grade7,
	eLotteryResult_Type_Grade8,
	eLotteryResult_Type_Grade9,
	eLotteryResult_Type_Grade10,
	eLotteryResult_Type_NoHaveItem,
};

enum eResponseErrorType_Snake_dice
{
	eResponseErrorType_Snake_dice_ok = 0,
	eResponseErrorType_Snake_dice_not_coupon,
};

#pragma pack(push, 1)

struct SetItemStatus
{
	tINT nType;
	tINT nValue_;
};

struct NoteMessage
{
	tINT index;
	tINT state;
	tCHAR char_name[NOTE_MESSAGE_NAME_MAX];
	tCHAR title[NOTE_MESSAGE_TITLE_MAX];
	int send_time;		// 보낸 편지함(보낸날짜) 받은편지함(받은날짜) 운영자편지함(받은날짜)
	int keep_end_time;
};

struct GetBaseBallGiftData
{
	tINT  rating;
	tINT  rating_count;
	tBOOL enable;
};

struct BaseBallInfo
{
	tINT  rating;
	tINT  rating_count;
};

struct SnakeDicePresent
{
	tINT success_count;
	tINT enable;
};

#pragma pack(pop)

///////////////////////////////////////

enum eCharStatusType
{
	eCharStatusType_Str = 0,
	eCharStatusType_Int,
	eCharStatusType_Dex,
	eCharStatusType_Con,
	eCharStatusType_Cha,

	eCharStatusType_Max,
};

enum eExchangeResponseType
{
	eExchangeResponseType_Accept = 0,
	eExchangeResponseType_Refuse,
	eExchangeResponseType_Disable,

	eExchangeResponseType_Max,
};

enum eCastleTowerControlType
{
	eCastleTowerControlType_Info = 0,
	eCastleTowerControlType_On,
	eCastleTowerControlType_Off,

	eCastleTowerControlType_Max,
};

enum ePetKeepType
{
	ePetKeepType_Open = 0,
	ePetKeepType_In,
	ePetKeepType_Out,

	ePetKeepType_Max,
};

enum ePetBagSubCmd
{
	ePetBagSubCmd_Put = 0,
	ePetBagSubCmd_Neck,

	ePetBagSubCmd_Max,
};

enum ePetMode
{
	ePetMode_None =	0,		//펫 없는상태
	ePetMode_Egg,			//펫 알상태
	ePetMode_Keep,			//일반 보관상태
	ePetMode_Hold,			//공성전으로 인한 보관상태
	ePetMode_Hatch = 10,	//부화모드
	ePetMode_Combat,		//전투모드
	ePetMode_Follow,		//동행모드
	ePetMode_Pickup,		//획득모드
	ePetMode_Wimpy,			//회복모드
	ePetMode_Supp,			//생산모드
	ePetMode_Long,			//계열해제모드
	ePetMode_Guard,			//경험치모드

	ePetMode_Max,
};

enum eWarZone
{
	eWarZone_StartPos = 0,		// 시작위치
	eWarZone_WarPos,

	eWarZone_Max,
};

enum eWarNotifyType
{
	eWarNotifyType_Agree = 0,	// 팀배틀 수락
	eWarNotifyType_Already,		// 이미 신청중
	eWarNotifyType_NotExsist,	// 상대팀 존재하지 않음
	eWarNotifyType_WarStart,	// 팀배틀 시작
	eWarNotifyType_WarEnd,		// 팀배틀 끝

	eWarNotifyType_Max,
};

enum eSupportAcceptType
{
	eSupportAcceptType_Accept = 0,	// 수락
	eSupportAcceptType_Deny,		// 거절
	eSupportAcceptType_AutoDeny,	// 거부

	eSupportAcceptType_Max,
};

enum eSupportType
{
	eSupportType_Supporter = 0,		// 후견인
	eSupportType_Apprentice,		// 견습생

	eSupportType_Max,
};

enum eSupportEnableType
{
	eSupportEnableType_LowLevel = 0,	// 레벨이 낮음
	eSupportEnableType_Giveup,			// 포기
	eSupportEnableType_Expired,			// 기간 만료
	eSupportEnableType_HighLevel,		// 레벨이 높음

	eSupportEnableType_Max,
};

enum eItemRingCombineType
{
	eItemRingCombineType_Combine = 0,
	eItemRingCombineType_Divide,
	eItemRingCombineType_Init,

	eItemRingCombineType_Max,
};

enum eItemNpcUpgradeCmd
{
	eItemNpcUpgradeCmd_Weapon = 0,
	eItemNpcUpgradeCmd_Armor,
	eItemNpcUpgradeCmd_Armor400,
	eItemNpcUpgradeCmd_ArmorCheck,
	eItemNpcUpgradeCmd_GodWeapon,
	eItemNpcUpgradeCmd_GodPower,
	eItemNpcUpgradeCmd_Conv,
	eItemNpcUpgradeCmd_Reroad,
	eItemNpcUpgradeCmd_Refine,
	eItemNpcUpgradeCmd_Enchant,

	eItemNpcUpgradeCmd_Max,
};

enum eItemNpcUpgradeResult
{
	eItemNpcUpgradeResult_Success = 0,
	eItemNpcUpgradeResult_Fail,
	eItemNpcUpgradeResult_Break,
	eItemNpcUpgradeResult_Error,
	eItemNpcUpgradeResult_Unsuccess,
	eItemNpcUpgradeResult_Check,

	eItemNpcUpgradeResult_Max,
};

enum eGuildSearchType
{
	eGuildSearchType_Guild = 0,		// 길드 이름
	eGuildSearchType_Master,		// 길마 이름

	eGuildSearchType_Max,
};

struct PreCharInfo
{
	tINT nSlotIndex;
	tINT nCharIndex;
	tCHAR szId[USER_ID_LENGTH];
	tINT nRace;
	tINT nSex;
	tINT nHair;
	tINT nLevel;
	tINT nVital;
	tINT nMaxVital;
	tINT nMana;
	tINT nMaxMana;
	tINT nStamina;
	tINT nMaxStamina;
	tINT nEpower;
	tINT nMaxEpower;
	tINT nStr;
	tINT nInt;
	tINT nDex;
	tINT nCon;
	tINT nCha;
	tINT nGuildIndex;
	tINT nWearing[WEARING_SHOW];
	tBOOL bIsGuildMaster;
	tBOOL bIsSupport;
};

struct SkillLevel
{
	tINT nSkillIndex;
	tINT nSkillLevel;
};

struct ItemInfo
{
	tINT nItemIndex;
	tINT nItemVnum;
	tINT nPlusPoint;
	tINT nSpecialFlag1;
	tINT nSpecialFlag2;
	tINT nUpgradeEndurance;
	tINT nUpgradeEnduranceMax;
	tINT nEndurance;
	tINT nEnduranceMax;
};

struct PartyInfo
{
	tINT nCharIndex;
	tCHAR strCharName[USER_ID_LENGTH];
};

struct GuildHuntInfo
{
	tCHAR strGuildName[GUILD_NAME_LENGTH];
	tCHAR strCharName[USER_ID_LENGTH];
	tBYTE btState;
};

struct ItemMultiInfo
{
	tINT nSlotX;
	tINT nSlotY;
};

struct SpShopItemInfo
{
	ItemInfo stInfo;
	tINT nLimitTime;
	tINT nLimitCount;
	tINT nItemPrice;
};

struct StashItemInfo
{
	tINT nStashSlotIndex;
	tINT nItemCount;
	ItemInfo stInfo;
};

struct StashItemMultiInfo
{
	tINT nSlotX;
	tINT nSlotY;
	tINT nItemIndex;
};

struct ItemWithIndexMultiInfo
{
	tINT nSlotX;
	tINT nSlotY;
	tINT nItemIndex;
};

struct SupportApprenticeInfo
{
	tINT nCharIndex;
	tCHAR strCharName[USER_ID_LENGTH];
	tINT nCharLevel;
	tINT nCharRace;
	tINT nCharSex;
	tINT nZone;			// 미접속 : -1
	tINT nStartTime;	// 관계 시작 시간
};

struct GuildPosInfo
{
	tINT nGuildGrade;
	tCHAR strCharName[USER_ID_LENGTH];
	tFLOAT fPosX;
	tFLOAT fPosZ;
};

struct GuildSearchInfo
{
	tCHAR strGuildName[GUILD_NAME_LENGTH];
	tCHAR strMasterName[USER_ID_LENGTH];
	tINT nGuildRank;
	tINT nGuildLevel;
	tINT nGuildPoint;
};

struct GuildStashItemInfo
{
	tINT nGuildStashSlotIndex;
	ItemInfo stInfo;
	tINT nItemCount;
};

struct WantedListInfo
{
	tCHAR strCharName[USER_ID_LENGTH];
	tINT nWantedLaim;
	tINT nCharRace;
	tINT nZone;
	tINT timeEndTime;
};

struct OccpTransInfo
{
	tINT nLocation;
	tINT nPrice;
};

struct OccpBattleInfo
{
	tINT nLocation;
	tCHAR nGuildName[GUILD_NAME_LENGTH];
	tINT nBattleStatus;
	tINT nStage;
	tINT nBuff1;
	tINT nBuff2;
};

struct OccpBattleCountInfo
{
	tINT nLocation;
	tBOOL bExtraInfo;		// 부가 정보 있음 여부
	tINT nStatus;
	tCHAR strCurGuildName[GUILD_NAME_LENGTH];
	tINT nCurGuildPoint;
	tCHAR strBestGuildName[GUILD_NAME_LENGTH];
	tINT nBestGuildPoint;
	tINT nRemainTime;		// 남은 시간(단위 초)
	tINT nElapsedTime;		// 점령 시간(단위 초)
};

struct EventGiftInfo
{
	tINT nItemIndex;
	tINT nItemVnum;
	tINT nItemCount;
	tCHAR strDate[DATE_STR_LENGTH];
};

struct FriendInfo
{
	tINT nCharIndex;
	tCHAR nCharName[USER_ID_LENGTH];
	tBOOL bLogin;
	tINT nChannel;
	tINT nZone;
};

struct BattleInfo
{
	tINT nGuildZone;
	tCHAR strGuildName[GUILD_NAME_LENGTH];
};

struct BattleFlagInfo
{
	tCHAR strGuildName[GUILD_NAME_LENGTH];
	tCHAR strCharName[USER_ID_LENGTH];
};

struct MultiAttackInfo
{
	tBYTE btCharType;
	tINT nCharIndex;
	tINT nVital;
};

struct EventAncientInfo
{
	tINT nInvenSlotX;
	tINT nInvenSlotY;
	tINT nItemIndex;
	tINT nItemVnum;
	tINT nSpecialFlag1;
	tINT nPlusPoint;
};

struct TeleportItemInfo
{
	tINT nZone;
	tFLOAT fPosX;
	tFLOAT fPosZ;
};

struct MovingSkillInfo
{
	tBYTE btTargetCharType;
	tINT nTargetCharIndex;
	tFLOAT fPosX;
	tFLOAT fPosZ;
};

struct BpShopListInfo
{
	tBYTE btShopType;
	tINT nNpcIndex;
};

#endif // _INFO_DEFINE_H_
