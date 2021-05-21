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

// 개수를 나타내는 TEXTOUTBOX를 위한 정의
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
	m_BtnWindow_3.SetFileName("common/btn_tab_03");	// 버튼맴버의 이름만 셋팅

	m_MoneyText.Init(12, RGB(255, 255, 0), 0, 0);

	for (k=0; k < WARE_PACK_NUM; k++)
	{
		for (i=0; i < WARE_SLOT_X_NUM; i++)
		{
			for (j=0; j < WARE_SLOT_Y_NUM; j++)
			{
				m_WareInven[k][i][j] = NULL;
				m_WareSlot[k][i][j] = 0;	// 슬롯 카운트
			}
		}
	}
	m_byInOutMoneyState = 0;
	m_bMsgWaiting = FALSE;
	m_nPackNum = 0;

	// [4/16/2007 Theodoric] 태국 유료 창고
	m_pCashViewBtn	= NULL;
	m_pNormalViewBtn = NULL;
	m_pCashDateText  = NULL;
	m_bCash = FALSE;
	m_nCashMaxPackNum = 0;
}

// [4/16/2007 Theodoric] 태국 유료 창고
void CControlWareHouse::InvenSlotInfoInit()
{
	int k, i, j;
	// nate : 슬롯에 보이는 그림 초기화.
	for (k=0; k < WARE_PACK_NUM; k++)
	{
		for (i=0; i < WARE_SLOT_X_NUM; i++)
		{
			for (j=0; j < WARE_SLOT_Y_NUM; j++)
			{
				m_WareInven[k][i][j] = NULL;
				m_WareSlot[k][i][j] = 0;	// 슬롯 카운트
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
	m_BtnWindow_3.LoadRes();	// nate : 창고 슬롯 확장 - 버튼 추가
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

	// [4/16/2007 Theodoric] 태국 유료 창고
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
	m_BtnWindow_3.DeleteRes();	// nate : 창고 슬롯 확장 - 버튼 추가
	m_MoneyText.DeleteRes();

	for (int k=0; k < WARE_PACK_NUM; k++)
	{
		for (int i=0; i < WARE_SLOT_X_NUM; i++)
		{
			for (int j=0; j < WARE_SLOT_Y_NUM; j++)
				m_pNumText[k][i][j].DeleteRes();
		}
	}

	// [4/16/2007 Theodoric] 태국 유로 창고
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


// nate : 창고 Inven 을 그린다.
//-- 2004.07.12
//-- Lyul / 수정완료
//-- 창고 Inven 은 같은 종류의 Item 이 다량으로 포함 될 수 있으므로
//-- 한 cell 의 Index 를 구조적으로 가질 수가 없다.
//-- 그런데 ControlInven 에서 AlphaPopup 에서는 이전 아이템의 Index 를 가지고
//-- 정보를 Update 하는지 판별하므로 버그가 된다.
//-- controlinven 에서는 일반 inven 도 처리하므로
//-- 이전의 창고안의 좌표를 기억하여 창고 안의 Item 만 제어하자.
CItem *CControlWareHouse::Draw()
{
	int i = 0, j = 0;

	//-- 이전에 정보가 출력된 Inven
	static int past_i= -1;
	static int past_j= -1;
	static int past_pack= -1;
	//-- 이전에 사용된 Index 번호 값에는 큰 의미가 없고 틀리다는 것만 중요
	static int past_idx = 1;

	CItem *pItem=NULL;

	// 창고 작업이 끝났는가 검사
	if (!m_bGetStashEnd)
		return NULL;

	// BOTTOM 그리기
	if( g_pDisplay && m_pBack )
		g_pDisplay->Blt(m_pBack->Xpos , m_pBack->Ypos, m_pBack);

	if( m_bStoreWidening )
	{
		g_pDisplay->Blt(m_pStoreWidening_time_back->Xpos , m_pStoreWidening_time_back->Ypos, m_pStoreWidening_time_back);
		m_pCashDateText->Draw();
	}

	if( !m_bCash ) // 유료확장 창고는 라임을 넣지 못한다. 입/출금 버튼을 안보이게 막아준다. ///071015
	{
		m_InMoneyBtn.Draw();
		m_OutMoneyBtn.Draw();
	}
	m_CloseBtn.Draw();

	// nate : 창고 슬롯 확장
	// 버튼 추가
	// 창고 팩버튼
	// 버튼의 클릭 순서에 따라 그리기 순서 결정
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

	// 마우스 포인터의 현재위치 계산
	POINT point;
	GetCursorPos(&point);
	ScreenToClient(g_pDisplay->GetHWnd(), &point);

	if (pCMyApp->m_pMouse && IsInside(point.x, point.y))
		pCMyApp->m_pMouse->SetMouseType(M_NORMAL);

	int x = (int)point.x;
	int y = (int)point.y;

	m_iCurSlot_X = -1;
	m_iCurSlot_Y = -1;
	// 마우스가 창고의 슬롯 영역에 눌러졌는지 체크
	if ( x >= m_wareslot_start_x
			&& x < m_wareslot_start_x + WARE_SLOT_WIDTH * WARE_SLOT_X_NUM
			&& y >= m_wareslot_start_y
			&& y < m_wareslot_start_y + WARE_SLOT_HEIGHT * WARE_SLOT_Y_NUM )
	{
		int slot_x_num, slot_y_num, slot_index;

		// 현재 슬롯 셋팅
		m_iCurSlot_X = slot_x_num = (x - m_wareslot_start_x) / WARE_SLOT_WIDTH;
		m_iCurSlot_Y = slot_y_num = (y - m_wareslot_start_y) / WARE_SLOT_HEIGHT;
		// 슬롯의 인텍스 계산
		// 팩의 몇번째 슬롯인가 계산
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
							|| pItem->m_Vnum == 1308 // 이판사판 행운권 밝기 조절 05-10-05 원석
							|| pItem->m_Vnum == 1286
							|| ( pItem->m_Vnum >= 1598 && pItem->m_Vnum <= 1695 ) // 브라질 요청 남미국기망토 종류도 성조기에서 응용이므로 예외처리.
							|| pItem->m_Vnum == 1287 ) // 미국 성조기 망토, 성조기 날개도 어둡게 보여서 클라이언트에서 직접 밝게 처리.(by 원석)
						ambient = 0x00cccccc;
					else if (pItem->m_PlusNum > 0)
						ambient = UPGRADE_ITEM_COLOR;

					if(i == m_iCurSlot_X && j == m_iCurSlot_Y)
					{
						m_pOverItem = pItem;

						//-- 2004.07.12 / Lyul
						//-- 이전에 위치했던 Overitem 이랑 틀리다면
						//-- 새로운 위치를 저장하고 표시하라고 알린다.
						//-- index 는 살짝 바꿔준다.
						if(past_i != i || past_j != j || past_pack != m_nPackNum)
						{
							if( m_pOverItem )
								m_pOverItem->m_Index = past_idx; //-- controlinven::drawiteminfo 에서 필요

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

	// 아이템을 들거나 메세지창이 있을경우 아이템인포를창을 출력하지 않는다
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

	// 인벤창이 가려지므로 nk2d에서 드로우를 한다..
	m_MoneyText.Draw(m_pBack->Xpos+29, m_pBack->Ypos+386, 108);

	if (m_pOverItem)
		return m_pOverItem;
	else
		return NULL;
}

BOOL CControlWareHouse::CheckMoneyStr(char* szMoney)
{
	int nMoney = 0;

	// 입력된 스트링을 검사한다
	int len = strlen(szMoney);

	for(int i=0; i<len; i++)
	{
		if(isdigit(szMoney[i]) == FALSE)
		{
			if(is_space(szMoney[i]) ==TRUE)
				//-- IDS_WAREHOUSE_BLANK : 공백문자는 허용하지 않습니다.
				g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_WAREHOUSE_BLANK), TYPE_EXCHANGE, 19 );
			else
				//-- IDS_WAREHOUSE_NOTNUM : 잘못된 금액입니다. 다시 입력하세요
				g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_WAREHOUSE_NOTNUM), TYPE_EXCHANGE, 19 );

			return FALSE;
		}
	}

	// 100 단위로 입력이 되지 않은경우

	if(len >2)
	{
		if(atoi(&szMoney[len-2]) != 0
				|| atoi(&szMoney[len-1]) != 0)
		{
			//-- IDS_WAREHOUSE_NOT100 : 금액은 100 단위로 입력해 주세요
			g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_WAREHOUSE_NOT100), TYPE_EXCHANGE, 19 );
			return FALSE;
		}
	}
	else
	{
		//-- IDS_WAREHOUSE_NOT100 : 금액은 100 단위로 입력해 주세요
		g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_WAREHOUSE_NOT100), TYPE_EXCHANGE, 19 );
		return FALSE;
	}

	// 3. 현제 가지구 있는 보유금액보다 입력된 금액이 더큰경우
	nMoney = atoi(szMoney);

	if (nMoney < 0)
	{
		//-- IDS_WAREHOUSE_NOT0 : 입력값이 잘못되었습니다. 다시 입력하세요
		g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_WAREHOUSE_NOT0), TYPE_EXCHANGE, 19 );
		return FALSE;
	}

	if(m_byInOutMoneyState != 2)
	{
		if(m_pNkCha->m_Money < nMoney)
		{
			//-- IDS_WAREHOUSE_NOTENOUGH :  금액이 부족합니다. 다시 입력하세요
			g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_WAREHOUSE_NOTENOUGH), TYPE_EXCHANGE, 19 );
			return FALSE;
		}
	}
	else
	{
		if((m_nSaveMoney*100) < nMoney)
		{
			//-- IDS_WAREHOUSE_NOTENOUGH :  금액이 부족합니다. 다시 입력하세요
			g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_WAREHOUSE_NOTENOUGH), TYPE_EXCHANGE, 19 );
			return FALSE;
		}
	}

	return TRUE;
}


LRESULT CControlWareHouse::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	int x = 0, y = 0;
	static DWORD BtnClickTick = 0; // 캐쉬창고 전환버튼 빠르게 누르면 튕기는 거 때문에 딜레이 틱을 적용. ///071015

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
				// 창고를 닫는다.
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
			// 마석 업그레이드 창이 떠있는경우 때문에 넣었따..
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

			// [4/16/2007 Theodoric] 태국 유료 창고::Thai cash stash

			if( g_dwClientCountry == CTRY_TH || g_dwClientCountry == CTRY_JPN ) // #define CTRY_TH 7
			{
				if( timeGetTime() - BtnClickTick > 1000 ) // 캐쉬창고 전환버튼 빠르게 누르면 튕기는 거 때문에 딜레이 틱을 적용. ///071015
				{
					if( m_bCash ) // 유료 창고를 보고 있는 상태라면 일반 상점 보기 버튼 처리
					{
						m_pNormalViewBtn->MsgProc(hWnd, msg, wParam, lParam);

						if( m_pNormalViewBtn->GetState() == BTN_DOWN )
						{
							m_bCash = !m_bCash;
							InvenSlotInfoInit();
							// by evilkiki 2009.12.16 일본창고
							// g_pTcpIp->SendNetMessage("stash_open chash 0\n"); // 일반 창고 오픈
							g_pTcpIp->SendNetMessage("stash_open 0\n");

							BtnClickTick = timeGetTime(); // 캐쉬창고 전환버튼 빠르게 누르면 튕기는 거 때문에 딜레이 틱을 적용. ///071015

						}
						m_pNormalViewBtn->SetState(BTN_NORMAL);
					}
					else // 일반 창고를 보고 있는 상태라면 유료상점 보기 버튼 처리
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
								// by evilkiki 2009.12.16 일본창고
								//g_pTcpIp->SendNetMessage("stash_open chash 1\n"); // 유료 창고 오픈
								g_pTcpIp->SendNetMessage("stash_open 1\n");

								BtnClickTick = timeGetTime(); // 캐쉬창고 전환버튼 빠르게 누르면 튕기는 거 때문에 딜레이 틱을 적용. ///071015
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

							BtnClickTick = timeGetTime(); // 캐쉬창고 전환버튼 빠르게 누르면 튕기는 거 때문에 딜레이 틱을 적용. ///071015

						}
						m_pNormalViewBtn->SetState(BTN_NORMAL);
					}
					else // 일반 창고를 보고 있는 상태라면 유료상점 보기 버튼 처리
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

			// mate : 창고 슬롯 확장
			// 버튼 추가( 영역 재 설정 요망 )
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

				max_cnt = PER_COUNT; // 해외쪽도 전부 100개까지 넣을수있게 바뀌었다. (이전엔 20개) (by 원석)

				// 아이템을 넣는 것.
				if (m_pNkCha && m_pNkCha->m_ExtraSlot)
				{
					if( m_pNkCha->m_ExtraSlot->m_bTimeLimit ) // 시간제 아이템이면 창고 불가처리 (by 원석) ///시간제아이템
					{
						//-- IDS_LIMITED_CANT : 제한된 아이템은 불가능합니다.
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

						// 창고슬롯의 아이템이 1개이상이고 extraslot의 vnum과 다를 경우 아무 동작도하지마라~
						if((cur_cnt >= 1))
							if(m_WareInven[m_nPackNum][slot_x_num][slot_y_num]->m_Vnum != vnum || (cur_cnt >= max_cnt))
								return 1;

						// [2008/6/4 Theodoric] 복사 버그때문에 1개만 넣을 수 있는 아이템이라면 스왑도 해주지 말자.
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

						// [4/18/2007 Theodoric] 태국 유료 창고 :: 캐쉬 창고라면 +3을 해서 서버에 보내줘야한다.
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
				// 창고를 닫는다.
				ActivateWareHouse(FALSE);

			g_pNk2DFrame->ShowWareHouseWindow(FALSE);
		}
		else if (m_bMsgWaiting)
			return 0;
		else
			return 0;

		return 1;
//=================================================================================
	// nate : 아이템 다량이동관련
	case WM_RBUTTONDBLCLK:
		{
			// ExtraSlot에 아이템이 있을경우에는 안됨
			if( m_pNkCha->m_ExtraSlot )
				return 0;

			x = LOWORD (lParam);
			y = HIWORD (lParam);

			//영역확인
			if(IsInside(x, y))
			{
				// 무엇인지 모름
				// 그냥 LButtonclk 따라했음
				if (m_bGetStashEnd == FALSE && m_bActivate)
					return 1;

				// 커서가 영역안에 있는지 검사
				if ( x >= m_wareslot_start_x
						&& x < m_wareslot_start_x + WARE_SLOT_WIDTH*WARE_SLOT_X_NUM
						&& y >= m_wareslot_start_y
						&& y < m_wareslot_start_y + WARE_SLOT_HEIGHT*WARE_SLOT_Y_NUM )
				{

					int slot_x_num, slot_y_num, slot_index;
					int nVnum;

					// 슬롯넘버와 팩에서의 슬롯 인덱스 계산
					slot_x_num = (x - m_wareslot_start_x) / WARE_SLOT_WIDTH;
					slot_y_num = (y - m_wareslot_start_y) / WARE_SLOT_HEIGHT;
					slot_index = slot_x_num * WARE_SLOT_Y_NUM + slot_y_num;

					// 슬롯에 아이템이 있으면
					if ( m_WareSlot[ m_nPackNum ][ slot_x_num ][ slot_y_num ] > 0 )
					{

						int chaotic = g_pRoh->m_Chaotic_Num * 0.1;
						if( chaotic < 0 && chaotic > -500 )
						{
							// 3배의 과금
						}
						else if( chaotic <= -500 )
						{
							if( g_dev_client )
								g_pNk2DFrame->AddChatStr( "Can not find" , -1 );
							return 0;
						}

						// 현재 슬롯의 아이템번호 추출
						if( m_WareInven[ m_nPackNum ][ slot_x_num ][ slot_y_num ] )
							nVnum = m_WareInven[ m_nPackNum ][ slot_x_num ][ slot_y_num ]->m_Vnum;

						// 아이템의 크기 계산
						int nWidth = GET_ITEM_WIDTH( m_WareInven[ m_nPackNum ][ slot_x_num ][ slot_y_num ] );
						int nHeight = GET_ITEM_HEIGHT( m_WareInven[ m_nPackNum ][ slot_x_num ][ slot_y_num ] );

						// 1칸짜리 아이템만 다량 이동 가능
						if( nWidth != 1 || nHeight != 1 )
							return 0;

						// 1.5초 딜레이
						if ( m_bWareLock)
						{
							DWORD curTime = timeGetTime();
							if (curTime > m_timeWareLock + 1500) // 1.5초가 지났으면,
								m_bWareLock = FALSE;
							else
								return 1;
						}

						// 락 해제
						SetWareLock( TRUE );

						// [4/18/2007 Theodoric] 태국 유료 창고 :: 캐쉬 창고라면 +3을 해서 서버에 보내줘야한다.
						int nPackNum = m_nPackNum;
						if( m_bCash )
						{
							nPackNum += 3;
						}

						// WARE_PACK_INTERVAL  = 20  : 팩간의 인덱스 간격st
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

			if( !m_bCash ) // 유료확장 창고는 라임을 넣지 못한다. 입/출금 버튼을 안보이게 막아준다. ///071015
			{
				m_InMoneyBtn.MsgProc(hWnd, msg, wParam, lParam);
				m_OutMoneyBtn.MsgProc(hWnd, msg, wParam, lParam);
			}
			m_CloseBtn.MsgProc(hWnd, msg, wParam, lParam);
			if(m_InMoneyBtn.GetState() == BTN_ACTION && m_InMoneyBtn.IsInside(x,y))
			{

				//-- IDS_WAREHOUSE_INMONEY :  입금하실 금액을   적어주세요
				g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_WAREHOUSE_INMONEY), TYPE_EXCHANGE, 19 );
				m_byInOutMoneyState = 1;
				m_InMoneyBtn.m_iBtnFlg = BTN_NORMAL;
			}
			else if(m_OutMoneyBtn.GetState() == BTN_ACTION && m_OutMoneyBtn.IsInside(x,y))
			{

				int chaotic = g_pRoh->m_Chaotic_Num * 0.1;
				if( chaotic < 0 && chaotic > -500 )
				{
					// 3배의 과금
				}
				else if( chaotic <= -500 )
				{
					if( g_dev_client )
						g_pNk2DFrame->AddChatStr("No withdrawal",-1);
					return 0;
				}

				//-- IDS_WAREHOUSE_OUTMONEY : 출금하실 금액을   적어주세요
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

// 창고의 문이 닫힌다.
// 창고 데이타를 삭제한다.
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

	// 슬롯데이타 초기화.
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
	// [4/18/2007 Theodoric] 태국 유료 창고
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

// [6/1/2007 Theodoric] 내구도 수리 :: 함수 파라미터 내구도 포함
void CControlWareHouse::Stash(int slot_num, int vnum, int plus, int special, int special2,
							  int UpEndur, int MaxUpdEndur, int Endur, int MaxEndur, int count, int time_limit ) ///시간제아이템 시간값 추가(050711 by 원석) ///신방어구강화시스템
{
	// 위치 파악.
	int x = 0, y = 0;
	CItem *pItem=NULL;
	int pack_num = 0;		// 임시.

	pack_num = slot_num / WARE_PACK_INTERVAL;
	slot_num = slot_num % WARE_PACK_INTERVAL;

	// [4/18/2007 Theodoric] 태국 유료 창고
	if( m_bCash && slot_num != 12 )
	{
		pack_num  = pack_num - 3;
	}

	if (slot_num == 12)  				// 돈이 들어온 것이다.
	{
		m_nSaveMoney = plus;
		m_MoneyText.SetString((m_nSaveMoney*100), true);
		return;
	}

	x = slot_num / WARE_SLOT_Y_NUM;
	y = slot_num % WARE_SLOT_Y_NUM;

	// 이미 슬롯에 물품이 있는 경우와 없는 경우를 구별한다.
	// 지금 단계에서 꼭 해야 되는지에 대해선 의문.
	if (m_WareSlot[pack_num][x][y] == 0)
	{
		m_WareSlot[pack_num][x][y] = count;

		pItem = new CItem;
		if( !pItem )
			return;

		pItem->m_SlotX = x;
		pItem->m_SlotY = y;
		pItem->m_Index = slot_num;		// 임시 인덱스
		pItem->m_Vnum = vnum;
		pItem->m_Special = special;

		///신방어구강화시스템
		pItem->m_Special2 = special2; // 추가 옵션
		pItem->m_UpgradeEndurance = UpEndur;           // 현재 강화 내구도
		pItem->m_MaxUpgradeEndurance = MaxUpdEndur;    // 강화 내구도 최대치

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

		// 아이템 값을 입력해보자!
		ResetItemValue(pItem);
		SetSpecialValue(pItem, GET_TYPE(pItem), GET_SHAPE(pItem), special, special2);

		if( time_limit ) // 시간 값이 있으면 시간제 아이템으로 셋팅 (by 원석) ///시간제아이템
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

// nate : extra에서 창고해당슬롯으로 아이템이 옮겨졌음.
// [6/1/2007 Theodoric] 내구도 수리 :: 함수 파라미터 내구도 포함
void CControlWareHouse::StashTo(int slot_num, int vnum, int plus, int special, int special2,
								int UpEndur, int MaxUpdEndur, int Endur, int MaxEndur, int count, int swap_flag) ///신방어구강화시스템
// 인자 : 창고슬롯넘, 아이템넘, 플러스넘, 스페셜넘, 창고의 카운트, 교환플래그
// 인벤 -> 창고, ExtraSlot -> 창고 (교환플래그 : 아치템 갯수가 1일경우 교환 )
// 창고에 아이템을 넣고 카운트를 출력
{
	int x = 0, y = 0, pack_num = 0;
	CItem *pItem=NULL;

	// 슬롯넘으로 인덱스 계산
	// 슬롯넘은 선형적 ( 팩은 여러개지만 값은 선형적(?)으로 관리 )
	pack_num = slot_num / WARE_PACK_INTERVAL;		// WARE_PACK_INTERVAL = 20
	slot_num = slot_num % WARE_PACK_INTERVAL;		// 팩 사이의 간격이 20으로 설정되어 있음

	// [4/18/2007 Theodoric] 태국 유료 창고
	if( m_bCash && slot_num != 12 )
	{
		pack_num  = pack_num - 3;
	}

	// 현재 슬롯의 좌표 계산
	x = slot_num / WARE_SLOT_Y_NUM;
	y = slot_num % WARE_SLOT_Y_NUM;

	// 팩 하나당 11개의 슬롯
	// 12는 머니슬롯
	if (slot_num == 12)  				// 돈이 들어온 것이다.
	{
		m_nSaveMoney = plus;
		m_MoneyText.SetString((m_nSaveMoney*100), true);
		return;
	}

	// 현재 창고가 빈 슬롯인가?
	if (m_WareSlot[pack_num][x][y] == 0 )
	{
		// 슬롯에 보여질 그림 생성.
		if (swap_flag)
		{
			// 창고슬롯에 현재 인자값 아이템 생성
			m_WareInven[pack_num][x][y] = new CItem;
			if( !m_WareInven[pack_num][x][y] )
				return;

			m_WareInven[pack_num][x][y]->m_Vnum = vnum;
			m_WareInven[pack_num][x][y]->m_ProtoNum = g_ItemList.FindItem(vnum);

			if(GET_TYPE(m_WareInven[pack_num][x][y])!=ITYPE_SUB)
			{
				if(g_ItemList.m_ItemArray[m_WareInven[pack_num][x][y]->m_ProtoNum].m_MinLevel>=181)
					// 다이아급 보석 : 다이아급은 박혀있는 gem속성은 없구 다이아 갯수가 바로 플러스넘이다.
					m_WareInven[pack_num][x][y]->SetDiaGemNum(plus);
				else
					// 그외 젬넘에따른 플러스값 설정
					m_WareInven[pack_num][x][y]->SetGemNum(plus);
			}
			else
				// 서브 아이템은 보석 갯수가 플러스 값
				m_WareInven[pack_num][x][y]->m_PlusNum = plus;

			m_WareInven[pack_num][x][y]->m_Special = special;

			///신방어구강화시스템
			m_WareInven[pack_num][x][y]->m_Special2 = special2; // 추가 옵션
			m_WareInven[pack_num][x][y]->m_UpgradeEndurance = UpEndur;           // 현재 강화 내구도
			m_WareInven[pack_num][x][y]->m_MaxUpgradeEndurance = MaxUpdEndur;    // 강화 내구도 최대치

			m_WareInven[pack_num][x][y]->m_Endurance = Endur;
			m_WareInven[pack_num][x][y]->m_MaxEndurance = MaxEndur;

			// 아이템 속성값 셋팅
			// 아이템 값을 입력해보자!
			ResetItemValue(m_WareInven[pack_num][x][y]);
			SetSpecialValue(m_WareInven[pack_num][x][y], GET_TYPE(m_WareInven[pack_num][x][y]), GET_SHAPE(m_WareInven[pack_num][x][y]), special, special2);
		}
		else
		{
			// ExtraSlot에 있는 아이템을 창고에 넣는다.
			// ExtraSlot의 아이템을 넣는다.
			m_WareInven[pack_num][x][y] = m_pNkCha->m_ExtraSlot;	// m_WareInven : 창고의 아이템 관리
			m_pNkCha->m_ExtraSlot = NULL;
		}

		if (count < 0)
		{
			// 카운트가 0보다 작으면 빈 슬롯
			// m_WareSlot과 m_WareInven, m_pNumText를 하나의 구조체로 만들어서 쓰는게 더 편리하지 않을까
			m_WareSlot[pack_num][x][y] ++;		// 창고 슬롯의 카운트관리
			count = m_WareSlot[pack_num][x][y];	// count값으로 슬롯 카운틀의 통일성을 위해
		}
		else
			m_WareSlot[pack_num][x][y] = count;
	}
	else
	{
		// 창고 슬롯이 빈슬롯이 아니면
		if (count < 0)
		{
			m_WareSlot[pack_num][x][y] ++;
			count = m_WareSlot[pack_num][x][y];
		}
		else
			m_WareSlot[pack_num][x][y] = count;

		// extra 제거.
		if (!swap_flag)
			SAFE_DELETE( m_pNkCha->m_ExtraSlot );
	}

	char tmp[10] = "";
	// 슬롯에 아이템이 있으면 슬롯갯수 출력
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

// [6/1/2007 Theodoric] 내구도 수리 :: 함수 파라미터 내구도 포함
// nate : 창고 -> 인벤, 창고 -> Extra Slot
// 인자 : 창고슬롯넘, 아이템인덱스, 아이템번호, 플러스넘, 스페셜, 아이템갯수
void CControlWareHouse::StashFrom(int slot_num, int index, int vnum, int plus, int &special, int special2,
								  int UpEndur, int MaxUpdEndur, int Endur, int MaxEndur, int count) ///신방어구강화시스템 // 이녀석은 왜 special을 & 붙여서 인자 받는지 모르겠다...
{
	int x, y, pack_num;
	int special_temp = 0;
	CItem *pItem = NULL;

	// 창고 슬롯 계산
	pack_num = slot_num / WARE_PACK_INTERVAL;
	slot_num = slot_num % WARE_PACK_INTERVAL;

	// [4/18/2007 Theodoric] 태국 유료 창고
	if( m_bCash && slot_num != 12 )
	{
		pack_num  = pack_num - 3;
	}

	x = slot_num / WARE_SLOT_Y_NUM;
	y = slot_num % WARE_SLOT_Y_NUM;

	// 현금창
	if (slot_num == 12)  				// 돈이 들어온 것이다.
	{
		m_nSaveMoney = vnum;
		m_MoneyText.SetString((m_nSaveMoney*100), true);
		return;
	}

	if (m_WareInven[pack_num][x][y])
		special_temp = m_WareInven[pack_num][x][y]->m_Special;

	// -1 이상이면 다들어오는거 아닌가?
	if (count >= -1)
	{
		// 아이템 다량이동 명령시
		if (count < 0)
			m_WareSlot[pack_num][x][y]--;	// 카운트 감소
		else
			m_WareSlot[pack_num][x][y] = count;

		// 현재 슬롯에 아이템이 없으면
		//if (m_WareSlot[pack_num][x][y] == 0)
		if (m_WareSlot[pack_num][x][y] <= 0)
		{
			SAFE_DELETE(m_WareInven[pack_num][x][y]);
			m_WareInven[pack_num][x][y] = NULL;
			m_WareSlot[pack_num][x][y] = 0;
		}

		// 아이템이 있고 다량이동 명령이 아니면
		if (m_pNkCha && count >= 0)
		{
			// ExtraSlot에 아이템이 있는가 체크
			SAFE_DELETE( m_pNkCha->m_ExtraSlot );

			// ExtraSlot에 아이템 생성
			m_pNkCha->m_ExtraSlot = new CItem;
			if(m_pNkCha->m_ExtraSlot)
			{
				m_pNkCha->m_ExtraSlot->m_Index = index;
				m_pNkCha->m_ExtraSlot->m_Vnum = vnum;
				m_pNkCha->m_ExtraSlot->m_ProtoNum = g_ItemList.FindItem(vnum);;

				// ExtraSlot의 아이템이 서브( 세컨 ) 아이템인지 체크
				if(GET_TYPE(m_pNkCha->m_ExtraSlot)!=ITYPE_SUB) //1111
				{

					// 클라이언트는 아이템 전체목록을 가지고 있다
					// 목록에서 ProtoNum으로 아이템의 종류를 찾는다.
					// 다이아급인가?
					if(g_ItemList.m_ItemArray[m_pNkCha->m_ExtraSlot->m_ProtoNum].m_MinLevel>=181)
						m_pNkCha->m_ExtraSlot->SetDiaGemNum(plus);
					else	// 다이아 이하급
						m_pNkCha->m_ExtraSlot->SetGemNum(plus);
				}
				else	// 서브아이템의 경우
					m_pNkCha->m_ExtraSlot->m_PlusNum = plus;

				m_pNkCha->m_ExtraSlot->m_Special = special;

				///신방어구강화시스템
				m_pNkCha->m_ExtraSlot->m_Special2 = special2; // 추가 옵션
				m_pNkCha->m_ExtraSlot->m_UpgradeEndurance = UpEndur;           // 현재 강화 내구도
				m_pNkCha->m_ExtraSlot->m_MaxUpgradeEndurance = MaxUpdEndur;    // 강화 내구도 최대치

				m_pNkCha->m_ExtraSlot->m_Endurance = Endur;
				m_pNkCha->m_ExtraSlot->m_MaxEndurance = MaxEndur;

				// 아이템 값을 입력해보자!
				// 아이템 속성값들 설정하는 함수, 아래 주석부분
				ResetItemValue(m_pNkCha->m_ExtraSlot);
				// 아이템의 스페셜값 셋팅
				SetSpecialValue(m_pNkCha->m_ExtraSlot, GET_TYPE(m_pNkCha->m_ExtraSlot), GET_SHAPE(m_pNkCha->m_ExtraSlot), special, special2);
			}
		}
		/*

		//오른 더블 클릭 다량 이동시 버그 때문에 주석 처리함... 지워도 될듯 하지만,, 일단 남겨둠.

		else {	// 아이템 다량이동의 경우

			// 선택된 창고슬롯에 아이템이 있는지 확인
			// 예회상황에 대한 처리
			// 혹시나 인벤에 데이터가 있을까 하는 계산
			SAFE_DELETE( m_WareInven[pack_num][x][y] );
			// 아이템 생성
			m_WareInven[pack_num][x][y] = new CItem;
			if( m_WareInven[pack_num][x][y] )
			{
				m_WareInven[pack_num][x][y]->m_Index = index;
				m_WareInven[pack_num][x][y]->m_Vnum = vnum;
				m_WareInven[pack_num][x][y]->m_ProtoNum = g_ItemList.FindItem(vnum);

				// 서브아이템인지 체크
				if(GET_TYPE(m_WareInven[pack_num][x][y])!=ITYPE_SUB){//1111
					if(g_ItemList.m_ItemArray[m_WareInven[pack_num][x][y]->m_ProtoNum].m_MinLevel>=181)
						m_WareInven[pack_num][x][y]->SetDiaGemNum(plus);
					else
						m_WareInven[pack_num][x][y]->SetGemNum(plus);
				}
				else
					m_WareInven[pack_num][x][y]->m_PlusNum = plus;

				m_WareInven[pack_num][x][y]->m_Special = special;

				///신방어구강화시스템
				m_WareInven[pack_num][x][y]->m_Special2 = special2; // 추가 옵션
				m_WareInven[pack_num][x][y]->m_UpgradeEndurance = UpEndur;           // 현재 강화 내구도
				m_WareInven[pack_num][x][y]->m_MaxUpgradeEndurance = MaxUpdEndur;    // 강화 내구도 최대치

				m_WareInven[pack_num][x][y]->m_Endurance = Endur;
				m_WareInven[pack_num][x][y]->m_MaxEndurance = MaxEndur;

				// 아이템 값을 입력해보자!
				ResetItemValue(m_WareInven[pack_num][x][y]);
				SetSpecialValue(m_WareInven[pack_num][x][y], GET_TYPE(m_WareInven[pack_num][x][y]), GET_SHAPE(m_WareInven[pack_num][x][y]), special, special2);
			}
		}*/
		// 작업대기 종료
		m_bMsgWaiting = FALSE;
	}

	// 창고슬롯의 아이템이 0 이상이면 갯수 출력
	if ( m_bActivate && m_WareSlot[pack_num][x][y] > 0) // m_bActivate 체크추가. 패킷 오기전에 창 닫아버리면 튕긴기 때문. 06-03-18 원석.
	{
		char tmp[10] = "";

		sprintf(tmp, "%d",m_WareSlot[pack_num][x][y]);
		m_pNumText[pack_num][x][y].SetString(tmp);

		// 창고인벤의 좌표 셋팅
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

// nate : 창고 아이템 다량이동 기능 메세지 처리부
// 인자 : 아이템번호, 인벤팩번호, 창고팩번호, 창고슬롯번호, 인벤 X, 인벤 Y
// 인벤 -> 창고
// 기능 : 인벤의 아이템 제거, 창고 아이템 추가
void CControlWareHouse::StashPut( int nVnum, int nInvenPackNum, int nWarePackNum, int nWareSlotNum, int nSlot_X, int nSlot_Y )
{
	if( !m_pNkCha )
		return;

	CItem *pItem = NULL;
	// 인벤 슬롯 위치의 아이템 인덱스 추출
	int item_idx = m_pNkCha->GetItemIndexInSlot( nInvenPackNum, nSlot_X, nSlot_Y, 1, 1 );
	if ( item_idx >= 0 )
		// 현재 인벤팩넘에 있는 아이템을 인덱스로 검색해서 제거
		// 함수자체는 메모리 해제 안함( 수정 필요 )
		pItem = m_pNkCha->PickItemInPack( nInvenPackNum, item_idx );

	int nSpecial = 0;
	int nPlusNum = 0;

	// 현재 서버에서 전달된인덱스의 아이템이 있을경우
	if ( pItem )
	{
		nSpecial = pItem->m_Special;
		if( GET_TYPE( pItem ) != ITYPE_SUB )	// 서브아이템 ㅈ[외
		{
			// 이이템에는 착용 가능한 레벨이 잇다.
			// 181은 다이아급아이템
			// 이 아이템은 플러스값을 그대로 사용
			// PlusNum 와 GemNum 값의 차이를 알아야 함 SetGemNum() 함수 참고

			//-- 2004. 03. 22  / Lyul
			//-- 랩프리 다이아 장비 인식 오류 Big Fix
			if( GET_ITEM_LEVEL(pItem) >= 181 )
				nPlusNum = pItem->m_PlusNum;
			else
			{
				// 나머지는 레벨만큼의 보석이 추가딤
				// 예 : 1렙 1개 , 2렙 1 + 2개, 5랩 10 + 5개 필요
				for( int i = 1; i <= pItem->m_PlusNum; i++ )
					nPlusNum += i;
			}
		}
		else // 서브 아이템의 경우
			nPlusNum = pItem->m_PlusNum;

		// 창고에 아이템을 넣고 카운트를 출력
		// 수정필요 : 기존의 ExtraSlot을 사용하던 함수라 다이렉트 이동에 필요한 함수가 필요
		if( g_SvrType == ST_ADULT_ONLY ) // [6/1/2007 Theodoric] 내구도 수리
		{
			StashTo( nWareSlotNum, nVnum, nPlusNum, nSpecial, pItem->m_Special2, pItem->m_UpgradeEndurance,
					 pItem->m_MaxUpgradeEndurance, pItem->m_Endurance, pItem->m_MaxEndurance, -1/* 창고슬롯의 카운트*/, 1/*swap_flag*/); ///신방어구강화시스템
		}
		else
		{
			StashTo( nWareSlotNum, nVnum, nPlusNum, nSpecial, pItem->m_Special2, pItem->m_UpgradeEndurance,
					 pItem->m_MaxUpgradeEndurance,0, 0, -1/* 창고슬롯의 카운트*/, 1/*swap_flag*/); ///신방어구강화시스템
		}
	}

	// 아이템 추가후 삭제
	SAFE_DELETE( pItem );
}


// nate : 창고 아이템 다량이동 기능 메세지 처리부
// 인자 : 아이템번호, 인벤팩번호, 창고팩번호, 창고슬롯번호, 아이템 인덱스값, 인벤 X, 인벤 Y
// 창고 -> 인벤
// 기능 : 인벤에 아이템 추가, 창고에 아이템 제거
void CControlWareHouse::StashGet( int nVnum, int nInvenPackNum, int nWarePackNum, int nWareSlotNum, int nItemIndex, int nSlot_X, int nSlot_Y )
{
	//  [11/26/2008 parkmj] 유료창고시 더블클릭으로 아이템 빼는 부분 추가
	// [4/18/2007 Theodoric] 태국 유료 창고
	if( m_bCash && nWareSlotNum != 12 )
	{
		nWarePackNum  = nWarePackNum - 3;
	}

	// 서버에서 Interval간격으로 넘어온 슬롯넘을 현재팩의 슬롯 넘으로 계산
	int nTempSlotNum = nWareSlotNum % WARE_PACK_INTERVAL;

	// 현재 슬롯의 X, Y좌표 계산
	int nX = nTempSlotNum / WARE_SLOT_Y_NUM;
	int nY = nTempSlotNum % WARE_SLOT_Y_NUM;

	// 선택한 창고의 아이템 추출
	CItem *pItem = m_WareInven[ nWarePackNum ][ nX ][ nY ];

	// 아이템이 있으면
	if( pItem )
	{
		int nSpecial = pItem->m_Special;
		int nSpecial2 = pItem->m_Special2;
		int nPlusNum = 0;
		int nUpgradeEndurance = 0;
		int nMaxUpgradeEndurance = 0;

		int	nEndur = 0;
		int nMaxEndur = 0;

		// 서브 아이템은 보석이든 마석이든 달수없다.
		if( GET_TYPE( pItem ) != ITYPE_SUB )	// 세컨아이템인가?
		{
			if( GET_ITEM_LEVEL(pItem) >= 181 )	// 다이아급은 플러스넘 그대로 적용
				nPlusNum = pItem->m_PlusNum;
			else
			{
				// 나머지는 레벨만큼의 보석이 추가딤
				// 예 : 1렙 1개 , 2렙 1 + 2개, 5랩 10 + 5개 필요
				for( int i = 1; i <= pItem->m_PlusNum; i++ )
					nPlusNum += i;
			}
		}
		else // 서브 아이템의 경우 프러스넘 그대로 적용
			nPlusNum = pItem->m_PlusNum;

		nUpgradeEndurance = pItem->m_UpgradeEndurance; // 밑의 포인터조작때문에 위치가 바뀌는 경우가 있는 듯. 변수로 받아서 기억하고 써야한다.
		nMaxUpgradeEndurance = pItem->m_MaxUpgradeEndurance; // 밑의 포인터조작때문에 위치가 바뀌는 경우가 있는 듯. 변수로 받아서 기억하고 써야한다.

		nEndur = pItem->m_Endurance;
		nMaxEndur = pItem->m_MaxEndurance;

		// [6/7/2007 Theodoric] 내구도 수리
		StashFrom( nWareSlotNum, nItemIndex, nVnum, nPlusNum/*GemNum*/, nSpecial/*special*/,nSpecial2,
				   nUpgradeEndurance, nMaxUpgradeEndurance,nEndur, nMaxEndur, -1/*Item count는 플래그로 사용*/ ); ///신방어구강화시스템
		if( g_pRoh )
		{
			// [6/7/2007 Theodoric] 내구도 수리
			g_pRoh->AddToInven( nInvenPackNum, nSlot_X, nSlot_Y, nItemIndex, nVnum, nPlusNum/*GetNum*/, nSpecial/*special*/, nSpecial2, nUpgradeEndurance, nMaxUpgradeEndurance, nEndur, nMaxEndur  ); ///신방어구강화시스템
		}
	}
}

LRESULT CControlWareHouse::MoneyPopupMsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, int nMsg, char* szMoney)
{
	// "확인" 버튼이 눌린경우
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
	else if (nMsg == -1)	// "취소" 버튼이 눌린경우
		m_byInOutMoneyState = 0;

	return 0L;
}

LRESULT CControlWareHouse::PopupMsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, int nMsg)
{
	if( nMsg == 1)	// 확인버튼이 눌린경우
	{
		strcpy(m_MsgBuf, m_strStashPacketTemp.c_str());
		SendMsg();
		m_bMsgWaiting = TRUE;
	}
	else if(nMsg == -1) // 취소버튼이 눌린경우
	{
		m_strStashPacketTemp.clear();
	}

	return 0L;
}

// [4/16/2007 Theodoric] 태국 유료 창고 :: 관련 함수들
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


// 빈슬롯번호(0,1,2 선택사항)
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

// 빈슬롯번호( 0~2번팩)
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

// 넣을수 있는 공간이 있는가?
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
