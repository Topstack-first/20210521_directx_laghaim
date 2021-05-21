#include "stdafx.h"
#include <algorithm>
#include "UIFriendLine.h"
#include "UIFriendList.h"
#include "UIFriendListPopupMenu.h"

bool CompareFriendListSortByLogin(CUIFriendLine* l, CUIFriendLine* r)
{
	stFriend lf = l->GetInfo();
	stFriend rf = r->GetInfo();

	if( lf.login >= rf.login )
		return true;

	return false;
}

bool CompareFriendListSortByName(CUIFriendLine* l, CUIFriendLine* r)
{
	stFriend lf = l->GetInfo();
	stFriend rf = r->GetInfo();

	if( lf.name < rf.name )
		return true;

	return false;
}

CUIFriendList::CUIFriendList( int x, int y )
	: CUIFriendList_Base(x, y)
	, m_pOnlineIcon(NULL)
	, m_pOfflineIcon(NULL)
{
	LoadRes();
}

CUIFriendList::~CUIFriendList(void)
{
	DeleteRes();
}

void CUIFriendList::LoadRes()
{
	CUIFriendList_Base::LoadRes();

	m_pPopupMenu = new UIFriendListPopupMenu();
	m_pPopupMenu->SetPosition(m_iPosX, m_iPosY, true);

	ListLoadRes();
}

void CUIFriendList::ListLoadRes()
{
	g_pDisplay->CreateSurfaceFromBitmap(&m_pOnlineIcon, "INTERFACE/LETTERFRIEND/ONLINE_ICON.BMP");
	m_pOnlineIcon->SetColorKey(RGB(0, 0, 0)); // CreateSurfaceFromBitmap 뒤에 해야함

	g_pDisplay->CreateSurfaceFromBitmap(&m_pOfflineIcon, "INTERFACE/LETTERFRIEND/OFFLINE_ICON.BMP");
	m_pOfflineIcon->SetColorKey(RGB(0, 0, 0)); // CreateSurfaceFromBitmap 뒤에 해야함
}

void CUIFriendList::DeleteRes()
{
	SAFE_DELETE(m_pOnlineIcon);
	SAFE_DELETE(m_pOfflineIcon);

	CUIFriendList_Base::DeleteRes();
}

bool CUIFriendList::AddFriend(stFriend newFriend)
{
	if( m_pPopupMenu != NULL )
		m_pPopupMenu->SetVisible(false);

	if( m_vecFriend.size() >= eFriendListMax )
		return false;

	if( newFriend.index <= 0 )
		return false;

	if( isExistFriend(newFriend.index) == true )
		return false;

	CUIFriendLine* line = new CUIFriendLine();
	line->SetInfo(newFriend);
	m_vecFriend.push_back(line);

	unsigned int size = m_vecFriend.size();
	if( size < eFriendListLineMax )
		m_Scroll.SetRange(1);
	else
		m_Scroll.SetRange(m_vecFriend.size() - eFriendListLineMax + 1);

	SortList();

	return true;
}

bool CUIFriendList::RemoveFriend(const int& index)
{
	if( m_pPopupMenu != NULL )
		m_pPopupMenu->SetVisible(false);

	if( isExistFriend(index) == false )
		return false;

	VEC_FRIEND_INFO::iterator itBeg = m_vecFriend.begin();
	VEC_FRIEND_INFO::iterator itEnd = m_vecFriend.end();
	stFriend info;
	for( ; itBeg != itEnd; itBeg++ )
	{
		info = (*itBeg)->GetInfo();
		if( info.index == index )
		{
			SAFE_DELETE((*itBeg));
			m_vecFriend.erase(itBeg);

			unsigned int size = m_vecFriend.size();
			if( size < eFriendListLineMax )
				m_Scroll.SetRange(1);
			else
				m_Scroll.SetRange(m_vecFriend.size() - eFriendListLineMax + 1);

			m_Scroll.NowPos = 0;

			SortList();

			return true;
		}
	}

	return false;
}

bool CUIFriendList::ChangeLogin(const int& index, const bool& login)
{
	if( m_pPopupMenu != NULL )
		m_pPopupMenu->SetVisible(false);

	if( isExistFriend(index) == false )
		return false;

	VEC_FRIEND_INFO::iterator itBeg = m_vecFriend.begin();
	VEC_FRIEND_INFO::iterator itEnd = m_vecFriend.end();
	stFriend info;
	for( ; itBeg != itEnd; itBeg++ )
	{
		info = (*itBeg)->GetInfo();
		if( info.index == index )
		{
			(*itBeg)->ChangeLogin(login);
			SortList();
			return true;
		}
	}

	return false;
}

bool CUIFriendList::ChangeChannel(const int& index, const int& channel)
{
	if( isExistFriend(index) == false )
		return false;

	VEC_FRIEND_INFO::iterator itBeg = m_vecFriend.begin();
	VEC_FRIEND_INFO::iterator itEnd = m_vecFriend.end();
	stFriend info;
	for( ; itBeg != itEnd; itBeg++ )
	{
		info = (*itBeg)->GetInfo();
		if( info.index == index )
		{
			(*itBeg)->ChangeChannel(channel);
			return true;
		}
	}

	return false;
}

bool CUIFriendList::ChangeZone(const int& index, const int& zone)
{
	if( isExistFriend(index) == false )
		return false;

	VEC_FRIEND_INFO::iterator itBeg = m_vecFriend.begin();
	VEC_FRIEND_INFO::iterator itEnd = m_vecFriend.end();
	stFriend info;
	for( ; itBeg != itEnd; itBeg++ )
	{
		info = (*itBeg)->GetInfo();
		if( info.index == index )
		{
			(*itBeg)->ChangeZone(zone);
			return true;
		}
	}

	return false;
}

void CUIFriendList::Draw()
{
	int TextPosX = m_iPosX + eTextListGapX;
	int TextPosY = m_iPosY + eTextListGapY;

	int iStartIndex = m_Scroll.GetPos();

	unsigned int size = m_vecFriend.size();
	for( int i = iStartIndex; i < size; ++i )
	{
		if( i >= iStartIndex + eFriendListLineMax )
			break;

		m_vecFriend[i]->Draw(15 + TextPosX, TextPosY);
		g_pDisplay->Blt(TextPosX, TextPosY, m_vecFriend[i]->GetLogin() ? m_pOnlineIcon : m_pOfflineIcon);
		TextPosY += eLineHeight;
	}

	CUIFriendList_Base::Draw();
}

void CUIFriendList::SortList()
{
	VEC_FRIEND_INFO vLogin;
	VEC_FRIEND_INFO vLogout;

	unsigned int size = m_vecFriend.size();
	for( int i = 0; i < size; i++ )
	{
		if( m_vecFriend[i]->GetLogin() == true )
			vLogin.push_back(m_vecFriend[i]);
		else
			vLogout.push_back(m_vecFriend[i]);
	}

	std::sort(vLogin.begin(), vLogin.end(), CompareFriendListSortByName);
	std::sort(vLogout.begin(), vLogout.end(), CompareFriendListSortByName);

	m_vecFriend.clear();

	size = vLogin.size();
	for( int i = 0; i < size; i++ )
	{
		m_vecFriend.push_back(vLogin[i]);
	}
	size = vLogout.size();
	for( int i = 0; i < size; i++ )
	{
		m_vecFriend.push_back(vLogout[i]);
	}
}
