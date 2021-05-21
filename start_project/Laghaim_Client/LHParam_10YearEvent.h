#ifndef _LHParam_10YearEvent_H_
#define _LHParam_10YearEvent_H_

#include "ItemProto.h"

class LHParam_10YearEvent
{
public:
	struct Data
	{
		int		m_index;
		bool	m_enable;
		int		m_eventType;
		int		m_item_index[10];
		int		m_item_prob [10];
		int		m_item_count[10];
	};
	typedef std::map< int, Data * >		Data_Map;
	typedef Data_Map::iterator			Data_Map_it;
	typedef Data_Map_it::value_type		Data_Map_it_vt;

public:
	LHParam_10YearEvent(void);
	~LHParam_10YearEvent(void);

public:
	bool 		Check();

protected:
	bool 		LoadData();
	bool 		IsItemCount1();
	bool 		IsEventType1();
	Data_Map *	GetDataMap( int nEventType );
	bool		IsExistItem( int nIndex, int nCount );
	int			GetExgInvenItemCount();

	int			CheckEventType();

protected:
	Data_Map 	m_DataMap_EventType1;
	Data_Map 	m_DataMap_EventType2;
	Data_Map 	m_DataMap_EventType3;
};
#endif // _LHParam_10YearEvent_H_