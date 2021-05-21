#include "stdafx.h"
#include "EventOnOffMgr.h"
#include "LHExcelReader.h"
#include "YutManager.h"
#include "FishingManager.h"
#include "LottoManager.h"
#include "BingoManager.h"
#include "BaseballManager.h"
#include "LotteryManager.h"
#include "DiceGameManager.h"

EventOnOffMgr::EventOnOffMgr()
{
	load();
}

EventOnOffMgr::~EventOnOffMgr()
{
}

void EventOnOffMgr::load()
{
	LHExcelReader er;
	if( er.Open("data/EVENT_ONOFF.TXT") == false )
		return;

	std::string eventName;
	int onoff = 0;
	int col = 0;
	int rowCount = er.GetRowSize();
	for(int i = 0; i < rowCount; i++)
	{
		col = 0;
		eventName = er.GetData(i, col++);
		onoff = er.GetInt(i, col++);

		m_mapOnOff.insert(std::make_pair(eventName, onoff));
	}

	checkPreData();
}

void EventOnOffMgr::checkPreData()
{
	checkYutnori();
	checkFishing();
	checkLotto();
	checkBingo();
	checkBaseball();
	checkLottery();
	checkDiceGame();
}

void EventOnOffMgr::checkYutnori()
{
	MAP_EVENT_ONOFF::iterator itFind = m_mapOnOff.find("yutnori");
	if( itFind == m_mapOnOff.end() )
	{
		YutManager::GetInstance()->SetYutnoriOn(false);
		return;
	}

	int onoff = itFind->second;
	bool bOn = onoff == 0 ? false : true;
	YutManager::GetInstance()->SetYutnoriOn(bOn);
}

void EventOnOffMgr::checkFishing()
{
	MAP_EVENT_ONOFF::iterator itFind = m_mapOnOff.find("fishing");
	if( itFind == m_mapOnOff.end() )
	{
		FishingManager::GetInstance()->SetFishingOn(false);
		return;
	}

	int onoff = itFind->second;
	bool bOn = onoff == 0 ? false : true;
	FishingManager::GetInstance()->SetFishingOn(bOn);
}

void EventOnOffMgr::checkLotto()
{
	MAP_EVENT_ONOFF::iterator itFind = m_mapOnOff.find("lotto");
	if( itFind == m_mapOnOff.end() )
	{
		LottoManager::GetInstance()->SetLottoOn(false);
		return;
	}

	int onoff = itFind->second;
	bool bOn = onoff == 0 ? false : true;
	LottoManager::GetInstance()->SetLottoOn(bOn);	
}

void EventOnOffMgr::checkBingo()
{
	MAP_EVENT_ONOFF::iterator itFind = m_mapOnOff.find("bingo");
	if( itFind == m_mapOnOff.end() )
	{
		BingoManager::GetInstance()->SetBingoOn(false);
		return;
	}

	int onoff = itFind->second;
	bool bOn = onoff == 0 ? false : true;
	BingoManager::GetInstance()->SetBingoOn(bOn);	
}

void EventOnOffMgr::checkBaseball()
{
	MAP_EVENT_ONOFF::iterator itFind = m_mapOnOff.find("baseball");
	if( itFind == m_mapOnOff.end() )
	{
		BaseballManager::GetInstance()->SetBaseballOn(false);
		return;
	}

	int onoff = itFind->second;
	bool bOn = onoff == 0 ? false : true;
	BaseballManager::GetInstance()->SetBaseballOn(bOn);	
}

void EventOnOffMgr::checkLottery()
{
	MAP_EVENT_ONOFF::iterator itFind = m_mapOnOff.find("lottery");
	if( itFind == m_mapOnOff.end() )
	{
		LotteryManager::GetInstance()->SetLotteryOn(false);
		return;
	}

	int onoff = itFind->second;
	bool bOn = onoff == 0 ? false : true;
	LotteryManager::GetInstance()->SetLotteryOn(bOn);	
}

void EventOnOffMgr::checkDiceGame()
{
	MAP_EVENT_ONOFF::iterator itFind = m_mapOnOff.find("dicegame");
	if( itFind == m_mapOnOff.end() )
	{
		DiceGameManager::GetInstance()->SetDiceGameOn(false);
		return;
	}

	int onoff = itFind->second;
	bool bOn = onoff == 0 ? false : true;
	DiceGameManager::GetInstance()->SetDiceGameOn(bOn);	
}
