#include "stdafx.h"


#include <string.h>
#include "main.h"
#include "basicbutton.h"
#include "headers.h"
#include "refine.h"
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
#include "LoadEffectSound.h"
#include "DirtSnd.h"
#include "alphapopup.h"
#include "g_stringmanager.h"

extern int g_EffectSound[];

extern DWORD g_dwClientCountry;



#define _TYPE_A       0   // ������ ����� ����â�� TRUE
#define _TYPE_B		  1   // �������� ����� ����â�� TRUE


CRefine::CRefine()
	: m_nType(0)
	, m_my_slotstart_x(0)
	, m_my_slotstart_y(0)
	, m_opp_slotstart_x(0)
	, m_opp_slotstart_y(0)
{
	memset(m_MsgBuf, 0, sizeof(m_MsgBuf));
	memset(m_MsOppSlot, 0, sizeof(m_MsOppSlot));
	memset(m_strMsMoney, 0, sizeof(m_strMsMoney));

	m_hWnd			= NULL;
	m_pBack			= NULL;		 // ������ ����� ����â
	m_pBtnCancel	= NULL;	 // "cancle" ��ư
	m_pBtnExecute	= NULL;	 // "����" ��ư
	m_pAlphaPopUp	= NULL;
	m_pInfo_A       = NULL;
	m_pInfo_B		= NULL;
	m_pBtnMoney		= NULL;
	m_pEnableSlot	= NULL;
	m_pDisableSlot	= NULL;
	m_pExistSlot	= NULL;
	m_MsOppInven = NULL;
	m_MsMoney = 0;
	m_bRefineCon = FALSE;
	m_bRefineLock = FALSE;
	m_bMainActive = FALSE;
	m_bClick = m_bMove = m_bMoveEnd = FALSE;
	m_strOppName[0] = NULL;
	m_pOppNameBox = NULL;

	m_pNkCha = NULL;
	m_pItemRender = NULL;
	m_pOverItem = NULL;
}

CRefine::~CRefine()
{
	DeleteRes();
	m_pNkCha = NULL;
	m_pItemRender = NULL;
	m_pOverItem = NULL;
}

void CRefine::SetOppName(char *player_name)
{
	if( player_name == NULL )
		return;

	if (strlen(player_name))
	{
		strcpy(m_strOppName, player_name);
		strcat(m_strOppName, " ");
		//-- IDS_REFINE_REFINNING : �԰� ������
		strcat(m_strOppName, G_STRING(IDS_REFINE_REFINNING));
	}
	else
		m_strOppName[0] = NULL;
}

//-------------------------------------------------------------------
// naem : LoadRes
//-------------------------------------------------------------------
void CRefine::LoadRes(int Type, CItemRender *pItemRender)
{
	if( !g_pDisplay || !pItemRender )
		return;

	m_pItemRender = pItemRender;
	m_nType = Type;
	m_bMoveEnd = FALSE;

	m_hWnd =  g_pDisplay->GetHWnd();
	GetClientRect(m_hWnd, &m_ClientRc);

	g_pDisplay->CreateSurfaceFromBitmap (&m_pEnableSlot, "interface/slot/slot_enable.bmp");
	g_pDisplay->CreateSurfaceFromBitmap (&m_pDisableSlot, "interface/slot/slot_disable.bmp");
	g_pDisplay->CreateSurfaceFromBitmap (&m_pExistSlot, "interface/slot/slot_exist.bmp");

	m_pBtnCancel = new CBasicButton();
	m_pBtnCancel->SetFileName("common/btn_close_01");
	m_pBtnCancel->LoadRes();

	m_pBtnMoney  = new CBasicButton();
	m_pBtnMoney->SetFileName("common/btn_money");
	m_pBtnMoney->LoadRes();

	if (strlen(m_strOppName))
	{
		m_pOppNameBox = new CTextOutBox();
		m_pOppNameBox->Init(12, RGB(255, 210, 0),0, 0);
		m_pOppNameBox->SetString(m_strOppName);
	}
	else
		SAFE_DELETE(m_pOppNameBox);

	{
		g_pDisplay->CreateSurfaceFromBitmap (&m_pBack, "interface/refine/refine.BMP");
		m_pBack->SetColorKey( TRANS_COLOR );
		m_pBack->Xpos = g_pNk2DFrame->GetClientWidth() - 291 - m_pBack->GetWidth();
		m_pBack->Ypos = 0;

		m_my_slotstart_x = m_pBack->Xpos + 35;
		m_my_slotstart_y = m_pBack->Ypos + 286;

		m_opp_slotstart_x = m_pBack->Xpos + 35;
		m_opp_slotstart_y = m_pBack->Ypos + 80;

		m_pBtnCancel->SetPosition(m_pBack->Xpos+214, m_pBack->Ypos+6);
		m_pBtnMoney->SetPosition(m_pBack->Xpos+36, m_pBack->Ypos+212);

	}


	// ������ ����� ����â�� TRUE
	if(m_nType == _TYPE_A)
	{
		Load_A();
		if (m_pOppNameBox)
			m_pOppNameBox->SetPos(m_pBack->Xpos+36, m_pBack->Ypos+56);
	}
	// �������� ����� ����â�� TRUE
	else
	{
		Load_B();
		if (m_pOppNameBox)
			m_pOppNameBox->SetPos(m_pBack->Xpos+36, m_pBack->Ypos+56);
	}

	m_MoneyText.Init(12, RGB(255, 255, 0), 0, 0);
	m_MoneyText.SetPos(m_pBack->Xpos+71, m_opp_slotstart_y+138);
	strcpy(m_strMsMoney, "0");
	m_MsMoney = 0;
	m_MoneyText.SetString(_atoi64(m_strMsMoney));

	m_bMainActive = TRUE;
	m_bRefineCon = TRUE;
}

void CRefine::Load_A()
{
	g_pDisplay->CreateSurfaceFromBitmap (&m_pInfo_A, "interface/description/refine/refine_info_a.bmp");

	{
		m_pInfo_A->Xpos = m_pBack->Xpos + 35;
		m_pInfo_A->Ypos = m_pBack->Ypos + 81;

	}
}

void CRefine::Load_B()
{
	m_pBtnExecute = new CBasicButton();
	m_pBtnExecute->SetFileName("common/btn_ok_03"); // ������ư
	m_pBtnExecute->LoadRes();
	// �ʱ⿡�� ������ �� �� ����.
	m_pBtnExecute->SetState(BTN_DISABLE);

	g_pDisplay->CreateSurfaceFromBitmap (&m_pInfo_B, "interface/description/refine/refine_info_b.bmp");

	{
		m_pBtnExecute->SetPosition(m_pBack->Xpos+67, m_pBack->Ypos+404);
		m_pInfo_B->Xpos = m_pBack->Xpos + 35;
		m_pInfo_B->Ypos = m_pBack->Ypos + 287;
	}
}

//-------------------------------------------------------------------
// naem : DeleteRes()
//-------------------------------------------------------------------
void CRefine::DeleteRes()
{
	SAFE_DELETE(m_pEnableSlot);
	SAFE_DELETE(m_pDisableSlot);
	SAFE_DELETE(m_pExistSlot);
	SAFE_DELETE(m_pBack);
	SAFE_DELETE(m_pBtnCancel);
	SAFE_DELETE(m_pBtnMoney);
	SAFE_DELETE(m_pOppNameBox);
	m_MoneyText.DeleteRes();

	// ������ ����� ����â�� TRUE
	if(m_nType == _TYPE_A)
	{
		Del_A();
	}
	// �������� ����� ����â�� TRUE
	else
	{
		Del_B();
	}

	RemoveOppInven();
}

void CRefine::Del_A()
{
	SAFE_DELETE(m_pInfo_A);
}
void CRefine::Del_B()
{
	SAFE_DELETE(m_pBtnExecute);
	SAFE_DELETE(m_pInfo_B);
}

void CRefine::RemoveOppInven()
{
	CItem *pItem = NULL;
	CItem *nextItem = NULL;

	pItem = m_MsOppInven;

	while (pItem)
	{
		nextItem = pItem->m_Next;
		delete pItem;
		pItem = nextItem;
	}
	m_MsOppInven = NULL;

	int i, j;

	for ( i=0; i < EXG_SLOT_X_NUM; i++)
	{
		for ( j = 0; j < EXG_SLOT_Y_NUM; j++)
		{
			m_MsOppSlot[i][j] = -1;
		}
	}
}


//-------------------------------------------------------------------
// name : Draw
//-------------------------------------------------------------------
CItem* CRefine::Draw()
{
	if(!m_bMainActive)
		return NULL;

	m_pOverItem = NULL;

	g_pDisplay->Blt(m_pBack->Xpos, m_pBack->Ypos, m_pBack);
	m_pBtnCancel->Draw();
	m_pBtnMoney->Draw();

	if(m_nType == _TYPE_A)
	{
		Draw_A();
		DrawMySlot();
	}
	else
	{
		Draw_B();
		DrawOppSlot();
	}

	m_MoneyText.Draw(m_pBack->Xpos+70 , m_pBack->Ypos+217 , 127);

	if (m_pOppNameBox && g_dwClientCountry != CTRY_TH ) // �±��� ���� ������ �ټ� ������ ���� ����� �̸��� ��� ���Ѵ�.(by ����)
		m_pOppNameBox->Draw(160);

	return m_pOverItem;
}



void  CRefine::Draw_A()
{
	g_pDisplay->Blt(m_pInfo_A->Xpos, m_pInfo_A->Ypos, m_pInfo_A);
}

void  CRefine::Draw_B()
{
	m_pBtnExecute->Draw();
	g_pDisplay->Blt(m_pInfo_B->Xpos, m_pInfo_B->Ypos, m_pInfo_B);
}

// [6/7/2007 Theodoric] ������ ���� :: �Լ� ���� �߰�.
void CRefine::MsInvenTo(int x, int y, int index, int vnum, int gem_num, int special,
						int UpEndur, int MaxUpdEndur, int Endur, int MaxEndur)
{
	char plus = '+';
	char tmp[10];
	if (vnum < 0)
		return;

	int proto_num = g_ItemList.FindItem(vnum);
	if (proto_num >= 0)
	{
		CItem *pItem = new CItem;

		pItem->m_Index = index;
		pItem->m_Vnum = vnum;
		pItem->m_ProtoNum = proto_num;

		// ������ ���� �Է��غ���!
		ResetItemValue(pItem);
		// +�������� ��� CTextOutBox ����
		if (gem_num > 0)
		{
			//-- 2004. 03. 22  / Lyul
			//-- ������ ���̾� ��� �ν� ���� Big Fix
			if(GET_ITEM_LEVEL(pItem)>=181)
				pItem->SetDiaGemNum(gem_num);
			else
				pItem->SetGemNum(gem_num);

			if(pItem->m_pTextOutBox)
				SAFE_DELETE(pItem->m_pTextOutBox);

			pItem->m_pTextOutBox = new CTextOutBox();
			pItem->m_pTextOutBox->Init(MS_TEXT_SIZE, MS_TEXT_COLOR, 0, 0);
			sprintf(tmp, "%c%d",plus, pItem->m_PlusNum);
			pItem->m_pTextOutBox->SetString(tmp, MS_TEXT_COLOR, MS_BAKC_COLOR);
		}

		// �� �ڸ��� ���� ������ ���ش�.
		MsInvenFrom(x, y, GET_ITEM_WIDTH(pItem), GET_ITEM_HEIGHT(pItem));

		pItem->m_SlotX = x;
		pItem->m_SlotY = y;
		pItem->m_Special = special;
		SetSpecialValue(pItem, GET_TYPE(pItem), GET_SHAPE(pItem), special, 0);

		pItem->m_UpgradeEndurance = UpEndur;
		pItem->m_MaxUpgradeEndurance = MaxUpdEndur;
		pItem->m_Endurance = Endur;
		pItem->m_MaxEndurance = MaxEndur;

		pItem->m_Next = m_MsOppInven;
		m_MsOppInven = pItem;

		for (int x_idx = 0; x_idx < GET_ITEM_WIDTH(pItem); x_idx++)
		{
			for (int y_idx = 0; y_idx < GET_ITEM_HEIGHT(pItem); y_idx++)
			{
				m_MsOppSlot[x+x_idx][y+y_idx] = index;
			}
		}
	}
}

int CRefine::GetOppSlotIndex(int x, int y)
{
	if (x < 0 || x >= EXG_SLOT_X_NUM || y < 0 || y >= EXG_SLOT_Y_NUM)
		return -1;

	return m_MsOppSlot[x][y];
}

//--------------------------------------------------------------------------
// ����� ��ȯâ �κп��� �������� ���ٶ�.
//--------------------------------------------------------------------------
int CRefine::GetItemIndexInOppSlot(int x, int y, int w, int h)
{
	int index = -1;

	for (int x_idx = x; x_idx < x+w; x_idx++)
	{
		for (int y_idx = y; y_idx < y+h; y_idx++)
		{
			if (m_MsOppSlot[x_idx][y_idx] != -1)
			{
				if (index == -1)
					index = m_MsOppSlot[x_idx][y_idx];
				else if (index != m_MsOppSlot[x_idx][y_idx])
					return -2;
			}
		}
	}

	return index;
}

CItem * CRefine::PickItemInOppSlot(int index, BOOL isVnum)
{
	CItem *beforeItem = NULL;
	CItem *pItem = m_MsOppInven;

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
				m_MsOppInven = pItem->m_Next;
			}

			for (int x_idx = 0; x_idx < GET_ITEM_WIDTH(pItem); x_idx++)
			{
				for (int y_idx = 0; y_idx < GET_ITEM_HEIGHT(pItem); y_idx++)
				{
					m_MsOppSlot[pItem->m_SlotX+x_idx][pItem->m_SlotY+y_idx] = -1;
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

void CRefine::MsInvenFrom(int x, int y, int w, int h)
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

void CRefine::MsMoney(char *strMoney)
{
	if( strMoney == NULL )
		return;

	if (strlen(strMoney) < 11)
	{
		strcpy(m_strMsMoney, strMoney);
		m_MsMoney = _atoi64(m_strMsMoney);
		if (m_MsMoney == 0)
			m_MoneyText.SetString(m_MsMoney);
		else
			m_MoneyText.SetString(m_MsMoney, true);
	}

	// ������ư�� enable ���ش�.
	if (m_nType != _TYPE_A)
	{
		m_pBtnExecute->SetState(BTN_NORMAL);
	}
}

void CRefine::MsSuccess(int clear_flag, int vnum, int index, int x, int y)
{
	if (m_nType == _TYPE_A)
	{
		// ��û��. (�ϴ�)
		if(clear_flag)
			RemoveAllMyInven();

		if (vnum == -1)
			return;

		// [6/1/2007 Theodoric] ������ ���� :: �ĸ����� �߰�
		g_pRoh->AddToInven(3, x, y, index, vnum, 0, 0, 0, 0, 0, 0, 0); // 05-10-10 �ɼ��÷��� 2 �߰� (����) ///�Ź���ȭ�ý���

		// �ݾ� ���� �ʱ�ȭ.
		strcpy(m_strMsMoney, "0");
		m_MsMoney = _atoi64(m_strMsMoney);
		m_MoneyText.SetString(m_MsMoney);

		m_bRefineLock = FALSE;
	}
	else
	{
		// �������� ���. (���)
		if(clear_flag)
			RemoveOppInven();
		MsInvenTo(x, y, index, vnum, 0, 0, 0, 0, 0, 0);

		// �ݾ� ǥ�� �ʱ�ȭ.
		strcpy(m_strMsMoney, "0");
		m_MsMoney = _atoi64(m_strMsMoney);
		m_MoneyText.SetString(m_MsMoney);

		// ���� ��ư�� �𽺿��̺� �Ѵ�.
		if(m_pBtnExecute)
			m_pBtnExecute->m_iBtnFlg = BTN_DISABLE;
	}
}

//-------------------------------------------------------------------
// name : MsgProc
//-------------------------------------------------------------------
LRESULT CRefine::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int x, y;
	int ret = 0;
	int ren = -1;

	x = LOWORD (lParam);
	y = HIWORD (lParam);

	switch ( msg )
	{

	case WM_KEYDOWN:
	case WM_CHAR:
		if(m_nType == _TYPE_A)
		{
			ret = Message_A(  hWnd,  msg,  wParam,  lParam);
		}
		break;
	case WM_MOUSEMOVE:
	case WM_LBUTTONUP:
	case WM_LBUTTONDOWN:

		if(m_nType == _TYPE_A)
			ret = Message_A(  hWnd,  msg,  wParam,  lParam);
		else
			ret = Message_B( hWnd, msg, wParam, lParam);

		break;
	}
	return ret;
}




LRESULT CRefine::Message_A( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int x, y;

	switch (msg)
	{
	case WM_LBUTTONUP:
	case WM_MOUSEMOVE:
		x = LOWORD (lParam);
		y = HIWORD (lParam);
		if (!IsInside(x, y))
			return 0;

		m_pBtnCancel->MsgProc(hWnd, msg, wParam, lParam);
		m_pBtnMoney->MsgProc(hWnd, msg, wParam, lParam);

		if (m_pBtnCancel->GetState() == BTN_ACTION)
		{
			sprintf(m_MsgBuf, "ms reject\n");
			SendMsg();
			g_pNk2DFrame->ShowRefineWindow(FALSE, m_nType);
			return 1;
		}
		if (m_pBtnMoney->GetState() == BTN_ACTION)
		{
			//-- IDS_REFINE_INPUT_MONEY : �ݾ��� �Է��ϼ���
			g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_REFINE_INPUT_MONEY), TYPE_EXCHANGE, 21 );
		}
		m_pBtnCancel->SetState(BTN_NORMAL);
		m_pBtnMoney->SetState(BTN_NORMAL);
		return 1;

	case WM_LBUTTONDOWN:
		x = LOWORD (lParam);
		y = HIWORD (lParam);

		if (IsInside(x, y))
		{
			if (m_pBtnCancel->IsInside(x, y))
				m_pBtnCancel->MsgProc(hWnd, msg, wParam, lParam);
			else if (m_pBtnMoney->IsInside(x, y))
				m_pBtnMoney->MsgProc(hWnd, msg, wParam, lParam);
			else if (x >= m_my_slotstart_x
					 && x < m_my_slotstart_x + (SLOT_WIDTH * EXG_SLOT_X_NUM)
					 && y >= m_my_slotstart_y
					 && y < m_my_slotstart_y + (SLOT_HEIGHT * EXG_SLOT_Y_NUM) )
			{
				int slot_x_num = 0, slot_y_num = 0;
				if (m_bRefineLock)
					return 1;

				// ����Ʈ�� ���Կ� �ִ� ���, ��ȯâ ���Կ� ���´�.
				if (m_pNkCha->m_ExtraSlot)
				{
					if (GET_ITEM_WIDTH(m_pNkCha->m_ExtraSlot) % 2)
						slot_x_num = (x - m_my_slotstart_x) / SLOT_WIDTH - (GET_ITEM_WIDTH(m_pNkCha->m_ExtraSlot)/2);
					else
						slot_x_num = (x - m_my_slotstart_x + (SLOT_WIDTH/2)) / SLOT_WIDTH - (GET_ITEM_WIDTH(m_pNkCha->m_ExtraSlot)/2);

					if (GET_ITEM_HEIGHT(m_pNkCha->m_ExtraSlot) % 2)
						slot_y_num = (y - m_my_slotstart_y) / SLOT_HEIGHT - (GET_ITEM_HEIGHT(m_pNkCha->m_ExtraSlot)/2);
					else
						slot_y_num = (y - m_my_slotstart_y + (SLOT_HEIGHT/2)) / SLOT_HEIGHT - (GET_ITEM_HEIGHT(m_pNkCha->m_ExtraSlot)/2);

					if (slot_x_num >= 0
							&& slot_x_num + GET_ITEM_WIDTH(m_pNkCha->m_ExtraSlot) - 1 < EXG_SLOT_X_NUM
							&& slot_y_num >= 0
							&& slot_y_num + GET_ITEM_HEIGHT(m_pNkCha->m_ExtraSlot) - 1 < EXG_SLOT_Y_NUM)
					{
						if (m_pNkCha->AddToExgInven(slot_x_num, slot_y_num))
						{
							sprintf(m_MsgBuf, "ms inven %d %d\n", slot_x_num, slot_y_num);
							SendMsg();
						}
					}
				}
				else
				{
					// �������� ���� ��.
					if (m_pNkCha->AddToExgInven((x-m_my_slotstart_x) / SLOT_WIDTH, (y-m_my_slotstart_y) / SLOT_HEIGHT))
					{
						sprintf(m_MsgBuf, "ms inven %d %d\n", (x-m_my_slotstart_x) / SLOT_WIDTH, (y-m_my_slotstart_y) / SLOT_HEIGHT);
						SendMsg();
					}
				}
			}
		}
		else if (IsPlayArea(x, y))
		{
			if (m_bRefineCon)
				CloseConnection();

			g_pNk2DFrame->ShowRefineWindow(FALSE, m_nType);
		}
		else
			return 0;

		return 1;
		break;
	}
	return 0;
}





LRESULT CRefine::Message_B( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int x, y;
	int hr = -1;

	switch (msg)
	{
	case WM_LBUTTONUP:
	case WM_MOUSEMOVE:
		x = LOWORD (lParam);
		y = HIWORD (lParam);
		if (!IsInside(x, y))
			return 0;

		m_pBtnCancel->MsgProc(hWnd, msg, wParam, lParam);

		if(m_pBtnExecute->GetState() != BTN_DISABLE)
			m_pBtnExecute->MsgProc(hWnd, msg, wParam, lParam);

		if (m_pBtnCancel->GetState() == BTN_ACTION)
		{
			sprintf(m_MsgBuf, "ms reject\n");
			SendMsg();
			m_pBtnCancel->SetState(BTN_NORMAL);

			g_pNk2DFrame->ShowRefineWindow(FALSE, m_nType);
			return 1;
		}

		if (m_pBtnExecute->GetState() == BTN_ACTION)
		{
			if ( IsRefinableItems() )
			{
				sprintf(m_MsgBuf, "ms upgrade\n");
				SendMsg();
				m_pBtnExecute->SetState(BTN_DISABLE);
			}
			else
				m_pBtnExecute->SetState(BTN_NORMAL);
		}

		m_pBtnMoney->SetState(BTN_DISABLE);

		return 1;

	case WM_LBUTTONDOWN:
		x = LOWORD (lParam);
		y = HIWORD (lParam);

		if (IsInside(x, y))
		{
			if (m_pBtnCancel->IsInside(x, y))
				m_pBtnCancel->MsgProc(hWnd, msg, wParam, lParam);
			else if (m_pBtnExecute->IsInside(x, y))
			{
				if(m_pBtnExecute->GetState() != BTN_DISABLE)
					m_pBtnExecute->MsgProc(hWnd, msg, wParam, lParam);
			}
		}
		else if (IsPlayArea(x, y))
		{
			if (m_bRefineCon)
				CloseConnection();

			g_pNk2DFrame->ShowRefineWindow(FALSE, m_nType);
		}
		else
			return 0;

		return 1;
		break;
	}
	return 0;
}


#define GET_OPP_SLOT_X(point_x)		((point_x - m_opp_slotstart_x) / SLOT_WIDTH)
#define GET_OPP_SLOT_Y(point_y)		((point_y - m_opp_slotstart_y) / SLOT_HEIGHT)


void CRefine::DrawOppSlot()
{
	int xpos, ypos;

	if (g_pDisplay && m_pBack)
	{
		RECT rcRect;
		POINT point;
		GetCursorPos(&point);
		ScreenToClient(g_pDisplay->GetHWnd(), &point);

		// �������� ��ġ�� ���� ǥ��
		CItem *pItem = m_MsOppInven;
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

		// Extra Slot�� �ִ� �������� ���� �� ǥ��
		int slot_x_num, slot_y_num;

		// �� ��ȯǰ �׸���..
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

		int index = GetOppSlotIndex(slot_x_num, slot_y_num);
		pItem = m_MsOppInven;
		m_pItemRender->BeginRenderLolo();				// �߿�....... �߿�..

		while (pItem)
		{
			if (pItem->m_Index == index)
			{
				g_pNk2DFrame->RenderItemInUI(m_opp_slotstart_x + pItem->m_SlotX * SLOT_WIDTH,
											 m_opp_slotstart_y + pItem->m_SlotY * SLOT_HEIGHT,
											 pItem , TRUE , FALSE, FALSE, TRUE );
				m_pOverItem = pItem;
			}
			else
			{
				DWORD ambient = 0x00555555;
				if (g_pNk2DFrame->IsScroll(pItem) )
					ambient = 0x00cccccc;
				else if (pItem->m_PlusNum > 0)
					ambient = UPGRADE_ITEM_COLOR;

				g_pNk2DFrame->RenderItemInUI(m_opp_slotstart_x + pItem->m_SlotX * SLOT_WIDTH,
											 m_opp_slotstart_y + pItem->m_SlotY * SLOT_HEIGHT,
											 pItem , FALSE , FALSE , FALSE, TRUE );
				//xpos = m_opp_slotstart_x + pItem->m_SlotX * SLOT_WIDTH + (GET_ITEM_WIDTH(pItem) * SLOT_WIDTH - ITEM_RENDER_WIDTH) /2;
				//ypos = m_opp_slotstart_y + pItem->m_SlotY * SLOT_HEIGHT + (GET_ITEM_HEIGHT(pItem) * SLOT_HEIGHT - ITEM_RENDER_HEIGHT) /2;
				int cx, cy;

				xpos = m_opp_slotstart_x + (pItem->m_SlotX*SLOT_WIDTH);
				ypos = m_opp_slotstart_y + (pItem->m_SlotY*SLOT_HEIGHT);
				cx = SLOT_WIDTH; // * GET_ITEM_WIDTH(pItem);
				cy = SLOT_HEIGHT;// * GET_ITEM_HEIGHT(pItem);

				// +������ ������ ����� �����ؼ� ����Ѵ�
				if(	pItem->m_pTextOutBox)
					pItem->m_pTextOutBox->Draw(xpos+((cx/2) - (MS_TEXT_SIZE/2)), ypos+((cy/2) - (MS_TEXT_SIZE/2)));
			}
			pItem = pItem->m_Next;
		}
		m_pItemRender->EndRenderLolo();
	}
}



//-------------------------------------------------------------------
// name : DrawMySlot
//-------------------------------------------------------------------

#define GET_MY_SLOT_X(point_x)		((point_x - m_my_slotstart_x) / SLOT_WIDTH)
#define GET_MY_SLOT_Y(point_y)		((point_y - m_my_slotstart_y) / SLOT_HEIGHT)

void CRefine::DrawMySlot()
{
	RECT rcRect;
	POINT point;
	GetCursorPos(&point);
	ScreenToClient(g_pDisplay->GetHWnd(), &point);

	// �������� ��ġ�� ���� ǥ��
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

	// Extra Slot�� �ִ� �������� ���� �� ǥ��
	int slot_x_num, slot_y_num;
	if (m_pNkCha->m_ExtraSlot)
	{
		if (point.x >= m_my_slotstart_x
				&& point.x < m_my_slotstart_x + (SLOT_WIDTH * EXG_SLOT_X_NUM )
				&& point.y >= m_my_slotstart_y
				&& point.y < m_my_slotstart_y + (SLOT_HEIGHT * EXG_SLOT_Y_NUM) )
		{
			if (GET_ITEM_WIDTH(m_pNkCha->m_ExtraSlot) % 2)
				slot_x_num = (point.x - m_my_slotstart_x) / SLOT_WIDTH - (GET_ITEM_WIDTH(m_pNkCha->m_ExtraSlot)/2);
			else
				slot_x_num = (point.x - m_my_slotstart_x + (SLOT_WIDTH/2)) / SLOT_WIDTH - (GET_ITEM_WIDTH(m_pNkCha->m_ExtraSlot)/2);

			if (GET_ITEM_HEIGHT(m_pNkCha->m_ExtraSlot) % 2)
				slot_y_num = (point.y - m_my_slotstart_y) / SLOT_HEIGHT - (GET_ITEM_HEIGHT(m_pNkCha->m_ExtraSlot)/2);
			else
				slot_y_num = (point.y - m_my_slotstart_y + (SLOT_HEIGHT/2)) / SLOT_HEIGHT - (GET_ITEM_HEIGHT(m_pNkCha->m_ExtraSlot)/2);

			if (slot_x_num >= 0
					&& slot_x_num + GET_ITEM_WIDTH(m_pNkCha->m_ExtraSlot) - 1 < EXG_SLOT_X_NUM
					&& slot_y_num >= 0
					&& slot_y_num + GET_ITEM_HEIGHT(m_pNkCha->m_ExtraSlot) - 1 < EXG_SLOT_Y_NUM)
			{
				rcRect.left = 0;
				rcRect.right = SLOT_WIDTH * GET_ITEM_WIDTH(m_pNkCha->m_ExtraSlot);
				rcRect.top = 0;
				rcRect.bottom = SLOT_HEIGHT * GET_ITEM_HEIGHT(m_pNkCha->m_ExtraSlot);
				if (m_pNkCha->GetItemIndexInExgSlot(slot_x_num, slot_y_num,
													GET_ITEM_WIDTH(m_pNkCha->m_ExtraSlot), GET_ITEM_HEIGHT(m_pNkCha->m_ExtraSlot)) >= -1)
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

	int index = m_pNkCha->GetExgSlotIndex(slot_x_num, slot_y_num);
	pItem = m_pNkCha->m_ExgInven;
	m_pItemRender->BeginRenderLolo();		// �߿�..  m_pItemRender->RenderLolo�� �Ķ���Ϳ� ������ �ش�.

	while (pItem)
	{
		if (pItem->m_Index == index)
		{
			g_pNk2DFrame->RenderItemInUI(m_my_slotstart_x + pItem->m_SlotX * SLOT_WIDTH,
										 m_my_slotstart_y + pItem->m_SlotY * SLOT_HEIGHT,
										 pItem , TRUE , FALSE, FALSE, TRUE );
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
										 pItem , FALSE , FALSE, FALSE, TRUE );
		}
		pItem = pItem->m_Next;
	}
	m_pItemRender->EndRenderLolo();
}

BOOL CRefine::CheckMoneyStr(char* szMoney)
{
	int nMoney;
	// �Էµ� ��Ʈ���� �˻��Ѵ�
	int len = strlen(szMoney);
	for(int i=0; i<len; i++)
	{
		if(isdigit(szMoney[i]) == FALSE)
		{
			if(is_space(szMoney[i]) ==TRUE)
				//-- IDS_EXCHANGE_UNABLE_BLANK : ������ ���Ե� �� �����ϴ�
				g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_EXCHANGE_UNABLE_BLANK), TYPE_EXCHANGE, 21 );
			else
				//-- IDS_EXCHANGE_UNABLE_STRING : ���ڸ� �Է����ּ���
				g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_EXCHANGE_UNABLE_STRING), TYPE_EXCHANGE, 21 );

			return FALSE;
		}
	}
	// 3. ���� ������ �ִ� �����ݾ׺��� �Էµ� �ݾ��� ��ū���
	nMoney = _atoi64(szMoney);

	if (nMoney < 0)
	{
		//-- IDS_EXCHANGE_FAULT_STR : ������ �ʴ� ���Դϴ�
		g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_EXCHANGE_FAULT_STR), TYPE_EXCHANGE, 21 );
		return FALSE;
	}

	if(m_pNkCha->m_Money < nMoney)
	{
		//-- IDS_EXCHANGE_SCARCE_MONEY : �ݾ��� �����մϴ�.
		g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_EXCHANGE_SCARCE_MONEY), TYPE_EXCHANGE, 21 );
		return FALSE;
	}

	return TRUE;
}

BOOL CRefine::IsInside(int x, int y)
{
	if( !m_pBack )
		return FALSE;

	return m_pBack->IsIn( x, y);
}

BOOL CRefine::IsPlayArea(int x, int y)
{
	if( !IsInside(x,y) && !g_pNk2DFrame->IsInPopup(x,y) )
		return true;

	return false;
}

void CRefine::SendMsg()
{
	if (g_pTcpIp)
		g_pTcpIp->SendNetMessage(m_MsgBuf);
}

void CRefine::RemoveAllMyInven()
{

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
}

// ------------------------------------------------------------------
// ResetConnection()
// Desc : ���� ������ �� ���¿��� ������� �޽����� ������ ���� ������ ���´�.
// ------------------------------------------------------------------
void CRefine::CloseConnection()
{
	m_bRefineCon = FALSE;
	m_bRefineLock = FALSE;
	sprintf(m_MsgBuf, "ms reject\n");
	SendMsg();
}

// ------------------------------------------------------------------
// ResetConnection()
// Desc : ���� ������ �� ���¿��� �� �ʸ� ������ ���´�.
// ------------------------------------------------------------------
void CRefine::ResetConnection()
{
	m_bRefineCon = FALSE;
	m_bRefineLock = FALSE;

	if (m_nType == _TYPE_B)	// ������ ���ִ� ���̵���� ���.
		RemoveOppInven();
}

BOOL IsRefiner(int vnum)
{
	if (vnum == 555 || vnum == 556 || vnum == 557)
		return TRUE;
	else
		return FALSE;
}
// ------------------------------------------------------------------
// IsRefinalbeItems()
// Desc : ������ �� �ִ� �����۵��� ����� �ִ°� Ȯ���Ѵ�.
// ------------------------------------------------------------------
BOOL CRefine::IsRefinableItems()
{
	int  ms_count,  wash_count, level_index;
	int wash_list[4] = {555,556,557,558};	// ������.
	int ms_list[4] = {550,551,552,553};
	int upgrade_list [4][4] = {{2,555,550,551},{2,556,551,552},{2,557,552,553},{2,558,553,554}};

	CItem *items = m_MsOppInven;

	wash_count = 0;
	ms_count = 0;
	level_index = -1;

	return TRUE;
}

void CRefine::AskMsRequest(char *player_name)
{
	char strTemp[128] = "";
	//-- IDS_NK2D_REQUIRE_REFINE : %s�Կ��Լ� ������û�� �ֽ��ϴ�
	sprintf(strTemp, G_STRING(IDS_NK2D_REQUIRE_REFINE), player_name);
	g_pNk2DFrame->InsertPopup(strTemp,TYPE_NOR_OKCANCLE, 12);
}

LRESULT CRefine::PopupMsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, int nMsg)
{
	char commOutBuf[128] = "";
	if (nMsg == 1)
	{
		// ���� ��û �³� �޽����� ������.
		sprintf(commOutBuf, "ms accept\n");
		m_bRefineCon = TRUE;
	}
	else if (nMsg == -1)
		sprintf(commOutBuf, "ms reject\n");

	if (g_pTcpIp)
		g_pTcpIp->SendNetMessage(commOutBuf);

	return 1;
}

LRESULT CRefine::MoneyPopupMsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, int nMsg, char* szMoney)
{
	// "Ȯ��" ��ư�� �������
	if(nMsg == 1)
	{
		if(CheckMoneyStr(szMoney) == TRUE)
		{
			strcpy(m_strMsMoney, szMoney);
			__int64 money = _atoi64(m_strMsMoney);

			//////////////////////////////////////////////////////////
			// ������ : �̿���
			// ������ : 05-02-24
			// �������� : ���� �ݾ� 1000���� �̸��� ������ 1000�������� ����
			if( money < 1000 ) // 1000���� �̸���...
			{
				money = 1000; // ������ 1000����
				sprintf( m_strMsMoney, "%d", money ); // �������� �� ��Ʈ������ ������ ��Ʈ���� �ٲ�����Ѵ�.
			}
			//////////////////////////////////////////////////////////

			if (money >= m_MsMoney)
			{
				m_bRefineLock = TRUE;
				if (money == 0)
					m_MoneyText.SetString(money);
				else
					m_MoneyText.SetString(money, true);

				// ����ٰ� ���� ��� ������.
				sprintf(m_MsgBuf, "ms money %s\n", m_strMsMoney);	// �����̽� ����.
				SendMsg();
				m_MsMoney = money;
			}
		}
	}

	return 0L;
}