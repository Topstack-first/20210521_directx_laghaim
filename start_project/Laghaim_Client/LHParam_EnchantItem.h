#ifndef _LHParam_EnchantItem_H_
#define _LHParam_EnchantItem_H_

#pragma once

#include "ItemProto.h"

class LHParam_EnchantItem
{
public:
	struct sData
	{
		int m_upgrade_type;		// 일단은 신의무기는 0으로 간다.
		int m_item_index;		// 아이템번호
	};

private:
	bool m_initialized;

	typedef std::multimap<int, sData*>			LHEnchnatMap;
	typedef LHEnchnatMap::iterator				LHEnchnatMap_it;
	typedef LHEnchnatMap::value_type			LHEnchnatMap_vt;

	LHEnchnatMap * m_enchant_map;


public:
	void Init();
	void Release();
	bool Open();
	void Close();


public:
	bool IsUpgadeType(int upgrade_type, int item_index);
	int IsUpgadeType(int item_index);

	int CheckUpgrade(CItem ** selected_item);
	int CheckUpgrade_Weapon(CItem ** selected_item);
	int CheckUpgrade_Armor(CItem ** selected_item);
	int CheckUpgrade_Sub(CItem ** selected_item);
	bool SkipItem(CItem * item);
	bool CheckUpgrade(CItem ** selected_item, int upgrade_type);





public:
	const sData * GetParam(CItem * item);

	sData * CheckMain();
	bool CheckSub(const LHParam_EnchantItem::sData * param);


public:
	LHParam_EnchantItem();
	~LHParam_EnchantItem(void);
};

class LHParam_EnchantItem_Ex
{
public:
	struct sData
	{
		int m_upgrade_type;					// 일단 0이 신의무기등급임
		int	m_upgrade_value;				// plus 수치
		int m_upgrade_addvalue;				// 증가분(누적수치)-서버
		int m_upgrade_laim;					// 필요 라임
		int m_penalty_type;					//
		int m_gem[3];						// 가능 보석
		int m_probability;					// 성공확률-서버
	};


private:
	typedef std::multimap<int,sData*>		LHEnchantExMap;
	typedef LHEnchantExMap::iterator		LHEnchantExMap_it;
	typedef LHEnchantExMap::value_type		LHEnchantExMap_vt;


	bool m_initialized;
	LHEnchantExMap * m_enchant_ex_map;



public:
	void Init();
	void Release();
	bool Open();
	void Close();


public:
	bool CheckGem(int upgrade_type, int plus_value);
	bool CheckEtc(int upgrade_type, int plus_value);
	int GetNeedLaim(int upgrade_type, int plus_value);
	int GetAddValue(int upgrade_type, int plus_value);



public:
	const sData * GetParam(int upgrade_type, int upgrade_value); // 현재값 + 1을 넘겨줘야한다.


public:
	LHParam_EnchantItem_Ex();
	~LHParam_EnchantItem_Ex();
};

#endif // _LHParam_EnchantItem_H_