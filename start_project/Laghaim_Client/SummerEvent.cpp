#include "stdafx.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


#include <string.h>
#include "main.h"
#include "basicbutton.h"
#include "headers.h"
#include "ItemRender.h"
#include "ItemProto.h"
#include "Nk2DFrame.h"
#include "tcpipcon.h"
#include "AlphaPopup.h"
#include "textoutbox.h"
#include "SpecialItem.h"
#include "controlbottom.h"
#include "controlinven.h"
#include "msgpopup.h"
#include "mouse.h"
#include "gamble.h"
#include "alphapopup.h"
#include "SummerEvent.h"
#include "g_stringmanager.h"

extern int g_EffectSound[];

//-------------------------------------------------------------------
// name : 생성자
//-------------------------------------------------------------------
CSummerEvent::CSummerEvent()
	: m_hWnd(NULL)
	, m_pNkCha(NULL)
	, m_my_slotstart_x(0)
	, m_my_slotstart_y(0)
	, m_bQuestWindow(FALSE)
{
	memset(m_MsgBuf, 0, sizeof(m_MsgBuf));

	m_pBack = NULL;
	m_pBack_Title = NULL;
	m_pBtnCancel = NULL;	 // "cancle" 버튼
	m_pBtnExchange = NULL;	 // "교환" 버튼
	m_pTextInfo = NULL;
	m_pEnableSlot = NULL;
	m_pDisableSlot = NULL;
	m_pExistSlot  = NULL;
	m_pItemRender = NULL;
	m_pOverItem = NULL;
	m_bMainActive = false;
	m_bClick = m_bMove = m_bMoveEnd = false;
	m_bExchange = false;
	m_dwMouseClickTickTime = timeGetTime();
}

CSummerEvent::~CSummerEvent()
{
	DeleteRes();
	m_pNkCha = NULL;
}



#include <direct.h>
//-------------------------------------------------------------------
// naem : LoadRes
//-------------------------------------------------------------------
void CSummerEvent::LoadRes(int Type, CItemRender *pItemRender)
{
	if( !pItemRender || !g_pDisplay )
		return;

	m_pItemRender = pItemRender;
	m_bMoveEnd = FALSE;

	m_hWnd =  g_pDisplay->GetHWnd();
	GetClientRect(m_hWnd, &m_ClientRc);

	g_pDisplay->CreateSurfaceFromBitmap (&m_pEnableSlot, "interface/slot/slot_enable.bmp");
	g_pDisplay->CreateSurfaceFromBitmap (&m_pDisableSlot, "interface/slot/slot_disable.bmp");
	g_pDisplay->CreateSurfaceFromBitmap (&m_pExistSlot, "interface/slot/slot_exist.bmp");

	g_pDisplay->CreateSurfaceFromBitmap (&m_pBack, "interface/gamble/bg_gamble.bmp");
	m_pBack->Xpos = g_pNk2DFrame->GetClientWidth() - m_pBack->GetWidth() - 291;
	m_pBack->Ypos = 0;
	m_pBack->SetColorKey(TRANS_COLOR);

	g_pDisplay->CreateSurfaceFromBitmap (&m_pBack_Title, "interface/gamble/title_exchange.bmp");
	m_pBack_Title->SetColorKey(TRANS_COLOR);



	m_my_slotstart_x = m_pBack->Xpos + 35;
	m_my_slotstart_y = m_pBack->Ypos + 251;

	LoadExchange();

	m_bMainActive = TRUE;
}

//  교환시 로드 루틴
void CSummerEvent::LoadExchange()
{
	m_pBtnCancel = new CBasicButton();
	m_pBtnCancel->SetFileName("common/btn_close_01");	  // 취소버튼

	m_pBtnExchange = new CBasicButton();
	m_pBtnExchange->SetFileName("gamble/btn_trade"); // 실행 버튼 이름 변경해야함
	//	g_pDisplay->CreateSurfaceFromBitmap(&m_pTextInfo, "interface/description/summerevent/info_event_summer.bmp");
	g_pDisplay->CreateSurfaceFromBitmap(&m_pTextInfo, "interface/description/summerevent/info_event_shark.bmp");//불량 상어 이벤트

	m_pBtnCancel->LoadRes();
	m_pBtnExchange->LoadRes();


	m_pTextInfo->Xpos = m_pBack->Xpos+34;
	m_pTextInfo->Ypos = m_pBack->Ypos+80;

	if( m_pBtnCancel )
		m_pBtnCancel->SetPosition(m_pBack->Xpos+214, m_pBack->Ypos+6);
	if( m_pBtnExchange )
		m_pBtnExchange->SetPosition(m_pBack->Xpos+47, m_pBack->Ypos+198);
}

//-------------------------------------------------------------------
// naem : DeleteRes()
//-------------------------------------------------------------------
void CSummerEvent::DeleteRes()
{
	SAFE_DELETE(m_pEnableSlot);
	SAFE_DELETE(m_pDisableSlot);
	SAFE_DELETE(m_pExistSlot);
	DeleteExchange();
}

void CSummerEvent::DeleteExchange()
{
	SAFE_DELETE(m_pBack);
	SAFE_DELETE(m_pBack_Title);
	SAFE_DELETE(m_pBtnCancel);
	SAFE_DELETE(m_pBtnExchange); //0428
	SAFE_DELETE(m_pTextInfo);
}

//-------------------------------------------------------------------
// name : Draw
//-------------------------------------------------------------------
CItem* CSummerEvent::Draw()
{
	if(!m_bMainActive)
		return NULL;

	m_pOverItem = NULL;
	DrawExchange();

	return m_pOverItem;
}

void CSummerEvent::DrawExchange()
{
	g_pDisplay->Blt(m_pBack->Xpos, m_pBack->Ypos, m_pBack);
	g_pDisplay->Blt(m_pBack->Xpos, m_pBack->Ypos, m_pBack_Title);
	g_pDisplay->Blt(m_pTextInfo->Xpos, m_pTextInfo->Ypos, m_pTextInfo);
	m_pBtnCancel->Draw();
	m_pBtnExchange->Draw();
	DrawMySlot();
}


//-------------------------------------------------------------------
// name : MsgProc
//-------------------------------------------------------------------
LRESULT CSummerEvent::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int ret = 0;

	int x = LOWORD (lParam);
	int y = HIWORD (lParam);

	switch ( msg )
	{
	case WM_KEYDOWN:
		if(wParam ==VK_ESCAPE ) {}	break;
	case WM_CHAR:
		break;
	case WM_RBUTTONDOWN:
	case WM_MOUSEMOVE:
	case WM_LBUTTONUP:
	case WM_LBUTTONDOWN:
		ret = ExgMessage( hWnd,  msg,  wParam,  lParam );
		break;
	default:
		break;
	}
	return ret;
}

// 칩에서 보석으로의 교환시에 메세지루틴..
LRESULT CSummerEvent::ExgMessage( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static char commOutBuf[512];	// 나중에 변수 다른 곳에 설정.
	int x, y;
	int hr = -1;
	int chip = -1;

	switch (msg)
	{

	case WM_LBUTTONDOWN:
		x = LOWORD (lParam);
		y = HIWORD (lParam);

		if(m_bExchange) // 교환하는순간 extra 하는 것을 막기 위함이다.lock
			return 1;

		if (IsInside(x, y))
		{
			if (m_pBtnCancel->IsInside(x, y))
				m_pBtnCancel->MsgProc(hWnd, msg, wParam, lParam);

			else if (m_pBtnExchange->IsInside(x, y))
				m_pBtnExchange->MsgProc(hWnd, msg, wParam, lParam);
			else if (x >= m_my_slotstart_x && x < m_my_slotstart_x + (SLOT_WIDTH * EXG_SLOT_X_NUM) &&
					 y >= m_my_slotstart_y && y < m_my_slotstart_y + (SLOT_HEIGHT * EXG_SLOT_Y_NUM) )
			{
				if( timeGetTime() - m_dwMouseClickTickTime < 300)   // 0.3초
					return 1;

				m_dwMouseClickTickTime = timeGetTime();
				int slot_x_num, slot_y_num;
				// 엑스트라 슬롯에 있는 경우, 교환창 슬롯에 놓는다.
				if (g_pRoh->m_ExtraSlot)
				{
					//if (m_bExch)
					//	return 1;
					if (GET_ITEM_WIDTH(g_pRoh->m_ExtraSlot) % 2)
					{
						slot_x_num = (x - m_my_slotstart_x) / SLOT_WIDTH - (GET_ITEM_WIDTH(g_pRoh->m_ExtraSlot)/2);
					}
					else
					{
						slot_x_num = (x - m_my_slotstart_x + (SLOT_WIDTH/2)) / SLOT_WIDTH - (GET_ITEM_WIDTH(g_pRoh->m_ExtraSlot)/2);
					}
					if (GET_ITEM_HEIGHT(g_pRoh->m_ExtraSlot) % 2)
					{
						slot_y_num = (y - m_my_slotstart_y) / SLOT_HEIGHT - (GET_ITEM_HEIGHT(g_pRoh->m_ExtraSlot)/2);
					}
					else
					{
						slot_y_num = (y - m_my_slotstart_y + (SLOT_HEIGHT/2)) / SLOT_HEIGHT - (GET_ITEM_HEIGHT(g_pRoh->m_ExtraSlot)/2);
					}
					if (slot_x_num >= 0 && slot_x_num + GET_ITEM_WIDTH(g_pRoh->m_ExtraSlot) - 1 < EXG_SLOT_X_NUM &&
							slot_y_num >= 0 && slot_y_num + GET_ITEM_HEIGHT(g_pRoh->m_ExtraSlot) - 1 < EXG_SLOT_Y_NUM)
					{
						if (g_pRoh->AddToExgInven(slot_x_num, slot_y_num))
						{

							sprintf(m_MsgBuf, "exch inven %d %d\n", slot_x_num, slot_y_num);
							SendMsg();
						}
					}
				}
				else
				{
					// 아이템을 빼는 것.
					//if (!m_bExch)
					if (g_pRoh->AddToExgInven((x-m_my_slotstart_x) / SLOT_WIDTH, (y-m_my_slotstart_y) / SLOT_HEIGHT))
					{
						sprintf(m_MsgBuf, "exch inven %d %d\n", (x-m_my_slotstart_x) / SLOT_WIDTH, (y-m_my_slotstart_y) / SLOT_HEIGHT);
						SendMsg();
					}
				}
			}
		}
		else if (IsPlayArea(x, y))
		{
			//if (m_bExch)
			//	InterCancel(TRUE);
			g_pNk2DFrame->ShowSummerEventWindow(FALSE);
		}
		else
			return 0;

		return 1;
		break;

	case WM_LBUTTONUP:
	case WM_MOUSEMOVE:
		x = LOWORD (lParam);
		y = HIWORD (lParam);
		if (!IsInside(x, y))
			return 0;

		m_pBtnCancel->MsgProc(hWnd, msg, wParam, lParam);
		m_pBtnExchange->MsgProc(hWnd, msg, wParam, lParam);

		if (m_pBtnCancel->GetState() == BTN_ACTION)
			g_pNk2DFrame->ShowSummerEventWindow(FALSE);

		if (m_pBtnExchange->GetState() == BTN_ACTION)
		{
			if(CheckMyslot_Exg())  // 해당 아이템만 있는지 있는지 체크
			{
				sprintf(m_MsgBuf, "ex_vc\n");
				SendMsg();
				m_bExchange = TRUE;
			}
		}

		m_pBtnExchange->SetState(BTN_NORMAL);

		return 1;

	case WM_RBUTTONDOWN:
		x = LOWORD (lParam);
		y = HIWORD (lParam);
		if (IsInside(x, y))
			return 1;
		break;
	}
	return 0;
}

//-------------------------------------------------------------------
// name : DrawMySlot
//-------------------------------------------------------------------

#define GET_MY_SLOT_X(point_x)		((point_x - m_my_slotstart_x) / SLOT_WIDTH)
#define GET_MY_SLOT_Y(point_y)		((point_y - m_my_slotstart_y) / SLOT_HEIGHT)

void CSummerEvent::DrawMySlot()
{
	RECT rcRect;
	POINT point;
	GetCursorPos(&point);
	ScreenToClient(g_pDisplay->GetHWnd(), &point);

	if (pCMyApp->m_pMouse && IsInside(point.x, point.y))
		pCMyApp->m_pMouse->SetMouseType(M_NORMAL);

	// 아이템이 위치한 슬롯 표시
	CItem *pItem = g_pRoh->m_ExgInven;
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
	if (g_pRoh->m_ExtraSlot)
	{
		if (point.x >= m_my_slotstart_x && point.x < m_my_slotstart_x + (SLOT_WIDTH * EXG_SLOT_X_NUM ) &&
				point.y >= m_my_slotstart_y && point.y < m_my_slotstart_y + (SLOT_HEIGHT * EXG_SLOT_Y_NUM) )
		{
			if (GET_ITEM_WIDTH(g_pRoh->m_ExtraSlot) % 2)
			{
				slot_x_num = (point.x - m_my_slotstart_x) / SLOT_WIDTH - (GET_ITEM_WIDTH(g_pRoh->m_ExtraSlot)/2);
			}
			else
			{
				slot_x_num = (point.x - m_my_slotstart_x + (SLOT_WIDTH/2)) / SLOT_WIDTH - (GET_ITEM_WIDTH(g_pRoh->m_ExtraSlot)/2);
			}
			if (GET_ITEM_HEIGHT(g_pRoh->m_ExtraSlot) % 2)
			{
				slot_y_num = (point.y - m_my_slotstart_y) / SLOT_HEIGHT - (GET_ITEM_HEIGHT(g_pRoh->m_ExtraSlot)/2);
			}
			else
			{
				slot_y_num = (point.y - m_my_slotstart_y + (SLOT_HEIGHT/2)) / SLOT_HEIGHT - (GET_ITEM_HEIGHT(g_pRoh->m_ExtraSlot)/2);
			}
			if (slot_x_num >= 0 && slot_x_num + GET_ITEM_WIDTH(g_pRoh->m_ExtraSlot) - 1 < EXG_SLOT_X_NUM &&
					slot_y_num >= 0 && slot_y_num + GET_ITEM_HEIGHT(g_pRoh->m_ExtraSlot) - 1 < EXG_SLOT_Y_NUM)
			{
				rcRect.left = 0;
				rcRect.right = SLOT_WIDTH * GET_ITEM_WIDTH(g_pRoh->m_ExtraSlot);
				rcRect.top = 0;
				rcRect.bottom = SLOT_HEIGHT * GET_ITEM_HEIGHT(g_pRoh->m_ExtraSlot);
				if (g_pRoh->GetItemIndexInExgSlot(slot_x_num, slot_y_num,
												  GET_ITEM_WIDTH(g_pRoh->m_ExtraSlot), GET_ITEM_HEIGHT(g_pRoh->m_ExtraSlot))
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
	int index = g_pRoh->GetExgSlotIndex(slot_x_num, slot_y_num);

	pItem = g_pRoh->m_ExgInven;

	if( m_pItemRender )
	{
		m_pItemRender->BeginRenderLolo();		// 중요..  m_pItemRender->RenderLolo의 파라미터에 영향을 준다.
		while (pItem)
		{
			if (pItem->m_Index == index)
			{
				g_pNk2DFrame->RenderItemInUI( m_my_slotstart_x + pItem->m_SlotX * SLOT_WIDTH,
											  m_my_slotstart_y + pItem->m_SlotY * SLOT_HEIGHT,
											  pItem , TRUE , FALSE , FALSE, TRUE );
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
											  pItem , FALSE , FALSE , FALSE, TRUE );
			}
			pItem = pItem->m_Next;
		}
		m_pItemRender->EndRenderLolo();
	}
}


int CSummerEvent::CheckMyslot_Exg() //임시 이벤트용 함수
{
	//mungmae-2006/08/02 불량 상어 이벤트
	int  vnum = -1;

	CItem* pItem = NULL;
	if( g_pRoh )
		pItem = g_pRoh->m_ExgInven;

	if(!pItem)
		return 0;
	else
	{
		vnum = pItem->m_Vnum;
		if(!(vnum == 1056 || vnum == 1057 || vnum == 1058
				|| vnum == 1059 || vnum == 1060 || vnum == 1061 ))//수영복, 비키니
		{
			//-- IDS_QUEST_ITEM_NOT_MATCH : 다른 종류의 아이템이 섞여있습니다.
			g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_QUEST_ITEM_NOT_MATCH), TYPE_NOR_OK, 1 );
			return 0;
		}
		if(pItem->m_Next)
		{
			//아이템이 두개 이상일때
			//--IDS_INTERCHA_PUT_ITEM : 바꿀 아이템 하나만 놓아주세요.
			g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_INTERCHA_PUT_ITEM), TYPE_NOR_OK, 1 );
			return 0;
		}
	}
	return 1;
}

BOOL CSummerEvent::IsSummerEventItem(int vnum)
{
	static int summer_vnum[6] = {1056,1057,1058,1059,1060,1061};

	for (int i=0; i < 6; i++)
	{
		if (vnum == summer_vnum[i])
			return TRUE;
	}

	return FALSE;
}

BOOL CSummerEvent::IsInside(int x, int y)
{
	if( !m_pBack )
		return FALSE;

	return m_pBack->IsIn( x, y);
}

BOOL CSummerEvent::IsPlayArea(int x, int y)
{
	if( !IsInside(x,y) && !g_pNk2DFrame->IsInPopup(x,y) )
		return true;

	return false;
}

void CSummerEvent::SendMsg()
{
	if (g_pTcpIp)
		g_pTcpIp->SendNetMessage(m_MsgBuf);
}

void CSummerEvent::Exchange(int bsucceed, int type, int vnum, int index)
{
	if(bsucceed == 0)
	{
		//실패
		/*		if(type == 0){//감정결과 실패
					//-- IDS_CHECK_ITEM_KIND : 아이템 종류를 확인해 주세요
					g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_CHECK_ITEM_KIND), TYPE_NOR_OK, 1 );
				}
				else if(type == 1){
					// 다른 아이템이 섞여있어서 감정불가
					//-- IDS_CHECK_ITEM_SET : 아이템 셋트를 확인해주세요.
					g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_CHECK_ITEM_SET), TYPE_NOR_OK, 1 );
				}			*/
	}
	else if(bsucceed == 1)
	{
		//성공
		// 아이템 교환
		if(type == 0)
		{
			//아이템 습극

			ChangeTo(vnum, index);
			//-- IDS_SUCCESS_EXCHANGE_ITEM : 아이템 교환을 성공하였습니다
			g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_SUCCESS_EXCHANGE_ITEM), TYPE_NOR_OK, 1 );
		}
		else if(type == 1)
		{
			// 라임 습득
			LaimTo(vnum);
			char buf[256] = "";
			sprintf(buf,G_STRING(IDS_EXCHANGE_ITEM_GET_LAIM),vnum);
			g_pNk2DFrame->InsertPopup( buf, TYPE_NOR_OK, 1 );
		}
	}
	m_bExchange = FALSE;
}

void CSummerEvent::ChangeTo(int vnum, int svr_idx)
{
	int x_idx, y_idx;

	RemoveAllMyInven();

	if( g_ItemList.FindItem(vnum) == -1)
		return;

	if (vnum == -1)
		return;

	g_pRoh->m_ExgInven = new CItem;

	g_pRoh->m_ExgInven->m_Index = svr_idx;
	g_pRoh->m_ExgInven->m_SlotX = 0;
	g_pRoh->m_ExgInven->m_SlotY = 0;
	g_pRoh->m_ExgInven->m_Vnum = vnum;
	g_pRoh->m_ExgInven->m_ProtoNum = g_ItemList.FindItem(vnum);

	g_pRoh->m_ExgInven->m_Num0 = GET_ITEM_NUM0(g_pRoh->m_ExgInven);
	g_pRoh->m_ExgInven->m_Num1 = GET_ITEM_NUM1(g_pRoh->m_ExgInven);
	g_pRoh->m_ExgInven->m_Num2 = GET_ITEM_NUM2(g_pRoh->m_ExgInven);
	g_pRoh->m_ExgInven->m_Num3 = GET_ITEM_NUM3(g_pRoh->m_ExgInven);
	g_pRoh->m_ExgInven->m_Num4 = GET_ITEM_NUM4(g_pRoh->m_ExgInven);
	g_pRoh->m_ExgInven->m_Num5 = GET_ITEM_NUM5(g_pRoh->m_ExgInven);
	g_pRoh->m_ExgInven->m_Num6 = GET_ITEM_NUM6(g_pRoh->m_ExgInven);
	g_pRoh->m_ExgInven->m_Num7 = GET_ITEM_NUM7(g_pRoh->m_ExgInven);
	g_pRoh->m_ExgInven->m_Num8 = GET_ITEM_NUM8(g_pRoh->m_ExgInven);
	g_pRoh->m_ExgInven->m_Num9 = GET_ITEM_NUM9(g_pRoh->m_ExgInven);
	g_pRoh->m_ExgInven->m_MinLevel = GET_ITEM_LEVEL(g_pRoh->m_ExgInven);
	g_pRoh->m_ExgInven->m_MinLevel_Org = g_pRoh->m_ExgInven->m_MinLevel;
	g_pRoh->m_ExgInven->m_Int = GET_ITEM_INT(g_pRoh->m_ExgInven);
	g_pRoh->m_ExgInven->m_Dex = GET_ITEM_DEX(g_pRoh->m_ExgInven);
	g_pRoh->m_ExgInven->m_Weight = GET_WEIGHT(g_pRoh->m_ExgInven);

	g_pRoh->m_ExgInven->m_Next= NULL;

	// Clear
	for (x_idx = 0; x_idx < EXG_SLOT_X_NUM; x_idx++)
	{
		for (y_idx = 0; y_idx < EXG_SLOT_Y_NUM; y_idx++)
		{
			g_pRoh->m_ExgSlot[x_idx][y_idx] = -1;
		}
	}
	// Add
	for (x_idx = 0; x_idx < GET_ITEM_WIDTH(g_pRoh->m_ExgInven); x_idx++)
	{
		for (y_idx = 0; y_idx < GET_ITEM_HEIGHT(g_pRoh->m_ExgInven); y_idx++)
		{
			g_pRoh->m_ExgSlot[x_idx][y_idx] = svr_idx;
		}
	}
	m_bExchange = FALSE;
}


void CSummerEvent::RemoveAllMyInven()
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

	int x_idx, y_idx;

	for (x_idx = 0; x_idx < EXG_SLOT_X_NUM; x_idx++)
	{
		for (y_idx = 0; y_idx < EXG_SLOT_Y_NUM; y_idx++)
		{
			g_pRoh->m_ExgSlot[x_idx][y_idx] = -1;
		}
	}

}


void CSummerEvent::LaimTo(int Laim)
{
	int x_idx = 0, y_idx = 0;

	RemoveAllMyInven();

	for (x_idx = 0; x_idx < EXG_SLOT_X_NUM; x_idx++)
	{
		for (y_idx = 0; y_idx < EXG_SLOT_Y_NUM; y_idx++)
			g_pRoh->m_ExgSlot[x_idx][y_idx] = -1;
	}
	m_bExchange = FALSE;
}
