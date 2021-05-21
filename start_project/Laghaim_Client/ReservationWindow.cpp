#include "stdafx.h"
// ReservationWindow.cpp: implementation of the CReservationWindow class.
//
//////////////////////////////////////////////////////////////////////

#include "headers.h"
#include "main.h"
#include "UIMgr.h"
#include "Land.h"
#include "Nk2DFrame.h"
#include "BasicButton.h"
#include "TcpIpCon.h"
#include "ReservationWindow.h"
#include "MsgPopUp.h"
#include "string_res.h"
#include "g_stringmanager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////






#define GUILDHUNT_REQUEST	1
#define GUILDHUNT_CLOSE		2

CReservationWindow::CReservationWindow()
	: m_nCristalCount(0)
	, m_nGuildHuntCost(0)
{
	m_pBack = NULL;
	m_dwLastOpenTime = timeGetTime();
	m_bIsOpenWindow = FALSE;
	m_byCurPage = 0;
	m_bySelectGuildHuntIndex = 0;
}

CReservationWindow::~CReservationWindow()
{
	DeleteRes();
}

// �ڿ�E�ε�E
void CReservationWindow::LoadRes()
{
	if( !g_pDisplay )
		return;

	g_pDisplay->CreateSurfaceFromBitmap( &m_pBack, "interface/GuildHunt/sub_win.BMP");
	m_pBack->SetColorKey( TRANS_COLOR );

	m_ReservBtn.SetFileName( "GuildHunt/btn_confirm" );
	m_CancelBtn.SetFileName( "GuildHunt/btn_hunt_cancel" );

	m_ReservBtn.LoadRes();
	m_CancelBtn.LoadRes();

	// ����E���� ��¹ڽ� �ʱ�ȭ
	InitTextOutBox();
}

// �ڿ�E����
void CReservationWindow::DeleteRes()
{
	SAFE_DELETE( m_pBack );

	m_ReservBtn.DeleteRes();
	m_CancelBtn.DeleteRes();
	m_Proposer.DeleteRes();
	m_GuildName.DeleteRes();
	m_GuildHuntDate.DeleteRes();
	m_GuildHuntTime.DeleteRes();
}

// ȭ��E���
void CReservationWindow::Draw()
{
	if( !g_pDisplay || !m_pBack )
		return;

	SetDrawPos();

	// ���� �׸���E
	g_pDisplay->Blt( m_pBack->Xpos, m_pBack->Ypos, m_pBack );

	// �ؽ�Ʈ �ڽ� �׸���E
	m_Proposer.Draw();
	m_GuildName.Draw();
	m_GuildHuntDate.Draw();
	m_GuildHuntTime.Draw();

	// ��ư �׸���E
	m_ReservBtn.Draw();
	m_CancelBtn.Draw();

	if( m_bIsOpenWindow == FALSE )
	{
		if( ( timeGetTime() - m_dwLastOpenTime ) > 300 )
		{
			m_bIsOpenWindow = TRUE;
			m_dwLastOpenTime = timeGetTime();
		}
	}
}

// ����E��?������E�ȿ� �龁E�Դ���Eüũ
BOOL CReservationWindow::IsInside( int x, int y )
{
	if( !m_pBack )
		return false;
	return m_pBack->IsIn(x, y);
}

// ���� ������E?��ư�� ��ġ ����
void CReservationWindow::SetDrawPos()
{
	m_pBack->Xpos = (g_pNk2DFrame->GetClientWidth() - m_pBack->GetWidth())>>1;
	m_pBack->Ypos = (g_pNk2DFrame->GetClientHeight() - m_pBack->GetHeight())>>1;

	// ����E���� ��� ��ǥ ����
	SetStringPos();

	// ��ư ��ǥ ����
	SetButtonPos();
}

// �޼���Eó��
LRESULT CReservationWindow::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	int x, y;
	x = LOWORD( lParam );
	y = HIWORD( lParam );

	switch( uMsg )
	{
	case WM_LBUTTONDOWN:
		if( !IsInside( x, y ) )
			return 3;

		m_ReservBtn.MsgProc( hWnd, uMsg, wParam, lParam );
		m_CancelBtn.MsgProc( hWnd, uMsg, wParam, lParam );

		return 1;

	case WM_LBUTTONUP:
		if( !IsInside( x, y ) )
			if( !pCMyApp->m_pUIMgr->m_bIsGuildHunt )
				return 3;

		m_ReservBtn.MsgProc( hWnd, uMsg, wParam, lParam );
		if( m_ReservBtn.m_iBtnFlg == BTN_ACTION )
		{
			m_ReservBtn.m_iBtnFlg = BTN_NORMAL;
			SendQuestApplyMsg( GUILDHUNT_REQUEST );
			return 2;
		}

		m_CancelBtn.MsgProc( hWnd, uMsg, wParam, lParam );
		if( m_CancelBtn.m_iBtnFlg == BTN_ACTION )
		{
			m_CancelBtn.m_iBtnFlg = BTN_NORMAL;
			SendQuestApplyMsg( GUILDHUNT_CLOSE );
			return 2;
		}
		break;

	case WM_MOUSEMOVE:
		return 1;
	case WM_RBUTTONDOWN:
		return 1;
	}

	return 0;
}

void CReservationWindow::SendQuestApplyMsg( BYTE byMessageType )
// ������ �޼���E��E?
{
	if( !g_pTcpIp )
		return;

	char szMsg[ 50 ];

	if( byMessageType == GUILDHUNT_REQUEST )
	{
		// [2009/3/23 Theodoric] ī���� �� �翁E���θ� ����ָ��E������ ��Ŷ�� ��E����?
		// IDS_EVOL_USE_NOT_CANCEL

		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_EVOL_USE_NOT_CANCEL), TYPE_NOR_OKCANCLE, POPUP_EVOLUTIONBALL_USE);

		CMsgPopUp * pPopup = g_pNk2DFrame->FindPopup( POPUP_EVOLUTIONBALL_USE );
		if( pPopup )
		{
			sprintf( pPopup->m_InputValue1.GetStrBuf() , "%d", m_byCurPage);
			sprintf( pPopup->m_InputValue2.GetStrBuf() , "%d", m_bySelectGuildHuntIndex);
		}
		pPopup = NULL;
		return;
	}
	else if( byMessageType == GUILDHUNT_CLOSE )
	{
		sprintf( szMsg, "gh close %d %d\n", m_byCurPage, m_bySelectGuildHuntIndex );
	}

	g_pTcpIp->SendNetMessage( szMsg );
}

void CReservationWindow::SetStringPos()
// �ؽ�Ʈ �ڽ� ��ǥ ����
{
	m_GuildHuntDate.SetPos( m_pBack->Xpos + 108, m_pBack->Ypos + 44 );
	m_GuildHuntTime.SetPos( m_pBack->Xpos + 108, m_pBack->Ypos + 72 );
	m_GuildName.SetPos( m_pBack->Xpos + 108, m_pBack->Ypos + 99 );
	m_Proposer.SetPos( m_pBack->Xpos + 108, m_pBack->Ypos + 127 );
}

void CReservationWindow::SetButtonPos()
// ��ư ��ǥ ����
{
	// �����ư ��ǥ ����
	m_ReservBtn.SetPosition( m_pBack->Xpos + 50, m_pBack->Ypos + 180 );

	//  ÁE?��ư ��ǥ ����
	m_CancelBtn.SetPosition( m_pBack->Xpos + 140, m_pBack->Ypos + 180 );
}

void CReservationWindow::InitTextOutBox()
// �ؽ�Ʈ �ڽ� �ʱ�ȭ
{
	m_Proposer.Init( 12, _FCOLOR_YELLOW, 0, 0 );
	m_GuildName.Init( 12, _FCOLOR_YELLOW, 0, 0 );
	m_GuildHuntDate.Init( 12, _FCOLOR_YELLOW, 0, 0 );
	m_GuildHuntTime.Init( 12, _FCOLOR_YELLOW, 0, 0 );
}

void CReservationWindow::SetReservInfo( BYTE byCurPage
										, BYTE bySelectGuildHuntIndex
										, char* szGuildHuntDate
										, char* szGuildName
										, char* szProposer )
{
	m_byCurPage = byCurPage;
	m_bySelectGuildHuntIndex = bySelectGuildHuntIndex;
	m_GuildName.SetString( szGuildName );
	m_GuildHuntDate.SetString( szGuildHuntDate );
	m_Proposer.SetString( szProposer );

	if(pCMyApp->m_nCurWorldIdx == WORLD_DMITRON)	// ���Ʈ�� ����Ʈ�� ��ƶ�E�̺�Ʈ
	{
		switch( bySelectGuildHuntIndex )
		{
		case 1:
			m_GuildHuntTime.SetString( "19:00~19:50" );
			break;
		case 2:
			m_GuildHuntTime.SetString( "20:00~20:50" );
			break;
		case 3:
			m_GuildHuntTime.SetString( "21:00~21:50" );
			break;
		default:
			m_GuildHuntTime.SetString( "Time Error" );
			break;
		}
	}
	else	// ���Ϸ� ������� ��ī���
	{
		switch( bySelectGuildHuntIndex )
		{
		case 1:
			m_GuildHuntTime.SetString( "00:00~00:50" );
			break;
		case 2:
			m_GuildHuntTime.SetString( "20:00~20:50" );
			break;
		case 3:
			m_GuildHuntTime.SetString( "21:00~21:50" );
			break;
		case 4:
			m_GuildHuntTime.SetString( "22:00~22:50" );
			break;
		case 5:
			m_GuildHuntTime.SetString( "23:00~23:50" );
			break;
		default:
			m_GuildHuntTime.SetString( "Time Error" );
			break;
		}
	}
}