#ifndef _CONTROL_CASHSHOP_H_
#define _CONTROL_CASHSHOP_H_

#include "Config.h"

/*
#define MAX_CASHSHOP_SELLITEM_INTAB 100 // 한 소분류 탭(세로탭)에서 등록될 수 있는 아이템 한도.
#define VIEW_SELLLIST_NUM 6 // 한 화면에 보여지는 판매 아이템 갯수.
/*
// 페이지 구분 매크로.
#define CASHSHOP_PAGE_BUY 0
#define CASHSHOP_PAGE_WEB 1

#define MAX_SELLLIST_ICON 30 // 판매목록의 아이템 분류 아이콘 갯수.
#define MAX_SELLINFO_ICON 4 // 판매목록의 아이템 정보부분의 정보 표시 아이콘.(플러스,옵션,시간제 등.)
*/

#include "Scroll.h"
#include "WebCtrl.h"

#define CASHSHOP_TYPE_NORMAL 0 // 일반 캐쉬 상점
#define MAX_CASHSHOP_KIND 1 // 캐쉬상점의 종류. 현재는 캐쉬샵만.

#define MAX_LENGTH_CATALOG_NAME 50 // 상품명 길이 제한
#define MAX_LENGTH_CATALOG_INFO 256 // 상품 설명글 길이 제한 // 서버랑 상의결과 상품 설명글 255까지만 해주기로 했다.

#define MAX_VIEWCATALOG_NUM 6 // 한 화면에 보여지는 판매 아이템 겟수

#define MAX_ITEM_IN_CATALOG 10 // 한 카탈로그에 들어갈수있는 아이템 갯수 한

#define MAX_INFO_PIXELLINE 2000 // 인포 부의 들어갈수있는 픽셀 라인수
#define VIEW_INFO1_LINE 167 // 인포 부의 상세정보 탭시에 한 화면에 보여지는 픽셀 라인수(픽셀 단위임에 유의.)
#define VIEW_INFO2_LINE 9 // 인포 부의 구매내역 탭시에 한 화면에 보여지는 라인수

#define CASHSHOP_INFO_FONT_SIZE 14 // 인포 부의 폰트 사이즈.


#define CASHSHOP_DTA_ENC_INT_SEED		(2136)
#define CASHSHOP_DTA_ENC_CHAR_SEED		(43)
#define CASHSHOP_DTA_ENC_INT_VAL			(3765)
#define CASHSHOP_DTA_ENC_CHAR_VAL		(139)
#define CASHSHOP_DTA_ENC_DUMMY_COUNT		(17)

struct sImageInfo // 탭 이미지,상품(Catalog)의 이미지 데이터 리스트.(구조가 같아 같이 씀.)
{
	int m_Index;
//	char m_strName[256]; // 탭 이름등..
	char m_strFileName[256]; // 파일명. 리스토어를 위해 기억.
};

struct sSellCatalogInfo // 판매 목록 데이터.(판매 목록안에 여러 아이템이 있는 구조다.)
{
	int m_CatalogIndex; // 상품 인덱스.
	int m_IconImageIndex; // 이미지 아이콘 인덱스.
	char m_strCatalogName[MAX_LENGTH_CATALOG_NAME]; // 상품 이름.
	char m_strCatalogInfo[MAX_LENGTH_CATALOG_INFO]; // 상품 설명글. 길이에 제한을 둬야한다.
	DWORD m_Price; // 가격
	int m_Milease; // 마일리지.

	int m_GroupIndex; // 소속 그룹(탭)의 인덱스.
	int m_bNewCatalog; // 신상품인지의 체크.

	int m_ItemCount; // 이 카탈로그에 들어가있는 아이템 갯수.
	CItem m_Item[MAX_ITEM_IN_CATALOG]; // 한 상품에 들어가잇는 아이템들.

	SYSTEMTIME m_BuyTime; // 구매시간.(구매 내역 리스트에서만 쓰인다.)

	CSurface* m_pTextSur; // 카탈로그안에 들어가는 텍스트를 미리 만들어서 가지고있을 녀석.

};

struct sCashShopDta // 상점 단위의 dta정보 구조체. 안은 각 상점의 그룹(탭),상품(Catalog) 이미지, 상품(Catalog) 정보, 아이템 정보 의 리스트로 구성되어있다.
{
	vector<sImageInfo *> m_GruopList; // 상품(Catalog)의 이미지 데이터 리스트.
	vector<sImageInfo *> m_CatalogImageList; // 상품(Catalog)의 이미지 데이터 리스트.
	vector<sSellCatalogInfo *> m_SellCatalogList; // 판매 목록 데이터.(판매 목록안에 여러 아이템이 있는 구조다.)

	BOOL m_bImageLoad; // dta 정보는 열때마다 로드하지만 이미지는 한번만 로드하기 위한 플래그.
};

class CControlCashShop // 유료상점의 메인 클래스.
{
public:
	CControlCashShop();
	~CControlCashShop();

	void	Init();

	void	DeleteRes();
	void	LoadRes();
	void RestoreSurfaces();

	void    OpenInit(); // 열릴때마다 초기화해줘야하는 녀석들.

	void ReadDta( int ShopType, const char *FileName ); // Dta 정보를 로드.
	void DummyDta( int ShopType ); // Dta 정보를 만듬. 임시 테스트용
	void ClearDta(); // Dta 정보들을 초기화.
	void ClearDta( int i ); // 특정 Dta 만을 초기화.
	void LoadFromDta( int ShopType ); // Dta에 저장된 파일명을 기준으로 하는 이미지 로드.

	CItem*  Draw(); // 이 함수의 리턴은 DrawItemInfo()함수에서 표시되는 아이템 정보를 갱신하기 위해 쓰인다.
	void DrawCatalogList(); // 상품 목록 출력.
	void GetInfo1Text( int ItemIndex ); // 상세정보부분의 MultiText를 읽어 오는 부분.(변경 될때만 한번씩 호출해주자)
	int DrawInfo(); // 상세정보,구매내역 등의 출력. 리턴값은 보여질 라인(또는 픽셀)수를 리턴.
	void DrawInven();

	LRESULT MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
	HRESULT BuyPopupMsg( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, int nMsg ); // 구입 팝업용 프로시져.

	void GroupChange( int TabIndex ); // 메인 탭이 바뀌었을 때의 처리.

	int FindIconImage( int DbIndex ); // 아이콘 이미지를 DB인덱스로 부터 찾는다.

	BOOL	IsInside( int x, int y );
	BOOL    IsInsideInven( int x, int y );

	HWND			m_hWnd;
	RECT			m_ClientRc;

	BOOL m_bResLoad; // 리소스 로드되었나의 플래그

	int m_iShopKind; // 어떤 상점 인덱스. (0은 일반 캐쉬상점)

	CTextOutBox m_TextOut; // 텍스트 출력 범용.
	sCashShopDta m_DtaData[MAX_CASHSHOP_KIND]; // 상점별 Dta 데이터. 리스트로 구성.


	CSurface *m_pBackSur; // 인터페이스 배경

	CBasicButton	m_CloseBtn;        // 닫기 버튼

	CTabControl m_GroupTab; // 메인 가로 탭 (아이템 종류를 나누어주는 텝)
	int m_NowViewCatalogPage; // 현재 그룹의 보고있는 페이지.
	CSurface *m_pItemSlotSur; // 아이템 판매목록의 한 아이템 당 배경 그래픽.
	CSurface *m_pSelectSlotSur; // 선택된 슬롯 표시해주는 녀석.

	CSurface **m_pCatalogImageList; // 상품 목록 에 보여지는 이미지들.

	sSellCatalogInfo *m_pWantBuyCatalog; // 현재 살려고 하는 중의 카탈로그의 참조 포인터.




	CTabControl m_InfoTab; // 상세정보,구매내역등을 구분해주는 텝.
	int m_NowOnCatalogIndex; // 현재 마우스가 올라간 카날로그의 인덱스.
	sSellCatalogInfo *m_pSelectCatalog; // 현재 선택된 카탈로그의 참조 포인터.
	CScroll m_InfoScroll; // 상세정보 등의 스크롤.
	CMultiText m_MultiText; // Info 출력용 멀티텍스트
	int m_NowInfoPage; // 상세정보상의 아이템 갯수에 해당하는 페이지 번호.
	CSurface *m_pInfo1BackSur; // Info1 시에 배경에 출력될 녀석.
	CSurface *m_pInfo2BackSur; // Info2 시에 배경에 출력될 녀석.
	CBasicButton	m_InfoPrevBtn; // 이전 아이템 보기 버튼.
	CBasicButton	m_InfoNextBtn; // 다음 아이켐 보기 버튼.
	int m_TotalUseCash; // 구매정보에서 표시되는 총 사용 캐쉬량.




	RECT m_InvenRect; // 인벤 영역 체크용.
	CBasicButton	m_InvenTabBtn[PACK_NUM]; // 인벤 탭 버튼
	CItem *m_pInvenOverItem; // 인벤의 마우스가 올라간 아이템의 참조 포인터.



	CBasicButton	m_PrevBtn; // 이전 판매 목록 보기 버튼.
	CBasicButton	m_NextBtn; // 다음 판매 목록 보기 버튼.


	CTextOutBox m_HaveCash; // 보유 캐쉬
	CBasicButton	m_DummyBuyBtn;        // 구매 버튼. 실제 이미지 로드는 여기서만 한다.
	CBasicButton	m_BuyBtn[MAX_VIEWCATALOG_NUM];        // 구매 버튼. 보여져야 할 수만큼 가지고있느다.하지만 이미지는 로드 하지 않고 위의 Dummy를 참조해서 사용한다.

	vector<sSellCatalogInfo *> m_ViewCatalogList; // 보여지는 상품(Catalog)의 이미지 데이터 리스트. 탭 따라 보여지는 리스트가 달라지기 때문에 필요.

	vector<sSellCatalogInfo *> m_BuyCatalogList; // 구매한 상품 리스트.

	// 출력 속도 향상용...범용 TextOutBox를 하나 둬서 매번 SetString()하는 방식은 편하긴해도 매우 느리다. 속도 향상을 위해 대다수의 스트링을 한번만 서피스로 만들어두고 출력한다.(자주 바뀌는 녀석은 그냥 범용 TextOutBox를 이용한다.(보유 캐쉬 같은 경우) )
	void MakeCatalogTextSurface(); // 카탈로그안에 들어가는 텍스트를 서피스로 만든다.(전 카탈로그 리스트를 돌면서...)
	void MakeBuyListText( sSellCatalogInfo *pCatalog );  // 구매내역의 텍스트부를 미리 생성.
	CSurface *m_pItemInfoTextSur; // 선택된 아이템 정보의 텍스트를 미리 서피스로 만들어 놓은 녀석.



};

class CCashShopIntro // 캐쉬 샵의 메인 페이지가 열리기전의 웹 공지 페이지
{
public:
	CCashShopIntro();
	~CCashShopIntro();

	void	Init();

	void	DeleteRes();
	void	LoadRes();
	void RestoreSurfaces();

	void Open(); // 열때 해줄거.
	void Close(); // 닫을때 해줄거.

	void  Draw(); // 이 함수의 리턴은 DrawItemInfo()함수에서 표시되는 아이템 정보를 갱신하기 위해 쓰인다.

	LRESULT MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

	BOOL	IsInside( int x, int y );

	HWND			m_hWnd;
	RECT			m_ClientRc;

	CSurface *m_pBackSur; // 인터페이스 배경

	CWebWnd m_WebPage1;
	CWebWnd m_WebPage2;

	CBasicButton	m_InBtn;        // 구매페이지 들어가기 버튼
	CBasicButton	m_CloseBtn;     // 닫기 버튼

};

#endif // _CONTROL_CASHSHOP_H_