#include "stdafx.h"
#include <stdio.h>
#include <string>
#include <json/json.h>
#include <boost/filesystem.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/format.hpp>
#include "CommonConfig.h"
#include "Def.h"

extern long getFileSize(PFILE* fp);

CommonConfig* CommonConfig::Instance()
{
	static CommonConfig __instance;
	return &__instance;
}

bool CommonConfig::loadConfigFromFile()
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

bool CommonConfig::Init( std::string filename /*= "./data/config.json"*/ )
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
	// get value
	fgt_flag_				= root.get("FGT_FLAG", false).asBool();
	kor_flag_				= root.get("KOR_FLAG", false).asBool();
	tha_flag_				= root.get("THA_FLAG", false).asBool();
	dont_drop_				= root.get("DONT_DROP", false).asBool();	
	weapon360_				= root.get("WEAPON360", false).asBool();
	stash_popup_			= root.get("STASH_POPUP", false).asBool();
	guild_invasion_enable_	= root.get("GUILD_INVASION_ENABLE", false).asBool();
	use_item_soccerzone_	= root.get("USE_ITEM_SOCCERZONE", false).asBool();
	bossriad_channel_		= root.get("BOSSRAID_CHANNEL", false).asBool();
	event_sakura_			= root.get("EVENT_SAKURA", false).asBool();
	event_less_exp_			= root.get("EVENT_LESS_EXP", false).asBool();
	zone16_warp_block_		= root.get("ZONE16_WARP_BLOCK", false).asBool();
	god_weapon_				= root.get("GOD_WEAPON", false).asBool();
	siege_crystal_scale_	= root.get("SIEGE_CRYSTAL_SCALE", false).asBool();
	calc_float_skill_		= root.get("CALC_FLOAT_SKILL", false).asBool();
	login_process_			= root.get("LOGIN_PROCESS", false).asBool();
	friend_system_			= root.get("FRIEND_SYSTEM", false).asBool();
	renewer_charinfo_		= root.get("RENEWER_CHARINFO", false).asBool();
	captcha_				= root.get("CAPTCHA", false).asBool();
	guild_mark_				= root.get("GUILD_MARK", false).asBool();
	lagrush_				= root.get("LAGRUSH", false).asBool();
	using_lp_				= root.get("USING_LP", false).asBool();
	bugtrap_				= root.get("BUGTRAP", false).asBool();
	sparring_				= root.get("SPARRING", false).asBool();

	{
		// fishing event
		const Json::Value fish_index_table = root["FISH_INDEX_TABLE"];
		if( fish_index_table.isNull() == false )
		{
			const Json::Value low = fish_index_table["LOW"];
			const Json::Value mid = fish_index_table["MID"];
			const Json::Value high = fish_index_table["HIGH"];

			int fishCount = 0;
			int rewardIndex = 0;

			for(int i = 0; i < (int)low.size(); i++)
			{
				fishCount = low[i].asInt();
				fish_reward_index_[0].insert(std::make_pair(fishCount, i));
				fish_reward_fishcount_[0].insert(std::make_pair(i, fishCount));
			}

			for(int i = 0; i < (int)mid.size(); i++)
			{
				fishCount = mid[i].asInt();
				fish_reward_index_[1].insert(std::make_pair(fishCount, i));
				fish_reward_fishcount_[1].insert(std::make_pair(i, fishCount));
			}

			for(int i = 0; i < (int)high.size(); i++)
			{
				fishCount = high[i].asInt();
				fish_reward_index_[2].insert(std::make_pair(fishCount, i));
				fish_reward_fishcount_[2].insert(std::make_pair(i, fishCount));
			}
		}		
	}

	lotto_auto_time_		= root.get("LOTTO_AUTO_TIME", 0.5f).asFloat();
	
	const Json::Value shout_chat_color_table = root["SHOUT_CHAT_COLOR"];
	int shout_chat_color_r	= shout_chat_color_table[0].asInt();
	int shout_chat_color_g	= shout_chat_color_table[1].asInt();
	int shout_chat_color_b	= shout_chat_color_table[2].asInt();
	shout_chat_color_		= RGB(shout_chat_color_r, shout_chat_color_g, shout_chat_color_b);
		
	gear_gem_index_			= root.get("GEAR_GEM_INDEX", -1).asInt();

	const Json::Value gear_item_table = root["GEAR_ITEM"];	
	const unsigned int GEAR_ITEM_VNUM = 0;
	const unsigned int GEAR_ITEM_WHERE = 1;
	int gear_item_where = -1;
	int gear_item_vnum = -1;
	for(int i = 0; i < (int)gear_item_table.size(); i++)
	{
		gear_item_where = gear_item_table[i][GEAR_ITEM_WHERE].asInt();
		gear_item_vnum = gear_item_table[i][GEAR_ITEM_VNUM].asInt();

		gear_item_.insert(std::make_pair(gear_item_where, gear_item_vnum));
	}

	{
		// baseball event
		const Json::Value baseball_index_table = root["BASEBALL_INDEX_TABLE"];
		if( baseball_index_table.isNull() == false )
		{
			const Json::Value foul = baseball_index_table["FOUL"];
			const Json::Value one = baseball_index_table["ONE"];
			const Json::Value two = baseball_index_table["TWO"];
			const Json::Value three = baseball_index_table["THREE"];
			const Json::Value homerun = baseball_index_table["HOMERUN"];


			int baseballCount = 0;
			int rewardIndex = 0;

			for(int i = 0; i < (int)foul.size(); i++)
			{
				baseballCount = foul[i].asInt();
				baseball_reward_index_[0].insert(std::make_pair(baseballCount, i));
				baseball_reward_ballcount_[0].insert(std::make_pair(i, baseballCount));
			}

			for(int i = 0; i < (int)one.size(); i++)
			{
				baseballCount = one[i].asInt();
				baseball_reward_index_[1].insert(std::make_pair(baseballCount, i));
				baseball_reward_ballcount_[1].insert(std::make_pair(i, baseballCount));
			}

			for(int i = 0; i < (int)two.size(); i++)
			{
				baseballCount = two[i].asInt();
				baseball_reward_index_[2].insert(std::make_pair(baseballCount, i));
				baseball_reward_ballcount_[2].insert(std::make_pair(i, baseballCount));
			}

			for(int i = 0; i < (int)three.size(); i++)
			{
				baseballCount = three[i].asInt();
				baseball_reward_index_[3].insert(std::make_pair(baseballCount, i));
				baseball_reward_ballcount_[3].insert(std::make_pair(i, baseballCount));
			}

			for(int i = 0; i < (int)homerun.size(); i++)
			{
				baseballCount = homerun[i].asInt();
				baseball_reward_index_[4].insert(std::make_pair(baseballCount, i));
				baseball_reward_ballcount_[4].insert(std::make_pair(i, baseballCount));
			}
		}		
	}

	{
		// dicegame event
		const Json::Value dicegame_index_table = root["DICEGAME_INDEX_TABLE"];
		if( dicegame_index_table.isNull() == false )
		{
			int succCount = 0;
			for(int i = 0; i < (int)dicegame_index_table.size(); i++)
			{
				succCount = dicegame_index_table[i].asInt();
				dicegame_reward_index_.insert(std::make_pair(succCount, i));
				dicegame_reward_succcount_.insert(std::make_pair(i, succCount));
			}
		}
	}
	
	return true;
}

