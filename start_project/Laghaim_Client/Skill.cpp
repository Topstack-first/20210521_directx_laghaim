#include "stdafx.h"
#include "io.h"
#include "Skill.h"
#include "ddutil.h"
#include "BasicButton.h"
#include "TextOutBox.h"
#include "ControlSkill.h"
#include "NkCharacter.h"
#include "main.h"
#include "UIMgr.h"
#include "ChaAct.h"
#include "TcpUtil.h"
#include "Country.h"
#include "LHExcelReader.h"
#include "CommonConfig.h"
#include "SkillExtendManager.h"

CSkill * g_Skills = NULL;
int g_SkillsNum = 0;

CRaceSkill g_RaceSkill[NUM_RACE];
CChaSkill g_ChaSkill;
enum {SKILL_DTA = 0x70, RACE_SKILL = 0x57, BPDINDEX = 0x86, BPDLIST = 0x23, RACECSY = 0x94};

extern DWORD	g_dwLangType; ///skill.dta 암호화.
extern DWORD	g_dwClientCountry;

//----------------------------------------------------------------------------
//-- Skill Shadow  에서 사용될 각 스킬의 Index 값
//----------------------------------------------------------------------------
//-- ( CoolTime / 쿨타임 과 같은 의미 shadow 로 사용 )
//-- 2004.11.6
//-- Lyul / Skill
//-- 모든 스킬 중 시간 지연을 필요로 하는 스킬들의 Shadow 에 사용될 Define
//-- 일부는 서버응답 후 세팅
//-- Define 을 여기에 둬야 외부에서 실수로 인한 Define 혼동을 막을 수 있다.

//-- 불칸
#define SKILL_SHD_HELL_CRY			1	// 헬 크라이		: 사용뒤 8
//-- 카이
#define SKILL_SHD_REMOVE			2	// 해제				: 사용뒤 3
#define SKILL_SHD_VAMPIRE_TOUCH		3	// 뱀파이어 터치	: 사용뒤 5
#define SKILL_SHD_HELL_TOUCH		4	// 헬 터치			: 사용뒤 20
#define SKILL_SHD_TORNADO_RUIN		5	// 토네이도 루인	: 사용뒤 10 
//-- 휴먼
#define SKILL_SHD_AIR				6	// 에어레이드		: 사용뒤 10        	    	
//-- 에이디아
#define SKILL_SHD_BLESSING			7	// 대지의 축복		: 사용뒤 7
#define SKILL_SHD_JOINT_LIFE		8	// 생명공유		: 해제뒤 60
#define SKILL_SHD_LIFE_LIGHT		9	// 생명의 빛		: 사용뒤 5
#define SKILL_SHD_TELEPORT			10  // 텔레포트			: 사용뒤 1
#define SKILL_SHD_MELODY_OF_DEF		11  // 보호의 음계		: 사용뒤 15
#define SKILL_SHD_SUMMON			12  // 서먼피션			: 사용뒤 10
//-- 프리크 (신종족)
#define SKILL_SHD_DSLASH			13  // 더블 슬래쉬		: 사용뒤 3초
#define SKILL_SHD_DASH				14  // 대쉬				: 사용뒤 2초
#define SKILL_SHD_ASSAULT			15  // 어썰트			: 사용뒤 7초
#define SKILL_SHD_BASH				16  // 배쉬				: 샤용뒤 5초
#define SKILL_SHD_DSTONE			17  // 디바인 스톤	: 해제뒤 117초 
#define SKILL_SHD_TIMECHG			18  // 타임차지			: 사용뒤 30초 
#define SKILL_SHD_REFLECTION		19  // 리플렉션		: 해제뒤 20초 
#define SKILL_SHD_CLOAKING			20  // 하이드		: 해제뒤 60초

#define SKILL_SHD_FORCE				21  // mungmae-2005/05/16 포스어택	: 사용뒤 6초
#define SKILL_SHD_BURNINGCRASH		22	// nate 2005-06-07 : Burning Crash - CoolTime
#define SKILL_SHD_AUTOMINE			23	// nate 2005-06-23 : Auto Mine - CoolTime
#define SKILL_SHD_HELLDOWN			24	// mungmae-2005/07/15 : 헬다운 쿨타임 시간
#define SKILL_SHD_SUMMONS_SQUAD		25	// nate 2005-07-15 : Summon Squad - Cool Time
// 2006 New Skill
#define SKILL_SHD_FLYINGCRASH		26	// 불칸 - 플라잉 크래쉬
#define SKILL_SHD_POISON			27	// nate 2005-11-08 : 포이즌
#define SKILL_SHD_ELECTRICSHOT		28	// 휴먼 - 일렉트릭 샷
#define SKILL_SHD_SHADOWSLASH		29	// 하이 - 쉐도우 슬레쉬
#define SKILL_SHD_ADRENALINE		30	// 하이 - 아드레날린
#define SKILL_SHD_SPACESLASH		31	// 하이 - 스패이스 슬래쉬
#define SKILL_SHD_ELECTRICHAMMER	32	// 하이 - 일렉트로닉 해머
#define SKILL_SHD_FROZENSHOT		33	// 휴먼 - 프로즌 샷.
#define SKILL_SHD_FRAMESPEAR		34	// 카이 - 프레임 스피어.
#define SKILL_SHD_RESURRECTION		35	// 에이 - 리저렉션
#define SKILL_SHD_ICEBOLT			36	// 카이 - 아이스볼트
#define SKILL_SHD_MERCERNERY_SLASH  37  // 하이 - 머셔리스 슬래쉬
#define SKILL_SHD_WILDSMASH			38  // 불칸 - 와일드 스매쉬
#define SKILL_SHD_PROTECTION_SHIELDS	39  // 불칸 - 프로택션 쉴드
#define SKILL_SHD_SUMMONFIRE		40  // 에이 - 서먼 파이어 
#define SKILL_SHD_RANDOMFIRE		41  // 휴먼 - 랜덤 파이어
#define SKILL_SHD_TORNADO_HAMMER	42  // 하이 - 토네이도 해머
// 스킬이 추가될때마다 맥스값을 갱신한다.

enum eSKILL_SHD
{
	SKILL_SHD_PEROM_SKILL_S125 = 43,
	SKILL_SHD_MAX
};

const char * SKILL_INFO_FILENAME = "data/language/Skill_New_Cool.txt";  //PABLO  , try hide file ,ant hack

//-- 각 스킬별 MAX DELAY 를 지정.
//-- (-)일 경우 무한대로 설정되고 별도의 처리가 없는한 시간이 줄지 않는다.
//-- 서버에서 지시가 올경우 -(MAX) 로 세팅되어 시간 감소가 된다.
int  gi_SkillShadowMAX[SKILL_SHD_MAX]=
{
	0,										// Dummy
	6000,										// 불칸
	3000, 5000, 20000,10000,					// 카이
	12000,									// 휴먼
	7000, -60000, 5000, 1000, 7000, 10000,	// 에디아
	3000, 2000, 7000, 5000,-117000, 30000, 20000, 110000, 6000,  // 프리크 어썰트 7초
	20000,	// nate 2005-06-07 : Burning Crash - 쿨타임 시간
	12000,	// nate 2005-06-23 : Auto Mine - 쿨타임 시간
	30000,	// mungmae-2005/07/15 : 헬다운 쿨타임 시간
	600000,	// nate 2005-07-15 : Summon Squad - Cool Time
	15000,	// 불칸 - 플라잉 크래쉬
	15000,	// nate 2005-11-08 : 포이즌 - CoolTime
	15000,	// 휴먼 - 일렉트릭 샷
	15000,	// 하이 - 쉐도우 슬래쉬
	15000,	// 하이 - 아드레날린k
	18000,  // 하이 - 스페이스 슬래쉬
	18000,  // 하이 - 일렉 해머
	8000,  // 휴먼 - 프로즌 샷. // 수치는 임시.
	18000,  // 카이 - 프레임 스피어. // 수치는 임시.
	15000,  // 에이 - 리져렉션
	4000,  // 카이 - 아이스 볼트
	20000,  // 하이 - 머셔리스 슬래쉬
	18000,	// 불칸 - 와일드 스매쉬
	10000,  // 불칸 - 프로택션 쉴드
	10000,  // 에이 - 서먼 파이어
	18000,  // 휴먼 - 난사
	10000,   // 하이 - 토네이도 해머
	1000,
};


int		gi_SkillShadowTime[MAX_SKILL]= {0,};
bool	gb_SkillShadow[MAX_SKILL]= {false,};

int		pasttime = 0;


extern int UTF8ToUnicode(wchar_t* wcstr, const char * utfstr, size_t count);
// {
// 	if( count == 0 && wcstr != NULL )
// 		return 0;
//
// 	int result = MultiByteToWideChar(CP_UTF8 , 0 , utfstr , -1 , wcstr , (int)count);
// 	if( result > 0 )
// 		wcstr[result-1] = 0;
//
// 	return result;
// 	return 0;
// }

extern int UniToMulti(char * mbstr, const wchar_t*wcstr , size_t count);
// {
// 	if( count == 0 && mbstr != NULL )
// 		return 0;
//
// 	int result = ::WideCharToMultiByte( CP_ACP , 0 , wcstr , -1 , mbstr , (int)count , NULL, NULL );
// 	if( result > 0 )
// 		mbstr[result-1] = 0;
// 	return result;
// }

//-- Skill Index 를 Shadow Index 로 변환한다.
int L_SK_ConvertSKILLtoSHD(int li_curskill)
{

	if( !g_Skills  )
		return 0;

	if( g_Skills[li_curskill].m_cool_time != 0 )
		return g_Skills[li_curskill].m_cool_time;

	return 0;
}

//-- 절대치로 부터 떨어진 값을 계산하여 각도로 Return
//-- 현재 값이 음수일 경우 무한 대기 -> 서버에서 지시가 와야 시간 Setting
int G_SK_GetShdangle(int li_curskill, int li_curskill2 )
{

	int cool_time = L_SK_ConvertSKILLtoSHD(li_curskill);
	if( cool_time < 200 )
	{
		cool_time = 200;
	}

	int ret_val = 0;

	if( ( li_curskill == SKILL_TELEPORT || li_curskill == SKILLF_A_DASH )
			&& g_pRoh->m_CancelState
			&& ( g_pRoh->m_ChaCondition == CONDITION_STONY ||
				 g_pRoh->m_ChaCondition == CONDITION_ENTANGLE))
	{
		ret_val = (abs(cool_time * 3) - gi_SkillShadowTime[li_curskill]) *
				  360 / abs(cool_time * 3);
	}
	else
	{
		ret_val = (abs(cool_time) - gi_SkillShadowTime[li_curskill]) *
				  360 / abs(cool_time);
	}


	return ret_val;
}

//-- 모든 Shadow를 초기화 한다.
//-- 존 이동 및 필요시 사용
void G_SK_ResetShadow()
{
	for(int i= 0 ; i < SKILL_SHD_MAX; i++)
	{
		gb_SkillShadow[i] = false;
		gi_SkillShadowTime[i] = 0;
	}
}

bool G_SK_Getflag(int li_curskill)
{
	return gb_SkillShadow[li_curskill];
}

//-- 지정된 스킬의 Cooltime 와 시간을 Setting 한다.
//-- Cooltime 를 가지지 않는 스킬은 무시된다.
extern bool g_bNoCool;
void G_SK_SetCooltime(int li_curskill, int li_curlevel)
{
	if( g_bNoCool == true )
		return;

	int cool_time = L_SK_ConvertSKILLtoSHD(li_curskill);
	if( cool_time < 200 )
	{
		cool_time = 200;
	}

	if( ( li_curskill == SKILL_TELEPORT || li_curskill == SKILLF_A_DASH )
			&& g_pRoh->m_CancelState
			&& ( g_pRoh->m_ChaCondition == CONDITION_STONY || g_pRoh->m_ChaCondition == CONDITION_ENTANGLE) )
		gi_SkillShadowTime[li_curskill] = cool_time * 3;
	else
		gi_SkillShadowTime[li_curskill] = cool_time + (li_curlevel?(li_curlevel - 1) * 3000:0);

	gb_SkillShadow[li_curskill] = true;

	for(int i= 0 ; i < MAX_SKILL; i++)
	{	
		if( gi_SkillShadowTime[i] < 200 )
		{
			gi_SkillShadowTime[i] = 200;
			gb_SkillShadow[i] = true;
		}
	}

	pCMyApp->m_bHellDown_Cooltime = TRUE;
}

//-- - 로 세팅된 skill 을 +로 변환시켜 Cooltime을 진행시킨다.
//-- Cooltime 를 가지지 않는 스킬은 무시된다.
//-- - 가 아닐경우 두번의 Invs 로 다시 원래대로 돌아감을 방지.
void G_SK_INVCooltime(int li_curskill)
{
	int cool_time = L_SK_ConvertSKILLtoSHD(li_curskill);
	if( cool_time < 200 )
	{
		cool_time = 200;
	}

	if(gi_SkillShadowTime[li_curskill] < 0)
	{
		gi_SkillShadowTime[li_curskill] = -gi_SkillShadowTime[li_curskill];
	}

	gb_SkillShadow[li_curskill] = true;

}

//-- 지정된 스킬의 Shadow 를 진행한다.
void G_SK_RotateShadow()
{
	int curtime = timeGetTime();
	int frame =curtime - pasttime;
	pasttime = curtime ;

	for(int i= 0 ; i < MAX_SKILL; i++)
	{
		//-- Shadow 가 Set 된 Skill 만 진행
		if(gb_SkillShadow[i] && gi_SkillShadowTime[i] > 0)
		{
			gi_SkillShadowTime[i] -=frame;
			if(gi_SkillShadowTime[i] <= 0)
			{
				gi_SkillShadowTime[i] = 1;
				gb_SkillShadow[i] = false;
				pCMyApp->m_bHellDown_Cooltime = FALSE;
			}
		}
	}
}

//-- 현재 Skill 이 단일 Attack 인가.
//-- Lyul / Skill
bool G_SK_IsOnceAttack( int cur_skill )
{
	int li_skill = g_ChaSkill.m_SkillIndex[ cur_skill ];

	if(li_skill == SKILL_BLESSING
			|| li_skill == SKILL_A_LIFE_EXPANSION	// nate 2005-07-06 : Life Expansion - OnceAttack Check
			|| li_skill == SKILL_JOINT_LIFE			//-- 대지축복 / 생명공유
			|| li_skill == SKILL_LIFE_LIGHT
			|| li_skill == SKILL_MELODY_OF_DEF		//-- 생명의 빛 / 보호의 음계
			|| li_skill == SKILL_REMOVE )
		return true;

	return false;
}

//-- 현재 Skill 이 Second Attack 이면 true
//-- Lyul / Skill
bool G_SK_IsSecAttack(int cur_skill)
{
	if( cur_skill < 0 )
		return false;

	int li_skill = g_ChaSkill.m_SkillIndex[cur_skill];

	if( li_skill == SKILL_THROW
			|| li_skill == SKILL_FORCE_ATTACK
			|| li_skill == SKILL_HELL_CRY			// 슬레이어(차크람) | 헬크라이
			|| li_skill == SKILL_TORNADO_RUIN
			|| li_skill == SKILL_MAGIC_MOVE			// 토네이도 루인 | 완드
			|| li_skill == SKILL_VANPIRE_TOUCH
			|| li_skill == SKILL_HELL_TOUCH			// 벰파이어터치 | 헬 터치
			|| li_skill == SKILL_AIR_RAID
			|| li_skill == SKILL_CHAIN_ATTACK
			|| li_skill == SKILL_DEEP_ATTACK		// 에어레이드 | 체인어택 | 딥어택.
			|| li_skill == SKILL_HANDLE				// 조작술
			|| li_skill == SKILL_SUMMON_PARTITION
			|| li_skill == SKILLF_A_DSLASH
			|| li_skill == SKILLF_A_ASSAULT			// 프리크 더블 슬래쉬 | 어썰트
			|| li_skill == SKILLF_A_BASH			// 프리크 배쉬
			|| li_skill == SKILL_B_BURNINGCRASH		// nate 2005-06-10 : Burning Crash - 세컨어택 정의
			|| li_skill == PEROM_SKILL_S128
			|| li_skill == PEROM_SKILL_S129
			|| li_skill == PEROM_SKILL_S130
			|| li_skill == PEROM_SKILL_S131
			|| li_skill == PEROM_SKILL_S132
			|| li_skill == PEROM_SKILL_S133
			|| li_skill == PEROM_SKILL_S134
			|| li_skill == PEROM_SKILL_S135
			|| li_skill == PEROM_SKILL_S136
			|| li_skill == PEROM_SKILL_S137
			|| li_skill == PEROM_SKILL_S138
			|| li_skill == PEROM_SKILL_S139
			|| li_skill == PEROM_SKILL_S140
			|| li_skill == PEROM_SKILL_S141
			|| li_skill == PEROM_SKILL_S142
			|| li_skill == PEROM_SKILL_S143
			|| li_skill == PEROM_SKILL_S144
			|| li_skill == PEROM_SKILL_S145
			|| li_skill == PEROM_SKILL_S146
			|| li_skill == PEROM_SKILL_S147
			|| li_skill == PEROM_SKILL_S148
			|| li_skill == PEROM_SKILL_S149
			|| li_skill == PEROM_SKILL_S150
			|| ( li_skill == SKILL_C_HELLDOWN		// mungmae-2005/07/14 : 헬다운에 마우스 띤 상태
				 && pCMyApp->m_pUIMgr->m_bRClickDown == 2 )
			|| ( li_skill >= SKILL_H_AUTOMINE && li_skill <= SKILL_F_TORNADO_HAMMER
				 && ( li_skill != SKILL_A_SUMMONS_SQUAD
					  && li_skill != SKILL_A_SUMMON_FLOWER
					  && li_skill != SKILL_F_ADRENALINE
					)))
		return true;
	return false;
}

// nate 2005-11-23 : 함수 설명 : 즉시 이펙트 스킬인지 체크
// 주의 : 마우스 클릭시 즉시 이펙트가 발동하는 스킬만 추가
// 참고 : 이펙트가 동작의 일정 프레임에 발동하는 스킬은 dta의 공격 프레임을 이용할것
bool G_SK_IsEffectSkill(int li_skill)
{
	if(li_skill == SKILL_FIRE_BALL
			|| li_skill == SKILL_LIGHTNING_BALL
			|| li_skill == SKILL_PEBBLE_SHOT
			|| li_skill == SKILL_FIRE_PILLAR
			|| li_skill == SKILL_LIGHTNING
			|| li_skill == SKILL_FALL_STONE
			|| li_skill == SKILL_STAR_FLARE
			|| li_skill == SKILL_SHOCK_SPEAR
			|| li_skill == SKILL_STONE_LANCE
			|| li_skill == SKILL_TORNADO_RUIN
			|| li_skill == SKILL_HELL_CRY
			|| li_skill == SKILL_AIR_RAID
			|| li_skill == SKILL_SUMMON_PARTITION
			|| li_skill == SKILLF_A_BASH
			|| li_skill == SKILLF_A_DSLASH
			|| li_skill == SKILLF_A_ASSAULT
			|| li_skill == SKILL_A_LIFE_EXPANSION	// nate 2005-07-18 : Life Expansion - Effect Skill
			|| li_skill == SKILL_C_HELLDOWN
			|| li_skill == SKILL_F_SPACESLASH
			|| li_skill == SKILL_F_ELECTRICHAMMER
			|| li_skill == SKILL_K_FLAMESPEAR
			|| li_skill == SKILL_H_FROZENSHOT
			|| li_skill == SKILL_A_SUMMONFIRE
			|| li_skill == SKILL_H_RANDOMFIRE
			|| li_skill == SKILL_F_MERCERNERY_SLASH
			|| li_skill == SKILL_K_ICEBOLT
			|| li_skill == SKILL_B_WILDSMASH
			|| li_skill == SKILL_A_RESURRECTION
			|| li_skill == SKILL_F_TORNADO_HAMMER
			|| li_skill == PEROM_SKILL_S127
			|| li_skill == PEROM_SKILL_S128
			|| li_skill == PEROM_SKILL_S129
			|| li_skill == PEROM_SKILL_S130
			|| li_skill == PEROM_SKILL_S131
			|| li_skill == PEROM_SKILL_S132
			|| li_skill == PEROM_SKILL_S133
			|| li_skill == PEROM_SKILL_S134
			|| li_skill == PEROM_SKILL_S135
			|| li_skill == PEROM_SKILL_S136
			|| li_skill == PEROM_SKILL_S137
			|| li_skill == PEROM_SKILL_S138
			|| li_skill == PEROM_SKILL_S140
	  )
		return true;
	return false;
}

void ReadLine(FILE *fp, char *str)
{
	char ch;
	int read_byte;

	// 개행문자 건너뜀.
	do
	{
		read_byte = fread(&ch, 1, 1, fp);
	}
	while ((ch == '\n' || ch == '\r' || ch == '\t') && read_byte > 0);
	*(str++) = ch;

	// 개행문자까지 한줄 읽음.
	while (ch != '\n' && ch != '\r' && ch != '\t' && read_byte > 0)
	{
		read_byte = fread(&ch, 1, 1, fp);
		*(str++) = ch;
	}

	*(str-1) = '\0';
}

void PReadLine(PFILE *fp, char *str)
{
	char ch;
	int read_byte;

	// 개행문자 건너뜀.
	do
	{
		read_byte = pfread(&ch, 1, 1, fp);
	}
	while ((ch == '\n' || ch == '\r' || ch == '\t') && read_byte > 0);
	*(str++) = ch;

	// 개행문자까지 한줄 읽음.
	while (ch != '\n' && ch != '\r' && ch != '\t' && read_byte > 0)
	{
		read_byte = pfread(&ch, 1, 1, fp);
		*(str++) = ch;
	}

	*(str-1) = '\0';
}


#define is_space(c)		(c == 0x20 || (c >= 0x09 && c <= 0x0D))

void ReadSentence(FILE *fp, char *str)
{
	char ch;
	BOOL bQ = FALSE;

	fread(&ch, 1, 1, fp);
	while (is_space(ch))
	{
		fread(&ch, 1, 1, fp);
	}
	//wan:2004-11
	//해외 스킬이름중 '<=이게 많이들어가서 이제 이걸론 체크안함
	//" "로만 스킬이름 묶어줌
	if (ch == '\"') // || ch == '\'') {
	{
		bQ = TRUE;
		fread(&ch, 1, 1, fp);
	}

	do
	{
		*(str++) = ch;
		fread(&ch, 1, 1, fp);
	}
	while ((!bQ && !is_space(ch)) || bQ && ch != '\"');
	*str = '\0';
}

void PReadSentence(PFILE *fp, char *str)
{
	char ch;
	BOOL bQ = FALSE;

	pfread(&ch, 1, 1, fp);
	while (is_space(ch))
	{
		pfread(&ch, 1, 1, fp);
	}
	//wan:2004-11
	//해외 스킬이름중 '<=이게 많이들어가서 이제 이걸론 체크안함
	//" "로만 스킬이름 묶어줌
	if (ch == '\"') // || ch == '\'') {
	{
		bQ = TRUE;
		pfread(&ch, 1, 1, fp);
	}

	do
	{
		*(str++) = ch;
		pfread(&ch, 1, 1, fp);
	}
	while ((!bQ && !is_space(ch)) || bQ && ch != '\"');
	*str = '\0';
}


int GetEngyptGap( int nKind )
{
	int nGap = 0;
	switch( nKind )
	{
	case SKILL_DTA:
		nGap = 3;
		break;
	case RACE_SKILL:
		nGap = 5;
		break;
	case BPDINDEX:
		nGap = 2;
		break;
	case BPDLIST:
		nGap = 8;
		break;
	case RACECSY:
		nGap = 11;
		break;
	}
	return nGap;
}

void Dummy( int nCode, FILE *fp )
{
	int nCount = 0;
	char szTemp[64] = "";

	switch( nCode )
	{
	case SKILL_DTA:
		nCount = 26;
		break;
	case RACE_SKILL:
		nCount = 59;
		break;
	case BPDINDEX:
		nCount = 34;
		break;
	case BPDLIST:
		nCount = 43;
		break;
	case RACECSY:
		nCount = 13;
		break;
	}

	fgets( szTemp, nCount, fp );
}

void PDummy( int nCode, PFILE *fp )
{
	int nCount = 0;
	char szTemp[64] = "";

	switch( nCode )
	{
	case SKILL_DTA:
		nCount = 26;
		break;
	case RACE_SKILL:
		nCount = 59;
		break;
	case BPDINDEX:
		nCount = 34;
		break;
	case BPDLIST:
		nCount = 43;
		break;
	case RACECSY:
		nCount = 13;
		break;
	}

	pfgets( szTemp, nCount, fp );
}

void Decode( char* strTemp, int nCode, bool bTap )
{
	int nGap = GetEngyptGap( nCode );
	int j = 0;
	while( strTemp[j] != '\n' && strTemp[j] != '\0' )
	{
		if(j%nGap)
			strTemp[j] ^= 0x11;
		else
			strTemp[j] ^= nCode;

		if( !bTap && strTemp[j] == '\t' )
		{
			strTemp[j] = '\0';
			break;
		}
		//- rumist
		if( strTemp[j] == '\0' )
			break;

		j++;
	}
}

//////////////////////////////////////////////////////////////
// 수정자 : 이원석                 ///skill.dta 암호화.  // UpdateLog 172번 참고.
// 수정일 : 05-04-19
// 수정내용 : skill.dta의 암호화 처리. 현재는 대만만 시범 적용. 후에 문제가 없으면 다른나라도 적용시켜주자.
#define SKILL_DTA_FRONT_DUMMY_SIZE 235 // 앞쪽에 들어가는 더미 데이터 부의 바이트 사이즈. 숫자는 막 정한거지만 암호화 툴 소스에서도 같게 맞춰줘야한다.
#define SKILL_DTA_LAST_DUMMY_SIZE  52  // 뒤쪽에 들어가는 더미 데이터 부의 바이트 사이즈. 숫자는 막 정한거지만 암호화 툴 소스에서도 같게 맞춰줘야한다.
#define SKILL_DTA_ENCODE_BYTE 0x84     // 암호화 비트. 실제 데이터부에 ^= 연산해서 사용한다. 숫자는 막 정한거지만 암호화 툴 소스에서도 같게 맞춰줘야한다.

void SkillDtaDecode() // skill.dta 파일의 암호화를 푼다. 풀어서 skill.tmp 파일을 임시로 생성한다.
{
	PFILE *fp;
	FILE *tempFp;
	DWORD FileSize = 0; // 원본 데이터의 크기.
	BYTE *FileData = NULL; // 원본 데이터.

	BYTE *FrontDummyData = NULL; // 앞쪽에 들어갈 더미 데이터.
	BYTE *LastDummyData = NULL; // 뒤쪽에 들어갈 더미 데이터.


	fp = pfopen( "data/skill.dta", "rb" );

	if (!fp)
		return;

	pfseek( fp, 0, SEEK_END );
	FileSize = pftell( fp );
	pfseek( fp, 0, SEEK_SET );
	//FileSize = filelength( fileno(fp) ); // 파일 크기를 구한다.
	FileSize -= SKILL_DTA_FRONT_DUMMY_SIZE + SKILL_DTA_LAST_DUMMY_SIZE; // 더미를 뺀 사이즈가 원본의 데이터 사이즈.

	FileData = new BYTE[FileSize];
	FrontDummyData = new BYTE[SKILL_DTA_FRONT_DUMMY_SIZE]; // 더미도 읽어야하니 메모리를 일단 잡는다.
	LastDummyData = new BYTE[SKILL_DTA_LAST_DUMMY_SIZE];

	pfread( FrontDummyData, sizeof(BYTE), SKILL_DTA_FRONT_DUMMY_SIZE, fp ); // 더미부 리드.
	pfread( FileData, sizeof(BYTE), FileSize, fp ); // 통째로 읽어온다.
	pfread( LastDummyData, sizeof(BYTE), SKILL_DTA_LAST_DUMMY_SIZE, fp ); // 더미부 리드.

	pfclose( fp );

	for( DWORD i = 0 ; i < FileSize ; ++i ) // 암호화를 푼다.
	{
		FileData[i] ^= SKILL_DTA_ENCODE_BYTE;
	}




	// 임시 파일의 생성.(스킬 로드가 끝나면 지운다.)
	tempFp = fopen( "data/skill.tmp", "wb" ); // 저장은 바이너리로 해버린다.(읽을때만 텍스트로 읽으면 된다.)

	fwrite( FileData, sizeof(BYTE), FileSize, tempFp ); // 암호화된 데이터 부를 통째로 넣는다.

	fclose( tempFp );

}

//////////////////////////////////////////////////////////////

void LoadSkills()
{

	char* szTemp = NULL;	// 포인터 변수
	g_SkillsNum = 0;
	FILE *Tempfp = NULL;
	int i = 0, j = 0;


	{
		LHExcelReader er;
		if( !er.Open( SKILL_INFO_FILENAME ) )
		{
			MessageBox(NULL, "Unable to read skill information.", "error!", MB_OK);
			return;
		}

		g_SkillsNum = er.GetRowSize();
		if( g_SkillsNum <= 0 )
		{
			MessageBox(NULL, "Unable to read skill information.", "error!", MB_OK);
			return;
		}

		SAFE_DELETE_ARRAY(g_Skills);
		g_Skills = new CSkill[g_SkillsNum];

		char multibyte[1024];
		wchar_t unicode[1024];

		for( int row=0 ; row<er.GetRowSize() ; ++row )
		{
			int col = 0;
			g_Skills[row].m_Index = er.GetInt(row, col++);

			const char * text = er.GetData(row, col++);

			UTF8ToUnicode( unicode , text , 1024 );
			UniToMulti( multibyte , unicode , 1024 );
			g_Skills[row].m_Name = multibyte;

			g_Skills[row].m_MinLevel = er.GetInt(row, col++);
			g_Skills[row].m_Energe0Min = er.GetInt(row, col++);
			g_Skills[row].m_Energe0Max = er.GetInt(row, col++);
			//		col++;
			//		col++;
			// 		g_Skills[row].m_Energe1Min = er.GetInt(row, col++);
			// 		g_Skills[row].m_Energe1Max = er.GetInt(row, col++);
			g_Skills[row].m_WhichEnergy = er.GetInt(row, col++);
			g_Skills[row].m_App0Min = er.GetInt(row, col++);
			g_Skills[row].m_App0Max = er.GetInt(row, col++);
			g_Skills[row].m_App1Min = er.GetInt(row, col++);
			g_Skills[row].m_App1Max = er.GetInt(row, col++);
			g_Skills[row].m_bLeft = er.GetInt(row, col++);
			g_Skills[row].m_bRight = er.GetInt(row, col++);
			g_Skills[row].m_bAttack = er.GetInt(row, col++);
			g_Skills[row].m_Kind = er.GetInt(row, col++);
			g_Skills[row].m_AttCount = er.GetInt(row, col++);
			g_Skills[row].m_MaxLevel = er.GetInt(row, col++);
			g_Skills[row].m_MinLaim = er.GetInt(row, col++);
			g_Skills[row].m_MaxLaim = er.GetInt(row, col++);
			g_Skills[row].m_race = er.GetInt(row, col++);

			g_Skills[row].m_cool_time = er.GetInt(row, col++);

			text = er.GetData(row, col++);
			UTF8ToUnicode( unicode , text , 1024 );
			UniToMulti( multibyte , unicode , 1024 );
			g_Skills[row].m_Descr = multibyte;
		}

		er.Close();
	}
	{
		LHExcelReader er;
		if( !er.Open("data/character/race_skill.txt") )
		{
			MessageBox(NULL, "Unable to read skill information.", "error!", MB_OK);
			return;
		}

		for( int row=0 ; row<er.GetRowSize() ; ++row )
		{
			int col = 0;

			int skill_count = er.GetInt(row, col++);

			if( g_RaceSkill[row].m_SkillIndex )
				delete[] g_RaceSkill[row].m_SkillIndex;

			if( skill_count > 0 )
				g_RaceSkill[row].m_SkillIndex = new int[skill_count];

			for( int num=0 ; num<skill_count ; ++num )
			{
				g_RaceSkill[row].m_SkillIndex[num] = er.GetInt(row, col++);
			}

			g_RaceSkill[row].m_SkillNum = skill_count;
		}
	}

}

void DeleteSkills()
{
	SAFE_DELETE_ARRAY( g_Skills );
	g_SkillsNum = 0;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSkill::CSkill()
	: m_Index(0)
	, m_MinLevel(0)
	, m_Energe0Min(0)
	, m_Energe0Max(0)
	, m_WhichEnergy(0)
	, m_App0Min(0)
	, m_App0Max(0)
	, m_App1Min(0)
	, m_App1Max(0)
	, m_MaxLevel(0)
	, m_bLeft(FALSE)
	, m_bRight(FALSE)
	, m_bAttack(FALSE)
	, m_Kind(0)
	, m_AttCount(0)
	, m_MinLaim(0)
	, m_MaxLaim(0)
	, m_race(0)
{

	m_cool_time = 0;
}

CSkill::~CSkill()
{
}

int CSkill::GetEnerge0(int skill_level)
{
	if (skill_level < 1)
		return 0;

	if (skill_level > m_MaxLevel)
		skill_level = m_MaxLevel;

	return (m_Energe0Max - m_Energe0Min) *
		   (skill_level - 1) / ( m_MaxLevel - 1 ) + m_Energe0Min;
}

int CSkill::GetMoney( int nSkill_Level )	// nate 2005-11-08 : 포이즌 - 요구 라임 계산
{
	if( nSkill_Level < 1 )
		return 0;

	if( nSkill_Level > m_MaxLevel )
		nSkill_Level = m_MaxLevel;

	int money = SkillExtendManager::instnace()->getMoney(m_Index, nSkill_Level);
	if( money != -1 )
	{
		return money;
	}

	int nMaxLevel = m_MaxLevel;
	if( SkillExtendManager::instnace()->isExtend(m_Index) == true )
	{
		nMaxLevel -= 5;
	}

	return (m_MaxLaim - m_MinLaim) *
		   (nSkill_Level - 1) / ( nMaxLevel - 1 ) + m_MinLaim;
}

int CSkill::GetApp0(int nSkill_level, int nStep, bool automine)
{
	if (nSkill_level < 1)
		return 0;

	if (nSkill_level > m_MaxLevel)
		nSkill_level = m_MaxLevel;

	int data = SkillExtendManager::instnace()->getData(m_Index, nSkill_level);
	if( data != -1 )
	{
		return data;
	}

	int nMaxLevel = m_MaxLevel;
	if( SkillExtendManager::instnace()->isExtend(m_Index) == true )
	{
		nMaxLevel -= 5;
	}	

	int nValue = 0;
	
	if( nStep )
	{
		if( automine )
		{
			nValue = ((nSkill_level*(2.0*m_App0Min + (nSkill_level - 1)*m_App0Max)/2.0 + m_AttCount)/(float)(((nSkill_level-1)/(float)nStep) + 1));
		}
		else
		{
			nValue = ((nSkill_level*(2*m_App0Min + (nSkill_level - 1)*m_App0Max)/2 + m_AttCount)/(((nSkill_level-1)/nStep) + 1));
		}
	}
	else
	{
		nValue = (m_App0Max - m_App0Min) * (nSkill_level - 1) / ( nMaxLevel - 1 ) + m_App0Min;
	}		

	return nValue;
}

int CSkill::GetManaShieldTime(int skill_level)
{
	if (skill_level < 1)
		return 0;

	return skill_level * 1 + 5;
}

int CSkill::GetManaShieldAddTime(int skill_level)
{
	if (skill_level < 1)
		return 0;

	return skill_level * 10 + 100;
}

int	CSkill::GetProtectionShieldData(int skill_level)
{
	if( skill_level < 1 )
		return 0;

	int result = skill_level / 2;
	if( skill_level > 20 )
	{
		result = skill_level - 10;
	}
	else if( (skill_level % 2) != 0 )
	{
		result += 1;
	}

	return result;
}

int CSkill::GetApp1(int nSkill_level)
{
	if (nSkill_level < 1)
		return 0;

	if (nSkill_level > m_MaxLevel)
		nSkill_level = m_MaxLevel;

	return (m_App1Max - m_App1Min) * (nSkill_level - 1) / ( m_MaxLevel - 1 ) + m_App1Min;
}


char * CSkill::GetName()
{
	return (char*)m_Name.c_str();
}

char * CSkill::GetDesc()
{
	return (char*)m_Descr.c_str();
}

int GetMagicDistance(int skill_level)
{
	int maxLevel = 25;
	int app1Max = 170;
	int app1Min = 145;

	if (skill_level < 1)
		return 0;

	if (skill_level > maxLevel)
		skill_level = maxLevel;

	return (app1Max - app1Min) *
		   (skill_level - 1) / ( maxLevel - 1 ) + app1Min;
}

int GetHideSight(int skill_level)
{
	int maxLevel = 5;
	int app1Max = 100;
	int app1Min = 20;

	if (skill_level < 1)
		return 0;

	if (skill_level > maxLevel)
		skill_level = maxLevel;

	return (app1Max - app1Min) *
		   (skill_level - 1) / ( maxLevel - 1 ) + app1Min;
}

int GetMultiShot(int skill_level)
{
	int maxLevel = 25;
	int app1Max = 95;
	int app1Min = 4;

	if (skill_level < 1)
		return 0;

	if (skill_level > maxLevel)
		skill_level = maxLevel;

	return (app1Max - app1Min) *
		   (skill_level - 1) / ( maxLevel - 1 ) + app1Min;
}

int GetSnifingRange(int skill_level)
{
	int maxLevel = 5;
	int app1Max = 90;
	int app1Min = 20;

	if (skill_level < 1)
		return 0;

	if (skill_level > maxLevel)
		skill_level = maxLevel;

	return (app1Max - app1Min) *
		   (skill_level - 1) / ( maxLevel - 1 ) + app1Min;
}

CRaceSkill::CRaceSkill()
	: m_SkillNum(0)
{
	m_SkillIndex = NULL;
}

CRaceSkill::~CRaceSkill()
{
	SAFE_DELETE_ARRAY( m_SkillIndex );
}

CChaSkill::CChaSkill()
{
	m_SkillNum = 0;
	m_SkillIndex = NULL;
	m_SkillLevel = NULL;
	m_SkillIcon = NULL;
	m_LevelText = NULL;
	m_bLeft = NULL;
	m_bRight = NULL;

	m_rcRSkill = NULL;
	m_rcLSkill = NULL;
}

CChaSkill::~CChaSkill()
{
	SAFE_DELETE_ARRAY( m_SkillIndex );
	SAFE_DELETE_ARRAY( m_SkillLevel );
	SAFE_DELETE_ARRAY( m_SkillIcon );
	SAFE_DELETE_ARRAY( m_LevelText );
	SAFE_DELETE_ARRAY( m_bLeft );
	SAFE_DELETE_ARRAY( m_bRight );

	SAFE_DELETE_ARRAY( m_rcRSkill );
	SAFE_DELETE_ARRAY( m_rcLSkill );
}

void CChaSkill::LoadRes(int icon_w, int icon_h)
{
	for (int i = 0; i < m_SkillNum; i++)
	{
		m_SkillIcon[i].LoadRes(icon_w, icon_h);
		m_LevelText[i].Init(14, RGB(255, 255, 255), 0, 0, -1, -1, TRUE);
		m_LevelText[i].SetString(m_SkillLevel[i]);
	}
}

void CChaSkill::DeleteRes()
{
	for (int i = 0; i < m_SkillNum; i++)
	{
		m_SkillIcon[i].DeleteRes();
		m_LevelText[i].DeleteRes();
	}
}

void CChaSkill::Reset()
{
	SAFE_DELETE_ARRAY( m_SkillIndex );
	SAFE_DELETE_ARRAY( m_SkillLevel );
	SAFE_DELETE_ARRAY( m_SkillIcon );
	SAFE_DELETE_ARRAY( m_LevelText );
	SAFE_DELETE_ARRAY( m_bLeft );
	SAFE_DELETE_ARRAY( m_bRight );
	SAFE_DELETE_ARRAY( m_rcRSkill );
	SAFE_DELETE_ARRAY( m_rcLSkill );

	m_SkillNum = 0;
	m_SkillIndex = NULL;
	m_SkillLevel = NULL;
	m_SkillIcon = NULL;
	m_bLeft = NULL;
	m_bRight = NULL;
}

void CChaSkill::Init(int race)
{
	Reset();

	m_SkillNum = g_RaceSkill[race].m_SkillNum; // 현제 선택된 종족의 스킬의 개수..

	if (m_SkillNum > 0)
	{
		m_SkillIndex = new int[m_SkillNum];
		m_SkillLevel = new int[m_SkillNum];
		m_SkillIcon = new CBasicButton[m_SkillNum];
		m_LevelText = new CTextOutBox[m_SkillNum];
		m_rcRSkill = new RECT[m_SkillNum];
		m_rcLSkill = new RECT[m_SkillNum];
		m_bLeft = new BOOL[m_SkillNum];
		m_bRight = new BOOL[m_SkillNum];
		for (int i = 0; i < m_SkillNum; i++)
		{
			m_SkillIndex[i] = g_RaceSkill[race].m_SkillIndex[i];
			m_SkillLevel[i] = 0;
			m_bLeft[i] = g_Skills[m_SkillIndex[i]].m_bLeft;
			m_bRight[i] = g_Skills[m_SkillIndex[i]].m_bRight;

			char fileName[128];
			sprintf(fileName, "skill/skill_%d", m_SkillIndex[i]);
			m_SkillIcon[i].SetFileName(fileName);
		}
	}
	else
	{
		OutputDebugString("Check This later\n");
		m_SkillIndex = NULL;
		m_SkillLevel = NULL;
		m_SkillIcon = NULL;
		m_LevelText = NULL;
		m_bLeft = NULL;
		m_bRight = NULL;
	}
}

void CChaSkill::SetSkillLevel(int skill_idx, int skill_level)
{
	for (int i = 0; i < m_SkillNum; i++)
	{
		if (m_SkillIndex[i] == skill_idx)
		{
			m_SkillLevel[i] = skill_level;
			m_LevelText[i].SetString(skill_level);
			break;
		}
	}
}

BOOL CChaSkill::CanUseSkill(int skill_idx)
{
	for (int i = 0; i < m_SkillNum; i++)
	{
		if (m_SkillIndex[i] == skill_idx)
		{
			if (m_SkillLevel[i] >= 1)
				return TRUE;

			return FALSE;
		}
	}

	return FALSE;
}

int CChaSkill::GetLevelByIndex(int skill_idx)
{
	for (int i = 0; i < m_SkillNum; i++)
	{
		if (m_SkillIndex[i] == skill_idx)
			return m_SkillLevel[i];
	}

	return 0;
}

int CChaSkill::GetArrayIndex( int skill_idx ) // 스킬 인덱스로 부터 배열 인덱스를 얻어오는 함수 ///하이콤보 때 추가.
{
	for (int i = 0; i < m_SkillNum; i++)
	{
		if (m_SkillIndex[i] == skill_idx)
			return i;
	}

	return -1;
}

void CChaSkill::UpdateRskill(int index, int x, int y)
{
	if( !m_SkillIcon || index < 0 || index >= m_SkillNum )
		return;

	SetRect( &m_rcRSkill[index] , x, y , x+m_SkillIcon[index].Width, y+m_SkillIcon[index].Height );
}

void CChaSkill::UpdateLskill(int index, int x, int y)
{
	if( !m_SkillIcon || index < 0 || index >= m_SkillNum )
		return;

	SetRect( &m_rcLSkill[index] , x, y , x+m_SkillIcon[index].Width, y+m_SkillIcon[index].Height );
}

bool CChaSkill::IsInRskill(int index, int x, int y)
{
	if( !m_rcRSkill || index < 0 || index >= m_SkillNum )
		return false;

	POINT pt;
	pt.x = x;
	pt.y = y;
	return PtInRect( &m_rcRSkill[index] , pt )?true:false;
}

bool CChaSkill::IsInLskill(int index, int x, int y)
{
	if( !m_rcLSkill || index < 0 || index >= m_SkillNum )
		return false;

	POINT pt;
	pt.x = x;
	pt.y = y;
	return PtInRect( &m_rcLSkill[index] , pt )?true:false;
}


int CChaSkill::GetCurrentSkill(int cur_skill)
{
	if( m_SkillNum && m_SkillIndex )
		return m_SkillIndex[cur_skill];

	return -1;
}