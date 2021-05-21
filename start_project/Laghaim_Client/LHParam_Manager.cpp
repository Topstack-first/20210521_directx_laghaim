#include "stdafx.h"
#include ".\lhparam_manager.h"
#include "UIMgr.h"
#include "LHExcelReader.h"

const char * map_file = "data/mapinfo_table.txt";
const char * map_path = "map";
const char * deco_path = "data/building";

LHParam_Manager::LHParam_Manager(void)
	:	m_initialized(false)
	,	m_mapinfo_list(new mapinfo_list)
{
}

LHParam_Manager::~LHParam_Manager(void)
{
	//m_mapinfo_list->clear();
	safe_delete_sequence(*m_mapinfo_list);
	safe_delete(m_mapinfo_list);
}

bool LHParam_Manager::Load()
{
	LHExcelReader er;
	if( !er.Open(map_file) )
		return false;

	safe_delete_sequence(*m_mapinfo_list);

	for( int row=0 ; row<er.GetRowSize() ; ++row )
	{
		int col =  0;
		LHParam_MapInfo * info = new LHParam_MapInfo;

		info->m_zone_index = er.GetInt(row, col++);
		col++;
		info->m_filename = er.GetData(row, col++);
		info->m_path_map = er.GetData(row, col++);
		info->m_path_deco = er.GetData(row, col++);

		info->m_dungeon = er.GetBool(row, col++);
		info->m_detail_ground = er.GetBool(row, col++);
		info->m_new_dungeon = er.GetBool(row, col++);
		info->m_new_terrain = er.GetBool(row, col++);
		info->m_music_id = er.GetInt(row, col++);
		info->m_snow = er.GetBool(row, col++);
		info->m_floor = er.GetInt(row, col++);

		m_mapinfo_list->push_back(info);
	}

	m_initialized = true;

	return true;
}