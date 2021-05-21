#include "stdafx.h"
#include "UIFriendListPopupMenu.h"
#include "ddutil.h"
#include "headers.h"
#include "DataFriend.h"
#include "Nk2DFrame.h"
#include "ControlBottom.h"

extern void SendDelFriend(const int& order, const int& index);
extern void SendRefusalFriend(const int& order, const std::string& name);

UIFriendListPopupMenu::UIFriendListPopupMenu()
	: ControlPopupMenu()
{
	LoadRes();
}

UIFriendListPopupMenu::~UIFriendListPopupMenu()
{
	DeleteRes();
}

void UIFriendListPopupMenu::LoadRes()
{
	g_pDisplay->CreateSurfaceFromBitmap(&m_pSurpBack, "INTERFACE/LETTERFRIEND/POPUP_BACK.BMP");
	if( m_pSurpBack != NULL )
	{
		m_pSurpBack->SetColorKey(RGB(0, 0, 0));
	}

	LoadRes_Btn();

	SetGap(5, 5);
	SetPosition(m_iPosX, m_iPosY, true);
}

void UIFriendListPopupMenu::LoadRes_Btn()
{
	CBasicButton* pBtn = NULL;

	pBtn = new CBasicButton();
	pBtn->SetFileName("LETTERFRIEND/POPUP_WHISPER");
	pBtn->LoadRes();
	m_vecBtns.push_back(pBtn);

	pBtn = new CBasicButton();
	pBtn->SetFileName("LETTERFRIEND/POPUP_BLOCK");
	pBtn->LoadRes();
	m_vecBtns.push_back(pBtn);
}

void UIFriendListPopupMenu::DeleteRes()
{
	ControlPopupMenu::DeleteRes();
}

LRESULT UIFriendListPopupMenu::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
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

			if( size > eFRIEND_POPUP_MENU_WHISPER && m_vecBtns[eFRIEND_POPUP_MENU_WHISPER]->GetState() == BTN_ACTION )
			{
				ProcWhisper();
				SetVisible(false);
				return TRUE;
			}

			if( size > eFRIEND_POPUP_MENU_REMOVE && m_vecBtns[eFRIEND_POPUP_MENU_REMOVE]->GetState() == BTN_ACTION )
			{
				ProcRemove();
				SetVisible(false);
				return TRUE;
			}
		}
		break;
	}

	return bResult;
}

void UIFriendListPopupMenu::ProcWhisper()
{
	DataFriend* friendMgr = DataFriend::GetInstance();

	stFriend st = friendMgr->GetFriendInfo(m_index);

	if( pCMyApp->m_pUIMgr && g_pNk2DFrame
			&& g_pNk2DFrame->GetControlBottom() )
	{
		char strTemp[MAX_PATH];
		sprintf( strTemp,"/%s ", st.name.c_str());
		g_pNk2DFrame->GetControlBottom()->SetFocus();
		g_pNk2DFrame->GetControlBottom()->m_InputBox.ClearText();
		g_pNk2DFrame->GetControlBottom()->m_InputBox.SetText(strTemp);
	}
}

void UIFriendListPopupMenu::ProcRemove()
{
	DataFriend* friendMgr = DataFriend::GetInstance();

	stFriend st = friendMgr->GetFriendInfo(m_index);

	SendDelFriend(1, st.index);
}