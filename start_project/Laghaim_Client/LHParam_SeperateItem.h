#ifndef _LHParam_SeperateItem_H_
#define _LHParam_SeperateItem_H_

#include "ItemProto.h"

class LHParam_SeperateItem
{
public:
	struct Data
	{
		int		m_index;
		bool	m_enable;
	};
	typedef std::map< int, Data * >		Data_Map;
	typedef Data_Map::iterator			Data_Map_it;
	typedef Data_Map_it::value_type		Data_Map_it_vt;

public:
	LHParam_SeperateItem(void);
	~LHParam_SeperateItem(void);

public:
	bool		CheckNeedItem();

protected:
	bool		LoadData();

protected:
	Data_Map 	m_DataMap;
};
#endif // _LHParam_SeperateItem_H_