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

#define MEMBER_NUM 8 // 800*600 �ػ󵵿��� �� ȭ�鿡 ���̴� ��Ƽ�� ����Ʈ ��.

#define PARTY_STATE_NONE	0
#define PARTY_STATE_REQUEST	1
#define PARTY_STATE_MEMBER	2

#define VOICECHAT_NONE	0
#define VOICECHAT_PREPARE	1
#define VOICECHAT_STARTED	2


// ��ǥ ������ ��ũ�ε�
#define PARTY_LIST_START_X		60
#define PARTY_LIST_START_Y		98 // �ػ� ���� �ٸ���.
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
	BOOL    IsMemberListInside(int x, int y); // ��� ����Ʈ ����(����̸�, ��Ƽ�� ���� ��ư��) üũ��.
	int     IsMemberNameInside(int x, int y); // ��� ����Ʈ �������� ����̸� üũ��.
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

			m_ExpGetRule.SetState( -1 ); // ��Ȱ���� ��Ų��. ���̻� ���ٲٰ�..
			m_ItemGetRule.SetState( -1 ); // ��Ȱ���� ��Ų��. ���̻� ���ٲٰ�..
		}
		else
		{
			m_PartyState = PARTY_STATE_NONE;

			m_ExpGetRule.SetState( 0 ); // ��Ȱ���� Ǭ��.
			m_ExpGetRule.SetSelect( 0 ); // ����Ʈ ������..
			m_ItemGetRule.SetState( 0 ); // ��Ȱ���� Ǭ��.
			m_ItemGetRule.SetSelect( 0 ); // ����Ʈ ������..

			m_bIsMaster = FALSE; // ��ü �� �� ������ ������ ����

		}
	}

	HFONT m_Font;
	int	  m_CurClientMode; // �ػ� �� ����

	float m_fStatrTime;

	CSurface *m_pNameBox;     // �̸� ��� ���.
	CSurface *m_pMasterMark;  // ���帶ũ ��¿�

	CScroll m_MemberScroll;

	CComboBox m_ExpGetRule; // ����ġ �й� ���.
	CComboBox m_ItemGetRule; // ������ �й� ���.

	CToggleButton m_ViewMemberState;  // ��Ƽ���� ����.
	CToggleButton m_RequestRejection; // �ʴ�ź�.
	CBasicButton	m_AskjoinBtn;		// ���Խ�û.

	CBasicButton	m_AskoutBtn;		// Ż���û
	CBasicButton	m_CloseBtn;


	// ��Ƽ������ �ʿ� ��ư �̹��� ������ ��ư��.(�������� ������ ��Ƽ ����� ���� ���� ������ �ְ�, �� �༮���� �̹��� �ε� �������̴�.)
	CBasicButton	m_DumyMandateBtn;		// ����.
	CBasicButton	m_DumyAbsOutBtn;		// �ػ�.

	// �� ��ư���� ����Ʈ���� ������ �� �ִ� �ִ�ġ�� �������� ������ ����, �̹����� ���� ���̿��� �����ؼ� ����.
	CBasicButton	m_DisbandBtn;		// �ػ�(�̳༮�� ������ �ϳ� ���̴�. ��, ���̰� �ʿ����.)
	CBasicButton	m_MandateBtn[MEMBER_NUM];		// ����
	CBasicButton	m_AbsOutBtn[MEMBER_NUM];		// ����

	int m_PartyState;
	BOOL m_bIsMaster;
	int m_MemberCount;
	int m_ViewMemberNum; // �� ȭ�鿡 ���̴� �������Ʈ�� ��. �ػ� ���� �޶�����.
	int m_RequestMode; // ������Ʈ ��Ŷ�� ���.(���� �������� �ɾ����Ŀ� ���� ��尡 �ٲ��. �¶� ��Ŷ������ ���ȴ�.)

	sPartyMember m_aPartyList[MAX_PARTY_NUM_BOSSRAID_FINAL];
	CSurface*		m_pPartyCountSur;	// [2008/2/28 Theodoric] ��Ƽ�� ǥ��

	void AddMember(int char_idx, char *name);
	void SetPartyCount();	// [2008/2/28 Theodoric] ��Ƽ�� ǥ��
	void RemoveMember(int char_idx);
	void RemoveMemberAll();

	void ShowMyPartysPos(int, int, int);
	BOOL IsMyParty(int char_idx);

	char* GetMemberName( int char_idx );

	BOOL m_bShowPartyVital;

	LRESULT PopupMsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, int nMsg);
	LRESULT ReQuestPopupMsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, int nMsg, char* szMoney);
	void AskParty( int RequestMode, char *subscriber_name, int leader_index, char *leader_name, int ExpGetMode = -1, int ItemGetMode = -1 ); ///��Ƽ����
};


#endif // _CONTROL_PARTY_H_
