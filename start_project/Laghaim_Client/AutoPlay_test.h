#ifndef __AUTO_PLAY_TEST_20070808__
#define __AUTO_PLAY_TEST_20070808__

#include <string>
#include <vector>
#include <list>

enum _GAMEMODE
{
	GAMEMODE_STOP = -1,
	GAMEMODE_POTION_ONLY = 0,			// no attack support, only potion support.
	GAMEMODE_SKILL_POTION,					// potion and skill support.
	GAMEMODE_FULL_PACKAGE,				// full auto support. skill, attack, potion, pet system.
	GAMEMODE_AUTO

};

enum _ATTACKMODE
{
	ATTACKMOD_HOLD_AREA_ATTACK = 0,	// attack mode : hunting within a certain period of time. but if time over your limit time, character return start position.
	ATTACKMOD_MOVING_ATTACK,		// attack mode : hunting and find mob in your basis time and moving position. but it's not have basis area. so, character secede from your hunting area.
	ATTACKMOD_FIND_MOB,				// attack mode : attack and find mob in your basis area.
};

enum _QUICKSLOT_TYPE
{
	QTELEPORT = 0,
	QRETURN,
	QPETCALLER,
};

enum _GAMESTATUS
{
	NONE_STATE = -1,
	ATTACK = 0,					// attack											0
	ITEM,						// item pick										1
	RETURN_TO_CITY,				// return to city that used return item.			2
	MOVE_IN_PEACE,				// go to shop npc in peace area.					3
	SHOP_BUY,					// buy action in shop								4
	SHOP_SELL,					// sell action in shop								5
	REPAIR,						// repair action in blacksmith						6
	WARE,						// trade in warehouse								7
	FIELD_CHANGE,				// change field										8
	SET_ITEM_IN_QUICK,			// move item to quick slot from inventory.			9
	USE_QUICK,					// using quick slot									10
	GO_TO_AREA_ON_WALK,			// go to hunting area on foot.						11
	AVOID,						// avoid to city that find in mob list.				12
	REVIVAL,					// revival char.									13
	DELAY_TIME,					//													14
	RETURN_TO_BASE,				// return to start point.							15
	WAIT_IN_CITY,				// wait in city.									16
	PRE_ATTACK,					// 마을내 사용 또?
};

enum _PETMOD
{
	PET_ATTK = 0,				// attack mode
	PET_TOGETHER,				// going together
	PET_PICKUPITEM,					// item pickup mode
	PET_AVOID,					// avoid mode
	PET_SUPPORT,				// support mode
	PET_NOT_LONGRANGE,			//
	PET_GUARD,					// guard mode
	PET_MACRO					// macro mode
};

#define MAX_SKILL_COUNT			8

#define STAYLIMITDISTANCE		500
#define CHARACTER_RANGE			500
#define POTION_LIMIT_TIME		200		// potion limit time
#define WM_AUTOTESTKEY			30000
#define WM_AUTORELOAD			30100
#define WM_AUTOSTOPKEY			30200
#define SKILL_KEY_COUNT			8


#define AH_CFG_DIR				"./AHCfg/"

#define AH_CFG_FILE_USERINFO	"userInfo.dat"
#define AH_CFG_FILE_DEFAULT		"./AHCfg/default/AHcfg_default.ini"
#define AH_CFG_FILE_NAME		"/cfg.ini"				// user name + AH_CFG_FILE_NAME

#define AH_MODE					"MODE"
#define AH_MODE_TYPE			"TYPE"

#define AH_CFG_MAIN				"MAIN"
#define AH_CFG_MAIN_AUTO		"AUTOSTART"
#define AH_CFG_MAIN_ATTK		"ATTKSTART"
#define AH_CFG_MAIN_SKLL		"SKLLSTART"
#define AH_CFG_MAIN_ITEM		"ITEMSTART"
#define AH_CFG_MAIN_ATTKRANGE	"RANGE"
#define AH_CFG_MAIN_RACE		"RACE"

// 포션
#define AH_CFG_PTN				"POTION"
#define AH_CFG_PTN_USE			"USE"
#define AH_CFG_PTN_VITAL		"VITALSTART"
#define AH_CFG_PTN_MAGIC		"MAGICSTART"
#define AH_CFG_PTN_STAMI		"STAMISTART"
#define AH_CFG_PTN_ELECT		"ELECTSTART"
#define AH_CFG_PTN_SUB			"SUBSTART"
#define AH_CFG_PTN_ADIASUM		"ADIASUMSTART"
#define AH_CFG_PTN_SUBPOTION1	"SPPOTION1START"
#define AH_CFG_PTN_SUBPOTION2	"SPPOTION2START"
#define AH_CFG_PTN_VTLMT		"VITALLMT"
#define AH_CFG_PTN_MGLMT		"MAGICLMT"
#define AH_CFG_PTN_STLMT		"STAMILMT"
#define AH_CFG_PTN_ELLMT		"ELECLMT"
#define AH_CFG_PTN_SUBLMT		"SUBLMT"
#define AH_CFG_PTN_ADSUMLMT		"ADIASUMLMT"
#define AH_CFG_PTN_SPPOTION1LMT "SPPOTION1LMT"
#define AH_CFG_PTN_SPPOTION2LMT "SPPOTION2LMT"
#define	AH_CFG_PTN_VTKEY		"VITALKEY"
#define AH_CFG_PTN_MGKEY		"MAGICKEY"
#define AH_CFG_PTN_STKEY		"STAMIKEY"
#define AH_CFG_PTN_ELKEY		"ELECKEY"
#define AH_CFG_PTN_SUBKEY		"SUBKEY"
#define AH_CFG_PTN_ADKEY		"ADIASUMKEY"
#define AH_CFG_PTN_SPPOTION1KEY "SPPOTION1KEY"
#define AH_CFG_PTN_SPPOTION2KEY "SPPOTION2KEY"
#define AH_CFG_PTN_CALLKEY		"CALLKEY"
#define AH_CFG_PTN_CALLLIMIT	"CALLLMT"


// 스킬
#define AH_CFG_SKL				"SKILL"
#define AH_CFG_SKL_ROTATE		"ROTATE"
#define AH_CFG_SKL_USED			"USE"
#define AH_CFG_SKL_REC			"RECSTART"
#define AH_CFG_SKL_BLO			"BLODDYSTART"
#define AH_CFG_SKL_VMP			"VAMPSTART"
#define AH_CFG_SKL_SHL			"SHIELDSTART"
#define AH_CFG_SKL_NO			"SKILL%dSTART"
#define AH_CFG_SKL_RCLMT		"RECLMT"
#define AH_CFG_SKL_BLLMT		"BLOODYLMT"
#define AH_CFG_SKL_VMLMT		"VAMPLMT"
#define AH_CFG_SKL_DIR			"SKILL%dDIR"
#define AH_CFG_SKL_SHTIME		"SHIELDTIME"
#define AH_CFG_SKL_TIME			"SKILL%dTIME"
#define AH_CFG_SKL_RCKEY		"RECKEY"
#define AH_CFG_SKL_BLKEY		"BLOODYKEY"
#define AH_CFG_SKL_VMKEY		"VAMPKEY"
#define AH_CFG_SKL_SHKEY		"SHIELDKEY"
#define AH_CFG_SKL_KEY			"SKILL%dKEY"
#define AH_CFG_SKL_SHREPEAT		"SHIELDREPEAT"


#define AH_CFG_SKL_SHL2			"SHIELDSTART2"
#define AH_CFG_SKL_SHKEY2		"SHIELDKEY2"
#define AH_CFG_SKL_SHTIME2		"SHIELDTIME2"

#define AH_CFG_SKL_SHL3			"SHIELDSTART3"
#define AH_CFG_SKL_SHKEY3		"SHIELDKEY3"
#define AH_CFG_SKL_SHTIME3		"SHIELDTIME3"

#define AH_CFG_SKL_SHL4			"SHIELDSTART4"
#define AH_CFG_SKL_SHKEY4		"SHIELDKEY4"
#define AH_CFG_SKL_SHTIME4		"SHIELDTIME4"


#define AH_CFG_SKL_DSYS			"DSYSSTART"
#define AH_CFG_SKL_DSYSKEY		"DSYSKEY"


// 사냥
#define AH_CFG_HNT				"HUNT"
#define AH_CFG_HNT_MOVE			"MOVEATTACK"
#define AH_CFG_HNT_PARTY		"PARTYACCEPT"
#define AH_CFG_HNT_AVOID		"AVOIDSTART"
//#define AH_CFG_HNT_RETURN		"RETURNSTART"
#define AH_CFG_HNT_AVDLMT		"AVOIDLMT"


// 이동
#define AH_CFG_RTN				"RETURN"
#define AH_CFG_RTN_HRETURN		"HRETURN"	// ?E
#define AH_CFG_RTN_MRETURN		"MRETURN"	// 엠
#define AH_CFG_RTN_SRETURN		"SRETURN"	// 스템
#define AH_CFG_RTN_ERETURN		"ERETURN"	// 픸E
#define AH_CFG_RTN_ARETURN		"ARETURN"	// 보조
#define AH_CFG_RTN_URETURN		"URETURN"	// 소환
#define AH_CFG_RTN_VRETURN		"VRETURN"	// hp
#define AH_CFG_RTN_PRETURN		"PRETURN"	// ?E怜갋
#define AH_CFG_RTN_CRETURN		"CRETURN"	// 호출콅E
#define AH_CFG_RTN_HCOUNT		"HCOUNT"
#define AH_CFG_RTN_MCOUNT		"MCOUNT"
#define AH_CFG_RTN_SCOUNT		"SCOUNT"
#define AH_CFG_RTN_ECOUNT		"ECOUNT"
#define AH_CFG_RTN_ACOUNT		"ACOUNT"
#define AH_CFG_RTN_UCOUNT		"UCOUNT"
#define AH_CFG_RTN_VCOUNT		"VCOUNT"
#define AH_CFG_RTN_RETURNKEY	"RETURNKEY"
#define AH_CFG_RTN_TRANSE_USE	"USE"
#define AH_CFG_RTN_TRANSE_POS	"POS"
#define AH_CFG_RTN_TRANSE_KEY	"KEY"
#define AH_CFG_RTN_TRANSE_WALK	"WALK"



// 구매
#define AH_CFG_BUY				"BUY"
#define AH_CFG_BUY_USE			"USE"
#define AH_CFG_BUY_COUNT		"COUNT%d"
#define AH_CFG_BUY_CHECK		"CHECK%d"
#define AH_CFG_BUY_KEEP			"KEEP%d"
#define AH_CFG_BUY_REPAIR		"REPAIR"
#define AH_CFG_BUY_WARE			"WAREHOUSE"
#define AH_CFG_BUY_BUYAPPLE		"BUYAPPLE"
#define AH_CFG_BUY_APPLECNT		"BUYAPPLECOUNT"
#define AH_CFG_BUY_CALL			"BUYCALL"
#define AH_CFG_BUY_CALLCNT		"BUYCALLCOUNT"



// ?E
#define AH_CFG_PET				"PET"
#define AH_CFG_PET_ACTIVE		"ACTIVE"
#define AH_CFG_PET_TOGETHER		"WITH"
#define AH_CFG_PET_HUNTMOD		"HUNTMOD"
#define AH_CFG_PET_AVOID		"AVOID"
#define AH_CFG_PET_AVOIDPER		"AVOIDPERCENT"
#define AH_CFG_PET_WALK			"WALK"
#define AH_CFG_PET_WALKMOD		"WALKMOD"



// [1/5/2009 D.K ] : used enum definition.
//===============================================================
// 0 : 불칸
// 1 : 카이립흟E
// 2 : 에이디아
// 3 : 휴먼
// 4 : 하이틒E?갋
//===============================================================
enum _CHAR_TYPE
{
	BULKAN = 0,
	KAILIPTION,
	AIDIA,
	HUMAN,
	HYBRIDER
};

#define CALC_BIT( src, num )			( src << num )

//////////////////////////////////////////////////////////////////////////
// return options.
#define AH_RETURN_HEALTH		0
#define AH_RETURN_MANA			1
#define AH_RETURN_STAMINA		2
#define AH_RETURN_EPOWER		3
#define AH_RETURN_SUBARMS		4
#define AH_RETURN_SUMMONS		5
#define AH_RETURN_HP			6

enum State_ReturnValue
{
	State_RV__None,
	State_RV__Wait,
	State_RV__Finish,
	State_RV__EndOfEnum
};

enum StashState
{
	StashState__Pop,
	StashState__Push,
	StashState__EndOfEnum
};

enum ShopState
{
	ShopState__Repair,
	ShopState__Buy,
	ShopState__MoveToQuick,

	ShopState__EndOfEnum
};


enum LagRush_Interrupt
{
	LagRush_Interrupt__None = -1,
	LagRush_Interrupt__Error_Setting = 0,
	LagRush_Interrupt__WalkPos_Empty ,
	LagRush_Interrupt__Machine_Error,
	LagRush_Interrupt__NPC_NotFound,
	LagRush_Interrupt__Slot_NotMatch,
	LagRush_Interrupt__Inven_Full,
	LagRush_Interrupt__BuyItemFail,
	LagRush_Interrupt__Disconnect_Server,
	LagRush_Interrupt__Limited_Dungeon,
	LagRush_Interrupt__NotSetting,
	LagRush_Interrupt__Version_Low,
	LagRush_Interrupt__Tribe_NotMatch,
	LagRush_Interrupt__Error_Stash_Put,
	LagRush_Interrupt__Error_Stash_Get,
	LagRush_Interrupt__Error_Shop_Buy,
	LagRush_Interrupt__Error_NoMsg,
	LagRush_Interrupt__Wrong_Zone ,
	LagRush_Interrupt__Wrong_OpenStash,
	LagRush_Interrupt__Wrong_Stash,
	LagRush_Interrupt__NonExist_Item,
	LagRush_Interrupt__NoValid_Inven,
	LagRush_Interrupt__NoValid_Stash,
	LagRush_Interrupt__Fail_FindItem,
	LagRush_Interrupt__OptionTime,
	LagRush_Interrupt__NoSame_Item,
	LagRush_Interrupt__Fail_Input,
	LagRush_Interrupt__InvenFull,
	LagRush_Interrupt__NoMoney,
	LagRush_Interrupt__NoBuyItem,
	LagRush_Interrupt__CantZone,

	LagRush_Interrupt__EndOfEnum,
};


enum LR_Warning_Type
{
	LR_Warning__None,
	LR_Warning__InRange,
	LR_Warning__OutRange,

	LR_Warning__EndOfEnum
};


#define AH_RETURN_APPLE			7
#define AH_RETURN_CALLER		8
#define AH_RETURN_MAX			9



#define AH_SPECIALPOTION		6
#define AH_QUICKSLOT_COUNT		12
#define AH_CFG_PTN_SUBPOTION3	"SPPOTION3START"
#define AH_CFG_PTN_SUBPOTION4	"SPPOTION4START"
#define AH_CFG_PTN_SUBPOTION5	"SPPOTION5START"
#define AH_CFG_PTN_SUBPOTION6	"SPPOTION6START"
#define AH_CFG_PTN_SPPOTION3LMT "SPPOTION3LMT"
#define AH_CFG_PTN_SPPOTION4LMT "SPPOTION4LMT"
#define AH_CFG_PTN_SPPOTION5LMT "SPPOTION5LMT"
#define AH_CFG_PTN_SPPOTION6LMT "SPPOTION6LMT"
#define AH_CFG_PTN_SPPOTION3KEY "SPPOTION3KEY"
#define AH_CFG_PTN_SPPOTION4KEY "SPPOTION4KEY"
#define AH_CFG_PTN_SPPOTION5KEY "SPPOTION5KEY"
#define AH_CFG_PTN_SPPOTION6KEY "SPPOTION6KEY"


typedef std::pair<float, float>			POSPAIR;
typedef std::pair<std::string, u_short>	STRPAIR;
typedef std::list<int>					MOBLIST;
typedef MOBLIST::iterator				MOBLIST_i;



class CItemInfo;
class CNkMob;
class CItem;
class CNkMyPet;

enum AutoPlay_StopProc
{
	AutoPlay_StopProc__Laim_Not,
	AutoPlay_StopProc__TimeOver,
	AutoPlay_StopProc__NpcNotFound,
	AutoPlay_StopProc__DontTrans,
	AutoPlay_StopProc__FullInven,
	AutoPlay_StopProc__NotWalkPos,


	AutoPlay_StopProc__EndOfEnum
};

typedef struct _TEMP_QUICK
{
	int quick_slot_vnum;
	DWORD tick;

} TEMP_QUICK;

class LH_Decal;
class CAutoPlay
{
public:
	bool		m_loaded_setting;
	bool		m_initialized;
	DWORD		m_dwGameMode;
	DWORD		m_dwGameAttkMode;
	DWORD		m_dwGameStatus;
	DWORD		m_dwLastestTime;
	DWORD		m_dwChangeLimitTime;
	CNkMob*		m_pSelectedMob;
	CNkMob*		m_pTemporaryMob;
	CItem*		m_pTemporaryItem;

	std::string m_szWaringMobList;			// 선 타격 녀석. 주의해야할 녀석을 저장함.
	std::string m_szEscapeList;				// 도망갈 리스트. 예를 들푳E필탛E보스.
	std::string m_szItemList;				// 먹을 아이템 리스트.
	std::string m_szAvoidUserList;			// User.
	std::string m_szAvoidMobList;			// Mob.
	std::vector<int>				m_vecPickItemList;
	std::vector<int>				m_vecFirstTarget;
	std::vector<int>				m_vecAvoidMob;
	std::vector<STRPAIR>			m_vecAvoidChar;
	std::vector<POSPAIR> m_pairPoslist;
	std::vector<POSPAIR> m_pairPoslist_temp;

	WORD		m_wWalkPosCount;
	WORD		m_wTempCount;
	DWORD		m_dwTemporaryTime;
	CHAR		m_szFileName[128];

	unsigned int m_uActive;
	bool m_bAuto;
	bool m_bAttk;
	bool m_bSpecialMob;
	bool m_bSkll;
	bool m_bItem;
	bool m_bUsePotion;						// [4/9/2009 D.K ] : potion 추가.
	bool m_bMove;
	bool m_bFirst;
	bool m_bAdiaSummon;
	bool m_bShield;
	bool m_bshield2;
	bool m_bshield3;
	bool m_bshield4;

	bool m_bRecovery;
	bool m_bBloodyMana;
	bool m_bVempTouch;

	bool m_bRotateSkill;

	bool m_bLeftSkill;
	DWORD m_dwLeftSkill;

	bool m_bSkillArray[8];					// 스킬 사퓖E?
	bool m_bSubArms;						// 보조무콅E충픸E?
	bool m_bSpecialPotion[AH_SPECIALPOTION];				// 특수물푳E
	bool m_bWalkToArea;
	bool m_bItemRepair;
	bool m_bWareHouse;

	bool m_bHealth;
	bool m_bMana;
	bool m_bStamina;
	bool m_bEPower;

	bool	m_hunt_move;		// 이동사냥?
	bool	m_party_confirm;	// 파티수락

	bool	m_use_dsys;
	DWORD	m_dsys_key;
	DWORD	m_perom_global_cool;

	bool m_bAvoid;
	WORD m_wReturn;
	WORD m_wBuyFromShop;
	WORD m_wCurSKill;

	BYTE m_bSkillDirArray[8];				// 스킬 방향값. 0이툈Eleft, 1이툈Eright, 2툈E키보탛E

	DWORD m_wHealthLimitTime;				// 물약을 계속 먹는걸 방지하콅E위해서. 제한 타임에 큱E?
	DWORD m_wManaLimitTime;					// 정보를 저장한다.
	DWORD m_wStaminaLimitTime;
	DWORD m_wEPowerLimitTime;
	DWORD m_wSkillLimitTime[8];					// 스킬 제한 타임.
	DWORD m_wSpecialPotionLimitTime[AH_SPECIALPOTION];			//
	DWORD m_wAvoidLimitTime;

	DWORD m_dwHealthCurTime;				// 카웝獸 된 타임.
	DWORD m_dwManaCurTime;
	DWORD m_dwStaminaCurTime;
	DWORD m_dwEPowerCurTime;
	
	DWORD m_dwShieldTime;					// shield Time
	DWORD m_dwShieldTime2;					// shield Time
	DWORD m_dwShieldTime3;					// shield Time
	DWORD m_dwShieldTime4;					// shield Time

	DWORD m_dwSkillCurTime[8];				// 마지막 스킬 사퓖E시간.
	DWORD m_bSkillUsed[8];
	DWORD m_dwSpecialPotionCurTime[AH_SPECIALPOTION];
	DWORD m_dwAvoidCurTime;

	DWORD m_dwReturnCount[AH_RETURN_MAX];	// if input data is smaller than this variables, activate return options.
	// [0] : Health potions
	// [1] : Mana potions
	// [2] : Stamina potions
	// [3] : EPower potions
	// [4] : Sub Arms potions
	// [5] : Summon stones
	DWORD m_dwBuyFromShopCount[AH_QUICKSLOT_COUNT];
	DWORD m_dwKeepCount[AH_QUICKSLOT_COUNT];					// 퀵슬롯 아이템 맡기기에 사용됨
	
	DWORD m_dwHealthKey;					// 사퓖E?위치값. 원래는 키 값을 저장하여야 하나.
	DWORD m_dwManaKey;						// 여기서는 위치값(훮E슘?을 저장한다.
	DWORD m_dwStaminaKey;
	DWORD m_dwEPowerKey;
	DWORD m_dwAdiaSummonKey;
	DWORD m_dwShieldKey;					// SHield Key.
	DWORD m_dwShieldKey2;					// 필드 ex
	DWORD m_dwShieldKey3;					// 인게이지
	DWORD m_dwShieldKey4;					// 크리티컬

	DWORD m_dwRecoveryKey;					// Recovery Key.
	DWORD m_dwBloodyManaKey;				// BloodyMana Key.
	DWORD m_dwVempTouchKey;					// Vempireric touch.
	DWORD m_dwSkillKeyArray[8];				// skill key array.
	DWORD m_dwSubArmsKey;					// sub arms key.
	DWORD m_dwSpecialPotionKey[AH_SPECIALPOTION];
	DWORD m_dwReturnKey;					// return option key.
	DWORD m_dwTransportingKey;

	DWORD m_dwShieldRepeat;					// Repeat Numbers.
	DWORD m_dwSubArmsLimit;					// 내구도.
	DWORD m_dwTransportingPos;

	float m_fAttkRange;						// 공격 거리
	float m_fStayRange;						// 최큱E움직일 펯E있는 거리.
	float m_fMoveRange;						// 사퓖E舊갋않음.
	float m_fItemRange;						// 아이템 줍는 거리.

	float m_fStayPosX;						// 시작할때의 좌표값.
	float m_fStayPosZ;						// 이는 나중에 최큱E움직이는 거리에 큱E?측정을 한다.


	float m_fHealthPercent;			// 포션 사퓖E첼?큱E?퍼센트 지정.
	float m_fManaPercent;
	float m_fStaminaPercent;
	float m_fEPowerPercent;

	float m_fAdiaSummonPercent;
	float m_fRecoveryPercent;				// recovery.
	float m_fBloodyManaPercent;				// Bloody mana.
	float m_fVempTouchPercent;				// vempireric touch.
	float m_fSkillPercent[8];

	//////////////////////////////////////////////////////////////////////////
	// pet
	bool	m_bPetActive;						// false : no activate this class || true : activate this class
	bool	m_bHuntWithMaster;					// false : without master || true : with master
	bool	m_bBuyApple;						// false : no buy apples || true : buy apples
	UINT	m_uApplesCount;						// count of apples
	bool	m_bAppleEmpty;						// if true, go to peace area.
	bool	m_bUseApplebag;						// if true, use pet food bag.
	bool	m_bUsedCallSign;					// if true, use pet call system.
	bool	m_bBuyCallSign;
	bool	m_bCallerEmpty;						// if true, go to peace area and buy pet caller.
	UINT	m_uCallersCount;					// count of caller.
	UINT	m_uCallKeyMapping;					// key mapping
	BYTE	m_yHuntingType;						// select hunting type.
	bool	m_bUsedAvoid;						// if pet hp is smaller than limit basis, pet mode change to avoid mode.
	bool	m_bUseTrans;						// [4/9/2009 D.K ] : translate 추가.
	bool	m_bReturn;							// [4/9/2009 D.K ] : return 추가.
	bool	m_bUseBuy;							// [4/9/2009 D.K ] : 상점 추가.
	UINT	m_uPetAvoidVital;					// percent of pet hp
	bool	m_bUsedPetWalkMode;
	BYTE	m_yWalkMode;
	bool	m_bOnPickItem;						// if on pick item.

	//////////////////////////////////////////////////////////////////////////

	CItemInfo*			m_pTargetItem;		// 큱E갋아이템 정보.
	//////////////////////////////////////////////////////////////////////////
	FILE* fp;

	bool m_bUseSkill;



	virtual void SetLostQuickSlot()						{}
	virtual int IsLostQuickSlot(DWORD dt)
	{
		return 1;
	}
	virtual bool FillLostQuickSlot(int slot , int vnum)
	{
		return true;
	}
	virtual void SetSendMove(int slot)					{}


public:
	virtual void	Init(bool playing=true);

	bool	IsAuto ()
	{
		return m_bAuto;
	}
	bool	IsAttk ()
	{
		return m_bAttk;
	}
	bool	IsSkll ()
	{
		return m_bSkll;
	}
	bool	IsItem ()
	{
		return m_bItem;
	}
	bool	IsMove ()
	{
		return m_bMove;
	}
	bool	IsFirst()
	{
		return m_bFirst;
	}
	bool	IsAdiaSummon()
	{
		return m_bAdiaSummon;
	}
	bool	IsShield	()
	{
		return m_bShield;
	}
	bool	IsRecovery	()
	{
		return m_bRecovery;
	}
	bool	IsBloodyMana()
	{
		return m_bBloodyMana;
	}
	bool	IsVempTouch ()
	{
		return m_bVempTouch;
	}
	bool	IsSkill		( int _pos )
	{
		return m_bSkillArray[_pos];
	}
	bool	IsSubArms	()
	{
		return m_bSubArms;
	}
	bool	IsAvoid		()
	{
		return m_bAvoid;
	}
	WORD	IsReturn	( WORD _type/*= AH_RETURN_HEALTH*/ )
	{
		return m_wReturn & CALC_BIT( 1, _type);
	}

	bool	IsHealth ()
	{
		return m_bHealth;
	}
	bool	IsMana   ()
	{
		return m_bMana;
	}
	bool	IsStamina()
	{
		return m_bStamina;
	}
	bool	IsEPower ()
	{
		return m_bEPower;
	}
	bool	IsSpPotion( int _type = 0/*기본키*/ )
	{
		return m_bSpecialPotion[_type];
	}
	bool	IsPetActive()
	{
		return m_bPetActive;
	}

	void	SetAuto	(bool bAuto)
	{
		m_bAuto = bAuto;
	}
	void	SetAttk	(bool bAttk)
	{
		m_bAttk = bAttk;
	}
	void	SetSkll (bool bSkll)
	{
		m_bSkll = bSkll;
	}
	void	SetItem	(bool bItem)
	{
		m_bItem = bItem;
	}
	void	SetMove	(bool bMove)
	{
		m_bMove = bMove;
	}
	void	SetFirst(bool bfirst)
	{
		m_bFirst = bfirst;
	}
	virtual void	SetPos	(float x, float z)
	{
		m_fStayPosX = x;
		m_fStayPosZ = z;
	}
	void	SetAdiaSummon	(bool bAdia )
	{
		m_bAdiaSummon = bAdia;
	}
	void	SetShield		( bool _bShield )
	{
		m_bShield = _bShield;
	}
	void	SetRecovery		( bool _bRecovery )
	{
		m_bRecovery = _bRecovery;
	}
	void	SetBloodyMana	( bool _bBloody )
	{
		m_bBloodyMana = _bBloody;
	}
	void	SetVempTouch	( bool _bVemp )
	{
		m_bVempTouch = _bVemp;
	}
	void	SetSkill		( bool _bSkill, int _pos )
	{
		m_bSkillArray[_pos] = _bSkill;
	}
	void	SetSubArms		( bool _bSub )
	{
		m_bSubArms = _bSub;
	}
	void	SetAvoid		( bool _bAvoid )
	{
		m_bAvoid = _bAvoid;
	}
	void	SetReturn		( WORD _type )
	{
		m_wReturn |= CALC_BIT(1, _type);
	}

	void	SetHealth ( bool _bHealth  )
	{
		m_bHealth		= _bHealth;
	}
	void	SetMana   ( bool _bMana    )
	{
		m_bMana		= _bMana;
	}
	void	SetStamina( bool _bStamina )
	{
		m_bStamina	= _bStamina;
	}
	void	SetEPower ( bool _bEPower  )
	{
		m_bEPower		= _bEPower;
	}
	void	SetSpPotion( bool _Use, int _type = 0/*기본키*/ )
	{
		m_bSpecialPotion[_type] = _Use;
	}
	void	SetPetActive( bool _bActive )
	{
		m_bPetActive	= _bActive;
	}

	//////////////////////////////////////////////////////////////////////////
	// String
	void	AddWarningList( char* _szWarning )
	{
		m_szWaringMobList += _szWarning;
	}
	void	AddItemList( char* _szItem )
	{
		m_szItemList += _szItem;
	}
	void	LoadWalkPosList();
	void	LoadWalkPosListTemp();
	void	SaveWalkPosList( float x, float z );
	void	SaveWalkPosList();
	void	LoadWarningList();
	void	LoadItemList();
	void	LoadItemListImpl(char* filename);
	virtual void LoadSetting ();
	void	LoadAvoidUserList();
	void	LoadAvoidMobList();
	virtual void ReLoadData()
	{
		LoadSetting();
		LoadItemList();
		LoadAvoidMobList();
		LoadAvoidUserList();
		LoadWarningList();
		LoadWalkPosList();
	}

	//////////////////////////////////////////////////////////////////////////
	// Toggle Functions.
	void	ToggleAuto ()
	{
		SetAuto(!IsAuto());
	}
	void	ToggleAttk ()
	{
		SetAttk(!IsAttk());
	}
	void	ToggleSkll ()
	{
		SetSkll(!IsSkll());
	}
	void	ToggleItem ()
	{
		SetItem(!IsItem());
	}
	void	ToggleMove ()
	{
		SetMove(!IsMove());
	}
	void	ToggleFirst()
	{
		SetFirst(!IsFirst());
	}
	void	ToggleAdiaSummon()
	{
		SetAdiaSummon(!IsAdiaSummon());
	}
	void	ToggleShield    ()
	{
		SetShield(!IsShield());
	}
	void	ToggleRecovery	()
	{
		SetRecovery(!IsRecovery());
	}
	void	ToggleBloodyMana()
	{
		SetBloodyMana(!IsBloodyMana());
	}
	void	ToggleVempTouch ()
	{
		SetVempTouch(!IsVempTouch());
	}
	void	ToggleSkill		( int _pos )
	{
		SetSkill(!IsSkill( _pos ), _pos );
	}
	void	ToggleSubArms	()
	{
		SetSubArms(!IsSubArms());
	}
	void	ToggleAvoid		()
	{
		SetAvoid(!IsAvoid());
	}
	void	ToggleReturn	( WORD _type )
	{
		SetReturn(IsReturn(_type));
	}

	void	ToggleHealth ()
	{
		SetHealth( !IsHealth() );
	}
	void	ToggleMana   ()
	{
		SetMana( !IsMana() );
	}
	void	ToggleStamina()
	{
		SetStamina( !IsStamina() );
	}
	void	ToggleEPower ()
	{
		SetEPower( !IsEPower() );
	}
	void	ToggleSpPotion( int _type = 0 )
	{
		SetSpPotion( !IsSpPotion(_type), _type );
	}
	void	TogglePetActive()
	{
		SetPetActive(!IsPetActive());
	}

	//////////////////////////////////////////////////////////////////////////
	// Setting Data.

	void	SetHealthPercent ( float _percent = 50.0f )
	{
		m_fHealthPercent = _percent;	   // default : 50%
	}
	void	SetManaPercent   ( float _percent = 50.0f )
	{
		m_fManaPercent = _percent;
	}
	void	SetStaminaPercent( float _percent = 50.0f )
	{
		m_fStaminaPercent = _percent;
	}
	void	SetEPowerPercent ( float _percent = 50.0f )
	{
		m_fEPowerPercent = _percent;
	}
	void	SetAdiaSummonPercent( float _percent = 50.0f )
	{
		m_fAdiaSummonPercent = _percent;
	}
	void	SetRecoveryPercent( float _percent = 50.0f )
	{
		m_fRecoveryPercent = _percent;
	}
	void	SetBloodyPercent ( float _percent = 50.0f )
	{
		m_fBloodyManaPercent = _percent;
	}
	void	SetVempPecent	 ( float _percent = 50.0f )
	{
		m_fVempTouchPercent = _percent;
	}
	void	SetSkillPercent	 ( float _percent = 50.0f, int _pos = 0 )
	{
		m_fSkillPercent[_pos] = _percent;
	}
	void	SetReturnCount	 ( WORD _type, DWORD _count )
	{
		m_dwReturnCount[_type] = _count;
	}

	void	SetHealthKey	 ( DWORD _key = 0 )
	{
		m_dwHealthKey = _key;			   // default key is 0
	}
	void	SetManaKey		 ( DWORD _key = 1 )
	{
		m_dwManaKey = _key;
	}
	void	SetStaminaKey	 ( DWORD _key = 2 )
	{
		m_dwStaminaKey = _key;
	}
	void	SetEPowerKey	 ( DWORD _key = 3 )
	{
		m_dwEPowerKey = _key;
	}
	void	SetAdiaSummonKey ( DWORD _key = 5 )
	{
		m_dwAdiaSummonKey = _key;
	}
	void	SetShieldKey	 ( DWORD _key = 0/*VK_F1*/ )
	{
		m_dwShieldKey	= _key;
	}
	void	SetRecoveryKey	 ( DWORD _key = 0/*VK_F1*/ )
	{
		m_dwRecoveryKey = _key;
	}
	void	SetBloodyKey	 ( DWORD _key = 0/*VK_F1*/ )
	{
		m_dwBloodyManaKey = _key;
	}
	void	SetVempKey		 ( DWORD _key = 0/*VK_F1*/ )
	{
		m_dwVempTouchKey = _key;
	}
	void	SetSkillKey		 ( DWORD _key = 0/*VK_F1*/, int _pos = 0 )
	{
		m_dwSkillKeyArray[_pos] = _key;
	}
	void	SetSubArmsKey	 ( DWORD _key = 5 )
	{
		m_dwSubArmsKey = _key;
	}
	void	SetSpPotionKey	 ( DWORD _key = 2, int _type = 0 )
	{
		m_dwSpecialPotionKey[_type] = _key;
	}
	void	SetReturnKey	 ( DWORD _key = 5 )
	{
		m_dwReturnKey = _key;
	}

	void	SetShieldRepeat	 ( DWORD _Repeat = 1 )
	{
		m_dwShieldRepeat = _Repeat;
	}
	void	SetShieldTime	 ( DWORD _Time = 1 /*second*/ )
	{
		m_dwShieldTime = _Time;
	}
	void	SetSubArmsDurance( DWORD _Endurance )
	{
		m_dwSubArmsLimit = _Endurance;
	}
	void	SetAvoidTime	 ( DWORD _Time = 1 )
	{
		m_wAvoidLimitTime = _Time;
	}
	//////////////////////////////////////////////////////////////////////////
	// Pet
	void	ChangeMode		 ( DWORD _Mode = 11 );
	//////////////////////////////////////////////////////////////////////////
	// Getting Data.
	float	GetHealthPercent ()
	{
		return m_fHealthPercent;
	}
	float	GetManaPercent   ()
	{
		return m_fManaPercent;
	}
	float	GetStaminaPercent()
	{
		return m_fStaminaPercent;
	}
	float	GetEPowerPercent ()
	{
		return m_fEPowerPercent;
	}
	float	GetAdiaSummonPercent()
	{
		return m_fAdiaSummonPercent;
	}
	float	GetRecoveryPercent()
	{
		return m_fRecoveryPercent;
	}
	float	GetBloodyPercent ()
	{
		return m_fBloodyManaPercent;
	}
	float	GetVempPercent	 ()
	{
		return m_fVempTouchPercent;
	}
	float	GetSkillPercent	 ( int _pos )
	{
		return m_fSkillPercent[_pos];
	}

	DWORD	GetHealthKey	()
	{
		return m_dwHealthKey;
	}
	DWORD	GetManaKey		()
	{
		return m_dwManaKey;
	}
	DWORD	GetStaminaKey	()
	{
		return m_dwStaminaKey;
	}
	DWORD	GetEPowerKey	()
	{
		return m_dwEPowerKey;
	}
	DWORD	GetAdiaSummonKey()
	{
		return m_dwAdiaSummonKey;
	}
	DWORD	GetShieldKey	()
	{
		return m_dwShieldKey;
	}
	DWORD	GetRecoveryKey	()
	{
		return m_dwRecoveryKey;
	}
	DWORD	GetBloodyKey	()
	{
		return m_dwBloodyManaKey;
	}
	DWORD	GetVempKey		()
	{
		return m_dwVempTouchKey;
	}
	DWORD	GetSkillKey		( int _pos )
	{
		return m_dwSkillKeyArray[_pos];
	}
	DWORD	GetSubArmsKey	()
	{
		return m_dwSubArmsLimit;
	}

	DWORD	GetCurShopNo	( int _CurWorld );
	void	GetCurCenterPos ( int _CurWorld, float& x, float& z );

	bool	OnPotion();
	bool	OnSubSkll();
	bool	OnAttk();
	bool	OnAvoid();
	bool	OnSkll( int _x, int _y );
	bool	OnItem();
	bool	OnMove( bool _mode = false );
	virtual bool	_Move( float _x, float _z );
	CNkMob*		_selectTarget( bool& _isSpecial, INT npcCode = -1 );
	bool	OnMoveOnWalk();
	bool	OnAdiaSummon();
	bool	OnShield();
	bool	OnRecovery();
	bool	OnBloody();
	bool	OnVempTouch();
	bool	OnSubArms();
	bool	OnReturn( UINT status = WAIT_IN_CITY );
	bool	OnNPCShop();
	bool	SetQuickItem( UINT uType = QTELEPORT );
	bool	OnTransItem();
	bool	OnMoveInPeace();
	bool	OnPet();
	bool	OnWait();
	virtual void StartProc(DWORD dwGameMode);
	virtual void	StopProc(int type=AutoPlay_StopProc__EndOfEnum);
	//////////////////////////////////////////////////////////////////////////
	void	ShowInfo();
	virtual bool	MsgProc( UINT uMsg, WPARAM wParam, LPARAM lParam );

public:
	bool OnNPCStash();
	virtual void AutoProc();
	void FullProc();
	void PotionProc();
	void PotionSkillProc();
	virtual void Render(bool activate=false, bool edit_mode=false);
	bool SetUserName();
	char* changeSpecialWord( char* szString );
	CAutoPlay();
	virtual ~CAutoPlay();
	//////////////////////////////////////////////////////////////////////////
public:
	float m_fEscapeX;
	float m_fEscapeZ;
	DWORD m_dwEscapeTime;
	DWORD m_dwSubSkillTime;
	DWORD m_lastMoveTime;
	CNkMob*		m_pSpecialMob;
	CNkMob*		m_pCurrentMob;
	void	OnEscapeBlocked();


public:
	bool m_bUsed_Trans;
	bool IsFullInvenInAllPack();

	virtual int GetCurrentState()
	{
		return m_dwGameStatus;
	}
	virtual bool IsRun()
	{
		return true;
	}
	virtual bool _OnInterrupt(LagRush_Interrupt interrupted)
	{
		return false;
	}


public:
	virtual bool IsAutoParty()
	{
		return m_party_confirm;
	}

	virtual void SetDead() {}
	virtual void SetRivival() {}
	virtual void OnRecvShop() {}
	virtual void OnRecvStash() {}
	virtual void OnRecvApple() {}
	virtual void OnDontPick() {}
	virtual void SetSurport(int source, int target) {}
	virtual bool _OnDecideTarget_Surport(DWORD dt)
	{
		return false;
	}
	virtual void KillMob(int mob_index) {}

	virtual void AddLog(char * log , int phase_state=1) {}
	virtual void LogStart() {}
	virtual void LogEnd(bool force=false) {}

	virtual void Init_variable();
	virtual void OutOfRange_Mob() {}

	void CheckMode();
	void StartLagrushMsg();
	void EndLagrushMsg();
};



class CAutoPlay_EX : public CAutoPlay
{
public:
	LH_Decal* m_pDecal;

public:
	bool	m_running;				// 라그러쉬 실행가능유무

	DWORD	m_olddt;				//

	bool	m_change_state;			// 모드가 변경되었다. 일정시간 딜레이
	int		m_prev_state;			// 이픸E모탛E
	int		m_current_state;		// 현픸E모탛E

	bool	m_use_return;			// 귀환장치 사퓖E??
	DWORD	m_changed_delay;		// 모드변경싯 변경시간

	DWORD 	m_pick_item_time;		// 아이템습득모탛E: 시간처리-오버시 시작점으로 이동
	DWORD 	m_last_quick_tick;		// 훮E슘犢玲갋: 0.5초후 초기화
	DWORD 	m_last_action_tick;		// 공격외 마지막 사퓖E시간
	DWORD 	m_last_attack_tick;		// 마지막 공격 성공시간 : 일정시간후 상태 변컖E
	DWORD	m_last_callsign_tick;	// 일단 최소 10초로 잡자!!


	DWORD	m_dead_time;


	bool	m_found_npc;			// 라그러쉬 상인 검퍊E
	int		m_npc_index;			// 라그러쉬 상인 인덱스
	float 	m_buy_x;				// 라그러쉬 상인 위치
	float 	m_buy_z;				// 라그러쉬 상인 위치

	StashState m_stash_state;		//
	ShopState m_shop_state;			// 물품구매 상태

	int		m_repair_count;			// 내구도 수리( 위치 )

	bool	m_picked_apple;			// 커서에 사컖E붙임

	DWORD	m_using_shield_count;	// 실탛E사퓖E회펯E
	DWORD	m_using_shield_time;	// 실탛E사퓖E시간


	DWORD m_using_shield_time2;
	DWORD m_using_shield_time3;
	DWORD m_using_shield_time4;



	int		m_skill_slot;			// 현재사퓖E?스킬 인덱스
	DWORD	m_last_decide_target;	// 마지막 타겟 지정 시간
	DWORD	m_move_to_base_tick;	// 시작위치로 이동한 시작 시간

	CNkMob * m_pPrevSelectedMob;

	bool	m_success_movetobase;
	int		m_move_to_base_dir;		// 8방?E

	bool	m_global_attack_time;	// 공격못하컖E지속적으로다가 아무 행동도 못하컖E있을때...
	bool	m_recived_stash_item;

	MOBLIST m_exception_target;		// 타겟지정후 공격못하는퀋E목록

	int		m_attack_try_count;		// 가쾪E공격을 못할때가 있다.
	int		m_movetobase_try_count; //

	int		m_shop_sendmsg;			// 메세지를 받았으면 false
	int		m_stash_sendmsg;		// 메세지를 받았으면 false
	int		m_apple_sendmsg;



	DWORD	m_used_limit_delay;		// 지속시간
	DWORD	m_used_skill_delay;		// 발동시간

	bool	_OnSkillTimer(DWORD dt);
	void	_OnSetSkill(bool bNext);

	bool	m_using_surport; // 기본 다굴모드!!!
	bool	m_surport_sub;

	bool	m_need_change;
	int		m_need_surport;


	virtual void SetSurport(int source, int target);
	virtual bool _OnDecideTarget_Surport(DWORD dt);	

	virtual void AddLog(char * log , int phase_state=1);
	virtual void LogStart();
	virtual void LogEnd(bool force=false);

	void Lagrush_Profile();

	SYSTEMTIME m_start_time;
	UINT64	m_start_exp;
	UINT64	m_start_laim;

	int		m_kill_count;


public:
	void ResetAll();
	virtual void Init(bool playing=true);
	virtual void LoadSetting();
	virtual int GetCurrentState()
	{
		return m_current_state;
	}

	virtual void StopProc(int type=AutoPlay_StopProc__EndOfEnum);
	virtual void StartProc(DWORD dwGameMode);
	virtual void AutoProc()
	{
		if(!m_running) return;
		Polling();
	}
	virtual bool MsgProc( UINT uMsg, WPARAM wParam, LPARAM lParam )
	{
		return PassMsg(uMsg, wParam, lParam)?true:false;
	}


public:
	void Polling();
	UINT PassMsg(UINT msg, UINT wparam, UINT lparam);



protected:
	void _Polling_Potion(DWORD dt);
	void _Polling_PotionSkill(DWORD dt);
	void _Polling_Full(DWORD dt);

	bool _OnChangedProc(DWORD dt);
	bool _OnTownProc(DWORD dt);
	bool _OnFieldProc(DWORD dt);



protected:
	bool _IsPeaceArea();
	void _ChangeState(int state);
	bool _UseQuickItem(DWORD dwKey);
	bool _MoveToQuick(int pack_no, int slot, CItem* pItem);
	int	 _GetQuickSlotKey(int type);
	bool _OnMove(DWORD dt, float tx, float tz);


protected:
	State_ReturnValue _OnFindNpc(DWORD dt, bool ware=true);

	State_ReturnValue _OnStashItem(DWORD dt);
	State_ReturnValue _OnShopItem(DWORD dt);
	State_ReturnValue _OnRepairItem(DWORD dt);
	State_ReturnValue _OnBuyItem(DWORD dt);
	State_ReturnValue _OnSetQuickItem(DWORD dt);
	bool _OnMoveToField(DWORD dt);
	bool _OnMoveToWalk(DWORD dt);
	bool _OnMoveToMachine(DWORD dt);


	bool _OnAvoid(DWORD dt);

	bool _CheckReturnToCity(DWORD dt);
	void _ReturnToCity();

	bool _OnFieldUsing(DWORD dt);
	bool _OnPotion(DWORD dt);
	bool _OnPotion_Town(DWORD dt);
	bool _OnPotion_Spacial(DWORD dt);

	bool _OnSubArms(DWORD dt);
	bool _OnAdiaSummon(DWORD dt);
	bool _OnPet(DWORD dt);
	bool _OnCallSign(DWORD dt);
	bool _OnPickApple(DWORD dt);

	bool _OnShield(DWORD dt);		// none target
	bool _OnRecovery(DWORD dt);		// none target
	bool _OnBloody(DWORD dt);		// none target
	bool _OnVampTouch(DWORD dt);	// need target

	bool _OnShield2(DWORD dt);		// 버프류
	bool _OnShield3(DWORD dt);		// 버프류
	bool _OnShield4(DWORD dt);		// 버프류
	bool _OnDsystem(DWORD dt);		// 버프류



	bool _OnDecideTargetItem(DWORD dt);
	bool _OnDecideTarget(DWORD dt);
	LR_Warning_Type _IsWarning();

	bool _IsSpecialMob(int mobnum);

	bool _OnUseSkill(DWORD dt, int x, int y, CNkMob* target);	// 물푳E스킬
	bool _OnUseSkill(DWORD dt);


	bool _OnAttack(DWORD dt);					// need target
	bool _OnAttack_PotionSkill(DWORD dt);		// need target
	bool _OnPickItem(DWORD dt);		// 아이템 습탛E
	bool _OnMoveToBase(DWORD dt);	// 시작위치로 이동
	void _OnNextSkill(int next_skill);			// 다음 스킬선택


	void _ResetItem();
	void _ResetAttack();
	void _ResetMove();

	void _ResetCity();

	bool _CheckElapsedTime(DWORD dt, DWORD start_tick , DWORD delay_tick);



protected:
	bool m_dead;

public:
	bool IsDead()
	{
		return m_dead;
	}

	virtual void Render(bool activate=false, bool edit_mode=false);
	virtual void SetRivival()
	{
		m_dead = false;
	}
	virtual void SetDead()
	{
		m_dead = true;
	}
	virtual void OnRecvShop()
	{
		m_shop_sendmsg = 2;
	}
	virtual void OnRecvStash()
	{
		m_stash_sendmsg = 2;
	}
	virtual void OnRecvApple()
	{
		m_apple_sendmsg = 2;
	}

	virtual void OnDontPick()
	{
		m_bItem = false;
	}

	virtual void KillMob(int mob_index);

	virtual void	SetPos	(float x, float z); //						{ m_fStayPosX = x; m_fStayPosZ = z; }

	virtual void OutOfRange_Mob()
	{
		_ChangeState(NONE_STATE);
	}

public:
	bool _StartCheck();
	bool CheckEquipSkill(int cur_skill);
	bool _IsSubArms(CItem* pItem);

	bool _IsCantZone();
	bool _IsDungeon();
	bool _DontUsingFullAuto();


public:
	void _AddChat( char * msg );
	void _AddChat_Debug( char * msg );
	virtual bool IsRun()
	{
		return m_running;
	}
	virtual bool _OnInterrupt(LagRush_Interrupt interrupted);

	virtual void Init_variable();

	TEMP_QUICK m_temp_quick[QUICK_SLOT_NUM*QUICK_SLOT_PAGE];

	virtual void SetLostQuickSlot();
	virtual int IsLostQuickSlot(DWORD dt);
	virtual bool FillLostQuickSlot(int slot , int vnum);
	virtual void SetSendMove(int slot);


	bool _IsStashPush(CItem* pItem);


	bool m_devine_stone;


	bool _OnBuffTimer(DWORD dt);
	bool _CommonSkill(DWORD dt);



public:
	CAutoPlay_EX();
	~CAutoPlay_EX();// {}
};
#endif