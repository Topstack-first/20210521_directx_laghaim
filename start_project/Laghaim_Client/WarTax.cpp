#include "stdafx.h"
// WarTax.cpp: implementation of the CWarTax class.
//
//////////////////////////////////////////////////////////////////////




#include "main.h"
#include "Land.h"
#include "basicbutton.h"
#include "textoutbox.h"
#include "msgpopup.h"
#include "headers.h"
#include "nkcharacter.h"
#include "WarTax.h"
#include "tcpipcon.h"
#include "uimgr.h"
#include "AlphaPopUp.h"
#include "g_stringmanager.h"







//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWarTax::CWarTax()
	: m_iMainX(0)
	, m_iMainY(0)
	, m_iCurTaxRate(0)
	, m_iAccount(0)
	, m_iRate(0)
{
	m_bActive = FALSE;
	m_bArrowEnable = TRUE;

	if(!g_pDisplay)
		return;

	// 출금버튼이 눌렸다, sash open 메세지를 보내주자.
	SendMsgOpen();

	RECT rc;
	HWND hWnd =  g_pDisplay->GetHWnd();
	GetClientRect(hWnd, &rc);

	m_ScreenCx = rc.right;
	m_ScreenCy = rc.bottom;

	/////////////////////////////////////////////////////////////////////////
	// background
	m_pBack = NULL;
	g_pDisplay->CreateSurfaceFromBitmap(&m_pBack, "interface/war_tax/war_tax.bmp");
	m_pBack->SetColorKey(RGB(0, 0, 0));
	m_pBack->Xpos = (g_pNk2DFrame->GetClientWidth() - m_pBack->GetWidth())>>1;
	m_pBack->Ypos = (g_pNk2DFrame->GetClientHeight() - m_pBack->GetHeight())>>1;

	m_pBack->SetColorKey( TRANS_COLOR );

	int main_x = m_pBack->Xpos;
	int main_y = m_pBack->Ypos;

	/////////////////////////////////////////////////////////////////////////
	// button
	m_pBtn_OutMoney	= NULL;
	m_pBtn_ArrowUp	 = NULL;
	m_pBtn_ArrowDown = NULL;
	m_pBtn_Close	 = NULL;
	m_pBtn_Door_Open = NULL;
	m_pBtn_Door_Close = NULL;

	m_pBtn_OutMoney		= new CBasicButton();
	m_pBtn_ArrowUp		= new CBasicButton();
	m_pBtn_ArrowDown	= new CBasicButton();
	m_pBtn_Close		= new CBasicButton();
	m_pBtn_Door_Open    = new CBasicButton();
	m_pBtn_Door_Close   = new CBasicButton();


	m_pBtn_OutMoney->SetFileName("war_tax/btn_out");
	m_pBtn_ArrowUp->SetFileName("common/arrowup");
	m_pBtn_ArrowDown->SetFileName("common/arrowdown");
	m_pBtn_Close->SetFileName("common/btn_close_01");
	m_pBtn_Door_Open->SetFileName("war_tax/btn_opendoor");
	m_pBtn_Door_Close->SetFileName("war_tax/btn_closedoor");

	m_pBtn_OutMoney->LoadRes();
	m_pBtn_ArrowUp->LoadRes();
	m_pBtn_ArrowDown->LoadRes();
	m_pBtn_Close->LoadRes();
	m_pBtn_Door_Open->LoadRes();
	m_pBtn_Door_Close->LoadRes();


	m_pBtn_OutMoney->PosX	= main_x+198;
	m_pBtn_OutMoney->PosY	= main_y+28;
	m_pBtn_ArrowUp->PosX	= main_x+183;
	m_pBtn_ArrowUp->PosY	= main_y+122;
	m_pBtn_ArrowDown->PosX	= main_x+183;
	m_pBtn_ArrowDown->PosY	= main_y+122+m_pBtn_ArrowDown->Height;
	m_pBtn_Close->PosX = main_x+288+3;
	m_pBtn_Close->PosY = main_y+5+2;
	m_pBtn_Door_Open->PosX = main_x+198;
	m_pBtn_Door_Open->PosY = main_y+70;
	m_pBtn_Door_Close->PosX = main_x+198;
	m_pBtn_Door_Close->PosY = main_y+70;

	/////////////////////////////////////////////////////////////////////////
	// TextOutBox
	m_pText_Account		= NULL;
	m_pText_MyMoney			= NULL;
	m_pText_Rate  = NULL;

	m_pText_Account = new CTextOutBox();
	m_pText_MyMoney = new CTextOutBox();
	m_pText_Rate = new CTextOutBox();

	m_pText_Account->Init(12, RGB(255, 210, 0),0, 0);
	m_pText_MyMoney->Init(12, RGB(255, 210, 0),0, 0);
	m_pText_Rate->Init(12, RGB(255, 210, 0),0, 0);

	m_iMyMoney = g_pRoh->m_Money;
	m_pText_MyMoney->SetString(m_iMyMoney,true); // 세금 징수액

	// 공성전중에는 사용할 수 없다.
	if(pCMyApp->m_pUIMgr->m_bOnBattle)
	{
		m_pBtn_Door_Open->m_iBtnFlg = BTN_DISABLE;
		m_pBtn_Door_Close->m_iBtnFlg = BTN_DISABLE;
	}

	// 현재 성문의 상태는 ?
	m_nCastleStatus = pCMyApp->m_pUIMgr->m_nCastleDoorState;

	/*
	0 - 둘다 열림3
	3 - 둘다 닫힘0
	1 - 앞문 열림
	2 - 뒷문 열림
	*/

	if(m_nCastleStatus == 0 || m_nCastleStatus == 2)  // 앞문이 닫힌 경우
	{
		m_pBtn_Door_Close->SetVisible(FALSE);
		m_pBtn_Door_Close->SetDisable(true);

	}
	else if(m_nCastleStatus == 1 || m_nCastleStatus == 3) 	// 앞문이 열린경우
	{
		m_pBtn_Door_Open->SetVisible(FALSE);
		m_pBtn_Door_Open->SetDisable(true);
	}

	if (pCMyApp->m_nCurWorldIdx == WORLD_WHITEHORN)  // 화이트혼에서는 아직 문 못닫아.
	{
		m_pBtn_Door_Close->SetVisible(FALSE);
		m_pBtn_Door_Close->SetDisable(true);
		m_pBtn_Door_Open->SetVisible(FALSE);
		m_pBtn_Door_Open->SetDisable(true);
	}
}

void CWarTax::GetMsg(int acc, int rate)
{
	m_pText_Account->SetString(acc,true); // 세금 징수액

	// 서버에서 받아서 처리한다.
	m_pText_Rate->SetString(100+rate,false); // 세금 징수액
	m_iCurTaxRate = rate;
	m_iAccount    = acc;

	m_bActive = TRUE;
}



CWarTax::~CWarTax()
{
	SAFE_DELETE(m_pBack);
	SAFE_DELETE(m_pBtn_OutMoney);
	SAFE_DELETE(m_pBtn_ArrowUp);
	SAFE_DELETE(m_pBtn_ArrowDown);
	SAFE_DELETE(m_pBtn_Close);
	SAFE_DELETE(m_pText_Account);
	SAFE_DELETE(m_pText_MyMoney);
	SAFE_DELETE(m_pText_Rate);
	SAFE_DELETE(m_pBtn_Door_Open);
	SAFE_DELETE(m_pBtn_Door_Close);

	SendMsgClose();
}


// 서버->클라  [cash 금액, 세금비율 (금고를 오픈하거나 그외 정보가 바뀔때 보내준다)]
// 금고를 연다! 서버에 통보해주자
void CWarTax::SendMsgOpen()
{
	strcpy(m_MsgBuf, "cash open\n");
	if (g_pTcpIp)
		g_pTcpIp->SendNetMessage(m_MsgBuf);
}

// 금고를 닫는다!
void CWarTax::SendMsgClose()
{
	strcpy(m_MsgBuf, "cash close\n");
	if (g_pTcpIp)
		g_pTcpIp->SendNetMessage(m_MsgBuf);
}

// 금고에서 돈을 꺼낸다!
void CWarTax::SendMsgPayment(int money)
{
	if(money <= 0)
		return;

	sprintf(m_MsgBuf, "cash account %d\n", money);
	if (g_pTcpIp)
		g_pTcpIp->SendNetMessage(m_MsgBuf);
}

// 세금비율을 조절한다!
void CWarTax::SendMsgRate(int _rate)
{
	if(_rate < -10 || _rate > 20)
		return;

	sprintf(m_MsgBuf, "cash rate %d\n", _rate);
	if (g_pTcpIp)
		g_pTcpIp->SendNetMessage(m_MsgBuf);
}

// 성문 열기
void CWarTax::SendMsgOpenDoor()
{
	if(pCMyApp->m_pUIMgr->m_bOnBattle)
		return;

	strcpy(m_MsgBuf, "cash dopen 0\n");
	if (g_pTcpIp)
	{
		g_pTcpIp->SendNetMessage(m_MsgBuf);
		strcpy(m_MsgBuf, "cash dopen 1\n");
		g_pTcpIp->SendNetMessage(m_MsgBuf);
	}
}

// 성문 닫기
void CWarTax::SendMsgCloseDoor()
{
	if(pCMyApp->m_pUIMgr->m_bOnBattle)
		return;

	strcpy(m_MsgBuf, "cash dclose 0\n");
	if (g_pTcpIp)
	{
		g_pTcpIp->SendNetMessage(m_MsgBuf);
		strcpy(m_MsgBuf, "cash dclose 1\n");
		g_pTcpIp->SendNetMessage(m_MsgBuf);
	}
}





void CWarTax::Draw()
{
	if(!m_bActive)
		return ;

	g_pDisplay->Blt(m_pBack->Xpos, m_pBack->Ypos, m_pBack);
	m_pBtn_OutMoney->Draw();
	if (m_bArrowEnable)
	{
		m_pBtn_ArrowUp->Draw();
		m_pBtn_ArrowDown->Draw();
	}
	m_pBtn_Close->Draw();
	m_pBtn_Door_Open->Draw();
	m_pBtn_Door_Close->Draw();

	m_pText_Account->Draw(m_pBack->Xpos+100, m_pBack->Ypos+38,93);
	m_pText_MyMoney->Draw(m_pBack->Xpos+100, m_pBack->Ypos+80,93);
	m_pText_Rate->Draw(m_pBack->Xpos+100, m_pBack->Ypos+131,79);
}




LRESULT CWarTax::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int x, y;
	//char tmp[80];

	if(!m_bActive)
		return -1;

	x = LOWORD (lParam);
	y = HIWORD (lParam);

	switch ( msg )
	{
	case WM_RBUTTONDOWN:
		if(IsInside(x, y))
		{
			return 1;
		}
		else
		{
			return 0;
		}
		break;
	case WM_LBUTTONDOWN:
		if(IsInside(x, y))
		{
			m_pBtn_OutMoney->MsgProc( hWnd, msg, wParam, lParam );
			if (m_bArrowEnable)
			{
				m_pBtn_ArrowUp->MsgProc(hWnd, msg, wParam, lParam);
				m_pBtn_ArrowDown->MsgProc(hWnd, msg, wParam, lParam);
			}
			m_pBtn_Close->MsgProc(hWnd, msg, wParam, lParam);
			m_pBtn_Door_Open->MsgProc(hWnd, msg, wParam, lParam);
			m_pBtn_Door_Close->MsgProc(hWnd, msg, wParam, lParam);
			return 4;
		}
		break;
	case WM_LBUTTONUP:
		if(IsInside(x, y))
		{
			m_pBtn_OutMoney->MsgProc( hWnd, msg, wParam, lParam );
			if (m_bArrowEnable)
			{
				m_pBtn_ArrowUp->MsgProc(hWnd, msg, wParam, lParam);
				m_pBtn_ArrowDown->MsgProc(hWnd, msg, wParam, lParam);
			}
			m_pBtn_Close->MsgProc(hWnd, msg, wParam, lParam);
			m_pBtn_Door_Open->MsgProc(hWnd, msg, wParam, lParam);
			m_pBtn_Door_Close->MsgProc(hWnd, msg, wParam, lParam);

			if(m_pBtn_OutMoney->m_iBtnFlg == BTN_ACTION)
			{
				//-- IDS_TAX_WITHDRAWAL : 출금할 금액을 입력하세요
				g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_TAX_WITHDRAWAL), TYPE_EXCHANGE, 24 );
				return 1;
			}

			// 세금징수율을 조절한다. 조절함과 동시에 cash rate 메세지를 보내주자!
			else if(m_pBtn_ArrowUp->m_iBtnFlg == BTN_ACTION)
			{
				if(m_iCurTaxRate<20)
				{
					SendMsgRate(m_iCurTaxRate+5);
				}
				return 1;
			}
			else if(m_pBtn_ArrowDown->m_iBtnFlg == BTN_ACTION)
			{
				if(m_iCurTaxRate> -10)
				{
					SendMsgRate(m_iCurTaxRate-5);
				}
				return 1;
			}
			else if(m_pBtn_Close->m_iBtnFlg == BTN_ACTION)
			{
				return 2;
			}
			else if(m_pBtn_Door_Open->m_iBtnFlg == BTN_ACTION)
			{
				SendMsgOpenDoor();
				return 2;
			}
			else if(m_pBtn_Door_Close->m_iBtnFlg == BTN_ACTION)
			{
				SendMsgCloseDoor();
				return 2;
			}

			m_pBtn_OutMoney->m_iBtnFlg = BTN_NORMAL;
			m_pBtn_ArrowUp->m_iBtnFlg = BTN_NORMAL;
			m_pBtn_ArrowDown->m_iBtnFlg = BTN_NORMAL;
			m_pBtn_Close->m_iBtnFlg = BTN_NORMAL;
			m_pBtn_Door_Open->m_iBtnFlg = BTN_NORMAL;
			m_pBtn_Door_Close->m_iBtnFlg = BTN_NORMAL;

		}
		break;
	case WM_MOUSEMOVE:
		if(IsInside(x, y))
		{
			m_pBtn_OutMoney->MsgProc( hWnd, msg, wParam, lParam );
			if (m_bArrowEnable)
			{
				m_pBtn_ArrowUp->MsgProc(hWnd, msg, wParam, lParam);
				m_pBtn_ArrowDown->MsgProc(hWnd, msg, wParam, lParam);
			}
			m_pBtn_Close->MsgProc(hWnd, msg, wParam, lParam);
			m_pBtn_Door_Open->MsgProc(hWnd, msg, wParam, lParam);
			m_pBtn_Door_Close->MsgProc(hWnd, msg, wParam, lParam);
			return 1;
		}
		break;
	}

	return 0;
}


BOOL CWarTax::IsInside(int x, int y)
{
	if( !m_pBack )
		return false;

	return m_pBack->IsIn(x, y);
}



#define is_space(c)		(c == 0x20 || (c >= 0x09 && c <= 0x0D))

BOOL CWarTax::CheckMoneyStr(char* szMoney)
{

	// 입력된 스트링을 검사한다
	int len = strlen(szMoney);
	for(int i=0; i<len; i++)
	{
		if(isdigit(szMoney[i]) == FALSE)
		{
			if(is_space(szMoney[i]) ==TRUE)	// 1. 공백문자가 입력된경우
				//-- IDS_TAX_NO_BLANK : 공백문자는 입력할 수 없습니다.
				g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_TAX_NO_BLANK), TYPE_EXCHANGE, 24 );
			else	// 2. 숫자 이외의 문자열이 입력된경우
				//-- IDS_TAX_NO_SPECIAL_CHAR : 숫자 이외의 문자는 입력할 수 없습니다.
				g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_TAX_NO_SPECIAL_CHAR), TYPE_EXCHANGE, 24 );

			return FALSE;
		}
	}
	// 3. 현제 가지구 있는 보유금액보다 입력된 금액이 더큰경우
	int nMoney = atoi(szMoney);

	if (nMoney < 0)
	{
		//-- IDS_TAX_SCARCE_MONEY : 세금징수 보유금액이 부족합니다.
		g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_TAX_SCARCE_MONEY), TYPE_EXCHANGE, 24 );
		return FALSE;
	}

	if(m_iAccount < nMoney)
	{
		//-- IDS_TAX_SCARCE_MONEY : 세금징수 보유금액이 부족합니다.
		g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_TAX_SCARCE_MONEY), TYPE_EXCHANGE, 24 );
		return FALSE;
	}
	return TRUE;
}

void CWarTax::SetArrowEnable(BOOL bEnable)
{
	m_bArrowEnable = bEnable;
	if (bEnable)
	{
		m_pBtn_ArrowUp->m_iBtnFlg = BTN_NORMAL;
		m_pBtn_ArrowDown->m_iBtnFlg = BTN_NORMAL;
	}
	else
	{
		m_pBtn_ArrowUp->m_iBtnFlg = BTN_DISABLE;
		m_pBtn_ArrowDown->m_iBtnFlg = BTN_DISABLE;
	}
}

LRESULT CWarTax::MoneyPopupMsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, int nMsg, char* szMoney)
{
	// "확인" 버튼이 눌린경우
	if(nMsg == 1)
	{
		if(CheckMoneyStr(szMoney) == TRUE)
		{
			// 세금징수 보유금액이 모자라지 않고 정상적인 출금금액이 입력된경우
			// 서보로 cash account 메세지를 보내준다.
			char ttmp[20] = "";
			int nMoney = atoi(szMoney);
			SendMsgPayment(nMoney);
			m_iMyMoney +=nMoney;
			m_pText_MyMoney->SetString(m_iMyMoney, true); // 세금 징수액
		}
	}

	return 0L;
}