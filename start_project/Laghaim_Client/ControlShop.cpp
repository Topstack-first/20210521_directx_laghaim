#include "stdafx.h"


#include "Country.h"
#include "headers.h"
#include "main.h"
#include "ControlShop.h"
#include "ControlInven.h"
#include "ddutil.h"
#include "ItemRender.h"
#include "ItemProto.h"
#include "SpecialItem.h"
#include "Nk2DFrame.h"
#include "UIMgr.h"
#include "tcpipcon.h"
#include "AlphaPopUp.h"
#include "Mouse.h"
#include "msgpopup.h"
#include "g_stringmanager.h"
#include "CommonConfig.h"

extern POINT			g_CursorPoint;
extern HINSTANCE		g_hDllInst;
extern DWORD			g_dwClientCountry;
extern int				g_MatrixSvr;
extern BOOL				g_bGlobalMatrix; ///국가대항전 용 클라이언트 인지 여부.
extern	SVR_TYPE		g_SvrType;


#define GET_SLOT_X(point_x)		((point_x - m_shopslotstart_x) / SHOP_SLOT_WIDTH)
#define GET_SLOT_Y(point_y)		((point_y - m_shopslotstart_y) / SHOP_SLOT_HEIGHT)

extern BOOL CheckIsConversionItem( CItem *pItem, int type = 0 ,int sub_type=0); // 컨버젼되어 나오는 아이템인지의 체크.(마석제한 풀기등에 사용된다.)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CControlShop::CControlShop()
	: m_hWnd(NULL)
	, m_ScreenWidth(0)
	, m_ScreenHeight(0)
	, m_KeeperX(0)
	, m_KeeperZ(0)
	, m_shopslotstart_x(0)
	, m_shopslotstart_y(0)
	, m_CurrLPShopPage(0)
{
	memset(m_MsgBuf, 0, sizeof(m_MsgBuf));

	m_pSurfaceBack	= NULL;
	m_pNkCha		= NULL;
	m_pOverItem		= NULL;
	m_lpshop_wait = false;

	Init();
}

CControlShop::~CControlShop()
{
	DeleteRes();
	RemoveAllItems();

	m_pNkCha = NULL;
}

void CControlShop::Init()
{
	m_CloseBtn.SetFileName("common/btn_close_01");
	m_RepairBtn.SetFileName("shop/btn_repair");

	for (int i = 0; i < SHOP_PACK_NUM; i++)
	{
		m_SellItem[i] = NULL;
		for (int x = 0; x < SHOP_SLOT_X_NUM; x++)
		{
			for (int y = 0; y < SHOP_SLOT_Y_NUM; y++)
				m_Slot[i][x][y] = -1;
		}
	}
	m_KeeperIndex = -1;
	m_ShopIdx = 0;
	m_SellRate = 100;
	m_BuyRate = 100;
	m_shop_type = 0; // 상위 4비트는 체크 플래그(스페셜 상점인지 등..) 하위 4비트는 라임상점인지 BP상점인지의 지불 타입을 표시. (by 원석) ///BP상점

	m_bLPShop = FALSE;
	m_vecLPShopIndex.clear();

	m_iMulitCnt = 0;
	m_bMultibuy = false;
}

void CControlShop::LoadRes()
{
	if( !g_pDisplay )
		return;

	m_hWnd =  g_pDisplay->GetHWnd();
	if( g_pNk2DFrame )
	{
		m_ScreenWidth = g_pNk2DFrame->GetClientWidth();
		m_ScreenHeight = g_pNk2DFrame->GetClientHeight();
	}

	if( FAILED( g_pDisplay->CreateSurfaceFromBitmap(&m_pSurfaceBack, "interface/shop/shop.bmp") ) )
		return;
	m_pSurfaceBack->SetColorKey( TRANS_COLOR );

	m_CloseBtn.LoadRes();
	m_RepairBtn.LoadRes();

	m_pSurfaceBack->Xpos = (g_pNk2DFrame->GetClientWidth() - 291) - m_pSurfaceBack->GetWidth();
	m_pSurfaceBack->Ypos = 0;

	m_MoneyText.Init(12, RGB(0, 255, 255), m_pSurfaceBack->Xpos+84, m_pSurfaceBack->Ypos+349+7);
	m_CloseBtn.SetPosition(m_pSurfaceBack->Xpos+213, m_pSurfaceBack->Ypos+6);
	m_RepairBtn.SetPosition(m_pSurfaceBack->Xpos+162, m_pSurfaceBack->Ypos+315);
	m_shopslotstart_x = m_pSurfaceBack->Xpos+36;
	m_shopslotstart_y = m_pSurfaceBack->Ypos+65;

}

void CControlShop::DeleteRes()
{
	SAFE_DELETE(m_pSurfaceBack);
	m_CloseBtn.DeleteRes();
	m_RepairBtn.DeleteRes();
	m_MoneyText.DeleteRes();

	m_vecLPShopIndex.clear();
	m_bLPShop = FALSE;
	m_CurrLPShopPage = 0;
}

void CControlShop::RemoveAllItems()
{
	for (int i = 0; i < SHOP_PACK_NUM; i++)
	{
		CItem *nextItem = m_SellItem[i];

		while (nextItem)
		{
			CItem *pItem = nextItem;
			nextItem = pItem->m_Next;
			delete pItem;
		}
		m_SellItem[i] = NULL;

		for (int x = 0; x < SHOP_SLOT_X_NUM; x++)
		{
			for (int y = 0; y < SHOP_SLOT_Y_NUM; y++)
				m_Slot[i][x][y] = -1;
		}
	}

	m_iMulitCnt = 0;
	m_bMultibuy = false;
}

void CControlShop::AddItem(int pack_num, int vnum)
{
	if (vnum < 0)
		return;

	int proto_num = g_ItemList.FindItem(vnum);
	if (proto_num >= 0)
	{
		CItem *pItem = new CItem;
		if( !pItem )
			return;

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
				{
					m_Slot[pack_num][x+x_idx][y+y_idx] = vnum;
				}
			}
		}
		else
		{
			delete pItem;
		}
	}
}

void CControlShop::AddSPItem(int pack_num, int index, int vnum, int plus, int option, int option2, int endurance, int time, int price, int count ) //옵션값이 추가된 아이템도 받을수있게 하는 버전 by 원석 ///BP상점
{
	if (vnum < 0)
		return;

	int proto_num = g_ItemList.FindItem(vnum);
	if (proto_num >= 0)
	{
		CItem *pItem = new CItem;
		if( !pItem )
			return;

		pItem->m_Index = index; // 아이템 고유번호가 아니라 상점 아이템 고유번호임에 유의. 여기다 이렇게 넣어도 되는지 확실히 모르겠다.
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

		if(GET_TYPE(pItem)==ITYPE_SUB) // 서브아이템은 plus가 보석업그레이드 수치가 아닌듯.
			pItem->m_PlusNum = plus; // 그냥 넣어준다.
		else if( plus > 0 )
		{
			if( GET_ITEM_LEVEL(pItem) >= 181 ) // 다이아 급.
				pItem->SetDiaGemNum(plus);
			else
				pItem->SetGemNum(plus);
		}

		pItem->m_Special = (option&0xfffffff); // 옵션값 셋팅은 무조건 상위 4비트를 제거해서 셋팅.( 부호비트,특별가격지정 플래그,가격 타입 등이 상위 4비트에 들어간다. 이 값들은 제거해주어야만 정상값이 됨. ) (이 상위 4비트는 이 패킷에서만 옵션 값으로 옴에 유의.)
		pItem->m_Special2 = option2;
		pItem->m_UpgradeEndurance = endurance;
		pItem->m_MaxUpgradeEndurance = endurance;

		SetSpecialValue(pItem, GET_TYPE(pItem), GET_SHAPE(pItem), pItem->m_Special, pItem->m_Special2);

		if( time ) // 시간값이 있으면 시간제 아이템으로 셋팅.
			pItem->SetTimeLimitItem( time );

		if( count != -1 ) // 제한개수는 -1일 경우 제한없음이다. 0이상은 제한이 있음. (제한개수는 클라이언트는 표기를 위해서만 쓰인다. 실제 처리는 서버에서 함.)
			// 뭔가 변수를 만들어서 처리해야할 듯.
			pItem->m_LimitCount = count;

		pItem->SetExPrice( (int)m_shop_type, price ); // 가격 셋팅.

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

BOOL CControlShop::FindBlankSlot(int pack_num, int &x, int &y, int w, int h)
{
	BOOL success = FALSE;
	for (int x_idx = 0; !success && x_idx+w-1 < SHOP_SLOT_X_NUM; x_idx++)
	{
		for (int y_idx = 0; y_idx+h-1 < SHOP_SLOT_Y_NUM; y_idx++)
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

int CControlShop::GetSlotVnum(int x, int y, int *type, int *shape)
{
	if (x < 0 || x >= SHOP_SLOT_X_NUM || y < 0 || y >= SHOP_SLOT_Y_NUM)
		return -1;

	int vnum = m_Slot[0][x][y];
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

int CControlShop::GetSlotIndex( int x, int y ) // 선택한 슬롯에 해당하는 아이템의 인덱스를 리턴. (by 원석)
{
	if (x < 0 || x >= SHOP_SLOT_X_NUM || y < 0 || y >= SHOP_SLOT_Y_NUM)
		return -1;

	CItem *pItem = m_SellItem[0];
	while (pItem) // 등록되어있는 아이템들을 전부 돌며 체크.
	{
		if( x >= pItem->m_SlotX
				&& x < pItem->m_SlotX+GET_ITEM_WIDTH(pItem)
				&& y >= pItem->m_SlotY
				&& y < pItem->m_SlotY+GET_ITEM_HEIGHT(pItem) ) // 아이템마다의 범위를 검사.
			return pItem->m_Index;

		pItem = pItem->m_Next;
	}
	return -1;
}

void CControlShop::SetMoney(bool isFameShop, __int64 fame)
{
	if( m_pNkCha == NULL ) // 예외 처리.
		return;

	if(isFameShop)
	{
		char TempString[200];
		sprintf( TempString, "%d Fame", fame );
		m_MoneyText.SetString( TempString );
	}
	else if( (m_shop_type&MASK_SHOP_PAY_TYPE) == SHOP_PAY_TYPE_LAIM ) // 라임상점일 경우
		m_MoneyText.SetString(m_pNkCha->m_Money,true);
	else if( (m_shop_type&MASK_SHOP_PAY_TYPE) == SHOP_PAY_TYPE_LP ) // BP상점일 경우
	{
		char TempString[200];
		sprintf( TempString, "%d LP", m_pNkCha->m_nLaghaimPoint);
		m_MoneyText.SetString( TempString );
	}
}

CItem* CControlShop::Draw()
{
	if (g_pDisplay && m_pSurfaceBack)
	{
		POINT point;
		GetCursorPos(&point);
		ScreenToClient(g_pDisplay->GetHWnd(), &point);

		m_pOverItem = NULL;
		if( m_pSurfaceBack )
			g_pDisplay->Blt(m_pSurfaceBack->Xpos, m_pSurfaceBack->Ypos, m_pSurfaceBack);

		m_CloseBtn.Draw();
		m_RepairBtn.Draw();


		int slot_x_num, slot_y_num;
		if (point.x >= m_shopslotstart_x && point.y >= m_shopslotstart_y)
		{
			slot_x_num = GET_SLOT_X(point.x);
			slot_y_num = GET_SLOT_Y(point.y);
		}
		else
		{
			slot_x_num = -1;
			slot_y_num = -1;
		}

		int vnum = 0;
		if( m_shop_type&MASK_SP_SHOP ) // 상점이 특별 상점일 경우..처리가 달라야 한다.(같은 Vnum을 가진 아이템을 여러개 팔수있으니까 Vnum으로 구분하면 안된다.)
			vnum = GetSlotIndex( slot_x_num, slot_y_num ); // 마우스가 올려져있는 위치 아이템의 인덱스를 찾는다.
		else
			vnum = GetSlotVnum(slot_x_num, slot_y_num);

		if( !g_pNk2DFrame->GetItemRender() )
			return NULL;

		g_pNk2DFrame->GetItemRender()->BeginRenderLolo();		// 중요.........  꼭 있어야 한다.. m_pItemRender->RenderLolo의 파라미터에 영향을 줌.
		for (int pack_num = 0; pack_num < SHOP_PACK_NUM; pack_num++)
		{
			CItem *pItem = m_SellItem[pack_num];

			while (pItem)
			{
				if( vnum >= 0 &&
						( ( !(m_shop_type&MASK_SP_SHOP)&&pItem->m_Vnum==vnum ) // 특별상점이 아닐경우엔 m_Vnum을 체크.
						  || ( (m_shop_type&MASK_SP_SHOP)&&pItem->m_Index==vnum ))) // 특별상점이면 m_Index를 체크.
				{
					g_pNk2DFrame->RenderItemInUI( m_shopslotstart_x + pItem->m_SlotX * SHOP_SLOT_WIDTH,
												  m_shopslotstart_y + pItem->m_SlotY * SHOP_SLOT_HEIGHT,
												  pItem , TRUE, FALSE, FALSE, TRUE );
					m_pOverItem = pItem;
				}
				else
				{
					DWORD ambient = 0x00555555;
					if (g_pNk2DFrame->IsScroll(pItem) )
						ambient = 0x00cccccc;
					else if (pItem->m_PlusNum > 0) // 상점 아이템도 플러스 아이템은 녹색빛 돌개 처리.
						ambient = UPGRADE_ITEM_COLOR;

					g_pNk2DFrame->RenderItemInUI( m_shopslotstart_x + pItem->m_SlotX * SHOP_SLOT_WIDTH,
												  m_shopslotstart_y + pItem->m_SlotY * SHOP_SLOT_HEIGHT,
												  pItem , FALSE, FALSE, FALSE, TRUE );
				}
				pItem = pItem->m_Next;
			}
		}
		g_pNk2DFrame->GetItemRender()->EndRenderLolo();

		m_MoneyText.Draw(m_pSurfaceBack->Xpos+84, m_pSurfaceBack->Ypos+353 , 120);

		if (pCMyApp->m_pMouse && IsInside(point.x, point.y))
			pCMyApp->m_pMouse->SetMouseType(M_NORMAL);

		if (m_pNkCha && m_pNkCha->m_ExtraSlot)
			return NULL;
		else
			return m_pOverItem;
	}
	return NULL;
}

BOOL CControlShop::IsInside(int x, int y)
{
	if( !m_pSurfaceBack )
		return false;

	return m_pSurfaceBack->IsIn( x, y );
}

BOOL CControlShop::IsPlayArea(int x, int y)
{
	if( !IsInside(x,y) && !g_pNk2DFrame->IsInPopup(x,y) )
		return true;

	return false;
}

CItem* CControlShop::GetItemInfo(int index)
{
	CItem * pItem = m_SellItem[0];

	while (pItem)
	{
		if ( pItem->m_Index == index )
			return pItem;

		pItem = pItem->m_Next;
	}

	return NULL;
}

LRESULT CControlShop::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static char commOutBuf[512] = "";	// 나중에 변수 다른 곳에 설정.
	int x = 0, y = 0;
	int slot_x_num = 0, slot_y_num = 0, vnum = 0;
	int type = 0, shape = 0;
	int ret = 0;
	int nExtraShape = 0;

	switch ( msg )
	{
	case WM_LBUTTONUP:
	case WM_MOUSEMOVE:

		x = LOWORD (lParam);
		y = HIWORD (lParam);
		if (!IsInside(x, y))
			return 0;

		m_RepairBtn.MsgProc(hWnd, msg, wParam, lParam);
		if (m_RepairBtn.GetState() == BTN_ACTION)
			// 버튼이 눌렸을떄 처리는 아직 하지 않았다
			m_RepairBtn.SetState(BTN_NORMAL);

		m_CloseBtn.MsgProc(hWnd, msg, wParam, lParam);

		if (m_CloseBtn.GetState() == BTN_ACTION)
		{
			g_pNk2DFrame->ShowShopWindow(FALSE);
			m_CloseBtn.SetState(BTN_NORMAL);
		}
		return 1;
		break;
	case WM_LBUTTONDOWN:

		x = LOWORD (lParam);
		y = HIWORD (lParam);

		m_RepairBtn.MsgProc(hWnd, msg, wParam, lParam);
		if (m_RepairBtn.GetState() == BTN_DOWN)
		{

			if( g_SvrType == ST_ADULT_ONLY ) // [5/29/2007 Theodoric] 내구도 수리 :: 마우스 타입을 바궈버린다.
			{
				pCMyApp->m_pMouse->SetMouseType(M_REPAIR);
				m_RepairBtn.SetState(BTN_NORMAL);
				return 1;
			}
			else
			{
				// 버튼이 눌렸을떄 처리는 아직 하지 않았다
				m_RepairBtn.SetState(BTN_NORMAL);
			}
		}

		if (IsInside(x, y))
		{
			// [5/29/2007 Theodoric] 내구도 수리 :: 내구 수리 마우스 가 활성화일때는 메시지 처리를 하지 않는다.
			if(pCMyApp->m_pMouse->GetMouseType() == M_REPAIR && g_SvrType == ST_ADULT_ONLY)
				return 1;

			if (m_CloseBtn.IsInside(x, y))
				m_CloseBtn.MsgProc(hWnd, msg, wParam, lParam);
			else if (m_pNkCha && m_pNkCha->m_ExtraSlot && !(GET_ATT_FLAG(m_pNkCha->m_ExtraSlot) & IATT_NO_SELL) )
			{
				if( g_dwClientCountry != CTRY_KOR && m_pNkCha->m_ExtraSlot->m_bTimeLimit ) // 한국외에는 시간제 아이템이면 팔기 불가처리 (by 원석) ///시간제아이템 ///07-04-10 중국만 불가였던거 한국제외는 불가로 변경
					return 1;

				if (g_MatrixSvr != 0 && !g_bGlobalMatrix ) ///국가대항전 은 아이템 팔수있도록...
				{
					if( ( GET_TYPE(m_pNkCha->m_ExtraSlot) == ITYPE_WEAPON
							|| GET_TYPE(m_pNkCha->m_ExtraSlot) == ITYPE_SUB // 05-11-15 매트릭스에서 타입 'SUB'인 아이템도 못팔게 수정.(원석)
							|| GET_TYPE(m_pNkCha->m_ExtraSlot) == ITYPE_ARMOR )
							// 신 매트릭스(뫼비우스 아레나)에서 데미_크루셔와 홀리 스테프는 예외적으로 팔 수 있음.
							&& ( m_pNkCha->m_ExtraSlot->m_Vnum != 1238			// 데미크루셔
								 // nate 2005-05-20 : 홀리부대
								 && m_pNkCha->m_ExtraSlot->m_Vnum != 1259		// 홀리 스테프( 호크 )
								 && m_pNkCha->m_ExtraSlot->m_Vnum != 1260		// 홀리 스테프( 쿨레인 )
								 && m_pNkCha->m_ExtraSlot->m_Vnum != 1261		// 홀리 스테프( 쉬버 )
								 && m_pNkCha->m_ExtraSlot->m_Vnum != 2170		// [2008/11/24 Theodoric] WORLD_MARIX_2009
								 && m_pNkCha->m_ExtraSlot->m_Vnum != 2171
								 && m_pNkCha->m_ExtraSlot->m_Vnum != 2172) )

						if( pCMyApp->GetCurWorld() == 19 ) //WORLD_MATRIX_2009 )
						{
							if( m_pNkCha->m_ExtraSlot->m_Vnum != 1087 ) // 신규 매트릭스에서 에볼류선은 판매가 가능하도록
								return 1;
						}
						else
							return 1;
				}
				
				if( m_bLPShop )
					return 1;

				// [2008/5/26 Theodoric] 시간제 아이템은 팔지 말자
				if( m_pNkCha->m_ExtraSlot->m_bTimeLimit )
				{
					pCMyApp->m_pUIMgr->CmdErrorMsg( "Time-based items are not available for sale." );
					return 1;
				}

				nExtraShape = GET_SHAPE(m_pNkCha->m_ExtraSlot);

				if( CheckSellWarning( m_pNkCha->m_ExtraSlot, 5 ) ) // 경고 띄워야하는녀석은 여기서 경고 띄워준다. ///다중판매 시 수정
					return 1; // 경고 팝업을 띄웠으면 여기서 멈춘다.

				// 소지한도 금액 오버체크.(06-01-05 원석)
				__int64 ResultMoney = m_pNkCha->m_Money + GET_PRICE(m_pNkCha->m_ExtraSlot); // 팔고 난후의 소지금.

				if( ResultMoney < 0 || ResultMoney > LIME_LIMIT ) // -거나 라임 한계를 넘었다면..(무지 클경우 -가 된다.)
				{
					if( pCMyApp && pCMyApp->m_pUIMgr ) // 에러메세지 출력해준다.
						pCMyApp->m_pUIMgr->CmdErrorMsg( (char *)G_STRING(IDS_WATCH_ITEM) ); //-- IDS_WATCH_ITEM : 아이템 혹은 소지금을 확인해주세요

					return 1;
				}

				if (m_KeeperIndex == -1 && m_ShopIdx > 0)
					sprintf(m_MsgBuf, "" );
				else
					sprintf(m_MsgBuf, "sell %lld %.0f %.0f\n", m_KeeperIndex, m_KeeperX, m_KeeperZ);

				SendMsg();
				SAFE_DELETE( m_pNkCha->m_ExtraSlot );
			}
		}
		else if (g_pNk2DFrame && IsPlayArea(x, y))
			g_pNk2DFrame->ShowShopWindow(FALSE);
		else
			return 0;

		return 1;

	case WM_RBUTTONDOWN:

		x = LOWORD (lParam);
		y = HIWORD (lParam);
		if (IsInside(x, y))
			return 1;
		break;

	case WM_LBUTTONDBLCLK:

		x = LOWORD (lParam);
		y = HIWORD (lParam);

		if (!IsInside(x, y))
			return 0;

		// [5/29/2007 Theodoric] 내구도 수리 :: 내구 수리 마우스가 M_REPAIR 일때는 메시지 처리를 하지 않는다.
		if(pCMyApp->m_pMouse->GetMouseType() == M_REPAIR  && g_SvrType == ST_ADULT_ONLY )
			return 1;

		if (x >= m_shopslotstart_x && y >= m_shopslotstart_y)
		{
			if( m_pNkCha->m_ExtraSlot ) // 엑스트라 슬롯이 있다면 안사지게... (06-01-05 원석)
				return 1;

			slot_x_num = GET_SLOT_X(x);
			slot_y_num = GET_SLOT_Y(y);

			// 슬롯 이외에 범위를 더블 클릭하면 그냥 리턴한다.
			if( !IsInsideSlotWindow(x, y) )
				return 1;

			if( m_shop_type & MASK_SP_SHOP )
			{
				vnum = GetSlotIndex(slot_x_num, slot_y_num); // 변수는 vnum을 쓰지만 실제로 인덱스를 받아온다.
				if (vnum >= 0)
				{
					CItem *pItem = m_SellItem[0];
					while (pItem)
					{
						if( pItem->m_Index == vnum )
							break;

						pItem = pItem->m_Next;
					}

					if( !pItem )
						return 1;

					int price = pItem->m_ExPrice * g_pNk2DFrame->GetControlShop()->m_SellRate / 100;
										
					if( m_shop_type == 19 )
					{
						if( price > g_pRoh->m_nLaghaimPoint )
						{
							g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_RULLET_MSG),TYPE_NOR_OK, 1);
							return 1;
						}
					}
					else if( m_shop_type == 16 )
					{
						if( price > g_pRoh->m_Money )
						{
							g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_WANTED_REG_FAIL_LAIM),TYPE_NOR_OK, 1);
							return 1;
						}
					}

					CItem * item = GetItemInfo( vnum );
					if( item && item->m_LimitCount != -1 )
					{
						int real_vnum = item->m_Vnum;
						if( g_pRoh->FindItemByVnumFromAll(real_vnum) )
						{
							g_pNk2DFrame->InsertPopup((char *)G_STRING(IDS_DONT_PICKUP_MSG_01), TYPE_NOR_OK, 1);
							return 0;
						}
					}

					if( m_shop_type == 19 )
					{
						if( m_lpshop_wait )
						{
							g_pNk2DFrame->InsertPopup("Please wait a moment.", TYPE_NOR_OK, 1);
							return 0;
						}

						sprintf(m_MsgBuf, "sp_buy %d %d %d\n", m_KeeperIndex, vnum, m_pNkCha->m_CurPackNum); // 아이템의 툴 인덱스가 아닌 고유번호를 보내주는 패킷.
						SendMsg();
						m_lpshop_wait = true;
					}
					else if( m_shop_type == 16 )
					{
						sprintf(m_MsgBuf, "sp_buy %d %d %d\n", m_KeeperIndex, vnum, m_pNkCha->m_CurPackNum); // 아이템의 툴 인덱스가 아닌 고유번호를 보내주는 패킷.
						SendMsg();
						m_lpshop_wait = false;
					}
				}
			}
			else // 완전히 일반 상점이면...
			{
				vnum = GetSlotVnum(slot_x_num, slot_y_num);

				if (vnum >= 0)
				{
					if (m_KeeperIndex == -1 && m_ShopIdx > 0)
						sprintf(m_MsgBuf, "shopbuy %d %d %d\n", m_ShopIdx, vnum, m_pNkCha->m_CurPackNum);
					else
					{
						if( g_dev_client )
						{
							int chaotic = g_pRoh->m_Chaotic_Num * 0.1;
							if( chaotic < 0 && chaotic > -500 )
							{
								g_pNk2DFrame->AddChatStr( "3 times the fine" , -1 );
							}
							else if( chaotic <= -500 )
							{
								g_pNk2DFrame->AddChatStr( "10 times the fine" , -1 );
							}
						}
						sprintf(m_MsgBuf, "buy %d %.0f %.0f %d %d\n", m_KeeperIndex, m_KeeperX, m_KeeperZ, vnum, m_pNkCha->m_CurPackNum);

					}

					SendMsg();
				}
			}
		}
		return 1;

	case WM_RBUTTONDBLCLK:
		x = LOWORD (lParam);
		y = HIWORD (lParam);

		if (!IsInside(x, y))
			return 0;

		// [5/29/2007 Theodoric] 내구도 수리 :: 내구 수리 마우스가 M_REPAIR 일때는 메시지 처리를 하지 않는다.
		if(pCMyApp->m_pMouse->GetMouseType() == M_REPAIR && g_SvrType == ST_ADULT_ONLY )
			return 1;

		if (x >= m_shopslotstart_x && y >= m_shopslotstart_y)
		{

			slot_x_num = GET_SLOT_X(x);
			slot_y_num = GET_SLOT_Y(y);
			vnum = GetSlotVnum(slot_x_num, slot_y_num, &type, &shape);

			// nate 2004 - 9 라비용 사과 10개씩 이동
			// nate 2004 - 10 소환석 10개씩 이동
			if( ( type == ITYPE_ETC ) ||
					( type == ITYPE_SUB && (shape == ISUB_COMPONENT) ) )
			{
				// 소환석
				if( g_pNk2DFrame && g_pNk2DFrame->GetControlInven() )
					g_pNk2DFrame->GetControlInven()->SetInvenLock(TRUE);

				if( m_bLPShop )
				{
					//
					g_pNk2DFrame->AddChatStr( "Shops can only be purchased individually.." , -1);
					return 0;
				}

			/*	if( m_pNkCha )
				{
					//
					g_pNk2DFrame->AddChatStr( "Shops can only be purchased individually.." , -1);
					return 0;
				}*/

				if( m_pNkCha )
				{
					if (m_KeeperIndex == -1 && m_ShopIdx > 0)
						sprintf(m_MsgBuf, "pshopbuy %d %d %d\n", m_ShopIdx, vnum, m_pNkCha->m_CurPackNum);
					else
					{
						if( g_dev_client )
						{
							int chaotic = g_pRoh->m_Chaotic_Num * 0.1;
							if( chaotic < 0 && chaotic > -500 )
							{
								g_pNk2DFrame->AddChatStr( "3 times the fine" , -1 );
							}
							else if( chaotic <= -500 )
							{
								g_pNk2DFrame->AddChatStr( "10 times the fine" , -1 );
							}
						}

						m_bMultibuy = true;
						sprintf(m_MsgBuf, "pbuy %d %d %d\n", m_KeeperIndex, vnum, m_pNkCha->m_CurPackNum);
					}
				}

				if( vnum == 1239 ) // BP 박스는 옵션값으로 BP 량이 정해지기때문에 기존의 상점 구입 패킷으로 다중쓰면 문제생긴다.
				{
					// 강제로 패킷을 바꿔주자. 후에 옵션이 구분되는 다중 구입 패킷 만들어 넣기로함.
					if (m_KeeperIndex == -1 && m_ShopIdx > 0)
						sprintf(m_MsgBuf, "shopbuy %d %d %d\n", m_ShopIdx, vnum, m_pNkCha->m_CurPackNum);
					else
						sprintf(m_MsgBuf, "buy %d %.0f %.0f %d %d\n", m_KeeperIndex, m_KeeperX, m_KeeperZ, vnum, m_pNkCha->m_CurPackNum);
				}

				SendMsg();
			}
		}
		return 1;
	}

	return 0;
}

void CControlShop::SetNkCha(CNkCharacter *pNkCha)
{
	m_pNkCha = pNkCha;
}

void CControlShop::SendMsg()
{
	if (g_pTcpIp)
		g_pTcpIp->SendNetMessage(m_MsgBuf);
}

void CControlShop::SetRate(int sell_rate, int buy_rate, BYTE shop_type ) // 상점 타입 추가. 이 상점이 BP상점인지, 라임상점인지등을 알려준다. 0 이면 라임상점, 1이면 BP상점 (by 원석) ///BP상점
{
	m_SellRate = sell_rate;
	m_BuyRate = buy_rate;
	m_shop_type = shop_type; // 1바이트 중 상위 4비트는 체크 플래그, 하위 4비트는 상점타입 콤보로 쓰인다.

	SetMoney(); // 이안에서 BP상점이면 금액란을 BP로 표시해준다.
}

HRESULT CControlShop::WarningPopup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	pCMyApp->m_bInverseReturn = TRUE;

	// 소지한도 금액 오버체크.(06-01-05 원석)
	__int64 ResultMoney = m_pNkCha->m_Money + GET_PRICE(m_pNkCha->m_ExtraSlot); // 팔고 난후의 소지금.

	if( ResultMoney < 0 || ResultMoney > LIME_LIMIT ) // -거나 라임 한계를 넘었다면..(무지 클경우 -가 된다.)
	{
		if( pCMyApp && pCMyApp->m_pUIMgr ) // 에러메세지 출력해준다.
			pCMyApp->m_pUIMgr->CmdErrorMsg( (char *)G_STRING(IDS_WATCH_ITEM) ); //-- IDS_WATCH_ITEM : 아이템 혹은 소지금을 확인해주세요

		return 1;
	}

	sprintf(m_MsgBuf, "sell %lld %.0f %.0f\n", m_KeeperIndex, m_KeeperX, m_KeeperZ);
	SendMsg();
	SAFE_DELETE( m_pNkCha->m_ExtraSlot );
	pCMyApp->m_bInverseReturn = FALSE;

	return 0L;
}

BOOL CControlShop::CheckSellWarning( CItem *pItem, int PopupType ) // 팔때 경고 띄우기. (이 안에서 팝업 호출이 있음에 유의) ///다중판매 시 작업
{
	if( !pItem )
		return FALSE;

	if( CheckIsConversionItem(pItem)
			|| CheckIsConversionItem(pItem, 1)
	  )
	{
		//-- IDS_CONVERSIONED_ITEM_SELL : 컨버젼 된 아이템입니다. 정말 파시겠습니까?
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_CONVERSIONED_ITEM_SELL),TYPE_NOR_OKCANCLE, PopupType);	// 5:POPUP_SHOP
		return 1;
	}

	if( CommonConfig::Instance()->GetWeapon360() == true )
	{	
		if( CheckIsConversionItem(pItem,2)
			|| CheckIsConversionItem(pItem,1) )
		{
			g_pNk2DFrame->InsertPopup("It is an expensive item. Do you really want to sell it?",TYPE_NOR_OKCANCLE, PopupType);	// 5:POPUP_SHOP
			return 1;
		}
	}

	if(pItem->m_PlusNum >0)
	{
		///2003 서브아이템을 상점에 팔때 플러스아이템으로 인식하는것 막기.
		if(GET_TYPE(pItem)!=ITYPE_SUB)
		{
			//-- IDS_SHOP_PLUS_ITEM : 플러스 아이템 입니다 정말 파시겠습니까?
			g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_SHOP_PLUS_ITEM),TYPE_NOR_OKCANCLE, PopupType);		// 5:POPUP_SHOP
			return 1;
		}
	}
	if(pItem->m_Special >0)
	{
		if(GET_TYPE(pItem)!=ITYPE_SUB)
		{
			//-- IDS_SHOP_SPECIALITEM_CELL : 스페셜 아이템 입니다. 정말 파시겠습니까?
			g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_SHOP_SPECIALITEM_CELL),TYPE_NOR_OKCANCLE, PopupType);		// 5:POPUP_SHOP
			return 1;
		}
	}
	//if (nExtraShape == IETC_GEMSTONE || nExtraShape == IETC_GEM)
	if(GET_TYPE(pItem) == ITYPE_ETC && (GET_SHAPE(pItem) == IETC_GEMSTONE || GET_SHAPE(pItem) == IETC_GEM ))
	{
		//-- IDS_SHOP_JEWEL_ITEM : 보석입니다.  정말 파시겠습니까?
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_SHOP_JEWEL_ITEM),TYPE_NOR_OKCANCLE, PopupType);		// 5:POPUP_SHOP
		return 1;
	}
	//===============================================================================================
	// nate 2004 - 4
	// 서브 아이템 확인 시스템
	// 조건 검사하고 확인 메세지 뛰우는 부분
	// 우선 서브 아이템인지 검사
	if( GET_TYPE( pItem ) == ITYPE_SUB )
	{
		// 슬레이어인가?
		if( GET_SHAPE( pItem ) == ISUB_SLAYER )
		{
			//-- IDS_ALR_SHOP_SLAYER :  슬레이어 입니다. 정말 파시겠습니까?
			g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_ALR_SHOP_SLAYER),TYPE_NOR_OKCANCLE, PopupType);		// 5:POPUP_SHOP
			return 1;
		}
		// 차크람인가
		if( GET_SHAPE( pItem ) == ISUB_CHAKRAM )
		{
			//-- IDS_ALR_SHOP_CHAKRAM :     차크람 입니다.   정말 파시겠습니까?
			g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_ALR_SHOP_CHAKRAM),TYPE_NOR_OKCANCLE, PopupType);		// 5:POPUP_SHOP
			return 1;
		}
		// 마법책인가
		if( GET_SHAPE( pItem ) == ISUB_BOOK )
		{
			//-- IDS_ALR_SHOP_BOOK :     마법책 입니다.   정말 파시겠습니까?
			g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_ALR_SHOP_BOOK),TYPE_NOR_OKCANCLE, PopupType);		// 5:POPUP_SHOP
			return 1;
		}
		// 완드류인가
		if( GET_SHAPE( pItem ) == ISUB_WAND )
		{
			if( pItem->m_Vnum >= 1020 && pItem->m_Vnum <= 1023)//-- 보조무기
			{
				//-- IDS_ALR_SHOP_SUB : 보조무기입니다 정말 파시겠습니까.
				g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_ALR_SHOP_SUB),TYPE_NOR_OKCANCLE, PopupType);		// 5:POPUP_SHOP
			}
			else
			{
				//-- IDS_ALR_SHOP_WAND :      완드 입니다.      정말 파시겠습니까?
				g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_ALR_SHOP_WAND),TYPE_NOR_OKCANCLE, PopupType);		// 5:POPUP_SHOP
			}
			return 1;
		}
		// 소형장비인가
		if( GET_SHAPE( pItem ) == ISUB_S_EQUIP )
		{
			//-- IDS_ALR_SHOP_S_EQUIP :   소형 장비입니다. 정말 파시겠습니까?
			g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_ALR_SHOP_S_EQUIP),TYPE_NOR_OKCANCLE, PopupType);		// 5:POPUP_SHOP
			return 1;
		}
		// 대형장비인가
		if( GET_SHAPE( pItem ) == ISUB_L_EQUIP )
		{
			//-- IDS_ALR_SHOP_L_EQUIP :   대형 장비 입니다. 정말 파시겠습니까?
			g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_ALR_SHOP_L_EQUIP),TYPE_NOR_OKCANCLE, PopupType);		// 5:POPUP_SHOP
			return 1;
		}
	}
	//===============================================================================================
	//0318
	if(GET_TYPE(pItem) == ITYPE_ETC && (pItem->m_Vnum >=550 && pItem->m_Vnum <=554))
	{
		//-- IDS_SHOP_MASUCK_CELL : 마석입니다. 정말 파시겠습니까?
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_SHOP_MASUCK_CELL),TYPE_NOR_OKCANCLE, PopupType);		// 5:POPUP_SHOP
		return 1;
	}

	if(GET_TYPE(pItem) == ITYPE_ARMOR && (pItem->m_Vnum >= 1392 && pItem->m_Vnum <= 1397))
	{
		//-- IDS_SHOP_QUEST_ITEM : 퀘스트 아이템입니다. 정말 파시겠습니까?
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_SHOP_QUEST_ITEM),TYPE_NOR_OKCANCLE, PopupType);		// 5:POPUP_SHOP
		return 1;
	}

	switch( pItem->m_Vnum )// 아이템 팔 시 확인창 띄워주는 예외처리 품목들..
	{
	case 1521:
	case 1522:
	case 1523:
	case 1524:
	case 1709:
	case 1710:
	case 1711:
	case 1791:
	case 1827:
	case 1828:
	case 1829:
	case 1830:
	case 2163: // [2008/11/3 Theodoric] 국내 프리미엄 상품

		//-- IDS_SHOP_SPECIALITEM_CELL : 스페셜 아이템 입니다. 정말 파시겠습니까?
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_SHOP_SPECIALITEM_CELL),TYPE_NOR_OKCANCLE, PopupType);		// 5:POPUP_SHOP
		return 1;

	case 2049:
	case 2050:
	case 2051:
	case 2052:
	case 2053:
	case 2054:
	case 2055: // 하이메타 무기
		//g_pNk2DFrame->InsertPopup("하이메타 무기를 판매하시겠습니까?", TYPE_NOR_OKCANCLE, PopupType);		// 5:POPUP_SHOP
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_SHOP_MSG1),TYPE_NOR_OKCANCLE, PopupType);		// 5:POPUP_SHOP
		return 1;

	case 2173:
	case 2174:
	case 2175:
	case 2176:
	case 2177:
	case 2178:
	case 2179: // 신의 무기
		//g_pNk2DFrame->InsertPopup("신의 무기를 판매하시겠습니까?", TYPE_NOR_OKCANCLE, PopupType);		// 5:POPUP_SHOP
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_SHOP_MSG2),TYPE_NOR_OKCANCLE, PopupType);		// 5:POPUP_SHOP
		return 1;
	}

	if( GET_ATT_FLAG(pItem) & IATT_CONFIRM_SELL ) // 팔때 물어봄 플래그 체크된 아이템들
	{
		//-- IDS_SHOP_SPECIALITEM_CELL : 스페셜 아이템 입니다. 정말 파시겠습니까?
		//g_pNk2DFrame->InsertPopup("해당 아이템을 정말 판매 하시겠습니까?",TYPE_NOR_OKCANCLE, PopupType);		// 5:POPUP_SHOP
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_SHOP_MSG3),TYPE_NOR_OKCANCLE, PopupType);		// 5:POPUP_SHOP
		return 1;
	}


	return FALSE;
}


BOOL CControlShop::IsInsideSlotWindow(int x, int y)
{
	int x1 = m_shopslotstart_x + SHOP_SLOT_WIDTH * 6;
	int x2 = m_shopslotstart_x;
	int y1 = m_shopslotstart_y + SHOP_SLOT_HEIGHT * 9;
	int y2 = m_shopslotstart_y;

	if(  x < m_shopslotstart_x + SHOP_SLOT_WIDTH * 6
			&& x > m_shopslotstart_x
			&& y < m_shopslotstart_y + SHOP_SLOT_HEIGHT * 9
			&& y > m_shopslotstart_y  )
		return TRUE;

	return FALSE;
}

int CControlShop::GetCurrLPShopIndex()
{
	if( m_vecLPShopIndex.size() == 0)
		return -1;

	SetLpShopPage();

	return m_vecLPShopIndex[m_CurrLPShopPage];
}


int CControlShop::GetPrevLPShopIndex()
{
	if( m_vecLPShopIndex.size() == 0)
		return -1;

	m_CurrLPShopPage --;
	if( m_CurrLPShopPage < 0 )
	{
		m_CurrLPShopPage = m_vecLPShopIndex.size()-1;
	}

	return m_vecLPShopIndex[m_CurrLPShopPage];
}


int CControlShop::GetNextLPShopIndex()
{
	if( m_vecLPShopIndex.size() == 0)
		return -1;

	m_CurrLPShopPage ++;

	if( m_CurrLPShopPage > m_vecLPShopIndex.size()-1 )
	{
		m_CurrLPShopPage = 0;
	}

	return m_vecLPShopIndex[m_CurrLPShopPage];

}

void CControlShop::OnPrevLPShopPage()
{
	int ShopIndex = GetPrevLPShopIndex();

	if( ShopIndex == -1)
		return;

	RemoveAllItems();
	SetKeeper( ShopIndex, 7967.0f, 5415.0f );
	sprintf(m_MsgBuf, "shop %d 7967.0 5415.0\n", ShopIndex);
	SendMsg();
	SetLpShopPage();
}

void CControlShop::OnNextLPShopPage()
{
	int ShopIndex = GetNextLPShopIndex();

	if( ShopIndex == -1)
		return;

	RemoveAllItems();
	SetKeeper( ShopIndex, 7967.0f, 5415.0f );
	sprintf(m_MsgBuf, "shop %d 7967.0 5415.0\n", ShopIndex );
	SendMsg();
	SetLpShopPage();
}

void CControlShop::OnFirstLPShopPage()
{
	if( m_vecLPShopIndex.size() == 0 || m_CurrLPShopPage == 0)
		return;

	m_CurrLPShopPage = 0;
	RemoveAllItems();
	SetKeeper( m_vecLPShopIndex[m_CurrLPShopPage], 7967.0f, 5415.0f );
	sprintf(m_MsgBuf, "shop %d 7967.0 5415.0\n", m_vecLPShopIndex[m_CurrLPShopPage] );
	SendMsg();
	SetLpShopPage();
}


void CControlShop::OnLastLPShopPage()
{
	if( m_vecLPShopIndex.size() == 0 || m_CurrLPShopPage == m_vecLPShopIndex.size()-1)
		return;

	m_CurrLPShopPage = m_vecLPShopIndex.size()-1;
	RemoveAllItems();
	SetKeeper( m_vecLPShopIndex[m_CurrLPShopPage], 7967.0f, 5415.0f );
	sprintf(m_MsgBuf, "shop %d 7967.0 5415.0\n", m_vecLPShopIndex[m_CurrLPShopPage] );
	SendMsg();
	SetLpShopPage();
}

void CControlShop::AddLPShopIndex(int LPShopIndex)
{
	m_vecLPShopIndex.push_back(LPShopIndex);
}

void CControlShop::SetLpShopPage()
{
}
