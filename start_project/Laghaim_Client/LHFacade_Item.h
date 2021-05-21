#ifndef _LHFacade_Item_H_
#define _LHFacade_Item_H_
#pragma once
#include "config.h"
#include "LHSingleton.h"

enum eItem_Up_Type
{
	eItem_Up__Enchant,
	eItem_Up__Masuk,
	eItem_Up__Conv,
	eItem_Up__Conf,

	eItem_Up__EndOfEnum
};


// class LHFacade_Item : public LHSingleton<LHFacade_Item>
// {
// public:
// 	bool CheckItem(eItem_Up_Type type, CItem * item);
//
// 	bool Check_Enchant(CItem * item);
// 	bool Check_Masuk(CItem * item);
// 	bool Check_Conversion(CItem * item);
// 	bool Check_Confusion(CItem * item);
//
//
// public:
//
//
//
//
//
//
//
// public:
// 	LHFacade_Item(void);
// 	virtual ~LHFacade_Item(void);
// };
#endif // _LHFacade_Item_H_
