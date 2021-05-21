#ifndef _LHParam_MakeItem_H_
#define _LHParam_MakeItem_H_

#include "ItemProto.h"

class LHParam_MakeItem
{
public:
	struct Data
	{
		int		m_index;
		bool	m_enable;
		int		m_need_item_index[MAX_KIND];
		int		m_need_item_count[MAX_KIND];
		int		m_price;
	};
	typedef std::map< int, Data * >		Data_Map;
	typedef Data_Map::iterator			Data_Map_it;
	typedef Data_Map_it::value_type		Data_Map_it_vt;

public:
	LHParam_MakeItem(void);
	~LHParam_MakeItem(void);

public:
	bool CheckNeedItem( __int64 & rnNeedLaim );
	bool CheckNeedItemBuff( __int64 & rnNeedLaim );

protected:
	bool LoadData();

protected:
	Data_Map 		m_DataMap;
};
#endif // _LHParam_MakeItem_H_