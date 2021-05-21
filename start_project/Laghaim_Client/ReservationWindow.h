#ifndef _RESERVATION_WINDOW_H_
#define _RESERVATION_WINDOW_H_

#include "Config.h"

class CTcpIpConnection;
class CSurface;

class CReservationWindow
{
private:

	CSurface*			m_pBack;		// ��� ������ �̹���

	DWORD				m_dwLastOpenTime;
	BOOL				m_bIsOpenWindow;
//=================================================================
	// nate 2004 - 10
	CTextOutBox			m_Proposer;			// ũ����Ż ī��Ʈ ��¹ڽ�
	CTextOutBox			m_GuildName;		// ����� ��� ��� �ڽ�
	CTextOutBox			m_GuildHuntDate;	// ����� ��¥
	CTextOutBox			m_GuildHuntTime;	// ����� �ð�
//=================================================================

	CBasicButton		m_ReservBtn;	// ���� ��ư
	CBasicButton		m_CancelBtn;	// ��� ��ư

	int					m_nCristalCount;	// ũ����Ż ����
	int					m_nGuildHuntCost;	// ����� ���

	BYTE				m_byCurPage;		// ���� ���µ� ������
	BYTE				m_bySelectGuildHuntIndex;	// ���õ� ��� ��� �ε���

public:
//=================================================================
	// nate 2004 - 10
	void SetReservInfo( BYTE byCurPage
						, BYTE bySelectGuildHuntIndex
						, char* szGuildHuntDate
						, char* szGuildName
						, char* szProposer );
//=================================================================
	void InitTextOutBox();
	void SetButtonPos();
	void SetStringPos();
	void SetReservationInform();
	void SendQuestApplyMsg( BYTE byMessageType );
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
	CReservationWindow();
	virtual ~CReservationWindow();
};

#endif // _RESERVATION_WINDOW_H_
