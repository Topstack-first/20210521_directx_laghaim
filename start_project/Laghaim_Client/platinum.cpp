#include "stdafx.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
// [2/28/2007 Theodoric] ��þƮ ����� ���� �߰��Ǿ��ֽ��ϴ�.
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
#include "platinum.h"
#include "g_stringmanager.h"

extern int g_EffectSound[];


//-------------------------------------------------------------------
// name : ������
//-------------------------------------------------------------------

Cplatinum::Cplatinum()
	: m_hWnd(NULL)
	, m_iType(0)
	, m_pNkCha(NULL)
	, m_my_slotstart_x(0)
	, m_my_slotstart_y(0)
	, m_bQuestWindow(FALSE)
{
	memset(m_MsgBuf, 0, sizeof(m_MsgBuf));

	m_pBack = NULL;
	m_pBtnCancel = NULL;	 // "cancle" ��ư
	m_pBtnExchange = NULL;	 // "��ȯ" ��ư
	m_pTextInfo = NULL;
	m_pAlphaPopUp = NULL;
	m_pEnableSlot = NULL;
	m_pDisableSlot = NULL;
	m_pExistSlot  = NULL;
	m_pItemRender = NULL;
	m_pOverItem = NULL;
	m_bMainActive = false;
	m_bClick = m_bMove = m_bMoveEnd = false;
	m_bExchange = false;
	m_dwMouseClickTickTime = timeGetTime();

	m_pBtnExchangePaper = NULL;
	m_pBtnExchangeRing = NULL;
}

Cplatinum::~Cplatinum()
{
	DeleteRes();
	m_pNkCha = NULL;
}

#include <direct.h>
//-------------------------------------------------------------------
// naem : LoadRes
//-------------------------------------------------------------------
void Cplatinum::LoadRes(int Type, CItemRender *pItemRender)
{
	if( !pItemRender )
		return;

	m_iType = Type;
	m_pItemRender = pItemRender;
	m_bMoveEnd = FALSE;

	m_hWnd =  g_pDisplay->GetHWnd();
	GetClientRect(m_hWnd, &m_ClientRc);

	g_pDisplay->CreateSurfaceFromBitmap (&m_pEnableSlot, "interface/slot/slot_enable.bmp");
	g_pDisplay->CreateSurfaceFromBitmap (&m_pDisableSlot, "interface/slot/slot_disable.bmp");
	g_pDisplay->CreateSurfaceFromBitmap (&m_pExistSlot, "interface/slot/slot_exist.bmp");

	g_pDisplay->CreateSurfaceFromBitmap (&m_pBack, "interface/Platinum/Platinum.bmp");
	m_pBack->Xpos = g_pNk2DFrame->GetClientWidth() - 291 - m_pBack->GetWidth();
	m_pBack->Ypos = 0;

	m_pBack->SetColorKey( TRANS_COLOR );

	m_my_slotstart_x = m_pBack->Xpos + 35;
	m_my_slotstart_y = m_pBack->Ypos + 251;

	LoadExchange();
	m_bMainActive = TRUE;
}



//  ��ȯ�� �ε� ��ƾ
void Cplatinum::LoadExchange()
{
	m_pBtnCancel = new CBasicButton();
	m_pBtnCancel->SetFileName("common/btn_close_01");	  // ��ҹ�ư

	m_pBtnExchange = new CBasicButton();

	// [2/27/2007 Theodoric] ��þƮ
	switch( m_iType )
	{
	case 0: // ��ȭ
		m_pBtnExchange->SetFileName("NpcUpgrade/btn_enhancement");
		g_pDisplay->CreateSurfaceFromBitmap(&m_pTextInfo, "interface/description/platinum_exchange/enchant_upgrade.bmp");
		break;
	case 1: // �÷�Ƽ�� ����
		m_pBtnExchange->SetFileName("NpcUpgrade/btn_execute"); // ���� ��ư �̸� �����ؾ���
		g_pDisplay->CreateSurfaceFromBitmap(&m_pTextInfo, "interface/description/platinum_exchange/pet_gamble_box.bmp");
		break;
	case 2: // �ɼ� �ʱ�ȭ
		m_pBtnExchange->SetFileName("NpcUpgrade/btn_init");
		g_pDisplay->CreateSurfaceFromBitmap(&m_pTextInfo, "interface/description/platinum_exchange/enchant_init.bmp");
		break;
	}

	m_pBtnCancel->LoadRes();
	m_pBtnExchange->LoadRes();


	m_pBtnCancel->LoadRes();
	m_pBtnExchange->LoadRes();


	m_pTextInfo->Xpos = m_pBack->Xpos + 34;
	m_pTextInfo->Ypos = m_pBack->Ypos + 80;

	m_pBtnCancel->SetPosition(m_pBack->Xpos+214, m_pBack->Ypos+6);
	m_pBtnExchange->SetPosition(m_pBack->Xpos+47, m_pBack->Ypos+199);
}

//-------------------------------------------------------------------
// naem : DeleteRes()
//-------------------------------------------------------------------
void Cplatinum::DeleteRes()
{
	SAFE_DELETE(m_pEnableSlot);
	SAFE_DELETE(m_pDisableSlot);
	SAFE_DELETE(m_pExistSlot);

	SAFE_DELETE(m_pBtnExchangePaper);
	SAFE_DELETE(m_pBtnExchangeRing);

	DeleteExchange();
}

void Cplatinum::DeleteExchange()
{
	SAFE_DELETE(m_pBack);
	SAFE_DELETE(m_pBtnCancel);
	SAFE_DELETE(m_pBtnExchange); //0428
//	SAFE_DELETE(m_pBtnExchangePaper);
//	SAFE_DELETE(m_pBtnExchangeRing);
	SAFE_DELETE(m_pTextInfo);
}


//-------------------------------------------------------------------
// name : Draw
//-------------------------------------------------------------------
CItem* Cplatinum::Draw()
{
	if(!m_bMainActive)
		return NULL;

	m_pOverItem = NULL;

	DrawExchange();

	return m_pOverItem;
}

void Cplatinum::DrawExchange()
{
	g_pDisplay->Blt(m_pBack->Xpos, m_pBack->Ypos, m_pBack);
	g_pDisplay->Blt(m_pTextInfo->Xpos, m_pTextInfo->Ypos, m_pTextInfo);
	m_pBtnCancel->Draw();

//	if(m_iType ==0){
//		m_pBtnExchangePaper->Draw();
//		m_pBtnExchangeRing->Draw();
//	}else if(m_iType ==1){
	m_pBtnExchange->Draw();
//	}else if(m_iType==2){
//		m_pBtnExchange->Draw();
//	}

	DrawMySlot();
}


//-------------------------------------------------------------------
// name : MsgProc
//-------------------------------------------------------------------
LRESULT Cplatinum::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int ret = 0;

	int x = LOWORD (lParam);
	int y = HIWORD (lParam);

	switch ( msg )
	{
	case WM_KEYDOWN:
		if(wParam ==VK_ESCAPE )	{}
		break;

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

// Ĩ���� ���������� ��ȯ�ÿ� �޼�����ƾ..
LRESULT Cplatinum::ExgMessage( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static char commOutBuf[512] = "";	// ���߿� ���� �ٸ� ���� ����.
	int x = 0, y = 0;
	int hr = -1;
	int chip = -1;

	switch (msg)
	{

	case WM_LBUTTONDOWN:

		x = LOWORD (lParam);
		y = HIWORD (lParam);

		if(m_bExchange) // ��ȯ�ϴ¼��� extra �ϴ� ���� ���� �����̴�.lock
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
				if( timeGetTime() - m_dwMouseClickTickTime < 300)  // 0.3��
					return 1;

				m_dwMouseClickTickTime = timeGetTime();
				int slot_x_num, slot_y_num;
				// ����Ʈ�� ���Կ� �ִ� ���, ��ȯâ ���Կ� ���´�.
				if (g_pRoh->m_ExtraSlot)
				{
					//if (m_bExch)
					//	return 1;
					if (GET_ITEM_WIDTH(g_pRoh->m_ExtraSlot) % 2)
						slot_x_num = (x - m_my_slotstart_x) / SLOT_WIDTH - (GET_ITEM_WIDTH(g_pRoh->m_ExtraSlot)/2);
					else
						slot_x_num = (x - m_my_slotstart_x + (SLOT_WIDTH/2)) / SLOT_WIDTH - (GET_ITEM_WIDTH(g_pRoh->m_ExtraSlot)/2);

					if (GET_ITEM_HEIGHT(g_pRoh->m_ExtraSlot) % 2)
						slot_y_num = (y - m_my_slotstart_y) / SLOT_HEIGHT - (GET_ITEM_HEIGHT(g_pRoh->m_ExtraSlot)/2);
					else
						slot_y_num = (y - m_my_slotstart_y + (SLOT_HEIGHT/2)) / SLOT_HEIGHT - (GET_ITEM_HEIGHT(g_pRoh->m_ExtraSlot)/2);

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
					if (g_pRoh->AddToExgInven((x-m_my_slotstart_x) / SLOT_WIDTH, (y-m_my_slotstart_y) / SLOT_HEIGHT))
					{
						sprintf(m_MsgBuf, "exch inven %d %d\n", (x-m_my_slotstart_x) / SLOT_WIDTH, (y-m_my_slotstart_y) / SLOT_HEIGHT);
						SendMsg();
					}
				}
			}
		}
		else if (IsPlayArea(x, y))
			g_pNk2DFrame->ShowPlatinumWindow(FALSE);
		else
			return 0;

		return 1;

	case WM_LBUTTONUP:
	case WM_MOUSEMOVE:

		x = LOWORD (lParam);
		y = HIWORD (lParam);

		if (!IsInside(x, y))
			return 0;

		m_pBtnCancel->MsgProc(hWnd, msg, wParam, lParam);
		m_pBtnExchange->MsgProc(hWnd, msg, wParam, lParam);

		if (m_pBtnCancel->GetState() == BTN_ACTION)
			g_pNk2DFrame->ShowPlatinumWindow(FALSE);

		if (m_pBtnExchange->GetState() == BTN_ACTION)
		{
			if(Check2Myslot_Exg()) // �ش� �����۸� �ִ��� �ִ��� üũ
			{
				// [2/27/2007 Theodoric] ��þƮ
				switch( m_iType )
				{
				case 0: // ��ȭ ����
					sprintf(m_MsgBuf, "enchant 1\n");
					break;

				case 1: // �÷�Ƽ�� ����
					sprintf(m_MsgBuf, "pt_ex\n");
					break;

				case 2 : // �ʱ�ȭ
					sprintf(m_MsgBuf, "enchant 0\n");
					break;
				}
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



LRESULT Cplatinum::parentsDayMsg( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static char commOutBuf[512];	// ���߿� ���� �ٸ� ���� ����.
	int x, y;
	int hr = -1;
	int chip = -1;

	switch (msg)
	{

	case WM_LBUTTONDOWN:

		x = LOWORD (lParam);
		y = HIWORD (lParam);

		if(m_bExchange) // ��ȯ�ϴ¼��� extra �ϴ� ���� ���� �����̴�.lock
			return 1;

		if (IsInside(x, y))
		{
			m_pBtnCancel->MsgProc(hWnd, msg, wParam, lParam);
			m_pBtnExchange->MsgProc(hWnd, msg, wParam, lParam);

			if (x >= m_my_slotstart_x && x < m_my_slotstart_x + (SLOT_WIDTH * EXG_SLOT_X_NUM) &&
					y >= m_my_slotstart_y && y < m_my_slotstart_y + (SLOT_HEIGHT * EXG_SLOT_Y_NUM) )
			{
				int slot_x_num = 0, slot_y_num = 0;
				// ����Ʈ�� ���Կ� �ִ� ���, ��ȯâ ���Կ� ���´�.
				if (g_pRoh->m_ExtraSlot)
				{
					//if (m_bExch)
					//	return 1;
					if (GET_ITEM_WIDTH(g_pRoh->m_ExtraSlot) % 2)
						slot_x_num = (x - m_my_slotstart_x) / SLOT_WIDTH - (GET_ITEM_WIDTH(g_pRoh->m_ExtraSlot)/2);
					else
						slot_x_num = (x - m_my_slotstart_x + (SLOT_WIDTH/2)) / SLOT_WIDTH - (GET_ITEM_WIDTH(g_pRoh->m_ExtraSlot)/2);

					if (GET_ITEM_HEIGHT(g_pRoh->m_ExtraSlot) % 2)
						slot_y_num = (y - m_my_slotstart_y) / SLOT_HEIGHT - (GET_ITEM_HEIGHT(g_pRoh->m_ExtraSlot)/2);
					else
						slot_y_num = (y - m_my_slotstart_y + (SLOT_HEIGHT/2)) / SLOT_HEIGHT - (GET_ITEM_HEIGHT(g_pRoh->m_ExtraSlot)/2);

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
					if (g_pRoh->AddToExgInven((x-m_my_slotstart_x) / SLOT_WIDTH, (y-m_my_slotstart_y) / SLOT_HEIGHT))
					{
						sprintf(m_MsgBuf, "exch inven %d %d\n", (x-m_my_slotstart_x) / SLOT_WIDTH, (y-m_my_slotstart_y) / SLOT_HEIGHT);
						SendMsg();
					}
				}
			}
		}
		else if (IsPlayArea(x, y))
			g_pNk2DFrame->ShowEventAncientWindow(FALSE);
		else
			return 0;

		return 1;

	case WM_LBUTTONUP:
	case WM_MOUSEMOVE:

		x = LOWORD (lParam);
		y = HIWORD (lParam);

		if (!IsInside(x, y))
			return 0;

		m_pBtnCancel->MsgProc(hWnd, msg, wParam, lParam);
		m_pBtnExchange->MsgProc(hWnd, msg, wParam, lParam);

		if (m_pBtnCancel->GetState() == BTN_ACTION)
			g_pNk2DFrame->ShowEventAncientWindow(FALSE);

		if (m_pBtnExchange->GetState() == BTN_ACTION)
			CheckMyslot_ParentsDay();

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

#define GET_MY_SLOT_X(point_x)		((point_x - m_my_slotstart_x) / SLOT_WIDTH)
#define GET_MY_SLOT_Y(point_y)		((point_y - m_my_slotstart_y) / SLOT_HEIGHT)

void Cplatinum::DrawMySlot()
{
	RECT rcRect;
	POINT point;
	GetCursorPos(&point);
	ScreenToClient(g_pDisplay->GetHWnd(), &point);

	// �������� ��ġ�� ���� ǥ��
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

	// Extra Slot�� �ִ� �������� ���� �� ǥ��
	int slot_x_num = 0, slot_y_num = 0;
	if (g_pRoh->m_ExtraSlot)
	{
		if (point.x >= m_my_slotstart_x && point.x < m_my_slotstart_x + (SLOT_WIDTH * EXG_SLOT_X_NUM ) &&
				point.y >= m_my_slotstart_y && point.y < m_my_slotstart_y + (SLOT_HEIGHT * EXG_SLOT_Y_NUM) )
		{
			if (GET_ITEM_WIDTH(g_pRoh->m_ExtraSlot) % 2)
				slot_x_num = (point.x - m_my_slotstart_x) / SLOT_WIDTH - (GET_ITEM_WIDTH(g_pRoh->m_ExtraSlot)/2);
			else
				slot_x_num = (point.x - m_my_slotstart_x + (SLOT_WIDTH/2)) / SLOT_WIDTH - (GET_ITEM_WIDTH(g_pRoh->m_ExtraSlot)/2);

			if (GET_ITEM_HEIGHT(g_pRoh->m_ExtraSlot) % 2)
				slot_y_num = (point.y - m_my_slotstart_y) / SLOT_HEIGHT - (GET_ITEM_HEIGHT(g_pRoh->m_ExtraSlot)/2);
			else
				slot_y_num = (point.y - m_my_slotstart_y + (SLOT_HEIGHT/2)) / SLOT_HEIGHT - (GET_ITEM_HEIGHT(g_pRoh->m_ExtraSlot)/2);

			if (slot_x_num >= 0 && slot_x_num + GET_ITEM_WIDTH(g_pRoh->m_ExtraSlot) - 1 < EXG_SLOT_X_NUM &&
					slot_y_num >= 0 && slot_y_num + GET_ITEM_HEIGHT(g_pRoh->m_ExtraSlot) - 1 < EXG_SLOT_Y_NUM)
			{
				rcRect.left = 0;
				rcRect.right = SLOT_WIDTH * GET_ITEM_WIDTH(g_pRoh->m_ExtraSlot);
				rcRect.top = 0;
				rcRect.bottom = SLOT_HEIGHT * GET_ITEM_HEIGHT(g_pRoh->m_ExtraSlot);
				if (g_pRoh->GetItemIndexInExgSlot(slot_x_num, slot_y_num,
												  GET_ITEM_WIDTH(g_pRoh->m_ExtraSlot), GET_ITEM_HEIGHT(g_pRoh->m_ExtraSlot)) >= -1)
					g_pDisplay->Blt( m_my_slotstart_x + (SLOT_WIDTH*slot_x_num)
									 , m_my_slotstart_y + (SLOT_HEIGHT*slot_y_num)
									 , m_pEnableSlot, &rcRect);
				else
					g_pDisplay->Blt( m_my_slotstart_x + (SLOT_WIDTH*slot_x_num)
									 , m_my_slotstart_y + (SLOT_HEIGHT*slot_y_num)
									 , m_pDisableSlot, &rcRect);
			}
		}
	}

	// �� ��ȯǰ �׸���..
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
	m_pItemRender->BeginRenderLolo();		// �߿�..  m_pItemRender->RenderLolo�� �Ķ���Ϳ� ������ �ش�.

	while (pItem)
	{
		if (pItem->m_Index == index)
		{
			g_pNk2DFrame->RenderItemInUI(m_my_slotstart_x + pItem->m_SlotX * SLOT_WIDTH,
										 m_my_slotstart_y + pItem->m_SlotY * SLOT_HEIGHT,
										 pItem , TRUE , FALSE , FALSE, TRUE );
			m_pOverItem = pItem;
		}
		else
		{
			DWORD ambient = 0x00555555;
			if (g_pNk2DFrame->IsScroll(pItem) )
				ambient = 0x00cccccc;
			else if (pItem->m_PlusNum > 0)
				ambient = UPGRADE_ITEM_COLOR;

			g_pNk2DFrame->RenderItemInUI(m_my_slotstart_x + pItem->m_SlotX * SLOT_WIDTH,
										 m_my_slotstart_y + pItem->m_SlotY * SLOT_HEIGHT,
										 pItem , FALSE , FALSE , FALSE, TRUE );
		}
		pItem = pItem->m_Next;
	}
	m_pItemRender->EndRenderLolo();

}

int Cplatinum::CheckMyslot_Exg()
{
	int  vnum = -1;
	int  kind = 0;

	CItem* pItem = g_pRoh->m_ExgInven;

	if(!pItem)
		return -1;

	while (pItem)
	{
		vnum = pItem->m_Vnum;

		// nate 2004-4 : quest - ������ ó��
		if( vnum == 925 || vnum == 1054 )//0409 �������� ��� �߰�
			kind = 3;
		else if(vnum ==901) //0409 ��ī���� ����
			kind = 4;
		else
		{
			// [2/27/2007 Theodoric] ��þƮ
			switch(m_iType)
			{
			case 0:
			case 1: //-- IDS_QUEST_ITEM_NOT_MATCH : �ٸ� ������ �������� �����ֽ��ϴ�.
				g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_QUEST_ITEM_NOT_MATCH), TYPE_NOR_OK, 1 );
				break;
			case 2:
				g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_QUEST_ITEM_NOT_MATCH), TYPE_NOR_OK, 1 );
				break;
			}
			return -1;
		}
		pItem = pItem->m_Next;
	}

	return kind;
}

int Cplatinum::Check2Myslot_Exg() //�ӽ� �̺�Ʈ�� �Լ�
{
	int  vnum = -1;
	int  kind = 0;

	CItem* pItem = g_pRoh->m_ExgInven;

	if(!pItem)
	{
		// [2/27/2007 Theodoric] ��þƮ
		switch(m_iType)
		{
		case 0: // ��ȭ //-- ������ ������ �������� �����ּ���.
			g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_ENCHANT_PLZ_ITEM), TYPE_NOR_OK, 1 );
			break;
		case 1: // �÷�Ƽ�� ���� //-- IDS_PLATINUM_PLACE_ITEM : �����÷�Ƽ���� �������� �����ּ���.
			g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_PLATINUM_PLACE_ITEM), TYPE_NOR_OK, 1 );
			break;
		case 2: // �ʱ�ȭ
			g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_ENCHANT_PLZ_ITEM), TYPE_NOR_OK, 1 );
			break;
		}
		return 0;
	}


	// [2/28/2007 Theodoric] ��þƮ : ���ѵ� ������ �Ǻ�
	switch( m_iType )
	{
	case 0: // ��ȭ
		while (pItem)
		{
			if( pItem->m_bTimeLimit ) // �ð��� ������ �Ұ�ó��. �߰�. (06-05-16 ����)
			{
				//-- IDS_LIMITED_CANT : ���ѵ� �������� �Ұ����մϴ�.
				g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_LIMITED_CANT), TYPE_NOR_OK, 1 );
				return 0;
			}
			else
			{
				pItem = pItem->m_Next;
			}
		}
		break;

	case 1: // �÷�Ƽ�� ����
		while (pItem)
		{
			if( pItem->m_bTimeLimit ) // �ð��� ������ �Ұ�ó��. �߰�. (06-05-16 ����)
			{
				//-- IDS_LIMITED_CANT : ���ѵ� �������� �Ұ����մϴ�.
				g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_LIMITED_CANT), TYPE_NOR_OK, 1 );
				return 0;
			}
			else if ( GET_ITEM_LEVEL(pItem) > 150 )
			{
				if ( IsPlatinumItem(pItem->m_Vnum) )
				{
					pItem = pItem->m_Next;
					continue;
				}
				else if( GET_TYPE(pItem) != ITYPE_WEAPON && pItem->m_Vnum != 978 ) ///07-04-27 ���� ����.�������ϱ� ����� �÷�Ƽ���� ���� ��ȯ�� �ֱ⶧���� �����,�÷�Ƽ�������� Ȯ������   // ���� �÷�Ƽ���̳� ��ȯ ���� �������� �ƴϸ�... ///07-04-10 �Ϻ� ��û���� ó��.
				{
					//-- IDS_QUEST_ITEM_NOT_MATCH : �ٸ� ������ �������� �����ֽ��ϴ�.
					g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_QUEST_ITEM_NOT_MATCH), TYPE_NOR_OK, 1 );
					return 0;
				}
				else
				{
					//-- IDS_CANNOT_PLACE_EMERALD : ���޶��� �̻�� �������� ���� �� �����ϴ�.
					g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_CANNOT_PLACE_EMERALD), TYPE_NOR_OK, 1 );
					return 0;
				}
			}
			else
			{
				pItem = pItem->m_Next;
			}
		}
		break;

	case 2: // �ʱ�ȭ
		while (pItem)
		{
			// ��ȭ �ɼ��� �پ� ���� �ʴٸ� ���� �޽����� �ٿ���.
			int special = pItem->m_Special & (~(0x03000000));
			if( (GET_TYPE(pItem) == ITYPE_WEAPON || GET_TYPE(pItem) ==  ITYPE_ARMOR)  && special == 0)
			{
				// IDS_ENCHANT_PLZ_INIT : �ʱ�ȭ�� �Ұ����� �������Դϴ�.
				g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_ENCHANT_PLZ_INIT), TYPE_NOR_OK, 1 );
				return 0;
			}
			else
			{
				pItem = pItem->m_Next;
			}
		}
		break;
	}
	return 1;
}

BOOL Cplatinum::IsPlatinumItem(int vnum)
{
	static int platinum_vnum[5] = {979, 980, 981, 982, 983};

	for (int i=0; i < 5; i++)
	{
		if (vnum == platinum_vnum[i])
			return TRUE;
	}

	return FALSE;
}

int Cplatinum::CheckMyslot_ParentsDay()
{
	int  vnum = -1;

	CItem* pItem = g_pRoh->m_ExgInven;

	int a, b, c;

	a = b = c = 0;

	if(!pItem)
		return -1;

	while (pItem)
	{
		vnum = pItem->m_Vnum;
		if(vnum == 957)
			a++;
		else if(vnum ==958)
			b++;
		else if(vnum == 959)
			c++;
		else
		{
			//-- IDS_QUEST_ITEM_NOT_MATCH : �ٸ� ������ �������� �����ֽ��ϴ�.
			g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_QUEST_ITEM_NOT_MATCH), TYPE_NOR_OK, 1 );
			return -1;
		}
		pItem = pItem->m_Next;
	}

	if((a==1&&b==0&&c==1) ||(a==1&&b==1&&c==1))
	{
		// ����
		if( g_pTcpIp )
			g_pTcpIp->SendNetMessage("powerup\n");
	}
	else
		//-- IDS_CHECK_EVENT_ITEM : ������,Ǯ �������� Ȯ���� �ּ���
		g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_CHECK_EVENT_ITEM), TYPE_NOR_OK, 1 );

	return 1;
}



BOOL Cplatinum::IsInside(int x, int y)
{
	if( !m_pBack )
		return FALSE;

	return m_pBack->IsIn( x, y);
}

BOOL Cplatinum::IsPlayArea(int x, int y)
{
	if( !IsInside(x,y) && !g_pNk2DFrame->IsInPopup(x,y) )
		return true;

	return false;
}

void Cplatinum::SendMsg()
{
	if (g_pTcpIp)
		g_pTcpIp->SendNetMessage(m_MsgBuf);
}

void Cplatinum::Exchange(int result, int type, int vnum, int index, int gem_num)
{
	// [2/27/2007 Theodoric] ��þƮ : ��þƮ ���� �߰��� else if���� switch�� ����
	switch( result )
	{
	case 0: // �÷�ƼƬ ���� ����
		switch( type )
		{
		case 0: //-- IDS_PLATINUM_ITEM_MISMATCH : �������� �������� �ʽ��ϴ�
			g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_PLATINUM_ITEM_MISMATCH), TYPE_NOR_OK, 1 );
			break;

		case 1: //-- IDS_CHECK_ITEM_COUNT : ������ ������ Ȯ�����ּ���.
			g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_CHECK_ITEM_COUNT), TYPE_NOR_OK, 1 );
			break;

		case 2: //-- IDS_CHECK_PLATINUM_COUNT : �����÷�Ƽ���� ������ Ȯ���� �ּ���
			g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_CHECK_PLATINUM_COUNT), TYPE_NOR_OK, 1 );
			break;

		case 3: //-- IDS_PLATINUM_DEFICIT_LAIM : �÷�Ƽ�� ������ ���ñݾ��� �����մϴ�
			g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_PLATINUM_DEFICIT_LAIM), TYPE_NOR_OK, 1 );
			break;

		case 4: //-- IDS_PLATINUM_FAIL_REFINE : �÷�Ƽ�� ���ÿ� �����߽��ϴ�
			RemoveAllMyInven();
			g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_PLATINUM_FAIL_REFINE), TYPE_NOR_OK, 1 );
			break;
		}
		break;

	case 1: // �÷�Ƽ�� ���� ����
		switch( type )
		{
		case 0: //-- IDS_PLATINUM_SUCCESS_REFINE_WEAPON : �÷�Ƽ�� ���� ���ÿ� �����Ͽ����ϴ�
			ChangeTo(vnum, index, gem_num);
			g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_PLATINUM_SUCCESS_REFINE_WEAPON), TYPE_NOR_OK, 1 );
			break;
		case 1: //-- IDS_PLATINUM_SUCCESS_REFINE_ARMOR : �÷�Ƽ�� �� ���ÿ� �����Ͽ����ϴ�
			ChangeTo(vnum, index, gem_num);
			g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_PLATINUM_SUCCESS_REFINE_ARMOR), TYPE_NOR_OK, 1 );
			break;
		}
		break;

	case 5: // [2007/02/27 Theodoric] ��þƮ : ���� �޽��� ó��
		int sid = -1;
		switch( type )
		{
		case -7 : // IDS_ENCHANT_FAIL_OPTION01 "�ʱ�ȭ �� �� �����ϴ�.."
			g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_ENCHANT_FAIL_OPTION01), TYPE_NOR_OK, 1 );
			sid = IDS_ENCHANT_FAIL_OPTION01;
			break;
		case -6 : // IDS_ENCHANT_FAIL_OPTION "�ɼ��� ��� �ο��Ǿ� �ֽ��ϴ�. ������ ���Ͻø� �ʱ�ȭ �� �ٽ� �õ����ּ���."
			g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_ENCHANT_FAIL_OPTION), TYPE_NOR_OK, 1 );
			sid = IDS_ENCHANT_FAIL_OPTION;
			break;
		case -5 : // IDS_ENCHANT_FAIL_NEED_MASUK "�ʱ�ȭ �� �ƹ̰� ������ �ʿ��մϴ�. Ȯ�� �� �ٽ� �õ��� �ּ���"
			g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_ENCHANT_FAIL_NEED_MASUK), TYPE_NOR_OK, 1 );
			sid = IDS_ENCHANT_FAIL_NEED_MASUK;
			break;
		case -4 : //- �߸��� ��ȣ (������)
			g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_ENCHANT_FAILED), TYPE_NOR_OK, 1 );
			sid = IDS_ENCHANT_FAILED;
			break;
		case -3 : //- �� ���� ������
			g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_ENCHANT_FAIL_ITEM), TYPE_NOR_OK, 1 );
			sid = IDS_ENCHANT_FAIL_ITEM;
			break;
		case -2 : //- ��� ����
			g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_ENCHANT_FAIL_MONEY), TYPE_NOR_OK, 1 );
			sid = IDS_ENCHANT_FAIL_MONEY;
			break;
		case -1 : //- ��ȭ ����
			g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_ENCHANT_FAILED), TYPE_NOR_OK, 1 );
			sid = IDS_ENCHANT_FAILED;
			break;
		case  0 : //- �ʱ�ȭ ����
			g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_ENCHANT_INIT), TYPE_NOR_OK, 1 );
			sid = IDS_ENCHANT_INIT;
			break;
		case  1 : //- ��ȭ ����
			g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_ENCHANT_SUCC), TYPE_NOR_OK, 1 );
			sid = IDS_ENCHANT_SUCC;
			break;
		}

		if( sid != -1 )
			g_pNk2DFrame->AddChatStr( (char*)G_STRING(sid) , -1 );

		break;
	}
	m_bExchange = FALSE;
}

/////////////////0401
void Cplatinum::ExchangeTree(int result, int type, int vnum, int index)
{
	if(result == 0)
	{
		//����
		if(type == 1)
			g_pNk2DFrame->InsertPopup( "??????", TYPE_NOR_OK, 1 );
		else if(type == 2) //-- IDS_QUEST_ITEM_NOT_MATCH : �ٸ� ������ �������� �����ֽ��ϴ�.
			g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_QUEST_ITEM_NOT_MATCH), TYPE_NOR_OK, 1 );
		else if(type == 3) //-- IDS_ERROR : ����
			g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_ERROR), TYPE_NOR_OK, 1 );
	}

	m_bExchange = FALSE;
}
//////////////


void Cplatinum::GetMsgParentsDay(BOOL bsuccess, int vnum, int sver_idx)
{
	if(bsuccess)
		ChangeTo(vnum, sver_idx, 0);
	else
		//-- IDS_FAIL_PRODUCT : ������ �����Ͽ����ϴ�
		g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_FAIL_PRODUCT), TYPE_NOR_OK, 1 );
}


void Cplatinum::ChangeTo(int vnum, int svr_idx, int gem_num)
{
	int x_idx, y_idx;

	RemoveAllMyInven();

	if (vnum == -1)
		return;

	g_pRoh->m_ExgInven = new CItem;

	g_pRoh->m_ExgInven->m_Index = svr_idx;
	g_pRoh->m_ExgInven->m_SlotX = 0;
	g_pRoh->m_ExgInven->m_SlotY = 0;
	g_pRoh->m_ExgInven->m_Vnum = vnum;
	g_pRoh->m_ExgInven->m_ProtoNum = g_ItemList.FindItem(vnum);

	if(gem_num > 0)
	{
		if(GET_ITEM_LEVEL(g_pRoh->m_ExgInven) >= 181) //���̾Ʊ�
		{
			g_pRoh->m_ExgInven->SetDiaGemNum(gem_num);
		}
		else
			g_pRoh->m_ExgInven->SetGemNum(gem_num);
	}

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


void Cplatinum::RemoveAllMyInven()
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

void Cplatinum::QuestExchange(int money)
{

	if(money == -1)
		//-- IDS_EVENTACIENT_FAIL : ������ �����߽��ϴ�.
		g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_EVENTACIENT_FAIL), TYPE_NOR_OK, 1 );
	else
	{
		RemoveAllMyInven();
		char strTemp[128];

		//-- IDS_EVENTACIENT_HAVE_LAIM2 : %d ������ �����̽��ϴ�.
		sprintf(strTemp, (char*)G_STRING(IDS_EVENTACIENT_HAVE_LAIM2),money);
		g_pNk2DFrame->InsertPopup( strTemp, TYPE_NOR_OK, 1 );
	}
	m_bExchange= FALSE;
}
