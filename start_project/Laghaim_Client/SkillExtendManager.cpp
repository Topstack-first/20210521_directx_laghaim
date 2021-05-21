#include "stdafx.h"
#include <string>
#include <map>
#include <json/json.h>
#include <boost/filesystem.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/format.hpp>
#include "SkillExtendManager.h"
#include "Def.h"

extern long getFileSize(PFILE* fp);

SkillExtendManager* SkillExtendManager::instnace()
{
	static SkillExtendManager _instance;
	return &_instance;
}

bool SkillExtendManager::loadConfigFromFile()
{
	PFILE* fp = pfopen(filename_.c_str(), "rb");
	if (fp == NULL)
	{	
		return false;
	}

	int filesize = getFileSize(fp);

	char* tbuf = new char[filesize + 1];

	pfread(tbuf, 1, filesize, fp);

	tbuf[filesize] = '\0';
	fileBuffer_ = tbuf;
	delete [] tbuf;	

	pfclose(fp);

	return true;
}

bool SkillExtendManager::Init(std::string filename)
{
	filename_ = filename;

	if (loadConfigFromFile() == false)
		return false;

	Json::Value root;
	Json::Reader reader;
	bool parsingSuccessful = reader.parse( fileBuffer_, root );
	if (parsingSuccessful == false)
	{	
		return false;
	}
	
	//////////////////////////////////////////////////////////////////////////
	const Json::Value skill_table = root["SKILL_TABLE"];

	const unsigned int temp_skill_race = 0;
	const unsigned int temp_skill_index = 1;
	const unsigned int temp_skill_level = 2;
	const unsigned int temp_char_min_level = 3;
	const unsigned int temp_laim = 4;
	const unsigned int temp_point = 5;
	const unsigned int temp_item_index = 6;
	const unsigned int temp_updam_per = 7;
	const unsigned int temp_attr_rate = 8;
	
	for(int i = 0;i < (int)skill_table.size(); ++i)
	{
		const Json::Value element_ = skill_table[i];

		int skill_index = element_[temp_skill_index].asInt();
		int skill_level = element_[temp_skill_level].asInt();

		key_t pair_key = std::make_pair(skill_index,skill_level);
		
		SkillData data;
		data.race = element_[temp_skill_race].asInt();
		data.char_min_level = element_[temp_char_min_level].asInt();
		data.updam_per = element_[temp_updam_per].asInt();
		data.laim = element_[temp_laim].asInt();
		data.point = element_[temp_point].asInt();
		data.item_index = element_[temp_item_index].asInt();
		data.attr_rate = element_[temp_attr_rate].asInt();

		skill_extend.insert(map_t::value_type(pair_key,data));

		if( skill_include.find(skill_index) == skill_include.end() )
		{
			skill_include.insert(std::make_pair(skill_index, 0));
		}
	}
	//////////////////////////////////////////////////////////////////////////

	return true;
}

int SkillExtendManager::getData(int skill_idx, int skill_level)
{
	int t_return = -1;

	map_t::iterator find_it = skill_extend.find(std::make_pair(skill_idx, skill_level));
	if(find_it != skill_extend.end())
	{
		t_return = find_it->second.updam_per;
	}

	return t_return;
}

int	SkillExtendManager::getMoney(int skill_idx, int skill_level)
{
	int t_return = -1;

	map_t::iterator find_it = skill_extend.find(std::make_pair(skill_idx, skill_level));
	if(find_it != skill_extend.end())
	{
		t_return = find_it->second.laim;
	}

	return t_return;
}

bool SkillExtendManager::isExtend(int skill_idx)
{
	if( skill_include.find(skill_idx) == skill_include.end() )
		return false;

	return true;
}
