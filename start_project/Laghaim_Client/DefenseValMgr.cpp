#include "stdafx.h"
#include "DefenseValMgr.h"
#include "LHExcelReader.h"

#include <d3d.h>
#include "ItemProto.h"
#include "Item.h"
#include "SpecialItem.h"

extern int g_ArmorSpecialMask[SPECIAL_ARMOR_NUM];
extern int g_ArmorSpecialShift[SPECIAL_ARMOR_NUM];
extern int g_ArmorSpecialValue[SPECIAL_ARMOR_NUM][SPECIAL_LEVEL_NUM];

DefenseValMgr::DefenseValMgr()
{

}

DefenseValMgr::~DefenseValMgr()
{
	clear();
}

bool DefenseValMgr::IsExist(const int& vnum)
{
	MAP_DEF_VAL::iterator itFindVnum = m_mapDefVal.find(vnum);
	if( itFindVnum == m_mapDefVal.end() )
		return false;

	return true;
}

int DefenseValMgr::GetLaim(const int& vnum, const int& plus)
{
	int ret = 0;

	MAP_DEF_VAL::iterator itFindVnum = m_mapDefVal.find(vnum);
	if( itFindVnum == m_mapDefVal.end() )
		return ret;

	MAP_PLUS_DEFENSE mapPlusDef = (*itFindVnum).second;
	MAP_PLUS_DEFENSE::iterator itFindPlus = mapPlusDef.find(plus);
	if( itFindPlus == mapPlusDef.end() )
		return ret;

	return (*itFindPlus).second->needLaim;
}

int DefenseValMgr::GetGemVnum(const int& vnum, const int& plus, const int& gemNum)
{
	if( gemNum < 0 || gemNum >= NEEDGEM_MAX )
		return 0;

	int ret = 0;

	MAP_DEF_VAL::iterator itFindVnum = m_mapDefVal.find(vnum);
	if( itFindVnum == m_mapDefVal.end() )
		return ret;

	MAP_PLUS_DEFENSE mapPlusDef = (*itFindVnum).second;
	MAP_PLUS_DEFENSE::iterator itFindPlus = mapPlusDef.find(plus);
	if( itFindPlus == mapPlusDef.end() )
		return ret;

	return (*itFindPlus).second->needGemVnum[gemNum];
}
int DefenseValMgr::GetGemCount(const int& vnum, const int& plus, const int& gemNum)
{
	if( gemNum < 0 || gemNum >= NEEDGEM_MAX )
		return 0;

	int ret = 0;

	MAP_DEF_VAL::iterator itFindVnum = m_mapDefVal.find(vnum);
	if( itFindVnum == m_mapDefVal.end() )
		return ret;

	MAP_PLUS_DEFENSE mapPlusDef = (*itFindVnum).second;
	MAP_PLUS_DEFENSE::iterator itFindPlus = mapPlusDef.find(plus);
	if( itFindPlus == mapPlusDef.end() )
		return ret;

	return (*itFindPlus).second->needGemCount[gemNum];
}

int DefenseValMgr::GetAbilityType(const int& vnum, const int& plus)
{
	int ret = 0;

	MAP_DEF_VAL::iterator itFindVnum = m_mapDefVal.find(vnum);
	if( itFindVnum == m_mapDefVal.end() )
		return ret;

	MAP_PLUS_DEFENSE mapPlusDef = (*itFindVnum).second;
	MAP_PLUS_DEFENSE::iterator itFindPlus = mapPlusDef.find(plus);
	if( itFindPlus == mapPlusDef.end() )
		return ret;

	return (*itFindPlus).second->abilityType;
}

int DefenseValMgr::GetAbilityVal(const int& vnum, const int& plus)
{
	int ret = 0;

	MAP_DEF_VAL::iterator itFindVnum = m_mapDefVal.find(vnum);
	if( itFindVnum == m_mapDefVal.end() )
		return ret;

	MAP_PLUS_DEFENSE mapPlusDef = (*itFindVnum).second;
	MAP_PLUS_DEFENSE::iterator itFindPlus = mapPlusDef.find(plus);
	if( itFindPlus == mapPlusDef.end() )
		return ret;

	return (*itFindPlus).second->abilitys[(*itFindPlus).second->abilityType];
}

int DefenseValMgr::GetDefense(const int& vnum, const int& plus)
{
	int ret = 0;

	MAP_DEF_VAL::iterator itFindVnum = m_mapDefVal.find(vnum);
	if( itFindVnum == m_mapDefVal.end() )
		return ret;

	MAP_PLUS_DEFENSE mapPlusDef = (*itFindVnum).second;
	MAP_PLUS_DEFENSE::iterator itFindPlus = mapPlusDef.find(plus);
	if( itFindPlus == mapPlusDef.end() )
		return ret;

	return (*itFindPlus).second->abilitys[eUPGRADE_ABILITY_TYPE_DEFENSE];
}

int DefenseValMgr::GetDefense(CItem* pItem)
{
	if( pItem == NULL )
		return 0;

	int ret = GetDefense(pItem->m_Vnum, pItem->m_PlusNum);
	if( ret == 0 )
		return 0;

	ret += getSpecialDefense(pItem, ret);

	return ret;
}

int DefenseValMgr::getSpecialDefense(CItem* pItem, int curRet)
{
	if( pItem == NULL )
		return 0;

	int ret = 0;

	int type = GET_TYPE(pItem);
	int shape = GET_SHAPE(pItem);
	int special = pItem->m_Special;
	int special2 = pItem->m_Special2;

	if( type != ITYPE_ARMOR )
		return ret;

	if( shape > IARMOR_SHIELD )
		return ret;

	int special_level = (special & g_ArmorSpecialMask[SPECIAL_SPDEFENSE_ALL]) >> g_ArmorSpecialShift[SPECIAL_SPDEFENSE_ALL];
	if (special_level > 0)
	{
		return g_ArmorSpecialValue[SPECIAL_SPDEFENSE_ALL][special_level-1];
	}
	for(int sp_idx = SPECIAL_ARMOR_NUM-1; sp_idx >= 0; sp_idx--)
	{
		int special_level = (special & g_ArmorSpecialMask[sp_idx]) >> g_ArmorSpecialShift[sp_idx];
		if (special_level > 0)
		{
			switch (sp_idx)
			{
			case SPECIAL_DEFENSE_ALL:
				return g_ArmorSpecialValue[sp_idx][special_level-1];
				break;
			case SPECIAL_DEFENSE_PER:
				return curRet * g_ArmorSpecialValue[sp_idx][special_level-1] / 100;
				break;
			}
		}
	}

	return ret;
}

void DefenseValMgr::Load()
{
	LHExcelReader er;
	if( er.Open("data/DEFENSE_TABLE.txt") == false )
		return;

	int vnum = 0;
	int plus = 0;
	int val = 0;
	int col = 0;
	int rowCount = er.GetRowSize();
	for(int i = 0; i < rowCount; i++)
	{
		col = 0;

		vnum = er.GetInt(i, col++);
		plus = er.GetInt(i, col++);
		addKey(vnum, plus);
		stDefenseVal* info;
		if( false == getInfo(vnum, plus, &info) )
			continue;

		info->needLaim = er.GetInt(i, col++);
		for(int gemCnt = 0; gemCnt < NEEDGEM_MAX; gemCnt++)
		{
			info->needGemVnum[gemCnt] = er.GetInt(i, col++);
			info->needGemCount[gemCnt] = er.GetInt(i, col++);
		}

		info->abilityType = er.GetInt(i, col++);
		for(int abilCnt = 0; abilCnt < eUPGRADE_ABILITY_TYPE_MAX; abilCnt++)
		{
			info->abilitys[abilCnt] = er.GetInt(i, col++);
		}
	}

	er.Close();
}

void DefenseValMgr::addKey(const int& vnum, const int& plus)
{
	MAP_PLUS_DEFENSE mapPlusDef;

	MAP_DEF_VAL::iterator itFindVnum = m_mapDefVal.find(vnum);
	if( itFindVnum != m_mapDefVal.end() )
	{
		mapPlusDef = (*itFindVnum).second;
	}

	MAP_PLUS_DEFENSE::iterator itFindPlus = mapPlusDef.find(plus);
	if( itFindPlus != mapPlusDef.end() )
		return;

	stDefenseVal* val = new stDefenseVal();
	val->itemVnum = vnum;
	val->plusPoint = plus;
	mapPlusDef.insert(std::make_pair(plus, val));

	m_mapDefVal[vnum] = mapPlusDef;
}

bool DefenseValMgr::getInfo(const int& vnum, const int& plus, stDefenseVal** outVal)
{
	MAP_PLUS_DEFENSE mapPlusDef;

	MAP_DEF_VAL::iterator itFindVnum = m_mapDefVal.find(vnum);
	if( itFindVnum == m_mapDefVal.end() )
	{
		return false;
	}
	else
	{
		mapPlusDef = (*itFindVnum).second;
	}

	MAP_PLUS_DEFENSE::iterator itFindPlus = mapPlusDef.find(plus);
	if( itFindPlus == mapPlusDef.end() )
		return false;

	*outVal = (*itFindPlus).second;

	return true;
}

void DefenseValMgr::clear()
{
	MAP_DEF_VAL::iterator itDefVal = m_mapDefVal.begin();
	MAP_DEF_VAL::iterator itDefValEnd = m_mapDefVal.end();

	MAP_PLUS_DEFENSE mapPlusDef;
	MAP_PLUS_DEFENSE::iterator itPlusDef;
	MAP_PLUS_DEFENSE::iterator itPlusDefEnd;

	for( ; itDefVal != itDefValEnd; itDefVal++ )
	{
		mapPlusDef = (*itDefVal).second;
		itPlusDef = mapPlusDef.begin();
		itPlusDefEnd = mapPlusDef.end();
		for( ; itPlusDef != itPlusDefEnd; itPlusDef++ )
		{
			SAFE_DELETE((*itPlusDef).second);
		}

		mapPlusDef.clear();
	}

	m_mapDefVal.clear();
}