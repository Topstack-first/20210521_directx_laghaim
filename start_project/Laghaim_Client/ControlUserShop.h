#ifndef _CONTROL_USER_SHOP_H_
#define _CONTROL_USER_SHOP_H_

#include "Config.h"

#include <dsound.h>

//#include "basicbutton.h"

#define USERSHOP_USE_SOUND_NUM 2 // 효과음 사용 갯수.

#define USHOP_STATES_NOTOPEN -1
#define USHOP_STATES_NORMAL 1
#define USHOP_STATES_HIDE 2
#define USHOP_STATES_HIDING_IN 3
#define USHOP_STATES_HIDING_OUT 4
#define USHOP_STATES_HIDING_OUT2 5 // 쿵.하고 흔들리는 부분.

#define USHOP_TYPE_SELLER  0   // 상점 타입. 파는 사람. 
#define USHOP_TYPE_BUYER   1   // 상점 타입. 사는 사람.
#define USHOP_TYPE_ADMIN   2   // 상점 타입. 점포 주인.(수정모드)
#define USHOP_TYPE_STORE   3   // 상점 타입. 점포 개설 준비중.

#define MAX_COUNT_IN_ONESLOT 100 // 한 슬롯에 들어갈수있는 최대 겹치는 수.
#define USHOP_PACK_NUM		3 // 탭의 갯수. 
#define USHOP_SLOT_X_NUM		4
#define USHOP_SLOT_Y_NUM		3
#define USHOP_SLOT_WIDTH		53
#define USHOP_SLOT_HEIGHT	    83

#define MAX_MOVE_ONECHANCE 10 // 한번에 이동시킬수있는 수량 한계.

#define MAX_USHOP_SEARCH_NUM 100 // 개인 상점 검색할 수 있는 갯수 한계.
#define USHOP_SEARCH_NUM 20 // 800 해상도에서 한 화면에 보이는 리스트 갯수.

#define SELL_MIN_LIMIT 1000 // 판매가격 제한

class CSurface;
class CTcpIpConnection;
class CItemRender;
class CTextOutBox;
class CItem;
class CMsgPopUp;
class CBasicButton;
class CScroll;

enum { USERSHOP_SOUND_IN = 0, USERSHOP_SOUND_OUT, USERSHOP_SOUND_NUM };

struct sMoveItemInfo // 이동하기 위한 아이템의 정보를 임시로 넣어두기 위한 구조체(패킷에 들어갈 정보들을 기억해둔다.)
{
	int SlotIndex;		// 상점 슬롯의 인덱스
	int InvenPack;		// 인벤의 팩번호.
	DWORD Price;		// 판매 가격.
	DWORD GoldCount;	// 금괴 갯수
	DWORD SilverCount;	// 은괴 갯수
	DWORD CopperCount;	// 동괴 갯수
	DWORD RedCount;	
	DWORD PlatinumCount;	
	int Count;			// 수량

	int InvenX[MAX_MOVE_ONECHANCE];
	int InvenY[MAX_MOVE_ONECHANCE];
};

// NPC를 통한 장비 강화,가공인터페이스
class CControlUserShop
{
public:
	CControlUserShop();
	~CControlUserShop();

	void	Init();

	void	DeleteRes();
	void	LoadRes( CItemRender *pItemRender);
	HRESULT RestoreSurfaces();

	void Open( int Type, int grade, char *ShopName, char *ShopMent ); // 오픈시에 해줘야 할 초기화들.(위의 Init()은 오픈시에 하면 안되는게 섞여있어서 따로 만듬.)
	void Close(); // 상점을 닫았다고 서버에 알려주기 위한 함수.

	void ClearSlot(); // 슬롯의 아이템들을 초기화해주는 녀석.

	CItem*  Draw(); // 이 함수의 리턴은 DrawItemInfo()함수에서 표시되는 아이템 정보를 갱신하기 위해 쓰인다.
	void	DrawButtons(); // 상황에 따라 달라지는 버튼들을 구분하여 출력.
	void	DrawItemSlot();

	LRESULT MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

	BOOL	IsInside( int x, int y );
	BOOL	IsSlotInside( int x, int y ); // 슬롯부분 영역체크.
	BOOL	IsInsideSelledMoney( int x, int y ); // 판매 금액란 체크.
	BOOL    IsInOutBtnInside( int x, int y ); // 인터페이스 영역 밖에 있는 인/아웃 버튼위치검사.
	BOOL	IsPlayArea( int x, int y ); // 인터페이스를 벗어나 땅을 클릭했는지의 체크.(인벤은 제외하고 체크.)

	CItem*	GetOverItem( int x, int y ); // 특정 아이템 위로 마우스가 올라갔는지의 체크.

	void	SetNkCha( CNkCharacter *pNkCha )
	{
		m_pNkCha = pNkCha;
	}

	BOOL CheckMoneyStr(CMsgPopUp* pPopup); // 가격 입력시 제대로 된 값이 입력되었는지의 여부.

	BOOL SlotIn( int SlotPosNum, int RedLime,int PlatinumLime,  int GoldLime, int SilverLime, int BronzeLime, int Price, int InvenPack, int InvenX, int InvenY ); // 인벤에서 상점 슬롯으로 넣음.(한개를 넣어주는 함수다. 패킷을 받은 시점에서 카운트 만큼 이 함수가 호출된다.)(실질적으로 서버에서 받은 이후의 처리임.)
	BOOL SlotOut( int SlotPosNum, int Count ); // 상점 슬롯에서 인벤으로 넣음. (실제 아이템의 이동은 서버에서 inven 패킷으로 보내준다. 여기선 상점 슬롯쪽의 카운트 갱신만 해준다.)
//	BOOL SlotOut( int SlotPosNum, int InvenPack, int InvenX, int InvenY ); // 상점 슬롯에서 인벤으로 넣음.(한개를 넣어주는 함수다. 패킷을 받은 시점에서 카운트 만큼 이 함수가 호출된다.)(실질적으로 서버에서 받은 이후의 처리임.)

	void SelledItem( int SlotPosNum, int TotalSelledRedLime, int TotalSelledPlatinumLime, int TotalSelledGoldLime, int TotalSelledSilverLime
					 , int TotalSelledBronzeLime, __int64 TotalSelledMoney, __int64 SelledCount );

	// [6/1/2007 Theodoric] 내구도 수리 :: 함수 파라미터 내구도 포함
	void SetSlotItem( int SlotPosNum, int vnum, int plus, int special, int special2,
					  int UpEndur, int MaxUpdEndur, int Endur, int MaxEndur,
					  int RedLime, int PlatinumLime, int GoldLime, int SilverLime, int BronzeLime, int price, int count ); // 아이템 정보를 셋팅.


	HWND			m_hWnd;
	RECT			m_ClientRc;
	CItemRender		*m_pItemRender;  // 아이템을 렌더링 하기 위해선 이게 필요하다.
	CItem	 *m_pOverItem;
	CNkCharacter *m_pNkCha; // 주인공의 참조 포인터.

	BOOL m_bResLoad; // 리소스 로드되었나의 플래그

	CItem *m_pSellItem[USHOP_PACK_NUM][USHOP_SLOT_X_NUM][USHOP_SLOT_Y_NUM]; // 판매할 아이템의 링크드 리스트.
	int m_iPackNum; // 현재 팩이 아니라 팩 갯수임에 유의. 위의 상점 종류에 따라 팩 갯수가 틀려질수있다.
	int m_iNowPack; // 현재 선택되어있는 팩.
	int m_SlotItemCount[USHOP_PACK_NUM][USHOP_SLOT_X_NUM][USHOP_SLOT_Y_NUM];		// 슬롯당 아이템 인풋 갯수.
	CTextOutBox m_CountText[USHOP_PACK_NUM][USHOP_SLOT_X_NUM][USHOP_SLOT_Y_NUM]; // 아이템 갯수 출력용.

	int m_YesNoType; // 뭘 물어보고있는지의 플래그.

	CSurface *m_pBackSur; // 인터페이스 배경

	CSurface *m_pPlzWait; // 서버로 부터 메세지를 기다릴때 출력해주기 위한 문구.
	CSurface *m_pNowSelling; // 판매중에 출력.

	CSurface *m_pSelledMoneyText; // 판매 금액 글자.(구매자의 경우 안출력해야함으로. 따로 가지고 있는다.
	CSurface *m_pSelledMoneyBack; // 판매 금액 표시용 패경
	/*	// 이건 현재 맞는 이미지가 없으므로 보류. 굳이 이미지 까지 만들어가며 해줄 필요성이 현재로선 없다.
		CSurface *m_pEnableSlot;   // 활성화된 슬롯 배경 이미지용.
		CSurface *m_pDisableSlot;  // 비활성화된 슬롯 배경 이미지용.
		CSurface *m_pExistSlot;    // 아이템이 놓인 곳의 슬롯 배경 이미지용.
	*/
	CBasicButton	m_ModifyBtn;		// 수정 버튼
	CBasicButton	m_SellStartBtn;        // 판매시작 버튼
	CBasicButton	m_CloseShopBtn;        // 상점닫기 버튼
	//CBasicButton	m_RefundBtn;  disable to fix DC

	CBasicButton m_TabBtn[USHOP_PACK_NUM]; // 탭 버튼.

	CBasicButton	m_InBtn;	     // 인터페이스를 숨기기 위한 버튼
	CBasicButton	m_OutBtn;        // 숨겨진 인터페이스를 다시 꺼내는 버튼.

	CTextOutBox m_ShopNameText;     // "XXX의 개인상점"을 표기해주기 위한 판때기.
	CTextBox m_ShopMentText;     // 상점주인이 써놓은 멘트 출력용 서피스.
	CTextOutBox m_SellMoneyText;   // 판매 금액란.

	int m_iType; // 판매자인지 구매자인지 플래그.
	int m_iShopKind; // 어떤 상점인지 플래그. (0~9는 개인상점, 10~19는 점포상점)

	int m_my_slotstart_x, m_my_slotstart_y; // 단순히 위치 기억용.

	int m_States; // 현재 어떤 상태인가의 플래그. -1은 안열림, 1은 열림, 2는 하이드, 3은 하이딩 인, 4는 하이딩 아웃 등이다.
	BOOL m_bSlotLock; // 상점 시작후 슬롯 조작을 방지시키기 위한 플래그.
	BOOL m_bSrvMsgWaiting; // 답변 패킷을 기다리는 중의 상태 플래그.
	sMoveItemInfo m_TempMoveInfo; // 옮기려는 아이템들의 정보를 임시로 기억해두는 구조체 변수.  가격 입력시등에 사용된다.

	int m_TopWidth; // Top 메뉴의 가로 길이. In/Out버튼이 영향을 주므로 필요.
	float InOutMovePlusX; // In/Out 애니메이션 시에 보정될 최종 X치.
	DWORD m_StatesTick; // 상태가 변할때 기억해두는 틱.(In/Out 애니메이션시 사용한다.)
	DWORD m_SrvMsgWaitTick; // 서버 메세지를 기다리고 있기 시작한 시점을 기억해두는 틱. 일정시간 서버메세지가 없으면 플래그를 풀어주기위해 둔다.(이렇게 풀릴경우 상점을 닫아버려야 할듯.)

	CSurface *m_pInOutTempImage; // In/Out시에 찍어줄 짝퉁 인벤 이미지. pDis로 부터 따온다.
	CSurface *m_pInOutTempImage2; // In/Out시에 사용할 백버퍼의 임시 사본.

	LPDIRECTSOUNDBUFFER m_pSound[USERSHOP_SOUND_NUM]; // 효과음.
	
	int m_SelledRedLime;  // Red Bullion
	int m_SelledPlatinumLime; // Platinum Bullion
	int m_SelledGoldLime; // 팔린 금괴 수.(팔아서 얻은 금괴수다.)
	int m_SelledSilverLime; // 판린 은괴 수.
	int m_SelledBronzeLime; // 팔린 동괴 수.
	__int64 m_SelledLime; // 팔린 라임.

	BOOL m_bDrawSelledPricePopUp; // 팝업을 띄울지 말지.
	CAlphaPopUp	 *m_pSelledPriceInfo; // 팔린액수를 금괴,은괴,동괴,라임 단위로 보여주기 위한 팝업.

	CItem *m_pRefTempExtraItem; // 가격 입력시 ExtraSlot의 아이템을 숨겨주기 위한 임시 참조 포인터.(가격입력중에 ExtraSlot의 포인터를 여기에 기억해두고 ExtraSlot을 NULL 셋팅한다. 가격 입력후 복구해준다.)
	HRESULT PopupMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, int nMsg, CMsgPopUp* ArgPopup);
	LRESULT MoneyPopupMsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, int nMsg, CMsgPopUp* ArgPopup);
};


////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 개인 상점 검색용 인터페이스
//
//
#define USS_LIST_GAP 18 // 개인상점 서치 리스트의 줄 간격.

struct sUShopSearchData
{
	CItem Item; // 아이템 정보 // 어차피 더미니깐 포인터로 쓰지 말자.
	char ItemName[30]; // 아이템 명
	char CharName[30]; // 캐릭터 명
	int ZoneNum;    // 종위치
	int PosX,PosY;  // 좌표

	UINT64 MinPrice; // 최소 금액
	UINT64 MaxPrice; // 최대 금액
	/*
		int GoldLime; // 금괴
		int SilverLime; // 은괴
		int BronzeLime; // 동괴
		int Price; // 라임
	*/
	int Count; // 수량.
};

class CControlUShopSearch
{
	CItemRender		*m_pItemRender;  // 아이템을 렌더링 하기 위해선 이게 필요하다.

public:
	CControlUShopSearch();
	~CControlUShopSearch();

	void	Init();

	void	DeleteRes();
	void	LoadRes(CItemRender *pItemRender);

	void  Draw(); // 이 함수의 리턴은 DrawItemInfo()함수에서 표시되는 아이템 정보를 갱신하기 위해 쓰인다.
	void  DrawResultList(); // 검색결과 리스트를 그리는 함수.

	LRESULT MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

	BOOL	IsInside( int x, int y );
	int    IsListInside(int x, int y); // 리스트 영역 체크용.
	BOOL	IsPlayArea( int x, int y ); // 인터페이스를 벗어나 땅을 클릭했는지의 체크.(인벤은 제외하고 체크.)

	void ClearList();

	int     GetSearchVnum( const char *SearchName ); // 찾으려는 아이템의 Vnum을 구해주는 함수.

	HWND			m_hWnd;
	RECT			m_ClientRc;

	CSurface *m_pBackSur; // 인터페이스 배경


	CBasicButton	m_SearchBtn;	// 찾기 버튼
	CBasicButton	m_CloseBtn;     // 닫기 버튼

	CTextBox m_SearchNameInput;     // 찾을 문자열을 입력하는 녀석.

	int m_ViewListNum; // 한 화면에 보이는 리스트 갯수.(해상도 별로 틀리다.)
	RECT m_ListRect; // 리스트 부분의 크기.
	CScroll m_ListScroll;
	vector<sUShopSearchData *> m_ResultList;
	CTextOutBox m_ItemNameText; // 리스트의 아이템 명 부분 출력용.
	CTextOutBox m_CharNameText; // 리스트의 캐릭터 명 부분 출력용.



	DWORD m_ClickTick; // 찾기를 마구 누르지 못하게...

	BOOL m_bDrawInfoAlpha; // 그릴지 말지.
	CAlphaPopUp	 *m_pDetailInfoAlpha; // 검색된 아이템의 관련 정보(존,좌표)등을 자세히 알려주기위한 팝업.
	CAlphaPopUp	 *m_pItemImageAlpha; // 아이템 이미지 배경용.
	CItem *m_pRefSelOnItem; // 인포를 그릴 아이템의 참조 포인터.

	BOOL m_bNowSearching; // 리스트의 캐릭터 명 부분 출력용.
	DWORD m_NowSearchTick; // 찾기를 마구 누르지 못하게...
	char m_strSearchName[256]; // 찾고있는 아이템 이름.
	CTextOutBox m_NowSearchingText; // 현재 찾고 있는 아이템 명을 출력해줌.
};

#endif // _CONTROL_USER_SHOP_H_