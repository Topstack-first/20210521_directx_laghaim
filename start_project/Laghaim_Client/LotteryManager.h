#ifndef _LOTTERYMANAGER_H_
#define _LOTTERYMANAGER_H_

#include "singleton.h"

class LotteryManager : public CSingleton<LotteryManager>
{
public:
	LotteryManager();
	~LotteryManager();

public:
	void Clear();

public:
	void SetLotteryOn(bool bOn)
	{
		m_isLotteryOn = bOn;
	}
	bool GetLotteryOn()
	{
		return m_isLotteryOn;
	}	

private:
	bool m_isLotteryOn;	
};

#endif // _LOTTERYMANAGER_H_
