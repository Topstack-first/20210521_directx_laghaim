#ifndef DMITRON_BATTLE_TAX_H
#define DMITRON_BATTLE_TAX_H

#include "Config.h"

#include "basewindow.h"
#include "basicbutton.h"

class CDmitronBattleTax : public CUIBaseWindow
{
public:
	CDmitronBattleTax();
	~CDmitronBattleTax();

	/*******************************************************************/
	int	 Draw();
	void DeleteRes();
	void RestoreSurfaces();

	BOOL IsInside(int x, int y);
	BOOL IsMouseUpperWindow(int x, int y)
	{
		return TRUE;
	}

	BOOL	InitMode();
	void	LoadRes();


	LRESULT MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	/*******************************************************************/

	void InitTextOutBox();
	void UpdateLayout();
	void UpdateState();
	void HandleError(int error_code);

	/*******************************************************************/

	void InitVariables(int tax, int rate);
	BOOL m_bArrowEnable;
	void SetArrowEnable(BOOL bEnable);
	/*******************************************************************/

	CBasicButton * m_pBtn_OutMoney;
	CBasicButton * m_pBtn_ArrowUp;
	CBasicButton * m_pBtn_ArrowDown;
	CBasicButton * m_pBtn_Close;
	CBasicButton * m_pBtn_Door_Open;
	CBasicButton * m_pBtn_Door_Close;
	CTextOutBox  * m_pText_Account; 	// 세금 징수액
	CTextOutBox  * m_pText_MyMoney;		// 보유금액
	CTextOutBox  * m_pText_Rate;		// 세금 징수율

	char        m_MsgBuf[1024];

	static int m_iZone;
	int	m_iCurTaxRate;
	int m_iAccount;
	int m_iMyMoney;
	BOOL m_bActive;

	BOOL CheckMoneyStr(char* szMoney);
	LRESULT MoneyPopupMsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, int nMsg, char* szMoney);

	// 세금비율을 조절한다!
	void SendMsgPayment(int money);
	void SendMsgRate(int _rate);
	static void SetSubType(int nType);

};

#endif