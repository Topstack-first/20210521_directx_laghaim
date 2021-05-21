#include "stdafx.h"


#include "SpecialItem.h"
#include "ItemProto.h"
#include "AlphaPopUp.h"
#include "g_stringmanager.h"
#include "ControlInven.h"
//CSpecialValue g_SpecialValue[6][3];


extern int g_AirshipProto[];
extern int g_AirshipProto_VNUM[];

int g_WeaponSpecialValue[SPECIAL_WEAPON_NUM][SPECIAL_LEVEL_NUM] =
{
	{1,	2,	5,	0,	0,	0,	0},	// 최소데미지상승
	{1,	2,	5,	0,	0,	0,	0}, // 최대 데미지 상승
	{1,	2,	5,	0,	0,	0,	0}, // 전체 데미지 상승
	{2,	5,	10,	0,	0,	0,	0}, // 전체 데미지 % 상승
	{3,	6,	12,	0,	0,	0,	0}, // 결정타(데미지 20% 추가) 확률
	{3,	6,	12,	0,	0,	0,	0}, // 치명타(데미지 50% 추가) 확률
	{10,0,	0,	0,	0,	0,	0}, // 무기 공격속도 상승(한단계)
	{3,	10,	25,	0,	0,	0,	0}, // 기민제 감소
	{1,	0,	0,	0,	0,	0,	0}, // 기민제 없음
	{3,	10,	25,	0,	0,	0,	0}, // 힘제 감소
	{1,	0,	0,	0,	0,	0,	0}, // 힘제 없음
	{3,	10,	25,	0,	0,	0,	0}, // 렙제 감소
	{1,	0,	0,	0,	0,	0,	0}, // 렙제 없음
	{4,	10,	20,	0,	0,	0,	0}, // 특수 데미지 상승
	{5, 10, 20,	0,	0,	0,	0},  // 상대 회피율 감소
	{10, 15, 25,	0,	0,	0,	0},  // 데미지 상승
	{3, 7, 10,	0,	0,	0,	0}  // 데들리 히트
};

int g_WeaponSpecialMask[SPECIAL_WEAPON_NUM] =
{
	0x3, 0xC, 0x30 , 0xC0 , 0x300 , 0xC00 , 0x1000 , 0x6000 , 0x8000 , 0x30000 ,
	0x40000 , 0x180000 , 0x200000, 0xC00000, 0x3000000 , 0x06000000, 0x18000000,
};

int g_WeaponSpecialShift[SPECIAL_WEAPON_NUM] =
{ 0, 2 , 4 , 6 , 8 , 10 , 12 , 13 , 15 , 16 , 18 , 19 , 21, 22, 24 , 25, 27  };


/*int g_StaffSpecialValue[SPECIAL_STAFF_NUM][SPECIAL_LEVEL_NUM] =
{
{1,	3,	7,	0,	0,	0,	0}, // 전격 계열 마법 데미지 추가
{1,	3,	7,	0,	0,	0,	0}, // 화염 계열 마법 데미지 추가
{1,	3,	7,	0,	0,	0,	0}, // 스톤 계열 마법 데미지 추가
{1,	2,	5,	0,	0,	0,	0}, // 전체 마법 데미지 추가
{5,	10,	15,	0,	0,	0,	0}, // 마법 공격속도 상승
{3,	10,	25,	0,	0,	0,	0}, // 지혜제 감소
{1,	0,	0,	0,	0,	0,	0} // 지혜제 없음
};
int g_StaffSpecialMask[SPECIAL_STAFF_NUM] =
{ 0x3 , 0xC , 0x30 , 0xC0 , 0x300 , 0xC00 , 0x1000 };
int g_StaffSpecialShift[SPECIAL_STAFF_NUM] =
{ 0 , 2 , 4 , 6 , 8 , 10 , 12 };
*/

int g_ArmorSpecialValue[SPECIAL_ARMOR_NUM][SPECIAL_LEVEL_NUM] =
{
	/*
	{2,	5,	10,	0,	0,	0,	0}, // 전체 마법 방어력 상승
	{3,	7,	15,	0,	0,	0,	0}, // 전격계열 마법 방어력 상승
	{3,	7,	15,	0,	0,	0,	0}, // 화염계열 마법 방어력 상승
	{3,	7,	15,	0,	0,	0,	0}, // 암석계열 마법 방어력 상승
	{3,	7,	15,	0,	0,	0,	0}, // 원격전(총, 마법) 방어력 상승
	{7,	15,	30,	0,	0,	0,	0}, // 근접전(칼, 도끼) 방어력 상승
	{5,	12,	25,	0,	0,	0,	0} // 물리적(칼, 도끼, 총) 방어력 상승
	  */
	{1,	2,	5,	0,	0,	0,	0}, // 물리적(칼, 도끼, 총) 방어력 상승
	{2,	5,	10,	0,	0,	0,	0}, // 전체 마법 방어력 상승
	{1,	2,	5,	0,	0,	0,	0}, // 방어력 상승
	{5,	10,	15,	0,	0,	0,	0}, // 방어력 % 상승
	{10,20,	30,	0,	0,	0,	0}, // 특수 방어력 % 상승
	{872,873,874,0,	0,	0,	0}	// 비행기 아이템 번호
};
int g_ArmorSpecialMask[SPECIAL_ARMOR_NUM] =
//{ 0x3 , 0xC , 0x30 , 0xC0 , 0x300 , 0xC00 , 0x3000 };
{ 0x3 , 0xC , 0x30 , 0xC0, 0xC00000, 0x70000000};
int g_ArmorSpecialShift[SPECIAL_ARMOR_NUM] =
{ 0 , 2 , 4 , 6, 22, 28};

int g_ArmorSpecialValue2[SPECIAL_ARMOR_NUM2][SPECIAL_LEVEL_NUM] =
{
	{50, 250, 500, 750, 1000, 1250, 1500},
	{50, 250, 500, 750, 1000, 1250, 1500},
	{50, 250, 500, 750, 1000, 1250, 1500},
	{50, 250, 500, 750, 1000, 1250, 1500},
	{2, 4, 6, 8, 10, 30, 50},
};

int g_ArmorSpecialMask2[SPECIAL_ARMOR_NUM2] =
{ 0x1C00, 0xE000, 0x70000, 0x380000, 0x1C00000 };

int g_ArmorSpecialShift2[SPECIAL_ARMOR_NUM2] =
{ 10, 13, 16, 19, 22 };


//----------------------------------------------------------------------------
//-- Evolution 팔찌
//----------------------------------------------------------------------------
//-- 2004.10.14
//-- Lyul / 추가.

//-- 각 +Option 은 32bit(DWORD) 형 안에 Bit 단위로 저장된다.
//-- 따라서 BitMsk 값을 지정하고 BitShf 값을 지정후 그 값을 얻는다.
//-- 현 Table 은 DB에 있는 정보와 동일한 값이며
//-- DB 와의 Network 로 변경 비교 등은 없는 로컬 값이다.
//-- 따라서 DB값이 변경 되면 Local 값도 변경 해야 한다.
//-- SPECIAL_RING_NUM :  속성 개수
/*

HEX : 0x02FFFFFF
DEC : 50331647

//-----------------------------------------------------------------------------------------
weight : 8421 8421 8421 8421    8421 8421 8421 8421
bit    : 0000 0000 0000 0000  | 0000 0000 0000 0000
                                                 11 (0x0000003)        -- 현상금 획득 수치 상승     shift 0
                                               1100 (0x000000c)        -- 경험치 획득 수치 상승     shift 2
                                           111 0000 (0x0000070)        -- 아이템 획득 확률 상승     shift 4
                                          1000 0000 (0x0000080)        -- 포인트 재분배 자유        shift 7

                                       11 0000 0000 (0x0000300)        -- 생명력 %d 상승            shift 8
                                     1100 0000 0000 (0x0000C00)        -- 마법력 %d 상승            shift 10
                                  11 0000 0000 0000 (0x0003000)        -- 전기력 %d 상승            shift 12
                                1100 0000 0000 0000 (0x000C000)        -- 스테미너 %d 상승          shift 14
                          11    0000 0000 0000 0000 (0x0030000)        -- 반사데미지 %d 상승        shift 16
                        1100    0000 0000 0000 0000 (0x00C0000)        -- 데미지흡수 %d 상승        shift 18
                     11 0000    0000 0000 0000 0000 (0x0300000)        -- 전체저항력 %d 상승        shift 20
                   1100 0000    0000 0000 0000 0000 (0x0C00000)        -- 몬스터 혼돈 확률 %d 상승  shift 22
                 1 0000 0000    0000 0000 0000 0000 (0x1000000)        -- 마법 방어력 %d 상승       shift 24
                10 0000 0000    0000 0000 0000 0000 (0x2000000)        -- 회복력증가  %d 상승       shift 25

*/
//-- Table BRACELET
int g_BraceletSpecialValue[SPECIAL_BRACELET_NUM][SPECIAL_LEVEL_NUM] =
{
	{2,	5,	10,	0,	0,	0,	0},  // 현상금 획득 수치 상승
	{5,	10,	2,	0,	0,	0,	0},  // 경험치 획득 수치 상승, 2, 5, 10에서 기획자요청으로 5,10,2로 변경(06/04/04)
	{1,	2,	5,	10,	15,	30,	50}, // 아이템 획득 확률 상승,
	{1,	0,	0,	0,	0,	0,	0},   // 포인트재분배 자유

	{3, 5, 7, 0, 0, 0, 0},       //-- 생명력
	{3, 5, 7, 0, 0, 0, 0},       //-- 마법력
	{3, 5, 7, 0, 0, 0, 0},       //-- 전기력
	{3, 5, 7, 0, 0, 0, 0},       //-- 스테미너

	{10, 15, 20, 0, 0, 0, 0},    //-- 반사 데미지
	{5, 10, 15, 0, 0, 0, 0},     //-- 데미지 흡수
	{10, 20, 30, 0, 0, 0, 0},    //-- 전체 저항력
	{10, 15, 20, 0, 0, 0, 0},    //-- 몬스터 혼돈

	{10, 0, 0, 0, 0, 0, 0},      //-- 마법 방어력
	{10, 0, 0, 0, 0, 0, 0}      //-- 물약회복
};


//-- Bit Mask 값
int g_BraceletSpecialMask[SPECIAL_BRACELET_NUM] =
{
	0x3,     0xc,     0x70,     0x80,    0x300,
	0xc00,   0x3000,  0xc000,   0x30000, 0xc0000,
	0x300000,0xc00000,0x1000000,0x2000000
};

//-- Bit Shift 값
int g_BraceletSpecialShift[SPECIAL_BRACELET_NUM] =
{ 0, 2, 4, 7, 8, 10, 12, 14, 16, 18, 20, 22, 24, 25};


//----------------------------------------------------------------------------
//-- 고대 반지
//----------------------------------------------------------------------------
//-- 각 +Option 은 32bit(DWORD) 형 안에 Bit 단위로 저장된다.
//-- 따라서 BitMsk 값을 지정하고 BitShf 값을 지정후 그 값을 얻는다.
//-- 현 Table 은 DB에 있는 정보와 동일한 값이며
//-- DB 와의 Network 로 변경 비교 등은 없는 로컬 값이다.
//-- 따라서 DB값이 변경 되면 Local 값도 변경 해야 한다.
//-- SPECIAL_RING_NUM :  속성 개수

/*
※ Skill 은 11 안된다 10 이나 01 둘 중 하나.

//-----------------------------------------------------------------------------------------
weight : 8421 8421 8421 8421    8421 8421 8421 8421
bit    : 0000 0000 0000 0000  | 0000 0000 0000 0000
                                                111 (0x07)        -- 통솔력      shift 0
                                            11 1000 (0x38)        -- 체질        shift 3
                                        1 1100 0000 (0x1C0)       -- 기민성      shift 6
                                     1110 0000 0000 (0xE00)       -- 힘          shift 9
                                 111 0000 0000 0000 (0x7000)      -- 지혜        shift 12
                                1000 0000 0000 0000 (0x8000)      -- 카운터      shift 15
                           1                        (0x10000)     -- 크리티컬    shift 16
                          10                        (0x20000)     -- 누적저항    shift 17
                         100                        (0x40000)     -- 볼캐닉 저항 shift 18
                        1000                        (0x80000)     -- 석화저항    shift 19
                      1 0000                        (0x100000)    -- 콜드저항    shift 20
                     10 0000                        (0x200000)    -- 전체저항    shift 21
                   1100 0000                        (0xC00000)    -- 스킬시간    shift 22
                11 0000 0000                        (0x3000000)   -- 물약회복    shift 24
               100 0000 0000                        (0x4000000)   -- 데미지흡수  shift 26
              1000 0000 0000                        (0x8000000)   -- 회피율      shift 27

*/

//-- Table
int g_RingSpecialValue[SPECIAL_RING_NUM][SPECIAL_LEVEL_NUM] =
{
	{2,	3,	5,	8,	12,	18,	27}, // 통솔력 상승
	{1,	2,	3,	5,	8,	12,	18}, // 체질 상승
	{1,	2,	3,	5,	8,	12,	18}, // 기민성 상승
	{1,	2,	3,	5,	8,	12,	18}, // 힘 상승
	{1,	2,	3,	5,	8,	12,	18} // 지혜 상승

	,{5, 0, 0, 0, 0, 0, 0},//카운터
	{5, 0, 0, 0, 0, 0, 0},//크리티컬
	{5, 0, 0, 0, 0, 0, 0},//누적저항
	{5, 0, 0, 0, 0, 0, 0},//볼캐닉저항
	{5, 0, 0, 0, 0, 0, 0},//석화저항
	{5, 0, 0, 0, 0, 0, 0},//콜드저항
	{5, 0, 0, 0, 0, 0, 0},//전체저항
	{20, 40, 60, 0, 0, 0, 0},//스킬시간
	{10, 20, 30, 0, 0, 0, 0},//물약회복

	{5, 0, 0, 0, 0, 0, 0},//DAMAGE 흡수
	{5, 0, 0, 0, 0, 0, 0}//회피율
};


//-- Bit Mask 값
int g_RingSpecialMask[SPECIAL_RING_NUM] =
{
	0x7 , 0x38 , 0x1C0 , 0xE00 , 0x7000 ,
	0x8000 , 0x10000 , 0x20000 , 0x40000 , 0x80000 , 0x100000 , 0x200000 , 0xC00000 ,0X3000000,
	0x4000000, 0x8000000
};

//-- Bit Shift 값
//--
int g_RingSpecialShift[SPECIAL_RING_NUM] =
{
	0 , 3 , 6 , 9 , 12 ,
	15 , 16 , 17 , 18 , 19 , 20 , 21 , 22 , 24, 26, 27
};



int g_NecklaceSpecialValue[SPECIAL_NECKLACE_NUM][SPECIAL_LEVEL_NUM] =
{
	{10,	30,	50,	100,	150,	300,	500}, // 생명력 상승
	{10,	30,	50,	100,	150,	300,	500}, // 마법력 상승
	{10,	30,	50,	100,	150,	300,	500}, // 스테미너 상승
	{10,	30,	50,	100,	150,	300,	500}, // 전기력 상승
	{4,	10,	20,	 30, 	 40,	 60,	100}, // 통솔 상승
	{2,	 5,	10,	 15,	 20,	 30,	 50}, // 체질 상승
	{2,	 5,	10,	 15,	 20,	 30,	 50}, // 기민 상승
	{2,	 5,	10,	 15,	 20,	 30,	 50}, //  힘  상승
	{2,	 5,	10,	 15,	 20,	 30,	 50}, // 지혜 상승
	{20,	2,   3,   4,	  5,	 10,	 20} // Block 옵션

};
int g_NecklaceSpecialMask[SPECIAL_NECKLACE_NUM] =
{ 0x7 , 0x38 , 0x1C0 , 0xE00, 0x7000, 0x38000, 0x1C0000, 0xE00000, 0x7000000, 0x38000000 };
int g_NecklaceSpecialShift[SPECIAL_NECKLACE_NUM] =
{ 0 , 3 , 6 , 9, 12, 15, 18, 21, 24, 27 };


void InitSpecial()
{
}

#define SPECIAL_DESCR_COLOR	RGB(100, 255, 100)

//----------------------------------------------------------------------------
//-- GetSpecial
//----------------------------------------------------------------------------
//-- 특수 flag 에 대한 표현

//-- 04.03.16 - 추가(수정) 완료
//-- Lyul
//-- 데미지 흡수와 회피율 추가
//-- String 추가
//-- String DLL 변경
void GetSpecial(CMultiText &multiText, int item_type, int item_shape, int special, int special2)
{
	char strTemp[128];
	if (item_type == ITYPE_WEAPON)
	{
		//------------------------------------------------------------------------
		//-- 지팡이 말고 일반 무기
		if (item_shape != IWEAPON_STAFF)
		{
			for (int sp_idx = 0; sp_idx < SPECIAL_STAFF_NUM; sp_idx++)
			{
				int special_level = (special & g_WeaponSpecialMask[sp_idx]) >> g_WeaponSpecialShift[sp_idx];
				if (special_level > 0)
				{
					switch (sp_idx)
					{
					case SPECIAL_DAM_MIN:
						//-- IDS_SPECIAL_MIN_ATT_INC : 최소공격력 %d 상승
						sprintf(strTemp, G_STRING(IDS_SPECIAL_MIN_ATT_INC), g_WeaponSpecialValue[sp_idx][special_level-1]);
						break;
					case SPECIAL_DAM_MAX:
						//-- IDS_SPECIAL_MAX_ATT_INC : 최대공격력 %d 상승
						sprintf(strTemp, G_STRING(IDS_SPECIAL_MAX_ATT_INC) , g_WeaponSpecialValue[sp_idx][special_level-1]);
						break;
					case SPECIAL_DAM_ALL:
						//-- IDS_SPECIAL_ATT_INC : 공격력 %d 상승
						sprintf(strTemp, G_STRING(IDS_SPECIAL_ATT_INC), g_WeaponSpecialValue[sp_idx][special_level-1]);
						break;
					case SPECIAL_DAM_PER:
						//-- IDS_SPECIAL_ATT_INC2 : 공격력 %d%% 상승
						sprintf(strTemp, G_STRING(IDS_SPECIAL_ATT_INC2), g_WeaponSpecialValue[sp_idx][special_level-1]);
						break;
					case SPECIAL_HIT_1:
						//-- IDS_SPECIAL_HIT_ATT : 결정타 공격 %d%%
						sprintf(strTemp, G_STRING(IDS_SPECIAL_HIT_ATT), g_WeaponSpecialValue[sp_idx][special_level-1]);
						break;
					case SPECIAL_HIT_2:
						//-- IDS_SPECIAL_FATAL_ATT : 치명타 공격 %d%%
						sprintf(strTemp, G_STRING(IDS_SPECIAL_FATAL_ATT), g_WeaponSpecialValue[sp_idx][special_level-1]);
						break;
					case SPECIAL_ATT_SPEED:
						//-- IDS_SPECIAL_ATT_SPEED_INC : 공격속도 상승
						sprintf(strTemp, G_STRING(IDS_SPECIAL_ATT_SPEED_INC));
						break;

					default:
						*strTemp = '\0';
						break;
					}
					multiText.AddString(strTemp, SPECIAL_DESCR_COLOR);
				}
			}
		}
		//------------------------------------------------------------------------
		//-- 지팡이
		else
		{
			for (int sp_idx = 0; sp_idx < SPECIAL_STAFF_NUM; sp_idx++)
			{
				int special_level = (special & g_WeaponSpecialMask[sp_idx]) >> g_WeaponSpecialShift[sp_idx];
				if (special_level > 0)
				{
					switch (sp_idx)
					{
					case SPECIAL_DAM_MIN:
						//-- IDS_SPECIAL_DOWN_MAGIC_DAM : 마법데미지 감소폭 %d%% 하락
						sprintf(strTemp, G_STRING(IDS_SPECIAL_DOWN_MAGIC_DAM), g_WeaponSpecialValue[sp_idx][special_level-1]);
						break;
					case SPECIAL_DAM_MAX:
						//-- IDS_SPECIAL_UP_MAGIC_DAM : 마법데미지 상승폭 %d%% 상승
						sprintf(strTemp, G_STRING(IDS_SPECIAL_UP_MAGIC_DAM), g_WeaponSpecialValue[sp_idx][special_level-1]);
						break;
					case SPECIAL_DAM_ALL:
						//-- IDS_SPECIAL_INC_BASEMAGIC_DAM : 기본 마법데미지 %d 상승
						sprintf(strTemp, G_STRING(IDS_SPECIAL_INC_BASEMAGIC_DAM), g_WeaponSpecialValue[sp_idx][special_level-1]);
						break;
					case SPECIAL_DAM_PER:
						//-- IDS_SPECIAL_INC_MAGIC_DAM : 마법데미지 %d%% 상승
						sprintf(strTemp, G_STRING(IDS_SPECIAL_INC_MAGIC_DAM), g_WeaponSpecialValue[sp_idx][special_level-1]);
						break;
					case SPECIAL_HIT_1:
						//-- IDS_SPECIAL_HIT_MAGIC : 마법 결정타 %d%%
						sprintf(strTemp, G_STRING(IDS_SPECIAL_HIT_MAGIC), g_WeaponSpecialValue[sp_idx][special_level-1]);
						break;
					case SPECIAL_HIT_2:
						//-- IDS_SPECIAL_FATAL_MAGIC : 마법 치명타 %d%%
						sprintf(strTemp, G_STRING(IDS_SPECIAL_FATAL_MAGIC), g_WeaponSpecialValue[sp_idx][special_level-1]);
						break;
					case SPECIAL_ATT_SPEED:
						//-- IDS_SPECIAL_INC_SPEED : 마법속도 %d%% 상승
						sprintf(strTemp, G_STRING(IDS_SPECIAL_INC_SPEED), g_WeaponSpecialValue[sp_idx][special_level-1]);
						break;

					default:
						*strTemp = '\0';
						break;
					}
					multiText.AddString(strTemp, SPECIAL_DESCR_COLOR);
				}
			}
		}
		for (int sp_idx = SPECIAL_STAFF_NUM; sp_idx < SPECIAL_WEAPON_NUM; sp_idx++)
		{
			int special_level = (special & g_WeaponSpecialMask[sp_idx]) >> g_WeaponSpecialShift[sp_idx];
			if (special_level > 0)
			{
				switch (sp_idx)
				{
				case SPECIAL_REDUCE_DEX_INT:
					if (item_shape != IWEAPON_STAFF)
					{
						//-- IDS_SPECIAL_DEX_DEC : 필요기민성 %d 감소
						sprintf(strTemp, G_STRING(IDS_SPECIAL_DEX_DEC), g_WeaponSpecialValue[sp_idx][special_level-1]);
					}
					else
					{
						//-- IDS_SPECIAL_INT_DEC : 필요지혜 %d 감소
						sprintf(strTemp, G_STRING(IDS_SPECIAL_INT_DEC), g_WeaponSpecialValue[sp_idx][special_level-1]);
					}
					multiText.AddString(strTemp, SPECIAL_DESCR_COLOR);
					break;
				case SPECIAL_NO_DEX_INT:
					if (item_shape == IWEAPON_STAFF || item_shape == IWEAPON_RING)
					{
						//-- IDS_SPECIAL_DEX_NOT_EXIST :필요기민성 없음
						sprintf(strTemp, G_STRING(IDS_SPECIAL_INT_NOT_EXIST));
					}
					else
					{
						//-- IDS_SPECIAL_INT_NOT_EXIST : 필요지혜 없음
						sprintf(strTemp, G_STRING(IDS_SPECIAL_DEX_NOT_EXIST));
					}
					multiText.AddString(strTemp, SPECIAL_DESCR_COLOR);
					break;
				case SPECIAL_REDUCE_STR:
					//-- IDS_SPECIAL_POWER_DEC : 요구힘 %d 감소
					sprintf(strTemp, G_STRING(IDS_SPECIAL_POWER_DEC), g_WeaponSpecialValue[sp_idx][special_level-1]);
					multiText.AddString(strTemp, SPECIAL_DESCR_COLOR);
					break;
				case SPECIAL_NO_STR:
					//-- IDS_SPECIAL_POWER_NOT_EXIST : 요구힘 없음
					sprintf(strTemp, G_STRING(IDS_SPECIAL_POWER_NOT_EXIST));
					multiText.AddString(strTemp, SPECIAL_DESCR_COLOR);
					break;
				case SPECIAL_REDUCE_LEV:
					//-- IDS_SPECIAL_LEVEL_DEC : 요구레벨 %d 감소
					sprintf(strTemp, G_STRING(IDS_SPECIAL_LEVEL_DEC), g_WeaponSpecialValue[sp_idx][special_level-1]);
					multiText.AddString(strTemp, SPECIAL_DESCR_COLOR);
					break;
				case SPECIAL_NO_LEV:
					//-- IDS_SPECIAL_LEVEL_NOT_EXIST : 요구레벨 없음
					sprintf(strTemp, G_STRING(IDS_SPECIAL_LEVEL_NOT_EXIST));
					multiText.AddString(strTemp, SPECIAL_DESCR_COLOR);
					break;
				case SPECIAL_SPDAM_ALL:
					//-- IDS_SPECIAL_INC_SPECIAL_DAM : 특수데미지 %d 상승
					sprintf(strTemp, G_STRING(IDS_SPECIAL_INC_SPECIAL_DAM), g_WeaponSpecialValue[sp_idx][special_level-1]);
					multiText.AddString(strTemp, SPECIAL_DESCR_COLOR);
					break;
				case SPECIAL_IGNORE_AVOID:
					//-- SPECIAL_IGNORE_AVOID : 상대 회피율 감소 %d%%
					sprintf(strTemp, G_STRING(IDS_SPECIAL_IGNORE_AVOID), g_WeaponSpecialValue[sp_idx][special_level-1]);
					multiText.AddString(strTemp, SPECIAL_DESCR_COLOR);
					break;

				default:
					*strTemp = '\0';
					break;
				}
			}
			int special_level2 = (special2 & g_WeaponSpecialMask[sp_idx]) >> g_WeaponSpecialShift[sp_idx];
			if (special_level2 > 0)
			{
				{
					*strTemp = '\0';
				}
			}
		}
	}
	//--------------------------------------------------------------------------
	//-- Armor( 갑옷 이란 말이다)
	else if (item_type == ITYPE_ARMOR)
	{
		if (item_shape <= IARMOR_SHIELD)
		{
			for (int sp_idx = 0; sp_idx < SPECIAL_ARMOR_NUM; sp_idx++)
			{
				int special_level = (special & g_ArmorSpecialMask[sp_idx]) >> g_ArmorSpecialShift[sp_idx];
				if (special_level > 0)
				{
					switch (sp_idx)
					{
					/*case SPECIAL_RESIST_MAGIC:
					sprintf(strTemp, ids_all_magic_sheld_inc, g_ArmorSpecialValue[sp_idx][special_level-1]);
					break;
					case SPECIAL_RESIST_LIGHTNING:
					sprintf(strTemp, ids_electric_magic_sheld_inc, g_ArmorSpecialValue[sp_idx][special_level-1]);
					break;
					case SPECIAL_RESIST_FIRE:
					sprintf(strTemp, ids_fire_magic_sheld_inc, g_ArmorSpecialValue[sp_idx][special_level-1]);
					break;
					case SPECIAL_RESIST_STONE:
					sprintf(strTemp, ids_stone_magic_sheld_inc, g_ArmorSpecialValue[sp_idx][special_level-1]);
					break;
					case SPECIAL_DEFENSE_LONG:
					sprintf(strTemp, ids_fal_sheld_inc, g_ArmorSpecialValue[sp_idx][special_level-1]);
					break;
					case SPECIAL_DEFENSE_SHORT:
					sprintf(strTemp, IDS_SPECIAL_NEAR_SHELD_INC, g_ArmorSpecialValue[sp_idx][special_level-1]);
					break;
					case SPECIAL_DEFENSE_MELEE:
					sprintf(strTemp, IDS_SPECIAL_PHYSICAL_SHELD_INC, g_ArmorSpecialValue[sp_idx][special_level-1]);
					  break;*/
					case SPECIAL_DEFENSE_MELEE:
						//-- IDS_SPECIAL_PHYSICAL_SHELD_INC : 물리적 방어력 %d 상승
						sprintf(strTemp, G_STRING(IDS_SPECIAL_PHYSICAL_SHELD_INC), g_ArmorSpecialValue[sp_idx][special_level-1]);
						break;
					case SPECIAL_RESIST_MAGIC:
						//-- IDS_SPECIAL_ALL_MAGIC_SHELD_INC : 전체 마법방어력 %d 상승
						sprintf(strTemp, G_STRING(IDS_SPECIAL_ALL_MAGIC_SHELD_INC), g_ArmorSpecialValue[sp_idx][special_level-1]);
						break;
					case SPECIAL_DEFENSE_ALL:
						//-- IDS_SPECIAL_INC_DEF : 방어력 %d 상승
						sprintf(strTemp, G_STRING(IDS_SPECIAL_INC_DEF), g_ArmorSpecialValue[sp_idx][special_level-1]);
						break;
					case SPECIAL_DEFENSE_PER:
						//-- IDS_SPECIAL_INC_DEF2 : 방어력 %d%% 상승
						sprintf(strTemp, G_STRING(IDS_SPECIAL_INC_DEF2), g_ArmorSpecialValue[sp_idx][special_level-1]);
						break;
					case SPECIAL_SPDEFENSE_ALL:
						//-- IDS_SPECIAL_INC_SPECIAL_DEF : 특수방어력 %d 상승
						sprintf(strTemp, G_STRING(IDS_SPECIAL_INC_SPECIAL_DEF), g_ArmorSpecialValue[sp_idx][special_level-1]);
						break;
					case SPECIAL_FLY:
						//-- IDS_SPECIAL_TAKE : %s 장착
						sprintf(strTemp, G_STRING(IDS_SPECIAL_TAKE), GET_ITEM_NAME_VNUM(g_AirshipProto_VNUM[special_level-1]) );
						break;
					default:
						*strTemp = '\0';
						break;
					}
					multiText.AddString(strTemp, SPECIAL_DESCR_COLOR);
				}
			}
			for (int sp_idx = SPECIAL_REDUCE_STR; sp_idx < SPECIAL_WEAPON_NUM-1; sp_idx++)
			{
				int special_level = (special & g_WeaponSpecialMask[sp_idx]) >> g_WeaponSpecialShift[sp_idx];
				if (special_level > 0)
				{
					switch (sp_idx)
					{
					case SPECIAL_REDUCE_STR:
						//-- IDS_SPECIAL_POWER_DEC : 요구힘 %d 감소
						sprintf(strTemp, G_STRING(IDS_SPECIAL_POWER_DEC), g_WeaponSpecialValue[sp_idx][special_level-1]);
						break;
					case SPECIAL_NO_STR:
						//-- IDS_SPECIAL_POWER_NOT_EXIST : 요구힘 없음
						sprintf(strTemp, G_STRING(IDS_SPECIAL_POWER_NOT_EXIST));
						break;
					case SPECIAL_REDUCE_LEV:
						//-- IDS_SPECIAL_LEVEL_DEC : 요구레벨 %d 감소
						sprintf(strTemp, G_STRING(IDS_SPECIAL_LEVEL_DEC), g_WeaponSpecialValue[sp_idx][special_level-1]);
						break;
					case SPECIAL_NO_LEV:
						//-- IDS_SPECIAL_LEVEL_NOT_EXIST : 요구레벨 없음
						sprintf(strTemp, G_STRING(IDS_SPECIAL_LEVEL_NOT_EXIST));
						break;
					default:
						*strTemp = '\0';
						break;
					}
					multiText.AddString(strTemp, SPECIAL_DESCR_COLOR);
				}
			}
			for (int sp_idx2 = 0; sp_idx2 < SPECIAL_ARMOR_NUM2; sp_idx2++)
			{
				int special_level = (special2 & g_ArmorSpecialMask2[sp_idx2]) >> g_ArmorSpecialShift2[sp_idx2];
				if (special_level > 0)
				{
					switch (sp_idx2)
					{
					case SPECIAL_ARMOR_INC_HP:
						//-- IDS_SPECIAL_VITAL_INC : 생명력 %d상승
						sprintf(strTemp, G_STRING(IDS_SPECIAL_VITAL_INC), g_ArmorSpecialValue2[sp_idx2][special_level-1]);
						break;
					case SPECIAL_ARMOR_INC_MP:
						//-- IDS_SPECIAL_MANA_INC : 마법력 %d상승
						sprintf(strTemp, G_STRING(IDS_SPECIAL_MANA_INC), g_ArmorSpecialValue2[sp_idx2][special_level-1]);
						break;
					case SPECIAL_ARMOR_INC_EP:
						//-- IDS_SPECIAL_ELECTRIC_INC : 전기력 %d상승
						sprintf(strTemp, G_STRING(IDS_SPECIAL_ELECTRIC_INC), g_ArmorSpecialValue2[sp_idx2][special_level-1]);
						break;
					case SPECIAL_ARMOR_INC_ST:
						//-- IDS_SPECIAL_STA_INC : 스테미너 %d상승
						sprintf(strTemp, G_STRING(IDS_SPECIAL_STA_INC), g_ArmorSpecialValue2[sp_idx2][special_level-1]);
						break;
					case SPECIAL_ARMOR_INC_RECOVER:
						//-- //-- IDS_EVRB_OPT_RECOV : 회복력증가 %d%% 상승
						sprintf(strTemp, G_STRING(IDS_EVRB_OPT_RECOV),g_ArmorSpecialValue2[sp_idx2][special_level-1]);
						break;
					default:
						*strTemp = '\0';
						break;
					}
					multiText.AddString(strTemp, SPECIAL_DESCR_COLOR);
				}
			}
		}
		else if (item_shape == IARMOR_NECKLACE)
		{
			for (int sp_idx = 0; sp_idx < SPECIAL_NECKLACE_NUM; sp_idx++)
			{
				int special_level = (special & g_NecklaceSpecialMask[sp_idx]) >> g_NecklaceSpecialShift[sp_idx];
				if (special_level > 0)
				{
					switch (sp_idx)
					{
					case SPECIAL_NECK_INC_VITAL:
						//-- IDS_SPECIAL_VITAL_INC : 생명력 %d 상승
						sprintf(strTemp, G_STRING(IDS_SPECIAL_VITAL_INC), g_NecklaceSpecialValue[sp_idx][special_level-1]);
						break;
					case SPECIAL_NECK_INC_MANA:
						//-- IDS_SPECIAL_MANA_INC : 마법력 %d 상승
						sprintf(strTemp, G_STRING(IDS_SPECIAL_MANA_INC), g_NecklaceSpecialValue[sp_idx][special_level-1]);
						break;
					case SPECIAL_NECK_INC_STAM:
						//-- IDS_SPECIAL_STA_INC : 스테미너 %d 상승
						sprintf(strTemp, G_STRING(IDS_SPECIAL_STA_INC), g_NecklaceSpecialValue[sp_idx][special_level-1]);
						break;
					case SPECIAL_NECK_INC_EPOWER:
						//-- IDS_SPECIAL_ELECTRIC_INC : 전기력 %d 상승
						sprintf(strTemp, G_STRING(IDS_SPECIAL_ELECTRIC_INC), g_NecklaceSpecialValue[sp_idx][special_level-1]);
						break;
					case SPECIAL_NECK_INC_CHA:
						//-- IDS_SPECIAL_READERSHIP_INC : 통솔력 %d 상승
						sprintf(strTemp, G_STRING(IDS_SPECIAL_READERSHIP_INC), g_NecklaceSpecialValue[sp_idx][special_level-1]);
						break;
					case SPECIAL_NECK_INC_CON:
						//-- IDS_SPECIAL_CON_INC : 체질 %d 상승
						sprintf(strTemp, G_STRING(IDS_SPECIAL_CON_INC), g_NecklaceSpecialValue[sp_idx][special_level-1]);
						break;
					case SPECIAL_NECK_INC_DEX:
						//-- IDS_SPECIAL_DEX_INC : 기민성 %d 상승
						sprintf(strTemp, G_STRING(IDS_SPECIAL_DEX_INC), g_NecklaceSpecialValue[sp_idx][special_level-1]);
						break;
					case SPECIAL_NECK_INC_STR:
						//-- IDS_SPECIAL_POWER_INC : 힘 %d 상승
						sprintf(strTemp, G_STRING(IDS_SPECIAL_POWER_INC), g_NecklaceSpecialValue[sp_idx][special_level-1]);
						break;
					case SPECIAL_NECK_INC_INT:
						//-- IDS_SPECIAL_INT_INC : 지혜 %d 상승
						sprintf(strTemp, G_STRING(IDS_SPECIAL_INT_INC), g_NecklaceSpecialValue[sp_idx][special_level-1]);
						break;
					case SPECIAL_NECK_INC_BLOCK:
						//-- IDS_BLOCK_RATE : 블럭 확률: %d%%
						sprintf(strTemp, G_STRING(IDS_BLOCK_RATE), g_NecklaceSpecialValue[sp_idx][special_level-1]);
						break;

					default:
						*strTemp = '\0';
						break;
					}
					multiText.AddString(strTemp, SPECIAL_DESCR_COLOR);
				}
			}
		}
		//------------------------------------------------------------------------
		//-- 반지 (RING) String 출력
		//------------------------------------------------------------------------
		else if (item_shape == IARMOR_RING)
		{
			for (int sp_idx = 0; sp_idx < SPECIAL_RING_NUM; sp_idx++)
			{
				int special_level = (special & g_RingSpecialMask[sp_idx]) >> g_RingSpecialShift[sp_idx];
				if (special_level > 0)
				{
					switch (sp_idx)
					{
					case SPECIAL_INC_CHA:
						//-- IDS_SPECIAL_READERSHIP_INC : 통솔력 %d 상승
						sprintf(strTemp, G_STRING(IDS_SPECIAL_READERSHIP_INC), g_RingSpecialValue[sp_idx][special_level-1]);
						break;
					case SPECIAL_INC_CON:
						//-- IDS_SPECIAL_CON_INC : 체질 %d 상승
						sprintf(strTemp, G_STRING(IDS_SPECIAL_CON_INC), g_RingSpecialValue[sp_idx][special_level-1]);
						break;
					case SPECIAL_INC_DEX:
						//-- IDS_SPECIAL_DEX_INC : 기민성 %d 상승
						sprintf(strTemp, G_STRING(IDS_SPECIAL_DEX_INC), g_RingSpecialValue[sp_idx][special_level-1]);
						break;
					case SPECIAL_INC_STR:
						//-- IDS_SPECIAL_POWER_INC : 힘 %d 상승
						sprintf(strTemp, G_STRING(IDS_SPECIAL_POWER_INC), g_RingSpecialValue[sp_idx][special_level-1]);
						break;
					case SPECIAL_INC_INT:
						//-- IDS_SPECIAL_INT_INC : 지혜 %d 상승
						sprintf(strTemp, G_STRING(IDS_SPECIAL_INT_INC), g_RingSpecialValue[sp_idx][special_level-1]);
						break;
					case SPECIAL_INC_COUNT:
						//-- IDS_SPECIAL_INC_COUNT : 카운터 %d%% 상승
						sprintf(strTemp, G_STRING(IDS_SPECIAL_INC_COUNT), g_RingSpecialValue[sp_idx][special_level-1]);
						break;
					case SPECIAL_INC_CRITICAL:
						//-- IDS_SPECIAL_INC_CRITICAL : 크리티컬 %d%% 상승
						sprintf(strTemp, G_STRING(IDS_SPECIAL_INC_CRITICAL), g_RingSpecialValue[sp_idx][special_level-1]);
						break;
					case SPECIAL_INC_ACCUMUL:
						//-- IDS_SPECIAL_INC_ACCUMUL : 볼트저항 %d%% 상승
						sprintf(strTemp, G_STRING(IDS_SPECIAL_INC_ACCUMUL), g_RingSpecialValue[sp_idx][special_level-1]);
						break;
					case SPECIAL_INC_BOLCANIC:
						//-- IDS_SPECIAL_INC_BOLCANIC : 볼캐닉저항 %d%% 상승
						sprintf(strTemp, G_STRING(IDS_SPECIAL_INC_BOLCANIC), g_RingSpecialValue[sp_idx][special_level-1]);
						break;
					case SPECIAL_INC_STONE:
						//-- IDS_SPECIAL_INC_STONE : 석화저항 %d%% 상승
						sprintf(strTemp, G_STRING(IDS_SPECIAL_INC_STONE), g_RingSpecialValue[sp_idx][special_level-1]);
						break;
					case SPECIAL_INC_COLD:
						//-- IDS_SPECIAL_INC_COLD : 콜드저항 %d%% 상승
						sprintf(strTemp, G_STRING(IDS_SPECIAL_INC_COLD), g_RingSpecialValue[sp_idx][special_level-1]);
						break;
					case SPECIAL_INC_TOTAL:
						//-- IDS_SPECIAL_INC_TOTAL : 전체저항 %d%% 상승
						sprintf(strTemp, G_STRING(IDS_SPECIAL_INC_TOTAL), g_RingSpecialValue[sp_idx][special_level-1]);
						break;
					case SPECIAL_INC_SKILLTIME:
						//-- IDS_SPECIAL_INC_SKILLTIME : 스킬시간 %d%% 상승
						sprintf(strTemp, G_STRING(IDS_SPECIAL_INC_SKILLTIME), g_RingSpecialValue[sp_idx][special_level-1]);
						break;
					case SPECIAL_INC_RECOVER:
						//-- IDS_SPECIAL_INC_RECOVER : 물약회복률 %d%% 상승
						sprintf(strTemp, G_STRING(IDS_SPECIAL_INC_RECOVER), g_RingSpecialValue[sp_idx][special_level-1]);
						break;
					case SPECIAL_INC_ABS_DAMAGE:
						//-- IDS_SPECIAL_INC_ABS_DAMAGE : 데미지 흡수 %d%% 상승
						sprintf(strTemp, G_STRING(IDS_SPECIAL_INC_ABS_DAMAGE), g_RingSpecialValue[sp_idx][special_level-1]);
						break;
					case SPECIAL_INC_AVOIDANCE:
						//-- IDS_SPECIAL_INC_AVOIDANCE : 회피율 %d%% 상승
						sprintf(strTemp, G_STRING(IDS_SPECIAL_INC_AVOIDANCE), g_RingSpecialValue[sp_idx][special_level-1]);
						break;


					default:
						*strTemp = '\0';
						break;
					}
					multiText.AddString(strTemp, SPECIAL_DESCR_COLOR);
				}
			}
		}
		//------------------------------------------------------------------------
		//-- 팔찌 (BRACELET) String 출력
		//------------------------------------------------------------------------
		//-- 2004.10.14
		//-- Lyul / 추가.

		else if (item_shape == IARMOR_BRACELET)
		{
			for (int sp_idx = 0; sp_idx < SPECIAL_BRACELET_NUM; sp_idx++)
			{
				int special_level = (special & g_BraceletSpecialMask[sp_idx]) >> g_BraceletSpecialShift[sp_idx];
				if (special_level > 0)
				{
					switch (sp_idx)
					{
					case SPECIAL_MONEY:
						//-- IDS_SPECIAL_MONEY_INC : 라임 획득량 %d%% 상승
						sprintf(strTemp, G_STRING(IDS_SPECIAL_MONEY_INC), g_BraceletSpecialValue[sp_idx][special_level-1]);
						break;
					case SPECIAL_EXP:
						//-- IDS_SPECIAL_EXP_INC : 경험치 획득수치 %d%% 상승
						sprintf(strTemp, G_STRING(IDS_SPECIAL_EXP_INC), g_BraceletSpecialValue[sp_idx][special_level-1]);
						break;
					case SPECIAL_ITEM:
						//-- IDS_SPECIAL_ITEM_INC : 아이템 획득확률 %d%% 상승
						sprintf(strTemp, G_STRING(IDS_SPECIAL_ITEM_INC), g_BraceletSpecialValue[sp_idx][special_level-1]);
						break;
					case SPECIAL_REALLOC:
						//-- IDS_SPECIAL_REPOINT_ABLE : 포인트 재분배 1회 가능
						sprintf(strTemp, G_STRING(IDS_SPECIAL_REPOINT_ABLE));
						break;


					case SPECIAL_BR_INC_HP     :
						//-- IDS_EVRB_OPT_VITAL : 생명력 %d%% 상승
						sprintf(strTemp,G_STRING(IDS_EVRB_OPT_VITAL), g_BraceletSpecialValue[sp_idx][special_level-1]);
						break;
					case SPECIAL_BR_INC_MP     :
						//-- IDS_EVRB_OPT_MANA : 마법력 %d%% 상승
						sprintf(strTemp,G_STRING(IDS_EVRB_OPT_MANA), g_BraceletSpecialValue[sp_idx][special_level-1]);
						break;
					case SPECIAL_BR_INC_EP     :
						//-- IDS_EVRB_OPT_ELEC : 전기력 %d%% 상승
						sprintf(strTemp, G_STRING(IDS_EVRB_OPT_ELEC), g_BraceletSpecialValue[sp_idx][special_level-1]);
						break;
					case SPECIAL_BR_INC_ST     :
						//-- IDS_EVRB_OPT_STA : 스테미너 %d%% 상승
						sprintf(strTemp, G_STRING(IDS_EVRB_OPT_STA), g_BraceletSpecialValue[sp_idx][special_level-1]);
						break;
					//-- IDS_EVRB_OPT_REF : 반사데미지 %d%%
					case SPECIAL_BR_REF_DAM    :
						sprintf(strTemp, G_STRING(IDS_EVRB_OPT_REF), g_BraceletSpecialValue[sp_idx][special_level-1]);
						break;
					//-- IDS_EVRB_OPT_REF : 데미지 흡수 %d%% 상승
					case SPECIAL_BR_ABS_DAM    :
						sprintf(strTemp, G_STRING(IDS_SPECIAL_INC_ABS_DAMAGE), g_BraceletSpecialValue[sp_idx][special_level-1]);
						break;
					//-- IDS_SPECIAL_INC_TOTAL : 전체저항 %d%% 상승
					case SPECIAL_BR_RESIST     :
						sprintf(strTemp, G_STRING(IDS_SPECIAL_INC_TOTAL), g_BraceletSpecialValue[sp_idx][special_level-1]);
						break;
					//-- IDS_EVRB_OPT_CHOAS : 몬스터 혼돈확률 %d%% 상승
					case SPECIAL_BR_STUN       :
						sprintf(strTemp, G_STRING(IDS_EVRB_OPT_CHOAS), g_BraceletSpecialValue[sp_idx][special_level-1]);
						break;
					//-- IDS_INVEN_DEFANCE_MANA_RATE : 마법방어율 %d%% 상승
					case SPECIAL_BR_DEF_MAGIC  :
						sprintf(strTemp, G_STRING(IDS_INVEN_DEFANCE_MANA_RATE), g_BraceletSpecialValue[sp_idx][special_level-1]);
						break;
					//-- IDS_EVRB_OPT_RECOV : 회복력증가 %d%% 상승
					case SPECIAL_BR_RECOVER    :
						sprintf(strTemp, G_STRING(IDS_EVRB_OPT_RECOV), g_BraceletSpecialValue[sp_idx][special_level-1]);
						break;

					default:
						*strTemp = '\0';
						break;
					}
					multiText.AddString(strTemp, SPECIAL_DESCR_COLOR);
				}
			}
		}
	}
}



//----------------------------------------------------------------------------
//-- SetSpecialValue
//----------------------------------------------------------------------------
//-- Item 의 Special 값을 Setting 한다.
//-- 각 종류 별로 Special 에 의해 변화되는 기본 속성을 Setting 한다.

//-- 2004.10.18
//-- Lyul / 추가.
//-- 팔찌는 값 변형없이 속성적용
void SetSpecialValue(CItem *pItem, int item_type, int item_shape, int special, int special2)
{
	// 레벨제한 랜덤 보정치 옵션 적용.(special2에 있음)
	//if( (pItem->m_Special2 & SPECIAL2_MASK_LEV_REVISION) ) // 방어구 보정치가 있는지의 체크. (이 마스크는 부호비트를 뺀 4비트를 얻어온다.)
	//{
	//	if( (pItem->m_Special2 & SPECIAL2_MASK_LEV_REVISION_SIGN) ) // 부호 비트가 있으면...
	//		pItem->m_MinLevel -= pItem->m_MinLevel*((pItem->m_Special2&SPECIAL2_MASK_LEV_REVISION)>>5)*0.01f; // -퍼센트. 기본 방어력을 기준으로 계산해서 더해준다.
	//	else
	//		pItem->m_MinLevel += pItem->m_MinLevel*((pItem->m_Special2&SPECIAL2_MASK_LEV_REVISION)>>5)*0.01f; // +퍼센트. 기본 방어력을 기준으로 계산해서 더해준다.
	//}


	if (item_type == ITYPE_WEAPON)
	{
		// 무기인가 검사
		if (item_shape != IWEAPON_STAFF)   // 지팡이 말고...
		{
			for (int sp_idx = SPECIAL_STAFF_NUM-1; sp_idx >= 0; sp_idx--)
			{
				int special_level = (special & g_WeaponSpecialMask[sp_idx]) >> g_WeaponSpecialShift[sp_idx];
				if (special_level > 0)
				{
					switch (sp_idx)
					{
					case SPECIAL_DAM_MIN:
						pItem->m_Dam1 += g_WeaponSpecialValue[sp_idx][special_level-1];
						break;
					case SPECIAL_DAM_MAX:
						pItem->m_Dam2 += g_WeaponSpecialValue[sp_idx][special_level-1];
						break;
					case SPECIAL_DAM_ALL:
						pItem->m_Dam1 += g_WeaponSpecialValue[sp_idx][special_level-1];
						pItem->m_Dam2 += g_WeaponSpecialValue[sp_idx][special_level-1];
						break;
					case SPECIAL_DAM_PER:
						pItem->m_Dam1 += pItem->m_Dam1 * g_WeaponSpecialValue[sp_idx][special_level-1] / 100;
						pItem->m_Dam2 += pItem->m_Dam2 * g_WeaponSpecialValue[sp_idx][special_level-1] / 100;
						break;
					case SPECIAL_ATT_SPEED:
						pItem->m_Speed --;
						if (pItem->m_Speed < 0)
							pItem->m_Speed = 0;
						break;

					}
				}
			}
		}
		else     // 이건 지팡이
		{
			for (int sp_idx = SPECIAL_STAFF_NUM-1; sp_idx >= 0; sp_idx--)
			{
				/*int special_level = (special & g_StaffSpecialMask[sp_idx]) >> g_StaffSpecialShift[sp_idx];
				if (special_level > 0) {
				switch (sp_idx) {
				case SPECIAL_ADD_MAGIC:
								pItem->m_MagicDamBonus += g_StaffSpecialValue[sp_idx][special_level-1];
				      break;
				      case SPECIAL_MAGIC_SPEED:
				      pItem->m_MagicAttackSpeed += g_StaffSpecialValue[sp_idx][special_level-1];
				      break;
				      case SPECIAL_REDUCE_INT:
				      pItem->m_Int -= g_StaffSpecialValue[sp_idx][special_level-1];
				      if (pItem->m_Int < 0)
				      pItem->m_Int = 0;
				      break;
				      case SPECIAL_NO_INT:
				      pItem->m_Int = 0;
				      break;
				      }
				}*/
				int special_level = (special & g_WeaponSpecialMask[sp_idx]) >> g_WeaponSpecialShift[sp_idx];
				if (special_level > 0)
				{
					switch (sp_idx)
					{
					case SPECIAL_DAM_MIN:
						break;
					case SPECIAL_DAM_MAX:
						break;
					case SPECIAL_DAM_ALL:
						break;
					case SPECIAL_DAM_PER:
						break;
					case SPECIAL_ATT_SPEED:
						pItem->m_MagicAttackSpeed += g_WeaponSpecialValue[sp_idx][special_level-1];
						break;
					}
				}
			}
		}

		// 더하기니까 먼저 적용해준다.
		int special_level = (special & g_WeaponSpecialMask[SPECIAL_SPDAM_ALL]) >> g_WeaponSpecialShift[SPECIAL_SPDAM_ALL];
		if (special_level > 0)
		{
			pItem->m_Dam1 += g_WeaponSpecialValue[SPECIAL_SPDAM_ALL][special_level-1];
			pItem->m_Dam2 += g_WeaponSpecialValue[SPECIAL_SPDAM_ALL][special_level-1];
		}
		for (int sp_idx = SPECIAL_WEAPON_NUM-2; sp_idx >= SPECIAL_STAFF_NUM ; sp_idx--)
		{
			int special_level = (special & g_WeaponSpecialMask[sp_idx]) >> g_WeaponSpecialShift[sp_idx];
			if (special_level > 0)
			{
				switch (sp_idx)
				{
				case SPECIAL_REDUCE_DEX_INT:
					if (item_shape != IWEAPON_STAFF)
					{
						pItem->m_Dex -= g_WeaponSpecialValue[sp_idx][special_level-1];
						if (pItem->m_Dex < 0)		// 아이템의 기민값
							pItem->m_Dex = 0;
					}
					else
					{
						pItem->m_Int -= g_WeaponSpecialValue[sp_idx][special_level-1];
						if (pItem->m_Int < 0)		// 아이템의 지예값
							pItem->m_Int = 0;
					}
					break;
				case SPECIAL_NO_DEX_INT:			// 아이템이 지혜와 기민이 필요없는 아이템
					if (item_shape == IWEAPON_STAFF || item_shape == IWEAPON_RING)
					{
						pItem->m_Int = 0;
					}
					else
					{
						pItem->m_Dex = 0;
					}
					break;
				case SPECIAL_REDUCE_STR:
					pItem->m_Weight -= g_WeaponSpecialValue[sp_idx][special_level-1];
					if (pItem->m_Weight < 0)
						pItem->m_Weight = 0;
					break;
				case SPECIAL_NO_STR:
					pItem->m_Weight = 0;
					break;
				case SPECIAL_REDUCE_LEV:
					pItem->m_MinLevel -= g_WeaponSpecialValue[sp_idx][special_level-1];
					if (pItem->m_MinLevel < 0)
						pItem->m_MinLevel = 0;
					break;
				case SPECIAL_NO_LEV:
					pItem->m_MinLevel = 0;
					break;
				}
			}


		}
	}
	else if (item_type == ITYPE_ARMOR)  	// 갑옷
	{

		pItem->m_Num9 = pItem->m_Num1;//임시저장 1023

		// 반지랑 팔찌도 타입을 체크.(2005에서 새로이 추가된 해머와 이도류가 item_shape에서 중복된다.)(by 원석)
		// 반지랑 팔찌가 위에서 체크하던걸 옮김.
		//-- 반지
		if (item_shape == IARMOR_RING)
		{
			pItem->m_Special = special;
			return;
		}
		//-- 팔찌
		if (item_shape == IARMOR_BRACELET)
		{
			pItem->m_Special = special;
			return;
		}
		//-- 목걸이
		if (item_shape == IARMOR_NECKLACE) // 목걸이도 옵션값만 셋팅하고 탈출 시킨다.(06-04-01 원석)
		{
			pItem->m_Special = special;
			return;
		}

		if (item_shape <= IARMOR_SHIELD)
		{
			// 이것도 더하기니까 먼저 적용해준다.
			int special_level = (special & g_ArmorSpecialMask[SPECIAL_SPDEFENSE_ALL]) >> g_ArmorSpecialShift[SPECIAL_SPDEFENSE_ALL];
			if (special_level > 0)
			{
				pItem->m_Defense += g_ArmorSpecialValue[SPECIAL_SPDEFENSE_ALL][special_level-1];
			}
			for (int sp_idx = SPECIAL_ARMOR_NUM-1; sp_idx >= 0; sp_idx--)
			{
				int special_level = (special & g_ArmorSpecialMask[sp_idx]) >> g_ArmorSpecialShift[sp_idx];
				if (special_level > 0)
				{
					switch (sp_idx)
					{
					/*case SPECIAL_RESIST_MAGIC:
					pItem->m_MagicDefense += g_ArmorSpecialValue[sp_idx][special_level-1];
					break;
					case SPECIAL_RESIST_LIGHTNING:
					break;
					case SPECIAL_RESIST_FIRE:
					break;
					case SPECIAL_RESIST_STONE:
					break;
					case SPECIAL_DEFENSE_LONG:
					break;
					case SPECIAL_DEFENSE_SHORT:
					break;
					case SPECIAL_DEFENSE_MELEE:
					  break;*/
					case SPECIAL_DEFENSE_MELEE://마석
						break;
					case SPECIAL_RESIST_MAGIC:
						pItem->m_MagicDefense += g_ArmorSpecialValue[sp_idx][special_level-1];
						break;
					case SPECIAL_DEFENSE_ALL:
						pItem->m_Defense += g_ArmorSpecialValue[sp_idx][special_level-1];
						break;
					case SPECIAL_DEFENSE_PER:
						pItem->m_Defense += pItem->m_Defense * g_ArmorSpecialValue[sp_idx][special_level-1] / 100;
						break;
					}
				}
			}
		}
		for (int sp_idx = SPECIAL_REDUCE_STR; sp_idx < SPECIAL_WEAPON_NUM; sp_idx++)
		{
			int special_level = (special & g_WeaponSpecialMask[sp_idx]) >> g_WeaponSpecialShift[sp_idx];
			if (special_level > 0)
			{
				switch (sp_idx)
				{
				case SPECIAL_REDUCE_STR:
					pItem->m_Weight -= g_WeaponSpecialValue[sp_idx][special_level-1];
					if (pItem->m_Weight < 0)
						pItem->m_Weight = 0;
					break;
				case SPECIAL_NO_STR:
					pItem->m_Weight = 0;
					break;
				case SPECIAL_REDUCE_LEV:
					pItem->m_MinLevel -= g_WeaponSpecialValue[sp_idx][special_level-1];
					if (pItem->m_MinLevel < 0)
						pItem->m_MinLevel = 0;
					break;
				case SPECIAL_NO_LEV:
					pItem->m_MinLevel = 0;
					break;
				}
			}
		}
		pItem->m_Num1 = pItem->m_Num9;//다시 가져오기.1023
	}
}


// nate
void ResetItemValue(CItem *pItem)
// 아이템의 속성값을 셋팅한다.
// CItem 참고
{
	pItem->m_Num0 = GET_ITEM_NUM0(pItem);
	pItem->m_Num1 = GET_ITEM_NUM1(pItem);
	pItem->m_Num2 = GET_ITEM_NUM2(pItem);
	pItem->m_Num3 = GET_ITEM_NUM3(pItem);
	pItem->m_Num4 = GET_ITEM_NUM4(pItem);
	pItem->m_Num5 = GET_ITEM_NUM5(pItem);
	pItem->m_Num6 = GET_ITEM_NUM6(pItem);
	pItem->m_Num7 = GET_ITEM_NUM7(pItem);
	pItem->m_Num8 = GET_ITEM_NUM8(pItem);
	pItem->m_Num9 = GET_ITEM_NUM9(pItem);
	pItem->m_MinLevel = GET_ITEM_LEVEL(pItem);
	pItem->m_MinLevel_Org = pItem->m_MinLevel;
	pItem->m_Int = GET_ITEM_INT(pItem);		// 지혜
	pItem->m_Dex = GET_ITEM_DEX(pItem);		// 기민
	pItem->m_Weight = GET_WEIGHT(pItem);	// 아이템의 무게
}

BOOL IsOptionFull(CItem *pItem, int option_num, int special_level)
{
	if (!pItem)
		return FALSE;

	int special = pItem->m_Special;
	int special2 = pItem->m_Special2;
	int item_type = GET_TYPE(pItem);
	int item_shape = GET_SHAPE(pItem);

	if (item_type == ITYPE_WEAPON)
	{
		if (option_num < 0 || (option_num > SPECIAL_STAFF_NUM && option_num < SPECIAL_IGNORE_AVOID))
			return FALSE;

		if ( option_num == SPECIAL_DAMAGE_UP2 || option_num == SPECIAL_DEAD_HIT )
		{
			if ((special2 & g_WeaponSpecialMask[option_num]) == g_WeaponSpecialMask[option_num])
				return TRUE;
		}

		if ((special & g_WeaponSpecialMask[option_num]) == g_WeaponSpecialMask[option_num])
			return TRUE;
	}
	else if (item_type == ITYPE_ARMOR)
	{
		if (option_num < 0 || option_num >= SPECIAL_ARMOR_NUM)
			return FALSE;
		if (item_shape <= IARMOR_SHIELD)
		{
			if ((special & g_ArmorSpecialMask[option_num]) == g_ArmorSpecialMask[option_num])
				return TRUE;
		}
		if (item_shape == IARMOR_SHOES && option_num == SPECIAL_FLY)
		{
			if (special_level <= ((special & g_ArmorSpecialMask[SPECIAL_FLY]) >> g_ArmorSpecialShift[SPECIAL_FLY]))
			{
				return TRUE;
			}
		}
	}

	return FALSE;
}