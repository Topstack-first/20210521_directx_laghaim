#include "stdafx.h"
#include "BaseballRewardManager.h"
#include "g_stringmanager.h"
#include "CommonConfig.h"

BaseballRewardManager::BaseballRewardManager()
{
	init();
}

BaseballRewardManager::~BaseballRewardManager()
{
}

void BaseballRewardManager::init()
{
	CommonConfig* config = CommonConfig::Instance();

	int cntFoul = config->GetBaseballRewardCount(0);
	int cntOne = config->GetBaseballRewardCount(1);
	int cntTwo = config->GetBaseballRewardCount(2);
	int cntThree = config->GetBaseballRewardCount(3);
	int cntHomerun = config->GetBaseballRewardCount(4);

	for(int i = 0; i < cntFoul; i++)
	{
		m_vecRewardList[0].push_back(G_STRING(IDS_BASEBALL_REWARD_FOUL0 + i));
	}

	for(int i = 0; i < cntOne; i++)
	{
		m_vecRewardList[1].push_back(G_STRING(IDS_BASEBALL_REWARD_ONE0 + i));
	}

	for(int i = 0; i < cntTwo; i++)
	{
		m_vecRewardList[2].push_back(G_STRING(IDS_BASEBALL_REWARD_TWO0 + i));
	}

	for(int i = 0; i < cntThree; i++)
	{
		m_vecRewardList[3].push_back(G_STRING(IDS_BASEBALL_REWARD_THREE0 + i));
	}

	for(int i = 0; i < cntHomerun; i++)
	{
		m_vecRewardList[4].push_back(G_STRING(IDS_BASEBALL_REWARD_HR0 + i));
	}
}

void BaseballRewardManager::Clear()
{
	for(int i = 0; i < MAX_BASEBALL_GRADE; i++)
	{
		m_vecRewardCanGet[i].clear();
		m_vecRewardAlready[i].clear();
	}
}

void BaseballRewardManager::AddCanGet(int grade, int fishCount)
{
	if( grade < 0 || grade >= MAX_BASEBALL_GRADE )
		return;

	CommonConfig* config = CommonConfig::Instance();
	int index = config->GetBaseballRewardIndex(grade, fishCount);
	if( index == -1 )
		return;

	m_vecRewardCanGet[grade].push_back(index);
}

void BaseballRewardManager::AddAlready(int grade, int fishCount)
{
	if( grade < 0 || grade >= MAX_BASEBALL_GRADE )
		return;

	CommonConfig* config = CommonConfig::Instance();
	int index = config->GetBaseballRewardIndex(grade, fishCount);
	if( index == -1 )
		return;

	m_vecRewardAlready[grade].push_back(index);
}

const std::vector<int>& BaseballRewardManager::GetCanGetList(int grade)
{
	if( grade < 0 || grade >= MAX_BASEBALL_GRADE )
	{
		grade = 0;
	}

	return m_vecRewardCanGet[grade];
}

const std::vector<int>&	BaseballRewardManager::GetAlreadyList(int grade)
{
	if( grade < 0 || grade >= MAX_BASEBALL_GRADE )
	{
		grade = 0;
	}

	return m_vecRewardAlready[grade];
}

const std::vector<std::string>& BaseballRewardManager::GetRewardList(int grade)
{
	if( grade < 0 || grade >= MAX_BASEBALL_GRADE )
	{
		grade = 0;
	}

	return m_vecRewardList[grade];
}
