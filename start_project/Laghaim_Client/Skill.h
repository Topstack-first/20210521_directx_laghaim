#ifndef _SKILL_H_
#define _SKILL_H_

#include "Config.h"
#include <string>

class CSurface;
class CBasicButton;
class CTextOutBox;

class CSkill
{
public:
	CSkill();
	virtual ~CSkill();

	int		GetEnerge0(int skill_level);
	int		GetMoney( int nSkill_Level );	// nate 2005-11-08 : ������ - �䱸 ���� ���
	int		GetApp0(int skill_level, int nStep = 0, bool automine=false);
	int		GetApp1(int skill_level);
	int		GetManaShieldTime(int skill_level);
	int		GetManaShieldAddTime(int skill_level);
	int		GetProtectionShieldData(int skill_level);

	int		m_Index;	// ��ǻ� �ʿ� ����.

	std::string m_Name;
	std::string m_Descr;

	int		m_MinLevel;
	int		m_Energe0Min;	// �ּ� ����
	int		m_Energe0Max;	// �ִ� ����
	int		m_WhichEnergy;	// ���� ����ϴ� ������ Ÿ��.
	int		m_App0Min;		// �ּ� ������(ġ����, �ۼ�Ʈ ���)
	int		m_App0Max;		// �ִ� ������(ġ����, �ۼ�Ʈ ���)
	int		m_App1Min;		// �ּ� �Ÿ�()
	int		m_App1Max;		// �ִ� �Ÿ�()
	int		m_MaxLevel;
	BOOL	m_bLeft;
	BOOL	m_bRight;
	BOOL	m_bAttack;
	int		m_Kind;
	int		m_AttCount;
	int		m_MinLaim;	// nate 2005-11-08 : ������ - ��ų�� �ּ� �䱸 ����
	int		m_MaxLaim;	// nate 2005-11-08 : ������ - ��ų�� �ִ� �䱸 ����

	int		m_race;		// ���� �÷���

	int		m_cool_time;

	char * GetName();
	char * GetDesc();
};

extern CSkill * g_Skills;

class CRaceSkill
{
public:
	CRaceSkill();
	virtual ~CRaceSkill();

	int m_SkillNum;
	int *m_SkillIndex;
};

class CChaSkill
{
public:
	int GetCurrentSkill(int cur_skill);


public:
	CChaSkill();
	virtual ~CChaSkill();

	void	LoadRes(int w, int h);
	void	DeleteRes();
	void	Reset();
	void	Init(int race);
	void	SetSkillLevel(int skill_idx, int skill_level);
	BOOL	CanUseSkill(int skill_idx);
	int		GetLevelByIndex(int skill_idx);
	int     GetArrayIndex( int skill_idx ); // ��ų �ε����� ���� �迭 �ε����� ������ �Լ� ///�����޺� �� �߰�.

	int				m_SkillNum;
	int				*m_SkillIndex;
	int				*m_SkillLevel;
	BOOL			*m_bLeft;
	BOOL			*m_bRight;
	CBasicButton	*m_SkillIcon;
	CTextOutBox		*m_LevelText;

	RECT			*m_rcRSkill;
	RECT			*m_rcLSkill;

	void			UpdateRskill(int index, int x, int y);
	void			UpdateLskill(int index, int x, int y);
	bool			IsInRskill(int index, int x, int y);
	bool			IsInLskill(int index, int x, int y);
};

extern CChaSkill g_ChaSkill;

#define SKILL_KIND_AUTO		0
#define SKILL_KIND_TO		1
#define SKILL_KIND_FIELD	2
#define SKILL_KIND_ME		3

#define SKILL_ATTACK		0
#define SKILL_USE_MANA		1
#define SKILL_USE_STAMINA	2
#define SKILL_USE_EPOWER	3
#define SKILL_USE_VITAL		4
//====================================================================================
// nate 2005-06-07 : ĳ���� ��ų
#define SKILL_SWORD_MASTERY		1	// �˼��� ���� 
#define SKILL_AXE_MASTERY		2	// ������ ����
#define SKILL_FIRE_BALL			3   // ���̾
#define SKILL_LIGHTNING_BALL	4   // ����Ʈ�� ��
#define SKILL_LIGHTNING		    5   // ����Ʈ��
#define SKILL_HEAL_SELF			6	// ���̵�� - ��Ŀ����
#define SKILL_MANA_SHIELD		7   // ī�� - ���� �ǵ� 
#define SKILL_FIRE_MASTERY		8	// ī�� - ���̾���͸�
#define SKILL_PROTECTION		9	// ��ȣ ��ũ��
#define SKILL_FIRE_PILLAR	    10	// �ұ��
#define SKILL_STONE_MASTERY		11	// ī�� - ���渶���͸�
#define SKILL_PEBBLE_SHOT	    12  // ��� 
#define SKILL_FALL_STONE	    13	// ����
#define SKILL_LIGHTNING_MASTERY	14	// ī�� - ����Ʈ�׸Ž��͸�
#define SKILL_RECALL			15  // ����
#define SKILL_POWERUP_STAMINA	16	// �Ŀ������׹̳�
#define SKILL_IRON_SKIN			17	// ��ö�Ǹ�
#define SKILL_DROP_ATTACK		18	// �������	
#define SKILL_RECOVERY			19	// ȸ��������
#define SKILL_GUN_MASTERY 		20  // ��ݼ�
#define SKILL_WORK_GEM			21	// ����������
#define SKILL_TELEPORT			22	// �ڷ���Ʈ
#define SKILL_SUMMON_PARTY		23	// ��Ƽ��ȯ
#define SKILL_ENERGYBALL		24	// �����ȯ��
#define SKILL_HEAL_OTHER		25	// ����
#define SKILL_STAR_FLARE	    26  // ��Ÿ �÷���
#define SKILL_SHOCK_SPEAR	    27  // ��ũ ���Ǿ�
#define SKILL_STONE_LANCE	    28  // ���� ����
#define SKILL_MULTI_SHOT    	29  // ����
#define SKILL_ELECTRIC_SHIELD	30  // �Ϸ�Ʈ�� �ǵ�
#define SKILL_ROTATE_ATTACK		31	// ���� ����
#define SKILL_SAFETY_SHIELD		32	// ������Ƽ�ǵ�
#define SKILL_ANIMAL_SUMMON		33	// ������ȯ��
#define SKILL_PHENOMENON		34	// ���޳�
#define SKILL_SNIFING		    35  // ���ݼ�
#define SKILL_HIDE			    36  // �ẹ��
#define SKILL_IRON_MUSCLE		37	// ��ö����
#define SKILL_FURY_EXPLOSION	38	// �г�����
#define SKILL_INSIGHT			39	// �����Ǵ�
#define SKILL_THROW				40	// ������ (�����̾�)
#define SKILL_CRUSH				41	// �м�
#define SKILL_AIMING		    42  // ���ط�
#define SKILL_PIERCING	    	43  // ����� 
#define SKILL_HANDLE		    44  // ���ۼ�
#define SKILL_MAKING			45  // ������
#define SKILL_ABSORB_MAGIC		46  // �������
#define SKILL_CONCENTRATION		47	// ����
#define SKILL_THOUGHT_STORM		48  // ����� ��ǳ
#define SKILL_BOOK_READ	    	49  // ���� �ؼ�
#define SKILL_MAGIC_MOVE	    50  // �������� ( �ϵ� )
#define SKILL_DISTURBANCE		51	// ����
#define SKILL_REMOVE			52  // ����
#define SKILL_BLESSING			53	// ������ �ູ
#define SKILL_JOINT_LIFE		54	// �������
#define SKILL_CHARMING			55	// ��Ȥ�ǹٶ�
#define SKILL_SUBMISSION		56	// ����
#define SKILL_LIFE_LIGHT		57	// �����Ǻ�
#define SKILL_PEACEFUL			58	// ���
#define SKILL_FORCE_ATTACK		60	// ��ĭ_�������� (�˱�)
#define SKILL_FORCE_CONTROL		61	// ��ĭ_������Ʈ��
#define SKILL_BLOODY_MANA		62  // ī��_���𸶳� (2004)
#define SKILL_VANPIRE_TOUCH		63  // ī��_�����̾���ġ(2004)
#define SKILL_HELL_TOUCH		64  // ī��_����ġ(2004)
#define SKILL_DEEP_ATTACK		65  // ������
#define SKILL_CHAIN_ATTACK	 	66  // ü�ξ���
#define SKILL_MELODY_OF_DEF	  	67	// ���̵��_��ȣ������
#define SKILL_MELODY_OF_GUD		68	// ���̵��_��ȣ������
#define SKILL_PASSION_OF_MELODY	69	// ���̵��_�����ǿ���
#define SKILL_PASSION_OF_FIGHT	70	// ���̵��_����������
#define SKILL_HELL_CRY          71  // ��ĭ
#define SKILL_TORNADO_RUIN		72  // ����̵� ����(���߽�ų)
#define SKILL_SUMMON_PARTITION  73  // ���̵��	
#define SKILL_AIR_RAID          74  // ����̵�
#define SKILL_HELL_CRY_CONTROLL 75  // ��ũ������Ʈ��
#define SKILL_TORNADO_OPERATION 76	// ����̵� ���並
#define SKILL_AIR_RAID_CONCENT  77  // ����̵� ����Ʈ
#define SKILL_SUMMON_CONTROL	78  // �����Ǽ� ��Ʈ��
// P�� �нú�. A �� ��Ƽ��,E �� ��þƮ, C �� �ڵ� ����⸦ ���Ѵ�.
#define SKILLF_P_MASTER_DSWORD  79  // �̵��������͸�
#define SKILLF_P_REC_UP         80  // ȸ���� ����
#define SKILLF_P_HASTE          81  // ���̽�Ʈ	
#define SKILLF_A_DSLASH         82  // ���� ������
#define SKILLF_P_EVASION        83  // �̺��̼�	
#define SKILLF_E_BLEED          84  // ����
#define SKILLF_A_DASH           85  // �뽬
#define SKILLF_A_ASSAULT        86  // ���Ʈ	
#define SKILLF_C_DASSAULT       87  // ��� ���Ʈ (�ڵ� ���� ���)
#define SKILLF_E_CLOAKING       88  // ���̵�
#define SKILLF_P_MASET_HAMMER   89	// �ظӸ����͸�
#define SKILLF_P_VITAL_UP       90	// ����� ����
#define SKILLF_P_MASERT_HEAVY	91	// �غ� �����͸�
#define SKILLF_A_BASH           92	// �转
#define SKILLF_P_IRONSKIN       93	// ���̾�ŲF
#define SKILLF_A_DSTONE         94	// ����� ����
#define SKILLF_P_STONRINC       95	// ���� ��ũ����
#define SKILLF_A_TIMECHARGE     96	// Ÿ������
#define SKILLF_C_STONEBRUST     97	// ���� ����Ʈ ( �ڵ� ���� ��� )
#define SKILLF_E_REFLECTION     98	// ���÷���
#define SKILL_B_BURNING_CONTROL	99	// nate 2005-06-07 : ��ĭ - ���׹̳� �Ʒ�
#define SKILL_B_BURNINGCRASH	100	// nate 2005-06-07 : ��ĭ - ���� ũ����
#define SKILL_C_DOWN_CONTROL	101	// nate 2005-06-07 : ī�� - ���� ����
#define SKILL_C_HELLDOWN		102	// nate 2005-06-07 : ī�� - �ٿ���
#define SKILL_H_MINE_CONTROL	103	// nate 2005-06-15 : �޸� - ������
#define SKILL_H_AUTOMINE		104	// nate 2005-06-15 : �޸� - ���丶��
#define SKILL_A_SUMMONS_SQUAD	105	// nate 2005-06-15 : ���� - ��Ƽ ��ȯ
#define SKILL_A_LIFE_EXPANSION	106	// nate 2005-06-15 : ���� - ���� ����
#define SKILL_B_FLYINGCRASH		107	// ��ĭ - �ø��� ũ����
#define SKILL_K_POISON			108 // nate 2005-11-08 : ī�� - ������
#define SKILL_H_ELECTRICSHOT	109 // �޸� - �Ϸ�Ʈ����
#define SKILL_A_SUMMON_FLOWER	110 // ���̴پ� - �����ö��
#define SKILL_F_SHADOWSLASH		111	// ���� - �����콽����
#define SKILL_F_ADRENALINE		112	// ���� - �Ƶ巹����
#define SKILL_B_WILDSMASH	    113 // ��ĭ - ���ϵ� ���Ž�
#define SKILL_K_FLAMESPEAR	    114 // ī�� - ������ ���Ǿ�
#define SKILL_H_RANDOMFIRE	    115 // �޸� - ����
#define SKILL_A_SUMMONFIRE	    116 // ���̵�� - ���� ���̾�
#define SKILL_F_SPACESLASH	    117 // ���� - �����̽� ������
#define SKILL_F_ELECTRICHAMMER	118 // ���� - �Ϸ�Ʈ�δ� �ظ�
#define SKILL_B_PROTECTION_SHIELDS 119 // ��ĭ - �����ü� �ǵ� 
#define SKILL_K_ICEBOLT           120 // ī�� - ���̽� ��Ʈ
#define SKILL_H_FROZENSHOT        121 // �޸� - ������ ��
#define SKILL_A_RESURRECTION      122 // ���̵�� - ��������
#define SKILL_F_MERCERNERY_SLASH  123 // ���� - �Ӽųʸ� ������
#define SKILL_F_TORNADO_HAMMER    124 // ���� - ����̵� �ظ�


// �۷� ��ų
enum SKILL_PEROM
{
	PEROM_SKILL_S125 = 125 ,	// ���Ǿ� �����͸�(�нú�)
	PEROM_SKILL_S126 = 126 ,	// ��������(�нú�)
	PEROM_SKILL_S127 = 127 ,	// PL ����(��Ƽ��)
	PEROM_SKILL_S128 = 128 ,	// PL ������Ʈ(��Ƽ��)
	PEROM_SKILL_S129 = 129 ,	// PL ���̴�(��Ƽ��)
	PEROM_SKILL_S130 = 130 ,	// PL ��Ʈ����ũ(��Ƽ��)
	PEROM_SKILL_S131 = 131 ,	// PL �Ǿ(��Ƽ��)
	PEROM_SKILL_S132 = 132 ,	// PL ��(��Ƽ��)
	PEROM_SKILL_S133 = 133 ,	// PL ����(��Ƽ��)
	PEROM_SKILL_S134 = 134 ,	// PL �ʵ�(��Ƽ��)
	PEROM_SKILL_S135 = 135 ,	// PL �Ǿ ����(��Ƽ��)
	PEROM_SKILL_S136 = 136 ,	// PL ������(��Ƽ��)
	PEROM_SKILL_S137 = 137 ,	// PL ���� ��Ʈ����ũ(��Ƽ��)
	PEROM_SKILL_S138 = 138 ,	// PL ũ�ν�(��Ƽ��)
	PEROM_SKILL_S139 = 139 ,	// D �ý���(��Ÿ����)
	PEROM_SKILL_S140 = 140 ,	// D �����ε�(��Ÿ��Ƽ��)
	PEROM_SKILL_S141 = 141 ,	// D Ŭ����(��Ÿ��Ƽ��)
	PEROM_SKILL_S142 = 142 ,	// ��ü ���� - Ÿ��(�нú�)
	PEROM_SKILL_S143 = 143 ,	// ��ü ���� - ���(�нú�)
	PEROM_SKILL_S144 = 144 ,	// D Ŭ���� �����͸�(�нú�)
	PEROM_SKILL_S145 = 145 ,	// D �ý���-����(�нú�)
	PEROM_SKILL_S146 = 146 ,	// D �ý���-���(�нú�)
	PEROM_SKILL_S147 = 147 ,	// D �ý���-����(�нú�)
	PEROM_SKILL_S148 = 148 ,	// ũ��Ƽ�� ��Ʈ����ũ(����)
	PEROM_SKILL_S149 = 149 ,	// �ΰ�����(����)
	PEROM_SKILL_S150 = 150 ,	// D �ý���-�ʵ�(��Ÿ����)

	SKILL_B_PASSIVE = 151,
	SKILL_K_PASSIVE = 152,
	SKILL_A_PASSIVE = 153,
	SKILL_H_PASSIVE = 154,
	SKILL_F_PASSIVE = 155,
	SKILL_P_PASSIVE = 156,
};



#define MAX_SKILL	157


// Ȯ�� ��ų��(skill.dta�� ���� ������ ���� ����ϴ� ��ų ����)
#define SKILL_F_DASHSLASH		10001 // ���� - �޺���ų �뽬->�������� ///�����޺�
#define SKILL_F_SLASHSHADOW		10002 // ���� - �޺���ų �뽬������->�����콽���� ///�����޺�
#define SKILL_FAIRY				10003 // 345���� ���� ����Ʈ ��. ĳ���� ���� ����Ʈ�̱⶧���� k ��Ŷ�� ���Խ�Ų��. ///��
#define SKILL_KNOCKBACK			10004 // 345���� ������ Ư��ȿ�� ��ŷ ���� �˹�. mv_skill �� �ޱ⶧���� ���⼭ ó��.
#define SKILL_F_STONEBASH1		10005 // ���� - �޺���ų ����ν���->�转 ��� ���� ��Ŷ.///�����޺�
#define SKILL_F_STONEBASH2		10006 // ���� - �޺���ų ����ν���->�转 Ÿ�� ��Ŷ.///�����޺�
#define SKILL_F_BASHREFLECT1	10007 // ���� - �޺���ų ����转->���÷�Ʈ ��� ���� ��Ŷ. ///�����޺�
#define SKILL_F_BASHREFLECT2	10008 // ���� - �޺���ų ����转->���÷�Ʈ Ÿ�� ��Ŷ. ///�����޺�
#define SKILL_PET_LIFE			10009 // �� ������ ��ų "����" �����ð����� HP ȸ�� (����Ʈ�� ������ �ͽ����)
#define SKILL_KNOCKBACK2		10010 // ���Ǹ��� �ֺ� ���͵��� �˹�ó��.
#define SKILL_LUCKYPIG1			10011 // ���� ������.(�� �񽺹Ǹ��� ���.)
#define SKILL_LUCKYPIG2			10012 // ���� ������.(�� �񽺹Ǹ��� ���.)
#define SKILL_LUCKYPIG3			10013 // ���� ������.(�� �񽺹Ǹ��� ���.)
#define SKILL_LUCKYPIG4			10014 // ���� ������.(�� �񽺹Ǹ��� ���.)


// [7/31/2007 Theodoric] 6�ֳ� ��� �̺�Ʈ ���̺� �ҿ� (�� �񽺹Ǹ��� ���)
#define SKILL_BABYSOUL_1		10015	// ��ġ
#define SKILL_BABYSOUL_2		10016	// ��ġ
#define SKILL_BABYSOUL_3		10017	// ��ġ
#define SKILL_VAMPIREBAT		10018 ///�����̾���� ����
#define SKILL_PETRUDOLPH		10019 ///��絹��
#define SKILL_RING_LIGHT_WHITE	10020 // [12/4/2007 parkmj] �Ϻ� �������� ����Ʈ
#define SKILL_RING_LIGHT_DARK	10021 // [12/4/2007 parkmj] �Ϻ� �������� ����Ʈ
#define SKILL_RING_LIGHT_RED	10022 // [12/4/2007 parkmj] �Ϻ� �������� ����Ʈ
#define SKILL_RING_LIGHT_BLUE	10023 // [12/4/2007 parkmj] �Ϻ� �������� ����Ʈ
#define SKILL_RING_LIGHT_GREEN	10024 // [12/4/2007 parkmj] �Ϻ� �������� ����Ʈ
#define SKILL_PETRUDOLPH_BLACK	10025 // [12/10/2007 parkmj] ��絹��_����
#define SKILL_PETRUDOLPH_BLUE	10026 // [12/10/2007 parkmj] ��絹��_�Ķ�
#define SKILL_PETRUDOLPH_GREEN	10027 // [12/10/2007 parkmj] ��絹��_���
#define SKILL_PETRUDOLPH_YELLOW	10028 // [12/10/2007 parkmj] ��絹��_���
#define SKILL_PETRUDOLPH_KOR_PREMIUM 10029 // [2008/11/3 Theodoric] ���� �����̾� ��ǰ
#define SKILL_ARUPAKA_PET			10030 // ��ī����
#define SKILL_ARUPAKA_PET_BE		10031 // ��ī����
#define SKILL_ARUPAKA_PET_BR		10032 // ��ī����
#define	SKILL_FAIRY_HUMAN_SILVER	10033	// 91 152 255
#define	SKILL_FAIRY_HUMAN_BLACK		10034	// 242 207 117
#define	SKILL_FAIRY_KAI_BLUE		10035	// 240 210 156
#define	SKILL_FAIRY_KAI_RED			10036	// 236 151 142
#define	SKILL_FAIRY_HYB_SILVER		10037	// 242 221 140
#define	SKILL_FAIRY_HYB_GOLD		10038	// 218 201 220
#define	SKILL_FAIRY_AID_BLUE		10039	// 240 210 156
#define	SKILL_FAIRY_AID_GREEN		10040	// 117 239 230
#define	SKILL_FAIRY_BUL_SILVER		10041	// 221 148 111
#define	SKILL_FAIRY_BUL_VIOLET		10042	// 147 126 205
#define SKILL_RING_LIGHT_GOLD		10043	// ?


#define SKILL_PET_CATS_BLACK			10044
#define SKILL_PET_CATS_WHITE			10045
#define SKILL_PET_CATS_MIX			10046

#define SKILL_LV400_GREEN			10047
#define SKILL_LV400_BULE			10048
#define SKILL_LV400_RED				10049

#define SKILL_FAIRY_NECK1		10200
#define SKILL_FAIRY_NECK2		10201
#define SKILL_FAIRY_NECK3		10202
#define SKILL_FAIRY_NECK4		10203
#define SKILL_FAIRY_NECK5		10204

#define SKILL_LV400_GO				20072
#define SKILL_LV400_PI				20073
#define SKILL_LV400_ALL				20074
#define SKILL_LV400_ALL2			20075
#define SKILL_LV400_WH				20076
#define SKILL_LV400_BL				20077
#define SKILL_LV400_W01				20078
#define SKILL_LV400_W02				20079
#define SKILL_LV400_W03				20080
#define SKILL_LV400_W04				20081

#define	SKILL_ROYALE_1				20082	
#define	SKILL_ROYALE_2				20083
#define	SKILL_ROYALE_3				20084
#define	SKILL_ROYALE_4				20085
#define	SKILL_ROYALE_5				20086
#define	SKILL_ROYALE_6				20087
#define	SKILL_ROYALE_7				20088
#define	SKILL_ROYALE_8				20089
#define	SKILL_ROYALE_9				20090
#define	SKILL_ROYALE_10				20091
#define	SKILL_ROYALE_11				20092
#define	SKILL_ROYALE_12				20093
#define	SKILL_ROYALE_13				20094
#define	SKILL_ROYALE_14				20095
#define	SKILL_ROYALE_15				20096

#define SKILL_PET_ZORO_BLACK			10053
#define SKILL_PET_ZORO_WHITE			10054
#define SKILL_PET_ZORO_GREEN			10055

#define SKILL_PET_VALO_BLACK			10059
#define SKILL_PET_VALO_WHITE			10060
#define SKILL_PET_VALO_GREEN			10061

#define SKILL_PET_BEAR_BLACK			10065
#define SKILL_PET_BEAR_WHITE			10066

#define SKILL_PET_HORSE_BLUE			10067
#define SKILL_PET_HORSE_RED				10068

#define SKILL_PET_JEGUAR_1				10069
#define SKILL_PET_JEGUAR_2				10070
#define SKILL_PET_JEGUAR_3				10071

#define SKILL_PET_BONI_1				10072
#define SKILL_PET_BONI_2				10073
#define SKILL_PET_BONI_3				10074

#define SKILL_PET_AZUL					10078	//NEW PET PABLO
#define SKILL_PET_VERDE					10079
#define SKILL_PET_ROXO					10080
#define SKILL_PET_AMARELO				10088
#define SKILL_PET_VERMELHO				10089
#define SKILL_PET_CINZA					10090	//Altair
#define SKILL_PET_FESHIK				10091   //Feshik
#define SKILL_PET_ALTAIR_P				10092	//Altair premium
#define SKILL_PET_FESHIK_P				10093   //Feshik premium
#define SKILL_PET_EZULAN				10094  
#define SKILL_PET_THUNDERBIRD			10095 
#define SKILL_PET_THUNDERBIRD_P			10096  
#define SKILL_PET_EZULAN_P				10097
#define SKILL_PET_PETFAME1				10098
#define SKILL_PET_PETFAME2				10099
#define SKILL_PET_PETFAME3				10100
#define SKILL_PET_PETFAME4				10101
#define SKILL_PET_HELPER				10102
#define SKILL_PET_HALL1				10103
#define SKILL_PET_HALL2				10104
#define SKILL_PET_HALL3				10105
#define SKILL_PET_NATAL1				10106
#define SKILL_PET_NATAL2				10107
#define SKILL_PET_NATAL3				10108



#define SKILL_K_FLAMESPEAR_TARGET 10114 // �÷��� ���۾��� Ÿ�� ��Ŷ�������.
//====================================================================================

//==========================================================================
// nate 2005-06-07 : ��ų ����Ʈ ��ȣ
#define EFFECT_SKILL_FIRE_BALL			3   // ���̾
#define EFFECT_SKILL_LIGHTNING_BALL		4   // ����Ʈ�� ��
#define EFFECT_SKILL_LIGHTNING		    5   // ����Ʈ��
#define EFFECT_SKILL_HEAL_SELF			6	// ���̵�� - ��Ŀ����
#define EFFECT_SKILL_PROTECTION			9	// ��ȣ ��ũ��
#define EFFECT_SKILL_FIRE_PILLAR	    10	// �ұ��
#define EFFECT_SKILL_PEBBLE_SHOT	    12  // ��� 
#define EFFECT_SKILL_FALL_STONE			13	// ����
#define EFFECT_SKILL_ENERGYBALL			24
#define EFFECT_SKILL_HEAL_OTHER			25
#define EFFECT_SKILL_STAR_FLARE			26  // ��Ÿ �÷���
#define EFFECT_SKILL_SHOCK_SPEAR	    27  // ��ũ ���Ǿ�
#define EFFECT_SKILL_STONE_LANCE	    28  // ���� ����
#define EFFECT_SKILL_IRON_MUSCLE		37	// ��ö����
#define EFFECT_SKILL_FURY_EXPLOSION		38	// �г�����
#define EFFECT_SKILL_PIERCING	    	43  // ����� 
#define EFFECT_SKILL_THOUGHT_STORM		48  // ����� ��ǳ
#define EFFECT_SKILL_BLESSING			53	// ������ �ູ
#define EFFECT_SKILL_JOINT_LIFE			54	// �������
#define EFFECT_SKILL_LIFE_LIGHT			57	// �����Ǻ�
#define EFFECT_SKILL_PEACEFUL			58	// ���
#define EFFECT_SKILL_FORCE_ATTACK		60	// ��ĭ_�������� (�˱�)
#define EFFECT_SKILL_BLOODY_MANA		62  // ī��_���𸶳� (2004)
#define EFFECT_SKILL_VANPIRE_TOUCH		63  // ī��_�����̾���ġ(2004)
#define EFFECT_SKILL_HELL_TOUCH			64  // ī��_����ġ(2004)
#define EFFECT_SKILL_MELODY_OF_DEF	  	67	// ���̵��_��ȣ������
#define EFFECT_SKILL_MELODY_OF_GUD		68	// ���̵��_��ȣ������
#define EFFECT_SKILL_HELL_CRY			71  // ��ĭ - ��ũ����
#define EFFECT_SKILL_TORNADO_RUIN		72  // ī�� - ����̵� ����(���߽�ų)
#define EFFECT_SKILL_SUMMON_PARTITION	73  // ���̵��	- �����Ǽ�
#define EFFECT_SKILL_AIR_RAID			74  // �޸� - ����̵�
#define EFFECT_SKILL_SUMMONSQUAD		75	// nate 2005-07-08 : Summon Squad - Effect Num
#define EFFECT_SKILL_MINE				76	// nate 2005-06-15 : �޸� ��ų �߰� - Auto Mine
#define EFFECT_SKILL_BURNINGCRASH		77	// nate 2005-06-16 : ��ĭ ��ų �߰� - Burning Crash
#define EFFECT_SKILL_BURNINGCRASH2		78	// nate 2005-06-16 : ��ĭ ��ų �߰� - Burning Crash
#define EFFECT_SKILL_HELLDOWN			79	// mungmae-2005/07/04 : ī�̸��� ��ٿ�
#define EFFECT_SKILL_LIFEEXPENSION		80	// nate 2005-07-05 : ���̵�� ��ų - Life Expension
#define EFFECT_SKILL_FLYINGCRASH		81	// ��ĭ - �ö��� ũ����(���۽�)
#define EFFECT_SKILL_POISON				82	// nate 2005-11-03 : ī�� - ������
#define EFFECT_SKILL_ELECTRICSHOT		83	// �޸� - �Ϸ�Ʈ�� ��
#define EFFECT_SKILL_SHADOWSLASH2		84	// ���� - ������ ������(���ݽ�)
#define EFFECT_SKILL_ADRENALINE			85	// ���� - �Ƶ巹����
#define EFFECT_SKILL_FLYINGCRASH2		86	// ��ĭ - �ö��� ũ����(Ÿ�ݽ�)
#define EFFECT_SKILL_SHADOWSLASH		87	// ���� - ������ ������(���۽�)
#define EFFECT_SKILL_FLAMESPEAR			88	// ī�� - ///�÷��ӽ��Ǿ�
//==============================================================================================
#define EFFECT_SKILL_SLAYER				97	// �����̾� 
#define EFFECT_SKILL_CHACRAM			98	// íũ��
#define EFFECT_SKILL_ICE				99	// ���� ����
#define EFFECT_CONDITION				100	// ����(��ȭ,����..) ȿ��
#define EFFECT_OPTIONSKILL				101	// ���ӽ�ų ȿ��
#define EFFECT_HITPOWER					102	// Ư������(ũ��Ƽ��,ī��Ʈ...)ȿ��
#define EFFECT_BLACKHOLE				104	// ��Ȧ
#define EFFECT_BOOK 					105	// å���� �´� ȿ��
#define EFFECT_USE_BOOK					106	// å����ȿ��
#define EFFECT_REMOVE 					107	// ���� 
#define EFFECT_USE_BOOK_SPELL			108	// å���� ���� �ö󰡴� ȿ��
#define EFFECT_LIFE_LIGHT 				108	// ������ �� ȿ��
#define EFFECT_SKILL_SPELL				109 // ��ų���� �ö󰡴� ȿ��
#define EFFECT_CHA_LIFE_LIGHT			109	// ������ �� Ÿ��ȿ��
#define EFFECT_JOINT_LIFE 				110	// ������� ȿ��
#define EFFECT_AUTOLASER				110	// �ڵ�������
#define EFFECT_CHARGECANNON				111	// ������
#define EFFECT_FLAME					112	// ȭ������
#define EFFECT_S_EQUIP					113	// ������� �Ѱ� ����
#define EFFECT_SCANVIEW					114	// ��������� ����
#define EFFECT_SMALL_ROCKET				115	// ��������
#define EFFECT_MIDDLE_ROCKET			116	// ��������
#define EFFECT_GATHERING				117	// ä����
#define EFFECT_SKILL_DSTONE				118	// ����Ʈ ����� ����
#define EFFECT_SKILL_DSLASH				119	// ����Ʈ ���� ������
#define EFFECT_SKILL_BASH				120	// ����Ʈ �转
#define EFFECT_DSTONE_EFF				121	// ����� ������ ���� ���� Effect
#define EFFECT_SKILL_ASSAULT			122	// ���Ʈ�� Effect
#define EFFECT_SKILL_DASSAULT			123	// ��� ���Ʈ�� Effect
#define EFFECT_SKILL_TIMECHG			124	// Ÿ�������� Effect
#define EFFECT_DEMICRUSHER				125	// ����ũ���
#define EFFECT_FIRETOWER_EFF			126	// ��Ʈ���� ��� ȿ��
#define EFFECT_SKILL_STONEBRUST			127	// ���� ����Ʈ
#define EFFECT_SKILL_HOLLEY				128	// Ȧ�� ������
#define EFFECT_SPEEDUPSMOKE				129	// ������ ���ǵ�� ���� ȿ��
#define EFFECT_SPIRITMODE				130	// ���Ǹ� ��嶧 ȿ��(�ߵ��� ȿ��)
#define EFFECT_SPIRITMODE2				131	// ���Ǹ� ��嶧 ȿ��(������ ȿ��)
#define EFFECT_SPACE_SLASH				132	// ���� - �����̽� ������
#define EFFECT_ELECTRIC_HAMMER			133	// ���� - �Ϸ�Ʈ�δ� �ظ�
#define EFFECT_SKILL_SUMMONFIRE			134	// ���� - �������̾�
#define EFFECT_FROZEN_SHOT				135	// �޸� - ������ ��
#define EFFECT_SKILL_RANDOMFIRE			136	// �޸� - �������̾�
#define EFFECT_SKILL_MERCERNERYSLASH	137	// ���� - �Ӽųʸ� ������
#define EFFECT_SKILL_ICEBOLT			138	// ī�� - ���̽���Ʈ
#define EFFECT_SKILL_WILDSMASH			139	// ��ĭ - ���ϵ� ���Ž�
#define EFFECT_SKILL_RESURRECTION		140	// ���� - ��������
#define EFFECT_SKILL_TORNADO_HAMMER		141	// ���� - ����̵� �ظ�
#define EFFECT_CONDITION2				142

enum EFFECT_PEROM
{
	EFFECT_PEROM_SKILL_S125	= 143,
	EFFECT_PEROM_SKILL_S126	,
	EFFECT_PEROM_SKILL_S127	,
	EFFECT_PEROM_SKILL_S128	,
	EFFECT_PEROM_SKILL_S129	,
	EFFECT_PEROM_SKILL_S130	,
	EFFECT_PEROM_SKILL_S131	,
	EFFECT_PEROM_SKILL_S132	,
	EFFECT_PEROM_SKILL_S133	,
	EFFECT_PEROM_SKILL_S134	,
	EFFECT_PEROM_SKILL_S135	,
	EFFECT_PEROM_SKILL_S136	,
	EFFECT_PEROM_SKILL_S137	,
	EFFECT_PEROM_SKILL_S138	,
	EFFECT_PEROM_SKILL_S139	,
	EFFECT_PEROM_SKILL_S140	,
	EFFECT_PEROM_SKILL_S141	,
	EFFECT_PEROM_SKILL_S142	,
	EFFECT_PEROM_SKILL_S143	,
	EFFECT_PEROM_SKILL_S144	,
	EFFECT_PEROM_SKILL_S145	,
	EFFECT_PEROM_SKILL_S146	,
	EFFECT_PEROM_SKILL_S147	,
	EFFECT_PEROM_SKILL_S148	,
	EFFECT_PEROM_SKILL_S149	,
	EFFECT_PEROM_SKILL_S150 ,

	EFFECT_PEROM_OPTIONSKILL_S134 ,
	EFFECT_PEROM_OPTIONSKILL_S148 ,
	EFFECT_PEROM_OPTIONSKILL_S149 ,
	EFFECT_PEROM_OPTIONSKILL_S150 ,
};

//==============================================================================================
#define EFFECT_SKILL_RING_BEE			1431
#define EFFECT_SKILL_RING_BUTTER		1432
#define EFFECT_SKILL_RING_BORA			1439
#define EFFECT_SKILL_RING_MOTH			1440
#define EFFECT_SKILL_RING_HORN			1510
#define EFFECT_SKILL_RING_SILVERFLY		1544
#define EFFECT_SKILL_RING_RAVEN			1545
#define EFFECT_SKILL_RING_KINES			1546
#define EFFECT_SKILL_RING_NEO			1547
#define EFFECT_SKILL_RING_KAISEN		1548
#define EFFECT_SKILL_RING_SHININGEAGLE	1860
#define EFFECT_SKILL_RING_GRYPHON		1929
#define EFFECT_SKILL_RING_DRAGON		1930
#define EFFECT_SKILL_RING_DRAGON2		2317 // �� �巡�︵ ///����������
#define EFFECT_SKILL_RING_SPHINX		1982
#define EFFECT_SKILL_RING_DEVIL			2065
#define EFFECT_SKILL_RING_DEVIL2		2326 // �� ������ ///����������
#define EFFECT_SKILL_RING_BULKAN		2417 ///��ĭ��

#define EFFECT_SKILL_RING_SARNGA		2418 // [12/3/2008 Theodoric] �縪��
#define EFFECT_SKILL_RING_SARNGA2		2419 // [12/3/2008 Theodoric] �� �縪��
#define EFFECT_SKILL_RING_GRIFFON		2420 // [12/3/2008 Theodoric] �� �縪��
#define EFFECT_SKILL_RING_YETI			2421	// AIDIA LEVEL 450
#define EFFECT_SKILL_RING_PEAKOCK		2422	// AIDIA LEVEL 500
#define EFFECT_SKILL_RING_TIGER			2423	// AIDIA LEVEL 550
#define EFFECT_SKILL_RING_UNICORN		2424	// AIDIA LEVEL 600
#define EFFECT_SKILL_RING_650			2425	// AIDIA LEVEL 650
#define EFFECT_SKILL_RING_700			2426	// AIDIA LEVEL 700
#define EFFECT_SKILL_RING_750			2427	// AIDIA LEVEL 750
#define EFFECT_SKILL_RING_800			2428	// AIDIA LEVEL 800
#define EFFECT_SKILL_RING_850			2429	// AIDIA LEVEL 850
#define EFFECT_SKILL_RING_900			2430	// AIDIA LEVEL 900
#define EFFECT_SKILL_RING_950			2431	// AIDIA LEVEL 950
#define EFFECT_SKILL_RING_1000			2432	// AIDIA LEVEL 1000
#define EFFECT_SKILL_RING_CHR			2433
#define EFFECT_SKILL_RING_CHIN			2434

#define EFFECT_SKILL_WAND_ARROW			1768
#define EFFECT_SKILL_WAND_LASER			1769
#define EFFECT_SKILL_WAND_CHAIN			1770
#define EFFECT_SKILL_WAND_METEORITE		1771
#define EFFECT_SKILL_WAND_ICE_STORM		1772
#define EFFECT_SKILL_WAND_MAGICSWORD	1949
#define EFFECT_SKILL_WAND_CHAOS			1774  // ȥ���� ��ǳ �ϵ�        
#define EFFECT_SKILL_WAND_WAVE_OF_DEATH	1775  // ������ ���� �ϵ�

#define EFFECT_DROPCANDY						50000	// �ջ��� �ӽ� ��ȣ
#define EFFECT_DROPAPPLE						50001	// �뻧��� �ӽ� ��ȣ
#define EFFECT_POISON_FOG						50002	// ��ī�� ������ ���Ȱ� ///�δ�1
#define EFFECT_HEART_PARTICLE					50003   // ���� 2007-01-18 : ///��Ʈ�ε� ��ƼŬ ����Ʈ 
#define EFFECT_PIG_HEALING						50004   // ���� 2007-02-01 : // ���� ������ ȸ�� ����Ʈ
#define EFFECT_LIP_PARTICLE						50005   // 2007.03.14 ȭ��Ʈ
#define EFFECT_LOVE_PARTICLE					50006   // 2007.03.14 ȭ��Ʈ
#define	EFFECT_MURAMASA_GHOST					50007   ///�Ϻ����������׽�Ʈ ������ ���󸶻� �� �ͽ� ����Ʈ.
#define EFFECT_DROPSTARCANDY					50008	// ������ �ӽ� ��ȣ

//  [10/30/2008 parkmj] �Ϻ���û - �ջ���(30/90��)
#define EFFECT_DROPCANDY30						50009	// �ջ���30
#define EFFECT_DROPCANDY90						50010	// �ջ���90

//  [3/23/2009 ppmmjj83] �Ϻ���û - ��� / ���� / �ҵ�
#define EFFECT_DROPBASIN0						50011	// ���(10��)
#define EFFECT_DROPBASIN1						50012	// ���(30��)
#define EFFECT_DROPBASIN2						50013	// ���(60��)
#define EFFECT_DROPTHUNDER0						50014	// ����(10��)
#define EFFECT_DROPTHUNDER1						50015	// ����(30��)
#define EFFECT_DROPTHUNDER2						50016	// ����(60��)
#define EFFECT_DROPSWORD0						50017	// �ҵ�(10��)
#define EFFECT_DROPSWORD1						50018	// �ҵ�(30��)
#define EFFECT_DROPSWORD2						50019	// �ҵ�(60��)

#define EFFECT_DROPAPPLEPVP						50020



#define EFFECT_SKILL_STONEBASH					10005   // ���̺긮�� �޺���ų ���� �转. ///�����޺�
#define EFFECT_SKILL_BASHREFLECTION				10008   // ���̺긮�� �޺���ų �转���÷���. ///�����޺�
#define EFFECT_SKILL_BASHREFLECTION2			100082  // ���̺긮�� �޺���ų �转���÷���. ///�����޺�
//==========================================================================
#define ATTACK_COUNT_HIT		1		//ī��Ʈ
#define ATTACK_CRITICAL_HIT		2		//ũ��Ƽ��	
#define ATTACK_DEMOLITION_HIT	3		//��������
#define ATTACK_CROSS_HIT		4		//����
#define ATTACK_VAMPIRE_HIT		5		//�� ���� ���� ///�Ϻ����������׽�Ʈ ������ ���󸶻��� ȿ��.

// nate 2005-06-07 : ���� �з�
#define CONDITION_NORMAL		0		//����
#define	CONDITION_STONY			1		//��ȭ
#define	CONDITION_PARALYSIS		2		//����
#define	CONDITION_CHAOS			3		//ȥ��
#define	CONDITION_FREEZE		4		//�ð�
#define	CONDITION_SHOCK			5		//����
#define CONDITION_ENTANGLE		6		//����

//============================================================================
// nate 2005-06-07 : ���� ����Ʈ
#define EFFECT_SKILL_MOB_SHILON					4000
#define EFFECT_DURANT_BEAM						4001	// nate 2005-09-26 : ���Ʈ - ����
#define	EFFECT_DURANT_ATTACK					4002	// nate 2005-09-26 : ���Ʈ - ����
#define EFFECT_DURATT_TARGET					4003	// nate 2005-09-29 : ���Ʈ - ��󿡰� �������� ����Ʈ
#define EFFECT_CACTUS							4004	// nate 2005-11-22 : ��ȯ ĳũ�ͽ�
#define EFFECT_DANDELION						4005	// nate 2005-11-22 : �������
#define EFFECT_SPORE							4006	// nate 2005-11-22 : ������
#define EFFECT_SKILL_MOB_SNAKEBOSS				4007	// nate 2006-01-03 : Ʈ��Ʈ�� - �����޵��
#define EFFECT_SKILL_MOB_SNAKEBOSS2				4008	// nate 2006-01-17 : �޵��
#define EFFECT_SKILL_MOB_SNAKEBOSS3				4009	// nate 2006-01-26 : ����Ʈ�� - ����
#define EFFECT_SKILL_MOB_LEGKNOCK1				4010	// ��ī�� �δ� ���� ���׳� �Ϲݰ��� ����Ʈ. ///�δ�1
#define EFFECT_SKILL_MOB_LEGKNOCK2				4011	// ��ī�� �δ� ���� ���׳� ���߰��� ����Ʈ. ///�δ�1
#define EFFECT_DECKAREN_CUT_IN					4012	// ��ī�� 1�� ���� �� �̺�Ʈ ����. �� 4��. ///�δ�1
#define EFFECT_KOREA_SERVICE_LEVEL_MARK			4013	// �ѱ� ���� ��� ǥ�ÿ� ��ũ.

#define EFFECT_CHA_DONT_ATTACK_STATE			4014	// ĳ���� �������� �������� on/off ���ش�.

#define EFFECT_SKILL_MOB_WORLDCUP_HAND			5001
#define EFFECT_SKILL_MOB_WORLDCUP_FOOT			5002
#define EFFECT_SKILL_MOB_GUARDTOWER_FIRE		5003
#define EFFECT_SKILL_MOB_SHILONMASTER			5004
#define EFFECT_SKILL_CONTROL_LIGHT_GUARD		5005
#define EFFECT_SKILL_CONTROL_LIGHT_DAMAGE		5006
#define EFFECT_SKILL_MOB_GUARDTOWER_LIGHTBALL	5007
#define EFFECT_SKILL_MOB_GUARDTOWER_PEBBLESHOT	5008
#define EFFECT_SKILL_MOB_BABYWORM				5009	// ��������
#define EFFECT_SKILL_MOB_MOB_SHINING_JAKAL		5010	// 1������=���̴��ڰT
#define EFFECT_SKILL_MOB_ACCUMULATE				5011	// ���� ��������
#define EFFECT_SKILL_MOB_FIRE					5012	// ���� �ҼӼ� ����
#define EFFECT_SKILL_DRAGON_FIRE				5013	// �巹���� �Ұ���
#define EFFECT_SKILL_DRAGON_ICE					5014	// �巹���� �ñ����
#define EFFECT_SKILL_KHAN_HIT					5015	// ĭ�� �������� 
#define EFFECT_SKILL_WARGIANT_MULTI				5016	// �����̾�Ʈ�� �������� 
#define EFFECT_SKILL_MOB_SOULDUST				5017	// ���� ��ȥ���� ���� 
#define EFFECT_SKILL_MOB_FALLOFFSTONE			5018	// ���� ���� ���� 
#define EFFECT_SKILL_MOB_STONERANCE2			5019	// ���� ������� ���� ���� 
#define EFFECT_SKILL_MOB_DRILLSTONE				5020	// ���� �ؿ��� �ڴ� �۰� ���� 
#define EFFECT_SKILL_PET_BLESS					5021    // �� ��ų �극�� ����Ʈ.
#define EFFECT_SONGKRAN_WTRSPRINKLE				5022	// [3/27/2007 Theodoric] �±� ��ũ�� �̺�Ʈ

#define EFFECT_BOSSRAID_BULKAN1_02		5023
#define EFFECT_BOSSRAID_BULKAN1_03		5024
#define EFFECT_BOSSRAID_BULKAN2_02		5025
#define EFFECT_BOSSRAID_BULKAN3_01		5026
#define EFFECT_BOSSRAID_BULKAN3_02		5027

#define EFFECT_BOSSRAID_HUMAN1_02		5028
#define EFFECT_BOSSRAID_HUMAN1_03		5029
#define EFFECT_BOSSRAID_HUMAN2_02		5030
#define EFFECT_BOSSRAID_HUMAN2_03		5031
#define EFFECT_BOSSRAID_HUMAN3_02		5032

#define EFFECT_BOSSRAID_AIDIA1_01		5033
#define EFFECT_BOSSRAID_AIDIA1_02		5034
#define EFFECT_BOSSRAID_AIDIA2_02		5035
#define EFFECT_BOSSRAID_AIDIA2_03		5036
#define EFFECT_BOSSRAID_AIDIA3_02		5037

#define EFFECT_BOSSRAID_HYBRI1_01		5038
#define EFFECT_BOSSRAID_HYBRI1_02		5039
#define EFFECT_BOSSRAID_HYBRI2_02		5040
#define EFFECT_BOSSRAID_HYBRI3_01		5041
#define EFFECT_BOSSRAID_HYBRI3_02		5042

#define EFFECT_BOSSRAID_KAI1_01			5043
#define EFFECT_BOSSRAID_KAI1_02			5044
#define EFFECT_BOSSRAID_KAI2_01			5045
#define EFFECT_BOSSRAID_KAI2_02			5046
#define EFFECT_BOSSRAID_KAI2_03			5047
#define EFFECT_BOSSRAID_KAI3_01			5048
#define EFFECT_BOSSRAID_KAI3_02			5049
#define EFFECT_BOSSRAID_KAI3_03			5050

#define EFFECT_BOSSRAID_LASTBOSS_02		5051
#define EFFECT_BOSSRAID_LASTBOSS_03		5052
#define EFFECT_BOSSRAID_AIDIA2_01		5053
#define EFFECT_BOSSRAID_AIDIA3_01		5054	// [3/6/2008 �����] �������̵� ���̵�ƺ���3 ��� ����
#define EFFECT_BOSSRAID_LASTBOSS_01		5055	// ����
#define EFFECT_BOSSRAID_LASTBOSS_04		5056

#define EFFECT_HEROEFFECT				5057

#define EFFECT_SKILL_WAND_WAVE_OF_DEATH2	5058
#define EFFECT_SKILL_WAND_WAVE_OF_DEATH3	5059

//============================================================================
#define EFFECT_NOTSKILL_SPEEDUP					1
#define EFFECT_NOTSKILL_SPIRITMODE				2


//#define MAX_SKILL_LEVEL		30
#define SKILL_UNSELECTED	(-50)
#define SELECTED_SKILL_ATTACT (-1)	// �Ϲ� ����
#define SELECTED_SKILL_ROTATE (-2)	// ī�޶� ȸ��

//----------------------------------------------------------------------------
//-- ��ų�� ���� �Լ���
//-- 2004.11.19
//-- Lyul / �߰�
void G_SK_SetCooltime(int li_shdidx, int li_curlevel = 0);     //-- �� ��ų�� �ð� Delay �� �����Ѵ�. ������ ����
void G_SK_RotateShadow();               //-- ��ų�� �ð� ������ ���� ��Ŵ
//-- ���� ��ų ���� Shadow ��(mungmae-2005/05/09 int li_shdidx2 �߰�)
// ����� ������ �ٸ� ��ų�� �޸� ��ų�� �ö󰥶� ���� Shadow ���� �޶����� ������
// li_shdidx ���� ��ų, li_shdidx2 ��ų ����
int  G_SK_GetShdangle(int li_shdidx, int li_shdidx2);
void G_SK_ResetShadow();								//-- �� �̵� �� ��Ÿ Reset Ÿ�ֿ̹� �ʿ��� �Լ�
bool G_SK_Getflag(int li_shdidx);       //-- ���� ��ų��Shadow bool ��
bool G_SK_IsOnceAttack(int cur_skill);  //-- �ѹ��� ���� �Ǵ� ��ų���� �Ǵ�.
bool G_SK_IsSecAttack(int cur_skill);   //-- Secattack ���� �Ǵ�
bool G_SK_IsEffectSkill(int li_skill);   //-- Effct �� �ʿ����� �Ǵ�.
void G_SK_INVCooltime(int li_skill);      //-- - ���� + �� ��ȯ�ؼ� ����

#endif // SKILL_H
