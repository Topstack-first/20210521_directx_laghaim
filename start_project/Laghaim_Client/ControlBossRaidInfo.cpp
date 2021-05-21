#include "stdafx.h"


#include "headers.h"
#include "BaseInterface.h"
#include "main.h"
#include "ControlSocial.h"
#include "ddutil.h"
#include "Nk2DFrame.h"
#include "tcpipcon.h"
#include "ListBox.h"
#include "ControlBossraidInfo.h"
#include "Land.h"
#include "UIMgr.h"
#include "g_stringmanager.h"
#include "GUtil.h"
#include "Mouse.h"

#pragma warning(disable : 4244)






extern DWORD				g_dwClientCountry;			// 0: Korean 1: Japanese  2: English
extern BOOL					g_bActivePackage;
extern BOOL					g_bGlobalMatrix; ///국가대항전 용 클라이언트 인지 여부.

CControlBossraidInfo::CControlBossraidInfo()
{
	Init();
}

CControlBossraidInfo::~CControlBossraidInfo()
{
	DeleteRes();
}

void CControlBossraidInfo::LoadRes()
{
	char str[256];

	int i,j;
	for( i=0; i<5; i++)
		for( j=0; j<3; j++)
		{
			sprintf( str, "interface/Bossraid/info/%02d_%02d.bmp",i,j);
			g_pDisplay->CreateSurfaceFromBitmap( &m_bBossRaidInfo[i][j].pEna, str );
			if(m_bBossRaidInfo[i][j].pEna)
				m_bBossRaidInfo[i][j].pEna->SetColorKey(RGB(0, 0, 0));
		}

	g_pDisplay->CreateSurfaceFromBitmap( &m_pFinalBossEna, "interface/Bossraid/info/05.bmp");
	if(m_pFinalBossEna)
		m_pFinalBossEna->SetColorKey(RGB(0, 0, 0));

	g_pDisplay->CreateSurfaceFromBitmap( &m_pBack, "interface/Bossraid/info/bg.bmp");
	if(m_pBack)
		m_pBack->SetColorKey(RGB(0, 0, 0));


	HWND	  hWnd;

	hWnd = g_pDisplay->GetHWnd();
	GetClientRect(hWnd, &m_ClientRc);

	m_pBack->Xpos = ( m_ClientRc.right  - m_pBack->GetWidth() ) / 2;
	m_pBack->Ypos = ( m_ClientRc.bottom - m_pBack->GetHeight() ) / 2;

	if( m_ClientRc.bottom > 600 )
		m_pBack->Ypos -= 30;


	for( j=0; j<3; j++)
	{
		m_bBossRaidInfo[0][j].pEna->Xpos = m_pBack->Xpos + 144;
		m_bBossRaidInfo[0][j].pEna->Ypos = m_pBack->Ypos + 107;
	}

	for( j=0; j<3; j++)
	{
		m_bBossRaidInfo[1][j].pEna->Xpos = m_pBack->Xpos + 512;
		m_bBossRaidInfo[1][j].pEna->Ypos = m_pBack->Ypos + 253;
	}

	for( j=0; j<3; j++)
	{
		m_bBossRaidInfo[2][j].pEna->Xpos = m_pBack->Xpos + 184;
		m_bBossRaidInfo[2][j].pEna->Ypos = m_pBack->Ypos + 257;
	}

	for( j=0; j<3; j++)
	{
		m_bBossRaidInfo[3][j].pEna->Xpos = m_pBack->Xpos + 347;
		m_bBossRaidInfo[3][j].pEna->Ypos = m_pBack->Ypos + 336;
	}

	for( j=0; j<3; j++)
	{
		m_bBossRaidInfo[4][j].pEna->Xpos = m_pBack->Xpos + 552;
		m_bBossRaidInfo[4][j].pEna->Ypos = m_pBack->Ypos + 107;
	}

	m_pFinalBossEna->Xpos = m_pBack->Xpos + 302;
	m_pFinalBossEna->Ypos = m_pBack->Ypos + 123;
}

void CControlBossraidInfo::DeleteRes()
{
	for(int i=0; i<5; i++)
		for(int j=0; j<3; j++)
			SAFE_DELETE(m_bBossRaidInfo[i][j].pEna );
	SAFE_DELETE(m_pFinalBossEna);
	SAFE_DELETE(m_pBack);
}


void CControlBossraidInfo::Init()
{
	for(int i=0; i<5; i++)
		for(int j=0; j<3; j++)
		{
			m_bBossRaidInfo[i][j].bKill = FALSE;
			m_bBossRaidInfo[i][j].pEna = NULL;
		}
	m_pFinalBossEna = NULL;
	m_pBack = NULL;
	m_KillCount = 0;
}


void CControlBossraidInfo::InitKillCount()
{
	for(int i=0; i<5; i++)
		for(int j=0; j<3; j++)
			m_bBossRaidInfo[i][j].bKill = FALSE;

	m_KillCount = 0;
}
void CControlBossraidInfo::SetBossraidKillFlag( int BossZoneNum, int BossNum, BOOL KillFlag )
{
	if( BossZoneNum >= 5 || BossNum >= 3)
		return;

	m_bBossRaidInfo[BossZoneNum][BossNum].bKill = KillFlag;

	if(KillFlag)
		m_KillCount ++;
}


BOOL CControlBossraidInfo::IsBossAllKill()
{
	int count = 0;
	for(int i=0; i<5; i++)
		for(int j=0; j<3; j++)
		{
			if( m_bBossRaidInfo[i][j].bKill == TRUE )
				count ++;
		}

	if( count == 5*3 )
		return TRUE;

	return FALSE;
}

int CControlBossraidInfo::IsInside(int x, int y)
{
	// 가운데 보스 마우스 오버 확인
	int tmpX = m_pFinalBossEna->Xpos + m_pFinalBossEna->GetWidth()  / 2 ;
	int tmpY = m_pFinalBossEna->Ypos + m_pFinalBossEna->GetHeight() / 2 ;

	float dist = DistPlToPl( tmpX, tmpY, x, y);

	if( dist <= 63.0f )
		return 2;

	// 마우스가 bg안에 있는지 검사
	int left, right, top, bottom;
	left	= m_pBack->Xpos;
	top		= m_pBack->Ypos;
	right	= left + m_pBack->GetWidth();
	bottom	= top  + m_pBack->GetHeight();


	if( left < x && right > x && right < y && bottom > y)
		return 1;


	return -1;
}


void CControlBossraidInfo::Draw()
{
	if( m_pBack )
		g_pDisplay->Blt(m_pBack->Xpos, m_pBack->Ypos, m_pBack);

	POINT point;
	GetCursorPos(&point);
	ScreenToClient(g_pDisplay->GetHWnd(), &point);

	if (pCMyApp->m_pMouse && IsInside(point.x, point.y))
		pCMyApp->m_pMouse->SetMouseType(M_NORMAL);

	for(int i=0; i<5; i++)
		for(int j=0; j<3; j++)
		{
			if( m_bBossRaidInfo[i][j].bKill == TRUE && m_bBossRaidInfo[i][j].pEna)
				g_pDisplay->Blt(m_bBossRaidInfo[i][j].pEna->Xpos, m_bBossRaidInfo[i][j].pEna->Ypos, m_bBossRaidInfo[i][j].pEna);
		}

	if( m_KillCount == 15 && m_pFinalBossEna )
		g_pDisplay->Blt(m_pFinalBossEna->Xpos, m_pFinalBossEna->Ypos, m_pFinalBossEna);

}


LRESULT CControlBossraidInfo::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if( !g_pRoh )
		return 0;

	int x = LOWORD (lParam);
	int y = HIWORD (lParam);

	switch ( msg )
	{
	case WM_MOUSEMOVE:

		if(IsInside(x, y) == 2 && m_KillCount == 15)
		{
			pCMyApp->m_pMouse->SetMouseType(M_HOLD);
			return 1;
		}
		else
		{
			pCMyApp->m_pMouse->SetMouseType(M_NORMAL);
			return 1;
		}

		break;

	case WM_LBUTTONDOWN:
		if(IsInside(x, y) == 2)
		{
			if( m_KillCount == 15 )
			{
				// 마르보덴 통행증 교환 패켓을 보낸다.
				if(g_pTcpIp)
				{
					if( g_pRoh->IsFullInvenInPack( 0 ) )
					{
						// IDS_CASHSHOP_ERROR3 "인벤토리에 빈 공간이 부족합니다."
						g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_CASHSHOP_ERROR3), TYPE_NOR_OK, 1);
					}
					else
					{
						m_KillCount = 0;
						g_pTcpIp->SendNetMessage("br_lic\n");
					}
				}

			}
			else
				g_pNk2DFrame->ShowBossraidInfoWindow(FALSE);

			return 1;
		}
		else
		{
			g_pNk2DFrame->ShowBossraidInfoWindow(FALSE);
			return 1;
		}

		break;
	case WM_RBUTTONDOWN:
		g_pNk2DFrame->ShowBossraidInfoWindow(FALSE);
		return 1;

	}
	return 0;
}

