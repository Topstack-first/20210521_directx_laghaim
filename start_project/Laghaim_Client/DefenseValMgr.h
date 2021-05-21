#ifndef _DEFENSE_VAL_MGR_H_
#define _DEFENSE_VAL_MGR_H_

#include "singleton.h"

enum eUPGRADE_ABILITY_TYPE
{
	eUPGRADE_ABILITY_TYPE_DEFENSE = 0,

	eUPGRADE_ABILITY_TYPE_MAX,
};

enum eUPGRADE_NEED_GEM
{
	GEM_0 = 0,
	GEM_1,
	GEM_2,
	RATEADD_GEM,
	NOTPENALTY_GEM,
	NEEDGEM_MAX,
};

struct stDefenseVal
{
	stDefenseVal()
		: itemVnum(0)
		, plusPoint(0)
		, needLaim(0)
		, abilityType(0)
	{
		memset(needGemVnum, 0, sizeof(needGemVnum));
		memset(needGemCount, 0, sizeof(needGemCount));
		memset(abilitys, 0, sizeof(abilitys));
	}

	int itemVnum;
	int plusPoint;
	__int64 needLaim;
	int needGemVnum[NEEDGEM_MAX];
	int needGemCount[NEEDGEM_MAX];
	int abilityType;
	int abilitys[eUPGRADE_ABILITY_TYPE_MAX];
};

class CItem;
class DefenseValMgr : public CSingleton<DefenseValMgr>
{
public:
	typedef std::map<int ,stDefenseVal*> MAP_PLUS_DEFENSE;
	typedef std::map<int, MAP_PLUS_DEFENSE> MAP_DEF_VAL;

	DefenseValMgr();
	~DefenseValMgr();

public:
	void Load();

public:
	bool IsExist(const int& vnum);
	int GetLaim(const int& vnum, const int& plus);
	int GetGemVnum(const int& vnum, const int& plus, const int& gemNum);
	int GetGemCount(const int& vnum, const int& plus, const int& gemNum);
	int GetAbilityType(const int& vnum, const int& plus);
	int GetAbilityVal(const int& vnum, const int& plus);
	int GetDefense(const int& vnum, const int& plus);
	int GetDefense(CItem* pItem);

public:
	void addKey(const int& vnum, const int& plus);
	bool getInfo(const int& vnum, const int& plus, stDefenseVal** outVal);
	void clear();

private:
	int getSpecialDefense(CItem* pItem, int curRet);

private:
	MAP_DEF_VAL m_mapDefVal;
};

#endif