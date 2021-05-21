#ifndef _LHItemSystem_Manager_H_
#define _LHItemSystem_Manager_H_

#include "LHSingleton.h"

#include <map>
#include <vector>



class CItem;

class LHParam_MakeItem;
class LHParam_SeperateItem;
class LHParam_GMagicStone;

class LHParam_ConfusionItem;
class LHParam_EnchantItem;
class LHParam_EnchantItem_Ex;

typedef std::map<int, LHParam_MakeItem*>			LHParam_MakeItem_Map;
typedef LHParam_MakeItem_Map::iterator				LHParam_MakeItem_Map_it;
typedef LHParam_MakeItem_Map::value_type			LHParam_MakeItem_Map_vt;


enum LHIS_Type
{
	LHIS_Type__MakeItem,
	LHIS_Type__SeperateItem,
	LHIS_Type__GMagicStone,

	LHIS_Type__Enchant,
	LHIS_Type__Confusion,
	LHIS_Type__Conversion, // 제작쪽으로 묶는 방향을 생각해보자!

	LHIS_Type__GodWeaponUpgrade,

	LHIS_Type__EndOfEnum
};


class LHItemSystem_Manager : public LHSingleton<LHItemSystem_Manager>
{
protected:
	LHParam_MakeItem *		m_pMakeitem;
	LHParam_SeperateItem *	m_pSeparateitem;
	LHParam_GMagicStone *	m_pGMagicStone;


public:
	void Init();
	void Release();

public:
	LHParam_MakeItem *		GetMakeItemParam()
	{
		return m_pMakeitem;
	}
	LHParam_SeperateItem *	GetSeperateItemParam()
	{
		return m_pSeparateitem;
	}
	LHParam_GMagicStone *	GetGMagicStoneParam()
	{
		return m_pGMagicStone;
	}




protected:
	LHParam_ConfusionItem *		m_confusion_table;
	LHParam_EnchantItem *		m_enchant_table;
	LHParam_EnchantItem_Ex *	m_enchant_ex_table;


public:
	LHParam_ConfusionItem *	GetConfusionItemParam()
	{
		return m_confusion_table;
	}
	LHParam_EnchantItem *	GetEnchantItemParam()
	{
		return m_enchant_table;
	}
	LHParam_EnchantItem_Ex * GetEnchantItemExParam()
	{
		return m_enchant_ex_table;
	}


public:
	bool CheckEquip(CItem * item);
	bool CheckJewelry(CItem * item);

	bool Execute_Confusion();
	bool Execute_Enchant();


public:
	LHItemSystem_Manager(void);
	~LHItemSystem_Manager(void);
};

extern LHItemSystem_Manager theItemSystem;


#define LHIS_MAKE()			LHItemSystem_Manager::GetSingletonPtr()->GetMakeItemParam()
#define LHIS_SEPERATE()		LHItemSystem_Manager::GetSingletonPtr()->GetSeperateItemParam()
#define LHIS_GMAGICSTONE()	LHItemSystem_Manager::GetSingletonPtr()->GetGMagicStoneParam()
#define LHIS_ENCHANT()		LHItemSystem_Manager::GetSingletonPtr()->GetEnchantItemParam()
#define LHIS_ENCHANT_EX()	LHItemSystem_Manager::GetSingletonPtr()->GetEnchantItemExParam()

#endif // _LHItemSystem_Manager_H_