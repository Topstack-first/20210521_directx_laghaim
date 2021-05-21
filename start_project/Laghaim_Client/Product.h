#ifndef _PRODUCT_H_
#define _PRODUCT_H_

#include "Config.h"

#include "BasicButton.h"
#include "TextOutBox.h"
#include "AlphaPopup.h"
#include "NkCharacter.h"

class CSurface;
class CTcpIpConnection;
class CItemRender;
class CMsgPopUp;
class CTextOutBox;
class CAlphaPopUp;

class CProduct
{
public:
	HWND	m_hWnd;
	RECT	m_ClientRc;

	CProduct();
	virtual ~CProduct();

	void Init();
	void LoadRes();
	void DeleteRes();
	CItem* Draw();
	void DrawMySlot();
	void DrawOppSlot();
	void DrawItemInfo();
	//void ResetItemInfo() { m_pItemInfo->SetIndex(-1); }
	void SetNkCha(CNkCharacter *pNkCha);
	BOOL IsInside(int x, int y);
	BOOL IsPlayArea(int x, int y);
	void SendMsg();

	// �ŷ�â�� ���̴� ���.
	void SetVisibleMode(BOOL bExch);		// �ŷ����ΰ� �ƴϸ� �Ϲ� ����ΰ�.
	void ShowItemsToChange();
	// ������ ����.
	LRESULT MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
	CSurface *m_pSurfaceBack;
	CSurface *m_pEnableSlot;
	CSurface *m_pDisableSlot;
	CSurface *m_pExistSlot;
	CNkCharacter *m_pNkCha;
	CItem *m_pOverItem;

	CBasicButton m_CancelBtn;

	char m_MsgBuf[256];

	int m_ScreenWidth;
	int m_ScreenHeight;

	CAlphaPopUp *m_pInfo;

	//----���ο� ������
	int m_opp_slotstart_x, m_opp_slotstart_y;
	int m_my_slotstart_x, m_my_slotstart_y;
	int m_cur_click_slot_x, m_cur_click_slot_y;

	BOOL	m_bShowWindow;
	BOOL	m_bWaitingExch;		// ������ ������ ��ٸ��� �ִ� ���̴�.

	// �ŷ� ������� �ӽ� �κ�.
	CItem	*m_ExgOppInven;
	//#define EXG_SLOT_X_NUM  6
	//#define EXG_SLOT_Y_NUM  4
	//int		m_ExgOppSlot[EXG_SLOT_X_NUM][EXG_SLOT_Y_NUM];
	int		m_ExgOppSlot[EXG_SLOT_Y_NUM][EXG_SLOT_X_NUM];

	DWORD m_dwMouseClickTickTime;

	int		GetExgOppSlotVnum(int x, int y);

	void	RemoveAllOppInven();
	void	AssemblySuccess(int vnum, int index, int plus, int special);
	BOOL    AssemblyFail(BOOL bfail);

	// �ŷ��� ���õǴ� �Լ���.
	void	ActivateExch(BOOL bExch = FALSE);
	void	InterCancel(BOOL bSend = FALSE);
};

#endif // _PRODUCT_H_
