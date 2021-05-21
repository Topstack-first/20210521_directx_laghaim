#include "stdafx.h"
#include "LHItemSystem_Manager.h"
#include "LHParam_MakeItem.h"
#include "LHParam_SeperateItem.h"
#include "LHParam_GMagicStone.h"

#include "LHParam_EnchantItem.h"
#include "LHParam_ConfusionItem.h"

#include "UIMgr.h"

LHItemSystem_Manager theItemSystem;

LHItemSystem_Manager::LHItemSystem_Manager(void)
	:	m_pMakeitem(new LHParam_MakeItem)
	,	m_pSeparateitem(new LHParam_SeperateItem)
	,	m_pGMagicStone(new LHParam_GMagicStone)
	,	m_confusion_table(new LHParam_ConfusionItem)
	,	m_enchant_table(new LHParam_EnchantItem)
	,	m_enchant_ex_table(new LHParam_EnchantItem_Ex)
{
	Init();
}

LHItemSystem_Manager::~LHItemSystem_Manager(void)
{
	safe_delete(m_pMakeitem);
	safe_delete(m_pSeparateitem);
	safe_delete(m_pGMagicStone);
	safe_delete(m_confusion_table);
	safe_delete(m_enchant_table);
	safe_delete(m_enchant_ex_table);

	Release();
}

void LHItemSystem_Manager::Init()
{
}

void LHItemSystem_Manager::Release()
{
}

bool LHItemSystem_Manager::CheckEquip(CItem * item)
{
	if( !item )
		return false;

	if( GET_TYPE(item) == ITYPE_WEAPON || GET_TYPE(item) == ITYPE_ARMOR )
		return true;

	return false;
}

bool LHItemSystem_Manager::CheckJewelry(CItem * item)
{
	if( !item )
		return false;

	if( GET_TYPE(item) == ITYPE_ETC && GET_SHAPE(item) == IETC_GEM )
		return true;

	return false;
}


bool LHItemSystem_Manager::Execute_Confusion()
{
	if( !m_confusion_table )
	{
		return false;
	}

	assert(!g_pRoh);

	CItem * item = g_pRoh->m_ExgInven;
	if( !item )
	{
		return false;		
	}

	CItem * equip_item = NULL;
	int gem_count[eGem_Grade__20] = {0,};
	int equip_count[2] = {0,};				// 0:armor, 1:weapon

	while( item )
	{
		if( GET_TYPE(item) == ITYPE_ARMOR )
		{
			equip_item = item;
			equip_count[0]++;
		}
		if(GET_TYPE(item) == ITYPE_WEAPON )
		{
			equip_item = item;
			equip_count[1]++;
		}
		if( GET_TYPE(item) == ITYPE_ETC && GET_SHAPE(item) == IETC_GEM )
		{
			int g_grade = 0;
			gem_count[g_grade]++;
		}

		if( equip_count[0]+equip_count[1] > 1 )
		{
			return false;			
		}

		item = item->m_Next;
	}

	if( !equip_item )
		return false;

	int current_equip_grade = equip_item->m_item_grade;

	LHParam_ConfusionItem::sData * param = m_confusion_table->GetParam((eItem_Grade_Type)current_equip_grade);

	if( false == param->enable )
	{
		return false;		
	}

	if( g_pRoh->m_Money < param->need_laim )
	{
		return false;		
	}

	if( gem_count[current_equip_grade] == 0 )
	{
		return false;		
	}

	int num_gem = 0;
	for( int i=0 ; i<eGem_Grade__20 ; i++ )
	{
		if( gem_count[i] )
			num_gem += gem_count[i];
	}

	if( num_gem == 0 )
	{
		return false;		
	}

	if( param->need_gem_count != num_gem )
	{
		return false;		
	}

	return true;
}

bool LHItemSystem_Manager::Execute_Enchant()
{
	return true;
}
