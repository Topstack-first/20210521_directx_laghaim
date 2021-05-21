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

// 특수상점 플래그 마스크.
#define MASK_SP_SHOP 0x10 // 스페셜화 된 아이템을 파는 상점인지의 플래그.(플러스,옵션,시간제한,가격지정등이 가능한 아이템을 판다.)
#define MASK_SHOP_PAY_TYPE 0xf // 지불 타입을 알아내기 위한 비트 마스크.

// 상점 지불 타입 추가.(by 원석) ///BP상점
#define SHOP_PAY_TYPE_LAIM 0 // 라임 상점.
#define SHOP_PAY_TYPE_BP   1 //  BP 상점.
#define SHOP_PAY_TYPE_CASH	2 // 해외에서 사용 하는 캐쉬 타입때문에 추가해놨다. 버그일지는 나도 모른다.
#define SHOP_PAY_TYPE_LP   3 // [2008/5/16 Theodoric] BP -> LP 로 바뀜

#define LIME_LIMIT 20100000000ll // 라인 소지 제한액.

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

	// 아이템 관련....
	void RemoveAllItems();
	void AddItem(int pack_num, int vnum);
	void AddSPItem(int pack_num, int index, int vnum, int plus, int option, int option2, int endurance, int time, int price, int count ); //옵션값이 추가된 아이템도 받을수있게 하는 버전 by 원석 ///BP상점
	BOOL FindBlankSlot(int pack_num, int &x, int &y, int w, int h);
	int GetSlotVnum(int x, int y, int *type = NULL, int *shape = NULL);
	int GetSlotIndex( int x, int y ); // 선택한 슬롯에 해당하는 아이템의 인덱스를 리턴. (by 원석)
	void SetMoney(bool isFameShop = false, __int64 amount = 0);
	void SetRate(int sell_rate, int buy_rate, BYTE shop_type = 0 ); // 상점 타입 추가. 이 상점이 BP상점인지, 라임상점인지등을 알려준다. 0 이면 라임상점, 1이면 BP상점 (by 원석) ///BP상점
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

	BYTE m_shop_type; // 라임상점인지 BP상점인지를 표시. (by 원석) ///BP상점
	CTextOutBox m_MoneyText;
	int m_shopslotstart_x, m_shopslotstart_y;
	// 보석이나 플러스 아이템을 상점에 팔라구 할때..경고 메세지
	HRESULT WarningPopup(HWND, UINT, WPARAM, LPARAM);

	BOOL CheckSellWarning( CItem *pItem, int PopupType = 5 ); // 팔때 경고 띄우기. (이 안에서 팝업 호출이 있음에 유의) ///다중판매 시 작업

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
