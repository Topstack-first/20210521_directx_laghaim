#ifndef _DICEGAME_REWARD_MANAGER_H_
#define _DICEGAME_REWARD_MANAGER_H_

#include "singleton.h"

#define MAX_DICEGAME_REWARD_COUNT	15

class DiceGameRewardManager : public CSingleton<DiceGameRewardManager>
{
public:
	DiceGameRewardManager();
	~DiceGameRewardManager();

public:
	void Clear();

public:
	void AddCanGet(int count);
	void AddAlready(int count);
	const std::vector<int>& GetCanGetList();
	const std::vector<int>& GetAlreadyList();
	const std::vector<std::string>& GetRewardList();

private:
	void init();

private:
	std::vector<std::string>	m_vecRewardList;
	std::vector<int>			m_vecRewardCanGet;
	std::vector<int>			m_vecRewardAlready;	
};

#endif // _DICEGAME_REWARD_MANAGER_H_
