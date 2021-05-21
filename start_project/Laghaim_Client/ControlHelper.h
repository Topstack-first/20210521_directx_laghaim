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

#define TRAINEE_NUM 4 // 800*600 �ػ󵵿��� �� ȭ�鿡 ���̴� �߽��� ����Ʈ ��.

#define MAX_TRAINEE_NUM 8

struct sTraineeMember
{
	int m_Index;
	char *m_Name;
	int	m_nLevel;
	int	m_nRace;
	int	m_nSex;
	int	m_nConnect;
	DWORD	m_dwStartedTime; ///07-05-03 ���� ��¥�� ǥ��

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
		m_dwStartedTime = -1; ///07-05-03 ���� ��¥�� ǥ��

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
	CSurface *m_pInfoBoxSur[MAX_TRAINEE_NUM];    // �İ���/�߽��� ���� �ڽ�.
	CSurface *m_pInfoBoxSeleteSur;               // �İ���/�߽��� ���� �ڽ� ����.
	CSurface *m_pExplanHelperSur;				  // �İ��� ����
	CSurface *m_pExplanTraineeSur;				  // �߽��� ����
	CSurface *m_pExplanGiveupSur;				  // �߽��� ���� ����
	CSurface *m_pExplanFailSur;				  // ���� ����
	CSurface *m_pExplanUnableSur;                // ��� ������ ������ �ƴ�


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
	int	  m_CurClientMode; // �ػ� �� ����
	BOOL  m_bHelperAccept; // �İ��� �¶� ����
	BOOL  m_bInfoClick;	   // �İ���/�߽��� ���� ǥ�� ����

	CScroll m_MemberScroll;
	CScroll m_ExplanScroll;

	CBasicButton	m_LetterSendBtn;		// ���� ������
	CBasicButton	m_HelperRejectBtn;		// �İ��� �ź�
	CBasicButton	m_TraineegiveupBtn;		// �߽��� ����
	CBasicButton	m_CloseBtn;

	int				m_nHelper;						//�İ���/�߽���
	int				m_nMember;						//�ɹ���
	int				m_nIndex;						//�ɸ� �ε���
	char			*m_chName[MAX_TRAINEE_NUM];		//�̸�
	int				m_nLevel[MAX_TRAINEE_NUM];
	int				m_nRace[MAX_TRAINEE_NUM];
	int				m_nSex[MAX_TRAINEE_NUM];
	int				m_nConnect[MAX_TRAINEE_NUM];
	DWORD			m_dwStartedTime[MAX_TRAINEE_NUM]; ///07-05-03 ���� ��¥�� ǥ���ϱ� ���� ���� ������
	int				m_nExplan;						//������ ����
	int				m_nClickCount;
	int				m_nListNum;					    //���° ����Ʈ�� ����

	CTextOutBox		m_NameTex[MAX_TRAINEE_NUM];
	CTextOutBox		m_LevelTex[MAX_TRAINEE_NUM];
	CTextOutBox		m_RaceTex[MAX_TRAINEE_NUM];
	CTextOutBox		m_SexTex[MAX_TRAINEE_NUM];
	CTextOutBox		m_ConnectTex[MAX_TRAINEE_NUM];
	CTextOutBox		m_DayLimitTex[MAX_TRAINEE_NUM]; ///07-05-03 ���� ��¥�� ���� ��� �߰�

	int m_PartyState;
	BOOL m_bIsMaster;
	int m_MemberCount;
	int m_ViewMemberNum; // �� ȭ�鿡 ���̴� �������Ʈ�� ��. �ػ� ���� �޶�����.
	int m_RequestMode; // ������Ʈ ��Ŷ�� ���.(���� �������� �ɾ����Ŀ� ���� ��尡 �ٲ��. �¶� ��Ŷ������ ���ȴ�.)
	sTraineeMember m_aTraineeList[MAX_TRAINEE_NUM];

	int GetMemberlist( int nIndex );

	LRESULT PopupMsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, int nMsg);
	LRESULT PopupMsgProc2(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, int nMsg);
	void SetMember(int nHelper, int nMember);
	void Information( int nIndex, int count, char *ch_name, int nLevel, int nSex, int nRace, int nConnect, DWORD dwStartedTime ); //�İ���/�߽��� ���� ///07-05-03 ���� ��¥�� ���� ���� �߰�
	void AskHelperRequest(char *player_name);
	void ConnectState(int nIndex, int nState);
	void HelperExplan( int nEnplan );
	void ResetAll();
	int IsMemberIndexInside( int x, int y );
};

#endif // !defined(AFX_CONTROLHELPER_H__71C46795_37FB_42D7_9A2A_0FF8BBE4C246__INCLUDED_)
