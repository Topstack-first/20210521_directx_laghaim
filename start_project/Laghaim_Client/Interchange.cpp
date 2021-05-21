#include "stdafx.h"


#include "main.h"
#include "ddutil.h"
#include "ItemRender.h"
#include "ItemProto.h"
#include "Nk2DFrame.h"
#include "tcpipcon.h"
#include "AlphaPopup.h"
#include "ControlBottom.h"
#include "ControlInven.h"
#include "Mouse.h"
#include "BasicButton.h"
#include "Interchange.h"
#include "textoutbox.h"
#include "SpecialItem.h"
#include "g_stringmanager.h"

extern POINT		g_CursorPoint;

#define EXCH_TEXT_SIZE			12
#define EXCH_TEXT_COLOR			RGB(255, 0, 0)
#define EXCH_BAKC_COLOR         RGB(47, 0, 17)

//===============================
// nate 2004 - 5 : CastleWar
#define INTERCHANGE_NUM 50 // 43 플래 이전꺼
//===============================
static int InterChangeVnum [][8] =
{
	{2743,2744,2745,2746,2747,2748,2749,3231},			//weapon 400
	{10454,10455,10456,10457,10458,10459,10460,10461},  //weapon 450
	{10462,10463,10464,10465,10466,10467,10468,10469},  //weapon 500
	{10470,10471,10472,10473,10474,10475,10476,10517},  //weapon 550
	{10477,10478,10479,10480,10481,10482,10483,10484},  //weapon 600
	{10522,10523,10524,10525,10526,10527,10528,10529},  //weapon 650
	{10530,10531,10532,10533,10534,10535,10536,10537},  //weapon 700
	{10538,10539,10540,10541,10542,10543,10544,10545},  //weapon 750
	{10546,10547,10548,10549,10550,10551,10552,10553},  //weapon 800
	{10554,10555,10556,10557,10558,10559,10560,10561},  //weapon 850
	{10562,10563,10564,10565,10566,10567,10568,10569},  //weapon 870
	{10570,10571,10572,10573,10574,10575,10576,10577},  //weapon 890
	{10723,10724,10725,10726,10727,10728,10729,10730},  //weapon 910
	{3624,3628,3632,3636,3640,3644,3644,3644},		 //helm 450
	{3625,3629,3633,3637,3641,3645,3645,3645},		 //armor 450
	{3626,3630,3634,3638,3642,3646,3646,3646},		 //pants 450
	{3627,3631,3635,3639,3643,3647,3647,3647},		 //boots 450
	{10118,10122,10126,10130,10134,10138,10138,10138}, //helm 600
	{10119,10123,10127,10131,10135,10139,10139,10139}, //armor 600
	{10120,10124,10128,10132,10136,10140,10140,10140}, //pants 600
	{10121,10125,10129,10133,10137,10141,10141,10141}, //boots 600
	{10142,10146,10150,10154,10158,10162,10162,10162}, //helm 750
	{10143,10147,10151,10155,10159,10163,10163,10163}, //armor 750
	{10144,10148,10152,10156,10160,10164,10164,10164}, //pants 750
	{10145,10149,10153,10157,10161,10165,10165,10165}, //boots 750
	{10166,10170,10174,10178,10182,10186,10186,10186}, //helm 870
	{10167,10171,10175,10179,10183,10187,10187,10187}, //armor 870
	{10168,10172,10176,10180,10184,10188,10188,10188}, //pants 870
	{10169,10173,10177,10181,10185,10189,10189,10189}, //boots 870
	{10865,10869,10873,10877,10881,10885,10885,10885}, //helm 890
	{10866,10870,10874,10878,10882,10886,10886,10886}, //armor 890
	{10867,10871,10875,10879,10883,10887,10887,10887}, //pants 890
	{10868,10872,10876,10880,10884,10888,10888,10888}, //boots 890
	{10745,10749,10753,10757,10761,10765,10765,10765}, //helm 910
	{10746,10750,10754,10758,10762,10766,10766,10766}, //armor 910
	{10747,10751,10755,10759,10763,10767,10767,10767}, //pants 910
	{10748,10752,10756,10760,10764,10768,10768,10768}, //boots 910
	{3064,3065,3066,3067,3068,3232,3232,3232},		  //Sub Weapon lv.1
	{10206,10371,10372,10373,10374,10375,10375,10375},	  //Sub Weapon Lv2
	{10413,10414,10415,10416,10417,10453,10453,10453},	  //Sub Weapon Lv3
	{11061,11062,11063,11064,11065,11066,11067,11067}, //Sub Weapon Lv4
	{11082,11083,11084,11085,11086,11087,11087,11087}, //Sub Weapon Lv5
};

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CInterchange::CInterchange()
	: m_hWnd(NULL)
	, m_ScreenWidth(0)
	, m_ScreenHeight(0)
	, m_opp_slotstart_x(0)
	, m_opp_slotstart_y(0)
	, m_my_slotstart_x(0)
	, m_my_slotstart_y(0)
	, m_cur_click_slot_x(0)
	, m_cur_click_slot_y(0)
{
	memset(m_MsgBuf, 0, sizeof(m_MsgBuf));

	m_pSurfaceBack = NULL;
	m_pEnableSlot = NULL;
	m_pDisableSlot = NULL;
	m_pExistSlot = NULL;
	m_pNkCha = NULL;
	m_pOverItem = NULL;
	Init();

	m_bExch = FALSE;
	m_bWaitingExch = FALSE;
	m_ExgOppInven = NULL;
	int i, j;

	for ( i=0; i < EXG_SLOT_X_NUM; i++)
	{
		for ( j = 0; j < EXG_SLOT_Y_NUM; j++)
			m_ExgOppSlot[i][j] = -1;
	}

	m_dwMouseClickTickTime = timeGetTime();
}

CInterchange::~CInterchange()
{
	DeleteRes();
	RemoveAllOppInven();

	m_pNkCha = NULL;
}

void CInterchange::RemoveAllOppInven()
{
	CItem *pItem = NULL;
	CItem *nextItem = NULL;

	pItem = m_ExgOppInven;

	while (pItem)
	{
		nextItem = pItem->m_Next;
		delete pItem;
		pItem = nextItem;
	}
	m_ExgOppInven = NULL;

	int i, j;

	for ( i=0; i < EXG_SLOT_X_NUM; i++)
	{
		for ( j = 0; j < EXG_SLOT_Y_NUM; j++)
			m_ExgOppSlot[i][j] = -1;
	}
}

void CInterchange::Init()
{
	m_CancelBtn.SetFileName("common/btn_close_01");
}

void CInterchange::SetVisibleMode(BOOL bExch)
{
	if (m_bExch)
		// 상대편 교환창에 바꿀 아이템을 놓는다.
		ShowItemsToChange();
	else
		// 아무것도 하지 않는다.
		RemoveAllOppInven();
}

//--------------------------------------------------------------------------
// Name : ShowItemsToChange()
// Desc : 바꿀 수 있는 아이템을 보여주기 위한 루틴.
//--------------------------------------------------------------------------
void CInterchange::ShowItemsToChange()
{
	if( !m_pNkCha )
		return;

	int i, x_pos, x_idx, y_idx;
	BOOL bWillChange = FALSE;

	// get item info to change
	// make sure that there is only one item in exchange inventory.
	RemoveAllOppInven();
	CItem *pChangeItem = m_pNkCha->m_ExgInven;

	// search old index
	CItem *pItem = NULL;
	for (i=0; i < INTERCHANGE_NUM; i++)
	{
		///////////////// search old index ///////////////////////////
		if ( pChangeItem->m_Vnum == InterChangeVnum[i][0])
		{
			bWillChange = TRUE;
			// make dummy items
			pItem = new CItem;
			if( !pItem )
				continue;

			pItem->m_SlotX = 0;
			pItem->m_SlotY = 0;
			pItem->m_Index = pChangeItem->m_Index+1;		// 임시 인덱스
			pItem->m_PlusNum = pChangeItem->m_PlusNum;
			pItem->m_Vnum = InterChangeVnum[i][1];
			pItem->m_ProtoNum = g_ItemList.FindItem(InterChangeVnum[i][1]);
			// 아이템 값을 입력해보자!
			pItem->m_Num0 = GET_ITEM_NUM0(pItem);
			pItem->m_Num1 = GET_ITEM_NUM1(pItem);
			pItem->m_Num2 = GET_ITEM_NUM2(pItem);
			pItem->m_Num3 = GET_ITEM_NUM3(pItem);
			pItem->m_Num4 = GET_ITEM_NUM4(pItem);
			pItem->m_Num5 = GET_ITEM_NUM5(pItem);
			pItem->m_Num6 = GET_ITEM_NUM6(pItem);
			pItem->m_Num7 = GET_ITEM_NUM7(pItem);
			pItem->m_Num8 = GET_ITEM_NUM8(pItem);
			pItem->m_Num9 = GET_ITEM_NUM9(pItem);
			pItem->m_MinLevel = GET_ITEM_LEVEL(pItem);
			pItem->m_MinLevel_Org = pItem->m_MinLevel;
			pItem->m_Int = GET_ITEM_INT(pItem);
			pItem->m_Dex = GET_ITEM_DEX(pItem);
			pItem->m_Weight = GET_WEIGHT(pItem);

			pItem->m_Next = m_ExgOppInven;
			m_ExgOppInven = pItem;

			x_pos = 0;

			for (x_idx = 0; x_idx < GET_ITEM_WIDTH(pItem); x_idx++)
			{
				for (y_idx = 0; y_idx < GET_ITEM_HEIGHT(pItem); y_idx++)
				{
					m_ExgOppSlot[x_pos+x_idx][y_idx] = InterChangeVnum[i][1];
				}
			}
			x_pos += GET_ITEM_WIDTH(pItem);

			if (InterChangeVnum[i][1] != InterChangeVnum[i][2])
			{
				// make dummy items
				pItem = new CItem;
				if( !pItem )
					continue;

				pItem->m_SlotX = x_pos;
				pItem->m_SlotY = 0;
				pItem->m_Index = pChangeItem->m_Index+2;
				pItem->m_PlusNum = pChangeItem->m_PlusNum;
				pItem->m_Vnum = InterChangeVnum[i][2];
				pItem->m_ProtoNum = g_ItemList.FindItem(InterChangeVnum[i][2]);
				pItem->m_Num0 = GET_ITEM_NUM0(pItem);
				pItem->m_Num1 = GET_ITEM_NUM1(pItem);
				pItem->m_Num2 = GET_ITEM_NUM2(pItem);
				pItem->m_Num3 = GET_ITEM_NUM3(pItem);
				pItem->m_Num4 = GET_ITEM_NUM4(pItem);
				pItem->m_Num5 = GET_ITEM_NUM5(pItem);
				pItem->m_Num6 = GET_ITEM_NUM6(pItem);
				pItem->m_Num7 = GET_ITEM_NUM7(pItem);
				pItem->m_Num8 = GET_ITEM_NUM8(pItem);
				pItem->m_Num9 = GET_ITEM_NUM9(pItem);
				pItem->m_MinLevel = GET_ITEM_LEVEL(pItem);
				pItem->m_MinLevel_Org = pItem->m_MinLevel;
				pItem->m_Int = GET_ITEM_INT(pItem);
				pItem->m_Dex = GET_ITEM_DEX(pItem);
				pItem->m_Weight = GET_WEIGHT(pItem);

				pItem->m_Next = m_ExgOppInven;
				m_ExgOppInven = pItem;

				for (x_idx = 0; x_idx < GET_ITEM_WIDTH(pItem); x_idx++)
				{
					for (y_idx = 0; y_idx < GET_ITEM_HEIGHT(pItem); y_idx++)
					{
						m_ExgOppSlot[x_pos+x_idx][y_idx] = InterChangeVnum[i][2];
					}
				}
				x_pos += GET_ITEM_WIDTH(pItem);
			}

			if (InterChangeVnum[i][2] != InterChangeVnum[i][3])
			{
				// make dummy items
				pItem = new CItem;
				if( !pItem )
					continue;

				pItem->m_SlotX = x_pos;
				pItem->m_SlotY = 0;
				pItem->m_Index = pChangeItem->m_Index+3;
				pItem->m_PlusNum = pChangeItem->m_PlusNum;
				pItem->m_Vnum = InterChangeVnum[i][3];
				pItem->m_ProtoNum = g_ItemList.FindItem(InterChangeVnum[i][3]);
				pItem->m_Num0 = GET_ITEM_NUM0(pItem);
				pItem->m_Num1 = GET_ITEM_NUM1(pItem);
				pItem->m_Num2 = GET_ITEM_NUM2(pItem);
				pItem->m_Num3 = GET_ITEM_NUM3(pItem);
				pItem->m_Num4 = GET_ITEM_NUM4(pItem);
				pItem->m_Num5 = GET_ITEM_NUM5(pItem);
				pItem->m_Num6 = GET_ITEM_NUM6(pItem);
				pItem->m_Num7 = GET_ITEM_NUM7(pItem);
				pItem->m_Num8 = GET_ITEM_NUM8(pItem);
				pItem->m_Num9 = GET_ITEM_NUM9(pItem);
				pItem->m_MinLevel = GET_ITEM_LEVEL(pItem);
				pItem->m_MinLevel_Org = pItem->m_MinLevel;
				pItem->m_Int = GET_ITEM_INT(pItem);
				pItem->m_Dex = GET_ITEM_DEX(pItem);
				pItem->m_Weight = GET_WEIGHT(pItem);

				pItem->m_Next = m_ExgOppInven;
				m_ExgOppInven = pItem;

				for (x_idx = 0; x_idx < GET_ITEM_WIDTH(pItem); x_idx++)
				{
					for (y_idx = 0; y_idx < GET_ITEM_HEIGHT(pItem); y_idx++)
					{
						m_ExgOppSlot[x_pos+x_idx][y_idx] = InterChangeVnum[i][3];
					}
				}
				x_pos += GET_ITEM_WIDTH(pItem);
			}

			if (InterChangeVnum[i][3] != InterChangeVnum[i][4])
			{
				// make dummy items
				pItem = new CItem;
				if( !pItem )
					continue;

				pItem->m_SlotX = x_pos;
				pItem->m_SlotY = 0;
				pItem->m_Index = pChangeItem->m_Index+4;
				pItem->m_PlusNum = pChangeItem->m_PlusNum;
				pItem->m_Vnum = InterChangeVnum[i][4];
				pItem->m_ProtoNum = g_ItemList.FindItem(InterChangeVnum[i][4]);
				pItem->m_Num0 = GET_ITEM_NUM0(pItem);
				pItem->m_Num1 = GET_ITEM_NUM1(pItem);
				pItem->m_Num2 = GET_ITEM_NUM2(pItem);
				pItem->m_Num3 = GET_ITEM_NUM3(pItem);
				pItem->m_Num4 = GET_ITEM_NUM4(pItem);
				pItem->m_Num5 = GET_ITEM_NUM5(pItem);
				pItem->m_Num6 = GET_ITEM_NUM6(pItem);
				pItem->m_Num7 = GET_ITEM_NUM7(pItem);
				pItem->m_Num8 = GET_ITEM_NUM8(pItem);
				pItem->m_Num9 = GET_ITEM_NUM9(pItem);
				pItem->m_MinLevel = GET_ITEM_LEVEL(pItem);
				pItem->m_MinLevel_Org = pItem->m_MinLevel;
				pItem->m_Int = GET_ITEM_INT(pItem);
				pItem->m_Dex = GET_ITEM_DEX(pItem);
				pItem->m_Weight = GET_WEIGHT(pItem);

				pItem->m_Next = m_ExgOppInven;
				m_ExgOppInven = pItem;

				for (x_idx = 0; x_idx < GET_ITEM_WIDTH(pItem); x_idx++)
				{
					for (y_idx = 0; y_idx < GET_ITEM_HEIGHT(pItem); y_idx++)
					{
						m_ExgOppSlot[x_pos+x_idx][y_idx] = InterChangeVnum[i][4];
					}
				}
				x_pos += GET_ITEM_WIDTH(pItem);
			}

			if (InterChangeVnum[i][4] != InterChangeVnum[i][5])
			{
				// make dummy items
				pItem = new CItem;
				if( !pItem )
					continue;

				pItem->m_SlotX = x_pos;
				pItem->m_SlotY = 0;
				pItem->m_Index = pChangeItem->m_Index+5;
				pItem->m_PlusNum = pChangeItem->m_PlusNum;
				pItem->m_Vnum = InterChangeVnum[i][5];
				pItem->m_ProtoNum = g_ItemList.FindItem(InterChangeVnum[i][5]);
				pItem->m_Num0 = GET_ITEM_NUM0(pItem);
				pItem->m_Num1 = GET_ITEM_NUM1(pItem);
				pItem->m_Num2 = GET_ITEM_NUM2(pItem);
				pItem->m_Num3 = GET_ITEM_NUM3(pItem);
				pItem->m_Num4 = GET_ITEM_NUM4(pItem);
				pItem->m_Num5 = GET_ITEM_NUM5(pItem);
				pItem->m_Num6 = GET_ITEM_NUM6(pItem);
				pItem->m_Num7 = GET_ITEM_NUM7(pItem);
				pItem->m_Num8 = GET_ITEM_NUM8(pItem);
				pItem->m_Num9 = GET_ITEM_NUM9(pItem);
				pItem->m_MinLevel = GET_ITEM_LEVEL(pItem);
				pItem->m_MinLevel_Org = pItem->m_MinLevel;
				pItem->m_Int = GET_ITEM_INT(pItem);
				pItem->m_Dex = GET_ITEM_DEX(pItem);
				pItem->m_Weight = GET_WEIGHT(pItem);

				pItem->m_Next = m_ExgOppInven;
				m_ExgOppInven = pItem;

				for (x_idx = 0; x_idx < GET_ITEM_WIDTH(pItem); x_idx++)
				{
					for (y_idx = 0; y_idx < GET_ITEM_HEIGHT(pItem); y_idx++)
					{
						m_ExgOppSlot[x_pos+x_idx][y_idx] = InterChangeVnum[i][5];
					}
				}
				x_pos += GET_ITEM_WIDTH(pItem);
			}

			if (InterChangeVnum[i][5] != InterChangeVnum[i][6])
			{
				// make dummy items
				pItem = new CItem;
				if( !pItem )
					continue;

				pItem->m_SlotX = x_pos;
				pItem->m_SlotY = 0;
				pItem->m_Index = pChangeItem->m_Index+6;
				pItem->m_PlusNum = pChangeItem->m_PlusNum;
				pItem->m_Vnum = InterChangeVnum[i][6];
				pItem->m_ProtoNum = g_ItemList.FindItem(InterChangeVnum[i][6]);
				pItem->m_Num0 = GET_ITEM_NUM0(pItem);
				pItem->m_Num1 = GET_ITEM_NUM1(pItem);
				pItem->m_Num2 = GET_ITEM_NUM2(pItem);
				pItem->m_Num3 = GET_ITEM_NUM3(pItem);
				pItem->m_Num4 = GET_ITEM_NUM4(pItem);
				pItem->m_Num5 = GET_ITEM_NUM5(pItem);
				pItem->m_Num6 = GET_ITEM_NUM6(pItem);
				pItem->m_Num7 = GET_ITEM_NUM7(pItem);
				pItem->m_Num8 = GET_ITEM_NUM8(pItem);
				pItem->m_Num9 = GET_ITEM_NUM9(pItem);
				pItem->m_MinLevel = GET_ITEM_LEVEL(pItem);
				pItem->m_MinLevel_Org = pItem->m_MinLevel;
				pItem->m_Int = GET_ITEM_INT(pItem);
				pItem->m_Dex = GET_ITEM_DEX(pItem);
				pItem->m_Weight = GET_WEIGHT(pItem);

				pItem->m_Next = m_ExgOppInven;
				m_ExgOppInven = pItem;

				for (x_idx = 0; x_idx < GET_ITEM_WIDTH(pItem); x_idx++)
				{
					for (y_idx = 0; y_idx < GET_ITEM_HEIGHT(pItem); y_idx++)
					{
						m_ExgOppSlot[x_pos+x_idx][y_idx] = InterChangeVnum[i][6];
					}
				}
				x_pos += GET_ITEM_WIDTH(pItem);
			}

			if (InterChangeVnum[i][0] != InterChangeVnum[i][7] && InterChangeVnum[i][1] != InterChangeVnum[i][7] && InterChangeVnum[i][2] != InterChangeVnum[i][7] && InterChangeVnum[i][3] != InterChangeVnum[i][7]
			&& InterChangeVnum[i][4] != InterChangeVnum[i][7]&& InterChangeVnum[i][5] != InterChangeVnum[i][7]&& InterChangeVnum[i][6] != InterChangeVnum[i][7])
			{
				// make dummy items
				pItem = new CItem;
				if( !pItem )
					continue;

				pItem->m_SlotX = 2;
				pItem->m_SlotY = 2;
				pItem->m_Index = pChangeItem->m_Index+7;
				pItem->m_PlusNum = pChangeItem->m_PlusNum;
				pItem->m_Vnum = InterChangeVnum[i][7];
				pItem->m_ProtoNum = g_ItemList.FindItem(InterChangeVnum[i][7]);
				pItem->m_Num0 = GET_ITEM_NUM0(pItem);
				pItem->m_Num1 = GET_ITEM_NUM1(pItem);
				pItem->m_Num2 = GET_ITEM_NUM2(pItem);
				pItem->m_Num3 = GET_ITEM_NUM3(pItem);
				pItem->m_Num4 = GET_ITEM_NUM4(pItem);
				pItem->m_Num5 = GET_ITEM_NUM5(pItem);
				pItem->m_Num6 = GET_ITEM_NUM6(pItem);
				pItem->m_Num7 = GET_ITEM_NUM7(pItem);
				pItem->m_Num8 = GET_ITEM_NUM8(pItem);
				pItem->m_Num9 = GET_ITEM_NUM9(pItem);
				pItem->m_MinLevel = GET_ITEM_LEVEL(pItem);
				pItem->m_MinLevel_Org = pItem->m_MinLevel;
				pItem->m_Int = GET_ITEM_INT(pItem);
				pItem->m_Dex = GET_ITEM_DEX(pItem);
				pItem->m_Weight = GET_WEIGHT(pItem);

				pItem->m_Next = m_ExgOppInven;
				m_ExgOppInven = pItem;

				for (x_idx = 0; x_idx < GET_ITEM_WIDTH(pItem); x_idx++)
				{
					for (y_idx = 0; y_idx < GET_ITEM_HEIGHT(pItem); y_idx++)
					{
						m_ExgOppSlot[2+x_idx][2+y_idx] = InterChangeVnum[i][7];
					}
				}
				x_pos += GET_ITEM_WIDTH(pItem);
			}

///////////////////////0304
			if(pChangeItem->m_Vnum == 490) //태극보관함이라면 교환 아이템이 4개가 추가 되어야 한다.
			{

				pItem = new CItem;
				if( !pItem )
					continue;

				pItem->m_SlotX = 2;
				pItem->m_SlotY = 2;
				pItem->m_Index = pChangeItem->m_Index+4;
				pItem->m_PlusNum = pChangeItem->m_PlusNum;
				pItem->m_Vnum = 920; //이건 태극 두건.
				pItem->m_ProtoNum = g_ItemList.FindItem(920);
				pItem->m_Num0 = GET_ITEM_NUM0(pItem);
				pItem->m_Num1 = GET_ITEM_NUM1(pItem);
				pItem->m_Num2 = GET_ITEM_NUM2(pItem);
				pItem->m_Num3 = GET_ITEM_NUM3(pItem);
				pItem->m_Num4 = GET_ITEM_NUM4(pItem);
				pItem->m_Num5 = GET_ITEM_NUM5(pItem);
				pItem->m_Num6 = GET_ITEM_NUM6(pItem);
				pItem->m_Num7 = GET_ITEM_NUM7(pItem);
				pItem->m_Num8 = GET_ITEM_NUM8(pItem);
				pItem->m_Num9 = GET_ITEM_NUM9(pItem);
				pItem->m_MinLevel = GET_ITEM_LEVEL(pItem);
				pItem->m_MinLevel_Org = pItem->m_MinLevel;
				pItem->m_Int = GET_ITEM_INT(pItem);
				pItem->m_Dex = GET_ITEM_DEX(pItem);
				pItem->m_Weight = GET_WEIGHT(pItem);

				pItem->m_Next = m_ExgOppInven;
				m_ExgOppInven = pItem;

				for (x_idx = 0; x_idx < GET_ITEM_WIDTH(pItem); x_idx++)
				{
					for (y_idx = 0; y_idx < GET_ITEM_HEIGHT(pItem); y_idx++)
					{
						m_ExgOppSlot[2+x_idx][2+y_idx] = 920;
					}
				}
				x_pos += GET_ITEM_WIDTH(pItem);
			}
/////////////////////
			break;
		}
		/////////////////////////// search new index /////////////////////
		else if ( pChangeItem->m_Vnum == InterChangeVnum[i][1])
		{
			bWillChange = ShowItemsToChange2(i, 1, 0, 2, 3, 4, 5, 6, 7);
			break;
		}
		else if ( pChangeItem->m_Vnum == InterChangeVnum[i][2])
		{
			bWillChange = ShowItemsToChange2(i, 2, 0, 1, 3, 4, 5, 6, 7);
			break;
		}
		else if ( pChangeItem->m_Vnum == InterChangeVnum[i][3])
		{
			bWillChange = ShowItemsToChange2(i, 3, 0, 1, 2, 4, 5, 6, 7);
			break;
		}


		else if ( pChangeItem->m_Vnum == InterChangeVnum[i][4])
		{
			bWillChange = ShowItemsToChange2(i, 4, 0, 1, 2, 3, 5, 6, 7);
			break;
		}
		else if ( pChangeItem->m_Vnum == InterChangeVnum[i][5])
		{
			bWillChange = ShowItemsToChange2(i, 5, 0, 1, 2, 3, 4, 6, 7);
			break;
		}
		else if ( pChangeItem->m_Vnum == InterChangeVnum[i][6])
		{
			bWillChange = ShowItemsToChange2(i, 6, 0, 1, 2, 3, 4, 5, 7);
			break;
		}
		else if ( pChangeItem->m_Vnum == InterChangeVnum[i][7])
		{
			bWillChange = ShowItemsToChange2(i, 7, 0, 1, 2, 3, 4, 5, 6);
			break;
		}
	}

	if (bWillChange == FALSE)
	{
		//-- IDS_INTERCHA_UNABLE_CHANGE : 바꿀수 있는 아이템이 없습니다.
		g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_INTERCHA_UNABLE_CHANGE), TYPE_NOR_OK, 1 );
		m_bExch = FALSE;
	}
}

// 동급의 갑옷으로 바꾸기 위한 루틴.
BOOL CInterchange::ShowItemsToChange2(int idx, int sel, int candi1, int candi2, int candi3, int candi4, int candi5, int candi6, int candi7)
{
	int x_pos, x_idx, y_idx;
	BOOL bWillChange = FALSE;

	// get item info to change
	// make sure that there is only one item in exchange inventory.
	CItem *pChangeItem = m_pNkCha->m_ExgInven;

	// search old index
	CItem *pItem = NULL;
	RemoveAllOppInven();
	x_pos = 0;

	if ( InterChangeVnum[idx][sel] != InterChangeVnum[idx][candi1])
	{
		// make dummy items
		pItem = new CItem;
		if( pItem )
		{
			pItem->m_SlotX = 0;
			pItem->m_SlotY = 0;
			pItem->m_Index = pChangeItem->m_Index+1;	// 임시인덱스
			pItem->m_PlusNum = pChangeItem->m_PlusNum;
			pItem->m_Vnum = InterChangeVnum[idx][candi1];
			pItem->m_ProtoNum = g_ItemList.FindItem(InterChangeVnum[idx][candi1]);
			pItem->m_Num0 = GET_ITEM_NUM0(pItem);
			pItem->m_Num1 = GET_ITEM_NUM1(pItem);
			pItem->m_Num2 = GET_ITEM_NUM2(pItem);
			pItem->m_Num3 = GET_ITEM_NUM3(pItem);
			pItem->m_Num4 = GET_ITEM_NUM4(pItem);
			pItem->m_Num5 = GET_ITEM_NUM5(pItem);
			pItem->m_Num6 = GET_ITEM_NUM6(pItem);
			pItem->m_Num7 = GET_ITEM_NUM7(pItem);
			pItem->m_Num8 = GET_ITEM_NUM8(pItem);
			pItem->m_Num9 = GET_ITEM_NUM9(pItem);
			pItem->m_MinLevel = GET_ITEM_LEVEL(pItem);
			pItem->m_MinLevel_Org = pItem->m_MinLevel;
			pItem->m_Int = GET_ITEM_INT(pItem);
			pItem->m_Dex = GET_ITEM_DEX(pItem);
			pItem->m_Weight = GET_WEIGHT(pItem);
			pItem->m_Next = m_ExgOppInven;
			m_ExgOppInven = pItem;

			bWillChange = TRUE;

			for (x_idx = 0; x_idx < GET_ITEM_WIDTH(pItem); x_idx++)
			{
				for (y_idx = 0; y_idx < GET_ITEM_HEIGHT(pItem); y_idx++)
				{
					m_ExgOppSlot[x_pos+x_idx][y_idx] = InterChangeVnum[idx][candi1];
				}
			}
			x_pos += GET_ITEM_WIDTH(pItem);
		}
	}

	if (InterChangeVnum[idx][sel] != InterChangeVnum[idx][candi2] &&
			InterChangeVnum[idx][candi1] != InterChangeVnum[idx][candi2])
	{
		// make dummy items
		pItem = new CItem;
		if( pItem )
		{
			pItem->m_SlotX = x_pos;
			pItem->m_SlotY = 0;
			pItem->m_Index = pChangeItem->m_Index+2;
			pItem->m_PlusNum = pChangeItem->m_PlusNum;
			pItem->m_Vnum = InterChangeVnum[idx][candi2];
			pItem->m_ProtoNum = g_ItemList.FindItem(InterChangeVnum[idx][candi2]);

			pItem->m_Num0 = GET_ITEM_NUM0(pItem);
			pItem->m_Num1 = GET_ITEM_NUM1(pItem);
			pItem->m_Num2 = GET_ITEM_NUM2(pItem);
			pItem->m_Num3 = GET_ITEM_NUM3(pItem);
			pItem->m_Num4 = GET_ITEM_NUM4(pItem);
			pItem->m_Num5 = GET_ITEM_NUM5(pItem);
			pItem->m_Num6 = GET_ITEM_NUM6(pItem);
			pItem->m_Num7 = GET_ITEM_NUM7(pItem);
			pItem->m_Num8 = GET_ITEM_NUM8(pItem);
			pItem->m_Num9 = GET_ITEM_NUM9(pItem);
			pItem->m_MinLevel = GET_ITEM_LEVEL(pItem);
			pItem->m_MinLevel_Org = pItem->m_MinLevel;
			pItem->m_Int = GET_ITEM_INT(pItem);
			pItem->m_Dex = GET_ITEM_DEX(pItem);
			pItem->m_Weight = GET_WEIGHT(pItem);

			pItem->m_Next = m_ExgOppInven;
			m_ExgOppInven = pItem;

			bWillChange = TRUE;

			for (x_idx = 0; x_idx < GET_ITEM_WIDTH(pItem); x_idx++)
			{
				for (y_idx = 0; y_idx < GET_ITEM_HEIGHT(pItem); y_idx++)
				{
					m_ExgOppSlot[x_pos+x_idx][y_idx] = InterChangeVnum[idx][candi2];
				}
			}
			x_pos += GET_ITEM_WIDTH(pItem);
		}
	}

	if (InterChangeVnum[idx][sel] != InterChangeVnum[idx][candi3] &&
			InterChangeVnum[idx][candi1] != InterChangeVnum[idx][candi3] &&
			InterChangeVnum[idx][candi2] != InterChangeVnum[idx][candi3])
	{
		// make dummy items
		pItem = new CItem;
		if( pItem )
		{
			pItem->m_SlotX = x_pos;
			pItem->m_SlotY = 0;
			pItem->m_Index = pChangeItem->m_Index+3;
			pItem->m_PlusNum = pChangeItem->m_PlusNum;
			pItem->m_Vnum = InterChangeVnum[idx][candi3];
			pItem->m_ProtoNum = g_ItemList.FindItem(InterChangeVnum[idx][candi3]);

			pItem->m_Num0 = GET_ITEM_NUM0(pItem);
			pItem->m_Num1 = GET_ITEM_NUM1(pItem);
			pItem->m_Num2 = GET_ITEM_NUM2(pItem);
			pItem->m_Num3 = GET_ITEM_NUM3(pItem);
			pItem->m_Num4 = GET_ITEM_NUM4(pItem);
			pItem->m_Num5 = GET_ITEM_NUM5(pItem);
			pItem->m_Num6 = GET_ITEM_NUM6(pItem);
			pItem->m_Num7 = GET_ITEM_NUM7(pItem);
			pItem->m_Num8 = GET_ITEM_NUM8(pItem);
			pItem->m_Num9 = GET_ITEM_NUM9(pItem);
			pItem->m_MinLevel = GET_ITEM_LEVEL(pItem);
			pItem->m_MinLevel_Org = pItem->m_MinLevel;
			pItem->m_Int = GET_ITEM_INT(pItem);
			pItem->m_Dex = GET_ITEM_DEX(pItem);
			pItem->m_Weight = GET_WEIGHT(pItem);

			pItem->m_Next = m_ExgOppInven;
			m_ExgOppInven = pItem;

			bWillChange = TRUE;

			for (x_idx = 0; x_idx < GET_ITEM_WIDTH(pItem); x_idx++)
			{
				for (y_idx = 0; y_idx < GET_ITEM_HEIGHT(pItem); y_idx++)
				{
					m_ExgOppSlot[x_pos+x_idx][y_idx] = InterChangeVnum[idx][candi3];
				}
			}
			x_pos += GET_ITEM_WIDTH(pItem);
		}
	}

	if (InterChangeVnum[idx][sel] != InterChangeVnum[idx][candi4] &&
			InterChangeVnum[idx][candi1] != InterChangeVnum[idx][candi4] &&
			InterChangeVnum[idx][candi2] != InterChangeVnum[idx][candi4] &&
			InterChangeVnum[idx][candi3] != InterChangeVnum[idx][candi4])
	{
		// make dummy items
		pItem = new CItem;
		if( pItem )
		{
			pItem->m_SlotX = x_pos;
			pItem->m_SlotY = 0;
			pItem->m_Index = pChangeItem->m_Index+4;
			pItem->m_PlusNum = pChangeItem->m_PlusNum;
			pItem->m_Vnum = InterChangeVnum[idx][candi4];
			pItem->m_ProtoNum = g_ItemList.FindItem(InterChangeVnum[idx][candi4]);

			pItem->m_Num0 = GET_ITEM_NUM0(pItem);
			pItem->m_Num1 = GET_ITEM_NUM1(pItem);
			pItem->m_Num2 = GET_ITEM_NUM2(pItem);
			pItem->m_Num3 = GET_ITEM_NUM3(pItem);
			pItem->m_Num4 = GET_ITEM_NUM4(pItem);
			pItem->m_Num5 = GET_ITEM_NUM5(pItem);
			pItem->m_Num6 = GET_ITEM_NUM6(pItem);
			pItem->m_Num7 = GET_ITEM_NUM7(pItem);
			pItem->m_Num8 = GET_ITEM_NUM8(pItem);
			pItem->m_Num9 = GET_ITEM_NUM9(pItem);
			pItem->m_MinLevel = GET_ITEM_LEVEL(pItem);
			pItem->m_MinLevel_Org = pItem->m_MinLevel;
			pItem->m_Int = GET_ITEM_INT(pItem);
			pItem->m_Dex = GET_ITEM_DEX(pItem);
			pItem->m_Weight = GET_WEIGHT(pItem);

			pItem->m_Next = m_ExgOppInven;
			m_ExgOppInven = pItem;

			bWillChange = TRUE;

			for (x_idx = 0; x_idx < GET_ITEM_WIDTH(pItem); x_idx++)
			{
				for (y_idx = 0; y_idx < GET_ITEM_HEIGHT(pItem); y_idx++)
				{
					m_ExgOppSlot[x_pos+x_idx][y_idx] = InterChangeVnum[idx][candi4];
				}
			}
			x_pos += GET_ITEM_WIDTH(pItem);
		}
	}

	if (InterChangeVnum[idx][sel] != InterChangeVnum[idx][candi5] &&
			InterChangeVnum[idx][candi1] != InterChangeVnum[idx][candi5] &&
			InterChangeVnum[idx][candi2] != InterChangeVnum[idx][candi5] &&
			InterChangeVnum[idx][candi3] != InterChangeVnum[idx][candi5] &&
			InterChangeVnum[idx][candi4] != InterChangeVnum[idx][candi5])
	{
		// make dummy items
		pItem = new CItem;
		if( pItem )
		{
			pItem->m_SlotX = x_pos;
			pItem->m_SlotY = 0;
			pItem->m_Index = pChangeItem->m_Index+5;
			pItem->m_PlusNum = pChangeItem->m_PlusNum;
			pItem->m_Vnum = InterChangeVnum[idx][candi5];
			pItem->m_ProtoNum = g_ItemList.FindItem(InterChangeVnum[idx][candi5]);

			pItem->m_Num0 = GET_ITEM_NUM0(pItem);
			pItem->m_Num1 = GET_ITEM_NUM1(pItem);
			pItem->m_Num2 = GET_ITEM_NUM2(pItem);
			pItem->m_Num3 = GET_ITEM_NUM3(pItem);
			pItem->m_Num4 = GET_ITEM_NUM4(pItem);
			pItem->m_Num5 = GET_ITEM_NUM5(pItem);
			pItem->m_Num6 = GET_ITEM_NUM6(pItem);
			pItem->m_Num7 = GET_ITEM_NUM7(pItem);
			pItem->m_Num8 = GET_ITEM_NUM8(pItem);
			pItem->m_Num9 = GET_ITEM_NUM9(pItem);
			pItem->m_MinLevel = GET_ITEM_LEVEL(pItem);
			pItem->m_MinLevel_Org = pItem->m_MinLevel;
			pItem->m_Int = GET_ITEM_INT(pItem);
			pItem->m_Dex = GET_ITEM_DEX(pItem);
			pItem->m_Weight = GET_WEIGHT(pItem);

			pItem->m_Next = m_ExgOppInven;
			m_ExgOppInven = pItem;

			bWillChange = TRUE;

			for (x_idx = 0; x_idx < GET_ITEM_WIDTH(pItem); x_idx++)
			{
				for (y_idx = 0; y_idx < GET_ITEM_HEIGHT(pItem); y_idx++)
				{
					m_ExgOppSlot[x_pos+x_idx][y_idx] = InterChangeVnum[idx][candi5];
				}
			}
			x_pos += GET_ITEM_WIDTH(pItem);
		}
	}

	if (InterChangeVnum[idx][sel] != InterChangeVnum[idx][candi6] &&
			InterChangeVnum[idx][candi1] != InterChangeVnum[idx][candi6] &&
			InterChangeVnum[idx][candi2] != InterChangeVnum[idx][candi6] &&
			InterChangeVnum[idx][candi3] != InterChangeVnum[idx][candi6] &&
			InterChangeVnum[idx][candi4] != InterChangeVnum[idx][candi6] &&
			InterChangeVnum[idx][candi5] != InterChangeVnum[idx][candi6])
	{
		// make dummy items
		pItem = new CItem;
		if( pItem )
		{
			pItem->m_SlotX = x_pos;
			pItem->m_SlotY = 0;
			pItem->m_Index = pChangeItem->m_Index+6;
			pItem->m_PlusNum = pChangeItem->m_PlusNum;
			pItem->m_Vnum = InterChangeVnum[idx][candi6];
			pItem->m_ProtoNum = g_ItemList.FindItem(InterChangeVnum[idx][candi6]);

			pItem->m_Num0 = GET_ITEM_NUM0(pItem);
			pItem->m_Num1 = GET_ITEM_NUM1(pItem);
			pItem->m_Num2 = GET_ITEM_NUM2(pItem);
			pItem->m_Num3 = GET_ITEM_NUM3(pItem);
			pItem->m_Num4 = GET_ITEM_NUM4(pItem);
			pItem->m_Num5 = GET_ITEM_NUM5(pItem);
			pItem->m_Num6 = GET_ITEM_NUM6(pItem);
			pItem->m_Num7 = GET_ITEM_NUM7(pItem);
			pItem->m_Num8 = GET_ITEM_NUM8(pItem);
			pItem->m_Num9 = GET_ITEM_NUM9(pItem);
			pItem->m_MinLevel = GET_ITEM_LEVEL(pItem);
			pItem->m_MinLevel_Org = pItem->m_MinLevel;
			pItem->m_Int = GET_ITEM_INT(pItem);
			pItem->m_Dex = GET_ITEM_DEX(pItem);
			pItem->m_Weight = GET_WEIGHT(pItem);

			pItem->m_Next = m_ExgOppInven;
			m_ExgOppInven = pItem;

			bWillChange = TRUE;

			for (x_idx = 0; x_idx < GET_ITEM_WIDTH(pItem); x_idx++)
			{
				for (y_idx = 0; y_idx < GET_ITEM_HEIGHT(pItem); y_idx++)
				{
					m_ExgOppSlot[x_pos+x_idx][y_idx] = InterChangeVnum[idx][candi6];
				}
			}
			x_pos += GET_ITEM_WIDTH(pItem);
		}
	}

	if (InterChangeVnum[idx][sel] != InterChangeVnum[idx][candi7] &&
			InterChangeVnum[idx][candi1] != InterChangeVnum[idx][candi7] &&
			InterChangeVnum[idx][candi2] != InterChangeVnum[idx][candi7] &&
			InterChangeVnum[idx][candi3] != InterChangeVnum[idx][candi7] &&
			InterChangeVnum[idx][candi4] != InterChangeVnum[idx][candi7] &&
			InterChangeVnum[idx][candi5] != InterChangeVnum[idx][candi7] &&
			InterChangeVnum[idx][candi6] != InterChangeVnum[idx][candi7])
	{
		// make dummy items
		pItem = new CItem;
		if( pItem )
		{
			pItem->m_SlotX = 2;
			pItem->m_SlotY = 2;
			pItem->m_Index = pChangeItem->m_Index+7;
			pItem->m_PlusNum = pChangeItem->m_PlusNum;
			pItem->m_Vnum = InterChangeVnum[idx][candi7];
			pItem->m_ProtoNum = g_ItemList.FindItem(InterChangeVnum[idx][candi7]);

			pItem->m_Num0 = GET_ITEM_NUM0(pItem);
			pItem->m_Num1 = GET_ITEM_NUM1(pItem);
			pItem->m_Num2 = GET_ITEM_NUM2(pItem);
			pItem->m_Num3 = GET_ITEM_NUM3(pItem);
			pItem->m_Num4 = GET_ITEM_NUM4(pItem);
			pItem->m_Num5 = GET_ITEM_NUM5(pItem);
			pItem->m_Num6 = GET_ITEM_NUM6(pItem);
			pItem->m_Num7 = GET_ITEM_NUM7(pItem);
			pItem->m_Num8 = GET_ITEM_NUM8(pItem);
			pItem->m_Num9 = GET_ITEM_NUM9(pItem);
			pItem->m_MinLevel = GET_ITEM_LEVEL(pItem);
			pItem->m_MinLevel_Org = pItem->m_MinLevel;
			pItem->m_Int = GET_ITEM_INT(pItem);
			pItem->m_Dex = GET_ITEM_DEX(pItem);
			pItem->m_Weight = GET_WEIGHT(pItem);

			pItem->m_Next = m_ExgOppInven;
			m_ExgOppInven = pItem;

			bWillChange = TRUE;

			for (x_idx = 0; x_idx < GET_ITEM_WIDTH(pItem); x_idx++)
			{
				for (y_idx = 0; y_idx < GET_ITEM_HEIGHT(pItem); y_idx++)
				{
					m_ExgOppSlot[2+x_idx][2+y_idx] = InterChangeVnum[idx][candi7];
				}
			}
			x_pos += GET_ITEM_WIDTH(pItem);
		}
	}

	return bWillChange;
}

void CInterchange::LoadRes()
{
	if( !g_pDisplay )
		return;

	m_hWnd =  g_pDisplay->GetHWnd();
	GetClientRect(m_hWnd, &m_ClientRc);

	if( g_pNk2DFrame )
	{
		m_ScreenWidth = g_pNk2DFrame->GetClientWidth();
		m_ScreenHeight = g_pNk2DFrame->GetClientHeight();
	}

	m_CancelBtn.LoadRes();
	SetVisibleMode(m_bExch);

	g_pDisplay->CreateSurfaceFromBitmap (&m_pEnableSlot, "interface/slot/slot_enable.bmp");
	g_pDisplay->CreateSurfaceFromBitmap (&m_pDisableSlot, "interface/slot/slot_disable.bmp");
	g_pDisplay->CreateSurfaceFromBitmap (&m_pExistSlot, "interface/slot/slot_exist.bmp");

	SAFE_DELETE(m_pSurfaceBack);
	g_pDisplay->CreateSurfaceFromBitmap(&m_pSurfaceBack, "interface/interchange/interchange.bmp");

	if( !m_pSurfaceBack )
		return;

	m_pSurfaceBack->SetColorKey( TRANS_COLOR );
	m_pSurfaceBack->Xpos = g_pNk2DFrame->GetClientWidth() - 291 - m_pSurfaceBack->GetWidth();
	m_pSurfaceBack->Ypos = 0;

	// 상대편 슬롯 창.
	m_opp_slotstart_x = m_pSurfaceBack->Xpos + 35;
	m_opp_slotstart_y = m_pSurfaceBack->Ypos + 80;

	// 나의 슬롯창.
	m_my_slotstart_x = m_pSurfaceBack->Xpos + 35;
	m_my_slotstart_y = m_pSurfaceBack->Ypos + 261;

	// 버튼 위치 정하기.
	m_CancelBtn.SetPosition(m_pSurfaceBack->Xpos+214, m_pSurfaceBack->Ypos+5);


	char plus = '+';
	char tmp[10] = "";
	if(m_ExgOppInven)
	{
		CItem *pItem = m_ExgOppInven;
		while (pItem)
		{
			if(pItem->m_pTextOutBox)
			{
				sprintf(tmp, "%c%d",plus, pItem->m_PlusNum);
				pItem->m_pTextOutBox->SetString(tmp, EXCH_TEXT_COLOR, EXCH_BAKC_COLOR);
			}
			pItem = pItem->m_Next;
		}
	}
}

void CInterchange::DeleteRes()
{
	SAFE_DELETE(m_pSurfaceBack);
	SAFE_DELETE(m_pEnableSlot);
	SAFE_DELETE(m_pDisableSlot);
	SAFE_DELETE(m_pExistSlot);

	m_CancelBtn.DeleteRes();

	if(m_ExgOppInven)
	{
		CItem *pItem = m_ExgOppInven;
		while (pItem)
		{
			if(pItem->m_pTextOutBox)
			{
				pItem->m_pTextOutBox->DeleteRes();
			}
			pItem = pItem->m_Next;
		}
	}
}

#define GET_OPP_SLOT_X(point_x)		((point_x - m_opp_slotstart_x) / SLOT_WIDTH)
#define GET_OPP_SLOT_Y(point_y)		((point_y - m_opp_slotstart_y) / SLOT_HEIGHT)

#define GET_MY_SLOT_X(point_x)		((point_x - m_my_slotstart_x) / SLOT_WIDTH)
#define GET_MY_SLOT_Y(point_y)		((point_y - m_my_slotstart_y) / SLOT_HEIGHT)

CItem* CInterchange::Draw()
{
	if (g_pDisplay && m_pSurfaceBack)
	{
		POINT point;
		GetCursorPos(&point);
		ScreenToClient(g_pDisplay->GetHWnd(), &point);

		//m_pOverItem = NULL;
		//CItem *pOverItem = NULL;

		g_pDisplay->Blt(m_pSurfaceBack->Xpos, m_pSurfaceBack->Ypos, m_pSurfaceBack);

		// 버튼들 그리기.
		m_CancelBtn.Draw();

		m_pOverItem = NULL;
		DrawMySlot();
		DrawOppSlot();


		// 마우스가 아이템 위로 올라갔을때의 설명.
		CMultiText multiText;
		if (m_pNkCha && m_pNkCha->m_ExtraSlot)
		{
			m_pOverItem = NULL;
		}
		else if (pCMyApp->m_pMouse && m_pOverItem)
		{
			pCMyApp->m_pMouse->SetMouseType(M_HOLD);
		}
		else if (pCMyApp->m_pMouse && IsInside(point.x, point.y))
		{
			pCMyApp->m_pMouse->SetMouseType(M_NORMAL);
		}
	}

	return m_pOverItem;
}

void CInterchange::DrawMySlot()
{
	if (g_pDisplay && m_pSurfaceBack)
	{
		RECT rcRect;
		POINT point;
		GetCursorPos(&point);
		ScreenToClient(g_pDisplay->GetHWnd(), &point);

		// 아이템이 위치한 슬롯 표시
		CItem *pItem = m_pNkCha->m_ExgInven;
		while (pItem)
		{
			rcRect.left = 0;
			rcRect.right = SLOT_WIDTH * GET_ITEM_WIDTH(pItem);
			rcRect.top = 0;
			rcRect.bottom = SLOT_HEIGHT * GET_ITEM_HEIGHT(pItem);
			g_pDisplay->Blt( m_my_slotstart_x + (SLOT_WIDTH*pItem->m_SlotX),
							 m_my_slotstart_y + (SLOT_HEIGHT*pItem->m_SlotY),
							 m_pExistSlot, &rcRect);
			pItem = pItem->m_Next;
		}

		// Extra Slot에 있는 아이템이 놓일 곳 표시
		int slot_x_num, slot_y_num;
		if (m_pNkCha && m_pNkCha->m_ExtraSlot)
		{
			if (point.x >= m_my_slotstart_x && point.x < m_my_slotstart_x + (SLOT_WIDTH * EXG_SLOT_X_NUM ) &&
					point.y >= m_my_slotstart_y && point.y < m_my_slotstart_y + (SLOT_HEIGHT * EXG_SLOT_Y_NUM) )
			{
				if (GET_ITEM_WIDTH(m_pNkCha->m_ExtraSlot) % 2)
				{
					slot_x_num = (point.x - m_my_slotstart_x) / SLOT_WIDTH - (GET_ITEM_WIDTH(m_pNkCha->m_ExtraSlot)/2);
				}
				else
				{
					slot_x_num = (point.x - m_my_slotstart_x + (SLOT_WIDTH/2)) / SLOT_WIDTH - (GET_ITEM_WIDTH(m_pNkCha->m_ExtraSlot)/2);
				}
				if (GET_ITEM_HEIGHT(m_pNkCha->m_ExtraSlot) % 2)
				{
					slot_y_num = (point.y - m_my_slotstart_y) / SLOT_HEIGHT - (GET_ITEM_HEIGHT(m_pNkCha->m_ExtraSlot)/2);
				}
				else
				{
					slot_y_num = (point.y - m_my_slotstart_y + (SLOT_HEIGHT/2)) / SLOT_HEIGHT - (GET_ITEM_HEIGHT(m_pNkCha->m_ExtraSlot)/2);
				}
				if (slot_x_num >= 0 && slot_x_num + GET_ITEM_WIDTH(m_pNkCha->m_ExtraSlot) - 1 < EXG_SLOT_X_NUM &&
						slot_y_num >= 0 && slot_y_num + GET_ITEM_HEIGHT(m_pNkCha->m_ExtraSlot) - 1 < EXG_SLOT_Y_NUM)
				{
					rcRect.left = 0;
					rcRect.right = SLOT_WIDTH * GET_ITEM_WIDTH(m_pNkCha->m_ExtraSlot);
					rcRect.top = 0;
					rcRect.bottom = SLOT_HEIGHT * GET_ITEM_HEIGHT(m_pNkCha->m_ExtraSlot);
					if (m_pNkCha->GetItemIndexInExgSlot(slot_x_num, slot_y_num,
														GET_ITEM_WIDTH(m_pNkCha->m_ExtraSlot), GET_ITEM_HEIGHT(m_pNkCha->m_ExtraSlot))
							>= -1)
					{
						g_pDisplay->Blt( m_my_slotstart_x + (SLOT_WIDTH*slot_x_num),
										 m_my_slotstart_y + (SLOT_HEIGHT*slot_y_num),
										 m_pEnableSlot, &rcRect);
					}
					else
					{
						g_pDisplay->Blt( m_my_slotstart_x + (SLOT_WIDTH*slot_x_num),
										 m_my_slotstart_y + (SLOT_HEIGHT*slot_y_num),
										 m_pDisableSlot, &rcRect);
					}
				}
			}
		}

		// 내 교환품 그리기..
		if (point.x >= m_my_slotstart_x && point.y >= m_my_slotstart_y)
		{
			slot_x_num = GET_MY_SLOT_X(point.x);
			slot_y_num = GET_MY_SLOT_Y(point.y);
		}
		else
		{
			slot_x_num = -1;
			slot_y_num = -1;
		}
		int index = 0;
		if( m_pNkCha )
			index = m_pNkCha->GetExgSlotIndex(slot_x_num, slot_y_num);

		if( m_pNkCha )
			pItem = m_pNkCha->m_ExgInven;

		if( g_pNk2DFrame->GetItemRender() )
		{
			g_pNk2DFrame->GetItemRender()->BeginRenderLolo();		// 중요..  m_pItemRender->RenderLolo의 파라미터에 영향을 준다.
			while (pItem)
			{
				if (pItem->m_Index == index)
				{
					g_pNk2DFrame->RenderItemInUI( m_my_slotstart_x + pItem->m_SlotX * SLOT_WIDTH,
												  m_my_slotstart_y + pItem->m_SlotY * SLOT_HEIGHT,
												  pItem , TRUE, FALSE, FALSE, TRUE );
					m_pOverItem = pItem;
				}
				else
				{
					DWORD ambient = 0x00555555;
					if(g_pNk2DFrame->IsScroll(pItem))
					{
						ambient = 0x00cccccc;
					}
					else if (pItem->m_PlusNum > 0)
					{
						ambient = UPGRADE_ITEM_COLOR;
					}
					g_pNk2DFrame->RenderItemInUI( m_my_slotstart_x + pItem->m_SlotX * SLOT_WIDTH,
												  m_my_slotstart_y + pItem->m_SlotY * SLOT_HEIGHT,
												  pItem , FALSE, FALSE, FALSE, TRUE );
				}
				pItem = pItem->m_Next;
			}
			g_pNk2DFrame->GetItemRender()->EndRenderLolo();
		}
	}
}

void CInterchange::DrawOppSlot()
{
	int xpos, ypos;

	if (g_pDisplay && m_pSurfaceBack)
	{
		RECT rcRect;
		POINT point;
		GetCursorPos(&point);
		ScreenToClient(g_pDisplay->GetHWnd(), &point);

		// 아이템이 위치한 슬롯 표시
		CItem *pItem = m_ExgOppInven;
		while (pItem)
		{
			rcRect.left = 0;
			rcRect.right = SLOT_WIDTH * GET_ITEM_WIDTH(pItem);
			rcRect.top = 0;
			rcRect.bottom = SLOT_HEIGHT * GET_ITEM_HEIGHT(pItem);
			xpos = m_opp_slotstart_x + (SLOT_WIDTH*pItem->m_SlotX);
			ypos = m_opp_slotstart_y + (SLOT_HEIGHT*pItem->m_SlotY);
			g_pDisplay->Blt( xpos, ypos, m_pExistSlot, &rcRect);

			pItem = pItem->m_Next;
		}

		// Extra Slot에 있는 아이템이 놓일 곳 표시
		int slot_x_num, slot_y_num;

		// 내 교환품 그리기..
		if (point.x >= m_opp_slotstart_x && point.y >= m_opp_slotstart_y)
		{
			slot_x_num = GET_OPP_SLOT_X(point.x);
			slot_y_num = GET_OPP_SLOT_Y(point.y);
		}
		else
		{
			slot_x_num = -1;
			slot_y_num = -1;
		}
		// 이 부분에서 vnum을 얻어와야 한다. (interchange class의 경우에)
		int vnum = GetExgOppSlotVnum(slot_x_num, slot_y_num);

		pItem = m_ExgOppInven;

		if( g_pNk2DFrame->GetItemRender() )
		{
			g_pNk2DFrame->GetItemRender()->BeginRenderLolo();				// 중요....... 중요..
			while (pItem)
			{
				if (pItem->m_Vnum == vnum)
				{
					g_pNk2DFrame->RenderItemInUI( m_opp_slotstart_x + pItem->m_SlotX * SLOT_WIDTH,
												  m_opp_slotstart_y + pItem->m_SlotY * SLOT_HEIGHT,
												  pItem , TRUE, FALSE, FALSE, TRUE );

					m_pOverItem = pItem;
				}
				else
				{
					DWORD ambient = 0x00555555;
					if(g_pNk2DFrame->IsScroll(pItem))
					{
						ambient = 0x00cccccc;
					}
					else if (pItem->m_PlusNum > 0)
					{
						ambient = UPGRADE_ITEM_COLOR;
					}
					g_pNk2DFrame->RenderItemInUI( m_opp_slotstart_x + pItem->m_SlotX * SLOT_WIDTH,
												  m_opp_slotstart_y + pItem->m_SlotY * SLOT_HEIGHT,
												  pItem , FALSE, FALSE, FALSE, TRUE );

					//xpos = m_opp_slotstart_x + pItem->m_SlotX * SLOT_WIDTH + (GET_ITEM_WIDTH(pItem) * SLOT_WIDTH - ITEM_RENDER_WIDTH) /2;
					//ypos = m_opp_slotstart_y + pItem->m_SlotY * SLOT_HEIGHT + (GET_ITEM_HEIGHT(pItem) * SLOT_HEIGHT - ITEM_RENDER_HEIGHT) /2;
					int cx, cy;

					xpos = m_opp_slotstart_x + (pItem->m_SlotX*SLOT_WIDTH);
					ypos = m_opp_slotstart_y + (pItem->m_SlotY*SLOT_HEIGHT);
					cx = SLOT_WIDTH; // * GET_ITEM_WIDTH(pItem);
					cy = SLOT_HEIGHT;// * GET_ITEM_HEIGHT(pItem);

					// +아이템 정보를 가운데로 정열해서 출력한다
					if(	pItem->m_pTextOutBox)
					{

						pItem->m_pTextOutBox->Draw(xpos+((cx/2) - (EXCH_TEXT_SIZE/2)), ypos+((cy/2) - (EXCH_TEXT_SIZE/2)));

					}
				}


				pItem = pItem->m_Next;
			}
			g_pNk2DFrame->GetItemRender()->EndRenderLolo();
		}
	}
}

BOOL CInterchange::IsInside(int x, int y)
{
	if( !m_pSurfaceBack )
		return false;

	return m_pSurfaceBack->IsIn(x,y);
}

BOOL CInterchange::IsPlayArea(int x, int y)
{
	if( !IsInside(x,y) && !g_pNk2DFrame->IsInPopup(x,y) )
		return true;

	return false;
}

void CInterchange::DrawItemInfo()
{}

//----------------------------------------------------------------------------
//-- MsgProc
//----------------------------------------------------------------------------
//-- 2005.06.14
//-- Lyul /수정 완료
//-- 경비소장에서 교환시 아이템을 들고 교환할 경우 팅 나는 오류 수정
LRESULT CInterchange::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static char commOutBuf[512] = "";	// 나중에 변수 다른 곳에 설정.
	int x = 0, y = 0;
	int slot_x_num = 0, slot_y_num = 0, vnum = 0;
	int price = 0;
	int hr = -1;

	switch (msg)
	{
	case WM_LBUTTONDBLCLK:


		x = LOWORD (lParam);
		y = HIWORD (lParam);
		if (x >= m_opp_slotstart_x && x < m_opp_slotstart_x + (SLOT_WIDTH * EXG_SLOT_X_NUM) &&
				y >= m_opp_slotstart_y && y < m_opp_slotstart_y + (SLOT_HEIGHT * EXG_SLOT_Y_NUM) )
		{
			m_cur_click_slot_x = slot_x_num = GET_OPP_SLOT_X(x);
			m_cur_click_slot_y = slot_y_num = GET_OPP_SLOT_Y(y);

			vnum = GetExgOppSlotVnum(m_cur_click_slot_x, m_cur_click_slot_y);

//===============================
// nate 2004 - 5 : CastleWar
			if( vnum == 1010 || vnum == 1031 || vnum == 956 || vnum == 1308 || vnum == 1468 )
			{
//===============================
				sprintf(m_MsgBuf, "ichange %d\n", vnum);
				SendMsg();
				RemoveAllOppInven();

				// 아이템 이동을 못하게 하기 위해서.
				m_bWaitingExch = TRUE;
			}
			//-- Lyul
			else if (vnum >= 0)
			{
				//-- Extra Slot 에 뭘 들고 더블 클릭을 하면 팅~!
				if(m_pNkCha->m_ExtraSlot) return 0;

				//price = GET_PRICE(m_pOverItem)/3;//-- Over 된 Item 의 1/3 가격
				price = GET_PRICE(m_pOverItem);

				if(m_pNkCha && (m_pNkCha->m_Money < price))
				{
					//-- IDS_INTERCHA_NOT_ENOUGH_MONEY : 보 유 금 액 이   부 족 합 니 다.
					g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_INTERCHA_NOT_ENOUGH_MONEY), TYPE_NOR_OK, 1 );
				}
				else
				{
					char tmp[80] = "";
					//-- IDS_INTERCHA_NEED_MONEY : 교환시 필요금액은  %d 라임 입니다.
					sprintf(tmp, G_STRING(IDS_INTERCHA_NEED_MONEY), price);
					g_pNk2DFrame->InsertPopup(tmp,TYPE_NOR_OKCANCLE, 9);
				}
			}
		}
		return 1;
		break;

	case WM_LBUTTONUP:
	case WM_MOUSEMOVE:
		x = LOWORD (lParam);
		y = HIWORD (lParam);
		if (!IsInside(x, y))
			return 0;

		m_CancelBtn.MsgProc(hWnd, msg, wParam, lParam);

		if (m_CancelBtn.GetState() == BTN_ACTION)
		{
			if (m_bExch)
				InterCancel(TRUE);

			if( g_pNk2DFrame )
				g_pNk2DFrame->ShowInterchangeWindow(FALSE);
			m_CancelBtn.SetState(BTN_NORMAL);
		}
		return 1;
		break;

	case WM_RBUTTONDOWN:
		x = LOWORD (lParam);
		y = HIWORD (lParam);
		if (IsPlayArea(x, y))
		{
			if (m_bExch)
				InterCancel(TRUE);
			if( g_pNk2DFrame )
				g_pNk2DFrame->ShowInterchangeWindow(FALSE);
		}
		return 1;

	case WM_LBUTTONDOWN:
		x = LOWORD (lParam);
		y = HIWORD (lParam);

		if (IsInside(x, y))
		{
			if (m_CancelBtn.IsInside(x, y))
				m_CancelBtn.MsgProc(hWnd, msg, wParam, lParam);
			else if (x >= m_my_slotstart_x && x < m_my_slotstart_x + (SLOT_WIDTH * EXG_SLOT_X_NUM) &&
					 y >= m_my_slotstart_y && y < m_my_slotstart_y + (SLOT_HEIGHT * EXG_SLOT_Y_NUM) )
			{
				if( timeGetTime() - m_dwMouseClickTickTime < 300 )  // 0.3초
					return 1;

				m_dwMouseClickTickTime = timeGetTime();

				int slot_x_num, slot_y_num;
				// 엑스트라 슬롯에 있는 경우, 교환창 슬롯에 놓는다.
				if (m_pNkCha && m_pNkCha->m_ExtraSlot)
				{
					if (m_bExch)
						return 1;
					if (GET_ITEM_WIDTH(m_pNkCha->m_ExtraSlot) % 2)
					{
						slot_x_num = (x - m_my_slotstart_x) / SLOT_WIDTH - (GET_ITEM_WIDTH(m_pNkCha->m_ExtraSlot)/2);
					}
					else
					{
						slot_x_num = (x - m_my_slotstart_x + (SLOT_WIDTH/2)) / SLOT_WIDTH - (GET_ITEM_WIDTH(m_pNkCha->m_ExtraSlot)/2);
					}
					if (GET_ITEM_HEIGHT(m_pNkCha->m_ExtraSlot) % 2)
					{
						slot_y_num = (y - m_my_slotstart_y) / SLOT_HEIGHT - (GET_ITEM_HEIGHT(m_pNkCha->m_ExtraSlot)/2);
					}
					else
					{
						slot_y_num = (y - m_my_slotstart_y + (SLOT_HEIGHT/2)) / SLOT_HEIGHT - (GET_ITEM_HEIGHT(m_pNkCha->m_ExtraSlot)/2);
					}
					if (slot_x_num >= 0 && slot_x_num + GET_ITEM_WIDTH(m_pNkCha->m_ExtraSlot) - 1 < EXG_SLOT_X_NUM &&
							slot_y_num >= 0 && slot_y_num + GET_ITEM_HEIGHT(m_pNkCha->m_ExtraSlot) - 1 < EXG_SLOT_Y_NUM)
					{
						if (m_pNkCha->AddToExgInven(slot_x_num, slot_y_num))
						{
							sprintf(m_MsgBuf, "exch inven %d %d\n", slot_x_num, slot_y_num);
							SendMsg();
						}
						m_bExch = CanInterchange(TRUE);
						SetVisibleMode(m_bExch);
					}
				}
				else
				{
					// 아이템을 빼는 것.
					if (!m_bWaitingExch)
					{
						if (m_pNkCha && m_pNkCha->AddToExgInven((x-m_my_slotstart_x) / SLOT_WIDTH, (y-m_my_slotstart_y) / SLOT_HEIGHT))
						{
							sprintf(m_MsgBuf, "exch inven %d %d\n", (x-m_my_slotstart_x) / SLOT_WIDTH, (y-m_my_slotstart_y) / SLOT_HEIGHT);
							SendMsg();
						}
						m_bExch = CanInterchange(FALSE);
						SetVisibleMode(m_bExch);
					}
				}
			}
		}
		else if (IsPlayArea(x, y))
		{
			if (m_bExch)
				InterCancel(TRUE);
			if( g_pNk2DFrame )
				g_pNk2DFrame->ShowInterchangeWindow(FALSE);	// 이 내부에서 자동적으로 취소 메시지가 간다.
		}
		else
			return 0;

		return 1;
		break;
	}

	return 0;
}

void CInterchange::SetNkCha(CNkCharacter *pNkCha)
{
	m_pNkCha = pNkCha;
}

void CInterchange::SendMsg()
{
	if (g_pTcpIp)
		g_pTcpIp->SendNetMessage(m_MsgBuf);
}

// 거래를 활성/비활성 상태로 만든다.
// 거래중일때 비활성 상태로 변하면 거래를 중지한다.
void CInterchange::ActivateExch(BOOL bExch)
{
	if (m_bExch && bExch == FALSE)
	{
		InterCancel(TRUE);
		return;
	}
	else
	{
		if (bExch)
		{
			// check how many item in my slot
			m_bExch = CanInterchange();
			sprintf(m_MsgBuf, "ichange\n");
			SendMsg();
		}
		SetVisibleMode(m_bExch);
	}
}

BOOL CInterchange::CanInterchange(BOOL bMsg)
{
	int nCount = 0;
	BOOL bExch = FALSE;
	CItem *pItem = m_pNkCha->m_ExgInven;

	while (pItem)
	{
		pItem = pItem->m_Next;
		nCount++;
	}

	if (nCount == 0)
	{
		//해외에서 빼달라고 왔는대 국내도 빼야대나..진짜 필요없고 귀찮긴한대.. 우선막아놓고 물어보장
		bExch = FALSE;
	}
	else if (nCount == 1)
	{
		// 태극망토와 태극날개가 놓여 있으면 FALSE
		if (m_pNkCha
				&& (m_pNkCha->m_ExgInven->m_Vnum == 496
					|| m_pNkCha->m_ExgInven->m_Vnum == 497
					|| m_pNkCha->m_ExgInven->m_Vnum == 920
					|| m_pNkCha->m_ExgInven->m_Vnum == 921))
		{
			//0304
			bExch = FALSE;
		}
		else if (m_pNkCha->m_ExgInven->m_Vnum == 564)     // 월드컵 트로피
		{
			// 월드컵 트로피는 561, 562, 563으로 바뀐다.
			bExch = TRUE;
			//-- IDS_EXCHANGE_UNIFORM_SET : 유니폼 셋트로 교환하시겠습니까?
			g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_EXCHANGE_UNIFORM_SET),TYPE_NOR_OKCANCLE, 9);
		}
		/*else if (m_pNkCha && m_pNkCha->m_ExgInven->m_PlusNum != 0)     // || m_pNkCha->m_ExgInven->m_Special != 0
		{
			bExch = FALSE;
			if (bMsg)
				//-- IDS_INTERCHA_NORMAL_ITEM : 노말아이템만 교환가능합니다
				g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_INTERCHA_NORMAL_ITEM), TYPE_NOR_OK, 1 );
		}*/
		else
			bExch = TRUE;
	}
	else if (nCount > 1)
	{
		bExch = FALSE;
		if (bMsg)
			//-- IDS_INTERCHA_PUT_ITEM : 바꿀 아이템 하나만 놓아주세요.
			g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_INTERCHA_PUT_ITEM), TYPE_NOR_OK, 1 );
	}

	return bExch;
}

void CInterchange::ExchInvenTo(int x, int y, int index, int vnum, int gem_num, int special)
{
	if (vnum < 0)
		return;

	char plus = '+';
	int proto_num = g_ItemList.FindItem(vnum);

	if (proto_num >= 0)
	{
		CItem *pItem = new CItem;
		if( !pItem )
			return;

		pItem->m_Index = index;
		pItem->m_Vnum = vnum;
		pItem->m_ProtoNum = proto_num;

		// 아이템 값을 입력해보자!
		ResetItemValue(pItem);
		// +아이템을 경우 CTextOutBox 생성
		if (gem_num > 0)
		{
			//-- 2004. 03. 22  / Lyul
			//-- 랩프리 다이아 장비 인식 오류 Big Fix
			if(GET_ITEM_LEVEL(pItem) >=181)
				pItem->SetDiaGemNum(gem_num);
			else
				pItem->SetGemNum(gem_num);

			if(pItem->m_pTextOutBox)  SAFE_DELETE(pItem->m_pTextOutBox);

			pItem->m_pTextOutBox = new CTextOutBox();

			if( pItem->m_pTextOutBox )
			{
				char tmp[10] = "";
				pItem->m_pTextOutBox->Init(EXCH_TEXT_SIZE, EXCH_TEXT_COLOR, 0, 0);
				sprintf(tmp, "%c%d",plus, pItem->m_PlusNum);
				pItem->m_pTextOutBox->SetString(tmp, EXCH_TEXT_COLOR, EXCH_BAKC_COLOR);
			}
		}

		// 그 자리에 무언가 있을때 없앤다.
		ExchInvenFrom(x, y, GET_ITEM_WIDTH(pItem), GET_ITEM_HEIGHT(pItem));

		pItem->m_SlotX = x;
		pItem->m_SlotY = y;

		pItem->m_Next = m_ExgOppInven;
		pItem->m_Special = special;
		SetSpecialValue(pItem, GET_TYPE(pItem), GET_SHAPE(pItem), special, 0);

		m_ExgOppInven = pItem;

		for (int x_idx = 0; x_idx < GET_ITEM_WIDTH(pItem); x_idx++)
		{
			for (int y_idx = 0; y_idx < GET_ITEM_HEIGHT(pItem); y_idx++)
			{
				m_ExgOppSlot[x+x_idx][y+y_idx] = index;
			}
		}
	}

}

int CInterchange::GetExgOppSlotIndex(int x, int y)
{
	if (x < 0 || x >= EXG_SLOT_X_NUM || y < 0 || y >= EXG_SLOT_Y_NUM)
		return -1;

	return m_ExgOppSlot[x][y];
}

int	CInterchange::GetExgOppSlotVnum(int x, int y)
{
	if ( x < 0 || x >= EXG_SLOT_X_NUM || y < 0 || y >= EXG_SLOT_Y_NUM )
		return -1;

	int vnum = m_ExgOppSlot[x][y];

	return vnum;
}

//--------------------------------------------------------------------------
// 상대의 교환창 부분에서 아이템이 없앨때.
//--------------------------------------------------------------------------
int CInterchange::GetItemIndexInOppSlot(int x, int y, int w, int h)
{
	int index = -1;

	for (int x_idx = x; x_idx < x+w; x_idx++)
	{
		for (int y_idx = y; y_idx < y+h; y_idx++)
		{
			if (m_ExgOppSlot[x_idx][y_idx] != -1)
			{
				if (index == -1)
					index = m_ExgOppSlot[x_idx][y_idx];
				else if (index != m_ExgOppSlot[x_idx][y_idx])
					return -2;
			}
		}
	}

	return index;
}

CItem * CInterchange::PickItemInOppSlot(int index, BOOL isVnum)
{
	CItem *beforeItem = NULL;
	CItem *pItem = m_ExgOppInven;

	while (pItem)
	{
		if ((!isVnum && pItem->m_Index == index) || (isVnum && pItem->m_Vnum == index))
		{
			if (beforeItem)
			{
				beforeItem->m_Next = pItem->m_Next;
			}
			else
			{
				m_ExgOppInven = pItem->m_Next;
			}

			for (int x_idx = 0; x_idx < GET_ITEM_WIDTH(pItem); x_idx++)
			{
				for (int y_idx = 0; y_idx < GET_ITEM_HEIGHT(pItem); y_idx++)
				{
					m_ExgOppSlot[pItem->m_SlotX+x_idx][pItem->m_SlotY+y_idx] = -1;
				}
			}
			pItem->m_Next = NULL;
			break;
		}
		beforeItem = pItem;
		pItem = pItem->m_Next;
	}

	return pItem;
}

void CInterchange::ExchInvenFrom(int x, int y, int w, int h)
{
	CItem *pItem = NULL;
	int index;

	index = GetItemIndexInOppSlot(x, y, w, h);
	if (index >= 0)
	{
		pItem = PickItemInOppSlot(index);

		if (pItem)
			delete pItem;
	}
}

// [6/1/2007 Theodoric] 내구도 수리 :: 함수 파라미터 내구도 포함
void CInterchange::InterchangeTo(int old_idx, int new_idx, int vnum, int gem_num, int special, int special2,
								 int UpEndur, int MaxUpdEndur, int Endur, int MaxEndur) ///신방어구강화시스템
{
	int x_idx, y_idx;

	// 닌자복에 대한 것을 처리한다.
	// 닌자복은 old_idx가 -1, -2, -3으로 인자가 넘어온다.
	if (old_idx == -1 || old_idx == -2 || old_idx == -3 || old_idx == -4 ||
			(m_pNkCha->m_ExgInven && old_idx == m_pNkCha->m_ExgInven->m_Index))   // 교환창에 있는 아이템의 인덱스를 검사한다.
	{
		// 상대편에게 준 리스트를 지워버린다.
		RemoveAllOppInven();

		if (old_idx < 0)
		{
			RemoveAllMyInven();
			if( m_pNkCha ) // [6/7/2007 Theodoric] 내구도 수리
				m_pNkCha->AddToExgInven(gem_num, special, new_idx, vnum, 0, 0, 0, 0, 0, 0, 0 ); // 닌자복의 경우 특이하게 gem_num과 special에 x,y가 들어오는듯. (원석)	///신방어구강화시스템
		}
		else if( m_pNkCha && m_pNkCha->m_ExgInven )
		{
			m_pNkCha->m_ExgInven->m_Index = new_idx;
			m_pNkCha->m_ExgInven->m_Vnum = vnum;
			m_pNkCha->m_ExgInven->m_SlotX = 0;
			m_pNkCha->m_ExgInven->m_SlotY = 0;
			m_pNkCha->m_ExgInven->m_PlusNum = gem_num;
			m_pNkCha->m_ExgInven->m_Special = special;

			///신방어구강화시스템
			m_pNkCha->m_ExgInven->m_Special2 = special2; // 추가 옵션
			m_pNkCha->m_ExgInven->m_UpgradeEndurance = UpEndur;           // 현재 강화 내구도
			m_pNkCha->m_ExgInven->m_MaxUpgradeEndurance = MaxUpdEndur;    // 강화 내구도 최대치

			m_pNkCha->m_ExgInven->m_Endurance = Endur;
			m_pNkCha->m_ExgInven->m_MaxEndurance = MaxEndur;

			m_pNkCha->m_ExgInven->m_ProtoNum = g_ItemList.FindItem(vnum);

			m_pNkCha->m_ExgInven->m_Num0 = GET_ITEM_NUM0(m_pNkCha->m_ExgInven);
			m_pNkCha->m_ExgInven->m_Num1 = GET_ITEM_NUM1(m_pNkCha->m_ExgInven);
			m_pNkCha->m_ExgInven->m_Num2 = GET_ITEM_NUM2(m_pNkCha->m_ExgInven);
			m_pNkCha->m_ExgInven->m_Num3 = GET_ITEM_NUM3(m_pNkCha->m_ExgInven);
			m_pNkCha->m_ExgInven->m_Num4 = GET_ITEM_NUM4(m_pNkCha->m_ExgInven);
			m_pNkCha->m_ExgInven->m_Num5 = GET_ITEM_NUM5(m_pNkCha->m_ExgInven);
			m_pNkCha->m_ExgInven->m_Num6 = GET_ITEM_NUM6(m_pNkCha->m_ExgInven);
			m_pNkCha->m_ExgInven->m_Num7 = GET_ITEM_NUM7(m_pNkCha->m_ExgInven);
			m_pNkCha->m_ExgInven->m_Num8 = GET_ITEM_NUM8(m_pNkCha->m_ExgInven);
			m_pNkCha->m_ExgInven->m_Num9 = GET_ITEM_NUM9(m_pNkCha->m_ExgInven);
			m_pNkCha->m_ExgInven->m_MinLevel = GET_ITEM_LEVEL(m_pNkCha->m_ExgInven);
			m_pNkCha->m_ExgInven->m_MinLevel_Org = m_pNkCha->m_ExgInven->m_MinLevel;
			m_pNkCha->m_ExgInven->m_Int = GET_ITEM_INT(m_pNkCha->m_ExgInven);
			m_pNkCha->m_ExgInven->m_Dex = GET_ITEM_DEX(m_pNkCha->m_ExgInven);
			m_pNkCha->m_ExgInven->m_Weight = GET_WEIGHT(m_pNkCha->m_ExgInven);

			//-- 2004. 03. 22  / Lyul
			//-- 랩프리 다이아 장비 인식 오류 Big Fix
			if(GET_ITEM_LEVEL(m_pNkCha->m_ExgInven) >= 181)
				m_pNkCha->m_ExgInven->SetDiaGemNum(gem_num);
			else
				m_pNkCha->m_ExgInven->SetGemNum(gem_num);

			// Clear
			if( old_idx > -2 )
			{
				for (x_idx = 0; x_idx < EXG_SLOT_X_NUM; x_idx++)
				{
					for (y_idx = 0; y_idx < EXG_SLOT_Y_NUM; y_idx++)
					{
						m_pNkCha->m_ExgSlot[x_idx][y_idx] = -1;
					}
				}
			}

			if( old_idx != -4 && new_idx != -1 )   // 경품권 추첨시 라임이 들어오면 아이템 추가할 필요 없음
			{
				// Add
				for (x_idx = 0; x_idx < GET_ITEM_WIDTH(m_pNkCha->m_ExgInven); x_idx++)
				{
					for (y_idx = 0; y_idx < GET_ITEM_HEIGHT(m_pNkCha->m_ExgInven); y_idx++)
					{
						m_pNkCha->m_ExgSlot[x_idx][y_idx] = new_idx;
					}
				}
			}

		}
	}

	// 마우스 락 걸려 있는 부분을 해제한다.
	if (old_idx != -1 && old_idx != -2)
	{
		m_bExch = FALSE;
		m_bWaitingExch = FALSE;
	}
}

void CInterchange::InterChange()
{
	// 아이템 리스트를 바꿔치기 한다.
	CItem *pItem;

	pItem = g_pRoh->m_ExgInven;
	g_pRoh->m_ExgInven = m_ExgOppInven;
	m_ExgOppInven = pItem;

	// 아이템의 슬롯 위치에 대한 지도도 바꾼다.
	int i, j;

	for (i=0; i < EXG_SLOT_X_NUM; i++)
	{
		for (j=0; j < EXG_SLOT_Y_NUM; j++)
		{
			g_pRoh->m_ExgSlot[i][j] = m_ExgOppSlot[i][j];
		}
	}

	// 상대편에게 준 리스트를 지워버린다.
	RemoveAllOppInven();
	//거래가 종료되고 일반 모드로 오픈된 생태로 플래그를 설정.
	m_bExch = FALSE;
	// 컨트들과 플래그 변화.
	SetVisibleMode(FALSE);
}

// 상대편이 거래를 취소했거나 내가 거래를 취소한 경우.
void CInterchange::InterCancel(BOOL bSend)
{
	// 교환창을 일반 모드로 다시 호출한다.
	RemoveAllOppInven();
	m_bExch = FALSE;
	m_bWaitingExch = FALSE;
	// 버튼들 상태 조정.
	SetVisibleMode(FALSE);
}

void CInterchange::RemoveAllMyInven()
{
	if( !m_pNkCha )
		return;

	CItem *pItem = NULL;
	CItem *nextItem = NULL;
	pItem = m_pNkCha->m_ExgInven;

	while (pItem)
	{
		nextItem = pItem->m_Next;
		delete pItem;
		pItem = nextItem;
	}
	m_pNkCha->m_ExgInven = NULL;

	int x_idx, y_idx;

	for (x_idx = 0; x_idx < EXG_SLOT_X_NUM; x_idx++)
	{
		for (y_idx = 0; y_idx < EXG_SLOT_Y_NUM; y_idx++)
		{
			m_pNkCha->m_ExgSlot[x_idx][y_idx] = -1;
		}
	}

}

HRESULT CInterchange::PopupMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, int nMsg)
{
	if(nMsg == 1)
	{
		int nVnum = GetExgOppSlotVnum(m_cur_click_slot_x, m_cur_click_slot_y);

		if (nVnum >= 0)
		{
			sprintf(m_MsgBuf, "ichange %d\n", nVnum);
			SendMsg();
			RemoveAllOppInven();

			// 아이템 이동을 못하게 하기 위해서.
			m_bWaitingExch = TRUE;
		}
	}
	return 0L;
}