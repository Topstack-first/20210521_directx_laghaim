#ifndef _FISHINGMANAGER_H_
#define _FISHINGMANAGER_H_

#include "singleton.h"

#define MAX_FISHTYPE	10

struct stFishOffset
{
	std::string filename;
	POINT	offset;
};

class FishingManager : public CSingleton<FishingManager>
{
public:
	typedef std::map<int, stFishOffset>	MAP_FISHOFFSET;
	FishingManager();
	~FishingManager();

public:
	void Clear();

public:
	void SetFishingOn(bool bOn)
	{
		m_isFishingOn = bOn;
	}
	bool GetFishingOn()
	{
		return m_isFishingOn;
	}

	void SetFishCount(int index, int count);
	int	 GetFishCount(int index);

	std::string GetFileName(int index);
	POINT GetOffset(int index);

	int  GetBallSpeed();

private:
	void load();

private:
	bool m_isFishingOn;
	int  m_nFishCount[MAX_FISHTYPE];
	int  m_nBallSpeed;

	MAP_FISHOFFSET	m_mapOffset;
};

#endif
