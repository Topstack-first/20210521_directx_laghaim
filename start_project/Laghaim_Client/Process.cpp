#include "stdafx.h"


#include <string.h>
#include "main.h"
#include "basicbutton.h"
#include "headers.h"
#include "Process.h"
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

#define _TYPE_A       0   // 가공할 사람의 가공창이 TRUE
#define _TYPE_B		  1   // 가공해줄 사람의 가공창이 TRUE
#define GET_OPP_SLOT_X(point_x)		((point_x - m_opp_slotstart_x) / SLOT_WIDTH)
#define GET_OPP_SLOT_Y(point_y)		((point_y - m_opp_slotstart_y) / SLOT_HEIGHT)

CProcess::CProcess()
	: m_nType(0)
	, m_my_slotstart_x(0)
	, m_my_slotstart_y(0)
	, m_opp_slotstart_x(0)
	, m_opp_slotstart_y(0)
{
	memset(m_MsgBuf, 0, sizeof(m_MsgBuf));
	memset(m_ProcessOppSlot, 0, sizeof(m_ProcessOppSlot));
	memset(m_strProcessMoney, 0, sizeof(m_strProcessMoney));

	m_hWnd			= NULL;
	m_pBack			= NULL;		 // 가공할 사람의 가공창
	m_pBtnCancel	= NULL;	 // "cancle" 버튼
	m_pBtnExecute	= NULL;	 // "가공" 버튼
	m_pAlphaPopUp	= NULL;
	m_pInfo_A       = NULL;
	m_pInfo_B		= NULL;
	m_pBtnMoney		= NULL;
	m_pEnableSlot	= NULL;
	m_pDisableSlot	= NULL;
	m_pExistSlot	= NULL;
	m_ProcessOppInven = NULL;
	m_ProcessMoney = 0;
	m_bProcessCon = FALSE;
	m_bProcessLock = FALSE;
	m_bMainActive = FALSE;
	m_bClick = m_bMove = m_bMoveEnd = FALSE;
	m_strOppName[0] = NULL;
	m_pOppNameBox = NULL;

	m_pNkCha = NULL;
	m_pItemRender = NULL;
	m_pOverItem = NULL;
	m_pTcpIp = NULL;
}

CProcess::~CProcess()
{
	DeleteRes();

	m_pNkCha = NULL;
	m_pItemRender = NULL;
	m_pOverItem = NULL;
	m_pTcpIp = NULL;
}

void CProcess::SetOppName(char *player_name)
{
	if( player_name == NULL )
		return;

	if (strlen(player_name))
	{
		strcpy(m_strOppName, player_name);
		strcat(m_strOppName, " ");
		//-- IDS_PROCESS_PROCESSING : 님과 정련중
		strcat(m_strOppName, G_STRING(IDS_PROCESS_PROCESSING));
	}
	else
		m_strOppName[0] = NULL;
}

//-------------------------------------------------------------------
// naem : LoadRes
//-------------------------------------------------------------------
void CProcess::LoadRes(int Type, CItemRender *pItemRender)
{
	if( !pItemRender )
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

	g_pDisplay->CreateSurfaceFromBitmap (&m_pBack, "interface/process/process.bmp");
	m_pBack->SetColorKey( TRANS_COLOR );
	m_pBack->Xpos = g_pNk2DFrame->GetClientWidth() - 291 - m_pBack->GetWidth();
	m_pBack->Ypos = 0;

	m_my_slotstart_x = m_pBack->Xpos + 34;
	m_my_slotstart_y = m_pBack->Ypos + 285;

	m_opp_slotstart_x = m_pBack->Xpos + 34;
	m_opp_slotstart_y = m_pBack->Ypos + 80;

	m_pBtnCancel->SetPosition(m_pBack->Xpos+214, m_pBack->Ypos+6);
	m_pBtnMoney->SetPosition(m_pBack->Xpos+35, m_pBack->Ypos+213);


	// 가공할 사람의 가공창이 TRUE
	if(m_nType == _TYPE_A)
	{
		Load_A();
		if (m_pOppNameBox)
			m_pOppNameBox->SetPos(m_pBack->Xpos+40, m_opp_slotstart_y+4);
	}
	// 가공해줄 사람의 가공창이 TRUE
	else
	{
		Load_B();
		if (m_pOppNameBox)
			m_pOppNameBox->SetPos(m_pBack->Xpos+40, m_my_slotstart_y+8);
	}

	m_MoneyText.Init(12, RGB(255, 255, 0), 0, 0);
	m_MoneyText.SetPos(m_pBack->Xpos+71, m_opp_slotstart_y+138);
	strcpy(m_strProcessMoney, "0");
	m_ProcessMoney = 0;
	m_MoneyText.SetString(_atoi64(m_strProcessMoney));

	m_bMainActive = TRUE;
	m_bProcessCon = TRUE;
}

void CProcess::Load_A()
{
	g_pDisplay->CreateSurfaceFromBitmap (&m_pInfo_A, "interface/description/process/process_info_a.bmp");

	m_pInfo_A->Xpos = m_pBack->Xpos + 34;
	m_pInfo_A->Ypos = m_pBack->Ypos + 80;
}

void CProcess::Load_B()
{
	m_pBtnExecute = new CBasicButton();
	m_pBtnExecute->SetFileName("common/btn_ok_03"); // 정제버튼
//	m_pBtnExecute->SetFileName("process/btn_execute2"); // 가공버튼
	m_pBtnExecute->LoadRes();
	// 초기에는 가공를 할 수 없다.
	m_pBtnExecute->SetState(BTN_DISABLE);

	g_pDisplay->CreateSurfaceFromBitmap (&m_pInfo_B, "interface/description/process/process_info_b.bmp");


	m_pBtnExecute->SetPosition(m_pBack->Xpos+67, m_pBack->Ypos+404);
	//m_pBtnExecute->SetPosition(m_pBack->Xpos+48, m_pBack->Ypos+239);
	m_pInfo_B->Xpos = m_pBack->Xpos + 34;
	m_pInfo_B->Ypos = m_pBack->Ypos + 285;
}

//-------------------------------------------------------------------
// naem : DeleteRes()
//-------------------------------------------------------------------
void CProcess::DeleteRes()
{
	SAFE_DELETE(m_pEnableSlot);
	SAFE_DELETE(m_pDisableSlot);
	SAFE_DELETE(m_pExistSlot);
	SAFE_DELETE(m_pBack);
	SAFE_DELETE(m_pBtnCancel);
	SAFE_DELETE(m_pBtnMoney);
	SAFE_DELETE(m_pOppNameBox);
	m_MoneyText.DeleteRes();

	// 가공할 사람의 가공창이 TRUE
	if(m_nType == _TYPE_A)
		Del_A();
	// 가공해줄 사람의 가공창이 TRUE
	else
		Del_B();

	RemoveOppInven();
}

void CProcess::Del_A()
{
	SAFE_DELETE(m_pInfo_A);
}
void CProcess::Del_B()
{
	SAFE_DELETE(m_pBtnExecute);
	SAFE_DELETE(m_pInfo_B);
}

void CProcess::RemoveOppInven()
{
	CItem *pItem = NULL;
	CItem *nextItem = NULL;

	pItem = m_ProcessOppInven;

	while (pItem)
	{
		nextItem = pItem->m_Next;
		delete pItem;
		pItem = nextItem;
	}
	m_ProcessOppInven = NULL;

	int i, j;

	for ( i=0; i < EXG_SLOT_X_NUM; i++)
	{
		for ( j = 0; j < EXG_SLOT_Y_NUM; j++)
		{
			m_ProcessOppSlot[i][j] = -1;
		}
	}
}


//-------------------------------------------------------------------
// name : Draw
//-------------------------------------------------------------------
CItem* CProcess::Draw()
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

	m_MoneyText.Draw(m_pBack->Xpos+69,m_pBack->Ypos+215,127);

	if (m_pOppNameBox && g_dwClientCountry != CTRY_TH ) // 태국은 현재 설명문의 줄수 문제로 가공 상대자 이름을 출력 안한다.(by 원석)
		m_pOppNameBox->Draw();

	return m_pOverItem;
}

void  CProcess::Draw_A()
{
	g_pDisplay->Blt(m_pInfo_A->Xpos, m_pInfo_A->Ypos, m_pInfo_A);
}

void  CProcess::Draw_B()
{
	m_pBtnExecute->Draw();
	g_pDisplay->Blt(m_pInfo_B->Xpos, m_pInfo_B->Ypos, m_pInfo_B);
}


// [6/7/2007 Theodoric] 내구도 수리 :: 함수 인자 추가.
void CProcess::ProcessInvenTo(int x, int y, int index, int vnum, int gem_num, int special,
							  int UpEndur, int MaxUpdEndur, int Endur, int MaxEndur )
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

		// 아이템 값을 입력해보자!
		ResetItemValue(pItem);
		// +아이템을 경우 CTextOutBox 생성
		if (gem_num > 0)
		{
			//-- 2004. 03. 22  / Lyul
			//-- 랩프리 다이아 장비 인식 오류 Big Fix
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

		// 그 자리에 무언가 있을때 없앤다.
		ProcessInvenFrom(x, y, GET_ITEM_WIDTH(pItem), GET_ITEM_HEIGHT(pItem));

		pItem->m_SlotX = x;
		pItem->m_SlotY = y;
		pItem->m_Special = special;
		SetSpecialValue(pItem, GET_TYPE(pItem), GET_SHAPE(pItem), special, 0);

		pItem->m_UpgradeEndurance = UpEndur;
		pItem->m_MaxUpgradeEndurance = MaxUpdEndur;
		pItem->m_Endurance = Endur;
		pItem->m_MaxEndurance = MaxEndur;

		pItem->m_Next = m_ProcessOppInven;
		m_ProcessOppInven = pItem;

		for (int x_idx = 0; x_idx < GET_ITEM_WIDTH(pItem); x_idx++)
		{
			for (int y_idx = 0; y_idx < GET_ITEM_HEIGHT(pItem); y_idx++)
			{
				m_ProcessOppSlot[x+x_idx][y+y_idx] = index;
			}
		}
	}
}

int CProcess::GetOppSlotIndex(int x, int y)
{
	if (x < 0 || x >= EXG_SLOT_X_NUM || y < 0 || y >= EXG_SLOT_Y_NUM)
		return -1;

	return m_ProcessOppSlot[x][y];
}

//--------------------------------------------------------------------------
// 상대의 교환창 부분에서 아이템이 없앨때.
//--------------------------------------------------------------------------
int CProcess::GetItemIndexInOppSlot(int x, int y, int w, int h)
{
	int index = -1;

	for (int x_idx = x; x_idx < x+w; x_idx++)
	{
		for (int y_idx = y; y_idx < y+h; y_idx++)
		{
			if (m_ProcessOppSlot[x_idx][y_idx] != -1)
			{
				if (index == -1)
					index = m_ProcessOppSlot[x_idx][y_idx];
				else if (index != m_ProcessOppSlot[x_idx][y_idx])
					return -2;
			}
		}
	}

	return index;
}

CItem * CProcess::PickItemInOppSlot(int index, BOOL isVnum)
{
	CItem *beforeItem = NULL;
	CItem *pItem = m_ProcessOppInven;

	while (pItem)
	{
		if ((!isVnum && pItem->m_Index == index) || (isVnum && pItem->m_Vnum == index))
		{
			if (beforeItem)
				beforeItem->m_Next = pItem->m_Next;
			else
				m_ProcessOppInven = pItem->m_Next;

			for (int x_idx = 0; x_idx < GET_ITEM_WIDTH(pItem); x_idx++)
			{
				for (int y_idx = 0; y_idx < GET_ITEM_HEIGHT(pItem); y_idx++)
				{
					m_ProcessOppSlot[pItem->m_SlotX+x_idx][pItem->m_SlotY+y_idx] = -1;
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

void CProcess::ProcessInvenFrom(int x, int y, int w, int h)
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

void CProcess::ProcessMoney(char *strMoney)
{
	if( strMoney == NULL )
		return;

	if (strlen(strMoney) < 11)
	{
		strcpy(m_strProcessMoney, strMoney);
		m_ProcessMoney = _atoi64(m_strProcessMoney);
		if (m_ProcessMoney == 0)
			m_MoneyText.SetString(m_ProcessMoney);
		else
			m_MoneyText.SetString(m_ProcessMoney, true);
	}

	// 가공버튼을 enable 해준다.
	if (m_nType != _TYPE_A)
		m_pBtnExecute->SetState(BTN_NORMAL);
}

void CProcess::ProcessSuccess(int vnum, int index, int x, int y)
{
	if (m_nType == _TYPE_A)
	{
		// 신청자. (하단)
		RemoveAllMyInven();

		if (vnum == -1)
			return;

		// [6/1/2007 Theodoric] 내구도 수리 :: 파리미터 추가
		g_pRoh->AddToInven(3, x, y, index, vnum,0, 0, 0, 0, 0, 0, 0); // 05-10-10 옵션플래그 2 추가 (원석) ///신방어구강화시스템

		// 금액 관련 초기화.
		strcpy(m_strProcessMoney, "0");
		m_ProcessMoney = _atoi64(m_strProcessMoney);
		m_MoneyText.SetString(m_ProcessMoney);

		m_bProcessLock = FALSE;
	}
	else
	{
		// 가공해준 사람. (상단)
		RemoveOppInven();

		// [6/1/2007 Theodoric] 내구도 수리 :: 파리미터 추가
		ProcessInvenTo(x, y, index, vnum, 0, 0, 0, 0, 0, 0);

		// 금액 표시 초기화.
		strcpy(m_strProcessMoney, "0");
		m_ProcessMoney = _atoi64(m_strProcessMoney);
		m_MoneyText.SetString(m_ProcessMoney);

		// 가공 버튼을 디스에이블 한다.
		if(m_pBtnExecute)
			m_pBtnExecute->m_iBtnFlg = BTN_DISABLE;
	}
}

//-------------------------------------------------------------------
// name : MsgProc
//-------------------------------------------------------------------
LRESULT CProcess::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
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
			ret = Message_A(  hWnd,  msg,  wParam,  lParam);
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

LRESULT CProcess::Message_A( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
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
			sprintf(m_MsgBuf, "process ans reject\n");
			SendMsg();
			g_pNk2DFrame->ShowProcessWindow(FALSE, m_nType);
			return 1;
		}
		if (m_pBtnMoney->GetState() == BTN_ACTION)
		{
			//-- IDS_REFINE_INPUT_MONEY : 금액을 입력하세요
			g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_REFINE_INPUT_MONEY), TYPE_EXCHANGE, 20 );
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
				if (m_bProcessLock)
					return 1;

				// 엑스트라 슬롯에 있는 경우, 교환창 슬롯에 놓는다.
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
							sprintf(m_MsgBuf, "process item %d %d\n", slot_x_num, slot_y_num);
							SendMsg();
						}
					}
				}
				else
				{
					// 아이템을 빼는 것.
					if (m_pNkCha->AddToExgInven((x-m_my_slotstart_x) / SLOT_WIDTH, (y-m_my_slotstart_y) / SLOT_HEIGHT))
					{
						sprintf(m_MsgBuf, "process item %d %d\n",  (x-m_my_slotstart_x) / SLOT_WIDTH, (y-m_my_slotstart_y) / SLOT_HEIGHT);
						SendMsg();
					}
				}
			}
		}
		else if (IsPlayArea(x, y))
		{
			if (m_bProcessCon)
				CloseConnection();

			g_pNk2DFrame->ShowProcessWindow(FALSE, m_nType);
		}
		else
			return 0;

		return 1;
		break;
	}
	return 0;
}

LRESULT CProcess::Message_B( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
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
			sprintf(m_MsgBuf, "process ans reject\n");
			SendMsg();
			m_pBtnCancel->SetState(BTN_NORMAL);

			g_pNk2DFrame->ShowProcessWindow(FALSE, m_nType);
			return 1;
		}

		if (m_pBtnExecute->GetState() == BTN_ACTION)
		{
			sprintf(m_MsgBuf, "process proc\n");
			SendMsg();
			m_pBtnExecute->SetState(BTN_DISABLE);
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
			if (m_bProcessCon)
				CloseConnection();

			g_pNk2DFrame->ShowProcessWindow(FALSE, m_nType);
		}
		else
			return 0;

		return 1;
		break;
	}
	return 0;
}

void CProcess::DrawOppSlot()
{
	int xpos, ypos;

	if (g_pDisplay && m_pBack)
	{
		RECT rcRect;
		POINT point;
		GetCursorPos(&point);
		ScreenToClient(g_pDisplay->GetHWnd(), &point);

		// 아이템이 위치한 슬롯 표시
		CItem *pItem = m_ProcessOppInven;
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

		int index = GetOppSlotIndex(slot_x_num, slot_y_num);
		pItem = m_ProcessOppInven;
		m_pItemRender->BeginRenderLolo();				// 중요....... 중요..

		while (pItem)
		{
			if (pItem->m_Index == index)
			{
				g_pNk2DFrame->RenderItemInUI(m_opp_slotstart_x + pItem->m_SlotX * SLOT_WIDTH,
											 m_opp_slotstart_y + pItem->m_SlotY * SLOT_HEIGHT,
											 pItem , TRUE , FALSE , FALSE, TRUE);
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

				int cx, cy;

				xpos = m_opp_slotstart_x + (pItem->m_SlotX*SLOT_WIDTH);
				ypos = m_opp_slotstart_y + (pItem->m_SlotY*SLOT_HEIGHT);
				cx = SLOT_WIDTH; // * GET_ITEM_WIDTH(pItem);
				cy = SLOT_HEIGHT;// * GET_ITEM_HEIGHT(pItem);

				// +아이템 정보를 가운데로 정열해서 출력한다
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

void CProcess::DrawMySlot()
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
	m_pItemRender->BeginRenderLolo();		// 중요..  m_pItemRender->RenderLolo의 파라미터에 영향을 준다.

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

BOOL CProcess::CheckMoneyStr(char* szMoney)
{
	__int64 nMoney;
	// 입력된 스트링을 검사한다
	int len = strlen(szMoney);
	for(int i=0; i<len; i++)
	{
		if(isdigit(szMoney[i]) == FALSE)
		{
			if(is_space(szMoney[i]) ==TRUE)
				//-- IDS_EXCHANGE_UNABLE_BLANK : 공백은 포함될 수 없습니다
				g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_EXCHANGE_UNABLE_BLANK), TYPE_EXCHANGE, 20 );
			else
				//-- IDS_EXCHANGE_UNABLE_STRING : 숫자만 입력해주세요
				g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_EXCHANGE_UNABLE_STRING), TYPE_EXCHANGE, 20 );

			return FALSE;
		}
	}
	// 3. 현제 가지구 있는 보유금액보다 입력된 금액이 더큰경우
	nMoney = _atoi64(szMoney);

	if (nMoney < 0)
	{
		//-- IDS_EXCHANGE_FAULT_STR : 허용되지 않는 값입니다
		g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_EXCHANGE_FAULT_STR), TYPE_EXCHANGE, 20 );
		return FALSE;
	}

	if(g_pRoh->m_Money < nMoney)
	{
		//-- IDS_EXCHANGE_SCARCE_MONEY : 금액이 부족합니다.
		g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_EXCHANGE_SCARCE_MONEY), TYPE_EXCHANGE, 20 );
		return FALSE;
	}

	if( nMoney < 50000 )
	{
		//-- IDS_INPUT_MONEY50000 : 50000라임을 입력해 주세요.
		g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_INPUT_MONEY50000), TYPE_EXCHANGE, 20 );
		return FALSE;
	}
	return TRUE;
}

BOOL CProcess::IsInside(int x, int y)
{
	if( !m_pBack )
		return FALSE;

	return m_pBack->IsIn( x, y);
}

BOOL CProcess::IsPlayArea(int x, int y)
{
	if( !IsInside(x,y) && !g_pNk2DFrame->IsInPopup(x,y) )
		return true;

	return false;
}

void CProcess::SendMsg()
{
	if (g_pTcpIp)
		g_pTcpIp->SendNetMessage(m_MsgBuf);
}

void CProcess::RemoveAllMyInven()
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

void CProcess::CloseConnection()
{
	m_bProcessCon = FALSE;
	m_bProcessLock = FALSE;
	sprintf(m_MsgBuf, "process ans reject\n");
	SendMsg();
}

void CProcess::ResetConnection()
{
//	m_bProcessCon = FALSE;
	m_bProcessLock = FALSE;
	strcpy(m_strProcessMoney, "0");
	m_ProcessMoney = _atoi64(m_strProcessMoney);
	m_MoneyText.SetString(m_ProcessMoney);
}

void CProcess::AskProcessRequest(char *player_name)
{
	char strTemp[128] = "";
	//-- IDS_NK2D_REQUIRE_PROCESS : %s님에게서 정련요청이 있습니다
	sprintf(strTemp, G_STRING(IDS_NK2D_REQUIRE_PROCESS), player_name);
	g_pNk2DFrame->InsertPopup(strTemp,TYPE_NOR_OKCANCLE, 13);
}

LRESULT CProcess::PopupMsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, int nMsg)
{
	char commOutBuf[128] = "";
	if (nMsg == 1)
	{
		// 가공 요청 승낙 메시지를 보낸다.
		sprintf(commOutBuf, "process ans accept\n");
		m_bProcessCon = TRUE;
	}
	else if (nMsg == -1)
		sprintf(commOutBuf, "process ans reject\n");

	if (g_pTcpIp)
		g_pTcpIp->SendNetMessage(commOutBuf);

	return 1;
}

LRESULT CProcess::MoneyPopupMsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, int nMsg, char* szMoney)
{
	// "확인" 버튼이 눌린경우
	if(nMsg == 1)
	{
		if(CheckMoneyStr(szMoney) == TRUE)
		{
			m_bProcessCon = TRUE;
			strcpy(m_strProcessMoney, szMoney);
			__int64 money = _atoi64(m_strProcessMoney);

			if( money > 50000 )
			{
				money = 50000; // 강제로 50000세팅
				sprintf( m_strProcessMoney, "%d", money ); // 서버에는 이 스트링으로 보내니 스트링도 바꿔줘야한다.
			}

			if (money >= m_ProcessMoney)
			{
				m_bProcessLock = TRUE;

				if (money == 0)
					m_MoneyText.SetString(money);
				else
					m_MoneyText.SetString(money, true);

				// 여기다가 돈을 적어서 보낸다.
				sprintf(m_MsgBuf, "process money %s\n",m_strProcessMoney);	// 스페이스 주의.
				SendMsg();
				m_ProcessMoney = money;
			}
		}
	}

	return 0L;
}