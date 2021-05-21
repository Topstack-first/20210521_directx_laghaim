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

#define MAX_TRACELINE_FRAME					50		// 칼, 도끼 최대 공격 프레임 

#define MAX_PARTY_NUM_BOSSRAID_FINAL		16		// 보스레이드 최종 보스 존에서는 50명으로
#define MAX_PARTY_NUM_BOSSRAID				16		// 보스레이드 존에서는 파티 인원수를 16명으로 제한한다. 캐릭터 클래스에서도 쓰기때문에 여기다 둠.
#define MAX_PARTY_NUM						16		// 파티 인원수 제한 디파인. 캐릭터 클래스에서도 쓰기때문에 여기다 둠.

#define MAX_MULTITARGET						10		// 하이브리더 콤보시에 때릴수있는 개체수 한계 ///하이콤보
#define MULTITARGET_KIND_MOB				1		// 하이브리더 콤보시에 때리는 개체의 종류가 캐릭터임.  ///하이콤보
#define MULTITARGET_KIND_CHAR				2		// 하이브리더 콤보시에 때리는 개체의 종류가 캐릭터임.  ///하이콤보


// [2008/6/17 Theodoric] 셋트 효과
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
	TRACELINE_DASHSLASH,			///하이콤보
	TRACELINE_SLASHSHADOW,			///하이콤보
} TraceLine_Type;

//////////////////////////////////////////////////////////////////////////
// 클래스 전 선언.
class CGuildData;
struct EffectSort;					///페어리 시 추
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
class CRectPlane;					// 데카르디 공성 캐릭 바닥 그리기.
class CFxSiren;						// wanted!!

class CNkCharacter
{
	// define variable or attribute
	// 윈도우 메모리는 전체적으로 32bit로 구성되어있기 때문에
	// 메모리 정렬을 위해서는 항상 8바이트로 구성해 두어야 한다.
	// 때문에 8바이트가 넘지 않는 변수들의 경우에는 인접한
	// 부족한 변수와 조합하여 8byte조합을 하여야 한다.
	// 때문에 배열을 조정한다.
private:
	//===============================
	//fpe방지 위치와 변수형 변경
	//===============================
	double							m_AniRate;
	int								m_CurFrame;
	DWORD							m_dwBaseTime;
	int								m_PrevFrame;
	DWORD							m_dwTimeCapPrev;
	//Motion State
	unsigned short					m_nMotionState;
	float							m_TargetHeight;	// 공격 대상 높이.
	int								m_nFlyFrame;

	CBiped				*			m_Biped;
	CCapsyong			*			m_CapSyong[CAPSYONG_NUM];
	BOOL							m_bShow[ CAPSYONG_NUM + 2 ];

	//	BOOL						m_bEddy;
	// 텍스처
	//	IndexedTexture	*			m_CapsyongTexture;
	//	IndexedTexture	*			m_TraceTexture;
	//	IndexedTexture	*			m_LightswordTexture;
	//	int							m_LightswordFrame;

protected:
	static float					m_fRunSpeed;
	//	static float	m_fWalkSpeed;
	// nate 2004 - 7 글로벌 ShadowRect사용
	//	CShadowRect		*m_pShadowRect;
	CGoodShadow			*			m_pGoodShadow;
	CClrAlloc			*			m_pClrAlloc;
	// 모션에 관련된 변수
	BOOL							m_bMoving, m_bAttack;
	float							m_dx, m_dz;			// 이동 방향 벡터.
	float							m_tx, m_ty, m_tz;	// target (x, z) 좌표.
	// Score 기록을 위한 윈도우 포인터
	CControlScore		*			m_pScoreWnd;
	CFxSiren			*			m_pSiren;			// 현상수배.
	// [2008/6/20 Theodoric]
	int								m_BossRaidHeroEffectIndex;
	SETITEM							m_SetItem;
	SETITEM							m_SetItemEventWear;
	int								m_SvrChannelNum;

public:
	bool							m_bGuildPK;
	bool							m_bChakramatt;
	char							m_sEnemyGName[100];
	// 캐릭터의 특성값.
	int								m_nCharIndex;
	WORD							m_wColorIdx;		// 컬러 테이블 인덱스
	DWORD							m_dwColorPick;		// 컬러 패러미터.
	char				*			m_pstrName;
	char				*			m_pstrGuildName;
	char				*			m_pstrGuildGradeName;
	int								m_GuildIndex;
	int								m_GuildGradeIndex;
	char							m_Race;
	char							m_Sex;
	char							m_Hair;
	int								m_nUserIndex;		//유저인덱스
	CGuildData			*			m_pMyGuilddata;

	//===============================
	//fpe방지 위치와 변수형 변경
	//===============================
	// 캐릭터의 Status
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


	double							m_Vital;		//오토마우스 막기위해 자리 변경
	double							m_MaxMana;
	double							m_MaxStamina;
	int								m_Level;
	int								m_BpdRace;
	__int64							m_Money;
	double							m_Fame;
	// 캐릭터의 상태2
	int								m_nManaShield;
	int								m_nElectricShield;
	int								m_nSafetyShield;
	//매트릭스 배틀 포인트
	int								m_nBattlePoint;
	// 아이템 관련
	CItem				*			m_Wearing[WEARING_NUM];			// equipment(장비)
	CItem				*			m_EventWearing[WEARING_NUM];	///이벤트복장착용 시스템 정보.
	CItem				*			m_ExtraSlot;					// 잡았을때.
	//Display Event Wear
	BOOL							m_bEventWearFlag;				// [2008/7/4 Theodoric] 국내에선 이벤트 복장을 않볼 수 있는 선택권을 달라고 해서 추가.
	// 팩넘버로 연결리스트 생성
	// 아이템 자체( 연결리스트 )
	CItem				*			m_Inven[PACK_NUM];				// inventory
	CItem				*			m_ExgInven;						//
	CItem				*			m_QuickSlot[QUICK_SLOT_NUM*QUICK_SLOT_PAGE];	// quick slot
	int								m_QuickCount[QUICK_SLOT_NUM*QUICK_SLOT_PAGE];	// 인벤에 들어있는 포션 아이템의 수량.
	BOOL							m_bQuickSlotLock[QUICK_SLOT_NUM*QUICK_SLOT_PAGE];// 사용 여부.
	// 현 슬롯의 아이템 인덱스값
	int								m_Slot[PACK_NUM][SLOT_X_NUM][SLOT_Y_NUM]; // 인벤
	int								m_ExgSlot[EXG_SLOT_X_NUM][EXG_SLOT_Y_NUM];
	int								m_CurPackNum;
	BOOL							m_bExgLock;
	int								m_ActNum;
	int								m_DeadFrame;
	// 사용자 위치와 방향 정보 : world coordinate
	float							m_wx;
	float							m_wy;
	float							m_wz;
	float							m_dir;
	float							m_toDir;
	D3DMATRIX						m_matWorld;
	float							m_fScaleSize;

	int								m_nLaghaimPoint; // [2008/5/16 Theodoric] BP -> LP 로 바뀜

	int								m_nGHPoint;		// 그랜드 히어로 포인트, 사용치를 고려하지 않은 총 누적 포인트이다.
	bool							isShieldLoad;
	// 캐릭터의 그림자 크기 조절을 위해서.
	static int						m_nCurHour;
	int								m_FlyLevel;
	BOOL							m_bOwnAmbient;
	DWORD							m_dwOwnAmbient;
	// 현재 움직이는 모드.
	BOOL							m_bRun;
	// 공격하고 있는 맙이나 캐릭터 아이디
	int								m_AttackChaId;
	int								m_AttackMobId;
	// 맞아서 변경될 체력
	int								m_AttackVital;
	DWORD							m_LastAttackVitalTime;
	BOOL							m_bCombat;			// 전투중인지
	//BOOL							m_bHiding;
	// 마법을 쏠 맙이나 캐릭터 아이디
	int								m_MagicChaId[10];
	int								m_MagicChaCount;
	int								m_MagicMobId[10];
	int								m_MagicMobCount;
	int								m_MagicNum;
	// 주인공이 다가오고 있구나!!!
	BOOL							m_bHeroAttCome;
	BOOL							m_bPlace;
	float							m_fPlaceDir;
	// 렌더링 함수
	BOOL							m_bVisible; // 프러스텀 컬링상에서 보이는가의 플래그. 06-01-03 원석 추가(개인상점 말칸 때문에 필요했음.)
	// 개인상점
	CAlphaPopUp			*			m_pChatPopup;
	CAlphaPopUp			*			m_pUserShopPopup;
	// 개인 상점 이름과 멘트를 위한 팝업(위의 m_pChatPopup와는 따로 쓰는게 여러모로 편해서 따로 씀) ///개인상점
	// 기타 flag
	DWORD							m_LastChatTime;
	BOOL							m_bShowChatPopup;
	BOOL							m_bMyParty;
	BOOL							m_bMyGuild;
	BOOL							m_bOppGuild;
	BOOL							m_bCrystalOwner;
	BOOL							m_bCrystalGuild;
	int								m_nTeamMemGrade;		// 0 : 팀원이 아니다. 1 : A팀장 2: A팀원 , 3:B팀장. 4: B팀원
	BOOL							m_bShowMatrixMark;
	bool							m_bCatchFlagShow;
	///////////////////////////////////////////////////////////////
	// picking buffer에서 사용할 정육면체 데이타.
	static CLolos					m_CubeLolo;
	static DWORD					m_dwAmbient;
	CDropStar			*			m_pDropStar;			// 발밑에 떨어지는 별
	int								m_MultiShot;
	int								m_NextMultiShot;
	int								m_RenderMultiShot;
	// 맵과 관련한 변수.
	int								m_HideStatus;			// 휴먼의 땅속 숨는 스킬
	DWORD							m_HideTime;
	int								m_GuildLevel;			// 길드 변수
	///2003 캐릭의 특수효과를 위한 변수와 함수.
	int								m_ChaAttackType;		//특수공격 (카운트,크리티컬...)
	int								m_ChaCondition;			//캐릭의 상태(석화,마비...)
	int								m_CancelState;			//상태 on,off
	int								m_ChaCondition2;		//캐릭의 상태(석화,마비...)
	int								m_CancelState2;			//상태 on,off
	int								m_InChaSkill;			//화면에 들어온 캐릭에 걸려있는 지속스킬.
	int								m_InChaAidiaSkill;		//화면에 들어온 캐릭에 걸려있는 에이디아 지속스킬.
	//BOOL							m_UseBook,m_ReceiveBook;//책 사용 효과 ,책마법 맞는 효과
	int								m_UseBook;				//여기에 책마법과 해제도 같이 쓰인다.
	BOOL							m_ShowConditionEffect;	//상태효과
	BOOL							m_ShowConditionEffect2; //상태효과
	BOOL							m_ShowSkillEffect;		//지속스킬효과
	BOOL							m_ShowAidiaSkillEffect;	//에이디아 지속스킬효과
	BOOL							m_Accumulate;			//누적.
	BOOL							m_DoubleAttack;			//휴먼의 더블공격
	int								m_Chaotic_Grade;		//카오 등급.
	int								m_Free_Pk;				//프리 피케이.
	int								m_Chaotic_Num;			// 카오 수치.
	BOOL							m_bPk_render;			//피케이 선언과 해제시에 새로 그려주기 1112
	BOOL							m_bMoonEventEffect;		//0212 대보름 이벤트
	BOOL							m_ShowMoonEventEffect;	//이벤트 효과 지속.
	BOOL							m_bNonPK;
	// 보스 레이드.
	int								m_iPartysListId[MAX_PARTY_NUM_BOSSRAID_FINAL];			// 파티원 id
	float							m_fPartysVitalList[MAX_PARTY_NUM_BOSSRAID_FINAL];		// vital
	int								m_iPartysListXpos[MAX_PARTY_NUM_BOSSRAID_FINAL];		// Xpos
	int								m_iPartysListYpos[MAX_PARTY_NUM_BOSSRAID_FINAL];		// Zpos
	int								m_iMaxPartysNum;		// 현제 파티원 수
	int								mi_Snifing_GAP_Frame;	// 휴먼의 스나이핑 스킬과 관련
	int								mb_Snifing;
	float							m_fDash_Frame;			// Lyul_2005 프리크 대쉬이동 횟수 (정확한 보정을 위해 float)
	float							m_fFlying_frame;		//mungmae: 2005/11/16 플라잉 클래쉬
	float							m_fShadow_frame;		//mungmae: 2005/11/30 쉐도우 클래쉬
	BOOL							m_bstone;				// 디바인 스톤을 사용했나. Framemov에서 땅에 박히는 순간 활성화 된다.
	// 서버에서 Dstone 이 사라지면 다시 돌아온다.
	BOOL							m_bBleed;				// 현재 블레드 스킬이 발동중인가.
	BOOL							m_bresstone;			// 디바인 스톤이 생성될 준비가 됐는가.
	int								m_resmobidx;			// 생성될 몹의 인덱스
	int								m_resmobvnum;			// 생성될 몹의 vnum
	int								m_resmobx;				// 생성될 몹의 좌표
	int								m_resmobz;				// 생성될 몹의 좌표
	int								m_resmobdir;			// 생성될 몹의 디랙션
	BOOL							m_bhaste;				// 프리크가 헤이스트를 마스터 했을 경우 공속 업.
	BOOL							m_bShowPartysVital;
	BOOL							m_bRecivedPartysInfo;
	BOOL							m_bIsParty;
	// particle
	Deco_Particle		*			m_NkCha_Particle;
	D3DLVERTEX						m_TraceLine[(MAX_TRACELINE_FRAME * 4) + 4]; // 칼 자취가 그려질 버텍스 정의
	int								m_drawLineNum;								// 실제로 그려질 버텍스 갯수
	int								m_DrawWeaponEffects;
	int								m_CurAttackType;
	BOOL							m_bFlagOwner[4];
	BOOL							m_bFlagGuild[4];
	//===========================================================
	// nate 2004 - 4 : Crash_Miss 관련
	// 구조체로 변경
	int								m_nElapTime;
	DWORD							m_dwElapTime;
	CAlphaPopUp			*			m_pCrash_MissPopup[ CRASH_MISS_COUNT ];
	BYTE							m_byShowCrash_Miss;
	BYTE							m_byShowGiftMessage;
	CAlphaPopUp			*			m_pGiftMessage;

	BOOL							m_bComboReady;			//mungmae-2005/04/11 - 설명: 불칸 기술 헬크라이 중에 슬레이어가 가능하게 끔 만들기 위해(콤보처럼)
	BOOL							m_bComboSucceed;		//mungmae-2005/04/12 - 설명: 헬크라이, 슬레이어 콤보 성공 여부
	BOOL							m_ShowReflection;		//mungmae-2005/04/20 - 설명: 리플렉션 효과가 유무
	int								m_nComboFrame;			//mungmae-2005/05/03 - 설명: 콤보 스킬이 성공된 프레임을 서버로 보내기 위해

	DWORD							m_MultiTargetArray[MAX_MULTITARGET]; // 하이브리더 해머 콤보시 타겟 인뎃스를 저장해놓을 필요가있어서... ///하이콤보
	POINT							m_MultiTargetPos[MAX_MULTITARGET]; // 타겟 좌표.(매번 찾으려면 오래걸리수있으니 좌표를 기억해둔다.) ///하이콤보
	int								m_MultiTargetNum;		// 하이브리더 해머 콤보시 타겟 갯수를 저장해놓을 필요가있어서... ///하이콤보
	BYTE							m_MultiTargetKind;		// 몹인지 캐릭터인지 플래그. ///하이콤보

	BYTE							m_byBurningStep;		// nate 2005-07-29 : Burning Crash - 스킬 단계 저장
	int								m_nSkillLevel;			// nate 2005-06-17 : 스킬의 숙련도( 스킬포인트 )
	int								m_nSkillStep;			// nate 2005-06-27 : 휴먼-마인갯수, 불칸-버닝크래쉬단계
	int								m_nFailStep;			// mungmae 2007/1/4 : 스킬 취소
	int								m_nOldFrame;			// nate 2005-07-21 : 메세지를 보낼지 판단
	BYTE							m_byShowLIfeExpantion;	// nate 2005-07-24 : Life Expansion - 생명연장 이펙트 보이기
	//mungmae-2005/06/23
	//설명: 뫼비우스에서 리젠 될때까지의 부활 게이지
	CFxRegenBar			*			m_pRegenbar;
	BOOL							m_bDrawRegenbar;
	DWORD							m_dwRegenTime;
	int								m_nRegenIndex;
	BOOL							m_bAttacked;			//mungmae 2005-07-13 : 자기 케릭터가 공격받고있나?
	int								m_nAttVital;			//mungmae 2005-07-23 : 자기 케릭터의 바이탈

	BOOL							m_bCloaking;			//mungmae-2005/07/26 : 하이브리더 하이드
	BOOL							m_bCloaking_Waring;		//mungmae-2005/07/26 : 하이브리더 하이드 풀리기전에 깜빡거림 경고
	BOOL							m_bPoisoning;			// nate 2005-11-14 : 포이즌 - 포이즌상태 저장
	BOOL							m_bShowAdrenaline;		//mungmae-2005/11/21 - 설명: 아드레날린 효과가 유무
	float							m_fSubAttackTime;
	// 개인상점용 변수 추가. ///개인상점
	int								m_UserShopLevel;		// 개인 상점을 열었는가?
	char							m_strUserShopMent[256]; // 상점 멘트 스트링. 머리위에 표시해줘야한다.
	BOOL							m_bYanQuest;			//mungmae 2005/12/12 얀 퀘스트(저렙 퀘스트)수행 여부
	BOOL							m_bSecialQuest;			//mungmae 2006/06/02 205~275스폐셜 퀘스트 여부
	BOOL							m_bSwapWeapon;			// mungmae-2006/06/21 : 마법이전 중에 무기 바꾸기.
	int								m_HaveCash;				// 보유 캐쉬 ///유료상점
	DWORD							m_dwComboTick;			///하이콤보
	D3DVECTOR						m_vMvSkillStartPos;		// 무브 스킬이 발동되는 시점의 위치. ///하이콤보
	BOOL							m_bSpiritModeEffect;
	BOOL							m_bProtectionShield;
	DWORD							m_dwReturnTime;
	DWORD							m_dwResurrectionTime;
	DWORD							m_dwResurrection_StartTime;
	BOOL							m_bResurrectionSkill;
	int								m_nResurrection_ChaId;
	int								m_NvNWin;				///국가전승리 횟수.
	///자유대련
	BOOL							m_bNowFreePVP;			// 자유대련 중인지 여부
	DWORD							m_dwPvpDestIndex;		// 대련상대의 인덱스.(자신이 자유대련 중에만 셋팅.)
	D3DVECTOR						m_vPvpRingPos;			// 자유대련 영역 표시하는 링의 중심 좌표.
	DWORD							m_dwRingCreateTick;		// 링이 생성된 시간.
	BOOL							m_bchaDontAttackEffect;
	int								m_nGuildNameLine;
	bool							m_nHaveGuildMark;

	bool							m_bHellDowning;

	BYTE							m_SirenColorFlag;
	// 캐릭터와 관련된 effects
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
	int								CheckInvasionGNameType(char* guildname);	 // 길드명으로 검색
	t_Info							GetinvasionRemainTime();
	t_Info							GetMyGuildRevisionTime();
	// Status 저장
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
	void							SetCash( int Cash );			///유료상점
	void							SetFame(int money);				//후견인
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
	// 아이템 관련...
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
	// Attack 관련 함수.
	void							SetAttackChaId(int chaId, BOOL bPetAttack = TRUE );
	// 펫도 공격시킬지 플래그를 추가. (일부 스킬은 공격을 안시키게 하기위해서..) 06-01-31 원석
	void							SetAttackMobId(int mobId);
	void							SetAttackVital(int vital);
	void							Attacked();
	//////////////////////////////////////////////////////////////////////////
	// Magic 관련 함수.
	void							SetMagicCha(CNkCharacter *pCha, int magicNum);
	void							AddMagicCha(CNkCharacter *pCha);
	void							SetMagicMob(CNkMob *pMob, int magicNum);
	void							AddMagicMob(CNkMob *pMob);
	//////////////////////////////////////////////////////////////////////////
	// motion 관련 함수.
	BOOL							Move(float tx, float tz);
	BOOL							Place(float tx, float tz, float dir, int nFrame);
	void							Stop(float tx, float tz, float dir);
	void							AttackChakram(float tx, float ty, float tz);
	void							Attack(float tx, float ty, float tz, int attkType = -1);
	void							Magic(float tx, float ty, float tz, int act_num);
	void							Kill();

	//////////////////////////////////////////////////////////////////////////
	// 초기화 함수
	void							MakeBasicBody(int nRace, int nSex, int nHair);
	void							MakeBasicBody();
	void							SetBiped(CBiped *pBiped);
	CBiped				*			GetBiped()
	{
		return m_Biped;
	}
	// 2007 때 추가. 프로즌 샷 이펙트에 필요해서...
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
	// 프레임 계산하는 함수
	void							FrameMove(unsigned int nInc, BOOL bVisible = TRUE,
			CNkMob *pTargetMob = NULL, CNkCharacter *pTargetCha = NULL);
	BOOL							EventWearingFrameMove( int where ); // 특정 위치의 이벤트 복장을 그린다. ///이벤트복장착용

	DWORD hitpoints[10][3];
	void							addhitpoint(DWORD num, DWORD attacker);
	bool							ishit();
	//////////////////////////////////////////////////////////////////////////
	// Render 함수.
	BOOL							IsAlphaWear( int w, bool bEvent );
	BOOL							RenderAlphaWear( CItem* pItem, BOOL bLow );
	BOOL							Render( LPDIRECT3DDEVICE7 lpd3dDevPick, BOOL bMouseOn = FALSE, BOOL bVisible = TRUE);
	BOOL							EventWearingRender( int where ); // 특정 위치의 이벤트 복장을 그린다. ///이벤트복장착용

	//////////////////////////////////////////////////////////////////////////
	// 캐릭터에 대한 명령 함수
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
	// 공격스킬은 모두 추가 해 줘야 된다.
	// 공격 로직에서는 사용하지 않지만 사교니 뭐니에서 쓴다 젠장 -_-
	BOOL							IsCantMove();	// 스킬 사용중인지 판단.
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
	// 현재 Motion State 의 초기 모션으로 Reset
	// 2004.03.09 - 수정완료
	// Lyul / 수정(TAB 버그 )
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
	// 아이템 관련 함수
	// [6/1/2007 Theodoric] 내구도 수리 :: 함수 파라미터 내구도 포함
	void							Wear(int where, int index, int vnum, int gem_num = 0, int special = 0, int special2 = 0,
										 int UpEndur = 0, int MaxUpdEndur = 0, int Endur = 0, int MaxEndur = 0); ///신방어구강화시스템
	void							EventWear(int where, int index, int vnum, int gem_num = 0,
			int special = 0, int special2 = 0, int UpEndur = 0, int MaxUpdEndur = 0, int Endur = 0, int MaxEndur = 0); ///이벤트복장착용
	int								Wear(int where);
	int								EventWear(int where); ///이벤트복장착용
	int								CanWear();
	int								CanPetWear();
	void							Remove(int where);
	void							EventWearRemove(int where); ///이벤트복장착용
	// [6/1/2007 Theodoric] 내구도 수리 :: 함수 파라미터 내구도 포함
	void							AddToInven(int pack_num, int x, int y, int index, int vnum, int gem_num, int special, int special2,
			int UpEndur, int MaxUpdEndur, int Endur, int MaxEndur); ///신방어구강화시스템
	// nate
	// 다중 창고 이동 관련
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

	///2003	함수의 인자 증가
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
	// [6/1/2007 Theodoric] 내구도 수리 :: 함수 파라미터 내구도 포함
	void							SetExtraSlot(int index, int vnum, int gem_num, int special, int special2,
			int UpEndur, int MaxUpdEndur, int Endur, int MaxEndur); ///신방어구강화시스템
	void							RemoveByIndex( int index ); // 아이템 고유번호로 찾아서 지우는 함수.(원석)
	void							RemoveAll();
	void							RemoveAllEggs();
	void							RemoveAllInven(int slot = 0 ); // [2008/8/14 Theodoric] 운영자가 인벤을 손쉽게 지우기 위해 만든 패킷
	int								GetKitsCount();				// 가공 키트의 수를 구하는 함수.
	int								Upgrade(int where);
	int								CanUpgrade(int where);

	CItem				*			FindItemAndPackByIndexFromAll(int index, int& pack);
	CItem				*			FindItemByIndexFromAll(int index);
	CItem				*			FindItemByVnumFromAll( int vnum ); // 위치한 인벤 팩 등을 돌려주는 기능도 추가 ///인던1
	int								CheckWherePackItem( CItem *DestItem ); ///인던1

	// exchange itme 관련 함수.
	// [6/7/2007 Theodoric] 내구도 수리 :: 함수 인자 추가
	BOOL							AddToExgInven(int x, int y, int index, int vnum, int gem_num, int special, int special2,
			int UpEndur, int MaxUpdEndur, int Endur, int MaxEndur, BOOL bForce=FALSE); ///신방어구강화시스템
	BOOL							AddToExgInven(int x, int y, BOOL bForce=FALSE);
	BOOL							DelFromExgInven(int x, int y);
	int								GetItemIndexInExgSlot(int x, int y, int w, int h);
	CItem				*			PickItemInExgPack(int index, BOOL isVnum=FALSE);
	int								GetExgSlotIndex(int x, int y);
	//////////////////////////////////////////////////////////////////////////
	// Item Effect
	void							InsertEffectNum(int num);
	void							DeleteEffectNum(int num); // 해당 캐릭터에 등록된 이펙트중 이펙트번호(고유인덱스)를 찾아서 지워주는 녀석. 기존엔 이게 없었다. 즉, 기존 이펙트들은 사라져도 계속 메모리가 쌓여간다는 뜻... ///페어리 시 작업

	/////////////////////////////////////////////////////////////////////////
	// 개인상점 & 채팅
	void							InitChatPopup(int width, int height);
	void							DrawChatPopup(BOOL bOnlyName = FALSE);
	void							SetChatMsg(char *name, char *chat_msg);
	void							OnChatMsg();

	/////////////////////////////////////////////////////////////////////////
	// 기타 함수들.
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
	// picking 관련 함수.
	void							SetMultiShot(int multiShot)
	{
		m_MultiShot = multiShot;
	}
	void							SetNextMultiShot(int nextMultiShot)
	{
		m_NextMultiShot = nextMultiShot;
	}

	//////////////////////////////////////////////////////////////////////////
	// 맵과 관련된 함수.
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
	// 저격술 스킬 : 연사 변수를 사용하자!
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
	// nate 2004 - 4 : Crash_Miss 관련
	// 구조체로 변경
	void							DrawCrash_Miss( int nI, int nElapTime );
	// 1 : Crash, 2 : Miss
	void							SetShowCrash_Miss( BYTE byCrash_Miss = 0 );
	void							RenderCrash_Miss();
	void							MoveSkill(float tx, float tz, int nSkillIndex = 0); //mungmae: 2005/11/16 프라잉 크래쉬, 새도우 스킬
	// nate 2004 - 4 : CastleWar
	void							RenderGiftMessage();
	void							SetBattlePoint(int BattlePoint);
	void							SetLaghaimPoint(int LaghaimPoint );

	void							SetGHPoint(int GHPoint);
	void							SetSkillStep( int nSkillStep );	// nate 2005-06-17 : 연속스킬 사용시 현재 스킬단계
	void							SetFailStep( int nFailStep );	// mungmae 2007/1/4 : 헬다운 취소조건이 서버에서 왔을때 처리하기 위해서 만들었다.
	int								BurningCrashEndFrame( int nComboStep );
	int								MatrixTeamGrade(int nTeamindex);
	void							HellDownFail( CNkCharacter* pCha, CNkMob* pMob, BOOL bPacket = TRUE);

	EffectSort			*			FindMyFairy( int Type );		//자기 자신의 페어리 이펙트(구조체)를 찾아주는 함수 ///페어리 // 07-01-30 타입 인자를 추가하여 응용


	int								GetToX()
	{
		return (int)m_tx;
	}; ///하이콤보 시 작업.
	int								GetToY()
	{
		return (int)m_ty;
	};
	int								GetToZ()
	{
		return (int)m_tz;
	};
	void							SetTargetPos(float tx, float ty, float tz);

	EffectSort*						FindMySpirit();					//자기 자신의 스피릿 이펙트(구조체)를 찾아주는 함수
	BOOL							IsDrawPicking();				// 피킹을 그릴지 말지 여부. ///자유대련 시 추가

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

	// 일본측 요구로 인한 축 : 공성전 패널티 - 크리스탈 소유자는 뛸수 없게 하기 위해 , 2007.11.12 반재승
	void							SetRunState( BOOL state );


private:
	bool m_bEventWearRendered[WEARING_NUM];


	// by evilkiki 2009.09.14 캐릭터 경험치 관련
public:
	void InitExpTable();



	UINT64	GetTotalExp();				// 총 누적 exp
	UINT64	GetNeedExp();				// 현재레벨에서 다음레벨까지 필요한 경험치
	UINT64	GetLevelExp(int level);		// 특정레벨에서 필요한 경험치




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

	// 현상수배 플레그
	bool	m_bWanted;

public:
	bool m_send_rivial;												// 클라에서 메세지 보낸후


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
