#pragma once

#include "LHSingleton.h"

#include <map>
#include <list>
#include <vector>
#include <string>


typedef std::map<int, std::string>		LHSTRING_MAP;
typedef LHSTRING_MAP::iterator			LHSTRING_MAP_it;
typedef LHSTRING_MAP::value_type		LHSTRING_MAP_vt;


class LHResource_Manager : public LHSingleton<LHResource_Manager>
{
private:
	LHSTRING_MAP	m_string_map;
	LHSTRING_MAP	m_item_name_map;
	LHSTRING_MAP	m_npc_name_map;


public:
	void Init();
	void Release();

	int UTF8ToUnicode(wchar_t* wcstr, const char * utfstr, size_t count);
	int UniToMulti(char * mbstr, const wchar_t*wcstr , size_t count);


public:
	const char * GetString(int index);
	const char * GetItemName(int index);
	const char * GetNpcName(int index);


public:
	LHResource_Manager(void);
	virtual ~LHResource_Manager(void);
};
