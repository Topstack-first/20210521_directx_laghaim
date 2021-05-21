#ifndef _CONTROL_PARTY_H_
#define _CONTROL_PARTY_H_

#include "Config.h"

#include "BasicButton.h"
#include "ExtraButton.h"
#include "ComboBox.h"
#include "AlphaPopup.h"
#include "Scroll.h"
#include "NkCharacter.h"

class CSurface;
class CSurface;
class CTcpIpConnection;
class CTextOutBox;

#define MEMBER_NUM 8 // 800*600 해상도에서 한 화면에 보이는 파티원 리스트 수.

#define PARTY_STATE_NONE	0
#define PARTY_STATE_REQUEST	1
#define PARTY_STATE_MEMBER	2

#define VOICECHAT_NONE	0
#define VOICECHAT_PREPARE	1
#define VOICECHAT_STARTED	2


// 좌표 보정용 매크로들
#define PARTY_LIST_START_X		60
#define PARTY_LIST_START_Y		98 // 해상도 별로 다르다.
#define PARTY_LIST_LINE_X_GAB	8
#define PARTY_LIST_LINE_Y_GAB	25

struct sPartyMember
{
	int m_Index;
	char *m_Name;
	int m_Vital;
	int m_MaxVital;

	CSurface *m_pSurName;

	sPartyMember()
	{
		m_Name = NULL;
		m_pSurName = NULL;

		Init();
	}

	~sPartyMember()
	{
		Init();
	}

	void Init()
	{
		m_Index = -1;
		m_Vital = 0;
		m_MaxVital = 0;

		if( m_Name )
		{
			delete m_Name;
			m_Name = NULL;
		}

		if( m_pSurName )
		{
			delete m_pSurName;
			m_pSurName = NULL;
		}
	}
};

class CControlParty
{
	CSurface*		m_pBackSur;

public:
	CControlParty();
	virtual ~CControlParty();

	void	Init();
	void	DeleteRes();
	void	LoadRes();
	HRESULT RestoreSurfaces();
	void	Draw();
	void	DrawMembers();
	BOOL	IsInside(int x, int y);
	BOOL    IsMemberListInside(int x, int y); // 멤버 리스트 영역(멤버이름, 파티장 전용 버튼들) 체크용.
	int     IsMemberNameInside(int x, int y); // 멤버 리스트 영역안의 멤버이름 체크용.
	void	SetFont (int f_w, int f_h, char *fontname);
	LRESULT MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

	int GetExpRule()
	{
		return m_ExpGetRule.GetSelect();
	}

	int GetItemRule()
	{
		return m_ItemGetRule.GetSelect();
	}

	void SetParty( BOOL state )
	{
		if( state )
		{
			m_PartyState = PARTY_STATE_MEMBER;

			m_ExpGetRule.SetState( -1 ); // 비활성을 시킨다. 더이상 못바꾸게..
			m_ItemGetRule.SetState( -1 ); // 비활성을 시킨다. 더이상 못바꾸게..
		}
		else
		{
			m_PartyState = PARTY_STATE_NONE;

			m_ExpGetRule.SetState( 0 ); // 비활성을 푼다.
			m_ExpGetRule.SetSelect( 0 ); // 디폴트 값으로..
			m_ItemGetRule.SetState( 0 ); // 비활성을 푼다.
			m_ItemGetRule.SetSelect( 0 ); // 디폴트 값으로..

			m_bIsMaster = FALSE; // 해체 될 때 무조건 파장은 해제

		}
	}

	HFONT m_Font;
	int	  m_CurClientMode; // 해상도 값 저장

	float m_fStatrTime;

	CSurface *m_pNameBox;     // 이름 출력 배경.
	CSurface *m_pMasterMark;  // 파장마크 출력용

	CScroll m_MemberScroll;

	CComboBox m_ExpGetRule; // 경험치 분배 방식.
	CComboBox m_ItemGetRule; // 아이템 분배 방식.

	CToggleButton m_ViewMemberState;  // 파티상태 보기.
	CToggleButton m_RequestRejection; // 초대거부.
	CBasicButton	m_AskjoinBtn;		// 가입신청.

	CBasicButton	m_AskoutBtn;		// 탈퇴신청
	CBasicButton	m_CloseBtn;


	// 파티원마다 필요 버튼 이미지 참조용 버튼들.(실질적인 쓰임은 파티 멤버들 마다 따로 가지고 있고, 이 녀석들은 이미지 로드 보관용이다.)
	CBasicButton	m_DumyMandateBtn;		// 위임.
	CBasicButton	m_DumyAbsOutBtn;		// 해산.

	// 이 버튼들은 리스트에서 보여질 수 있는 최대치를 기준으로 갯수를 산정, 이미지는 위의 더미에서 참조해서 쓴다.
	CBasicButton	m_DisbandBtn;		// 해산(이녀석은 무조건 하나 뿐이다. 즉, 더미가 필요없다.)
	CBasicButton	m_MandateBtn[MEMBER_NUM];		// 위임
	CBasicButton	m_AbsOutBtn[MEMBER_NUM];		// 강퇴

	int m_PartyState;
	BOOL m_bIsMaster;
	int m_MemberCount;
	int m_ViewMemberNum; // 한 화면에 보이는 멤버리스트의 수. 해상도 별로 달라진다.
	int m_RequestMode; // 리퀘스트 패킷의 모드.(누가 누구한테 걸었느냐에 따라 모드가 바뀐다. 승락 패킷에서도 사용된다.)

	sPartyMember m_aPartyList[MAX_PARTY_NUM_BOSSRAID_FINAL];
	CSurface*		m_pPartyCountSur;	// [2008/2/28 Theodoric] 파티수 표시

	void AddMember(int char_idx, char *name);
	void SetPartyCount();	// [2008/2/28 Theodoric] 파티수 표시
	void RemoveMember(int char_idx);
	void RemoveMemberAll();

	void ShowMyPartysPos(int, int, int);
	BOOL IsMyParty(int char_idx);

	char* GetMemberName( int char_idx );

	BOOL m_bShowPartyVital;

	LRESULT PopupMsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, int nMsg);
	LRESULT ReQuestPopupMsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, int nMsg, char* szMoney);
	void AskParty( int RequestMode, char *subscriber_name, int leader_index, char *leader_name, int ExpGetMode = -1, int ItemGetMode = -1 ); ///파티개편
};


#endif // _CONTROL_PARTY_H_
