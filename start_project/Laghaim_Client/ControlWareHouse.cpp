#include "stdafx.h"


#include "main.h"
#include "BasicButton.h"
#include "ControlInven.h"
#include "ControlWarehouse.h"
#include "headers.h"
#include "ControlBottom.h"
#include "Nk2DFrame.h"
#include "NkCharacter.h"
#include "itemProto.h"
#include "itemrender.h"
#include "tcpipcon.h"
#include "SpecialItem.h"
#include "msgpopup.h"
#include "textoutbox.h"
#include "UIMgr.h"
#include "g_stringmanager.h"

#include "Country.h"
#include "Mouse.h"

#include "CommonConfig.h"

extern DWORD	g_dwClientCountry;


const int PER_COUNT = 200;

extern DWORD		g_dwLangType;
extern int			g_nCurWorldIdx;
extern	SVR_TYPE	g_SvrType;

// ������ ��Ÿ���� TEXTOUTBOX�� ���� ����
#define _TEXT_COLOR1 RGB(255, 255, 0)
#define _TEXT_COLOR2 RGB(0, 0, 0)
#define _TEXT_SIZE   20
#define IS_HERO_ITEM( vnum ) ( 1817 <= vnum && vnum <= 1820 )

CControlWareHouse::CControlWareHouse()
	: m_hWnd(NULL)
	, m_timeWareLock(0)
	, m_wareslot_start_x(0)
	, m_wareslot_start_y(0)
	, m_ScreenWidth(0)
	, m_ScreenHeight(0)
	, m_nSaveMoney(0)
	, m_iCurSlot_X(0)
	, m_iCurSlot_Y(0)
	, m_indexNpc(-1)
{
	memset(m_MsgBuf, 0, sizeof(m_MsgBuf));
	memset(m_strMoney1, 0, sizeof(m_strMoney1));

	m_pBack = NULL;
	m_bActivate = FALSE;
	m_bWareLock = FALSE;

	m_pNkCha = NULL;

	m_pStoreWidening_time_back	= NULL;
	m_bStoreWidening			= false;

	Init();
}

CControlWareHouse::~CControlWareHouse()
{
	DeleteRes();
	m_pNkCha = NULL;
	m_bStoreWidening = false;
}


void CControlWareHouse::Init()
{
	int i = 0;
	int j = 0;
	int k = 0;

	m_bGetStashEnd = FALSE;
	m_InMoneyBtn.SetFileName("warehouse/btn_deposit");
	m_OutMoneyBtn.SetFileName("warehouse/btn_withdraw");
	m_CloseBtn.SetFileName("common/btn_close_01");
	m_BtnWindow_1.SetFileName("common/btn_tab_01");
	m_BtnWindow_2.SetFileName("common/btn_tab_02");
	m_BtnWindow_3.SetFileName("common/btn_tab_03");	// ��ư�ɹ��� �̸��� ����

	m_MoneyText.Init(12, RGB(255, 255, 0), 0, 0);

	for (k=0; k < WARE_PACK_NUM; k++)
	{
		for (i=0; i < WARE_SLOT_X_NUM; i++)
		{
			for (j=0; j < WARE_SLOT_Y_NUM; j++)
			{
				m_WareInven[k][i][j] = NULL;
				m_WareSlot[k][i][j] = 0;	// ���� ī��Ʈ
			}
		}
	}
	m_byInOutMoneyState = 0;
	m_bMsgWaiting = FALSE;
	m_nPackNum = 0;

	// [4/16/2007 Theodoric] �±� ���� â��
	m_pCashViewBtn	= NULL;
	m_pNormalViewBtn = NULL;
	m_pCashDateText  = NULL;
	m_bCash = FALSE;
	m_nCashMaxPackNum = 0;
}

// [4/16/2007 Theodoric] �±� ���� â��
void CControlWareHouse::InvenSlotInfoInit()
{
	int k, i, j;
	// nate : ���Կ� ���̴� �׸� �ʱ�ȭ.
	for (k=0; k < WARE_PACK_NUM; k++)
	{
		for (i=0; i < WARE_SLOT_X_NUM; i++)
		{
			for (j=0; j < WARE_SLOT_Y_NUM; j++)
			{
				m_WareInven[k][i][j] = NULL;
				m_WareSlot[k][i][j] = 0;	// ���� ī��Ʈ
				m_pNumText[k][i][j].SetString("0");
			}
		}
	}
	m_nSaveMoney = 0;
	m_BtnWindow_1.SetState(BTN_DOWN);
	m_BtnWindow_2.SetState(BTN_NORMAL);
	m_BtnWindow_3.SetState(BTN_NORMAL);
	m_nPackNum = 0;
}

void CControlWareHouse::LoadRes()
{
	if(!g_pDisplay)
		return;

	m_hWnd =  g_pDisplay->GetHWnd();
	GetClientRect(m_hWnd, &m_ClientRc);

	if( g_pNk2DFrame )
	{
		m_ScreenWidth = g_pNk2DFrame->GetClientWidth();
		m_ScreenHeight = g_pNk2DFrame->GetClientHeight();
	}

	if( FAILED( g_pDisplay->CreateSurfaceFromBitmap( &m_pBack, "interface/warehouse/warehouse.BMP") ) )
		return;
	m_pBack->SetColorKey( TRANS_COLOR );

	m_InMoneyBtn.LoadRes();
	m_OutMoneyBtn.LoadRes();
	m_CloseBtn.LoadRes();
	m_BtnWindow_1.LoadRes();
	m_BtnWindow_2.LoadRes();
	m_BtnWindow_3.LoadRes();	// nate : â�� ���� Ȯ�� - ��ư �߰�
	m_BtnWindow_1.SetState(BTN_DOWN);


	m_pBack->Xpos = g_pNk2DFrame->GetClientWidth() - 291 - m_pBack->GetWidth();
	m_pBack->Ypos = 0;



	if( FAILED( g_pDisplay->CreateSurfaceFromBitmap( &m_pStoreWidening_time_back, "interface/warehouse/store_widenig_time_back.BMP") ) )
		return;

	if( m_pStoreWidening_time_back )
	{
		m_pStoreWidening_time_back->Xpos = m_pBack->Xpos + 30;
		m_pStoreWidening_time_back->Ypos = m_pBack->Ypos + 401;
	}	

	m_wareslot_start_x = m_pBack->Xpos + 29;
	m_wareslot_start_y = m_pBack->Ypos + 116;


	m_InMoneyBtn.SetPosition(m_pBack->Xpos+139, m_pBack->Ypos+380);
	m_OutMoneyBtn.SetPosition(m_pBack->Xpos+181, m_pBack->Ypos+380);
	m_CloseBtn.SetPosition(m_pBack->Xpos+255, m_pBack->Ypos+5);
	m_BtnWindow_1.SetPosition(m_pBack->Xpos+20, m_pBack->Ypos+72);
	m_BtnWindow_2.SetPosition(m_pBack->Xpos+99, m_pBack->Ypos+72);
	m_BtnWindow_3.SetPosition(m_pBack->Xpos+178, m_pBack->Ypos+72);

	m_MoneyText.SetPos(m_pBack->Xpos+29, m_pBack->Ypos+368);

	// [4/16/2007 Theodoric] �±� ���� â��
	if( g_dwClientCountry == CTRY_TH || g_dwClientCountry == CTRY_JPN ) // #define CTRY_TH 7
	{
		if(m_pCashViewBtn == NULL )
		{
			m_pCashViewBtn = new CBasicButton;
			m_pCashViewBtn->SetFileName("warehouse/btn_cashView");
			m_pCashViewBtn->LoadRes();
		}

		if(m_pNormalViewBtn == NULL )
		{
			m_pNormalViewBtn = new CBasicButton;
			m_pNormalViewBtn->SetFileName("warehouse/btn_normalView");
			m_pNormalViewBtn->LoadRes();
		}

		if( m_pCashDateText == NULL )
		{
			m_pCashDateText = new CTextOutBox;
			m_pCashDateText->SetString("0");

			m_pCashDateText->Init(12, RGB(255, 255, 0), 0, 0);
		}

		m_pCashViewBtn->SetPosition(m_pBack->Xpos+223, m_pBack->Ypos+380);
		m_pNormalViewBtn->SetPosition(m_pBack->Xpos+223, m_pBack->Ypos+380);
		//m_pCashDateText->SetPos(m_pBack->Xpos+82, m_pBack->Ypos+425 + 5 );
	}

	if( CommonConfig::Instance()->GetKorFlag() == true )
	{
		if(m_pCashViewBtn == NULL )
		{
			m_pCashViewBtn = new CBasicButton;
			m_pCashViewBtn->SetFileName("warehouse/btn_cashView");
			m_pCashViewBtn->LoadRes();
		}

		if(m_pNormalViewBtn == NULL )
		{
			m_pNormalViewBtn = new CBasicButton;
			m_pNormalViewBtn->SetFileName("warehouse/btn_normalView");
			m_pNormalViewBtn->LoadRes();
		}

		if( m_pCashDateText == NULL )
		{
			m_pCashDateText = new CTextOutBox;
			m_pCashDateText->SetString("0");
			m_pCashDateText->Init(14, RGB(255, 255, 0), 0, 0, 149, 24, TRUE);
		}

		m_pCashViewBtn->SetPosition(m_pBack->Xpos+223, m_pBack->Ypos+380);
		m_pNormalViewBtn->SetPosition(m_pBack->Xpos+223, m_pBack->Ypos+380);
	}

	int temp_MoneyText_Y= 0;


	if (m_pNkCha)
		m_MoneyText.SetString((m_nSaveMoney*100), true);

	char tmp[10];
	int count = -1;

	for (int k=0; k < WARE_PACK_NUM; k++)
	{
		for (int x=0; x < WARE_SLOT_X_NUM; x++)
		{
			for (int y=0; y < WARE_SLOT_Y_NUM; y++)
			{
				m_pNumText[k][x][y].Init(_TEXT_SIZE, _TEXT_COLOR1, 0, 0);

				if(m_WareSlot[k][x][y] > 0)
				{
					count = m_WareSlot[k][x][y];
					sprintf(tmp, "%d",count);
					m_pNumText[k][x][y].SetString(tmp);
				}
			}
		}
	}
}

void CControlWareHouse::DeleteRes()
{
	SAFE_DELETE(m_pBack);
	SAFE_DELETE(m_pStoreWidening_time_back);

	m_InMoneyBtn.DeleteRes();
	m_OutMoneyBtn.DeleteRes();;
	m_CloseBtn.DeleteRes();
	m_BtnWindow_1.DeleteRes();
	m_BtnWindow_2.DeleteRes();
	m_BtnWindow_3.DeleteRes();	// nate : â�� ���� Ȯ�� - ��ư �߰�
	m_MoneyText.DeleteRes();

	for (int k=0; k < WARE_PACK_NUM; k++)
	{
		for (int i=0; i < WARE_SLOT_X_NUM; i++)
		{
			for (int j=0; j < WARE_SLOT_Y_NUM; j++)
				m_pNumText[k][i][j].DeleteRes();
		}
	}

	// [4/16/2007 Theodoric] �±� ���� â��
	if( g_dwLangType == 7 ) // #define CTRY_TH 7
	{
		if( m_pCashViewBtn != NULL )
		{
			m_pCashViewBtn->DeleteRes();
			SAFE_DELETE(m_pCashViewBtn);
		}

		if( m_pNormalViewBtn != NULL )
		{
			m_pNormalViewBtn->DeleteRes();
			SAFE_DELETE(m_pNormalViewBtn);
		}

		if( m_pCashDateText != NULL )
		{
			m_pCashDateText->DeleteRes();
			SAFE_DELETE(m_pCashDateText);
		}
	}

	if( m_pCashViewBtn != NULL )
	{
		m_pCashViewBtn->DeleteRes();
		SAFE_DELETE(m_pCashViewBtn);
	}

	if( m_pNormalViewBtn != NULL )
	{
		m_pNormalViewBtn->DeleteRes();
		SAFE_DELETE(m_pNormalViewBtn);
	}

	if( m_pCashDateText != NULL )
	{
		m_pCashDateText->DeleteRes();
		SAFE_DELETE(m_pCashDateText);
	}

	m_bCash = FALSE;
	m_nCashMaxPackNum = 0;
}


void CControlWareHouse::SetWareLock( BOOL bLock )
{
	if ( m_bWareLock )
		m_timeWareLock += 1500;
	else
	{
		m_bWareLock = TRUE;
		m_timeWareLock = timeGetTime();
	}
}


// nate : â�� Inven �� �׸���.
//-- 2004.07.12
//-- Lyul / �����Ϸ�
//-- â�� Inven �� ���� ������ Item �� �ٷ����� ���� �� �� �����Ƿ�
//-- �� cell �� Index �� ���������� ���� ���� ����.
//-- �׷��� ControlInven ���� AlphaPopup ������ ���� �������� Index �� ������
//-- ������ Update �ϴ��� �Ǻ��ϹǷ� ���װ� �ȴ�.
//-- controlinven ������ �Ϲ� inven �� ó���ϹǷ�
//-- ������ â����� ��ǥ�� ����Ͽ� â�� ���� Item �� ��������.
CItem *CControlWareHouse::Draw()
{
	int i = 0, j = 0;

	//-- ������ ������ ��µ� Inven
	static int past_i= -1;
	static int past_j= -1;
	static int past_pack= -1;
	//-- ������ ���� Index ��ȣ ������ ū �ǹ̰� ���� Ʋ���ٴ� �͸� �߿�
	static int past_idx = 1;

	CItem *pItem=NULL;

	// â�� �۾��� �����°� �˻�
	if (!m_bGetStashEnd)
		return NULL;

	// BOTTOM �׸���
	if( g_pDisplay && m_pBack )
		g_pDisplay->Blt(m_pBack->Xpos , m_pBack->Ypos, m_pBack);

	if( m_bStoreWidening )
	{
		g_pDisplay->Blt(m_pStoreWidening_time_back->Xpos , m_pStoreWidening_time_back->Ypos, m_pStoreWidening_time_back);
		m_pCashDateText->Draw();
	}

	if( !m_bCash ) // ����Ȯ�� â��� ������ ���� ���Ѵ�. ��/��� ��ư�� �Ⱥ��̰� �����ش�. ///071015
	{
		m_InMoneyBtn.Draw();
		m_OutMoneyBtn.Draw();
	}
	m_CloseBtn.Draw();

	// nate : â�� ���� Ȯ��
	// ��ư �߰�
	// â�� �ѹ�ư
	// ��ư�� Ŭ�� ������ ���� �׸��� ���� ����
	if(m_BtnWindow_1.GetState() == BTN_DOWN)
	{
		m_BtnWindow_3.Draw();
		m_BtnWindow_2.Draw();
		m_BtnWindow_1.Draw();
	}
	else if( m_BtnWindow_2.GetState() == BTN_DOWN )
	{
		m_BtnWindow_1.Draw();
		m_BtnWindow_3.Draw();
		m_BtnWindow_2.Draw();
	}
	else
	{
		m_BtnWindow_1.Draw();
		m_BtnWindow_2.Draw();
		m_BtnWindow_3.Draw();
	}

	if( m_bCash )
	{
		m_pNormalViewBtn->Draw();
	}
	else
	{
		m_pCashViewBtn->Draw();
	}
	m_pCashDateText->Draw();	

	// ���콺 �������� ������ġ ���
	POINT point;
	GetCursorPos(&point);
	ScreenToClient(g_pDisplay->GetHWnd(), &point);

	if (pCMyApp->m_pMouse && IsInside(point.x, point.y))
		pCMyApp->m_pMouse->SetMouseType(M_NORMAL);

	int x = (int)point.x;
	int y = (int)point.y;

	m_iCurSlot_X = -1;
	m_iCurSlot_Y = -1;
	// ���콺�� â���� ���� ������ ���������� üũ
	if ( x >= m_wareslot_start_x
			&& x < m_wareslot_start_x + WARE_SLOT_WIDTH * WARE_SLOT_X_NUM
			&& y >= m_wareslot_start_y
			&& y < m_wareslot_start_y + WARE_SLOT_HEIGHT * WARE_SLOT_Y_NUM )
	{
		int slot_x_num, slot_y_num, slot_index;

		// ���� ���� ����
		m_iCurSlot_X = slot_x_num = (x - m_wareslot_start_x) / WARE_SLOT_WIDTH;
		m_iCurSlot_Y = slot_y_num = (y - m_wareslot_start_y) / WARE_SLOT_HEIGHT;
		// ������ ���ؽ� ���
		// ���� ���° �����ΰ� ���
		slot_index = slot_x_num * WARE_SLOT_Y_NUM + slot_y_num;
	}

	CItem *m_pOverItem = NULL;

	if( g_pNk2DFrame->GetItemRender() )
	{
		g_pNk2DFrame->GetItemRender()->BeginRenderLolo();
		for (i=0; i < WARE_SLOT_X_NUM; i++)
		{
			for (j=0; j < WARE_SLOT_Y_NUM; j++)
			{
				if (m_WareSlot[m_nPackNum][i][j] > 0)
				{
					pItem = m_WareInven[m_nPackNum][i][j];
					DWORD ambient = 0x00555555;

					if( pItem == NULL )
						continue;


					if( g_pNk2DFrame->IsScroll(pItem)
							|| pItem->m_Vnum == 1251
							|| pItem->m_Vnum == 1308 // ���ǻ��� ���� ��� ���� 05-10-05 ����
							|| pItem->m_Vnum == 1286
							|| ( pItem->m_Vnum >= 1598 && pItem->m_Vnum <= 1695 ) // ����� ��û ���̱������ ������ �����⿡�� �����̹Ƿ� ����ó��.
							|| pItem->m_Vnum == 1287 ) // �̱� ������ ����, ������ ������ ��Ӱ� ������ Ŭ���̾�Ʈ���� ���� ��� ó��.(by ����)
						ambient = 0x00cccccc;
					else if (pItem->m_PlusNum > 0)
						ambient = UPGRADE_ITEM_COLOR;

					if(i == m_iCurSlot_X && j == m_iCurSlot_Y)
					{
						m_pOverItem = pItem;

						//-- 2004.07.12 / Lyul
						//-- ������ ��ġ�ߴ� Overitem �̶� Ʋ���ٸ�
						//-- ���ο� ��ġ�� �����ϰ� ǥ���϶�� �˸���.
						//-- index �� ��¦ �ٲ��ش�.
						if(past_i != i || past_j != j || past_pack != m_nPackNum)
						{
							if( m_pOverItem )
								m_pOverItem->m_Index = past_idx; //-- controlinven::drawiteminfo ���� �ʿ�

							past_idx = -past_idx;
							past_i = i;
							past_j = j;
							past_pack = m_nPackNum;
						}
					}

					int w = (GET_ITEM_WIDTH(pItem) * SLOT_WIDTH - ITEM_RENDER_WIDTH) /2;
					g_pNk2DFrame->RenderItemInUI( m_wareslot_start_x+ i *WARE_SLOT_WIDTH
												  , m_wareslot_start_y + j *WARE_SLOT_HEIGHT + (WARE_SLOT_HEIGHT - ITEM_RENDER_HEIGHT) /2,
												  pItem , FALSE, FALSE, TRUE, TRUE );
				}
			}
		}
		g_pNk2DFrame->GetItemRender()->EndRenderLolo();
	}

	// �������� ��ų� �޼���â�� ������� ������������â�� ������� �ʴ´�
	if (m_pNkCha && m_pNkCha->m_ExtraSlot || g_pNk2DFrame->m_Popup)
		m_pOverItem = NULL;

	for (i=0; i < WARE_SLOT_X_NUM; i++)
	{
		for (j=0; j < WARE_SLOT_Y_NUM; j++)
		{
			if(m_WareSlot[m_nPackNum][i][j] > 0)
			{
				int xpos, ypos;
				if( m_WareInven[m_nPackNum][i][j] )
				{
					xpos = m_wareslot_start_x + (m_WareInven[m_nPackNum][i][j]->m_SlotX*WARE_SLOT_WIDTH);
					ypos = m_wareslot_start_y + (m_WareInven[m_nPackNum][i][j]->m_SlotY*WARE_SLOT_HEIGHT);

					m_pNumText[m_nPackNum][i][j].Draw(xpos, ypos);
				}
			}
		}
	}

	// �κ�â�� �������Ƿ� nk2d���� ��ο츦 �Ѵ�..
	m_MoneyText.Draw(m_pBack->Xpos+29, m_pBack->Ypos+386, 108);

	if (m_pOverItem)
		return m_pOverItem;
	else
		return NULL;
}

BOOL CControlWareHouse::CheckMoneyStr(char* szMoney)
{
	int nMoney = 0;

	// �Էµ� ��Ʈ���� �˻��Ѵ�
	int len = strlen(szMoney);

	for(int i=0; i<len; i++)
	{
		if(isdigit(szMoney[i]) == FALSE)
		{
			if(is_space(szMoney[i]) ==TRUE)
				//-- IDS_WAREHOUSE_BLANK : ���鹮�ڴ� ������� �ʽ��ϴ�.
				g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_WAREHOUSE_BLANK), TYPE_EXCHANGE, 19 );
			else
				//-- IDS_WAREHOUSE_NOTNUM : �߸��� �ݾ��Դϴ�. �ٽ� �Է��ϼ���
				g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_WAREHOUSE_NOTNUM), TYPE_EXCHANGE, 19 );

			return FALSE;
		}
	}

	// 100 ������ �Է��� ���� �������

	if(len >2)
	{
		if(atoi(&szMoney[len-2]) != 0
				|| atoi(&szMoney[len-1]) != 0)
		{
			//-- IDS_WAREHOUSE_NOT100 : �ݾ��� 100 ������ �Է��� �ּ���
			g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_WAREHOUSE_NOT100), TYPE_EXCHANGE, 19 );
			return FALSE;
		}
	}
	else
	{
		//-- IDS_WAREHOUSE_NOT100 : �ݾ��� 100 ������ �Է��� �ּ���
		g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_WAREHOUSE_NOT100), TYPE_EXCHANGE, 19 );
		return FALSE;
	}

	// 3. ���� ������ �ִ� �����ݾ׺��� �Էµ� �ݾ��� ��ū���
	nMoney = atoi(szMoney);

	if (nMoney < 0)
	{
		//-- IDS_WAREHOUSE_NOT0 : �Է°��� �߸��Ǿ����ϴ�. �ٽ� �Է��ϼ���
		g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_WAREHOUSE_NOT0), TYPE_EXCHANGE, 19 );
		return FALSE;
	}

	if(m_byInOutMoneyState != 2)
	{
		if(m_pNkCha->m_Money < nMoney)
		{
			//-- IDS_WAREHOUSE_NOTENOUGH :  �ݾ��� �����մϴ�. �ٽ� �Է��ϼ���
			g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_WAREHOUSE_NOTENOUGH), TYPE_EXCHANGE, 19 );
			return FALSE;
		}
	}
	else
	{
		if((m_nSaveMoney*100) < nMoney)
		{
			//-- IDS_WAREHOUSE_NOTENOUGH :  �ݾ��� �����մϴ�. �ٽ� �Է��ϼ���
			g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_WAREHOUSE_NOTENOUGH), TYPE_EXCHANGE, 19 );
			return FALSE;
		}
	}

	return TRUE;
}


LRESULT CControlWareHouse::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	int x = 0, y = 0;
	static DWORD BtnClickTick = 0; // ĳ��â�� ��ȯ��ư ������ ������ ƨ��� �� ������ ������ ƽ�� ����. ///071015

	x = LOWORD (lParam);
	y = HIWORD (lParam);

	switch ( msg )
	{
	case WM_RBUTTONDOWN:
		x = LOWORD (lParam);
		y = HIWORD (lParam);

		if (IsPlayArea(x, y) && !m_bMsgWaiting)
		{
			if (m_bActivate)
				// â�� �ݴ´�.
				ActivateWareHouse(FALSE);

			g_pNk2DFrame->ShowWareHouseWindow(FALSE);
			return 1;
		}
		return 1;

	case WM_LBUTTONDOWN:

		x = LOWORD (lParam);
		y = HIWORD (lParam);

		if(IsInside(x, y))
		{
			// ���� ���׷��̵� â�� ���ִ°�� ������ �־���..
			if(g_pNk2DFrame->GetControlInven()
					&& g_pNk2DFrame->GetControlInven()->m_bAct_Masuk)
			{
				g_pNk2DFrame->GetControlInven()->Close_Masuk();
				return 1;
			}

			if (m_bGetStashEnd == FALSE && m_bActivate)
				return 1;

			m_InMoneyBtn.MsgProc(hWnd, msg, wParam, lParam);
			m_OutMoneyBtn.MsgProc(hWnd, msg, wParam, lParam);
			m_CloseBtn.MsgProc(hWnd, msg, wParam, lParam);

			// [4/16/2007 Theodoric] �±� ���� â��::Thai cash stash

			if( g_dwClientCountry == CTRY_TH || g_dwClientCountry == CTRY_JPN ) // #define CTRY_TH 7
			{
				if( timeGetTime() - BtnClickTick > 1000 ) // ĳ��â�� ��ȯ��ư ������ ������ ƨ��� �� ������ ������ ƽ�� ����. ///071015
				{
					if( m_bCash ) // ���� â�� ���� �ִ� ���¶�� �Ϲ� ���� ���� ��ư ó��
					{
						m_pNormalViewBtn->MsgProc(hWnd, msg, wParam, lParam);

						if( m_pNormalViewBtn->GetState() == BTN_DOWN )
						{
							m_bCash = !m_bCash;
							InvenSlotInfoInit();
							// by evilkiki 2009.12.16 �Ϻ�â��
							// g_pTcpIp->SendNetMessage("stash_open chash 0\n"); // �Ϲ� â�� ����
							g_pTcpIp->SendNetMessage("stash_open 0\n");

							BtnClickTick = timeGetTime(); // ĳ��â�� ��ȯ��ư ������ ������ ƨ��� �� ������ ������ ƽ�� ����. ///071015

						}
						m_pNormalViewBtn->SetState(BTN_NORMAL);
					}
					else // �Ϲ� â�� ���� �ִ� ���¶�� ������� ���� ��ư ó��
					{
						m_pCashViewBtn->MsgProc(hWnd, msg, wParam, lParam);

						if( m_pCashViewBtn->GetState() == BTN_DOWN )
						{
							if( m_nCashMaxPackNum <= 0  )
							{
								g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_NEED_CASH_STASH_ITEM), TYPE_NOR_OK, 1 );
							}
							else
							{
								m_bCash = !m_bCash;
								InvenSlotInfoInit();
								// by evilkiki 2009.12.16 �Ϻ�â��
								//g_pTcpIp->SendNetMessage("stash_open chash 1\n"); // ���� â�� ����
								g_pTcpIp->SendNetMessage("stash_open 1\n");

								BtnClickTick = timeGetTime(); // ĳ��â�� ��ȯ��ư ������ ������ ƨ��� �� ������ ������ ƽ�� ����. ///071015
							}
							m_pCashViewBtn->SetState(BTN_NORMAL);
						}
					}
				}
			}
			else if( g_dwClientCountry == CTRY_KOR )
			{
				if( timeGetTime() - BtnClickTick > 1000 )
				{
					if( m_bCash )
					{
						m_pNormalViewBtn->MsgProc(hWnd, msg, wParam, lParam);

						if( m_pNormalViewBtn->GetState() == BTN_DOWN )
						{
							m_bCash = !m_bCash;
							InvenSlotInfoInit();
							g_pTcpIp->SendNetMessage("stash_open 0\n");

							BtnClickTick = timeGetTime(); // ĳ��â�� ��ȯ��ư ������ ������ ƨ��� �� ������ ������ ƽ�� ����. ///071015

						}
						m_pNormalViewBtn->SetState(BTN_NORMAL);
					}
					else // �Ϲ� â�� ���� �ִ� ���¶�� ������� ���� ��ư ó��
					{
						m_pCashViewBtn->MsgProc(hWnd, msg, wParam, lParam);

						if( m_pCashViewBtn->GetState() == BTN_DOWN )
						{
							if( m_nCashMaxPackNum <= 0  )
							{
								//g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_NEED_CASH_STASH_ITEM), TYPE_NOR_OK, 1 );
								g_pNk2DFrame->InsertPopup( "Warehouse expansion period has ended or not activated.", TYPE_NOR_OK, 1 );


							}
							else
							{
								m_bCash = !m_bCash;
								InvenSlotInfoInit();
								g_pTcpIp->SendNetMessage("stash_open 1\n");

								BtnClickTick = timeGetTime();
							}
							m_pCashViewBtn->SetState(BTN_NORMAL);
						}
					}
				}
			}

			// mate : â�� ���� Ȯ��
			// ��ư �߰�( ���� �� ���� ��� )
			if( m_BtnWindow_1.IsInside( x, y) )
			{
				if( (m_nCashMaxPackNum >= 1 && m_bCash ) || !m_bCash )
				{
					m_BtnWindow_1.SetState(BTN_DOWN);
					m_nPackNum = 0;

					if(m_BtnWindow_2.GetState() != BTN_NORMAL)
						m_BtnWindow_2.SetState(BTN_NORMAL);
					if(m_BtnWindow_3.GetState() != BTN_NORMAL)
						m_BtnWindow_3.SetState(BTN_NORMAL);
				}
				else
				{
					g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_DONT_USE_CASH_STASH), TYPE_NOR_OK, 1 );
				}

			}
			else if( m_BtnWindow_2.IsInside( x, y) )
			{
				if( (m_nCashMaxPackNum >= 2 && m_bCash ) || !m_bCash )
				{
					m_BtnWindow_2.SetState(BTN_DOWN);
					m_nPackNum = 1;
					if(m_BtnWindow_1.GetState() != BTN_NORMAL)
						m_BtnWindow_1.SetState(BTN_NORMAL);
					if(m_BtnWindow_3.GetState() != BTN_NORMAL)
						m_BtnWindow_3.SetState(BTN_NORMAL);
				}
				else
				{
					g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_DONT_USE_CASH_STASH), TYPE_NOR_OK, 1 );
				}

			}
			else if( m_BtnWindow_3.IsInside( x, y) )
			{
				if( (m_nCashMaxPackNum == 3 && m_bCash ) || !m_bCash )
				{
					m_BtnWindow_3.SetState(BTN_DOWN);
					m_nPackNum = 2;
					if(m_BtnWindow_1.GetState() != BTN_NORMAL)
						m_BtnWindow_1.SetState(BTN_NORMAL);
					if(m_BtnWindow_2.GetState() != BTN_NORMAL)
						m_BtnWindow_2.SetState(BTN_NORMAL);
				}
				else
				{
					g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_DONT_USE_CASH_STASH), TYPE_NOR_OK, 1 );
				}

			}

			if ( x >= m_wareslot_start_x
					&& x < m_wareslot_start_x + WARE_SLOT_WIDTH*WARE_SLOT_X_NUM
					&& y >= m_wareslot_start_y
					&& y < m_wareslot_start_y + WARE_SLOT_HEIGHT*WARE_SLOT_Y_NUM )
			{

				int slot_x_num, slot_y_num, slot_index;
				int vnum, item_plus, item_flag;

				slot_x_num = (x - m_wareslot_start_x) / WARE_SLOT_WIDTH;
				slot_y_num = (y - m_wareslot_start_y) / WARE_SLOT_HEIGHT;
				slot_index = slot_x_num * WARE_SLOT_Y_NUM + slot_y_num;

				int max_cnt = 0;
				int cur_cnt = 0;

				max_cnt = PER_COUNT; // �ؿ��ʵ� ���� 100������ �������ְ� �ٲ����. (������ 20��) (by ����)

				// �������� �ִ� ��.
				if (m_pNkCha && m_pNkCha->m_ExtraSlot)
				{
					if( m_pNkCha->m_ExtraSlot->m_bTimeLimit ) // �ð��� �������̸� â�� �Ұ�ó�� (by ����) ///�ð���������
					{
						//-- IDS_LIMITED_CANT : ���ѵ� �������� �Ұ����մϴ�.
						g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_LIMITED_CANT), TYPE_NOR_OK, 1 );
						return 1;
					}
					else if( GET_ATT_FLAG(m_pNkCha->m_ExtraSlot) & IATT_NO_STASH )
					{
						g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_CANT_STASH_WITH_NOSTASH), TYPE_NOR_OK, 1 );
						return 1;
					}
					else if( !(GET_ATT_FLAG(m_pNkCha->m_ExtraSlot) & IATT_NO_STASH) )
					{
						vnum = m_pNkCha->m_ExtraSlot->m_Vnum;
						cur_cnt = m_WareSlot[m_nPackNum][slot_x_num][slot_y_num];

						// â������ �������� 1���̻��̰� extraslot�� vnum�� �ٸ� ��� �ƹ� ���۵���������~
						if((cur_cnt >= 1))
							if(m_WareInven[m_nPackNum][slot_x_num][slot_y_num]->m_Vnum != vnum || (cur_cnt >= max_cnt))
								return 1;

						// [2008/6/4 Theodoric] ���� ���׶����� 1���� ���� �� �ִ� �������̶�� ���ҵ� ������ ����.
						if( m_WareInven[m_nPackNum][slot_x_num][slot_y_num] )
							if( m_WareInven[m_nPackNum][slot_x_num][slot_y_num]->m_Vnum					!= m_pNkCha->m_ExtraSlot->m_Vnum
									|| m_WareInven[m_nPackNum][slot_x_num][slot_y_num]->m_PlusNum				!= m_pNkCha->m_ExtraSlot->m_PlusNum
									|| m_WareInven[m_nPackNum][slot_x_num][slot_y_num]->m_UpgradeEndurance		!= m_pNkCha->m_ExtraSlot->m_UpgradeEndurance
									|| m_WareInven[m_nPackNum][slot_x_num][slot_y_num]->m_MaxUpgradeEndurance	!= m_pNkCha->m_ExtraSlot->m_MaxUpgradeEndurance
									|| m_WareInven[m_nPackNum][slot_x_num][slot_y_num]->m_Endurance			!= m_pNkCha->m_ExtraSlot->m_Endurance
									|| m_WareInven[m_nPackNum][slot_x_num][slot_y_num]->m_MaxEndurance 		!= m_pNkCha->m_ExtraSlot->m_MaxEndurance
									|| m_WareInven[m_nPackNum][slot_x_num][slot_y_num]->m_bTimeLimit 			!= m_pNkCha->m_ExtraSlot->m_bTimeLimit
									|| m_WareInven[m_nPackNum][slot_x_num][slot_y_num]->m_PriceType 			!= m_pNkCha->m_ExtraSlot->m_PriceType
									|| m_WareInven[m_nPackNum][slot_x_num][slot_y_num]->m_Special2				!= m_pNkCha->m_ExtraSlot->m_Special2
									|| m_WareInven[m_nPackNum][slot_x_num][slot_y_num]->m_Special				!= m_pNkCha->m_ExtraSlot->m_Special
							  )
							{
								char szBuf[1024];
								sprintf( szBuf, "Not returnable %d, %p, %d, %d \n", m_WareSlot[m_nPackNum][slot_x_num][slot_y_num],
										 m_WareInven[m_nPackNum][slot_x_num][slot_y_num],
										 m_WareInven[m_nPackNum][slot_x_num][slot_y_num]->m_Vnum,
										 m_pNkCha->m_ExtraSlot->m_Vnum );
								OutputDebugString( szBuf );
								return 1;
							}

						item_plus = m_pNkCha->m_ExtraSlot->m_GemNum;
						item_flag = m_pNkCha->m_ExtraSlot->m_Special;

						// [4/18/2007 Theodoric] �±� ���� â�� :: ĳ�� â���� +3�� �ؼ� ������ ��������Ѵ�.
						int nPackNum = m_nPackNum;
						if( m_bCash )
						{
							nPackNum += 3;
						}

						slot_index = nPackNum*WARE_PACK_INTERVAL + slot_index;
						BOOL bBySummonNpc = this->IsFromSummonNpc();
						sprintf(m_MsgBuf, "stash_click %d %d %d %d %d\n", bBySummonNpc, slot_index, vnum, item_plus, item_flag);

						if( CommonConfig::Instance()->GetStashPopup() == true )
						{
							m_strStashPacketTemp = m_MsgBuf;
							g_pNk2DFrame->InsertPopup(" Click OK if you confirm to keep out item in warehouse ", TYPE_NOR_OKCANCLE, POPUP_STASH_FROM_WAREHOUSE_PROC);
						}
						else
						{
							m_bMsgWaiting = TRUE;
							SendMsg();
						}						
					}
				}
				else if (m_WareSlot[m_nPackNum][slot_x_num][slot_y_num] > 0)
				{
					int chaotic = g_pRoh->m_Chaotic_Num * 0.1;
					if( chaotic < 0 && chaotic > -500 )
					{
					}
					else if( chaotic <= -500 )
					{
						if( g_dev_client )
							g_pNk2DFrame->AddChatStr( "Can not find" , -1 );
						return 1;
					}

					if( m_WareInven[m_nPackNum][slot_x_num][slot_y_num] )
					{
						vnum = m_WareInven[m_nPackNum][slot_x_num][slot_y_num]->m_Vnum;
						item_plus = m_WareInven[m_nPackNum][slot_x_num][slot_y_num]->m_GemNum;
						item_flag = m_WareInven[m_nPackNum][slot_x_num][slot_y_num]->m_Special;
					}

					int nPackNum = m_nPackNum;
					if( m_bCash )
					{
						nPackNum += 3;
					}

					slot_index = nPackNum*WARE_PACK_INTERVAL + slot_index;
					BOOL bBySummonNpc = this->IsFromSummonNpc();
					sprintf(m_MsgBuf, "stash_click %d %d %d %d %d\n", bBySummonNpc, slot_index, vnum, item_plus, item_flag);

					if( CommonConfig::Instance()->GetStashPopup() == true )
					{
						m_strStashPacketTemp = m_MsgBuf;
						g_pNk2DFrame->InsertPopup(" Click OK if you confirm to keep in item in warehouse ", TYPE_NOR_OKCANCLE, POPUP_STASH_FROM_WAREHOUSE_PROC);
					}
					else
					{
						m_bMsgWaiting = TRUE;
						
						if( m_WareInven[m_nPackNum][slot_x_num][slot_y_num]->m_bTimeLimit )
						{
							if ( g_dwClientCountry == CTRY_JPN )
							{
								g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_TIMELIMIT_ITEMSTR), TYPE_NOR_OKCANCLE, 64 );

								g_pNk2DFrame->WareHouseStrCopy( m_MsgBuf );
							}
						}
						else
						{
							SendMsg();
						}
					}
				}
			}
			return 1;
		}
		else if (IsPlayArea(x, y) && !m_bMsgWaiting)
		{
			if (m_bActivate)
				// â�� �ݴ´�.
				ActivateWareHouse(FALSE);

			g_pNk2DFrame->ShowWareHouseWindow(FALSE);
		}
		else if (m_bMsgWaiting)
			return 0;
		else
			return 0;

		return 1;
//=================================================================================
	// nate : ������ �ٷ��̵�����
	case WM_RBUTTONDBLCLK:
		{
			// ExtraSlot�� �������� ������쿡�� �ȵ�
			if( m_pNkCha->m_ExtraSlot )
				return 0;

			x = LOWORD (lParam);
			y = HIWORD (lParam);

			//����Ȯ��
			if(IsInside(x, y))
			{
				// �������� ��
				// �׳� LButtonclk ��������
				if (m_bGetStashEnd == FALSE && m_bActivate)
					return 1;

				// Ŀ���� �����ȿ� �ִ��� �˻�
				if ( x >= m_wareslot_start_x
						&& x < m_wareslot_start_x + WARE_SLOT_WIDTH*WARE_SLOT_X_NUM
						&& y >= m_wareslot_start_y
						&& y < m_wareslot_start_y + WARE_SLOT_HEIGHT*WARE_SLOT_Y_NUM )
				{

					int slot_x_num, slot_y_num, slot_index;
					int nVnum;

					// ���Գѹ��� �ѿ����� ���� �ε��� ���
					slot_x_num = (x - m_wareslot_start_x) / WARE_SLOT_WIDTH;
					slot_y_num = (y - m_wareslot_start_y) / WARE_SLOT_HEIGHT;
					slot_index = slot_x_num * WARE_SLOT_Y_NUM + slot_y_num;

					// ���Կ� �������� ������
					if ( m_WareSlot[ m_nPackNum ][ slot_x_num ][ slot_y_num ] > 0 )
					{

						int chaotic = g_pRoh->m_Chaotic_Num * 0.1;
						if( chaotic < 0 && chaotic > -500 )
						{
							// 3���� ����
						}
						else if( chaotic <= -500 )
						{
							if( g_dev_client )
								g_pNk2DFrame->AddChatStr( "Can not find" , -1 );
							return 0;
						}

						// ���� ������ �����۹�ȣ ����
						if( m_WareInven[ m_nPackNum ][ slot_x_num ][ slot_y_num ] )
							nVnum = m_WareInven[ m_nPackNum ][ slot_x_num ][ slot_y_num ]->m_Vnum;

						// �������� ũ�� ���
						int nWidth = GET_ITEM_WIDTH( m_WareInven[ m_nPackNum ][ slot_x_num ][ slot_y_num ] );
						int nHeight = GET_ITEM_HEIGHT( m_WareInven[ m_nPackNum ][ slot_x_num ][ slot_y_num ] );

						// 1ĭ¥�� �����۸� �ٷ� �̵� ����
						if( nWidth != 1 || nHeight != 1 )
							return 0;

						// 1.5�� ������
						if ( m_bWareLock)
						{
							DWORD curTime = timeGetTime();
							if (curTime > m_timeWareLock + 1500) // 1.5�ʰ� ��������,
								m_bWareLock = FALSE;
							else
								return 1;
						}

						// �� ����
						SetWareLock( TRUE );

						// [4/18/2007 Theodoric] �±� ���� â�� :: ĳ�� â���� +3�� �ؼ� ������ ��������Ѵ�.
						int nPackNum = m_nPackNum;
						if( m_bCash )
						{
							nPackNum += 3;
						}

						// WARE_PACK_INTERVAL  = 20  : �Ѱ��� �ε��� ����st
						slot_index = nPackNum * WARE_PACK_INTERVAL + slot_index;

						if( m_pNkCha )
							sprintf(m_MsgBuf, "stash_get %d %d %d %d\n", nVnum, m_pNkCha->m_CurPackNum, nPackNum, slot_index );

						if( CommonConfig::Instance()->GetStashPopup() == true )
						{
							m_strStashPacketTemp = m_MsgBuf;
							g_pNk2DFrame->InsertPopup(" Click OK if you confirm to keep in item in warehouse ", TYPE_NOR_OKCANCLE, POPUP_STASH_FROM_WAREHOUSE_PROC);
						}
						else
						{
							m_bMsgWaiting = TRUE;
							SendMsg();
						}
					}
				}

			}
		}
		break;
//=================================================================================

	case WM_LBUTTONUP:
		x = LOWORD (lParam);
		y = HIWORD (lParam);
		if(IsInside(x, y))
		{
			if (m_bGetStashEnd == FALSE && m_bActivate)
				return 1;

			{
				if( m_bCash)
				{
					m_pNormalViewBtn->MsgProc(hWnd, msg, wParam, lParam);
				}
				else
				{
					m_pCashViewBtn->MsgProc(hWnd, msg, wParam, lParam);
				}
			}

			if( !m_bCash ) // ����Ȯ�� â��� ������ ���� ���Ѵ�. ��/��� ��ư�� �Ⱥ��̰� �����ش�. ///071015
			{
				m_InMoneyBtn.MsgProc(hWnd, msg, wParam, lParam);
				m_OutMoneyBtn.MsgProc(hWnd, msg, wParam, lParam);
			}
			m_CloseBtn.MsgProc(hWnd, msg, wParam, lParam);
			if(m_InMoneyBtn.GetState() == BTN_ACTION && m_InMoneyBtn.IsInside(x,y))
			{

				//-- IDS_WAREHOUSE_INMONEY :  �Ա��Ͻ� �ݾ���   �����ּ���
				g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_WAREHOUSE_INMONEY), TYPE_EXCHANGE, 19 );
				m_byInOutMoneyState = 1;
				m_InMoneyBtn.m_iBtnFlg = BTN_NORMAL;
			}
			else if(m_OutMoneyBtn.GetState() == BTN_ACTION && m_OutMoneyBtn.IsInside(x,y))
			{

				int chaotic = g_pRoh->m_Chaotic_Num * 0.1;
				if( chaotic < 0 && chaotic > -500 )
				{
					// 3���� ����
				}
				else if( chaotic <= -500 )
				{
					if( g_dev_client )
						g_pNk2DFrame->AddChatStr("No withdrawal",-1);
					return 0;
				}

				//-- IDS_WAREHOUSE_OUTMONEY : ����Ͻ� �ݾ���   �����ּ���
				g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_WAREHOUSE_OUTMONEY), TYPE_EXCHANGE, 19 );
				m_byInOutMoneyState = 2;
				m_OutMoneyBtn.m_iBtnFlg = BTN_NORMAL;
			}
			else if (m_CloseBtn.GetState() == BTN_ACTION)
			{
				if( g_pNk2DFrame )
					g_pNk2DFrame->ShowWareHouseWindow(FALSE);
				m_CloseBtn.SetState(BTN_NORMAL);
			}
			return 1;
		}
		else
			return 0;

		break;
	}
	return 0;
}


BOOL CControlWareHouse::InMoneyMsg(char* money)
{
	BOOL bBySummonNpc = this->IsFromSummonNpc();
	sprintf(m_MsgBuf, "stash_click %d 12 0 ", bBySummonNpc);
	strcat(m_MsgBuf, money);
	strcat(m_MsgBuf, "\n");
	SendMsg();
	m_byInOutMoneyState = 0;

	return true;
}
BOOL CControlWareHouse::OutMoneyMsg(char* money)
{
	BOOL bBySummonNpc = this->IsFromSummonNpc();
	sprintf(m_MsgBuf, "stash_click %d 12 0 ", bBySummonNpc);
	strcat(m_MsgBuf, "-");
	strcat(m_MsgBuf, money);
	strcat(m_MsgBuf, "\n");
	SendMsg();
	m_byInOutMoneyState = 0;

	return true;
}


BOOL CControlWareHouse::IsInside(int x, int y)
{
	if( !m_pBack )
		return false;

	return m_pBack->IsIn(x, y);
}

int CControlWareHouse::GetWareSlotIndex (int x, int y)
{
	if (x < 0 || x >= WARE_SLOT_X_NUM || y < 0 || y >= WARE_SLOT_Y_NUM)
		return -1;

	return m_WareSlot[m_nPackNum][x][y];
}

// â���� ���� ������.
// â�� ����Ÿ�� �����Ѵ�.
void CControlWareHouse::RemoveAll()
{
	int i, j, k;

	for (k=0; k < WARE_PACK_NUM; k++)
	{
		for (i=0; i < WARE_SLOT_X_NUM; i++)
		{
			for (j=0; j < WARE_SLOT_Y_NUM; j++)
				SAFE_DELETE( m_WareInven[k][i][j] );
		}
	}

	// ���Ե���Ÿ �ʱ�ȭ.
	for (k=0; k < WARE_PACK_NUM; k++)
	{
		for ( i=0; i < WARE_SLOT_X_NUM; i++)
		{
			for (j=0; j < WARE_SLOT_Y_NUM; j++)
			{
				m_WareSlot[k][i][j] = 0;
			}
		}
	}

	m_nSaveMoney = 0;
	// [4/18/2007 Theodoric] �±� ���� â��
	m_bCash = FALSE;
	m_nCashMaxPackNum = 0;
}

void CControlWareHouse::ActivateWareHouse(BOOL bActivate)
{
	if (bActivate)
	{
		RemoveAll();
		strcpy(m_MsgBuf, "stash_open\n");

		CControlWareHouse* wareHouse = g_pNk2DFrame->GetControlWareHouse();
		BOOL bBySummonNpc = wareHouse->IsFromSummonNpc();
		if( pCMyApp->m_pUIMgr && g_nCurWorldIdx !=4  && g_nCurWorldIdx != 11 && bBySummonNpc == FALSE )
		{
			g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_WAREHOUSE_ALARM), TYPE_NOR_OK, 1 );
		}

		if( g_pNk2DFrame )
		{
			g_pNk2DFrame->SetShowWareHouseFlag(TRUE);
		}
	}
	else
	{
		strcpy(m_MsgBuf, "stash_close\n");
		m_bGetStashEnd = FALSE;
		RemoveAll();

		m_bStoreWidening = false;
	}

	m_bActivate = bActivate;
	
	if (m_bActivate == FALSE)
		m_bGetStashEnd = FALSE;
	
	SendMsg();
}

BOOL CControlWareHouse::IsPlayArea(int x, int y)
{
	if( !IsInside(x,y) && !g_pNk2DFrame->IsInPopup(x,y) )
		return true;

	return false;
}

void CControlWareHouse::SendMsg()
{
	if (g_pTcpIp)
		g_pTcpIp->SendNetMessage(m_MsgBuf);
}

// [6/1/2007 Theodoric] ������ ���� :: �Լ� �Ķ���� ������ ����
void CControlWareHouse::Stash(int slot_num, int vnum, int plus, int special, int special2,
							  int UpEndur, int MaxUpdEndur, int Endur, int MaxEndur, int count, int time_limit ) ///�ð��������� �ð��� �߰�(050711 by ����) ///�Ź���ȭ�ý���
{
	// ��ġ �ľ�.
	int x = 0, y = 0;
	CItem *pItem=NULL;
	int pack_num = 0;		// �ӽ�.

	pack_num = slot_num / WARE_PACK_INTERVAL;
	slot_num = slot_num % WARE_PACK_INTERVAL;

	// [4/18/2007 Theodoric] �±� ���� â��
	if( m_bCash && slot_num != 12 )
	{
		pack_num  = pack_num - 3;
	}

	if (slot_num == 12)  				// ���� ���� ���̴�.
	{
		m_nSaveMoney = plus;
		m_MoneyText.SetString((m_nSaveMoney*100), true);
		return;
	}

	x = slot_num / WARE_SLOT_Y_NUM;
	y = slot_num % WARE_SLOT_Y_NUM;

	// �̹� ���Կ� ��ǰ�� �ִ� ���� ���� ��츦 �����Ѵ�.
	// ���� �ܰ迡�� �� �ؾ� �Ǵ����� ���ؼ� �ǹ�.
	if (m_WareSlot[pack_num][x][y] == 0)
	{
		m_WareSlot[pack_num][x][y] = count;

		pItem = new CItem;
		if( !pItem )
			return;

		pItem->m_SlotX = x;
		pItem->m_SlotY = y;
		pItem->m_Index = slot_num;		// �ӽ� �ε���
		pItem->m_Vnum = vnum;
		pItem->m_Special = special;

		///�Ź���ȭ�ý���
		pItem->m_Special2 = special2; // �߰� �ɼ�
		pItem->m_UpgradeEndurance = UpEndur;           // ���� ��ȭ ������
		pItem->m_MaxUpgradeEndurance = MaxUpdEndur;    // ��ȭ ������ �ִ�ġ

		pItem->m_Endurance = Endur;
		pItem->m_MaxEndurance = MaxEndur;

		pItem->m_ProtoNum = g_ItemList.FindItem(vnum);

		if(GET_TYPE(pItem)!=ITYPE_SUB)
		{
			if(g_ItemList.m_ItemArray[pItem->m_ProtoNum].m_MinLevel>=181)
				pItem->SetDiaGemNum(plus);
			else
				pItem->SetGemNum(plus);
		}
		else
			pItem->m_PlusNum = plus;

		// ������ ���� �Է��غ���!
		ResetItemValue(pItem);
		SetSpecialValue(pItem, GET_TYPE(pItem), GET_SHAPE(pItem), special, special2);

		if( time_limit ) // �ð� ���� ������ �ð��� ���������� ���� (by ����) ///�ð���������
			pItem->SetTimeLimitItem( time_limit );

		m_WareInven[pack_num][x][y] = pItem;
	}


	if (count > 0)
	{
		char tmp[10] = "";
		sprintf(tmp, "%d",count);
		m_pNumText[pack_num][x][y].SetString(tmp);
		m_WareInven[pack_num][x][y]->m_SlotX = x;
		m_WareInven[pack_num][x][y]->m_SlotY = y;
	}
	m_bMsgWaiting = FALSE;
}

// nate : extra���� â���ش罽������ �������� �Ű�����.
// [6/1/2007 Theodoric] ������ ���� :: �Լ� �Ķ���� ������ ����
void CControlWareHouse::StashTo(int slot_num, int vnum, int plus, int special, int special2,
								int UpEndur, int MaxUpdEndur, int Endur, int MaxEndur, int count, int swap_flag) ///�Ź���ȭ�ý���
// ���� : â���Գ�, �����۳�, �÷�����, ����ȳ�, â���� ī��Ʈ, ��ȯ�÷���
// �κ� -> â��, ExtraSlot -> â�� (��ȯ�÷��� : ��ġ�� ������ 1�ϰ�� ��ȯ )
// â�� �������� �ְ� ī��Ʈ�� ���
{
	int x = 0, y = 0, pack_num = 0;
	CItem *pItem=NULL;

	// ���Գ����� �ε��� ���
	// ���Գ��� ������ ( ���� ���������� ���� ������(?)���� ���� )
	pack_num = slot_num / WARE_PACK_INTERVAL;		// WARE_PACK_INTERVAL = 20
	slot_num = slot_num % WARE_PACK_INTERVAL;		// �� ������ ������ 20���� �����Ǿ� ����

	// [4/18/2007 Theodoric] �±� ���� â��
	if( m_bCash && slot_num != 12 )
	{
		pack_num  = pack_num - 3;
	}

	// ���� ������ ��ǥ ���
	x = slot_num / WARE_SLOT_Y_NUM;
	y = slot_num % WARE_SLOT_Y_NUM;

	// �� �ϳ��� 11���� ����
	// 12�� �ӴϽ���
	if (slot_num == 12)  				// ���� ���� ���̴�.
	{
		m_nSaveMoney = plus;
		m_MoneyText.SetString((m_nSaveMoney*100), true);
		return;
	}

	// ���� â�� �� �����ΰ�?
	if (m_WareSlot[pack_num][x][y] == 0 )
	{
		// ���Կ� ������ �׸� ����.
		if (swap_flag)
		{
			// â���Կ� ���� ���ڰ� ������ ����
			m_WareInven[pack_num][x][y] = new CItem;
			if( !m_WareInven[pack_num][x][y] )
				return;

			m_WareInven[pack_num][x][y]->m_Vnum = vnum;
			m_WareInven[pack_num][x][y]->m_ProtoNum = g_ItemList.FindItem(vnum);

			if(GET_TYPE(m_WareInven[pack_num][x][y])!=ITYPE_SUB)
			{
				if(g_ItemList.m_ItemArray[m_WareInven[pack_num][x][y]->m_ProtoNum].m_MinLevel>=181)
					// ���̾Ʊ� ���� : ���̾Ʊ��� �����ִ� gem�Ӽ��� ���� ���̾� ������ �ٷ� �÷������̴�.
					m_WareInven[pack_num][x][y]->SetDiaGemNum(plus);
				else
					// �׿� ���ѿ����� �÷����� ����
					m_WareInven[pack_num][x][y]->SetGemNum(plus);
			}
			else
				// ���� �������� ���� ������ �÷��� ��
				m_WareInven[pack_num][x][y]->m_PlusNum = plus;

			m_WareInven[pack_num][x][y]->m_Special = special;

			///�Ź���ȭ�ý���
			m_WareInven[pack_num][x][y]->m_Special2 = special2; // �߰� �ɼ�
			m_WareInven[pack_num][x][y]->m_UpgradeEndurance = UpEndur;           // ���� ��ȭ ������
			m_WareInven[pack_num][x][y]->m_MaxUpgradeEndurance = MaxUpdEndur;    // ��ȭ ������ �ִ�ġ

			m_WareInven[pack_num][x][y]->m_Endurance = Endur;
			m_WareInven[pack_num][x][y]->m_MaxEndurance = MaxEndur;

			// ������ �Ӽ��� ����
			// ������ ���� �Է��غ���!
			ResetItemValue(m_WareInven[pack_num][x][y]);
			SetSpecialValue(m_WareInven[pack_num][x][y], GET_TYPE(m_WareInven[pack_num][x][y]), GET_SHAPE(m_WareInven[pack_num][x][y]), special, special2);
		}
		else
		{
			// ExtraSlot�� �ִ� �������� â�� �ִ´�.
			// ExtraSlot�� �������� �ִ´�.
			m_WareInven[pack_num][x][y] = m_pNkCha->m_ExtraSlot;	// m_WareInven : â���� ������ ����
			m_pNkCha->m_ExtraSlot = NULL;
		}

		if (count < 0)
		{
			// ī��Ʈ�� 0���� ������ �� ����
			// m_WareSlot�� m_WareInven, m_pNumText�� �ϳ��� ����ü�� ���� ���°� �� ������ ������
			m_WareSlot[pack_num][x][y] ++;		// â�� ������ ī��Ʈ����
			count = m_WareSlot[pack_num][x][y];	// count������ ���� ī��Ʋ�� ���ϼ��� ����
		}
		else
			m_WareSlot[pack_num][x][y] = count;
	}
	else
	{
		// â�� ������ �󽽷��� �ƴϸ�
		if (count < 0)
		{
			m_WareSlot[pack_num][x][y] ++;
			count = m_WareSlot[pack_num][x][y];
		}
		else
			m_WareSlot[pack_num][x][y] = count;

		// extra ����.
		if (!swap_flag)
			SAFE_DELETE( m_pNkCha->m_ExtraSlot );
	}

	char tmp[10] = "";
	// ���Կ� �������� ������ ���԰��� ���
	if( count > 0 )
	{
		sprintf(tmp, "%d",count);
		m_pNumText[pack_num][x][y].SetString(tmp);

		if( m_WareInven[pack_num][x][y] )
		{
			m_WareInven[pack_num][x][y]->m_SlotX = x;
			m_WareInven[pack_num][x][y]->m_SlotY = y;
		}
		m_bMsgWaiting = FALSE;
	}
}

// [6/1/2007 Theodoric] ������ ���� :: �Լ� �Ķ���� ������ ����
// nate : â�� -> �κ�, â�� -> Extra Slot
// ���� : â���Գ�, �������ε���, �����۹�ȣ, �÷�����, �����, �����۰���
void CControlWareHouse::StashFrom(int slot_num, int index, int vnum, int plus, int &special, int special2,
								  int UpEndur, int MaxUpdEndur, int Endur, int MaxEndur, int count) ///�Ź���ȭ�ý��� // �̳༮�� �� special�� & �ٿ��� ���� �޴��� �𸣰ڴ�...
{
	int x, y, pack_num;
	int special_temp = 0;
	CItem *pItem = NULL;

	// â�� ���� ���
	pack_num = slot_num / WARE_PACK_INTERVAL;
	slot_num = slot_num % WARE_PACK_INTERVAL;

	// [4/18/2007 Theodoric] �±� ���� â��
	if( m_bCash && slot_num != 12 )
	{
		pack_num  = pack_num - 3;
	}

	x = slot_num / WARE_SLOT_Y_NUM;
	y = slot_num % WARE_SLOT_Y_NUM;

	// ����â
	if (slot_num == 12)  				// ���� ���� ���̴�.
	{
		m_nSaveMoney = vnum;
		m_MoneyText.SetString((m_nSaveMoney*100), true);
		return;
	}

	if (m_WareInven[pack_num][x][y])
		special_temp = m_WareInven[pack_num][x][y]->m_Special;

	// -1 �̻��̸� �ٵ����°� �ƴѰ�?
	if (count >= -1)
	{
		// ������ �ٷ��̵� ��ɽ�
		if (count < 0)
			m_WareSlot[pack_num][x][y]--;	// ī��Ʈ ����
		else
			m_WareSlot[pack_num][x][y] = count;

		// ���� ���Կ� �������� ������
		//if (m_WareSlot[pack_num][x][y] == 0)
		if (m_WareSlot[pack_num][x][y] <= 0)
		{
			SAFE_DELETE(m_WareInven[pack_num][x][y]);
			m_WareInven[pack_num][x][y] = NULL;
			m_WareSlot[pack_num][x][y] = 0;
		}

		// �������� �ְ� �ٷ��̵� ����� �ƴϸ�
		if (m_pNkCha && count >= 0)
		{
			// ExtraSlot�� �������� �ִ°� üũ
			SAFE_DELETE( m_pNkCha->m_ExtraSlot );

			// ExtraSlot�� ������ ����
			m_pNkCha->m_ExtraSlot = new CItem;
			if(m_pNkCha->m_ExtraSlot)
			{
				m_pNkCha->m_ExtraSlot->m_Index = index;
				m_pNkCha->m_ExtraSlot->m_Vnum = vnum;
				m_pNkCha->m_ExtraSlot->m_ProtoNum = g_ItemList.FindItem(vnum);;

				// ExtraSlot�� �������� ����( ���� ) ���������� üũ
				if(GET_TYPE(m_pNkCha->m_ExtraSlot)!=ITYPE_SUB) //1111
				{

					// Ŭ���̾�Ʈ�� ������ ��ü����� ������ �ִ�
					// ��Ͽ��� ProtoNum���� �������� ������ ã�´�.
					// ���̾Ʊ��ΰ�?
					if(g_ItemList.m_ItemArray[m_pNkCha->m_ExtraSlot->m_ProtoNum].m_MinLevel>=181)
						m_pNkCha->m_ExtraSlot->SetDiaGemNum(plus);
					else	// ���̾� ���ϱ�
						m_pNkCha->m_ExtraSlot->SetGemNum(plus);
				}
				else	// ����������� ���
					m_pNkCha->m_ExtraSlot->m_PlusNum = plus;

				m_pNkCha->m_ExtraSlot->m_Special = special;

				///�Ź���ȭ�ý���
				m_pNkCha->m_ExtraSlot->m_Special2 = special2; // �߰� �ɼ�
				m_pNkCha->m_ExtraSlot->m_UpgradeEndurance = UpEndur;           // ���� ��ȭ ������
				m_pNkCha->m_ExtraSlot->m_MaxUpgradeEndurance = MaxUpdEndur;    // ��ȭ ������ �ִ�ġ

				m_pNkCha->m_ExtraSlot->m_Endurance = Endur;
				m_pNkCha->m_ExtraSlot->m_MaxEndurance = MaxEndur;

				// ������ ���� �Է��غ���!
				// ������ �Ӽ����� �����ϴ� �Լ�, �Ʒ� �ּ��κ�
				ResetItemValue(m_pNkCha->m_ExtraSlot);
				// �������� ����Ȱ� ����
				SetSpecialValue(m_pNkCha->m_ExtraSlot, GET_TYPE(m_pNkCha->m_ExtraSlot), GET_SHAPE(m_pNkCha->m_ExtraSlot), special, special2);
			}
		}
		/*

		//���� ���� Ŭ�� �ٷ� �̵��� ���� ������ �ּ� ó����... ������ �ɵ� ������,, �ϴ� ���ܵ�.

		else {	// ������ �ٷ��̵��� ���

			// ���õ� â���Կ� �������� �ִ��� Ȯ��
			// ��ȸ��Ȳ�� ���� ó��
			// Ȥ�ó� �κ��� �����Ͱ� ������ �ϴ� ���
			SAFE_DELETE( m_WareInven[pack_num][x][y] );
			// ������ ����
			m_WareInven[pack_num][x][y] = new CItem;
			if( m_WareInven[pack_num][x][y] )
			{
				m_WareInven[pack_num][x][y]->m_Index = index;
				m_WareInven[pack_num][x][y]->m_Vnum = vnum;
				m_WareInven[pack_num][x][y]->m_ProtoNum = g_ItemList.FindItem(vnum);

				// ������������� üũ
				if(GET_TYPE(m_WareInven[pack_num][x][y])!=ITYPE_SUB){//1111
					if(g_ItemList.m_ItemArray[m_WareInven[pack_num][x][y]->m_ProtoNum].m_MinLevel>=181)
						m_WareInven[pack_num][x][y]->SetDiaGemNum(plus);
					else
						m_WareInven[pack_num][x][y]->SetGemNum(plus);
				}
				else
					m_WareInven[pack_num][x][y]->m_PlusNum = plus;

				m_WareInven[pack_num][x][y]->m_Special = special;

				///�Ź���ȭ�ý���
				m_WareInven[pack_num][x][y]->m_Special2 = special2; // �߰� �ɼ�
				m_WareInven[pack_num][x][y]->m_UpgradeEndurance = UpEndur;           // ���� ��ȭ ������
				m_WareInven[pack_num][x][y]->m_MaxUpgradeEndurance = MaxUpdEndur;    // ��ȭ ������ �ִ�ġ

				m_WareInven[pack_num][x][y]->m_Endurance = Endur;
				m_WareInven[pack_num][x][y]->m_MaxEndurance = MaxEndur;

				// ������ ���� �Է��غ���!
				ResetItemValue(m_WareInven[pack_num][x][y]);
				SetSpecialValue(m_WareInven[pack_num][x][y], GET_TYPE(m_WareInven[pack_num][x][y]), GET_SHAPE(m_WareInven[pack_num][x][y]), special, special2);
			}
		}*/
		// �۾���� ����
		m_bMsgWaiting = FALSE;
	}

	// â������ �������� 0 �̻��̸� ���� ���
	if ( m_bActivate && m_WareSlot[pack_num][x][y] > 0) // m_bActivate üũ�߰�. ��Ŷ �������� â �ݾƹ����� ƨ��� ����. 06-03-18 ����.
	{
		char tmp[10] = "";

		sprintf(tmp, "%d",m_WareSlot[pack_num][x][y]);
		m_pNumText[pack_num][x][y].SetString(tmp);

		// â���κ��� ��ǥ ����
		if( m_WareInven[pack_num][x][y] )
		{
			m_WareInven[pack_num][x][y]->m_SlotX = x;
			m_WareInven[pack_num][x][y]->m_SlotY = y;
		}
	}
	special = special_temp;
}

void CControlWareHouse::StashEnd()
{
	m_bGetStashEnd = TRUE;
}

// nate : â�� ������ �ٷ��̵� ��� �޼��� ó����
// ���� : �����۹�ȣ, �κ��ѹ�ȣ, â���ѹ�ȣ, â���Թ�ȣ, �κ� X, �κ� Y
// �κ� -> â��
// ��� : �κ��� ������ ����, â�� ������ �߰�
void CControlWareHouse::StashPut( int nVnum, int nInvenPackNum, int nWarePackNum, int nWareSlotNum, int nSlot_X, int nSlot_Y )
{
	if( !m_pNkCha )
		return;

	CItem *pItem = NULL;
	// �κ� ���� ��ġ�� ������ �ε��� ����
	int item_idx = m_pNkCha->GetItemIndexInSlot( nInvenPackNum, nSlot_X, nSlot_Y, 1, 1 );
	if ( item_idx >= 0 )
		// ���� �κ��ѳѿ� �ִ� �������� �ε����� �˻��ؼ� ����
		// �Լ���ü�� �޸� ���� ����( ���� �ʿ� )
		pItem = m_pNkCha->PickItemInPack( nInvenPackNum, item_idx );

	int nSpecial = 0;
	int nPlusNum = 0;

	// ���� �������� ���޵��ε����� �������� �������
	if ( pItem )
	{
		nSpecial = pItem->m_Special;
		if( GET_TYPE( pItem ) != ITYPE_SUB )	// ��������� ��[��
		{
			// �����ۿ��� ���� ������ ������ �մ�.
			// 181�� ���̾Ʊ޾�����
			// �� �������� �÷������� �״�� ���
			// PlusNum �� GemNum ���� ���̸� �˾ƾ� �� SetGemNum() �Լ� ����

			//-- 2004. 03. 22  / Lyul
			//-- ������ ���̾� ��� �ν� ���� Big Fix
			if( GET_ITEM_LEVEL(pItem) >= 181 )
				nPlusNum = pItem->m_PlusNum;
			else
			{
				// �������� ������ŭ�� ������ �߰���
				// �� : 1�� 1�� , 2�� 1 + 2��, 5�� 10 + 5�� �ʿ�
				for( int i = 1; i <= pItem->m_PlusNum; i++ )
					nPlusNum += i;
			}
		}
		else // ���� �������� ���
			nPlusNum = pItem->m_PlusNum;

		// â�� �������� �ְ� ī��Ʈ�� ���
		// �����ʿ� : ������ ExtraSlot�� ����ϴ� �Լ��� ���̷�Ʈ �̵��� �ʿ��� �Լ��� �ʿ�
		if( g_SvrType == ST_ADULT_ONLY ) // [6/1/2007 Theodoric] ������ ����
		{
			StashTo( nWareSlotNum, nVnum, nPlusNum, nSpecial, pItem->m_Special2, pItem->m_UpgradeEndurance,
					 pItem->m_MaxUpgradeEndurance, pItem->m_Endurance, pItem->m_MaxEndurance, -1/* â������ ī��Ʈ*/, 1/*swap_flag*/); ///�Ź���ȭ�ý���
		}
		else
		{
			StashTo( nWareSlotNum, nVnum, nPlusNum, nSpecial, pItem->m_Special2, pItem->m_UpgradeEndurance,
					 pItem->m_MaxUpgradeEndurance,0, 0, -1/* â������ ī��Ʈ*/, 1/*swap_flag*/); ///�Ź���ȭ�ý���
		}
	}

	// ������ �߰��� ����
	SAFE_DELETE( pItem );
}


// nate : â�� ������ �ٷ��̵� ��� �޼��� ó����
// ���� : �����۹�ȣ, �κ��ѹ�ȣ, â���ѹ�ȣ, â���Թ�ȣ, ������ �ε�����, �κ� X, �κ� Y
// â�� -> �κ�
// ��� : �κ��� ������ �߰�, â�� ������ ����
void CControlWareHouse::StashGet( int nVnum, int nInvenPackNum, int nWarePackNum, int nWareSlotNum, int nItemIndex, int nSlot_X, int nSlot_Y )
{
	//  [11/26/2008 parkmj] ����â��� ����Ŭ������ ������ ���� �κ� �߰�
	// [4/18/2007 Theodoric] �±� ���� â��
	if( m_bCash && nWareSlotNum != 12 )
	{
		nWarePackNum  = nWarePackNum - 3;
	}

	// �������� Interval�������� �Ѿ�� ���Գ��� �������� ���� ������ ���
	int nTempSlotNum = nWareSlotNum % WARE_PACK_INTERVAL;

	// ���� ������ X, Y��ǥ ���
	int nX = nTempSlotNum / WARE_SLOT_Y_NUM;
	int nY = nTempSlotNum % WARE_SLOT_Y_NUM;

	// ������ â���� ������ ����
	CItem *pItem = m_WareInven[ nWarePackNum ][ nX ][ nY ];

	// �������� ������
	if( pItem )
	{
		int nSpecial = pItem->m_Special;
		int nSpecial2 = pItem->m_Special2;
		int nPlusNum = 0;
		int nUpgradeEndurance = 0;
		int nMaxUpgradeEndurance = 0;

		int	nEndur = 0;
		int nMaxEndur = 0;

		// ���� �������� �����̵� �����̵� �޼�����.
		if( GET_TYPE( pItem ) != ITYPE_SUB )	// �����������ΰ�?
		{
			if( GET_ITEM_LEVEL(pItem) >= 181 )	// ���̾Ʊ��� �÷����� �״�� ����
				nPlusNum = pItem->m_PlusNum;
			else
			{
				// �������� ������ŭ�� ������ �߰���
				// �� : 1�� 1�� , 2�� 1 + 2��, 5�� 10 + 5�� �ʿ�
				for( int i = 1; i <= pItem->m_PlusNum; i++ )
					nPlusNum += i;
			}
		}
		else // ���� �������� ��� �������� �״�� ����
			nPlusNum = pItem->m_PlusNum;

		nUpgradeEndurance = pItem->m_UpgradeEndurance; // ���� ���������۶����� ��ġ�� �ٲ�� ��찡 �ִ� ��. ������ �޾Ƽ� ����ϰ� ����Ѵ�.
		nMaxUpgradeEndurance = pItem->m_MaxUpgradeEndurance; // ���� ���������۶����� ��ġ�� �ٲ�� ��찡 �ִ� ��. ������ �޾Ƽ� ����ϰ� ����Ѵ�.

		nEndur = pItem->m_Endurance;
		nMaxEndur = pItem->m_MaxEndurance;

		// [6/7/2007 Theodoric] ������ ����
		StashFrom( nWareSlotNum, nItemIndex, nVnum, nPlusNum/*GemNum*/, nSpecial/*special*/,nSpecial2,
				   nUpgradeEndurance, nMaxUpgradeEndurance,nEndur, nMaxEndur, -1/*Item count�� �÷��׷� ���*/ ); ///�Ź���ȭ�ý���
		if( g_pRoh )
		{
			// [6/7/2007 Theodoric] ������ ����
			g_pRoh->AddToInven( nInvenPackNum, nSlot_X, nSlot_Y, nItemIndex, nVnum, nPlusNum/*GetNum*/, nSpecial/*special*/, nSpecial2, nUpgradeEndurance, nMaxUpgradeEndurance, nEndur, nMaxEndur  ); ///�Ź���ȭ�ý���
		}
	}
}

LRESULT CControlWareHouse::MoneyPopupMsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, int nMsg, char* szMoney)
{
	// "Ȯ��" ��ư�� �������
	if (nMsg == 1)
	{
		if(CheckMoneyStr(szMoney) == TRUE)
		{
			if(m_byInOutMoneyState == 1)
				InMoneyMsg(szMoney);
			if(m_byInOutMoneyState == 2)
				OutMoneyMsg(szMoney);
		}
	}
	else if (nMsg == -1)	// "���" ��ư�� �������
		m_byInOutMoneyState = 0;

	return 0L;
}

LRESULT CControlWareHouse::PopupMsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, int nMsg)
{
	if( nMsg == 1)	// Ȯ�ι�ư�� �������
	{
		strcpy(m_MsgBuf, m_strStashPacketTemp.c_str());
		SendMsg();
		m_bMsgWaiting = TRUE;
	}
	else if(nMsg == -1) // ��ҹ�ư�� �������
	{
		m_strStashPacketTemp.clear();
	}

	return 0L;
}

// [4/16/2007 Theodoric] �±� ���� â�� :: ���� �Լ���
void CControlWareHouse::SetCashInfo(int packMaxNumber, int seconds )
{
	if( CommonConfig::Instance()->GetKorFlag() == true )
	{
		m_bStoreWidening = true;
	}

	m_nCashMaxPackNum = packMaxNumber;

	int	minut = seconds / 60 ;
	int vdays = ( minut / 60 ) / 24 ;
	int vhour = ( minut / 60 ) % 24;
	int vminu =   minut % 60 ;

	if( CommonConfig::Instance()->GetKorFlag() == true )
	{
		char strTemp[64];

		if(  seconds == 0 )
		{
			sprintf( strTemp, " -" );
		}
		else if( seconds < 0)
			sprintf( strTemp, (char*)G_STRING(IDS_STOREWIDENING_TIME3), vhour, vminu);
		else if( vdays <= 0)
		{
			sprintf( strTemp, (char*)G_STRING(IDS_STOREWIDENING_TIME2), vhour, vminu);
		}
		else
		{
			sprintf( strTemp, (char*)G_STRING(IDS_STOREWIDENING_TIME), vdays, vhour, vminu);
		}

		m_pCashDateText->SetString( strTemp);

		if( g_pNk2DFrame && (vdays==0 && vhour==0) && (vminu==30 || vminu==15 || vminu==15 || vminu==5 ) )
		{
			char temp[255];
			sprintf( temp, (char*)G_STRING(IDS_LHSTRING1918) , packMaxNumber , strTemp );
			g_pNk2DFrame->AddChatStr( temp , -1);
		}
	}
	else
	{
		char strTemp[64];

		if(  seconds == 0 )
		{
			sprintf( strTemp, " -" );
		}
		else if( vdays <= 0)
		{
			sprintf( strTemp, "%02d:%02d", vhour, vminu);
		}
		else
		{
			sprintf( strTemp, "%dD, %02d:%02d", vdays, vhour, vminu);
		}

		m_pCashDateText->SetString( strTemp);
	}

	int stringgHarf = m_pCashDateText->GetBoxWidth();
	int tempX = 0;
	int tempY = 0;

	if( CommonConfig::Instance()->GetKorFlag() == true )
	{
		tempX =	m_pStoreWidening_time_back->Xpos + m_pStoreWidening_time_back->GetWidth()/2;
		tempY = m_pStoreWidening_time_back->Ypos + 5;
	}
	else
	{
		tempX =	m_pBack->Xpos + 65  + ( 84 - stringgHarf ) / 2 ;
		tempY = m_pBack->Ypos + 425 + 5 -18;
	}

	m_pCashDateText->SetPos( tempX, tempY);
}


// �󽽷Թ�ȣ(0,1,2 ���û���)
int CControlWareHouse::GetEmptySlot(int slot_num)
{
	if( false == IsActivate() )
		return -1;

	for ( int x=0; x < WARE_SLOT_X_NUM; x++)
	{
		for (int y=0; y < WARE_SLOT_Y_NUM; y++)
		{
			if( NULL == m_WareInven[slot_num][x][y] )
			{
				return (slot_num*20 + WARE_SLOT_Y_NUM*x + y);
			}
		}
	}

	return -1;
}

// �󽽷Թ�ȣ( 0~2����)
int CControlWareHouse::GetEmptySlot()
{
	if( false == IsActivate() )
		return -1;

	int rv_slot = -1;

	for( int pack=0 ; pack<WARE_PACK_NUM ; ++pack )
	{
		rv_slot = GetEmptySlot(pack);
		if( rv_slot != -1 )
			return rv_slot;
	}

	return -1;
}


CItem * CControlWareHouse::GetWareSlotItem(int slot_index)
{
	if( slot_index < 0 )
		return NULL;

	int pack = slot_index / 20;
	int x = (slot_index % 20) / WARE_SLOT_Y_NUM;
	int y = (slot_index % 20) % WARE_SLOT_Y_NUM;

	return m_WareInven[pack][x][y];
}

int CControlWareHouse::GetWareSlotCount(int slot_index)
{
	if( slot_index < 0 )
		return NULL;

	int pack = slot_index / 20;
	int x = (slot_index % 20) / WARE_SLOT_Y_NUM;
	int y = (slot_index % 20) % WARE_SLOT_Y_NUM;

	return m_WareSlot[pack][x][y];
}

// ������ �ִ� ������ �ִ°�?
int CControlWareHouse::FindSlot(int vnum, bool add_item)
{
	if( vnum < 0 )
		return -1;

	int slot_index = -1;
	if( add_item )
	{
		for( int pack=0 ; pack<WARE_PACK_NUM ; ++pack )
		{
			slot_index = FindSlot( vnum , pack , true );
			if( slot_index != -1 )
				return slot_index;
		}

		return GetEmptySlot();
	}
	else
	{
		for( int pack=0 ; pack<WARE_PACK_NUM ; ++pack )
		{
			slot_index = FindSlot( vnum , pack , false );
			if( slot_index != -1 )
				return slot_index;
		}
	}

	return -1;
}

int CControlWareHouse::FindSlot(int vnum , int pack , bool add_item)
{
	if( vnum < 0 )
		return false;

	if( add_item )
	{
		for ( int x=0; x < WARE_SLOT_X_NUM; x++)
		{
			for (int y=0; y < WARE_SLOT_Y_NUM; y++)
			{
				CItem * pWareItem = m_WareInven[pack][x][y];
				if( pWareItem && pWareItem->m_Vnum == vnum  && m_WareSlot[pack][x][y] < PER_COUNT )
				{
					return (pack*20 + WARE_SLOT_Y_NUM*x + y);
				}
			}
		}
	}
	else
	{
		for ( int x=0; x < WARE_SLOT_X_NUM; x++)
		{
			for (int y=0; y < WARE_SLOT_Y_NUM; y++)
			{
				CItem * pWareItem = m_WareInven[pack][x][y];
				if( pWareItem && pWareItem->m_Vnum == vnum  && m_WareSlot[pack][x][y] != 0 )
				{
					return (pack*20 + WARE_SLOT_Y_NUM*x + y);
				}
			}
		}
	}

	return -1;
}

int CControlWareHouse::SlotToPack(int slot_index)
{
	if( slot_index < 0 )
		return -1;

	return (slot_index / 20);
}

int CControlWareHouse::SlotToX(int slot_index)
{
	if( slot_index < 0 )
		return -1;

	int cur_index = slot_index % 20;

	return (cur_index / WARE_SLOT_Y_NUM);
}

int CControlWareHouse::SlotToY(int slot_index)
{
	if( slot_index < 0 )
		return -1;

	int cur_index = slot_index % 20;

	return (cur_index % WARE_SLOT_Y_NUM);
}

void CControlWareHouse::StashPuts(int vnum , int ware_slot, int inven_pack , int item_index , int plus, int special, int special2, int UpEndur, int MaxUpdEndur, int Endur, int MaxEndur, int count, int swap_flag)
{
	if( !g_pRoh )
		return;

	CItem *inven_item = m_pNkCha->PickItemInPack( inven_pack, item_index );
	if( !inven_item )
		return;

	CItem * stash_item = GetWareSlotItem( ware_slot );

	int ware_pack = SlotToPack(ware_slot);
	int ware_x = SlotToX(ware_slot);
	int ware_y = SlotToY(ware_slot);

	if( !stash_item )
	{
		stash_item = new CItem();

		stash_item->m_Vnum	= vnum;
		stash_item->m_SlotX = ware_x;
		stash_item->m_SlotY = ware_y;

		stash_item->m_ProtoNum = g_ItemList.FindItem(vnum);

		if( GET_TYPE(stash_item) != ITYPE_SUB )
		{
			if(g_ItemList.m_ItemArray[stash_item->m_ProtoNum].m_MinLevel>=181)
				stash_item->SetDiaGemNum(plus);
			else
				stash_item->SetGemNum(plus);
		}
		else
		{
			stash_item->m_PlusNum = plus;
		}

		stash_item->m_Special				= special;
		stash_item->m_Special2				= special2;
		stash_item->m_UpgradeEndurance		= UpEndur;
		stash_item->m_MaxUpgradeEndurance	= MaxUpdEndur;
		stash_item->m_Endurance				= Endur;
		stash_item->m_MaxEndurance			= MaxEndur;

		ResetItemValue(stash_item);
		SetSpecialValue(stash_item, GET_TYPE(stash_item), GET_SHAPE(stash_item), special, special2);

		m_WareInven[ware_pack][ware_x][ware_y] = stash_item;
		m_WareSlot[ware_pack][ware_x][ware_y] = 0;
	}

	m_WareSlot[ware_pack][ware_x][ware_y]++;

	if( m_WareSlot[ware_pack][ware_x][ware_y] > 0)
	{
		char tmp[10] = "";
		sprintf(tmp, "%d",m_WareSlot[ware_pack][ware_x][ware_y]);
		m_pNumText[ware_pack][ware_x][ware_y].SetString(tmp);
	}
	else
	{
		char tmp[10] = "";
		sprintf(tmp, "0");
		m_pNumText[ware_pack][ware_x][ware_y].SetString(tmp);
	}

	SAFE_DELETE(inven_item);

	m_bMsgWaiting = FALSE;
}

void CControlWareHouse::StashPuts(int vnum , int ware_slot, int inven_pack , int item_index)
{
	if( !g_pRoh )
		return;

	CItem *inven_item = NULL;
	inven_item = g_pRoh->GetInvenItem( inven_pack, item_index );

	if( !inven_item )
		return;

	int sp1 = 0;
	int plus_num = 0;

	if ( inven_item )
	{
		sp1 = inven_item->m_Special;
		if( GET_TYPE( inven_item ) != ITYPE_SUB )
		{
			if( GET_ITEM_LEVEL(inven_item) >= 181 )
			{
				plus_num = inven_item->m_PlusNum;
			}
			else
			{
				for( int i = 1; i <= inven_item->m_PlusNum; i++ )
				{
					plus_num += i;
				}
			}
		}
		else
		{
			plus_num = inven_item->m_PlusNum;
		}

		if( g_SvrType == ST_ADULT_ONLY )
		{
			StashPuts(vnum , ware_slot, inven_pack , item_index ,
					  plus_num , sp1 , inven_item->m_Special2,
					  inven_item->m_UpgradeEndurance, inven_item->m_MaxUpgradeEndurance,
					  inven_item->m_Endurance, inven_item->m_MaxEndurance, -1 , 1 );
		}
		else
		{
			StashPuts(vnum , ware_slot, inven_pack , item_index ,
					  plus_num , sp1 , inven_item->m_Special2 ,
					  inven_item->m_UpgradeEndurance, inven_item->m_MaxUpgradeEndurance,
					  0 , 0 , -1 , 1 );
		}
	}
}

void CControlWareHouse::StashGets(int vnum , int ware_slot , int inven_pack, int item_index , int inven_x, int inven_y)
{
	if( !g_pRoh )
		return;

	CItem * stash_item = GetWareSlotItem( ware_slot );
	if( !stash_item )
		return;

	int plus_num	= 0;
	int sp1			= stash_item->m_Special;
	int sp2			= stash_item->m_Special2;
	int upendur		= stash_item->m_UpgradeEndurance;
	int maxupendure = stash_item->m_MaxUpgradeEndurance;
	int endur		= stash_item->m_Endurance;
	int maxendur	= stash_item->m_MaxEndurance;

	if( GET_TYPE( stash_item ) != ITYPE_SUB )
	{
		if( GET_ITEM_LEVEL(stash_item) >= 181 )
		{
			plus_num = stash_item->m_PlusNum;
		}
		else
		{
			for( int i = 1; i <= stash_item->m_PlusNum; i++ )
			{
				plus_num += i;
			}
		}
	}
	else
	{
		plus_num = stash_item->m_PlusNum;
	}

	int ware_count = GetWareSlotCount(ware_slot);

	StashGets( vnum , ware_slot , inven_pack , item_index , inven_x , inven_y ,
			   plus_num , sp1 , sp2 , upendur , maxupendure , endur , maxendur , ware_count-1 );
}

void CControlWareHouse::StashGets(int vnum , int ware_slot , int inven_pack, int item_index , int inven_x, int inven_y, int plus, int special, int special2, int UpEndur, int MaxUpdEndur, int Endur, int MaxEndur, int count)
{
	if( !g_pRoh )
		return;

	CItem * stash_item = GetWareSlotItem( ware_slot );
	if( !stash_item )
		return;

	int ware_pack = SlotToPack(ware_slot);
	int ware_x = SlotToX(ware_slot);
	int ware_y = SlotToY(ware_slot);

	if( count )
	{
		m_WareSlot[ware_pack][ware_x][ware_y] = count;
	}
	else
	{
		SAFE_DELETE( stash_item );
		m_WareInven[ware_pack][ware_x][ware_y] = NULL;
		m_WareSlot[ware_pack][ware_x][ware_y] = 0;
	}

	g_pRoh->AddToInven(inven_pack , inven_x , inven_y , item_index , vnum , plus , special , special2 , UpEndur , MaxUpdEndur , Endur , MaxEndur );

	if( m_bActivate && stash_item )
	{
		char tmp[10] = "";
		sprintf(tmp, "%d",m_WareSlot[ware_pack][ware_x][ware_y]);
		m_pNumText[ware_pack][ware_x][ware_y].SetString(tmp);
	}
}

int CControlWareHouse::Push_Available(CItem* pItem)
{
	if( !pItem )
		return -1;

	int slot_index = -1;

	for( int pack=0 ; pack<WARE_PACK_NUM ; pack++ )
	{
		for ( int x=0; x < WARE_SLOT_X_NUM; x++)
		{
			for (int y=0; y < WARE_SLOT_Y_NUM; y++)
			{
				CItem * pWareItem = m_WareInven[pack][x][y];

				if( pWareItem && pWareItem->m_Vnum == pItem->m_Vnum && m_WareSlot[pack][x][y] < PER_COUNT )
				{	
					if( (pWareItem->m_PlusNum == pItem->m_PlusNum) &&
							(pWareItem->m_Special == pItem->m_Special) &&
							(pWareItem->m_Num0 == pItem->m_Num0) &&
							(pWareItem->m_Defense == pItem->m_Defense) &&
							(pWareItem->m_Dam1 == pItem->m_Dam1) &&
							(pWareItem->m_Special2 == pItem->m_Special2) &&
							(pWareItem->m_UpgradeEndurance == pItem->m_UpgradeEndurance) &&
							(pWareItem->m_MaxUpgradeEndurance == pItem->m_MaxUpgradeEndurance) &&
							(pWareItem->m_Endurance == pItem->m_Endurance) &&
							(pWareItem->m_MaxEndurance == pItem->m_MaxEndurance) )
					{
						return (pack*20 + WARE_SLOT_Y_NUM*x + y);
					}
				}
			}
		}
	}

	for( int pack=0 ; pack<WARE_PACK_NUM ; pack++ )
	{
		for ( int x=0; x < WARE_SLOT_X_NUM; x++)
		{
			for (int y=0; y < WARE_SLOT_Y_NUM; y++)
			{
				if( m_WareInven[pack][x][y] == NULL )
				{
					return (pack*20 + WARE_SLOT_Y_NUM*x + y);
				}
			}
		}
	}

	return -1;
}

void CControlWareHouse::SetNpcIndex(const int& index)
{
	m_indexNpc = index;
}

const int& CControlWareHouse::GetNpcIndex()
{
	return m_indexNpc;
}

BOOL CControlWareHouse::IsFromSummonNpc()
{
	BOOL bRet = FALSE;

	if( g_pRoh == NULL )
		return bRet;

	int summonNpcIndex = g_pRoh->GetSummonNpcIndex();
	if( summonNpcIndex == this->GetNpcIndex() )
	{
		bRet = TRUE;
	}

	return bRet;
}
