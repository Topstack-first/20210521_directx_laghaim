#include "stdafx.h"
#include "UIFriendList_Base.h"
#include "UIFriendLine.h"
#include "DataFriend.h"
#include "ControlPopupMenu.h"

CUIFriendList_Base::CUIFriendList_Base(int x, int y)
	: m_iPosX(x)
	, m_iPosY(y)
	, m_bVisible(false)
	, m_pPopupMenu(NULL)
{
}

CUIFriendList_Base::~CUIFriendList_Base()
{
}

void CUIFriendList_Base::LoadRes()
{
	m_Scroll.LoadRes("common/scroll_up_t01", "common/scroll_dn_t01", "interface/login/serverlist_scbar.bmp");
	m_Scroll.SetDrawPos(m_iPosX + 314, m_iPosY, 228);
	m_Scroll.MoveGap = 1;
}

void CUIFriendList_Base::DeleteRes()
{
	SAFE_DELETE(m_pPopupMenu);
	m_Scroll.DeleteRes();

	VEC_FRIEND_INFO::iterator itBeg = m_vecFriend.begin();
	VEC_FRIEND_INFO::iterator itEnd = m_vecFriend.end();
	stFriend info;
	for( ; itBeg != itEnd; itBeg++ )
	{
		SAFE_DELETE((*itBeg));
	}
}

LRESULT CUIFriendList_Base::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	const int pointX = LOWORD(lParam);
	const int pointY = HIWORD(lParam);

	if (!IsInside(pointX, pointY))
	{
		return FALSE;
	}

	unsigned int sizeList = m_vecFriend.size();

	if( sizeList > eFriendListLineMax)
	{
		if( m_pPopupMenu != NULL && m_pPopupMenu->GetVisible() == false )
		{
			if( E_FAIL != m_Scroll.MsgProc(hWnd, msg, wParam, lParam) )
				return TRUE;
		}
	}

	LRESULT bResult = FALSE;

	switch (msg)
	{
	case WM_MOUSEMOVE:
		{
			if( m_pPopupMenu != NULL && m_pPopupMenu->GetVisible() == true )
				bResult = m_pPopupMenu->MsgProc(hWnd, msg, wParam, lParam);

			break;
		}
	case WM_LBUTTONDOWN:
		{
			if( m_pPopupMenu != NULL && m_pPopupMenu->GetVisible() == true )
				bResult = m_pPopupMenu->MsgProc(hWnd, msg, wParam, lParam);

			break;
		}
	case WM_LBUTTONUP:
		{
			if( m_pPopupMenu != NULL && m_pPopupMenu->GetVisible() == true )
				bResult = m_pPopupMenu->MsgProc(hWnd, msg, wParam, lParam);

			break;
		}
	case WM_RBUTTONUP:
		{
			OpenPopupMenu(pointX, pointY);

			break;
		}
	case 0x020A: // WM_MOUSEWHEEL
		{
			if( m_pPopupMenu != NULL && m_pPopupMenu->GetVisible() == true )
				return FALSE;

			if( sizeList > eFriendListLineMax)
			{
				if( (short)HIWORD(wParam) < 0 )
				{
					m_Scroll.IncPos();
					return TRUE;
				}
				else if( (short)HIWORD(wParam) > 0 )
				{
					m_Scroll.DecPos();
					return TRUE;
				}
			}

			break;
		}
	default:
		break;
	}

	return bResult;
}

void CUIFriendList_Base::Draw()
{
	DrawScroll();

	if( m_pPopupMenu != NULL && m_pPopupMenu->GetVisible() == true )
		m_pPopupMenu->Draw();
}

bool CUIFriendList_Base::isExistFriend(const int& index)
{
	VEC_FRIEND_INFO::iterator itBeg = m_vecFriend.begin();
	VEC_FRIEND_INFO::iterator itEnd = m_vecFriend.end();
	stFriend info;
	for( ; itBeg != itEnd; itBeg++ )
	{
		info = (*itBeg)->GetInfo();
		if( info.index == index )
			return true;
	}

	return false;
}

void CUIFriendList_Base::DrawScroll()
{
	if( m_vecFriend.size() > eFriendListLineMax)
	{
		m_Scroll.Draw();
	}
}

bool CUIFriendList_Base::IsInside(const int pointX, const int pointY)
{
	if (m_iPosX <= pointX && pointX <= m_iPosX + 332)
	{
		if (m_iPosY <= pointY && pointY <= m_iPosY + 228)
		{
			return true;
		}
	}

	return false;
}

void CUIFriendList_Base::OpenPopupMenu(int posX, int posY)
{
	int offsetX = posX - (m_iPosX + 1);
	int offsetY = posY - (m_iPosY + 4);
	if( offsetX < 0 || offsetY < 0 || offsetX > 365 || offsetY > 380)
		return;

	int lineNum = m_Scroll.NowPos + (offsetY / eLineHeight);
	if( lineNum >= m_vecFriend.size() )
		return;

	if( m_pPopupMenu == NULL )
		return;

	bool bOpenDown = true;
	if( offsetY > eLineHeight * (eFriendListLineMax / 2) )
		bOpenDown = false;

	m_pPopupMenu->SetIndex(m_vecFriend[lineNum]->GetIndex());
	m_pPopupMenu->SetPosition(posX, posY, bOpenDown);
	m_pPopupMenu->SetVisible(true);
}

void CUIFriendList_Base::ClosePopupMenu()
{
	if( m_pPopupMenu == NULL )
		return;

	m_pPopupMenu->SetVisible(false);
}

void CUIFriendList_Base::SetVisible(const bool& visible)
{
	m_bVisible = visible;
	ClosePopupMenu();
}

HRESULT CUIFriendList_Base::RestoreSurfaces()
{
	unsigned int size = m_vecFriend.size();
	for(int i = 0; i < size; i++)
	{
		if( m_vecFriend[i] )
			m_vecFriend[i]->RestoreSurfaces();
	}

	return S_OK;
}