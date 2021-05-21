#ifndef REFINE_H
#define REFINE_H

#include "Config.h"


#include "ddutil.h"
#include "TextOutBox.h"
#include "NkCharacter.h"

class CSurface;
class CBasicButton;
class CItemRender;
class CMsgPopUp;
class CAlphaPopUp;
class CTextOutBox;

#define MS_TEXT_SIZE			12
#define MS_TEXT_COLOR			RGB(255, 0, 0)
#define MS_BAKC_COLOR         RGB(47, 0, 17)

class CRefine
{

public:
	HWND			m_hWnd;
	RECT			m_ClientRc;
	CSurface	*   m_pBack;
	CSurface	*   m_pInfo_A;
	CSurface	*   m_pInfo_B;
	CBasicButton*	m_pBtnCancel;	 // "cancle" ��ư
	CBasicButton*	m_pBtnExecute;	 // "����" ��ư
	CBasicButton*   m_pBtnMoney;

	CAlphaPopUp*    m_pAlphaPopUp;

	BOOL    m_bMainActive;
	int		m_nType;				 // ��û�� ���ΰ�? �����ϴ� ��� ���ΰ�?
	BOOL    m_bClick, m_bMove, m_bMoveEnd;

	CNkCharacter *m_pNkCha;
	CSurface *m_pEnableSlot;
	CSurface *m_pDisableSlot;
	CSurface *m_pExistSlot;
	CItemRender		*m_pItemRender;
	CItem			*m_pOverItem;

	int m_my_slotstart_x, m_my_slotstart_y;
	int m_opp_slotstart_x, m_opp_slotstart_y;
	char m_MsgBuf[256];

	// ���� ������� �ӽ� �κ�.
	CItem	*m_MsOppInven;
	int		m_MsOppSlot[EXG_SLOT_X_NUM][EXG_SLOT_Y_NUM];

	char		m_strMsMoney[12];
	int			m_MsMoney;
	CTextOutBox m_MoneyText;
	char		m_strOppName[60];
	CTextOutBox *m_pOppNameBox;

public:
	CRefine();
	~CRefine();

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
	void SetOppName(char *player_name);
	BOOL IsInside(int x, int y);
	BOOL IsPlayArea(int x, int y);
	void SendMsg();
	void RemoveAllMyInven();

	// �������� ����� �޽���.
	// [6/7/2007 Theodoric] ������ ���� :: �Լ� ���� �߰�.
	void MsInvenTo(int x, int y, int index, int vnum, int gem_num, int special,
				   int UpEndur, int MaxUpdEndur, int Endur, int MaxEndur);		// ����.
	void MsInvenFrom(int x, int y, int w = 1, int h = 1);						// ����.
	void MsMoney(char *strMoney);
	void MsSuccess(int clear_flag, int vnum, int index, int x, int y);
	// ���� �κ� ���� �Լ�
	int		GetOppSlotIndex(int x, int y);
	int		GetItemIndexInOppSlot(int x, int y, int w, int h);
	CItem*	PickItemInOppSlot(int index, BOOL isVnum=FALSE);

	// ���� ���� �޽���.
	BOOL	m_bRefineCon;
	BOOL	m_bRefineLock;
	BOOL	IsRefineConnection()
	{
		return m_bRefineCon;
	};
	void	CloseConnection();
	void	ResetConnection();

	// ���̵�ư� ������ �� �� �ִ°� ���°� �˻� ����.
	BOOL	IsRefinableItems();
	void	AskMsRequest(char *player_name);
	LRESULT PopupMsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, int nMsg);
	LRESULT MoneyPopupMsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, int nMsg, char* szMoney);
};

#endif // REFINE_H
