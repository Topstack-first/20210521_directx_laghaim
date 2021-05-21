#include "stdafx.h"
#include "Config.h"
#include "basicbutton.h"
#include "UIFriendList.h"
#include "UIFriendDenyList.h"
#include "Nk2DFrame.h"
#include "UIFriend.h"
#include "ControlBottom.h"
#include "Mouse.h"

extern CNk2DFrame* g_pNk2DFrame;
extern void SendAddFriend(const int& order, const std::string& name);
extern void SendRefusalFriend(const int& order, const std::string& name);

CUIFriend::CUIFriend()
	: m_iPosX(0)
	, m_iPosY(110)
	, m_pBack(NULL)
	, m_pBtnClose(NULL)
	, m_pFriendList(NULL)
	, m_pFriendDenyList(NULL)
	, m_pTglRefusal(NULL)
	, m_bVisible(false)
{
	for (int i = eBtnListFriend; i < eBtnMax; ++i)
	{
		m_pBtn[i] = NULL;
	}

	LoadRes();
}

CUIFriend::~CUIFriend()
{
	DeleteRes();

	m_tbAddFriend.CloseIMC();
}

void CUIFriend::LoadRes()
{
	g_pDisplay->CreateSurfaceFromBitmap(&m_pBack, "INTERFACE/LETTERFRIEND/FRIEND_LIST_BACK.BMP");

	m_iPosX = g_pNk2DFrame->GetClientWidth() / 2;
	m_iPosX -= (m_pBack->GetWidth() / 2);
	m_iPosY = 80;

	m_pBack->SetPos(m_iPosX, m_iPosY); // CreateSurfaceFromBitmap 뒤에 해야함
	m_pBack->SetColorKey(RGB(0, 0, 0)); // CreateSurfaceFromBitmap 뒤에 해야함

	BtnLoadRes(m_iPosX, m_iPosY);

	m_tbAddFriend.Init(m_iPosX + 65, m_iPosY + 348, 24, RGB(255,255,255), RGB(255, 179, 0));
	m_tbAddFriend.SetfontSize();

	m_pFriendList = new CUIFriendList(m_iPosX + 16, m_iPosY + 99);
	m_pFriendDenyList = new CUIFriendDenyList(m_iPosX + 16, m_iPosY + 99);
}

void CUIFriend::BtnLoadRes( int posX, int posY )
{
	m_pBtnClose = new CBasicButton();
	m_pBtnClose->SetFileName("common/btn_close_01");
	m_pBtnClose->SetPosition(posX + 346, posY + 6);
	m_pBtnClose->LoadRes();

	for (int i = eBtnListFriend; i < eBtnMax; ++i)
	{
		m_pBtn[i] = new CBasicButton();
	}

	m_pBtn[eBtnListFriend]->SetFileName("LETTERFRIEND/BTN_FRIEND_LIST");
	m_pBtn[eBtnListFriend]->SetPosition(posX + 34, posY + 60);
	m_pBtn[eBtnListFriend]->SetState(BTN_DOWN);

	m_pBtn[eBtnListBlock]->SetFileName("LETTERFRIEND/BLOCK_LIST_TAB");
	m_pBtn[eBtnListBlock]->SetPosition(posX + 114, posY + 60);

	m_pBtn[eBtnAddFriend]->SetFileName("LETTERFRIEND/BTN_FRIEND_ADD");
	m_pBtn[eBtnAddFriend]->SetPosition(posX + 222, posY + 341);

	m_pBtn[eBtnAddBlock]->SetFileName("LETTERFRIEND/BTN_DENY_ADD"); // 임시
	m_pBtn[eBtnAddBlock]->SetPosition(posX + 222, posY + 341);

	for (int i = eBtnListFriend; i < eBtnMax; ++i)
	{
		m_pBtn[i]->LoadRes();
	}

	m_pTglRefusal = new CToggleButton();
	m_pTglRefusal->SetFileName("LETTERFRIEND/BTN_FRIEND_REFUSAL");
	m_pTglRefusal->SetPosition(posX + 270, posY + 51);
	m_pTglRefusal->LoadRes();
}

void CUIFriend::DeleteRes()
{
	for (int i = eBtnListFriend; i < eBtnMax; ++i)
	{
		SAFE_DELETE(m_pBtn[i]);
	}

	SAFE_DELETE(m_pTglRefusal);

	SAFE_DELETE(m_pBtnClose);
	SAFE_DELETE(m_pBack);

	SAFE_DELETE(m_pFriendList);
	SAFE_DELETE(m_pFriendDenyList);

	m_tbAddFriend.DeleteRes();
}

LRESULT CUIFriend::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	if( IsVisible() == false )
		return FALSE;

	const int pointX = LOWORD(lParam);
	const int pointY = HIWORD(lParam);

	if( m_tbAddFriend.GetEnable() && g_pNk2DFrame->GetControlBottom()->HasFocus() )
	{
		m_tbAddFriend.SetEnable(FALSE);
		pCMyApp->ConvertImmStatus();
	}

	BOOL bHeroStop = FALSE;

	if( m_pFriendList->GetVisible() == true )
		bHeroStop = m_pFriendList->MsgProc(hWnd, msg, wParam, lParam);
	if( m_pFriendDenyList->GetVisible() == true )
		bHeroStop = m_pFriendDenyList->MsgProc(hWnd, msg, wParam, lParam);

	switch (msg)
	{
	case WM_MOUSEMOVE:
		{
			if (!m_pBack->IsIn(pointX, pointY))
				return 0;

			m_pBtnClose->MsgProc(hWnd, msg, wParam, lParam);

			for (int i = eBtnAddFriend; i < eBtnMax; ++i)
				m_pBtn[i]->MsgProc(hWnd, msg, wParam, lParam);

		}
		break;

	case WM_LBUTTONDOWN:
		{
			if (!m_pBack->IsIn(pointX, pointY))
				return 0;

			int bInputEnable = m_tbAddFriend.GetEnable();

			if( E_FAIL != m_tbAddFriend.MsgProc( hWnd, msg, wParam, lParam ) )
				return TRUE;

			if( bInputEnable == FALSE && m_tbAddFriend.GetEnable() )
				pCMyApp->RestoreImmStatus();

			m_pBtnClose->MsgProc(hWnd, msg, wParam, lParam);

			for (int i = eBtnListFriend; i < eBtnMax; ++i)
			{
				if (m_pBtn[i]->IsInside(pointX, pointY))
				{
					m_pBtn[i]->MsgProc(hWnd, msg, wParam, lParam);
				}
			}

			if( m_pTglRefusal->IsInside(pointX, pointY) )
			{
				m_pTglRefusal->MsgProc(hWnd, msg, wParam, lParam);
			}

			bHeroStop = TRUE;

		}
		break;

	case WM_LBUTTONUP:
		{
			if (!m_pBack->IsIn(pointX, pointY))
				return 0;

			m_tbAddFriend.MsgProc(hWnd, msg, wParam, lParam);

			if (m_pBtnClose->IsInside(pointX, pointY) && m_pBtnClose->bDown)
			{
				m_pBtnClose->bDown = false;
				g_pNk2DFrame->ShowFriend(FALSE);

				bHeroStop = TRUE;
			}

			for (int i = eBtnListFriend; i < eBtnMax; ++i)
			{
				if (m_pBtn[i]->IsInside(pointX, pointY))
				{
					m_pBtn[i]->MsgProc(hWnd, msg, wParam, lParam);
				}
			}

			if (m_pBtn[eBtnListFriend]->GetState() == BTN_ACTION)
			{
				m_pBtn[eBtnListFriend]->SetState(BTN_DOWN);
				m_pBtn[eBtnListBlock]->SetState(BTN_NORMAL);
				m_pFriendList->SetVisible(true);
				m_pFriendDenyList->SetVisible(false);
				bHeroStop = TRUE;
			}

			if (m_pBtn[eBtnListBlock]->GetState() == BTN_ACTION)
			{
				m_pBtn[eBtnListBlock]->SetState(BTN_DOWN);
				m_pBtn[eBtnListFriend]->SetState(BTN_NORMAL);
				m_pFriendList->SetVisible(false);
				m_pFriendDenyList->SetVisible(true);
				bHeroStop = TRUE;
			}

			if( m_pFriendList->GetVisible() )
			{
				if( m_pBtn[eBtnAddFriend]->IsInside(pointX, pointY) )
				{
					m_pBtn[eBtnAddFriend]->bDown = false;
					SendAddFriendReq();
				}
			}

			if( m_pFriendDenyList->GetVisible() )
			{
				if( m_pBtn[eBtnAddBlock]->IsInside(pointX, pointY) )
				{
					m_pBtn[eBtnAddBlock]->bDown = false;
					SendAddDenyReq();
				}
			}

			if( m_pTglRefusal->IsInside(pointX, pointY) )
			{
				m_pTglRefusal->MsgProc(hWnd, msg, wParam, lParam);
				SendRefusalReq(m_pTglRefusal->GetToggleState());
			}

		}
		break;

	case WM_KEYDOWN:
	case WM_CHAR:
	case WM_IME_STARTCOMPOSITION:
	case WM_IME_COMPOSITION:
	case WM_IME_ENDCOMPOSITION:
	case WM_IME_CHAR:
	case WM_IME_NOTIFY:
		{
			if( m_tbAddFriend.GetEnable() )
				m_tbAddFriend.MsgProc( hWnd, msg, wParam, lParam );

			if( m_tbAddFriend.GetEnable() )
				return TRUE;


		}
		break;

	default:
		break;
	}

	return bHeroStop;
}

void CUIFriend::Draw()
{
	if( IsVisible() == false )
		return;

	g_pDisplay->Blt(m_pBack->Xpos, m_pBack->Ypos, m_pBack);

	for (int i = eBtnListFriend; i < eBtnAddFriend; ++i)
	{
		m_pBtn[i]->Draw();
	}

	m_pTglRefusal->Draw();

	m_tbAddFriend.Draw();

	if( m_pFriendList->GetVisible() )
	{
		m_pBtn[eBtnAddFriend]->Draw();
		m_pFriendList->Draw();
	}

	if( m_pFriendDenyList->GetVisible() )
	{
		m_pBtn[eBtnAddBlock]->Draw();
		m_pFriendDenyList->Draw();
	}

	POINT point;
	GetCursorPos(&point);
	ScreenToClient(g_pDisplay->GetHWnd(), &point);

	if( pCMyApp->m_pMouse && m_pBack->IsIn(point.x, point.y) )
		pCMyApp->m_pMouse->SetMouseType(M_NORMAL);
}

bool CUIFriend::IsFocus()
{
	return m_tbAddFriend.GetEnable() == 0 ? false : true;
}

bool CUIFriend::AddFriend(stFriend newFriend)
{
	if( m_pFriendList == NULL )
		return false;

	return m_pFriendList->AddFriend(newFriend);
}

bool CUIFriend::RemoveFriend(const int& index)
{
	if( m_pFriendList == NULL )
		return false;

	return m_pFriendList->RemoveFriend(index);
}

bool CUIFriend::ChangeLogin(const int& index, const bool& login)
{
	if( m_pFriendList == NULL )
		return false;

	return m_pFriendList->ChangeLogin(index, login);
}

bool CUIFriend::ChangeChannel(const int& index, const int& channel)
{
	if( m_pFriendList == NULL )
		return false;

	return m_pFriendList->ChangeChannel(index, channel);
}

bool CUIFriend::ChangeZone(const int& index, const int& zone)
{
	if( m_pFriendList == NULL )
		return false;

	return m_pFriendList->ChangeZone(index, zone);
}

bool CUIFriend::AddDeny(stFriend newDeny)
{
	if( m_pFriendDenyList == NULL )
		return false;

	return m_pFriendDenyList->AddFriend(newDeny);
}

bool CUIFriend::RemoveDeny(const int& index)
{
	if( m_pFriendDenyList == NULL )
		return false;

	return m_pFriendDenyList->RemoveFriend(index);
}

bool CUIFriend::SetRefusal(BOOL bRefusal)
{
	if( m_pTglRefusal == NULL )
		return false;

	m_pTglRefusal->SetToggleState(bRefusal);

	return true;
}

void CUIFriend::SendAddFriendReq()
{
	std::string name = m_tbAddFriend.GetString();
	unsigned int len = name.length();

	if( len == 0 )
	{
		return;
	}
	else if( len > 16 )
	{
		name = name.substr(0, 16);
	}

	SendAddFriend(1, name);
}

void CUIFriend::SendAddDenyReq()
{
	std::string name = m_tbAddFriend.GetString();
	unsigned int len = name.length();

	if( len == 0 )
	{
		return;
	}
	else if( len > 16 )
	{
		name = name.substr(0, 16);
	}

	SendRefusalFriend(3, name);
}

void CUIFriend::SendRefusalReq(BOOL bRefusal)
{
	std::string name = "NULL";

	if( bRefusal == TRUE )
	{
		SendRefusalFriend(1, name);
	}
	else
	{
		SendRefusalFriend(2, name);
	}
}

void CUIFriend::SetVisible(BOOL visible)
{
	m_bVisible = visible;
	if( m_pFriendList != NULL )
	{
		bool bVisible = visible == TRUE ? true : false;
		m_pFriendList->SetVisible(bVisible);
	}

	if( m_pFriendDenyList != NULL )
	{
		m_pFriendDenyList->SetVisible(false);
	}

	m_pBtn[eBtnListFriend]->SetState(BTN_DOWN);
	m_pBtn[eBtnListBlock]->SetState(BTN_NORMAL);
}

HRESULT CUIFriend::RestoreSurfaces()
{
	m_tbAddFriend.RestoreSurfaces();

	if( m_pFriendList )
		m_pFriendList->RestoreSurfaces();

	if( m_pFriendDenyList )
		m_pFriendDenyList->RestoreSurfaces();

	return S_OK;
}