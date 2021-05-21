#include "stdafx.h"
#include "SetItemEffMgr.h"

SetItemEffMgr::SetItemEffMgr()
{
}

SetItemEffMgr::~SetItemEffMgr()
{
}

void SetItemEffMgr::Clear()
{
	for(int i = 0; i < WEARING_NUM; i++)
	{
		m_mapEff[i].clear();
	}
}

void SetItemEffMgr::AddEffect(const int* itemIndices, const int statusCount, const SetItemStatus* statuses)
{
	int itemIndex = -1;
	for(int i = 0; i < WEARING_NUM; i++)
	{
		itemIndex = itemIndices[i];
		if( itemIndex == -1 )
		{
			continue;
		}

		SetItemStatus tmpStatus;
		std::vector<SetItemStatus> vecStatus;

		MAP_SETITEM::iterator itFind = m_mapEff[i].find(itemIndex);
		if( itFind != m_mapEff[i].end() )
		{
			vecStatus = itFind->second;
		}

		for(int j = 0; j < statusCount; j++)
		{
			tmpStatus = statuses[j];
			vecStatus.push_back(tmpStatus);
		}

		m_mapEff[i][itemIndex] = vecStatus;
	}
}

void SetItemEffMgr::RemoveEffect(const int* itemIndices)
{
	int itemIndex = -1;
	for(int i = 0; i < WEARING_NUM; i++)
	{
		itemIndex = itemIndices[i];
		if( itemIndex == -1 )
		{
			continue;
		}

		MAP_SETITEM::iterator itFind = m_mapEff[i].find(itemIndex);
		if( itFind == m_mapEff[i].end() )
		{
			continue;
		}

		m_mapEff[i].erase(itFind);
	}
}

std::vector< std::pair<int, int> > SetItemEffMgr::GetEffect(int itemIndex)
{
	int type = -1;
	int val = -1;
	std::vector< std::pair<int, int> > vecReturn;

	for(int i = 0; i < WEARING_NUM; i++)
	{
		MAP_SETITEM::iterator itFind = m_mapEff[i].find(itemIndex);
		if( itFind == m_mapEff[i].end() )
		{
			continue;
		}

		std::vector<SetItemStatus> vecStatus = itFind->second;
		unsigned int cntStatus = vecStatus.size();
		for(unsigned int j = 0; j < cntStatus; j++)
		{
			SetItemStatus stat = vecStatus[j];
			vecReturn.push_back(std::make_pair<int, int>(stat.nType, stat.nValue_));
		}
	}

	return vecReturn;
}