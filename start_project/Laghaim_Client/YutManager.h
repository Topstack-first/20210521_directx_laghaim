#ifndef _YUTMANAGER_H_
#define _YUTMANAGER_H_

#include "singleton.h"

enum eYutResult
{
	eYutResult_Do = 0,
	eYutResult_Gae,
	eYutResult_Gul,
	eYutResult_Yut,
	eYutResult_Mo,
	eYutResult_Back,

	eYutResult_MAX,
};

class YutManager : public CSingleton<YutManager>
{
public:
	typedef std::map<int, POINT> MAP_POSITION;
	YutManager();
	~YutManager();

public:
	void Clear();

public:
	void SetYutnoriOn(bool bOn)
	{
		m_isYutnoriOn = bOn;
	}
	bool GetYutnoriOn()
	{
		return m_isYutnoriOn;
	}
	void SetPosIndex(int index)
	{
		m_unitPosIndex = index;
	}
	int GetPosIndex()
	{
		return m_unitPosIndex;
	}
	void SetCompleteCount(int count)
	{
		m_completeCount = count;
	}
	int GetCompleteCount()
	{
		return m_completeCount;
	}
	POINT GetUIPos(int index = -1);
	int GetRoute(int curIndex, int dice);

private:
	void load();

private:
	bool m_isYutnoriOn;
	int m_completeCount;
	int m_unitPosIndex;
	MAP_POSITION m_mapPos;
};

#endif // _YUTMANAGER_H_
