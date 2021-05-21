#include "stdafx.h"
#include "LHParam_EnchantItem.h"
#include "LHExcelReader.h"

#include "UIMgr.h"

const char * enchant_filename = "data/tables/enchant_table.txt";
const char * enchant_ex_filename = "data/tables/enchant_ex_table.txt";



LHParam_EnchantItem::LHParam_EnchantItem()
	:	m_initialized(false)
	,	m_enchant_map(new LHEnchnatMap)
{
	Init();
}

LHParam_EnchantItem::~LHParam_EnchantItem(void)
{
	safe_delete_associate(*m_enchant_map);
	safe_delete(m_enchant_map);
}

void LHParam_EnchantItem::Init()
{
}

void LHParam_EnchantItem::Release()
{
}

bool LHParam_EnchantItem::Open()
{
	if( m_initialized )
		return true;

	LHExcelReader er;

	if( !er.Open( enchant_filename ) )
		return false;


	int upgrade_type = -1;

	for( int row=0 ; row<er.GetRowSize() ; ++row )
	{
		sData * param = new sData;

		int col = 0;
		int type = er.GetInt(row, col++);;

		if( type == -1 && upgrade_type != -1 )
			type = upgrade_type;


		param->m_upgrade_type = type;
		param->m_item_index = er.GetInt(row, col++);

		m_enchant_map->insert( LHEnchnatMap_vt(param->m_upgrade_type, param) );
	}

	m_initialized = true;

	return true;
}

void LHParam_EnchantItem::Close()
{
}

bool LHParam_EnchantItem::IsUpgadeType(int upgrade_type, int item_index)
{
	if( m_enchant_map->empty() )
		Open();

	for( LHEnchnatMap_it it=m_enchant_map->lower_bound(upgrade_type) ; it!=m_enchant_map->upper_bound(upgrade_type) ; ++it )
	{
		if( (*it).second && (*it).second->m_item_index == item_index )
		{
			return true;
		}
	}

	return false;
}

int LHParam_EnchantItem::IsUpgadeType(int item_index)
{
	if( m_enchant_map->empty() )
		Open();

	LHEnchnatMap_it it = m_enchant_map->begin();

	while( it!=m_enchant_map->end() )
	{
		if( (*it).second->m_item_index == item_index )
			return (*it).first;
		it++;
	}

	return -1;
}

const LHParam_EnchantItem::sData * LHParam_EnchantItem::GetParam(CItem * item)
{
	if( !m_initialized )
	{
		if( false == Open() )
			return NULL;
	}

	return NULL;
}

bool LHParam_EnchantItem::CheckUpgrade(CItem ** selected_item, int upgrade_type)
{
	return true;
}


int LHParam_EnchantItem::CheckUpgrade(CItem ** selected_item)
{
	if( !m_initialized )
	{
		if( false == Open() )
			return -1;
	}


	CItem * ex_item = g_pRoh->m_ExgInven;

	if( !ex_item )
		return NULL;

	int upgrade_type = -1;
	CItem * temp_item = NULL;

	while( ex_item )
	{
		if( GET_TYPE(ex_item) == ITYPE_WEAPON || GET_TYPE(ex_item) == ITYPE_ARMOR )
		{
			if( ex_item->m_LimitTime || temp_item ) // 시간제이거나, 무기&방어구가 두개이상 올라가있다.
			{
				*selected_item = NULL;
				return -1;
			}

			upgrade_type = IsUpgadeType(ex_item->m_Vnum);
			temp_item = ex_item;
			*selected_item = ex_item;
		}

		ex_item = ex_item->m_Next;
	}

	return upgrade_type;
}

int LHParam_EnchantItem::CheckUpgrade_Weapon(CItem ** selected_item)
{
	if( !m_initialized )
	{
		if( false == Open() )
			return -1;
	}


	CItem * ex_item = g_pRoh->m_ExgInven;

	if( !ex_item )
		return NULL;

	int upgrade_type = -1;
	CItem * temp_item = NULL;

	while( ex_item )
	{
		if( GET_TYPE(ex_item) == ITYPE_WEAPON )
		{
			if( ex_item->m_LimitTime || temp_item ) // 시간제이거나, 무기&방어구가 두개이상 올라가있다.
			{
				*selected_item = NULL;
				return -1;
			}

			upgrade_type = IsUpgadeType(ex_item->m_Vnum);
			temp_item = ex_item;
			*selected_item = ex_item;
		}

		ex_item = ex_item->m_Next;
	}

	return upgrade_type;
}
int LHParam_EnchantItem::CheckUpgrade_Armor(CItem ** selected_item)
{
	if( !m_initialized )
	{
		if( false == Open() )
			return -1;
	}


	CItem * ex_item = g_pRoh->m_ExgInven;

	if( !ex_item )
		return NULL;

	int upgrade_type = -1;
	CItem * temp_item = NULL;

	while( ex_item )
	{
		if( GET_TYPE(ex_item) == ITYPE_ARMOR )
		{
			if( ex_item->m_LimitTime || temp_item ) // 시간제이거나, 무기&방어구가 두개이상 올라가있다.
			{
				*selected_item = NULL;
				return -1;
			}

			upgrade_type = IsUpgadeType(ex_item->m_Vnum);
			temp_item = ex_item;
			*selected_item = ex_item;
		}

		ex_item = ex_item->m_Next;
	}

	return upgrade_type;
}
int LHParam_EnchantItem::CheckUpgrade_Sub(CItem ** selected_item)
{
	if( !m_initialized )
	{
		if( false == Open() )
			return -1;
	}

	CItem * ex_item = g_pRoh->m_ExgInven;

	if( !ex_item )
		return NULL;

	int upgrade_type = -1;
	CItem * temp_item = NULL;

	while( ex_item )
	{
		if( GET_TYPE(ex_item) == ITYPE_SUB )
		{
			if( ex_item->m_LimitTime || temp_item ) // 시간제이거나, 무기&방어구가 두개이상 올라가있다.
			{
				*selected_item = NULL;
				return -1;
			}

			upgrade_type = IsUpgadeType(ex_item->m_Vnum);
			temp_item = ex_item;
			*selected_item = ex_item;
		}

		ex_item = ex_item->m_Next;
	}

	return upgrade_type;
}

bool LHParam_EnchantItem::SkipItem(CItem * item)
{
	if( !item )
		return false;
	if( GET_TYPE(item) != ITYPE_ETC )
		return false;
	if( GET_SHAPE(item) != IETC_GEM )
		return false;

	return true;
}

LHParam_EnchantItem_Ex::LHParam_EnchantItem_Ex()
	:	m_initialized(false)
	,	m_enchant_ex_map(new LHEnchantExMap)
{
	Init();
}

LHParam_EnchantItem_Ex::~LHParam_EnchantItem_Ex()
{
	safe_delete_associate(*m_enchant_ex_map);
	safe_delete(m_enchant_ex_map);
}

void LHParam_EnchantItem_Ex::Init()
{
}

void LHParam_EnchantItem_Ex::Release()
{
}

bool LHParam_EnchantItem_Ex::Open()
{
	if( m_initialized )
		return true;

	LHExcelReader er;

	if( !er.Open( enchant_ex_filename ) )
		return false;

	for( int row=0 ; row<er.GetRowSize() ; ++row )
	{
		sData * param = new sData;

		int col = 0;
		param->m_upgrade_type		= er.GetInt(row, col++);
		param->m_upgrade_value		= er.GetInt(row, col++);
		param->m_upgrade_addvalue	= er.GetInt(row, col++);
		param->m_upgrade_laim		= er.GetInt(row, col++);
		param->m_penalty_type		= er.GetInt(row, col++);

		for( int i=0 ; i<3 ; i++ )
			param->m_gem[i]	= er.GetInt(row, col++);

		param->m_probability		= er.GetInt(row, col++);

		m_enchant_ex_map->insert(LHEnchantExMap_vt(param->m_upgrade_type , param));
	}

	m_initialized = true;

	return true;
}

void LHParam_EnchantItem_Ex::Close()
{
}

bool LHParam_EnchantItem_Ex::CheckGem(int upgrade_type, int plus_value)
{
	const LHParam_EnchantItem_Ex::sData * param = GetParam(upgrade_type, plus_value);

	if( !param )
		return false;

	CItem * ex_item = g_pRoh->m_ExgInven;

	int total_gem = 0;

	while( ex_item )
	{
		if( GET_TYPE(ex_item) == ITYPE_WEAPON || GET_TYPE(ex_item) == ITYPE_ARMOR || ex_item->m_Vnum == 1739 || ex_item->m_Vnum == 1740)
		{
			ex_item = ex_item->m_Next;
			continue;
		}

		if( param->m_gem[0] == ex_item->m_Vnum
				|| param->m_gem[1] == ex_item->m_Vnum )
		{
			total_gem++;
		}
		else if( param->m_gem[2] == ex_item->m_Vnum )
		{
			// 다른처리 없음
		}
		else
		{
			return false;
		}

		ex_item = ex_item->m_Next;
	}

	if( total_gem != 1 )
		return false;

	return true;
}


bool LHParam_EnchantItem_Ex::CheckEtc(int upgrade_type, int plus_value)
{
	const LHParam_EnchantItem_Ex::sData * param = GetParam(upgrade_type, plus_value);

	if( !param )
		return false;

	if( 0 == param->m_gem[2] )
		return true;

	CItem * ex_item = g_pRoh->m_ExgInven;

	int total_gem = 0;

	while( ex_item )
	{
		if( GET_TYPE(ex_item) == ITYPE_WEAPON || GET_TYPE(ex_item) == ITYPE_ARMOR || ex_item->m_Vnum == 1739 || ex_item->m_Vnum == 1740)
		{
			ex_item = ex_item->m_Next;
			continue;
		}
		if( param->m_gem[2] == ex_item->m_Vnum )
			total_gem++;

		ex_item = ex_item->m_Next;
	}

	if( total_gem != 1 )
		return false;

	return true;
}

int LHParam_EnchantItem_Ex::GetNeedLaim(int upgrade_type, int plus_value)
{
	const LHParam_EnchantItem_Ex::sData * param = GetParam(upgrade_type, plus_value);
	if( !param )
		return 0;

	return param->m_upgrade_laim;
}

int LHParam_EnchantItem_Ex::GetAddValue(int upgrade_type, int plus_value)
{
	const LHParam_EnchantItem_Ex::sData * param = GetParam(upgrade_type, plus_value);
	if( !param )
		return 0;

	return param->m_upgrade_addvalue;
}

const LHParam_EnchantItem_Ex::sData * LHParam_EnchantItem_Ex::GetParam(int upgrade_type, int upgrade_value)
{
	if( !m_initialized )
	{
		if( false == Open() )
			return NULL;
	}

	sData * param = NULL;

	for( LHEnchantExMap_it it=m_enchant_ex_map->lower_bound(upgrade_type) ; it!=m_enchant_ex_map->upper_bound(upgrade_type) ; ++it )
	{
		if( (*it).second && (*it).second->m_upgrade_value == upgrade_value )
		{
			param = (*it).second;
			return param;
		}
	}

	return NULL;
}