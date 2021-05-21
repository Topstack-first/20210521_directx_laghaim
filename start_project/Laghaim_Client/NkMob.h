#ifndef _NK_MOB_H_
#define _NK_MOB_H_

#include "Config.h"

#include <list>
#include <vector> ///점포상점
#include <map>
using namespace std;

#include "DecoObj.h"

#include "singleton.h"

//==================================
// nate 2004 - 4
// Crash_Miss
#define CRASH_MISS_COUNT	3
//==================================

#define NPC_PROCEDURE		(1 << 0)
#define NPC_MOVING			(1 << 1)
#define NPC_AGGRESSIVE		(1 << 2)
#define NPC_OUTAREA			(1 << 3)
#define NPC_WIMPY			(1 << 4)
#define NPC_ANYWHERE		(1 << 5)
#define NPC_HELPER			(1 << 6)
#define NPC_GUARD			(1 << 7)
#define NPC_SHOPKEEPER		(1 << 8)
#define NPC_NORADER			(1 << 9)
#define NPC_NOMUTANT		(1 << 10)
#define NPC_FISH			(1 << 11)
#define NPC_QUEST			(1 << 12)
#define NPC_SUMMON			(1 << 13)
#define NPC_KING			(1 << 14)
#define NPC_NEW_SUMMON		(1 << 15)
#define NPC_PEACEFUL		(1 << 16)
#define NPC_BOSS			(1 << 17)
#define NPC_DSTONE			(1 << 18)
#define NPC_HELPPET			(1 << 19)
#define NPC_SUMMON_FLOWER	(1 << 25)
#define NPC_TRAP			(1 << 26)
#define NPC_NODAMAGE		(1 << 27)

class CBiped;
class CCapsyong;
class IndexedTexture;
class CClrAlloc;
class CNkCharacter;
class CDropStar;
//============================
// nate 2004 - 4
// Crash_Miss
class CAlphaPopUp;
class CDisplay;
//============================

class MobScaleData : public CSingleton<MobScaleData>
{
	typedef map<int, float> MAP_SCALE;
public:
	MobScaleData();
	~MobScaleData();

public:
	float GetScaleValue(const int& mobIndex);
	void LoadScript();

private:
	MAP_SCALE m_mapScale;
};

class CMobData
{
public:
	CMobData();
	~CMobData();

	static int npc_count;

	int	 nVnum;
	char *mob_name;
	int  attack_frame;
	int  speed;
	CCapsyong *pCapsyong;
	CCapsyong *pBound;
	int		textureNum;
	CBiped	  *pBpd[7];			// [10/23/2007 Theodoric] npc 2개의 동작 추가를 위해 5에서 7로 늘린다.
	int		  idxSound[7];		// [10/23/2007 Theodoric] npc 2개의 동작 추가를 위해 5에서 7로 늘린다.
	int       nSoundFrame[7];	// [10/23/2007 Theodoric] npc 2개의 동작 추가를 위해 5에서 7로 늘린다.
	int		flag;
	int		height;
	int		flystone;
};

class MobDataLoading
{
	CBiped	**m_dpBiped;
	char	**m_dpBipedName;

	char	*m_pBipedDir;
	char	*m_pCapDir;

public:
	MobDataLoading();
	~MobDataLoading();

	// 초기 설정
	void	InitSize(int nMaxCapNum, int nMaxBipedNum);
	void	SetBipedDir(char *dir_name);
	void	SetCapDir(char *dir_name);
	void	SetTexturePath(char *dir_name);

	// 화일 열기
	void	ReadDataFile(char *file_name);
	CBiped  *LoadBiped(char *file_name);
	int		m_nNpcNameNum;

	int		m_nMaxBipedNum, m_nCurBipedNum;

	CMobData *m_pMobData;
	int		 m_nNpcNum;
};

extern MobDataLoading * g_pMobDataLoading;


#define MOBMSG_UNARMED_ATTACKED		0
#define MOBMSG_SWORD_ATTACKED		1
#define MOBMSG_MAGIC_ATTACKED		2

class CNkMob
{
protected:
	unsigned short	m_nMotionState;			// 0 : stand,

	// state 가 0일때는 Stand
	//    1 일때는 Move
	//    2 일때는 Attack
	//	  3 일때는 Die
	//	  4 일때는 Knock
	//	  5 추가동작 1
	//	  6 추가동작 2
	CBiped *m_pBiped[8];				// [10/23/2007 Theodoric] npc 2개의 동작 추가를 위해 5에서 7로 늘린다.
	CCapsyong *m_pBound;

	int		m_MobDataIdx;				// CMobData (맙들에 대한 집합체) 에 대한 위치 포인터.
	// 아래의 사운드 인덱스나 사운드 프레임은 꼭 필요한 것은 아니다.
	int		m_CurFrame, m_PrevFrame;
	int		m_nSoundIdx[8];				// [10/23/2007 Theodoric] npc 2개의 동작 추가를 위해 5에서 7로 늘린다.
	int		m_nSoundFrame[8];			// 소리가 날 프레임.	// [10/23/2007 Theodoric] npc 2개의 동작 추가를 위해 5에서 7로 늘린다.
	float	m_AniRate;
	float	m_fMoveSpeed;
	int		m_AttackFrame;
	int		m_DeadFrame;

	//IndexedTexture	*m_CapsyongTexture;

	// 애니메이션
	BOOL	m_bMoving, m_bAttack;
	float	m_dx, m_dz;
	float	m_tx, m_tz;

	// 시간 계산
	DWORD	m_dwTimeCapPrev, m_dwBaseTime;

	int		m_MobSkill;

public:
	CNkMob();
	CNkMob(CClrAlloc *pClrAlloc);
	virtual ~CNkMob();

	CCapsyong *m_pCapSyong;

	// 캐릭터의 특성값.
	int		m_nMobNum;								// 맙의 고유 id (db)
	int		m_nVirId;
	WORD		m_wColorIdx;						// 컬러 테이블 인덱스
	DWORD		m_dwColorPick;						// 컬러 패러미터.
	CClrAlloc	*m_pClrAlloc;
	double m_Vital;

	// 공격하려고 하는 맙이나 캐릭터 아이디
	int		m_AttackChaId;
	int		m_AttackMobId;
	int		m_AttackPetId;
	void	SetAttackChaId(int chaId);
	void	SetAttackMobId(int mobId);
	void	SetAttackPetId(int petId);

	// 맞아서 변경될 체력
	int		m_AttackVital;
	void	SetAttackVital(int vital);
	DWORD	m_LastAttackVitalTime;
	void	Attacked();

	// 맙이 떨어뜨리는 아이템
	//CItem	*m_pDropItem;
//	int	 m_DropItemIndex;
	int  m_DropItemVnum;
	int  m_DropItemX;
	int  m_DropItemZ;
	int  m_DropItemDir;
//	void SetDropItem(int item_index, int vnum, int x, int z, float dir);

	// Flag..
	int		m_Flag;

	// 주인공이 다가오고 있구나!!!
	BOOL	m_bHeroAttCome;


	// 맙의 위치와 방향 정보
	int		m_Height;
	float	m_wx, m_wy, m_wz, m_dir, m_toDir;
	int		m_FlyStone;

	BOOL	Move(float tx, float tz, int nFrameCount = 20);
	BOOL	Place(float tx, float tz, float dir, int nFrame);
	void	Stop(float tx, float tz, float dir);
	BOOL	Attack(float tx, float tz);
	BOOL	Magic(float tx, float tz);

	// 맞을때 효과 구현.
//	int		m_nAttackedType;
	int		m_bAttacked;
//	int		m_nAttackedFrame;
	int		m_nAttackedTime;
	void	SendMsg (int msg, int from_id = -1, int type = 0);

	// 초기화 함수
	void	SetInfo(int mob_num, int id, int nState=0);
	//void	SetMotionData(int MobNum, int nState);	// 맙 고유 인덱스와 동작 상태. 바이페드와 연결
	void	SetInitPos(float x, float z, float dir);

	// 프레임 계산하는 함수
	void	FrameMove(unsigned int nInc, BOOL bVisible = TRUE);

	// [2/21/2008 반재승] 몹 일반 공격 이펙트 관련 함수
	BOOL    Mob_NormalAttack_EffectFrame( int mob_Num, int effect_Num );
	DWORD hitpoints[10][3];
	void addhitpoint(DWORD num, DWORD attacker);
	bool ishit();
	// 렌더링 함수
	void	Render(BOOL bMouseOn = FALSE, BOOL bVisible = TRUE, BOOL bDontPick = FALSE);

	// 캐릭터에 대한 명령 함수
	void	SetMotionState(unsigned short nState);
	unsigned short GetMotionState()
	{
		return m_nMotionState;
	};
	BOOL	IsMoving()
	{
		return m_bMoving;
	};
	BOOL	IsAttacking()
	{
		return m_bAttack;
	};
	BOOL	IsDead()
	{
		return m_nMotionState == 3;
	}
	void	Kill();
	int		GetDeadFrame()
	{
		return m_DeadFrame;
	}

	char *  GetMobName();

	// 맙과 관련된 effects
	list<int>		m_listEffect;
	void			InsertEffectNum(int num);

	int m_MobAttackType;
	void SetAttackType(int attackType)
	{
		m_MobAttackType = attackType;
	}

	// 보스몹 관련...
	int m_EffFrame;
	int m_Mutant;

	BOOL	m_bPet;
	bool	isControlLight;

////2003 몹의 계열과 속성 효과를 나타내기 위한 함수와 변수
	int m_MobClassType; //계열몹
	void SetClassType(int classType)
	{
		m_MobClassType = classType;
	}
	int m_MobAttributeType; //이벤트 몹(몬스터들의 반란) 속성으로 씀

	int m_FissionType;// 서먼피션을 위한 변수
	void SetFissionType(int FissionType);

///2003 몹의 상태(석화,마비) 효과를 나타내기 위한 함수와 변수
	int m_ChaCondition; //석화,마비...
	int m_CancelState;//상태 on,off
	void SetChaCondition(int Condition, int cancel);
	BOOL m_ShowConditionEffect;//상태효과
	int m_ChaCondition2; //석화,마비...
	int m_CancelState2;//상태 on,off
	void SetChaCondition2(int Condition, int cancel);
	BOOL m_ShowConditionEffect2;//상태효과2
////2003 몹이 책주문의 타겟이 되었을때 효과를 나타내기 위한 함수와 변수
	int m_ReceiveBook;

	void SetUseBookEffect(int book)
	{
		m_ReceiveBook = book;
	}

	BOOL m_bQuestMobDied;//0319
	BOOL m_bQuestMob;
////////
	int m_nFlyFrame;
	CDropStar *m_pDropStar; // 발밑에 떨어지는 별

	// 너따
	CBiped *GetBiped(int num)
	{
		return m_pBiped[num];
	}
	int GetCurFrame()
	{
		return m_CurFrame;
	}

	// 뛸때 나는 먼지 효과를 위한 프레임
	int		m_RunSmokeFrame;

	Deco_Particle*	m_NkMob_Particle;
	bool			m_bDrawEffects;			// 혹시 나중에 몹 특성별루 사라져야 될 효과이라면...(ex 마법해제?)

	void Initial_Particle();
//=============================================================
	// nate 2004 - 4 : Crash_Miss
	int				m_nElapTime;
	DWORD			m_dwElapTime;
	BYTE			m_byShowCrash_Miss;

	CAlphaPopUp*	m_NPC_Event;
	CAlphaPopUp*	m_pCrash_MissPopup[ CRASH_MISS_COUNT ];

	void RenderNPC_Event( int nEventNum = 0 );
	void DrawCrash_Miss( int nI, int nElapTime );

	// 1 : Crash, 2 ; Miss
	void SetShowCrash_Miss( BYTE byCrash_Miss = 0 );
	void RenderCrash_Miss();

	void InitCrash_MissPopup( int width, int height );
//=============================================================
//=================================================================================
	// nate 2004 - 4 : quest level - 몬스터 등급
	int m_nLevel;
	void SetLevel( int nLevel )
	{
		m_nLevel = nLevel;
	}
//=================================================================================
	BOOL m_bPoisoning;	// nate 2005-11-14 : 포이즌 - npc에 독성분 입히기
	BOOL IsTrap();
	BYTE m_byInsertEffect;	// nate 2006-01-24 : 이펙트 삽입여부 체크(비트단위로 종류 분류)

	void MoveSkill(float tx, float tz, int nSkillIndex = 0); // 넉백을 위한 함수. 캐릭터 쪽꺼 배껴옴.
	float   m_fDash_Frame; // 넉백을 위한 변수.
	int m_NowMoveSkill; // 이동형 스킬 적용 여부.(몬스터는 모션 스테이트로 구분 안하고 이걸로 구분해서 넉백등 시켜준다.)

	// mungmae 2006/10/12 오토 방지용 몹 풍선말.
	DWORD m_LastChatTime;
	BOOL m_bShowMobChatPopup;
	CAlphaPopUp* m_pMobChatPopup;

	void InitMobChatPopup(int width, int height);
	void DrawMobChatPopup();
	void SetMobChatMsg(char *chat_msg);

	int m_RegeningType; // 리젠 중에 셋팅되는 리젠 연출 종류. 이 값이 셋팅되어있으면...연출중인거다. ///인던1

	bool CheckRenderHolloweenMob(const int& mobNum);
	void ExecuteBossSkill(int type);
};

class CNkMob_ByPool : public CNkMob, public MemoryPoolBase<CNkMob_ByPool>
{
public:
	CNkMob_ByPool();
	CNkMob_ByPool(CClrAlloc *pClrAlloc);
	~CNkMob_ByPool();
};

float GetMobExceptioncale( CNkMob *DestMob ); // 예외적인 클라이언트 스케일이 적용되는 경우 스케일 값을 리턴해주는 함수.

#define INSERT_EFFECT_FIRE	(1 << 0)
//#define INSERT_EFFECT_	(1 << 1)
//#define INSERT_EFFECT_	(1 << 2)
//#define INSERT_EFFECT_	(1 << 3)
//#define INSERT_EFFECT_	(1 << 4)
//#define INSERT_EFFECT_	(1 << 5)
//#define INSERT_EFFECT_	(1 << 6)
//#define INSERT_EFFECT_	(1 << 7)

#define MOB_LIGHTENING_WORRIOR	48
#define MOB_SKELETON_MAGE		61
#define MOB_DARKSOLDIER			104
#define MOB_MADSAIBOG			107
#define MOB_MADSAIBOG2			109
#define MOB_DRAKION				112
#define MOB_DARK_SOUL			125
#define MOB_SAD_SHADOW			128
#define MOB_SHILONMASTER		135
#define MOB_SAILON_MAGICIAN		137
#define MOB_SHOP_MAGIC1			140
#define MOB_SAILON_MAGICIAN2	146
#define MOB_SAILON_MAGICIAN3	164
#define MOB_KHAN_HAND			171
#define MOB_KHAN_FOOT			172
#define MOB_GUARDTOWER			170
#define MOB_GUARDTOWER_1		179
#define MOB_GUARDTOWER_9		187
#define MOB_KING_BUG			193
#define MOB_SHINING_JAKAL		194
#define MOB_BABY_BUG			196
#define MOB_LIGHT_BOOM			201
#define MOB_SAD_SHADOW_2		215
#define MOB_AIREN				256
#define	MOB_SPECTER				417	// nte 2005-10-25 : 스펙터 추가

#define MONSTER_CONTROL_DAMAGE	177
#define MONSTER_CONTROL_GUARD	178

#define MOB_HEIGHT_HIGH_NUM		0
#define MOB_HEIGHT_MIDDLE_NUM	1
#define MOB_HEIGHT_LOW_NUM		2

#define MOB_HEIGHT_HIGH_VAL		(30.0f)
#define MOB_HEIGHT_MIDDLE_VAL	(17.0f)
#define MOB_HEIGHT_MIDDLE_VAL2	(15.0f)
#define MOB_HEIGHT_LOW_VAL		(5.0f)

#define MOB_ATTACK_NORMAL			0
#define MOB_ATTACK_STARFLARE		1
#define MOB_ATTACK_LIGHTNING		2
//2003 몹의 속성 공격 define
#define MOB_ATTACK_ACCUMULATE		3
#define MOB_ATTACK_FIRE				4



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 점포상점 NPC 클래스( NKMob 클래스를 상속받아쓴다. 루틴은 완전 독립적, 멤버 변수들을 가져다쓰기위해 상속한다. ) ///점포상점
class CUserShopNpc:public CNkMob
{
public:
	CUserShopNpc();
	CUserShopNpc( CClrAlloc *pClrAlloc );
	~CUserShopNpc();

	void	FrameMove( BOOL bVisible = TRUE );

	void	Render( BOOL bMouseOn = FALSE, BOOL bVisible = TRUE, BOOL bDontPick = FALSE );


	DWORD m_MasterIndex; // 상점 주인의 인덱스. 패킷을 보낼땐 이걸로 보내야 한다.
	char m_strMasterName[100]; // 상점 주인 이름.
	char m_strUserShopMent[256]; // 상점 멘트 스트링. 머리위에 표시해줘야한다.

	BOOL m_bRemoveHair; // 머리없앰 옵션 체크
	BOOL m_bRemoveFace; // 얼굴없앰 옵션 체크
	BOOL m_bRemoveBody; // 몸통없앰 옵션 체크
	BOOL m_bRemoveArms; // 팔없앰 옵션 체크
	BOOL m_bRemoveLegs; // 다리없앰 옵션 체크
	BOOL m_bRemoveFoot; // 발없앰 옵션 체크

//	int m_UserShopLevel; // 상점 등급. 현재 안쓰임....
	vector<int> m_WearList; // 장식 아이템들의 Vnum 리스트.
};


// 점포상점 NPC 클래스( NKMob 클래스를 상속받아쓴다. 루틴은 완전 독립적, 멤버 변수들을 가져다쓰기위해 상속한다. )
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define GET_NPC_NAME(npc)		(LHGET_NPC_NAME(npc->m_nMobNum))
#define GET_NPC_NAME_VNUM(vnum)	(LHGET_NPC_NAME(vnum))
//#define GET_NPC_NAME_IDX(vnum)	()

#endif // _NK_MOB_H_
