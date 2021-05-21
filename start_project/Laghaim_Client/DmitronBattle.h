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
	static int SearchPortal(int x, int y); // ��Ż
	static int SearchGateStone(int x, int y); // �̵� ����Ʈ, ũ����Ż, ����Ʈ ����
	static int SearchPortalManager(int x, int y); // ��Ż �Ŵ���
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

//���Ʈ�� ���� ����
class CDmitronInfo
{
public:
	struct Zone
	{
		char	Guild[256]; // ���Ʈ�� ��� �̸�
		int		Status;		// ���Ʈ�� �� ����
		int		Stage;		// UI ����
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

	//���� ���� �޽���
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

// ���Ʈ�� �ֵ��� â
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
	CTextOutBox			m_arGuildHuntIndex[ DMITRON_BATTLE_ZONE ]; // ��� ��� ���� ����
	CTextOutBox			m_arGuildName[ DMITRON_BATTLE_ZONE ]; // ��� �̸�
	CTextOutBox			m_arStatus[ DMITRON_BATTLE_ZONE ]; // ���� ����

	CBasicButton		m_arRequestBtn[ DMITRON_BATTLE_ZONE ]; // ��û ��ư
	CBasicButton		m_arInformBtn[ DMITRON_BATTLE_ZONE ]; // ���� ��ư
	CBasicButton		m_arJoinBtn[ DMITRON_BATTLE_ZONE ];	// ���� ��ư

	CBasicButton		m_ConfirmationBtn;		// Ȯ�� ��ư
	CBasicButton		m_RenewalBtn;			// ���� ��ư

	CBasicButton		m_btn_close;

};

// ���Ʈ�� ����â
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
	CTextOutBox			m_RequestDate;		// ��û ��¥
	CTextOutBox			m_RequestTime;		// ��û �ð�
	CTextOutBox			m_RequestGuild;		// ��û ���
	CTextOutBox			m_RequestClient;	// ��û��

	//���� ������ ��ư
	CBasicButton		m_InvadeBtn;		// ħ�� ��ư
	CBasicButton		m_CancelBtn;		// ��� ��ư

};

#endif // _GUILD_SIEGE_H_
