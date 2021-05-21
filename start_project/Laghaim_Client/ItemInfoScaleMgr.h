#ifndef _ITEM_INFO_SCALE_MGR_H_
#define _ITEM_INFO_SCALE_MGR_H_

#include "singleton.h"

class ItemInfoScaleMgr : public CSingleton<ItemInfoScaleMgr>
{
public:
	typedef std::map<int, int> _map;
	ItemInfoScaleMgr();
	~ItemInfoScaleMgr();

public:
	bool IsNoScale(const int vnum);

private:
	void load();

private:
	_map m_map;
};

#endif // _ITEM_INFO_SCALE_MGR_H_
