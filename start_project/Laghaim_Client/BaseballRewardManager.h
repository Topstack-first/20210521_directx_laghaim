#ifndef _BASEBALLREWARDMANAGER_H_
#define _BASEBALLREWARDMANAGER_H_

#include "singleton.h"

#define MAX_BASEBALL_GRADE			5
#define MAX_BASEBALL_REWARD_COUNT	20

class BaseballRewardManager : public CSingleton<BaseballRewardManager>
{
public:
	BaseballRewardManager();
	~BaseballRewardManager();

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
	std::vector<std::string>	m_vecRewardList[MAX_BASEBALL_GRADE];
	std::vector<int>			m_vecRewardCanGet[MAX_BASEBALL_GRADE];
	std::vector<int>			m_vecRewardAlready[MAX_BASEBALL_GRADE];	
};

#endif // _BASEBALLREWARDMANAGER_H_
