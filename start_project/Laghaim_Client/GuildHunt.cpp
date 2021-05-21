#include "stdafx.h"
#include "headers.h"
#include "main.h"
#include "UIMgr.h"
#include "Nk2DFrame.h"
#include "NKCharacter.h"
#include "BasicButton.h"
#include "TcpIpCon.h"
#include "Land.h"
#include "g_stringmanager.h"
#include "GuildHunt.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////






#define GUILDHUNT_OPEN		1
#define GUILDHUNT_REQUEST	2
#define GUILDHUNT_CANCEL	3
#define EVENT_MOVE			4

CGuildHunt::CGuildHunt()
{
	m_pBack = NULL;
	m_nScreenWidth = 0;
	m_nScreenHeight = 0;
	m_dwLastOpenTime = timeGetTime();
	m_bIsOpenWindow = FALSE;
	m_byCurPage = 1;
	m_bySelectGuildHuntIndex = 0;
	m_byReservBtn_Count = 0;
	ZeroMemory( m_byReserved, sizeof(m_byReserved) );
}

CGuildHunt::~CGuildHunt()
{
	DeleteRes();
}

// 자원 로딩
void CGuildHunt::LoadRes()
{
	int i = 0;

	if( !g_pDisplay)
		return;

	RECT rc;
	HWND hWnd = g_pDisplay->GetHWnd();
	GetClientRect( hWnd, &rc );

	m_nScreenWidth = rc.right;
	m_nScreenHeight = rc.bottom;

	// 바탕이미지 로딩
	if(pCMyApp->m_nCurWorldIdx == WORLD_DMITRON)
	{
		g_pDisplay->CreateSurfaceFromBitmap( &m_pBack, "interface/GuildHunt/EventBack.bmp");
		m_byReservBtn_Count = 3;
	}
	else
	{
		g_pDisplay->CreateSurfaceFromBitmap( &m_pBack, "interface/GuildHunt/guildhunt.bmp");
		m_byReservBtn_Count = 5;
	}

	m_pBack->SetColorKey( TRANS_COLOR );

	// 페이지 버튼
	char szTemp[ 100 ] = {0,};

	if(pCMyApp->m_nCurWorldIdx == WORLD_DMITRON)
	{
		m_arPageBtn[ 0 ].SetFileName( "GuildHunt/Page1_btn" );
		m_arPageBtn[ 1 ].SetFileName( "GuildHunt/Page2_btn" );
		m_arPageBtn[ 2 ].SetFileName( "GuildHunt/Page3_btn" );
	}
	else
	{
		m_arPageBtn[ 0 ].SetFileName( "GuildHunt/btn_today" );
		m_arPageBtn[ 1 ].SetFileName( "GuildHunt/btn_tomorrow" );
		m_arPageBtn[ 2 ].SetFileName( "GuildHunt/btn_after" );
	}

	for( i = 0 ; i < 3 ; i ++ )
		m_arPageBtn[ i ].LoadRes();


	for( i = 0 ; i < m_byReservBtn_Count ; i ++ )
	{
		// 예약 버튼
		m_arReservBtn[ i ].SetFileName( "GuildHunt/btn_booking" );
		m_arReservBtn[ i ].LoadRes();

		// 에약 취소 버튼
		m_arCancelBtn[ i ].SetFileName( "GuildHunt/btn_hunt_cancel" );
		m_arCancelBtn[ i ].LoadRes();
	}

	// x버튼
	m_CancelBtn.SetFileName("common/btn_close_01");
	m_CancelBtn.LoadRes();

	// 확인 버튼
	m_ConfirmationBtn.SetFileName( "GuildHunt/btn_confirm" );
	m_ConfirmationBtn.LoadRes();

	// 갱신 버튼
	if(pCMyApp->m_nCurWorldIdx == WORLD_DMITRON)
		m_RenewalBtn.SetFileName( "GuildHunt/Entrance_btn" );
	else
		m_RenewalBtn.SetFileName( "GuildHunt/btn_renewal" );
	m_RenewalBtn.LoadRes();

	// 예약 정보 출력박스 초기화
	InitTextOutBox();
}

// 자원 해제
void CGuildHunt::DeleteRes()
{
	int i = 0;

	SAFE_DELETE( m_pBack );

	// 페이지 버튼 해제
	for( i = 0 ; i < 3 ; i ++ )
		m_arPageBtn[ i ].DeleteRes();

	for( i = 0 ; i < m_byReservBtn_Count ; i ++ )
	{
		// 텍스트 박스 자원헤제
		m_arGuildHuntIndex[ i ].DeleteRes();
		m_arGuildName[ i ].DeleteRes();
		m_arReservState[ i ].DeleteRes();
		m_arProposer[ i ].DeleteRes();
		// 예약 버튼 헤제
		m_arReservBtn[ i ].DeleteRes();
		// 취소버튼 해제
		m_arCancelBtn[ i ].DeleteRes();
	}
	// 날짜 박스 헤제
	m_GuildDate.DeleteRes();
	// 확인 버튼 해제
	m_ConfirmationBtn.DeleteRes();
	// 갱신 버튼 헤제
	m_RenewalBtn.DeleteRes();

	// X버튼
	m_CancelBtn.DeleteRes();
}

// 화면 출력
void CGuildHunt::Draw()
{
	if( !g_pDisplay || !m_pBack )
		return;

	int i  = 0;

	SetDrawPos();

	// 바탕 그리기
	g_pDisplay->Blt( m_pBack->Xpos, m_pBack->Ypos, m_pBack );

	// 날짜 박스 그리기
	m_GuildDate.Draw(170);

	for( i  = 0 ; i < m_byReservBtn_Count ; i ++ )
	{
		// 텍스트 그리기
		m_arGuildHuntIndex[ i ].Draw(37);
		m_arGuildName[ i ].Draw(128);
		m_arProposer[ i ].Draw(98);
		m_arReservState[ i ].Draw(64);

		if( m_byReserved[ i ] != 5 )
		{
			if( m_byReserved[ i ] == 2 )
				// 예약 취소버튼 그리기
				m_arCancelBtn[ i ].Draw();
			else if( m_byReserved[ i ] == 0 )
			{
				// 예약버튼 그리기
				if( ( pCMyApp->m_nCurWorldIdx == WORLD_GUILD
						|| pCMyApp->m_nCurWorldIdx == WORLD_WHITEHORN )
						&& !(m_byCurPage == 1) )
					m_arReservBtn[ i ].Draw();
			}
		}
	}

	//  확인 버튼 그리기
	m_ConfirmationBtn.Draw();

	// 갱신버튼 그리기
	m_RenewalBtn.Draw();

	// X버튼
	m_CancelBtn.Draw();

	// 페이지 버튼 그리기
	for( i = 0 ; i < 3 ; i ++ )
		m_arPageBtn[ i ].Draw();

	if( m_bIsOpenWindow == FALSE )
	{
		if( ( timeGetTime() - m_dwLastOpenTime ) > 300 )
		{
			m_bIsOpenWindow = TRUE;
			m_dwLastOpenTime = timeGetTime();
		}
	}
}

// 마우스가 윈도우 안에 들어 왔는지 체크
BOOL CGuildHunt::IsInside( int x, int y )
{
	if( !m_pBack )
		return FALSE;

	return m_pBack->IsIn( x, y );
}

// 바탕 윈도우및 버튼의 위치 설정
// 매 프레임 마다 해야 출력 되네..이상타
void CGuildHunt::SetDrawPos()
{
	if( !g_pNk2DFrame || !m_pBack )
		return;

	int i = 0;
	// 인터페이스 윈도우가 열린 상태인지 체크
	if( g_pNk2DFrame->IsHelpVisible()			|| g_pNk2DFrame->IsInvenVisible()
			|| g_pNk2DFrame->IsPartyVisible()		|| g_pNk2DFrame->IsInterfaceVisible(SCORE)
			|| g_pNk2DFrame->IsSkillVisible()		|| g_pNk2DFrame->IsInterfaceVisible(MAP)
			|| g_pNk2DFrame->IsPetStatusVisible()	|| g_pNk2DFrame->IsInterfaceVisible(QUEST) )
		m_pBack->Xpos = abs( m_nScreenWidth - (int)m_pBack->GetWidth() - 274 )  / 2;	// 274는 인터페이스 크기
	else
		m_pBack->Xpos = abs( m_nScreenWidth - (int)m_pBack->GetWidth() ) / 2;

	m_pBack->Ypos = abs( m_nScreenHeight - (int)m_pBack->GetHeight() ) / 2 - 15;

	// 예약 정보 출력 좌표 설정
	SetStringPos();
	// 버튼 좌표 설정
	SetButtonPos();
}

// 메세지 처리
LRESULT CGuildHunt::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	int x, y, i;
	x = LOWORD( lParam );
	y = HIWORD( lParam );

	switch( uMsg )
	{
	case WM_RBUTTONDOWN:
		return 1;

	case WM_LBUTTONDOWN:

		if( !IsInside( x, y ) )
			return 2;

		// 페이지 버튼 메세지 처리
		for( i = 0 ; i < 3 ; i ++ )
			m_arPageBtn[ i ].MsgProc( hWnd, uMsg, wParam, lParam );

		for( i = 0 ; i < m_byReservBtn_Count ; i ++ )
		{
			if( m_byReserved[ i ] != 5 )
			{
				if( m_byReserved[ i ] == 2 )
					// 예약 취소 버튼
					m_arCancelBtn[ i ].MsgProc( hWnd, uMsg, wParam, lParam );
				else if( m_byReserved[ i ] == 0 )
				{
					// 예약버튼
					if( ( pCMyApp->m_nCurWorldIdx == WORLD_GUILD
							|| pCMyApp->m_nCurWorldIdx == WORLD_WHITEHORN )
							&& !(m_byCurPage == 1) )
						m_arReservBtn[ i ].MsgProc( hWnd, uMsg, wParam, lParam );
				}
			}
		}

		// 확인 버튼
		m_ConfirmationBtn.MsgProc( hWnd, uMsg, wParam, lParam );
		// 갱신 버튼
		m_RenewalBtn.MsgProc( hWnd, uMsg, wParam, lParam );
		// x버튼
		m_CancelBtn.MsgProc( hWnd, uMsg, wParam, lParam );

		return 1;

	case WM_LBUTTONUP:
		if( !IsInside( x, y ) )
			if( !pCMyApp->m_pUIMgr->m_bIsGuildHunt )
				return 2;

		// x버튼
		m_CancelBtn.MsgProc( hWnd, uMsg, wParam, lParam );
		if( m_CancelBtn.m_iBtnFlg == BTN_ACTION )
		{
			m_CancelBtn.m_iBtnFlg = BTN_NORMAL;
			return 2;
		}

		// 확인 버튼
		m_ConfirmationBtn.MsgProc( hWnd, uMsg, wParam, lParam );
		if( m_ConfirmationBtn.m_iBtnFlg == BTN_ACTION )
		{
			m_ConfirmationBtn.m_iBtnFlg = BTN_NORMAL;
			return 2;
		}

		// 갱신 버튼
		m_RenewalBtn.MsgProc( hWnd, uMsg, wParam, lParam );
		if( m_RenewalBtn.m_iBtnFlg == BTN_ACTION )
		{
			m_RenewalBtn.m_iBtnFlg = BTN_NORMAL;
			if( pCMyApp->m_nCurWorldIdx == WORLD_DMITRON )
			{
				g_pNk2DFrame->SetVisibleTopWindow(0);
				pCMyApp->m_pUIMgr->m_bWarInfo = 0;
				SendQuestApplyMsg( EVENT_MOVE );
				return 2;
			}
			else
			{
				SendQuestApplyMsg( GUILDHUNT_OPEN );
				return 1;
			}
		}

		// 페이지 버튼 처리
		for( i = 0 ; i < 3 ; i ++ )
		{
			m_arPageBtn[ i ].MsgProc( hWnd, uMsg, wParam, lParam );

			if( m_arPageBtn[ i ].m_iBtnFlg == BTN_ACTION )
			{
				m_arPageBtn[ i ].m_iBtnFlg = BTN_NORMAL;
				m_byCurPage = i + 1;
				for( int j = 0 ; j < 5 ; j ++ )
				{
					m_byReserved[ j ] = -1;
				}
				SendQuestApplyMsg( GUILDHUNT_OPEN );
				return 1;
			}
		}

		for( i = 0 ; i < m_byReservBtn_Count ; i ++ )
		{
			if( m_byReserved[ i ] != 5 )
			{
				// 예약 취소 버튼 처리
				if( m_byReserved[ i ] == 2 )
				{
					m_arCancelBtn[ i ].MsgProc( hWnd, uMsg, wParam, lParam );

					if( m_arCancelBtn[ i ].m_iBtnFlg == BTN_ACTION )
					{
						m_arCancelBtn[ i ].m_iBtnFlg = BTN_NORMAL;
						m_bySelectGuildHuntIndex = i + 1;
						SendQuestApplyMsg( GUILDHUNT_CANCEL );
						return 2;
					}
				}
				// 예약버튼 처리
				else if( m_byReserved[ i ] == 0 )
				{
					if( (pCMyApp->m_nCurWorldIdx == WORLD_GUILD
							|| pCMyApp->m_nCurWorldIdx == WORLD_WHITEHORN )
							&& !(m_byCurPage == 1) )
					{
						m_arReservBtn[ i ].MsgProc( hWnd, uMsg, wParam, lParam );

						if( m_arReservBtn[ i ].m_iBtnFlg == BTN_ACTION )
						{
							m_arReservBtn[ i ].m_iBtnFlg = BTN_NORMAL;
							m_bySelectGuildHuntIndex = i + 1;
							if( g_pRoh->m_pstrGuildName )
							{
								SendQuestApplyMsg( GUILDHUNT_REQUEST );
								return 3;
							}
							else
							{
								// nate 2005-02-18 - IDS_NOGUILD : 소속된 길드가 없습니다.
								g_pNk2DFrame->InsertPopup((char *)G_STRING(IDS_NOGUILD),TYPE_NOR_OK, 1);
								return 2;
							}
						}
					}
				}
			}
		}

		break;

	case WM_MOUSEMOVE:
		return 1;
	}

	return 0;
}

void CGuildHunt::SendQuestApplyMsg( BYTE nApplyIndex )
// 길드 사냥 관련 클라이언트 메세지 처리
{
	if( !g_pTcpIp )
		return;

	char szMsg[ 50 ] = "";

	// 페이지 요청
	if( nApplyIndex == GUILDHUNT_OPEN )
	{
		sprintf( szMsg, "gh open %d\n", m_byCurPage );
	}
	// 서버에 락걸기
	// 현재 인덱스의 예약을 독점할것을 요청
	else if( nApplyIndex == GUILDHUNT_REQUEST )
	{
		sprintf( szMsg, "gh req %d 0 0 0 %d\n", m_byCurPage, m_bySelectGuildHuntIndex );
	}
	// 예약 취소
	else if( nApplyIndex == GUILDHUNT_CANCEL )
	{
		sprintf( szMsg, "gh cancel %d %d\n",  m_byCurPage, m_bySelectGuildHuntIndex );
	}
	else if( nApplyIndex == EVENT_MOVE )
	{
		sprintf( szMsg, "gh go\n" );
	}

	g_pTcpIp->SendNetMessage( szMsg );
}

void CGuildHunt::SetReservationInform( char* szReservGuildName,
									   char* szProposer,
									   int nReservState,
									   int nReservIndex )
// 서버로 부터 받은 예약 정보를 맵버 변수에 셋팅
{
	// 예약 상태 체크
	m_byReserved[ nReservIndex ] = nReservState;

	// 상태 플레그 문자화
	char szTemp[ 30 ] = { "\0", };

	switch( nReservState )
	{
	case 0:
		//-- IDS_GUH_DONT_FIX	: 미정
		strcpy( szTemp, (char*)G_STRING(IDS_GUH_DONT_FIX));
		break;
	case 1:
		//-- IDS_GUH_RESRV_DOING : 예약중..
		strcpy( szTemp, (char*)G_STRING(IDS_GUH_RESRV_DOING) );
		break;
	case 2:
		//-- IDS_GUH_RESRV_COMP	: 예약 완료
		strcpy( szTemp,(char*)G_STRING(IDS_GUH_RESRV_COMP) );
		break;
	case 3:
		//-- IDS_GUH_GAME_DOING	: 경기중
		strcpy( szTemp, (char*)G_STRING(IDS_GUH_GAME_DOING));
		break;
	case 4:
		//-- IDS_GUH_GAME_COMP	: 경기종료
		strcpy( szTemp, (char*)G_STRING(IDS_GUH_GAME_COMP));
		break;
	case 5:
		//-- IDS_GUH_CANT_APP	:접근 불가
		strcpy( szTemp, (char*)G_STRING(IDS_GUH_CANT_APP));
		break;
	}

	// 길드사냥 인덱스 셋팅
	SetHuntIndex();

	m_arGuildName[ nReservIndex ].SetString( szReservGuildName );
	m_arReservState[ nReservIndex ].SetString( szTemp );
	m_arProposer[ nReservIndex ].SetString( szProposer );
}

void CGuildHunt::SetStringPos()
// 텍스트 출력 박스 좌표 설정
{
	if( !m_pBack )
		return;

	for( int i = 0 ; i < m_byReservBtn_Count ; i ++ )
	{
		m_arGuildHuntIndex[ i ].SetPos(		m_pBack->Xpos + 32,		m_pBack->Ypos + i * 33 + 165 + 4 );
		m_arGuildName[ i ].SetPos(			m_pBack->Xpos + 173,	m_pBack->Ypos + i * 33 + 165 + 4 );
		m_arProposer[ i ].SetPos(			m_pBack->Xpos + 308,	m_pBack->Ypos + i * 33 + 165 + 4 );
		m_arReservState[ i ].SetPos(		m_pBack->Xpos + 409,	m_pBack->Ypos + i * 33 + 165 + 4 );
	}

	m_GuildDate.SetPos( m_pBack->Xpos + 205, m_pBack->Ypos + 33 + 4 );
}

void CGuildHunt::SetButtonPos()
// 각 버튼 좌표 설정
{
	if( !m_pBack )
		return;

	int i = 0;
	// 페이지 버튼 좌표 설정
	for( i = 0 ; i < 3 ; i ++ )
		m_arPageBtn[ i ].SetPosition( m_pBack->Xpos + i * 80 + 26, m_pBack->Ypos + 340 );

	for( i = 0 ; i < m_byReservBtn_Count ; i ++ )
	{
		// 예약 버튼 좌표 설정
		m_arReservBtn[ i ].SetPosition( m_pBack->Xpos + 480, m_pBack->Ypos + i * 33 + 163 );
		// 에약 취소 버튼 좌표 설정
		m_arCancelBtn[ i ].SetPosition( m_pBack->Xpos + 480, m_pBack->Ypos + i * 33 + 163 );
	}

	// 확인 버튼
	m_ConfirmationBtn.SetPosition( m_pBack->Xpos + 480, m_pBack->Ypos + 340 );
	// 갱신 버튼
	m_RenewalBtn.SetPosition( m_pBack->Xpos + 400, m_pBack->Ypos + 340 );

	// x버튼
	m_CancelBtn.SetPosition( m_pBack->Xpos + 581-18, m_pBack->Ypos + 6 );
}

void CGuildHunt::InitTextOutBox()
// 텍스트 출력 박스 초기화
{
	int i;
	// 예약 정보 출력 창
	for( i = 0 ; i < m_byReservBtn_Count ; i ++ )
	{
		m_arGuildHuntIndex[ i ].Init( 12, _FCOLOR_YELLOW, 0, 0, TRUE );
		m_arGuildName[ i ].Init( 12, _FCOLOR_YELLOW, 0, 0, TRUE );
		m_arReservState[ i ].Init( 12, _FCOLOR_YELLOW, 0, 0, TRUE );
		m_arProposer[ i ].Init( 12, _FCOLOR_YELLOW, 0, 0, TRUE );
	}
	m_GuildDate.Init( 12, _FCOLOR_YELLOW, 0, 0, TRUE );
}

void CGuildHunt::SetHuntIndex()
// 길드사냥 인덱스 셋팅
{
	char szTemp[ 10 ];

	for( int i = 0 ; i < m_byReservBtn_Count ; i ++ )
	{
		sprintf( szTemp, "%d", ( m_byCurPage - 1 ) * m_byReservBtn_Count + ( i + 1 ) );
		m_arGuildHuntIndex[ i ].SetString( szTemp );
	}
}

void CGuildHunt::SetGuildDate(char *szGuildDate)
// 길드사냥 날짜 셋팅
{
	m_GuildDate.SetString( szGuildDate );
}

//=============================================
// nate 2004 - 10
char* CGuildHunt::GetGuildName()
// 현재 선택된 길드이름 반환
{
	return m_arGuildName[ m_bySelectGuildHuntIndex ].m_str;
}

char* CGuildHunt::GetProposer()
// 현재 선택된 신청자 반환
{
	return m_arProposer[ m_bySelectGuildHuntIndex ].m_str;

}
//=============================================
