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

#include "DmitronBattle.h"
#include "window_resource.h"

#include <time.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
extern char				commOutBuf[];

#define GUILDHUNT_OPEN		1
#define GUILDHUNT_REQUEST	2
#define GUILDHUNT_CANCEL	3
#define EVENT_MOVE			4

//////////////////////////////////////////////////////////////////////
//  ���Ʈ�� �� ����
//////////////////////////////////////////////////////////////////////
char* CDmitronString::Crystal(int type)
{
	switch(type)
	{
	case 0:
		return (char*)G_STRING(IDS_DMITRON_BATTLE_CRYSTAL1);//"�ı��� ũ����Ż1";
	case 1:
		return (char*)G_STRING(IDS_DMITRON_BATTLE_CRYSTAL2);//"��ȣ�� ũ����Ż1";
	case 2:
		return (char*)G_STRING(IDS_DMITRON_BATTLE_CRYSTAL3);//"�Ǵ��� ũ����Ż1";
	case 3:
		return (char*)G_STRING(IDS_DMITRON_BATTLE_CRYSTAL4);//"����� ũ����Ż1";
	case 4:
		return (char*)G_STRING(IDS_DMITRON_BATTLE_CRYSTAL5);//"�ı��� ũ����Ż2";
	case 5:
		return (char*)G_STRING(IDS_DMITRON_BATTLE_CRYSTAL6);//"��ȣ�� ũ����Ż2";
	case 6:
		return (char*)G_STRING(IDS_DMITRON_BATTLE_CRYSTAL7);//"�Ǵ��� ũ����Ż2";
	case 7:
		return (char*)G_STRING(IDS_DMITRON_BATTLE_CRYSTAL8);//"����� ũ����Ż2";
	}
	return 0;
}

char* CDmitronString::Status(int type)
{
	switch(type)
	{
	case 0:
		return (char*)G_STRING(IDS_DMITRON_BATTLE_STATUS1);//"���� �Ұ�";
	case 1:
		return (char*)G_STRING(IDS_DMITRON_BATTLE_STATUS2);//"ħ�� ����";
	case 2:
		return (char*)G_STRING(IDS_DMITRON_BATTLE_STATUS3);//"ħ���� ��";
	case 3:
		return (char*)G_STRING(IDS_DMITRON_BATTLE_STATUS3);//"ħ���� ��";
	}
	return 0;
}

char* CDmitronString::Effect(int type)
{
	switch(type)
	{
	case 0:
		return (char*)G_STRING(IDS_DMITRON_BATTLE_EFFECT1);//"���ݷ� 10% ���";
	case 1:
		return (char*)G_STRING(IDS_DMITRON_BATTLE_EFFECT2);//"���� 10% ���";
	case 2:
		return (char*)G_STRING(IDS_DMITRON_BATTLE_EFFECT3);//"����� 20% ���";
	case 3:
		return (char*)G_STRING(IDS_DMITRON_BATTLE_EFFECT4);//"����� 15% ���";
	}
	return 0;
}

char* CDmitronString::Portal(int type)
{
	switch(type)
	{
	case 0:
		return (char*)G_STRING(IDS_DMITRON_BATTLE_ZONE1);//"�׸���";
	case 1:
		return (char*)G_STRING(IDS_DMITRON_BATTLE_ZONE2);//"���� ����";
	case 2:
		return (char*)G_STRING(IDS_DMITRON_BATTLE_ZONE3);//"����� ����";
	case 3:
		return (char*)G_STRING(IDS_DMITRON_BATTLE_ZONE4);//"������ ����";
	case 4:
		return (char*)G_STRING(IDS_DMITRON_BATTLE_ZONE5);//"���׸��� ��";
	case 5:
		return (char*)G_STRING(IDS_DMITRON_BATTLE_ZONE6);//"���Ʈ�� �ֵ���";
	}
	return 0;
}

char* CDmitronString::Battle(int type)
{
	switch(type)
	{
	case 0:
		return (char*)G_STRING(IDS_DMITRON_BATTLE_BATTLE1);//"����",
	case 1:
		return (char*)G_STRING(IDS_DMITRON_BATTLE_BATTLE2);//"������",
	case 2:
		return (char*)G_STRING(IDS_DMITRON_BATTLE_BATTLE3);//"������",
	}
	return 0;
}

char* CDmitronString::Zone(int type)
{
	switch(type)
	{
	case 0:
		return (char*)G_STRING(IDS_DMITRON_BATTLE_ZONE1);//"�׸���";
	case 1:
		return (char*)G_STRING(IDS_DMITRON_BATTLE_ZONE2);//"���� ����";
	case 2:
		return (char*)G_STRING(IDS_DMITRON_BATTLE_ZONE3);//"����� ����";
	case 3:
		return (char*)G_STRING(IDS_DMITRON_BATTLE_ZONE4);//"������ ����";
	case 4:
		return (char*)G_STRING(IDS_DMITRON_BATTLE_ZONE5);//"���׸��� ��";
	}
	return 0;
}

char* CDmitronInfo::Zone::GetBuff1()
{
	return CDmitronString::Crystal(Buff1);
}

char* CDmitronInfo::Zone::GetBuff2()
{
	return CDmitronString::Crystal(Buff2);
}

int CDmitronZone::SearchPortal(int x, int z) //���� ��Ż �̵�
{
	if (pCMyApp->m_nCurWorldIdx == WORLD_DMITRON)
	{
		if ( x == 9467 && z == 2253) return 0;
		else if ( x == 2864 && z == 4507) return 1;
		else if ( x == 6749 && z == 7927) return 2;
		else if ( x == 5504 && z == 9784) return 3;
		else if ( x == 2083 && z == 10631) return 4;
		else if ( ( x == 8020 && z == 1451) ||
				  ( x == 8876 && z == 11945) ) return 5; // ��Ÿ : �׸��� -> ����� ����, ����� ���� -> ����
	}
	else if(pCMyApp->m_nCurWorldIdx == WORLD_DMITRON_BATTLE)
	{
		if ( x == 9676 && z == 4105) return 0;
		else if ( x == 3416 && z == 3003) return 1;
		else if ( x == 7853 && z == 8849) return 2;
		else if ( x == 4708 && z == 8570) return 3;
		else if ( x == 2074 && z == 10633) return 4;
	}
	return -1;
}

int CDmitronZone::SearchPortalManager(int x, int z) //��Ż �Ŵ���
{
	if ( x == 9532 && z == 2308) return 0;
	else if ( x == 2868 && z == 4410) return 1;
	else if ( x == 6805 && z == 7849) return 2;
	else if ( x == 5495 && z == 9654) return 3;
	else if ( x == 2146 && z == 10696) return 4;
	return -1;
}

int CDmitronZone::SearchGateStone(int x, int z) // �̵� ����Ʈ, ũ����Ż, ����Ʈ ����
{
	if ( x == 9716 && z == 3304) return 0;
	else if ( x == 3970 && z == 3592) return 1;
	else if ( x == 8320 && z == 8394) return 2;
	else if ( x == 3747 && z == 8303) return 3;
	else if ( x == 2334 && z == 10319) return 4;
	return -1;
}

//////////////////////////////////////////////////////////////////////
// ���Ʈ�� ����
//////////////////////////////////////////////////////////////////////
void CDmitronInfo::SetZoneInfo1(int nIndex, char* guildName, int status, int buff1, int buff2)
{
	strcpy(g_Zone[nIndex].Guild, guildName);
	g_Zone[nIndex].Status = status;
	g_Zone[nIndex].Buff1 = buff1;
	g_Zone[nIndex].Buff2 = buff2;
}

void CDmitronInfo::SetZoneInfo2(int nIndex, char* guildName, int status, int stage)
{
	strcpy(g_Zone[nIndex].Guild, guildName);
	g_Zone[nIndex].Status = status;
	g_Zone[nIndex].Stage = stage;
}

CDmitronInfo::Info CDmitronInfo::g_Info = { {0, }, {0, }, {0, }, {0, } };

CDmitronInfo::Lock CDmitronInfo::g_Lock = { -1, FALSE };

BOOL CDmitronInfo::g_Buff[DMITRON_BATTLE_BUFF] = { FALSE, FALSE, FALSE, FALSE };

int  CDmitronInfo::Selection()
{
	return g_Lock.Index;
}

BOOL CDmitronInfo::IsSelected()
{
	return g_Lock.Flag;
}

void CDmitronInfo::SelectZone(int n)
{
	g_Lock.Flag = TRUE;
	g_Lock.Index = n;
}

void CDmitronInfo::DeselectZone()
{
	g_Lock.Flag = FALSE;
	g_Lock.Index = -1;
}

void CDmitronInfo::UpdateZone(BOOL succ)
{
	if (succ == TRUE)
	{
		switch(g_Zone[g_Lock.Index].Stage)
		{
		case DMITRON_BATTLE_READY:
		case DMITRON_BATTLE_WAITING:
		case DMITRON_BATTLE_RUNNING:
			g_Zone[g_Lock.Index].Stage++;
			break;

		case DMITRON_BATTLE_CANCELING:
			break;
		}
	}
	else
	{
		switch(g_Zone[g_Lock.Index].Stage)
		{
		case DMITRON_BATTLE_WAITING:
			g_Zone[g_Lock.Index].Stage--;
			break;

		case DMITRON_BATTLE_READY:
		case DMITRON_BATTLE_RUNNING:
		case DMITRON_BATTLE_CANCELING:
			break;
		}
	}

	g_Lock.Flag = FALSE;
	g_Lock.Index = -1;
}

//���� ���� �޽���
BOOL CDmitronInfo::GetBuff(int n)
{
	return g_Buff[n];
}

void CDmitronInfo::SetBuff(int nIndex, BOOL nState)
{
	g_Buff[nIndex] = nState;
}

CDmitronInfo::Zone CDmitronInfo::g_Zone[DMITRON_BATTLE_ZONE] =
{
	{ {0, }, -1, -1 },
	{ {0, }, -1, -1 },
	{ {0, }, -1, -1 },
	{ {0, }, -1, -1 },
	{ {0, }, -1, -1 },
};

///////////////////////////////////////////////////////////////////////////////

// ȭ�� ���
int CDmitronBattle::Draw()
{
	if( g_pDisplay && m_pBackScene)
	{
		UpdateLayout();

		g_pDisplay->Blt(m_pBackScene->Xpos, m_pBackScene->Ypos, m_pBackScene);
		{
			for( int i  = 0 ; i < DMITRON_BATTLE_ZONE ; i ++ )
			{
				// �ؽ�Ʈ �׸���
				m_arGuildHuntIndex[i].Draw(114);
				m_arGuildName[i].Draw(151);
				m_arStatus[i].Draw(96);

				switch(CDmitronInfo::GetZone(i).Stage)
				{
				case 0:
					//��û + ����
					m_arRequestBtn[i].Draw();
					m_arInformBtn[i].Draw();
					break;
				case 1:
					//��û + ����
					m_arRequestBtn[i].Draw();
					m_arInformBtn[i].Draw();
					break;
				case 2:
					//�ƹ��͵� ������� �ʴ´�.
					break;
				case 3:
					m_arRequestBtn[i].Draw();
					m_arInformBtn[i].Draw();
					break;
				case 4:
					m_arJoinBtn[i].Draw();
					break;
				}
			}

			//  Ȯ�� ��ư �׸���
			m_ConfirmationBtn.Draw();
			m_RenewalBtn.Draw();
			m_btn_close.Draw();
		}
	}
	return -1;
}

// �޼��� ó��
LRESULT CDmitronBattle::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if(!IsInside(LOWORD(lParam), HIWORD(lParam)))
		return IM_NONE;

	for( int i = 0 ; i < DMITRON_BATTLE_ZONE ; i ++ )
	{
		if (CDmitronInfo::GetZone(i).Stage == 4)
		{
			if (m_arJoinBtn[i].IsDisable() == FALSE)
			{
				m_arJoinBtn[i].MsgProc( hWnd, uMsg, wParam, lParam );
			}
		}
		else
		{
			if (m_arRequestBtn[i].IsDisable() == FALSE)
			{
				m_arRequestBtn[ i ].MsgProc( hWnd, uMsg, wParam, lParam );
			}

			if (m_arInformBtn[i].IsDisable() == FALSE)
			{
				m_arInformBtn[i].MsgProc( hWnd, uMsg, wParam, lParam );
			}
		}
	}
	// Ȯ�� / ���� ��ư
	if (m_ConfirmationBtn.IsDisable() == FALSE)
		m_ConfirmationBtn.MsgProc( hWnd, uMsg, wParam, lParam );
	if (m_btn_close.IsDisable() == FALSE)
		m_btn_close.MsgProc( hWnd, uMsg, wParam, lParam );

	if (m_RenewalBtn.IsDisable() == FALSE)
		m_RenewalBtn.MsgProc( hWnd, uMsg, wParam, lParam );

	switch( uMsg )
	{
	case WM_LBUTTONDOWN:
		return IM_INPUT;

	case WM_LBUTTONUP:
		// Ȯ�� ��ư
		if( m_btn_close.m_iBtnFlg == BTN_ACTION )
		{
			m_btn_close.m_iBtnFlg = BTN_NORMAL;
			return IM_CLOSE;
		}
		if( m_ConfirmationBtn.m_iBtnFlg == BTN_ACTION )
		{
			m_ConfirmationBtn.m_iBtnFlg = BTN_NORMAL;
			return IM_CLOSE;
		}

		// ���� ��ư
		if( m_RenewalBtn.m_iBtnFlg == BTN_ACTION )
		{
			m_RenewalBtn.m_iBtnFlg = BTN_NORMAL;
			if (g_pTcpIp)
				g_pTcpIp->SendNetMessage("occp bt open\n");
		}
		else
		{
			m_RenewalBtn.m_iBtnFlg = BTN_NORMAL;
		}

		for( int i = 0 ; i < DMITRON_BATTLE_ZONE ; i ++ )
		{
			switch( CDmitronInfo::GetZone(i).Stage )
			{
			case DMITRON_BATTLE_READY:
				if( m_arInformBtn[i].m_iBtnFlg == BTN_ACTION )
				{
					CDmitronInfo::SelectZone(i);
					sprintf(commOutBuf, "occp bt info %d\n", i);
					if(g_pTcpIp)
					{
						g_pTcpIp->SendNetMessage(commOutBuf);
					}
					m_arInformBtn[i].m_iBtnFlg = BTN_NORMAL;
					return IM_INPUT;
				}

				if( m_arRequestBtn[i].m_iBtnFlg == BTN_ACTION )
				{
					if( g_pRoh->m_pstrGuildName )
					{
						g_pNk2DFrame->InsertPopup((char *)G_STRING(IDS_DMITRON_BATTLE_REQUEST),
												  TYPE_NOR_OKCANCLE, POPUP_DMITRON_BATTLE_REQUEST, 30, 0, (void*)i);
						return IM_INPUT;
					}
					else
					{
						// nate 2005-02-18 - IDS_NOGUILD : �Ҽӵ� ��尡 �����ϴ�.
						g_pNk2DFrame->InsertPopup((char *)G_STRING(IDS_NOGUILD),TYPE_NOR_OK, 1);
						return 1;
					}
				}
				break;
			case DMITRON_BATTLE_WAITING:
				if( m_arInformBtn[i].m_iBtnFlg == BTN_ACTION )
				{
					CDmitronInfo::SelectZone(i);
					if(g_pTcpIp)
					{
						sprintf(commOutBuf, "occp bt info %d\n", i);
						g_pTcpIp->SendNetMessage(commOutBuf);
					}
					m_arInformBtn[i].m_iBtnFlg = BTN_NORMAL;
					return IM_INPUT;
				}

				if( m_arInformBtn[i].m_iBtnFlg == BTN_ACTION )
				{
					if( g_pRoh->m_pstrGuildName )
					{
						g_pNk2DFrame->InsertPopup((char *)G_STRING(IDS_DMITRON_BATTLE_REQUEST),
												  TYPE_NOR_OKCANCLE, POPUP_DMITRON_BATTLE_REQUEST, 30, 0, (void*)i);
						return IM_INPUT;
					}
					else
					{
						// nate 2005-02-18 - IDS_NOGUILD : �Ҽӵ� ��尡 �����ϴ�.
						g_pNk2DFrame->InsertPopup((char *)G_STRING(IDS_NOGUILD),TYPE_NOR_OK, 1);
						return 1;
					}
				}
				break;
			case DMITRON_BATTLE_RUNNING:
				break;
			case DMITRON_BATTLE_CANCELING:
				break;
			case DMITRON_BATTLE_OWNING:
				if (m_arJoinBtn[i].m_iBtnFlg == BTN_ACTION)
				{
					pCMyApp->m_pUIMgr->PrepareChangeWorld(TRUE, WORLD_DMITRON_BATTLE, i+1);
				}
				break;
			}
		}
		break;
	}
	return CUIBaseWindow::MsgProc(hWnd, uMsg, wParam, lParam);
}

void CDmitronBattle::InitTextOutBox()
// �ؽ�Ʈ ��� �ڽ� �ʱ�ȭ
{
	int i;
	// ���� ���� ��� â
	for( i = 0 ; i < DMITRON_BATTLE_ZONE  ; i ++ )
	{
		m_arGuildHuntIndex[ i ].Init( 12, _FCOLOR_YELLOW, 0, 0, TRUE );
		m_arGuildName[ i ].Init( 12, _FCOLOR_YELLOW, 0, 0, TRUE );
		m_arStatus[ i ].Init( 12, _FCOLOR_YELLOW, 0, 0, TRUE );

		m_arGuildHuntIndex[ i ].SetString(CDmitronString::Zone(i));
		m_arGuildName[ i ].SetString(CDmitronInfo::GetZone(i).Guild);
		m_arStatus[ i ].SetString( CDmitronString::Status(CDmitronInfo::GetZone(i).Status) );
	}
}

//=============================================================================


CDmitronBattle::CDmitronBattle() : CUIBaseWindow()
{
}

CDmitronBattle::~CDmitronBattle()
{
	DeleteRes();
}

BOOL CDmitronBattle::InitMode()
{
	InitTextOutBox();
	return TRUE;
}

void CDmitronBattle::LoadRes()
{
	SetBackground(WBK_DMITRON_MENU);

	for( int i = 0 ; i < DMITRON_BATTLE_ZONE ; i ++ )
	{
		m_arRequestBtn[i].SetFileName( "GuildSiege/Request_btn" );
		m_arRequestBtn[i].LoadRes();

		m_arInformBtn[i].SetFileName( "GuildSiege/info_btn" );
		m_arInformBtn[i].LoadRes();

		m_arJoinBtn[i].SetFileName( "GuildSiege/Move_ok_btn" );
		m_arJoinBtn[i].LoadRes();
	}

	// Ȯ�� ��ư : (������ ���)
	m_ConfirmationBtn.SetFileName( "GuildHunt/btn_confirm" );
	m_ConfirmationBtn.LoadRes();

	// ���� ��ư : (������ ���)
	m_RenewalBtn.SetFileName( "GuildHunt/btn_renewal" );
	m_RenewalBtn.LoadRes();


	// by evilkiki 2009.12.08 �ݱ��ư �߰�
	m_btn_close.SetFileName( "common/btn_close_01" );
	m_btn_close.LoadRes();

	UpdateLayout();
}


void CDmitronBattle::DeleteRes()
{
	int i = 0;
	for( i = 0 ; i < DMITRON_BATTLE_ZONE ; i ++ )
	{
		// �ؽ�Ʈ �ڽ� �ڿ�����
		m_arGuildHuntIndex[ i ].DeleteRes();
		m_arGuildName[ i ].DeleteRes();
		m_arStatus[ i ].DeleteRes();

		m_arRequestBtn[i].DeleteRes();
		m_arInformBtn[i].DeleteRes();
		m_arJoinBtn[i].DeleteRes();
	}

	// Ȯ�� ��ư ����
	m_ConfirmationBtn.DeleteRes();
	// ���� ��ư ����
	m_RenewalBtn.DeleteRes();

	m_btn_close.DeleteRes();
}

void CDmitronBattle::RestoreSurfaces()
{
}

BOOL CDmitronBattle::MoveWindow(UINT msg, int x, int y)
{
	return CUIBaseWindow::MoveWindow(msg, x, y);
}

BOOL CDmitronBattle::IsInside(int x, int y)
{
	return CUIBaseWindow::IsInside(x, y);
}

void CDmitronBattle::UpdateState()
{
	for( int i  = 0 ; i < DMITRON_BATTLE_ZONE ; i ++ )
	{
		switch(CDmitronInfo::GetZone(i).Stage	)
		{
		case 0:
			m_arRequestBtn[i].SetDisable2(FALSE);
			m_arInformBtn[i].SetDisable2(TRUE);
			m_arJoinBtn[i].SetDisable2(TRUE);
			break;
		case 1:
			m_arRequestBtn[i].SetDisable2(TRUE);
			m_arInformBtn[i].SetDisable2(FALSE);
			m_arJoinBtn[i].SetDisable2(TRUE);
			break;
		case 2:
			m_arRequestBtn[i].SetDisable2(TRUE);
			m_arInformBtn[i].SetDisable2(FALSE);
			m_arJoinBtn[i].SetDisable2(TRUE);
			break;
		case 3:
			m_arRequestBtn[i].SetDisable2(TRUE);
			m_arInformBtn[i].SetDisable2(TRUE);
			m_arJoinBtn[i].SetDisable2(TRUE);
			break;
		case 4:
			m_arRequestBtn[i].SetDisable2(TRUE);
			m_arInformBtn[i].SetDisable2(TRUE);
			m_arJoinBtn[i].SetDisable2(FALSE);
		}
	}
}

void CDmitronBattle::HandleError(int error_code)
{
	switch (error_code)
	{
	case -1: // IDS_DMOTRON_BATTLE_MSG_01 �̹� ��û�Ǿ� �ֽ��ϴ�.
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_DMOTRON_BATTLE_MSG_01),TYPE_NOR_OK, 1);
		break;
	case -2: // IDS_DMOTRON_BATTLE_MSG_02 ������ �����ϴ�.
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_DMOTRON_BATTLE_MSG_02),TYPE_NOR_OK, 1);
		break;
	case -3: // IDS_GUILD_CREATE_GUILD_FAIL_NEED_LAIM ������ �����մϴ�.
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_CREATE_GUILD_FAIL_NEED_LAIM),TYPE_NOR_OK, 1);
		break;
	case -4: // IDS_DMOTRON_BATTLE_MSG_03 ������ ħ���� �� �����ϴ�.
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_DMOTRON_BATTLE_MSG_03),TYPE_NOR_OK, 1);
		break;
	case -5: // IDS_DMOTRON_BATTLE_MSG_04 ������ ��û�� �� �����ϴ�.
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_DMOTRON_BATTLE_MSG_04),TYPE_NOR_OK, 1);
		break;
	case -6: // IDS_DMOTRON_BATTLE_MSG_05 ������ ����� �� �����ϴ�.
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_DMOTRON_BATTLE_MSG_05),TYPE_NOR_OK, 1);
		break;
	case -7: // IDS_DMOTRON_BATTLE_MSG_06 ��û ����ϸ� 24�ð� ���� ��û�� �� �� �����ϴ�.
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_DMOTRON_BATTLE_MSG_06),TYPE_NOR_OK, 1);
		break;
	default:
		break;
	}
}

void CDmitronBattle::UpdateLayout()
{
	int i = 0;

	m_nPosX = m_pBackScene->Xpos = (g_pNk2DFrame->GetClientWidth() - m_pBackScene->GetWidth() ) / 2 - 15;
	m_nPosY = m_pBackScene->Ypos = (g_pNk2DFrame->GetClientHeight() - m_pBackScene->GetHeight() ) / 2;

	// ���� ���� ��� ��ǥ ����
	for( i = 0 ; i < DMITRON_BATTLE_ZONE ; i ++ )
	{
		m_arGuildHuntIndex[ i ].SetPos( m_nPosX + 26, m_nPosY + i * 33 + 168);
		m_arGuildName[ i ].SetPos( m_nPosX + 152, m_nPosY + i * 33 + 168 );
		m_arStatus[ i ].SetPos( m_nPosX + 333, m_nPosY + i * 33 + 168 );
	}

	// ��ư ��ǥ ����
	for( i = 0 ; i < DMITRON_BATTLE_ZONE  ; i ++ )
	{
		m_arRequestBtn[ i ].SetPosition( m_nPosX + 450, m_nPosY + i * 33 + 163 );
		m_arInformBtn[ i ].SetPosition( m_nPosX + 450+38, m_nPosY + i * 33 + 163 );
		m_arJoinBtn[ i ].SetPosition( m_nPosX + 450+6, m_nPosY + i * 33 + 163 );
	}

	// Ȯ�� ��ư / ���� ��ư
	m_ConfirmationBtn.SetPosition( m_nPosX + 395, m_nPosY + 348-5 );
	m_RenewalBtn.SetPosition( m_nPosX + 469, m_nPosY + 348-5 );
	m_btn_close.SetPosition( m_nPosX + m_pBackScene->GetWidth()-19, m_nPosY + 6 );

}

///////////////////////////////////////////////////////////////////////////////

CDmitronBattleInfo::CDmitronBattleInfo()
{
}

CDmitronBattleInfo::~CDmitronBattleInfo()
{
	DeleteRes();
}

BOOL CDmitronBattleInfo::InitMode()
{
	return TRUE;
}


void CDmitronBattleInfo::LoadRes()
{
	SetBackground(WBK_DMITRON_INFO);
	MakeCloseBtn("common/btn_close_01");

	m_RequestDate.Init( 12, _FCOLOR_YELLOW, 0, 0);
	m_RequestTime.Init( 12, _FCOLOR_YELLOW, 0, 0);
	m_RequestGuild.Init( 12, _FCOLOR_YELLOW, 0, 0);
	m_RequestClient.Init( 12, _FCOLOR_YELLOW, 0, 0);

	// ħ�� ��ư
	m_InvadeBtn.SetFileName( "GuildSiege/Agress_ok_btn" );
	m_InvadeBtn.LoadRes();

	// ��û ��� ��ư
	m_CancelBtn.SetFileName( "GuildSiege/Cancel_req_btn" );
	m_CancelBtn.LoadRes();

	UpdateLayout();
}

int CDmitronBattleInfo::Draw()
{
	if( g_pDisplay && m_pBackScene)
	{
		UpdateLayout();

		if(CUIBaseWindow::Draw())
		{
			m_InvadeBtn.Draw();
			m_CancelBtn.Draw();

			m_RequestDate.Draw(125);
			m_RequestTime.Draw(125);
			m_RequestGuild.Draw(125);
			m_RequestClient.Draw(125);
		}
	}
	return -1;
}

BOOL CDmitronBattleInfo::IsInside(int x, int y)
{
	return CUIBaseWindow::IsInside(x, y);
}

void CDmitronBattleInfo::DeleteRes()
{
	m_RequestDate.DeleteRes();
	m_RequestTime.DeleteRes();
	m_RequestGuild.DeleteRes();
	m_RequestClient.DeleteRes();

	// ħ�� ��ư ����
	m_InvadeBtn.DeleteRes();
	// ��û ��� ��ư ����
	m_CancelBtn.DeleteRes();
}

void CDmitronBattleInfo::RestoreSurfaces()
{
}

void CDmitronBattleInfo::ActivateInfoWindow(int nTime, char* pStrGuild, char* pStrClient)
{
	time_t timeTime = (time_t)nTime;
	struct tm *newtime;
	newtime = localtime( &timeTime ); /* Convert to local time. */

	newtime->tm_year += 1900;
	newtime->tm_mon += 1;
	newtime->tm_mday += 0;

	sprintf(CDmitronInfo::GetInfo().Date, "%02d/%02d/%02d", newtime->tm_year, newtime->tm_mon, newtime->tm_mday);
	sprintf(CDmitronInfo::GetInfo().Time, "%02d:%02d:%02d", newtime->tm_hour, newtime->tm_min, newtime->tm_sec);
	strcpy(CDmitronInfo::GetInfo().Guild, pStrGuild);
	strcpy(CDmitronInfo::GetInfo().Client, pStrClient);

	m_RequestDate.SetString_Center_Arrange(CDmitronInfo::GetInfo().Date, RGB(0, 0, 0), RGB(0, 0, 0), TRUE);
	m_RequestTime.SetString_Center_Arrange(CDmitronInfo::GetInfo().Time, RGB(0, 0, 0), RGB(0, 0, 0), TRUE);
	m_RequestGuild.SetString_Center_Arrange(CDmitronInfo::GetInfo().Guild, RGB(0, 0, 0), RGB(0, 0, 0), TRUE);
	m_RequestClient.SetString_Center_Arrange(CDmitronInfo::GetInfo().Client, RGB(0, 0, 0), RGB(0, 0, 0), TRUE);
}

BOOL CDmitronBattleInfo::MoveWindow(UINT msg, int x, int y)
{
	return CUIBaseWindow::MoveWindow(msg, x, y);
}

LRESULT CDmitronBattleInfo::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if(!IsInside(LOWORD(lParam), HIWORD(lParam)))
		return IM_NONE;

	if (m_InvadeBtn.IsDisable() == FALSE)
		m_InvadeBtn.MsgProc( hWnd, uMsg, wParam, lParam );

	if (m_CancelBtn.IsDisable() == FALSE)
		m_CancelBtn.MsgProc( hWnd, uMsg, wParam, lParam );

	switch( uMsg )
	{
	case WM_LBUTTONUP:
		if (m_InvadeBtn.m_iBtnFlg == BTN_ACTION)
		{
			m_InvadeBtn.m_iBtnFlg = BTN_NORMAL;
			CDmitronInfo::GetZone( CDmitronInfo::GetLock().Index ).Stage = 2;
			sprintf(commOutBuf, "occp bt start %d\n", CDmitronInfo::GetLock().Index);
			if(g_pTcpIp)
				g_pTcpIp->SendNetMessage(commOutBuf);
			return IM_CLOSE;
		}
		if (m_CancelBtn.m_iBtnFlg == BTN_ACTION)
		{
			m_CancelBtn.m_iBtnFlg = BTN_NORMAL;
			CDmitronInfo::GetZone( CDmitronInfo::GetLock().Index ).Stage = 3;
			sprintf(commOutBuf, "occp bt cancel %d\n", CDmitronInfo::GetLock().Index );
			if(g_pTcpIp)
				g_pTcpIp->SendNetMessage(commOutBuf);
			return IM_CLOSE;
		}
	}
	return CUIBaseWindow::MsgProc(hWnd, uMsg, wParam, lParam);
}

void CDmitronBattleInfo::UpdateLayout()
{
	if( !g_pNk2DFrame )
		return;

	int i = 0;

	m_nPosX = m_pBackScene->Xpos = (g_pNk2DFrame->GetClientWidth() - m_pBackScene->GetWidth() ) / 2;
	m_nPosY = m_pBackScene->Ypos = (g_pNk2DFrame->GetClientHeight() - m_pBackScene->GetHeight() ) / 2;

	// ��ư ��ǥ ����
	m_InvadeBtn.SetPosition( m_nPosX + 60, m_nPosY + 177);
	m_CancelBtn.SetPosition( m_nPosX + 140, m_nPosY + 177);

	m_RequestDate.SetPos(m_nPosX + 105, m_nPosY + 43);
	m_RequestTime.SetPos(m_nPosX + 105, m_nPosY + 71);
	m_RequestGuild.SetPos(m_nPosX + 105, m_nPosY + 98);
	m_RequestClient.SetPos(m_nPosX + 105, m_nPosY + 128);

	SetExitBtnPostion(225, 4);
}