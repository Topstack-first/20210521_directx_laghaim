#ifndef _EVENT_ONOFF_MGR_H_
#define _EVENT_ONOFF_MGR_H_

#include "singleton.h"

class EventOnOffMgr : public CSingleton<EventOnOffMgr>
{
public:
	typedef std::map<std::string, int> MAP_EVENT_ONOFF;
	EventOnOffMgr();
	~EventOnOffMgr();

private:
	void load();
	void checkPreData();

private:
	void checkYutnori();
	void checkFishing();
	void checkLotto();
	void checkBingo();
	void checkBaseball();
	void checkLottery();
	void checkDiceGame();

private:
	MAP_EVENT_ONOFF m_mapOnOff;
};

#endif // _EVENT_ONOFF_MGR_H_
