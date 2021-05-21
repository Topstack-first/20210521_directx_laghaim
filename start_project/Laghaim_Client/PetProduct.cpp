#include "stdafx.h"
// PetProduct.cpp: implementation of the CPetProduct class.
//
//////////////////////////////////////////////////////////////////////



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
#include "PetProduct.h"
#include "textoutbox.h"
#include "SpecialItem.h"
#include "headers.h"
#include "skill.h"
#include "g_stringmanager.h"

#include "NkMob.h"
#include "NkPet.h"
#include "NkMyPet.h"

extern POINT			g_CursorPoint;

extern int				g_SkillsNum;
extern CRaceSkill		g_RaceSkill[];



#define EXCH_TEXT_SIZE			12
#define EXCH_TEXT_COLOR			RGB(255, 0, 0)
#define EXCH_BAKC_COLOR         RGB(47, 0, 17)

//#define INTERCHANGE_NUM 74
#define INTERCHANGE_NUM 6

static int InterChangeVnum [6] = {972,973,974,975,976,977};


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPetProduct::CPetProduct()
	: m_hWnd(NULL)
	, m_opp_slotstart_x(0)
	, m_opp_slotstart_y(0)
{
	memset(m_ExgOppSlot, 0, sizeof(m_ExgOppSlot));

	m_pBack = NULL;
	m_pPetBack = NULL;
	m_pEnableSlot = NULL;
	m_pDisableSlot= NULL;
	m_pExistSlot  = NULL;
	m_ExgOppInven = NULL;
	m_pOverItem = NULL;
	m_CancelBtn = NULL;
	m_pMsgPopUp = NULL;
	m_pPetLevel = NULL;
	m_pPetExp= NULL;
	m_pInfo = NULL;
	m_pInfo = new CAlphaPopUp;
	Init();
	m_ScreenWidth = 0;
	m_ScreenHeight = 0;
	m_my_slotstart_x = 0;
	m_my_slotstart_y = 0;
	m_cur_click_slot_x = 0;
	m_cur_click_slot_y = 0;
	ZeroMemory(m_MsgBuf,sizeof(m_MsgBuf));
	ZeroMemory(cPetLevel,sizeof(cPetLevel));
	ZeroMemory(cPetexp,sizeof(cPetexp));
	m_bWaitingExch = FALSE;
	m_bShowWindow = FALSE;

	m_pNkCha = NULL;
}
CPetProduct::~CPetProduct()
{
	DeleteRes();
	RemoveAllOppInven();

	SAFE_DELETE(m_pMsgPopUp);
	SAFE_DELETE(m_pInfo);

	m_pNkCha = NULL;
}

void CPetProduct::Init()
{
	m_pInfo->Init(14);
}
void CPetProduct::DeleteRes()
{
	if(m_ExgOppInven)
	{
		CItem *pItem = m_ExgOppInven;
		while (pItem)
		{
			if(pItem->m_pTextOutBox)
				pItem->m_pTextOutBox->DeleteRes();
			pItem = pItem->m_Next;
		}
	}

	SAFE_DELETE(m_pBack);
	SAFE_DELETE(m_pPetBack);
	SAFE_DELETE(m_pEnableSlot);
	SAFE_DELETE(m_pDisableSlot);
	SAFE_DELETE(m_pExistSlot);
	SAFE_DELETE(m_CancelBtn);
	SAFE_DELETE(m_pPetLevel);
	SAFE_DELETE(m_pPetExp);
}

void CPetProduct::LoadRes()
{
	if( !g_pDisplay || !g_pNk2DFrame->GetItemRender() || !pCMyApp->GetDevice() )
		return;

	m_hWnd =  g_pDisplay->GetHWnd();
	GetClientRect(m_hWnd, &m_ClientRc);

	m_ScreenWidth = g_pNk2DFrame->GetClientWidth();
	m_ScreenHeight = g_pNk2DFrame->GetClientHeight();

	SAFE_DELETE(m_CancelBtn);
	SAFE_DELETE(m_pPetLevel);
	SAFE_DELETE(m_pPetExp);

	m_CancelBtn = new CBasicButton;
	m_CancelBtn->SetFileName("common/btn_close_01"); // 버튼 이미지는 추후에 바꾼다
	m_CancelBtn->LoadRes();


	//	char clevel[20],cexp[30];
	m_pPetLevel = new CTextOutBox;
	m_pPetLevel->Init(12, RGB(255, 255, 0),0, 0);

	m_pPetExp = new CTextOutBox;
	m_pPetExp->Init(12, RGB(255, 255, 0),0, 0);

	SetVisibleMode(m_bShowWindow);

	m_pInfo->LoadRes(m_ScreenWidth, m_ScreenHeight);
	SAFE_DELETE(m_pPetBack);
	g_pDisplay->CreateSurfaceFromBitmap (&m_pPetBack, "interface/PetProduct/pet_product.bmp");

	SAFE_DELETE(m_pBack);
	g_pDisplay->CreateSurfaceFromBitmap(&m_pBack, "interface/PetProduct/PetProduct_BG.bmp");

	if( m_pBack && m_pPetBack )
	{
		m_pBack->SetColorKey( TRANS_COLOR );
		m_pBack->Xpos = g_pNk2DFrame->GetClientWidth()-m_pBack->GetWidth()-291;
		m_pBack->Ypos = 0;

		// PetBack Pos
		m_pPetBack->Xpos = m_pBack->Xpos + 31;
		m_pPetBack->Ypos = m_pBack->Ypos + 196;

		m_opp_slotstart_x = m_pBack->Xpos + 33 + 2;
		m_opp_slotstart_y = m_pBack->Ypos + 78 + 2;

		m_my_slotstart_x = m_pBack->Xpos + 33 + 2;
		m_my_slotstart_y = m_pBack->Ypos + 258 + 2;

		// 버튼 위치 정하기.
		m_CancelBtn->SetPosition(m_pBack->Xpos+214, m_pBack->Ypos+5);

		// Level,Exp TextBox 위치 정하기
		m_pPetLevel->SetPos(m_pPetBack->Xpos+57,m_pPetBack->Ypos+11);
		m_pPetExp->SetPos(m_pPetBack->Xpos+57,m_pPetBack->Ypos+33);
	}
}

BOOL CPetProduct::IsInside(int x, int y)
{
	if( !m_pBack )
		return FALSE;

	return m_pBack->IsIn( x, y);
}

BOOL CPetProduct::IsPlayArea(int x, int y)
{
	if( !IsInside(x,y) && !g_pNk2DFrame->IsInPopup(x,y) )
		return true;

	return false;
}

void CPetProduct::SendMsg()
{
	if (g_pTcpIp)
		g_pTcpIp->SendNetMessage(m_MsgBuf);
}

#define GET_OPP_SLOT_X(point_x)		((point_x - m_opp_slotstart_x) / SLOT_WIDTH)
#define GET_OPP_SLOT_Y(point_y)		((point_y - m_opp_slotstart_y) / SLOT_HEIGHT)

#define GET_MY_SLOT_X(point_x)		((point_x - m_my_slotstart_x) / SLOT_WIDTH)
#define GET_MY_SLOT_Y(point_y)		((point_y - m_my_slotstart_y) / SLOT_HEIGHT)

CItem* CPetProduct::Draw()
{
	if (g_pDisplay && m_pBack)
	{
		POINT point;
		GetCursorPos(&point);
		ScreenToClient(g_pDisplay->GetHWnd(), &point);

		g_pDisplay->Blt(m_pBack->Xpos, m_pBack->Ypos, m_pBack);
		// Petbackinfo Surface그리기
		g_pDisplay->Blt(m_pPetBack->Xpos, m_pPetBack->Ypos, m_pPetBack);

		// 버튼들 그리기.
		m_CancelBtn->Draw();
		//TextBox 그리기
		if(m_pPetLevel)
		{
			sprintf(cPetLevel,"%d",(int)g_MyPetStatus.level);
			m_pPetLevel->SetString(cPetLevel);
			m_pPetLevel->Draw();
		}
		if(m_pPetExp)
		{
			sprintf(cPetexp,"%d",(int)g_MyPetStatus.exp);
			m_pPetExp->SetString(cPetexp);
			m_pPetExp->Draw();
		}

		m_pOverItem = NULL;
		DrawMySlot();
		DrawOppSlot();

		// 마우스가 아이템 위로 올라갔을때의 설명.
		//CMultiText multiText;
		if (m_pNkCha->m_ExtraSlot)
		{
			m_pOverItem = NULL;
		}
		else if (m_pOverItem)
		{
			pCMyApp->m_pMouse->SetMouseType(M_HOLD);
		}
		else if (IsInside(point.x, point.y))
		{
			pCMyApp->m_pMouse->SetMouseType(M_NORMAL);
		}
	}

	return m_pOverItem;
}

void CPetProduct::DrawOppSlot()
{
	int xpos, ypos;
	bool bOver = false;
	char commBuf1[50];	// 나중에 변수 다른 곳에 설정.
	char commBuf2[50];
	if (g_pDisplay && m_pBack)
	{
		POINT point;
		GetCursorPos(&point);
		ScreenToClient(g_pDisplay->GetHWnd(), &point);

		// 아이템이 위치한 슬롯 표시
		CItem *pItem = m_ExgOppInven;

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
		// 이 부분에서 vnum을 얻어와야 한다. (interchange class의 경우에)
		int vnum = GetExgOppSlotVnum(slot_x_num, slot_y_num);
		//	int t_slot_width, t_item_width, t_item_render_width;
		g_pNk2DFrame->GetItemRender()->BeginRenderLolo();				// 중요....... 중요..
		while (pItem)
		{
			if (pItem->m_Vnum == vnum)
			{
				g_pNk2DFrame->RenderItemInUI( m_opp_slotstart_x + pItem->m_SlotX * SLOT_WIDTH,
											  m_opp_slotstart_y + pItem->m_SlotY * SLOT_HEIGHT,
											  pItem , TRUE, FALSE, FALSE, TRUE );
				//m_pOverItem = pItem;

				CMultiText multiText;

				if(vnum == 972)
				{
					//-- IDS_PET_PRODUCT_CAPSULE1 : 데미지 누적 해제 캡슐
					sprintf(commBuf1,G_STRING(IDS_PET_PRODUCT_CAPSULE1));
					//-- IDS_PET_PRODUCT_NEED_LEVEL1 : 경험치 10,000 이 필요합니다
					sprintf(commBuf2,G_STRING(IDS_PET_PRODUCT_NEED_LEVEL1));
					multiText.AddString(commBuf1, _FCOLOR_YELLOW);
					multiText.AddString(commBuf2, _FCOLOR_GREEN);
				}
				else if(vnum == 973)
				{
					//-- IDS_PET_PRODUCT_CAPSULE2 : 일반 공격력 향상 캡슐
					sprintf(commBuf1,G_STRING(IDS_PET_PRODUCT_CAPSULE2));
					//-- IDS_PET_PRODUCT_NEED_LEVEL2 : 경험치 30,000 이 필요합니다
					sprintf(commBuf2,G_STRING(IDS_PET_PRODUCT_NEED_LEVEL2));
					multiText.AddString(commBuf1, _FCOLOR_YELLOW);
					multiText.AddString(commBuf2, _FCOLOR_GREEN);
				}
				else if(vnum == 974)
				{
					//-- IDS_PET_PRODUCT_CAPSULE3 : 일반 방어력 향상 캡슐
					sprintf(commBuf1,G_STRING(IDS_PET_PRODUCT_CAPSULE3));
					//-- IDS_PET_PRODUCT_NEED_LEVEL3 : 경험치 40,000 이 필요합니다
					sprintf(commBuf2,G_STRING(IDS_PET_PRODUCT_NEED_LEVEL3));
					multiText.AddString(commBuf1, _FCOLOR_YELLOW);
					multiText.AddString(commBuf2, _FCOLOR_GREEN);
				}
				else if(vnum == 975)
				{
					//-- IDS_PET_PRODUCT_CAPSULE4 : 상태 해제 캡슐
					sprintf(commBuf1,G_STRING(IDS_PET_PRODUCT_CAPSULE4));
					//-- IDS_PET_PRODUCT_NEED_LEVEL4 : 경험치 50,000 이 필요합니다
					sprintf(commBuf2,G_STRING(IDS_PET_PRODUCT_NEED_LEVEL4));
					multiText.AddString(commBuf1, _FCOLOR_YELLOW);
					multiText.AddString(commBuf2, _FCOLOR_GREEN);
				}
				else if(vnum == 976)
				{
					//-- IDS_PET_PRODUCT_CAPSULE5 : 고급 공격력 향상 캡슐
					sprintf(commBuf1,G_STRING(IDS_PET_PRODUCT_CAPSULE5));
					//-- IDS_PET_PRODUCT_NEED_LEVEL5 : 경험치 50,000 이 필요합니다
					sprintf(commBuf2,G_STRING(IDS_PET_PRODUCT_NEED_LEVEL5));
					multiText.AddString(commBuf1, _FCOLOR_YELLOW);
					multiText.AddString(commBuf2, _FCOLOR_GREEN);
				}
				else if(vnum == 977)
				{
					//-- IDS_PET_PRODUCT_CAPSULE6 : 고급 방어력 향상 캡슐
					sprintf(commBuf1,G_STRING(IDS_PET_PRODUCT_CAPSULE6));
					//-- IDS_PET_PRODUCT_NEED_LEVEL6 : 경험치 70,000 이 필요합니다
					sprintf(commBuf2,G_STRING(IDS_PET_PRODUCT_NEED_LEVEL6));
					multiText.AddString(commBuf1, _FCOLOR_YELLOW);
					multiText.AddString(commBuf2, _FCOLOR_GREEN);
				}
				m_pInfo->PutString(&multiText);
				bOver = true;
				//m_pInfo->Draw(point.x, point.y);
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
				g_pNk2DFrame->RenderItemInUI( m_opp_slotstart_x + pItem->m_SlotX * SLOT_WIDTH,
											  m_opp_slotstart_y + pItem->m_SlotY * SLOT_HEIGHT,
											  pItem , FALSE, FALSE, FALSE, TRUE );

				xpos = m_opp_slotstart_x + pItem->m_SlotX * SLOT_WIDTH + (GET_ITEM_WIDTH(pItem) * SLOT_WIDTH - ITEM_RENDER_WIDTH) /2;
				ypos = m_opp_slotstart_y + pItem->m_SlotY * SLOT_HEIGHT + (GET_ITEM_HEIGHT(pItem) * SLOT_HEIGHT - ITEM_RENDER_HEIGHT) /2;
				int cx, cy;

				xpos = m_opp_slotstart_x + (pItem->m_SlotX*SLOT_WIDTH);
				ypos = m_opp_slotstart_y + (pItem->m_SlotY*SLOT_HEIGHT);
				cx = SLOT_WIDTH; // * GET_ITEM_WIDTH(pItem);
				cy = SLOT_HEIGHT;// * GET_ITEM_HEIGHT(pItem);

				// +아이템 정보를 가운데로 정열해서 출력한다
				if(	pItem->m_pTextOutBox)
				{
					pItem->m_pTextOutBox->Draw(xpos+((cx/2) - (EXCH_TEXT_SIZE/2)), ypos+((cy/2) - (EXCH_TEXT_SIZE/2)));
				}
			}


			pItem = pItem->m_Next;
		}
		g_pNk2DFrame->GetItemRender()->EndRenderLolo();
		if(bOver)
			m_pInfo->Draw(point.x, point.y);
	}

}

void CPetProduct::DrawMySlot()
{
	if (g_pDisplay && m_pBack)
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
			if (point.x >= m_my_slotstart_x && point.x < m_my_slotstart_x + (SLOT_WIDTH * EXG_SLOT_X_NUM ) &&
					point.y >= m_my_slotstart_y && point.y < m_my_slotstart_y + (SLOT_HEIGHT * EXG_SLOT_Y_NUM) )
			{
				if (GET_ITEM_WIDTH(m_pNkCha->m_ExtraSlot) % 2)
				{
					slot_x_num = (point.x - m_my_slotstart_x) / SLOT_WIDTH - (GET_ITEM_WIDTH(m_pNkCha->m_ExtraSlot)/2);
				}
				else
				{
					slot_x_num = (point.x - m_my_slotstart_x + (SLOT_WIDTH/2)) / SLOT_WIDTH - (GET_ITEM_WIDTH(m_pNkCha->m_ExtraSlot)/2);
				}
				if (GET_ITEM_HEIGHT(m_pNkCha->m_ExtraSlot) % 2)
				{
					slot_y_num = (point.y - m_my_slotstart_y) / SLOT_HEIGHT - (GET_ITEM_HEIGHT(m_pNkCha->m_ExtraSlot)/2);
				}
				else
				{
					slot_y_num = (point.y - m_my_slotstart_y + (SLOT_HEIGHT/2)) / SLOT_HEIGHT - (GET_ITEM_HEIGHT(m_pNkCha->m_ExtraSlot)/2);
				}
				if (slot_x_num >= 0 && slot_x_num + GET_ITEM_WIDTH(m_pNkCha->m_ExtraSlot) - 1 < EXG_SLOT_X_NUM &&
						slot_y_num >= 0 && slot_y_num + GET_ITEM_HEIGHT(m_pNkCha->m_ExtraSlot) - 1 < EXG_SLOT_Y_NUM)
				{
					rcRect.left = 0;
					rcRect.right = SLOT_WIDTH * GET_ITEM_WIDTH(m_pNkCha->m_ExtraSlot);
					rcRect.top = 0;
					rcRect.bottom = SLOT_HEIGHT * GET_ITEM_HEIGHT(m_pNkCha->m_ExtraSlot);
					if (m_pNkCha->GetItemIndexInExgSlot(slot_x_num, slot_y_num,
														GET_ITEM_WIDTH(m_pNkCha->m_ExtraSlot), GET_ITEM_HEIGHT(m_pNkCha->m_ExtraSlot))
							>= -1)
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
				if (g_pNk2DFrame->IsScroll(pItem) )
				{
					ambient = 0x00cccccc;
				}
				else if (pItem->m_PlusNum > 0)
				{
					ambient = UPGRADE_ITEM_COLOR;
				}
				g_pNk2DFrame->RenderItemInUI( m_my_slotstart_x + pItem->m_SlotX * SLOT_WIDTH,
											  m_my_slotstart_y + pItem->m_SlotY * SLOT_HEIGHT,
											  pItem , FALSE, FALSE, FALSE, TRUE );

			}
			pItem = pItem->m_Next;
		}
		g_pNk2DFrame->GetItemRender()->EndRenderLolo();
	}
}

LRESULT CPetProduct::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static char commOutBuf[512];	// 나중에 변수 다른 곳에 설정.
	int x, y;
	int slot_x_num, slot_y_num, vnum;
	int hr = -1;


	// 현제 메세지가 들어왔는가??
	/*	if(m_MsgPopUp.CheckMsgExist() == TRUE) 	{
			int ret;
			if(msg == WM_MOVE)
				 return 0;
			ret = m_MsgPopUp.MsgProc(hWnd, msg, wParam, lParam);
			if(ret == 1)
				m_bWaitingExch = FALSE;
			else if(ret==-1)
				m_MsgPopUp.m_bActive = FALSE;
			return 1;
		}
	*/
	switch (msg)
	{
	case WM_LBUTTONDBLCLK:
		x = LOWORD (lParam);
		y = HIWORD (lParam);
		if (x >= m_opp_slotstart_x && x < m_opp_slotstart_x + (SLOT_WIDTH * EXG_SLOT_X_NUM) &&
				y >= m_opp_slotstart_y && y < m_opp_slotstart_y + (SLOT_HEIGHT * EXG_SLOT_Y_NUM) )
		{
			// 범위 재조정의 필요?
			m_cur_click_slot_x = slot_x_num = GET_OPP_SLOT_X(x);
			m_cur_click_slot_y = slot_y_num = GET_OPP_SLOT_Y(y);

			vnum = GetExgOppSlotVnum(m_cur_click_slot_x, m_cur_click_slot_y);

			if (vnum >= 0)
			{
				if(g_MyPetStatus.mode < MYPET_MODE_HOLD)  // 항상 애완동물이 옆에 있어야 한다
				{
					return 1;
				}
				else
				{
					int item_level = -1;

					CItem *pItem = m_ExgOppInven; // 아이템 찾기
					while (pItem)
					{
						if(pItem->m_Vnum == vnum)
						{
							item_level = pItem->m_MinLevel;
							if(item_level == 0) item_level = 1;
							break;
						}
						pItem = pItem->m_Next;
					}
					switch(pItem->m_Vnum)
					{
					case 972:
						if(g_MyPetStatus.m_Class==0 && g_MyPetStatus.level < 77)
						{
							//-- IDS_PET_PRODUCT_PETLEVEL1 : 애완동물 레벨 77 이상 가능합니다
							g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_PET_PRODUCT_PETLEVEL1), TYPE_NOR_OK, 1 );
							return 1;
						}
						if(g_MyPetStatus.exp<10000)
						{
							//-- IDS_PET_PRODUCT_PETEXP1 : 애완동물 경험치가  10000 이상 가능합니다
							g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_PET_PRODUCT_PETEXP1), TYPE_NOR_OK, 1 );
							return 1;
						}
						break;
					case 973:
						if(g_MyPetStatus.m_Class==0 && g_MyPetStatus.level < 79)
						{
							//-- IDS_PET_PRODUCT_PETLEVEL2 :  애완동물 레벨 79 이상 가능합니다
							g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_PET_PRODUCT_PETLEVEL2), TYPE_NOR_OK, 1 );
							return 1;
						}
						if(g_MyPetStatus.exp<30000)
						{
							//-- IDS_PET_PRODUCT_PETEXP2 : 애완동물 경험치가  30000 이상 가능합니다
							g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_PET_PRODUCT_PETEXP2), TYPE_NOR_OK, 1 );
							return 1;
						}
						break;
					case 974:
						if(g_MyPetStatus.m_Class==0 && g_MyPetStatus.level < 81)
						{
							//-- IDS_PET_PRODUCT_PETLEVEL3 : 애완동물 레벨 81 이상 가능합니다
							g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_PET_PRODUCT_PETLEVEL3), TYPE_NOR_OK, 1 );
							return 1;
						}
						if(g_MyPetStatus.exp<40000)
						{
							//-- IDS_PET_PRODUCT_PETEXP3 :  애완동물 경험치가  40000 이상 가능합니다
							g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_PET_PRODUCT_PETEXP3), TYPE_NOR_OK, 1 );
							return 1;
						}
						break;
					case 975:
						if(g_MyPetStatus.m_Class==0 && g_MyPetStatus.level < 89)
						{
							//-- IDS_PET_PRODUCT_PETLEVEL4 :  애완동물 레벨 89 이상 가능합니다
							g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_PET_PRODUCT_PETLEVEL4), TYPE_NOR_OK, 1 );
							return 1;
						}
						if(g_MyPetStatus.exp<50000)
						{
							//-- IDS_PET_PRODUCT_PETEXP4 : 애완동물 경험치가  50000 이상 가능합니다
							g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_PET_PRODUCT_PETEXP4), TYPE_NOR_OK, 1 );
							return 1;
						}
						break;
					case 976:
						if(g_MyPetStatus.m_Class==0 && g_MyPetStatus.level < 91)
						{
							//-- IDS_PET_PRODUCT_PETLEVEL5 : 애완동물 레벨 91 이상 가능합니다
							g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_PET_PRODUCT_PETLEVEL5), TYPE_NOR_OK, 1 );
							return 1;
						}
						if(g_MyPetStatus.exp<50000)
						{
							//-- IDS_PET_PRODUCT_PETEXP5 :  애완동물 경험치가  50000 이상 가능합니다
							g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_PET_PRODUCT_PETEXP5), TYPE_NOR_OK, 1 );
							return 1;
						}
						break;
					case 977:
						if(g_MyPetStatus.m_Class==0 && g_MyPetStatus.level < 93)
						{
							//-- IDS_PET_PRODUCT_PETLEVEL6 : 애완동물 레벨 93 이상 가능합니다
							g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_PET_PRODUCT_PETLEVEL6), TYPE_NOR_OK, 1 );
							return 1;
						}
						if(g_MyPetStatus.exp<70000)
						{
							//-- IDS_PET_PRODUCT_PETEXP6 :  애완동물 경험치가  70000 이상 가능합니다
							g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_PET_PRODUCT_PETEXP6), TYPE_NOR_OK, 1 );
							return 1;
						}
						break;
					}
					sprintf(m_MsgBuf, "p_compo %d\n", vnum);//생산 메시지 보내기.
					SendMsg();
					m_bWaitingExch = TRUE;
				}
			}
		}
		return 1;
		break;
	case WM_LBUTTONUP:
	case WM_MOUSEMOVE:
		x = LOWORD (lParam);
		y = HIWORD (lParam);
		if (!IsInside(x, y))
			return 0;
		else
		{
			m_CancelBtn->MsgProc(hWnd, msg, wParam, lParam);
			if (m_CancelBtn->GetState() == BTN_ACTION)
			{
				g_pNk2DFrame->ShowPetProductWindow(FALSE);
				m_CancelBtn->SetState(BTN_NORMAL);
			}
			return 1;
		}
		break;
	case WM_RBUTTONDOWN:
		x = LOWORD (lParam);
		y = HIWORD (lParam);
		if (IsPlayArea(x, y))
		{
			g_pNk2DFrame->ShowPetProductWindow(FALSE);
		}
		return 1;

	case WM_LBUTTONDOWN:
		x = LOWORD (lParam);
		y = HIWORD (lParam);

		if (IsInside(x, y))
		{
			if (m_CancelBtn->IsInside(x, y))
				m_CancelBtn->MsgProc(hWnd, msg, wParam, lParam);
			else if (x >= m_my_slotstart_x && x < m_my_slotstart_x + (SLOT_WIDTH * EXG_SLOT_X_NUM) &&
					 y >= m_my_slotstart_y && y < m_my_slotstart_y + (SLOT_HEIGHT * EXG_SLOT_Y_NUM) )
			{
				int slot_x_num, slot_y_num;
				// 엑스트라 슬롯에 있는 경우, 교환창 슬롯에 놓는다.
				if (m_pNkCha->m_ExtraSlot)
				{
					//if (m_bExch)
					//	return 1;
					if (GET_ITEM_WIDTH(m_pNkCha->m_ExtraSlot) % 2)
					{
						slot_x_num = (x - m_my_slotstart_x) / SLOT_WIDTH - (GET_ITEM_WIDTH(m_pNkCha->m_ExtraSlot)/2);
					}
					else
					{
						slot_x_num = (x - m_my_slotstart_x + (SLOT_WIDTH/2)) / SLOT_WIDTH - (GET_ITEM_WIDTH(m_pNkCha->m_ExtraSlot)/2);
					}
					if (GET_ITEM_HEIGHT(m_pNkCha->m_ExtraSlot) % 2)
					{
						slot_y_num = (y - m_my_slotstart_y) / SLOT_HEIGHT - (GET_ITEM_HEIGHT(m_pNkCha->m_ExtraSlot)/2);
					}
					else
					{
						slot_y_num = (y - m_my_slotstart_y + (SLOT_HEIGHT/2)) / SLOT_HEIGHT - (GET_ITEM_HEIGHT(m_pNkCha->m_ExtraSlot)/2);
					}
					if (slot_x_num >= 0 && slot_x_num + GET_ITEM_WIDTH(m_pNkCha->m_ExtraSlot) - 1 < EXG_SLOT_X_NUM &&
							slot_y_num >= 0 && slot_y_num + GET_ITEM_HEIGHT(m_pNkCha->m_ExtraSlot) - 1 < EXG_SLOT_Y_NUM)
					{
						if (m_pNkCha->AddToExgInven(slot_x_num, slot_y_num))
						{

							sprintf(m_MsgBuf, "exch inven %d %d\n", slot_x_num, slot_y_num);
							SendMsg();
						}
					}
				}
				else
				{
					// 아이템을 빼는 것.
					if (!m_bWaitingExch)
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
		else if (IsPlayArea(x, y))
		{
			g_pNk2DFrame->ShowPetProductWindow(FALSE);	// 이 내부에서 자동적으로 취소 메시지가 간다.

		}
		else
			return 0;

		return 1;
		break;
	}

	return 0;
}

int	CPetProduct::GetExgOppSlotVnum(int x, int y)
{
	if ( x < 0 || x >= EXG_SLOT_X_NUM || y < 0 || y >= EXG_SLOT_Y_NUM )
		return -1;

	//829,830,831,832,833,834,
	//835,836,837,838,839,840,
	//841,842,843,844;
	int vnum = m_ExgOppSlot[y][x];

	return vnum;
}

void CPetProduct::SetVisibleMode(BOOL bExch)
{
//	m_pPetLevel->Draw();
//	m_pPetExp->Draw();
	/*
		m_MoneyBtn1.SetDisable(TRUE);
	m_MoneyBtn1.SetState(BTN_DISABLE);

	strcpy(m_strMoney1, "0");
	strcpy(m_strMoney2, "0");

	m_MoneyText1.SetString(atoi(m_strMoney1));
	m_MoneyText2.SetString(atoi(m_strMoney2));
	*/
	if (bExch)
	{
		// 상대편 교환창에 바꿀 아이템을 놓는다.
		ShowItemsToChange();
	}
	else
	{
		// 아무것도 하지 않는다.
		RemoveAllOppInven();
	}
}

//--------------------------------------------------------------------------
// Name : ShowItemsToChange()
// Desc : 바꿀 수 있는 아이템을 보여주기 위한 루틴.
//--------------------------------------------------------------------------
void CPetProduct::ShowItemsToChange()
{
	int i, x_pos;

	// get item info to change
	// make sure that there is only one item in exchange inventory.

	//CItem *pChangeItem = m_pNkCha->m_ExgInven;

	// search old index
	CItem *pItem = NULL;

	int cw, ch;
	int slot_ch = 0;
	int slot_cw = 0;
	cw = ch = 0;

	x_pos = 0;

	RemoveAllOppInven();

	//for (i=0; i < 4; i++) {
	//	for (int j=0; j < 4; j++) {
	for(i=12; i<INTERCHANGE_NUM+12; i++)
	{
		int w = i % 6;
		int h = i / 6;

		// make dummy items
		pItem = new CItem;
		pItem->m_SlotX = w;
		pItem->m_SlotY = h;
		pItem->m_Index = -1050-(w+h);
		pItem->m_PlusNum = 0;//pChangeItem->m_PlusNum;
		pItem->m_Vnum = InterChangeVnum[i-12];
		pItem->m_ProtoNum = g_ItemList.FindItem(InterChangeVnum[i-12]);
		// 아이템 값을 입력해보자!
		pItem->m_Num0 = GET_ITEM_NUM0(pItem);
		pItem->m_Num1 = GET_ITEM_NUM1(pItem);
		pItem->m_Num2 = GET_ITEM_NUM2(pItem);
		pItem->m_Num3 = GET_ITEM_NUM3(pItem);
		pItem->m_Num4 = GET_ITEM_NUM4(pItem);
		pItem->m_Num5 = GET_ITEM_NUM5(pItem);
		pItem->m_Num6 = GET_ITEM_NUM6(pItem);
		pItem->m_Num7 = GET_ITEM_NUM7(pItem);
		pItem->m_Num8 = GET_ITEM_NUM8(pItem);
		pItem->m_Num9 = GET_ITEM_NUM9(pItem);
		pItem->m_MinLevel = GET_ITEM_LEVEL(pItem);
		pItem->m_MinLevel_Org = pItem->m_MinLevel;
		pItem->m_Int = GET_ITEM_INT(pItem);
		pItem->m_Dex = GET_ITEM_DEX(pItem);
		pItem->m_Weight = GET_WEIGHT(pItem);


		pItem->m_Next = m_ExgOppInven;
		m_ExgOppInven = pItem;

		//829,830,831,832,833,834,
		//835,836,837,838,839,840,
		//841,842,843,844;
		m_ExgOppSlot[h][w] = InterChangeVnum[i-12];
	}
	m_bShowWindow = TRUE;
}

void CPetProduct::RemoveAllOppInven()
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

	for ( i=0; i < EXG_SLOT_Y_NUM; i++)
	{
		for ( j = 0; j < EXG_SLOT_X_NUM; j++)
		{
			m_ExgOppSlot[i][j] = -1;
		}
	}

	m_bShowWindow = FALSE;
}

void CPetProduct::InterCancel(BOOL bSend)
{
	// 교환창을 일반 모드로 다시 호출한다.
	RemoveAllOppInven();

	// 버튼들 상태 조정.
	SetVisibleMode(FALSE);
}

// 거래를 활성/비활성 상태로 만든다.
// 거래중일때 비활성 상태로 변하면 거래를 중지한다.
void CPetProduct::ActivateExch(BOOL bExch)
{
	if (bExch == FALSE)
	{
		InterCancel(TRUE);
		return;
	}
	else
	{
		SetVisibleMode(bExch);
	}
}

void CPetProduct::SetNkCha(CNkCharacter *pNkCha)
{
	if( pNkCha )
		m_pNkCha = pNkCha;
}
///Pet 생산 성공. 부품 없애고 완성품만 교환창에...
void CPetProduct::AssemblySuccess(int vnum, int index, int slot_x, int slot_y)
{
	if (vnum == -1)
	{
		m_bWaitingExch = FALSE;
		return;
	}

	// 지워질 아이템을 찾는 것이 필요.

	CItem *pItem = g_pRoh->m_ExgInven;

	while(pItem)
	{
		if (pItem->m_SlotX == slot_x && pItem->m_SlotY == slot_y)
		{
			break;
		}
		pItem = pItem->m_Next;
	}

	if (!pItem)				// 못 찾았다.. 몬가 문제가 있음.
		return;

	// 찾은 아이템을 교체한다.
	pItem->m_Index = index;
	pItem->m_Vnum = vnum;
	pItem->m_ProtoNum = g_ItemList.FindItem(vnum);
	pItem->m_PlusNum = 0;
	pItem->m_Special = 0;

	pItem->m_Num0 = GET_ITEM_NUM0(pItem);
	pItem->m_Num1 = GET_ITEM_NUM1(pItem);
	pItem->m_Num2 = GET_ITEM_NUM2(pItem);
	pItem->m_Num3 = GET_ITEM_NUM3(pItem);
	pItem->m_Num4 = GET_ITEM_NUM4(pItem);
	pItem->m_Num5 = GET_ITEM_NUM5(pItem);
	pItem->m_Num6 = GET_ITEM_NUM6(pItem);
	pItem->m_Num7 = GET_ITEM_NUM7(pItem);
	pItem->m_Num8 = GET_ITEM_NUM8(pItem);
	pItem->m_Num9 = GET_ITEM_NUM9(pItem);
	pItem->m_MinLevel = GET_ITEM_LEVEL(pItem);
	pItem->m_MinLevel_Org = pItem->m_MinLevel;
	pItem->m_Int = GET_ITEM_INT(pItem);
	pItem->m_Dex = GET_ITEM_DEX(pItem);
	pItem->m_Weight = GET_WEIGHT(pItem);

	for (int x_idx = slot_x; x_idx < slot_x + GET_ITEM_WIDTH(pItem); x_idx++)
	{
		for (int y_idx = slot_y; y_idx < slot_y + GET_ITEM_HEIGHT(pItem); y_idx++)
		{
			g_pRoh->m_ExgSlot[x_idx][y_idx] = index;
		}
	}

	m_bWaitingExch = FALSE;
}

BOOL CPetProduct::AssemblyFail(BOOL bfail)
{
	m_bWaitingExch = FALSE;
	return bfail;
}