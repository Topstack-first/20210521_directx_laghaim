#ifndef _SET_ITEM_EFF_MGR_H_
#define _SET_ITEM_EFF_MGR_H_

#include "singleton.h"
#include "info_define.h"

class SetItemEffMgr : public CSingleton<SetItemEffMgr>
{
private:
	typedef std::map< int, std::vector<SetItemStatus> > MAP_SETITEM;

public:
	SetItemEffMgr();
	~SetItemEffMgr();

public:
	void Clear();

	void AddEffect(const int* itemIndices, const int statusCount, const SetItemStatus* statuses);
	void RemoveEffect(const int* itemIndices);

	std::vector< std::pair<int, int> > GetEffect(int itemIndex);

private:
	MAP_SETITEM m_mapEff[WEARING_NUM];
};

#endif // _SET_ITEM_EFF_MGR_H_
