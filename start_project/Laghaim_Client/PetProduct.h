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

class CPetProduct  // ��ưó�� ���� (btn_close����)
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

	char m_MsgBuf[256]; // �޽��� ����
	BOOL m_bWaitingExch;//// ������ ������ ��ٸ��� �ִ� ���̴�.
	BOOL m_bShowWindow;

	int		m_ExgOppSlot[EXG_SLOT_Y_NUM][EXG_SLOT_X_NUM];// ���� 6x4ũ���� ����(�������� ũ��� 6x2)

	CNkCharacter *m_pNkCha;

	CSurface * m_pBack;
	CSurface * m_pPetBack;

	CSurface *m_pEnableSlot;
	CSurface *m_pDisableSlot;
	CSurface *m_pExistSlot;

	CBasicButton *m_CancelBtn;
	CMsgPopUp   *m_pMsgPopUp;

	CTextOutBox *m_pPetLevel;//����
	CTextOutBox *m_pPetExp;//����ġ

	CAlphaPopUp *m_pInfo;

	CItem *m_pOverItem;
	// �ŷ� ������� �ӽ� �κ�.
	CItem	*m_ExgOppInven;


	void Init();
	void SetNkCha(CNkCharacter *pNkCha);
	// �ŷ�â�� ���̴� ���.
	void SetVisibleMode(BOOL bExch);		// �ŷ����ΰ� �ƴϸ� �Ϲ� ����ΰ�.
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
	// �ŷ��� ���õǴ� �Լ���. // ������ ���Ǵ� ���������ǹ� ������,
	void	ActivateExch(BOOL bExch = FALSE);
	void	InterCancel(BOOL bSend = FALSE);

	LRESULT MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

	void	AssemblySuccess(int vnum, int index, int slot_x, int slot_y);
	BOOL    AssemblyFail(BOOL bfail);
};

#endif // _PET_PRODUCT_H_
