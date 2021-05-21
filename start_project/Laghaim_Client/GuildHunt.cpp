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

// �ڿ� �ε�
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

	// �����̹��� �ε�
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

	// ������ ��ư
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
		// ���� ��ư
		m_arReservBtn[ i ].SetFileName( "GuildHunt/btn_booking" );
		m_arReservBtn[ i ].LoadRes();

		// ���� ��� ��ư
		m_arCancelBtn[ i ].SetFileName( "GuildHunt/btn_hunt_cancel" );
		m_arCancelBtn[ i ].LoadRes();
	}

	// x��ư
	m_CancelBtn.SetFileName("common/btn_close_01");
	m_CancelBtn.LoadRes();

	// Ȯ�� ��ư
	m_ConfirmationBtn.SetFileName( "GuildHunt/btn_confirm" );
	m_ConfirmationBtn.LoadRes();

	// ���� ��ư
	if(pCMyApp->m_nCurWorldIdx == WORLD_DMITRON)
		m_RenewalBtn.SetFileName( "GuildHunt/Entrance_btn" );
	else
		m_RenewalBtn.SetFileName( "GuildHunt/btn_renewal" );
	m_RenewalBtn.LoadRes();

	// ���� ���� ��¹ڽ� �ʱ�ȭ
	InitTextOutBox();
}

// �ڿ� ����
void CGuildHunt::DeleteRes()
{
	int i = 0;

	SAFE_DELETE( m_pBack );

	// ������ ��ư ����
	for( i = 0 ; i < 3 ; i ++ )
		m_arPageBtn[ i ].DeleteRes();

	for( i = 0 ; i < m_byReservBtn_Count ; i ++ )
	{
		// �ؽ�Ʈ �ڽ� �ڿ�����
		m_arGuildHuntIndex[ i ].DeleteRes();
		m_arGuildName[ i ].DeleteRes();
		m_arReservState[ i ].DeleteRes();
		m_arProposer[ i ].DeleteRes();
		// ���� ��ư ����
		m_arReservBtn[ i ].DeleteRes();
		// ��ҹ�ư ����
		m_arCancelBtn[ i ].DeleteRes();
	}
	// ��¥ �ڽ� ����
	m_GuildDate.DeleteRes();
	// Ȯ�� ��ư ����
	m_ConfirmationBtn.DeleteRes();
	// ���� ��ư ����
	m_RenewalBtn.DeleteRes();

	// X��ư
	m_CancelBtn.DeleteRes();
}

// ȭ�� ���
void CGuildHunt::Draw()
{
	if( !g_pDisplay || !m_pBack )
		return;

	int i  = 0;

	SetDrawPos();

	// ���� �׸���
	g_pDisplay->Blt( m_pBack->Xpos, m_pBack->Ypos, m_pBack );

	// ��¥ �ڽ� �׸���
	m_GuildDate.Draw(170);

	for( i  = 0 ; i < m_byReservBtn_Count ; i ++ )
	{
		// �ؽ�Ʈ �׸���
		m_arGuildHuntIndex[ i ].Draw(37);
		m_arGuildName[ i ].Draw(128);
		m_arProposer[ i ].Draw(98);
		m_arReservState[ i ].Draw(64);

		if( m_byReserved[ i ] != 5 )
		{
			if( m_byReserved[ i ] == 2 )
				// ���� ��ҹ�ư �׸���
				m_arCancelBtn[ i ].Draw();
			else if( m_byReserved[ i ] == 0 )
			{
				// �����ư �׸���
				if( ( pCMyApp->m_nCurWorldIdx == WORLD_GUILD
						|| pCMyApp->m_nCurWorldIdx == WORLD_WHITEHORN )
						&& !(m_byCurPage == 1) )
					m_arReservBtn[ i ].Draw();
			}
		}
	}

	//  Ȯ�� ��ư �׸���
	m_ConfirmationBtn.Draw();

	// ���Ź�ư �׸���
	m_RenewalBtn.Draw();

	// X��ư
	m_CancelBtn.Draw();

	// ������ ��ư �׸���
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

// ���콺�� ������ �ȿ� ��� �Դ��� üũ
BOOL CGuildHunt::IsInside( int x, int y )
{
	if( !m_pBack )
		return FALSE;

	return m_pBack->IsIn( x, y );
}

// ���� ������� ��ư�� ��ġ ����
// �� ������ ���� �ؾ� ��� �ǳ�..�̻�Ÿ
void CGuildHunt::SetDrawPos()
{
	if( !g_pNk2DFrame || !m_pBack )
		return;

	int i = 0;
	// �������̽� �����찡 ���� �������� üũ
	if( g_pNk2DFrame->IsHelpVisible()			|| g_pNk2DFrame->IsInvenVisible()
			|| g_pNk2DFrame->IsPartyVisible()		|| g_pNk2DFrame->IsInterfaceVisible(SCORE)
			|| g_pNk2DFrame->IsSkillVisible()		|| g_pNk2DFrame->IsInterfaceVisible(MAP)
			|| g_pNk2DFrame->IsPetStatusVisible()	|| g_pNk2DFrame->IsInterfaceVisible(QUEST) )
		m_pBack->Xpos = abs( m_nScreenWidth - (int)m_pBack->GetWidth() - 274 )  / 2;	// 274�� �������̽� ũ��
	else
		m_pBack->Xpos = abs( m_nScreenWidth - (int)m_pBack->GetWidth() ) / 2;

	m_pBack->Ypos = abs( m_nScreenHeight - (int)m_pBack->GetHeight() ) / 2 - 15;

	// ���� ���� ��� ��ǥ ����
	SetStringPos();
	// ��ư ��ǥ ����
	SetButtonPos();
}

// �޼��� ó��
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

		// ������ ��ư �޼��� ó��
		for( i = 0 ; i < 3 ; i ++ )
			m_arPageBtn[ i ].MsgProc( hWnd, uMsg, wParam, lParam );

		for( i = 0 ; i < m_byReservBtn_Count ; i ++ )
		{
			if( m_byReserved[ i ] != 5 )
			{
				if( m_byReserved[ i ] == 2 )
					// ���� ��� ��ư
					m_arCancelBtn[ i ].MsgProc( hWnd, uMsg, wParam, lParam );
				else if( m_byReserved[ i ] == 0 )
				{
					// �����ư
					if( ( pCMyApp->m_nCurWorldIdx == WORLD_GUILD
							|| pCMyApp->m_nCurWorldIdx == WORLD_WHITEHORN )
							&& !(m_byCurPage == 1) )
						m_arReservBtn[ i ].MsgProc( hWnd, uMsg, wParam, lParam );
				}
			}
		}

		// Ȯ�� ��ư
		m_ConfirmationBtn.MsgProc( hWnd, uMsg, wParam, lParam );
		// ���� ��ư
		m_RenewalBtn.MsgProc( hWnd, uMsg, wParam, lParam );
		// x��ư
		m_CancelBtn.MsgProc( hWnd, uMsg, wParam, lParam );

		return 1;

	case WM_LBUTTONUP:
		if( !IsInside( x, y ) )
			if( !pCMyApp->m_pUIMgr->m_bIsGuildHunt )
				return 2;

		// x��ư
		m_CancelBtn.MsgProc( hWnd, uMsg, wParam, lParam );
		if( m_CancelBtn.m_iBtnFlg == BTN_ACTION )
		{
			m_CancelBtn.m_iBtnFlg = BTN_NORMAL;
			return 2;
		}

		// Ȯ�� ��ư
		m_ConfirmationBtn.MsgProc( hWnd, uMsg, wParam, lParam );
		if( m_ConfirmationBtn.m_iBtnFlg == BTN_ACTION )
		{
			m_ConfirmationBtn.m_iBtnFlg = BTN_NORMAL;
			return 2;
		}

		// ���� ��ư
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

		// ������ ��ư ó��
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
				// ���� ��� ��ư ó��
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
				// �����ư ó��
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
								// nate 2005-02-18 - IDS_NOGUILD : �Ҽӵ� ��尡 �����ϴ�.
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
// ��� ��� ���� Ŭ���̾�Ʈ �޼��� ó��
{
	if( !g_pTcpIp )
		return;

	char szMsg[ 50 ] = "";

	// ������ ��û
	if( nApplyIndex == GUILDHUNT_OPEN )
	{
		sprintf( szMsg, "gh open %d\n", m_byCurPage );
	}
	// ������ ���ɱ�
	// ���� �ε����� ������ �����Ұ��� ��û
	else if( nApplyIndex == GUILDHUNT_REQUEST )
	{
		sprintf( szMsg, "gh req %d 0 0 0 %d\n", m_byCurPage, m_bySelectGuildHuntIndex );
	}
	// ���� ���
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
// ������ ���� ���� ���� ������ �ʹ� ������ ����
{
	// ���� ���� üũ
	m_byReserved[ nReservIndex ] = nReservState;

	// ���� �÷��� ����ȭ
	char szTemp[ 30 ] = { "\0", };

	switch( nReservState )
	{
	case 0:
		//-- IDS_GUH_DONT_FIX	: ����
		strcpy( szTemp, (char*)G_STRING(IDS_GUH_DONT_FIX));
		break;
	case 1:
		//-- IDS_GUH_RESRV_DOING : ������..
		strcpy( szTemp, (char*)G_STRING(IDS_GUH_RESRV_DOING) );
		break;
	case 2:
		//-- IDS_GUH_RESRV_COMP	: ���� �Ϸ�
		strcpy( szTemp,(char*)G_STRING(IDS_GUH_RESRV_COMP) );
		break;
	case 3:
		//-- IDS_GUH_GAME_DOING	: �����
		strcpy( szTemp, (char*)G_STRING(IDS_GUH_GAME_DOING));
		break;
	case 4:
		//-- IDS_GUH_GAME_COMP	: �������
		strcpy( szTemp, (char*)G_STRING(IDS_GUH_GAME_COMP));
		break;
	case 5:
		//-- IDS_GUH_CANT_APP	:���� �Ұ�
		strcpy( szTemp, (char*)G_STRING(IDS_GUH_CANT_APP));
		break;
	}

	// ����� �ε��� ����
	SetHuntIndex();

	m_arGuildName[ nReservIndex ].SetString( szReservGuildName );
	m_arReservState[ nReservIndex ].SetString( szTemp );
	m_arProposer[ nReservIndex ].SetString( szProposer );
}

void CGuildHunt::SetStringPos()
// �ؽ�Ʈ ��� �ڽ� ��ǥ ����
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
// �� ��ư ��ǥ ����
{
	if( !m_pBack )
		return;

	int i = 0;
	// ������ ��ư ��ǥ ����
	for( i = 0 ; i < 3 ; i ++ )
		m_arPageBtn[ i ].SetPosition( m_pBack->Xpos + i * 80 + 26, m_pBack->Ypos + 340 );

	for( i = 0 ; i < m_byReservBtn_Count ; i ++ )
	{
		// ���� ��ư ��ǥ ����
		m_arReservBtn[ i ].SetPosition( m_pBack->Xpos + 480, m_pBack->Ypos + i * 33 + 163 );
		// ���� ��� ��ư ��ǥ ����
		m_arCancelBtn[ i ].SetPosition( m_pBack->Xpos + 480, m_pBack->Ypos + i * 33 + 163 );
	}

	// Ȯ�� ��ư
	m_ConfirmationBtn.SetPosition( m_pBack->Xpos + 480, m_pBack->Ypos + 340 );
	// ���� ��ư
	m_RenewalBtn.SetPosition( m_pBack->Xpos + 400, m_pBack->Ypos + 340 );

	// x��ư
	m_CancelBtn.SetPosition( m_pBack->Xpos + 581-18, m_pBack->Ypos + 6 );
}

void CGuildHunt::InitTextOutBox()
// �ؽ�Ʈ ��� �ڽ� �ʱ�ȭ
{
	int i;
	// ���� ���� ��� â
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
// ����� �ε��� ����
{
	char szTemp[ 10 ];

	for( int i = 0 ; i < m_byReservBtn_Count ; i ++ )
	{
		sprintf( szTemp, "%d", ( m_byCurPage - 1 ) * m_byReservBtn_Count + ( i + 1 ) );
		m_arGuildHuntIndex[ i ].SetString( szTemp );
	}
}

void CGuildHunt::SetGuildDate(char *szGuildDate)
// ����� ��¥ ����
{
	m_GuildDate.SetString( szGuildDate );
}

//=============================================
// nate 2004 - 10
char* CGuildHunt::GetGuildName()
// ���� ���õ� ����̸� ��ȯ
{
	return m_arGuildName[ m_bySelectGuildHuntIndex ].m_str;
}

char* CGuildHunt::GetProposer()
// ���� ���õ� ��û�� ��ȯ
{
	return m_arProposer[ m_bySelectGuildHuntIndex ].m_str;

}
//=============================================
