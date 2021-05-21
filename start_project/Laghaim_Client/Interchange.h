#ifndef _INTERCHANGE_H_
#define _INTERCHANGE_H_

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

class CInterchange
{
public:
	HWND	m_hWnd;
	RECT	m_ClientRc;

	CInterchange();
	virtual ~CInterchange();

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
	BOOL CanInterchange(BOOL bMsg = TRUE);
	void SendMsg();

	// �ŷ�â�� ���̴� ���.
	void SetVisibleMode(BOOL bExch);		// �ŷ����ΰ� �ƴϸ� �Ϲ� ����ΰ�.
	void ShowItemsToChange();
	BOOL ShowItemsToChange2(int idx, int sel, int candi1, int candi2, int candi3, int candi4, int candi5, int candi6, int candi7);

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

	//----���ο� ������
	int m_opp_slotstart_x, m_opp_slotstart_y;
	int m_my_slotstart_x, m_my_slotstart_y;
	int m_cur_click_slot_x, m_cur_click_slot_y;
	DWORD m_dwMouseClickTickTime;

	// �ŷ� ������� �ӽ� �κ�.
	CItem	*m_ExgOppInven;
	int		m_ExgOppSlot[EXG_SLOT_X_NUM][EXG_SLOT_Y_NUM];

	void	ExchInvenTo(int x, int y, int index, int vnum, int gem_num, int special);		// ����.
	int		GetExgOppSlotIndex(int x, int y);
	int		GetItemIndexInOppSlot(int x, int y, int w, int h);
	CItem*	PickItemInOppSlot(int index, BOOL isVnum=FALSE);
	void	ExchInvenFrom(int x, int y, int w = 1, int h = 1);						// ����.
	int		GetExgOppSlotVnum(int x, int y);

	void	RemoveAllOppInven();

	// �ŷ��� ���Ǵ� ������
	BOOL	m_bExch;			// �� ������ true�� ��쿡�� ����Ŭ�� ó���� �Ѵ�.
	BOOL	m_bWaitingExch;		// ������ ������ ��ٸ��� �ִ� ���̴�.

	// �ŷ��� ���õǴ� �Լ���.
	BOOL	IsInterch()
	{
		return m_bExch;
	};
	void	ActivateExch(BOOL bExch = FALSE);
	void	InterChange();
	void	InterCancel(BOOL bSend = FALSE);

	// [6/1/2007 Theodoric] ������ ���� :: �Լ� �Ķ���� ������ ����
	void	InterchangeTo(int old_idx, int new_idx, int vnum, int plus_point, int special, int special2,
						  int UpEndur, int MaxUpdEndur, int Endur, int Maxendur); ///�Ź���ȭ�ý���
	void	RemoveAllMyInven();
	HRESULT PopupMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, int nMsg);
};

#endif // _INTERCHANGE_H_
