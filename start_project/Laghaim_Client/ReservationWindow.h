#ifndef _RESERVATION_WINDOW_H_
#define _RESERVATION_WINDOW_H_

#include "Config.h"

class CTcpIpConnection;
class CSurface;

class CReservationWindow
{
private:

	CSurface*			m_pBack;		// 배경 윈도우 이미지

	DWORD				m_dwLastOpenTime;
	BOOL				m_bIsOpenWindow;
//=================================================================
	// nate 2004 - 10
	CTextOutBox			m_Proposer;			// 크리스탈 카운트 출력박스
	CTextOutBox			m_GuildName;		// 길드사냥 비용 출력 박스
	CTextOutBox			m_GuildHuntDate;	// 길드사냥 날짜
	CTextOutBox			m_GuildHuntTime;	// 길드사냥 시간
//=================================================================

	CBasicButton		m_ReservBtn;	// 예약 버튼
	CBasicButton		m_CancelBtn;	// 취소 버튼

	int					m_nCristalCount;	// 크리스탈 갯수
	int					m_nGuildHuntCost;	// 길드사냥 비용

	BYTE				m_byCurPage;		// 현재 오픈된 페이지
	BYTE				m_bySelectGuildHuntIndex;	// 선택된 길드 사냥 인덱스

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
