#ifndef _ITEM_FAKE_OPTION_MGR_H_
#define _ITEM_FAKE_OPTION_MGR_H_

#include "singleton.h"

enum eFakeOptionType
{
	eFakeOptionType_DEF70 = 0,
	eFakeOptionType_DEFMANARATE,
	eFakeOptionType_RESIST_NUM2,
	eFakeOptionType_RESIST_NUM3,

	eFakeOptionType_Max,
};

class ItemFakeOptionMgr : public CSingleton<ItemFakeOptionMgr>
{
public:
	typedef std::map<int ,int> MAP_ITEM_INDEX;
	typedef std::map<int, MAP_ITEM_INDEX> MAP_ITEM_HAS_OPTION;

	ItemFakeOptionMgr();
	~ItemFakeOptionMgr();

public:
	void Load();

	bool HasOption(int type, const int& vnum);

private:
	void addVal(const int& type, const int& vnum);

private:
	MAP_ITEM_HAS_OPTION m_mapHasOption;
};

#endif