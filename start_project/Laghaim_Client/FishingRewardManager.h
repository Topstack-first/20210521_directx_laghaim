#ifndef _FISHINGREWARDMANAGER_H_
#define _FISHINGREWARDMANAGER_H_

#include "singleton.h"

#define MAX_FISHING_GRADE			3
#define MAX_FISHING_REWARD_COUNT	20

class FishingRewardManager : public CSingleton<FishingRewardManager>
{
public:
	FishingRewardManager();
	~FishingRewardManager();

public:
	void Clear();

public:
	void AddCanGet(int grade, int fishCount);
	void AddAlready(int grade, int fishCount);
	const std::vector<int>& GetCanGetList(int grade);
	const std::vector<int>&	GetAlreadyList(int grade);
	const std::vector<std::string>& GetRewardList(int grade);

private:
	void init();

private:
	std::vector<std::string>	m_vecRewardList[MAX_FISHING_GRADE];
	std::vector<int>			m_vecRewardCanGet[MAX_FISHING_GRADE];
	std::vector<int>			m_vecRewardAlready[MAX_FISHING_GRADE];	
};

#endif // _FISHINGREWARDMANAGER_H_
