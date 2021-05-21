#include "stdafx.h"
#include "headers.h"
#include "main.h"
#include "UIMgr.h"
#include "Nk2DFrame.h"
#include "NKCharacter.h"
#include "BasicButton.h"
#include "TcpIpCon.h"
#include "Land.h"
#include "g_stringmanager.h"

#include "DmitronBattleTax.h"
#include "window_resource.h"

#include <time.h>

extern char				commOutBuf[];

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

int CDmitronBattleTax::m_iZone = -1;

CDmitronBattleTax::CDmitronBattleTax()
	: m_bArrowEnable(FALSE)
	, m_iCurTaxRate(0)
	, m_iAccount(0)
	, m_iMyMoney(0)
	, m_bActive(FALSE)
{
	memset(m_MsgBuf, 0, sizeof(m_MsgBuf));

	m_pBtn_OutMoney = 0;
	m_pBtn_ArrowUp = 0;
	m_pBtn_ArrowDown = 0;
	m_pBtn_Close = 0;
	m_pBtn_Door_Open = 0;
	m_pBtn_Door_Close = 0;
	m_pText_Account = 0; 	// 세금 징수액
	m_pText_MyMoney = 0;		// 보유금액
	m_pText_Rate = 0;		// 세금 징수율
}

CDmitronBattleTax::~CDmitronBattleTax()
{
	DeleteRes();
}

void CDmitronBattleTax::SetArrowEnable(BOOL bEnable)
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

int CDmitronBattleTax::Draw()
{
	if( g_pDisplay && m_pBackScene)
	{
		UpdateLayout();

		g_pDisplay->Blt(m_pBackScene->Xpos, m_pBackScene->Ypos, m_pBackScene);

		m_pBtn_OutMoney->Draw();
		if (m_bArrowEnable)
		{
			m_pBtn_ArrowUp->Draw();
			m_pBtn_ArrowDown->Draw();
		}
		m_pBtn_Close->Draw();

		m_pText_Account->Draw(m_pBackScene->Xpos+100, m_pBackScene->Ypos+38,93);
		m_pText_MyMoney->Draw(m_pBackScene->Xpos+100, m_pBackScene->Ypos+80,93);
		m_pText_Rate->Draw(m_pBackScene->Xpos+100, m_pBackScene->Ypos+131,79);
	}

	return -1;
}

void CDmitronBattleTax::DeleteRes()
{
	SAFE_DELETE(m_pBtn_OutMoney);
	SAFE_DELETE(m_pBtn_ArrowUp);
	SAFE_DELETE(m_pBtn_ArrowDown);
	SAFE_DELETE(m_pBtn_Close);

	SAFE_DELETE(m_pText_Account);
	SAFE_DELETE(m_pText_MyMoney);
	SAFE_DELETE(m_pText_Rate);
}

void CDmitronBattleTax::RestoreSurfaces()
{
}

BOOL CDmitronBattleTax::IsInside(int x, int y)
{
	return CUIBaseWindow::IsInside(x, y);
}

BOOL CDmitronBattleTax::InitMode()
{
	return TRUE;
}

void CDmitronBattleTax::LoadRes()
{
	SetBackground(WBK_DMITRON_TAX);

	m_pText_Account		= new CTextOutBox();
	m_pText_MyMoney		= new CTextOutBox();
	m_pText_Rate		= new CTextOutBox();

	m_pText_Account->Init(12, RGB(255, 210, 0),0, 0);
	m_pText_MyMoney->Init(12, RGB(255, 210, 0),0, 0);
	m_pText_Rate->Init(12, RGB(255, 210, 0),0, 0);

	m_pBtn_OutMoney		= new CBasicButton();
	m_pBtn_ArrowUp		= new CBasicButton();
	m_pBtn_ArrowDown	= new CBasicButton();
	m_pBtn_Close		= new CBasicButton();

	m_pBtn_OutMoney->SetFileName("war_tax/btn_out");
	m_pBtn_ArrowUp->SetFileName("common/arrowup");
	m_pBtn_ArrowDown->SetFileName("common/arrowdown");
	m_pBtn_Close->SetFileName("common/btn_close_01");

	m_pBtn_OutMoney->LoadRes();
	m_pBtn_ArrowUp->LoadRes();
	m_pBtn_ArrowDown->LoadRes();
	m_pBtn_Close->LoadRes();

	SetArrowEnable(TRUE);

	UpdateLayout();
}

LRESULT CDmitronBattleTax::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if(!IsInside(LOWORD(lParam), HIWORD(lParam)))
		return IM_NONE;

	if (m_pBtn_OutMoney->IsDisable() == FALSE)
		m_pBtn_OutMoney->MsgProc(hWnd, msg, wParam, lParam);

	if (m_pBtn_ArrowUp->IsDisable() == FALSE)
		m_pBtn_ArrowUp->MsgProc(hWnd, msg, wParam, lParam);

	if (m_pBtn_ArrowDown->IsDisable() == FALSE)
		m_pBtn_ArrowDown->MsgProc(hWnd, msg, wParam, lParam);

	if (m_pBtn_Close->IsDisable() == FALSE)
		m_pBtn_Close->MsgProc(hWnd, msg, wParam, lParam);

	switch( msg )
	{
	case WM_LBUTTONUP:
		if (m_pBtn_Close->m_iBtnFlg == BTN_ACTION)
			return IM_CLOSE;

		if (m_pBtn_ArrowUp->m_iBtnFlg == BTN_ACTION)
		{
			m_pBtn_ArrowUp->m_iBtnFlg = BTN_NORMAL;
			if (m_iCurTaxRate < 100)
			{
				if(g_pTcpIp)
				{
					SendMsgRate(m_iCurTaxRate+5);
				}
			}
		}

		if (m_pBtn_ArrowDown->m_iBtnFlg == BTN_ACTION)
		{
			m_pBtn_ArrowDown->m_iBtnFlg = BTN_NORMAL;
			if (m_iCurTaxRate > -10)
			{
				if(g_pTcpIp)
				{
					SendMsgRate(m_iCurTaxRate-5);
				}
			}
		}

		if (m_pBtn_OutMoney->m_iBtnFlg == BTN_ACTION)
		{
			m_pBtn_OutMoney->m_iBtnFlg = BTN_NORMAL;
			if (g_pTcpIp)
			{
				//-- IDS_TAX_WITHDRAWAL : 출금할 금액을 입력하세요
				g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_TAX_WITHDRAWAL), TYPE_EXCHANGE, POPUP_DMITRON_TAX_PAYMENT );
				return 1;
			}
		}
	}

	return CUIBaseWindow::MsgProc(hWnd, msg, wParam, lParam);
}

void CDmitronBattleTax::UpdateLayout()
{
	m_pBackScene->Xpos = (g_pNk2DFrame->GetClientWidth() - m_pBackScene->GetWidth())>>1;
	m_pBackScene->Ypos = (g_pNk2DFrame->GetClientHeight() - m_pBackScene->GetHeight())>>1;

	m_nPosX = m_pBackScene->Xpos;
	m_nPosY = m_pBackScene->Ypos;

	m_pBtn_OutMoney->PosX	= m_pBackScene->Xpos+198;
	m_pBtn_OutMoney->PosY	= m_pBackScene->Ypos+28;
	m_pBtn_ArrowUp->PosX	= m_pBackScene->Xpos+183;
	m_pBtn_ArrowUp->PosY	= m_pBackScene->Ypos+122;
	m_pBtn_ArrowDown->PosX	= m_pBackScene->Xpos+183;
	m_pBtn_ArrowDown->PosY	= m_pBackScene->Ypos+122+m_pBtn_ArrowDown->Height;
	m_pBtn_Close->PosX = m_pBackScene->Xpos+288+3;
	m_pBtn_Close->PosY = m_pBackScene->Ypos+5+2;

}

void CDmitronBattleTax::InitVariables(int acc, int rate)
{
	char tmp[300];
	m_pText_Account->SetString(itoa(acc, tmp, 10)); // 세금 징수액

	// 서버에서 받아서 처리한다.
	sprintf(tmp, "%d%", 100+rate);
	m_pText_Rate->SetString(tmp); // 세금 징수액

	m_iMyMoney = g_pRoh->m_Money;
	m_pText_MyMoney->SetString(itoa(m_iMyMoney, tmp, 10)); // 세금 징수액


	m_iCurTaxRate = rate;
	m_iAccount    = acc;

	m_bActive = TRUE;
}

void CDmitronBattleTax::SendMsgPayment(int money)
{
	if(money <= 0)
		return;

	sprintf(commOutBuf, "occp cash account %d %d\n", m_iZone, money);
	g_pTcpIp->SendNetMessage(commOutBuf);
}

// 세금비율을 조절한다!
void CDmitronBattleTax::SendMsgRate(int _rate)
{
	if(_rate < -10 || _rate > 100)
		return;

	sprintf(commOutBuf, "occp cash rate %d %d\n", m_iZone, _rate);
	g_pTcpIp->SendNetMessage(commOutBuf);
}

#define is_space(c)		(c == 0x20 || (c >= 0x09 && c <= 0x0D))

BOOL CDmitronBattleTax::CheckMoneyStr(char* szMoney)
{
	// 입력된 스트링을 검사한다
	int len = strlen(szMoney);
	for(int i=0; i<len; i++)
	{
		if(isdigit(szMoney[i]) == FALSE)
		{
			if(is_space(szMoney[i]) ==TRUE)	// 1. 공백문자가 입력된경우
				//-- IDS_TAX_NO_BLANK : 공백문자는 입력할 수 없습니다.
				g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_TAX_NO_BLANK), TYPE_EXCHANGE, POPUP_DMITRON_TAX_PAYMENT );
			else	// 2. 숫자 이외의 문자열이 입력된경우
				//-- IDS_TAX_NO_SPECIAL_CHAR : 숫자 이외의 문자는 입력할 수 없습니다.
				g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_TAX_NO_SPECIAL_CHAR), TYPE_EXCHANGE, POPUP_DMITRON_TAX_PAYMENT );

			return FALSE;
		}
	}
	// 3. 현제 가지구 있는 보유금액보다 입력된 금액이 더큰경우
	int nMoney = atoi(szMoney);

	if (nMoney < 0)
	{
		//-- IDS_TAX_SCARCE_MONEY : 세금징수 보유금액이 부족합니다.
		g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_TAX_SCARCE_MONEY), TYPE_EXCHANGE, POPUP_DMITRON_TAX_PAYMENT );
		return FALSE;
	}

	if(m_iAccount < nMoney)
	{
		//-- IDS_TAX_SCARCE_MONEY : 세금징수 보유금액이 부족합니다.
		g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_TAX_SCARCE_MONEY), TYPE_EXCHANGE, POPUP_DMITRON_TAX_PAYMENT );
		return FALSE;
	}
	return TRUE;
}

LRESULT CDmitronBattleTax::MoneyPopupMsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, int nMsg, char* szMoney)
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
			m_pText_MyMoney->SetString(itoa(m_iMyMoney+=nMoney, ttmp, 10)); // 세금 징수액
		}
	}

	return 0L;
}

void CDmitronBattleTax::SetSubType(int nType)
{
	m_iZone = nType;
}