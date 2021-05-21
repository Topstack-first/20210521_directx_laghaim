#ifndef _NK_MOB_H_
#define _NK_MOB_H_

#include "Config.h"

#include <list>
#include <vector> ///��������
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
	CBiped	  *pBpd[7];			// [10/23/2007 Theodoric] npc 2���� ���� �߰��� ���� 5���� 7�� �ø���.
	int		  idxSound[7];		// [10/23/2007 Theodoric] npc 2���� ���� �߰��� ���� 5���� 7�� �ø���.
	int       nSoundFrame[7];	// [10/23/2007 Theodoric] npc 2���� ���� �߰��� ���� 5���� 7�� �ø���.
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

	// �ʱ� ����
	void	InitSize(int nMaxCapNum, int nMaxBipedNum);
	void	SetBipedDir(char *dir_name);
	void	SetCapDir(char *dir_name);
	void	SetTexturePath(char *dir_name);

	// ȭ�� ����
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

	// state �� 0�϶��� Stand
	//    1 �϶��� Move
	//    2 �϶��� Attack
	//	  3 �϶��� Die
	//	  4 �϶��� Knock
	//	  5 �߰����� 1
	//	  6 �߰����� 2
	CBiped *m_pBiped[8];				// [10/23/2007 Theodoric] npc 2���� ���� �߰��� ���� 5���� 7�� �ø���.
	CCapsyong *m_pBound;

	int		m_MobDataIdx;				// CMobData (���鿡 ���� ����ü) �� ���� ��ġ ������.
	// �Ʒ��� ���� �ε����� ���� �������� �� �ʿ��� ���� �ƴϴ�.
	int		m_CurFrame, m_PrevFrame;
	int		m_nSoundIdx[8];				// [10/23/2007 Theodoric] npc 2���� ���� �߰��� ���� 5���� 7�� �ø���.
	int		m_nSoundFrame[8];			// �Ҹ��� �� ������.	// [10/23/2007 Theodoric] npc 2���� ���� �߰��� ���� 5���� 7�� �ø���.
	float	m_AniRate;
	float	m_fMoveSpeed;
	int		m_AttackFrame;
	int		m_DeadFrame;

	//IndexedTexture	*m_CapsyongTexture;

	// �ִϸ��̼�
	BOOL	m_bMoving, m_bAttack;
	float	m_dx, m_dz;
	float	m_tx, m_tz;

	// �ð� ���
	DWORD	m_dwTimeCapPrev, m_dwBaseTime;

	int		m_MobSkill;

public:
	CNkMob();
	CNkMob(CClrAlloc *pClrAlloc);
	virtual ~CNkMob();

	CCapsyong *m_pCapSyong;

	// ĳ������ Ư����.
	int		m_nMobNum;								// ���� ���� id (db)
	int		m_nVirId;
	WORD		m_wColorIdx;						// �÷� ���̺� �ε���
	DWORD		m_dwColorPick;						// �÷� �з�����.
	CClrAlloc	*m_pClrAlloc;
	double m_Vital;

	// �����Ϸ��� �ϴ� ���̳� ĳ���� ���̵�
	int		m_AttackChaId;
	int		m_AttackMobId;
	int		m_AttackPetId;
	void	SetAttackChaId(int chaId);
	void	SetAttackMobId(int mobId);
	void	SetAttackPetId(int petId);

	// �¾Ƽ� ����� ü��
	int		m_AttackVital;
	void	SetAttackVital(int vital);
	DWORD	m_LastAttackVitalTime;
	void	Attacked();

	// ���� ����߸��� ������
	//CItem	*m_pDropItem;
//	int	 m_DropItemIndex;
	int  m_DropItemVnum;
	int  m_DropItemX;
	int  m_DropItemZ;
	int  m_DropItemDir;
//	void SetDropItem(int item_index, int vnum, int x, int z, float dir);

	// Flag..
	int		m_Flag;

	// ���ΰ��� �ٰ����� �ֱ���!!!
	BOOL	m_bHeroAttCome;


	// ���� ��ġ�� ���� ����
	int		m_Height;
	float	m_wx, m_wy, m_wz, m_dir, m_toDir;
	int		m_FlyStone;

	BOOL	Move(float tx, float tz, int nFrameCount = 20);
	BOOL	Place(float tx, float tz, float dir, int nFrame);
	void	Stop(float tx, float tz, float dir);
	BOOL	Attack(float tx, float tz);
	BOOL	Magic(float tx, float tz);

	// ������ ȿ�� ����.
//	int		m_nAttackedType;
	int		m_bAttacked;
//	int		m_nAttackedFrame;
	int		m_nAttackedTime;
	void	SendMsg (int msg, int from_id = -1, int type = 0);

	// �ʱ�ȭ �Լ�
	void	SetInfo(int mob_num, int id, int nState=0);
	//void	SetMotionData(int MobNum, int nState);	// �� ���� �ε����� ���� ����. �������� ����
	void	SetInitPos(float x, float z, float dir);

	// ������ ����ϴ� �Լ�
	void	FrameMove(unsigned int nInc, BOOL bVisible = TRUE);

	// [2/21/2008 �����] �� �Ϲ� ���� ����Ʈ ���� �Լ�
	BOOL    Mob_NormalAttack_EffectFrame( int mob_Num, int effect_Num );
	DWORD hitpoints[10][3];
	void addhitpoint(DWORD num, DWORD attacker);
	bool ishit();
	// ������ �Լ�
	void	Render(BOOL bMouseOn = FALSE, BOOL bVisible = TRUE, BOOL bDontPick = FALSE);

	// ĳ���Ϳ� ���� ��� �Լ�
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

	// ���� ���õ� effects
	list<int>		m_listEffect;
	void			InsertEffectNum(int num);

	int m_MobAttackType;
	void SetAttackType(int attackType)
	{
		m_MobAttackType = attackType;
	}

	// ������ ����...
	int m_EffFrame;
	int m_Mutant;

	BOOL	m_bPet;
	bool	isControlLight;

////2003 ���� �迭�� �Ӽ� ȿ���� ��Ÿ���� ���� �Լ��� ����
	int m_MobClassType; //�迭��
	void SetClassType(int classType)
	{
		m_MobClassType = classType;
	}
	int m_MobAttributeType; //�̺�Ʈ ��(���͵��� �ݶ�) �Ӽ����� ��

	int m_FissionType;// �����Ǽ��� ���� ����
	void SetFissionType(int FissionType);

///2003 ���� ����(��ȭ,����) ȿ���� ��Ÿ���� ���� �Լ��� ����
	int m_ChaCondition; //��ȭ,����...
	int m_CancelState;//���� on,off
	void SetChaCondition(int Condition, int cancel);
	BOOL m_ShowConditionEffect;//����ȿ��
	int m_ChaCondition2; //��ȭ,����...
	int m_CancelState2;//���� on,off
	void SetChaCondition2(int Condition, int cancel);
	BOOL m_ShowConditionEffect2;//����ȿ��2
////2003 ���� å�ֹ��� Ÿ���� �Ǿ����� ȿ���� ��Ÿ���� ���� �Լ��� ����
	int m_ReceiveBook;

	void SetUseBookEffect(int book)
	{
		m_ReceiveBook = book;
	}

	BOOL m_bQuestMobDied;//0319
	BOOL m_bQuestMob;
////////
	int m_nFlyFrame;
	CDropStar *m_pDropStar; // �߹ؿ� �������� ��

	// �ʵ�
	CBiped *GetBiped(int num)
	{
		return m_pBiped[num];
	}
	int GetCurFrame()
	{
		return m_CurFrame;
	}

	// �۶� ���� ���� ȿ���� ���� ������
	int		m_RunSmokeFrame;

	Deco_Particle*	m_NkMob_Particle;
	bool			m_bDrawEffects;			// Ȥ�� ���߿� �� Ư������ ������� �� ȿ���̶��...(ex ��������?)

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
	// nate 2004 - 4 : quest level - ���� ���
	int m_nLevel;
	void SetLevel( int nLevel )
	{
		m_nLevel = nLevel;
	}
//=================================================================================
	BOOL m_bPoisoning;	// nate 2005-11-14 : ������ - npc�� ������ ������
	BOOL IsTrap();
	BYTE m_byInsertEffect;	// nate 2006-01-24 : ����Ʈ ���Կ��� üũ(��Ʈ������ ���� �з�)

	void MoveSkill(float tx, float tz, int nSkillIndex = 0); // �˹��� ���� �Լ�. ĳ���� �ʲ� �貸��.
	float   m_fDash_Frame; // �˹��� ���� ����.
	int m_NowMoveSkill; // �̵��� ��ų ���� ����.(���ʹ� ��� ������Ʈ�� ���� ���ϰ� �̰ɷ� �����ؼ� �˹�� �����ش�.)

	// mungmae 2006/10/12 ���� ������ �� ǳ����.
	DWORD m_LastChatTime;
	BOOL m_bShowMobChatPopup;
	CAlphaPopUp* m_pMobChatPopup;

	void InitMobChatPopup(int width, int height);
	void DrawMobChatPopup();
	void SetMobChatMsg(char *chat_msg);

	int m_RegeningType; // ���� �߿� ���õǴ� ���� ���� ����. �� ���� ���õǾ�������...�������ΰŴ�. ///�δ�1

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

float GetMobExceptioncale( CNkMob *DestMob ); // �������� Ŭ���̾�Ʈ �������� ����Ǵ� ��� ������ ���� �������ִ� �Լ�.

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
#define	MOB_SPECTER				417	// nte 2005-10-25 : ������ �߰�

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
//2003 ���� �Ӽ� ���� define
#define MOB_ATTACK_ACCUMULATE		3
#define MOB_ATTACK_FIRE				4



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// �������� NPC Ŭ����( NKMob Ŭ������ ��ӹ޾ƾ���. ��ƾ�� ���� ������, ��� �������� �����پ������� ����Ѵ�. ) ///��������
class CUserShopNpc:public CNkMob
{
public:
	CUserShopNpc();
	CUserShopNpc( CClrAlloc *pClrAlloc );
	~CUserShopNpc();

	void	FrameMove( BOOL bVisible = TRUE );

	void	Render( BOOL bMouseOn = FALSE, BOOL bVisible = TRUE, BOOL bDontPick = FALSE );


	DWORD m_MasterIndex; // ���� ������ �ε���. ��Ŷ�� ������ �̰ɷ� ������ �Ѵ�.
	char m_strMasterName[100]; // ���� ���� �̸�.
	char m_strUserShopMent[256]; // ���� ��Ʈ ��Ʈ��. �Ӹ����� ǥ��������Ѵ�.

	BOOL m_bRemoveHair; // �Ӹ����� �ɼ� üũ
	BOOL m_bRemoveFace; // �󱼾��� �ɼ� üũ
	BOOL m_bRemoveBody; // ������� �ɼ� üũ
	BOOL m_bRemoveArms; // �Ⱦ��� �ɼ� üũ
	BOOL m_bRemoveLegs; // �ٸ����� �ɼ� üũ
	BOOL m_bRemoveFoot; // �߾��� �ɼ� üũ

//	int m_UserShopLevel; // ���� ���. ���� �Ⱦ���....
	vector<int> m_WearList; // ��� �����۵��� Vnum ����Ʈ.
};


// �������� NPC Ŭ����( NKMob Ŭ������ ��ӹ޾ƾ���. ��ƾ�� ���� ������, ��� �������� �����پ������� ����Ѵ�. )
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define GET_NPC_NAME(npc)		(LHGET_NPC_NAME(npc->m_nMobNum))
#define GET_NPC_NAME_VNUM(vnum)	(LHGET_NPC_NAME(vnum))
//#define GET_NPC_NAME_IDX(vnum)	()

#endif // _NK_MOB_H_
