#ifndef _GUILD_HUNT_H_
#define _GUILD_HUNT_H_

#include "Config.h"

class CTcpIpConnection;
class CSurface;

class CGuildHunt
{
private:

	CSurface*			m_pBack;		// ��� ������ �̹���

	int					m_nScreenWidth;
	int					m_nScreenHeight;

	DWORD				m_dwLastOpenTime;
	BOOL				m_bIsOpenWindow;

	CTextOutBox			m_arGuildHuntIndex[ 5 ];		// ��� ��� ���� ����
	CTextOutBox			m_arGuildName[ 5 ];				// ��� �̸�
//	CTextOutBox			m_arGuildHuntTime[ 5 ];			// ��� ��� ���� �ð�
	CTextOutBox			m_arReservState[ 5 ];			// ���� ����
	CTextOutBox			m_arProposer[ 5 ];				// ��û��
	CTextOutBox			m_GuildDate;

	CBasicButton		m_arPageBtn[ 3 ];	// ������ ��ư
	CBasicButton		m_arReservBtn[ 5 ];	// ���� ��ư
	CBasicButton		m_arCancelBtn[ 5 ];	// ���� ��� ��ư
	CBasicButton		m_ConfirmationBtn;	// Ȯ�� ��ư
	CBasicButton		m_RenewalBtn;		// ���� ��ư

	//  [11/24/2009 ppmmjj83] x��ư �߰�
	CBasicButton		m_CancelBtn;		// x ��ư


	BYTE				m_byCurPage;		// ���� ���µ� ������
	BYTE				m_bySelectGuildHuntIndex;
	BYTE				m_byReserved[ 5 ];	// ���� �ε����� ����� �������� üũ�ϴ� �÷���
	BYTE				m_byReservBtn_Count;

public:
//=============================================
	// nate 2004 - 10
	char* GetProposer();
	char* GetGuildName();
	char* GetGuildHuntDate()
	{
		return m_GuildDate.m_str;
	}
//=============================================
	void SetGuildDate( char* szGuildDate );
	void SetHuntIndex();
	BYTE GetCurrentPage()
	{
		return m_byCurPage;
	}
	BYTE GetSelectGuildHuntIndex()
	{
		return m_bySelectGuildHuntIndex;
	}
	void InitTextOutBox();
	void SetButtonPos();
	void SetStringPos();
	void SetReservationInform( char* szReservGuildName, char* CristalCount, int nReservState, int nReservIndex );
	void SendQuestApplyMsg( BYTE byApplyIndex );
	BOOL GetIsOpenWindow()
	{
		return m_bIsOpenWindow;
	}
	void SetDrawPos();
	BOOL IsInside( int x, int y );
	LRESULT MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
	void Draw();
	void DeleteRes();
	void LoadRes();
	CGuildHunt();
	virtual ~CGuildHunt();

};

#endif // _GUILD_HUNT_H_
