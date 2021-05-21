#include "stdafx.h"


#include "headers.h"
#include "BaseInterface.h"
#include "main.h"
#include "ControlSocial.h"
#include "ddutil.h"
#include "Nk2DFrame.h"
#include "tcpipcon.h"
#include "TcpUtil.h"
#include "ListBox.h"
#include "ControlHeroPrivilege.h"
#include "Land.h"
#include "UIMgr.h"
#include "g_stringmanager.h"
#include "GUtil.h"
#include "Mouse.h"




extern char					g_strFontName[];

extern HFONT G_GetFont(int h, int w,const char  * const fontname);

CControlHeroPrivilege::CControlHeroPrivilege()
{
	m_pSurBack = NULL;
	m_pSurInfo = NULL;
	m_HeroPrivilege = 0;

	m_btnPrivilege.SetFileName("Bossraid/HeroPrivilege/btn_Privilege");
	m_btnClose.SetFileName("Bossraid/HeroPrivilege/btn_Close");
}

CControlHeroPrivilege::~CControlHeroPrivilege()
{
	DeleteRes();
}

void CControlHeroPrivilege::LoadRes()
{
	HWND	  hWnd;

	hWnd = g_pDisplay->GetHWnd();
	GetClientRect(hWnd, &m_ClientRc);

	g_pDisplay->CreateSurfaceFromBitmap( &m_pSurBack, "interface/Bossraid/HeroPrivilege/bg.bmp");

	m_pSurBack->Xpos = (m_ClientRc.right	- m_pSurBack->GetWidth())	/ 2;
	m_pSurBack->Ypos = (m_ClientRc.bottom	- m_pSurBack->GetHeight())	/ 2;

	m_btnPrivilege.LoadRes();
	m_btnClose.LoadRes();

	//m_btnPrivilege.Height = 41;

	m_btnPrivilege.SetPosition( m_pSurBack->Xpos + 71, m_pSurBack->Ypos + 233 );
	m_btnClose.SetPosition( m_pSurBack->Xpos + 244, m_pSurBack->Ypos + 7 );
}


void CControlHeroPrivilege::DeleteRes()
{
	SAFE_DELETE(m_pSurBack);
	SAFE_DELETE(m_pSurInfo);

	m_btnPrivilege.DeleteRes();
	m_btnClose.DeleteRes();
}


void CControlHeroPrivilege::Draw()
{
	if( m_pSurBack )
		g_pDisplay->Blt(m_pSurBack->Xpos, m_pSurBack->Ypos, m_pSurBack);

	if( m_HeroPrivilege == 1)
		m_btnPrivilege.Draw();

	m_btnClose.Draw();
}



int CControlHeroPrivilege::IsInside(int x, int y)
{
	// 마우스가 bg안에 있는지 검사
	int left, right, top, bottom;
	left	= m_pSurBack->Xpos;
	top		= m_pSurBack->Ypos;
	right	= left + m_pSurBack->GetWidth();
	bottom	= top  + m_pSurBack->GetHeight();

	if( left < x && right > x && top < y && bottom > y )
		return 1;

	return 0;
}


LRESULT CControlHeroPrivilege::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int x = LOWORD (lParam);
	int y = HIWORD (lParam);

	switch ( msg )
	{
	case WM_MOUSEMOVE:
		if(IsInside(x, y) == 1)
		{
			pCMyApp->m_pMouse->SetMouseType(M_NORMAL);
			return 1;
		}
		break;

	case WM_LBUTTONDOWN:

		if( IsInside(x, y) != 1)
		{
			g_pNk2DFrame->ShowHeroRankListWindow(FALSE);
			return 1;
		}

		if( IsInside(x, y) != 1)
			return 1;

		m_btnClose.MsgProc(hWnd, msg, wParam, lParam );
		m_btnPrivilege.MsgProc(hWnd, msg, wParam, lParam );
		return 1;

	case WM_LBUTTONUP:

		if(IsInside(x, y) != 1)
			return 1;

		m_btnClose.MsgProc(hWnd, msg, wParam, lParam );
		m_btnPrivilege.MsgProc(hWnd, msg, wParam, lParam );

		if(m_btnClose.GetState() == BTN_ACTION)
		{
			g_pNk2DFrame->ShowHeroPrivilegeWindow(FALSE);
			m_btnClose.SetState(BTN_NORMAL);
			return 1;
		}
		else
			m_btnClose.SetState(BTN_NORMAL);


		if(m_btnPrivilege.GetState() == BTN_ACTION)
		{
			if( m_HeroPrivilege == 1)
			{
				g_pTcpIp->SendNetMessage("hr_item req\n");
				g_pNk2DFrame->ShowHeroPrivilegeWindow(FALSE);
			}

			m_btnPrivilege.SetState(BTN_NORMAL);
			return 1;
		}
		else
			m_btnPrivilege.SetState(BTN_NORMAL);


		return 1;
	}
	return 0;
}


void CControlHeroPrivilege::SetHeroPrivilege(int heroPrivilege )
{
	m_HeroPrivilege = heroPrivilege;

	switch( m_HeroPrivilege )
	{
	case 0:
		m_btnPrivilege.SetDisable(TRUE);
		break;
	case 1:
		m_btnPrivilege.SetDisable(FALSE);
		break;
	}
}