#include "stdafx.h"
#define D3D_OVERLOADS



#include <math.h>
#include <d3d.h>
#include "d3dutil.h"
#include "d3dutil.h"
#include "main.h"
#include "IndexedTexture.h"
#include "basicbutton.h"
#include "textoutbox.h"
#include "msgpopup.h"
#include "nkcharacter.h"
#include "headers.h"
#include "msgpopup.h"
#include "tcpipcon.h"
#include "ControlPetShop2.h"
#include "Nk2DFrame.h"
#include "nkmob.h"
#include "nkpet.h"
#include "nkmypet.h"
#include "Country.h"
#include "g_stringmanager.h"

#define PETPAY 1000000







extern DWORD		g_dwLangType;
extern DWORD		g_dwClientCountry;


char * egg_name[128] = { "interface/PetShop/Egg_B.bmp"
						 , "interface/PetShop/Egg_K.bmp"
						 , "interface/PetShop/Egg_A.bmp"
						 , "interface/PetShop/Egg_H.bmp"
						 , "interface/PetShop/Egg_F.bmp"
						 , "interface/PetShop/Egg_F.bmp"
					   };


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CControlPetShop2::CControlPetShop2()
{
	m_pBack			= NULL;

	for(int i = 0; i < NUM_RACE; i++ )
		m_pSurPetEgg[i] = NULL;

	m_bActive = TRUE;
	m_iPetEgg_race = 0;
	m_dwLastOpenTime = timeGetTime();
	m_IsOpenWindow	 = FALSE;
	m_bIsOpenWindow2 = FALSE;

	m_petshop_status = PetShop_Status__None;
}

CControlPetShop2::~CControlPetShop2()
{
	DeleteRes();
}
void CControlPetShop2::LoadRes()
{
	if(!g_pDisplay)
		return;

	if(!pCMyApp)
		return;

	if( pCMyApp->m_pMyPet )
	{
		if( pCMyApp->m_pMyPet->m_PetClass == 0 )
			m_petshop_status = PetShop_Status__Rabi;
		else
			m_petshop_status = PetShop_Status__Pera;
	}

	switch( m_petshop_status )
	{
	case PetShop_Status__None:
		g_pDisplay->CreateSurfaceFromBitmap(&m_pBack, "interface/PetShop/Pet_Shop.bmp");
		break;
	case PetShop_Status__Rabi:
		g_pDisplay->CreateSurfaceFromBitmap(&m_pBack, "interface/PetShop/Pet_Shop_none.bmp");
		break;
	case PetShop_Status__Pera:
		g_pDisplay->CreateSurfaceFromBitmap(&m_pBack, "interface/PetShop/Pet_Shop_12.bmp");
		break;
	}

	if( !m_pBack )
		return;

	m_pBack->SetColorKey(TRANS_COLOR);
	m_pBack->Xpos = ( g_pNk2DFrame->GetClientWidth() - m_pBack->GetWidth() ) >> 1;
	m_pBack->Ypos = ( g_pNk2DFrame->GetClientHeight() - m_pBack->GetHeight() ) >> 1;

	m_CloseBtn.SetFileName("common/btn_close_01");
	m_CloseBtn.LoadRes();
	m_CloseBtn.SetPosition(m_pBack->Xpos+342, m_pBack->Ypos+5);

	switch( m_petshop_status )
	{
	case PetShop_Status__None:
		{
			m_BuyBtn.SetFileName("common/btn_buy_02");
			m_BuyBtn.LoadRes();
			m_BuyBtn.SetPosition(m_pBack->Xpos+230, m_pBack->Ypos+140);

			for( int i = 0 ; i < NUM_RACE ; i ++ )
			{
				g_pDisplay->CreateSurfaceFromBitmap(&m_pSurPetEgg[i], egg_name[i]);

				if( m_pSurPetEgg[ i ] )
				{
					m_pSurPetEgg[ i ]->SetColorKey(RGB(0, 0, 0));
					m_pSurPetEgg[ i ]->Xpos = m_pBack->Xpos+60;
					m_pSurPetEgg[ i ]->Ypos = m_pBack->Ypos+70;
				}
			}

			char tmp[255] = {0,};

			m_pCur_money.Init(12, _FCOLOR_YELLOW,0, 0);
			m_pPay_money.Init(12, _FCOLOR_YELLOW,0, 0);
			m_pCur_money.SetString(g_pRoh->m_Money, true);
			if (g_dwClientCountry == CTRY_IDN)
				m_pPay_money.SetString(2000000,true);
			else if (g_dwClientCountry == CTRY_TWN
					 || g_dwClientCountry == CTRY_CN
					 || g_dwClientCountry == CTRY_JPN)
				m_pPay_money.SetString(500000, true);
			else
				m_pPay_money.SetString(1000000, true);

			m_pCur_money.SetPos(m_pBack->Xpos+216, m_pBack->Ypos+70);
			m_pPay_money.SetPos(m_pBack->Xpos+216, m_pBack->Ypos+112);

			m_iPetEgg_race = g_pRoh->m_Race;
		}
		break;
	case PetShop_Status__Rabi:
	case PetShop_Status__Pera:
		{
			m_Btn_Ok.SetFileName("Petshop/btn_newpet");
			m_Btn_Cancel.SetFileName("common/btn_cancel_02");
			m_Btn_Ok.LoadRes();
			m_Btn_Cancel.LoadRes();

			m_Btn_Ok.SetPosition(m_pBack->Xpos+100, m_pBack->Ypos+180);
			m_Btn_Cancel.SetPosition(m_pBack->Xpos+180, m_pBack->Ypos+180);
		}
		break;
	}
}

void CControlPetShop2::DeleteRes()
{
	SAFE_DELETE(m_pBack);

	for( int i= 0 ; i < NUM_RACE ; i ++ )
		SAFE_DELETE(m_pSurPetEgg[i]);

	m_CloseBtn.DeleteRes();
	m_BuyBtn.DeleteRes();

	m_pCur_money.DeleteRes();
	m_pPay_money.DeleteRes();
	m_Btn_Ok.DeleteRes();
	m_Btn_Cancel.DeleteRes();
}
BOOL CControlPetShop2::IsInside(int x, int y)
{
	if(!m_pBack)
		return false;

	return m_pBack->IsIn(x, y);
}
void CControlPetShop2::SendChMsg()
{
	char tmp[100] = "";
	sprintf(tmp, "buy_egg %d\n", g_pRoh->m_CurPackNum);

	if (g_pTcpIp)
		g_pTcpIp->SendNetMessage(tmp);
}
void CControlPetShop2::SelectPetEgg()
{
	m_iPetEgg_race = 0;
	return ;
}
void CControlPetShop2::Draw()
{
	if(!m_bActive || !g_pDisplay || !m_pBack )
		return;

	g_pDisplay->Blt(m_pBack->Xpos, m_pBack->Ypos, m_pBack);
	m_CloseBtn.Draw();

	switch( m_petshop_status )
	{
	case PetShop_Status__None:
		{
			if( m_pSurPetEgg[m_iPetEgg_race] )
				g_pDisplay->Blt(m_pSurPetEgg[m_iPetEgg_race]->Xpos, m_pSurPetEgg[m_iPetEgg_race]->Ypos, m_pSurPetEgg[m_iPetEgg_race]);

			m_pCur_money.Draw(105);
			m_pPay_money.Draw(105);
			m_BuyBtn.Draw();
		}
		break;
	case PetShop_Status__Rabi:
	case PetShop_Status__Pera:
		{
			m_Btn_Ok.Draw(m_pBack->Xpos+100, m_pBack->Ypos+180);
			m_Btn_Cancel.Draw(m_pBack->Xpos+180, m_pBack->Ypos+180);
		}
		break;
	}
}


LRESULT CControlPetShop2::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int x, y;

	if(!m_bActive)
		return -1;

	x = LOWORD (lParam);
	y = HIWORD (lParam);

	switch (msg)
	{

	case WM_LBUTTONDOWN:
		x = LOWORD (lParam);
		y = HIWORD (lParam);

		if(!IsInside(x, y))
			return 2;

		switch( m_petshop_status )
		{
		case PetShop_Status__None:
			{
				m_CloseBtn.MsgProc(hWnd, msg, wParam, lParam);
				m_BuyBtn.MsgProc(hWnd, msg, wParam, lParam);
			}
			break;
		case PetShop_Status__Rabi:
		case PetShop_Status__Pera:
			{
				m_Btn_Ok.MsgProc(hWnd, msg, wParam, lParam);
				m_Btn_Cancel.MsgProc(hWnd, msg, wParam, lParam);
			}
			break;
		}
		return 1;

	case WM_LBUTTONUP:
		x = LOWORD (lParam);
		y = HIWORD (lParam);

		switch( m_petshop_status )
		{
		case PetShop_Status__None:
			{
				m_CloseBtn.MsgProc(hWnd, msg, wParam, lParam);
				m_BuyBtn.MsgProc(hWnd, msg, wParam, lParam);

				if(m_CloseBtn.GetState() == BTN_ACTION)
				{
					//닫기
					m_CloseBtn.SetState(BTN_NORMAL);
					return 2;
				}
				else if(m_BuyBtn.GetState() == BTN_ACTION)
				{
					if( g_MyPetStatus.mode == MYPET_MODE_NONE )
					{
						int petpay;
						if (g_dwClientCountry == CTRY_IDN)
							petpay = 2000000;
						else if (g_dwClientCountry == CTRY_JPN
								 || g_dwClientCountry == CTRY_TWN
								 || g_dwClientCountry == CTRY_CN)
							petpay = 500000;
						else
							petpay = 1000000;

						if (g_pRoh && g_pRoh->m_Level < 90)
						{
							//-- IDS_PETSHOP_90LEVEL : 90레벨부터 애완동물을 키울 수 있습니다.
							g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_PETSHOP_90LEVEL), TYPE_NOR_OK, 1 );
							return 1;
						}
						else if (g_pRoh && g_pRoh->m_Money < petpay)
						{
							//-- IDS_PETKEEP_NOTENOUGH_MONEY : 보유금액이 부족합니다.
							g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_PETKEEP_NOTENOUGH_MONEY), TYPE_NOR_OK, 1 );
							return 1;
						}
						else
						{
							SendChMsg();
							return 2;
						}
					}
					else
					{
						//-- IDS_PETSHOP_UNABLE_BUY : 더이상 구매하실 수 없습니다
						g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_PETSHOP_UNABLE_BUY), TYPE_NOR_OK, 1 );
						return 1;
					}
					m_BuyBtn.SetState(BTN_NORMAL);
					return 1;
				}
			}
			break;
		// 현재 라비용 상태임
		case PetShop_Status__Rabi:
			{
				m_Btn_Ok.MsgProc(hWnd, msg, wParam, lParam);
				m_Btn_Cancel.MsgProc(hWnd, msg, wParam, lParam);

				if(m_Btn_Ok.m_iBtnFlg == BTN_ACTION)
				{
					char tmp[100] = "";
					if( g_pRoh )
					{
						sprintf(tmp, "p_drop %d\n", g_pRoh->m_CurPackNum);
						if (g_pTcpIp)
							g_pTcpIp->SendNetMessage(tmp);
					}

					m_bIsOpenWindow2 = FALSE;
					m_Btn_Ok.m_iBtnFlg = BTN_NORMAL;
					return 2;
				}
				else if(m_Btn_Cancel.m_iBtnFlg == BTN_ACTION)
				{
					m_bIsOpenWindow2 = FALSE;
					m_Btn_Cancel.m_iBtnFlg = BTN_NORMAL;
					return 2;
				}
			}
			break;
		// 현재 페라곤 상태임
		case PetShop_Status__Pera:
			{
				m_Btn_Ok.MsgProc(hWnd, msg, wParam, lParam);
				m_Btn_Cancel.MsgProc(hWnd, msg, wParam, lParam);

				if(m_Btn_Ok.m_iBtnFlg == BTN_ACTION)
				{
					if( g_pRoh )
					{
						CItem* pItem = NULL;
						int num_item = 0;
						pItem = g_pRoh->m_ExgInven;

						while (pItem)
						{
							int vnum = pItem->m_Vnum;

							if( vnum != 1454 )
							{
								//-- IDS_GAMBLE_INCLUDE_NOTCHIP : 다른종류의 아이템입니다
								g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_GAMBLE_INCLUDE_NOTCHIP), TYPE_NOR_OK, 1 );
								return -1;
							}
							else
							{
								num_item++;
							}
							pItem = pItem->m_Next;
						}


						if( num_item != 3 ) // 여의주 3개 가 아니면..
						{
							//-- IDS_NEED_ONE_DRAGONBALL : 교환창에 여의주 아이템 하나만 놓아주세요.
							g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_NEED_THREE_DRAGONBALL), TYPE_NOR_OK, 1 );

							return 1;
						}

						if (g_pTcpIp)
							g_pTcpIp->SendNetMessage("p_pdrop 0\n");
					}
					m_Btn_Ok.m_iBtnFlg = BTN_NORMAL;
					return 2;
				}
				else if(m_Btn_Cancel.m_iBtnFlg == BTN_ACTION)
				{
					m_Btn_Cancel.m_iBtnFlg = BTN_NORMAL;
					return 2;
				}
			}
			break;
		}
		break;

	case WM_MOUSEMOVE:
		x = LOWORD (lParam);
		y = HIWORD (lParam);

		switch( m_petshop_status )
		{
		case PetShop_Status__None:
			{
				m_CloseBtn.MsgProc(hWnd, msg, wParam, lParam);
				m_BuyBtn.MsgProc(hWnd, msg, wParam, lParam);
			}
			break;
		case PetShop_Status__Rabi:
		case PetShop_Status__Pera:
			{
				m_Btn_Ok.MsgProc(hWnd, msg, wParam, lParam);
				m_Btn_Cancel.MsgProc(hWnd, msg, wParam, lParam);
			}
			break;
		}
		return 1;
	}
	return 0;
}
