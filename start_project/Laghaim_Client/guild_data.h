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

// 1 �Ϲݱ��
// 3 ��� ����[���̺긮��]
// 4 ��� ����[�޸�]
// 5 ��� ����[���̵��]
// 6 ��� ����[ī�̸���]
// 7 ��� ����[��ĭ]
// 9 ��� �θ�����
// 10 ��� ������

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

// ��� ����
typedef struct _stInfo1
{
	char	s_guildName[256];	// ����
	int		s_rank;				// ��巩ŷ
	int		s_glevel;			// ��巹��( ���� ������ ȥ�� ���� ���� )
	INT		s_gPoint;			// ��� ����
	int		s_guildIndex;		// ��� �ε���
	t_Info	s_revisiontime;	// ��� �϶��� ���� �ð�(��)

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

// ��� ���
typedef struct _stInfo2
{
	int	s_index;	// ĳ�� �ε���
	int s_glevel;	// ĳ�� ��å ����

	char s_name[256];		// ĳ�� �̸�

	int		s_race;		// ����
	int		s_connect;  // ���� ����
	int		s_jtype;	// ���� ����
	t_Info	s_waittime ; // ���Դ���� �ð�(��)


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

// ��� ����
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

// ��� ħ�� ����
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


// ��� ���� + ��� ��� + ��� ����
typedef struct _stTotalInfo
{
	int					s_totalmember;
	int					s_genericmember;
	int					s_recomwaitmember;
	int					s_generalwaitmember;
	int					s_deletemember;
	int					s_InvasionNum;

	guild_Info			s_guildInfo;	 // ��� ����
	guild_alert			s_guildnotice;	 // ��� ����
	guild_member		s_guildMyinfo;	 // ���� ����

	list_guildmember	s_guildRecom;	 // ��õ���Կ�û..���� ���� ����� �ƴ�
	list_guildmember	s_guildGeneral;  // �Ϲݰ��Կ�û..���� ���� ����� �ƴ�
	list_guildmember	s_guildmember;   // ���ԵǾ��ִ� �������Ʈ

	list_guildInvasion	s_guildInvasion; // ��� ħ�� ����

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
	// �������
	void	Insert_guildInfo(guild_Info& info);
	void	Insert_guilddata(guild_member& member);
	void	Insert_guildMember(guild_member& member);
	void	Insert_guildRecomWait(guild_member& member);
	void	Insert_guildGeneralWait(guild_member& member);
	void	Insert_guildMyInfo(guild_member& member);
	void	Insert_guildnotice(guild_alert& alert);
	void	Insert_guildupdate(guild_member& member);
	void	Insert_guildInvasion(guild_Invasion& invasion);

	void	Change_guildnotice(char* szNotice);					// ��� ���� ����
	void	Change_guildgradename(char *pszname, int nLevel);	// ��� ��޸� ����
	void	Change_guildLevel(int nLevel);						// ��� ��� ����

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

	INVASION_CHECK CheckInvasionGNameType(char* guildname);	 // �������� �˻�

private:
	CNkCharacter*	   m_pNkChar;

	guild_total_Info   m_guild_TT_Info;

	list_guildmember_ptr   m_GeneralMember;	// �Ϲ� ��� ����� ������ �ִ�.
	list_guildmember_ptr   m_CaptainMember; // ��帶����, ����, ������� ������ �ִ�.

	char			   m_MsgBuf[1024];

	CGuildData();

	friend class CControlGuild;
};
#endif //_GUILD_DATA_