#ifndef _CONTROL_SHOP_H_
#define _CONTROL_SHOP_H_

#include "Config.h"

#include "NkCharacter.h"
#include "BasicButton.h"
#include "TextOutBox.h"
#include "AlphaPopup.h"

#define BASE_X_SIZE		640
#define BASE_Y_SIZE		480
#define BASE_X_SIZE_F	(640.0f)
#define BASE_Y_SIZE_F	(480.0f)

#define SHOP_WIDTH		(220)// * m_ScreenWidth / BASE_X_SIZE)	//272
#define SHOP_HEIGHT		(419)// * m_ScreenHeight / BASE_Y_SIZE)//370
#define SHOP_START_X	(150)// * m_ScreenWidth / BASE_X_SIZE)	//368-183
#define SHOP_START_Y	0//(36 * m_ScreenHeight / BASE_Y_SIZE)

#define SHOP_SLOT_WIDTH		(27)// * m_ScreenWidth / BASE_X_SIZE)
#define SHOP_SLOT_HEIGHT		(27)// * m_ScreenHeight / BASE_Y_SIZE)

#define SHOP_SLOT_START_X	(179)// * m_ScreenWidth / BASE_X_SIZE)
#define SHOP_SLOT_START_Y	(72)// * m_ScreenHeight / BASE_Y_SIZE)

#define SHOP_PLAY_AREA_X		(186)// * m_ScreenWidth / BASE_X_SIZE) m_ScreenWidth - 273 - 219;
#define SHOP_PLAY_AREA_Y		(396)// * m_ScreenHeight / BASE_Y_SIZE)


#define SHOP_CLOSE_BTN_X		(SHOP_START_X+172)// * m_ScreenWidth / BASE_X_SIZE) 604-272
#define SHOP_CLOSE_BTN_Y		(SHOP_START_X+338)// * m_ScreenHeight / BASE_Y_SIZE)
//#define CLOSE_BTN_W		(24 * m_ScreenWidth / BASE_X_SIZE)
//#define CLOSE_BTN_H		(40 * m_ScreenHeight / BASE_Y_SIZE)

#define SHOP_PACK_NUM		3
#define SHOP_SLOT_X_NUM		6
#define SHOP_SLOT_Y_NUM		9

#define SHOP_MONEY_X		((SHOP_START_X+94))// * m_ScreenWidth / BASE_X_SIZE)
#define SHOP_MONEY_Y		((SHOP_START_Y+376))// * m_ScreenHeight / BASE_Y_SIZE)

// Ư������ �÷��� ����ũ.
#define MASK_SP_SHOP 0x10 // �����ȭ �� �������� �Ĵ� ���������� �÷���.(�÷���,�ɼ�,�ð�����,������������ ������ �������� �Ǵ�.)
#define MASK_SHOP_PAY_TYPE 0xf // ���� Ÿ���� �˾Ƴ��� ���� ��Ʈ ����ũ.

// ���� ���� Ÿ�� �߰�.(by ����) ///BP����
#define SHOP_PAY_TYPE_LAIM 0 // ���� ����.
#define SHOP_PAY_TYPE_BP   1 //  BP ����.
#define SHOP_PAY_TYPE_CASH	2 // �ؿܿ��� ��� �ϴ� ĳ�� Ÿ�Զ����� �߰��س���. ���������� ���� �𸥴�.
#define SHOP_PAY_TYPE_LP   3 // [2008/5/16 Theodoric] BP -> LP �� �ٲ�

#define LIME_LIMIT 20100000000ll // ���� ���� ���Ѿ�.

class CSurface;
class CTcpIpConnection;
class CItemRender;
class CAlphaPopUp;
class CItem;
class CMsgPopUp;

class CControlShop
{
public:
	HWND	m_hWnd;

	CControlShop();
	virtual ~CControlShop();

	void Init();
	void LoadRes();
	void DeleteRes();
	CItem* Draw();
	//void DrawItemInfo();
	//void ResetItemInfo() { m_pItemInfo->SetIndex(-1); }
	void SetNkCha(CNkCharacter *pNkCha);
	BOOL IsInside(int x, int y);
	BOOL IsPlayArea(int x, int y);
	void SendMsg();
	void SetShopIdx(int shopidx)
	{
		m_ShopIdx = shopidx;
	};
	void SetKeeper(int index, float x = 0, float z =0)
	{
		m_KeeperIndex = index, m_KeeperX = x, m_KeeperZ = z;
	}

	// ������ ����....
	void RemoveAllItems();
	void AddItem(int pack_num, int vnum);
	void AddSPItem(int pack_num, int index, int vnum, int plus, int option, int option2, int endurance, int time, int price, int count ); //�ɼǰ��� �߰��� �����۵� �������ְ� �ϴ� ���� by ���� ///BP����
	BOOL FindBlankSlot(int pack_num, int &x, int &y, int w, int h);
	int GetSlotVnum(int x, int y, int *type = NULL, int *shape = NULL);
	int GetSlotIndex( int x, int y ); // ������ ���Կ� �ش��ϴ� �������� �ε����� ����. (by ����)
	void SetMoney(bool isFameShop = false, __int64 amount = 0);
	void SetRate(int sell_rate, int buy_rate, BYTE shop_type = 0 ); // ���� Ÿ�� �߰�. �� ������ BP��������, ���ӻ����������� �˷��ش�. 0 �̸� ���ӻ���, 1�̸� BP���� (by ����) ///BP����
	CItem* GetItemInfo(int index);

	LRESULT MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

	CSurface		*m_pSurfaceBack;
	CNkCharacter	*m_pNkCha;
	CItem			*m_pOverItem;

	CBasicButton m_CloseBtn;
	CBasicButton m_RepairBtn;

	char m_MsgBuf[256];

	int m_ScreenWidth;
	int m_ScreenHeight;

	CItem *m_SellItem[SHOP_PACK_NUM];
	int m_Slot[SHOP_PACK_NUM][SHOP_SLOT_X_NUM][SHOP_SLOT_Y_NUM];
	int m_KeeperIndex;
	int m_ShopIdx;
	float m_KeeperX, m_KeeperZ;
	int m_SellRate, m_BuyRate;

	int	 m_iMulitCnt;
	bool m_bMultibuy;

	BYTE m_shop_type; // ���ӻ������� BP���������� ǥ��. (by ����) ///BP����
	CTextOutBox m_MoneyText;
	int m_shopslotstart_x, m_shopslotstart_y;
	// �����̳� �÷��� �������� ������ �ȶ� �Ҷ�..��� �޼���
	HRESULT WarningPopup(HWND, UINT, WPARAM, LPARAM);

	BOOL CheckSellWarning( CItem *pItem, int PopupType = 5 ); // �ȶ� ��� ����. (�� �ȿ��� �˾� ȣ���� ������ ����) ///�����Ǹ� �� �۾�

	BOOL IsInsideSlotWindow(int x, int y);

protected:
	BOOL			m_bLPShop;
	int				m_CurrLPShopPage;
	vector<int>		m_vecLPShopIndex;


public:
	void	SetLpShopFlag( BOOL bLPShop = FALSE)
	{
		m_bLPShop = bLPShop;
		m_lpshop_wait=true;
	}
	BOOL	IsLpShop()
	{
		return m_bLPShop;
	}
	void	SetLpShopPage();
	int		GetPrevLPShopIndex();
	int		GetNextLPShopIndex();
	int		GetCurrLPShopIndex();
	void	OnPrevLPShopPage();
	void	OnNextLPShopPage();
	void	OnFirstLPShopPage();
	void	OnLastLPShopPage();
	void	AddLPShopIndex(int LPShopIndex);
	void	LPShopIndexClear()
	{
		m_vecLPShopIndex.clear();
	}

	bool m_lpshop_wait;
	void SetLpShopReady()
	{
		m_lpshop_wait=false;
	}
};

#endif // _CONTROL_SHOP_H_
