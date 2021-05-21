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
#include "ControlPetShop.h"
#include "Nk2DFrame.h"
#include "nkmob.h"
#include "nkpet.h"
#include "nkmypet.h"
#include "Country.h"
#include "g_stringmanager.h"

#define PETPAY 1000000







extern DWORD		g_dwLangType;
extern DWORD		g_dwClientCountry;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CControlPetShop::CControlPetShop()
{
	m_pBack			= NULL;
	m_pBack2        = NULL;
	for(int i = 0; i < NUM_RACE; i++ )
		m_pSurPetEgg[i] = NULL;

	m_bActive = TRUE;
	m_iMainX = 0;
	m_iMainY = 0;
	m_ScreenWidth = 0;
	m_ScreenHeight = 0;
	m_iPetEgg_race = 0;
	m_dwLastOpenTime = timeGetTime();
	m_IsOpenWindow	 = FALSE;
	m_bIsOpenWindow2 = FALSE;
}

CControlPetShop::~CControlPetShop()
{
	DeleteRes();
}
void CControlPetShop::LoadRes()
{
	if(!g_pDisplay)
		return;

	RECT rc;
	HWND hWnd = g_pDisplay->GetHWnd();
	GetClientRect(hWnd, &rc);

	m_ScreenWidth = rc.right;
	m_ScreenHeight = rc.bottom;

	int X = 0, Y = 0;
	g_pDisplay->CreateSurfaceFromBitmap(&m_pBack, "interface/Petshop/Pet_Shop.bmp");
	g_pDisplay->CreateSurfaceFromBitmap(&m_pBack2, "interface/Petshop/Pet_Shop2.bmp");
	if( m_pBack )
	{
		m_pBack->SetColorKey(RGB(0, 0, 0));
		m_pBack->Xpos = m_ScreenWidth/2 - m_pBack->GetWidth()/2;
		m_pBack->Ypos = m_ScreenHeight/2 - m_pBack->GetHeight()/2;
		X = m_iMainX = m_pBack->Xpos;
		Y = m_iMainY =m_pBack->Ypos;
	}
	if( m_pBack2 )
	{
		m_pBack2->SetColorKey( TRANS_COLOR );
		m_pBack2->Xpos = X;
		m_pBack2->Ypos = Y;
	}

	g_pDisplay->CreateSurfaceFromBitmap(&m_pSurPetEgg[RACE_BULKAN], "interface/Petshop/Egg_B.bmp");
	g_pDisplay->CreateSurfaceFromBitmap(&m_pSurPetEgg[RACE_KAILIPTON], "interface/Petshop/Egg_K.bmp");
	g_pDisplay->CreateSurfaceFromBitmap(&m_pSurPetEgg[RACE_AIDIA], "interface/Petshop/Egg_A.bmp");
	g_pDisplay->CreateSurfaceFromBitmap(&m_pSurPetEgg[RACE_HUMAN], "interface/Petshop/Egg_H.bmp");
	g_pDisplay->CreateSurfaceFromBitmap(&m_pSurPetEgg[RACE_FREAK], "interface/Petshop/Egg_F.bmp");
	g_pDisplay->CreateSurfaceFromBitmap(&m_pSurPetEgg[RACE_PEROM], "interface/Petshop/Egg_P.bmp");
	for( int i = 0 ; i < NUM_RACE ; i ++ )
	{
		if( m_pSurPetEgg[ i ] )
		{
			m_pSurPetEgg[ i ]->SetColorKey(RGB(0, 0, 0));
			m_pSurPetEgg[ i ]->Xpos = X+60;
			m_pSurPetEgg[ i ]->Ypos = Y+70;
		}
	}
	//==========================================================

	m_CloseBtn.SetFileName("common/btn_close_01");
	m_BuyBtn.SetFileName("common/btn_buy_02");
	m_Btn_Ok.SetFileName("Petshop/btn_newpet");
	m_Btn_Cancel.SetFileName("common/btn_cancel_02");
	m_CloseBtn.LoadRes();
	m_BuyBtn.LoadRes();
	m_Btn_Ok.LoadRes();
	m_Btn_Cancel.LoadRes();

	m_CloseBtn.SetPosition(X+342,Y+5);
	m_BuyBtn.SetPosition(X+230,Y+140);
	m_Btn_Ok.SetPosition(X+100,Y+180);
	m_Btn_Cancel.SetPosition(X+180,Y+180);

	m_pCur_money.Init(12, _FCOLOR_YELLOW,0, 0);
	m_pPay_money.Init(12, _FCOLOR_YELLOW,0, 0);
	m_pCur_money.SetString(g_pRoh->m_Money, true);
	if (g_dwClientCountry == CTRY_IDN)
		m_pPay_money.SetString(2000000, true);
	else if (g_dwClientCountry == CTRY_TWN
			 || g_dwClientCountry == CTRY_CN
			 || g_dwClientCountry == CTRY_JPN)
		m_pPay_money.SetString(500000, true);
	else
		m_pPay_money.SetString(1000000, true);

	m_pCur_money.SetPos(X+216, Y+70);
	m_pPay_money.SetPos(X+216, Y+112);

	m_iPetEgg_race = g_pRoh->m_Race;
}

void CControlPetShop::DeleteRes()
{
	SAFE_DELETE(m_pBack);
	SAFE_DELETE(m_pBack2);
	for( int i= 0 ; i < NUM_RACE ; i ++ )
		SAFE_DELETE(m_pSurPetEgg[i]);

	m_CloseBtn.DeleteRes();
	m_BuyBtn.DeleteRes();

	m_pCur_money.DeleteRes();
	m_pPay_money.DeleteRes();
	m_Btn_Ok.DeleteRes();
	m_Btn_Cancel.DeleteRes();
}
BOOL CControlPetShop::IsInside(int x, int y)
{
	if( !m_pBack )
		return FALSE;

	return m_pBack->IsIn( x, y );
}
void CControlPetShop::SendChMsg()
{
	char tmp[100] = "";
	sprintf(tmp, "buy_egg %d\n", g_pRoh->m_CurPackNum);

	if (g_pTcpIp)
		g_pTcpIp->SendNetMessage(tmp);
}
void CControlPetShop::SelectPetEgg()
{
	m_iPetEgg_race = 0;
	return ;
}
void CControlPetShop::Draw()
{
	if( TRUE != m_bActive || NULL == g_pDisplay)
		return;

	if( TRUE  == m_bIsOpenWindow2 )
	{
		g_pDisplay->Blt(m_pBack2->Xpos, m_pBack2->Ypos, m_pBack2);
		m_Btn_Ok.Draw(m_pBack2->Xpos+100, m_pBack2->Ypos+180);
		m_Btn_Cancel.Draw(m_pBack2->Xpos+180, m_pBack2->Ypos+180);
		return;
	}

	if( NULL != m_pBack )
		g_pDisplay->Blt(m_pBack->Xpos, m_pBack->Ypos, m_pBack);
	if( m_pSurPetEgg[m_iPetEgg_race] )
		g_pDisplay->Blt(m_pSurPetEgg[m_iPetEgg_race]->Xpos, m_pSurPetEgg[m_iPetEgg_race]->Ypos, m_pSurPetEgg[m_iPetEgg_race]);

	m_CloseBtn.Draw();
	m_BuyBtn.Draw();
	m_pCur_money.Draw(105);
	m_pPay_money.Draw(105);

	if(	m_IsOpenWindow == FALSE )
	{
		if ( (timeGetTime() - m_dwLastOpenTime) > 300 )
		{
			m_IsOpenWindow = TRUE;
			m_dwLastOpenTime = timeGetTime();
		}
	}
}


LRESULT CControlPetShop::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
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

		if(!m_bIsOpenWindow2)
		{
			m_CloseBtn.MsgProc(hWnd, msg, wParam, lParam);
			m_BuyBtn.MsgProc(hWnd, msg, wParam, lParam);
		}
		else
		{
			m_Btn_Ok.MsgProc(hWnd, msg, wParam, lParam);
			m_Btn_Cancel.MsgProc(hWnd, msg, wParam, lParam);
		}

		return 1;

	case WM_LBUTTONUP:
		x = LOWORD (lParam);
		y = HIWORD (lParam);

		if(!m_bIsOpenWindow2)
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
				else if( g_MyPetStatus.mode >= MYPET_MODE_COMBAT)
				{
					// "새로운 애완동물 키우기 창" 은 여기에서 ..
					m_bIsOpenWindow2 = TRUE;
					return 1;

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
		else
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

	case WM_MOUSEMOVE:
		x = LOWORD (lParam);
		y = HIWORD (lParam);
		if(!m_bIsOpenWindow2)
		{
			m_CloseBtn.MsgProc(hWnd, msg, wParam, lParam);
			m_BuyBtn.MsgProc(hWnd, msg, wParam, lParam);
		}
		else
		{
			m_Btn_Ok.MsgProc(hWnd, msg, wParam, lParam);
			m_Btn_Cancel.MsgProc(hWnd, msg, wParam, lParam);
		}

		return 1;
	}
	return 0;
}
