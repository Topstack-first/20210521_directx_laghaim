#ifndef _NK_MY_PET_H_
#define _NK_MY_PET_H_

#include "Config.h"
#include "NkPet.h"
#include "info_define.h"

#define MYPET_MODE_NONE		0				//���»���
#define MYPET_MODE_EGG		1				//�˻���
#define MYPET_MODE_KEEP		2				//��������
#define MYPET_MODE_HOLD		3				//� �ٴҶ�

#define MYPET_MODE_COMBAT		11			//�������
#define MYPET_MODE_FOLLOW		12			//������
#define MYPET_MODE_PICKUP		13			//ȹ����
#define MYPET_MODE_WIMPY		14			//ȸ�Ǹ��
#define MYPET_MODE_SUPP			15			//������
#define MYPET_MODE_RANGECOMBAT  16  //���Ÿ� �迭���� ���
#define MYPET_MODE_GUARD		17  //�����
#define MYPET_MODE_MACRO		18  //��ũ�θ��

//#define MYPET_MODE_NUM		5
#define MYPET_MODE_DEFAULT	MYPET_MODE_NONE

#define MYPET_MASTER_RANGE_FOLLOW	(20.0f)
#define MYPET_MASTER_RANGE_FOLLOW_CLASS1	(30.0f) // ��ȭ 1�ܰ� ������ �������� �ٰ����� �Ÿ�.
#define MYPET_MASTER_RANGE_PICKUP	(150.0f)
#define MYPET_MASTER_RANGE_COMBAT	(200.0f)
#define MYPET_MASTER_RANGE_MAX		(400.0f)
#define MYPET_MASTER_RANGE_DEFAULT	MYPET_MASTER_RANGE_FOLLOW

#define MYPET_SOCIAL_PICKUP		4
#define MYPET_SOCIAL_1			18
#define MYPET_SOCIAL_2			19
#define MYPET_SOCIAL_3			20
#define MYPET_SOCIAL_4			21
#define MYPET_SOCIAL_5			22
#define MYPET_SOCIAL_6			23

class CItem;

// �ֿϵ��� ����â
class CPetStatus
{
public:
//===============================
//fpe���� ��ġ�� ������ ����
//===============================
	float  level;
	float  speed;
	__int64  exp;
	CItem *m_pNecklace;
	float  pick_level;
	float  loyalty;
	__int64  need_exp;
	float  hunger;
	int  mode;
	BOOL m_bUseMacroMode; // ���� ��ũ�� ��� ���� �÷���.
	float  max_vital;
	float  time;
	float  melee_att;
	float  food_count;
	float  melee_def;
	float  max_food;
	float  range_att;
	__int64  need_money;
	float  range_def;
	CItem *m_pBag;
	float  compos_level;

	int m_Class; // ������ ��ȭ Ÿ��(�⺻��0,�����1,������2)
	int m_SkillLevel[PET_SKILL_MAX]; // �ڽ��� ��ų�� ������� ����.

	CItem* m_Wearing[PET_PIECE_NUM];

	CPetStatus();
	~CPetStatus();

	void Init();

	BOOL	SetMode(int nMode);

	// CmdPetStatus ���� ���� ������ ü����
	void SetVital(int, int);
	void SetRangeAtt(int);
	void SetMeleeAtt(int);
	void SetRangeDef(int);
	void SetMeleeDef(int);
	void SetSpeed(int);
	void SetLevel_Compos(int);
	void SetLevel_Pick(int);
	void SetLoyalty(int);
	void SetHunger(int);
	void SetLevel(int);
	void SetExp(__int64);
	void SetNeedMoney(__int64);
	void SetNeedExp(__int64);
	/*void SetNeedExp(int);
	void SetMode(int);
	void SetTime(int);*/
	//CItem* SetBag(CItem *pItem);
	BOOL SetBag(int vnum);
	BOOL SetFoodCount(int);
	BOOL IncFoodCount();
	BOOL SetNecklace(int vnum);
	CItem* SetNecklace(CItem *pItem);
	void SetClassType(int); // ���� Ŭ����(�⺻0, �����1, ������2)
	void SetSkill(int,int); // ������ ��� ��ų ����.
	BOOL SetEquip(int where, int vnum);
};

extern CPetStatus g_MyPetStatus;

class CNkMyPet  : public CNkPet
{
public:
	CNkMyPet();
	CNkMyPet(CClrAlloc *pClrAlloc);
	virtual ~CNkMyPet();
//===============================
//fpe���� ��ġ�� ������ ����
//===============================
	float		m_RangeDef;

	//int		m_Mode;
	float		m_CombatMobId;
	float		m_RangeAtt;
	float		m_CombatChaId;
	float		m_MeleeDef;
	float		m_CombatPetId;
	float		m_MeleeAtt;
	float		m_MasterRange;
	float		m_MaxVital;
	DWORD		m_SocialTime;
	DWORD		m_UseSkillTime; // ���������� ��ų�� ����� �ð�. �ѹ� ��ų�� ������ �����ð� ������ ������ ���´�.

	char	m_TmpBuf[200];

	BOOL	AttackNearestMob();
	BOOL	AttackCombatMob();
	BOOL	FollowMaster();
	BOOL	SetMode(int nMode);
	BOOL	SetCombatMob(int nMobId, int nChaId = -1, int nPetId = -1);
	BOOL	PickItem();
	BOOL	Social(DWORD curTime);
	void	InitSocialTime();
	void	InitSocialTime(DWORD curTime);
	BOOL	MoveToFollow(float x, float z);
	BOOL	MoveToMaster(float x, float z);
	BOOL	MoveToAttack(float x, float z);
	BOOL	UsePetSkill( int DestType, int Index ); // ���� ��ų ��� Ȯ�� üũ. // DestType�� ������ ĳ��������(�����״� ��ų�Ⱦ�)


	void SetVital(int, int);
	void SetRangeAtt(int);
	void SetMeleeAtt(int);
	void SetRangeDef(int);
	void SetMeleeDef(int);
	void SetBody(int horn, int head, int body, int wings, int legs, int tail);

	char	*GetPieceName(int type);
};

#endif // _NK_MY_PET_H_
