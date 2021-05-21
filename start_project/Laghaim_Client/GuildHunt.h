#ifndef _GUILD_HUNT_H_
#define _GUILD_HUNT_H_

#include "Config.h"

class CTcpIpConnection;
class CSurface;

class CGuildHunt
{
private:

	CSurface*			m_pBack;		// 배경 윈도우 이미지

	int					m_nScreenWidth;
	int					m_nScreenHeight;

	DWORD				m_dwLastOpenTime;
	BOOL				m_bIsOpenWindow;

	CTextOutBox			m_arGuildHuntIndex[ 5 ];		// 길드 사냥 예약 순번
	CTextOutBox			m_arGuildName[ 5 ];				// 길드 이름
//	CTextOutBox			m_arGuildHuntTime[ 5 ];			// 길드 사냥 예약 시간
	CTextOutBox			m_arReservState[ 5 ];			// 예약 상태
	CTextOutBox			m_arProposer[ 5 ];				// 신청자
	CTextOutBox			m_GuildDate;

	CBasicButton		m_arPageBtn[ 3 ];	// 페이지 버튼
	CBasicButton		m_arReservBtn[ 5 ];	// 예약 버튼
	CBasicButton		m_arCancelBtn[ 5 ];	// 예약 취소 버튼
	CBasicButton		m_ConfirmationBtn;	// 확인 버튼
	CBasicButton		m_RenewalBtn;		// 갱신 버튼

	//  [11/24/2009 ppmmjj83] x버튼 추가
	CBasicButton		m_CancelBtn;		// x 버튼


	BYTE				m_byCurPage;		// 현재 오픈된 페이지
	BYTE				m_bySelectGuildHuntIndex;
	BYTE				m_byReserved[ 5 ];	// 현재 인덱스가 예약된 상태이지 체크하는 플래그
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
