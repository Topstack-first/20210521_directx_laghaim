#ifndef _ITEM_PROTO_H_
#define _ITEM_PROTO_H_

#include "Config.h"
#include <Def.h>
#include <string>
#include "CsyManager.h"
#include "LoloManager.h"

#include "LHGlobal_String.h"

#define ITYPE_WEAR		0
#define ITYPE_ARMOR		1
#define ITYPE_WEAPON	2
#define ITYPE_ETC		3
#define ITYPE_PET		4
#define ITYPE_SUB		5
#define ITYPE_NUM		6


#define ISHAPE_MAX		9

#define IWEAR_BODY		0
#define IWEAR_LEGS		1
#define IWEAR_NUM		2

#define IARMOR_HELMET	0
#define IARMOR_ARMOR	1
#define IARMOR_PANTS	2
#define IARMOR_CLOAK	3
#define IARMOR_SHOES	4
#define IARMOR_SHIELD	5
#define IARMOR_NECKLACE	6
#define IARMOR_RING		7
#define IARMOR_BRACELET	8
#define IARMOR_NUM		9

#define IWEAPON_SWORD	0
#define IWEAPON_AXE		1
#define IWEAPON_STAFF	2
#define IWEAPON_RING	3
#define IWEAPON_GUN		4
#define IWEAPON_MISSILE	5
#define IWEAPON_GATLING		6
#define IWEAPON_HAMMER		7
#define IWEAPON_DUAL_SWORD	8
#define IWEAPON_SPEAR		9
#define IWEAPON_NUM			10



#define IETC_GEMSTONE		0				// 보석원석
#define IETC_PART			1				// 
#define IETC_RECOVERY		2				// 회복제
#define IETC_GEM			3				// 보석
#define IETC_GIFT			4				// 보석
#define IETC_QUICK			5				// 퀵슬롯사용?
#define IETC_ETC			6				// ???
#define IETC_GEMCHIP		7				// ???
#define IETC_MAGICSTONE 	8
#define IETC_WARP			9
#define IETC_BUFF			10
#define IETC_QUEST			11
#define IETC_PET			12
#define IETC_PREMIUM		13
#define IETC_SKILL			14				// 미적용
#define IETC_RESOURCE		15
#define IETC_EFFECT			16
#define IETC_USING			17
#define IETC_SCROLL			18
#define IETC_SUMMON			19
#define IETC_SHELL			20
#define IETC_CHARGE			21
#define IECT_MOBSUMMON		22
#define IETC_CUBE			23
#define IETC_RECIPE			24
#define IETC_MATERIAL		25
#define IETC_SUMMONNPC		26
#define IETC_NUM			27

#define IPET_HORN		0
#define IPET_HEAD		1
#define IPET_BODY		2
#define IPET_WINGS		3
#define IPET_LEGS		4
#define IPET_TAIL		5
#define IPET_NUM		6

#define ISUB_SLAYER		0
#define ISUB_CHAKRAM	1
#define ISUB_BOOK		4
#define ISUB_SCROLL		3
#define ISUB_WAND		2
#define ISUB_SUMMON		5
#define ISUB_COMPONENT	6
#define ISUB_COLLECT	7
#define ISUB_S_EQUIP	8
#define ISUB_L_EQUIP	9
#define ISUB_SHELL		10
#define ISUB_ETC		11
#define ISUB_BLASTER	12
#define ISUB_DSTONE		13
#define ISUB_PANEL		14


#define ISPEED_VERY_FAST	0
#define ISPEED_FAST			1
#define ISPEED_NORMAL		2
#define ISPEED_SLOW			3
#define ISPEED_VERY_SLOW	4


#define ISEX_MAN		(1 << 0)
#define ISEX_WOMAN		(1 << 1)

#define IRACE_STINGER	(1 << 0)
#define IRACE_FIXY		(1 << 1)
#define IRACE_MYSTRIA	(1 << 2)
#define IRACE_HUMAN		(1 << 3)

#define WEAR_COUSE_SKIN		(1 << 0)
#define ARMOR_COUSE_SKIN	(1 << 0)
#define ARMOR_REMOVE_HAIR	(1 << 1)
#define ARMOR_REMOVE_FACE	(1 << 2)
#define ARMOR_REMOVE_BODY	(1 << 3)
#define ARMOR_REMOVE_ARMS	(1 << 4)
#define ARMOR_REMOVE_LEGS	(1 << 5)
#define ARMOR_COUSE_ARMOR	(1 << 6)
#define ARMOR_REMOVE_FEET	(1 << 7)

#define WEAPON_TWO_HANDS	(1 << 8)

#define IATT_NO_DROP		(1 << 0)
#define IATT_NO_SELL		(1 << 1)
#define IATT_CONFIRM_SELL	(1 << 2)
#define IATT_NO_EXCHANGE	(1 << 3)
#define IATT_NO_BUY			(1 << 4)
#define IATT_NO_GEM			(1 << 5)
#define IATT_NO_STASH		(1 << 6)
#define IATT_SAME_PRICE		(1 << 7)

/////2003  서브 아이템의 속성 define
#define IATT_SUB_WEAPON		(1 << 8)	//서브무기인가
#define IATT_SUB_QUICK		(1 << 9)	//퀵슬롯을 사용하는 서브아이템인가 
#define IATT_SUB_PASSPLUS   (1 << 10)	// 기존의 스페셜/플러스와 다른 개념으로 사용
#define IATT_MAX_LEVEL		(1 << 13)	// 최대레벨값 사용
#define IATT_CONVERSION		(1 << 14)	// 컨버전
#define IATT_REFINE			(1 << 15)	// 정련
#define IATT_GUILD_NO_STASH		(1 << 16)	// 길드창고 불가
#define IATT_NO_QUICK_DELETE	(1 << 17)   // 퀵슬롯 사용시 서버에서 확인 받고 지움.
#define IATT_EVENT_WEAR			(1 << 18)   ///이벤트복장착용 시스템에 사용할수있는 아이템인지 여부 플래그.
#define IATT_LIGHT_OFF			(1 << 19)	// [6/19/2008 반재승] 의상 아이템 라이팅 효과 끄기 관련 플래그.
#define IATT_PCBANG				(1 << 20)	// pc방 전용 아이템
#define IATT_NO_UPGRADE_ENDUR	(1 << 21)	// 강화 내구도 없음
#define IATT_DISABLE_NORMAL		(1 << 22)	// 일반 착용 불가

#define GET_CSY(item,bpdrace)		CsyManager::GetInstance()->GetCsy(g_ItemList.m_ItemArray[item->m_ProtoNum].m_CsyFile[bpdrace])
#define GET_CSYC(protoNum,bpdrace)	CsyManager::GetInstance()->GetCsy(g_ItemList.m_ItemArray[protoNum].m_CsyFile[bpdrace])
#define GET_LOLO(item)				LoloManager::GetInstance()->GetLolo(g_ItemList.m_ItemArray[item->m_ProtoNum].m_LoloFile)
#define GET_LOLOC(protoNum)			LoloManager::GetInstance()->GetLolo(g_ItemList.m_ItemArray[protoNum].m_LoloFile)
#define GET_SUB_CSY(item)			CsyManager::GetInstance()->GetCsy(g_ItemList.m_ItemArray[item->m_ProtoNum].m_SubCsyFile)
#define GET_SUB_CSYC(protoNum)		CsyManager::GetInstance()->GetCsy(g_ItemList.m_ItemArray[protoNum].m_SubCsyFile)

#define GET_ITEM_WIDTH(item)		(g_ItemList.m_ItemArray[item->m_ProtoNum].m_Width)
#define GET_ITEM_HEIGHT(item)		(g_ItemList.m_ItemArray[item->m_ProtoNum].m_Height)

#define GET_ITEM_WIDTH_S(item)		(g_ItemList.m_ItemArray[item->m_ProtoNum].m_WidthForScale)
#define GET_ITEM_HEIGHT_S(item)		(g_ItemList.m_ItemArray[item->m_ProtoNum].m_HeightForScale)

#	define GET_ITEM_NAME(item)		(LHGET_ITEM_NAME(item->m_Vnum))
#	define GET_ITEM_NAME_VNUM(vnum)	(LHGET_ITEM_NAME(vnum))
#	define GET_ITEM_NAME_IDX(idx)	(LHGET_ITEM_NAME(g_ItemList.FindItem(idx)) )



#define GET_ITEM_DESC(item)			(g_ItemList.m_ItemArray[item->m_ProtoNum].m_strDesc) ///아이템설명입력
#define GET_ITEM_LEVEL(item)		(g_ItemList.m_ItemArray[item->m_ProtoNum].m_MinLevel)
#define GET_ITEM_MAXLEVEL(item)		(g_ItemList.m_ItemArray[item->m_ProtoNum].m_MaxUse)//wan:2005-3  아이템의 최대레벨
#define GET_ITEM_INT(item)			(g_ItemList.m_ItemArray[item->m_ProtoNum].m_Int)
#define GET_ITEM_DEX(item)			(g_ItemList.m_ItemArray[item->m_ProtoNum].m_Dex)
#define GET_ITEM_RACE(item)			(g_ItemList.m_ItemArray[item->m_ProtoNum].m_RaceFlag)
#define GET_ITEM_NUM0(item)			(g_ItemList.m_ItemArray[item->m_ProtoNum].m_Num0)
#define GET_ITEM_NUM1(item)			(g_ItemList.m_ItemArray[item->m_ProtoNum].m_Num1)
#define GET_ITEM_NUM2(item)			(g_ItemList.m_ItemArray[item->m_ProtoNum].m_Num2)
#define GET_ITEM_NUM3(item)			(g_ItemList.m_ItemArray[item->m_ProtoNum].m_Num3)
#define GET_ITEM_NUM4(item)			(g_ItemList.m_ItemArray[item->m_ProtoNum].m_Num4)
#define GET_ITEM_NUM5(item)			(g_ItemList.m_ItemArray[item->m_ProtoNum].m_Num5)
#define GET_ITEM_NUM6(item)			(g_ItemList.m_ItemArray[item->m_ProtoNum].m_Num6)
#define GET_ITEM_NUM7(item)			(g_ItemList.m_ItemArray[item->m_ProtoNum].m_Num7)
#define GET_ITEM_NUM8(item)			(g_ItemList.m_ItemArray[item->m_ProtoNum].m_Num8)
#define GET_ITEM_NUM9(item)			(g_ItemList.m_ItemArray[item->m_ProtoNum].m_Num9)
#define GET_DEFENSE(item)			(g_ItemList.m_ItemArray[item->m_ProtoNum].m_Defense)
#define GET_DAM1(item)				(g_ItemList.m_ItemArray[item->m_ProtoNum].m_Dam1)
#define GET_DAM2(item)				(g_ItemList.m_ItemArray[item->m_ProtoNum].m_Dam2)
#define GET_MAGICDAM_BONUS(item)	(g_ItemList.m_ItemArray[item->m_ProtoNum].m_MagicDamBonus)
#define GET_MAGIC_DEFENSE(item)		(g_ItemList.m_ItemArray[item->m_ProtoNum].m_MagicDefense)
#define GET_REDUCE_MANA_USAGE(item)	(g_ItemList.m_ItemArray[item->m_ProtoNum].m_ReduceManaUsage)
#define GET_MAGIC_ATTACK_SPEED(item)	(g_ItemList.m_ItemArray[item->m_ProtoNum].m_MagicAttackSpeed)
#define GET_REFLECT_DAM(item)		(g_ItemList.m_ItemArray[item->m_ProtoNum].m_ReflectDam)
//#define GET_DAM2(item)			(g_ItemList.m_ItemArray[item->m_ProtoNum].m_Dam2)
#define GET_APPTO(item)				(g_ItemList.m_ItemArray[item->m_ProtoNum].m_AppTo)
#define GET_RECVAL(item)			(g_ItemList.m_ItemArray[item->m_ProtoNum].m_RecValue)
#define GET_WEIGHT(item)			(g_ItemList.m_ItemArray[item->m_ProtoNum].m_Weight)
#define GET_PRICE(item)				(g_ItemList.m_ItemArray[item->m_ProtoNum].m_Price)
#define GET_WATT(item)				(g_ItemList.m_ItemArray[item->m_ProtoNum].m_Watt)
#define GET_ITEM_MANA(item)			(g_ItemList.m_ItemArray[item->m_ProtoNum].m_Mana)
#define GET_TEXTR_NUM(item,bpdrace)	(item->m_ProtoNum < 0 ? -1 : CsyManager::GetInstance()->GetLoloTextureNum(g_ItemList.m_ItemArray[item->m_ProtoNum].m_CsyTextureFile[bpdrace]) )
#define GET_TEXTR_NUM_PROTO(protoNum,bpdrace)	(protoNum < 0 ? -1 : CsyManager::GetInstance()->GetLoloTextureNum(g_ItemList.m_ItemArray[protoNum].m_CsyTextureFile[bpdrace]) )
#define GET_LOLOTEXTR_NUM(item)		(item->m_ProtoNum < 0 ? -1 : LoloManager::GetInstance()->GetLoloTextureNum(g_ItemList.m_ItemArray[item->m_ProtoNum].m_LoloTextureFile) )
#define GET_RENDER_FLAG(item)		(item->m_ProtoNum < 0 ? 0 : g_ItemList.m_ItemArray[item->m_ProtoNum].m_RenderFlag)
#define GET_ATT_FLAG(item)			(item->m_ProtoNum < 0 ? 0 : g_ItemList.m_ItemArray[item->m_ProtoNum].m_AttFlag)
#define GET_TYPE(item)				(item->m_ProtoNum < 0 ? 0 : g_ItemList.m_ItemArray[item->m_ProtoNum].m_Type)
#define GET_TYPE_REF(item)			(item.m_ProtoNum < 0 ? 0 : g_ItemList.m_ItemArray[item.m_ProtoNum].m_Type)
#define GET_SHAPE(item)				(item->m_ProtoNum < 0 ? 0 : g_ItemList.m_ItemArray[item->m_ProtoNum].m_Shape)
#define GET_SHAPE_REF(item)			(item.m_ProtoNum < 0 ? 0 : g_ItemList.m_ItemArray[item.m_ProtoNum].m_Shape)
#define GET_ATT_RANGE(item)			(item->m_ProtoNum < 0 ? 0 : g_ItemList.m_ItemArray[item->m_ProtoNum].m_AttRange)
#define GET_WEAPON_SPEED(item)		(item->m_ProtoNum < 0 ? ISPEED_VERY_SLOW : (g_ItemList.m_ItemArray[item->m_ProtoNum].m_Speed))
#define GET_TOGEM_VNUM(item)		(item->m_ProtoNum < 0 ? 0 : g_ItemList.m_ItemArray[item->m_ProtoNum].m_ToGemVnum)
#define GET_NEEDKIT_NUM(item)		(item->m_ProtoNum < 0 ? 0 : g_ItemList.m_ItemArray[item->m_ProtoNum].m_NeedKitNum)
#define GET_GEMMIN_LEVEL(item)		(item->m_ProtoNum < 0 ? 0 : g_ItemList.m_ItemArray[item->m_ProtoNum].m_GemMinLevel)
#define GET_GEMMAX_LEVEL(item)		(item->m_ProtoNum < 0 ? 0 : g_ItemList.m_ItemArray[item->m_ProtoNum].m_GemMaxLevel)
#define GET_TOWN_NUM(item)			(item->m_ProtoNum < 0 ? 0 : g_ItemList.m_ItemArray[item->m_ProtoNum].m_TownNum)
#define GET_ITEM_NUM(item)			(item->m_ProtoNum < 0 ? 0 : g_ItemList.m_ItemArray[item->m_ProtoNum].m_Vnum)	// nate 2005-07-19 : Get Item Num
#define IS_LIGHT_OFF(item)			(GET_ATT_FLAG(item) & IATT_LIGHT_OFF)		// [6/19/2008 반재승] 의상 아이템 라이팅 효과 끄기 관련 플래그.



#define FLY_STONE_NUM	3
#define FLY_ITEM_NUM	3



#define MAX_KIND		5
#define MAX_PLUS		16
#define RESOURCE_ITEM_A	1
#define RESOURCE_ITEM_B	1
#define RESOURCE_ITEM_C	1
#define RESOURCE_ITEM_D	1
#define RESOURCE_ITEM_E	1


enum eGem_Grade_Type
{
	eGem_Grade__00,
	eGem_Grade__01,
	eGem_Grade__02,
	eGem_Grade__03,
	eGem_Grade__04,
	eGem_Grade__05,
	eGem_Grade__06,
	eGem_Grade__07,
	eGem_Grade__08,
	eGem_Grade__09,
	eGem_Grade__10,
	eGem_Grade__11,
	eGem_Grade__12,
	eGem_Grade__13,
	eGem_Grade__14,
	eGem_Grade__15,
	eGem_Grade__16,
	eGem_Grade__17,
	eGem_Grade__18,
	eGem_Grade__19,
	eGem_Grade__20,

	eGem_Grade__EndOfEnum
};

enum eItem_Grade_Type
{
	eItem_Grade__00,
	eItem_Grade__01,
	eItem_Grade__02,
	eItem_Grade__03,
	eItem_Grade__04,
	eItem_Grade__05,
	eItem_Grade__06,
	eItem_Grade__07,
	eItem_Grade__08,
	eItem_Grade__09,
	eItem_Grade__10,
	eItem_Grade__11,
	eItem_Grade__12,
	eItem_Grade__13,
	eItem_Grade__14,
	eItem_Grade__15,
	eItem_Grade__16,
	eItem_Grade__17,
	eItem_Grade__18,
	eItem_Grade__19,
	eItem_Grade__20,

	eItem_Grade__EndOfEnum
};


//
#include "IndexedTexture.h"
// nate 2004 - 11
// new race
#include "ChaAct.h"

class CVirtualFile;

#define MAX_LENGTH_ITEM_LOCAL_NAME 50 ///아이템설명입력
#define MAX_LENGTH_ITEM_DESCRIPTION 512 ///아이템설명입력

typedef struct
{
//	int m_NameLen;
	char m_strLocalName[MAX_LENGTH_ITEM_LOCAL_NAME];	// 아이템 현지 이름
//	int m_DescLen;
	char m_strDesc[MAX_LENGTH_ITEM_DESCRIPTION];   // 아이템 설명
} sItemDescription; ///아이템설명입력

class CItemProto
{
public:
	CItemProto();
	virtual ~CItemProto();

	//void ReadData(FILE *fp);
	int m_Vnum;		// 아이템 번호
	int m_Type;		// 아이템 대 분류
	int m_Shape;	// 아이템 소 분류
	std::string m_Name;	// 아이템 이름
	std::string m_strDesc;   // 아이템 설명 ///아이템설명입력

	int m_LoloNum;
	int m_LoloTextrNum;
	int m_SubCsyNum;
	int m_CsyNum[ NUM_RACE*NUM_SEX ];
	int m_TextrNum[ NUM_RACE*NUM_SEX ];

	std::string m_LoloFile;
	std::string m_LoloTextureFile;
	std::string m_SubCsyFile;
	std::string m_CsyFile[NUM_RACE*NUM_SEX];
	std::string m_CsyTextureFile[NUM_RACE*NUM_SEX];

	int m_RaceFlag;
	int m_EffectTextrNum;
	int m_RenderFlag;
	int m_AttFlag;

	union
	{
		int m_Num0;
		int m_Defense; // Armor
		int m_Dam1; // Weapon
		int m_AppTo;
		int m_ToGemVnum;	// 원석용
		int m_GemMinLevel;	// 보석용
		int m_PetVital;
	};
	union
	{
		int m_Num1;
		int m_Dam2; // Weapon
		int m_RecValue;
		int m_MagicDefense; // 망토
		int m_NeedKitNum;	// 원석용
		int m_GemMaxLevel;	// 보석용
		int m_TownNum;
		int m_PetRangeAtt;
	};
	union
	{
		int m_Num2;
		int m_BonusDam1;
		int m_ReduceManaUsage;
		int m_MagicDamBonus;
		int m_PetMeleeAtt;
	};
	union
	{
		int m_Num3;
		int m_BonusDam2;
		int m_MagicAttackSpeed;
		int m_PetRangeDef;
	};
	union
	{
		int m_Num4;
		int m_Watt; // Armor, Weapon
		int m_ReflectDam;
		int m_Mana;
		int m_PetMeleeDef;
	};
	union
	{
		int m_Num5;
		int m_AttRange;
		int m_PetSpeed;
	};
	union
	{
		int m_Num6;
		int m_Speed;
		int m_PetCompose;
	};
	union
	{
		int m_Num7;
		int m_PetPickup;
	};
	union
	{
		int m_Num8;
	};
	union
	{
		int m_Num9;
	};

	int m_Weight;
	int m_Int;
	int m_Dex;
	int m_MinLevel;
	int m_MaxUse;
	int m_Price;

	int m_Width;
	int m_Height;

	int m_WidthForScale;
	int m_HeightForScale;

	int m_item_grade;
};

class CItemList
{
public:
	CItemList(IndexedTexture *pIndexedTexture = NULL);
	virtual ~CItemList();
	void	DeleteAllData();

	void SetIndexedTexture(IndexedTexture *pIndexedTexture);
	BOOL ReadItems();
	BOOL RefreshItems();
	void ReadInt(int *i_data, PFILE *fp, int &seed);
	void ReadString(char *str, PFILE *fp, int &int_seed, char &char_seed);
	void ReadStringDup(char* &str, PFILE *fp, int &int_seed, char &char_seed);
	int SetTexture(char *fileName);
	int FindItem(int v_num);

	IndexedTexture *m_pIndexedTexture;
	char *m_CsyDir;
	char *m_LoloDir;

	CItemProto *m_ItemArray;
	int m_ItemCount;
};

extern CItemList g_ItemList;

#endif // _ITEM_PROTO_H_
