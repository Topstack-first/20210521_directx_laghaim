#include "stdafx.h"
#include "LHParam_GMagicStone.h"
#include "LHExcelReader.h"
#include "UIMgr.h"


LHParam_GMagicStone::LHParam_GMagicStone(void)
{
}

LHParam_GMagicStone::~LHParam_GMagicStone(void)
{
}

bool LHParam_GMagicStone::CheckNeedItem( __int64 & rnNeedLaim )
{
	assert( NULL != g_pRoh );

	const int nNeedLaim			= 10000000;
	const int nNeedItemIndex[]	= { 2956, 2963 }; //ºù°á¼® ¿ø¼®, È¥ÀÇ Á¶°¢
	const int nNeedItemCount[]	= { 3, 3 };
	const int nTotalModelCount	= sizeof( nNeedItemIndex ) / sizeof( nNeedItemIndex[0] );

	CItem * pItem = g_pRoh->m_ExgInven;

	if ( NULL == pItem )
	{
		return false;
	}

	rnNeedLaim = nNeedLaim;

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


	for( int i=0 ; i<nTotalModelCount ; ++i )
	{
		int nItemIndex = nNeedItemIndex[i];
		int nItemCount = nNeedItemCount[i];

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

