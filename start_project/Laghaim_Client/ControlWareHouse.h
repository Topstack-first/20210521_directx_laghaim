#ifndef _CONTROL_WAREHOUSE_H_
#define _CONTROL_WAREHOUSE_H_

#include "Config.h"

// nate
// â�� ���� Ȯ��
//#define WARE_PACK_NUM		2
#define WARE_PACK_NUM		3

#define WARE_PACK_INTERVAL	20
#define WARE_SLOT_X_NUM		4
#define WARE_SLOT_Y_NUM		3
#define WARE_SLOT_WIDTH		53
#define WARE_SLOT_HEIGHT	83

class CSurface;
class CBasicButton;
class CTcpIpConnection;
class CItemRender;
class CItem;
class CMsgPopUp;
class CTextOutBox;


class CControlWareHouse//:public CTransStrFormat
{
protected:
	HWND	m_hWnd;
	RECT	m_ClientRc;

	// [4/16/2007 Theodoric] �±� ���� â�� :: ���� ������
	CBasicButton	*m_pCashViewBtn;
	CBasicButton	*m_pNormalViewBtn;
	CTextOutBox		*m_pCashDateText;
	BOOL			m_bCash;			// ���� â�� �����ٶ� TRUE
	int				m_nCashMaxPackNum;

	// �⺻ �̹���.
	CSurface *m_pBack;			// ���� �̹���

	CSurface	*m_pStoreWidening_time_back;	// Ȯ��â�� �����ð� ǥ��
	bool		 m_bStoreWidening;				// ����������� ���������(true:���, false:������)

	CBasicButton m_InMoneyBtn;	// �Ա� ��ư
	CBasicButton m_OutMoneyBtn;	// ���
	CBasicButton m_CloseBtn;	// ����

	// nate : â�� ���� Ȯ�� - ��ư �߰�
	CBasicButton m_BtnWindow_1;
	CBasicButton m_BtnWindow_2;
	CBasicButton m_BtnWindow_3;

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
	CItem *m_WareInven[WARE_PACK_NUM][WARE_SLOT_X_NUM][WARE_SLOT_Y_NUM];
	int m_WareSlot[WARE_PACK_NUM][WARE_SLOT_X_NUM][WARE_SLOT_Y_NUM];		// slot�� ���ڰ� ����Ű�� ���� ����.
	__int64	m_nSaveMoney;
	int m_nPackNum;


	void SendMsg();

public:
	bool IsGetList()
	{
		return m_bGetStashEnd?true:false;
	}

	CControlWareHouse();
	~CControlWareHouse();

	// [4/16/2007 Theodoric] �±� ���� â�� :: ���� �Լ���
	void SetCashInfo(int packMaxNumber, int seconds );
	void InvenSlotInfoInit();

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
	BOOL IsPlayArea(int x, int y);

	//â�� ���̴°�
	BOOL IsActivate()
	{
		return m_bActivate;
	};
	void ActivateWareHouse(BOOL bActivate);

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
	CItem**	GetWareInven()
	{
		return &m_WareInven[0][0][0];
	}
	int*	GetWareSlotPtr()
	{
		return &m_WareSlot[0][0][0];
	}

	void SetCurPackNum(int pack_num)
	{
		m_nPackNum = pack_num;
	}

	// �����󿡼� �� �޽���.
	// [6/1/2007 Theodoric] ������ ���� :: �Լ� �Ķ���� ������ ����
	void Stash(int slot_num, int vnum, int plus, int special, int special2,
			   int UpEndur, int MaxUpdEndur, int Endur, int MaxEndur, int count, int time_limit ); ///�ð��������� �ð��� �߰�(050711 by ����) ///�Ź���ȭ�ý���
	// [6/1/2007 Theodoric] ������ ���� :: �Լ� �Ķ���� ������ ����
	void StashTo(int slot_num, int vnum, int plus, int special, int special2,
				 int UpEndur, int MaxUpdEndur, int Endur, int MaxEndur, int count, int swap_flag); ///�Ź���ȭ�ý���
	// [6/1/2007 Theodoric] ������ ���� :: �Լ� �Ķ���� ������ ����
	void StashFrom(int slot_num, int index, int vnum, int plus, int &special, int special2,
				   int UpEndur, int MaxUpdEndur, int Endur, int MaxEndur, int count); ///�Ź���ȭ�ý���
	void StashEnd();

	// nate
	// ���� : â���ѹ�ȣ â���Թ�ȣ �κ��ѹ�ȣ �ι���ǥ��
	void StashPut( int nVnum, int nInvenPackNum, int nWarePackNum, int nWareSlotNum, int nSlot_X, int nSlot_Y );
	void StashGet( int nItemNum, int nInvenPackNum, int nWarePackNum, int nWareSlotNum, int nItemIndex, int nSlot_X, int nSlot_Y );


	void StashPuts(int vnum , int ware_slot , int inven_pack , int item_index );
	void StashPuts(int vnum , int ware_slot , int inven_pack , int item_index , int plus, int special, int special2, int UpEndur, int MaxUpdEndur, int Endur, int MaxEndur, int count, int swap_flag);
	void StashGets(int vnum , int ware_slot , int inven_pack , int item_index , int inven_x, int inven_y);
	void StashGets(int vnum , int ware_slot , int inven_pack , int item_index , int inven_x, int inven_y, int plus, int special, int special2, int UpEndur, int MaxUpdEndur, int Endur, int MaxEndur, int count);


	void RemoveAll();

	int m_iCurSlot_X;
	int m_iCurSlot_Y;
	CTextOutBox m_MoneyText;
	CTextOutBox m_pNumText[WARE_PACK_NUM][WARE_SLOT_X_NUM][WARE_SLOT_Y_NUM];
	BOOL CheckMoneyStr(char* szMoney);
	char m_strMoney1[12];
	BOOL InMoneyMsg(char* money);
	BOOL OutMoneyMsg(char* money);
	BYTE m_byInOutMoneyState;	//nate 2006-02-23 : ����� ����(0:��Ȱ��ȭ ����, 1:�Ա�, 2:���)
	BOOL m_bMsgWaiting;
	LRESULT MoneyPopupMsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, int nMsg, char* szMoney);
	LRESULT PopupMsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, int nMsg);
	std::string m_strStashPacketTemp;

	//  [11/26/2008 parkmj] ����â��� ����Ŭ��
	BOOL IsCashStoreUsing()
	{
		return m_bCash;
	}; ///071003 �߰�


public:
	int GetEmptySlot(int slot_num);
	int GetEmptySlot();
	int FindSlot(int vnum, bool add_item);
	int FindSlot(int vnum , int pack , bool add_item);

	int SlotToPack(int slot_index);
	int SlotToX(int slot_index);
	int SlotToY(int slot_index);

	CItem * GetWareSlotItem(int slot_index);
	int GetWareSlotCount(int slot_index);

	int Push_Available(CItem* pItem);

public:
	void SetNpcIndex(const int& index);
	const int& GetNpcIndex();
	BOOL IsFromSummonNpc();

private:
	int m_indexNpc;
};

#endif // _CONTROL_WAREHOUSE_H_
