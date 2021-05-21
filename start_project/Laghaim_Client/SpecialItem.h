#ifndef _SPECIAL_ITEM_H_
#define _SPECIAL_ITEM_H_

#include "Config.h"

#define SPECIAL_DAM_MIN		0
#define SPECIAL_DAM_MAX		1
#define SPECIAL_DAM_ALL		2
#define SPECIAL_DAM_PER		3
#define SPECIAL_HIT_1		     4
#define SPECIAL_HIT_2        5
#define SPECIAL_ATT_SPEED    6
#define SPECIAL_STAFF_NUM    7

#define SPECIAL_REDUCE_DEX_INT 7
#define SPECIAL_NO_DEX_INT   8
#define SPECIAL_REDUCE_STR   9
#define SPECIAL_NO_STR       10
#define SPECIAL_REDUCE_LEV   11
#define SPECIAL_NO_LEV       12
#define SPECIAL_SPDAM_ALL    13
#define SPECIAL_IGNORE_AVOID 14
#define SPECIAL_DAMAGE_UP2 15
#define SPECIAL_DEAD_HIT   16
#define SPECIAL_WEAPON_NUM	17

/*
#define SPECIAL_ADD_LIGHTNING	0
#define SPECIAL_ADD_FIRE		1
#define SPECIAL_ADD_STONE		2
#define SPECIAL_ADD_MAGIC		3
#define SPECIAL_MAGIC_SPEED		4
#define SPECIAL_REDUCE_INT		5
#define SPECIAL_NO_INT			6
#define SPECIAL_STAFF_NUM		7
*/

/*
#define SPECIAL_RESIST_MAGIC		0
#define SPECIAL_RESIST_LIGHTNING	1
#define SPECIAL_RESIST_FIRE			2
#define SPECIAL_RESIST_STONE		3
#define SPECIAL_DEFENSE_LONG		4
#define SPECIAL_DEFENSE_SHORT		5
#define SPECIAL_DEFENSE_MELEE		6
#define SPECIAL_ARMOR_NUM			7
*/

#define SPECIAL_DEFENSE_MELEE		0
#define SPECIAL_RESIST_MAGIC		1
#define SPECIAL_DEFENSE_ALL			2
#define SPECIAL_DEFENSE_PER			3
#define SPECIAL_SPDEFENSE_ALL		4
#define SPECIAL_FLY					5
#define SPECIAL_ARMOR_NUM			6

//----------------------------------------------------------------------------
//-- ¹ÝÁö Flag
//----------------------------------------------------------------------------
#define SPECIAL_BRACELET_NUM	14 //-- NUM

#define SPECIAL_MONEY			0
#define SPECIAL_EXP				1
#define SPECIAL_ITEM			2
#define SPECIAL_REALLOC		3

#define SPECIAL_BR_INC_HP     4  //-- »ý¸í·Â %d »ó½Â
#define SPECIAL_BR_INC_MP     5  //-- ¸¶¹ý·Â %d »ó½Â
#define SPECIAL_BR_INC_EP     6  //-- Àü±â·Â %d »ó½Â
#define SPECIAL_BR_INC_ST     7  //-- ½ºÅ×¹Ì³Ê %d »ó½Â
#define SPECIAL_BR_REF_DAM    8  //-- ¹Ý»çµ¥¹ÌÁö %d »ó½Â
#define SPECIAL_BR_ABS_DAM    9  //-- µ¥¹ÌÁöÈí¼ö %d »ó½Â
#define SPECIAL_BR_RESIST     10 //-- ÀüÃ¼ÀúÇ×·Â %d »ó½Â
#define SPECIAL_BR_STUN       11 //-- ¸ó½ºÅÍ È¥µ· È®·ü %d »ó½Â
#define SPECIAL_BR_DEF_MAGIC  12 //-- ¸¶¹ý ¹æ¾î·Â %d »ó½Â
#define SPECIAL_BR_RECOVER    13 //-- È¸º¹·ÂÁõ°¡  %d »ó½Â 



//----------------------------------------------------------------------------
//-- ¹ÝÁö Flag
//----------------------------------------------------------------------------
#define SPECIAL_RING_NUM		16 //-- NUM

#define SPECIAL_INC_CHA		    0
#define SPECIAL_INC_CON		    1
#define SPECIAL_INC_DEX		    2
#define SPECIAL_INC_STR		    3
#define SPECIAL_INC_INT		    4

#define SPECIAL_INC_COUNT		    5
#define SPECIAL_INC_CRITICAL  	6
#define SPECIAL_INC_ACCUMUL	  	7
#define SPECIAL_INC_BOLCANIC  	8
#define SPECIAL_INC_STONE	  	  9

#define SPECIAL_INC_COLD		    10
#define SPECIAL_INC_TOTAL		    11
#define SPECIAL_INC_SKILLTIME	  12
#define SPECIAL_INC_RECOVER		  13
#define SPECIAL_INC_ABS_DAMAGE  14   //-- Damage Èí¼ö 

#define SPECIAL_INC_AVOIDANCE   15   //-- È¸ÇÇ 


//#define SPECIAL_RING_NUM	5

#define SPECIAL_NECK_INC_VITAL		0
#define SPECIAL_NECK_INC_MANA		1
#define SPECIAL_NECK_INC_STAM		2
#define SPECIAL_NECK_INC_EPOWER		3
#define SPECIAL_NECK_INC_CHA		4 // Åë¼Ö »ó½Â
#define SPECIAL_NECK_INC_CON		5 // Ã¼Áú »ó½Â
#define SPECIAL_NECK_INC_DEX		6 // ±â¹Î »ó½Â
#define SPECIAL_NECK_INC_STR		7 // Èû »ó½Â
#define SPECIAL_NECK_INC_INT		8 // ÁöÇý »ó½Â
#define SPECIAL_NECK_INC_BLOCK		9 // ºí·° ¿É¼Ç
#define SPECIAL_NECKLACE_NUM	10 // ¸ñ°ÉÀÌ ¿É¼Ç Áõ°¡ (06-04-01 ¿ø¼®)

#define SPECIAL_ARMOR_INC_HP		0  // »ý¸í·Â »ó½Â
#define SPECIAL_ARMOR_INC_MP		1  // ¸¶¹ý·Â »ó½Â
#define SPECIAL_ARMOR_INC_EP		2  // Àü±â·Â »ó½Â
#define SPECIAL_ARMOR_INC_ST		3  // ½ºÅ×¹Ì³Ê »ó½Â
#define SPECIAL_ARMOR_INC_RECOVER	4  // È¸º¹·Â Áõ°¡ »ó½Â
#define SPECIAL_ARMOR_NUM2			5

#define SPECIAL_LEVEL_NUM	7

class CMultiText;
class CItem;

extern void GetSpecial(CMultiText &multiText, int item_type, int item_shape, int special, int special2);
extern void SetSpecialValue(CItem *pItem, int item_type, int item_shape, int special, int special2);
extern void ResetItemValue(CItem *pItem);
extern BOOL IsOptionFull(CItem *pItem, int option_num, int special_level = 0);

#endif // _SPECIAL_ITEM_H_