#include "stdafx.h"
#include "CommonConfig.h"
#include "g_stringmanager.h"
#include "DiceGameRewardManager.h"

DiceGameRewardManager::DiceGameRewardManager()
{
	init();
}

DiceGameRewardManager::~DiceGameRewardManager()
{

}

void DiceGameRewardManager::init()
{
	CommonConfig* config = CommonConfig::Instance();

	int cnt = config->GetDiceGameRewardCount();
	for(int i = 0; i < cnt; ++i)
	{
		m_vecRewardList.push_back(G_STRING(IDS_DICEGAME_REWARD0 + i));
	}
}

void DiceGameRewardManager::Clear()
{
	m_vecRewardCanGet.clear();
	m_vecRewardAlready.clear();
}

void DiceGameRewardManager::AddCanGet(int count)
{
	CommonConfig* config = CommonConfig::Instance();
	int index = config->GetDiceGameRewardIndex(count);
	if( index == -1 )
		return;

	m_vecRewardCanGet.push_back(index);
}

void DiceGameRewardManager::AddAlready(int count)
{
	CommonConfig* config = CommonConfig::Instance();
	int index = config->GetDiceGameRewardIndex(count);
	if( index == -1 )
		return;

	m_vecRewardAlready.push_back(index);
}

const std::vector<int>& DiceGameRewardManager::GetCanGetList()
{
	return m_vecRewardCanGet;
}

const std::vector<int>& DiceGameRewardManager::GetAlreadyList()
{
	return m_vecRewardAlready;
}

const std::vector<std::string>& DiceGameRewardManager::GetRewardList()
{
	return m_vecRewardList;
}
