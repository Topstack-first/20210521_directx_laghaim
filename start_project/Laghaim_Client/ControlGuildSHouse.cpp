#include "stdafx.h"


#include "main.h"
#include "BasicButton.h"
#include "ControlInven.h"
#include "ControlGuildSHouse.h"
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
#include "2DResMgr.h"
#include "WindowMgr.h"

extern DWORD				g_dwLangType;
extern int					g_nCurWorldIdx;


// 개수를 나타내는 TEXTOUTBOX를 위한 정의
#define _TEXT_COLOR1 RGB(255, 255, 0)
#define _TEXT_COLOR2 RGB(0, 0, 0)
#define _TEXT_SIZE   20
#define TAB_BTN_NUM  9

const int PER_COUNT = 200;

CControlGuildSHouse::CControlGuildSHouse()
	: m_hWnd(NULL)
	, m_pNkCha(NULL)
	, m_timeWareLock(0)
	, m_wareslot_start_x(0)
	, m_wareslot_start_y(0)
	, m_ScreenWidth(0)
	, m_ScreenHeight(0)
	, m_nSaveMoney(0)
	, m_iCurSlot_X(0)
	, m_iCurSlot_Y(0)
{
	memset(m_MsgBuf, 0, sizeof(m_MsgBuf));
	memset(m_strMoney1, 0, sizeof(m_strMoney1));

	m_pBack = NULL;
	m_bActivate = FALSE;
	m_bWareLock = FALSE;
	m_pBtnTab = NULL;

	m_pItemBack	= NULL;

	if(m_pBtnTab == NULL)
		m_pBtnTab = new CBasicButton[TAB_BTN_NUM];

	m_pSlot = NULL;
	m_pMoneyBk = NULL;

	m_using_store = 0;

	Init();
}

CControlGuildSHouse::~CControlGuildSHouse()
{
	DeleteRes();

	SAFE_DELETE_ARRAY(m_pBtnTab);

	m_pBack = NULL;
	m_pNkCha = NULL;
}


void CControlGuildSHouse::Init()
{
	int i = 0;
	int j = 0;
	int k = 0;

	m_bGetStashEnd = FALSE;
	m_InMoneyBtn.SetFileName("guild/inven_moneyin");
	m_OutMoneyBtn.SetFileName("guild/inven_moneyout");
	m_CloseBtn.SetFileName("guild/close");

	char szTmp[128] = {0,};

	for(i = 0; i < TAB_BTN_NUM; i++)
	{
		sprintf(szTmp, "guild/inven_tap%d", i+1);
		m_pBtnTab[i].SetFileName(szTmp);
	}

	// 메세지 팝업창 초기화
	m_MoneyText.Init(12, RGB(255, 255, 0), 0, 0);

	// nate : 슬롯에 보이는 그림 초기화.
	for (k=0; k < GUILD_STORE_PACK_NUM; k++)
	{
		for (i=0; i < GUILD_STORE_SLOT_X_NUM; i++)
		{
			for (j=0; j < GUILD_STORE_SLOT_Y_NUM; j++)
			{
				m_WareInven[k][i][j] = NULL;
				m_WareSlot[k][i][j] = 0;	// 슬롯 카운트
			}
		}
	}
	m_byInOutMoneyState = 0;
	m_bMsgWaiting = FALSE;
	m_nPackNum = 0;
}

void CControlGuildSHouse::LoadRes()
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

	float StartClient_W = 800;
	float StartClient_H = 600;

	// button load img
	m_InMoneyBtn.LoadRes();
	m_OutMoneyBtn.LoadRes();
	m_CloseBtn.LoadRes();

	for(int i = 0; i < TAB_BTN_NUM; i++)
	{
		m_pBtnTab[i].LoadRes();
	}

	m_pBtnTab[0].SetState(BTN_DOWN);

	m_pBack = pCMyApp->Get2DResMgr()->GetSurfaceFrom_guildback("guild_back");

	g_pDisplay->CreateSurfaceFromBitmap(&m_pItemBack, "interface/guild/guild_inven/inven_bk.bmp");

	g_pDisplay->CreateSurfaceFromBitmap(&m_pMoneyBk, "interface/guild/guild_inven/money_bk.bmp");

	if( m_pBack && m_pItemBack)
	{
		m_pBack->Xpos = m_ClientRc.right - (m_pBack->GetWidth()*2);
		m_pBack->Ypos = 0;

		m_wareslot_start_x = m_pBack->Xpos + 29;
		m_wareslot_start_y = m_pBack->Ypos + 134;

		m_pMoneyBk->Xpos = m_pBack->Xpos + 58;
		m_pMoneyBk->Ypos = m_pBack->Ypos + 396;

		m_InMoneyBtn.SetPosition(m_pMoneyBk->Xpos+m_pMoneyBk->GetWidth()+2, m_pBack->Ypos+397);
		m_OutMoneyBtn.SetPosition(m_InMoneyBtn.PosX+m_InMoneyBtn.Width+2, m_InMoneyBtn.PosY);
		m_CloseBtn.SetPosition(m_pBack->Xpos+221, m_pBack->Ypos+435);

		m_pItemBack->Xpos	=	 m_pBack->Xpos + 25;
		m_pItemBack->Ypos	=	m_pBack->Ypos  + 101;

		int nX = 25;

		for(int s = 0; s < TAB_BTN_NUM; s++, nX += 25)
		{
			m_pBtnTab[s].SetPosition(m_pBack->Xpos+nX , m_pBack->Ypos+101);
		}

		m_MoneyText.SetPos(m_pBack->Xpos+74, m_pBack->Ypos+405);

	}

	if (m_pNkCha)
		m_MoneyText.SetString((m_nSaveMoney*100), true);

	char tmp[10];
	int count = -1;

	for (int k=0; k < GUILD_STORE_PACK_NUM; k++)
	{
		for (int x=0; x < GUILD_STORE_SLOT_X_NUM; x++)
		{
			for (int y=0; y < GUILD_STORE_SLOT_Y_NUM; y++)
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

void CControlGuildSHouse::DeleteRes()
{
	SAFE_DELETE(m_pItemBack);
	SAFE_DELETE(m_pMoneyBk);

	m_InMoneyBtn.DeleteRes();
	m_OutMoneyBtn.DeleteRes();
	m_CloseBtn.DeleteRes();
	m_MoneyText.DeleteRes();

	for (int k=0; k < GUILD_STORE_PACK_NUM; k++)
	{
		for (int i=0; i < GUILD_STORE_SLOT_X_NUM; i++)
		{
			for (int j=0; j < GUILD_STORE_SLOT_Y_NUM; j++)
				m_pNumText[k][i][j].DeleteRes();
		}
	}
}


void CControlGuildSHouse::SetWareLock( BOOL bLock )
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
CItem *CControlGuildSHouse::Draw()
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

	if( g_pDisplay && m_pItemBack )
		g_pDisplay->Blt(m_pItemBack->Xpos, m_pItemBack->Ypos, m_pItemBack);

	if( g_pDisplay && m_pMoneyBk )
		g_pDisplay->Blt(m_pMoneyBk->Xpos, m_pMoneyBk->Ypos, m_pMoneyBk);

	m_InMoneyBtn.Draw();
	m_OutMoneyBtn.Draw();
	m_CloseBtn.Draw();

	// nate : 창고 슬롯 확장
	// 버튼 추가
	// 창고 팩버튼
	// 버튼의 클릭 순서에 따라 그리기 순서 결정

	for(int k = 0; k < TAB_BTN_NUM; k++)
	{
		m_pBtnTab[k].Draw();
	}

	// 마우스 포인터의 현재위치 계산
	POINT point;
	GetCursorPos(&point);
	ScreenToClient(g_pDisplay->GetHWnd(), &point);

	int x = (int)point.x;
	int y = (int)point.y;

	m_iCurSlot_X = -1;
	m_iCurSlot_Y = -1;
	// 마우스가 창고의 슬롯 영역에 눌러졌는지 체크
	if ( x >= m_wareslot_start_x
			&& x < m_wareslot_start_x + GUILD_STORE_SLOT_WIDTH * GUILD_STORE_SLOT_X_NUM
			&& y >= m_wareslot_start_y
			&& y < m_wareslot_start_y + GUILD_STORE_SLOT_HEIGHT * GUILD_STORE_SLOT_Y_NUM )
	{
		int slot_x_num, slot_y_num, slot_index;

		// 현재 슬롯 셋팅
		m_iCurSlot_X = slot_x_num = (x - m_wareslot_start_x) / GUILD_STORE_SLOT_WIDTH;
		m_iCurSlot_Y = slot_y_num = (y - m_wareslot_start_y) / GUILD_STORE_SLOT_HEIGHT;
		// 슬롯의 인텍스 계산
		// 팩의 몇번째 슬롯인가 계산
		slot_index = slot_x_num * GUILD_STORE_SLOT_Y_NUM + slot_y_num;
	}

	CItem *m_pOverItem = NULL;

	if( g_pNk2DFrame->GetItemRender() )
	{
		g_pNk2DFrame->GetItemRender()->BeginRenderLolo();
		for (i=0; i < GUILD_STORE_SLOT_X_NUM; i++)
		{
			for (j=0; j < GUILD_STORE_SLOT_Y_NUM; j++)
			{
				if (m_WareSlot[m_nPackNum][i][j] > 0)
				{
					pItem = m_WareInven[m_nPackNum][i][j];
					DWORD ambient = 0x00555555;

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

					g_pNk2DFrame->RenderItemInUI( m_wareslot_start_x + i * GUILD_STORE_SLOT_WIDTH,
												  m_wareslot_start_y + j * GUILD_STORE_SLOT_HEIGHT + (GUILD_STORE_SLOT_HEIGHT - ITEM_RENDER_HEIGHT) /2,
												  pItem , FALSE, FALSE, TRUE, TRUE );

				}
			}
		}
		g_pNk2DFrame->GetItemRender()->EndRenderLolo();
	}

	// 아이템을 들거나 메세지창이 있을경우 아이템인포를창을 출력하지 않는다
	if (m_pNkCha && m_pNkCha->m_ExtraSlot || g_pNk2DFrame->m_Popup)
		m_pOverItem = NULL;

	for (i=0; i < GUILD_STORE_SLOT_X_NUM; i++)
	{
		for (j=0; j < GUILD_STORE_SLOT_Y_NUM; j++)
		{
			if(m_WareSlot[m_nPackNum][i][j] > 0)
			{
				int xpos, ypos;
				if( m_WareInven[m_nPackNum][i][j] )
				{
					xpos = m_wareslot_start_x + (m_WareInven[m_nPackNum][i][j]->m_SlotX*GUILD_STORE_SLOT_WIDTH);
					ypos = m_wareslot_start_y + (m_WareInven[m_nPackNum][i][j]->m_SlotY*GUILD_STORE_SLOT_HEIGHT);
				}
				m_pNumText[m_nPackNum][i][j].Draw(xpos, ypos);
			}
		}
	}

	// 인벤창이 가려지므로 nk2d에서 드로우를 한다..
	m_MoneyText.Draw();

	if (m_pOverItem)
		return m_pOverItem;
	else
		return NULL;
}

BOOL CControlGuildSHouse::CheckMoneyStr(char* szMoney)
{
	__int64 nMoney = 0;

	// 입력된 스트링을 검사한다
	int len = strlen(szMoney);

	for(int i=0; i<len; i++)
	{
		if(isdigit(szMoney[i]) == FALSE)
		{
			if(is_space(szMoney[i]) ==TRUE)
				//-- IDS_WAREHOUSE_BLANK : 공백문자는 허용하지 않습니다.
				g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_WAREHOUSE_BLANK), TYPE_GUILD_EXCHANGE, (int)(POPUP_GUILD_MONEYEXCHANGE)  );
			else
				//-- IDS_WAREHOUSE_NOTNUM : 잘못된 금액입니다. 다시 입력하세요
				g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_WAREHOUSE_NOTNUM), TYPE_GUILD_EXCHANGE, (int)(POPUP_GUILD_MONEYEXCHANGE)  );

			return FALSE;
		}
	}

	// 100 단위로 입력이 되지 않은경우

	if(len >2)
	{
		if(atoi(&szMoney[len-2]) != 0  || atoi(&szMoney[len-1]) != 0)
		{
			//-- IDS_WAREHOUSE_NOT100 : 금액은 100 단위로 입력해 주세요
			g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_WAREHOUSE_NOT100), TYPE_GUILD_EXCHANGE, (int)(POPUP_GUILD_MONEYEXCHANGE)  );
			return FALSE;
		}
	}
	else
	{
		//-- IDS_WAREHOUSE_NOT100 : 금액은 100 단위로 입력해 주세요
		g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_WAREHOUSE_NOT100), TYPE_GUILD_EXCHANGE, (int)(POPUP_GUILD_MONEYEXCHANGE)  );
		return FALSE;
	}

	// 3. 현제 가지구 있는 보유금액보다 입력된 금액이 더큰경우
	nMoney = atoi(szMoney);

	if (nMoney < 0)
	{
		//-- IDS_WAREHOUSE_NOT0 : 입력값이 잘못되었습니다. 다시 입력하세요
		g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_WAREHOUSE_NOT0), TYPE_GUILD_EXCHANGE, (int)(POPUP_GUILD_MONEYEXCHANGE)  );
		return FALSE;
	}

	if(m_byInOutMoneyState != 2)
	{
		if(m_pNkCha->m_Money < nMoney)
		{
			//-- IDS_WAREHOUSE_NOTENOUGH :  금액이 부족합니다. 다시 입력하세요
			g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_WAREHOUSE_NOTENOUGH), TYPE_GUILD_EXCHANGE, (int)(POPUP_GUILD_MONEYEXCHANGE)  );
			return FALSE;
		}
	}
	else
	{
		if((m_nSaveMoney*100) < nMoney)
		{
			//-- IDS_WAREHOUSE_NOTENOUGH :  금액이 부족합니다. 다시 입력하세요
			g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_WAREHOUSE_NOTENOUGH), TYPE_GUILD_EXCHANGE, (int)(POPUP_GUILD_MONEYEXCHANGE)  );
			return FALSE;
		}
	}

	return TRUE;
}

void CControlGuildSHouse::IsInsideBtn(int nBtNum, int x, int y)
{
	if(!m_pBtnTab[nBtNum].IsInside(x, y)) return;

	m_pBtnTab[nBtNum].SetState(BTN_DOWN);

	m_nPackNum = nBtNum;

	//for(int i = 0; i < TAB_BTN_NUM; i++)
	for(int i = 0; i < m_using_store; i++)
	{
		if((i != nBtNum) && (m_pBtnTab[i].GetState() != BTN_NORMAL) )
		{
			m_pBtnTab[i].SetState(BTN_NORMAL);
		}
	}
}

LRESULT CControlGuildSHouse::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	int x = 0, y = 0;

	switch ( msg )
	{
	case WM_MOUSEMOVE :
		{
			//for(int i = 0; i < TAB_BTN_NUM; i++)
			for(int i = 0; i < m_using_store; i++)
			{
				if(m_pBtnTab[i].GetState() != BTN_DOWN)
					m_pBtnTab[i].MsgProc(hWnd, msg, wParam, lParam);
			}

			m_InMoneyBtn.MsgProc(hWnd, msg, wParam, lParam);
			m_OutMoneyBtn.MsgProc(hWnd, msg, wParam, lParam);
			m_CloseBtn.MsgProc(hWnd, msg, wParam, lParam);
		}

		return 1;
	case WM_RBUTTONDOWN:
		x = LOWORD (lParam);
		y = HIWORD (lParam);

		if (IsPlayArea(x, y) && !m_bMsgWaiting)
		{
			if (m_bActivate)
				// 창고를 닫는다.
				ActivateGStoreHouse(FALSE);

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

			for(int i = 0; i < m_using_store; i++)
			{
				IsInsideBtn(i, x, y);
			}
// 			IsInsideBtn(0, x, y);
// 			IsInsideBtn(1, x, y);
// 			IsInsideBtn(2, x, y);
// 			IsInsideBtn(3, x, y);
// 			IsInsideBtn(4, x, y);
// 			IsInsideBtn(5, x, y);
// 			IsInsideBtn(6, x, y);
// 			IsInsideBtn(7, x, y);
// 			IsInsideBtn(8, x, y);

			if ( x >= m_wareslot_start_x
					&& x < m_wareslot_start_x + GUILD_STORE_SLOT_WIDTH*GUILD_STORE_SLOT_X_NUM
					&& y >= m_wareslot_start_y
					&& y < m_wareslot_start_y + GUILD_STORE_SLOT_HEIGHT*GUILD_STORE_SLOT_Y_NUM )
			{
				int slot_x_num, slot_y_num, slot_index;
				int vnum, item_plus, item_flag, count;

				slot_x_num = (x - m_wareslot_start_x) / GUILD_STORE_SLOT_WIDTH;
				slot_y_num = (y - m_wareslot_start_y) / GUILD_STORE_SLOT_HEIGHT;
				slot_index = slot_x_num * GUILD_STORE_SLOT_Y_NUM + slot_y_num;

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
					else if( GET_ATT_FLAG(m_pNkCha->m_ExtraSlot) & IATT_GUILD_NO_STASH )
					{
						g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_CANT_GSTASH_WITH_NOGSTASH), TYPE_NOR_OK, 1 );
						return 1;
					}
					else if( !(GET_ATT_FLAG(m_pNkCha->m_ExtraSlot) & IATT_GUILD_NO_STASH) )
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
								return 1;

						item_plus = m_pNkCha->m_ExtraSlot->m_GemNum;
						item_flag = m_pNkCha->m_ExtraSlot->m_Special;

						slot_index = m_nPackNum*GUILD_STORE_PACK_INTERVAL + slot_index;
						sprintf(m_MsgBuf, "mg stash t %d %d %d\n", slot_index, vnum, item_plus);
						m_bMsgWaiting = TRUE;
						SendMsg();
						count =  m_WareSlot[m_nPackNum][x][y];
					}
				}
				else if(g_pRoh->GetMyGuildData() && g_pRoh->GetMyGuildData()->GetMyInfo().s_glevel < 10 )
				{
					// IDS_GUILD_STASH_TAKE_FAIL_MEMBER "길드 마스터만 찾기가 가능합니다."
					g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_GUILD_STASH_TAKE_FAIL_MEMBER), TYPE_GUILD_CONFIRM );
				}
				else if(m_WareSlot[m_nPackNum][slot_x_num][slot_y_num] > 0)
				{
					if( m_WareInven[m_nPackNum][slot_x_num][slot_y_num] )
					{
						vnum = m_WareInven[m_nPackNum][slot_x_num][slot_y_num]->m_Vnum;
						item_plus = m_WareInven[m_nPackNum][slot_x_num][slot_y_num]->m_GemNum;
						item_flag = m_WareInven[m_nPackNum][slot_x_num][slot_y_num]->m_Special;
					}

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

					slot_index = m_nPackNum*GUILD_STORE_PACK_INTERVAL + slot_index;
					sprintf(m_MsgBuf, "mg stash t %d %d %d\n", slot_index, vnum, item_plus);
					m_bMsgWaiting = TRUE;
					SendMsg();
				}
			}
			return 1;
		}
		else if (IsPlayArea(x, y) && !m_bMsgWaiting)
		{
			if (m_bActivate)
				// 창고를 닫는다.
				ActivateGStoreHouse(FALSE);

			g_pNk2DFrame->ShowGStoreHouseWindow(FALSE);
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
						&& x < m_wareslot_start_x + GUILD_STORE_SLOT_WIDTH*GUILD_STORE_SLOT_X_NUM
						&& y >= m_wareslot_start_y
						&& y < m_wareslot_start_y + GUILD_STORE_SLOT_HEIGHT*GUILD_STORE_SLOT_Y_NUM )
				{
					int slot_x_num, slot_y_num, slot_index;
					int nVnum;

					// 슬롯넘버와 팩에서의 슬롯 인덱스 계산
					slot_x_num = (x - m_wareslot_start_x) / GUILD_STORE_SLOT_WIDTH;
					slot_y_num = (y - m_wareslot_start_y) / GUILD_STORE_SLOT_HEIGHT;
					slot_index = slot_x_num * GUILD_STORE_SLOT_Y_NUM + slot_y_num;

					// 슬롯에 아이템이 있으면
					if ( m_WareSlot[ m_nPackNum ][ slot_x_num ][ slot_y_num ] > 0 )
					{
						int chaotic = g_pRoh->m_Chaotic_Num * 0.1;
						if( chaotic < 0 && chaotic > -500 )
						{
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
						// WARE_PACK_INTERVAL  = 20  : 팩간의 인덱스 간격
						slot_index = m_nPackNum * GUILD_STORE_PACK_INTERVAL + slot_index;
						if( m_pNkCha )
							sprintf(m_MsgBuf, "mg stash g %d %d %d %d\n", nVnum, m_pNkCha->m_CurPackNum, m_nPackNum, slot_index);
						m_bMsgWaiting = TRUE;
						SendMsg();
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

			m_InMoneyBtn.MsgProc(hWnd, msg, wParam, lParam);
			m_OutMoneyBtn.MsgProc(hWnd, msg, wParam, lParam);
			m_CloseBtn.MsgProc(hWnd, msg, wParam, lParam);
			if(m_InMoneyBtn.GetState() == BTN_ACTION)
			{
				//-- IDS_WAREHOUSE_INMONEY :  입금하실 금액을   적어주세요
				g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_WAREHOUSE_INMONEY), TYPE_GUILD_EXCHANGE, (int)(POPUP_GUILD_MONEYEXCHANGE) );
				m_byInOutMoneyState = 1;
				m_InMoneyBtn.m_iBtnFlg = BTN_NORMAL;
			}
			else if(m_OutMoneyBtn.GetState() == BTN_ACTION)
			{
				if(g_pRoh->GetMyGuildData() && g_pRoh->GetMyGuildData()->GetMyInfo().s_glevel < 10 )
				{
					// IDS_GUILD_STASH_TAKE_FAIL_MEMBER "길드 마스터만 찾기가 가능합니다."
					g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_GUILD_STASH_TAKE_FAIL_MEMBER), TYPE_GUILD_CONFIRM );
					return 1;
				}

				int chaotic = g_pRoh->m_Chaotic_Num * 0.1;
				if( chaotic < 0 && chaotic > -500 )
				{
				}
				else if( chaotic <= -500 )
				{
					if( g_dev_client )
						g_pNk2DFrame->AddChatStr( "No withdrawal" , -1 );
					m_OutMoneyBtn.m_iBtnFlg = BTN_NORMAL;
					return 0;
				}
				//-- IDS_WAREHOUSE_OUTMONEY : 출금하실 금액을   적어주세요
				g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_WAREHOUSE_OUTMONEY), TYPE_GUILD_EXCHANGE, (int)(POPUP_GUILD_MONEYEXCHANGE) );
				m_byInOutMoneyState = 2;
				m_OutMoneyBtn.m_iBtnFlg = BTN_NORMAL;
			}
			else if (m_CloseBtn.GetState() == BTN_ACTION)
			{
				if( g_pNk2DFrame )
					g_pNk2DFrame->ShowGStoreHouseWindow(FALSE);

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


BOOL CControlGuildSHouse::InMoneyMsg(char* money)
{
	sprintf(m_MsgBuf, "mg stash t 12 0 %s\n", money);	// 스페이스 주의.
	SendMsg();
	m_byInOutMoneyState = 0;

	return true;
}

BOOL CControlGuildSHouse::OutMoneyMsg(char* money)
{
	sprintf(m_MsgBuf, "mg stash t 12 0 -%s\n", money);	// 스페이스 주의.
	SendMsg();
	m_byInOutMoneyState = 0;

	return true;
}


BOOL CControlGuildSHouse::IsInside(int x, int y)
{
	if (x >= m_pBack->Xpos
			&& x < m_pBack->Xpos + m_pBack->GetWidth()
			&& y >= m_pBack->Ypos
			&& y < m_pBack->Ypos + m_pBack->GetHeight() )
		return TRUE;

	return FALSE;
}

int CControlGuildSHouse::GetWareSlotIndex (int x, int y)
{
	if (x < 0 || x >= GUILD_STORE_SLOT_X_NUM || y < 0 || y >= GUILD_STORE_SLOT_Y_NUM)
		return -1;

	return m_WareSlot[m_nPackNum][x][y];
}

// 창고의 문이 닫힌다.
// 창고 데이타를 삭제한다.
void CControlGuildSHouse::RemoveAll()
{
	int i, j, k;

	for (k=0; k < GUILD_STORE_PACK_NUM; k++)
	{
		for (i=0; i < GUILD_STORE_SLOT_X_NUM; i++)
		{
			for (j=0; j < GUILD_STORE_SLOT_Y_NUM; j++)
				SAFE_DELETE( m_WareInven[k][i][j] );
		}
	}

	// 슬롯데이타 초기화.
	for (k=0; k < GUILD_STORE_PACK_NUM; k++)
	{
		for ( i=0; i < GUILD_STORE_SLOT_X_NUM; i++)
		{
			for (j=0; j < GUILD_STORE_SLOT_Y_NUM; j++)
			{
				m_WareSlot[k][i][j] = 0;
			}
		}
	}

	m_nSaveMoney = 0;
}

// nate
void CControlGuildSHouse::ActivateGStoreHouse(BOOL bActivate, int gLevel)
// 창고윈도우 활성화 시킴
{
	if (bActivate)
	{
		RemoveAll();
		strcpy(m_MsgBuf, "mg stash o\n");		// 서버에 창고가 열렸음을 알림

		if(gLevel >= 0 && gLevel <= 9)
			m_using_store = gLevel;
		else
			m_using_store = 0;
	}
	else
	{
		strcpy(m_MsgBuf, "mg stash c\n");		// 서버에 창고가 닫혔음을 알림
		m_bGetStashEnd = FALSE;	// 비활성화 되면 실패
		RemoveAll();		
	}

	m_bActivate = bActivate;
	// 불필요한 소스
	// 삭제 요맏
	if (m_bActivate == FALSE)
		m_bGetStashEnd = FALSE;

	// m_MsgBuf의 메세지 전송
	SendMsg();
}

BOOL CControlGuildSHouse::IsPlayArea(int x, int y)
{
	if( !IsInside(x,y) && !g_pNk2DFrame->IsInPopup(x,y) )
		return true;

	return false;
}

void CControlGuildSHouse::SendMsg()
{
	if (g_pTcpIp)
		g_pTcpIp->SendNetMessage(m_MsgBuf);
}


void CControlGuildSHouse::Stash(int slot_num, int vnum, int plus, int special, int special2,
								int UpEndur, int MaxUpdEndur, int Endur, int MaxEndur, int count, int time_limit ) ///시간제아이템 시간값 추가(050711 by 원석) ///신방어구강화시스템
{
	// 위치 파악.
	int x = 0, y = 0;
	CItem *pItem=NULL;
	int pack_num = 0;		// 임시.

	pack_num = slot_num / GUILD_STORE_PACK_INTERVAL;
	slot_num = slot_num % GUILD_STORE_PACK_INTERVAL;

	if (slot_num == 12)  				// 돈이 들어온 것이다.
	{
		m_nSaveMoney = plus;
		m_MoneyText.SetString((m_nSaveMoney*100), true);
		return;
	}

	x = slot_num / GUILD_STORE_SLOT_Y_NUM;
	y = slot_num % GUILD_STORE_SLOT_Y_NUM;

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

		pItem->m_Endurance = Endur;           // 현재 강화 내구도
		pItem->m_MaxEndurance = MaxEndur;    // 강화 내구도 최대치

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
		if( time_limit > 0 ) // 시간 값이 있으면 시간제 아이템으로 셋팅 (by 원석) ///시간제아이템
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
void CControlGuildSHouse::StashTo(int slot_num, int vnum, int plus, int special, int special2,
								  int UpEndur, int MaxUpdEndur, int Endur, int MaxEndur, int count, int swap_flag) ///신방어구강화시스템
// 인자 : 창고슬롯넘, 아이템넘, 플러스넘, 스페셜넘, 창고의 카운트, 교환플래그
// 인벤 -> 창고, ExtraSlot -> 창고 (교환플래그 : 아치템 갯수가 1일경우 교환 )
// 창고에 아이템을 넣고 카운트를 출력
{
	int x = 0, y = 0, pack_num = 0;
	CItem *pItem=NULL;

	// 슬롯넘으로 인덱스 계산
	// 슬롯넘은 선형적 ( 팩은 여러개지만 값은 선형적(?)으로 관리 )
	pack_num = slot_num / GUILD_STORE_PACK_INTERVAL;		// WARE_PACK_INTERVAL = 20
	slot_num = slot_num % GUILD_STORE_PACK_INTERVAL;		// 팩 사이의 간격이 20으로 설정되어 있음

	// 현재 슬롯의 좌표 계산
	x = slot_num / GUILD_STORE_SLOT_Y_NUM;
	y = slot_num % GUILD_STORE_SLOT_Y_NUM;

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

			m_WareInven[pack_num][x][y]->m_Endurance = Endur;           // 현재 강화 내구도
			m_WareInven[pack_num][x][y]->m_MaxEndurance = MaxEndur;    // 강화 내구도 최대치

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


// nate : 창고 -> 인벤, 창고 -> Extra Slot
// 인자 : 창고슬롯넘, 아이템인덱스, 아이템번호, 플러스넘, 스페셜, 아이템갯수
void CControlGuildSHouse::StashFrom(int slot_num, int index, int vnum, int plus, int &special, int special2,
									int UpEndur, int MaxUpdEndur, int Endur, int MaxEndur, int count) ///신방어구강화시스템 // 이녀석은 왜 special을 & 붙여서 인자 받는지 모르겠다...
{
	int x, y, pack_num;
	int special_temp = 0;
	CItem *pItem = NULL;

	// 창고 슬롯 계산
	pack_num = slot_num / GUILD_STORE_PACK_INTERVAL;
	slot_num = slot_num % GUILD_STORE_PACK_INTERVAL;

	x = slot_num / GUILD_STORE_SLOT_Y_NUM;
	y = slot_num % GUILD_STORE_SLOT_Y_NUM;

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
		if (m_WareSlot[pack_num][x][y] == 0)
		{
			SAFE_DELETE(m_WareInven[pack_num][x][y]);
			m_WareInven[pack_num][x][y] = NULL;
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

				m_pNkCha->m_ExtraSlot->m_Endurance = Endur;           // 현재 강화 내구도
				m_pNkCha->m_ExtraSlot->m_MaxEndurance = MaxEndur;    // 강화 내구도 최대치

				// 아이템 값을 입력해보자!
				// 아이템 속성값들 설정하는 함수, 아래 주석부분
				ResetItemValue(m_pNkCha->m_ExtraSlot);
				// 아이템의 스페셜값 셋팅
				SetSpecialValue(m_pNkCha->m_ExtraSlot, GET_TYPE(m_pNkCha->m_ExtraSlot), GET_SHAPE(m_pNkCha->m_ExtraSlot), special, special2);
			}
		}
		else  	// 아이템 다량이동의 경우
		{

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
				if(GET_TYPE(m_WareInven[pack_num][x][y])!=ITYPE_SUB) //1111
				{
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


				// 아이템 값을 입력해보자!
				ResetItemValue(m_WareInven[pack_num][x][y]);
				SetSpecialValue(m_WareInven[pack_num][x][y], GET_TYPE(m_WareInven[pack_num][x][y]), GET_SHAPE(m_WareInven[pack_num][x][y]), special, special2);
			}
		}
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

// nate : 창고를 연 후 모든 Item 정보가 도착했음을 알리는 메세지
//-- 2004.07.27
//-- Lyul
//-- 유료 창고 메세지 창을 표현한다.
void CControlGuildSHouse::StashEnd()
{
	m_bGetStashEnd = TRUE;
	// 창고 윈도우가 보임을 알림
	if( g_pNk2DFrame )
		g_pNk2DFrame->SetShowGStoreHouseFlag(TRUE);
	// 창고가 뜨면 화면 영역이 작아짐
	// by evilkiki 2009.10.01
	// pCMyApp->AdjustViewport();

	//-- 존이 라그라미아가 아닐 경우 창고 사용료 경고
	if( pCMyApp->m_pUIMgr && g_nCurWorldIdx !=4  && g_nCurWorldIdx !=11)
		g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_WAREHOUSE_ALARM), TYPE_NOR_OK, 1 );

	if( g_pNk2DFrame->GetUIWindow())
		g_pNk2DFrame->GetUIWindow()->AllCloseWindow();
}

// nate : 창고 아이템 다량이동 기능 메세지 처리부
// 인자 : 아이템번호, 인벤팩번호, 창고팩번호, 창고슬롯번호, 인벤 X, 인벤 Y
// 인벤 -> 창고
// 기능 : 인벤의 아이템 제거, 창고 아이템 추가
void CControlGuildSHouse::StashPut( int nVnum, int nInvenPackNum, int nWarePackNum, int nWareSlotNum, int nSlot_X, int nSlot_Y )
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
		StashTo( nWareSlotNum, nVnum, nPlusNum, nSpecial, pItem->m_Special2, pItem->m_UpgradeEndurance, pItem->m_MaxUpgradeEndurance, pItem->m_Endurance, pItem->m_MaxEndurance, -1/* 창고슬롯의 카운트*/, 1/*swap_flag*/ ); ///신방어구강화시스템
	}

	// 아이템 추가후 삭제
	SAFE_DELETE( pItem );
}


// nate : 창고 아이템 다량이동 기능 메세지 처리부
// 인자 : 아이템번호, 인벤팩번호, 창고팩번호, 창고슬롯번호, 아이템 인덱스값, 인벤 X, 인벤 Y
// 창고 -> 인벤
// 기능 : 인벤에 아이템 추가, 창고에 아이템 제거
void CControlGuildSHouse::StashGet( int nVnum, int nInvenPackNum, int nWarePackNum, int nWareSlotNum, int nItemIndex, int nSlot_X, int nSlot_Y )
{
	// 서버에서 Interval간격으로 넘어온 슬롯넘을 현재팩의 슬롯 넘으로 계산
	int nTempSlotNum = nWareSlotNum % GUILD_STORE_PACK_INTERVAL;

	// 현재 슬롯의 X, Y좌표 계산
	int nX = nTempSlotNum / GUILD_STORE_SLOT_Y_NUM;
	int nY = nTempSlotNum % GUILD_STORE_SLOT_Y_NUM;

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

		// [6/7/2007 Theodoric] 내구도 수리
		int nEndurance = 0;
		int	nMaxEndurance = 0;

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

		nEndurance = pItem->m_Endurance;
		nMaxEndurance = pItem->m_MaxEndurance;

		// [6/7/2007 Theodoric] 내구도 시스템 :: 함수 인자 추가
		StashFrom( nWareSlotNum, nItemIndex, nVnum, nPlusNum/*GemNum*/, nSpecial/*special*/,nSpecial2, nUpgradeEndurance, nMaxUpgradeEndurance,  nEndurance, nMaxEndurance,-1/*Item count는 플래그로 사용*/ ); ///신방어구강화시스템
		if( g_pRoh )
		{
			// [6/7/2007 Theodoric] 내구도 시스템 :: 함수 인자 추가
			g_pRoh->AddToInven( nInvenPackNum, nSlot_X, nSlot_Y, nItemIndex, nVnum, nPlusNum/*GetNum*/, nSpecial/*special*/, nSpecial2, nUpgradeEndurance, nMaxUpgradeEndurance, nEndurance, nMaxEndurance ); ///신방어구강화시스템
		}
	}
}

LRESULT CControlGuildSHouse::MoneyPopupMsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, int nMsg, char* szMoney)
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
