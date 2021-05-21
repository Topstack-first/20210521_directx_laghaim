// ControlHelper.h: interface for the CControlHelper class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CONTROLHELPER_H__71C46795_37FB_42D7_9A2A_0FF8BBE4C246__INCLUDED_)
#define AFX_CONTROLHELPER_H__71C46795_37FB_42D7_9A2A_0FF8BBE4C246__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BasicButton.h"
#include "ExtraButton.h"
#include "ComboBox.h"
#include "AlphaPopup.h"
#include "Scroll.h"
#include "NkCharacter.h"
#include "BasicStatic.h"

class CSurface;
class CSurface;
class CTcpIpConnection;
class CTextOutBox;

#define TRAINEE_NUM 4 // 800*600 해상도에서 한 화면에 보이는 견습생 리스트 수.

#define MAX_TRAINEE_NUM 8

struct sTraineeMember
{
	int m_Index;
	char *m_Name;
	int	m_nLevel;
	int	m_nRace;
	int	m_nSex;
	int	m_nConnect;
	DWORD	m_dwStartedTime; ///07-05-03 남은 날짜를 표시

	sTraineeMember()
	{
		m_Name = NULL;
		Init();
	}

	~sTraineeMember()
	{
		Init();
	}

	void Init()
	{
		m_Index = -1;
		m_nLevel = -1;
		m_nRace = -1;
		m_nSex = -1;
		m_nConnect = -1;
		m_dwStartedTime = -1; ///07-05-03 남은 날짜를 표시

		if( m_Name )
		{
			delete m_Name;
			m_Name = NULL;
		}
	}
};

class CControlHelper
{
	CSurface *m_pBackSur;
	CSurface *m_pInfoBoxSur[MAX_TRAINEE_NUM];    // 후견인/견습생 정보 박스.
	CSurface *m_pInfoBoxSeleteSur;               // 후견인/견습생 정보 박스 선택.
	CSurface *m_pExplanHelperSur;				  // 후견인 설명
	CSurface *m_pExplanTraineeSur;				  // 견습생 설명
	CSurface *m_pExplanGiveupSur;				  // 견습생 포기 설명
	CSurface *m_pExplanFailSur;				  // 실패 설명
	CSurface *m_pExplanUnableSur;                // 사용 가능한 레벨이 아님


	bool		 m_reject;
	CBasicStatic m_reject_helper[2];

public:
	CControlHelper();
	virtual ~CControlHelper();

	void	Init();
	void	DeleteRes();
	void	LoadRes();
	void	Draw();
	void	DrawMembers();
	BOOL	IsInside(int x, int y);
	void	SetFont (int f_w, int f_h, char *fontname);
	LRESULT MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

	HFONT m_Font;
	int	  m_CurClientMode; // 해상도 값 저장
	BOOL  m_bHelperAccept; // 후견인 승락 여부
	BOOL  m_bInfoClick;	   // 후견이/견습생 정보 표기 유무

	CScroll m_MemberScroll;
	CScroll m_ExplanScroll;

	CBasicButton	m_LetterSendBtn;		// 쪽지 보내기
	CBasicButton	m_HelperRejectBtn;		// 후견인 거부
	CBasicButton	m_TraineegiveupBtn;		// 견습생 포기
	CBasicButton	m_CloseBtn;

	int				m_nHelper;						//후견인/견습생
	int				m_nMember;						//맴버수
	int				m_nIndex;						//케릭 인덱스
	char			*m_chName[MAX_TRAINEE_NUM];		//이름
	int				m_nLevel[MAX_TRAINEE_NUM];
	int				m_nRace[MAX_TRAINEE_NUM];
	int				m_nSex[MAX_TRAINEE_NUM];
	int				m_nConnect[MAX_TRAINEE_NUM];
	DWORD			m_dwStartedTime[MAX_TRAINEE_NUM]; ///07-05-03 남은 날짜를 표시하기 위한 관계 시작일
	int				m_nExplan;						//설명할 종류
	int				m_nClickCount;
	int				m_nListNum;					    //몇번째 리스트가 선택

	CTextOutBox		m_NameTex[MAX_TRAINEE_NUM];
	CTextOutBox		m_LevelTex[MAX_TRAINEE_NUM];
	CTextOutBox		m_RaceTex[MAX_TRAINEE_NUM];
	CTextOutBox		m_SexTex[MAX_TRAINEE_NUM];
	CTextOutBox		m_ConnectTex[MAX_TRAINEE_NUM];
	CTextOutBox		m_DayLimitTex[MAX_TRAINEE_NUM]; ///07-05-03 남은 날짜를 위한 멤버 추가

	int m_PartyState;
	BOOL m_bIsMaster;
	int m_MemberCount;
	int m_ViewMemberNum; // 한 화면에 보이는 멤버리스트의 수. 해상도 별로 달라진다.
	int m_RequestMode; // 리퀘스트 패킷의 모드.(누가 누구한테 걸었느냐에 따라 모드가 바뀐다. 승락 패킷에서도 사용된다.)
	sTraineeMember m_aTraineeList[MAX_TRAINEE_NUM];

	int GetMemberlist( int nIndex );

	LRESULT PopupMsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, int nMsg);
	LRESULT PopupMsgProc2(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, int nMsg);
	void SetMember(int nHelper, int nMember);
	void Information( int nIndex, int count, char *ch_name, int nLevel, int nSex, int nRace, int nConnect, DWORD dwStartedTime ); //후견인/견습생 정보 ///07-05-03 남은 날짜를 위한 인자 추가
	void AskHelperRequest(char *player_name);
	void ConnectState(int nIndex, int nState);
	void HelperExplan( int nEnplan );
	void ResetAll();
	int IsMemberIndexInside( int x, int y );
};

#endif // !defined(AFX_CONTROLHELPER_H__71C46795_37FB_42D7_9A2A_0FF8BBE4C246__INCLUDED_)
