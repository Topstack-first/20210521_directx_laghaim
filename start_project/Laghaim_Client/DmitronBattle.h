#ifndef _DMITRON_BATTLE_H_
#define _DMITRON_BATTLE_H_

#include "Config.h"

#include "basewindow.h"
#include "basicbutton.h"

class CTcpIpConnection;
class CSurface;

#define DMITRON_BATTLE_BUFF 4
#define DMITRON_BATTLE_ZONE 5
#define DMITRON_BATTLE_PORTAL 7
#define DMITRON_BATTLE_STATUS 3

enum
{
	DMITRON_BATTLE_READY,
	DMITRON_BATTLE_WAITING,
	DMITRON_BATTLE_RUNNING,
	DMITRON_BATTLE_CANCELING,
	DMITRON_BATTLE_OWNING,
};

class CDmitronZone
{
public:
	static int SearchPortal(int x, int y); // 포탈
	static int SearchGateStone(int x, int y); // 이동 게이트, 크리스탈, 게이트 스톤
	static int SearchPortalManager(int x, int y); // 포탈 매니저
};

class CDmitronString
{
public:
	static char* Crystal(int type);
	static char* Status(int type);
	static char* Effect(int type);
	static char* Portal(int type);
	static char* Battle(int type);
	static char* Zone(int type);
};

//드미트론 전역 정보
class CDmitronInfo
{
public:
	struct Zone
	{
		char	Guild[256]; // 드미트론 길드 이름
		int		Status;		// 드미트론 존 상태
		int		Stage;		// UI 헬퍼
		int		Buff1;
		int		Buff2;
		char*	GetBuff1();
		char*	GetBuff2();
	};

	struct Info
	{
		char	Date[256];
		char	Time[256];
		char	Guild[256];
		char	Client[256];
	};

	struct Lock
	{
		int		Index;
		BOOL	Flag;
	};

	static	Info	g_Info;
	static	Lock	g_Lock;
	static	BOOL	g_Buff[DMITRON_BATTLE_BUFF];
	static	Zone	g_Zone[DMITRON_BATTLE_ZONE];

	CDmitronInfo();
	~CDmitronInfo();

	static	void	SetZoneInfo1(int nIndex, char* guildName, int status, int buff1, int buff2);
	static	void	SetZoneInfo2(int nIndex, char* guildName, int status, int stage);

	static  char*	GetCrystalName(int nIndex);
	static	char*	GetBattleStatus(int nIndex);

	static	inline	CDmitronInfo::Info &GetInfo();
	static	inline	CDmitronInfo::Lock &GetLock();
	static  inline	CDmitronInfo::Zone &GetZone(int nIndex);

	static int  Selection();
	static BOOL IsSelected();
	static void DeselectZone();
	static void SelectZone(int n);
	static void UpdateZone(BOOL succ);

	//버프 관련 메시지
	static BOOL GetBuff(int n);
	static void SetBuff(int nIndex, BOOL bState);
};

inline CDmitronInfo::Info &CDmitronInfo::GetInfo()
{
	return g_Info;
}

inline CDmitronInfo::Lock &CDmitronInfo::GetLock()
{
	return g_Lock;
}

inline CDmitronInfo::Zone &CDmitronInfo::GetZone(int nIndex)
{
	return g_Zone[nIndex];
}

// 드미트론 주둔지 창
class CDmitronBattle : public CUIBaseWindow
{
public:
	CDmitronBattle();
	virtual ~CDmitronBattle();

	/*******************************************************************/
	int	 Draw();
	void DeleteRes();
	void RestoreSurfaces();

	BOOL IsInside(int x, int y);
	BOOL IsMouseUpperWindow(int x, int y)
	{
		return TRUE;
	}

	BOOL	InitMode();
	void	LoadRes();

	BOOL	MoveWindow(UINT msg, int x, int y);
	LRESULT MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	/*******************************************************************/
	void InitTextOutBox();
	void UpdateLayout();
	void UpdateState();
	void HandleError(int error_code);

	/*******************************************************************/
	CTextOutBox			m_arGuildHuntIndex[ DMITRON_BATTLE_ZONE ]; // 길드 사냥 예약 순번
	CTextOutBox			m_arGuildName[ DMITRON_BATTLE_ZONE ]; // 길드 이름
	CTextOutBox			m_arStatus[ DMITRON_BATTLE_ZONE ]; // 예약 상태

	CBasicButton		m_arRequestBtn[ DMITRON_BATTLE_ZONE ]; // 요청 버튼
	CBasicButton		m_arInformBtn[ DMITRON_BATTLE_ZONE ]; // 정보 버튼
	CBasicButton		m_arJoinBtn[ DMITRON_BATTLE_ZONE ];	// 참가 버튼

	CBasicButton		m_ConfirmationBtn;		// 확인 버튼
	CBasicButton		m_RenewalBtn;			// 갱신 버튼

	CBasicButton		m_btn_close;

};

// 드미트론 정보창
class CDmitronBattleInfo : public CUIBaseWindow
{
public:
	CDmitronBattleInfo();
	~CDmitronBattleInfo();

	/*******************************************************************/
	int	 Draw();
	void DeleteRes();
	void RestoreSurfaces();

	BOOL IsInside(int x, int y);
	BOOL IsMouseUpperWindow(int x, int y)
	{
		return TRUE;
	}

	BOOL	InitMode();
	void	LoadRes();

	BOOL	MoveWindow(UINT msg, int x, int y);
	LRESULT MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	/*******************************************************************/
	void ActivateInfoWindow(int nTime, char* pStrGuild, char* pStrClient);

	void InitTextOutBox();
	void UpdateLayout();

	/*******************************************************************/
	CTextOutBox			m_RequestDate;		// 신청 날짜
	CTextOutBox			m_RequestTime;		// 신청 시간
	CTextOutBox			m_RequestGuild;		// 신청 길드
	CTextOutBox			m_RequestClient;	// 신청자

	//서브 윈도우 버튼
	CBasicButton		m_InvadeBtn;		// 침략 버튼
	CBasicButton		m_CancelBtn;		// 취소 버튼

};

#endif // _GUILD_SIEGE_H_
