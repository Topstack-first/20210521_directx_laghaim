#include "stdafx.h"


#include "main.h"
#include "ddutil.h"
#include "ItemRender.h"
#include "ItemProto.h"
#include "Nk2DFrame.h"
#include "tcpipcon.h"
#include "AlphaPopup.h"
#include "ControlBottom.h"
#include "ControlInven.h"
#include "Mouse.h"
#include "BasicButton.h"
#include "ControlExchange.h"
#include "textoutbox.h"
#include "SpecialItem.h"
#include "gamble.h"
#include "country.h"
#include "g_stringmanager.h"
#include "NkMob.h"
#include "NkPet.h"
#include "NkMyPet.h"
#include "UIMgr.h" ///Ʃ�丮�� �� �߰�

extern POINT g_CursorPoint;
extern HINSTANCE g_hDllInst;
extern DWORD g_dwClientCountry;
extern BOOL		g_bAdmin2;

extern int g_PetSkillGroup[PET_EVOLUTION_TYPENUM][PET_SKILL_MAX];


#define EXCH_TEXT_SIZE			12
#define EXCH_TEXT_COLOR			RGB(255, 0, 0)
#define EXCH_NORMAL_TEXT_COLOR  RGB(255,255,255)
#define EXCH_BAKC_COLOR         RGB(47, 0, 17)
#define LAIM_KOR_BACK_COLOR		RGB(20, 20, 20)


CControlExchange::CControlExchange()
	: m_hWnd(NULL)
	, m_ScreenWidth(0)
	, m_ScreenHeight(0)
	, m_opp_slotstart_x(0)
	, m_opp_slotstart_y(0)
	, m_my_slotstart_x(0)
	, m_my_slotstart_y(0)
	, m_cntItem(0)
	, m_bOppPetExchNote(false)
	, m_nOppNameX(0)
	, m_nOppNameY(0)
	, m_nOppLevelX(0)
	, m_nOppLevelY(0)
	, m_PetClass(0)
	, m_nPetClassTextX(0)
	, m_nPetClassTextY(0)
{
	memset(m_MsgBuf, 0, sizeof(m_MsgBuf));
	memset(m_strMoney1, 0, sizeof(m_strMoney1));
	memset(m_strMoney2, 0, sizeof(m_strMoney2));
	memset(m_nPetTextX, 0, sizeof(m_nPetTextX));
	memset(m_nPetTextY, 0, sizeof(m_nPetTextY));
	memset(m_PetSkillLevelInfo, 0, sizeof(m_PetSkillLevelInfo));

	int i, j;
	// �ֿϵ��� ���� �ʱ�ȭ
	m_bPetInfoDraw = false;
	// Test �ֿϵ��� TextBox
	ZeroMemory(m_strPetName,sizeof(m_strPetName));
	ZeroMemory(m_PetLevel,sizeof(m_PetLevel));
	ZeroMemory(m_PetHP,sizeof(m_PetHP));
	ZeroMemory(m_PetAttack,sizeof(m_PetAttack));
	ZeroMemory(m_PetDefense,sizeof(m_PetDefense));

	for(i = 0; i<5; i++)
		m_pPetInfoText[i] = NULL;
	m_pPetClassInfoText = NULL;

	m_pSurfaceBack = NULL;
	m_pEnableSlot = NULL;
	m_pDisableSlot = NULL;
	m_pExistSlot = NULL;
	m_pPetInfo = NULL;
	m_pNkCha = NULL;
	m_ExchMoney = 0;
	m_pOverItem = NULL;
	Init();

	m_bExch = FALSE;
	m_bDisable = FALSE;
	m_bTrade = FALSE;
	m_bOppTrade = FALSE;
	m_bMyPetExchNote = false;

	m_ExgOppInven = NULL;
	m_strOppName[0] = NULL;
	m_strOppLevel[0] = NULL;
	m_pOppNameBox = NULL;
	m_pOppLevelBox = NULL;

	// nate 2004 - 4 : exchange
	m_byExchLock = 0;
	m_timeExchLock = 0;

	for ( i=0; i < EXG_SLOT_X_NUM; i++)
		for ( j = 0; j < EXG_SLOT_Y_NUM; j++)
			m_ExgOppSlot[i][j] = -1;

	for( i = 0 ; i < PET_EVOLUTION_TYPENUM*PET_SKILL_MAX ; ++i )
	{
		m_pSkillIcon[i] = NULL; // ��ų�� ��������� ������. ��ų ���� ��ŭ ������ �ִ´�.
	}
	m_pSkillIconName = NULL;

	m_dwMouseClickTickTime = timeGetTime();
}

CControlExchange::~CControlExchange()
{
	DeleteRes();
	RemoveAllOppInven();

	SAFE_DELETE(m_pSurfaceBack);
	SAFE_DELETE(m_pEnableSlot);
	SAFE_DELETE(m_pDisableSlot);
	SAFE_DELETE(m_pExistSlot);
	SAFE_DELETE(m_pOppNameBox);
	SAFE_DELETE(m_pOppLevelBox);
	SAFE_DELETE(m_pPetInfo);
	for(int i = 0; i<5; i++)
		SAFE_DELETE(m_pPetInfoText[i]);
	SAFE_DELETE(m_pPetClassInfoText);
	SAFE_DELETE( m_pSkillIconName );

	m_pNkCha = NULL;
}

void CControlExchange::RemoveAllOppInven()
{
	CItem *pItem = NULL;
	CItem *nextItem = NULL;
	pItem = m_ExgOppInven;

	while (pItem)
	{
		nextItem = pItem->m_Next;
		delete pItem;
		pItem = nextItem;
	}
	m_ExgOppInven = NULL;

	int i, j;

	for ( i=0; i < EXG_SLOT_X_NUM; i++)
		for ( j = 0; j < EXG_SLOT_Y_NUM; j++)
			m_ExgOppSlot[i][j] = -1;
}

void CControlExchange::Init()
{
	m_CancelBtn.SetFileName("common/btn_close_01");
	m_ExgBtn1.SetFileName("Exchange/btn_trade");
	m_DisableBtn1.SetFileName("Exchange/btn_lock");
	m_MoneyBtn1.SetFileName("Exchange/btn_money");
	m_ExgBtn2.SetFileName("Exchange/btn_trade");
	m_DisableBtn2.SetFileName("Exchange/btn_lock");
	m_MoneyBtn2.SetFileName("Exchange/btn_money");

	m_MoneyText1.Init(12, RGB(255, 255, 0), 0, 0);
	m_MoneyText2.Init(12, RGB(255, 255, 0), 0, 0);
	m_MoneyTextKor1.Init(10, RGB(255, 255, 0), 0, 0);
	m_MoneyTextKor2.Init(10, RGB(255, 255, 0), 0, 0);
	m_MyMoneyText.Init(12, RGB(255, 255, 0), 0, 0);
}

void CControlExchange::SetVisibleMode(BOOL bExch)
{
	m_bExch = bExch;

	//-- �ѱ��� ����
//==============================================
	// nate 2004 - 7
	// �� �ѱ��� ��������?
//  if(g_dwClientCountry !=CTRY_KOR)return;
//==============================================

	m_MoneyBtn1.SetDisable(TRUE);
	m_MoneyBtn1.SetState(BTN_DISABLE);

	strcpy(m_strMoney1, "0");
	strcpy(m_strMoney2, "0");

	m_MoneyText1.SetString(_atoi64(m_strMoney1));
	m_MoneyText2.SetString(_atoi64(m_strMoney2));
	if(g_dwClientCountry == CTRY_KOR)	//-- �ѱ��� ����
	{
		m_MoneyTextKor1.SetString("(Lime)", RGB(0, 0, 0), LAIM_KOR_BACK_COLOR);
		m_MoneyTextKor2.SetString("(Lime)", RGB(0, 0, 0), LAIM_KOR_BACK_COLOR);
	}

	m_ExchMoney = 0;

	if (m_pNkCha)
		m_MyMoneyText.SetString(m_pNkCha->m_Money, true);
	else
		m_MyMoneyText.SetString(0, true);

	m_bDisable = FALSE;
	m_bTrade = FALSE;

	if (m_bExch)
	{
		m_MoneyBtn2.SetDisable(FALSE);
		m_MoneyBtn2.SetState(BTN_NORMAL);

		m_DisableBtn2.SetDisable(TRUE);		// ó�� �����ϴ� �Ŵϱ�...
		m_DisableBtn2.SetState(BTN_NORMAL);	// �ϴ� ��ȯ�Ұ� ��ư�� �Ⱦ���..

		m_ExgBtn2.SetDisable(FALSE);		// ��� ��ȯ��ư�� ���ϱ�...
		m_ExgBtn2.SetState(BTN_NORMAL);
	}
	else
	{
		m_DisableBtn1.SetDisable(TRUE);					// ���� disable ���ְ�
		m_DisableBtn1.SetState(BTN_NORMAL);				// ���¸� �ٲ۴�.

		m_ExgBtn1.SetDisable(TRUE);
		m_ExgBtn1.SetState(BTN_NORMAL);

		m_MoneyBtn2.SetDisable(TRUE);
		m_MoneyBtn2.SetState(BTN_DISABLE);

		m_DisableBtn2.SetDisable(TRUE);
		m_DisableBtn2.SetState(BTN_NORMAL);

		m_ExgBtn2.SetDisable(TRUE);
		m_ExgBtn2.SetState(BTN_NORMAL);
	}
}


void CControlExchange::LoadRes()
{
	if( !g_pDisplay )
		return;

	m_hWnd =  g_pDisplay->GetHWnd();
	GetClientRect(m_hWnd, &m_ClientRc);

	m_ScreenWidth = g_pNk2DFrame->GetClientWidth();
	m_ScreenHeight = g_pNk2DFrame->GetClientHeight();

	m_CancelBtn.LoadRes();
	m_ExgBtn1.LoadRes();
	m_DisableBtn1.LoadRes();
	m_MoneyBtn1.LoadRes();
	m_ExgBtn2.LoadRes();
	m_DisableBtn2.LoadRes();
	m_MoneyBtn2.LoadRes();

	SetVisibleMode(m_bExch);

	g_pDisplay->CreateSurfaceFromBitmap(&m_pSurfaceBack, "interface/exchange/bg_exchange.bmp");
	if( !m_pSurfaceBack )
		return;

	m_pSurfaceBack->SetColorKey( TRANS_COLOR );

	g_pDisplay->CreateSurfaceFromBitmap (&m_pPetInfo, "interface/exchange/bg_pet_exchange_c.bmp");
	m_pPetInfo->SetColorKey( TRANS_COLOR );

	g_pDisplay->CreateSurfaceFromBitmap (&m_pEnableSlot, "interface/slot/slot_enable.bmp");
	g_pDisplay->CreateSurfaceFromBitmap (&m_pDisableSlot, "interface/slot/slot_disable.bmp");
	g_pDisplay->CreateSurfaceFromBitmap (&m_pExistSlot, "interface/slot/slot_exist.bmp");


	m_pSurfaceBack->Xpos = g_pNk2DFrame->GetClientWidth() - 291 - m_pSurfaceBack->GetWidth();
	m_pSurfaceBack->Ypos = 0;

	int bgx = m_pSurfaceBack->Xpos;
	int bgy = m_pSurfaceBack->Ypos;

	m_CancelBtn.SetPosition( bgx + 205 , bgy + 5 );

	m_MoneyBtn1.SetPosition( bgx + 35 , bgy + 236 );
	m_ExgBtn1.SetPosition( bgx + 152 , bgy + 233 );

	m_MoneyBtn2.SetPosition( bgx + 35 , bgy + 265 );
	m_ExgBtn2.SetPosition( bgx + 152 , bgy + 262 );

	m_DisableBtn1.SetPosition(bgx + 152 , bgy + 233);
	m_DisableBtn2.SetPosition( bgx + 152 , bgy + 262 );


	m_MoneyText1.SetPos(bgx+65, bgy+237+7);
	m_MoneyText2.SetPos(bgx+65, bgy+266+7);

	m_MoneyTextKor1.SetPos(bgx+65, bgy+226);
	m_MoneyTextKor2.SetPos(bgx+65, bgy+255); // �ι��� ĭ
	m_MyMoneyText.SetPos(bgx+89, bgy+425+7);


	if( m_pPetInfo )
	{
		m_opp_slotstart_x = bgx + 34;
		m_pPetInfo->Xpos = 10;
		m_opp_slotstart_y = bgy + 118;
		m_pPetInfo->Ypos = 120;
	}

	m_my_slotstart_x = bgx + 34;//m_opp_slotstart_x;
	m_my_slotstart_y = bgy + 296;//m_opp_slotstart_y + 312 - 132;

	// �ŷ� ������ �̸� ��ġ ���ϱ�.
	m_nOppNameX = bgx + 110;
	m_nOppNameY = bgy + 90;

	m_nOppLevelX = bgx + 34;
	m_nOppLevelY = bgy + 90;


	// ���� ���� TextBox��ġ ���ϱ�
	if( m_pPetInfo )
	{
		m_nPetTextX[0] = m_pPetInfo->Xpos+25;//104
		m_nPetTextY[0] = m_pPetInfo->Ypos+79;

		m_nPetTextX[1] = m_pPetInfo->Xpos+144;//64
		m_nPetTextY[1] = m_pPetInfo->Ypos+79;

		m_nPetTextX[2] = m_pPetInfo->Xpos+79;//64
		m_nPetTextY[2] = m_pPetInfo->Ypos+115;

		m_nPetTextX[3] = m_pPetInfo->Xpos+79;
		m_nPetTextY[3] = m_pPetInfo->Ypos+144;

		m_nPetTextX[4] = m_pPetInfo->Xpos+79;
		m_nPetTextY[4] = m_pPetInfo->Ypos+173;

		m_nPetClassTextX = m_pPetInfo->Xpos+95;
		m_nPetClassTextY = m_pPetInfo->Ypos+35;
	}

	char plus = '+';
	char tmp[10];
	if(m_ExgOppInven)
	{
		CItem *pItem = m_ExgOppInven;
		while (pItem)
		{
			if(pItem->m_pTextOutBox)
			{
				sprintf(tmp, "%c%d",plus, pItem->m_PlusNum);
				pItem->m_pTextOutBox->SetString(tmp, EXCH_TEXT_COLOR, EXCH_BAKC_COLOR);
			}
			pItem = pItem->m_Next;
		}
	}

	// �� ��ų ������.
	for( int i = 0 ; i < PET_EVOLUTION_TYPENUM*PET_SKILL_MAX ; ++i )
	{
		// BUTTON
		m_pSkillIcon[i]	= new CBasicButton();
		if( m_pSkillIcon[i] )
		{
			char strTemp[256];
			sprintf( strTemp, "skill/pet_skill/mini_skill%03d", i );

			m_pSkillIcon[i]->SetFileName( strTemp );
			m_pSkillIcon[i]->LoadRes();
		}
	}

	m_pSkillIconName = new CAlphaPopUp;
	if( m_pSkillIconName )
	{
		m_pSkillIconName->Init(14);
		m_pSkillIconName->LoadRes( m_ScreenWidth, m_ScreenHeight );
	}

}

void CControlExchange::DeleteRes()
{
	SAFE_SURFACE_DESTROY(m_pSurfaceBack);
	SAFE_SURFACE_DESTROY(m_pEnableSlot);
	SAFE_SURFACE_DESTROY(m_pDisableSlot);
	SAFE_SURFACE_DESTROY(m_pExistSlot);
	SAFE_DELETE_RES(m_pOppNameBox);
	SAFE_DELETE_RES(m_pOppLevelBox);
	SAFE_SURFACE_DESTROY(m_pPetInfo);
	int i = 0;
	for(i = 0; i<5; i++)
		SAFE_DELETE_RES(m_pPetInfoText[i]);
	SAFE_DELETE_RES(m_pPetClassInfoText);

	m_CancelBtn.DeleteRes();
	m_ExgBtn1.DeleteRes();
	m_DisableBtn1.DeleteRes();
	m_MoneyBtn1.DeleteRes();
	m_ExgBtn2.DeleteRes();
	m_DisableBtn2.DeleteRes();
	m_MoneyBtn2.DeleteRes();
	m_MoneyText1.DeleteRes();
	m_MoneyText2.DeleteRes();
	m_MoneyTextKor1.DeleteRes();
	m_MoneyTextKor2.DeleteRes();
	m_MyMoneyText.DeleteRes();

	if(m_ExgOppInven)
	{
		CItem *pItem = m_ExgOppInven;
		while (pItem)
		{
			SAFE_DELETE(pItem->m_pTextOutBox);
			pItem = pItem->m_Next;
		}
	}

	for( i = 0 ; i < PET_EVOLUTION_TYPENUM*PET_SKILL_MAX ; ++i )
		SAFE_DELETE(m_pSkillIcon[i]);

	SAFE_DELETE( m_pSkillIconName );
}

#define GET_OPP_SLOT_X(point_x)		((point_x - m_opp_slotstart_x) / SLOT_WIDTH)
#define GET_OPP_SLOT_Y(point_y)		((point_y - m_opp_slotstart_y) / SLOT_HEIGHT)
#define GET_MY_SLOT_X(point_x)		((point_x - m_my_slotstart_x) / SLOT_WIDTH)
#define GET_MY_SLOT_Y(point_y)		((point_y - m_my_slotstart_y) / SLOT_HEIGHT)

CItem* CControlExchange::Draw()
{
	if (g_pDisplay && m_pSurfaceBack)
	{
		POINT point;
		GetCursorPos(&point);
		ScreenToClient(g_pDisplay->GetHWnd(), &point);

		g_pDisplay->Blt(m_pSurfaceBack->Xpos, m_pSurfaceBack->Ypos, m_pSurfaceBack);

		// ��ư�� �׸���.
		m_CancelBtn.Draw();
		m_MoneyBtn1.Draw();
		m_MoneyBtn2.Draw();
		m_MoneyText1.Draw(m_pSurfaceBack->Xpos+65, m_pSurfaceBack->Ypos+241 , 80);
		m_MoneyText2.Draw(m_pSurfaceBack->Xpos+65, m_pSurfaceBack->Ypos+271 , 80);
		m_MoneyTextKor1.Draw();
		m_MoneyTextKor2.Draw();
		m_MyMoneyText.Draw(m_pSurfaceBack->Xpos+89, m_pSurfaceBack->Ypos+430 , 108);

		// Disable ��ư�� ���� ���ΰ� ��ȯ��ư�� ���� ���ΰ�?
		if (!m_bExch || m_bDisable)
		{
			m_DisableBtn1.Draw();
			m_DisableBtn2.Draw();
		}
		else
		{
			m_ExgBtn1.Draw();
			m_ExgBtn2.Draw();
		}

		m_pOverItem = NULL;
		DrawMySlot();
		DrawOppSlot();

		// �ŷ� ������ �̸��� �����ش�.
		if (m_pOppNameBox )
			m_pOppNameBox->Draw(m_pSurfaceBack->Xpos+110, m_pSurfaceBack->Ypos+92,87);

		if( m_pOppLevelBox )
			m_pOppLevelBox->Draw(m_pSurfaceBack->Xpos+34, m_pSurfaceBack->Ypos+92,70);

		// ���콺�� ������ ���� �ö������� ����.
		CMultiText multiText;
		if (m_pNkCha && m_pNkCha->m_ExtraSlot)
			m_pOverItem = NULL;
		else if (m_pOverItem && pCMyApp->m_pMouse )
			pCMyApp->m_pMouse->SetMouseType(M_HOLD);
		else if (IsInside(point.x, point.y) && pCMyApp->m_pMouse)
			pCMyApp->m_pMouse->SetMouseType(M_NORMAL);
	}

	return m_pOverItem;
}

void CControlExchange::DrawMySlot()
{
	m_bMyPetExchNote = false;
	m_cntItem = 0;

	if (g_pDisplay && m_pSurfaceBack)
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
			if( g_pDisplay )
				g_pDisplay->Blt( m_my_slotstart_x + (SLOT_WIDTH*pItem->m_SlotX),
								 m_my_slotstart_y + (SLOT_HEIGHT*pItem->m_SlotY),
								 m_pExistSlot, &rcRect);
			pItem = pItem->m_Next;
		}

		// Extra Slot�� �ִ� �������� ���� �� ǥ��
		int slot_x_num, slot_y_num;
		if (m_pNkCha && m_pNkCha->m_ExtraSlot)
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

				if (m_pNkCha && g_pDisplay
						&& slot_x_num >= 0
						&& slot_x_num + GET_ITEM_WIDTH(m_pNkCha->m_ExtraSlot) - 1 < EXG_SLOT_X_NUM
						&& slot_y_num >= 0
						&& slot_y_num + GET_ITEM_HEIGHT(m_pNkCha->m_ExtraSlot) - 1 < EXG_SLOT_Y_NUM)
				{
					rcRect.left = 0;
					rcRect.right = SLOT_WIDTH * GET_ITEM_WIDTH(m_pNkCha->m_ExtraSlot);
					rcRect.top = 0;
					rcRect.bottom = SLOT_HEIGHT * GET_ITEM_HEIGHT(m_pNkCha->m_ExtraSlot);
					if (m_pNkCha->GetItemIndexInExgSlot(slot_x_num,
														slot_y_num,
														GET_ITEM_WIDTH(m_pNkCha->m_ExtraSlot),
														GET_ITEM_HEIGHT(m_pNkCha->m_ExtraSlot)) >= -1)
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
		int index = -1;
		if( m_pNkCha )
		{
			index = m_pNkCha->GetExgSlotIndex(slot_x_num, slot_y_num);
			pItem = m_pNkCha->m_ExgInven;
		}

		if( g_pNk2DFrame->GetItemRender() && pItem )
		{
			g_pNk2DFrame->GetItemRender()->BeginRenderLolo();		// �߿�..  m_pItemRender->RenderLolo�� �Ķ���Ϳ� ������ �ش�.

			while (pItem)
			{
				if (pItem->m_Index == index)
				{
					g_pNk2DFrame->RenderItemInUI( m_my_slotstart_x + pItem->m_SlotX * SLOT_WIDTH,
												  m_my_slotstart_y + pItem->m_SlotY * SLOT_HEIGHT,
												  pItem , TRUE, FALSE, FALSE, TRUE );
					m_pOverItem = pItem;
				}
				else
				{
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

					g_pNk2DFrame->RenderItemInUI( m_my_slotstart_x + pItem->m_SlotX * SLOT_WIDTH,
												  m_my_slotstart_y + pItem->m_SlotY * SLOT_HEIGHT,
												  pItem , FALSE, FALSE, FALSE, TRUE );
				}
				if(pItem->m_Vnum == 969 )//���� ��ȯ�����϶�
					m_bMyPetExchNote =  true;
				m_cntItem++;//�� ���Կ� �������� ��

				pItem = pItem->m_Next;
			}
			g_pNk2DFrame->GetItemRender()->EndRenderLolo();
		}
	}
}

void CControlExchange::DrawOppSlot() // ����� �������� �����ִ� �Լ�
{
	int xpos, ypos;
	m_bOppPetExchNote = false;

	if (g_pDisplay && m_pSurfaceBack)
	{
		RECT rcRect;
		POINT point;
		GetCursorPos(&point);
		ScreenToClient(g_pDisplay->GetHWnd(), &point);

		// �������� ��ġ�� ���� ǥ��
		CItem *pItem = m_ExgOppInven;
		if(m_pPetInfo && m_bPetInfoDraw)	//969 ���� ��ȯ�����϶�
		{
			g_pDisplay->Blt(m_pPetInfo->Xpos, m_pPetInfo->Ypos, m_pPetInfo);
			ExchPetOppInfoDraw();
		}
		else
		{
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

			int index = GetExgOppSlotIndex(slot_x_num, slot_y_num);

			pItem = m_ExgOppInven;

			if( g_pNk2DFrame->GetItemRender() && pItem )
			{
				g_pNk2DFrame->GetItemRender()->BeginRenderLolo();				// �߿�....... �߿�..
				while (pItem)
				{
					if (pItem->m_Index == index)
					{
						g_pNk2DFrame->RenderItemInUI( m_opp_slotstart_x + pItem->m_SlotX * SLOT_WIDTH,
													  m_opp_slotstart_y + pItem->m_SlotY * SLOT_HEIGHT,
													  pItem , TRUE, FALSE, FALSE, TRUE );

						m_pOverItem = pItem;
					}
					else
					{
						DWORD ambient = 0x00555555;
						if (pItem->m_PlusNum > 0)
							ambient = UPGRADE_ITEM_COLOR;

						g_pNk2DFrame->RenderItemInUI( m_opp_slotstart_x + pItem->m_SlotX * SLOT_WIDTH,
													  m_opp_slotstart_y + pItem->m_SlotY * SLOT_HEIGHT,
													  pItem , FALSE, FALSE, FALSE, TRUE );

						int cx, cy;
						xpos = m_opp_slotstart_x + (pItem->m_SlotX*SLOT_WIDTH);
						ypos = m_opp_slotstart_y + (pItem->m_SlotY*SLOT_HEIGHT);
						cx = SLOT_WIDTH; // * GET_ITEM_WIDTH(pItem);
						cy = SLOT_HEIGHT;// * GET_ITEM_HEIGHT(pItem);
						// +������ ������ ����� �����ؼ� ����Ѵ�
						if(	pItem->m_pTextOutBox)
							pItem->m_pTextOutBox->Draw(xpos+((cx/2) - (EXCH_TEXT_SIZE/2)), ypos+((cy/2) - (EXCH_TEXT_SIZE/2)));
					}
					if(pItem->m_Vnum == 969 )//���� ��ȯ�����϶�
						m_bOppPetExchNote =  true;

					pItem = pItem->m_Next;
				}
				g_pNk2DFrame->GetItemRender()->EndRenderLolo();
			}
		}
	}// ����
}

BOOL CControlExchange::IsInside(int x, int y)
{
	if( !m_pSurfaceBack )
		return false;

	return m_pSurfaceBack->IsIn(x,y);
}

BOOL CControlExchange::IsPlayArea(int x, int y)
{
	if( !IsInside(x,y) && !g_pNk2DFrame->IsInPopup(x,y) )
	{
		if( m_pPetInfo && m_pPetInfo->IsIn(x,y) )
			return false;

		return true;
	}

	return false;
}

void CControlExchange::DrawItemInfo()
{}

LRESULT CControlExchange::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static char commOutBuf[512] = "";	// ���߿� ���� �ٸ� ���� ����.
	int x = 0, y = 0;

	switch (msg)
	{
	case WM_LBUTTONUP:
	case WM_MOUSEMOVE:
		x = LOWORD (lParam);
		y = HIWORD (lParam);
		if (!IsInside(x, y))
			return 0;

		m_CancelBtn.MsgProc(hWnd, msg, wParam, lParam);
		if (m_DisableBtn2.m_bDisable != TRUE)
			m_DisableBtn2.MsgProc(hWnd, msg, wParam, lParam);
		if (m_ExgBtn2.m_bDisable != TRUE)
			m_ExgBtn2.MsgProc(hWnd, msg, wParam, lParam);
		if (m_MoneyBtn2.m_bDisable != TRUE)
			m_MoneyBtn2.MsgProc(hWnd, msg, wParam, lParam);

		// ���� ��ġ�� �ִ� ��ư  2���� ���� ó��.
		if (m_ExgBtn2.GetState() ==  BTN_ACTION)
		{
			///Ʃ�丮�� ����ó��
			if( pCMyApp->m_pUIMgr->IsEventScenePlaying() == 5 )
			{
				return 0;
			}

			// ���� �絵 �����̿��� �������� �����ϸ� ��ȯ �� �� ����
			if((m_bMyPetExchNote && m_cntItem != 1)
					|| ( m_bOppPetExchNote && m_cntItem))
			{
				// cancle
				if (m_bExch)
					ExchCancel(TRUE);

				//-- IDS_PET_TRANSFER : Pet �絵�� ������ ������ ��ȯ �����մϴ�
				g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_PET_TRANSFER), TYPE_NOR_OK, 1 );
				m_bPetInfoDraw = false;
			}
			else
			{
				if (m_bExch && !m_bDisable)	// ������� ���� �ɶ����� �޽��� ó���� ���� �ʴ´�.
				{
					// ��ȯ�� ������ �߿� ��ȯ�Ұ� �������� �ֳ�����.
					BOOL bCanExchange = TRUE;
					CItem *pItem = m_pNkCha->m_ExgInven;
					while (pItem)
					{
						if( g_bAdmin2 ) // ��ڴ� ��ȯ�Ұ� �ȸ԰� ����( by ����)
							break;

//						if( ( (GET_ATT_FLAG(pItem)&IATT_NO_EXCHANGE) && !g_bAdmin2 )
//							|| pItem->m_bTimeLimit || pItem->m_bTimeLimit /*|| pItem->m_PriceType == 2*/ ) // ��ڴ� ��ȯ�Ұ� �ȸ԰� ����( by ����) // �ð��� �������̸� ��ȯ �Ұ�ó�� (by ����) ///�ð��������� // ĳ�� ������ �Ұ�ó�� �߰�. ///�������			///07-02-20 �±����� ĳ������ �ٽ� �Ұ��Ӽ� ���ִ޶���...
						if( (GET_ATT_FLAG(pItem)&IATT_NO_EXCHANGE) || pItem->m_bTimeLimit || (g_dwClientCountry == CTRY_KOR&&pItem->m_Vnum == 1519) ) // 1519 ����ġ �������� ��� �ѱ��� �ؿ��� �Ӽ� ������ ������ �ѱ��� ��ȯ�Ұ��� ���ܷ� �����Ų��.
						{
							//-- IDS_EXCHANGE_UNABLE_ITEM : ��ȯ�� �Ұ����� ������ ���ԵǾ� �ֽ��ϴ�.
							g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_EXCHANGE_UNABLE_ITEM), TYPE_NOR_OK, 1 );
							bCanExchange = FALSE;
							m_bPetInfoDraw = false;
							break;
						}
						pItem = pItem->m_Next;
					}
					if (bCanExchange)
					{
						m_ExgBtn2.SetDisable(TRUE);
						m_ExgBtn2.SetState(BTN_DOWN);
						m_MoneyBtn2.SetDisable(TRUE);
						m_MoneyBtn2.SetState(BTN_DOWN);
						m_bTrade = TRUE;
						sprintf(m_MsgBuf, "exch trade\n");
						SendMsg();
					}
				}
			}
		}

		if (m_CancelBtn.GetState() == BTN_ACTION)
		{
			if (m_bExch)
				ExchCancel(TRUE);

			g_pNk2DFrame->ShowExchangeWindow(FALSE);
			m_CancelBtn.SetState(BTN_NORMAL);
		}
		else if (m_MoneyBtn1.GetState() == BTN_ACTION)
		{}
		else if (m_MoneyBtn2.GetState() == BTN_ACTION)
		{
			if (m_bExch && !m_bTrade && !m_bDisable)
				//-- IDS_EXCHANGE_INPUT_MONEY : �ݾ��� �����ּ���
				g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_EXCHANGE_INPUT_MONEY), TYPE_EXCHANGE, 16 );
		}
		return 1;
		break;

	case WM_RBUTTONDOWN:
		x = LOWORD (lParam);
		y = HIWORD (lParam);
		if (IsPlayArea(x, y))
		{
			if (m_bExch)
				ExchCancel(TRUE);

			g_pNk2DFrame->ShowExchangeWindow(FALSE);
			return 1;
		}
		return 1;
//====================================================
	// nate 2004 - 4
	// exchange
	case WM_LBUTTONDOWN:
		x = LOWORD (lParam);
		y = HIWORD (lParam);

		if(pCMyApp->m_bEquipLock==1)	//������� ��.1009
			return 1;
		if(pCMyApp->m_bRingLock==1)		//���� ��.0407
			return 1;
		if(pCMyApp->m_bBrLock==1)		//-- ���� ��
			return 1;
		if(pCMyApp->m_bNecklaceLock==1)		///��
			return 1;

		if (IsInside(x, y))
		{
			if( timeGetTime() - m_dwMouseClickTickTime < 300 )  // 0.5��
				return 1;

			m_dwMouseClickTickTime = timeGetTime();

			// ���� ���׷��̵� â�� ���ִ°�� ������ �־���..
			if(g_pNk2DFrame && g_pNk2DFrame->GetControlInven()->m_bAct_Masuk)
			{
				g_pNk2DFrame->GetControlInven()->Close_Masuk();
				return 1;
			}

			// ������ �ִ� �� ���� ��ư�� ���� ó���Ѵ�.
			if (m_ExgBtn2.IsInside(x, y))
				m_ExgBtn2.MsgProc(hWnd, msg, wParam, lParam);
			if (m_DisableBtn2.IsInside(x, y))
				m_DisableBtn2.MsgProc(hWnd, msg, wParam, lParam);
			if (m_CancelBtn.IsInside(x, y))
				m_CancelBtn.MsgProc(hWnd, msg, wParam, lParam);
			else if (m_MoneyBtn2.IsInside(x, y))
				m_MoneyBtn2.MsgProc(hWnd, msg, wParam, lParam);
			else if (x >= m_my_slotstart_x
					 && x < m_my_slotstart_x + (SLOT_WIDTH * EXG_SLOT_X_NUM)
					 && y >= m_my_slotstart_y
					 && y < m_my_slotstart_y + (SLOT_HEIGHT * EXG_SLOT_Y_NUM) )
			{
				int slot_x_num, slot_y_num;
				// �۾����� ������ Ÿ�� 1.5��
				// �ּ����� ������ġ
				if( m_byExchLock )
				{
					DWORD curTime = timeGetTime();

					if (curTime > m_timeExchLock + 1500) // 1.5�ʰ� ��������
						m_byExchLock = 0;
					else
						return 1;
				}

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

					if (m_pNkCha
							&& slot_x_num >= 0
							&& slot_x_num + GET_ITEM_WIDTH(m_pNkCha->m_ExtraSlot) - 1 < EXG_SLOT_X_NUM
							&& slot_y_num >= 0
							&& slot_y_num + GET_ITEM_HEIGHT(m_pNkCha->m_ExtraSlot) - 1 < EXG_SLOT_Y_NUM)
					{
						if (!m_bTrade)
						{
							// �������� ��ȯâ�� �־��ٰ� �˷��ش�.
							if (m_pNkCha && m_pNkCha->AddToExgInven(slot_x_num, slot_y_num))
							{
								sprintf(m_MsgBuf, "exch inven %d %d\n", slot_x_num, slot_y_num);
								SendMsg();
							}
						}
					}
				}
				// �������� ���� ��
				else
				{
					if (!m_bTrade)
					{
						if (m_pNkCha->AddToExgInven((x-m_my_slotstart_x) / SLOT_WIDTH, (y-m_my_slotstart_y) / SLOT_HEIGHT))
						{
							sprintf(m_MsgBuf, "exch inven %d %d\n", (x-m_my_slotstart_x) / SLOT_WIDTH, (y-m_my_slotstart_y) / SLOT_HEIGHT);
							SendMsg();
						}
					}
				}
			}
		}
		else if (g_pNk2DFrame && IsPlayArea(x, y))
		{
			if (m_bExch)
				ExchCancel(TRUE);
			g_pNk2DFrame->ShowExchangeWindow(FALSE);	// �� ���ο��� �ڵ������� ��� �޽����� ����.
		}
		else
			return 0;

		return 1;

	case WM_RBUTTONDBLCLK:
		{
			x = LOWORD( lParam );
			y = HIWORD( lParam );

			// ���� ��� �̵� �Ұ�
			if( pCMyApp->m_bEquipLock == 1 )
				return 1;
			// ���� �̵� �Ұ�
			if( pCMyApp->m_bRingLock == 1 )
				return 1;
			if(pCMyApp->m_bBrLock ==1)
				return 1;
			if(pCMyApp->m_bNecklaceLock ==1) ///��
				return 1;


			// ExtraSlot�� �������� �������
			if( m_pNkCha->m_ExtraSlot )
				return 1;

			if( m_bTrade ) // ��ȯ�� �������� �Ŀ� �̵� �Ұ� (05-12-15 ����)
				return 1;

			if( IsInside( x, y ) )
			{
				// ���� ���׷��̵� â�� ������ ���
				if( g_pNk2DFrame
						&& g_pNk2DFrame->GetControlInven()
						&& g_pNk2DFrame->GetControlInven()->m_bAct_Masuk )
				{
					g_pNk2DFrame->GetControlInven()->Close_Masuk();
					return 1;
				}

				// ���� �˻�
				if( m_pNkCha
						&& ( x >= m_my_slotstart_x )
						&& ( x < m_my_slotstart_x + ( SLOT_WIDTH * EXG_SLOT_X_NUM ) )
						&& ( y >= m_my_slotstart_y )
						&& ( y < m_my_slotstart_y + ( SLOT_HEIGHT * EXG_SLOT_Y_NUM ) ) )
				{

					int nSlot_X_Num = 0;
					int nSlot_Y_Num = 0;

					// ���� ���
					nSlot_X_Num = ( x - m_my_slotstart_x ) / SLOT_WIDTH;
					nSlot_Y_Num = ( y - m_my_slotstart_y ) / SLOT_HEIGHT;

					int nItemIndex = m_pNkCha->GetExgSlotIndex( nSlot_X_Num, nSlot_Y_Num );
					if( nItemIndex < 0 )
						return 1;

					int nVNum = 0;
					int nPlusNum = 0;
					int nSpecialNum = 0;
					int nSpecial2 = 0; // �ɼ�2
					int nNowEdr = 0; // ���� ������
					int nNowEdr2 = 0; // �ִ� ������

					CItem* pItem = m_pNkCha->m_ExgInven;
					CItem* pTempItem = pItem;

					// ���ؽ� ���� �̿��� �������� wnum ����
					while( pItem )
					{
						if( pItem->m_Index == nItemIndex )
						{
							int nWidth = GET_ITEM_WIDTH( pItem );
							int nHeight = GET_ITEM_HEIGHT( pItem );

							if( nWidth != 1 || nHeight != 1 )
								return 0;

							nVNum = pItem->m_Vnum;
							nPlusNum = pItem->m_PlusNum;
							nSpecialNum = pItem->m_Special;
							nSpecial2 = pItem->m_Special2;		// ������ ����Ȱ�
							nNowEdr = pItem->m_UpgradeEndurance;		// ������ ����Ȱ�
							nNowEdr2 = pItem->m_MaxUpgradeEndurance;		// ������ ����Ȱ�

							break;
						}
						pItem = pItem->m_Next;
					}

					// const ���� ����
					char szTmp[ 10 ] = {0,};
					char szStr[ 100 ] = {0,};

					// �۾����� ������ Ÿ�� 1.5��
					// �ּ����� ������ġ
					if( m_byExchLock )
					{
						DWORD curTime = timeGetTime();

						if (curTime > m_timeExchLock + 1500) // 1.5�ʰ� ��������
							m_byExchLock = 0;
						else
							return 1;
					}

					// �����̰� �������� �÷��� ����
					SetExchLock( 1 );

					// ������ �����۰� ���� ������ ����
					while( pTempItem )
					{
						if( ( pTempItem->m_Vnum == nVNum )
								&& ( pTempItem->m_PlusNum == nPlusNum )
								&& ( pTempItem->m_Special == nSpecialNum )
								&& ( pTempItem->m_Special2 == nSpecial2 )
								&& ( pTempItem->m_UpgradeEndurance == nNowEdr )
								&& ( pTempItem->m_MaxUpgradeEndurance == nNowEdr2 )
						  )
						{
							sprintf( szTmp, "%d %d ", pTempItem->m_SlotX, pTempItem->m_SlotY );
							strcat( szStr, szTmp );
						}
						pTempItem = pTempItem->m_Next;
					}

					sprintf( m_MsgBuf, "exch_get %d %s\n", m_pNkCha->m_CurPackNum, szStr );
					SendMsg();
				}

			}

			break;
		}
//====================================================
	}
	return 0;
}

void CControlExchange::SetNkCha(CNkCharacter *pNkCha)
{
	m_pNkCha = pNkCha;
}

void CControlExchange::SetOppName(char *player_name)
{
	strcpy(m_strOppName, player_name);
	SAFE_DELETE(m_pOppNameBox);

	m_pOppNameBox = new CTextOutBox();
	if( m_pOppNameBox )
	{
		m_pOppNameBox->Init(12, RGB(255, 210, 0),0, 0);
		m_pOppNameBox->SetPos(m_nOppNameX, m_nOppNameY);

		if( g_dwClientCountry == CTRY_TWN || g_dwClientCountry == CTRY_CN )
			m_pOppNameBox->SetString(m_strOppName, RGB(255, 210, 0), RGB(0, 0, 160));
		else
			m_pOppNameBox->SetString(m_strOppName);
	}
}

void CControlExchange::SetOppLevel(int nPlayer_level)
{
	// nate 2005-06-14 : ��ȯ�� ���� ��� ���
	if( nPlayer_level >= 1 && nPlayer_level <= 9 )
	{
		char szTemp[ 10 ];
		sprintf( szTemp, "%d", nPlayer_level );
		strcpy( m_strOppLevel, szTemp );
	}
	else if( nPlayer_level >= 10 && nPlayer_level <= 99 )
		strcpy(m_strOppLevel, "10 ~ 99");
	else if( nPlayer_level >= 100 && nPlayer_level <= 199 )
		strcpy(m_strOppLevel, "100 ~ 199");
	else if( nPlayer_level >= 200 && nPlayer_level <= 299 )
		strcpy(m_strOppLevel, "200 ~ 299");
	else if( nPlayer_level >= 300 )
		strcpy(m_strOppLevel, "300 ~");

	SAFE_DELETE( m_pOppLevelBox );

	m_pOppLevelBox = new CTextOutBox();
	if( m_pOppLevelBox )
	{
		m_pOppLevelBox->Init(12, RGB(255, 210, 0),0, 0);
		m_pOppLevelBox->SetPos( m_nOppLevelX, m_nOppLevelY );

		if( g_dwClientCountry == CTRY_TWN || g_dwClientCountry == CTRY_CN )
			m_pOppLevelBox->SetString( m_strOppLevel, RGB( 255, 210, 0 ), RGB( 0, 0, 160 ));
		else
			m_pOppLevelBox->SetString( m_strOppLevel );
	}
}

void CControlExchange::SendMsg()
{
	if (g_pTcpIp)
		g_pTcpIp->SendNetMessage(m_MsgBuf);
}

// �ŷ��� Ȱ��/��Ȱ�� ���·� �����.
// �ŷ����϶� ��Ȱ�� ���·� ���ϸ� �ŷ��� �����Ѵ�.
void CControlExchange::ActivateExch(BOOL bExch)
{
	if (m_bExch && bExch == FALSE)
	{
		ExchCancel(TRUE);
		return;
	}
	else
	{
		m_bExch = bExch;
		SetVisibleMode(bExch);
	}
}

void CControlExchange::ExchTrade()
{
	m_bOppTrade = TRUE;
	m_ExgBtn1.SetDisable(TRUE);
	m_ExgBtn1.SetState(BTN_DOWN);
}

void CControlExchange::ExchDisable()
{
	m_bDisable = TRUE;
	m_bTrade = FALSE;
	m_bPetInfoDraw = FALSE;
}


void CControlExchange::ExchInvenTo(int x, int y, int index, int vnum, int gem_num, int special, int special2,
								   int UpEndur, int MaxUpdEndur, int Endur, int MaxEndur) ///�Ź���ȭ�ý���
{
	char plus = '+';
	char tmp[10];
	if (vnum < 0)
		return;

	int proto_num = g_ItemList.FindItem(vnum);
	if (proto_num >= 0)
	{
		CItem *pItem = new CItem;
		if( !pItem )
			return;

		pItem->m_Index = index;
		pItem->m_Vnum = vnum;
		pItem->m_ProtoNum = proto_num;

		// ������ ���� �Է��غ���!
		ResetItemValue(pItem);
		// +�������� ��� CTextOutBox ����
		if (gem_num > 0)
		{
			if(GET_TYPE(pItem)==ITYPE_SUB )	//1107	0414 ������
				pItem->m_PlusNum = gem_num;
			else
			{
				// nate 2004 - 4
				// ������ �÷����� ���� ����250���� 300���� ����
				// �ִ밪 300�� ������ �ʿ䰡 ������ ������..
				// �ʿ��ϸ� ���߿� �߰��ص� �����ҵ�..
				//GET_ITEM_LEVEL(pItem)
				if(g_ItemList.m_ItemArray[pItem->m_ProtoNum].m_MinLevel>=181 ) //&& g_ItemList.m_ItemArray[pItem->m_ProtoNum].m_MinLevel <=300)
					pItem->SetDiaGemNum(gem_num);
				else
					pItem->SetGemNum(gem_num);
			}

			if(pItem->m_pTextOutBox)
				SAFE_DELETE(pItem->m_pTextOutBox);

			pItem->m_pTextOutBox = new CTextOutBox();
			if( pItem->m_pTextOutBox )
			{
				pItem->m_pTextOutBox->Init(EXCH_TEXT_SIZE, EXCH_TEXT_COLOR, 0, 0);

				if(GET_TYPE(pItem)!=ITYPE_SUB
						&& !(pItem->m_Vnum == 1087
							 || pItem->m_Vnum == 922
							 || pItem->m_Vnum == 933
							 || pItem->m_Vnum == 934
							 || pItem->m_Vnum == 935))
				{
					sprintf(tmp, "%c%d",plus, pItem->m_PlusNum);
					pItem->m_pTextOutBox->SetString(tmp, EXCH_TEXT_COLOR, EXCH_BAKC_COLOR);
				}
			}
		}
		// �� �ڸ��� ���� ������ ���ش�.
		ExchInvenFrom(x, y, GET_ITEM_WIDTH(pItem), GET_ITEM_HEIGHT(pItem));

		pItem->m_SlotX = x;
		pItem->m_SlotY = y;

		pItem->m_Special = special;

		///�Ź���ȭ�ý���
		pItem->m_Special2 = special2; // �߰� �ɼ�
		pItem->m_UpgradeEndurance = UpEndur;           // ���� ��ȭ ������
		pItem->m_MaxUpgradeEndurance = MaxUpdEndur;    // ��ȭ ������ �ִ�ġ

		pItem->m_Endurance = Endur;
		pItem->m_MaxEndurance = MaxEndur;

		SetSpecialValue(pItem, GET_TYPE(pItem), GET_SHAPE(pItem), special, special2);

		pItem->m_Next = m_ExgOppInven;
		m_ExgOppInven = pItem;

		for (int x_idx = 0; x_idx < GET_ITEM_WIDTH(pItem); x_idx++)
		{
			for (int y_idx = 0; y_idx < GET_ITEM_HEIGHT(pItem); y_idx++)
				m_ExgOppSlot[x+x_idx][y+y_idx] = index;
		}
	}

}

int CControlExchange::GetExgOppSlotIndex(int x, int y)
{
	if (x < 0 || x >= EXG_SLOT_X_NUM || y < 0 || y >= EXG_SLOT_Y_NUM)
		return -1;

	return m_ExgOppSlot[x][y];
}

//--------------------------------------------------------------------------
// ����� ��ȯâ �κп��� �������� ���ٶ�.
//--------------------------------------------------------------------------
int CControlExchange::GetItemIndexInOppSlot(int x, int y, int w, int h)
{
	int index = -1;

	for (int x_idx = x; x_idx < x+w; x_idx++)
	{
		for (int y_idx = y; y_idx < y+h; y_idx++)
		{
			if (m_ExgOppSlot[x_idx][y_idx] != -1)
			{
				if (index == -1)
					index = m_ExgOppSlot[x_idx][y_idx];
				else if (index != m_ExgOppSlot[x_idx][y_idx])
					return -2;
			}
		}
	}

	return index;
}

CItem * CControlExchange::PickItemInOppSlot(int index, BOOL isVnum)
{
	CItem *beforeItem = NULL;
	CItem *pItem = m_ExgOppInven;

	while (pItem)
	{
		if ((!isVnum && pItem->m_Index == index) || (isVnum && pItem->m_Vnum == index))
		{
			if (beforeItem)
				beforeItem->m_Next = pItem->m_Next;
			else
				m_ExgOppInven = pItem->m_Next;

			for (int x_idx = 0; x_idx < GET_ITEM_WIDTH(pItem); x_idx++)
			{
				for (int y_idx = 0; y_idx < GET_ITEM_HEIGHT(pItem); y_idx++)
					m_ExgOppSlot[pItem->m_SlotX+x_idx][pItem->m_SlotY+y_idx] = -1;
			}
			pItem->m_Next = NULL;
			break;
		}
		beforeItem = pItem;
		pItem = pItem->m_Next;
	}

	return pItem;
}

void CControlExchange::ExchInvenFrom(int x, int y, int w, int h)
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

void CControlExchange::ExchChange()
{
	// ���⼭ ���� ��ȯ �϶� â�� �ݴ´�.
	if( !g_pRoh )
		return;

	int k = 0;
	if (m_bTrade)
	{
		m_MyMoneyText.SetString(m_pNkCha->m_Money, true);

		// ������ ����Ʈ�� �ٲ�ġ�� �Ѵ�.
		CItem *pItem;

		pItem = g_pRoh->m_ExgInven;
		g_pRoh->m_ExgInven = m_ExgOppInven;
		m_ExgOppInven = pItem;

		// �������� ���� ��ġ�� ���� ������ �ٲ۴�.
		int i, j;

		for (i=0; i < EXG_SLOT_X_NUM; i++)
		{
			for (j=0; j < EXG_SLOT_Y_NUM; j++)
				g_pRoh->m_ExgSlot[i][j] = m_ExgOppSlot[i][j];
		}

		// ������� �� ����Ʈ�� ����������.
		RemoveAllOppInven();
		//�ŷ��� ����ǰ� �Ϲ� ���� ���µ� ���·� �÷��׸� ����.
		m_bExch = FALSE;
		// ��Ʈ��� �÷��� ��ȭ.
		SetVisibleMode(FALSE);
	}
}

// ������� �ŷ��� ����߰ų� ���� �ŷ��� ����� ���.
void CControlExchange::ExchCancel(BOOL bSend)
{
	// �ŷ��� �ݴ´�.
	// ��ȯâ�� �Ϲ� ���� �ٽ� ȣ���Ѵ�.
	RemoveAllOppInven();

	if (bSend && m_bExch)
	{
		sprintf(m_MsgBuf, "exch cancel\n");
		SendMsg();
	}

	m_bExch = FALSE;
	m_bPetInfoDraw = FALSE;
	// ��ư�� ���� ����.
	SetVisibleMode(FALSE);
}

// �ŷ� ������� ��ȯ�ݾ�.
void CControlExchange::ExchMoney(char *strMoney)
{
	if (strlen(strMoney) < 11)
	{
		strcpy(m_strMoney1, strMoney);
		m_MoneyText1.SetString(_atoi64(m_strMoney1), true);
		if(g_dwClientCountry ==CTRY_KOR)	//-- �ѱ��� ����
		{
			INT64 curMoney = _atoi64(m_strMoney1);
			int hundred_million = 0;
			int ten_thousand = 0;
			INT64 laim = 0;
			char strMoneyText[255];

			hundred_million = curMoney / 100000000;
			ten_thousand = (curMoney-(hundred_million*100000000)) / 10000;
			laim = curMoney-(hundred_million*100000000)-(ten_thousand*10000);

			strcpy(strMoneyText, "(");

			if( hundred_million )
				sprintf(strMoneyText, G_STRING(IDS_LHSTRING1737), hundred_million);
			if( ten_thousand )
				sprintf(strMoneyText, G_STRING(IDS_LHSTRING1738), strMoneyText, ten_thousand);
			if( laim )
				sprintf(strMoneyText, G_STRING(IDS_LHSTRING1739), strMoneyText, laim);

			strcat(strMoneyText, G_STRING(IDS_LHSTRING1740) );

			m_MoneyTextKor1.SetString(strMoneyText, RGB(0, 0, 0), LAIM_KOR_BACK_COLOR);
		}
	}
}

//----------------------------------------------------------------------------
//-- Check Exchange
//----------------------------------------------------------------------------
//-- ��ȯ �ݾ� ���Լ� üũ
//-- 2004.03.06 - �����Ϸ�
//-- Lyul
//-- Release Version ���� isdigit �� �ѱ� ���� �Է¿� ���� üũ�� ���� ���Ѵ�.
//-- !! �Է��� ASCII �� ���� �ȴٸ� �ܼ��� ASCII �� ���� ������ CHECK�� ����
//-- 0000.00.00 - ??????
//-- name / description
BOOL CControlExchange::CheckMoneyStr(char* szMoney)
{
	INT64 nMoney;

	// �Էµ� ��Ʈ���� �˻��Ѵ�
	int len = strlen(szMoney);
	for(int i=0; i<len; i++)
	{
		//-- �̳༮�� ������.
		if(isdigit(szMoney[i]) == FALSE
				|| (szMoney[i])<0x00)
		{
			if(is_space(szMoney[i]) ==TRUE)
				//-- IDS_EXCHANGE_UNABLE_BLANK : ������ ���Ե� �� �����ϴ�
				g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_EXCHANGE_UNABLE_BLANK), TYPE_EXCHANGE, 16 );
			else
				//-- IDS_EXCHANGE_UNABLE_STRING : ���ڸ� �Է����ּ���
				g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_EXCHANGE_UNABLE_STRING), TYPE_EXCHANGE, 16 );

			return FALSE;
		}
	}
	// 3. ���� ������ �ִ� �����ݾ׺��� �Էµ� �ݾ��� ��ū���
	nMoney = _atoi64(szMoney);

	if (nMoney < 0)
	{
		//-- IDS_EXCHANGE_FAULT_STR : ������ �ʴ� ���Դϴ�
		g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_EXCHANGE_FAULT_STR), TYPE_EXCHANGE, 16 );
		return FALSE;
	}

	if(m_pNkCha->m_Money < nMoney)
	{
		//-- IDS_EXCHANGE_SCARCE_MONEY : �ݾ��� �����մϴ�.
		g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_EXCHANGE_SCARCE_MONEY), TYPE_EXCHANGE, 16 );
		return FALSE;
	}

	return TRUE;
}

void CControlExchange::ExchPetOppInfoDraw()
{
	int i = 0;
	for(i = 0; i<5; i++)
	{
		if(m_pPetInfoText[i])
		{
			if( i == 0 )	m_pPetInfoText[i]->Draw(m_nPetTextX[i], m_nPetTextY[i], 104);
			else			m_pPetInfoText[i]->Draw(m_nPetTextX[i], m_nPetTextY[i], 64);
		}
	}
	if( m_pPetClassInfoText )
		m_pPetClassInfoText->Draw();

	// ��ų �������� �׷��ش�.
	if( m_PetClass ) // �⺻���� ��ų�� ����.
	{
		int SkillIconDrawCount = 0;
		int SkillIndex = 0; // ������ �ְ� �޴� ��ų �ε���.
		int MouseOnSkillIndex = -1; // ���콺�� �÷����ִ� ��ų�� �ε���.

		POINT point;
		GetCursorPos(&point);
		ScreenToClient(g_pDisplay->GetHWnd(), &point);
		CMultiText multiText;

		for( i = 0 ; i < PET_SKILL_MAX ; ++i ) // �ڱ� ���� �����ִ� ��ų ���� ��ŭ.
		{
			SkillIndex = g_PetSkillGroup[m_PetClass-1][i];
			m_pSkillIcon[SkillIndex]->Draw( m_pPetInfo->Xpos+78+(SkillIconDrawCount*SIZE_OF_SKILLMINICON), m_pPetInfo->Ypos+196, !m_PetSkillLevelInfo[i] ); // ��ų�� ��������� ������. ��ų ���� ��ŭ ������ �ִ´�.
			++SkillIconDrawCount;

			// ���콺 �� üũ.
			if( m_pSkillIconName )
			{
				if( m_pSkillIcon[SkillIndex]
						&& (point.x >= m_pSkillIcon[SkillIndex]->PosX && point.x < m_pSkillIcon[SkillIndex]->PosX + m_pSkillIcon[SkillIndex]->Width)
						&& (point.y >= m_pSkillIcon[SkillIndex]->PosY && point.y < m_pSkillIcon[SkillIndex]->PosY + m_pSkillIcon[SkillIndex]->Height) )
				{
					MouseOnSkillIndex = SkillIndex;
				}
			}
		}

		// �������� �� �׸��� �̸� �˾��� �׷��� ���� �׷�����.
		if( MouseOnSkillIndex != -1 ) // �ö󰣰� ������...
		{
			if( m_pSkillIconName->GetIndex() != MouseOnSkillIndex ) // ���� ������ �ƴҶ���...
			{
				char strTemp[50];

				//	by evilkiki 2008.08.04
				//	return value ���輺���� �Լ� ����
				GetPetSkillName(MouseOnSkillIndex , strTemp);
				//strncpy( strTemp, GetPetSkillName(MouseOnSkillIndex), 50 );

				multiText.AddString( strTemp, RGB(255, 255, 255) );
				m_pSkillIconName->PutString( &multiText );
				m_pSkillIconName->SetIndex( MouseOnSkillIndex );
			}
			m_pSkillIconName->Draw( point.x, point.y );
		}
	}
}

void CControlExchange::SetExchPetInfo(char *PetName, char* PetLevel, char* PetHP, char* PetAttack, char* PetDefense, int PetClass, int Skill1_Level, int Skill2_Level, int Skill3_Level, int Skill4_Level )
{
	char* Petinfo = NULL ;

	if(!PetName||!PetLevel || !PetHP || !PetAttack || !PetDefense)
	{
		m_bPetInfoDraw = false;
		return;
	}
	else
	{
		m_bPetInfoDraw = true;
		ZeroMemory(m_strPetName,sizeof(m_strPetName));
		strcpy(m_strPetName, PetName);
		ZeroMemory(m_PetLevel,sizeof(m_PetLevel));
		strcpy(m_PetLevel, PetLevel);
		ZeroMemory(m_PetHP,sizeof(m_PetHP));
		strcpy(m_PetHP, PetHP);
		ZeroMemory(m_PetAttack,sizeof(m_PetAttack));
		strcpy(m_PetAttack, PetAttack);
		ZeroMemory(m_PetDefense,sizeof(m_PetDefense));
		strcpy(m_PetDefense, PetDefense);

		m_PetClass = 0; // �ӽ÷�.
		m_PetSkillLevelInfo[0] = 0;
		m_PetSkillLevelInfo[1] = 0;
		m_PetSkillLevelInfo[2] = 0;
		m_PetSkillLevelInfo[3] = 0;
	}

	for(int i = 0; i<5; i++)
	{
		SAFE_DELETE(m_pPetInfoText[i]);
		m_pPetInfoText[i] = new CTextOutBox();
		if( m_pPetInfoText[i] )
		{
			m_pPetInfoText[i]->Init(12, RGB(255, 255, 0),0, 0, -1,-1, true);
			m_pPetInfoText[i]->SetVisible(TRUE);
			switch(i)
			{
			case 0:
				Petinfo = m_strPetName;
				break;
			case 1:
				Petinfo = m_PetLevel;
				break;
			case 2:
				Petinfo = m_PetHP;
				break;
			case 3:
				Petinfo = m_PetAttack;
				break;
			case 4:
				Petinfo = m_PetDefense;
				break;
			}
			m_pPetInfoText[i]->SetString(Petinfo);
			m_pPetInfoText[i]->SetPos(m_nPetTextX[i], m_nPetTextY[i]);
		}
	}

	// Ŭ���� ��Ʈ�� ����.
	m_PetClass = PetClass;
	SAFE_DELETE( m_pPetClassInfoText );
	m_pPetClassInfoText = new CTextOutBox();
	if( m_pPetClassInfoText )
	{
		char strTemp[50];
		char strTemp2[50];

		m_pPetClassInfoText->Init(12, RGB(255, 255, 0),0, 0);
		switch( m_PetClass )
		{
		case 0:
			// IDS_BASIC_TYPE : �⺻��
			strcpy( strTemp, (char *)G_STRING(IDS_BASIC_TYPE) );
			break;
		case 1:
			// IDS_DEFENCE_TYPE : �����
			strcpy( strTemp, (char *)G_STRING(IDS_DEFENCE_TYPE) );
			break;
		case 2:
			// IDS_ATTACK_TYPE : ������
			strcpy( strTemp, (char *)G_STRING(IDS_ATTACK_TYPE) );
			break;
		default:
			strcpy( strTemp, "-" );
			break;
		}
		sprintf( strTemp2, "[%s]", strTemp );
		m_pPetClassInfoText->SetString( strTemp2 );
		m_pPetClassInfoText->SetPos( m_nPetClassTextX, m_nPetClassTextY );
	}

	m_PetSkillLevelInfo[0] = Skill1_Level;
	m_PetSkillLevelInfo[1] = Skill2_Level;
	m_PetSkillLevelInfo[2] = Skill3_Level;
	m_PetSkillLevelInfo[3] = Skill4_Level;
}

void CControlExchange::ExchangedPetClear()
{
	// �ֿϵ����� ������ �����
	//�ڽ��� ����â�� �絵 ���� Clear/ ?
	CItem *pItem = NULL;
	CItem *nextItem = NULL;
	pItem = g_pRoh->m_ExgInven;
	while (pItem)
	{
		nextItem = pItem->m_Next;
		delete pItem;
		pItem = nextItem;
	}
	m_pNkCha->m_ExgInven = NULL;

	for (int x_idx = 0; x_idx < EXG_SLOT_X_NUM; x_idx++)
	{
		for (int y_idx = 0; y_idx < EXG_SLOT_Y_NUM; y_idx++)
			g_pRoh->m_ExgSlot[x_idx][y_idx] = -1;
	}
}

void CControlExchange::ExchPetEnd()
{
	m_bPetInfoDraw = false;
	m_bOppPetExchNote = false;
	m_bMyPetExchNote = false;
	g_pNk2DFrame->ShowExchangeWindow(FALSE);
}

// nate 2004 - 4 : exchange
void CControlExchange::SetExchLock( BYTE byExchLock )
{
	if( m_byExchLock )
		m_timeExchLock += 1500;
	else
	{
		m_byExchLock = TRUE;
		m_timeExchLock = timeGetTime();
	}
}

void CControlExchange::AskExchange(char *player_name)
{
	char strTemp[128] = "";
	//-- IDS_NK2D_REQUIRE_TRADE : %s ���� �ŷ���û�� �߽��ϴ�
	sprintf(strTemp, (char*)G_STRING(IDS_NK2D_REQUIRE_TRADE), player_name);
	g_pNk2DFrame->InsertPopup(strTemp,TYPE_NOR_OKCANCLE, 10);
}

LRESULT CControlExchange::PopupMsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, int nMsg)
{
	if( nMsg == 1)	// Ȯ�ι�ư�� �������
	{
		// �ŷ� ��û�� �³��ϴ� �޽����� ������.
		if (g_pTcpIp)
			g_pTcpIp->SendNetMessage("exch ok\n");
	}
	else if(nMsg == -1) // ��ҹ�ư�� �������
	{
		// �ŷ� ��û�� �����ϴ� �޽����� ������.
		if (g_pTcpIp)
			g_pTcpIp->SendNetMessage("exch cancel\n");
	}

	return 0L;
}

LRESULT CControlExchange::MoneyPopupMsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, int nMsg, char* szMoney)
{
	// "Ȯ��" ��ư�� �������
	if(nMsg == 1)
	{
		if(CheckMoneyStr(szMoney) == TRUE)
		{
			strcpy(m_strMoney2, szMoney);
			__int64 money = _atoi64(m_strMoney2);

			if(money > m_ExchMoney)
			{
				m_MoneyText2.SetString(money, true);

				if(g_dwClientCountry ==CTRY_KOR)	//-- �ѱ��� ����, ���� ȯ��
				{
					__int64 curMoney = money;
					int hundred_million = 0;
					int ten_thousand = 0;
					__int64 laim = 0;
					char strMoneyText[255];

					hundred_million = curMoney / 100000000;
					ten_thousand = (curMoney-(hundred_million*100000000)) / 10000;
					laim = curMoney-(hundred_million*100000000)-(ten_thousand*10000);

					strcpy(strMoneyText, "(");

					if( hundred_million )
						sprintf(strMoneyText, G_STRING(IDS_LHSTRING1737), hundred_million);
					if( ten_thousand )
						sprintf(strMoneyText, G_STRING(IDS_LHSTRING1738), strMoneyText, ten_thousand);
					if( laim )
						sprintf(strMoneyText, G_STRING(IDS_LHSTRING1739), strMoneyText, laim);

					strcat(strMoneyText, G_STRING(IDS_LHSTRING1740));

					m_MoneyTextKor2.SetString(strMoneyText, RGB(0, 0, 0), LAIM_KOR_BACK_COLOR);
				}

				// ����ٰ� ���� ��� ������.
				if (m_bExch)
				{
					sprintf(m_MsgBuf, "exch money %s\n",m_strMoney2);	// �����̽� ����.
					SendMsg();
					m_ExchMoney = money;
				}
			}
		}
	}
	else if (nMsg == -1)	// "���" ��ư�� �������
		m_bPetInfoDraw = false;	//����

	return 0L;
}

HRESULT CControlExchange::RestoreSurfaces()
{
	ActivateExch(FALSE);

	return S_OK;
}
/////////////////////////////////////////////////////////////////////////////////////
// �۷ι� �Լ�
int GetFindPoint( char *SrcString, char FindChar ) // ��Ʈ���󿡼� Ư�� ������ ��ġ�� ã�� �Լ�.
{
	int StrLen = strlen( SrcString );

	if( !SrcString )
		return -1;

	for( int i = 0 ; i < StrLen ; ++i )
	{
		if( SrcString[i] == FindChar )
			return i;
	}

	return -1;
}

int FindStringInString( char *SrcSting, char *FindString ) // ��Ʈ�� �ȿ��� Ư�� ��Ʈ���� ��ġ�� ã���ִ� �༮
{
	if( !SrcSting || !FindString )
		return -1;

	int SrcLen = strlen( SrcSting );
	int FindLen = strlen( FindString );

	if( FindLen > SrcLen )
		return -1;

	for( int j = 0 ; j <= SrcLen-FindLen ; ++j )
	{
		if( strncmp(&SrcSting[j],FindString,FindLen) == 0 ) // ���� �κ��� ������...
		{
			return j;
		}
	}

	return -1;
}

int InsertPriceComma( char *DestString, char *SrcString, char FindChar ) // ���� ��Ʈ������ ���� �ο� �޸��� �־��ִ� �Լ�.
{
	if( !DestString || !SrcString )
		return -1;

	char StrTemp[256];
	char StrTemp2[256];
	int StrLen;

	int ConvStartPos = GetFindPoint( SrcString, FindChar ); // Ư�� ��ġ�� ���

	sscanf( &SrcString[ConvStartPos+1], "%s", StrTemp ); // Ư�� ��ġ ������ �ܾ ��Ʈ������ ���´�.
	StrLen = strlen( StrTemp );

	int AddByte = (StrLen-1)/3; // �־�� �ϴ� �޸��� ����.

	int i = StrLen-1, j = 0, k = StrLen-1+AddByte;

	if( StrLen+AddByte > 255 )
		return -1;

	StrTemp[i+1] = '\0';
	StrTemp2[k+1] = '\0';

	ConvStartPos = FindStringInString( SrcString, StrTemp ); // ���ϴ� ���ںκ��� ���۵Ǵ� ��Ȯ�� ��ġ�� ���´�.

	if( ConvStartPos == -1 ) // ���� �߲�.
		return -1;


	strncpy( DestString, SrcString, ConvStartPos );	// ġȯ�� �κ��� �պκ��� ����.
	DestString[ConvStartPos] = '\0';

	while( i >= 0 )
	{
		if( j != 0 && j % 3 == 0 ) // �޸��� �����Ҷ�...
		{
			StrTemp2[k] = ',';
			--k; // k�� �ϳ��� ����.
		}

		StrTemp2[k] = StrTemp[i];

		--i;
		++j;
		--k;
	}

	strncpy( &DestString[ConvStartPos], StrTemp2, StrLen+AddByte );	// ġȯ �κ��� ����(�޸� ���̵� �����ؼ�...)

	strcpy( &DestString[ConvStartPos+StrLen+AddByte], &SrcString[ConvStartPos+StrLen] );  // ���� �κ� ���� ����.

	return ConvStartPos; // ġȯ��(���ں�)�� ��ġ�� ����
}

char *ConvertMoneyToString( char *DestString, __int64 Money, int SpaceSize, int Type ) // �� �ݾ��� ������ ���� �ٲ��ִ� �Լ�.
{
	if( !DestString && SpaceSize < 0 )
		return NULL;

	__int64 curMoney = Money;
	int hundred_million = 0;
	int ten_thousand = 0;
	__int64 laim = 0;
	char strMoneyText[255];

	hundred_million = Money / 100000000;
	ten_thousand = (Money-(hundred_million*100000000)) / 10000;
	laim = Money-(hundred_million*100000000)-(ten_thousand*10000);

	strcpy(strMoneyText, "(");

	if( hundred_million )
		//-- IDS_MONEY2STRING1 : (%d��
		sprintf(strMoneyText, (char*)G_STRING(IDS_MONEY2STRING1), hundred_million);
	if( ten_thousand )
		//-- IDS_MONEY2STRING2 : %s%d��
		sprintf(strMoneyText, (char*)G_STRING(IDS_MONEY2STRING2), strMoneyText, ten_thousand);
	if( laim )
		sprintf(strMoneyText, "%s%I64d", strMoneyText, laim);

	if( Type == 0 )
		//-- IDS_MONEY2STRING3 :  ����)
		strcat(strMoneyText, (char*)G_STRING(IDS_MONEY2STRING3) );
	else if( Type == 1 )
		strcat(strMoneyText, " BP)");
	else if( Type == 3 )
		strcat(strMoneyText, " LP)");

	int StrLen = strlen( strMoneyText );

	int i = 0;
	int j = SpaceSize;

	while( j > 0 )
	{
		DestString[i] = ' ';
		++i;
		--j;
	}

	strncpy( &DestString[i], strMoneyText, StrLen );

	DestString[StrLen+SpaceSize] = '\0';

	return DestString;
}
