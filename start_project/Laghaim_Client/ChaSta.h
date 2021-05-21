#ifndef _CHA_STA_H_
#define _CHA_STA_H_

#include "Config.h"

// ĳ���� ���� ����
#define CHA_STAND			0
#define CHA_WALK			1
#define CHA_RUN				2
#define CHA_ATTACK			3
#define CHA_SWIM			4
#define CHA_SWIM_STAND		5
#define CHA_SOCIAL			6
#define CHA_DEAD			7
#define CHA_MAGIC			8
#define CHA_JUMP_ATTACK		9
#define CHA_ROTATE_ATTACK	10
#define CHA_THROW_SLAYER	11
#define CHA_THROW_CHAKRAM	12
#define CHA_HANDLE			13
#define CHA_MAGIC_MOVE		14
#define CHA_AIDIA_SUB		15
#define CHA_KAI_SUB			16
// Laghaim_2004
#define CHA_BULKAN_BOW		17 // ��ĭ ����
#define CHA_KAI_TOUCH		18 // ī�̸��� ��ġ
#define CHA_AIDIA_PIPE		19 // ���̵�� �Ǹ�
#define CHA_HUMAN_DEEP		20 // �޸� �� ����
#define CHA_HUMAN_SHORT		21 // �޸� �� ����
#define CHA_MAGIC_SOFT		63
#define CHA_MAGIC_HARD		64
// �߰���ų �߰�
#define CHA_HELLCRY_ATTACK	65
#define CHA_AIRRAID			66
//----------------------------------------------------------------------------
//-- ����ũ ���
#define CHA_F_DSWORD        67
#define CHA_F_DSLASH        68
#define CHA_F_ASSAULT       69
#define CHA_F_DASSAULT      70
#define CHA_F_DASH          71
#define CHA_F_VANISH        72
#define CHA_F_BASH          73
#define CHA_F_DSTONE        74
#define CHA_F_TIMECHARGE    75
#define CHA_F_REFLECTION    76
// nate 2005-06-03 : ������ ��ų ���� �߰�
#define CHA_B_BURNINGCRASH	77	// nate 2005-06-28 : ��ĭ - ����ũ����
#define CHA_C_HELLDOWN		78	// mungmae-2005/07/13 : ī�� ��ٿ� �غ� ����
#define CHA_C_HELLDOWN2		79	// mungmae-2005/07/13 : ī�� ��ٿ� ���� ����
#define CHA_H_AUTOMINE		80	// nate 2005-06-28 : �޸� - ���丶��
#define CHA_A_SUMMONSQUAD	81	// nate 2005-07-15 : ���̵�� - ���� ������
#define CHA_B_FLYINGCRASH	82	// ��ĭ - �ö��� ũ����
#define CHA_K_POISON		83	// nate 2005-11-08 : ī�� - ������
#define CHA_H_ELECTRICSHOT	84	// �޸� - �Ϸ�Ʈ�� ��
#define CHA_F_SHADOWSLASH	85	// ���� - ������ ������
#define CHA_F_ADRENALINE	86	// ���� - �Ƶ巹����
#define CHA_KNOCKBACK       87  // ���� - �˹� ���.(�ϴ��� ������ ���ִ� ����� ���) ///��
#define CHA_WORLDCUP_KICK   88  // mungmae 2006/05/29 : ������ ű ���
#define CHA_WORLDCUP_DANCE	89	// nate 2006-05-23 : ������ ��
#define CHA_F_DASHSLASH 	90	// ���� - �޺� ��ų [�뽬 + ���� ������] ///�����޺�
#define CHA_F_SLASHSHADOW 	91	// ���� - �޺� ��ų [�뽬������ + �����콽����] ///�����޺�
#define CHA_F_STONEBASH 	92	// ���� - �޺� ��ų [����ν��� + �转] ///�����޺�
#define CHA_F_BASHREFLECT 	93	// ���� - �޺� ��ų [����转 + ���÷���] ///�����޺�
#define CHA_SPIRITMODE		94  // mungmae 2006/09/01 : ���Ǹ� ���
#define CHA_B_WILDSMASH	    95 // ��ĭ - ���ϵ� ���Ž�
#define CHA_K_FLAMESPEAR	    96 // ī�� - ������ ���Ǿ�
#define CHA_H_RANDOMFIRE_ME	    97 // �޸� - ���� �ڽ����� ���϶� 
#define CHA_H_RANDOMFIRE_YOU	98 // �޸� - ���� �ٸ� ����� ���� ���϶�
#define CHA_H_RANDOMFIRE5	    99 // ����� ���� ���� �ʰ� �Ǿ���
#define CHA_A_SUMMONFIRE	    100 // ���̵�� - ���� ���̾�
#define CHA_F_SPACESLASH	    101 // ���� - �����̽� ������
#define CHA_F_ELECTRICHAMMER	102 // ���� - �Ϸ�Ʈ�δ� �ظ�
#define CHA_B_PROTECTION_SHIELD 103 // ��ĭ - �÷��ؼ� ���� 
#define CHA_K_ICEBOLT			104 // ī�� - ���̽� ��Ʈ
#define CHA_A_RESURRECTION		105 // ���� - ��������
#define CHA_F_TONADOHAMMER 106 // ���� - ����̵� �ظ�
#define CHA_F_MERCERNERYSLASH	107 // ���� - �Ӽųʸ� ������
#define CHA_DIE					108	// �׾�� ����� �ƴ� �׾� �ִ� ����.



// �۷� ��ų������ �ִϸ��̼� ��ŷ!!

// �⺻�׼��� �����¿� �����ϸ�
// ��ų������ �׼Ǹ� ������ �Ͽ� ����Ī���ش�. �Ѥ�; �迭�� �����غ��?
enum LHCha_Action_Type
{
	LHCha_Action__Perom_S01 = 121,		//
	LHCha_Action__Perom_S02,			//
	LHCha_Action__Perom_S03,			//
	LHCha_Action__Perom_S04,			//
	LHCha_Action__Perom_S05,			//
	LHCha_Action__Perom_S06,			//
	LHCha_Action__Perom_S07,			//
	LHCha_Action__Perom_S08,			//
	LHCha_Action__Perom_S09,			//
	LHCha_Action__Perom_S10,			//
	LHCha_Action__Perom_S11,			//
	LHCha_Action__Perom_S12,			//
	LHCha_Action__Perom_S13,			//
	LHCha_Action__Perom_S14,			//
	LHCha_Action__Perom_S15,			//
	LHCha_Action__Perom_S16,			//
	LHCha_Action__Perom_S17,			//
	LHCha_Action__Perom_S18,			//
	LHCha_Action__Perom_S19,			//
	LHCha_Action__Perom_S20,			//
	LHCha_Action__Perom_S21,			//
	LHCha_Action__Perom_S22,			//
	LHCha_Action__Perom_S23,			//
	LHCha_Action__Perom_S24,			//
	LHCha_Action__Perom_S25,			//
	LHCha_Action__Perom_S26,			//

	LHCha_Action__EndOfEnum
};


#define RIDE_IDLE				200
#define RIDE_WALK				201
#define RIDE_RUN				202
#define RIDE_DEAD				203

// ĳ���� Status ��ȣ
#define STATUS_VITAL		0
#define STATUS_MANA			1
#define STATUS_STAMINA		2
#define STATUS_EPOWER		3
#define STATUS_LEVEL		4
#define STATUS_PLAYING_TIME	5
#define STATUS_DAM			6
#define STATUS_ATT_SPEED	7
#define STATUS_DEFENSE		8
#define STATUS_SPEED		9
#define STATUS_MONEY		10
#define STATUS_EXP			11
#define STATUS_NEED_EXP		12
#define STATUS_LEVELUP_POINT	13
#define STATUS_STR			14
#define STATUS_INT			15
#define STATUS_DEX			16
#define STATUS_CON			17
#define STATUS_CHA			18
#define STATUS_PENALTY		19
#define STATUS_CONFIRMCODE	20  // ���丶�콺 üũ ���� �ڵ�. ��Ŀ�� ���̱� ���� status ��Ŷ���� ���´�.(by ����) ///�����Է½ý��ۺ���
#define STATUS_CASH			23 ///�������.
#define STATUS_FAME			24 //��ġ(�İ���)

#define STATUS_BUFF			25


// �ֿϵ��� status ��ȣ
#define PET_VITAL			0
#define PET_DAM_RANGE		1
#define PET_DAM_MELEE		2
#define PET_ARMOR_RANGE		3
#define PET_ARMOR_MELEE		4
#define PET_SPEED			5
#define PET_COMPOSE			6
#define PET_PICK			7
#define PET_LOYALTY			8
#define PET_HUNGER			9
#define PET_LEVEL           10
#define PET_EXP				11
#define PET_NEED_EXP        12
#define PET_MODE			13
#define PET_TIME			14
#define PET_BAG				15
#define PET_BAG_COUNT		16
#define PET_NEED_MONEY		17
#define PET_NECKLACE		18
#define PET_SKILL			19

// ���� ���� ����
#define MOB_STAND			0
#define MOB_MOVE			1
#define MOB_ATTACK			2
#define MOB_DEAD			3
#define MOB_KNOCK			4
#define MOB_EMPTY1			5	// [10/23/2007 Theodoric] npc 2���� ���� �߰��� ���� 	
#define MOB_EMPTY2			6	// [10/23/2007 Theodoric] npc 2���� ���� �߰��� ���� 
#define MOB_EMPTY3			7
//==================================
// nate 2004 - 11 : new race - bpdindex number
#define BPDIDX_STAND_PEACE		0
#define BPDIDX_STAND_HAND		1
#define BPDIDX_STAND_1HAND		2
#define BPDIDX_STAND_2SWORD		3
#define BPDIDX_STAND_2AXE		4
#define BPDIDX_STAND_2GUN		5
#define BPDIDX_WALK_PEACE		12
#define BPDIDX_WALK_HAND		13
#define BPDIDX_WALK_1HAND		14
#define BPDIDX_WALK_2SWORD		15
#define BPDIDX_WALK_2AXE		16
#define BPDIDX_WALK_2GUN		17
#define BPDIDX_RUN_PEACE		18
#define BPDIDX_RUN_HAND			19
#define BPDIDX_RUN_1HAND		20
#define BPDIDX_RUN_2SWORD		21
#define BPDIDX_RUN_2AXE			22
#define BPDIDX_RUN_2GUN			23
#define BPDIDX_SWIM_GO			24
#define BPDIDX_SWIM_STOP		25
#define BPDIDX_DIE				26
#define BPDIDX_ATT_1HAND		29
#define BPDIDX_ATT_2SWORD		32
#define BPDIDX_ATT_2AXE			35
#define BPDIDX_ATT_1GUN			38
#define BPDIDX_ATT_2GUN			41
#define BPDIDX_ATT_2GUN_A		44
#define BPDIDX_ATT_1GUN_NO		47
#define BPDIDX_ATT_2GUN_NO		50
#define BPDIDX_ATT_HAND_L		53
#define BPDIDX_ATT_HAND_R		54
#define BPDIDX_ATT_HAND_U		55
#define BPDIDX_ATT_FOOT			56
#define BPDIDX_STAND_1GUN		98
#define BPDIDX_WALK_1GUN		100
#define BPDIDX_RUN_1GUN			101
#define BPDIDX_FLY_PEACE		108
#define BPDIDX_FLY_HAND			109
#define BPDIDX_FLY_1HAND		110
#define BPDIDX_FLY_1GUN			111
#define BPDIDX_FLY_2SWORD		112
#define BPDIDX_FLY_2AXE			113
#define BPDIDX_FLY_2GUN			114
#define BPDIDX_ATT_DSWORD		123
#define BPDIDX_STAND_DSWORD		126
#define BPDIDX_WALK_DSWORD		127
#define BPDIDX_RUN_DSWORD		128
#define BPDIDX_SKILL_DSLASH		129
#define BPDIDX_SKILL_DASSAULT	130
#define BPDIDX_SKILL_DASH		131
#define BPDIDX_SKILL_BASH		132
#define BPDIDX_SKILL_DSTONE		133
#define BPDIDX_SKILL_TIMECHARGE	134
#define BPDIDX_SKILL_REFLECTION	135
#define BPDIDX_SKILL_ASSAULT	136
#define BPDIDX_HOLLYSTAFF		137	// nate 2005-06-03 : Ȧ��������
#define BPDIDX_SKILL_BURNING	138	// nate 2005-06-03 : ��ĭ ����ũ����
#define BPDIDX_SKILL_DOWNHELL1	144	// nate 2005-06-03 : ī�� �ٿ���
#define BPDIDX_SKILL_DOWNHELL2	145	// nate 2005-06-03 : ī�� �ٿ���
#define BPDIDX_SKILL_SUMMONSQ	146	// nate 2005-06-28 : ���̵��
#define BPDIDX_SKILL_AUTOMINE	147	// nate 2005-06-28 : �޸� ���丶��
#define BPDIDX_SKILL_FLYINGCRASH	148	// ��ĭ - �ö��� ũ����
#define BPDIDX_SKILL_POISON			149	// nate 2005-11-08 : ī�� - ������
#define BPDIDX_SKILL_ELECTRICSHOT	150	// �޸� - �Ϸ�Ʈ�� ��
#define BPDIDX_SKILL_SHADOWSLASH1	151	// ���� - ������ ������
#define BPDIDX_WORLDCUP_KICK		152 // nate 2006-05-23 : �౸�� ����
#define BPDIDX_WORLDCUP_DANCE		153	// nate 2006-05-23 : ������ ��
#define BPDIDX_SPIRITMODE		159	// mungmae 2006-08-31 : ���Ǹ�
#define BPDIDX_SKILL_WILDSMASH	    160 // ��ĭ - ���ϵ� ���Ž�
#define BPDIDX_SKILL_FLAMESPEAR	    161 // ī�� - ������ ���Ǿ�
#define BPDIDX_SKILL_SUMMONFIRE	    162 // ���̵�� - ���� ���̾�
#define BPDIDX_SKILL_RANDOMFIRE2	163 // �޸� - ����2��
#define BPDIDX_SKILL_RANDOMFIRE3	164 // �޸� - ����3��
#define BPDIDX_SKILL_RANDOMFIRE4	165 // �޸� - ����4��
#define BPDIDX_SKILL_SPACESLASH		166
#define BPDIDX_SKILL_ELECTRICHAMMER	167
#define BPDIDX_SKILL_PROTECTION_SHIELD  168 // ��ĭ - �÷��ؼ� ���� 
#define BPDIDX_SKILL_ICEBOLT			169 // ī�� - ���̽� ��Ʈ
#define BPDIDX_SKILL_RESURRECTION		170 // ���� - ��������
#define BPDIDX_SKILL_TONADOHAMMER		171 // ���� - ����̵� ��
#define BPDIDX_SKILL_MERCERNERY_SLASH	172 // ���� - �ӼŸ��� ����
#define BPDIDX_SKILL_RANDOMFIRE_YOU		173 // �޸� - �ٸ� ��������� ���϶�(�ѹ� �պ� �ִϸ��̼�)




// �⺻ ���� �ִϸ��̼ǹ�ȣ�� ���� ����!!!
// Ȥ �������� �ִϸ��̼��� �����³��� �ִٸ�...�۾����� üũ����
#define PEROM_BPDIDX_SKILL_No01		121
#define PEROM_BPDIDX_SKILL_No02		122
#define PEROM_BPDIDX_SKILL_No03		123
#define PEROM_BPDIDX_SKILL_No04		124
#define PEROM_BPDIDX_SKILL_No05		125
#define PEROM_BPDIDX_SKILL_No06		126
#define PEROM_BPDIDX_SKILL_No07		127
#define PEROM_BPDIDX_SKILL_No08		128
#define PEROM_BPDIDX_SKILL_No09		129
#define PEROM_BPDIDX_SKILL_No10		130
#define PEROM_BPDIDX_SKILL_No11		131
#define PEROM_BPDIDX_SKILL_No12		132
#define PEROM_BPDIDX_SKILL_No13		133
#define PEROM_BPDIDX_SKILL_No14		134
#define PEROM_BPDIDX_SKILL_No15		135
#define PEROM_BPDIDX_SKILL_No16		136
#define PEROM_BPDIDX_SKILL_No17		137
#define PEROM_BPDIDX_SKILL_No18		138
#define PEROM_BPDIDX_SKILL_No19		139
#define PEROM_BPDIDX_SKILL_No20		140
#define PEROM_BPDIDX_SKILL_No21		141
#define PEROM_BPDIDX_SKILL_No22		142
#define PEROM_BPDIDX_SKILL_No23		143
#define PEROM_BPDIDX_SKILL_No24		144
#define PEROM_BPDIDX_SKILL_No25		145
#define PEROM_BPDIDX_SKILL_No26		146



//==================================
#endif // _CHA_STA_H_