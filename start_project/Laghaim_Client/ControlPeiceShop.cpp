#include "stdafx.h"


#include "main.h"
#include "Mouse.h"
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
#include "string_res.h"

int g_shop_item[4][7][8] =
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
	,

	{
		// 방어형
		{3, 1460, 1466, 2766, -1, -1, -1, -1},	// 뿔
		{3, 1456, 1462, 2762, -1, -1, -1, -1},	// 머리
		{3, 1457, 1463, 2763, -1, -1, -1, -1},	// 몸통
		{3, 1458, 1464, 2764, -1, -1, -1, -1},	// 날개
		{3, 1455, 1461, 2761, -1, -1, -1, -1},	// 다리
		{3, 1459, 1465, 2765, -1, -1, -1, -1},	// 꼬리
		{1, 619, -1, -1, -1, -1, -1, -1}	// 아이템.
	}
	,
	{ // 공격형
		{3, 1447, 1453, 2760, -1, -1, -1, -1},	// 뿔
		{3, 1443, 1449, 2756, -1, -1, -1, -1},	// 머리
		{3, 1444, 1450, 2757, -1, -1, -1, -1},	// 몸통
		{3, 1445, 1451, 2758, -1, -1, -1, -1},	// 날개
		{3, 1442, 1448, 2755, -1, -1, -1, -1},	// 다리
		{3, 1446, 1452, 2759, -1, -1, -1, -1},	// 꼬리
		{1, 619, -1, -1, -1, -1, -1, -1}	// 아이템.
	}
	,
	{ // 공격형
		{0, -1, -1, -1, -1, -1, -1, -1},	// 뿔
		{0, -1, -1, -1, -1, -1, -1, -1},	// 머리
		{0, -1, -1, -1, -1, -1, -1, -1},	// 몸통
		{0, -1, -1, -1, -1, -1, -1, -1},	// 날개
		{0, -1, -1, -1, -1, -1, -1, -1},	// 다리
		{0, -1, -1, -1, -1, -1, -1, -1},	// 꼬리
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

	m_pNkCha = NULL;
}


void CControlPeiceShop::SetNkCha(CNkCharacter * pNkCha)
{
	if(pNkCha)
		m_pNkCha = pNkCha;
}

void CControlPeiceShop::SetMyPetExp(__int64 a)
{
	if( a <= 0 )
		a = 0;

	m_iCurExp = a;

	if(m_pMoneyText)
	{
		m_pMoneyText->SetString(a, true);
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
		m_pPieceNameBox->SetPos((m_pSurfaceBack->Xpos+73+(84/2)-(w/2)), m_pPieceNameBox->m_PosY);
	}
}

void CControlPeiceShop::RestroeStr()
{
	if(m_iCurExp < 0)
		return ;

	if(m_pMoneyText)
	{
		m_pMoneyText->SetString(m_iCurExp, true);
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

	if( FAILED( g_pDisplay->CreateSurfaceFromBitmap(&m_pSurfaceBack, "interface/pieceshop/piecesshop.BMP") ) )
		return;


	m_pSurfaceBack->SetColorKey( TRANS_COLOR );
	m_pSurfaceBack->Xpos = g_pNk2DFrame->GetClientWidth() - m_pSurfaceBack->GetWidth() - 291;
	m_pSurfaceBack->Ypos = 0;
	m_pSurfaceBack->Ypos = 0;

	m_shopslotstart_x = m_pSurfaceBack->Xpos + 34;
	m_shopslotstart_y = m_pSurfaceBack->Ypos + 92;


	int bgx = m_pSurfaceBack->Xpos;
	int bgy = m_pSurfaceBack->Ypos;

	//-------------------------------------------------------------------
	// BUTTON
	m_pBtn_Close = new CBasicButton();
	if( m_pBtn_Close )
	{
		m_pBtn_Close->Init("common/btn_close_01");
		m_pBtn_Close->SetPosition( bgx + 212 , bgy + 5 );
	}

	m_pBtnHorn = new CBasicButton();
	if( m_pBtnHorn  ) // 36 64 ,
	{
		m_pBtnHorn->Init("pieceshop/horn");
		m_pBtnHorn->SetPosition( bgx + 35 , bgy + 65 );
	}

	m_pBtnHead = new CBasicButton();
	if( m_pBtnHead  )
	{
		m_pBtnHead->Init("pieceshop/head");
		m_pBtnHead->SetPosition( bgx + 35 + 21  , bgy + 65 );
	}

	m_pBtnBody = new CBasicButton();
	if( m_pBtnBody  )
	{
		m_pBtnBody->Init("pieceshop/body");
		m_pBtnBody->SetPosition( bgx + bgx + 35 + 21*2  , bgy + 65 );
	}

	m_pBtnLeg = new CBasicButton();
	if( m_pBtnLeg )
	{
		m_pBtnLeg->Init("pieceshop/leg");
		m_pBtnLeg->SetPosition( bgx + bgx + 35 + 21*3  , bgy + 65 );
	}

	m_pBtnWing = new CBasicButton();
	if( m_pBtnWing )
	{
		m_pBtnWing->Init("pieceshop/wing");
		m_pBtnWing->SetPosition( bgx + bgx + 35 + 21*4  , bgy + 65 );
	}

	m_pBtnTail = new CBasicButton();
	if( m_pBtnTail )
	{
		m_pBtnTail->Init("pieceshop/tail");
		m_pBtnTail->SetPosition( bgx + bgx + 35 + 21*5  , bgy + 65 );
	}

	m_pBtnItem = new CBasicButton();
	if( m_pBtnItem )
	{
		m_pBtnItem->Init("pieceshop/item");
		m_pBtnItem->SetPosition( bgx + bgx + 35 + 21*6  , bgy + 65 );
	}

	m_pPieceNameBox = new CTextOutBox();
	if( m_pPieceNameBox )
	{
		m_pPieceNameBox->Init(12, RGB(255, 210, 0),0, 0);
		m_pPieceNameBox->m_PosX = bgx;
		m_pPieceNameBox->m_PosY = bgy;
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
	g_pDisplay->Blt(m_pSurfaceBack->Xpos, m_pSurfaceBack->Ypos, m_pSurfaceBack);
	if( m_pBtn_Close )		m_pBtn_Close->Draw();

	if( m_pPieceNameBox )	m_pPieceNameBox->Draw(m_pSurfaceBack->Xpos+83 , m_pSurfaceBack->Ypos+340 ,120);
	if( m_pBtnHorn )		m_pBtnHorn->Draw(m_pSurfaceBack->Xpos+37+21*0 , m_pSurfaceBack->Ypos+69);
	if( m_pBtnHead )		m_pBtnHead->Draw(m_pSurfaceBack->Xpos+37+23*1 , m_pSurfaceBack->Ypos+69);
	if( m_pBtnBody )		m_pBtnBody->Draw(m_pSurfaceBack->Xpos+37+23*2 , m_pSurfaceBack->Ypos+69);
	if( m_pBtnLeg )			m_pBtnLeg->Draw(m_pSurfaceBack->Xpos+37+23*3 , m_pSurfaceBack->Ypos+69);
	if( m_pBtnWing )		m_pBtnWing->Draw(m_pSurfaceBack->Xpos+37+23*4 , m_pSurfaceBack->Ypos+69);
	if( m_pBtnTail )		m_pBtnTail->Draw(m_pSurfaceBack->Xpos+37+23*5 , m_pSurfaceBack->Ypos+69);
	if( m_pBtnItem )		m_pBtnItem->Draw(m_pSurfaceBack->Xpos+37+23*6 , m_pSurfaceBack->Ypos+69);

	// Display the content of shop
	POINT point;
	GetCursorPos(&point);
	ScreenToClient(g_pDisplay->GetHWnd(), &point);

	if (pCMyApp->m_pMouse && IsInside(point.x, point.y))
		pCMyApp->m_pMouse->SetMouseType(M_NORMAL);

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
				g_pNk2DFrame->RenderItemInUI( m_shopslotstart_x + pItem->m_SlotX * PETSHOP_SLOT_WIDTH + 2,
											  m_shopslotstart_y + pItem->m_SlotY * PETSHOP_SLOT_HEIGHT + 2,
											  pItem , TRUE, FALSE, FALSE, TRUE );

				m_pOverItem = pItem;
			}
			else
			{
				g_pNk2DFrame->RenderItemInUI( m_shopslotstart_x + pItem->m_SlotX * PETSHOP_SLOT_WIDTH + 2,
											  m_shopslotstart_y + pItem->m_SlotY * PETSHOP_SLOT_HEIGHT + 2,
											  pItem , FALSE, FALSE, FALSE, TRUE );
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
	if( !m_pSurfaceBack )
		return false;

	return m_pSurfaceBack->IsIn(x,y);
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
		pItem->m_MinLevel_Org = pItem->m_MinLevel;
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