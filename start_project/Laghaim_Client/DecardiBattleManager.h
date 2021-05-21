#ifndef _DECARDI_BATTLE_MANAGER_H_
#define _DECARDI_BATTLE_MANAGER_H_

#include "singleton.h"

#define DCB_TEAM_COUNT	2

struct attackableArea
{	
	void setData(int x1, int z1, int x2, int z2)
	{ 
		this->x1 = x1;
		this->z1 = z1;
		this->x2 = x2;
		this->z2 = z2;
	}

	bool isInside(int x, int z)
	{
		if( x >= x1 && x <= x2 && z >= z1 && z <= z2 )
			return true;

		return false;
	}

	int x1;
	int z1;
	int x2;
	int z2;
};

class DecardiBattleManager : public CSingleton<DecardiBattleManager>
{
public:
	// <charIndex, team>
	typedef std::map<int, int> MAP_DCB;
	DecardiBattleManager();
	~DecardiBattleManager();

public:
	void Clear();
	void ClearList();

	void AddTeam(const int team, const int charIndex);
	const int GetTeam(const int charIndex);
	const bool IsSameTeam(const int idx1, const int idx2);
	
	void SetPoint(const int team, const int point);
	const int GetPoint(const int team);

	void GetTime(int& min, int& sec);

	void Start();
	void End();

	const bool IsStart();

	void SetGate(const bool bDraw);
	const bool GetGate();

	const bool IsAttackable(const int x, const int z);

public:
	attackableArea m_attackable[5];

private:	
	bool m_bStart;
	bool m_bGate;
	int m_nPoint[DCB_TEAM_COUNT];
	MAP_DCB m_mapChar;
	DWORD m_dwStartTime;	
};

#endif // _DECARDI_BATTLE_MANAGER_H_
