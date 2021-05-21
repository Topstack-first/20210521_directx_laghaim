#include "stdafx.h"
#include "ItemFakeOptionMgr.h"
#include "LHExcelReader.h"

ItemFakeOptionMgr::ItemFakeOptionMgr()
{

}

ItemFakeOptionMgr::~ItemFakeOptionMgr()
{

}

bool ItemFakeOptionMgr::HasOption(int type, const int& vnum)
{
	MAP_ITEM_HAS_OPTION::iterator itFindType = m_mapHasOption.find(type);
	if( itFindType == m_mapHasOption.end() )
		return false;

	MAP_ITEM_INDEX mapItemIndex = (*itFindType).second;
	MAP_ITEM_INDEX::iterator itFindIndex = mapItemIndex.find(vnum);
	if( itFindIndex == mapItemIndex.end() )
		return false;

	return true;
}

void ItemFakeOptionMgr::Load()
{
	LHExcelReader er;
	if( er.Open("data/ITEMOPTION_TABLE.TXT") == false )
		return;

	int col = 0;
	int type = 0;
	int vnum = 0;
	int rowCount = er.GetRowSize();
	for(int i = 0; i < rowCount; i++)
	{
		col = 0;

		type = er.GetInt(i, col++);
		vnum = er.GetInt(i, col++);

		addVal(type, vnum);
	}

	er.Close();
}

void ItemFakeOptionMgr::addVal(const int& type, const int& vnum)
{
	MAP_ITEM_INDEX mapItemIndex;

	MAP_ITEM_HAS_OPTION::iterator itFindType = m_mapHasOption.find(type);
	if( itFindType != m_mapHasOption.end() )
	{
		mapItemIndex = (*itFindType).second;
	}

	MAP_ITEM_INDEX::iterator itFindIndex = mapItemIndex.find(vnum);
	if( itFindIndex != mapItemIndex.end() )
		return;

	mapItemIndex.insert(std::make_pair(vnum, 1));

	m_mapHasOption[type] = mapItemIndex;
}