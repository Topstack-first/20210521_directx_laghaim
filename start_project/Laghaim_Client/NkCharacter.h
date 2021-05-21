#ifndef _NK_CHARACTER_H_
#define _NK_CHARACTER_H_

#include "Config.h"

#include <set>
#include <list>
using namespace std;
#include "IndexedTexture.h"

#include "ChaSta.h"
#include "Item.h"
#include "DecoObj.h"
#include "FxRegenBar.h"
#include "guild_data.h"
#include "info_define.h"

//==================================
// nate 2004 - 4
// Crash_Miss
#define CRASH_MISS_COUNT					3
//==================================

#define RACE_BULKAN							0
#define RACE_KAILIPTON						1
#define RACE_AIDIA							2
#define RACE_HUMAN							3
#define RACE_FREAK							4
#define RACE_PEROM							5
#define NUM_RACE							6

#define SEX_MALE							0
#define SEX_FMALE							1
#define NUM_SEX								2

#define NKCHA_FACE							0
#define NKCHA_BREAST						1
#define NKCHA_ARMS							2
#define NKCHA_LEGS							3
#define NKCHA_FEET							4
#define NKCHA_HAIR							5
#define CAPSYONG_NUM						6

#define AUTO_PROTECTING_NUM					20711

#define PACK_NUM							5
#define SLOT_X_NUM							8
#define SLOT_Y_NUM							6

#define EXG_SLOT_X_NUM						6
#define EXG_SLOT_Y_NUM						4

#define QUICK_SLOT_X_NUM					6
#define QUICK_SLOT_Y_NUM					2
#define QUICK_SLOT_NUM						(QUICK_SLOT_X_NUM * QUICK_SLOT_Y_NUM)

#define QUICK_SLOT_PAGE						1

#define ATTK_NORMAL							-1
#define ATTK_HIGH							0
#define ATTK_MIDDLE							1
#define ATTK_LOW							2

#define SWIM_HEIGHT							(82.0f)
#define WATER_HEIGHT						(93.0f)

#define HIDE_SHOWN							0
#define HIDE_HIDING							1
#define HIDE_HIDDEN							2
#define HIDE_RISING							3

#define TEAM_BATTLE_NONE					0
#define TEAM_BATTLE_AHEAD					1
#define TEAM_BATTLE_AMEMBER					2
#define TEAM_BATTLE_BHEAD					3
#define TEAM_BATTLE_BMEMBER					4
#define TEAM_BATTLE_AFLAG					5
#define TEAM_BATTLE_BFLAG					6
#define TEAM_BATTLE_GUILDPK					9

#define CHA_FLY_FRAME						60

#define MAX_TRACELINE_FRAME					50		// Į, ���� �ִ� ���� ������ 

#define MAX_PARTY_NUM_BOSSRAID_FINAL		16		// �������̵� ���� ���� �������� 50������
#define MAX_PARTY_NUM_BOSSRAID				16		// �������̵� �������� ��Ƽ �ο����� 16������ �����Ѵ�. ĳ���� Ŭ���������� ���⶧���� ����� ��.
#define MAX_PARTY_NUM						16		// ��Ƽ �ο��� ���� ������. ĳ���� Ŭ���������� ���⶧���� ����� ��.

#define MAX_MULTITARGET						10		// ���̺긮�� �޺��ÿ� �������ִ� ��ü�� �Ѱ� ///�����޺�
#define MULTITARGET_KIND_MOB				1		// ���̺긮�� �޺��ÿ� ������ ��ü�� ������ ĳ������.  ///�����޺�
#define MULTITARGET_KIND_CHAR				2		// ���̺긮�� �޺��ÿ� ������ ��ü�� ������ ĳ������.  ///�����޺�


// [2008/6/17 Theodoric] ��Ʈ ȿ��
typedef enum SETITEM
{
	SETITEM_NONE = -1,
	LV300_BULKAN = 0,
	LV300_KAILIPTON,
	LV300_AIDIA,
	LV300_HUMAN,
	LV300_HYBRIDER,
	RARE_BULKAN,
	RARE_KAILIPTON,
	RARE_AIDIA,
	RARE_HUMAN,
	RARE_HYBRIDER,
	HEROSET,	
	T_RARE_BULKAN,
	T_RARE_KAILIPTON,
	T_RARE_AIDIA,
	T_RARE_HUMAN,
	T_RARE_HYBRIDER,
	GM_HEROSET,
	SETITEM_EVENT1,
	SETITEM_EVENT2,
	SETITEM_EVENT3,
	RARE_BULKAN1,
	RARE_KAILIPTON1,
	RARE_AIDIA1,
	RARE_HUMAN1,
	RARE_HYBRIDER1,
	RARE_PEROM1,
	
	SETITEM_MAX
};

typedef enum TraceLine_Type
{
	NONE = 0,
	TRACELINE_NORMAL,
	TRACELINE_ATTACK,
	TRACELINE_JUMPATTACK,
	TRACELINE_ROTATEATTACK,
	TRACELINE_SHOOTATTACK,
	TRACELINE_ATTACK_BLEED,
	TRACELINE_BURNINGCRASH,
	TRACELINE_DASHSLASH,			///�����޺�
	TRACELINE_SLASHSHADOW,			///�����޺�
} TraceLine_Type;

//////////////////////////////////////////////////////////////////////////
// Ŭ���� �� ����.
class CGuildData;
struct EffectSort;					///�� �� ��
class CBiped;
class CAniMatrix;
class CCsyObj;
class CLolos;
class CCapsyong;
class CShadowRect;
class CClrAlloc;
class CGoodShadow;
class CControlScore;
class CNkMob;
class CAlphaPopUp;
class CDropStar;
class CRectPlane;					// ��ī���� ���� ĳ�� �ٴ� �׸���.
class CFxSiren;						// wanted!!

class CNkCharacter
{
	// define variable or attribute
	// ������ �޸𸮴� ��ü������ 32bit�� �����Ǿ��ֱ� ������
	// �޸� ������ ���ؼ��� �׻� 8����Ʈ�� ������ �ξ�� �Ѵ�.
	// ������ 8����Ʈ�� ���� �ʴ� �������� ��쿡�� ������
	// ������ ������ �����Ͽ� 8byte������ �Ͽ��� �Ѵ�.
	// ������ �迭�� �����Ѵ�.
private:
	//===============================
	//fpe���� ��ġ�� ������ ����
	//===============================
	double							m_AniRate;
	int								m_CurFrame;
	DWORD							m_dwBaseTime;
	int								m_PrevFrame;
	DWORD							m_dwTimeCapPrev;
	//Motion State
	unsigned short					m_nMotionState;
	float							m_TargetHeight;	// ���� ��� ����.
	int								m_nFlyFrame;

	CBiped				*			m_Biped;
	CCapsyong			*			m_CapSyong[CAPSYONG_NUM];
	BOOL							m_bShow[ CAPSYONG_NUM + 2 ];

	//	BOOL						m_bEddy;
	// �ؽ�ó
	//	IndexedTexture	*			m_CapsyongTexture;
	//	IndexedTexture	*			m_TraceTexture;
	//	IndexedTexture	*			m_LightswordTexture;
	//	int							m_LightswordFrame;

protected:
	static float					m_fRunSpeed;
	//	static float	m_fWalkSpeed;
	// nate 2004 - 7 �۷ι� ShadowRect���
	//	CShadowRect		*m_pShadowRect;
	CGoodShadow			*			m_pGoodShadow;
	CClrAlloc			*			m_pClrAlloc;
	// ��ǿ� ���õ� ����
	BOOL							m_bMoving, m_bAttack;
	float							m_dx, m_dz;			// �̵� ���� ����.
	float							m_tx, m_ty, m_tz;	// target (x, z) ��ǥ.
	// Score ����� ���� ������ ������
	CControlScore		*			m_pScoreWnd;
	CFxSiren			*			m_pSiren;			// �������.
	// [2008/6/20 Theodoric]
	int								m_BossRaidHeroEffectIndex;
	SETITEM							m_SetItem;
	SETITEM							m_SetItemEventWear;
	int								m_SvrChannelNum;

public:
	bool							m_bGuildPK;
	bool							m_bChakramatt;
	char							m_sEnemyGName[100];
	// ĳ������ Ư����.
	int								m_nCharIndex;
	WORD							m_wColorIdx;		// �÷� ���̺� �ε���
	DWORD							m_dwColorPick;		// �÷� �з�����.
	char				*			m_pstrName;
	char				*			m_pstrGuildName;
	char				*			m_pstrGuildGradeName;
	int								m_GuildIndex;
	int								m_GuildGradeIndex;
	char							m_Race;
	char							m_Sex;
	char							m_Hair;
	int								m_nUserIndex;		//�����ε���
	CGuildData			*			m_pMyGuilddata;

	//===============================
	//fpe���� ��ġ�� ������ ����
	//===============================
	// ĳ������ Status
	double							m_Str;
	double							m_Int;
	double							m_Dex;
	double							m_Con;
	double							m_Cha;
	__int64							m_Exp;
	__int64							m_NeedExp;
	double							m_Stamina;
	double							m_Epower;
	double							m_MaxVital;		//
	double							m_Mana;
	double							m_LevelupPoint;
	double							m_MaxEpower;

	__int64							m_bpexp;
	__int64							m_bpmax;

	int								m_bBrLv;


	double							m_Vital;		//���丶�콺 �������� �ڸ� ����
	double							m_MaxMana;
	double							m_MaxStamina;
	int								m_Level;
	int								m_BpdRace;
	__int64							m_Money;
	double							m_Fame;
	// ĳ������ ����2
	int								m_nManaShield;
	int								m_nElectricShield;
	int								m_nSafetyShield;
	//��Ʈ���� ��Ʋ ����Ʈ
	int								m_nBattlePoint;
	// ������ ����
	CItem				*			m_Wearing[WEARING_NUM];			// equipment(���)
	CItem				*			m_EventWearing[WEARING_NUM];	///�̺�Ʈ�������� �ý��� ����.
	CItem				*			m_ExtraSlot;					// �������.
	//Display Event Wear
	BOOL							m_bEventWearFlag;				// [2008/7/4 Theodoric] �������� �̺�Ʈ ������ �ʺ� �� �ִ� ���ñ��� �޶�� �ؼ� �߰�.
	// �ѳѹ��� ���Ḯ��Ʈ ����
	// ������ ��ü( ���Ḯ��Ʈ )
	CItem				*			m_Inven[PACK_NUM];				// inventory
	CItem				*			m_ExgInven;						//
	CItem				*			m_QuickSlot[QUICK_SLOT_NUM*QUICK_SLOT_PAGE];	// quick slot
	int								m_QuickCount[QUICK_SLOT_NUM*QUICK_SLOT_PAGE];	// �κ��� ����ִ� ���� �������� ����.
	BOOL							m_bQuickSlotLock[QUICK_SLOT_NUM*QUICK_SLOT_PAGE];// ��� ����.
	// �� ������ ������ �ε�����
	int								m_Slot[PACK_NUM][SLOT_X_NUM][SLOT_Y_NUM]; // �κ�
	int								m_ExgSlot[EXG_SLOT_X_NUM][EXG_SLOT_Y_NUM];
	int								m_CurPackNum;
	BOOL							m_bExgLock;
	int								m_ActNum;
	int								m_DeadFrame;
	// ����� ��ġ�� ���� ���� : world coordinate
	float							m_wx;
	float							m_wy;
	float							m_wz;
	float							m_dir;
	float							m_toDir;
	D3DMATRIX						m_matWorld;
	float							m_fScaleSize;

	int								m_nLaghaimPoint; // [2008/5/16 Theodoric] BP -> LP �� �ٲ�

	int								m_nGHPoint;		// �׷��� ����� ����Ʈ, ���ġ�� ������� ���� �� ���� ����Ʈ�̴�.
	bool							isShieldLoad;
	// ĳ������ �׸��� ũ�� ������ ���ؼ�.
	static int						m_nCurHour;
	int								m_FlyLevel;
	BOOL							m_bOwnAmbient;
	DWORD							m_dwOwnAmbient;
	// ���� �����̴� ���.
	BOOL							m_bRun;
	// �����ϰ� �ִ� ���̳� ĳ���� ���̵�
	int								m_AttackChaId;
	int								m_AttackMobId;
	// �¾Ƽ� ����� ü��
	int								m_AttackVital;
	DWORD							m_LastAttackVitalTime;
	BOOL							m_bCombat;			// ����������
	//BOOL							m_bHiding;
	// ������ �� ���̳� ĳ���� ���̵�
	int								m_MagicChaId[10];
	int								m_MagicChaCount;
	int								m_MagicMobId[10];
	int								m_MagicMobCount;
	int								m_MagicNum;
	// ���ΰ��� �ٰ����� �ֱ���!!!
	BOOL							m_bHeroAttCome;
	BOOL							m_bPlace;
	float							m_fPlaceDir;
	// ������ �Լ�
	BOOL							m_bVisible; // �������� �ø��󿡼� ���̴°��� �÷���. 06-01-03 ���� �߰�(���λ��� ��ĭ ������ �ʿ�����.)
	// ���λ���
	CAlphaPopUp			*			m_pChatPopup;
	CAlphaPopUp			*			m_pUserShopPopup;
	// ���� ���� �̸��� ��Ʈ�� ���� �˾�(���� m_pChatPopup�ʹ� ���� ���°� ������� ���ؼ� ���� ��) ///���λ���
	// ��Ÿ flag
	DWORD							m_LastChatTime;
	BOOL							m_bShowChatPopup;
	BOOL							m_bMyParty;
	BOOL							m_bMyGuild;
	BOOL							m_bOppGuild;
	BOOL							m_bCrystalOwner;
	BOOL							m_bCrystalGuild;
	int								m_nTeamMemGrade;		// 0 : ������ �ƴϴ�. 1 : A���� 2: A���� , 3:B����. 4: B����
	BOOL							m_bShowMatrixMark;
	bool							m_bCatchFlagShow;
	///////////////////////////////////////////////////////////////
	// picking buffer���� ����� ������ü ����Ÿ.
	static CLolos					m_CubeLolo;
	static DWORD					m_dwAmbient;
	CDropStar			*			m_pDropStar;			// �߹ؿ� �������� ��
	int								m_MultiShot;
	int								m_NextMultiShot;
	int								m_RenderMultiShot;
	// �ʰ� ������ ����.
	int								m_HideStatus;			// �޸��� ���� ���� ��ų
	DWORD							m_HideTime;
	int								m_GuildLevel;			// ��� ����
	///2003 ĳ���� Ư��ȿ���� ���� ������ �Լ�.
	int								m_ChaAttackType;		//Ư������ (ī��Ʈ,ũ��Ƽ��...)
	int								m_ChaCondition;			//ĳ���� ����(��ȭ,����...)
	int								m_CancelState;			//���� on,off
	int								m_ChaCondition2;		//ĳ���� ����(��ȭ,����...)
	int								m_CancelState2;			//���� on,off
	int								m_InChaSkill;			//ȭ�鿡 ���� ĳ���� �ɷ��ִ� ���ӽ�ų.
	int								m_InChaAidiaSkill;		//ȭ�鿡 ���� ĳ���� �ɷ��ִ� ���̵�� ���ӽ�ų.
	//BOOL							m_UseBook,m_ReceiveBook;//å ��� ȿ�� ,å���� �´� ȿ��
	int								m_UseBook;				//���⿡ å������ ������ ���� ���δ�.
	BOOL							m_ShowConditionEffect;	//����ȿ��
	BOOL							m_ShowConditionEffect2; //����ȿ��
	BOOL							m_ShowSkillEffect;		//���ӽ�ųȿ��
	BOOL							m_ShowAidiaSkillEffect;	//���̵�� ���ӽ�ųȿ��
	BOOL							m_Accumulate;			//����.
	BOOL							m_DoubleAttack;			//�޸��� �������
	int								m_Chaotic_Grade;		//ī�� ���.
	int								m_Free_Pk;				//���� ������.
	int								m_Chaotic_Num;			// ī�� ��ġ.
	BOOL							m_bPk_render;			//������ ����� �����ÿ� ���� �׷��ֱ� 1112
	BOOL							m_bMoonEventEffect;		//0212 �뺸�� �̺�Ʈ
	BOOL							m_ShowMoonEventEffect;	//�̺�Ʈ ȿ�� ����.
	BOOL							m_bNonPK;
	// ���� ���̵�.
	int								m_iPartysListId[MAX_PARTY_NUM_BOSSRAID_FINAL];			// ��Ƽ�� id
	float							m_fPartysVitalList[MAX_PARTY_NUM_BOSSRAID_FINAL];		// vital
	int								m_iPartysListXpos[MAX_PARTY_NUM_BOSSRAID_FINAL];		// Xpos
	int								m_iPartysListYpos[MAX_PARTY_NUM_BOSSRAID_FINAL];		// Zpos
	int								m_iMaxPartysNum;		// ���� ��Ƽ�� ��
	int								mi_Snifing_GAP_Frame;	// �޸��� �������� ��ų�� ����
	int								mb_Snifing;
	float							m_fDash_Frame;			// Lyul_2005 ����ũ �뽬�̵� Ƚ�� (��Ȯ�� ������ ���� float)
	float							m_fFlying_frame;		//mungmae: 2005/11/16 �ö��� Ŭ����
	float							m_fShadow_frame;		//mungmae: 2005/11/30 ������ Ŭ����
	BOOL							m_bstone;				// ����� ������ ����߳�. Framemov���� ���� ������ ���� Ȱ��ȭ �ȴ�.
	// �������� Dstone �� ������� �ٽ� ���ƿ´�.
	BOOL							m_bBleed;				// ���� ���� ��ų�� �ߵ����ΰ�.
	BOOL							m_bresstone;			// ����� ������ ������ �غ� �ƴ°�.
	int								m_resmobidx;			// ������ ���� �ε���
	int								m_resmobvnum;			// ������ ���� vnum
	int								m_resmobx;				// ������ ���� ��ǥ
	int								m_resmobz;				// ������ ���� ��ǥ
	int								m_resmobdir;			// ������ ���� �𷢼�
	BOOL							m_bhaste;				// ����ũ�� ���̽�Ʈ�� ������ ���� ��� ���� ��.
	BOOL							m_bShowPartysVital;
	BOOL							m_bRecivedPartysInfo;
	BOOL							m_bIsParty;
	// particle
	Deco_Particle		*			m_NkCha_Particle;
	D3DLVERTEX						m_TraceLine[(MAX_TRACELINE_FRAME * 4) + 4]; // Į ���밡 �׷��� ���ؽ� ����
	int								m_drawLineNum;								// ������ �׷��� ���ؽ� ����
	int								m_DrawWeaponEffects;
	int								m_CurAttackType;
	BOOL							m_bFlagOwner[4];
	BOOL							m_bFlagGuild[4];
	//===========================================================
	// nate 2004 - 4 : Crash_Miss ����
	// ����ü�� ����
	int								m_nElapTime;
	DWORD							m_dwElapTime;
	CAlphaPopUp			*			m_pCrash_MissPopup[ CRASH_MISS_COUNT ];
	BYTE							m_byShowCrash_Miss;
	BYTE							m_byShowGiftMessage;
	CAlphaPopUp			*			m_pGiftMessage;

	BOOL							m_bComboReady;			//mungmae-2005/04/11 - ����: ��ĭ ��� ��ũ���� �߿� �����̾ �����ϰ� �� ����� ����(�޺�ó��)
	BOOL							m_bComboSucceed;		//mungmae-2005/04/12 - ����: ��ũ����, �����̾� �޺� ���� ����
	BOOL							m_ShowReflection;		//mungmae-2005/04/20 - ����: ���÷��� ȿ���� ����
	int								m_nComboFrame;			//mungmae-2005/05/03 - ����: �޺� ��ų�� ������ �������� ������ ������ ����

	DWORD							m_MultiTargetArray[MAX_MULTITARGET]; // ���̺긮�� �ظ� �޺��� Ÿ�� �ε����� �����س��� �ʿ䰡�־... ///�����޺�
	POINT							m_MultiTargetPos[MAX_MULTITARGET]; // Ÿ�� ��ǥ.(�Ź� ã������ �����ɸ��������� ��ǥ�� ����صд�.) ///�����޺�
	int								m_MultiTargetNum;		// ���̺긮�� �ظ� �޺��� Ÿ�� ������ �����س��� �ʿ䰡�־... ///�����޺�
	BYTE							m_MultiTargetKind;		// ������ ĳ�������� �÷���. ///�����޺�

	BYTE							m_byBurningStep;		// nate 2005-07-29 : Burning Crash - ��ų �ܰ� ����
	int								m_nSkillLevel;			// nate 2005-06-17 : ��ų�� ���õ�( ��ų����Ʈ )
	int								m_nSkillStep;			// nate 2005-06-27 : �޸�-���ΰ���, ��ĭ-����ũ�����ܰ�
	int								m_nFailStep;			// mungmae 2007/1/4 : ��ų ���
	int								m_nOldFrame;			// nate 2005-07-21 : �޼����� ������ �Ǵ�
	BYTE							m_byShowLIfeExpantion;	// nate 2005-07-24 : Life Expansion - ������ ����Ʈ ���̱�
	//mungmae-2005/06/23
	//����: ����콺���� ���� �ɶ������� ��Ȱ ������
	CFxRegenBar			*			m_pRegenbar;
	BOOL							m_bDrawRegenbar;
	DWORD							m_dwRegenTime;
	int								m_nRegenIndex;
	BOOL							m_bAttacked;			//mungmae 2005-07-13 : �ڱ� �ɸ��Ͱ� ���ݹް��ֳ�?
	int								m_nAttVital;			//mungmae 2005-07-23 : �ڱ� �ɸ����� ����Ż

	BOOL							m_bCloaking;			//mungmae-2005/07/26 : ���̺긮�� ���̵�
	BOOL							m_bCloaking_Waring;		//mungmae-2005/07/26 : ���̺긮�� ���̵� Ǯ�������� �����Ÿ� ���
	BOOL							m_bPoisoning;			// nate 2005-11-14 : ������ - ��������� ����
	BOOL							m_bShowAdrenaline;		//mungmae-2005/11/21 - ����: �Ƶ巹���� ȿ���� ����
	float							m_fSubAttackTime;
	// ���λ����� ���� �߰�. ///���λ���
	int								m_UserShopLevel;		// ���� ������ �����°�?
	char							m_strUserShopMent[256]; // ���� ��Ʈ ��Ʈ��. �Ӹ����� ǥ��������Ѵ�.
	BOOL							m_bYanQuest;			//mungmae 2005/12/12 �� ����Ʈ(���� ����Ʈ)���� ����
	BOOL							m_bSecialQuest;			//mungmae 2006/06/02 205~275����� ����Ʈ ����
	BOOL							m_bSwapWeapon;			// mungmae-2006/06/21 : �������� �߿� ���� �ٲٱ�.
	int								m_HaveCash;				// ���� ĳ�� ///�������
	DWORD							m_dwComboTick;			///�����޺�
	D3DVECTOR						m_vMvSkillStartPos;		// ���� ��ų�� �ߵ��Ǵ� ������ ��ġ. ///�����޺�
	BOOL							m_bSpiritModeEffect;
	BOOL							m_bProtectionShield;
	DWORD							m_dwReturnTime;
	DWORD							m_dwResurrectionTime;
	DWORD							m_dwResurrection_StartTime;
	BOOL							m_bResurrectionSkill;
	int								m_nResurrection_ChaId;
	int								m_NvNWin;				///�������¸� Ƚ��.
	///�������
	BOOL							m_bNowFreePVP;			// ������� ������ ����
	DWORD							m_dwPvpDestIndex;		// ��û���� �ε���.(�ڽ��� ������� �߿��� ����.)
	D3DVECTOR						m_vPvpRingPos;			// ������� ���� ǥ���ϴ� ���� �߽� ��ǥ.
	DWORD							m_dwRingCreateTick;		// ���� ������ �ð�.
	BOOL							m_bchaDontAttackEffect;
	int								m_nGuildNameLine;
	bool							m_nHaveGuildMark;

	bool							m_bHellDowning;

	BYTE							m_SirenColorFlag;
	// ĳ���Ϳ� ���õ� effects
	set<int>						m_listEffect;	// constructor & destructor & function
public:

	CNkCharacter(CShadowRect *pShadowRect= NULL, CGoodShadow
				 *pGoodShadow = NULL, CClrAlloc *pClrAlloc=NULL);
	~CNkCharacter();

	void							CreateSiren(int color = 0);
	void							Siren_Switch(int switch_flag, int color = 0);
	void							Siren_On(int color = 0);
	void							Siren_Off(int color = 0);
	CGuildData			*			GetMyGuildData();
	BOOL							MakeMyGuildData();
	void							DeleteMyGuildData();
	int								GetMyGuildLevel();
	int								GetGuildLevel();
	int								GetMyGuildIndex();
	int								GetMyGuildTotalMemberCnt();
	int								GetMyGuildGeneralMemberCnt();
	int								GetMyGuildRecomWaitCnt();
	int								GetMyGuildGeneralWatiCnt();
	char				*			GetMyGuildName();
	char				*			GetInvasionAttackGuild();
	char				*			GetInvasionDefenseGuild();
	void							Set_GuildName(char *szName);
	void							Set_GuildIndex(int nIndex);
	void							Set_GuildLevel(int nLevel);
	void							Set_GuildMyLevel(int nLevel);
	void							ResetGuildMyInfo();
	int								CheckInvasionGNameType(char* guildname);	 // �������� �˻�
	t_Info							GetinvasionRemainTime();
	t_Info							GetMyGuildRevisionTime();
	// Status ����
	void							SetScoreWnd(CControlScore *pScoreWnd)
	{
		m_pScoreWnd = pScoreWnd;
	}
	void							SetLevel(int level);
	void							SetVital(int vital, int max_vital);
	void							SetMana(int mana, int max_mana);
	void							SetStamina(int stamina, int max_stamina);
	void							SetEpower(int epower, int max_epower);
	void							SetAbil(int str, int intel, int dex, int con, int cha);
	void							SetStr(int str);
	void							SetInt(int intel);
	void							SetDex(int dex);
	void							SetCon(int con);
	void							SetCha(int cha);
	void							SetExp(__int64 exp);
	void							SetNeedExp(__int64 exp);
	void							SetLevelupPoint(int point);
	void							SetMoney(__int64 money);
	void							SetCash( int Cash );			///�������
	void							SetFame(int money);				//�İ���
	//1112
	void							SetPenalty();
	void							SetDefense();
	void							SetGuildName(char *guild_name);
	void							SetGuildGrade(char *guild_grade_name);
	void							SetGuildIndex(int guildindex)
	{
		m_GuildIndex = guildindex;
	}
	void							SetManaShield(int nShield)
	{
		m_nManaShield = nShield;
	};
	void							SetElectricShield(int nShield)
	{
		m_nElectricShield = nShield;
	};
	void							SetSafetyShield(int nShield)
	{
		m_nSafetyShield = nShield;
	};
	void							SetAmbient(BOOL bEnable, DWORD dwAmbient)
	{
		m_bOwnAmbient = bEnable;
		m_dwOwnAmbient = dwAmbient;
	};
	// ������ ����...
	void							CountQuickAll();
	void							CountQuickBySlot(int slot_num);
	void							SetCurrentHour(int hour)
	{
		m_nCurHour = hour;
	};
	void							SetCharName(const char* name);
	void							SetInfo(int id, const char *name, char *guild_name = NULL, char* guild_grade_name = NULL);
	void							SetFlyLevel(int flyLevel);
	BOOL							CanFly()
	{
		return (m_FlyLevel > 0);
	}
	//////////////////////////////////////////////////////////////////////////
	// Attack ���� �Լ�.
	void							SetAttackChaId(int chaId, BOOL bPetAttack = TRUE );
	// �굵 ���ݽ�ų�� �÷��׸� �߰�. (�Ϻ� ��ų�� ������ �Ƚ�Ű�� �ϱ����ؼ�..) 06-01-31 ����
	void							SetAttackMobId(int mobId);
	void							SetAttackVital(int vital);
	void							Attacked();
	//////////////////////////////////////////////////////////////////////////
	// Magic ���� �Լ�.
	void							SetMagicCha(CNkCharacter *pCha, int magicNum);
	void							AddMagicCha(CNkCharacter *pCha);
	void							SetMagicMob(CNkMob *pMob, int magicNum);
	void							AddMagicMob(CNkMob *pMob);
	//////////////////////////////////////////////////////////////////////////
	// motion ���� �Լ�.
	BOOL							Move(float tx, float tz);
	BOOL							Place(float tx, float tz, float dir, int nFrame);
	void							Stop(float tx, float tz, float dir);
	void							AttackChakram(float tx, float ty, float tz);
	void							Attack(float tx, float ty, float tz, int attkType = -1);
	void							Magic(float tx, float ty, float tz, int act_num);
	void							Kill();

	//////////////////////////////////////////////////////////////////////////
	// �ʱ�ȭ �Լ�
	void							MakeBasicBody(int nRace, int nSex, int nHair);
	void							MakeBasicBody();
	void							SetBiped(CBiped *pBiped);
	CBiped				*			GetBiped()
	{
		return m_Biped;
	}
	// 2007 �� �߰�. ������ �� ����Ʈ�� �ʿ��ؼ�...
	void							SetCapsyong(CCapsyong *pCapsyong, int capsyong_num);
	void							SetInitPos(float wx, float wy, float wz);
	void							SetInitPos(float wx, float wz);
	void							FrameReset()
	{
		m_CurFrame = 0;
		m_PrevFrame = 0;
	}
	int								Get_m_CurFrame()
	{
		return m_CurFrame ;
	};

	//////////////////////////////////////////////////////////////////////////
	// ������ ����ϴ� �Լ�
	void							FrameMove(unsigned int nInc, BOOL bVisible = TRUE,
			CNkMob *pTargetMob = NULL, CNkCharacter *pTargetCha = NULL);
	BOOL							EventWearingFrameMove( int where ); // Ư�� ��ġ�� �̺�Ʈ ������ �׸���. ///�̺�Ʈ��������

	DWORD hitpoints[10][3];
	void							addhitpoint(DWORD num, DWORD attacker);
	bool							ishit();
	//////////////////////////////////////////////////////////////////////////
	// Render �Լ�.
	BOOL							IsAlphaWear( int w, bool bEvent );
	BOOL							RenderAlphaWear( CItem* pItem, BOOL bLow );
	BOOL							Render( LPDIRECT3DDEVICE7 lpd3dDevPick, BOOL bMouseOn = FALSE, BOOL bVisible = TRUE);
	BOOL							EventWearingRender( int where ); // Ư�� ��ġ�� �̺�Ʈ ������ �׸���. ///�̺�Ʈ��������

	//////////////////////////////////////////////////////////////////////////
	// ĳ���Ϳ� ���� ��� �Լ�
	void							SetMotionState(unsigned short nState, int act_num = 0);
	unsigned short					GetMotionState()
	{
		return m_nMotionState;
	};
	int								GetActNum()
	{
		return m_ActNum;
	}
	int								GetDeadFrame()
	{
		return m_DeadFrame;
	}
	BOOL							IsMoving()
	{
		return m_bMoving;
	}
	BOOL							IsAttacking()
	{
		return m_bAttack;
	}
	BOOL							IsShowChatPopup()
	{
		return m_bShowChatPopup;
	}
	BOOL							IsDead()
	{
		return (m_nMotionState == CHA_DEAD || m_Vital <= 0);
	}
	//BOOL							IsSwimming()								{ return (m_nMotionState == CHA_SWIM || m_nMotionState == CHA_SWIM_STAND /*|| m_wy =< SWIM_HEIGHT*/) ; }

	//////////////////////////////////////////////////////////////////////////
	// Lyul Skill
	// 2004.12.2
	// ���ݽ�ų�� ��� �߰� �� ��� �ȴ�.
	// ���� ���������� ������� ������ �米�� ���Ͽ��� ���� ���� -_-
	BOOL							IsCantMove();	// ��ų ��������� �Ǵ�.
	void							SetCombat(BOOL bCombat)
	{
		m_bCombat = bCombat;
	}
	void							ToggleCombat()
	{
		m_bCombat = !m_bCombat;
	}
	BOOL							GetCombat()
	{
		return m_bCombat;
	}
	//void							SetHiding(BOOL bHiding)						{ m_bHiding = bHiding; }

	//-------------------------------------------------------------------------
	// ResetMotionState()
	//-------------------------------------------------------------------------
	// ���� Motion State �� �ʱ� ������� Reset
	// 2004.03.09 - �����Ϸ�
	// Lyul / ����(TAB ���� )
	void							ResetMotionState()
	{
		if(m_nMotionState < CHA_ATTACK)
		{
			SetMotionState(m_nMotionState, m_ActNum);
		}
	}
	void							ToggleMovingState()
	{
		m_bRun = m_bRun ? FALSE:TRUE;
	}

	//////////////////////////////////////////////////////////////////////////
	// ������ ���� �Լ�
	// [6/1/2007 Theodoric] ������ ���� :: �Լ� �Ķ���� ������ ����
	void							Wear(int where, int index, int vnum, int gem_num = 0, int special = 0, int special2 = 0,
										 int UpEndur = 0, int MaxUpdEndur = 0, int Endur = 0, int MaxEndur = 0); ///�Ź���ȭ�ý���
	void							EventWear(int where, int index, int vnum, int gem_num = 0,
			int special = 0, int special2 = 0, int UpEndur = 0, int MaxUpdEndur = 0, int Endur = 0, int MaxEndur = 0); ///�̺�Ʈ��������
	int								Wear(int where);
	int								EventWear(int where); ///�̺�Ʈ��������
	int								CanWear();
	int								CanPetWear();
	void							Remove(int where);
	void							EventWearRemove(int where); ///�̺�Ʈ��������
	// [6/1/2007 Theodoric] ������ ���� :: �Լ� �Ķ���� ������ ����
	void							AddToInven(int pack_num, int x, int y, int index, int vnum, int gem_num, int special, int special2,
			int UpEndur, int MaxUpdEndur, int Endur, int MaxEndur); ///�Ź���ȭ�ý���
	// nate
	// ���� â�� �̵� ����
	void							AddToInven( int nPackNum, int nItemIndex, int nSlot_X, int nSlot_Y, int nVnum );
	BOOL							AddToInven(int x, int y, int pack_num = -1);
	BOOL							AddToInven(int pack_num, int *x, int *y);
	BOOL							ToQuickSlot(int slot_num);
	BOOL							IsFullInvenInPack( int _packNo );
	BOOL							IsFullInvenInAllPack();
	int								GetEmptyInvenInPack(int _packNo );
	int								GetCountInExch( int nVnum );
	int								GetCountInPack( int nVnum );
	int								GetCountInPack( int pack ,int vnum );
	int								GetCountStrInPack(int pack , int vnum , int need_count, char * param);

	///2003	�Լ��� ���� ����
	//void							ToQuickSlot(int slot_num, int index, int vnum);
	void							ToQuickSlot(int slot_num, int index, int vnum, int plus_num, int special_num, int special2_num );
	BOOL							SwapWeapon();
	CItem				*			PickWeaponInPack(int pack_num, int &x, int &y);
	BOOL							CanSwapWeapon();

	int								GetItemIndexInSlot(int pack_num, int x, int y, int w, int h);
	CItem				*			PickItemInPack(int pack_num, int index, BOOL isVnum = FALSE);
	CItem				*			PickPowerInPack(int pack_num, int type_idx, int shape_idx, int app_to);
	BOOL							UseItemInPack(int index, BOOL isVnum = FALSE);
	void							ChangePackNum(int pack_num)
	{
		m_CurPackNum = pack_num;
	}
	int								GetSlotIndex(int x, int y);
	// [6/1/2007 Theodoric] ������ ���� :: �Լ� �Ķ���� ������ ����
	void							SetExtraSlot(int index, int vnum, int gem_num, int special, int special2,
			int UpEndur, int MaxUpdEndur, int Endur, int MaxEndur); ///�Ź���ȭ�ý���
	void							RemoveByIndex( int index ); // ������ ������ȣ�� ã�Ƽ� ����� �Լ�.(����)
	void							RemoveAll();
	void							RemoveAllEggs();
	void							RemoveAllInven(int slot = 0 ); // [2008/8/14 Theodoric] ��ڰ� �κ��� �ս��� ����� ���� ���� ��Ŷ
	int								GetKitsCount();				// ���� ŰƮ�� ���� ���ϴ� �Լ�.
	int								Upgrade(int where);
	int								CanUpgrade(int where);

	CItem				*			FindItemAndPackByIndexFromAll(int index, int& pack);
	CItem				*			FindItemByIndexFromAll(int index);
	CItem				*			FindItemByVnumFromAll( int vnum ); // ��ġ�� �κ� �� ���� �����ִ� ��ɵ� �߰� ///�δ�1
	int								CheckWherePackItem( CItem *DestItem ); ///�δ�1

	// exchange itme ���� �Լ�.
	// [6/7/2007 Theodoric] ������ ���� :: �Լ� ���� �߰�
	BOOL							AddToExgInven(int x, int y, int index, int vnum, int gem_num, int special, int special2,
			int UpEndur, int MaxUpdEndur, int Endur, int MaxEndur, BOOL bForce=FALSE); ///�Ź���ȭ�ý���
	BOOL							AddToExgInven(int x, int y, BOOL bForce=FALSE);
	BOOL							DelFromExgInven(int x, int y);
	int								GetItemIndexInExgSlot(int x, int y, int w, int h);
	CItem				*			PickItemInExgPack(int index, BOOL isVnum=FALSE);
	int								GetExgSlotIndex(int x, int y);
	//////////////////////////////////////////////////////////////////////////
	// Item Effect
	void							InsertEffectNum(int num);
	void							DeleteEffectNum(int num); // �ش� ĳ���Ϳ� ��ϵ� ����Ʈ�� ����Ʈ��ȣ(�����ε���)�� ã�Ƽ� �����ִ� �༮. ������ �̰� ������. ��, ���� ����Ʈ���� ������� ��� �޸𸮰� �׿����ٴ� ��... ///�� �� �۾�

	/////////////////////////////////////////////////////////////////////////
	// ���λ��� & ä��
	void							InitChatPopup(int width, int height);
	void							DrawChatPopup(BOOL bOnlyName = FALSE);
	void							SetChatMsg(char *name, char *chat_msg);
	void							OnChatMsg();

	/////////////////////////////////////////////////////////////////////////
	// ��Ÿ �Լ���.
	void							SetMyParty(BOOL bParty)
	{
		m_bMyParty = bParty;
	};
	void							SetMyGuild(BOOL bGuild)
	{
		m_bMyGuild = bGuild;
	};
	void							SetOppGuild(BOOL bOppGuild)
	{
		m_bOppGuild = bOppGuild;
	};
	void							SetTeamGrade(int nTeamGrade)
	{
		m_nTeamMemGrade = nTeamGrade;
	};
	void							ShowMatrixMark(BOOL flag)
	{
		m_bShowMatrixMark = flag;
	}
	BOOL							IsMyTeam();
	int								WhichTeam(CNkCharacter *pNkCha);
	//void							SetWhichTeam(int nTeamType) { m_nWhichTeam = nTeamType; };

	//////////////////////////////////////////////////////////////////////////
	// picking ���� �Լ�.
	void							SetMultiShot(int multiShot)
	{
		m_MultiShot = multiShot;
	}
	void							SetNextMultiShot(int nextMultiShot)
	{
		m_NextMultiShot = nextMultiShot;
	}

	//////////////////////////////////////////////////////////////////////////
	// �ʰ� ���õ� �Լ�.
	void							SetHide(int hideStatus);
	BOOL							ToggleHide();
	BOOL							IsShown()
	{
		return (m_HideStatus == HIDE_SHOWN);
	}
	BOOL							IsHidden()
	{
		return (m_HideStatus == HIDE_HIDDEN);
	}
	BOOL							IsHiding()
	{
		return (m_HideStatus == HIDE_HIDING);
	}
	void							SetHideMode(int hideMode)
	{
		m_HideStatus = hideMode;
	}
	// ���ݼ� ��ų : ���� ������ �������!
	void							SetSnifing(BOOL bSnifing);

	void							GetPvInfo(char* str);

	void							SetAttackType(int attackType)
	{
		m_ChaAttackType = attackType;
	}
	void							SetChaCondition(int Condition, int cancel);
	void							SetChaCondition2(int Condition, int cancel);
	void							SetInSkill(int InSkill)
	{
		m_InChaSkill = InSkill;
	}
	void							SetInAidiaSkill(int InSkill)
	{
		m_InChaAidiaSkill = InSkill;
	}
	void							SetUseBookEffect(int book)
	{
		m_UseBook = book;
	}
	void							SetTraceLineLVertex(int linetype, D3DVECTOR *pTargetVector = NULL, bool left = true);
	void							GetHalfMat(D3DMATRIX &mat, CBiped *pBiped, int bpd_idx, int CurFrame);
	void							Initial_Particle();


	//////////////////////////////////////////////////////////////////////////
	// nate 2004 - 4 : Crash_Miss ����
	// ����ü�� ����
	void							DrawCrash_Miss( int nI, int nElapTime );
	// 1 : Crash, 2 : Miss
	void							SetShowCrash_Miss( BYTE byCrash_Miss = 0 );
	void							RenderCrash_Miss();
	void							MoveSkill(float tx, float tz, int nSkillIndex = 0); //mungmae: 2005/11/16 ������ ũ����, ������ ��ų
	// nate 2004 - 4 : CastleWar
	void							RenderGiftMessage();
	void							SetBattlePoint(int BattlePoint);
	void							SetLaghaimPoint(int LaghaimPoint );

	void							SetGHPoint(int GHPoint);
	void							SetSkillStep( int nSkillStep );	// nate 2005-06-17 : ���ӽ�ų ���� ���� ��ų�ܰ�
	void							SetFailStep( int nFailStep );	// mungmae 2007/1/4 : ��ٿ� ��������� �������� ������ ó���ϱ� ���ؼ� �������.
	int								BurningCrashEndFrame( int nComboStep );
	int								MatrixTeamGrade(int nTeamindex);
	void							HellDownFail( CNkCharacter* pCha, CNkMob* pMob, BOOL bPacket = TRUE);

	EffectSort			*			FindMyFairy( int Type );		//�ڱ� �ڽ��� �� ����Ʈ(����ü)�� ã���ִ� �Լ� ///�� // 07-01-30 Ÿ�� ���ڸ� �߰��Ͽ� ����


	int								GetToX()
	{
		return (int)m_tx;
	}; ///�����޺� �� �۾�.
	int								GetToY()
	{
		return (int)m_ty;
	};
	int								GetToZ()
	{
		return (int)m_tz;
	};
	void							SetTargetPos(float tx, float ty, float tz);

	EffectSort*						FindMySpirit();					//�ڱ� �ڽ��� ���Ǹ� ����Ʈ(����ü)�� ã���ִ� �Լ�
	BOOL							IsDrawPicking();				// ��ŷ�� �׸��� ���� ����. ///������� �� �߰�

	void							ChaDontAttackEffectStart()
	{
		m_bchaDontAttackEffect = TRUE;
	}
	void							ChaDontAttackEffectEnd()
	{
		m_bchaDontAttackEffect = FALSE;
	}
	//////////////////////////////////////////////////////////////////////////
	// Siren Flag
	// [9/11/2007 Theodoric] Siren
	void							SetSirenOn(int color);
	void							SetSirenOff(int color);

	BOOL							IsSirenRed();
	BOOL							IsSirenBlue();
	BOOL							IsSirenGreen();
	BOOL							IsSirenYellow();

	int								GetSirenColor();
	BOOL							IsWearing();

	bool							IsWeared(CItem* pItem);
	bool							IsEventWeared(CItem* pItem);
		
	SETITEM							GetSetItem()
	{
		return m_SetItem;
	}
	SETITEM							IsSetItem(int vNum);
	void							SetSetItem(SETITEM nSetItem)
	{
		m_SetItem = nSetItem;
	}
	void							SetSetItem();

	SETITEM							GetSetItemEventWear()
	{
		return m_SetItemEventWear;
	}
	SETITEM							IsSetItemEventWear(int vNum);
	void							SetSetItemEventWear(SETITEM nSetItem)
	{
		m_SetItemEventWear = nSetItem;
	}
	void							SetSetItemEventWear();	

	BOOL							GetSetItemEffectString(char * str, bool bEventWear);

	//void							SetBossRaidHero(BOOL bHero)					{m_bBossRaidHero = bHero;}
	void							SetBossRaidHeroEffectIndex(int index)
	{
		m_BossRaidHeroEffectIndex = index;
	}
	int								GetBossRaidHeroEffectIndex()
	{
		return m_BossRaidHeroEffectIndex;
	}
	void							OnBossRaidHeroEffect();

	void							SetSvrChannelNum(int channel)
	{
		m_SvrChannelNum = channel;
	}
	int								GetSvrChannelNum()
	{
		return m_SvrChannelNum;
	}

	BOOL							IsPressMan();

	int m_lv400_effect_index;
	void SetLv400Effect(int index)
	{
		m_lv400_effect_index = index;
	}
	int GetLv400Effect()
	{
		return m_lv400_effect_index;
	}


	BOOL							FindExgSlotBlankSlot(int &x, int &y, int w, int h);

	// �Ϻ��� �䱸�� ���� �� : ������ �г�Ƽ - ũ����Ż �����ڴ� �ۼ� ���� �ϱ� ���� , 2007.11.12 �����
	void							SetRunState( BOOL state );


private:
	bool m_bEventWearRendered[WEARING_NUM];


	// by evilkiki 2009.09.14 ĳ���� ����ġ ����
public:
	void InitExpTable();



	UINT64	GetTotalExp();				// �� ���� exp
	UINT64	GetNeedExp();				// ���緹������ ������������ �ʿ��� ����ġ
	UINT64	GetLevelExp(int level);		// Ư���������� �ʿ��� ����ġ




	// by evilkiki 2009.10.01
	bool RemoveEquipItem(int index);
	bool RemoveEventItem(int index);
	bool RemoveInvenItem(int index);
	bool RemoveExchageItem(int index);
	bool RemoveExtraItem(int index);
	bool RemoveQuickItem(int index);

	bool RemoveQuickItem(int slot, int index, bool moveto=false ,bool similar=false);
	bool RemoveInvenItem(int pack, int index);

	bool InvenToQuick(int slot, int index);


	CItem * GetInvenItem(int pack , int index, bool vnum=false);
	CItem * GetPowerInPack(int pack_num, int type_idx, int shape_idx, int app_to);

	bool UpdateQuickItem(int plus_num, int special_num, int slot_num);
	bool UpdateEquipItem(int plus_num, int special_num, int equip_where);

	bool CheckMoveToQuick(int vnum);

	// ������� �÷���
	bool	m_bWanted;

public:
	bool m_send_rivial;												// Ŭ�󿡼� �޼��� ������


	bool IsEquiped();




	bool Check_TwoHandWeapon(CItem * pItem);


	void scaleCharacter(float ratio);


	bool m_d_system;
	bool m_activate_field;
	bool m_activate_engage;
	bool m_activate_critical;
	bool m_activate_field_ex;


	int m_activate_buff_skill;

	bool IsActiveDSystem()
	{
		return m_d_system;
	}

	bool IsActiveField()
	{
		return m_activate_field;
	}
	bool IsActiveField_Ex()
	{
		return m_activate_field_ex;
	}
	bool IsActiveEngage()
	{
		return m_activate_engage;
	}
	bool IsActiveCritical()
	{
		return m_activate_critical;
	}


	void SetActiveField()
	{
		m_activate_field = true;
	}
	void SetActiveField_Ex()
	{
		m_activate_field_ex = true;
	}
	void SetActiveEngage()
	{
		m_activate_engage = true;
	}
	void SetActiveCritical()
	{
		m_activate_critical = true;
	}

	void SetDeactiveField()
	{
		SetDeactiveField_Ex();
		m_activate_field = false;
	}
	void SetDeactiveField_Ex()
	{
		m_activate_field_ex = false;
	}
	void SetDeactiveEngage()
	{
		m_activate_engage = false;
	}
	void SetDeactiveCritical()
	{
		m_activate_critical = false;
	}


	void SetActiveDSystem()
	{
		m_d_system = true;
	}
	void SetDeactiveDSystem()
	{
		SetDeactiveField_Ex();
		m_d_system = false;
	}

public:
	void SetSummonNpcIndex(const int& index);
	const int& GetSummonNpcIndex();

private:
	int m_indexSummonNpc;
};

extern CNkCharacter * g_pRoh;

#endif // _NK_CHARACTER_H_
