#ifndef _GUILD_DATA_H_
#define _GUILD_DATA_H_

#include "Config.h"
#include "headers.h"


#pragma warning(disable : 4786)
#pragma warning(disable : 4183)

#include <list>
#include <algorithm>

using namespace std;

#include "MsgPopUp.h"
#include "headers.h"

// 1 일반길원
// 3 길드 부장[하이브리더]
// 4 길드 부장[휴먼]
// 5 길드 부장[에이디아]
// 6 길드 부장[카이립톤]
// 7 길드 부장[불칸]
// 9 길드 부마스터
// 10 길드 마스터

enum JOB_NAME
{
	JOB_EMPTY_I = 0,
	JOB_GENERIC,
	JOB_PEROM,
	JOB_HIV,
	JOB_HUMAN,
	JOB_EIDA,
	JOB_KAI,
	JOB_VULCAN,
	JOB_EMPTY_III,
	JOB_ASSISTANTMASTER,
	JOB_MASTER,

	JOB_NUM
};

enum DATA_CHECK
{
	CHECK_UPDATE,
	CHECK_ADD,
	CHECK_RECOMMAND_WAIT_ADD,
	CHECK_GENERAL_WAIT_ADD,
	CHECK_DELETE,
	CHECK_OK,
	CHECK_NONE,
	CHECK_TOTAL,
};

enum INVASION_CHECK
{
	INVASION_DEFENSE,
	INVASION_ATTACK,
	INVASION_NONE,
	INVASION_FLAG_TOTAL
};

// 길드 정보
typedef struct _stInfo1
{
	char	s_guildName[256];	// 길드명
	int		s_rank;				// 길드랭킹
	int		s_glevel;			// 길드레벨( 길드원 레벨과 혼동 하지 마삼 )
	INT		s_gPoint;			// 길드 점수
	int		s_guildIndex;		// 길드 인덱스
	t_Info	s_revisiontime;	// 등급 하락시 보정 시간(초)

	char s_guildjobname[JOB_NUM][256];

	_stInfo1()
	{
		ZeroMemory(s_guildName, sizeof(s_guildName));

		s_rank	 = 0;
		s_glevel = 0;
		s_gPoint = 0;
		s_guildIndex = 0;
		s_revisiontime = t_Info(-1, -1, -1, -1, -1, -1);

		for(int i = 0; i < JOB_NUM; i++)
			ZeroMemory(s_guildjobname[i], sizeof(s_guildjobname[i]));

	}

	_stInfo1& operator=(const _stInfo1& info)
	{
		strcpy(s_guildName, info.s_guildName);

		s_rank			= info.s_rank;
		s_glevel		= info.s_glevel;
		s_gPoint		= info.s_gPoint;
		s_guildIndex	= info.s_guildIndex;
		s_revisiontime	= info.s_revisiontime;

		for(int i = 0; i < JOB_NUM; i++)
			strcpy(s_guildjobname[i], info.s_guildjobname[i]);

		return (*this);
	}
public:
	void reset()
	{
		ZeroMemory(s_guildName, sizeof(s_guildName));

		s_rank	 = 0;
		s_glevel = 0;
		s_gPoint = 0;
		s_guildIndex = 0;
		s_revisiontime = t_Info(-1, -1, -1, -1, -1, -1);

		for(int i = 0; i < JOB_NUM; i++)
			ZeroMemory(s_guildjobname[i], sizeof(s_guildjobname[i]));
	}

} guild_Info;

// 길드 멤버
typedef struct _stInfo2
{
	int	s_index;	// 캐릭 인덱스
	int s_glevel;	// 캐릭 직책 레벨

	char s_name[256];		// 캐릭 이름

	int		s_race;		// 종족
	int		s_connect;  // 접속 상태
	int		s_jtype;	// 가입 형태
	t_Info	s_waittime ; // 가입대기중 시간(초)


	DATA_CHECK	s_check;

	_stInfo2()
	{
		s_index    = -1;
		s_glevel   = -1;
		s_race     = -1;
		s_connect  = -1;
		s_jtype    = -1;
		s_waittime = t_Info(-1, -1, -1, -1, -1, -1);

		s_check	   = CHECK_NONE;
		ZeroMemory(s_name, sizeof(s_name));
	}

	~_stInfo2()
	{
		reset();
	}

	_stInfo2& operator=(const _stInfo2& member)
	{
		s_index  = member.s_index;
		s_glevel = member.s_glevel;

		strcpy(s_name, member.s_name);
		s_race		= member.s_race;
		s_connect	= member.s_connect;
		s_jtype		= member.s_jtype;
		s_check     = member.s_check;
		s_waittime	= member.s_waittime;

		return (*this);
	}

public:
	void reset()
	{
		s_index    = -1;
		s_glevel   = -1;
		s_race     = -1;
		s_connect  = -1;
		s_jtype    = -1;
		s_waittime = t_Info(-1, -1, -1, -1, -1, -1);
		s_check	   = CHECK_NONE;
		ZeroMemory(s_name, sizeof(s_name));
	}

} guild_member;

// 길드 공지
typedef struct _stInfo3
{
	char s_guildnotic[512];

	_stInfo3()
	{
		ZeroMemory(s_guildnotic, sizeof(s_guildnotic));
	}

public:
	void reset()
	{
		ZeroMemory(s_guildnotic, sizeof(s_guildnotic));
	}
} guild_alert;

// 길드 침략 정보
typedef struct _stinfo4
{
	INVASION_CHECK	s_Invasion_flag;
	int				s_guild_Index;
	char			s_guild_Name[128];
	t_Info			s_time;

	_stinfo4()
	{
		s_Invasion_flag = INVASION_NONE;
		s_guild_Index	= -1;
		ZeroMemory(s_guild_Name, sizeof(s_guild_Name));
		s_time = t_Info(-1, -1, -1, -1, -1, -1);
	}

	_stinfo4& operator=(const _stinfo4& info)
	{
		strcpy(s_guild_Name, info.s_guild_Name);

		s_guild_Index	= info.s_guild_Index;
		s_Invasion_flag = info.s_Invasion_flag;
		s_time			= info.s_time;
	}
public:
	void Reset()
	{
		s_Invasion_flag = INVASION_NONE;
		s_guild_Index	= -1;
		ZeroMemory(s_guild_Name, sizeof(s_guild_Name));
		s_time = t_Info(-1, -1, -1, -1, -1, -1);
	}

} guild_Invasion;

typedef list<guild_member>	list_guildmember;
typedef list<guild_member*> list_guildmember_ptr;

typedef list<guild_Invasion > list_guildInvasion;
typedef list<guild_Invasion*> list_guildInvasion_ptr;


// 길드 정보 + 길드 멤버 + 길드 공지
typedef struct _stTotalInfo
{
	int					s_totalmember;
	int					s_genericmember;
	int					s_recomwaitmember;
	int					s_generalwaitmember;
	int					s_deletemember;
	int					s_InvasionNum;

	guild_Info			s_guildInfo;	 // 길드 정보
	guild_alert			s_guildnotice;	 // 길드 공지
	guild_member		s_guildMyinfo;	 // 나의 정보

	list_guildmember	s_guildRecom;	 // 추천가입요청..아직 정식 멤버는 아님
	list_guildmember	s_guildGeneral;  // 일반가입요청..아직 정식 멤버는 아님
	list_guildmember	s_guildmember;   // 가입되어있는 멤버리스트

	list_guildInvasion	s_guildInvasion; // 길드 침략 정보

	_stTotalInfo()
		: s_deletemember(0)
		, s_totalmember(0)
		, s_genericmember(0)
		, s_recomwaitmember(0)
		, s_generalwaitmember(0)
		, s_InvasionNum(0)
	{
		s_guildmember.clear();
		s_guildRecom.clear();
		s_guildGeneral.clear();
		s_guildInvasion.clear();
	}
public:
	void reset()
	{
		s_totalmember		= 0;
		s_genericmember		= 0;
		s_recomwaitmember	= 0;
		s_generalwaitmember = 0;
		s_InvasionNum		= 0;

		s_guildInfo.reset();
		s_guildnotice.reset();
		s_guildMyinfo.reset();
		s_guildmember.clear();
		s_guildRecom.clear();
		s_guildGeneral.clear();
		s_guildInvasion.clear();

	}

} guild_total_Info, *guild_total_pInfo;


class CNkCharacter;
class CControlGuild;

class CGuildData
{
public:
	enum
	{
		GUILD_MEMBER_DEL = -1,
		GUILD_MEMBER_WAITLIST = 0,
	};
public:
	static CGuildData* Create();

	~CGuildData();

	/**************************************************************************/
	void Init();
	/**************************************************************************/
	void	guilds_SendMsg();
	/**************************************************************************/
	// 길드정보
	void	Insert_guildInfo(guild_Info& info);
	void	Insert_guilddata(guild_member& member);
	void	Insert_guildMember(guild_member& member);
	void	Insert_guildRecomWait(guild_member& member);
	void	Insert_guildGeneralWait(guild_member& member);
	void	Insert_guildMyInfo(guild_member& member);
	void	Insert_guildnotice(guild_alert& alert);
	void	Insert_guildupdate(guild_member& member);
	void	Insert_guildInvasion(guild_Invasion& invasion);

	void	Change_guildnotice(char* szNotice);					// 길드 공지 변경
	void	Change_guildgradename(char *pszname, int nLevel);	// 길드 계급명 변경
	void	Change_guildLevel(int nLevel);						// 길드 등급 변경

	void	Reset_guildInfo();
	void	Reset_guildRecomWait();
	void	Reset_guildGeneralWait();
	void	Reset_guildMember();
	void	Reset_guildNotice();
	void	Reset_guildMyInfo();
	void	Reset_guildInvasion();
	void	Reset_guildDataAll();
	/**************************************************************************/
	/**************************************************************************/
	guild_total_Info& GetGuildinfo()
	{
		return m_guild_TT_Info;
	}

	guild_member&	  GetMyInfo()
	{
		return m_guild_TT_Info.s_guildMyinfo;
	}

	guild_Info&		  GetMyGuildInfo()
	{
		return m_guild_TT_Info.s_guildInfo;
	}
	guild_alert&	  GetMyNoticeInfo()
	{
		return m_guild_TT_Info.s_guildnotice;
	}

	list_guildmember&   GetMemberInfo()
	{
		return m_guild_TT_Info.s_guildmember;
	}
	list_guildmember&   GetRecomWaitInfo()
	{
		return m_guild_TT_Info.s_guildRecom;
	}
	list_guildmember&   GetGeneralWaitInfo()
	{
		return m_guild_TT_Info.s_guildGeneral;
	}
	list_guildInvasion& GetInvasionInfo()
	{
		return m_guild_TT_Info.s_guildInvasion;
	}

	list_guildmember_ptr& GetGeneralMember()
	{
		return m_GeneralMember;
	}
	list_guildmember_ptr& GetCaptainMember()
	{
		return m_CaptainMember;
	}

	char*			GetMemberName( int nNum );
	char*			GetInvasionAttackName();
	t_Info			GetInvasionRemainTime();

	/**************************************************************************/
	/**************************************************************************/

	/**************************************************************************/
	/**************************************************************************/
	guild_member*	  SearchMember_charIdx(int nCharIdx);
	/**************************************************************************/
	/**************************************************************************/
	void	Delete_Member(guild_member& member);
	void	Delete_RecommandWait(guild_member& member);
	void	Delete_GeneralWait(guild_member& member);
	void	Delete_Invasion(guild_Invasion& Invasion);

	/**************************************************************************/
	/**************************************************************************/
	void	Refresh_list(guild_member& member);
	/**************************************************************************/
	/**************************************************************************/
	BOOL	IsWaitlist(guild_member& g_member, int nType);
	BOOL	IsInvasion_Attack();
	BOOL	IsInvasion_Defense();
	BOOL	CheckInvasionGuild(int guildIndex );
	int		GetGLevel(int charindex);

	int		GetGuildMasterChaIndex();
	void	Member_list_check();

	INVASION_CHECK CheckInvasionGNameType(char* guildname);	 // 길드명으로 검색

private:
	CNkCharacter*	   m_pNkChar;

	guild_total_Info   m_guild_TT_Info;

	list_guildmember_ptr   m_GeneralMember;	// 일반 멤버 목록을 가지고 있다.
	list_guildmember_ptr   m_CaptainMember; // 길드마스터, 서브, 멤버들을 가지고 있다.

	char			   m_MsgBuf[1024];

	CGuildData();

	friend class CControlGuild;
};
#endif //_GUILD_DATA_