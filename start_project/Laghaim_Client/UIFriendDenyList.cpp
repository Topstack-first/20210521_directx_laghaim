#include "stdafx.h"
#include <algorithm>
#include "UIFriendLine.h"
#include "UIFriendDenyList.h"
#include "UIFriendDenyListPopupMenu.h"

bool CompareFriendDenyListSort(CUIFriendLine* l, CUIFriendLine* r)
{
	stFriend lf = l->GetInfo();
	stFriend rf = r->GetInfo();

	if( lf.name < rf.name )
		return true;

	return false;
}

CUIFriendDenyList::CUIFriendDenyList(int x, int y)
	: CUIFriendList_Base(x, y)
{
	LoadRes();
}

CUIFriendDenyList::~CUIFriendDenyList()
{
	DeleteRes();
}

void CUIFriendDenyList::LoadRes()
{
	CUIFriendList_Base::LoadRes();

	m_pPopupMenu = new UIFriendDenyListPopupMenu();
	m_pPopupMenu->SetPosition(m_iPosX, m_iPosY, true);
}

void CUIFriendDenyList::DeleteRes()
{
	CUIFriendList_Base::DeleteRes();
}

bool CUIFriendDenyList::AddFriend(stFriend newDeny)
{
	if( m_pPopupMenu != NULL )
		m_pPopupMenu->SetVisible(false);

	if( m_vecFriend.size() >= eFriendListMax )
		return false;

	if( newDeny.index <= 0 )
		return false;

	if( isExistFriend(newDeny.index) == true )
		return false;

	CUIFriendLine* line = new CUIFriendLine();
	newDeny.deny = true;
	line->SetInfo(newDeny);
	line->ResetChannelTxt(0, RGB(128, 128, 128), true);
	line->ResetZoneTxt(0, RGB(128, 128, 128), true);
	m_vecFriend.push_back(line);

	unsigned int size = m_vecFriend.size();
	if( size < eFriendListLineMax )
		m_Scroll.SetRange(1);
	else
		m_Scroll.SetRange(m_vecFriend.size() - eFriendListLineMax + 1);

	SortList();

	return true;
}

bool CUIFriendDenyList::RemoveFriend(const int& index)
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

void CUIFriendDenyList::Draw()
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
		TextPosY += eLineHeight;
	}

	CUIFriendList_Base::Draw();
}

void CUIFriendDenyList::SortList()
{
	std::sort(m_vecFriend.begin(), m_vecFriend.end(), CompareFriendDenyListSort);
}