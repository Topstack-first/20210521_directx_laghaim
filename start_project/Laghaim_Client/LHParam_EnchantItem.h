#ifndef _LHParam_EnchantItem_H_
#define _LHParam_EnchantItem_H_

#pragma once

#include "ItemProto.h"

class LHParam_EnchantItem
{
public:
	struct sData
	{
		int m_upgrade_type;		// �ϴ��� ���ǹ���� 0���� ����.
		int m_item_index;		// �����۹�ȣ
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
		int m_upgrade_type;					// �ϴ� 0�� ���ǹ�������
		int	m_upgrade_value;				// plus ��ġ
		int m_upgrade_addvalue;				// ������(������ġ)-����
		int m_upgrade_laim;					// �ʿ� ����
		int m_penalty_type;					//
		int m_gem[3];						// ���� ����
		int m_probability;					// ����Ȯ��-����
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
	const sData * GetParam(int upgrade_type, int upgrade_value); // ���簪 + 1�� �Ѱ�����Ѵ�.


public:
	LHParam_EnchantItem_Ex();
	~LHParam_EnchantItem_Ex();
};

#endif // _LHParam_EnchantItem_H_