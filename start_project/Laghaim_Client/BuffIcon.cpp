#include "stdafx.h"
#include "Bufficon.h"
#include <algorithm>
#include <sstream>
#include <iomanip>
#include "main.h"
#include "Nk2DFrame.h"
#include "Compas.h"
#include "AlphaPopUp.h"

CBuffIcon::CBuffIcon()
	: m_ImageMap(), m_BuffList(), m_IconSet()
	, m_pPopup(NULL)
{
	Init();
}

CBuffIcon::~CBuffIcon()
{
	DeleteRes();
	SAFE_DELETE(m_pPopup);
}

void CBuffIcon::Init()
{
	m_BuffList.clear();
	m_IconSet.clear();
	LoadRes();

	m_pPopup = new CAlphaPopUp;
	m_pPopup->Init(12);
	m_pPopup->SetIndex(0);

	if (m_pPopup)
		m_pPopup->LoadRes(g_pNk2DFrame->GetClientWidth(), g_pNk2DFrame->GetClientHeight());
}

void CBuffIcon::LoadRes()
{
	for (int i = 0; i < EBT_COUNT; ++i)
	{
		// 공방경 복합 버프는 따로 아이콘이 없음
		if (i >= EBT_INCREASE_AE && i <= EBT_INCREASE_ADE)
			continue;

		std::stringstream temp;
		temp << "BUFFICON/BUFF_" << std::setw(2) << std::setfill('0') << i;
		m_ImageMap[i].Init(temp.str().c_str());
	}
}

void CBuffIcon::DeleteRes()
{
	m_ImageMap.clear();
}

void CBuffIcon::Draw()
{
	if( !g_pDisplay || !pCMyApp )
		return;

	if (m_IconSet.empty())
		return;

	POINT point;
	GetCursorPos(&point);
	ScreenToClient(g_pDisplay->GetHWnd(), &point);

	int cx = g_pNk2DFrame->GetClientWidth();
	int cy = pCMyApp->m_pCompas->GetHalfHeight() * 2 + 5;
	int ox = 1;
	int oy = 0;

	m_pPopup->SetIndex(0);

	IconSet::iterator iter = m_IconSet.begin();
	while (iter != m_IconSet.end())
	{
		ImageMap::mapped_type& icon = m_ImageMap[*iter];

		int x = cx - ox - (ox * icon.m_w);
		int y = cy + oy + (icon.m_h * oy);

		iter->tick();

		if (iter->isVisible())
		{
			icon.Draw(x, y);
			//g_pDisplay->OpacityBlt(x, y, x + icon.m_w, y + icon.m_h, icon.m_surface->GetDDrawSurface(), &icon.m_rcDraw, 127);
		}

		RECT rect = {x, y, x + icon.m_w, y + icon.m_h};

		if (PtInRect(&rect, point))
		{
			char str[32] = {0, };
			strcpy(str, iter->getInfo2Text().c_str());
			CMultiText text;
			text.AddString(str);
			m_pPopup->PutString(&text);
			m_pPopup->SetIndex(1);
		}

		if (++oy > 9)
		{
			++ox;
			oy = 0;

			if (ox > 2)
				break;
		}

		++iter;
	}

	if (m_pPopup && m_pPopup->GetIndex() == 1)
		m_pPopup->Draw(point.x, point.y);
}

void CBuffIcon::addBuff(eBuffTypes type, int rate, unsigned int time)
{
	// out of range!
	if (type < 0 || type >= EBT_COUNT)
		return;

	if (m_BuffList.find(type) != m_BuffList.end())
		return;

	m_BuffList[type] = CBuffInfo(EBS_AFFECTED, type, rate, time);

	_addBuffIcon(m_BuffList[type]);
}

void CBuffIcon::refreshBuff(eBuffTypes type, unsigned int time)
{
	BuffList::iterator buff = m_BuffList.find(type);
	buff->second.refresh(time);
}

void CBuffIcon::removeBuff(eBuffTypes type)
{
	BuffList::iterator buff = m_BuffList.find(type);

	/*std::binder2nd<eraseBuff> eraser(std::bind2nd(eraseBuff(), buff->second));
	IconSet::iterator removes = std::remove_if(m_IconSet.begin(), m_IconSet.end(), eraser);
	m_IconSet.erase(removes, m_IconSet.end());*/

	IconSet::iterator itIcon = m_IconSet.begin();
	IconSet::iterator itEnd = m_IconSet.end();

	for( ; itIcon != itEnd; )
	{
		if( *itIcon == buff->second )
		{
			m_IconSet.erase(itIcon++);
		}
		else
		{
			itIcon++;
		}
	}

	m_BuffList.erase(buff);
}

void CBuffIcon::removeAll()
{
	m_BuffList.clear();
	m_IconSet.clear();
}

LRESULT CBuffIcon::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return 0;
}

void CBuffIcon::_addBuffIcon(CBuffInfo& buff)
{
	char str[32] = {0,};
	eBuffTypes type = buff;

	switch (type)
	{
	case EBT_INCREASE_AE :
		m_IconSet.insert(CBuff(&buff, EBT_INCREASE_ATT));
		m_IconSet.insert(CBuff(&buff, EBT_INCREASE_INCOMMING_EXP));
		break;

	case EBT_INCREASE_DE :
		m_IconSet.insert(CBuff(&buff, EBT_INCREASE_DEF));
		m_IconSet.insert(CBuff(&buff, EBT_INCREASE_INCOMMING_EXP));
		break;

	case EBT_INCREASE_AD :
		m_IconSet.insert(CBuff(&buff, EBT_INCREASE_ATT));
		m_IconSet.insert(CBuff(&buff, EBT_INCREASE_DEF));
		break;

	case EBT_INCREASE_ADE :
		m_IconSet.insert(CBuff(&buff, EBT_INCREASE_ATT));
		m_IconSet.insert(CBuff(&buff, EBT_INCREASE_DEF));
		m_IconSet.insert(CBuff(&buff, EBT_INCREASE_INCOMMING_EXP));
		break;

	default :
		m_IconSet.insert(CBuff(&buff));
		break;
	}
}