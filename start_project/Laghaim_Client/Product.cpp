#include "stdafx.h"
// Interchange.cpp: implementation of the CProduct class.
//
//////////////////////////////////////////////////////////////////////


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
#include "product.h"
#include "textoutbox.h"
#include "SpecialItem.h"
#include "headers.h"
#include "skill.h"
#include "g_stringmanager.h"

extern POINT			g_CursorPoint;

extern int				g_SkillsNum;
extern CRaceSkill		g_RaceSkill[];


#define EXCH_TEXT_SIZE			12
#define EXCH_TEXT_COLOR			RGB(255, 0, 0)
#define EXCH_BAKC_COLOR         RGB(47, 0, 17)

//#define INTERCHANGE_NUM 74
#define INTERCHANGE_NUM 16

/*static int InterChangeVnum [][4] = {
	{829,830,831,832},
	{833,834,835,836},
	{837,838,839,840},
	{841,842,843,844}
};*/


static int InterChangeVnum [16] = {829,830,831,832,833,834,835,836,837,838,841,842,844,936,967,966};

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CProduct::CProduct()
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
	m_pInfo = NULL;
	m_pInfo = new CAlphaPopUp;

	m_pOverItem = NULL;
	Init();
	m_bWaitingExch = FALSE;
	m_ExgOppInven = NULL;
	int i, j;

	for ( i=0; i < EXG_SLOT_Y_NUM; i++)
	{
		for ( j = 0; j < EXG_SLOT_X_NUM; j++)
		{
			m_ExgOppSlot[i][j] = -1;
		}
	}

	m_bShowWindow = FALSE;
	m_dwMouseClickTickTime = timeGetTime();
}

CProduct::~CProduct()
{
	DeleteRes();
	RemoveAllOppInven();

	SAFE_DELETE(m_pInfo);

	m_pNkCha = NULL;
}

void CProduct::RemoveAllOppInven()
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

	for ( i=0; i < EXG_SLOT_Y_NUM; i++)
	{
		for ( j = 0; j < EXG_SLOT_X_NUM; j++)
		{
			m_ExgOppSlot[i][j] = -1;
		}
	}

	m_bShowWindow = FALSE;
}

void CProduct::Init()
{
	m_pInfo->Init(14);
	m_CancelBtn.SetFileName("common/btn_close_01");
}

void CProduct::SetVisibleMode(BOOL bExch)
{
	if (bExch)
	{
		// 상대편 교환창에 바꿀 아이템을 놓는다.
		ShowItemsToChange();
	}
	else
	{
		// 아무것도 하지 않는다.
		RemoveAllOppInven();
	}
}


//--------------------------------------------------------------------------
// Name : ShowItemsToChange()
// Desc : 바꿀 수 있는 아이템을 보여주기 위한 루틴.
//--------------------------------------------------------------------------
void CProduct::ShowItemsToChange()
{
	int i, x_pos;

	// get item info to change
	// make sure that there is only one item in exchange inventory.

	//CItem *pChangeItem = m_pNkCha->m_ExgInven;

	// search old index
	CItem *pItem = NULL;

	int cw, ch;
	int slot_ch = 0;
	int slot_cw = 0;
	cw = ch = 0;

	x_pos = 0;

	RemoveAllOppInven();

	//for (i=0; i < 4; i++) {
	//	for (int j=0; j < 4; j++) {
	for(i=0; i<INTERCHANGE_NUM; i++)
	{
		int w = i % 6;
		int h = i / 6;

		// make dummy items
		pItem = new CItem;
		pItem->m_SlotX = w;
		pItem->m_SlotY = h;
		pItem->m_Index = -1050-(w+h);
		pItem->m_PlusNum = 0;//pChangeItem->m_PlusNum;
		pItem->m_Vnum = InterChangeVnum[i];
		pItem->m_ProtoNum = g_ItemList.FindItem(InterChangeVnum[i]);
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

		//829,830,831,832,833,834,
		//835,836,837,838,839,840,
		//841,842,843,844;
		m_ExgOppSlot[h][w] = InterChangeVnum[i];
	}

	//}
	//}
	m_bShowWindow = TRUE;
}


void CProduct::LoadRes()
{
	if( !g_pDisplay || !g_pNk2DFrame->GetItemRender() || !pCMyApp->GetDevice())
		return;

	m_hWnd =  g_pDisplay->GetHWnd();
	GetClientRect(m_hWnd, &m_ClientRc);

	m_ScreenWidth = g_pNk2DFrame->GetClientWidth();
	m_ScreenHeight = g_pNk2DFrame->GetClientHeight();

	m_CancelBtn.LoadRes();
	SetVisibleMode(m_bShowWindow);
	m_pInfo->LoadRes(m_ScreenWidth, m_ScreenHeight);

	// 금액 변수.

	g_pDisplay->CreateSurfaceFromBitmap (&m_pEnableSlot, "interface/slot/slot_enable.bmp");
	g_pDisplay->CreateSurfaceFromBitmap (&m_pDisableSlot, "interface/slot/slot_disable.bmp");
	g_pDisplay->CreateSurfaceFromBitmap (&m_pExistSlot, "interface/slot/slot_exist.bmp");

	SAFE_DELETE(m_pSurfaceBack);
	g_pDisplay->CreateSurfaceFromBitmap(&m_pSurfaceBack, "interface/interchange/interchange.bmp");

	m_pSurfaceBack->SetColorKey( TRANS_COLOR );
	m_pSurfaceBack->Xpos = g_pNk2DFrame->GetClientWidth() - 291 - m_pSurfaceBack->GetWidth();
	m_pSurfaceBack->Ypos = 0;

	// 상대편 슬롯 창.
	m_opp_slotstart_x = m_pSurfaceBack->Xpos + 33 + 2;
	m_opp_slotstart_y = m_pSurfaceBack->Ypos + 78 + 2;

	// 나의 슬롯창.
	m_my_slotstart_x = m_pSurfaceBack->Xpos + 33 + 2;
	m_my_slotstart_y = m_pSurfaceBack->Ypos + 258 + 2;
	m_CancelBtn.SetPosition(m_pSurfaceBack->Xpos+214, m_pSurfaceBack->Ypos+5);
}

void CProduct::DeleteRes()
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
	m_pInfo->DeleteRes();
}

#define GET_OPP_SLOT_X(point_x)		((point_x - m_opp_slotstart_x) / SLOT_WIDTH)
#define GET_OPP_SLOT_Y(point_y)		((point_y - m_opp_slotstart_y) / SLOT_HEIGHT)

#define GET_MY_SLOT_X(point_x)		((point_x - m_my_slotstart_x) / SLOT_WIDTH)
#define GET_MY_SLOT_Y(point_y)		((point_y - m_my_slotstart_y) / SLOT_HEIGHT)

CItem* CProduct::Draw()
{
	if (g_pDisplay && m_pSurfaceBack)
	{
		POINT point;
		GetCursorPos(&point);
		ScreenToClient(g_pDisplay->GetHWnd(), &point);

		g_pDisplay->Blt(m_pSurfaceBack->Xpos, m_pSurfaceBack->Ypos, m_pSurfaceBack);

		// 버튼들 그리기.
		m_CancelBtn.Draw();

		m_pOverItem = NULL;
		DrawMySlot();
		DrawOppSlot();

		// 마우스가 아이템 위로 올라갔을때의 설명.
		//CMultiText multiText;
		if (m_pNkCha->m_ExtraSlot)
			m_pOverItem = NULL;
		else if (m_pOverItem)
			pCMyApp->m_pMouse->SetMouseType(M_HOLD);
		else if (IsInside(point.x, point.y))
			pCMyApp->m_pMouse->SetMouseType(M_NORMAL);
	}

	return m_pOverItem;
}

void CProduct::DrawMySlot()
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
		if (m_pNkCha->m_ExtraSlot)
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
		int index = m_pNkCha->GetExgSlotIndex(slot_x_num, slot_y_num);

		pItem = m_pNkCha->m_ExgInven;

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
				if (g_pNk2DFrame->IsScroll(pItem) )
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

void CProduct::DrawOppSlot()
{
	int xpos, ypos;
	bool bOver = false;

	if (g_pDisplay && m_pSurfaceBack)
	{
		POINT point;
		GetCursorPos(&point);
		ScreenToClient(g_pDisplay->GetHWnd(), &point);

		// 아이템이 위치한 슬롯 표시
		CItem *pItem = m_ExgOppInven;
		/*while (pItem) {
			rcRect.left = 0;
			rcRect.right = SLOT_WIDTH * GET_ITEM_WIDTH(pItem);
			rcRect.top = 0;
			rcRect.bottom = SLOT_HEIGHT * GET_ITEM_HEIGHT(pItem);
			xpos = m_opp_slotstart_x + (SLOT_WIDTH*pItem->m_SlotX);
			ypos = m_opp_slotstart_y + (SLOT_HEIGHT*pItem->m_SlotY);
			g_pDisplay->Blt( xpos, ypos, m_pExistSlot, &rcRect);

			pItem = pItem->m_Next;
		}*/

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
		//	int t_slot_width, t_item_width, t_item_render_width;
		g_pNk2DFrame->GetItemRender()->BeginRenderLolo();				// 중요....... 중요..
		while (pItem)
		{
			if (pItem->m_Vnum == vnum)
			{
				//			m_opp_slotstart_x = m_opp_slotstart_x;
				//			pItem->m_SlotX = pItem->m_SlotX;
				//			t_slot_width = SLOT_WIDTH;
				//			t_item_width = GET_ITEM_WIDTH(pItem);
				//			t_item_render_width = ITEM_RENDER_WIDTH;

				g_pNk2DFrame->RenderItemInUI( m_opp_slotstart_x + pItem->m_SlotX * SLOT_WIDTH,
											  m_opp_slotstart_y + pItem->m_SlotY * SLOT_HEIGHT,
											  pItem , TRUE, FALSE, FALSE, TRUE );

				//m_pOverItem = pItem;
				CMultiText multiText;

				if(vnum == 723)  // 휴먼소형장비
				{
					multiText.AddString((char*)G_STRING(IDS_PRODUCT_INFO_0), _FCOLOR_YELLOW);
					multiText.AddString((char*)G_STRING(IDS_PRODUCT_INFO_0 +1), _FCOLOR_GREEN);
				}
				else if(vnum == 831)   // 퍼지변환
				{
					multiText.AddString((char*)G_STRING(IDS_PRODUCT_INFO_0 +2), _FCOLOR_YELLOW);
					multiText.AddString((char*)G_STRING(IDS_PRODUCT_INFO_0 +3), _FCOLOR_GREEN);
					multiText.AddString((char*)G_STRING(IDS_PRODUCT_INFO_0 +4), _FCOLOR_GREEN);
				}
				else if(vnum == 830)   // 하드배리어분해
				{
					multiText.AddString((char*)G_STRING(IDS_PRODUCT_INFO_0 +5), _FCOLOR_YELLOW);
					multiText.AddString((char*)G_STRING(IDS_PRODUCT_INFO_0 +6), _FCOLOR_GREEN);
					multiText.AddString((char*)G_STRING(IDS_PRODUCT_INFO_0 +7), _FCOLOR_GREEN);
				}
				else if(vnum == 829)   // 레이다
				{
					multiText.AddString((char*)G_STRING(IDS_PRODUCT_INFO_0 +8), _FCOLOR_YELLOW);
					multiText.AddString((char*)G_STRING(IDS_PRODUCT_INFO_0 +9), _FCOLOR_GREEN);
				}
				else if(vnum == 832)   // 볼트저항
				{
					multiText.AddString((char*)G_STRING(IDS_PRODUCT_INFO_0 +10), _FCOLOR_YELLOW);
					multiText.AddString((char*)G_STRING(IDS_PRODUCT_INFO_0 +11), _FCOLOR_GREEN);
					multiText.AddString((char*)G_STRING(IDS_PRODUCT_INFO_0 +12), _FCOLOR_GREEN);
				}
				else if(vnum == 833)   // 볼케닉저항
				{
					multiText.AddString((char*)G_STRING(IDS_PRODUCT_INFO_0+13), _FCOLOR_YELLOW);
					multiText.AddString((char*)G_STRING(IDS_PRODUCT_INFO_0+14), _FCOLOR_GREEN);
					multiText.AddString((char*)G_STRING(IDS_PRODUCT_INFO_0+15), _FCOLOR_GREEN);
				}
				else if(vnum == 834)   // 석화저항
				{
					multiText.AddString((char*)G_STRING(IDS_PRODUCT_INFO_0+16), _FCOLOR_YELLOW);
					multiText.AddString((char*)G_STRING(IDS_PRODUCT_INFO_0+17), _FCOLOR_GREEN);
					multiText.AddString((char*)G_STRING(IDS_PRODUCT_INFO_0+18), _FCOLOR_GREEN);
				}
				else if(vnum == 835)   // 콜드저항
				{
					multiText.AddString((char*)G_STRING(IDS_PRODUCT_INFO_0+19), _FCOLOR_YELLOW);
					multiText.AddString((char*)G_STRING(IDS_PRODUCT_INFO_0+20), _FCOLOR_GREEN);
					multiText.AddString((char*)G_STRING(IDS_PRODUCT_INFO_0+21), _FCOLOR_GREEN);
				}
				else if(vnum == 836)   // 전기충격
				{
					multiText.AddString((char*)G_STRING(IDS_PRODUCT_INFO_0+22), _FCOLOR_YELLOW);
					multiText.AddString((char*)G_STRING(IDS_PRODUCT_INFO_0+23), _FCOLOR_GREEN);
				}
				else if(vnum == 837)   // 전기흡수
				{
					multiText.AddString((char*)G_STRING(IDS_PRODUCT_INFO_0+24), _FCOLOR_YELLOW);
					multiText.AddString((char*)G_STRING(IDS_PRODUCT_INFO_0+25), _FCOLOR_GREEN);
					multiText.AddString((char*)G_STRING(IDS_PRODUCT_INFO_0+26), _FCOLOR_GREEN);
				}
				else if(vnum == 838)   // 전기증폭
				{
					multiText.AddString((char*)G_STRING(IDS_PRODUCT_INFO_0+27), _FCOLOR_YELLOW);
					multiText.AddString((char*)G_STRING(IDS_PRODUCT_INFO_0+28), _FCOLOR_GREEN);
					multiText.AddString((char*)G_STRING(IDS_PRODUCT_INFO_0+29), _FCOLOR_GREEN);
				}/*else if(vnum == 839){ // 원자이동
					multiText.AddString("원자이동", _FCOLOR_YELLOW);
					multiText.AddString("조립키트+원자변환(2)+셀캡슐(2)+", _FCOLOR_GREEN);
					multiText.AddString("유전자캡슐(1)", _FCOLOR_GREEN);
				}else if(vnum == 840){ // 공간왜곡
					multiText.AddString("공간왜곡", _FCOLOR_YELLOW);
					multiText.AddString("조립키트+레이다(5)+제어키트(5)+", _FCOLOR_GREEN);
					multiText.AddString("셀캡슐(5)+유전자캡슐(5)", _FCOLOR_GREEN);
				}*/else if(vnum == 841)  // 공간제어기
				{
					multiText.AddString((char*)G_STRING(IDS_PRODUCT_INFO_0+30), _FCOLOR_YELLOW);
					multiText.AddString((char*)G_STRING(IDS_PRODUCT_INFO_0+31), _FCOLOR_GREEN);
				}
				else if(vnum == 842)   // 통합저항
				{
					multiText.AddString((char*)G_STRING(IDS_PRODUCT_INFO_0+32), _FCOLOR_YELLOW);
					multiText.AddString((char*)G_STRING(IDS_PRODUCT_INFO_0+33), _FCOLOR_GREEN);
					multiText.AddString((char*)G_STRING(IDS_PRODUCT_INFO_0+34), _FCOLOR_GREEN);
					multiText.AddString((char*)G_STRING(IDS_PRODUCT_INFO_0+35), _FCOLOR_GREEN);
				}/*else if(vnum == 843){ // 빔샤벨
					multiText.AddString("빔샤벨", _FCOLOR_YELLOW);
				}*/else if(vnum == 844)  // 통합변환
				{
					multiText.AddString((char*)G_STRING(IDS_PRODUCT_INFO_0+36), _FCOLOR_YELLOW);
					multiText.AddString((char*)G_STRING(IDS_PRODUCT_INFO_0+37), _FCOLOR_GREEN);
					multiText.AddString((char*)G_STRING(IDS_PRODUCT_INFO_0+38), _FCOLOR_GREEN);
				}
				else if(vnum==936)
				{
					//-- IDS_PRODUCT_RECEPTION_MACHINE : 고출력임무수신기
					multiText.AddString((char*)G_STRING(IDS_PRODUCT_RECEPTION_MACHINE), _FCOLOR_YELLOW);
					//-- IDS_PRODUCT_KIT_INFO : 조립키트+제어키트(2)
					multiText.AddString((char*)G_STRING(IDS_PRODUCT_KIT_INFO), _FCOLOR_GREEN);
				}
				else if(vnum==967)
				{
					//-- IDS_PET_EARTH_RING : + 제어키트 1개 + 셀캡슐 1개
					multiText.AddString((char*)G_STRING(IDS_PET_EARTH_RING), _FCOLOR_YELLOW);
					//-- IDS_PET_EARTH_RING_ITEM1 : 조립키트 1개 + 프로세서키트 4개
					multiText.AddString((char*)G_STRING(IDS_PET_EARTH_RING_ITEM1), _FCOLOR_GREEN);
					//-- IDS_PET_EARTH_RING_ITEM2 : + 제어키트 1개 + 셀캡슐 1개
					multiText.AddString((char*)G_STRING(IDS_PET_EARTH_RING_ITEM2), _FCOLOR_GREEN);
				}
				else if(vnum==966)
				{
					//-- IDS_PET_SKY_RING : 라비용 천공의 목걸이
					multiText.AddString((char*)G_STRING(IDS_PET_SKY_RING), _FCOLOR_YELLOW);
					//-- IDS_PET_SKY_RING_ITEM1 : 조립키트 1개 + 프로세서키트 4개
					multiText.AddString((char*)G_STRING(IDS_PET_SKY_RING_ITEM1), _FCOLOR_GREEN);
					//-- IDS_PET_SKY_RING_ITEM2 : + 제어키트 1개 + 셀캡슐 1개
					multiText.AddString((char*)G_STRING(IDS_PET_SKY_RING_ITEM2), _FCOLOR_GREEN);
					//-- IDS_PET_SKY_RING_ITEM3 : + 항체캡슐 1개 + 유전자캡슐 1개
					multiText.AddString((char*)G_STRING(IDS_PET_SKY_RING_ITEM3), _FCOLOR_GREEN);
				}
				m_pInfo->PutString(&multiText);
				bOver = true;
				//m_pInfo->Draw(point.x, point.y);
			}
			else
			{
				DWORD ambient = 0x00555555;
				if (g_pNk2DFrame->IsScroll(pItem) )
					ambient = 0x00cccccc;
				else if (pItem->m_PlusNum > 0)
					ambient = UPGRADE_ITEM_COLOR;

				g_pNk2DFrame->RenderItemInUI( m_opp_slotstart_x + pItem->m_SlotX * SLOT_WIDTH,
											  m_opp_slotstart_y + pItem->m_SlotY * SLOT_HEIGHT,
											  pItem , FALSE, FALSE, FALSE, TRUE );


				xpos = m_opp_slotstart_x + pItem->m_SlotX * SLOT_WIDTH + (GET_ITEM_WIDTH(pItem) * SLOT_WIDTH - ITEM_RENDER_WIDTH) /2;
				ypos = m_opp_slotstart_y + pItem->m_SlotY * SLOT_HEIGHT + (GET_ITEM_HEIGHT(pItem) * SLOT_HEIGHT - ITEM_RENDER_HEIGHT) /2;
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
		if(bOver)
			m_pInfo->Draw(point.x, point.y);
	}

}

BOOL CProduct::IsInside(int x, int y)
{
	if( !m_pSurfaceBack )
		return FALSE;

	return m_pSurfaceBack->IsIn(x, y);
}

BOOL CProduct::IsPlayArea(int x, int y)
{
	if( !IsInside(x,y) && !g_pNk2DFrame->IsInPopup(x,y) )
		return true;

	return false;
}

void CProduct::DrawItemInfo()
{
}


LRESULT CProduct::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static char commOutBuf[512];	// 나중에 변수 다른 곳에 설정.
	int x, y;
	int slot_x_num, slot_y_num, vnum;
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

			if (vnum >= 0)
			{

				if(m_pNkCha->m_Race !=3)
				{
					//-- IDS_PRODUCT_ONLY_HUMAN : 휴먼종족만이 조립할 수 있습니다.
					g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_PRODUCT_ONLY_HUMAN), TYPE_NOR_OK, 1 );
					return 1;
				}
				else
				{
					int item_level = -1;

					CItem *pItem = m_ExgOppInven;
					while (pItem)
					{
						if(pItem->m_Vnum == vnum)
						{
							item_level = pItem->m_MinLevel;
							if(item_level == 0) item_level = 1;
							break;
						}
						pItem = pItem->m_Next;
					}

					if(g_ChaSkill.m_SkillLevel[9]< item_level)
					{
						//-- IDS_PRODUCT_NOTENOUGH_LEV
						g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_PRODUCT_NOTENOUGH_LEV), TYPE_NOR_OK, 1 );
						return 1;
					}

					sprintf(m_MsgBuf, "product %d\n", vnum);//조립 메시지 보내기.
					SendMsg();
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
			g_pNk2DFrame->ShowProductWindow(FALSE);
			m_CancelBtn.SetState(BTN_NORMAL);
		}
		return 1;
		break;

	case WM_RBUTTONDOWN:
		x = LOWORD (lParam);
		y = HIWORD (lParam);
		if (IsPlayArea(x, y))
		{
			g_pNk2DFrame->ShowProductWindow(FALSE);
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
				if (m_pNkCha->m_ExtraSlot)
				{
					//if (m_bExch)
					//	return 1;
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
					}
				}
				else
				{
					// 아이템을 빼는 것.
					if (!m_bWaitingExch)
					{
						if (m_pNkCha->AddToExgInven((x-m_my_slotstart_x) / SLOT_WIDTH, (y-m_my_slotstart_y) / SLOT_HEIGHT))
						{
							sprintf(m_MsgBuf, "exch inven %d %d\n", (x-m_my_slotstart_x) / SLOT_WIDTH, (y-m_my_slotstart_y) / SLOT_HEIGHT);
							SendMsg();
						}
					}
				}
			}
		}
		else if (IsPlayArea(x, y))
		{
			g_pNk2DFrame->ShowProductWindow(FALSE);	// 이 내부에서 자동적으로 취소 메시지가 간다.

		}
		else
			return 0;

		return 1;
		break;
	}

	return 0;
}

void CProduct::SetNkCha(CNkCharacter *pNkCha)
{
	if( pNkCha )
		m_pNkCha = pNkCha;
}

void CProduct::SendMsg()
{
	if (g_pTcpIp)
		g_pTcpIp->SendNetMessage(m_MsgBuf);
}

// 거래를 활성/비활성 상태로 만든다.
// 거래중일때 비활성 상태로 변하면 거래를 중지한다.
void CProduct::ActivateExch(BOOL bExch)
{
	if (bExch == FALSE)
	{
		InterCancel(TRUE);
		return;
	}
	else
	{
		SetVisibleMode(bExch);
	}
}

int	CProduct::GetExgOppSlotVnum(int x, int y)
{
	if ( x < 0 || x >= EXG_SLOT_X_NUM || y < 0 || y >= EXG_SLOT_Y_NUM )
		return -1;

	//829,830,831,832,833,834,
	//835,836,837,838,839,840,
	//841,842,843,844;
	int vnum = m_ExgOppSlot[y][x];

	return vnum;
}

// 상대편이 거래를 취소했거나 내가 거래를 취소한 경우.
void CProduct::InterCancel(BOOL bSend)
{
	// 교환창을 일반 모드로 다시 호출한다.
	RemoveAllOppInven();

	// 버튼들 상태 조정.
	SetVisibleMode(FALSE);
}

///2003 조립 성공. 부품 없애고 완성품만 교환창에...
void CProduct::AssemblySuccess(int vnum, int index, int plus, int special)
{
	if (vnum == -1)
	{
		m_bWaitingExch = FALSE;
		return;
	}

	int x_idx, y_idx;

	if(vnum == 0)
	{

		CItem *pItem = NULL;
		CItem *nextItem = NULL;

		pItem = g_pRoh->m_ExgInven;

		while (pItem)
		{
			nextItem = pItem->m_Next;
			delete pItem;
			pItem = nextItem;
		}
		g_pRoh->m_ExgInven = NULL;

		// Clear
		for (x_idx = 0; x_idx < EXG_SLOT_Y_NUM; x_idx++)
		{
			for (y_idx = 0; y_idx < EXG_SLOT_X_NUM; y_idx++)
			{
				g_pRoh->m_ExgSlot[y_idx][x_idx] = -1;
			}
		}

		m_bWaitingExch = FALSE;
		return;
	}


//	int x_idx, y_idx;

	g_pRoh->m_ExgInven = new CItem;

	g_pRoh->m_ExgInven->m_Index = index;
	g_pRoh->m_ExgInven->m_SlotX = 0;
	g_pRoh->m_ExgInven->m_SlotY = 0;
	g_pRoh->m_ExgInven->m_Vnum = vnum;
	g_pRoh->m_ExgInven->m_ProtoNum = g_ItemList.FindItem(vnum);

	g_pRoh->m_ExgInven->m_PlusNum = plus;
	g_pRoh->m_ExgInven->m_Special = special;

	g_pRoh->m_ExgInven->m_Num0 = GET_ITEM_NUM0(m_pNkCha->m_ExgInven);
	g_pRoh->m_ExgInven->m_Num1 = GET_ITEM_NUM1(m_pNkCha->m_ExgInven);
	g_pRoh->m_ExgInven->m_Num2 = GET_ITEM_NUM2(m_pNkCha->m_ExgInven);
	g_pRoh->m_ExgInven->m_Num3 = GET_ITEM_NUM3(m_pNkCha->m_ExgInven);
	g_pRoh->m_ExgInven->m_Num4 = GET_ITEM_NUM4(m_pNkCha->m_ExgInven);
	g_pRoh->m_ExgInven->m_Num5 = GET_ITEM_NUM5(m_pNkCha->m_ExgInven);
	g_pRoh->m_ExgInven->m_Num6 = GET_ITEM_NUM6(m_pNkCha->m_ExgInven);
	g_pRoh->m_ExgInven->m_Num7 = GET_ITEM_NUM7(m_pNkCha->m_ExgInven);
	g_pRoh->m_ExgInven->m_Num8 = GET_ITEM_NUM8(m_pNkCha->m_ExgInven);
	g_pRoh->m_ExgInven->m_Num9 = GET_ITEM_NUM9(m_pNkCha->m_ExgInven);
	g_pRoh->m_ExgInven->m_MinLevel = GET_ITEM_LEVEL(m_pNkCha->m_ExgInven);
	g_pRoh->m_ExgInven->m_MinLevel_Org = g_pRoh->m_ExgInven->m_MinLevel;
	g_pRoh->m_ExgInven->m_Int = GET_ITEM_INT(m_pNkCha->m_ExgInven);
	g_pRoh->m_ExgInven->m_Dex = GET_ITEM_DEX(m_pNkCha->m_ExgInven);
	g_pRoh->m_ExgInven->m_Weight = GET_WEIGHT(m_pNkCha->m_ExgInven);

	g_pRoh->m_ExgInven->m_Next= NULL;

	// Clear
	for (x_idx = 0; x_idx < EXG_SLOT_Y_NUM; x_idx++)
	{
		for (y_idx = 0; y_idx < EXG_SLOT_X_NUM; y_idx++)
		{
			g_pRoh->m_ExgSlot[y_idx][x_idx] = -1;
		}
	}
	// Add
	for (x_idx = 0; x_idx < GET_ITEM_WIDTH(g_pRoh->m_ExgInven); x_idx++)
	{
		for (y_idx = 0; y_idx < GET_ITEM_HEIGHT(g_pRoh->m_ExgInven); y_idx++)
		{
			g_pRoh->m_ExgSlot[x_idx][y_idx] = index;
		}
	}

	m_bWaitingExch = FALSE;

}

BOOL CProduct::AssemblyFail(BOOL bfail)
{
	m_bWaitingExch = FALSE;
	if(bfail)
		//-- IDS_PRODUCT_CONFIRM : '부품' 과 '키트'를 확인해 주세요.
		g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_PRODUCT_CONFIRM), TYPE_NOR_OK, 1 );

	return bfail;
}



