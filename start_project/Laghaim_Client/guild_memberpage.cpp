#include "stdafx.h"
#include "guild_memberpage.h"
#include "Nk2DFrame.h"
#include "headers.h"
#include "window_resource.h"
#include "WindowMgr.h"
#include "g_stringmanager.h"
#include "Mouse.h"







CGuildMemberPage::CGuildMemberPage() : CUIBaseWindow()
{
	m_pTextSur = NULL;
}

CGuildMemberPage::~CGuildMemberPage()
{
	DeleteRes();
}


BOOL CGuildMemberPage::InitMode()
{
	m_btPromotion.SetFileName("guild/Promotion");		// 승급
	m_btDissolve.SetFileName("guild/Disband");			// 해산
	m_btStore.SetFileName("guild/Inventory");			// 창고
	m_btSearch.SetFileName("guild/search");				// 검색
	m_btBattle.SetFileName("guild/Battle");				// 전투
	m_btMix.SetFileName("guild/Mix");					// 조합
	m_btOut.SetFileName("guild/Secede");				// 탈퇴

	return TRUE;
}

void CGuildMemberPage::LoadRes()
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

	g_pDisplay->CreateSurfaceFromBitmap(&m_pTextSur, "interface/guild/guild_text/NPC_guild.bmp");

	if(m_pBackScene)
	{

		int n_x = (nScreenWidth  / 2) - (m_pBackScene->GetWidth()  / 2);
		int n_y = (nScreenHeight / 2) - (m_pBackScene->GetHeight() / 2);

		m_nPosX = n_x;
		m_nPosY = n_y;

		if(m_pTextSur)
		{
			m_pTextSur->Xpos = m_nPosX + 23;
			m_pTextSur->Ypos = m_nPosY + 51;
		}

		m_btPromotion.LoadRes();	// 승급
		m_btDissolve.LoadRes();		// 해산
		m_btStore.LoadRes();		// 창고
		m_btSearch.LoadRes();		// 검색
		m_btBattle.LoadRes();		// 전투
		m_btMix.LoadRes();			// 조합
		m_btOut.LoadRes();			// 탈퇴

		SetPosition();

	}
}

void CGuildMemberPage::DeleteRes()
{
	SAFE_DELETE(m_pTextSur);

	m_btPromotion.DeleteRes();
	m_btDissolve.DeleteRes();
	m_btStore.DeleteRes();
	m_btSearch.DeleteRes();
	m_btBattle.DeleteRes();
	m_btOut.DeleteRes();

}

void CGuildMemberPage::RestoreSurfaces()
{

}

int CGuildMemberPage::Draw()
{
	if( g_pDisplay && m_pBackScene )
	{
		POINT point;
		GetCursorPos(&point);
		ScreenToClient(g_pDisplay->GetHWnd(), &point);

		if (pCMyApp->m_pMouse && IsInside(point.x, point.y))
			pCMyApp->m_pMouse->SetMouseType(M_NORMAL);

		if(CUIBaseWindow::Draw())
		{
			m_btPromotion.Draw();
			m_btDissolve.Draw();
			m_btStore.Draw();
			m_btSearch.Draw();
			m_btBattle.Draw();
			m_btMix.Draw();
			m_btOut.Draw();

			if(m_pTextSur)	g_pDisplay->Blt(m_pTextSur->Xpos, m_pTextSur->Ypos, m_pTextSur);
		}
	}


	return -1;
}

void CGuildMemberPage::SetPosition()
{
	m_pBackScene->Xpos = m_nPosX;
	m_pBackScene->Ypos = m_nPosY;
	m_btPromotion.SetPosition(m_nPosX + 41, m_nPosY + 235);
	m_btDissolve.SetPosition(m_nPosX + 121, m_nPosY + 235);
	m_btBattle.SetPosition(m_nPosX + 203, m_nPosY + 235);

	m_btStore.SetPosition(m_nPosX + 41, m_nPosY + 268);
	m_btSearch.SetPosition(m_nPosX + 121, m_nPosY + 268);
	m_btOut.SetPosition(m_nPosX + 203 , m_nPosY + 268);

	m_btMix.SetPosition(m_nPosX + 64, m_nPosY + 299);
	m_pBtExit->SetPosition(m_nPosX + 181, m_nPosY + 299);
// 	m_pBtExit->SetPosition(m_nPosX + m_pBackScene->GetWidth() - m_pBtExit->Width - 60,
// 						   m_nPosY + 296);

	m_pTextSur->Xpos = m_nPosX + 23;
	m_pTextSur->Ypos = m_nPosY + 51;
}

void CGuildMemberPage::guilds_SetEnable(BOOL bEnable)
{
	if(bEnable)
	{
		m_btPromotion.SetDisable( false );
		m_btDissolve.SetDisable( false );
		m_btBattle.SetDisable( false );
	}
	else
	{
		m_btPromotion.SetState(BTN_DISABLE);
		m_btDissolve.SetState(BTN_DISABLE);
		m_btBattle.SetState(BTN_DISABLE);

		m_btPromotion.SetDisable( true );
		m_btDissolve.SetDisable( true );
		m_btBattle.SetDisable( true );
	}
}

BOOL CGuildMemberPage::IsInside(int x, int y)
{
	return CUIBaseWindow::IsInside(x, y);
}

BOOL CGuildMemberPage::MoveWindow(UINT msg, int x, int y)
{
	return CUIBaseWindow::MoveWindow(msg, x, y);
}

LRESULT CGuildMemberPage::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	m_btPromotion.MsgProc(hWnd, msg, wParam, lParam);
	m_btDissolve.MsgProc(hWnd, msg, wParam, lParam);
	m_btStore.MsgProc(hWnd, msg, wParam, lParam);
	m_btSearch.MsgProc(hWnd, msg, wParam, lParam);
	m_btBattle.MsgProc(hWnd, msg, wParam, lParam);
	m_btMix.MsgProc(hWnd, msg, wParam, lParam);
	m_btOut.MsgProc(hWnd, msg, wParam, lParam);

	switch(msg)
	{
	case WM_LBUTTONDOWN:

		if(!IsInside(LOWORD(lParam), HIWORD(lParam)))
			return IM_NONE;

		if( m_btPromotion.GetState() == BTN_DOWN )
			return IM_INPUT;

		if( m_btDissolve.GetState() == BTN_DOWN )
			return IM_INPUT;

		if( m_btStore.GetState() == BTN_DOWN )
			return IM_INPUT;

		if( m_btSearch.GetState() == BTN_DOWN )
			return IM_INPUT;

		if( m_btBattle.GetState() == BTN_DOWN )
			return IM_INPUT;

		if( m_btMix.GetState() == BTN_DOWN )
			return IM_INPUT;

		if( m_btOut.GetState() == BTN_DOWN )
			return IM_INPUT;

		break;

	case WM_LBUTTONUP :

		if(!IsInside(LOWORD(lParam), HIWORD(lParam)))
			return IM_NONE;

		if(m_btSearch.GetState() == BTN_ACTION)	   // 검색
		{
			g_pNk2DFrame->GetUIWindow()->OnOpenWindow(WINDOW_GUILD_SEARCH);
			return IM_INPUT;
		}
		else if(m_btPromotion.GetState() == BTN_ACTION)	// 승급
		{
			// IDS_GUILD_LVLUP_SEND_MSG		"길드 승급을 진행하시겠습니까?"
			g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_LVLUP_SEND_MSG), TYPE_GUILD_CONFIRM_CANCEL, 46);
			return IM_INPUT;
		}
		else if(m_btBattle.GetState() == BTN_ACTION)		// 전투
		{
			g_pNk2DFrame->GetUIWindow()->OnOpenWindow(WINDOW_GUILD_BATTLE_KIND);
			return IM_INPUT;
		}
		else if(m_btDissolve.GetState() == BTN_ACTION)	// 해산
		{
			// IDS_GUILD_DISERSE_SEND_MSG01	"길드를 해산하시겠습니까?"
			g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_DISERSE_SEND_MSG01), TYPE_GUILD_CONFIRM_CANCEL, 47);
			return IM_INPUT;
		}
		else if(m_btOut.GetState() == BTN_ACTION)		// 탈퇴
		{
			// IDS_GUILD_MEMBER_OUT_SEND_MSG	"길드 탈퇴시 1천만 라임이 소모됩니다. 길드를 탈퇴하시겠습니까?"
			g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_MEMBER_OUT_SEND_MSG), TYPE_GUILD_CONFIRM_CANCEL, 48);
			return IM_INPUT;
		}
		else if(m_btStore.GetState() == BTN_ACTION)		// 창고
		{
			g_pNk2DFrame->ToggleGStoreHouseWindow();

			return IM_INPUT;
		}
		else if(m_btMix.GetState() == BTN_ACTION)		// 창고
		{
			g_pNk2DFrame->GetUIWindow()->OnOpenWindow(WINDOW_GUILD_MIX);
			return IM_INPUT;
		}
		break;
	}

	return CUIBaseWindow::MsgProc(hWnd, msg, wParam, lParam);
}
