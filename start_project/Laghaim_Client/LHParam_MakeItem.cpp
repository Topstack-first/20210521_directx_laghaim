#include "stdafx.h"
#include "LHParam_MakeItem.h"
#include "LHExcelReader.h"
#include "UIMgr.h"

const char * MakeItem_Filename = "data/item/makeitem_table.txt";

LHParam_MakeItem::LHParam_MakeItem(void)
{
}

LHParam_MakeItem::~LHParam_MakeItem(void)
{
	safe_delete_associate(m_DataMap);
}

bool LHParam_MakeItem::LoadData()
{
	LHExcelReader er;

	if( !er.Open(MakeItem_Filename) )
	{
		assert( false && "MakeItem File not found." );
		return false;
	}

	for( int row=0 ; row<er.GetRowSize() ; ++row )
	{
		Data * pData = new Data();

		int col = 0;

		pData->m_index			= er.GetInt(row, col);
		pData->m_enable			= er.GetBool(row, ++col);

		++col; // a_desc
		++col; // a_prob
		++col; // a_result_index
		++col; // a_pluspoint_tf
		++col; // a_overpluspoint_tf

		for( int i=0 ; i<MAX_KIND ; ++i )
		{
			pData->m_need_item_index[i] = er.GetInt(row, ++col);
			pData->m_need_item_count[i] = er.GetInt(row, ++col);
			++col; // a_keep_x
			++col; // a_minpulspoint_x
		}

		pData->m_price			= er.GetInt(row, ++col);

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

bool LHParam_MakeItem::CheckNeedItemBuff( __int64 & rnNeedLaim )
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

	int nRecipeNum = -1;

	for ( ; NULL != pItem; pItem = pItem->m_Next )
	{
		if ( ITYPE_ETC == GET_TYPE( pItem ) && IETC_RECIPE == GET_SHAPE( pItem ) )
		{
			if ( -1 != nRecipeNum )
			{
				//레시피 여러개일 경우 체크
				rnNeedLaim = 0;
				return false;
			}

			nRecipeNum = GET_ITEM_NUM0( pItem );
			//break;
		}
	}

	if ( -1 == nRecipeNum )
	{
		return false;
	}

	Data_Map_it it = m_DataMap.find( nRecipeNum );

	if ( it == m_DataMap.end() )
	{
		return false;
	}

	Data * pData = (*it).second;
	if ( false == pData->m_enable )
	{
		return false;
	}

	rnNeedLaim = pData->m_price;

	return true;
}

bool LHParam_MakeItem::CheckNeedItem( __int64 & rnNeedLaim )
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

	int nRecipeNum = -1;

	for ( ; NULL != pItem; pItem = pItem->m_Next )
	{
		if ( ITYPE_ETC == GET_TYPE( pItem ) && IETC_RECIPE == GET_SHAPE( pItem ) )
		{
			if ( -1 != nRecipeNum )
			{
				//레시피 여러개일 경우 체크
				rnNeedLaim = 0;
				return false;
			}

			nRecipeNum = GET_ITEM_NUM0( pItem );
			//break;
		}
	}

	if ( -1 == nRecipeNum )
	{
		return false;
	}

	Data_Map_it it = m_DataMap.find( nRecipeNum );

	if ( it == m_DataMap.end() )
	{
		return false;
	}

	Data * pData = (*it).second;
	if ( false == pData->m_enable )
	{
		return false;
	}

	rnNeedLaim = pData->m_price;

	typedef std::map< int, int >			ItemCount_Map;
	typedef ItemCount_Map::iterator			ItemCount_Map_it;
	typedef ItemCount_Map_it::value_type	ItemCount_Map_it_vt;

	ItemCount_Map ICountMap;

	int nExgCount = 0;
	int nNeedCount = 0;

	pItem = g_pRoh->m_ExgInven;
	for ( ; NULL != pItem; pItem = pItem->m_Next )
	{
		//if ( ITYPE_ETC != GET_TYPE( pItem ) ||
		//		( IETC_MATERIAL != GET_SHAPE( pItem ) && IETC_RECIPE != GET_SHAPE( pItem ) ) )
		//{
		//	return false;
		//}

		//if ( IETC_MATERIAL == GET_SHAPE( pItem ) )
		if ( IETC_RECIPE != GET_SHAPE( pItem ) )
		{
			ItemCount_Map_it it = ICountMap.find( GET_ITEM_NUM( pItem ) );
			if( it == ICountMap.end() )
			{
				ICountMap.insert( ItemCount_Map_it_vt( GET_ITEM_NUM( pItem ), 1 ) );
			}
			else
			{
				++(it->second);
			}
			++nExgCount;
		}
	}


	for( int i=0 ; i<MAX_KIND ; ++i )
	{
		int nItemIndex = pData->m_need_item_index[i];
		int nItemCount = pData->m_need_item_count[i];

		nNeedCount += nItemCount;

		ItemCount_Map_it it = ICountMap.find( nItemIndex );
		if ( it == ICountMap.end() )
		{
			if ( 0 == nItemCount )
			{
				continue;
			}
			return false;
		}

		if ( nItemCount != it->second )
		{
			return false;
		}
	}

	if ( nExgCount != nNeedCount )
	{
		return false;
	}

	return true;
}

