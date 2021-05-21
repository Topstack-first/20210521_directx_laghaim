#include "stdafx.h"
#include "LHParam_ConfusionItem.h"
#include "LHExcelReader.h"

#include "NkCharacter.h"
#include "Item.h"

extern CNkCharacter * g_pRoh;

const char * confusion_filename = "data/tables/confusion_table.txt";

LHParam_ConfusionItem::LHParam_ConfusionItem(void)
	:	m_initialized(false)
{
	ZeroMemory(m_infolist , sizeof(m_infolist));
}

LHParam_ConfusionItem::~LHParam_ConfusionItem(void)
{
}

void LHParam_ConfusionItem::Init()
{
}

void LHParam_ConfusionItem::Release()
{
}

bool LHParam_ConfusionItem::Open()
{
	LHExcelReader er;

	if( false == er.Open( confusion_filename ) )
		return false;

	for( int row=0 ; row<er.GetRowSize() ; ++row )
	{
		int col = 0;
		m_infolist[row].grade = er.GetInt(row, col++); // 배열로 잡혀있기 때문에 굳이 읽어들일 필요가 없다.
		col++; // 설명부분 스킵
		m_infolist[row].enable = er.GetBool(row, col++);
		m_infolist[row].need_laim = er.GetInt(row, col++);
		m_infolist[row].need_gem = er.GetInt(row, col++);
		m_infolist[row].need_gem_count = er.GetInt(row, col++);
		m_infolist[row].value_range = er.GetInt(row, col++);
	}

	m_initialized = true;

	return true;
}

void LHParam_ConfusionItem::Close()
{
}

LHParam_ConfusionItem::sData * LHParam_ConfusionItem::GetParam(eItem_Grade_Type item_grade)
{
	if( item_grade < eItem_Grade__00 || item_grade > eItem_Grade__20 )
		return NULL;

	return &m_infolist[item_grade];
}

eConfusion_Result LHParam_ConfusionItem::Execute()
{
	assert(!g_pRoh);

// 	CItem * item = g_pRoh->m_ExgInven;
// 	if( !item )
// 		return eConfusion_Result__Empty;
//
// 	CItem * equip_item = NULL;
// 	int gem_count[eGem_Grade__20] = {0,};
// 	int equip_count[2] = {0,};				// 0:armor, 1:weapon
//
// 	while( item )
// 	{
// 		if( GET_TYPE(item) == ITYPE_ARMOR )
// 		{
// 			equip_item = item;
// 			equip_count[0]++;
// 		}
// 		if(GET_TYPE(item) == ITYPE_WEAPON )
// 		{
// 			equip_item = item;
// 			equip_count[1]++;
// 		}
// 		if( GET_TYPE(item) == ITYPE_ETC && GET_SHAPE(item) == IETC_GEM )
// 		{
// 			int g_grade = 0;
// 			gem_count[g_grade]++;
// 		}
//
// 		// 무기 & 방어구는 반듯이 한개만 있어야함
// 		if( equip_count[0]+equip_count[1] > 1 )
// 			return eConfusion_Result__Equipments;
//
// 		item = item->m_Next;
// 	}
//
// 	int current_equip_grade = equip_item->m_item_grade;
// 	// 컨퓨전 가능 유무
// 	if( false == m_infolist[current_equip_grade].enable )
// 		return eConfusion_Result__Disable;
//
// 	// 필요라임
// 	if( false == _CheckLaim(equip_item) )
// 		return eConfusion_Result__Laim;
//
// 	// 필요보석이 없소
// 	if( gem_count[gem_count] == 0 )
// 		return eConfusion_Result__Gem;
//
// 	// m_infolist[current_equip_grade].need_gem; // 아이템 등급과 동일하기 때문에 굳이 필요 없음
// 	int num_gem = 0;
// 	for( eGem_Grade_Type i=0 ; i<eGem_Grade__EndOfEnum ; i++ )
// 	{
// 		if( gem_count[i] )
// 			num_gem += gem_count[i];
// 	}
//
// 	// 보석이 업소
// 	if( num_gem == 0 )
// 		return eConfusion_Result__Gem;
// 	// 필요보석 개수
// 	if( m_infolist[current_equip_grade].need_gem_count != num_gem )
// 		return eConfusion_Result__Gem_Count;

	return eConfusion_Result__Done;
}

bool LHParam_ConfusionItem::_CheckEquip(CItem * item)
{
	if( !item )
		return false;

	if( GET_TYPE(item) == ITYPE_WEAPON || GET_TYPE(item) == ITYPE_ARMOR )
		return true;

	return false;
}

bool LHParam_ConfusionItem::_CheckJewelry(CItem * item)
{
	if( !item )
		return false;

	if( GET_TYPE(item) == ITYPE_ETC && GET_SHAPE(item) == IETC_GEM )
		return true;

	return false;
}

bool LHParam_ConfusionItem::_CheckLaim(CItem * item)
{
	if( g_pRoh->m_Money < m_infolist[item->m_item_grade].need_laim )
		return false;

	return true;
}

bool LHParam_ConfusionItem::CheckItem(CItem * item)
{
	if( !item )
		return false;

	if( _CheckEquip(item) || _CheckJewelry(item) )
		return true;

	return false;
}
