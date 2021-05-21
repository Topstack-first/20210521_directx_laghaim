#include "stdafx.h"
#include "FishingRewardManager.h"
#include "g_stringmanager.h"
#include "CommonConfig.h"

FishingRewardManager::FishingRewardManager()
{
	init();
}

FishingRewardManager::~FishingRewardManager()
{
}

void FishingRewardManager::init()
{
	CommonConfig* config = CommonConfig::Instance();

	int cntLow = config->GetFishRewardCount(0);
	int cntMid = config->GetFishRewardCount(1);
	int cntHigh = config->GetFishRewardCount(2);

	for(int i = 0; i < cntLow; i++)
	{
		m_vecRewardList[0].push_back(G_STRING(IDS_FISHING_REWARD_LOW0 + i));
	}

	for(int i = 0; i < cntMid; i++)
	{
		m_vecRewardList[1].push_back(G_STRING(IDS_FISHING_REWARD_MID0 + i));
	}

	for(int i = 0; i < cntHigh; i++)
	{
		m_vecRewardList[2].push_back(G_STRING(IDS_FISHING_REWARD_HIGH0 + i));
	}
}

void FishingRewardManager::Clear()
{
	for(int i = 0; i < MAX_FISHING_GRADE; i++)
	{
		m_vecRewardCanGet[i].clear();
		m_vecRewardAlready[i].clear();
	}
}

void FishingRewardManager::AddCanGet(int grade, int fishCount)
{
	if( grade < 0 || grade >= MAX_FISHING_GRADE )
		return;

	CommonConfig* config = CommonConfig::Instance();
	int index = config->GetFishRewardIndex(grade, fishCount);
	if( index == -1 )
		return;

	m_vecRewardCanGet[grade].push_back(index);
}

void FishingRewardManager::AddAlready(int grade, int fishCount)
{
	if( grade < 0 || grade >= MAX_FISHING_GRADE )
		return;

	CommonConfig* config = CommonConfig::Instance();
	int index = config->GetFishRewardIndex(grade, fishCount);
	if( index == -1 )
		return;

	m_vecRewardAlready[grade].push_back(index);
}

const std::vector<int>& FishingRewardManager::GetCanGetList(int grade)
{
	if( grade < 0 || grade >= MAX_FISHING_GRADE )
	{
		grade = 0;
	}

	return m_vecRewardCanGet[grade];
}

const std::vector<int>&	FishingRewardManager::GetAlreadyList(int grade)
{
	if( grade < 0 || grade >= MAX_FISHING_GRADE )
	{
		grade = 0;
	}

	return m_vecRewardAlready[grade];
}

const std::vector<std::string>& FishingRewardManager::GetRewardList(int grade)
{
	if( grade < 0 || grade >= MAX_FISHING_GRADE )
	{
		grade = 0;
	}

	return m_vecRewardList[grade];
}
