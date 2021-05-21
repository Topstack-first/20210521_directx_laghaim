#ifndef _NK_MY_PET_H_
#define _NK_MY_PET_H_

#include "Config.h"
#include "NkPet.h"
#include "info_define.h"

#define MYPET_MODE_NONE		0				//없는상태
#define MYPET_MODE_EGG		1				//알상태
#define MYPET_MODE_KEEP		2				//보관상태
#define MYPET_MODE_HOLD		3				//댈꼬 다닐때

#define MYPET_MODE_COMBAT		11			//전투모드
#define MYPET_MODE_FOLLOW		12			//동행모드
#define MYPET_MODE_PICKUP		13			//획득모드
#define MYPET_MODE_WIMPY		14			//회피모드
#define MYPET_MODE_SUPP			15			//생산모드
#define MYPET_MODE_RANGECOMBAT  16  //원거리 계열해제 모드
#define MYPET_MODE_GUARD		17  //경계모드
#define MYPET_MODE_MACRO		18  //매크로모드

//#define MYPET_MODE_NUM		5
#define MYPET_MODE_DEFAULT	MYPET_MODE_NONE

#define MYPET_MASTER_RANGE_FOLLOW	(20.0f)
#define MYPET_MASTER_RANGE_FOLLOW_CLASS1	(30.0f) // 진화 1단계 라비용이 주인한테 다가오는 거리.
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

// 애완동물 상태창
class CPetStatus
{
public:
//===============================
//fpe방지 위치와 변수형 변경
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
	BOOL m_bUseMacroMode; // 현재 매크로 모드 인지 플래그.
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

	int m_Class; // 라비용의 진화 타입(기본형0,방어형1,공격형2)
	int m_SkillLevel[PET_SKILL_MAX]; // 자신이 스킬을 배웠는지 여부.

	CItem* m_Wearing[PET_PIECE_NUM];

	CPetStatus();
	~CPetStatus();

	void Init();

	BOOL	SetMode(int nMode);

	// CmdPetStatus 으로 부터 정보를 체우자
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
	void SetClassType(int); // 라비용 클래스(기본0, 방어형1, 공격형2)
	void SetSkill(int,int); // 라비용이 배운 스킬 셋팅.
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
//fpe방지 위치와 변수형 변경
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
	DWORD		m_UseSkillTime; // 마지막으로 스킬을 사용한 시간. 한번 스킬을 썼으면 일정시간 동안은 못쓰게 막는다.

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
	BOOL	UsePetSkill( int DestType, int Index ); // 펫이 스킬 사용 확률 체크. // DestType은 몹인지 캐릭터인지(펫한테는 스킬안씀)


	void SetVital(int, int);
	void SetRangeAtt(int);
	void SetMeleeAtt(int);
	void SetRangeDef(int);
	void SetMeleeDef(int);
	void SetBody(int horn, int head, int body, int wings, int legs, int tail);

	char	*GetPieceName(int type);
};

#endif // _NK_MY_PET_H_
