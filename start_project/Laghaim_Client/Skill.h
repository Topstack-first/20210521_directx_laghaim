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
	int		GetMoney( int nSkill_Level );	// nate 2005-11-08 : 포이즌 - 요구 라임 계산
	int		GetApp0(int skill_level, int nStep = 0, bool automine=false);
	int		GetApp1(int skill_level);
	int		GetManaShieldTime(int skill_level);
	int		GetManaShieldAddTime(int skill_level);
	int		GetProtectionShieldData(int skill_level);

	int		m_Index;	// 사실상 필요 없다.

	std::string m_Name;
	std::string m_Descr;

	int		m_MinLevel;
	int		m_Energe0Min;	// 최소 마나
	int		m_Energe0Max;	// 최대 마나
	int		m_WhichEnergy;	// 현재 사용하는 에너지 타입.
	int		m_App0Min;		// 최소 데미지(치료율, 퍼센트 등등)
	int		m_App0Max;		// 최대 데미지(치료율, 퍼센트 등등)
	int		m_App1Min;		// 최소 거리()
	int		m_App1Max;		// 최대 거리()
	int		m_MaxLevel;
	BOOL	m_bLeft;
	BOOL	m_bRight;
	BOOL	m_bAttack;
	int		m_Kind;
	int		m_AttCount;
	int		m_MinLaim;	// nate 2005-11-08 : 포이즌 - 스킬업 최소 요구 라임
	int		m_MaxLaim;	// nate 2005-11-08 : 포이즌 - 스킬업 최대 요구 라임

	int		m_race;		// 종족 플레그

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
	int     GetArrayIndex( int skill_idx ); // 스킬 인덱스로 부터 배열 인덱스를 얻어오는 함수 ///하이콤보 때 추가.

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
// nate 2005-06-07 : 캐릭터 스킬
#define SKILL_SWORD_MASTERY		1	// 검술의 달인 
#define SKILL_AXE_MASTERY		2	// 도끼의 달인
#define SKILL_FIRE_BALL			3   // 파이어볼
#define SKILL_LIGHTNING_BALL	4   // 라이트닝 볼
#define SKILL_LIGHTNING		    5   // 라이트닝
#define SKILL_HEAL_SELF			6	// 에이디아 - 리커버리
#define SKILL_MANA_SHIELD		7   // 카이 - 마나 실드 
#define SKILL_FIRE_MASTERY		8	// 카이 - 파이어마스터리
#define SKILL_PROTECTION		9	// 보호 스크롤
#define SKILL_FIRE_PILLAR	    10	// 불기둥
#define SKILL_STONE_MASTERY		11	// 카이 - 스톤마스터리
#define SKILL_PEBBLE_SHOT	    12  // 페블샷 
#define SKILL_FALL_STONE	    13	// 낙석
#define SKILL_LIGHTNING_MASTERY	14	// 카이 - 라이트닝매스터리
#define SKILL_RECALL			15  // 귀한
#define SKILL_POWERUP_STAMINA	16	// 파워업스테미너
#define SKILL_IRON_SKIN			17	// 강철의몸
#define SKILL_DROP_ATTACK		18	// 내려찍기	
#define SKILL_RECOVERY			19	// 회복력증가
#define SKILL_GUN_MASTERY 		20  // 사격술
#define SKILL_WORK_GEM			21	// 보석세공술
#define SKILL_TELEPORT			22	// 텔레포트
#define SKILL_SUMMON_PARTY		23	// 파티소환
#define SKILL_ENERGYBALL		24	// 곤충소환술
#define SKILL_HEAL_OTHER		25	// 힐링
#define SKILL_STAR_FLARE	    26  // 스타 플레어
#define SKILL_SHOCK_SPEAR	    27  // 쇼크 스피어
#define SKILL_STONE_LANCE	    28  // 스톤 랑스
#define SKILL_MULTI_SHOT    	29  // 연사
#define SKILL_ELECTRIC_SHIELD	30  // 일렉트릭 실드
#define SKILL_ROTATE_ATTACK		31	// 돌려 베기
#define SKILL_SAFETY_SHIELD		32	// 세이프티실드
#define SKILL_ANIMAL_SUMMON		33	// 동물소환술
#define SKILL_PHENOMENON		34	// 페노메논
#define SKILL_SNIFING		    35  // 저격술
#define SKILL_HIDE			    36  // 잠복술
#define SKILL_IRON_MUSCLE		37	// 강철근육
#define SKILL_FURY_EXPLOSION	38	// 분노폭발
#define SKILL_INSIGHT			39	// 통찰의눈
#define SKILL_THROW				40	// 던지기 (슬레이어)
#define SKILL_CRUSH				41	// 분쇄
#define SKILL_AIMING		    42  // 조준력
#define SKILL_PIERCING	    	43  // 관통력 
#define SKILL_HANDLE		    44  // 조작술
#define SKILL_MAKING			45  // 조립술
#define SKILL_ABSORB_MAGIC		46  // 마법흡수
#define SKILL_CONCENTRATION		47	// 집중
#define SKILL_THOUGHT_STORM		48  // 사념의 폭풍
#define SKILL_BOOK_READ	    	49  // 고대어 해석
#define SKILL_MAGIC_MOVE	    50  // 마법이전 ( 완드 )
#define SKILL_DISTURBANCE		51	// 교란
#define SKILL_REMOVE			52  // 해제
#define SKILL_BLESSING			53	// 대지의 축복
#define SKILL_JOINT_LIFE		54	// 생명공유
#define SKILL_CHARMING			55	// 매혹의바람
#define SKILL_SUBMISSION		56	// 순종
#define SKILL_LIFE_LIGHT		57	// 생명의빛
#define SKILL_PEACEFUL			58	// 평온
#define SKILL_FORCE_ATTACK		60	// 불칸_포스어텍 (검기)
#define SKILL_FORCE_CONTROL		61	// 불칸_포스컨트롤
#define SKILL_BLOODY_MANA		62  // 카이_블러디마나 (2004)
#define SKILL_VANPIRE_TOUCH		63  // 카이_뱀파이어터치(2004)
#define SKILL_HELL_TOUCH		64  // 카이_헬터치(2004)
#define SKILL_DEEP_ATTACK		65  // 딥어택
#define SKILL_CHAIN_ATTACK	 	66  // 체인어택
#define SKILL_MELODY_OF_DEF	  	67	// 에이디아_보호의음계
#define SKILL_MELODY_OF_GUD		68	// 에이디아_수호의음계
#define SKILL_PASSION_OF_MELODY	69	// 에이디아_음악의열정
#define SKILL_PASSION_OF_FIGHT	70	// 에이디아_투지의음계
#define SKILL_HELL_CRY          71  // 불칸
#define SKILL_TORNADO_RUIN		72  // 토네이도 루인(다중스킬)
#define SKILL_SUMMON_PARTITION  73  // 에이디아	
#define SKILL_AIR_RAID          74  // 에어레이드
#define SKILL_HELL_CRY_CONTROLL 75  // 헬크라이컨트롤
#define SKILL_TORNADO_OPERATION 76	// 토네이도 컨토를
#define SKILL_AIR_RAID_CONCENT  77  // 에어레이드 컨센트
#define SKILL_SUMMON_CONTROL	78  // 서먼피션 컨트롤
// P는 패시브. A 는 맥티브,E 는 인첸트, C 는 자동 옌계기를 말한다.
#define SKILLF_P_MASTER_DSWORD  79  // 이도류마스터리
#define SKILLF_P_REC_UP         80  // 회복력 증가
#define SKILLF_P_HASTE          81  // 헤이스트	
#define SKILLF_A_DSLASH         82  // 더블 슬래쉬
#define SKILLF_P_EVASION        83  // 이베이션	
#define SKILLF_E_BLEED          84  // 블레드
#define SKILLF_A_DASH           85  // 대쉬
#define SKILLF_A_ASSAULT        86  // 어썰트	
#define SKILLF_C_DASSAULT       87  // 듀얼 어썰트 (자동 연계 기술)
#define SKILLF_E_CLOAKING       88  // 하이드
#define SKILLF_P_MASET_HAMMER   89	// 해머마스터리
#define SKILLF_P_VITAL_UP       90	// 생명력 증가
#define SKILLF_P_MASERT_HEAVY	91	// 해비 마스터리
#define SKILLF_A_BASH           92	// 배쉬
#define SKILLF_P_IRONSKIN       93	// 아이언스킨F
#define SKILLF_A_DSTONE         94	// 디바인 스톤
#define SKILLF_P_STONRINC       95	// 스톤 인크리스
#define SKILLF_A_TIMECHARGE     96	// 타임차지
#define SKILLF_C_STONEBRUST     97	// 스톤 버스트 ( 자동 연계 기술 )
#define SKILLF_E_REFLECTION     98	// 리플렉션
#define SKILL_B_BURNING_CONTROL	99	// nate 2005-06-07 : 불칸 - 스테미너 훈련
#define SKILL_B_BURNINGCRASH	100	// nate 2005-06-07 : 불칸 - 버닝 크래쉬
#define SKILL_C_DOWN_CONTROL	101	// nate 2005-06-07 : 카이 - 정신 집중
#define SKILL_C_HELLDOWN		102	// nate 2005-06-07 : 카이 - 다운헬
#define SKILL_H_MINE_CONTROL	103	// nate 2005-06-15 : 휴먼 - 가공술
#define SKILL_H_AUTOMINE		104	// nate 2005-06-15 : 휴먼 - 오토마인
#define SKILL_A_SUMMONS_SQUAD	105	// nate 2005-06-15 : 에댜 - 파티 소환
#define SKILL_A_LIFE_EXPANSION	106	// nate 2005-06-15 : 에댜 - 생명 공유
#define SKILL_B_FLYINGCRASH		107	// 불칸 - 플리잉 크래쉬
#define SKILL_K_POISON			108 // nate 2005-11-08 : 카이 - 포이즌
#define SKILL_H_ELECTRICSHOT	109 // 휴먼 - 일렉트릭샷
#define SKILL_A_SUMMON_FLOWER	110 // 에이다아 - 서먼플라워
#define SKILL_F_SHADOWSLASH		111	// 하이 - 쉐도우슬래쉬
#define SKILL_F_ADRENALINE		112	// 하이 - 아드레날린
#define SKILL_B_WILDSMASH	    113 // 불칸 - 와일드 스매쉬
#define SKILL_K_FLAMESPEAR	    114 // 카이 - 프레임 스피어
#define SKILL_H_RANDOMFIRE	    115 // 휴먼 - 난사
#define SKILL_A_SUMMONFIRE	    116 // 에이디아 - 서먼 파이어
#define SKILL_F_SPACESLASH	    117 // 하이 - 스페이스 슬래쉬
#define SKILL_F_ELECTRICHAMMER	118 // 하이 - 일렉트로닉 해머
#define SKILL_B_PROTECTION_SHIELDS 119 // 불칸 - 프로택션 실드 
#define SKILL_K_ICEBOLT           120 // 카이 - 아이스 볼트
#define SKILL_H_FROZENSHOT        121 // 휴먼 - 프로즌 샷
#define SKILL_A_RESURRECTION      122 // 에이디아 - 리저렉션
#define SKILL_F_MERCERNERY_SLASH  123 // 하이 - 머셔너리 슬래쉬
#define SKILL_F_TORNADO_HAMMER    124 // 하이 - 토네이도 해머


// 퍼롬 스킬
enum SKILL_PEROM
{
	PEROM_SKILL_S125 = 125 ,	// 스피어 마스터리(패시브)
	PEROM_SKILL_S126 = 126 ,	// 내성증가(패시브)
	PEROM_SKILL_S127 = 127 ,	// PL 스핀(액티브)
	PEROM_SKILL_S128 = 128 ,	// PL 쓰러스트(액티브)
	PEROM_SKILL_S129 = 129 ,	// PL 블레이더(액티브)
	PEROM_SKILL_S130 = 130 ,	// PL 스트라이크(액티브)
	PEROM_SKILL_S131 = 131 ,	// PL 피어스(액티브)
	PEROM_SKILL_S132 = 132 ,	// PL 붐(액티브)
	PEROM_SKILL_S133 = 133 ,	// PL 샷건(액티브)
	PEROM_SKILL_S134 = 134 ,	// PL 필드(액티브)
	PEROM_SKILL_S135 = 135 ,	// PL 피어스 봄버(액티브)
	PEROM_SKILL_S136 = 136 ,	// PL 휠윈드(액티브)
	PEROM_SKILL_S137 = 137 ,	// PL 알파 스트라이크(액티브)
	PEROM_SKILL_S138 = 138 ,	// PL 크로스(액티브)
	PEROM_SKILL_S139 = 139 ,	// D 시스템(메타버프)
	PEROM_SKILL_S140 = 140 ,	// D 오버로드(메타액티브)
	PEROM_SKILL_S141 = 141 ,	// D 클리어(메타액티브)
	PEROM_SKILL_S142 = 142 ,	// 인체 공학 - 타격(패시브)
	PEROM_SKILL_S143 = 143 ,	// 인체 공학 - 충격(패시브)
	PEROM_SKILL_S144 = 144 ,	// D 클리어 마스터리(패시브)
	PEROM_SKILL_S145 = 145 ,	// D 시스템-내성(패시브)
	PEROM_SKILL_S146 = 146 ,	// D 시스템-방어(패시브)
	PEROM_SKILL_S147 = 147 ,	// D 시스템-공격(패시브)
	PEROM_SKILL_S148 = 148 ,	// 크리티컬 스트라이크(버프)
	PEROM_SKILL_S149 = 149 ,	// 인게이지(버프)
	PEROM_SKILL_S150 = 150 ,	// D 시스템-필드(메타버프)

	SKILL_B_PASSIVE = 151,
	SKILL_K_PASSIVE = 152,
	SKILL_A_PASSIVE = 153,
	SKILL_H_PASSIVE = 154,
	SKILL_F_PASSIVE = 155,
	SKILL_P_PASSIVE = 156,
};



#define MAX_SKILL	157


// 확장 스킬들(skill.dta에 없이 별도로 만들어서 사용하는 스킬 종류)
#define SKILL_F_DASHSLASH		10001 // 하이 - 콤보스킬 대쉬->더블슬래쉬 ///하이콤보
#define SKILL_F_SLASHSHADOW		10002 // 하이 - 콤보스킬 대쉬슬래쉬->새도우슬래쉬 ///하이콤보
#define SKILL_FAIRY				10003 // 345레벨 착용 이펙트 페어리. 캐릭터 상태 이펙트이기때문에 k 패킷에 포함시킨다. ///페어리
#define SKILL_KNOCKBACK			10004 // 345레벨 아이템 특수효과 블러킹 시의 넉백. mv_skill 로 받기때문에 여기서 처리.
#define SKILL_F_STONEBASH1		10005 // 하이 - 콤보스킬 디바인스톤->배쉬 모션 시작 패킷.///하이콤보
#define SKILL_F_STONEBASH2		10006 // 하이 - 콤보스킬 디바인스톤->배쉬 타겟 패킷.///하이콤보
#define SKILL_F_BASHREFLECT1	10007 // 하이 - 콤보스킬 스톤배쉬->리플렉트 모션 시작 패킷. ///하이콤보
#define SKILL_F_BASHREFLECT2	10008 // 하이 - 콤보스킬 스톤배쉬->리플렉트 타겟 패킷. ///하이콤보
#define SKILL_PET_LIFE			10009 // 펫 공격형 스킬 "생명" 일정시간동안 HP 회복 (이펙트는 라이프 익스펜션)
#define SKILL_KNOCKBACK2		10010 // 스피릿시 주변 몬스터들의 넉백처리.
#define SKILL_LUCKYPIG1			10011 // 설날 복돼지.(페어리 비스므리한 방식.)
#define SKILL_LUCKYPIG2			10012 // 설날 복돼지.(페어리 비스므리한 방식.)
#define SKILL_LUCKYPIG3			10013 // 설날 복돼지.(페어리 비스므리한 방식.)
#define SKILL_LUCKYPIG4			10014 // 설날 복돼지.(페어리 비스므리한 방식.)


// [7/31/2007 Theodoric] 6주년 기념 이벤트 베이비 소울 (페어리 비스므리한 방식)
#define SKILL_BABYSOUL_1		10015	// 포치
#define SKILL_BABYSOUL_2		10016	// 히치
#define SKILL_BABYSOUL_3		10017	// 에치
#define SKILL_VAMPIREBAT		10018 ///뱀파이어망토 박쥐
#define SKILL_PETRUDOLPH		10019 ///펫루돌프
#define SKILL_RING_LIGHT_WHITE	10020 // [12/4/2007 parkmj] 일본 링아이템 이펙트
#define SKILL_RING_LIGHT_DARK	10021 // [12/4/2007 parkmj] 일본 링아이템 이펙트
#define SKILL_RING_LIGHT_RED	10022 // [12/4/2007 parkmj] 일본 링아이템 이펙트
#define SKILL_RING_LIGHT_BLUE	10023 // [12/4/2007 parkmj] 일본 링아이템 이펙트
#define SKILL_RING_LIGHT_GREEN	10024 // [12/4/2007 parkmj] 일본 링아이템 이펙트
#define SKILL_PETRUDOLPH_BLACK	10025 // [12/10/2007 parkmj] 펫루돌프_검정
#define SKILL_PETRUDOLPH_BLUE	10026 // [12/10/2007 parkmj] 펫루돌프_파랑
#define SKILL_PETRUDOLPH_GREEN	10027 // [12/10/2007 parkmj] 펫루돌프_녹색
#define SKILL_PETRUDOLPH_YELLOW	10028 // [12/10/2007 parkmj] 펫루돌프_노랑
#define SKILL_PETRUDOLPH_KOR_PREMIUM 10029 // [2008/11/3 Theodoric] 국내 프리미엄 상품
#define SKILL_ARUPAKA_PET			10030 // 알카파펫
#define SKILL_ARUPAKA_PET_BE		10031 // 알카파펫
#define SKILL_ARUPAKA_PET_BR		10032 // 알카파펫
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



#define SKILL_K_FLAMESPEAR_TARGET 10114 // 플레임 스퍼어의 타겟 패킷보내기용.
//====================================================================================

//==========================================================================
// nate 2005-06-07 : 스킬 이펙트 번호
#define EFFECT_SKILL_FIRE_BALL			3   // 파이어볼
#define EFFECT_SKILL_LIGHTNING_BALL		4   // 라이트닝 볼
#define EFFECT_SKILL_LIGHTNING		    5   // 라이트닝
#define EFFECT_SKILL_HEAL_SELF			6	// 에이디아 - 리커버리
#define EFFECT_SKILL_PROTECTION			9	// 보호 스크롤
#define EFFECT_SKILL_FIRE_PILLAR	    10	// 불기둥
#define EFFECT_SKILL_PEBBLE_SHOT	    12  // 페블샷 
#define EFFECT_SKILL_FALL_STONE			13	// 낙석
#define EFFECT_SKILL_ENERGYBALL			24
#define EFFECT_SKILL_HEAL_OTHER			25
#define EFFECT_SKILL_STAR_FLARE			26  // 스타 플레어
#define EFFECT_SKILL_SHOCK_SPEAR	    27  // 쇼크 스피어
#define EFFECT_SKILL_STONE_LANCE	    28  // 스톤 랑스
#define EFFECT_SKILL_IRON_MUSCLE		37	// 강철근육
#define EFFECT_SKILL_FURY_EXPLOSION		38	// 분노폭발
#define EFFECT_SKILL_PIERCING	    	43  // 관통력 
#define EFFECT_SKILL_THOUGHT_STORM		48  // 사념의 폭풍
#define EFFECT_SKILL_BLESSING			53	// 대지의 축복
#define EFFECT_SKILL_JOINT_LIFE			54	// 생명공유
#define EFFECT_SKILL_LIFE_LIGHT			57	// 생명의빛
#define EFFECT_SKILL_PEACEFUL			58	// 평온
#define EFFECT_SKILL_FORCE_ATTACK		60	// 불칸_포스어텍 (검기)
#define EFFECT_SKILL_BLOODY_MANA		62  // 카이_블러디마나 (2004)
#define EFFECT_SKILL_VANPIRE_TOUCH		63  // 카이_뱀파이어터치(2004)
#define EFFECT_SKILL_HELL_TOUCH			64  // 카이_헬터치(2004)
#define EFFECT_SKILL_MELODY_OF_DEF	  	67	// 에이디아_보호의음계
#define EFFECT_SKILL_MELODY_OF_GUD		68	// 에이디아_수호의음계
#define EFFECT_SKILL_HELL_CRY			71  // 불칸 - 헬크라이
#define EFFECT_SKILL_TORNADO_RUIN		72  // 카이 - 토네이도 루인(다중스킬)
#define EFFECT_SKILL_SUMMON_PARTITION	73  // 에이디아	- 서먼피션
#define EFFECT_SKILL_AIR_RAID			74  // 휴먼 - 에어레이드
#define EFFECT_SKILL_SUMMONSQUAD		75	// nate 2005-07-08 : Summon Squad - Effect Num
#define EFFECT_SKILL_MINE				76	// nate 2005-06-15 : 휴먼 스킬 추가 - Auto Mine
#define EFFECT_SKILL_BURNINGCRASH		77	// nate 2005-06-16 : 불칸 스킬 추가 - Burning Crash
#define EFFECT_SKILL_BURNINGCRASH2		78	// nate 2005-06-16 : 불칸 스킬 추가 - Burning Crash
#define EFFECT_SKILL_HELLDOWN			79	// mungmae-2005/07/04 : 카이립톤 헬다운
#define EFFECT_SKILL_LIFEEXPENSION		80	// nate 2005-07-05 : 에이디아 스킬 - Life Expension
#define EFFECT_SKILL_FLYINGCRASH		81	// 불칸 - 플라잉 크래쉬(시작시)
#define EFFECT_SKILL_POISON				82	// nate 2005-11-03 : 카이 - 포이즌
#define EFFECT_SKILL_ELECTRICSHOT		83	// 휴먼 - 일렉트릭 샷
#define EFFECT_SKILL_SHADOWSLASH2		84	// 하이 - 쉐도우 슬레쉬(공격시)
#define EFFECT_SKILL_ADRENALINE			85	// 하이 - 아드레날린
#define EFFECT_SKILL_FLYINGCRASH2		86	// 불칸 - 플라잉 크래쉬(타격시)
#define EFFECT_SKILL_SHADOWSLASH		87	// 하이 - 쉐도우 슬레쉬(시작시)
#define EFFECT_SKILL_FLAMESPEAR			88	// 카이 - ///플레임스피어
//==============================================================================================
#define EFFECT_SKILL_SLAYER				97	// 슬레이어 
#define EFFECT_SKILL_CHACRAM			98	// 챠크람
#define EFFECT_SKILL_ICE				99	// 얼음 공격
#define EFFECT_CONDITION				100	// 상태(석화,마비..) 효과
#define EFFECT_OPTIONSKILL				101	// 지속스킬 효과
#define EFFECT_HITPOWER					102	// 특수공격(크리티컬,카운트...)효과
#define EFFECT_BLACKHOLE				104	// 블랙홀
#define EFFECT_BOOK 					105	// 책마법 맞는 효과
#define EFFECT_USE_BOOK					106	// 책쓰는효과
#define EFFECT_REMOVE 					107	// 해제 
#define EFFECT_USE_BOOK_SPELL			108	// 책마법 쓸때 올라가는 효과
#define EFFECT_LIFE_LIGHT 				108	// 생명의 빛 효과
#define EFFECT_SKILL_SPELL				109 // 스킬쓸때 올라가는 효과
#define EFFECT_CHA_LIFE_LIGHT			109	// 생명의 빛 타겟효과
#define EFFECT_JOINT_LIFE 				110	// 생명공유 효과
#define EFFECT_AUTOLASER				110	// 자동레이저
#define EFFECT_CHARGECANNON				111	// 충전포
#define EFFECT_FLAME					112	// 화염방사기
#define EFFECT_S_EQUIP					113	// 소형장비 켜고 끌때
#define EFFECT_SCANVIEW					114	// 공간제어기 사용시
#define EFFECT_SMALL_ROCKET				115	// 소형로켓
#define EFFECT_MIDDLE_ROCKET			116	// 중형로켓
#define EFFECT_GATHERING				117	// 채집기
#define EFFECT_SKILL_DSTONE				118	// 이펙트 디바인 스톤
#define EFFECT_SKILL_DSLASH				119	// 이펙트 더블 슬래쉬
#define EFFECT_SKILL_BASH				120	// 이펙트 배쉬
#define EFFECT_DSTONE_EFF				121	// 디바인 스톤이 박힌 후의 Effect
#define EFFECT_SKILL_ASSAULT			122	// 어썰트의 Effect
#define EFFECT_SKILL_DASSAULT			123	// 듀얼 어썰트의 Effect
#define EFFECT_SKILL_TIMECHG			124	// 타임차지의 Effect
#define EFFECT_DEMICRUSHER				125	// 데미크루셔
#define EFFECT_FIRETOWER_EFF			126	// 메트릭스 기둥 효과
#define EFFECT_SKILL_STONEBRUST			127	// 스톤 버스트
#define EFFECT_SKILL_HOLLEY				128	// 홀리 스테프
#define EFFECT_SPEEDUPSMOKE				129	// 월드컵 스피드업 연기 효과
#define EFFECT_SPIRITMODE				130	// 스피릿 모드때 효과(발동때 효과)
#define EFFECT_SPIRITMODE2				131	// 스피릿 모드때 효과(지속적 효과)
#define EFFECT_SPACE_SLASH				132	// 하이 - 스페이스 슬레쉬
#define EFFECT_ELECTRIC_HAMMER			133	// 하이 - 일렉트로닉 해머
#define EFFECT_SKILL_SUMMONFIRE			134	// 에이 - 서먼파이어
#define EFFECT_FROZEN_SHOT				135	// 휴먼 - 프로즌 샷
#define EFFECT_SKILL_RANDOMFIRE			136	// 휴먼 - 랜덤파이어
#define EFFECT_SKILL_MERCERNERYSLASH	137	// 하이 - 머셔너리 슬래쉬
#define EFFECT_SKILL_ICEBOLT			138	// 카이 - 아이스볼트
#define EFFECT_SKILL_WILDSMASH			139	// 불칸 - 와일드 스매쉬
#define EFFECT_SKILL_RESURRECTION		140	// 에이 - 리져렉션
#define EFFECT_SKILL_TORNADO_HAMMER		141	// 하이 - 토네이도 해머
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
#define EFFECT_SKILL_RING_DRAGON2		2317 // 진 드래곤링 ///컨버젼무기
#define EFFECT_SKILL_RING_SPHINX		1982
#define EFFECT_SKILL_RING_DEVIL			2065
#define EFFECT_SKILL_RING_DEVIL2		2326 // 진 데빌링 ///컨버젼무기
#define EFFECT_SKILL_RING_BULKAN		2417 ///불칸링

#define EFFECT_SKILL_RING_SARNGA		2418 // [12/3/2008 Theodoric] 사릉가
#define EFFECT_SKILL_RING_SARNGA2		2419 // [12/3/2008 Theodoric] 신 사릉가
#define EFFECT_SKILL_RING_GRIFFON		2420 // [12/3/2008 Theodoric] 신 사릉가
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
#define EFFECT_SKILL_WAND_CHAOS			1774  // 혼돈의 폭풍 완드        
#define EFFECT_SKILL_WAND_WAVE_OF_DEATH	1775  // 죽음의 물결 완드

#define EFFECT_DROPCANDY						50000	// 왕사탕 임시 번호
#define EFFECT_DROPAPPLE						50001	// 대빵사과 임시 번호
#define EFFECT_POISON_FOG						50002	// 데카렌 보스방 독안개 ///인던1
#define EFFECT_HEART_PARTICLE					50003   // 원석 2007-01-18 : ///하트로드 파티클 이펙트 
#define EFFECT_PIG_HEALING						50004   // 원석 2007-02-01 : // 설날 복돼지 회복 이펙트
#define EFFECT_LIP_PARTICLE						50005   // 2007.03.14 화이트
#define EFFECT_LOVE_PARTICLE					50006   // 2007.03.14 화이트
#define	EFFECT_MURAMASA_GHOST					50007   ///일본디자인컨테스트 아이템 무라마사 의 귀신 이펙트.
#define EFFECT_DROPSTARCANDY					50008	// 별사탕 임시 번호

//  [10/30/2008 parkmj] 일본요청 - 왕사탕(30/90분)
#define EFFECT_DROPCANDY30						50009	// 왕사탕30
#define EFFECT_DROPCANDY90						50010	// 왕사탕90

//  [3/23/2009 ppmmjj83] 일본요청 - 대야 / 선더 / 소드
#define EFFECT_DROPBASIN0						50011	// 대야(10분)
#define EFFECT_DROPBASIN1						50012	// 대야(30분)
#define EFFECT_DROPBASIN2						50013	// 대야(60분)
#define EFFECT_DROPTHUNDER0						50014	// 선더(10분)
#define EFFECT_DROPTHUNDER1						50015	// 선더(30분)
#define EFFECT_DROPTHUNDER2						50016	// 선더(60분)
#define EFFECT_DROPSWORD0						50017	// 소드(10분)
#define EFFECT_DROPSWORD1						50018	// 소드(30분)
#define EFFECT_DROPSWORD2						50019	// 소드(60분)

#define EFFECT_DROPAPPLEPVP						50020



#define EFFECT_SKILL_STONEBASH					10005   // 하이브리더 콤보스킬 스톤 배쉬. ///하이콤보
#define EFFECT_SKILL_BASHREFLECTION				10008   // 하이브리더 콤보스킬 배쉬리플렉션. ///하이콤보
#define EFFECT_SKILL_BASHREFLECTION2			100082  // 하이브리더 콤보스킬 배쉬리플렉션. ///하이콤보
//==========================================================================
#define ATTACK_COUNT_HIT		1		//카운트
#define ATTACK_CRITICAL_HIT		2		//크리티컬	
#define ATTACK_DEMOLITION_HIT	3		//데몰리션
#define ATTACK_CROSS_HIT		4		//절단
#define ATTACK_VAMPIRE_HIT		5		//피 빨기 어택 ///일본디자인컨테스트 아이템 무라마사의 효과.

// nate 2005-06-07 : 상태 분류
#define CONDITION_NORMAL		0		//정상
#define	CONDITION_STONY			1		//석화
#define	CONDITION_PARALYSIS		2		//마비
#define	CONDITION_CHAOS			3		//혼돈
#define	CONDITION_FREEZE		4		//냉각
#define	CONDITION_SHOCK			5		//감전
#define CONDITION_ENTANGLE		6		//묶임

//============================================================================
// nate 2005-06-07 : 몬스터 이펙트
#define EFFECT_SKILL_MOB_SHILON					4000
#define EFFECT_DURANT_BEAM						4001	// nate 2005-09-26 : 듀란트 - 광선
#define	EFFECT_DURANT_ATTACK					4002	// nate 2005-09-26 : 듀란트 - 어택
#define EFFECT_DURATT_TARGET					4003	// nate 2005-09-29 : 듀란트 - 대상에게 보여지는 이펙트
#define EFFECT_CACTUS							4004	// nate 2005-11-22 : 소환 캐크터스
#define EFFECT_DANDELION						4005	// nate 2005-11-22 : 댄더리온
#define EFFECT_SPORE							4006	// nate 2005-11-22 : 스포어
#define EFFECT_SKILL_MOB_SNAKEBOSS				4007	// nate 2006-01-03 : 트미트론 - 보스메듀사
#define EFFECT_SKILL_MOB_SNAKEBOSS2				4008	// nate 2006-01-17 : 메듀사
#define EFFECT_SKILL_MOB_SNAKEBOSS3				4009	// nate 2006-01-26 : 서펀트퀸 - 다중
#define EFFECT_SKILL_MOB_LEGKNOCK1				4010	// 데카렌 인던 보스 레그녹 일반공격 이펙트. ///인던1
#define EFFECT_SKILL_MOB_LEGKNOCK2				4011	// 데카렌 인던 보스 레그녹 다중공격 이펙트. ///인던1
#define EFFECT_DECKAREN_CUT_IN					4012	// 데카렌 1층 돌문 앞 이벤트 컷인. 총 4장. ///인던1
#define EFFECT_KOREA_SERVICE_LEVEL_MARK			4013	// 한국 심의 등급 표시용 마크.

#define EFFECT_CHA_DONT_ATTACK_STATE			4014	// 캐릭터 무적상태 서버에서 on/off 해준다.

#define EFFECT_SKILL_MOB_WORLDCUP_HAND			5001
#define EFFECT_SKILL_MOB_WORLDCUP_FOOT			5002
#define EFFECT_SKILL_MOB_GUARDTOWER_FIRE		5003
#define EFFECT_SKILL_MOB_SHILONMASTER			5004
#define EFFECT_SKILL_CONTROL_LIGHT_GUARD		5005
#define EFFECT_SKILL_CONTROL_LIGHT_DAMAGE		5006
#define EFFECT_SKILL_MOB_GUARDTOWER_LIGHTBALL	5007
#define EFFECT_SKILL_MOB_GUARDTOWER_PEBBLESHOT	5008
#define EFFECT_SKILL_MOB_BABYWORM				5009	// 새끼벌레
#define EFFECT_SKILL_MOB_MOB_SHINING_JAKAL		5010	// 1층보스=샤이닝자캴
#define EFFECT_SKILL_MOB_ACCUMULATE				5011	// 몹의 누적공격
#define EFFECT_SKILL_MOB_FIRE					5012	// 몹의 불속성 공격
#define EFFECT_SKILL_DRAGON_FIRE				5013	// 드레곤의 불공격
#define EFFECT_SKILL_DRAGON_ICE					5014	// 드레곤의 냉기공격
#define EFFECT_SKILL_KHAN_HIT					5015	// 칸의 지역공격 
#define EFFECT_SKILL_WARGIANT_MULTI				5016	// 워자이언트의 지역공격 
#define EFFECT_SKILL_MOB_SOULDUST				5017	// 몹의 영혼먼지 공격 
#define EFFECT_SKILL_MOB_FALLOFFSTONE			5018	// 몹의 낙석 공격 
#define EFFECT_SKILL_MOB_STONERANCE2			5019	// 몹의 스톤랑스 변형 공격 
#define EFFECT_SKILL_MOB_DRILLSTONE				5020	// 몹의 밑에서 솟는 송곳 공격 
#define EFFECT_SKILL_PET_BLESS					5021    // 펫 스킬 브레스 이펙트.
#define EFFECT_SONGKRAN_WTRSPRINKLE				5022	// [3/27/2007 Theodoric] 태국 송크란 이벤트

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
#define EFFECT_BOSSRAID_AIDIA3_01		5054	// [3/6/2008 반재승] 보스레이드 에이디아보스3 노멀 어택
#define EFFECT_BOSSRAID_LASTBOSS_01		5055	// 예약
#define EFFECT_BOSSRAID_LASTBOSS_04		5056

#define EFFECT_HEROEFFECT				5057

#define EFFECT_SKILL_WAND_WAVE_OF_DEATH2	5058
#define EFFECT_SKILL_WAND_WAVE_OF_DEATH3	5059

//============================================================================
#define EFFECT_NOTSKILL_SPEEDUP					1
#define EFFECT_NOTSKILL_SPIRITMODE				2


//#define MAX_SKILL_LEVEL		30
#define SKILL_UNSELECTED	(-50)
#define SELECTED_SKILL_ATTACT (-1)	// 일반 공격
#define SELECTED_SKILL_ROTATE (-2)	// 카메라 회전

//----------------------------------------------------------------------------
//-- 스킬에 관련 함수들
//-- 2004.11.19
//-- Lyul / 추가
void G_SK_SetCooltime(int li_shdidx, int li_curlevel = 0);     //-- 각 스킬의 시간 Delay 를 지정한다. 없으면 무시
void G_SK_RotateShadow();               //-- 스킬의 시간 지연을 감소 시킴
//-- 현재 스킬 지연 Shadow 값(mungmae-2005/05/09 int li_shdidx2 추가)
// 디바인 스톤은 다른 스킬과 달리 스킬이 올라갈때 마다 Shadow 값이 달라지기 때문에
// li_shdidx 현재 스킬, li_shdidx2 스킬 레벨
int  G_SK_GetShdangle(int li_shdidx, int li_shdidx2);
void G_SK_ResetShadow();								//-- 존 이동 및 기타 Reset 타이밍에 필요한 함수
bool G_SK_Getflag(int li_shdidx);       //-- 현재 스킬의Shadow bool 값
bool G_SK_IsOnceAttack(int cur_skill);  //-- 한번만 공격 되는 스킬인지 판단.
bool G_SK_IsSecAttack(int cur_skill);   //-- Secattack 인지 판단
bool G_SK_IsEffectSkill(int li_skill);   //-- Effct 가 필요한지 판단.
void G_SK_INVCooltime(int li_skill);      //-- - 값을 + 로 변환해서 진행

#endif // SKILL_H
