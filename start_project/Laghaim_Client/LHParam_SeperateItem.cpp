#include "stdafx.h"
#include ".\lhparam_seperateitem.h"
#include "LHExcelReader.h"
#include "UIMgr.h"

const char * SeparateItem_Filename = "data/item/separateitem_table.txt";

LHParam_SeperateItem::LHParam_SeperateItem(void)
{
}

LHParam_SeperateItem::~LHParam_SeperateItem(void)
{
	safe_delete_associate(m_DataMap);
}

bool LHParam_SeperateItem::LoadData()
{
	LHExcelReader er;

	if( !er.Open(SeparateItem_Filename) )
	{
		assert( false && "SeperateItem 파일을 찾을수 없습니다." );
		return false;
	}

	for( int row=0 ; row<er.GetRowSize() ; ++row )
	{
		Data * pData = new Data();

		int col = 0;

		pData->m_index			= er.GetInt(row, col);
		pData->m_enable			= er.GetBool(row, ++col);

		if ( false == pData->m_enable )
		{
			delete pData;
			pData = NULL;
			continue;
		}

		m_DataMap.insert(Data_Map_it_vt(pData->m_index, pData));
	}

	return true;
}

bool LHParam_SeperateItem::CheckNeedItem()
{
	assert( NULL != g_pRoh );

	if( true == m_DataMap.empty() )
	{
		if( false == LoadData() )
			return false;
	}

	CItem * pItem = g_pRoh->m_ExgInven;

	if ( NULL == pItem )
	{
		return false;
	}

	if ( NULL != pItem->m_Next )
	{
		return false;
	}

	Data_Map_it it = m_DataMap.find( pItem->m_Vnum );

	if( it != m_DataMap.end() )
	{
		Data * pData = (*it).second;
		if ( false == pData->m_enable )
		{
			return false;
		}

		if ( TRUE == pItem->m_bTimeLimit )
		{
			return false;
		}

		return true;
	}

	return false;
}
