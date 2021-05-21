#ifndef _GUILD_STOREHOUSE_
#define _GUILD_STOREHOUSE_

#include "Config.h"

#define GUILD_STORE_PACK_NUM		10

#define GUILD_STORE_PACK_INTERVAL	20
#define GUILD_STORE_SLOT_X_NUM		4
#define GUILD_STORE_SLOT_Y_NUM		3
#define GUILD_STORE_SLOT_WIDTH		54
#define GUILD_STORE_SLOT_HEIGHT		80


class CSurface;
class CBasicButton;
class CTcpIpConnection;
class CItemRender;
class CItem;
class CMsgPopUp;
class CTextOutBox;

class CControlGuildSHouse
{
protected:
	HWND	m_hWnd;
	RECT	m_ClientRc;

	// �⺻ �̹���.
	CSurface *m_pBack;			// �κ� �̹���
	CSurface *m_pItemBack;
	CSurface *m_pMoneyBk;		// �׸��⸸ �Ѵ� �ǹ� ����

	CBasicButton m_InMoneyBtn;	// �Ա� ��ư
	CBasicButton m_OutMoneyBtn;	// ���
	CBasicButton m_CloseBtn;	// ����

	CBasicButton* m_pBtnTab;

	CSurface* m_pSlot;

	// Helper Class
	CNkCharacter	*m_pNkCha;

	// ���º���
	char m_MsgBuf[1024];
	BOOL m_bActivate;
	BOOL m_bGetStashEnd;

	// nate : â�� ������ �ټ� �̵��� ������ Ÿ��
	BOOL m_bWareLock;
	DWORD m_timeWareLock;
	void SetWareLock( BOOL bLock );

	int m_wareslot_start_x, m_wareslot_start_y;
	int m_ScreenWidth;
	int m_ScreenHeight;

	// â�� ����
	CItem *m_WareInven[GUILD_STORE_PACK_NUM][GUILD_STORE_SLOT_X_NUM][GUILD_STORE_SLOT_Y_NUM];
	int m_WareSlot[GUILD_STORE_PACK_NUM][GUILD_STORE_SLOT_X_NUM][GUILD_STORE_SLOT_Y_NUM];		// slot�� ���ڰ� ����Ű�� ���� ����.
	__int64	m_nSaveMoney;
	int m_nPackNum;

	int	m_using_store;

	void SendMsg();

public:

	CControlGuildSHouse();
	~CControlGuildSHouse();

	void Init();
	void LoadRes();
	void SetNkCha(CNkCharacter *pNkCha)
	{
		m_pNkCha = pNkCha;
	};
	void DeleteRes();
	CItem *Draw();

	LRESULT MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
	BOOL IsInside(int x, int y);
	void IsInsideBtn(int nBtNum, int x, int y);
	BOOL IsPlayArea(int x, int y);

	//â�� ���̴°�
	BOOL IsActivate()
	{
		return m_bActivate;
	};
	void ActivateGStoreHouse(BOOL bActivate, int gLevel = 0);

//=======================================================================================
	// ������� ���� : â�� - From Network
	void WareInvenTo(int x, int y, int index, int vnum, int gem_num, int special);
	void WareInvenFrom(int x, int y, int w = 1, int h = 1);
//=======================================================================================

	// â�� �����ۿ� ���� ���� �Լ�.
	int  GetWareSlotIndex(int x, int y);
	int  GetCurPackNum()
	{
		return m_nPackNum;
	}

	// �����󿡼� �� �޽���.
	void Stash(int slot_num, int vnum, int plus, int special, int special2,
			   int UpEndur, int MaxUpdEndur, int Endur, int MaxEndur, int count, int time_limit ); ///�ð��������� �ð��� �߰�(050711 by ����) ///�Ź���ȭ�ý���
	// [6/7/2007 Theodoric] ������ ���� :: �Լ� ���� �߰�
	void StashTo(int slot_num, int vnum, int plus, int special, int special2,
				 int UpEndur, int MaxUpdEndur, int Endur, int MaxEndur, int count, int swap_flag); ///�Ź���ȭ�ý���
	// [6/7/2007 Theodoric] ������ ���� :: �Լ� ���� �߰�
	void StashFrom(int slot_num, int index, int vnum, int plus, int &special, int special2,
				   int UpEndur, int MaxUpdEndur, int Endur, int MaxEndur, int count); ///�Ź���ȭ�ý���
	void StashEnd();

	// nate
	// ���� : â���ѹ�ȣ â���Թ�ȣ �κ��ѹ�ȣ �ι���ǥ��
	void StashPut( int nVnum, int nInvenPackNum, int nWarePackNum, int nWareSlotNum, int nSlot_X, int nSlot_Y );
	void StashGet( int nItemNum, int nInvenPackNum, int nWarePackNum, int nWareSlotNum, int nItemIndex, int nSlot_X, int nSlot_Y );

	void RemoveAll();

	// ��� ������ ����Ҽ� �ִ� â���� Num ��.
	void setusingstore(int using_store)
	{
		m_using_store = using_store;
	}
	int	 getusingstoreNum() const
	{
		return m_using_store;
	}

	int m_iCurSlot_X;
	int m_iCurSlot_Y;
	CTextOutBox m_MoneyText;
	CTextOutBox m_pNumText[GUILD_STORE_PACK_NUM][GUILD_STORE_SLOT_X_NUM][GUILD_STORE_SLOT_Y_NUM];
	BOOL CheckMoneyStr(char* szMoney);
	char m_strMoney1[12];
	BOOL InMoneyMsg(char* money);
	BOOL OutMoneyMsg(char* money);
	BYTE m_byInOutMoneyState;	//nate 2006-02-23 : ����� ����(0:��Ȱ��ȭ ����, 1:�Ա�, 2:���)
	BOOL m_bMsgWaiting;
	LRESULT MoneyPopupMsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, int nMsg, char* szMoney);
};
#endif // _GUILD_STOREHOUSE_