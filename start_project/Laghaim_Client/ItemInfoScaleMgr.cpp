#include "stdafx.h"
#include "LHExcelReader.h"
#include "ItemInfoScaleMgr.h"

ItemInfoScaleMgr::ItemInfoScaleMgr()
{
	load();
}

ItemInfoScaleMgr::~ItemInfoScaleMgr()
{
}

void ItemInfoScaleMgr::load()
{
	LHExcelReader er;
	if( er.Open("data/ITEM_INFO_SCALE.TXT") == false )
		return;
		
	int vnum = 0;
	int col = 0;
	int rowCount = er.GetRowSize();
	for(int i = 0; i < rowCount; i++)
	{
		col = 0;
		vnum = er.GetInt(i, col++);

		m_map.insert(std::make_pair(vnum, 0));
	}
}

bool ItemInfoScaleMgr::IsNoScale(const int vnum)
{
	auto itFind = m_map.find(vnum);
	if( itFind == m_map.end() )
	{
		return false;
	}

	return true;
}

