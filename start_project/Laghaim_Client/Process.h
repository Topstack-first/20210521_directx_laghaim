#ifndef _PROCESS_H_
#define _PROCESS_H_

#include "Config.h"


#include "ddutil.h"
#include "TextOutBox.h"
#include "NkCharacter.h"
#include "Nk2DFrame.h"

class CDisplay;
class CSurface;
class CBasicButton;
class CItemRender;
class CMsgPopUp;
class CAlphaPopUp;
class CTextOutBox;

#define MS_TEXT_SIZE			12
#define MS_TEXT_COLOR			RGB(255, 0, 0)
#define MS_BAKC_COLOR         RGB(47, 0, 17)

class CProcess
{
public:
	HWND			m_hWnd;
	RECT			m_ClientRc;
	CSurface	*   m_pBack;
	CSurface	*   m_pInfo_A;
	CSurface	*   m_pInfo_B;
	CBasicButton*	m_pBtnCancel;	 // "cancle" 버튼
	CBasicButton*	m_pBtnExecute;	 // "가공" 버튼
	CBasicButton*   m_pBtnMoney;

	CAlphaPopUp*    m_pAlphaPopUp;

	BOOL    m_bMainActive;
	int		m_nType;				 // Is it for the applicant? Is it for processing?
	BOOL    m_bClick, m_bMove, m_bMoveEnd;

	CNkCharacter *m_pNkCha;
	CSurface *m_pEnableSlot;
	CSurface *m_pDisableSlot;
	CSurface *m_pExistSlot;
	CItemRender		*m_pItemRender;
	CItem			*m_pOverItem;
	CTcpIpConnection *m_pTcpIp;

	int m_my_slotstart_x, m_my_slotstart_y;
	int m_opp_slotstart_x, m_opp_slotstart_y;
	char m_MsgBuf[256];

	// 가공 상대자의 임시 인벤.
	CItem	*m_ProcessOppInven;
	int		m_ProcessOppSlot[EXG_SLOT_X_NUM][EXG_SLOT_Y_NUM];

	char		m_strProcessMoney[12];
	__int64			m_ProcessMoney;
	CTextOutBox m_MoneyText;
	char		m_strOppName[60];
	CTextOutBox *m_pOppNameBox;

public:

	CProcess();
	~CProcess();

	void	LoadRes(int Type, CItemRender *pItemRender);
	void	Load_A();
	void	Load_B();

	void	DeleteRes();
	void	Del_A();
	void	Del_B();
	void	RemoveOppInven();

	CItem*  Draw();
	void    Draw_A();
	void    Draw_B();
	void    DrawMySlot();
	void	DrawOppSlot();

	LRESULT	MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT Message_A( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT Message_B( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	BOOL CheckMoneyStr(char* szMoney);

	void SetNkCha(CNkCharacter *pNkCha)
	{
		m_pNkCha = pNkCha;
	}
	void SetTcpIp(CTcpIpConnection *pTcpIp)
	{
		m_pTcpIp = pTcpIp;
	}
	void SetOppName(char *player_name);
	BOOL IsInside(int x, int y);
	BOOL IsPlayArea(int x, int y);
	void SendMsg();
	void RemoveAllMyInven();

	// 서버에서 날라온 메시지.
	// [6/7/2007 Theodoric] 내구도 수리 :: 함수 인자 추가.
	void ProcessInvenTo(int x, int y, int index, int vnum, int gem_num, int special,
						int UpEndur, int MaxUpdEndur, int Endur, int MaxEndur );		// 삽입.
	void ProcessInvenFrom(int x, int y, int w = 1, int h = 1);						// 삭제.
	void ProcessMoney(char *strMoney);
	void ProcessSuccess(int vnum, int index, int x, int y);
	// 윗쪽 인벤 보조 함수
	int		GetOppSlotIndex(int x, int y);
	int		GetItemIndexInOppSlot(int x, int y, int w, int h);
	CItem*	PickItemInOppSlot(int index, BOOL isVnum=FALSE);

	// 가공 상태 메시지.
	BOOL	m_bProcessCon;
	BOOL	m_bProcessLock;
	BOOL	IsProcessConnection()
	{
		return m_bProcessCon;
	};
	void	CloseConnection();
	void	ResetConnection();
	void	AskProcessRequest(char *player_name);
	LRESULT	PopupMsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, int nMsg);
	LRESULT MoneyPopupMsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, int nMsg, char* szMoney);
};

#endif // _PROCESS_H_
