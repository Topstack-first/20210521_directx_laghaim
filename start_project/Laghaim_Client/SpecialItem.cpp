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
	{1,	2,	5,	0,	0,	0,	0},	// �ּҵ��������
	{1,	2,	5,	0,	0,	0,	0}, // �ִ� ������ ���
	{1,	2,	5,	0,	0,	0,	0}, // ��ü ������ ���
	{2,	5,	10,	0,	0,	0,	0}, // ��ü ������ % ���
	{3,	6,	12,	0,	0,	0,	0}, // ����Ÿ(������ 20% �߰�) Ȯ��
	{3,	6,	12,	0,	0,	0,	0}, // ġ��Ÿ(������ 50% �߰�) Ȯ��
	{10,0,	0,	0,	0,	0,	0}, // ���� ���ݼӵ� ���(�Ѵܰ�)
	{3,	10,	25,	0,	0,	0,	0}, // ����� ����
	{1,	0,	0,	0,	0,	0,	0}, // ����� ����
	{3,	10,	25,	0,	0,	0,	0}, // ���� ����
	{1,	0,	0,	0,	0,	0,	0}, // ���� ����
	{3,	10,	25,	0,	0,	0,	0}, // ���� ����
	{1,	0,	0,	0,	0,	0,	0}, // ���� ����
	{4,	10,	20,	0,	0,	0,	0}, // Ư�� ������ ���
	{5, 10, 20,	0,	0,	0,	0},  // ��� ȸ���� ����
	{10, 15, 25,	0,	0,	0,	0},  // ������ ���
	{3, 7, 10,	0,	0,	0,	0}  // ���鸮 ��Ʈ
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
{1,	3,	7,	0,	0,	0,	0}, // ���� �迭 ���� ������ �߰�
{1,	3,	7,	0,	0,	0,	0}, // ȭ�� �迭 ���� ������ �߰�
{1,	3,	7,	0,	0,	0,	0}, // ���� �迭 ���� ������ �߰�
{1,	2,	5,	0,	0,	0,	0}, // ��ü ���� ������ �߰�
{5,	10,	15,	0,	0,	0,	0}, // ���� ���ݼӵ� ���
{3,	10,	25,	0,	0,	0,	0}, // ������ ����
{1,	0,	0,	0,	0,	0,	0} // ������ ����
};
int g_StaffSpecialMask[SPECIAL_STAFF_NUM] =
{ 0x3 , 0xC , 0x30 , 0xC0 , 0x300 , 0xC00 , 0x1000 };
int g_StaffSpecialShift[SPECIAL_STAFF_NUM] =
{ 0 , 2 , 4 , 6 , 8 , 10 , 12 };
*/

int g_ArmorSpecialValue[SPECIAL_ARMOR_NUM][SPECIAL_LEVEL_NUM] =
{
	/*
	{2,	5,	10,	0,	0,	0,	0}, // ��ü ���� ���� ���
	{3,	7,	15,	0,	0,	0,	0}, // ���ݰ迭 ���� ���� ���
	{3,	7,	15,	0,	0,	0,	0}, // ȭ���迭 ���� ���� ���
	{3,	7,	15,	0,	0,	0,	0}, // �ϼ��迭 ���� ���� ���
	{3,	7,	15,	0,	0,	0,	0}, // ������(��, ����) ���� ���
	{7,	15,	30,	0,	0,	0,	0}, // ������(Į, ����) ���� ���
	{5,	12,	25,	0,	0,	0,	0} // ������(Į, ����, ��) ���� ���
	  */
	{1,	2,	5,	0,	0,	0,	0}, // ������(Į, ����, ��) ���� ���
	{2,	5,	10,	0,	0,	0,	0}, // ��ü ���� ���� ���
	{1,	2,	5,	0,	0,	0,	0}, // ���� ���
	{5,	10,	15,	0,	0,	0,	0}, // ���� % ���
	{10,20,	30,	0,	0,	0,	0}, // Ư�� ���� % ���
	{872,873,874,0,	0,	0,	0}	// ����� ������ ��ȣ
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
//-- Evolution ����
//----------------------------------------------------------------------------
//-- 2004.10.14
//-- Lyul / �߰�.

//-- �� +Option �� 32bit(DWORD) �� �ȿ� Bit ������ ����ȴ�.
//-- ���� BitMsk ���� �����ϰ� BitShf ���� ������ �� ���� ��´�.
//-- �� Table �� DB�� �ִ� ������ ������ ���̸�
//-- DB ���� Network �� ���� �� ���� ���� ���� ���̴�.
//-- ���� DB���� ���� �Ǹ� Local ���� ���� �ؾ� �Ѵ�.
//-- SPECIAL_RING_NUM :  �Ӽ� ����
/*

HEX : 0x02FFFFFF
DEC : 50331647

//-----------------------------------------------------------------------------------------
weight : 8421 8421 8421 8421    8421 8421 8421 8421
bit    : 0000 0000 0000 0000  | 0000 0000 0000 0000
                                                 11 (0x0000003)        -- ����� ȹ�� ��ġ ���     shift 0
                                               1100 (0x000000c)        -- ����ġ ȹ�� ��ġ ���     shift 2
                                           111 0000 (0x0000070)        -- ������ ȹ�� Ȯ�� ���     shift 4
                                          1000 0000 (0x0000080)        -- ����Ʈ ��й� ����        shift 7

                                       11 0000 0000 (0x0000300)        -- ����� %d ���            shift 8
                                     1100 0000 0000 (0x0000C00)        -- ������ %d ���            shift 10
                                  11 0000 0000 0000 (0x0003000)        -- ����� %d ���            shift 12
                                1100 0000 0000 0000 (0x000C000)        -- ���׹̳� %d ���          shift 14
                          11    0000 0000 0000 0000 (0x0030000)        -- �ݻ絥���� %d ���        shift 16
                        1100    0000 0000 0000 0000 (0x00C0000)        -- ��������� %d ���        shift 18
                     11 0000    0000 0000 0000 0000 (0x0300000)        -- ��ü���׷� %d ���        shift 20
                   1100 0000    0000 0000 0000 0000 (0x0C00000)        -- ���� ȥ�� Ȯ�� %d ���  shift 22
                 1 0000 0000    0000 0000 0000 0000 (0x1000000)        -- ���� ���� %d ���       shift 24
                10 0000 0000    0000 0000 0000 0000 (0x2000000)        -- ȸ��������  %d ���       shift 25

*/
//-- Table BRACELET
int g_BraceletSpecialValue[SPECIAL_BRACELET_NUM][SPECIAL_LEVEL_NUM] =
{
	{2,	5,	10,	0,	0,	0,	0},  // ����� ȹ�� ��ġ ���
	{5,	10,	2,	0,	0,	0,	0},  // ����ġ ȹ�� ��ġ ���, 2, 5, 10���� ��ȹ�ڿ�û���� 5,10,2�� ����(06/04/04)
	{1,	2,	5,	10,	15,	30,	50}, // ������ ȹ�� Ȯ�� ���,
	{1,	0,	0,	0,	0,	0,	0},   // ����Ʈ��й� ����

	{3, 5, 7, 0, 0, 0, 0},       //-- �����
	{3, 5, 7, 0, 0, 0, 0},       //-- ������
	{3, 5, 7, 0, 0, 0, 0},       //-- �����
	{3, 5, 7, 0, 0, 0, 0},       //-- ���׹̳�

	{10, 15, 20, 0, 0, 0, 0},    //-- �ݻ� ������
	{5, 10, 15, 0, 0, 0, 0},     //-- ������ ���
	{10, 20, 30, 0, 0, 0, 0},    //-- ��ü ���׷�
	{10, 15, 20, 0, 0, 0, 0},    //-- ���� ȥ��

	{10, 0, 0, 0, 0, 0, 0},      //-- ���� ����
	{10, 0, 0, 0, 0, 0, 0}      //-- ����ȸ��
};


//-- Bit Mask ��
int g_BraceletSpecialMask[SPECIAL_BRACELET_NUM] =
{
	0x3,     0xc,     0x70,     0x80,    0x300,
	0xc00,   0x3000,  0xc000,   0x30000, 0xc0000,
	0x300000,0xc00000,0x1000000,0x2000000
};

//-- Bit Shift ��
int g_BraceletSpecialShift[SPECIAL_BRACELET_NUM] =
{ 0, 2, 4, 7, 8, 10, 12, 14, 16, 18, 20, 22, 24, 25};


//----------------------------------------------------------------------------
//-- ��� ����
//----------------------------------------------------------------------------
//-- �� +Option �� 32bit(DWORD) �� �ȿ� Bit ������ ����ȴ�.
//-- ���� BitMsk ���� �����ϰ� BitShf ���� ������ �� ���� ��´�.
//-- �� Table �� DB�� �ִ� ������ ������ ���̸�
//-- DB ���� Network �� ���� �� ���� ���� ���� ���̴�.
//-- ���� DB���� ���� �Ǹ� Local ���� ���� �ؾ� �Ѵ�.
//-- SPECIAL_RING_NUM :  �Ӽ� ����

/*
�� Skill �� 11 �ȵȴ� 10 �̳� 01 �� �� �ϳ�.

//-----------------------------------------------------------------------------------------
weight : 8421 8421 8421 8421    8421 8421 8421 8421
bit    : 0000 0000 0000 0000  | 0000 0000 0000 0000
                                                111 (0x07)        -- ��ַ�      shift 0
                                            11 1000 (0x38)        -- ü��        shift 3
                                        1 1100 0000 (0x1C0)       -- ��μ�      shift 6
                                     1110 0000 0000 (0xE00)       -- ��          shift 9
                                 111 0000 0000 0000 (0x7000)      -- ����        shift 12
                                1000 0000 0000 0000 (0x8000)      -- ī����      shift 15
                           1                        (0x10000)     -- ũ��Ƽ��    shift 16
                          10                        (0x20000)     -- ��������    shift 17
                         100                        (0x40000)     -- ��ĳ�� ���� shift 18
                        1000                        (0x80000)     -- ��ȭ����    shift 19
                      1 0000                        (0x100000)    -- �ݵ�����    shift 20
                     10 0000                        (0x200000)    -- ��ü����    shift 21
                   1100 0000                        (0xC00000)    -- ��ų�ð�    shift 22
                11 0000 0000                        (0x3000000)   -- ����ȸ��    shift 24
               100 0000 0000                        (0x4000000)   -- ���������  shift 26
              1000 0000 0000                        (0x8000000)   -- ȸ����      shift 27

*/

//-- Table
int g_RingSpecialValue[SPECIAL_RING_NUM][SPECIAL_LEVEL_NUM] =
{
	{2,	3,	5,	8,	12,	18,	27}, // ��ַ� ���
	{1,	2,	3,	5,	8,	12,	18}, // ü�� ���
	{1,	2,	3,	5,	8,	12,	18}, // ��μ� ���
	{1,	2,	3,	5,	8,	12,	18}, // �� ���
	{1,	2,	3,	5,	8,	12,	18} // ���� ���

	,{5, 0, 0, 0, 0, 0, 0},//ī����
	{5, 0, 0, 0, 0, 0, 0},//ũ��Ƽ��
	{5, 0, 0, 0, 0, 0, 0},//��������
	{5, 0, 0, 0, 0, 0, 0},//��ĳ������
	{5, 0, 0, 0, 0, 0, 0},//��ȭ����
	{5, 0, 0, 0, 0, 0, 0},//�ݵ�����
	{5, 0, 0, 0, 0, 0, 0},//��ü����
	{20, 40, 60, 0, 0, 0, 0},//��ų�ð�
	{10, 20, 30, 0, 0, 0, 0},//����ȸ��

	{5, 0, 0, 0, 0, 0, 0},//DAMAGE ���
	{5, 0, 0, 0, 0, 0, 0}//ȸ����
};


//-- Bit Mask ��
int g_RingSpecialMask[SPECIAL_RING_NUM] =
{
	0x7 , 0x38 , 0x1C0 , 0xE00 , 0x7000 ,
	0x8000 , 0x10000 , 0x20000 , 0x40000 , 0x80000 , 0x100000 , 0x200000 , 0xC00000 ,0X3000000,
	0x4000000, 0x8000000
};

//-- Bit Shift ��
//--
int g_RingSpecialShift[SPECIAL_RING_NUM] =
{
	0 , 3 , 6 , 9 , 12 ,
	15 , 16 , 17 , 18 , 19 , 20 , 21 , 22 , 24, 26, 27
};



int g_NecklaceSpecialValue[SPECIAL_NECKLACE_NUM][SPECIAL_LEVEL_NUM] =
{
	{10,	30,	50,	100,	150,	300,	500}, // ����� ���
	{10,	30,	50,	100,	150,	300,	500}, // ������ ���
	{10,	30,	50,	100,	150,	300,	500}, // ���׹̳� ���
	{10,	30,	50,	100,	150,	300,	500}, // ����� ���
	{4,	10,	20,	 30, 	 40,	 60,	100}, // ��� ���
	{2,	 5,	10,	 15,	 20,	 30,	 50}, // ü�� ���
	{2,	 5,	10,	 15,	 20,	 30,	 50}, // ��� ���
	{2,	 5,	10,	 15,	 20,	 30,	 50}, //  ��  ���
	{2,	 5,	10,	 15,	 20,	 30,	 50}, // ���� ���
	{20,	2,   3,   4,	  5,	 10,	 20} // Block �ɼ�

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
//-- Ư�� flag �� ���� ǥ��

//-- 04.03.16 - �߰�(����) �Ϸ�
//-- Lyul
//-- ������ ����� ȸ���� �߰�
//-- String �߰�
//-- String DLL ����
void GetSpecial(CMultiText &multiText, int item_type, int item_shape, int special, int special2)
{
	char strTemp[128];
	if (item_type == ITYPE_WEAPON)
	{
		//------------------------------------------------------------------------
		//-- ������ ���� �Ϲ� ����
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
						//-- IDS_SPECIAL_MIN_ATT_INC : �ּҰ��ݷ� %d ���
						sprintf(strTemp, G_STRING(IDS_SPECIAL_MIN_ATT_INC), g_WeaponSpecialValue[sp_idx][special_level-1]);
						break;
					case SPECIAL_DAM_MAX:
						//-- IDS_SPECIAL_MAX_ATT_INC : �ִ���ݷ� %d ���
						sprintf(strTemp, G_STRING(IDS_SPECIAL_MAX_ATT_INC) , g_WeaponSpecialValue[sp_idx][special_level-1]);
						break;
					case SPECIAL_DAM_ALL:
						//-- IDS_SPECIAL_ATT_INC : ���ݷ� %d ���
						sprintf(strTemp, G_STRING(IDS_SPECIAL_ATT_INC), g_WeaponSpecialValue[sp_idx][special_level-1]);
						break;
					case SPECIAL_DAM_PER:
						//-- IDS_SPECIAL_ATT_INC2 : ���ݷ� %d%% ���
						sprintf(strTemp, G_STRING(IDS_SPECIAL_ATT_INC2), g_WeaponSpecialValue[sp_idx][special_level-1]);
						break;
					case SPECIAL_HIT_1:
						//-- IDS_SPECIAL_HIT_ATT : ����Ÿ ���� %d%%
						sprintf(strTemp, G_STRING(IDS_SPECIAL_HIT_ATT), g_WeaponSpecialValue[sp_idx][special_level-1]);
						break;
					case SPECIAL_HIT_2:
						//-- IDS_SPECIAL_FATAL_ATT : ġ��Ÿ ���� %d%%
						sprintf(strTemp, G_STRING(IDS_SPECIAL_FATAL_ATT), g_WeaponSpecialValue[sp_idx][special_level-1]);
						break;
					case SPECIAL_ATT_SPEED:
						//-- IDS_SPECIAL_ATT_SPEED_INC : ���ݼӵ� ���
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
		//-- ������
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
						//-- IDS_SPECIAL_DOWN_MAGIC_DAM : ���������� ������ %d%% �϶�
						sprintf(strTemp, G_STRING(IDS_SPECIAL_DOWN_MAGIC_DAM), g_WeaponSpecialValue[sp_idx][special_level-1]);
						break;
					case SPECIAL_DAM_MAX:
						//-- IDS_SPECIAL_UP_MAGIC_DAM : ���������� ����� %d%% ���
						sprintf(strTemp, G_STRING(IDS_SPECIAL_UP_MAGIC_DAM), g_WeaponSpecialValue[sp_idx][special_level-1]);
						break;
					case SPECIAL_DAM_ALL:
						//-- IDS_SPECIAL_INC_BASEMAGIC_DAM : �⺻ ���������� %d ���
						sprintf(strTemp, G_STRING(IDS_SPECIAL_INC_BASEMAGIC_DAM), g_WeaponSpecialValue[sp_idx][special_level-1]);
						break;
					case SPECIAL_DAM_PER:
						//-- IDS_SPECIAL_INC_MAGIC_DAM : ���������� %d%% ���
						sprintf(strTemp, G_STRING(IDS_SPECIAL_INC_MAGIC_DAM), g_WeaponSpecialValue[sp_idx][special_level-1]);
						break;
					case SPECIAL_HIT_1:
						//-- IDS_SPECIAL_HIT_MAGIC : ���� ����Ÿ %d%%
						sprintf(strTemp, G_STRING(IDS_SPECIAL_HIT_MAGIC), g_WeaponSpecialValue[sp_idx][special_level-1]);
						break;
					case SPECIAL_HIT_2:
						//-- IDS_SPECIAL_FATAL_MAGIC : ���� ġ��Ÿ %d%%
						sprintf(strTemp, G_STRING(IDS_SPECIAL_FATAL_MAGIC), g_WeaponSpecialValue[sp_idx][special_level-1]);
						break;
					case SPECIAL_ATT_SPEED:
						//-- IDS_SPECIAL_INC_SPEED : �����ӵ� %d%% ���
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
						//-- IDS_SPECIAL_DEX_DEC : �ʿ��μ� %d ����
						sprintf(strTemp, G_STRING(IDS_SPECIAL_DEX_DEC), g_WeaponSpecialValue[sp_idx][special_level-1]);
					}
					else
					{
						//-- IDS_SPECIAL_INT_DEC : �ʿ����� %d ����
						sprintf(strTemp, G_STRING(IDS_SPECIAL_INT_DEC), g_WeaponSpecialValue[sp_idx][special_level-1]);
					}
					multiText.AddString(strTemp, SPECIAL_DESCR_COLOR);
					break;
				case SPECIAL_NO_DEX_INT:
					if (item_shape == IWEAPON_STAFF || item_shape == IWEAPON_RING)
					{
						//-- IDS_SPECIAL_DEX_NOT_EXIST :�ʿ��μ� ����
						sprintf(strTemp, G_STRING(IDS_SPECIAL_INT_NOT_EXIST));
					}
					else
					{
						//-- IDS_SPECIAL_INT_NOT_EXIST : �ʿ����� ����
						sprintf(strTemp, G_STRING(IDS_SPECIAL_DEX_NOT_EXIST));
					}
					multiText.AddString(strTemp, SPECIAL_DESCR_COLOR);
					break;
				case SPECIAL_REDUCE_STR:
					//-- IDS_SPECIAL_POWER_DEC : �䱸�� %d ����
					sprintf(strTemp, G_STRING(IDS_SPECIAL_POWER_DEC), g_WeaponSpecialValue[sp_idx][special_level-1]);
					multiText.AddString(strTemp, SPECIAL_DESCR_COLOR);
					break;
				case SPECIAL_NO_STR:
					//-- IDS_SPECIAL_POWER_NOT_EXIST : �䱸�� ����
					sprintf(strTemp, G_STRING(IDS_SPECIAL_POWER_NOT_EXIST));
					multiText.AddString(strTemp, SPECIAL_DESCR_COLOR);
					break;
				case SPECIAL_REDUCE_LEV:
					//-- IDS_SPECIAL_LEVEL_DEC : �䱸���� %d ����
					sprintf(strTemp, G_STRING(IDS_SPECIAL_LEVEL_DEC), g_WeaponSpecialValue[sp_idx][special_level-1]);
					multiText.AddString(strTemp, SPECIAL_DESCR_COLOR);
					break;
				case SPECIAL_NO_LEV:
					//-- IDS_SPECIAL_LEVEL_NOT_EXIST : �䱸���� ����
					sprintf(strTemp, G_STRING(IDS_SPECIAL_LEVEL_NOT_EXIST));
					multiText.AddString(strTemp, SPECIAL_DESCR_COLOR);
					break;
				case SPECIAL_SPDAM_ALL:
					//-- IDS_SPECIAL_INC_SPECIAL_DAM : Ư�������� %d ���
					sprintf(strTemp, G_STRING(IDS_SPECIAL_INC_SPECIAL_DAM), g_WeaponSpecialValue[sp_idx][special_level-1]);
					multiText.AddString(strTemp, SPECIAL_DESCR_COLOR);
					break;
				case SPECIAL_IGNORE_AVOID:
					//-- SPECIAL_IGNORE_AVOID : ��� ȸ���� ���� %d%%
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
	//-- Armor( ���� �̶� ���̴�)
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
						//-- IDS_SPECIAL_PHYSICAL_SHELD_INC : ������ ���� %d ���
						sprintf(strTemp, G_STRING(IDS_SPECIAL_PHYSICAL_SHELD_INC), g_ArmorSpecialValue[sp_idx][special_level-1]);
						break;
					case SPECIAL_RESIST_MAGIC:
						//-- IDS_SPECIAL_ALL_MAGIC_SHELD_INC : ��ü �������� %d ���
						sprintf(strTemp, G_STRING(IDS_SPECIAL_ALL_MAGIC_SHELD_INC), g_ArmorSpecialValue[sp_idx][special_level-1]);
						break;
					case SPECIAL_DEFENSE_ALL:
						//-- IDS_SPECIAL_INC_DEF : ���� %d ���
						sprintf(strTemp, G_STRING(IDS_SPECIAL_INC_DEF), g_ArmorSpecialValue[sp_idx][special_level-1]);
						break;
					case SPECIAL_DEFENSE_PER:
						//-- IDS_SPECIAL_INC_DEF2 : ���� %d%% ���
						sprintf(strTemp, G_STRING(IDS_SPECIAL_INC_DEF2), g_ArmorSpecialValue[sp_idx][special_level-1]);
						break;
					case SPECIAL_SPDEFENSE_ALL:
						//-- IDS_SPECIAL_INC_SPECIAL_DEF : Ư������ %d ���
						sprintf(strTemp, G_STRING(IDS_SPECIAL_INC_SPECIAL_DEF), g_ArmorSpecialValue[sp_idx][special_level-1]);
						break;
					case SPECIAL_FLY:
						//-- IDS_SPECIAL_TAKE : %s ����
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
						//-- IDS_SPECIAL_POWER_DEC : �䱸�� %d ����
						sprintf(strTemp, G_STRING(IDS_SPECIAL_POWER_DEC), g_WeaponSpecialValue[sp_idx][special_level-1]);
						break;
					case SPECIAL_NO_STR:
						//-- IDS_SPECIAL_POWER_NOT_EXIST : �䱸�� ����
						sprintf(strTemp, G_STRING(IDS_SPECIAL_POWER_NOT_EXIST));
						break;
					case SPECIAL_REDUCE_LEV:
						//-- IDS_SPECIAL_LEVEL_DEC : �䱸���� %d ����
						sprintf(strTemp, G_STRING(IDS_SPECIAL_LEVEL_DEC), g_WeaponSpecialValue[sp_idx][special_level-1]);
						break;
					case SPECIAL_NO_LEV:
						//-- IDS_SPECIAL_LEVEL_NOT_EXIST : �䱸���� ����
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
						//-- IDS_SPECIAL_VITAL_INC : ����� %d���
						sprintf(strTemp, G_STRING(IDS_SPECIAL_VITAL_INC), g_ArmorSpecialValue2[sp_idx2][special_level-1]);
						break;
					case SPECIAL_ARMOR_INC_MP:
						//-- IDS_SPECIAL_MANA_INC : ������ %d���
						sprintf(strTemp, G_STRING(IDS_SPECIAL_MANA_INC), g_ArmorSpecialValue2[sp_idx2][special_level-1]);
						break;
					case SPECIAL_ARMOR_INC_EP:
						//-- IDS_SPECIAL_ELECTRIC_INC : ����� %d���
						sprintf(strTemp, G_STRING(IDS_SPECIAL_ELECTRIC_INC), g_ArmorSpecialValue2[sp_idx2][special_level-1]);
						break;
					case SPECIAL_ARMOR_INC_ST:
						//-- IDS_SPECIAL_STA_INC : ���׹̳� %d���
						sprintf(strTemp, G_STRING(IDS_SPECIAL_STA_INC), g_ArmorSpecialValue2[sp_idx2][special_level-1]);
						break;
					case SPECIAL_ARMOR_INC_RECOVER:
						//-- //-- IDS_EVRB_OPT_RECOV : ȸ�������� %d%% ���
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
						//-- IDS_SPECIAL_VITAL_INC : ����� %d ���
						sprintf(strTemp, G_STRING(IDS_SPECIAL_VITAL_INC), g_NecklaceSpecialValue[sp_idx][special_level-1]);
						break;
					case SPECIAL_NECK_INC_MANA:
						//-- IDS_SPECIAL_MANA_INC : ������ %d ���
						sprintf(strTemp, G_STRING(IDS_SPECIAL_MANA_INC), g_NecklaceSpecialValue[sp_idx][special_level-1]);
						break;
					case SPECIAL_NECK_INC_STAM:
						//-- IDS_SPECIAL_STA_INC : ���׹̳� %d ���
						sprintf(strTemp, G_STRING(IDS_SPECIAL_STA_INC), g_NecklaceSpecialValue[sp_idx][special_level-1]);
						break;
					case SPECIAL_NECK_INC_EPOWER:
						//-- IDS_SPECIAL_ELECTRIC_INC : ����� %d ���
						sprintf(strTemp, G_STRING(IDS_SPECIAL_ELECTRIC_INC), g_NecklaceSpecialValue[sp_idx][special_level-1]);
						break;
					case SPECIAL_NECK_INC_CHA:
						//-- IDS_SPECIAL_READERSHIP_INC : ��ַ� %d ���
						sprintf(strTemp, G_STRING(IDS_SPECIAL_READERSHIP_INC), g_NecklaceSpecialValue[sp_idx][special_level-1]);
						break;
					case SPECIAL_NECK_INC_CON:
						//-- IDS_SPECIAL_CON_INC : ü�� %d ���
						sprintf(strTemp, G_STRING(IDS_SPECIAL_CON_INC), g_NecklaceSpecialValue[sp_idx][special_level-1]);
						break;
					case SPECIAL_NECK_INC_DEX:
						//-- IDS_SPECIAL_DEX_INC : ��μ� %d ���
						sprintf(strTemp, G_STRING(IDS_SPECIAL_DEX_INC), g_NecklaceSpecialValue[sp_idx][special_level-1]);
						break;
					case SPECIAL_NECK_INC_STR:
						//-- IDS_SPECIAL_POWER_INC : �� %d ���
						sprintf(strTemp, G_STRING(IDS_SPECIAL_POWER_INC), g_NecklaceSpecialValue[sp_idx][special_level-1]);
						break;
					case SPECIAL_NECK_INC_INT:
						//-- IDS_SPECIAL_INT_INC : ���� %d ���
						sprintf(strTemp, G_STRING(IDS_SPECIAL_INT_INC), g_NecklaceSpecialValue[sp_idx][special_level-1]);
						break;
					case SPECIAL_NECK_INC_BLOCK:
						//-- IDS_BLOCK_RATE : �� Ȯ��: %d%%
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
		//-- ���� (RING) String ���
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
						//-- IDS_SPECIAL_READERSHIP_INC : ��ַ� %d ���
						sprintf(strTemp, G_STRING(IDS_SPECIAL_READERSHIP_INC), g_RingSpecialValue[sp_idx][special_level-1]);
						break;
					case SPECIAL_INC_CON:
						//-- IDS_SPECIAL_CON_INC : ü�� %d ���
						sprintf(strTemp, G_STRING(IDS_SPECIAL_CON_INC), g_RingSpecialValue[sp_idx][special_level-1]);
						break;
					case SPECIAL_INC_DEX:
						//-- IDS_SPECIAL_DEX_INC : ��μ� %d ���
						sprintf(strTemp, G_STRING(IDS_SPECIAL_DEX_INC), g_RingSpecialValue[sp_idx][special_level-1]);
						break;
					case SPECIAL_INC_STR:
						//-- IDS_SPECIAL_POWER_INC : �� %d ���
						sprintf(strTemp, G_STRING(IDS_SPECIAL_POWER_INC), g_RingSpecialValue[sp_idx][special_level-1]);
						break;
					case SPECIAL_INC_INT:
						//-- IDS_SPECIAL_INT_INC : ���� %d ���
						sprintf(strTemp, G_STRING(IDS_SPECIAL_INT_INC), g_RingSpecialValue[sp_idx][special_level-1]);
						break;
					case SPECIAL_INC_COUNT:
						//-- IDS_SPECIAL_INC_COUNT : ī���� %d%% ���
						sprintf(strTemp, G_STRING(IDS_SPECIAL_INC_COUNT), g_RingSpecialValue[sp_idx][special_level-1]);
						break;
					case SPECIAL_INC_CRITICAL:
						//-- IDS_SPECIAL_INC_CRITICAL : ũ��Ƽ�� %d%% ���
						sprintf(strTemp, G_STRING(IDS_SPECIAL_INC_CRITICAL), g_RingSpecialValue[sp_idx][special_level-1]);
						break;
					case SPECIAL_INC_ACCUMUL:
						//-- IDS_SPECIAL_INC_ACCUMUL : ��Ʈ���� %d%% ���
						sprintf(strTemp, G_STRING(IDS_SPECIAL_INC_ACCUMUL), g_RingSpecialValue[sp_idx][special_level-1]);
						break;
					case SPECIAL_INC_BOLCANIC:
						//-- IDS_SPECIAL_INC_BOLCANIC : ��ĳ������ %d%% ���
						sprintf(strTemp, G_STRING(IDS_SPECIAL_INC_BOLCANIC), g_RingSpecialValue[sp_idx][special_level-1]);
						break;
					case SPECIAL_INC_STONE:
						//-- IDS_SPECIAL_INC_STONE : ��ȭ���� %d%% ���
						sprintf(strTemp, G_STRING(IDS_SPECIAL_INC_STONE), g_RingSpecialValue[sp_idx][special_level-1]);
						break;
					case SPECIAL_INC_COLD:
						//-- IDS_SPECIAL_INC_COLD : �ݵ����� %d%% ���
						sprintf(strTemp, G_STRING(IDS_SPECIAL_INC_COLD), g_RingSpecialValue[sp_idx][special_level-1]);
						break;
					case SPECIAL_INC_TOTAL:
						//-- IDS_SPECIAL_INC_TOTAL : ��ü���� %d%% ���
						sprintf(strTemp, G_STRING(IDS_SPECIAL_INC_TOTAL), g_RingSpecialValue[sp_idx][special_level-1]);
						break;
					case SPECIAL_INC_SKILLTIME:
						//-- IDS_SPECIAL_INC_SKILLTIME : ��ų�ð� %d%% ���
						sprintf(strTemp, G_STRING(IDS_SPECIAL_INC_SKILLTIME), g_RingSpecialValue[sp_idx][special_level-1]);
						break;
					case SPECIAL_INC_RECOVER:
						//-- IDS_SPECIAL_INC_RECOVER : ����ȸ���� %d%% ���
						sprintf(strTemp, G_STRING(IDS_SPECIAL_INC_RECOVER), g_RingSpecialValue[sp_idx][special_level-1]);
						break;
					case SPECIAL_INC_ABS_DAMAGE:
						//-- IDS_SPECIAL_INC_ABS_DAMAGE : ������ ��� %d%% ���
						sprintf(strTemp, G_STRING(IDS_SPECIAL_INC_ABS_DAMAGE), g_RingSpecialValue[sp_idx][special_level-1]);
						break;
					case SPECIAL_INC_AVOIDANCE:
						//-- IDS_SPECIAL_INC_AVOIDANCE : ȸ���� %d%% ���
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
		//-- ���� (BRACELET) String ���
		//------------------------------------------------------------------------
		//-- 2004.10.14
		//-- Lyul / �߰�.

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
						//-- IDS_SPECIAL_MONEY_INC : ���� ȹ�淮 %d%% ���
						sprintf(strTemp, G_STRING(IDS_SPECIAL_MONEY_INC), g_BraceletSpecialValue[sp_idx][special_level-1]);
						break;
					case SPECIAL_EXP:
						//-- IDS_SPECIAL_EXP_INC : ����ġ ȹ���ġ %d%% ���
						sprintf(strTemp, G_STRING(IDS_SPECIAL_EXP_INC), g_BraceletSpecialValue[sp_idx][special_level-1]);
						break;
					case SPECIAL_ITEM:
						//-- IDS_SPECIAL_ITEM_INC : ������ ȹ��Ȯ�� %d%% ���
						sprintf(strTemp, G_STRING(IDS_SPECIAL_ITEM_INC), g_BraceletSpecialValue[sp_idx][special_level-1]);
						break;
					case SPECIAL_REALLOC:
						//-- IDS_SPECIAL_REPOINT_ABLE : ����Ʈ ��й� 1ȸ ����
						sprintf(strTemp, G_STRING(IDS_SPECIAL_REPOINT_ABLE));
						break;


					case SPECIAL_BR_INC_HP     :
						//-- IDS_EVRB_OPT_VITAL : ����� %d%% ���
						sprintf(strTemp,G_STRING(IDS_EVRB_OPT_VITAL), g_BraceletSpecialValue[sp_idx][special_level-1]);
						break;
					case SPECIAL_BR_INC_MP     :
						//-- IDS_EVRB_OPT_MANA : ������ %d%% ���
						sprintf(strTemp,G_STRING(IDS_EVRB_OPT_MANA), g_BraceletSpecialValue[sp_idx][special_level-1]);
						break;
					case SPECIAL_BR_INC_EP     :
						//-- IDS_EVRB_OPT_ELEC : ����� %d%% ���
						sprintf(strTemp, G_STRING(IDS_EVRB_OPT_ELEC), g_BraceletSpecialValue[sp_idx][special_level-1]);
						break;
					case SPECIAL_BR_INC_ST     :
						//-- IDS_EVRB_OPT_STA : ���׹̳� %d%% ���
						sprintf(strTemp, G_STRING(IDS_EVRB_OPT_STA), g_BraceletSpecialValue[sp_idx][special_level-1]);
						break;
					//-- IDS_EVRB_OPT_REF : �ݻ絥���� %d%%
					case SPECIAL_BR_REF_DAM    :
						sprintf(strTemp, G_STRING(IDS_EVRB_OPT_REF), g_BraceletSpecialValue[sp_idx][special_level-1]);
						break;
					//-- IDS_EVRB_OPT_REF : ������ ��� %d%% ���
					case SPECIAL_BR_ABS_DAM    :
						sprintf(strTemp, G_STRING(IDS_SPECIAL_INC_ABS_DAMAGE), g_BraceletSpecialValue[sp_idx][special_level-1]);
						break;
					//-- IDS_SPECIAL_INC_TOTAL : ��ü���� %d%% ���
					case SPECIAL_BR_RESIST     :
						sprintf(strTemp, G_STRING(IDS_SPECIAL_INC_TOTAL), g_BraceletSpecialValue[sp_idx][special_level-1]);
						break;
					//-- IDS_EVRB_OPT_CHOAS : ���� ȥ��Ȯ�� %d%% ���
					case SPECIAL_BR_STUN       :
						sprintf(strTemp, G_STRING(IDS_EVRB_OPT_CHOAS), g_BraceletSpecialValue[sp_idx][special_level-1]);
						break;
					//-- IDS_INVEN_DEFANCE_MANA_RATE : ��������� %d%% ���
					case SPECIAL_BR_DEF_MAGIC  :
						sprintf(strTemp, G_STRING(IDS_INVEN_DEFANCE_MANA_RATE), g_BraceletSpecialValue[sp_idx][special_level-1]);
						break;
					//-- IDS_EVRB_OPT_RECOV : ȸ�������� %d%% ���
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
//-- Item �� Special ���� Setting �Ѵ�.
//-- �� ���� ���� Special �� ���� ��ȭ�Ǵ� �⺻ �Ӽ��� Setting �Ѵ�.

//-- 2004.10.18
//-- Lyul / �߰�.
//-- ����� �� �������� �Ӽ�����
void SetSpecialValue(CItem *pItem, int item_type, int item_shape, int special, int special2)
{
	// �������� ���� ����ġ �ɼ� ����.(special2�� ����)
	//if( (pItem->m_Special2 & SPECIAL2_MASK_LEV_REVISION) ) // �� ����ġ�� �ִ����� üũ. (�� ����ũ�� ��ȣ��Ʈ�� �� 4��Ʈ�� ���´�.)
	//{
	//	if( (pItem->m_Special2 & SPECIAL2_MASK_LEV_REVISION_SIGN) ) // ��ȣ ��Ʈ�� ������...
	//		pItem->m_MinLevel -= pItem->m_MinLevel*((pItem->m_Special2&SPECIAL2_MASK_LEV_REVISION)>>5)*0.01f; // -�ۼ�Ʈ. �⺻ ������ �������� ����ؼ� �����ش�.
	//	else
	//		pItem->m_MinLevel += pItem->m_MinLevel*((pItem->m_Special2&SPECIAL2_MASK_LEV_REVISION)>>5)*0.01f; // +�ۼ�Ʈ. �⺻ ������ �������� ����ؼ� �����ش�.
	//}


	if (item_type == ITYPE_WEAPON)
	{
		// �����ΰ� �˻�
		if (item_shape != IWEAPON_STAFF)   // ������ ����...
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
		else     // �̰� ������
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

		// ���ϱ�ϱ� ���� �������ش�.
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
						if (pItem->m_Dex < 0)		// �������� ��ΰ�
							pItem->m_Dex = 0;
					}
					else
					{
						pItem->m_Int -= g_WeaponSpecialValue[sp_idx][special_level-1];
						if (pItem->m_Int < 0)		// �������� ������
							pItem->m_Int = 0;
					}
					break;
				case SPECIAL_NO_DEX_INT:			// �������� ������ ����� �ʿ���� ������
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
	else if (item_type == ITYPE_ARMOR)  	// ����
	{

		pItem->m_Num9 = pItem->m_Num1;//�ӽ����� 1023

		// ������ ��� Ÿ���� üũ.(2005���� ������ �߰��� �ظӿ� �̵����� item_shape���� �ߺ��ȴ�.)(by ����)
		// ������ ��� ������ üũ�ϴ��� �ű�.
		//-- ����
		if (item_shape == IARMOR_RING)
		{
			pItem->m_Special = special;
			return;
		}
		//-- ����
		if (item_shape == IARMOR_BRACELET)
		{
			pItem->m_Special = special;
			return;
		}
		//-- �����
		if (item_shape == IARMOR_NECKLACE) // ����̵� �ɼǰ��� �����ϰ� Ż�� ��Ų��.(06-04-01 ����)
		{
			pItem->m_Special = special;
			return;
		}

		if (item_shape <= IARMOR_SHIELD)
		{
			// �̰͵� ���ϱ�ϱ� ���� �������ش�.
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
					case SPECIAL_DEFENSE_MELEE://����
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
		pItem->m_Num1 = pItem->m_Num9;//�ٽ� ��������.1023
	}
}


// nate
void ResetItemValue(CItem *pItem)
// �������� �Ӽ����� �����Ѵ�.
// CItem ����
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
	pItem->m_Int = GET_ITEM_INT(pItem);		// ����
	pItem->m_Dex = GET_ITEM_DEX(pItem);		// ���
	pItem->m_Weight = GET_WEIGHT(pItem);	// �������� ����
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