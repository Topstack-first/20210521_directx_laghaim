#ifndef _LHParam_Manager_H_
#define _LHParam_Manager_H_

#pragma once

#include <map>
#include <list>
#include <string>


class LHParam_MapInfo
{
public:
	int m_zone_index;
	std::string m_filename;
	std::string m_path_map;
	std::string m_path_deco;

	bool m_dungeon;
	bool m_detail_ground;
	bool m_new_dungeon;
	bool m_new_terrain;
	int m_music_id;
	bool m_snow;
	int m_floor;


public:
	LHParam_MapInfo()
		:	m_zone_index(-1)
		,	m_dungeon(false)
		,	m_detail_ground(false)
		,	m_new_dungeon(false)
		,	m_new_terrain(false)
		,	m_music_id(-1)
		,	m_snow(false)
		,	m_floor(false)
	{
	}
};


class LHParam_Base
{
public:
	int m_index;				// tool's index
	std::string m_name;
	std::string m_desc;

public:
	const int GetIndex()
	{
		return m_index;
	}
	const char * GetName()
	{
		return m_name.c_str();
	}
	const char * GetDesc()
	{
		return m_desc.c_str();
	}


public:
	LHParam_Base() : m_index(-1)	{}
	~LHParam_Base()					{}

};


class LHParam_ItemProto : public LHParam_Base
{
public:




public:
	LHParam_ItemProto();
	~LHParam_ItemProto();

};


class LHParam_Skill : public LHParam_Base
{
public:
	int m_index;



public:
	LHParam_Skill();
	~LHParam_Skill();
};



typedef std::list<LHParam_MapInfo*>		mapinfo_list;
typedef mapinfo_list::iterator			mapinfo_list_it;

class LHParam_Manager
{
private:
	bool			m_initialized;
	mapinfo_list * m_mapinfo_list;


public:
	bool Load();


public:
	const LHParam_MapInfo * GetMapInfo(int zone_index)
	{
		if( !m_initialized )
			return false;

		mapinfo_list_it it = m_mapinfo_list->begin();

		for( ; it!=m_mapinfo_list->end() ; ++it )
		{
			if( (*it)->m_zone_index == zone_index )
				return *it;
		}

		return NULL;
	}


public:
	LHParam_Manager(void);
	~LHParam_Manager(void);
};
#endif // _LHParam_Manager_H_