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
#include "UIMgr.h" ///튜토리얼 시 추가

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
	// 애완동물 관련 초기화
	m_bPetInfoDraw = false;
	// Test 애완동물 TextBox
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
		m_pSkillIcon[i] = NULL; // 스킬을 배웠는지의 아이콘. 스킬 갯수 만큼 가지고 있는다.
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

	//-- 한국만 지원
//==============================================
	// nate 2004 - 7
	// 왜 한국만 지원하지?
//  if(g_dwClientCountry !=CTRY_KOR)return;
//==============================================

	m_MoneyBtn1.SetDisable(TRUE);
	m_MoneyBtn1.SetState(BTN_DISABLE);

	strcpy(m_strMoney1, "0");
	strcpy(m_strMoney2, "0");

	m_MoneyText1.SetString(_atoi64(m_strMoney1));
	m_MoneyText2.SetString(_atoi64(m_strMoney2));
	if(g_dwClientCountry == CTRY_KOR)	//-- 한국만 지원
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

		m_DisableBtn2.SetDisable(TRUE);		// 처음 시작하는 거니까...
		m_DisableBtn2.SetState(BTN_NORMAL);	// 일단 교환불가 버튼은 안쓰지..

		m_ExgBtn2.SetDisable(FALSE);		// 대신 교환버튼은 쓰니까...
		m_ExgBtn2.SetState(BTN_NORMAL);
	}
	else
	{
		m_DisableBtn1.SetDisable(TRUE);					// 먼저 disable 해주고
		m_DisableBtn1.SetState(BTN_NORMAL);				// 상태를 바꾼다.

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
	m_MoneyTextKor2.SetPos(bgx+65, bgy+255); // 두번재 칸
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

	// 거래 상대방의 이름 위치 정하기.
	m_nOppNameX = bgx + 110;
	m_nOppNameY = bgy + 90;

	m_nOppLevelX = bgx + 34;
	m_nOppLevelY = bgy + 90;


	// 라비용 정보 TextBox위치 정하기
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

	// 펫 스킬 아이콘.
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

		// 버튼들 그리기.
		m_CancelBtn.Draw();
		m_MoneyBtn1.Draw();
		m_MoneyBtn2.Draw();
		m_MoneyText1.Draw(m_pSurfaceBack->Xpos+65, m_pSurfaceBack->Ypos+241 , 80);
		m_MoneyText2.Draw(m_pSurfaceBack->Xpos+65, m_pSurfaceBack->Ypos+271 , 80);
		m_MoneyTextKor1.Draw();
		m_MoneyTextKor2.Draw();
		m_MyMoneyText.Draw(m_pSurfaceBack->Xpos+89, m_pSurfaceBack->Ypos+430 , 108);

		// Disable 버튼을 보일 것인가 교환버튼을 보일 것인가?
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

		// 거래 상대방의 이름을 보여준다.
		if (m_pOppNameBox )
			m_pOppNameBox->Draw(m_pSurfaceBack->Xpos+110, m_pSurfaceBack->Ypos+92,87);

		if( m_pOppLevelBox )
			m_pOppLevelBox->Draw(m_pSurfaceBack->Xpos+34, m_pSurfaceBack->Ypos+92,70);

		// 마우스가 아이템 위로 올라갔을때의 설명.
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

		// 아이템이 위치한 슬롯 표시
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

		// Extra Slot에 있는 아이템이 놓일 곳 표시
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
		int index = -1;
		if( m_pNkCha )
		{
			index = m_pNkCha->GetExgSlotIndex(slot_x_num, slot_y_num);
			pItem = m_pNkCha->m_ExgInven;
		}

		if( g_pNk2DFrame->GetItemRender() && pItem )
		{
			g_pNk2DFrame->GetItemRender()->BeginRenderLolo();		// 중요..  m_pItemRender->RenderLolo의 파라미터에 영향을 준다.

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
							|| pItem->m_Vnum == 1308 // 이판사판 행운권 밝기 조절 05-10-05 원석
							|| pItem->m_Vnum == 1286
							|| ( pItem->m_Vnum >= 1598 && pItem->m_Vnum <= 1695 ) // 브라질 요청 남미국기망토 종류도 성조기에서 응용이므로 예외처리.
							|| pItem->m_Vnum == 1287 ) // 미국 성조기 망토, 성조기 날개도 어둡게 보여서 클라이언트에서 직접 밝게 처리.(by 원석)
						ambient = 0x00cccccc;
					else if (pItem->m_PlusNum > 0)
						ambient = UPGRADE_ITEM_COLOR;

					g_pNk2DFrame->RenderItemInUI( m_my_slotstart_x + pItem->m_SlotX * SLOT_WIDTH,
												  m_my_slotstart_y + pItem->m_SlotY * SLOT_HEIGHT,
												  pItem , FALSE, FALSE, FALSE, TRUE );
				}
				if(pItem->m_Vnum == 969 )//라비용 교환문서일때
					m_bMyPetExchNote =  true;
				m_cntItem++;//내 슬롯에 아이템의 수

				pItem = pItem->m_Next;
			}
			g_pNk2DFrame->GetItemRender()->EndRenderLolo();
		}
	}
}

void CControlExchange::DrawOppSlot() // 상대편 아이템을 보여주는 함수
{
	int xpos, ypos;
	m_bOppPetExchNote = false;

	if (g_pDisplay && m_pSurfaceBack)
	{
		RECT rcRect;
		POINT point;
		GetCursorPos(&point);
		ScreenToClient(g_pDisplay->GetHWnd(), &point);

		// 아이템이 위치한 슬롯 표시
		CItem *pItem = m_ExgOppInven;
		if(m_pPetInfo && m_bPetInfoDraw)	//969 라비용 교환문서일때
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

			int index = GetExgOppSlotIndex(slot_x_num, slot_y_num);

			pItem = m_ExgOppInven;

			if( g_pNk2DFrame->GetItemRender() && pItem )
			{
				g_pNk2DFrame->GetItemRender()->BeginRenderLolo();				// 중요....... 중요..
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
						// +아이템 정보를 가운데로 정열해서 출력한다
						if(	pItem->m_pTextOutBox)
							pItem->m_pTextOutBox->Draw(xpos+((cx/2) - (EXCH_TEXT_SIZE/2)), ypos+((cy/2) - (EXCH_TEXT_SIZE/2)));
					}
					if(pItem->m_Vnum == 969 )//라비용 교환문서일때
						m_bOppPetExchNote =  true;

					pItem = pItem->m_Next;
				}
				g_pNk2DFrame->GetItemRender()->EndRenderLolo();
			}
		}
	}// 라비용
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
	static char commOutBuf[512] = "";	// 나중에 변수 다른 곳에 설정.
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

		// 같은 위치에 있는 버튼  2개는 따로 처리.
		if (m_ExgBtn2.GetState() ==  BTN_ACTION)
		{
			///튜토리얼 예외처리
			if( pCMyApp->m_pUIMgr->IsEventScenePlaying() == 5 )
			{
				return 0;
			}

			// 라비용 양도 문서이외의 아이템이 존재하면 교환 할 수 없다
			if((m_bMyPetExchNote && m_cntItem != 1)
					|| ( m_bOppPetExchNote && m_cntItem))
			{
				// cancle
				if (m_bExch)
					ExchCancel(TRUE);

				//-- IDS_PET_TRANSFER : Pet 양도는 문서와 돈만이 교환 가능합니다
				g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_PET_TRANSFER), TYPE_NOR_OK, 1 );
				m_bPetInfoDraw = false;
			}
			else
			{
				if (m_bExch && !m_bDisable)	// 상대편이 락을 걸때까지 메시지 처리를 하지 않는다.
				{
					// 교환할 아이템 중에 교환불가 아이템이 있나본다.
					BOOL bCanExchange = TRUE;
					CItem *pItem = m_pNkCha->m_ExgInven;
					while (pItem)
					{
						if( g_bAdmin2 ) // 운영자는 교환불가 안먹게 수정( by 원석)
							break;

//						if( ( (GET_ATT_FLAG(pItem)&IATT_NO_EXCHANGE) && !g_bAdmin2 )
//							|| pItem->m_bTimeLimit || pItem->m_bTimeLimit /*|| pItem->m_PriceType == 2*/ ) // 운영자는 교환불가 안먹게 수정( by 원석) // 시간제 아이템이면 교환 불가처리 (by 원석) ///시간제아이템 // 캐쉬 아이템 불가처리 추가. ///유료상점			///07-02-20 태국에서 캐쉬상점 다시 불가속성 없애달란다...
						if( (GET_ATT_FLAG(pItem)&IATT_NO_EXCHANGE) || pItem->m_bTimeLimit || (g_dwClientCountry == CTRY_KOR&&pItem->m_Vnum == 1519) ) // 1519 경험치 아이템의 경우 한국과 해외의 속성 관리가 꼬여서 한국만 교환불가를 예외로 적용시킨다.
						{
							//-- IDS_EXCHANGE_UNABLE_ITEM : 교환이 불가능한 물건이 포함되어 있습니다.
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
				//-- IDS_EXCHANGE_INPUT_MONEY : 금액을 적어주세요
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

		if(pCMyApp->m_bEquipLock==1)	//대형장비 락.1009
			return 1;
		if(pCMyApp->m_bRingLock==1)		//반지 락.0407
			return 1;
		if(pCMyApp->m_bBrLock==1)		//-- 팔찌 락
			return 1;
		if(pCMyApp->m_bNecklaceLock==1)		///페어리
			return 1;

		if (IsInside(x, y))
		{
			if( timeGetTime() - m_dwMouseClickTickTime < 300 )  // 0.5초
				return 1;

			m_dwMouseClickTickTime = timeGetTime();

			// 마석 업그레이드 창이 떠있는경우 때문에 넣었따..
			if(g_pNk2DFrame && g_pNk2DFrame->GetControlInven()->m_bAct_Masuk)
			{
				g_pNk2DFrame->GetControlInven()->Close_Masuk();
				return 1;
			}

			// 겹쳐져 있는 두 개의 버튼은 따로 처리한다.
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
				// 작업동안 딜레이 타임 1.5초
				// 최소한의 안전장치
				if( m_byExchLock )
				{
					DWORD curTime = timeGetTime();

					if (curTime > m_timeExchLock + 1500) // 1.5초가 지났으면
						m_byExchLock = 0;
					else
						return 1;
				}

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

					if (m_pNkCha
							&& slot_x_num >= 0
							&& slot_x_num + GET_ITEM_WIDTH(m_pNkCha->m_ExtraSlot) - 1 < EXG_SLOT_X_NUM
							&& slot_y_num >= 0
							&& slot_y_num + GET_ITEM_HEIGHT(m_pNkCha->m_ExtraSlot) - 1 < EXG_SLOT_Y_NUM)
					{
						if (!m_bTrade)
						{
							// 아이템을 교환창에 넣었다고 알려준다.
							if (m_pNkCha && m_pNkCha->AddToExgInven(slot_x_num, slot_y_num))
							{
								sprintf(m_MsgBuf, "exch inven %d %d\n", slot_x_num, slot_y_num);
								SendMsg();
							}
						}
					}
				}
				// 아이템을 빼는 것
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
			g_pNk2DFrame->ShowExchangeWindow(FALSE);	// 이 내부에서 자동적으로 취소 메시지가 간다.
		}
		else
			return 0;

		return 1;

	case WM_RBUTTONDBLCLK:
		{
			x = LOWORD( lParam );
			y = HIWORD( lParam );

			// 대형 장비 이동 불가
			if( pCMyApp->m_bEquipLock == 1 )
				return 1;
			// 반지 이동 불가
			if( pCMyApp->m_bRingLock == 1 )
				return 1;
			if(pCMyApp->m_bBrLock ==1)
				return 1;
			if(pCMyApp->m_bNecklaceLock ==1) ///페어리
				return 1;


			// ExtraSlot에 아이템이 있을경우
			if( m_pNkCha->m_ExtraSlot )
				return 1;

			if( m_bTrade ) // 교환을 눌러놓은 후엔 이동 불가 (05-12-15 원석)
				return 1;

			if( IsInside( x, y ) )
			{
				// 마석 업그레이드 창이 떠있을 경우
				if( g_pNk2DFrame
						&& g_pNk2DFrame->GetControlInven()
						&& g_pNk2DFrame->GetControlInven()->m_bAct_Masuk )
				{
					g_pNk2DFrame->GetControlInven()->Close_Masuk();
					return 1;
				}

				// 영역 검사
				if( m_pNkCha
						&& ( x >= m_my_slotstart_x )
						&& ( x < m_my_slotstart_x + ( SLOT_WIDTH * EXG_SLOT_X_NUM ) )
						&& ( y >= m_my_slotstart_y )
						&& ( y < m_my_slotstart_y + ( SLOT_HEIGHT * EXG_SLOT_Y_NUM ) ) )
				{

					int nSlot_X_Num = 0;
					int nSlot_Y_Num = 0;

					// 슬롯 계산
					nSlot_X_Num = ( x - m_my_slotstart_x ) / SLOT_WIDTH;
					nSlot_Y_Num = ( y - m_my_slotstart_y ) / SLOT_HEIGHT;

					int nItemIndex = m_pNkCha->GetExgSlotIndex( nSlot_X_Num, nSlot_Y_Num );
					if( nItemIndex < 0 )
						return 1;

					int nVNum = 0;
					int nPlusNum = 0;
					int nSpecialNum = 0;
					int nSpecial2 = 0; // 옵션2
					int nNowEdr = 0; // 현재 내구도
					int nNowEdr2 = 0; // 최대 내구도

					CItem* pItem = m_pNkCha->m_ExgInven;
					CItem* pTempItem = pItem;

					// 인텍스 값을 이용한 아이테의 wnum 추출
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
							nSpecial2 = pItem->m_Special2;		// 아이템 스페셜값
							nNowEdr = pItem->m_UpgradeEndurance;		// 아이템 스페셜값
							nNowEdr2 = pItem->m_MaxUpgradeEndurance;		// 아이템 스페셜값

							break;
						}
						pItem = pItem->m_Next;
					}

					// const 변수 선언
					char szTmp[ 10 ] = {0,};
					char szStr[ 100 ] = {0,};

					// 작업동안 딜레이 타임 1.5초
					// 최소한의 안전장치
					if( m_byExchLock )
					{
						DWORD curTime = timeGetTime();

						if (curTime > m_timeExchLock + 1500) // 1.5초가 지났으면
							m_byExchLock = 0;
						else
							return 1;
					}

					// 딜레이가 지났으면 플레그 해제
					SetExchLock( 1 );

					// 선택한 아이템과 동일 아이템 추출
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
	// nate 2005-06-14 : 교환자 레벨 등급 출력
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

// 거래를 활성/비활성 상태로 만든다.
// 거래중일때 비활성 상태로 변하면 거래를 중지한다.
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
								   int UpEndur, int MaxUpdEndur, int Endur, int MaxEndur) ///신방어구강화시스템
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

		// 아이템 값을 입력해보자!
		ResetItemValue(pItem);
		// +아이템을 경우 CTextOutBox 생성
		if (gem_num > 0)
		{
			if(GET_TYPE(pItem)==ITYPE_SUB )	//1107	0414 고대반지
				pItem->m_PlusNum = gem_num;
			else
			{
				// nate 2004 - 4
				// 아이템 플러스값 오류 수정250값을 300으로 수정
				// 최대값 300은 설정할 필요가 없을꺼 같은데..
				// 필요하면 나중에 추가해도 무관할듯..
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
		// 그 자리에 무언가 있을때 없앤다.
		ExchInvenFrom(x, y, GET_ITEM_WIDTH(pItem), GET_ITEM_HEIGHT(pItem));

		pItem->m_SlotX = x;
		pItem->m_SlotY = y;

		pItem->m_Special = special;

		///신방어구강화시스템
		pItem->m_Special2 = special2; // 추가 옵션
		pItem->m_UpgradeEndurance = UpEndur;           // 현재 강화 내구도
		pItem->m_MaxUpgradeEndurance = MaxUpdEndur;    // 강화 내구도 최대치

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
// 상대의 교환창 부분에서 아이템이 없앨때.
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
	// 여기서 라비용 교환 일때 창을 닫는다.
	if( !g_pRoh )
		return;

	int k = 0;
	if (m_bTrade)
	{
		m_MyMoneyText.SetString(m_pNkCha->m_Money, true);

		// 아이템 리스트를 바꿔치기 한다.
		CItem *pItem;

		pItem = g_pRoh->m_ExgInven;
		g_pRoh->m_ExgInven = m_ExgOppInven;
		m_ExgOppInven = pItem;

		// 아이템의 슬롯 위치에 대한 지도도 바꾼다.
		int i, j;

		for (i=0; i < EXG_SLOT_X_NUM; i++)
		{
			for (j=0; j < EXG_SLOT_Y_NUM; j++)
				g_pRoh->m_ExgSlot[i][j] = m_ExgOppSlot[i][j];
		}

		// 상대편에게 준 리스트를 지워버린다.
		RemoveAllOppInven();
		//거래가 종료되고 일반 모드로 오픈된 생태로 플래그를 설정.
		m_bExch = FALSE;
		// 컨트들과 플래그 변화.
		SetVisibleMode(FALSE);
	}
}

// 상대편이 거래를 취소했거나 내가 거래를 취소한 경우.
void CControlExchange::ExchCancel(BOOL bSend)
{
	// 거래를 닫는다.
	// 교환창을 일반 모드로 다시 호출한다.
	RemoveAllOppInven();

	if (bSend && m_bExch)
	{
		sprintf(m_MsgBuf, "exch cancel\n");
		SendMsg();
	}

	m_bExch = FALSE;
	m_bPetInfoDraw = FALSE;
	// 버튼들 상태 조정.
	SetVisibleMode(FALSE);
}

// 거래 당사자의 교환금액.
void CControlExchange::ExchMoney(char *strMoney)
{
	if (strlen(strMoney) < 11)
	{
		strcpy(m_strMoney1, strMoney);
		m_MoneyText1.SetString(_atoi64(m_strMoney1), true);
		if(g_dwClientCountry ==CTRY_KOR)	//-- 한국만 지원
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
//-- 교환 금액 정규성 체크
//-- 2004.03.06 - 수정완료
//-- Lyul
//-- Release Version 에서 isdigit 가 한글 문자 입력에 대한 체크를 하지 못한다.
//-- !! 입력이 ASCII 로 저장 된다면 단순히 ASCII 로 숫자 영역만 CHECK가 좋음
//-- 0000.00.00 - ??????
//-- name / description
BOOL CControlExchange::CheckMoneyStr(char* szMoney)
{
	INT64 nMoney;

	// 입력된 스트링을 검사한다
	int len = strlen(szMoney);
	for(int i=0; i<len; i++)
	{
		//-- 이녀석이 문제다.
		if(isdigit(szMoney[i]) == FALSE
				|| (szMoney[i])<0x00)
		{
			if(is_space(szMoney[i]) ==TRUE)
				//-- IDS_EXCHANGE_UNABLE_BLANK : 공백은 포함될 수 없습니다
				g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_EXCHANGE_UNABLE_BLANK), TYPE_EXCHANGE, 16 );
			else
				//-- IDS_EXCHANGE_UNABLE_STRING : 숫자만 입력해주세요
				g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_EXCHANGE_UNABLE_STRING), TYPE_EXCHANGE, 16 );

			return FALSE;
		}
	}
	// 3. 현제 가지구 있는 보유금액보다 입력된 금액이 더큰경우
	nMoney = _atoi64(szMoney);

	if (nMoney < 0)
	{
		//-- IDS_EXCHANGE_FAULT_STR : 허용되지 않는 값입니다
		g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_EXCHANGE_FAULT_STR), TYPE_EXCHANGE, 16 );
		return FALSE;
	}

	if(m_pNkCha->m_Money < nMoney)
	{
		//-- IDS_EXCHANGE_SCARCE_MONEY : 금액이 부족합니다.
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

	// 스킬 아이콘을 그려준다.
	if( m_PetClass ) // 기본형은 스킬이 없다.
	{
		int SkillIconDrawCount = 0;
		int SkillIndex = 0; // 서버랑 주고 받는 스킬 인덱스.
		int MouseOnSkillIndex = -1; // 마우스가 올려져있는 스킬의 인덱스.

		POINT point;
		GetCursorPos(&point);
		ScreenToClient(g_pDisplay->GetHWnd(), &point);
		CMultiText multiText;

		for( i = 0 ; i < PET_SKILL_MAX ; ++i ) // 자기 펫이 배울수있는 스킬 갯수 만큼.
		{
			SkillIndex = g_PetSkillGroup[m_PetClass-1][i];
			m_pSkillIcon[SkillIndex]->Draw( m_pPetInfo->Xpos+78+(SkillIconDrawCount*SIZE_OF_SKILLMINICON), m_pPetInfo->Ypos+196, !m_PetSkillLevelInfo[i] ); // 스킬을 배웠는지의 아이콘. 스킬 갯수 만큼 가지고 있는다.
			++SkillIconDrawCount;

			// 마우스 온 체크.
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

		// 아이콘을 다 그린후 이름 팝업을 그려야 위에 그려진다.
		if( MouseOnSkillIndex != -1 ) // 올라간게 있으면...
		{
			if( m_pSkillIconName->GetIndex() != MouseOnSkillIndex ) // 이전 보던게 아닐때만...
			{
				char strTemp[50];

				//	by evilkiki 2008.08.04
				//	return value 위험성으로 함수 수정
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

		m_PetClass = 0; // 임시로.
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

	// 클래스 스트링 셋팅.
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
			// IDS_BASIC_TYPE : 기본형
			strcpy( strTemp, (char *)G_STRING(IDS_BASIC_TYPE) );
			break;
		case 1:
			// IDS_DEFENCE_TYPE : 방어형
			strcpy( strTemp, (char *)G_STRING(IDS_DEFENCE_TYPE) );
			break;
		case 2:
			// IDS_ATTACK_TYPE : 공격형
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
	// 애완동물을 정보를 지운다
	//자신의 슬롯창의 양도 문서 Clear/ ?
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
	//-- IDS_NK2D_REQUIRE_TRADE : %s 님이 거래요청을 했습니다
	sprintf(strTemp, (char*)G_STRING(IDS_NK2D_REQUIRE_TRADE), player_name);
	g_pNk2DFrame->InsertPopup(strTemp,TYPE_NOR_OKCANCLE, 10);
}

LRESULT CControlExchange::PopupMsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, int nMsg)
{
	if( nMsg == 1)	// 확인버튼이 눌린경우
	{
		// 거래 요청을 승낙하는 메시지를 보낸다.
		if (g_pTcpIp)
			g_pTcpIp->SendNetMessage("exch ok\n");
	}
	else if(nMsg == -1) // 취소버튼이 눌린경우
	{
		// 거래 요청을 거절하는 메시지를 보낸다.
		if (g_pTcpIp)
			g_pTcpIp->SendNetMessage("exch cancel\n");
	}

	return 0L;
}

LRESULT CControlExchange::MoneyPopupMsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, int nMsg, char* szMoney)
{
	// "확인" 버튼이 눌린경우
	if(nMsg == 1)
	{
		if(CheckMoneyStr(szMoney) == TRUE)
		{
			strcpy(m_strMoney2, szMoney);
			__int64 money = _atoi64(m_strMoney2);

			if(money > m_ExchMoney)
			{
				m_MoneyText2.SetString(money, true);

				if(g_dwClientCountry ==CTRY_KOR)	//-- 한국만 지원, 단위 환산
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

				// 여기다가 돈을 적어서 보낸다.
				if (m_bExch)
				{
					sprintf(m_MsgBuf, "exch money %s\n",m_strMoney2);	// 스페이스 주의.
					SendMsg();
					m_ExchMoney = money;
				}
			}
		}
	}
	else if (nMsg == -1)	// "취소" 버튼이 눌린경우
		m_bPetInfoDraw = false;	//라비용

	return 0L;
}

HRESULT CControlExchange::RestoreSurfaces()
{
	ActivateExch(FALSE);

	return S_OK;
}
/////////////////////////////////////////////////////////////////////////////////////
// 글로벌 함수
int GetFindPoint( char *SrcString, char FindChar ) // 스트링상에서 특정 문자의 위치를 찾는 함수.
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

int FindStringInString( char *SrcSting, char *FindString ) // 스트링 안에서 특정 스트링의 위치를 찾아주는 녀석
{
	if( !SrcSting || !FindString )
		return -1;

	int SrcLen = strlen( SrcSting );
	int FindLen = strlen( FindString );

	if( FindLen > SrcLen )
		return -1;

	for( int j = 0 ; j <= SrcLen-FindLen ; ++j )
	{
		if( strncmp(&SrcSting[j],FindString,FindLen) == 0 ) // 같은 부분이 있으면...
		{
			return j;
		}
	}

	return -1;
}

int InsertPriceComma( char *DestString, char *SrcString, char FindChar ) // 가격 스트링들의 숫자 부에 콤마를 넣어주는 함수.
{
	if( !DestString || !SrcString )
		return -1;

	char StrTemp[256];
	char StrTemp2[256];
	int StrLen;

	int ConvStartPos = GetFindPoint( SrcString, FindChar ); // 특정 위치를 얻고

	sscanf( &SrcString[ConvStartPos+1], "%s", StrTemp ); // 특정 위치 다음의 단어를 스트링으로 얻어온다.
	StrLen = strlen( StrTemp );

	int AddByte = (StrLen-1)/3; // 넣어야 하는 콤마의 갯수.

	int i = StrLen-1, j = 0, k = StrLen-1+AddByte;

	if( StrLen+AddByte > 255 )
		return -1;

	StrTemp[i+1] = '\0';
	StrTemp2[k+1] = '\0';

	ConvStartPos = FindStringInString( SrcString, StrTemp ); // 원하는 숫자부분이 시작되는 정확한 위치를 얻어온다.

	if( ConvStartPos == -1 ) // 뭔가 삐꾸.
		return -1;


	strncpy( DestString, SrcString, ConvStartPos );	// 치환할 부분의 앞부분을 복사.
	DestString[ConvStartPos] = '\0';

	while( i >= 0 )
	{
		if( j != 0 && j % 3 == 0 ) // 콤마를 찍어야할때...
		{
			StrTemp2[k] = ',';
			--k; // k를 하나더 증가.
		}

		StrTemp2[k] = StrTemp[i];

		--i;
		++j;
		--k;
	}

	strncpy( &DestString[ConvStartPos], StrTemp2, StrLen+AddByte );	// 치환 부분을 복사(콤마 길이도 포함해서...)

	strcpy( &DestString[ConvStartPos+StrLen+AddByte], &SrcString[ConvStartPos+StrLen] );  // 남은 부분 전부 복사.

	return ConvStartPos; // 치환부(숫자부)의 위치를 리턴
}

char *ConvertMoneyToString( char *DestString, __int64 Money, int SpaceSize, int Type ) // 돈 금액의 단위를 말로 바꿔주는 함수.
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
		//-- IDS_MONEY2STRING1 : (%d억
		sprintf(strMoneyText, (char*)G_STRING(IDS_MONEY2STRING1), hundred_million);
	if( ten_thousand )
		//-- IDS_MONEY2STRING2 : %s%d만
		sprintf(strMoneyText, (char*)G_STRING(IDS_MONEY2STRING2), strMoneyText, ten_thousand);
	if( laim )
		sprintf(strMoneyText, "%s%I64d", strMoneyText, laim);

	if( Type == 0 )
		//-- IDS_MONEY2STRING3 :  라임)
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
