#ifndef _CONTROL_USER_SHOP_H_
#define _CONTROL_USER_SHOP_H_

#include "Config.h"

#include <dsound.h>

//#include "basicbutton.h"

#define USERSHOP_USE_SOUND_NUM 2 // ȿ���� ��� ����.

#define USHOP_STATES_NOTOPEN -1
#define USHOP_STATES_NORMAL 1
#define USHOP_STATES_HIDE 2
#define USHOP_STATES_HIDING_IN 3
#define USHOP_STATES_HIDING_OUT 4
#define USHOP_STATES_HIDING_OUT2 5 // ��.�ϰ� ��鸮�� �κ�.

#define USHOP_TYPE_SELLER  0   // ���� Ÿ��. �Ĵ� ���. 
#define USHOP_TYPE_BUYER   1   // ���� Ÿ��. ��� ���.
#define USHOP_TYPE_ADMIN   2   // ���� Ÿ��. ���� ����.(�������)
#define USHOP_TYPE_STORE   3   // ���� Ÿ��. ���� ���� �غ���.

#define MAX_COUNT_IN_ONESLOT 100 // �� ���Կ� �����ִ� �ִ� ��ġ�� ��.
#define USHOP_PACK_NUM		3 // ���� ����. 
#define USHOP_SLOT_X_NUM		4
#define USHOP_SLOT_Y_NUM		3
#define USHOP_SLOT_WIDTH		53
#define USHOP_SLOT_HEIGHT	    83

#define MAX_MOVE_ONECHANCE 10 // �ѹ��� �̵���ų���ִ� ���� �Ѱ�.

#define MAX_USHOP_SEARCH_NUM 100 // ���� ���� �˻��� �� �ִ� ���� �Ѱ�.
#define USHOP_SEARCH_NUM 20 // 800 �ػ󵵿��� �� ȭ�鿡 ���̴� ����Ʈ ����.

#define SELL_MIN_LIMIT 1000 // �ǸŰ��� ����

class CSurface;
class CTcpIpConnection;
class CItemRender;
class CTextOutBox;
class CItem;
class CMsgPopUp;
class CBasicButton;
class CScroll;

enum { USERSHOP_SOUND_IN = 0, USERSHOP_SOUND_OUT, USERSHOP_SOUND_NUM };

struct sMoveItemInfo // �̵��ϱ� ���� �������� ������ �ӽ÷� �־�α� ���� ����ü(��Ŷ�� �� �������� ����صд�.)
{
	int SlotIndex;		// ���� ������ �ε���
	int InvenPack;		// �κ��� �ѹ�ȣ.
	DWORD Price;		// �Ǹ� ����.
	DWORD GoldCount;	// �ݱ� ����
	DWORD SilverCount;	// ���� ����
	DWORD CopperCount;	// ���� ����
	DWORD RedCount;	
	DWORD PlatinumCount;	
	int Count;			// ����

	int InvenX[MAX_MOVE_ONECHANCE];
	int InvenY[MAX_MOVE_ONECHANCE];
};

// NPC�� ���� ��� ��ȭ,�����������̽�
class CControlUserShop
{
public:
	CControlUserShop();
	~CControlUserShop();

	void	Init();

	void	DeleteRes();
	void	LoadRes( CItemRender *pItemRender);
	HRESULT RestoreSurfaces();

	void Open( int Type, int grade, char *ShopName, char *ShopMent ); // ���½ÿ� ����� �� �ʱ�ȭ��.(���� Init()�� ���½ÿ� �ϸ� �ȵǴ°� �����־ ���� ����.)
	void Close(); // ������ �ݾҴٰ� ������ �˷��ֱ� ���� �Լ�.

	void ClearSlot(); // ������ �����۵��� �ʱ�ȭ���ִ� �༮.

	CItem*  Draw(); // �� �Լ��� ������ DrawItemInfo()�Լ����� ǥ�õǴ� ������ ������ �����ϱ� ���� ���δ�.
	void	DrawButtons(); // ��Ȳ�� ���� �޶����� ��ư���� �����Ͽ� ���.
	void	DrawItemSlot();

	LRESULT MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

	BOOL	IsInside( int x, int y );
	BOOL	IsSlotInside( int x, int y ); // ���Ժκ� ����üũ.
	BOOL	IsInsideSelledMoney( int x, int y ); // �Ǹ� �ݾ׶� üũ.
	BOOL    IsInOutBtnInside( int x, int y ); // �������̽� ���� �ۿ� �ִ� ��/�ƿ� ��ư��ġ�˻�.
	BOOL	IsPlayArea( int x, int y ); // �������̽��� ��� ���� Ŭ���ߴ����� üũ.(�κ��� �����ϰ� üũ.)

	CItem*	GetOverItem( int x, int y ); // Ư�� ������ ���� ���콺�� �ö󰬴����� üũ.

	void	SetNkCha( CNkCharacter *pNkCha )
	{
		m_pNkCha = pNkCha;
	}

	BOOL CheckMoneyStr(CMsgPopUp* pPopup); // ���� �Է½� ����� �� ���� �ԷµǾ������� ����.

	BOOL SlotIn( int SlotPosNum, int RedLime,int PlatinumLime,  int GoldLime, int SilverLime, int BronzeLime, int Price, int InvenPack, int InvenX, int InvenY ); // �κ����� ���� �������� ����.(�Ѱ��� �־��ִ� �Լ���. ��Ŷ�� ���� �������� ī��Ʈ ��ŭ �� �Լ��� ȣ��ȴ�.)(���������� �������� ���� ������ ó����.)
	BOOL SlotOut( int SlotPosNum, int Count ); // ���� ���Կ��� �κ����� ����. (���� �������� �̵��� �������� inven ��Ŷ���� �����ش�. ���⼱ ���� �������� ī��Ʈ ���Ÿ� ���ش�.)
//	BOOL SlotOut( int SlotPosNum, int InvenPack, int InvenX, int InvenY ); // ���� ���Կ��� �κ����� ����.(�Ѱ��� �־��ִ� �Լ���. ��Ŷ�� ���� �������� ī��Ʈ ��ŭ �� �Լ��� ȣ��ȴ�.)(���������� �������� ���� ������ ó����.)

	void SelledItem( int SlotPosNum, int TotalSelledRedLime, int TotalSelledPlatinumLime, int TotalSelledGoldLime, int TotalSelledSilverLime
					 , int TotalSelledBronzeLime, __int64 TotalSelledMoney, __int64 SelledCount );

	// [6/1/2007 Theodoric] ������ ���� :: �Լ� �Ķ���� ������ ����
	void SetSlotItem( int SlotPosNum, int vnum, int plus, int special, int special2,
					  int UpEndur, int MaxUpdEndur, int Endur, int MaxEndur,
					  int RedLime, int PlatinumLime, int GoldLime, int SilverLime, int BronzeLime, int price, int count ); // ������ ������ ����.


	HWND			m_hWnd;
	RECT			m_ClientRc;
	CItemRender		*m_pItemRender;  // �������� ������ �ϱ� ���ؼ� �̰� �ʿ��ϴ�.
	CItem	 *m_pOverItem;
	CNkCharacter *m_pNkCha; // ���ΰ��� ���� ������.

	BOOL m_bResLoad; // ���ҽ� �ε�Ǿ����� �÷���

	CItem *m_pSellItem[USHOP_PACK_NUM][USHOP_SLOT_X_NUM][USHOP_SLOT_Y_NUM]; // �Ǹ��� �������� ��ũ�� ����Ʈ.
	int m_iPackNum; // ���� ���� �ƴ϶� �� �����ӿ� ����. ���� ���� ������ ���� �� ������ Ʋ�������ִ�.
	int m_iNowPack; // ���� ���õǾ��ִ� ��.
	int m_SlotItemCount[USHOP_PACK_NUM][USHOP_SLOT_X_NUM][USHOP_SLOT_Y_NUM];		// ���Դ� ������ ��ǲ ����.
	CTextOutBox m_CountText[USHOP_PACK_NUM][USHOP_SLOT_X_NUM][USHOP_SLOT_Y_NUM]; // ������ ���� ��¿�.

	int m_YesNoType; // �� ������ִ����� �÷���.

	CSurface *m_pBackSur; // �������̽� ���

	CSurface *m_pPlzWait; // ������ ���� �޼����� ��ٸ��� ������ֱ� ���� ����.
	CSurface *m_pNowSelling; // �Ǹ��߿� ���.

	CSurface *m_pSelledMoneyText; // �Ǹ� �ݾ� ����.(�������� ��� ������ؾ�������. ���� ������ �ִ´�.
	CSurface *m_pSelledMoneyBack; // �Ǹ� �ݾ� ǥ�ÿ� �а�
	/*	// �̰� ���� �´� �̹����� �����Ƿ� ����. ���� �̹��� ���� ������ ���� �ʿ伺�� ����μ� ����.
		CSurface *m_pEnableSlot;   // Ȱ��ȭ�� ���� ��� �̹�����.
		CSurface *m_pDisableSlot;  // ��Ȱ��ȭ�� ���� ��� �̹�����.
		CSurface *m_pExistSlot;    // �������� ���� ���� ���� ��� �̹�����.
	*/
	CBasicButton	m_ModifyBtn;		// ���� ��ư
	CBasicButton	m_SellStartBtn;        // �ǸŽ��� ��ư
	CBasicButton	m_CloseShopBtn;        // �����ݱ� ��ư
	//CBasicButton	m_RefundBtn;  disable to fix DC

	CBasicButton m_TabBtn[USHOP_PACK_NUM]; // �� ��ư.

	CBasicButton	m_InBtn;	     // �������̽��� ����� ���� ��ư
	CBasicButton	m_OutBtn;        // ������ �������̽��� �ٽ� ������ ��ư.

	CTextOutBox m_ShopNameText;     // "XXX�� ���λ���"�� ǥ�����ֱ� ���� �Ƕ���.
	CTextBox m_ShopMentText;     // ���������� ����� ��Ʈ ��¿� ���ǽ�.
	CTextOutBox m_SellMoneyText;   // �Ǹ� �ݾ׶�.

	int m_iType; // �Ǹ������� ���������� �÷���.
	int m_iShopKind; // � �������� �÷���. (0~9�� ���λ���, 10~19�� ��������)

	int m_my_slotstart_x, m_my_slotstart_y; // �ܼ��� ��ġ ����.

	int m_States; // ���� � �����ΰ��� �÷���. -1�� �ȿ���, 1�� ����, 2�� ���̵�, 3�� ���̵� ��, 4�� ���̵� �ƿ� ���̴�.
	BOOL m_bSlotLock; // ���� ������ ���� ������ ������Ű�� ���� �÷���.
	BOOL m_bSrvMsgWaiting; // �亯 ��Ŷ�� ��ٸ��� ���� ���� �÷���.
	sMoveItemInfo m_TempMoveInfo; // �ű���� �����۵��� ������ �ӽ÷� ����صδ� ����ü ����.  ���� �Է½õ ���ȴ�.

	int m_TopWidth; // Top �޴��� ���� ����. In/Out��ư�� ������ �ֹǷ� �ʿ�.
	float InOutMovePlusX; // In/Out �ִϸ��̼� �ÿ� ������ ���� Xġ.
	DWORD m_StatesTick; // ���°� ���Ҷ� ����صδ� ƽ.(In/Out �ִϸ��̼ǽ� ����Ѵ�.)
	DWORD m_SrvMsgWaitTick; // ���� �޼����� ��ٸ��� �ֱ� ������ ������ ����صδ� ƽ. �����ð� �����޼����� ������ �÷��׸� Ǯ���ֱ����� �д�.(�̷��� Ǯ����� ������ �ݾƹ����� �ҵ�.)

	CSurface *m_pInOutTempImage; // In/Out�ÿ� ����� ¦�� �κ� �̹���. pDis�� ���� ���´�.
	CSurface *m_pInOutTempImage2; // In/Out�ÿ� ����� ������� �ӽ� �纻.

	LPDIRECTSOUNDBUFFER m_pSound[USERSHOP_SOUND_NUM]; // ȿ����.
	
	int m_SelledRedLime;  // Red Bullion
	int m_SelledPlatinumLime; // Platinum Bullion
	int m_SelledGoldLime; // �ȸ� �ݱ� ��.(�ȾƼ� ���� �ݱ�����.)
	int m_SelledSilverLime; // �Ǹ� ���� ��.
	int m_SelledBronzeLime; // �ȸ� ���� ��.
	__int64 m_SelledLime; // �ȸ� ����.

	BOOL m_bDrawSelledPricePopUp; // �˾��� ����� ����.
	CAlphaPopUp	 *m_pSelledPriceInfo; // �ȸ��׼��� �ݱ�,����,����,���� ������ �����ֱ� ���� �˾�.

	CItem *m_pRefTempExtraItem; // ���� �Է½� ExtraSlot�� �������� �����ֱ� ���� �ӽ� ���� ������.(�����Է��߿� ExtraSlot�� �����͸� ���⿡ ����صΰ� ExtraSlot�� NULL �����Ѵ�. ���� �Է��� �������ش�.)
	HRESULT PopupMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, int nMsg, CMsgPopUp* ArgPopup);
	LRESULT MoneyPopupMsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, int nMsg, CMsgPopUp* ArgPopup);
};


////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ���� ���� �˻��� �������̽�
//
//
#define USS_LIST_GAP 18 // ���λ��� ��ġ ����Ʈ�� �� ����.

struct sUShopSearchData
{
	CItem Item; // ������ ���� // ������ ���̴ϱ� �����ͷ� ���� ����.
	char ItemName[30]; // ������ ��
	char CharName[30]; // ĳ���� ��
	int ZoneNum;    // ����ġ
	int PosX,PosY;  // ��ǥ

	UINT64 MinPrice; // �ּ� �ݾ�
	UINT64 MaxPrice; // �ִ� �ݾ�
	/*
		int GoldLime; // �ݱ�
		int SilverLime; // ����
		int BronzeLime; // ����
		int Price; // ����
	*/
	int Count; // ����.
};

class CControlUShopSearch
{
	CItemRender		*m_pItemRender;  // �������� ������ �ϱ� ���ؼ� �̰� �ʿ��ϴ�.

public:
	CControlUShopSearch();
	~CControlUShopSearch();

	void	Init();

	void	DeleteRes();
	void	LoadRes(CItemRender *pItemRender);

	void  Draw(); // �� �Լ��� ������ DrawItemInfo()�Լ����� ǥ�õǴ� ������ ������ �����ϱ� ���� ���δ�.
	void  DrawResultList(); // �˻���� ����Ʈ�� �׸��� �Լ�.

	LRESULT MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

	BOOL	IsInside( int x, int y );
	int    IsListInside(int x, int y); // ����Ʈ ���� üũ��.
	BOOL	IsPlayArea( int x, int y ); // �������̽��� ��� ���� Ŭ���ߴ����� üũ.(�κ��� �����ϰ� üũ.)

	void ClearList();

	int     GetSearchVnum( const char *SearchName ); // ã������ �������� Vnum�� �����ִ� �Լ�.

	HWND			m_hWnd;
	RECT			m_ClientRc;

	CSurface *m_pBackSur; // �������̽� ���


	CBasicButton	m_SearchBtn;	// ã�� ��ư
	CBasicButton	m_CloseBtn;     // �ݱ� ��ư

	CTextBox m_SearchNameInput;     // ã�� ���ڿ��� �Է��ϴ� �༮.

	int m_ViewListNum; // �� ȭ�鿡 ���̴� ����Ʈ ����.(�ػ� ���� Ʋ����.)
	RECT m_ListRect; // ����Ʈ �κ��� ũ��.
	CScroll m_ListScroll;
	vector<sUShopSearchData *> m_ResultList;
	CTextOutBox m_ItemNameText; // ����Ʈ�� ������ �� �κ� ��¿�.
	CTextOutBox m_CharNameText; // ����Ʈ�� ĳ���� �� �κ� ��¿�.



	DWORD m_ClickTick; // ã�⸦ ���� ������ ���ϰ�...

	BOOL m_bDrawInfoAlpha; // �׸��� ����.
	CAlphaPopUp	 *m_pDetailInfoAlpha; // �˻��� �������� ���� ����(��,��ǥ)���� �ڼ��� �˷��ֱ����� �˾�.
	CAlphaPopUp	 *m_pItemImageAlpha; // ������ �̹��� ����.
	CItem *m_pRefSelOnItem; // ������ �׸� �������� ���� ������.

	BOOL m_bNowSearching; // ����Ʈ�� ĳ���� �� �κ� ��¿�.
	DWORD m_NowSearchTick; // ã�⸦ ���� ������ ���ϰ�...
	char m_strSearchName[256]; // ã���ִ� ������ �̸�.
	CTextOutBox m_NowSearchingText; // ���� ã�� �ִ� ������ ���� �������.
};

#endif // _CONTROL_USER_SHOP_H_