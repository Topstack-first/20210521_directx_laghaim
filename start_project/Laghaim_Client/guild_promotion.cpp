#include "stdafx.h"
#include "guild_promotion.h"

#include "d3dutil.h"
#include "headers.h"
#include "tcpipcon.h"
#include "Nk2DFrame.h"
#include "2DResMgr.h"
#include "window_resource.h"
#include "WindowMgr.h"
#include "ItemProto.h"
#include "NkCharacter.h"
#include "ItemRender.h"
#include "TextBox.h"
#include "ControlBottom.h"
#include "guild_data.h"
#include "Filtering.h"
#include "g_stringmanager.h"
#include "createcharacterpage.h"


extern DWORD				g_dwLangType;

#define SLOT_WIDTH		(27)// * m_ScreenWidth / BASE_X_SIZE)
#define SLOT_HEIGHT		(27)// * m_ScreenHeight / BASE_Y_SIZE)
#define SLOT_STARTX		54
#define SLOT_STARTY		57

// nate 2006-03-15 : 특수키 입력을 방지하기 위한 한자키 제한
LRESULT CALLBACK KeyboardHookProc(int code, WPARAM wParam, LPARAM lParam);
extern HHOOK g_hHook;




CGuildPromotion::CGuildPromotion() : CUIBaseWindow()
{
	ZeroMemory(m_MsgBuf, sizeof(m_MsgBuf));

	m_pBackground	= NULL;
	m_pTextInfo = NULL;
}

CGuildPromotion::~CGuildPromotion()
{
	DeleteRes();
	m_btCancel.DeleteRes();
	SAFE_DELETE(m_pBackground);
}

void CGuildPromotion::LoadRes()
{
	int nScreenWidth  = 0;
	int nScreenHeight = 0;

	if( g_pNk2DFrame )
	{
		nScreenWidth  = g_pNk2DFrame->GetClientWidth();
		nScreenHeight = g_pNk2DFrame->GetClientHeight();
	}

	SAFE_DELETE(m_pBackground);

	SetBackground(WBK_BK);
	MakeCloseBtn("guild/exit");
	m_btCancel.SetFileName("guild/msg_no");

	g_pDisplay->CreateSurfaceFromBitmap(&m_pBackground, "interface/guild/guild_promotion/guild_bk.bmp");

	if(m_pBackScene && m_pBackground)
	{
		m_nPosX = (nScreenWidth  - m_pBackScene->GetWidth() ) / 2;
		m_nPosY = (nScreenHeight - m_pBackScene->GetHeight()) / 2 - 20;

		if( m_nPosX <= 1 ) m_nPosX = 1;
		if( m_nPosY <= 1 ) m_nPosY = 1;

		for(int i = 0; i < BT_PROMOTION_NUM; i++)
		{
			m_btPromotion[i].LoadRes();
		}

		m_btCancel.LoadRes();
		SetPosition();
	}
}

void CGuildPromotion::SetPosition()
{
	int nHeight = 0;

	m_pBackScene->Xpos = m_nPosX;
	m_pBackScene->Ypos = m_nPosY;

	m_pBackground->Xpos = m_nPosX + 16;
	m_pBackground->Ypos = m_nPosY + 77;

	m_btCancel.SetPosition(  m_nPosX + 55, m_nPosY + m_pBackScene->GetHeight() - 90);
	m_pBtExit->SetPosition(m_nPosX + m_pBackScene->GetWidth() - ( m_pBtExit->Width + 55 ),
						   m_nPosY + m_pBackScene->GetHeight() - 90);

	for(int i = 0; i < BT_PROMOTION_NUM; i++, nHeight += 35)
	{
		m_btPromotion[i].SetPosition( m_pBackground->Xpos + 3, m_pBackground->Ypos + 33 + nHeight);
	}
}

BOOL CGuildPromotion::InitMode()
{
	char szTmp[256] = {0,};

	for(int i = 0; i < BT_PROMOTION_NUM; i++)
	{
		sprintf(szTmp, "guild/grade_%d", i + 1);
		m_btPromotion[i].SetFileName(szTmp);
	}

	return TRUE;
}

void CGuildPromotion::DeleteRes()
{
	for(int i = 0; i < BT_PROMOTION_NUM; i++)
		m_btPromotion[i].DeleteRes();

	SAFE_DELETE(m_pTextInfo);
}

int CGuildPromotion::Draw()
{
	if( g_pDisplay && m_pBackScene )
	{
		if(CUIBaseWindow::Draw())
		{
			g_pDisplay->Blt(m_pBackground->Xpos, m_pBackground->Ypos, m_pBackground );

			for(int i = 0; i < BT_PROMOTION_NUM; i++)
				m_btPromotion[i].Draw();

			m_btCancel.Draw();
		}
	}

	return -1;
}




void CGuildPromotion::guilds_Bt_Status(int nGLevel)
{
	int i = 0;

	for(i = 0; i < nGLevel; i++)
	{
		m_btPromotion[i].SetDisable( true );
		m_btPromotion[i].SetState(BTN_DISABLE);
	}

	m_btPromotion[i].SetDisable( false );
	m_btPromotion[i].SetState(BTN_NORMAL);

	for(i += 1; i < BT_PROMOTION_NUM; i++)
	{
		m_btPromotion[i].SetDisable( true );
		m_btPromotion[i].SetState(BTN_DISABLE);
	}
}

BOOL CGuildPromotion::MoveWindow(UINT msg, int x, int y)
{
	return CUIBaseWindow::MoveWindow(msg, x, y);
}

void CGuildPromotion::guilds_SendMsg()
{
	if(g_pTcpIp)
		g_pTcpIp->SendNetMessage(m_MsgBuf);
}

BOOL CGuildPromotion::IsInside(int x, int y)
{
	return CUIBaseWindow::IsInside(x, y);
}


LRESULT CGuildPromotion::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	for(int i = 0; i < BT_PROMOTION_NUM; i++)
	{
		m_btPromotion[i].MsgProc(hWnd, msg, wParam, lParam);
	}
	m_btCancel.MsgProc(hWnd, msg, wParam, lParam);

	switch( msg )
	{
	case WM_LBUTTONDOWN:
		{
			if(!IsInside(LOWORD(lParam), HIWORD(lParam)))
				return IM_NONE;

			for(int i = 0; i < BT_PROMOTION_NUM; i++)
			{
				if( m_btPromotion[i].GetState() == BTN_DOWN )
					return IM_INPUT;
			}

			if( m_btCancel.GetState() == BTN_DOWN )
				return IM_INPUT;
		}
		break;

	case WM_LBUTTONUP :
		{
			if(!IsInside(LOWORD(lParam), HIWORD(lParam)))
				return IM_NONE;

			int nSel = -1;

			for(int i = 0; i < BT_PROMOTION_NUM; i++)
			{
				if(m_btPromotion[i].GetState() == BTN_ACTION)
					nSel = i;
			}

			switch(nSel)
			{
			case FIRST_BETA :
			case SECOND_DELTA :
			case THIRD_ZETA :
			case FOURTH_THETA :
			case FIFTH_IOTA :
			case SIXTH_PI :
			case SEVENTH_PHI :
			case EIGHTTH_KAI :
			case NINETH_XI :
				g_pNk2DFrame->GetUIWindow()->OnOpenWindow(WINDOW_GUILD_COMBINATION);
				return IM_INPUT;
			}

			if( m_btCancel.GetState() == BTN_ACTION )
			{
				g_pNk2DFrame->GetUIWindow()->OnOpenPrvWindow();
				return IM_INPUT;
			}
		}
		break;
	}

	return CUIBaseWindow::MsgProc(hWnd, msg, wParam, lParam);
}

/*********************************************************************/


CGuildCombination::CGuildCombination() : CUIBaseWindow()
{
	ZeroMemory(m_MsgBuf, sizeof(m_MsgBuf));

//	m_pBackground	= NULL;
	m_pExistSlot	= NULL;
	m_pEnableSlot	= NULL;
	m_pDisableSlot	= NULL;
	m_pOverItem		= NULL;
}

CGuildCombination::~CGuildCombination()
{
	DeleteRes();

//	SAFE_DELETE(m_pBackground);
}

void CGuildCombination::LoadRes()
{
	int nScreenWidth  = 0;
	int nScreenHeight = 0;
	if( g_pNk2DFrame )
	{
		nScreenWidth  = g_pNk2DFrame->GetClientWidth();
		nScreenHeight = g_pNk2DFrame->GetClientHeight();
	}

//	SAFE_DELETE(m_pBackground);

	SetBackground(WBK_MIX);
	MakeCloseBtn("guild/exit");


	//g_pDisplay->CreateSurfaceFromBitmap(&m_pBackground, "interface/guild/guild_promotion/combination_bk.bmp");

	g_pDisplay->CreateSurfaceFromBitmap(&m_pExistSlot, "interface/guild/guild_promotion/slot_exist.bmp");
	g_pDisplay->CreateSurfaceFromBitmap(&m_pEnableSlot, "interface/guild/guild_promotion/slot_enable.bmp");
	g_pDisplay->CreateSurfaceFromBitmap(&m_pDisableSlot, "interface/guild/guild_promotion/slot_disable.bmp");

	if(m_pBackScene && m_pBackScene)
	{
		m_nPosX = (nScreenWidth  - m_pBackScene->GetWidth() ) / 2;
		m_nPosY = (nScreenHeight - m_pBackScene->GetHeight()) / 2;

		m_btPromote.LoadRes();
		m_btCancel.LoadRes();

		SetPosition();
	}

}

BOOL CGuildCombination::InitMode()
{
	char szTmp[256] = {0,};

	m_btPromote.SetFileName("guild/promote");
	m_btCancel.SetFileName("guild/msg_no");

	return TRUE;
}


void CGuildCombination::DeleteRes()
{
	/*
	for(int i = 0; i < BT_PROMOTION_NUM; i++)
		m_btPromotion[i].DeleteRes();
	*/

	m_btPromote.DeleteRes();
	m_btCancel.DeleteRes();

	SAFE_DELETE(m_pExistSlot);
	SAFE_DELETE(m_pEnableSlot);
	SAFE_DELETE(m_pDisableSlot);
}

int CGuildCombination::Draw()
{
	if( g_pDisplay && m_pBackScene )
	{
		if(CUIBaseWindow::Draw())
		{
			DrawMySlot();

			m_btPromote.Draw();
			m_btCancel.Draw();
		}
	}

	return -1;
}



void CGuildCombination::DrawMySlot()
{
	int slotStart_x = m_pBackScene->Xpos+SLOT_STARTX;
	int slotStart_y = m_pBackScene->Ypos+SLOT_STARTY;

	if( !g_pDisplay )
		return;

	RECT rcRect;
	POINT point;
	GetCursorPos(&point);
	ScreenToClient(g_pDisplay->GetHWnd(), &point);

	// 아이템이 위치한 슬롯 표시
	CItem *pItem = NULL;
	if( g_pRoh )
		pItem = g_pRoh->m_ExgInven;

	while (pItem)
	{
		rcRect.left		= 0;
		rcRect.right	= SLOT_WIDTH * GET_ITEM_WIDTH(pItem);
		rcRect.top		= 0;
		rcRect.bottom	= SLOT_HEIGHT * GET_ITEM_HEIGHT(pItem);

		g_pDisplay->Blt( slotStart_x + (SLOT_WIDTH*pItem->m_SlotX),	slotStart_y + (SLOT_HEIGHT*pItem->m_SlotY),
						 m_pExistSlot, &rcRect);

		pItem = pItem->m_Next;
	}

	// Extra Slot에 있는 아이템이 놓일 곳 표시
	int slot_x_num = 0;
	int	slot_y_num = 0;

	if (g_pRoh && g_pRoh->m_ExtraSlot)
	{
		if (point.x >= slotStart_x && point.x < slotStart_x + (SLOT_WIDTH * EXG_SLOT_X_NUM ) &&
				point.y >= slotStart_y && point.y < slotStart_y + (SLOT_HEIGHT * EXG_SLOT_Y_NUM) )
		{

			if (GET_ITEM_WIDTH(g_pRoh->m_ExtraSlot) % 2)
			{
				slot_x_num = (point.x - slotStart_x) / SLOT_WIDTH - (GET_ITEM_WIDTH(g_pRoh->m_ExtraSlot)/2);
			}
			else
			{
				slot_x_num = (point.x - slotStart_x + (SLOT_WIDTH/2)) / SLOT_WIDTH - (GET_ITEM_WIDTH(g_pRoh->m_ExtraSlot)/2);
			}


			if (GET_ITEM_HEIGHT(g_pRoh->m_ExtraSlot) % 2)
			{
				slot_y_num = (point.y - slotStart_y) / SLOT_HEIGHT - (GET_ITEM_HEIGHT(g_pRoh->m_ExtraSlot)/2);
			}
			else
			{
				slot_y_num = (point.y - slotStart_y + (SLOT_HEIGHT/2)) / SLOT_HEIGHT - (GET_ITEM_HEIGHT(g_pRoh->m_ExtraSlot)/2);
			}


			if (slot_x_num >= 0 && slot_x_num + GET_ITEM_WIDTH(g_pRoh->m_ExtraSlot) - 1 < EXG_SLOT_X_NUM &&
					slot_y_num >= 0	&& slot_y_num + GET_ITEM_HEIGHT(g_pRoh->m_ExtraSlot) - 1 < EXG_SLOT_Y_NUM)
			{
				rcRect.left		= 0;
				rcRect.right	= SLOT_WIDTH * GET_ITEM_WIDTH(g_pRoh->m_ExtraSlot);
				rcRect.top		= 0;
				rcRect.bottom	= SLOT_HEIGHT * GET_ITEM_HEIGHT(g_pRoh->m_ExtraSlot);

				if (g_pRoh->GetItemIndexInExgSlot(slot_x_num, slot_y_num, GET_ITEM_WIDTH(g_pRoh->m_ExtraSlot),
												  GET_ITEM_HEIGHT(g_pRoh->m_ExtraSlot)) >= -1)
				{
					g_pDisplay->Blt( slotStart_x + (SLOT_WIDTH*slot_x_num), slotStart_y + (SLOT_HEIGHT*slot_y_num),
									 m_pEnableSlot, &rcRect);
				}
				else
				{
					g_pDisplay->Blt( slotStart_x + (SLOT_WIDTH*slot_x_num),	slotStart_y + (SLOT_HEIGHT*slot_y_num),
									 m_pDisableSlot, &rcRect);
				}
			}
		}
	}

	// 내 교환품 그리기..
	if (point.x >= slotStart_x && point.y >= slotStart_y)
	{
		slot_x_num = (point.x - slotStart_x) / SLOT_WIDTH;
		slot_y_num = (point.y - slotStart_y) / SLOT_HEIGHT;
	}
	else
	{
		slot_x_num = -1;
		slot_y_num = -1;
	}

	int index = 0;

	if( g_pRoh )
		index = g_pRoh->GetExgSlotIndex(slot_x_num, slot_y_num);

	if( g_pRoh )
		pItem = g_pRoh->m_ExgInven;

	if( g_pNk2DFrame->GetItemRender() )
	{
		g_pNk2DFrame->GetItemRender()->BeginRenderLolo();		// 중요..  m_pItemRender->RenderLolo의 파라미터에 영향을 준다.

		while (pItem)
		{
			if (pItem->m_Index == index)
			{
				g_pNk2DFrame->RenderItemInUI( slotStart_x + pItem->m_SlotX * SLOT_WIDTH,
											  slotStart_y + pItem->m_SlotY * SLOT_HEIGHT,
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

				g_pNk2DFrame->RenderItemInUI( slotStart_x + pItem->m_SlotX * SLOT_WIDTH,
											  slotStart_y + pItem->m_SlotY * SLOT_HEIGHT,
											  pItem , FALSE, FALSE, FALSE, TRUE );

			}
			pItem = pItem->m_Next;
		}

		g_pNk2DFrame->GetItemRender()->EndRenderLolo();
	}
}


void CGuildCombination::SetPosition()
{
	m_pBackScene->Xpos = m_nPosX;
	m_pBackScene->Ypos = m_nPosY;

	m_btPromote.SetPosition(m_nPosX + ( m_pBackScene->GetWidth() - m_btPromote.Width ) / 2 ,
							m_nPosY + m_pBackScene->GetHeight() - m_btPromote.Height - 100);

	m_btCancel.SetPosition(m_nPosX + 45 ,
						   m_nPosY + m_pBackScene->GetHeight() - m_btCancel.Height - 50);

	SetExitBtnPostion(m_pBackScene->GetWidth() - m_pBtExit->Width  - 45,
					  m_pBackScene->GetHeight()- m_pBtExit->Height - 50);
}

BOOL CGuildCombination::MoveWindow(UINT msg, int x, int y)
{
	return CUIBaseWindow::MoveWindow(msg, x, y);
}

void CGuildCombination::guilds_SendMsg()
{
	if(g_pTcpIp)
		g_pTcpIp->SendNetMessage(m_MsgBuf);
}

BOOL CGuildCombination::IsInside(int x, int y)
{
	return CUIBaseWindow::IsInside(x, y);
}


LRESULT CGuildCombination::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	m_btPromote.MsgProc(hWnd, msg, wParam, lParam);
	m_btCancel.MsgProc(hWnd, msg, wParam, lParam);

	int x, y;

	int slotStart_x;
	int slotStart_y;

	switch( msg )
	{
	case WM_LBUTTONDOWN:

		x = LOWORD (lParam);
		y = HIWORD (lParam);

		if( !IsInside(x, y) )
			return IM_NONE;

		if (m_btPromote.GetState() == BTN_DOWN)
		{
			return IM_INPUT;
		}

		if (m_btCancel.GetState() == BTN_DOWN)
		{
			return IM_INPUT;
		}


		slotStart_x = m_pBackScene->Xpos+SLOT_STARTX;
		slotStart_y = m_pBackScene->Ypos+SLOT_STARTY;

		if( x >= slotStart_x && x < slotStart_x + (SLOT_WIDTH * EXG_SLOT_X_NUM) &&
				y >= slotStart_y && y < slotStart_y + (SLOT_HEIGHT * EXG_SLOT_Y_NUM) )
		{
			int slot_x_num, slot_y_num;
			// 엑스트라 슬롯에 있는 경우, 교환창 슬롯에 놓는다.
			if (g_pRoh->m_ExtraSlot)
			{
				if (GET_ITEM_WIDTH(g_pRoh->m_ExtraSlot) % 2)
					slot_x_num = (x - slotStart_x) / SLOT_WIDTH - (GET_ITEM_WIDTH(g_pRoh->m_ExtraSlot)/2);
				else
					slot_x_num = (x - slotStart_x + (SLOT_WIDTH/2)) / SLOT_WIDTH - (GET_ITEM_WIDTH(g_pRoh->m_ExtraSlot)/2);

				if (GET_ITEM_HEIGHT(g_pRoh->m_ExtraSlot) % 2)
					slot_y_num = (y - slotStart_y) / SLOT_HEIGHT - (GET_ITEM_HEIGHT(g_pRoh->m_ExtraSlot)/2);
				else
					slot_y_num = (y - slotStart_y + (SLOT_HEIGHT/2)) / SLOT_HEIGHT - (GET_ITEM_HEIGHT(g_pRoh->m_ExtraSlot)/2);

				if (slot_x_num >= 0 && slot_x_num + GET_ITEM_WIDTH(g_pRoh->m_ExtraSlot) - 1 < EXG_SLOT_X_NUM &&
						slot_y_num >= 0 && slot_y_num + GET_ITEM_HEIGHT(g_pRoh->m_ExtraSlot) - 1 < EXG_SLOT_Y_NUM)
				{
					if (g_pRoh->AddToExgInven(slot_x_num, slot_y_num))
					{
						sprintf(m_MsgBuf, "exch inven %d %d\n", slot_x_num, slot_y_num);
						guilds_SendMsg();
					}
				}
				return IM_INPUT;
			}
			else
			{
				if (g_pRoh->AddToExgInven((x - slotStart_x) / SLOT_WIDTH, (y - slotStart_y) / SLOT_HEIGHT))
				{
					sprintf(m_MsgBuf, "exch inven %d %d\n", (x - slotStart_x) / SLOT_WIDTH, (y - slotStart_y) / SLOT_HEIGHT);
					guilds_SendMsg();
				}
				return IM_INPUT;
			}
		}
		break;

	case WM_LBUTTONUP :
		{
			if(!IsInside(LOWORD(lParam), HIWORD(lParam)))
				return IM_NONE;

			if (m_btPromote.GetState() == BTN_ACTION)
			{
				sprintf(m_MsgBuf, "mg upgrade\n");
				guilds_SendMsg();
			}

			if( m_btCancel.GetState() == BTN_ACTION )
			{
				//	g_pNk2DFrame->GetUIWindow()->OnOpenPrvWindow();

				g_pNk2DFrame->GetUIWindow()->OnOpenWindow(WINDOW_GUILD_PROMOTION);

				CGuildPromotion *pPromotion = NULL;

				if(g_pNk2DFrame->GetUIWindow()->IsOpenWindow(WINDOW_GUILD_PROMOTION))
				{
					pPromotion = (CGuildPromotion*)g_pNk2DFrame->GetUIWindow()->GetWindow(WINDOW_GUILD_PROMOTION);

					if(pPromotion && g_pRoh->GetMyGuildData())
					{
						pPromotion->guilds_Bt_Status(g_pRoh->GetMyGuildData()->GetMyGuildInfo().s_glevel);
					}
				}
				return IM_INPUT;
			}

		}

		break;

	}

	return CUIBaseWindow::MsgProc(hWnd, msg, wParam, lParam);
}

/*********************************************************************/
/*********************************************************************/


CGuildCreate::CGuildCreate()
	: CUIBaseWindow()
	, m_nProperListCount(0)
{
	ZeroMemory(m_MsgBuf, sizeof(m_MsgBuf));
//	m_pBackground   = NULL;
	m_pExistSlot	= NULL;
	m_pEnableSlot	= NULL;
	m_pDisableSlot	= NULL;
	m_pOverItem		= NULL;
	m_GuildName.ClearText();

	m_pszProperList = NULL;
}

CGuildCreate::~CGuildCreate()
{
	DeleteRes();

	for(int i = 0 ; i < m_nProperListCount ; i ++ )
		SAFE_DELETE_ARRAY(m_pszProperList[i]);
	SAFE_DELETE_ARRAY(m_pszProperList);
	//SAFE_DELETE(m_pBackground);
}

void CGuildCreate::LoadRes()
{
	int nScreenWidth  = 0;
	int nScreenHeight = 0;

	if( g_pNk2DFrame )
	{
		nScreenWidth  = g_pNk2DFrame->GetClientWidth();
		nScreenHeight = g_pNk2DFrame->GetClientHeight();
	}

	//SAFE_DELETE(m_pBackground);

	SetBackground(WBK_CREATE);
	MakeCloseBtn("guild/promote_cancel");

	//g_pDisplay->CreateSurfaceFromBitmap(&m_pBackground, "interface/guild/guild_promotion/creation_bk.bmp"  );
	g_pDisplay->CreateSurfaceFromBitmap(&m_pExistSlot,  "interface/guild/guild_promotion/slot_exist.bmp"   );
	g_pDisplay->CreateSurfaceFromBitmap(&m_pEnableSlot, "interface/guild/guild_promotion/slot_enable.bmp"  );
	g_pDisplay->CreateSurfaceFromBitmap(&m_pDisableSlot,"interface/guild/guild_promotion/slot_disable.bmp" );

	if(m_pBackScene && m_pBackScene)
	{
		m_nPosX = (nScreenWidth -  m_pBackScene->GetWidth())  / 2;
		m_nPosY = (nScreenHeight - m_pBackScene->GetHeight()) / 2;

		m_pBackScene->Xpos = m_nPosX;
		m_pBackScene->Ypos = m_nPosY;

		m_btCreate.LoadRes();

		m_bMove = true;

		m_GuildName.Init( 0, 0, 20, RGB(255,255,255), RGB(255, 179, 0), FALSE, MSG_POPUP );
		m_GuildName.SetfontSize();

		SetPosition();
	}
}


BOOL CGuildCreate::InitMode()
{
	m_btCreate.SetFileName("guild/promote_create");

	return TRUE;
}

void CGuildCreate::SetPosition()
{
	int nHeight = 0;

	m_pBackScene->Xpos = m_nPosX;
	m_pBackScene->Ypos = m_nPosY;

	m_GuildName.SetPos(m_pBackScene->Xpos + 90,  m_pBackScene->Ypos + 242 + nHeight);
	m_btCreate.SetPosition(m_pBackScene->Xpos + 53,  m_pBackScene->Ypos + 275 + nHeight);
	m_pBtExit->SetPosition(m_pBackScene->Xpos+m_pBackScene->GetWidth() - (53 + m_pBtExit->Width),
						   m_pBackScene->Ypos + 275 + nHeight);
}


void CGuildCreate::DeleteRes()
{
	m_GuildName.DeleteRes();
	m_btCreate.DeleteRes();

	SAFE_DELETE(m_pExistSlot);
	SAFE_DELETE(m_pEnableSlot);
	SAFE_DELETE(m_pDisableSlot);
}

int CGuildCreate::Draw()
{
	if( g_pDisplay && m_pBackScene )
	{
		if(CUIBaseWindow::Draw())
		{
			DrawMySlot();
			m_btCreate.Draw();
			m_GuildName.Draw();
		}
		return 1;
	}
	return -1;
}



void CGuildCreate::DrawMySlot()
{
	int slotStart_x = m_pBackScene->Xpos+SLOT_STARTX;
	int slotStart_y = m_pBackScene->Ypos+SLOT_STARTY;

	if( !g_pDisplay )
		return;

	RECT rcRect;
	POINT point;
	GetCursorPos(&point);
	ScreenToClient(g_pDisplay->GetHWnd(), &point);

	// 아이템이 위치한 슬롯 표시
	CItem *pItem = NULL;
	if( g_pRoh )
		pItem = g_pRoh->m_ExgInven;

	while (pItem)
	{
		rcRect.left		= 0;
		rcRect.right	= SLOT_WIDTH * GET_ITEM_WIDTH(pItem);
		rcRect.top		= 0;
		rcRect.bottom	= SLOT_HEIGHT * GET_ITEM_HEIGHT(pItem);

		g_pDisplay->Blt( slotStart_x + (SLOT_WIDTH*pItem->m_SlotX),	slotStart_y + (SLOT_HEIGHT*pItem->m_SlotY),
						 m_pExistSlot, &rcRect);

		pItem = pItem->m_Next;
	}

	// Extra Slot에 있는 아이템이 놓일 곳 표시
	int slot_x_num = 0;
	int	slot_y_num = 0;

	if (g_pRoh && g_pRoh->m_ExtraSlot)
	{
		if (point.x >= slotStart_x && point.x < slotStart_x + (SLOT_WIDTH * EXG_SLOT_X_NUM ) &&
				point.y >= slotStart_y && point.y < slotStart_y + (SLOT_HEIGHT * EXG_SLOT_Y_NUM) )
		{

			if (GET_ITEM_WIDTH(g_pRoh->m_ExtraSlot) % 2)
			{
				slot_x_num = (point.x - slotStart_x) / SLOT_WIDTH - (GET_ITEM_WIDTH(g_pRoh->m_ExtraSlot)/2);
			}
			else
			{
				slot_x_num = (point.x - slotStart_x + (SLOT_WIDTH/2)) / SLOT_WIDTH - (GET_ITEM_WIDTH(g_pRoh->m_ExtraSlot)/2);
			}


			if (GET_ITEM_HEIGHT(g_pRoh->m_ExtraSlot) % 2)
			{
				slot_y_num = (point.y - slotStart_y) / SLOT_HEIGHT - (GET_ITEM_HEIGHT(g_pRoh->m_ExtraSlot)/2);
			}
			else
			{
				slot_y_num = (point.y - slotStart_y + (SLOT_HEIGHT/2)) / SLOT_HEIGHT - (GET_ITEM_HEIGHT(g_pRoh->m_ExtraSlot)/2);
			}


			if (slot_x_num >= 0 && slot_x_num + GET_ITEM_WIDTH(g_pRoh->m_ExtraSlot) - 1 < EXG_SLOT_X_NUM &&
					slot_y_num >= 0	&& slot_y_num + GET_ITEM_HEIGHT(g_pRoh->m_ExtraSlot) - 1 < EXG_SLOT_Y_NUM)
			{
				rcRect.left		= 0;
				rcRect.right	= SLOT_WIDTH * GET_ITEM_WIDTH(g_pRoh->m_ExtraSlot);
				rcRect.top		= 0;
				rcRect.bottom	= SLOT_HEIGHT * GET_ITEM_HEIGHT(g_pRoh->m_ExtraSlot);

				if (g_pRoh->GetItemIndexInExgSlot(slot_x_num, slot_y_num, GET_ITEM_WIDTH(g_pRoh->m_ExtraSlot),
												  GET_ITEM_HEIGHT(g_pRoh->m_ExtraSlot)) >= -1)
				{
					g_pDisplay->Blt( slotStart_x + (SLOT_WIDTH*slot_x_num), slotStart_y + (SLOT_HEIGHT*slot_y_num),
									 m_pEnableSlot, &rcRect);
				}
				else
				{
					g_pDisplay->Blt( slotStart_x + (SLOT_WIDTH*slot_x_num),	slotStart_y + (SLOT_HEIGHT*slot_y_num),
									 m_pDisableSlot, &rcRect);
				}
			}
		}
	}

	// 내 교환품 그리기..
	if (point.x >= slotStart_x && point.y >= slotStart_y)
	{
		slot_x_num = (point.x - slotStart_x) / SLOT_WIDTH;
		slot_y_num = (point.y - slotStart_y) / SLOT_HEIGHT;
	}
	else
	{
		slot_x_num = -1;
		slot_y_num = -1;
	}

	int index = 0;

	if( g_pRoh )
		index = g_pRoh->GetExgSlotIndex(slot_x_num, slot_y_num);

	if( g_pRoh )
		pItem = g_pRoh->m_ExgInven;

	if( g_pNk2DFrame->GetItemRender() )
	{
		g_pNk2DFrame->GetItemRender()->BeginRenderLolo();		// 중요..  m_pItemRender->RenderLolo의 파라미터에 영향을 준다.

		while (pItem)
		{
			if (pItem->m_Index == index)
			{
				g_pNk2DFrame->RenderItemInUI( slotStart_x + pItem->m_SlotX * SLOT_WIDTH,
											  slotStart_y + pItem->m_SlotY * SLOT_HEIGHT,
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

				g_pNk2DFrame->RenderItemInUI( slotStart_x + pItem->m_SlotX * SLOT_WIDTH,
											  slotStart_y + pItem->m_SlotY * SLOT_HEIGHT,
											  pItem , FALSE, FALSE, FALSE, TRUE );

			}
			pItem = pItem->m_Next;
		}

		g_pNk2DFrame->GetItemRender()->EndRenderLolo();
	}
}


BOOL CGuildCreate::MoveWindow(UINT msg, int x, int y)
{
	return CUIBaseWindow::MoveWindow(msg, x, y);
}

void CGuildCreate::guilds_SendMsg()
{
	if(g_pTcpIp)
		g_pTcpIp->SendNetMessage(m_MsgBuf);
}

BOOL CGuildCreate::IsInside(int x, int y)
{
	return CUIBaseWindow::IsInside(x, y);
}


LRESULT CGuildCreate::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	m_btCreate.MsgProc(hWnd, msg, wParam, lParam);

	int x, y;

	int slotStart_x;
	int slotStart_y;

	g_hHook = SetWindowsHookEx(WH_KEYBOARD, KeyboardHookProc, NULL, GetCurrentThreadId());// nate 2006-03-15 : 메세지 훅 설정

	if( m_GuildName.GetEnable() && g_pNk2DFrame->GetControlBottom()->HasFocus() )
	{
		m_GuildName.SetEnable( FALSE );
		pCMyApp->ConvertImmStatus();
	}

	switch( msg )
	{
	case WM_LBUTTONDOWN:
		{
			x = LOWORD (lParam);
			y = HIWORD (lParam);

			if(!IsInside(LOWORD(lParam), HIWORD(lParam)))
				return IM_NONE;

			if( m_btCreate.GetState() == BTN_DOWN)
				return IM_INPUT;

			int bInputEnable = m_GuildName.GetEnable();

			if( m_GuildName.MsgProc( hWnd, msg, wParam, lParam ) )

				if( bInputEnable == FALSE && m_GuildName.GetEnable() )
					pCMyApp->RestoreImmStatus();

			slotStart_x = m_pBackScene->Xpos+SLOT_STARTX;
			slotStart_y = m_pBackScene->Ypos+SLOT_STARTY;

			if( x >= slotStart_x && x < slotStart_x + (SLOT_WIDTH * EXG_SLOT_X_NUM) &&
					y >= slotStart_y && y < slotStart_y + (SLOT_HEIGHT * EXG_SLOT_Y_NUM) )
			{
				int slot_x_num, slot_y_num;
				// 엑스트라 슬롯에 있는 경우, 교환창 슬롯에 놓는다.
				if (g_pRoh->m_ExtraSlot)
				{
					if (GET_ITEM_WIDTH(g_pRoh->m_ExtraSlot) % 2)
						slot_x_num = (x - slotStart_x) / SLOT_WIDTH - (GET_ITEM_WIDTH(g_pRoh->m_ExtraSlot)/2);
					else
						slot_x_num = (x - slotStart_x + (SLOT_WIDTH/2)) / SLOT_WIDTH - (GET_ITEM_WIDTH(g_pRoh->m_ExtraSlot)/2);

					if (GET_ITEM_HEIGHT(g_pRoh->m_ExtraSlot) % 2)
						slot_y_num = (y - slotStart_y) / SLOT_HEIGHT - (GET_ITEM_HEIGHT(g_pRoh->m_ExtraSlot)/2);
					else
						slot_y_num = (y - slotStart_y + (SLOT_HEIGHT/2)) / SLOT_HEIGHT - (GET_ITEM_HEIGHT(g_pRoh->m_ExtraSlot)/2);

					if (slot_x_num >= 0 && slot_x_num + GET_ITEM_WIDTH(g_pRoh->m_ExtraSlot) - 1 < EXG_SLOT_X_NUM &&
							slot_y_num >= 0 && slot_y_num + GET_ITEM_HEIGHT(g_pRoh->m_ExtraSlot) - 1 < EXG_SLOT_Y_NUM)
					{
						if (g_pRoh->AddToExgInven(slot_x_num, slot_y_num))
						{
							sprintf(m_MsgBuf, "exch inven %d %d\n", slot_x_num, slot_y_num);
							guilds_SendMsg();
						}
					}
					return IM_INPUT;
				}
				else
				{
					if (g_pRoh->AddToExgInven((x - slotStart_x) / SLOT_WIDTH, (y - slotStart_y) / SLOT_HEIGHT))
					{
						sprintf(m_MsgBuf, "exch inven %d %d\n", (x - slotStart_x) / SLOT_WIDTH, (y - slotStart_y) / SLOT_HEIGHT);
						guilds_SendMsg();
					}
					return IM_INPUT;
				}
			}
		}
		break;

	case WM_LBUTTONUP:

		x = LOWORD (lParam);
		y = HIWORD (lParam);

		if(!IsInside(LOWORD(lParam), HIWORD(lParam)))
			return IM_NONE;

		m_GuildName.MsgProc( hWnd, msg, wParam, lParam );

		if (m_btCreate.GetState() == BTN_ACTION)
		{
			if( !CheckGuldName() ) // 길드 네임 필터링...
				return IM_INPUT;

			sprintf(m_MsgBuf, "mg create c %s\n", m_GuildName.GetStrBuf() );
			guilds_SendMsg();
			pCMyApp->m_IsMsgPopupInputEnable = false;
			return IM_CLOSE;
		}

		m_pBtExit->MsgProc(hWnd, msg, wParam, lParam);

		if( m_pBtExit->GetState() == BTN_ACTION )
		{
			g_pNk2DFrame->GetUIWindow()->OnOpenPrvWindow();
			return IM_INPUT;
		}
		break;

	//case WM_KEYDOWN:
	case WM_CHAR:
	case WM_IME_STARTCOMPOSITION:
	case WM_IME_COMPOSITION:
	case WM_IME_ENDCOMPOSITION:
	case WM_IME_CHAR:
	case WM_IME_NOTIFY:

		// 이상하게 입력 못하도 컨트롤 키를 막음
		if( GetKeyState(VK_CONTROL) & 0x8000 ) // 컨트롤 키가 눌려있으면...
			return IM_NONE;

		if( GetKeyState(VK_SPACE) & 0x8000 )
			return IM_NONE;

		if( m_GuildName.GetEnable() )
			m_GuildName.MsgProc( hWnd, msg, wParam, lParam );

		if( m_GuildName.GetEnable() )
			return IM_INPUT;

		break;
	}

	return CUIBaseWindow::MsgProc(hWnd, msg, wParam, lParam);
}

BOOL CGuildCreate::CheckSpace(char* String)
{
	int len = strlen(String);
	for(int i=0; i<len; i++)
	{
		if(is_space(String[i]) == TRUE)
			return TRUE;
	}
	return FALSE;
}

bool CGuildCreate::CheckGuldName()
{
	if (strlen(m_GuildName.GetStrBuf()) == 0)
	{
		// IDS_GUILD_PLZ_NAME_INPUT "길드명을 입력해주세요."
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_PLZ_NAME_INPUT), TYPE_GUILD_CONFIRM );
		return false;
	}

	if( CheckSpace(m_GuildName.GetStrBuf()) == TRUE )
	{
		// IDS_CANNOT_SPACE "공백문자는 허용하지 않습니다."
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_CANNOT_SPACE), TYPE_GUILD_CONFIRM );
		return false;
	}

	int len = strlen(m_GuildName.GetStrBuf() );

	if ( len > 10*2 || len < 2*2 )
	{
		// IDS_GUILD_CREATE_DONT_STRLEN "길드명은 한글 2~10자, 영문 4~20자로 입력해주세요."
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_CREATE_DONT_STRLEN), TYPE_GUILD_CONFIRM );
		return false;
	}

	char *found_sp = NULL;

	switch( g_dwLangType )
	{
	case 0://LANG_KOR:
		{
			char str1[]= {"~!@#$%^&*()_-+=|\\<>,./?;:'[]{}"};
			char str2[]=
			{
				"　！＇ㄱ，．／：；？＾＿｀｜￣、。·‥…¨〃­―∥＼∼´～ˇ˘˝˚˙¸˛¡¿ː" // ㄱ
				"＂（）［］｛｝‘’“”〔〕〈〉《》" // ㄴ
				"＋－＜＝＞×∠⌒∂≡≪≫∈⊂∪∧∨￢" // ㄷ
				"＄％￦Ｆ′″Å℉㎕㎖㎗ℓ㎘㏄㎣㎤㎥㎦㎙㎚㎛㎜㎝㎞㎟㎠㎡㎢㏊㎍㎎㎏㏏㎈㎉㏈㎧㎨㎰㎱㎲㎳㎴㎵㎶㎷㎸㎹" // ㄹ
				"㎀㎁㎂㎃㎄㎺㎻㎼㎽㎾㎿㎐㎑㎒㎓㎔㏀㏁㎊㎋㎌㏖㏅㎭㏛㎩㎪㎫㎬㏝㏐㏓㏃㏉㏜㏆"
				"＃＆＊＠§※☆★○●◎◇◆□■△▲▽▼→←↑↓↔〓◁◀▷▶♤♠♡♥♧♣⊙◈▣◐◑▒▤▥▨▧▦▩♨☏☎☜☞¶†‡↕↗↙↖↘♭♩♪♬㉿↗№㏇™㏂㏘℡®ªª"
				"─│┐└├┬┤┴┼━┃┓┗┣┳┫┻╋┠┯┨┷┿┝┰┥┸╂┒┑┖┕┞┟┡┢┦┧┩┪┭┮┱┲┵┶"	// ㄹ
				"┹┺┽┾╀╁╃╄╅╆╇╈╉╊"
				"０１２３４５６７８９ⅰⅱⅲⅳⅴⅵⅶⅷⅸⅹⅠⅡⅢⅣⅤⅥⅦⅧⅨⅩ" // ㅈ
				"ＡＢＣＤＥＦＧＨＩＪＫＬＭＮＯＰＱＲＳＴＵＶＷＸＹＺａｂｃｄｅｆｇｈｉｊｋｌｍｎｏｐｑｒｓｔｕｖｗｘｙｚ"	// ㅍ
				"ΑΒΕΖΗΘΙΚΛΜΝΟΠΡΤΥΧαβεκμνορστυχω"	// ㅎ
				"ĿŊıĳĸŀßŉ"	// ㄲ
				"ぁあぃいいぅうぇえぉおかがきぎくぐけげこごさざしじすせぜそぞただちぢっつづてでとどなにぬねのはばぱひびぴふぶ" // ㄸ
				"ぷへべぺほぼぽまみむめもゃやゅゆょよらりるれろゎわゐゑをん"
				"ァアィイゥウェエォオカガキギクグケゲコゴサザシジスズセゼソゾタダチヂッツヅテデトドナニヌネノハバパヒビピフブ"	// ㅃ
				"プヘベペホボポマミムメモャヤュユョヨラリルレロヮワヰヱヲンヴヵヶ"
			}; //ㅇ

			char msgStr[100] = "";

			bool b2BbyteWord = false;
			char *gName = m_GuildName.GetStrBuf() ;

			int len1 = strlen(str1);
			int len2 = strlen(str2);
			int glen = strlen(gName);

			int nRlt = 1;
			for( int j=0; j<glen; j++)
			{

				if( gName[j] > 0 && gName[j] < 127 )
				{
					b2BbyteWord = false;
					for( int i=0; i<len1; i++)
					{
						nRlt = strncmp(&gName[j], &str1[i], 1) ;
						if( nRlt == 0)
						{
							char temp[2]= {0,};
							memcpy(&temp, &str1[i], 1 );
							// IDS_GUILD_CREATE_DONT_WORD "길드명에 %'%s%'문자를 허용하지 않습니다."
							sprintf(msgStr, (char*)G_STRING(IDS_GUILD_CREATE_DONT_WORD), &temp);
							g_pNk2DFrame->InsertPopup(msgStr, TYPE_GUILD_CONFIRM );
							return false;
						}
					}
				}
				else
				{
					b2BbyteWord = true;
					for( int i=0; i<len2; i+=2)
					{
						nRlt = strncmp(&gName[j], &str2[i], 2) ;
						if( nRlt == 0)
						{
							char temp[3]= {0,};
							memcpy(&temp, &str2[i], 2 );
							// IDS_GUILD_CREATE_DONT_WORD "길드명에 %'%s%'문자를 허용하지 않습니다."
							sprintf(msgStr, (char*)G_STRING(IDS_GUILD_CREATE_DONT_WORD), &temp);
							g_pNk2DFrame->InsertPopup(msgStr, TYPE_GUILD_CONFIRM );
							return false;
						}
					}
				}

				if(b2BbyteWord)
					j++;

				b2BbyteWord = false;
			}

			for ( int j = 0; j < m_nProperListCount; j++)
			{
				strcpy(msgStr, m_GuildName.GetStrBuf());
				strlwr(msgStr);	// 소문자로 변환
				if( Str2Str( msgStr, m_pszProperList[j] ) )

				{
					Sleep(700);
					// IDS_CRECHA_UNABLE_STRING_02 : 캐릭터 이름에 "%s" 를 허용하지 않습니다
					sprintf(msgStr, G_STRING(IDS_CRECHA_UNABLE_STRING_02), m_pszProperList[j]);
					g_pNk2DFrame->InsertPopup(msgStr, TYPE_NOR_OK, 1);
					return false;
				}
			}
		}
		break;
	}

	return true;
}

void CGuildCreate::ReadProperList(char* szFileName)
{
	FILE* fp = NULL;
	int i = 0, j = 0;
	int nLen = 0;
	char szBuf[100] = "";

	fp = fopen(szFileName, "rt");
	if(!fp)	// 파일이 없으면 종료
	{
		fp = fopen(szFileName, "rt");
		if(!fp)
		{
			MessageBox(NULL, szFileName, "Warning", MB_OK);
			exit(1);
		}
	}

	fscanf(fp, "%s", szBuf);
	nLen = strlen(szBuf);
	for(i=0; i<nLen; i++)
		szBuf[i] = szBuf[i]-1;

	m_nProperListCount = atoi(szBuf);

	// 구조체를 리스트 개수만큼 생성한다
	m_pszProperList = new char*[m_nProperListCount];

	for( i = 0 ; i < m_nProperListCount ; i ++ )
	{
		m_pszProperList[i] = new char[16];

		fscanf(fp, "%s", szBuf);
		nLen = strlen(szBuf);

		for(j=0; j<nLen; j++)
			szBuf[j] = szBuf[j]-1;

		strcpy(m_pszProperList[i], szBuf);
	}
	fclose(fp);
}

void CGuildCreate::HookClose()
{
	UnhookWindowsHookEx(g_hHook);
}
/*********************************************************************/
/*********************************************************************/


CGuildMix::CGuildMix() : CUIBaseWindow()
{
	ZeroMemory(m_MsgBuf, sizeof(m_MsgBuf));

//	m_pBackground	= NULL;
	m_pExistSlot	= NULL;
	m_pEnableSlot	= NULL;
	m_pDisableSlot	= NULL;
	m_pOverItem		= NULL;
}

CGuildMix::~CGuildMix()
{
	DeleteRes();

//	SAFE_DELETE(m_pBackground);
}

void CGuildMix::LoadRes()
{
	int nScreenWidth  = 0;
	int nScreenHeight = 0;
	if( g_pNk2DFrame )
	{
		nScreenWidth  = g_pNk2DFrame->GetClientWidth();
		nScreenHeight = g_pNk2DFrame->GetClientHeight();
	}

//	SAFE_DELETE(m_pBackground);

	SetBackground(WBK_MIX);
	MakeCloseBtn("guild/exit");


	//g_pDisplay->CreateSurfaceFromBitmap(&m_pBackground, "interface/guild/guild_promotion/combination_bk.bmp");

	g_pDisplay->CreateSurfaceFromBitmap(&m_pExistSlot,		"interface/guild/guild_promotion/slot_exist.bmp");
	g_pDisplay->CreateSurfaceFromBitmap(&m_pEnableSlot,		"interface/guild/guild_promotion/slot_enable.bmp");
	g_pDisplay->CreateSurfaceFromBitmap(&m_pDisableSlot,	"interface/guild/guild_promotion/slot_disable.bmp");

	if(m_pBackScene && m_pBackScene)
	{
		m_nPosX = (nScreenWidth  - m_pBackScene->GetWidth() ) / 2;
		m_nPosY = (nScreenHeight - m_pBackScene->GetHeight()) / 2;

		m_btMix.LoadRes();
		m_btCancel.LoadRes();

		SetPosition();
	}

}


void CGuildMix::SetPosition()
{
	m_pBackScene->Xpos = m_nPosX;
	m_pBackScene->Ypos = m_nPosY;

	m_btMix.SetPosition(m_nPosX + ( m_pBackScene->GetWidth() - m_btMix.Width ) / 2 ,
						m_nPosY + m_pBackScene->GetHeight() - m_btMix.Height - 100);

	m_btCancel.SetPosition(m_nPosX + 45 ,
						   m_nPosY + m_pBackScene->GetHeight() - m_btCancel.Height - 50);

	SetExitBtnPostion(m_pBackScene->GetWidth() - m_pBtExit->Width  - 45,
					  m_pBackScene->GetHeight()- m_pBtExit->Height - 50);
}


BOOL CGuildMix::InitMode()
{
	char szTmp[256] = {0,};

	m_btMix.SetFileName("guild/mix");
	m_btCancel.SetFileName("guild/msg_no");

	return TRUE;
}


void CGuildMix::DeleteRes()
{
	/*
	for(int i = 0; i < BT_PROMOTION_NUM; i++)
		m_btPromotion[i].DeleteRes();
	*/

	m_btMix.DeleteRes();
	m_btCancel.DeleteRes();

	SAFE_DELETE(m_pExistSlot);
	SAFE_DELETE(m_pEnableSlot);
	SAFE_DELETE(m_pDisableSlot);
}

int CGuildMix::Draw()
{
	if( g_pDisplay && m_pBackScene )
	{
		if(CUIBaseWindow::Draw())
		{
			DrawMySlot();

			m_btMix.Draw();
			m_btCancel.Draw();
		}
	}

	return -1;
}



void CGuildMix::DrawMySlot()
{
	int slotStart_x = m_pBackScene->Xpos+SLOT_STARTX;
	int slotStart_y = m_pBackScene->Ypos+SLOT_STARTY;

	if( !g_pDisplay )
		return;

	RECT rcRect;
	POINT point;
	GetCursorPos(&point);
	ScreenToClient(g_pDisplay->GetHWnd(), &point);

	// 아이템이 위치한 슬롯 표시
	CItem *pItem = NULL;
	if( g_pRoh )
		pItem = g_pRoh->m_ExgInven;

	while (pItem)
	{
		rcRect.left		= 0;
		rcRect.right	= SLOT_WIDTH * GET_ITEM_WIDTH(pItem);
		rcRect.top		= 0;
		rcRect.bottom	= SLOT_HEIGHT * GET_ITEM_HEIGHT(pItem);

		g_pDisplay->Blt( slotStart_x + (SLOT_WIDTH*pItem->m_SlotX),	slotStart_y + (SLOT_HEIGHT*pItem->m_SlotY),
						 m_pExistSlot, &rcRect);

		pItem = pItem->m_Next;
	}

	// Extra Slot에 있는 아이템이 놓일 곳 표시
	int slot_x_num = 0;
	int	slot_y_num = 0;

	if (g_pRoh && g_pRoh->m_ExtraSlot)
	{
		if (point.x >= slotStart_x && point.x < slotStart_x + (SLOT_WIDTH * EXG_SLOT_X_NUM ) &&
				point.y >= slotStart_y && point.y < slotStart_y + (SLOT_HEIGHT * EXG_SLOT_Y_NUM) )
		{

			if (GET_ITEM_WIDTH(g_pRoh->m_ExtraSlot) % 2)
			{
				slot_x_num = (point.x - slotStart_x) / SLOT_WIDTH - (GET_ITEM_WIDTH(g_pRoh->m_ExtraSlot)/2);
			}
			else
			{
				slot_x_num = (point.x - slotStart_x + (SLOT_WIDTH/2)) / SLOT_WIDTH - (GET_ITEM_WIDTH(g_pRoh->m_ExtraSlot)/2);
			}


			if (GET_ITEM_HEIGHT(g_pRoh->m_ExtraSlot) % 2)
			{
				slot_y_num = (point.y - slotStart_y) / SLOT_HEIGHT - (GET_ITEM_HEIGHT(g_pRoh->m_ExtraSlot)/2);
			}
			else
			{
				slot_y_num = (point.y - slotStart_y + (SLOT_HEIGHT/2)) / SLOT_HEIGHT - (GET_ITEM_HEIGHT(g_pRoh->m_ExtraSlot)/2);
			}


			if (slot_x_num >= 0 && slot_x_num + GET_ITEM_WIDTH(g_pRoh->m_ExtraSlot) - 1 < EXG_SLOT_X_NUM &&
					slot_y_num >= 0	&& slot_y_num + GET_ITEM_HEIGHT(g_pRoh->m_ExtraSlot) - 1 < EXG_SLOT_Y_NUM)
			{
				rcRect.left		= 0;
				rcRect.right	= SLOT_WIDTH * GET_ITEM_WIDTH(g_pRoh->m_ExtraSlot);
				rcRect.top		= 0;
				rcRect.bottom	= SLOT_HEIGHT * GET_ITEM_HEIGHT(g_pRoh->m_ExtraSlot);

				if (g_pRoh->GetItemIndexInExgSlot(slot_x_num, slot_y_num, GET_ITEM_WIDTH(g_pRoh->m_ExtraSlot),
												  GET_ITEM_HEIGHT(g_pRoh->m_ExtraSlot)) >= -1)
				{
					g_pDisplay->Blt( slotStart_x + (SLOT_WIDTH*slot_x_num), slotStart_y + (SLOT_HEIGHT*slot_y_num),
									 m_pEnableSlot, &rcRect);
				}
				else
				{
					g_pDisplay->Blt( slotStart_x + (SLOT_WIDTH*slot_x_num),	slotStart_y + (SLOT_HEIGHT*slot_y_num),
									 m_pDisableSlot, &rcRect);
				}
			}
		}
	}

	// 내 교환품 그리기..
	if (point.x >= slotStart_x && point.y >= slotStart_y)
	{
		slot_x_num = (point.x - slotStart_x) / SLOT_WIDTH;
		slot_y_num = (point.y - slotStart_y) / SLOT_HEIGHT;
	}
	else
	{
		slot_x_num = -1;
		slot_y_num = -1;
	}

	int index = 0;

	if( g_pRoh )
		index = g_pRoh->GetExgSlotIndex(slot_x_num, slot_y_num);

	if( g_pRoh )
		pItem = g_pRoh->m_ExgInven;

	if( g_pNk2DFrame->GetItemRender() )
	{
		g_pNk2DFrame->GetItemRender()->BeginRenderLolo();		// 중요..  m_pItemRender->RenderLolo의 파라미터에 영향을 준다.

		while (pItem)
		{
			if (pItem->m_Index == index)
			{
				g_pNk2DFrame->RenderItemInUI( slotStart_x + pItem->m_SlotX * SLOT_WIDTH,
											  slotStart_y + pItem->m_SlotY * SLOT_HEIGHT,
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

				g_pNk2DFrame->RenderItemInUI( slotStart_x + pItem->m_SlotX * SLOT_WIDTH,
											  slotStart_y + pItem->m_SlotY * SLOT_HEIGHT,
											  pItem , FALSE, FALSE, FALSE, TRUE );

			}
			pItem = pItem->m_Next;
		}

		g_pNk2DFrame->GetItemRender()->EndRenderLolo();
	}
}



BOOL CGuildMix::MoveWindow(UINT msg, int x, int y)
{
	return CUIBaseWindow::MoveWindow(msg, x, y);
}

void CGuildMix::guilds_SendMsg()
{
	if(g_pTcpIp)
		g_pTcpIp->SendNetMessage(m_MsgBuf);
}

BOOL CGuildMix::IsInside(int x, int y)
{
	return CUIBaseWindow::IsInside(x, y);
}


LRESULT CGuildMix::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	m_btMix.MsgProc(hWnd, msg, wParam, lParam);
	m_btCancel.MsgProc(hWnd, msg, wParam, lParam);

	int x, y;

	int slotStart_x;
	int slotStart_y;

	switch( msg )
	{
	case WM_LBUTTONDOWN:

		x = LOWORD (lParam);
		y = HIWORD (lParam);

		if( !IsInside(x, y) )
			return IM_NONE;

		if (m_btMix.GetState() == BTN_DOWN)
		{
			return IM_INPUT;
		}

		if (m_btCancel.GetState() == BTN_DOWN)
		{
			return IM_INPUT;
		}

		slotStart_x = m_pBackScene->Xpos+SLOT_STARTX;
		slotStart_y = m_pBackScene->Ypos+SLOT_STARTY;

		if( x >= slotStart_x && x < slotStart_x + (SLOT_WIDTH * EXG_SLOT_X_NUM) &&
				y >= slotStart_y && y < slotStart_y + (SLOT_HEIGHT * EXG_SLOT_Y_NUM) )
		{
			int slot_x_num, slot_y_num;
			// 엑스트라 슬롯에 있는 경우, 교환창 슬롯에 놓는다.
			if (g_pRoh->m_ExtraSlot)
			{
				if (GET_ITEM_WIDTH(g_pRoh->m_ExtraSlot) % 2)
					slot_x_num = (x - slotStart_x) / SLOT_WIDTH - (GET_ITEM_WIDTH(g_pRoh->m_ExtraSlot)/2);
				else
					slot_x_num = (x - slotStart_x + (SLOT_WIDTH/2)) / SLOT_WIDTH - (GET_ITEM_WIDTH(g_pRoh->m_ExtraSlot)/2);

				if (GET_ITEM_HEIGHT(g_pRoh->m_ExtraSlot) % 2)
					slot_y_num = (y - slotStart_y) / SLOT_HEIGHT - (GET_ITEM_HEIGHT(g_pRoh->m_ExtraSlot)/2);
				else
					slot_y_num = (y - slotStart_y + (SLOT_HEIGHT/2)) / SLOT_HEIGHT - (GET_ITEM_HEIGHT(g_pRoh->m_ExtraSlot)/2);

				if (slot_x_num >= 0 && slot_x_num + GET_ITEM_WIDTH(g_pRoh->m_ExtraSlot) - 1 < EXG_SLOT_X_NUM &&
						slot_y_num >= 0 && slot_y_num + GET_ITEM_HEIGHT(g_pRoh->m_ExtraSlot) - 1 < EXG_SLOT_Y_NUM)
				{
					if (g_pRoh->AddToExgInven(slot_x_num, slot_y_num))
					{
						sprintf(m_MsgBuf, "exch inven %d %d\n", slot_x_num, slot_y_num);
						guilds_SendMsg();
					}
				}
				return IM_INPUT;
			}
			else
			{
				if (g_pRoh->AddToExgInven((x - slotStart_x) / SLOT_WIDTH, (y - slotStart_y) / SLOT_HEIGHT))
				{
					sprintf(m_MsgBuf, "exch inven %d %d\n", (x - slotStart_x) / SLOT_WIDTH, (y - slotStart_y) / SLOT_HEIGHT);
					guilds_SendMsg();
				}
				return IM_INPUT;
			}
		}
		break;

	case WM_LBUTTONUP :
		{
			if(!IsInside(LOWORD(lParam), HIWORD(lParam)))
				return IM_NONE;

			if (m_btMix.GetState() == BTN_ACTION)
			{
				sprintf(m_MsgBuf, "mg rankwar comp\n");
				guilds_SendMsg();
				return IM_INPUT;
			}

			if( m_btCancel.GetState() == BTN_ACTION)
			{
				g_pNk2DFrame->GetUIWindow()->OnOpenPrvWindow();
				return IM_INPUT;
			}
		}
		break;

	}

	return CUIBaseWindow::MsgProc(hWnd, msg, wParam, lParam);
}
