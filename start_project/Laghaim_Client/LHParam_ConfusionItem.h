#ifndef _LHParam_ConfusionItem_H_
#define _LHParam_ConfusionItem_H_
#pragma once

#include "ItemProto.h"

enum eConfusion_Result
{
	eConfusion_Result__Done,			// 성공
	eConfusion_Result__Empty,			// 교환창에 아이템 없음
	eConfusion_Result__Invalied_Item,	// 잘못된 아이템 포함
	eConfusion_Result__Disable,			// 잘못된 아이템 포함
	eConfusion_Result__Equipments,		// 장비개수가 잘못됨
	eConfusion_Result__Laim,			// 라임 부족
	eConfusion_Result__Gem,				// 필요보석아님
	eConfusion_Result__Gem_Count,		// 필요보석개수가 틀림

	eConfusion_Result__EndOfEnum
};


class LHParam_ConfusionItem
{
public:
	struct sData
	{
		int		grade;			// 등급 확인용
		bool	enable;			// 사용유무
		int 	need_laim;		// 필요라임
		int 	need_gem;		// 필요보석
		int 	need_gem_count;	// 필요보석 개수
		int		value_range;	// 변경폭( for server )
	};


private:
	bool m_initialized;
	sData m_infolist[eItem_Grade__EndOfEnum];


protected:
	void Init();
	void Release();

	bool Open();
	void Close();


protected:
	bool _CheckLaim(CItem * item);
	bool _CheckEquip(CItem * item);
	bool _CheckJewelry(CItem * item);


public:
	bool CheckItem(CItem * item);


public:
	sData * GetParam(eItem_Grade_Type item_grade);

	eConfusion_Result Execute();



public:
	LHParam_ConfusionItem(void);
	~LHParam_ConfusionItem(void);
};
#endif // _LHParam_ConfusionItem_H_