#ifndef _ITEM_OUTLINE_MGR_H_
#define _ITEM_OUTLINE_MGR_H_

#include "singleton.h"

#define ITEM_GRADE_COUNT	3
struct ItemOutlineInfo
{
	float width;
	int	grade[ITEM_GRADE_COUNT];
	DWORD color[ITEM_GRADE_COUNT];
};

enum eOutlineType
{
	eOutlineType_DSystem = 20,
	eOutlineType_NpcSelect,
	eOutlineType_MAX,
};

class ItemOutlineMgr : public CSingleton<ItemOutlineMgr>
{
private:
	typedef std::map<int, ItemOutlineInfo> MAP_ITEM_OUTLINE;

public:
	ItemOutlineMgr();
	~ItemOutlineMgr();

public:
	void Load();
	void Reload();

public:
	float	GetWidth(int wear);
	DWORD	GetColor(int wear, int forceLevel);

private:
	void	addVal(const int& wear, const ItemOutlineInfo& info);

private:
	MAP_ITEM_OUTLINE m_mapItemOutline;
};

#endif // _ITEM_OUTLINE_MGR_H_
