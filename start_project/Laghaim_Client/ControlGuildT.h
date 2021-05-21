#ifndef __CONTROLGUILDT_H_
#define __CONTROLGUILDT_H_

#include "Config.h"

#pragma warning(disable : 4786)
#pragma warning(disable : 4183)

#include <list>
#include <algorithm>

#include "Nk2dframe.h"
#include "BasicButton.h"
#include "Scroll.h"
#include "guild_data.h"
#include "TextOutBox.h"


class CSurface;
class CTextBoard;

class CControlGuild
{
public:
	enum
	{
		BACK_COMMON = 0,
		BACK_MASTER,
		BACK_ASSIST_MASTER,
		BACK_BULKAN,
		BACK_HUMAM,
		BACK_AIDA,
		BACK_KAI,
		BACK_HYBRID,
		BACK_PEROM,
		BACK_MEMBER,
		BACK_MEMBER_BK,
		BACK_NUM
	};

	enum
	{
		BT_MANAGE = 0,
		BT_RECOMMENT,
		BT_CLOSE,
		BT_NOTICE,
		BT_RECOMCANCEL,
		BT_NUM
	};

	enum
	{
		CW_CHAR_NAME = 0,
		CW_RACE_NAME,
		CW_ZONE_NAME
	};

	enum
	{
		TEXT_CHAR,
		TEXT_RACE,
		TEXT_ZONE,
		TEXT_TOTAL
	};

	enum
	{
		MASTER,
		ASSISTMASTER,
		VULKAN,
		HUMAN,
		AIDA,
		KAI,
		HYBRID,
		PEROM,
		GRADE_TOTAL
	};

public:
	class CBunch
	{
	public:
		static CBunch* Create(int nBaseX = 0, int nBaseY = 0);
		~CBunch();
	public:
		void Init();
		void LoadRes();

		void Draw(int nScrollPos);
		BOOL DrawToBlt(CSurface* pSurface, int nScrollPos);

		void DeleteRes();
		BOOL IsInside(int, int);

		void SetPosition(int nPosX, int nPosY, int nNum);
		void SetRePosition(int nNum, int nPosY);
		void SetCharIdx(int nIdx)
		{
			m_nCharidx = nIdx;
		}
		void Restore();

		void SetCharName(char* pName);
		void SetRaceName(char* pName);
		void SetZoneName(char* pName);
		void SendMsg();

		LRESULT MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, int nScroll);
	public:
		CTextOutBox m_CharInfoText[TEXT_TOTAL];

		int		m_nBackPosX;		// 공통 UI의 기본 좌표 X
		int		m_nBackPosY;		// 공통 UI의 기본 좌표 Y

		int		m_nSequence;		// 나의 순서

		int		m_nCharidx;

		HWND	m_hWnd;
		RECT    m_ClientRc;

		DATA_CHECK	s_check;

		char	m_MsgBuf[1024];

	private:
		CBunch(int nBaseX = 0, int nBaseY = 0);
	};

	CBunch* m_pGuildManager[GRADE_TOTAL];	// 길드 관리자

	vector<CBunch*> m_vMemberIndexlist;

public:
	HWND		m_hWnd;
	RECT		m_ClientRc;
	CSurface*	m_pBack[BACK_NUM];
	CSurface*	m_pGuildInfo;

	CTextOutBox m_GName;
	CTextOutBox m_GLevel;
	CTextOutBox m_GRank;
	CTextOutBox m_GPoint;

	CScroll	m_Scroll;

	int		m_nMoveInterval;
	int		m_nMouseClickPos;

	CBasicButton m_Bts[BT_NUM];

	int		m_nMemberLine;	// 일반길드원

	char	m_MsgBuf[1024];

	int		m_nBackPosX;
	int		m_nBackPosY;

	BOOL	m_bRevisionDraw;
	BOOL	m_bRecom;		// 18세 이상 서버에서 가입 상태인지를 체크.

	CTextOutBox	m_revision_text;
public:
	CControlGuild();
	~CControlGuild();

	void Init();
	void LoadRes();
	void Draw();
	void SendMsg();

	void Refresh_list(guild_member& g_member);

	void Insert_list(guild_member& g_member);

//	void Insert_GuildName( guild_member& g_memmber);
//	void Insert_GuildName( guild_member& g_memmber);
//	void Insert_GuildName( guild_member& g_memmber);
//	void Insert_GuildName( guild_member& g_memmber);

	void Insert_master(guild_member& g_member);
	void Insert_assistmaster(guild_member& g_member);
	void Insert_vulkan(guild_member& g_member);
	void Insert_human(guild_member& g_member);
	void Insert_aida(guild_member& g_member);
	void Insert_kai(guild_member& g_member);
	void Insert_hybrid(guild_member& g_member);
	void Insert_perom(guild_member& g_member);
	void Insert_member(guild_member& g_member);

	int  Delete_list(guild_member& g_member);
	BOOL Delete_captain(CBunch* g_member);
	BOOL Delete_member(CBunch* g_member);


	BOOL Update_list(guild_member& g_member);
	void UpdateData(guild_member& g_member);

	void SetGuildInfo();
	void ChangeGuildInfo(char* guild);
	void DrawGuildInfo();
	BOOL DrawToBlt(CSurface* pSurface);
	BOOL DrawToBltBlank(CSurface* pSurface);

	void CheckData();

	void DeleteRes();
	BOOL IsInside(int, int);

	void SetPosition();
	BOOL SetReposition(int nWho);

	BOOL IsMemberOrCaptain(guild_member& g_member);

	void Set_btn_status(BOOL bEnable, int nLevel);
	void Restore();
	void all_reset();

	void set_revision(t_Info time_re);
	BOOL IsRevision()
	{
		return m_bRevisionDraw;
	}

	LRESULT MsgPopUpProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, int nMsg, int nProcess, char* szString);
	LRESULT MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	//////////////////////////////////////////////////////////////////////////

	void	SetRecomInfo( BOOL bRecom, char* GuildName = NULL);


};
#endif // __CONTROLGUILDT_H_