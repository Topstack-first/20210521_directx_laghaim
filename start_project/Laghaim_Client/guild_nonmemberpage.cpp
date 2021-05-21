#include "stdafx.h"
#include "guild_nonmemberpage.h"
#include "Nk2DFrame.h"
#include "headers.h"
#include "window_resource.h"
#include "WindowMgr.h"



extern	SVR_TYPE			g_SvrType;

CGuildNoneMemberPage::CGuildNoneMemberPage() : CUIBaseWindow()
{
	m_pTextSur = NULL;
}

CGuildNoneMemberPage::~CGuildNoneMemberPage()
{
	DeleteRes();
}


BOOL CGuildNoneMemberPage::InitMode()
{
	m_btCreate.SetFileName("guild/Make"  );	// 창설
	m_btSearch.SetFileName("guild/Search");	// 검색

	return TRUE;
}

void CGuildNoneMemberPage::LoadRes()
{
	int nScreenWidth  = 0;
	int nScreenHeight = 0;
	if( g_pNk2DFrame )
	{
		nScreenWidth  = g_pNk2DFrame->GetClientWidth();
		nScreenHeight = g_pNk2DFrame->GetClientHeight();
	}

	SetBackground(WBK_NPCPAGE);
	MakeCloseBtn("guild/Exit");

	SAFE_DELETE(m_pTextSur);

	g_pDisplay->CreateSurfaceFromBitmap(&m_pTextSur, "interface/guild/guild_text/NPC_guildsetup.bmp");

	if(m_pBackScene)
	{

		int n_x = (nScreenWidth  / 2) - (m_pBackScene->GetWidth()  / 2);
		int n_y = (nScreenHeight / 2) - (m_pBackScene->GetHeight() / 2);

		m_nPosX = n_x;
		m_nPosY = n_y;

		if(m_pTextSur)
		{
			m_pTextSur->Xpos = m_nPosX + 40;
			m_pTextSur->Ypos = m_nPosY + 70;

		}

		m_btCreate.LoadRes();
		m_btSearch.LoadRes();

		SetPosition();

	}
}

void CGuildNoneMemberPage::DeleteRes()
{
	SAFE_DELETE(m_pTextSur);
	m_btCreate.DeleteRes();
	m_btSearch.DeleteRes();
}

void CGuildNoneMemberPage::RestoreSurfaces()
{

}

int CGuildNoneMemberPage::Draw()
{
	if( g_pDisplay && m_pBackScene )
	{
		if( CUIBaseWindow::Draw() )
		{
			m_btCreate.Draw();
			m_btSearch.Draw();

			if(m_pTextSur)
				g_pDisplay->Blt(m_pTextSur->Xpos, m_pTextSur->Ypos, m_pTextSur);
		}
	}

	return -1;
}

void CGuildNoneMemberPage::SetPosition()
{
	CUIBaseWindow::SetExitBtnPostion(209, 237);

	m_btCreate.SetPosition(m_nPosX + 33 , m_nPosY + 237);
	m_btSearch.SetPosition(m_nPosX + 121, m_nPosY + 237);

	m_pTextSur->Xpos = m_nPosX + 22;
	m_pTextSur->Ypos = m_nPosY + 52;
}

BOOL CGuildNoneMemberPage::IsInside(int x, int y)
{
	return CUIBaseWindow::IsInside(x, y);
}

BOOL CGuildNoneMemberPage::MoveWindow(UINT msg, int x, int y)
{
	return CUIBaseWindow::MoveWindow(msg, x, y);
}

LRESULT CGuildNoneMemberPage::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	m_btCreate.MsgProc(hWnd, msg, wParam, lParam);
	m_btSearch.MsgProc(hWnd, msg, wParam, lParam);

	switch( msg )
	{
	case WM_LBUTTONDOWN:
		if(!IsInside(LOWORD(lParam), HIWORD(lParam)))
			return IM_NONE;

		if( m_btCreate.GetState() == BTN_DOWN )
			return IM_INPUT;

		if( m_btSearch.GetState() == BTN_DOWN )
			return IM_INPUT;

		break;

	case WM_LBUTTONUP :
		if(m_btCreate.GetState() == BTN_ACTION)
		{
			g_pNk2DFrame->InsertPopup( "Create", TYPE_GUILD_CREATE_PROMPT, 41 );
			return IM_INPUT;
		}

		if(m_btSearch.GetState() == BTN_ACTION)
		{
			g_pNk2DFrame->GetUIWindow()->OnOpenWindow(WINDOW_GUILD_SEARCH);
			return IM_INPUT;
		}
		break;
	}

	return CUIBaseWindow::MsgProc(hWnd, msg, wParam, lParam);
}
