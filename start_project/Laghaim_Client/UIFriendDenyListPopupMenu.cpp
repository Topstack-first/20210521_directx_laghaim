#include "stdafx.h"
#include "UIFriendDenyListPopupMenu.h"
#include "ddutil.h"
#include "headers.h"
#include "DataFriend.h"

extern void SendRefusalFriend(const int& order, const std::string& name);

UIFriendDenyListPopupMenu::UIFriendDenyListPopupMenu()
	: ControlPopupMenu()
{
	LoadRes();
}

UIFriendDenyListPopupMenu::~UIFriendDenyListPopupMenu()
{
	DeleteRes();
}

void UIFriendDenyListPopupMenu::LoadRes()
{
	g_pDisplay->CreateSurfaceFromBitmap(&m_pSurpBack, "INTERFACE/LETTERFRIEND/POPUP_DENYBACK.BMP");
	if( m_pSurpBack != NULL )
	{
		m_pSurpBack->SetColorKey(RGB(0, 0, 0));
	}

	LoadRes_Btn();

	SetGap(5, 5);
	SetPosition(m_iPosX, m_iPosY, true);
}

void UIFriendDenyListPopupMenu::LoadRes_Btn()
{
	CBasicButton* pBtn = NULL;

	pBtn = new CBasicButton();
	pBtn->SetFileName("LETTERFRIEND/POPUP_UNBLOCK");
	pBtn->LoadRes();
	m_vecBtns.push_back(pBtn);
}

void UIFriendDenyListPopupMenu::DeleteRes()
{
	ControlPopupMenu::DeleteRes();
}

LRESULT UIFriendDenyListPopupMenu::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	if( m_bVisible == false )
		return FALSE;

	const int pointX = LOWORD(lParam);
	const int pointY = HIWORD(lParam);

	LRESULT bResult = FALSE;

	unsigned int size = m_vecBtns.size();
	for( unsigned int i = 0; i < size; i++ )
	{
		m_vecBtns[i]->MsgProc(hWnd, msg, wParam, lParam);
	}

	switch(msg)
	{
	case WM_LBUTTONUP:
		{
			if( isInside(pointX, pointY) == false )
			{
				SetVisible(false);
				return TRUE;
			}

			if( size > eFRIEND_DENY_POPUP_MENU_DENY && m_vecBtns[eFRIEND_DENY_POPUP_MENU_DENY]->GetState() == BTN_ACTION )
			{
				ProcUndeny();
				SetVisible(false);
				return TRUE;
			}
		}
		break;
	}

	return bResult;
}

void UIFriendDenyListPopupMenu::ProcUndeny()
{
	DataFriend* friendMgr = DataFriend::GetInstance();

	stFriend st = friendMgr->GetDenyInfo(m_index);

	SendRefusalFriend(4, st.name);
}