#ifndef _BASEBALLMANAGER_H_
#define _BASEBALLMANAGER_H_

#include "singleton.h"
#include "BaseballRewardManager.h"

class BaseballManager : public CSingleton<BaseballManager>
{
public:
	BaseballManager();
	~BaseballManager();

public:
	void Clear();

public:
	void SetBaseballOn(bool bOn)
	{
		m_isBaseballOn = bOn;
	}
	bool GetBaseballOn()
	{
		return m_isBaseballOn;
	}

	void SetBallCount(int index, int count);
	int  GetBallCount(int index);

	int GetBallSpeed();

private:
	void load();

private:
	bool m_isBaseballOn;
	int  m_nBallCount[MAX_BASEBALL_GRADE];
	int  m_nBallSpeed;
};


#endif // _BASEBALLMANAGER_H_
