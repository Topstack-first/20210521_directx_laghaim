#ifndef _DICEGAME_MANAGER_H_
#define _DICEGAME_MANAGER_H_

#include "singleton.h"

class DiceGameManager : public CSingleton<DiceGameManager>
{
public:
	DiceGameManager();
	~DiceGameManager();

public:
	void Clear();

public:
	void SetDiceGameOn(bool bOn)
	{
		m_isDiceGameOn = bOn;
	}
	bool GetDiceGameOn()
	{
		return m_isDiceGameOn;
	}

	void SetSuccessCount(int count);
	int  GetSuccessCount();

private:
	bool m_isDiceGameOn;
	int m_SuccessCount;
};

#endif // _DICEGAME_MANAGER_H_
