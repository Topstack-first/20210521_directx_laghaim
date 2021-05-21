#include "stdafx.h"
#include "LHParam_10YearEvent.h"
#include "LHExcelReader.h"
#include "UIMgr.h"

const char * EventTable_Filename = "data/item/10year_event_item_table.txt";

LHParam_10YearEvent::LHParam_10YearEvent(void)
{
}

LHParam_10YearEvent::~LHParam_10YearEvent(void)
{
	safe_delete_associate(m_DataMap_EventType1);
	safe_delete_associate(m_DataMap_EventType2);
	safe_delete_associate(m_DataMap_EventType3);
}

bool LHParam_10YearEvent::LoadData()
{
	LHExcelReader er;

	if( !er.Open(EventTable_Filename) )
	{
		assert( false && "EventTable_Filename File not found." );
		return false;
	}

	for( int row=0 ; row<er.GetRowSize() ; ++row )
	{
		Data * pData = new Data();

		int col = 0;

		pData->m_index			= er.GetInt(row, col);
		pData->m_enable			= er.GetBool(row, ++col);

		++col; // a_desc

		pData->m_eventType		= er.GetInt(row, ++col);

		for( int i=0 ; i<10; ++i )
		{
			pData->m_item_index[i]	= er.GetInt(row, ++col);
			pData->m_item_prob[i]	= er.GetInt(row, ++col);
			pData->m_item_count[i]	= er.GetInt(row, ++col);
		}

		if ( false == pData->m_enable )
		{
			delete pData;
			pData = NULL;
			continue;
		}

		Data_Map * pDataMap = GetDataMap( pData->m_eventType );
		assert( NULL != pDataMap );

		pDataMap->insert(Data_Map_it_vt(pData->m_index, pData));
	}

	return true;
}

bool LHParam_10YearEvent::Check()
{
	assert( NULL != g_pRoh );

	CItem * pItem = g_pRoh->m_ExgInven;

	if ( NULL == pItem )
	{
		return false;
	}

	if( true == m_DataMap_EventType1.empty() && true == m_DataMap_EventType2.empty() && true == m_DataMap_EventType3.empty() )
	{
		if( false == LoadData() )
			return false;
	}


	int		nEventType	= CheckEventType();// ( IsEventType1() ? 1 : 2 );

	Data_Map * pDataMap = GetDataMap( nEventType );

	switch ( nEventType )
	{
	case 1:
		{
			if ( FALSE == IsItemCount1() )
			{
				return false;
			}
			Data_Map_it it = m_DataMap_EventType1.find( pItem->m_Vnum );
			if ( it == m_DataMap_EventType1.end() )
			{
				return false;
			}
		}
		break;
	case 3:
		{
			if ( FALSE == IsItemCount1() )
			{
				return false;
			}
			Data_Map_it it = m_DataMap_EventType3.find( pItem->m_Vnum );
			if ( it == m_DataMap_EventType3.end() )
			{
				return false;
			}
		}
		break;

	case 2:
		{
			Data_Map_it it = m_DataMap_EventType2.begin();

			for ( ; it != m_DataMap_EventType2.end(); ++it )
			{
				bool bRequestItem = true;
				int nTotalNeedCount = 0;
				int nCurrentCount = GetExgInvenItemCount();

				Data * pData = it->second;
				for ( int i = 0; i < 6; ++i )
				{
					int nIndex = pData->m_item_index[i];
					int nCount = pData->m_item_count[i];
					nTotalNeedCount += nCount;

					if ( 0 == nCount )
					{
						break;
					}
					if ( false == IsExistItem( nIndex, nCount ) )
					{
						bRequestItem = false;
						break;
					}
				}
				if ( true == bRequestItem && nCurrentCount == nTotalNeedCount )
				{
					return true;
				}
			}
			return false;
		}
		break;
	}

	if( pItem->m_bTimeLimit )
	{
		return false;
	}

	return true;
}

LHParam_10YearEvent::Data_Map *
LHParam_10YearEvent::GetDataMap( int nEventType )
{
	Data_Map * pDataMap( NULL );
	if ( 1 == nEventType )
	{
		pDataMap = &m_DataMap_EventType1;
	}
	else if ( 2 == nEventType )
	{
		pDataMap = &m_DataMap_EventType2;
	}
	else if ( 3 == nEventType )
	{
		pDataMap = &m_DataMap_EventType3;
	}
	else
	{
		assert( false );
		pDataMap = NULL;
	}
	return pDataMap;
}

bool LHParam_10YearEvent::IsExistItem( int nIndex, int nCount )
{
	CItem * pItem = g_pRoh->m_ExgInven;

	for ( ; NULL != pItem; pItem = pItem->m_Next )
	{
		if ( nIndex != pItem->m_Vnum )
		{
			continue;
		}
		--nCount;
	}

	return ( 0 == nCount );

}

bool LHParam_10YearEvent::IsItemCount1()
{
	CItem * pItem = g_pRoh->m_ExgInven;

	if ( NULL == pItem )
	{
		assert( false );
		return false;
	}

	if ( NULL != pItem->m_Next )
	{
		return false;
	}

	return true;
}

bool LHParam_10YearEvent::IsEventType1()
{
	assert( NULL != g_pRoh );

	CItem * pItem = g_pRoh->m_ExgInven;

	if ( NULL == pItem )
	{
		assert( false );
		return false;
	}

	Data_Map_it it = m_DataMap_EventType1.find( pItem->m_Vnum );

	if ( it == m_DataMap_EventType1.end() || false == IsItemCount1() )
	{
		return false;
	}

	return true;
}

int	LHParam_10YearEvent::GetExgInvenItemCount()
{
	int nCount = 0;
	CItem * pItem = g_pRoh->m_ExgInven;

	for ( ; NULL != pItem; pItem = pItem->m_Next )
	{
		++nCount;
	}

	return nCount;
}


int	LHParam_10YearEvent::CheckEventType()
{
	assert( NULL != g_pRoh );

	CItem * pItem = g_pRoh->m_ExgInven;

	if ( NULL == pItem )
	{
		assert( false );
		return false;
	}

	Data_Map_it it = m_DataMap_EventType1.find( pItem->m_Vnum );
	if ( it != m_DataMap_EventType1.end() && true == IsItemCount1() )
		return 1;

	it = m_DataMap_EventType3.find( pItem->m_Vnum );
	if ( it != m_DataMap_EventType3.end() && true == IsItemCount1() )
		return 3;

	return 2;
}
