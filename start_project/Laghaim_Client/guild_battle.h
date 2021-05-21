#ifndef _GUILD_BATTLE_H_
#define _GUILD_BATTLE_H_

#include "Config.h"

#pragma warning(disable : 4786)
#pragma warning(disable : 4183)

#include <list>
#include <algorithm>

#include "basewindow.h"
#include "basicbutton.h"
#include "Scroll.h"
#include "TextOutBox.h"
#include "guild_data.h"

// 전투 선택창
class CBattleSelect	: public CUIBaseWindow
{
public:
	enum
	{
		RANK_BATTLE_1VS1 = 0,
		RANK_BATTLE_FLAG,
		RANK_BATTLE_OBSTACLE
	};
public:
	CBattleSelect();
	virtual ~CBattleSelect();

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
	void	SetPosition();

	LRESULT MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	/*******************************************************************/

	void	SendMsg();

private:

	CBasicButton	m_btRankBattle;
	CBasicButton	m_btInvasion;
	CBasicButton	m_btCancel;

	CSurface*		m_pTextSur;

	char			m_MsgBuf[1024];

};

class CBattleInvasion	: public CUIBaseWindow
{
public:
	CBattleInvasion();
	virtual ~CBattleInvasion();

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
	void	SetPosition();

	LRESULT MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	/*******************************************************************/

private:

	CBasicButton	m_btInvasionRequest;
	CBasicButton	m_btRequestInfo;
	CBasicButton	m_btCancel;

	CSurface*		m_pTextSur;

};


// 랭크전투중 선택창 : 1. 1:1전, 2. 깃발 대항전, 3. 장애물 경기전
class CBattleRankSelect : public CUIBaseWindow
{
public:
	enum
	{
		RANK_BATTLE_1VS1 = 0,
		RANK_BATTLE_FLAG,
		RANK_BATTLE_OBSTACLE
	};
public:
	CBattleRankSelect();
	virtual ~CBattleRankSelect();

	/*******************************************************************/
	int	 Draw();
	void DeleteRes();
	void RestoreSurfaces();

	BOOL    IsInside(int x, int y);
	BOOL	IsMouseUpperWindow(int x, int y)
	{
		return TRUE;
	}

	BOOL	InitMode();
	void	LoadRes();

	BOOL	MoveWindow(UINT msg, int x, int y);
	void	SetPosition();

	LRESULT MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	/*******************************************************************/

	void	SendMsg();

private:
	CBasicButton	m_bt1vs1;
	CBasicButton	m_btFlag;
	CBasicButton	m_btobstacl;
	CBasicButton	m_btCancel;

	char			m_MsgBuf[1024];

	CSurface*		m_pTextSur;
};

// 랭크전투중 선택창 : 1. 1:1전, 2. 깃발 대항전, 3. 장애물 경기전
class CBattleRankMenu : public CUIBaseWindow
{
public:
	CBattleRankMenu();
	virtual ~CBattleRankMenu();

	/*******************************************************************/
	int	 Draw();
	void DeleteRes();
	void RestoreSurfaces();

	BOOL    IsInside(int x, int y);
	BOOL	IsMouseUpperWindow(int x, int y)
	{
		return TRUE;
	}

	BOOL	InitMode();
	void	LoadRes();

	BOOL	MoveWindow(UINT msg, int x, int y);
	void	SetPosition();
	void	SetGameType(int type);

	LRESULT MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	/*******************************************************************/

	void	SendMsg();

private:
	CBasicButton	m_btRequest;
	CBasicButton	m_btWatch;
	CBasicButton	m_btTakeItem;
	CBasicButton	m_btCancel;

	char			m_MsgBuf[1024];

	CSurface*		m_pTextSur;
};


// 참여길드를 보여준다.
class CBattleRank : public CUIBaseWindow
{
public:
	enum
	{
		BT_5_10 = 0,
		BT_11_20,
		BT_21_30,
		BT_31_40,
		BT_UNLIMITED,
		BT_TOTAL
	};
public:
	CBattleRank();
	virtual ~CBattleRank();

	/*******************************************************************/
	int	 Draw();
	void DeleteRes();
	void RestoreSurfaces();

	BOOL    IsInside(int x, int y);
	BOOL	IsMouseUpperWindow(int x, int y)
	{
		return TRUE;
	}

	BOOL	InitMode();
	void	LoadRes();

	BOOL	MoveWindow(UINT msg, int x, int y);
	void	SetPosition();

	LRESULT MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	/*******************************************************************/


	void	SetSlotNum(int nSlotNum);
	void	SetText(int nNum, char* szNum);
	void	SendMsg();

private:

	CBasicButton	m_btJoin[BT_TOTAL];
	CBasicButton	m_btJoinCancel[BT_TOTAL];
	CBasicButton	m_btCancel;

	int				m_nSlotNum;

	CTextOutBox		m_text[BT_TOTAL];

	int				m_nSubType;

	char			m_MsgBuf[1024];
};


// 길드 랭크전 참여 명
class CBattleAcceptMember : public CUIBaseWindow
{
public:
	class CDataList
	{
	public:
		static CDataList* Create(int nBaseX = 0, int nBaseY = 0);
		~CDataList();
	public:
		void Init();
		void LoadRes();

		void Draw(int nScrollPos);
		BOOL DrawToBlt(CSurface* pSurface, int nScrollPos);

		void DeleteRes();
		BOOL IsInside(int, int, int);

		void SetPosition(int nPosX, int nPosY, int nNum);
		void SetRePosition(int nBaseX, int nBaseY, int nPosX, int nPosY, int nNum);
		void SetCharIdx(int nIdx)
		{
			m_nCharidx = nIdx;
		}
		void Restore();

		void SetCharName(char* pName, COLORREF fontColor);
		void SendMsg();
		void SetConnect( int Connect)
		{
			m_Connect = Connect;
		}

		LRESULT MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, int nScroll);
	public:

		CTextOutBox m_TCharName;

		int		m_nBaseX;		// 공통 UI의 기본 좌표 X
		int		m_nBaseY;		// 공통 UI의 기본 좌표 Y

		int		m_nSequence;	// 나의 순서

		int		m_nCharidx;
		int		m_Connect;

		HWND	m_hWnd;
		RECT    m_ClientRc;

		char	m_MsgBuf[1024];

	private:
		CDataList(int nBaseX = 0, int nBaseY = 0);
	};

	list<CDataList*>	m_listMember;
	list<CDataList*>	m_listComein;

	CScroll				m_ScrollMember;
	CScroll				m_ScrollComein;

	int					m_nMemberCnt;
	int					m_nComeinCnt;

	int					m_nMemberSelect;
	int					m_nComeInSelect;
public:
	enum
	{
		BATTLE_NUMBER_5_10,
		BATTLE_NUMBER_11_20,
		BATTLE_NUMBER_21_30,
		BATTLE_NUMBER_31_40,
		BATTLE_NUMBER_UNLIMITED,
		BATTLE_NUMBER_TOTAL
	};
public:
	CBattleAcceptMember();
	virtual ~CBattleAcceptMember();

	/*******************************************************************/
	int	 Draw();
	void DeleteRes();
	void RestoreSurfaces();

	BOOL    IsInside(int x, int y);
	BOOL	IsMouseUpperWindow(int x, int y)
	{
		return TRUE;
	}

	BOOL	InitMode();
	void	LoadRes();

	void	Insert_list_Member();
	void	Insert_list_Member(guild_member& g_member);
	void	Insert_list_ComeIn();
	void	Insert_list_Comein(guild_member& g_member);

	BOOL	Exchage_Left_Right(int nSelect);
	BOOL	Exchage_Right_Left(int nSelect);

	BOOL	MoveWindow(UINT msg, int x, int y);
	void	SetPosition();
	void	list_Member_sort();
	void	list_ComeIn_sort();

	LRESULT MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	/*******************************************************************/

	void	SetChangeColor(int nLeft, int nRight);

	void	SetSubType(int nNum)
	{
		m_nSubType = nNum;
	}
//	void	SetMainType(int nNum) { m_nMainType = nNum; }
	void	SendMsg();

	BOOL	IsSelectLimited();

private:
	CBasicButton	m_btLeftArrow;
	CBasicButton	m_btRightArrow;
	CBasicButton	m_btOk;
	CBasicButton	m_btCancel;

	int				m_nSubType;
	CSurface*		m_pSelectSur[BATTLE_NUMBER_TOTAL];

	char			m_MsgBuf[1024];
};

#include "ComboBox.h"
// 침략할 길드 지정
class CBattleInvasionAttack : public CUIBaseWindow
{
public:
	CBattleInvasionAttack();
	virtual ~CBattleInvasionAttack();

	/*******************************************************************/
	int	 Draw();
	void DeleteRes();
	void RestoreSurfaces();

	BOOL    IsInside(int x, int y);
	BOOL	IsMouseUpperWindow(int x, int y)
	{
		return TRUE;
	}

	BOOL	InitMode();
	void	LoadRes();

	BOOL	MoveWindow(UINT msg, int x, int y);
	void	SetPosition();

	LRESULT MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	/*******************************************************************/

	void	SendMsg();

	BOOL	IsTextBoxFocus()
	{
		return (BOOL)m_TextGuildName.GetEnable();
	}

private:
	CBasicButton	m_btConfirm;
	CBasicButton	m_btCancel;

	char			m_MsgBuf[1024];

	CSurface*		m_pTextSur;
	CSurface*		m_pTyping;
	CSurface*		m_pInvasionInfo;

	CComboBox		m_ComboBox;
	CTextBox		m_TextGuildName;
};

// 침략중일때는 이것을 보여준다.
class CBattleInvasionInfo : public CUIBaseWindow
{
public:
	class CBunch
	{
	public:
		static CBunch* Create(int nBaseX = 0, int nBaseY = 0, int nTopLimit = 0, int nBottomLimit = 0);
		~CBunch();
	public:
		void Init(char *pBtFilename);
		void LoadRes();

		void Draw(int nScrollPos);
		BOOL DrawToBlt(CSurface* pSurface, int nScrollPos);

		void DeleteRes();
		BOOL IsInside(int, int);

		void SetBasePosition( int nBaseX, int nBaseY);
		void SetPosition(int nPosX, int nPosY, int nNum);
		void SetRePosition(int nNum, int nPosY);
		void SetGuildIdx(int nIdx)
		{
			m_nGuildIdx = nIdx;
		}
		void Restore();

		void SetGuildName(char* pName);
		void SetTime(t_Info timeT);
		void SendMsg();

		void SetInvasionType(INVASION_CHECK type)
		{
			m_InvasionType = type;
		}

		LRESULT MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, int nScroll );
	public:

		CTextOutBox m_tGuildName;
		CTextOutBox m_tTime;

		CBasicButton	m_BtFire;
		CBasicButton	m_btCancel;

		int		m_nBaseX;		// 공통 UI의 기본 좌표 X
		int		m_nBaseY;		// 공통 UI의 기본 좌표 Y

		int		m_nTopLimit;	// 그려지는 범위
		int		m_nBottomLimit; // 그려지는 범위

		int		m_nSequence;	// 나의 순서

		int		m_nGuildIdx;

		HWND	m_hWnd;
		RECT    m_ClientRc;

		char	m_MsgBuf[1024];

		INVASION_CHECK m_InvasionType;

	private:
		CBunch(int nBaseX = 0, int nBaseY = 0, int nTopLimit = 0, int nBottomLimit = 0);
	};

	vector<CBunch*>	m_AttackList;
	vector<CBunch*>	m_DefenseList;

	int	m_nAttackCnt;
	int m_nDefenseCnt;

	CScroll	m_AttackScroll;
	CScroll m_DefenseScroll;

public:
	CBattleInvasionInfo();
	virtual ~CBattleInvasionInfo();

	/*******************************************************************/
	int	 Draw();
	void DeleteRes();
	void RestoreSurfaces();

	BOOL    IsInside(int x, int y);
	BOOL	IsMouseUpperWindow(int x, int y)
	{
		return TRUE;
	}

	BOOL InitMode();
	void LoadRes();

	BOOL	MoveWindow(UINT msg, int x, int y);
	void	SetPosition();

	LRESULT MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	/*******************************************************************/

	void InsertList();
	void InsertList(guild_Invasion& Info_Invasion);
	void deleteList(guild_Invasion& Info_Invasion);
	void EraseData();
	void resetlist();

	void SendMsg();

private:
	CBasicButton	m_btOk;
	CBasicButton	m_btCancel;

	int m_nAttackSelectNum;
	int m_nDefenseSelectNum;

	int	m_nSelectType;

	char m_MsgBuf[1024];

};
#endif // _GUILD_BATTLE_H_