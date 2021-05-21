#ifndef _WAR_TEX_H_
#define _WAR_TEX_H_

#include "Config.h"



class CSurface;
class CTextOutBox;
class CBasicButton;
class CMsgPopUp;
class CTcpIpConnection;

class CWarTax
{
public:
	CWarTax();
	virtual ~CWarTax();
	void        GetMsg(int acc, int rate);
	void		Draw();
	LRESULT		MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	BOOL		IsInside(int x, int y);
	BOOL		CheckMoneyStr(char* szMoney);

public:
	CSurface	 * m_pBack;
	CBasicButton * m_pBtn_OutMoney;
	CBasicButton * m_pBtn_ArrowUp;
	CBasicButton * m_pBtn_ArrowDown;
	CBasicButton * m_pBtn_Close;
	CBasicButton * m_pBtn_Door_Open;
	CBasicButton * m_pBtn_Door_Close;
	CTextOutBox  * m_pText_Account; 		// 세금 징수액
	CTextOutBox  * m_pText_MyMoney;			// 보유금액
	CTextOutBox  * m_pText_Rate;	// 세금 징수율

	//CMsgPopUp	 * m_pMsgPopUp;

	char        m_MsgBuf[1024];
	BOOL	    m_bActive;
	int			m_ScreenCx, m_ScreenCy, m_iMainX, m_iMainY;
	int			m_iCurTaxRate;
	int			m_nCastleStatus;
	// 서버 클라이언트 메세지 관련.
	void SendMsgOpen();
	void SendMsgClose();
	void SendMsgPayment(int money);
	void SendMsgRate(int _rate);
	void SendMsgOpenDoor();
	void SendMsgCloseDoor();
	int  m_iAccount, m_iRate, m_iMyMoney;
	BOOL m_bArrowEnable;
	void SetArrowEnable(BOOL bEnable);
	LRESULT MoneyPopupMsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, int nMsg, char* szMoney);
};

#endif // _WAR_TEX_H_



