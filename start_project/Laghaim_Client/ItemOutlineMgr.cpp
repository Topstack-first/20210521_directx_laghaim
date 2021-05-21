#include "stdafx.h"
#include "ItemOutlineMgr.h"
#include "LHExcelReader.h"
#include "IndexedTexture.h"

ItemOutlineMgr::ItemOutlineMgr()
{

}

ItemOutlineMgr::~ItemOutlineMgr()
{

}

void ItemOutlineMgr::Load()
{
	m_mapItemOutline.clear();

	LHExcelReader er;
	if( er.Open("data/ITEMFORCE_TABLE.TXT") == false )
		return;

	int col = 0;
	int wear = 0;
	ItemOutlineInfo info;
	int rowCount = er.GetRowSize();
	for(int i = 0; i < rowCount; i++)
	{
		col = 0;

		wear = er.GetInt(i, col++);
		info.width = er.GetFloat(i, col++);

		for(int j = 0; j < ITEM_GRADE_COUNT; j++)
		{
			info.grade[j] = er.GetInt(i, col++);
			info.color[j] = er.GetHex(i, col++);
		}

		addVal(wear, info);
	}

	er.Close();
}

void ItemOutlineMgr::Reload()
{
	Load();
}

void ItemOutlineMgr::addVal(const int& wear, const ItemOutlineInfo& info)
{
	MAP_ITEM_OUTLINE::iterator itFind = m_mapItemOutline.find(wear);
	if( itFind != m_mapItemOutline.end() )
		return;

	m_mapItemOutline.insert(std::make_pair(wear, info));
}

float ItemOutlineMgr::GetWidth(int wear)
{
	MAP_ITEM_OUTLINE::iterator itFind = m_mapItemOutline.find(wear);
	if( itFind == m_mapItemOutline.end() )
		return 0;

	return (*itFind).second.width;
}

DWORD ItemOutlineMgr::GetColor(int wear, int forceLevel)
{
	MAP_ITEM_OUTLINE::iterator itFind = m_mapItemOutline.find(wear);
	if( itFind == m_mapItemOutline.end() )
		return 0;

	DWORD dwRet = 0;
	ItemOutlineInfo info = (*itFind).second;
	for(int i = 0; i < ITEM_GRADE_COUNT; i++)
	{
		if( forceLevel >= info.grade[i] )
		{
			dwRet = info.color[i];
		}
	}

	return dwRet;
}

