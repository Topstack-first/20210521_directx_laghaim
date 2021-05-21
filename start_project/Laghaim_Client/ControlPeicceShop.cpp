#include "stdafx.h"
#include <windows.h>
#include <stdio.h>
#include "main.h"
#include "ddutil.h"
#include "ItemRender.h"
#include "ItemProto.h"
#include "Nk2DFrame.h"
#include "tcpipcon.h"
#include "AlphaPopup.h"
#include "ControlBottom.h"
#include "ControlInven.h"
#include "BasicButton.h"
#include "textoutbox.h"
#include "SpecialItem.h"
#include "controlpeiceshop.h"
#include "NkMob.h"
#include "NkPet.h"
#include "NkMyPet.h"
#include "g_stringmanager.h"

extern CMyD3DApplication	*pCMyApp;
extern CTcpIpConnection		*g_pTcpIp;
extern CDisplay				*g_pDisplay;
extern CNk2DFrame			*g_pNk2DFrame;
extern CItemList g_ItemList;
extern CLolos	*g_ItemLolos[];

int g_shop_item[3][7][8] =
{
	{
		// 기본형.
		{5, 576, 582, 583, 584, 585, -1, -1},	// 뿔
		{6, 586, 587, 588, 589, 590, 591, -1},	// 머리
		{4, 592, 593, 594, 595, -1, -1, -1},	// 몸통
		{6, 596, 597, 598, 599, 600, 601, -1},	// 날개
		{3, 602, 603, 604, -1, -1, -1, -1},		// 다리
		{6, 605, 606, 607, 608, 609, 610, -1},	// 꼬리
		{1, 619, -1, -1, -1, -1, -1, -1}		// 아이템.
	}
	,{ // 방어형
		{2, 1460, 1466, -1, -1, -1, -1, -1},	// 뿔
		{2, 1456, 1462, -1, -1, -1, -1, -1},	// 머리
		{2, 1457, 1463, -1, -1, -1, -1, -1},	// 몸통
		{2, 1458, 1464, -1, -1, -1, -1, -1},	// 날개
		{2, 1455, 1461, -1, -1, -1, -1, -1},	// 다리
		{2, 1459, 1465, -1, -1, -1, -1, -1},	// 꼬리
		{1, 619, -1, -1, -1, -1, -1, -1}	// 아이템.
	}
	,{ // 공격형
		{2, 1447, 1453, -1, -1, -1, -1, -1},	// 뿔
		{2, 1443, 1449, -1, -1, -1, -1, -1},	// 머리
		{2, 1444, 1450, -1, -1, -1, -1, -1},	// 몸통
		{2, 1445, 1451, -1, -1, -1, -1, -1},	// 날개
		{2, 1442, 1448, -1, -1, -1, -1, -1},	// 다리
		{2, 1446, 1452, -1, -1, -1, -1, -1},	// 꼬리
		{1, 619, -1, -1, -1, -1, -1, -1}	// 아이템.
	}
};

CControlPeiceShop::CControlPeiceShop()
	: m_ScreenWidth(0)
	, m_ScreenHeight(0)
	, m_iMainX(0)
	, m_iMainY(0)
	, m_shopslotstart_x(0)
	, m_shopslotstart_y(0)
{
	memset(m_MsgBuf, 0, sizeof(m_MsgBuf));
	memset(m_Slot, 0, sizeof(m_Slot));

	m_pNkCha = NULL;
	m_pSurfaceBack = NULL;
	m_pMoneyText   = NULL;
	m_pBtn_Close   = NULL;
	m_pBtnTail = NULL;
	m_pBtnLeg = NULL;
	m_pBtnWing = NULL;
	m_pBtnHead = NULL;
	m_pBtnBody = NULL;
	m_pBtnHorn = NULL;
	m_pBtnItem = NULL;
	m_nCurPackNum = 0;

	for (int i=0; i < PETSHOP_PACK_NUM+1; i++)
		m_SellItem[i] = NULL;

	m_pOverItem = NULL;
	m_iCurExp = -1;
	m_pPieceNameBox = NULL;
}

CControlPeiceShop::~CControlPeiceShop()
{
	DeleteRes();
}


void CControlPeiceShop::SetNkCha(CNkCharacter * pNkCha)
{
	if(pNkCha)
		m_pNkCha = pNkCha;
}

void CControlPeiceShop::SetMyPetExp(int a)
{
	if( a <= 0 )
		a = 0;

	m_iCurExp = a;

	char tmp[20];
	if(m_pMoneyText)
	{
		m_pMoneyText->SetString(itoa(a, tmp, 10));
		m_pMoneyText->SetPos((m_pMoneyText->m_PosX+84/2)-m_pMoneyText->m_Width/2, m_pMoneyText->m_PosY);
	}
}

void CControlPeiceShop::SetPieceName()
{
	if (m_pPieceNameBox && pCMyApp->m_pMyPet)
	{
		if(pCMyApp->m_pMyPet->GetPieceName(m_nCurPackNum))
			m_pPieceNameBox->SetString(pCMyApp->m_pMyPet->GetPieceName(m_nCurPackNum));
		else
		{
			if (m_nCurPackNum == 0)
				//-- IDS_PEICESHOP_NOTEXIST : 뿔없음
				m_pPieceNameBox->SetString((char*)G_STRING(IDS_PEICESHOP_NOTEXIST));
			else
				m_pPieceNameBox->SetString(".");
		}

		int w = m_pPieceNameBox->GetBoxWidth();
		m_pPieceNameBox->SetPos((m_iMainX+73+(84/2)-(w/2)), m_pPieceNameBox->m_PosY);
	}
}

void CControlPeiceShop::RestroeStr()
{
	if(m_iCurExp < 0)
		return ;

	char tmp[20];

	if(m_pMoneyText)
	{
		m_pMoneyText->SetString(itoa(m_iCurExp, tmp, 10));
		m_pMoneyText->SetPos((m_pMoneyText->m_PosX+84/2)-m_pMoneyText->m_Width/2, m_pMoneyText->m_PosY);
	}

	if (m_pPieceNameBox)
		SetPieceName();
}

void CControlPeiceShop::LoadRes()
{
	if(!g_pDisplay || !g_pNk2DFrame->GetItemRender())
		return;

	RECT rc;
	HWND hWnd =  g_pDisplay->GetHWnd();
	GetClientRect(hWnd, &rc);
	m_ScreenWidth = rc.right;
	m_ScreenHeight = rc.bottom;

	int close_btn_x, close_btn_y;
	int btn_left_top_x, btn_left_top_y;
	int text_x, text_y;
	int text_cx = 84;

	// 해상도
	if(m_ScreenWidth==640 && m_ScreenHeight==480)
	{
		g_pDisplay->CreateSurfaceFromBitmap(&m_pSurfaceBack, "interface/pieceshop/pieceshop.bmp");
		m_iMainX = 148;
		m_iMainY = 0;
		close_btn_x = m_iMainX+172;
		close_btn_y = m_iMainY+338;
		text_x = m_iMainX+72;
		text_y = m_iMainY+376;

		btn_left_top_x = m_iMainX + 30;
		btn_left_top_y = m_iMainY + 66;
		m_shopslotstart_x = m_iMainX + 29;
		m_shopslotstart_y = m_iMainY + 90;

	}
	else if(m_ScreenWidth==800 && m_ScreenHeight==600)
	{
		g_pDisplay->CreateSurfaceFromBitmap(&m_pSurfaceBack, "interface/800x600/pieceshop.bmp");
		m_iMainX = 308;
		m_iMainY = 0;
		close_btn_x = m_iMainX+172;
		close_btn_y = m_iMainY+458;
		text_x = m_iMainX+72;
		text_y = m_iMainY+431;

		btn_left_top_x = m_iMainX + 30;
		btn_left_top_y = m_iMainY + 103;
		m_shopslotstart_x = m_iMainX + 29;
		m_shopslotstart_y = m_iMainY + 128;

	}
	else if(m_ScreenWidth==1024 && m_ScreenHeight==768)
	{
		g_pDisplay->CreateSurfaceFromBitmap(&m_pSurfaceBack, "interface/1024x768/pieceshop.bmp");
		m_iMainX = 532;
		m_iMainY = 0;
		close_btn_x = m_iMainX+159;
		close_btn_y = m_iMainY+540;
		text_x = m_iMainX+72;
		text_y = m_iMainY+514;

		btn_left_top_x = m_iMainX + 30;
		btn_left_top_y = m_iMainY + 185;
		m_shopslotstart_x = m_iMainX + 29;
		m_shopslotstart_y = m_iMainY + 210;
	}

	//-------------------------------------------------------------------
	// BUTTON
	m_pBtn_Close = new CBasicButton();
	if( m_pBtn_Close )
	{
		m_pBtn_Close->SetFileName("btn_close");
		m_pBtn_Close->LoadRes();
		m_pBtn_Close->PosX = close_btn_x;
		m_pBtn_Close->PosY = close_btn_y;
	}

	m_pBtnHorn = new CBasicButton();
	if( m_pBtnHorn  )
	{
		m_pBtnHorn->SetFileName("pieceshop/horn");
		m_pBtnHorn->LoadRes();
		m_pBtnHorn->PosX = btn_left_top_x ;
		m_pBtnHorn->PosY = btn_left_top_y;
	}

	m_pBtnHead = new CBasicButton();
	if( m_pBtnHead  )
	{
		m_pBtnHead->SetFileName("pieceshop/head");
		m_pBtnHead->LoadRes();
		m_pBtnHead->PosX = btn_left_top_x + 23;
		m_pBtnHead->PosY = btn_left_top_y;
	}

	m_pBtnBody = new CBasicButton();
	if( m_pBtnBody  )
	{
		m_pBtnBody->SetFileName("pieceshop/body");
		m_pBtnBody->LoadRes();
		m_pBtnBody->PosX = btn_left_top_x + 23*2;
		m_pBtnBody->PosY = btn_left_top_y;
	}

	m_pBtnLeg = new CBasicButton();
	if( m_pBtnLeg )
	{
		m_pBtnLeg->SetFileName("pieceshop/leg");
		m_pBtnLeg->LoadRes();
		m_pBtnLeg->PosX = btn_left_top_x + 23*3;
		m_pBtnLeg->PosY = btn_left_top_y;
	}

	m_pBtnWing = new CBasicButton();
	if( m_pBtnWing )
	{
		m_pBtnWing->SetFileName("pieceshop/wing");
		m_pBtnWing->LoadRes();
		m_pBtnWing->PosX = btn_left_top_x + 23*4;
		m_pBtnWing->PosY = btn_left_top_y;
	}

	m_pBtnTail = new CBasicButton();
	if( m_pBtnTail )
	{
		m_pBtnTail->SetFileName("pieceshop/tail");
		m_pBtnTail->LoadRes();
		m_pBtnTail->PosX = btn_left_top_x + 23*5 ;
		m_pBtnTail->PosY = btn_left_top_y;
	}

	m_pBtnItem = new CBasicButton();
	if( m_pBtnItem )
	{
		m_pBtnItem->SetFileName("pieceshop/item");
		m_pBtnItem->LoadRes();
		m_pBtnItem->PosX = btn_left_top_x + 23*6 ;
		m_pBtnItem->PosY = btn_left_top_y;
	}

	m_pPieceNameBox = new CTextOutBox();
	if( m_pPieceNameBox )
	{
		m_pPieceNameBox->Init(12, RGB(255, 210, 0),0, 0);
		m_pPieceNameBox->m_PosX = text_x;
		m_pPieceNameBox->m_PosY = text_y;
	}

//	LoadShopItems(); // 진화 작업을 위해 아이템 등록 타이밍을 인터페이스 열때로 옮김.
	// string restore
	RestroeStr();
}

void CControlPeiceShop::DeleteRes()
{
	RemoveShopItems();
	SAFE_DELETE(m_pSurfaceBack);
	SAFE_DELETE(m_pMoneyText);
	SAFE_DELETE(m_pPieceNameBox);
	SAFE_DELETE(m_pBtn_Close);
	SAFE_DELETE(m_pBtnTail);
	SAFE_DELETE(m_pBtnWing);
	SAFE_DELETE(m_pBtnLeg);
	SAFE_DELETE(m_pBtnHead);
	SAFE_DELETE(m_pBtnBody);
	SAFE_DELETE(m_pBtnHorn);
	SAFE_DELETE(m_pBtnItem);
}

void CControlPeiceShop::LoadShopItems()
{
	RemoveShopItems();

	for (int i=0; i < PET_PIECE_NUM+1; i++)
		AddItemsToShop(i);
}

#define GET_PETSLOT_X(point_x)		((point_x - m_shopslotstart_x) / PETSHOP_SLOT_WIDTH)
#define GET_PETSLOT_Y(point_y)		((point_y - m_shopslotstart_y) / PETSHOP_SLOT_HEIGHT)

CItem *CControlPeiceShop::Draw()
{
	if( !g_pDisplay )
		return NULL;

	static int count = 1;

	if (count)
	{
		count = 0;
		LoadShopItems();
	}
	g_pDisplay->Blt(m_iMainX, m_iMainY, m_pSurfaceBack);
	if( m_pPieceNameBox )	m_pPieceNameBox->Draw();
	if( m_pBtn_Close )		m_pBtn_Close->Draw();
	if( m_pBtnHorn )		m_pBtnHorn->Draw();
	if( m_pBtnHead )		m_pBtnHead->Draw();
	if( m_pBtnBody )		m_pBtnBody->Draw();
	if( m_pBtnLeg )			m_pBtnLeg->Draw();
	if( m_pBtnWing )		m_pBtnWing->Draw();
	if( m_pBtnTail )		m_pBtnTail->Draw();
	if( m_pBtnItem )		m_pBtnItem->Draw();

	// Display the content of shop
	POINT point;
	GetCursorPos(&point);
	ScreenToClient(g_pDisplay->GetHWnd(), &point);

	//CItem *pOverItem = NULL;
	m_pOverItem = NULL;

	int slot_x_num, slot_y_num;
	if (point.x >= m_shopslotstart_x && point.y >= m_shopslotstart_y)
	{
		slot_x_num = GET_PETSLOT_X(point.x);
		slot_y_num = GET_PETSLOT_Y(point.y);
	}
	else
	{
		slot_x_num = -1;
		slot_y_num = -1;
	}

	int vnum = GetSlotVnum(slot_x_num, slot_y_num);

	if( g_pNk2DFrame->GetItemRender() )
	{
		g_pNk2DFrame->GetItemRender()->BeginRenderLolo();		// 중요.........  꼭 있어야 한다.. m_pItemRender->RenderLolo의 파라미터에 영향을 줌.

		CItem *pItem = m_SellItem[m_nCurPackNum];
		while (pItem)
		{
			if (vnum >= 0 && pItem->m_Vnum == vnum)
			{
				g_pNk2DFrame->GetItemRender()->RenderLolo(m_shopslotstart_x + pItem->m_SlotX * PETSHOP_SLOT_WIDTH + (GET_ITEM_WIDTH(pItem) * PETSHOP_SLOT_WIDTH - ITEM_RENDER_WIDTH) /2,
						m_shopslotstart_y + pItem->m_SlotY * PETSHOP_SLOT_HEIGHT + (GET_ITEM_HEIGHT(pItem) * PETSHOP_SLOT_HEIGHT - ITEM_RENDER_HEIGHT) /2,
						GET_LOLO(pItem), GET_LOLOTEXTR_NUM(pItem), 0x00ffffff, FALSE, -1, -1, TRUE);
				m_pOverItem = pItem;
			}
			else
			{
				g_pNk2DFrame->GetItemRender()->RenderLolo(m_shopslotstart_x + pItem->m_SlotX * PETSHOP_SLOT_WIDTH + (GET_ITEM_WIDTH(pItem) * PETSHOP_SLOT_WIDTH - ITEM_RENDER_WIDTH) /2,
						m_shopslotstart_y + pItem->m_SlotY * PETSHOP_SLOT_HEIGHT + (GET_ITEM_HEIGHT(pItem) * PETSHOP_SLOT_HEIGHT - ITEM_RENDER_HEIGHT) /2,
						GET_LOLO(pItem), GET_LOLOTEXTR_NUM(pItem), 0x00555555, FALSE, -1, -1, TRUE);
			}
			pItem = pItem->m_Next;
		}
		g_pNk2DFrame->GetItemRender()->EndRenderLolo();
	}

	return m_pOverItem;
}



LRESULT CControlPeiceShop::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int x, y;//, i;
	int slot_x_num, slot_y_num, vnum;

	x = LOWORD (lParam);
	y = HIWORD (lParam);

	switch ( msg )
	{
	case WM_LBUTTONDOWN:
		if(IsInside(x, y))
		{
			if( m_pBtn_Close )	m_pBtn_Close->MsgProc(hWnd, msg, wParam, lParam);
			if( m_pBtnTail )	m_pBtnTail->MsgProc(hWnd, msg, wParam, lParam);
			if( m_pBtnBody )	m_pBtnBody->MsgProc(hWnd, msg, wParam, lParam);
			if( m_pBtnHead )	m_pBtnHead->MsgProc(hWnd, msg, wParam, lParam);
			if( m_pBtnHorn )	m_pBtnHorn->MsgProc(hWnd, msg, wParam, lParam);
			if( m_pBtnLeg )		m_pBtnLeg->MsgProc(hWnd, msg, wParam, lParam);
			if( m_pBtnWing )	m_pBtnWing->MsgProc(hWnd, msg, wParam, lParam);
			if( m_pBtnItem )	m_pBtnItem->MsgProc(hWnd, msg, wParam, lParam);
			return 1;
		}
		else  //kwon
		{
			if( g_pNk2DFrame )	g_pNk2DFrame->ShowPeiceShopWindow(FALSE);
			return 0;
		}

		break;

	case WM_LBUTTONUP:
		if(IsInside(x, y))
		{
			if( m_pBtn_Close )	m_pBtn_Close->MsgProc(hWnd, msg, wParam, lParam);
			if( m_pBtnTail )	m_pBtnTail->MsgProc(hWnd, msg, wParam, lParam);
			if( m_pBtnBody )	m_pBtnBody->MsgProc(hWnd, msg, wParam, lParam);
			if( m_pBtnHead )	m_pBtnHead->MsgProc(hWnd, msg, wParam, lParam);
			if( m_pBtnHorn )	m_pBtnHorn->MsgProc(hWnd, msg, wParam, lParam);
			if( m_pBtnLeg )		m_pBtnLeg->MsgProc(hWnd, msg, wParam, lParam);
			if( m_pBtnWing )	m_pBtnWing->MsgProc(hWnd, msg, wParam, lParam);
			if( m_pBtnItem )	m_pBtnItem->MsgProc(hWnd, msg, wParam, lParam);

			if(m_pBtn_Close && m_pBtn_Close->m_iBtnFlg ==  BTN_ACTION)
			{
				// 창을 닫아라~
				g_pNk2DFrame->ShowPeiceShopWindow(FALSE);
				m_pBtn_Close->m_iBtnFlg = BTN_NORMAL;
			}
			else if (m_pBtnHorn && m_pBtnHorn->m_iBtnFlg == BTN_ACTION)
			{
				m_nCurPackNum = PET_PIECE_HORN;
				SetButtonDown(m_pBtnHorn);
			}
			else if (m_pBtnHead && m_pBtnHead->m_iBtnFlg == BTN_ACTION)
			{
				m_nCurPackNum = PET_PIECE_HEAD;
				SetButtonDown(m_pBtnHead);
			}
			else if (m_pBtnBody && m_pBtnBody->m_iBtnFlg == BTN_ACTION)
			{
				m_nCurPackNum = PET_PIECE_BODY;
				SetButtonDown(m_pBtnBody);
			}
			else if (m_pBtnLeg && m_pBtnLeg->m_iBtnFlg == BTN_ACTION)
			{
				m_nCurPackNum = PET_PIECE_LEGS;
				SetButtonDown(m_pBtnLeg);
			}
			else if (m_pBtnTail && m_pBtnTail->m_iBtnFlg == BTN_ACTION)
			{
				m_nCurPackNum = PET_PIECE_TAIL;
				SetButtonDown(m_pBtnTail);
			}
			else if (m_pBtnWing && m_pBtnWing->m_iBtnFlg == BTN_ACTION)
			{
				m_nCurPackNum = PET_PIECE_WINGS;
				SetButtonDown(m_pBtnWing);
			}
			else if (m_pBtnItem && m_pBtnItem->m_iBtnFlg == BTN_ACTION)
			{
				m_nCurPackNum = PET_PIECE_TAIL+1;
				SetButtonDown(m_pBtnItem);
			}

			return 1;
		}
		break;

	case WM_MOUSEMOVE:
		if(IsInside(x, y))
		{
			if (m_pBtnTail && m_pBtnTail->m_iBtnFlg != BTN_DOWN)
				m_pBtnTail->MsgProc(hWnd, msg, wParam, lParam);
			if(m_pBtnHead && m_pBtnHead->m_iBtnFlg != BTN_DOWN)
				m_pBtnHead->MsgProc(hWnd, msg, wParam, lParam);
			if(m_pBtnHorn && m_pBtnHorn->m_iBtnFlg != BTN_DOWN)
				m_pBtnHorn->MsgProc(hWnd, msg, wParam, lParam);
			if(m_pBtnLeg && m_pBtnLeg->m_iBtnFlg != BTN_DOWN)
				m_pBtnLeg->MsgProc(hWnd, msg, wParam, lParam);
			if(m_pBtnWing && m_pBtnWing->m_iBtnFlg != BTN_DOWN)
				m_pBtnWing->MsgProc(hWnd, msg, wParam, lParam);
			if (m_pBtnBody && m_pBtnBody->m_iBtnFlg != BTN_DOWN)
				m_pBtnBody->MsgProc(hWnd, msg, wParam, lParam);

			return 1;
		}
		break;

	case WM_LBUTTONDBLCLK:
		x = LOWORD (lParam);
		y = HIWORD (lParam);
		if (!IsInside(x, y))
			return 0;

		if (x >= m_shopslotstart_x && y >= m_shopslotstart_y)
		{
			slot_x_num = GET_PETSLOT_X(x);
			slot_y_num = GET_PETSLOT_Y(y);
			vnum = GetSlotVnum(slot_x_num, slot_y_num);
			if (vnum >= 0)
			{
				if (m_nCurPackNum <= PET_PIECE_TAIL)
					sprintf(m_MsgBuf, "p_chg %d\n", vnum);
				else
					sprintf(m_MsgBuf, "p_shop 0 %d\n", vnum);

				if (g_pTcpIp)
					g_pTcpIp->SendNetMessage(m_MsgBuf);
			}
			if( g_pNk2DFrame )
				g_pNk2DFrame->ShowPeiceShopWindow(FALSE);
		}
		return 1;
	}
	return 0;
}


void CControlPeiceShop::SetButtonDown(CBasicButton *pButton)
{
	if( m_pBtnTail )	m_pBtnTail->m_iBtnFlg = BTN_NORMAL;
	if( m_pBtnBody )	m_pBtnBody->m_iBtnFlg = BTN_NORMAL;
	if( m_pBtnHead )	m_pBtnHead->m_iBtnFlg = BTN_NORMAL;
	if( m_pBtnHorn )	m_pBtnHorn->m_iBtnFlg = BTN_NORMAL;
	if( m_pBtnLeg )		m_pBtnLeg->m_iBtnFlg = BTN_NORMAL;
	if( m_pBtnWing )	m_pBtnWing->m_iBtnFlg = BTN_NORMAL;
	if( m_pBtnItem )	m_pBtnItem->m_iBtnFlg = BTN_NORMAL;

	if (m_pBtnHorn == pButton)
		m_pBtnHorn->m_iBtnFlg = BTN_DOWN;

	if (m_pBtnHead == pButton)
		m_pBtnHead->m_iBtnFlg = BTN_DOWN;

	if (m_pBtnBody == pButton)
		m_pBtnBody->m_iBtnFlg = BTN_DOWN;

	if (m_pBtnLeg == pButton)
		m_pBtnLeg->m_iBtnFlg = BTN_DOWN;

	if (m_pBtnWing == pButton)
		m_pBtnWing->m_iBtnFlg = BTN_DOWN;

	if (m_pBtnTail == pButton)
		m_pBtnTail->m_iBtnFlg = BTN_DOWN;

	if (m_pBtnItem == pButton)
		m_pBtnItem->m_iBtnFlg = BTN_DOWN;

	SetPieceName();
}



BOOL CControlPeiceShop::IsInside(int x, int y)
{
	if (x >= (int)(m_iMainX)
			&& x < (int)(m_iMainX + m_pSurfaceBack->GetWidth())
			&& y >= (int)(m_iMainY)
			&& y < (int)(m_iMainY + m_pSurfaceBack->GetHeight()) )
		return TRUE;

	return FALSE;
}


void CControlPeiceShop::AddItemsToShop(int type)
{
	if (type > PET_PIECE_NUM)
		return;

	for ( int i = 1 ; i <= g_shop_item[g_MyPetStatus.m_Class][type][0] ; i++)
		AddItemToShop(type, g_shop_item[g_MyPetStatus.m_Class][type][i]);
}

void CControlPeiceShop::AddItemToShop(int pack_num, int vnum)
{
	if (vnum < 0)
		return;

	int proto_num = g_ItemList.FindItem(vnum);
	CItem *pItem = new CItem;
	if (pItem && proto_num >= 0)
	{

		pItem->m_Index = -1000-vnum;
		pItem->m_Vnum = vnum;
		pItem->m_ProtoNum = proto_num;

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
		pItem->m_Int = GET_ITEM_INT(pItem);
		pItem->m_Dex = GET_ITEM_DEX(pItem);
		pItem->m_Weight = GET_WEIGHT(pItem);

		int x, y;
		if (FindBlankSlot(pack_num, x, y, GET_ITEM_WIDTH(pItem), GET_ITEM_HEIGHT(pItem)))
		{
			pItem->m_SlotX = x;
			pItem->m_SlotY = y;

			pItem->m_Next = m_SellItem[pack_num];
			m_SellItem[pack_num] = pItem;

			for (int x_idx = 0; x_idx < GET_ITEM_WIDTH(pItem); x_idx++)
			{
				for (int y_idx = 0; y_idx < GET_ITEM_HEIGHT(pItem); y_idx++)
					m_Slot[pack_num][x+x_idx][y+y_idx] = vnum;
			}
		}
		else
			delete pItem;
	}
}

BOOL CControlPeiceShop::FindBlankSlot(int pack_num, int &x, int &y, int w, int h)
{
	BOOL success = FALSE;
	for (int x_idx = 0; !success && x_idx+w-1 < PETSHOP_SLOT_X_NUM; x_idx++)
	{
		for (int y_idx = 0; y_idx+h-1 < PETSHOP_SLOT_Y_NUM; y_idx++)
		{
			BOOL found = TRUE;
			for (int x_w = x_idx; found && x_w < x_idx + w; x_w++)
			{
				for (int y_h = y_idx; y_h < y_idx + h; y_h++)
				{
					if (m_Slot[pack_num][x_w][y_h] >= 0)
					{
						found = FALSE;
						break;
					}
				}
			}
			if (found)
			{
				x = x_idx;
				y = y_idx;
				success = TRUE;
				break;
			}
		}
	}
	return success;
}

void CControlPeiceShop::RemoveShopItems()
{
	for (int i = 0; i < PETSHOP_PACK_NUM; i++)
	{
		CItem *nextItem = m_SellItem[i];
		while (nextItem)
		{
			CItem *pItem = nextItem;
			nextItem = pItem->m_Next;
			delete pItem;
		}

		m_SellItem[i] = NULL;
		for (int x = 0; x < PETSHOP_SLOT_X_NUM; x++)
		{
			for (int y = 0; y < PETSHOP_SLOT_Y_NUM; y++)
				m_Slot[i][x][y] = -1;
		}
	}
}

int CControlPeiceShop::GetSlotVnum(int x, int y, int *type, int *shape)
{
	if (x < 0 || x >= PETSHOP_SLOT_X_NUM || y < 0 || y >= PETSHOP_SLOT_Y_NUM)
		return -1;

	int vnum = m_Slot[m_nCurPackNum][x][y];
	if (type && shape)
	{
		*type = -1;
		*shape = -1;

		CItem *pItem = m_SellItem[0];
		while (pItem)
		{
			if (vnum >= 0 && pItem->m_Vnum == vnum)
			{
				*type = GET_TYPE(pItem);
				*shape = GET_SHAPE(pItem);
				break;
			}
			pItem = pItem->m_Next;
		}

	}

	return vnum;
}