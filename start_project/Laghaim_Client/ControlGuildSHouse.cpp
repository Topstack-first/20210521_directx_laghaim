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


// ������ ��Ÿ���� TEXTOUTBOX�� ���� ����
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

	// �޼��� �˾�â �ʱ�ȭ
	m_MoneyText.Init(12, RGB(255, 255, 0), 0, 0);

	// nate : ���Կ� ���̴� �׸� �ʱ�ȭ.
	for (k=0; k < GUILD_STORE_PACK_NUM; k++)
	{
		for (i=0; i < GUILD_STORE_SLOT_X_NUM; i++)
		{
			for (j=0; j < GUILD_STORE_SLOT_Y_NUM; j++)
			{
				m_WareInven[k][i][j] = NULL;
				m_WareSlot[k][i][j] = 0;	// ���� ī��Ʈ
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


// nate : â�� Inven �� �׸���.
//-- 2004.07.12
//-- Lyul / �����Ϸ�
//-- â�� Inven �� ���� ������ Item �� �ٷ����� ���� �� �� �����Ƿ�
//-- �� cell �� Index �� ���������� ���� ���� ����.
//-- �׷��� ControlInven ���� AlphaPopup ������ ���� �������� Index �� ������
//-- ������ Update �ϴ��� �Ǻ��ϹǷ� ���װ� �ȴ�.
//-- controlinven ������ �Ϲ� inven �� ó���ϹǷ�
//-- ������ â����� ��ǥ�� ����Ͽ� â�� ���� Item �� ��������.
CItem *CControlGuildSHouse::Draw()
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

	if( g_pDisplay && m_pItemBack )
		g_pDisplay->Blt(m_pItemBack->Xpos, m_pItemBack->Ypos, m_pItemBack);

	if( g_pDisplay && m_pMoneyBk )
		g_pDisplay->Blt(m_pMoneyBk->Xpos, m_pMoneyBk->Ypos, m_pMoneyBk);

	m_InMoneyBtn.Draw();
	m_OutMoneyBtn.Draw();
	m_CloseBtn.Draw();

	// nate : â�� ���� Ȯ��
	// ��ư �߰�
	// â�� �ѹ�ư
	// ��ư�� Ŭ�� ������ ���� �׸��� ���� ����

	for(int k = 0; k < TAB_BTN_NUM; k++)
	{
		m_pBtnTab[k].Draw();
	}

	// ���콺 �������� ������ġ ���
	POINT point;
	GetCursorPos(&point);
	ScreenToClient(g_pDisplay->GetHWnd(), &point);

	int x = (int)point.x;
	int y = (int)point.y;

	m_iCurSlot_X = -1;
	m_iCurSlot_Y = -1;
	// ���콺�� â���� ���� ������ ���������� üũ
	if ( x >= m_wareslot_start_x
			&& x < m_wareslot_start_x + GUILD_STORE_SLOT_WIDTH * GUILD_STORE_SLOT_X_NUM
			&& y >= m_wareslot_start_y
			&& y < m_wareslot_start_y + GUILD_STORE_SLOT_HEIGHT * GUILD_STORE_SLOT_Y_NUM )
	{
		int slot_x_num, slot_y_num, slot_index;

		// ���� ���� ����
		m_iCurSlot_X = slot_x_num = (x - m_wareslot_start_x) / GUILD_STORE_SLOT_WIDTH;
		m_iCurSlot_Y = slot_y_num = (y - m_wareslot_start_y) / GUILD_STORE_SLOT_HEIGHT;
		// ������ ���ؽ� ���
		// ���� ���° �����ΰ� ���
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

					g_pNk2DFrame->RenderItemInUI( m_wareslot_start_x + i * GUILD_STORE_SLOT_WIDTH,
												  m_wareslot_start_y + j * GUILD_STORE_SLOT_HEIGHT + (GUILD_STORE_SLOT_HEIGHT - ITEM_RENDER_HEIGHT) /2,
												  pItem , FALSE, FALSE, TRUE, TRUE );

				}
			}
		}
		g_pNk2DFrame->GetItemRender()->EndRenderLolo();
	}

	// �������� ��ų� �޼���â�� ������� ������������â�� ������� �ʴ´�
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

	// �κ�â�� �������Ƿ� nk2d���� ��ο츦 �Ѵ�..
	m_MoneyText.Draw();

	if (m_pOverItem)
		return m_pOverItem;
	else
		return NULL;
}

BOOL CControlGuildSHouse::CheckMoneyStr(char* szMoney)
{
	__int64 nMoney = 0;

	// �Էµ� ��Ʈ���� �˻��Ѵ�
	int len = strlen(szMoney);

	for(int i=0; i<len; i++)
	{
		if(isdigit(szMoney[i]) == FALSE)
		{
			if(is_space(szMoney[i]) ==TRUE)
				//-- IDS_WAREHOUSE_BLANK : ���鹮�ڴ� ������� �ʽ��ϴ�.
				g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_WAREHOUSE_BLANK), TYPE_GUILD_EXCHANGE, (int)(POPUP_GUILD_MONEYEXCHANGE)  );
			else
				//-- IDS_WAREHOUSE_NOTNUM : �߸��� �ݾ��Դϴ�. �ٽ� �Է��ϼ���
				g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_WAREHOUSE_NOTNUM), TYPE_GUILD_EXCHANGE, (int)(POPUP_GUILD_MONEYEXCHANGE)  );

			return FALSE;
		}
	}

	// 100 ������ �Է��� ���� �������

	if(len >2)
	{
		if(atoi(&szMoney[len-2]) != 0  || atoi(&szMoney[len-1]) != 0)
		{
			//-- IDS_WAREHOUSE_NOT100 : �ݾ��� 100 ������ �Է��� �ּ���
			g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_WAREHOUSE_NOT100), TYPE_GUILD_EXCHANGE, (int)(POPUP_GUILD_MONEYEXCHANGE)  );
			return FALSE;
		}
	}
	else
	{
		//-- IDS_WAREHOUSE_NOT100 : �ݾ��� 100 ������ �Է��� �ּ���
		g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_WAREHOUSE_NOT100), TYPE_GUILD_EXCHANGE, (int)(POPUP_GUILD_MONEYEXCHANGE)  );
		return FALSE;
	}

	// 3. ���� ������ �ִ� �����ݾ׺��� �Էµ� �ݾ��� ��ū���
	nMoney = atoi(szMoney);

	if (nMoney < 0)
	{
		//-- IDS_WAREHOUSE_NOT0 : �Է°��� �߸��Ǿ����ϴ�. �ٽ� �Է��ϼ���
		g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_WAREHOUSE_NOT0), TYPE_GUILD_EXCHANGE, (int)(POPUP_GUILD_MONEYEXCHANGE)  );
		return FALSE;
	}

	if(m_byInOutMoneyState != 2)
	{
		if(m_pNkCha->m_Money < nMoney)
		{
			//-- IDS_WAREHOUSE_NOTENOUGH :  �ݾ��� �����մϴ�. �ٽ� �Է��ϼ���
			g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_WAREHOUSE_NOTENOUGH), TYPE_GUILD_EXCHANGE, (int)(POPUP_GUILD_MONEYEXCHANGE)  );
			return FALSE;
		}
	}
	else
	{
		if((m_nSaveMoney*100) < nMoney)
		{
			//-- IDS_WAREHOUSE_NOTENOUGH :  �ݾ��� �����մϴ�. �ٽ� �Է��ϼ���
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
				// â�� �ݴ´�.
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
					else if( GET_ATT_FLAG(m_pNkCha->m_ExtraSlot) & IATT_GUILD_NO_STASH )
					{
						g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_CANT_GSTASH_WITH_NOGSTASH), TYPE_NOR_OK, 1 );
						return 1;
					}
					else if( !(GET_ATT_FLAG(m_pNkCha->m_ExtraSlot) & IATT_GUILD_NO_STASH) )
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
					// IDS_GUILD_STASH_TAKE_FAIL_MEMBER "��� �����͸� ã�Ⱑ �����մϴ�."
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
				// â�� �ݴ´�.
				ActivateGStoreHouse(FALSE);

			g_pNk2DFrame->ShowGStoreHouseWindow(FALSE);
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
						&& x < m_wareslot_start_x + GUILD_STORE_SLOT_WIDTH*GUILD_STORE_SLOT_X_NUM
						&& y >= m_wareslot_start_y
						&& y < m_wareslot_start_y + GUILD_STORE_SLOT_HEIGHT*GUILD_STORE_SLOT_Y_NUM )
				{
					int slot_x_num, slot_y_num, slot_index;
					int nVnum;

					// ���Գѹ��� �ѿ����� ���� �ε��� ���
					slot_x_num = (x - m_wareslot_start_x) / GUILD_STORE_SLOT_WIDTH;
					slot_y_num = (y - m_wareslot_start_y) / GUILD_STORE_SLOT_HEIGHT;
					slot_index = slot_x_num * GUILD_STORE_SLOT_Y_NUM + slot_y_num;

					// ���Կ� �������� ������
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
						// WARE_PACK_INTERVAL  = 20  : �Ѱ��� �ε��� ����
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
				//-- IDS_WAREHOUSE_INMONEY :  �Ա��Ͻ� �ݾ���   �����ּ���
				g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_WAREHOUSE_INMONEY), TYPE_GUILD_EXCHANGE, (int)(POPUP_GUILD_MONEYEXCHANGE) );
				m_byInOutMoneyState = 1;
				m_InMoneyBtn.m_iBtnFlg = BTN_NORMAL;
			}
			else if(m_OutMoneyBtn.GetState() == BTN_ACTION)
			{
				if(g_pRoh->GetMyGuildData() && g_pRoh->GetMyGuildData()->GetMyInfo().s_glevel < 10 )
				{
					// IDS_GUILD_STASH_TAKE_FAIL_MEMBER "��� �����͸� ã�Ⱑ �����մϴ�."
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
				//-- IDS_WAREHOUSE_OUTMONEY : ����Ͻ� �ݾ���   �����ּ���
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
	sprintf(m_MsgBuf, "mg stash t 12 0 %s\n", money);	// �����̽� ����.
	SendMsg();
	m_byInOutMoneyState = 0;

	return true;
}

BOOL CControlGuildSHouse::OutMoneyMsg(char* money)
{
	sprintf(m_MsgBuf, "mg stash t 12 0 -%s\n", money);	// �����̽� ����.
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

// â���� ���� ������.
// â�� ����Ÿ�� �����Ѵ�.
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

	// ���Ե���Ÿ �ʱ�ȭ.
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
// â�������� Ȱ��ȭ ��Ŵ
{
	if (bActivate)
	{
		RemoveAll();
		strcpy(m_MsgBuf, "mg stash o\n");		// ������ â�� �������� �˸�

		if(gLevel >= 0 && gLevel <= 9)
			m_using_store = gLevel;
		else
			m_using_store = 0;
	}
	else
	{
		strcpy(m_MsgBuf, "mg stash c\n");		// ������ â�� �������� �˸�
		m_bGetStashEnd = FALSE;	// ��Ȱ��ȭ �Ǹ� ����
		RemoveAll();		
	}

	m_bActivate = bActivate;
	// ���ʿ��� �ҽ�
	// ���� �为
	if (m_bActivate == FALSE)
		m_bGetStashEnd = FALSE;

	// m_MsgBuf�� �޼��� ����
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
								int UpEndur, int MaxUpdEndur, int Endur, int MaxEndur, int count, int time_limit ) ///�ð��������� �ð��� �߰�(050711 by ����) ///�Ź���ȭ�ý���
{
	// ��ġ �ľ�.
	int x = 0, y = 0;
	CItem *pItem=NULL;
	int pack_num = 0;		// �ӽ�.

	pack_num = slot_num / GUILD_STORE_PACK_INTERVAL;
	slot_num = slot_num % GUILD_STORE_PACK_INTERVAL;

	if (slot_num == 12)  				// ���� ���� ���̴�.
	{
		m_nSaveMoney = plus;
		m_MoneyText.SetString((m_nSaveMoney*100), true);
		return;
	}

	x = slot_num / GUILD_STORE_SLOT_Y_NUM;
	y = slot_num % GUILD_STORE_SLOT_Y_NUM;

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

		pItem->m_Endurance = Endur;           // ���� ��ȭ ������
		pItem->m_MaxEndurance = MaxEndur;    // ��ȭ ������ �ִ�ġ

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
		if( time_limit > 0 ) // �ð� ���� ������ �ð��� ���������� ���� (by ����) ///�ð���������
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
void CControlGuildSHouse::StashTo(int slot_num, int vnum, int plus, int special, int special2,
								  int UpEndur, int MaxUpdEndur, int Endur, int MaxEndur, int count, int swap_flag) ///�Ź���ȭ�ý���
// ���� : â���Գ�, �����۳�, �÷�����, ����ȳ�, â���� ī��Ʈ, ��ȯ�÷���
// �κ� -> â��, ExtraSlot -> â�� (��ȯ�÷��� : ��ġ�� ������ 1�ϰ�� ��ȯ )
// â�� �������� �ְ� ī��Ʈ�� ���
{
	int x = 0, y = 0, pack_num = 0;
	CItem *pItem=NULL;

	// ���Գ����� �ε��� ���
	// ���Գ��� ������ ( ���� ���������� ���� ������(?)���� ���� )
	pack_num = slot_num / GUILD_STORE_PACK_INTERVAL;		// WARE_PACK_INTERVAL = 20
	slot_num = slot_num % GUILD_STORE_PACK_INTERVAL;		// �� ������ ������ 20���� �����Ǿ� ����

	// ���� ������ ��ǥ ���
	x = slot_num / GUILD_STORE_SLOT_Y_NUM;
	y = slot_num % GUILD_STORE_SLOT_Y_NUM;

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

			m_WareInven[pack_num][x][y]->m_Endurance = Endur;           // ���� ��ȭ ������
			m_WareInven[pack_num][x][y]->m_MaxEndurance = MaxEndur;    // ��ȭ ������ �ִ�ġ

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


// nate : â�� -> �κ�, â�� -> Extra Slot
// ���� : â���Գ�, �������ε���, �����۹�ȣ, �÷�����, �����, �����۰���
void CControlGuildSHouse::StashFrom(int slot_num, int index, int vnum, int plus, int &special, int special2,
									int UpEndur, int MaxUpdEndur, int Endur, int MaxEndur, int count) ///�Ź���ȭ�ý��� // �̳༮�� �� special�� & �ٿ��� ���� �޴��� �𸣰ڴ�...
{
	int x, y, pack_num;
	int special_temp = 0;
	CItem *pItem = NULL;

	// â�� ���� ���
	pack_num = slot_num / GUILD_STORE_PACK_INTERVAL;
	slot_num = slot_num % GUILD_STORE_PACK_INTERVAL;

	x = slot_num / GUILD_STORE_SLOT_Y_NUM;
	y = slot_num % GUILD_STORE_SLOT_Y_NUM;

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
		if (m_WareSlot[pack_num][x][y] == 0)
		{
			SAFE_DELETE(m_WareInven[pack_num][x][y]);
			m_WareInven[pack_num][x][y] = NULL;
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

				m_pNkCha->m_ExtraSlot->m_Endurance = Endur;           // ���� ��ȭ ������
				m_pNkCha->m_ExtraSlot->m_MaxEndurance = MaxEndur;    // ��ȭ ������ �ִ�ġ

				// ������ ���� �Է��غ���!
				// ������ �Ӽ����� �����ϴ� �Լ�, �Ʒ� �ּ��κ�
				ResetItemValue(m_pNkCha->m_ExtraSlot);
				// �������� ����Ȱ� ����
				SetSpecialValue(m_pNkCha->m_ExtraSlot, GET_TYPE(m_pNkCha->m_ExtraSlot), GET_SHAPE(m_pNkCha->m_ExtraSlot), special, special2);
			}
		}
		else  	// ������ �ٷ��̵��� ���
		{

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

				///�Ź���ȭ�ý���
				m_WareInven[pack_num][x][y]->m_Special2 = special2; // �߰� �ɼ�
				m_WareInven[pack_num][x][y]->m_UpgradeEndurance = UpEndur;           // ���� ��ȭ ������
				m_WareInven[pack_num][x][y]->m_MaxUpgradeEndurance = MaxUpdEndur;    // ��ȭ ������ �ִ�ġ


				// ������ ���� �Է��غ���!
				ResetItemValue(m_WareInven[pack_num][x][y]);
				SetSpecialValue(m_WareInven[pack_num][x][y], GET_TYPE(m_WareInven[pack_num][x][y]), GET_SHAPE(m_WareInven[pack_num][x][y]), special, special2);
			}
		}
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

// nate : â�� �� �� ��� Item ������ ���������� �˸��� �޼���
//-- 2004.07.27
//-- Lyul
//-- ���� â�� �޼��� â�� ǥ���Ѵ�.
void CControlGuildSHouse::StashEnd()
{
	m_bGetStashEnd = TRUE;
	// â�� �����찡 ������ �˸�
	if( g_pNk2DFrame )
		g_pNk2DFrame->SetShowGStoreHouseFlag(TRUE);
	// â�� �߸� ȭ�� ������ �۾���
	// by evilkiki 2009.10.01
	// pCMyApp->AdjustViewport();

	//-- ���� ��׶�̾ư� �ƴ� ��� â�� ���� ���
	if( pCMyApp->m_pUIMgr && g_nCurWorldIdx !=4  && g_nCurWorldIdx !=11)
		g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_WAREHOUSE_ALARM), TYPE_NOR_OK, 1 );

	if( g_pNk2DFrame->GetUIWindow())
		g_pNk2DFrame->GetUIWindow()->AllCloseWindow();
}

// nate : â�� ������ �ٷ��̵� ��� �޼��� ó����
// ���� : �����۹�ȣ, �κ��ѹ�ȣ, â���ѹ�ȣ, â���Թ�ȣ, �κ� X, �κ� Y
// �κ� -> â��
// ��� : �κ��� ������ ����, â�� ������ �߰�
void CControlGuildSHouse::StashPut( int nVnum, int nInvenPackNum, int nWarePackNum, int nWareSlotNum, int nSlot_X, int nSlot_Y )
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
		StashTo( nWareSlotNum, nVnum, nPlusNum, nSpecial, pItem->m_Special2, pItem->m_UpgradeEndurance, pItem->m_MaxUpgradeEndurance, pItem->m_Endurance, pItem->m_MaxEndurance, -1/* â������ ī��Ʈ*/, 1/*swap_flag*/ ); ///�Ź���ȭ�ý���
	}

	// ������ �߰��� ����
	SAFE_DELETE( pItem );
}


// nate : â�� ������ �ٷ��̵� ��� �޼��� ó����
// ���� : �����۹�ȣ, �κ��ѹ�ȣ, â���ѹ�ȣ, â���Թ�ȣ, ������ �ε�����, �κ� X, �κ� Y
// â�� -> �κ�
// ��� : �κ��� ������ �߰�, â�� ������ ����
void CControlGuildSHouse::StashGet( int nVnum, int nInvenPackNum, int nWarePackNum, int nWareSlotNum, int nItemIndex, int nSlot_X, int nSlot_Y )
{
	// �������� Interval�������� �Ѿ�� ���Գ��� �������� ���� ������ ���
	int nTempSlotNum = nWareSlotNum % GUILD_STORE_PACK_INTERVAL;

	// ���� ������ X, Y��ǥ ���
	int nX = nTempSlotNum / GUILD_STORE_SLOT_Y_NUM;
	int nY = nTempSlotNum % GUILD_STORE_SLOT_Y_NUM;

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

		// [6/7/2007 Theodoric] ������ ����
		int nEndurance = 0;
		int	nMaxEndurance = 0;

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

		nEndurance = pItem->m_Endurance;
		nMaxEndurance = pItem->m_MaxEndurance;

		// [6/7/2007 Theodoric] ������ �ý��� :: �Լ� ���� �߰�
		StashFrom( nWareSlotNum, nItemIndex, nVnum, nPlusNum/*GemNum*/, nSpecial/*special*/,nSpecial2, nUpgradeEndurance, nMaxUpgradeEndurance,  nEndurance, nMaxEndurance,-1/*Item count�� �÷��׷� ���*/ ); ///�Ź���ȭ�ý���
		if( g_pRoh )
		{
			// [6/7/2007 Theodoric] ������ �ý��� :: �Լ� ���� �߰�
			g_pRoh->AddToInven( nInvenPackNum, nSlot_X, nSlot_Y, nItemIndex, nVnum, nPlusNum/*GetNum*/, nSpecial/*special*/, nSpecial2, nUpgradeEndurance, nMaxUpgradeEndurance, nEndurance, nMaxEndurance ); ///�Ź���ȭ�ý���
		}
	}
}

LRESULT CControlGuildSHouse::MoneyPopupMsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, int nMsg, char* szMoney)
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
