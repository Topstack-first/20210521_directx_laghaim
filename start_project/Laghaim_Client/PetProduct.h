#ifndef _PET_PRODUCT_H_
#define _PET_PRODUCT_H_

#include "Config.h"

class CItemRender;
class CAlphaPopUp;
class CSurface;
class CBasicButton;
class CTextOutBox;
class CTcpIpConnection;
class CMsgPopUp;

class CPetProduct  // 버튼처리 삭제 (btn_close제외)
{
public:
	CPetProduct();
	virtual ~CPetProduct();

	HWND	m_hWnd;
	RECT	m_ClientRc;

	char cPetLevel[20];
	char cPetexp[50];

	int m_ScreenWidth;
	int m_ScreenHeight;

	int m_opp_slotstart_x, m_opp_slotstart_y;
	int m_my_slotstart_x, m_my_slotstart_y;
	int m_cur_click_slot_x, m_cur_click_slot_y;

	char m_MsgBuf[256]; // 메시지 버퍼
	BOOL m_bWaitingExch;//// 서버의 응답을 기다리고 있는 중이다.
	BOOL m_bShowWindow;

	int		m_ExgOppSlot[EXG_SLOT_Y_NUM][EXG_SLOT_X_NUM];// 실제 6x4크기의 슬롯(보여지는 크기는 6x2)

	CNkCharacter *m_pNkCha;

	CSurface * m_pBack;
	CSurface * m_pPetBack;

	CSurface *m_pEnableSlot;
	CSurface *m_pDisableSlot;
	CSurface *m_pExistSlot;

	CBasicButton *m_CancelBtn;
	CMsgPopUp   *m_pMsgPopUp;

	CTextOutBox *m_pPetLevel;//레밸
	CTextOutBox *m_pPetExp;//경험치

	CAlphaPopUp *m_pInfo;

	CItem *m_pOverItem;
	// 거래 상대자의 임시 인벤.
	CItem	*m_ExgOppInven;


	void Init();
	void SetNkCha(CNkCharacter *pNkCha);
	// 거래창이 보이는 모습.
	void SetVisibleMode(BOOL bExch);		// 거래중인가 아니면 일반 모드인가.
	void LoadRes();
	BOOL IsInside(int x, int y);
	BOOL IsPlayArea(int x, int y);
	void DrawOppSlot();
	void DrawMySlot();
	CItem* Draw();
	void SendMsg();
	int	GetExgOppSlotVnum(int x, int y);

	void ShowItemsToChange();

	void DeleteRes();
	void RemoveAllOppInven();
	// 거래에 관련되는 함수들. // 실제로 사용되는 힘수인지의문 스럾다,
	void	ActivateExch(BOOL bExch = FALSE);
	void	InterCancel(BOOL bSend = FALSE);

	LRESULT MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

	void	AssemblySuccess(int vnum, int index, int slot_x, int slot_y);
	BOOL    AssemblyFail(BOOL bfail);
};

#endif // _PET_PRODUCT_H_
