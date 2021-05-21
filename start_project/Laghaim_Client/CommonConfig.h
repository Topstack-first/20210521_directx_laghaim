#ifndef __COMMON_CONFIG_H__
#define __COMMON_CONFIG_H__

class CommonConfig
{
public:
	CommonConfig(void) {}
	~CommonConfig(void) {}

	static CommonConfig* Instance();

	bool Init(std::string filename = "./data/config.json");
	
public:
	const bool& GetFgtFlag()
	{
		return fgt_flag_; 
	}

	const bool& GetKorFlag()
	{
		return kor_flag_;
	}

	const bool& GetThaFlag()
	{
		return tha_flag_;
	}

	const bool& GetDontDrop()
	{
		return dont_drop_;
	}	

	const bool& GetWeapon360()
	{
		return weapon360_;
	}

	const bool& GetStashPopup()
	{
		return stash_popup_;
	}

	const bool& GetGuildInvasionEnable()
	{
		return guild_invasion_enable_;
	}

	const bool& GetUseItemSoccerZone()
	{
		return use_item_soccerzone_;
	}

	const bool& GetBossRaidChannel()
	{
		return bossriad_channel_;
	}

	const bool& GetEventSakura()
	{
		return event_sakura_;
	}

	const bool& GetEventLessExp()
	{
		return event_less_exp_;
	}

	const bool& GetZone16WarpBlock()
	{
		return zone16_warp_block_;
	}

	const bool& GetGodWeapon()
	{
		return god_weapon_;
	}

	const bool& GetSiegeCrystalScale()
	{
		return siege_crystal_scale_;
	}

	const bool& GetCalcFloatSkill()
	{
		return calc_float_skill_;
	}

	const bool& GetLoginProcess()
	{
		return login_process_;
	}

	const bool& GetFriendSystem()
	{
		return friend_system_;
	}

	const bool& GetRenewerCharInfo()
	{
		return renewer_charinfo_;
	}

	const bool& GetCaptcha()
	{
		return captcha_;
	}

	const bool& GetGuildMark()
	{
		return guild_mark_;
	}

	const bool& GetLagrush()
	{
		return lagrush_;
	}

	const bool& GetUsingLP()
	{
		return using_lp_;
	}

	const bool& GetBugTrap()
	{
		return bugtrap_;
	}

	const bool& GetSparring()
	{
		return sparring_;
	}

	const int GetFishRewardIndex(int grade, int fishCount)
	{
		if( grade < 0 || grade >= 3 )
			return -1;

		std::map<int, int>::iterator itFind = fish_reward_index_[grade].find(fishCount);
		if( itFind == fish_reward_index_[grade].end() )
			return -1;

		return itFind->second;
	}

	const int GetFishRewardFishCount(int grade, int index)
	{
		if( grade < 0 || grade >= 3 )
			return -1;

		std::map<int, int>::iterator itFind = fish_reward_fishcount_[grade].find(index);
		if( itFind == fish_reward_fishcount_[grade].end() )
			return -1;

		return itFind->second;
	}

	const int GetFishRewardCount(int grade)
	{
		if( grade < 0 || grade >= 3 )
			return 0;

		return fish_reward_index_[grade].size();
	}

	const float GetLottoAutoTime()
	{
		return lotto_auto_time_;
	}

	const DWORD GetShoutChatColor()
	{
		return shout_chat_color_;
	}

	const int GetGearGemIndex()
	{
		return gear_gem_index_;
	}

	const int GetGearItemVnum(int where)
	{
		std::map<int, int>::iterator itFind = gear_item_.find(where);
		if( itFind == gear_item_.end() )
			return -1;

		return itFind->second;
	}

	const int GetBaseballRewardIndex(int grade, int ballCount)
	{
		if( grade < 0 || grade >= 5 )
			return -1;

		std::map<int, int>::iterator itFind = baseball_reward_index_[grade].find(ballCount);
		if( itFind == baseball_reward_index_[grade].end() )
			return -1;

		return itFind->second;
	}

	const int GetBaseballRewardBallCount(int grade, int index)
	{
		if( grade < 0 || grade >= 5 )
			return -1;

		std::map<int, int>::iterator itFind = baseball_reward_ballcount_[grade].find(index);
		if( itFind == baseball_reward_ballcount_[grade].end() )
			return -1;

		return itFind->second;
	}

	const int GetBaseballRewardCount(int grade)
	{
		if( grade < 0 || grade >= 5 )
			return 0;

		return baseball_reward_index_[grade].size();
	}

	const int GetDiceGameRewardIndex(int succCount)
	{
		std::map<int, int>::iterator itFind = dicegame_reward_index_.find(succCount);
		if( itFind == dicegame_reward_index_.end() )
			return -1;

		return itFind->second;
	}

	const int GetDiceGameRewardSuccCount(int index)
	{
		std::map<int, int>::iterator itFind = dicegame_reward_succcount_.find(index);
		if( itFind == dicegame_reward_succcount_.end() )
			return -1;

		return itFind->second;
	}

	const int GetDiceGameRewardCount()
	{
		return dicegame_reward_index_.size();
	}
	

private:
	bool loadConfigFromFile();

private:
	std::string			filename_;
	std::string			fileBuffer_;

	//////////////////////////////////////////////////////////////////////////
	bool				fgt_flag_;
	bool				kor_flag_;
	bool				tha_flag_;
	bool				dont_drop_;	
	bool				weapon360_;
	bool				stash_popup_;
	bool				guild_invasion_enable_;
	bool				use_item_soccerzone_;
	bool				bossriad_channel_;
	bool				event_sakura_;
	bool				event_less_exp_;
	bool				zone16_warp_block_;
	bool				god_weapon_;
	bool				siege_crystal_scale_;
	bool				calc_float_skill_;
	bool				login_process_;
	bool				friend_system_;
	bool				renewer_charinfo_;
	bool				captcha_;
	bool				guild_mark_;
	bool				lagrush_;
	bool				using_lp_;
	bool				bugtrap_;
	bool				sparring_;
	std::map<int, int>	fish_reward_index_[3];
	std::map<int, int>	fish_reward_fishcount_[3];
	float				lotto_auto_time_;
	DWORD				shout_chat_color_;
	int					gear_gem_index_;
	std::map<int, int>	gear_item_;
	std::map<int, int>	baseball_reward_index_[5];
	std::map<int, int>	baseball_reward_ballcount_[5];
	std::map<int, int>	dicegame_reward_index_;
	std::map<int, int>	dicegame_reward_succcount_;
};

#endif
