#ifndef _CONTROL_CASHSHOP_H_
#define _CONTROL_CASHSHOP_H_

#include "Config.h"

/*
#define MAX_CASHSHOP_SELLITEM_INTAB 100 // �� �Һз� ��(������)���� ��ϵ� �� �ִ� ������ �ѵ�.
#define VIEW_SELLLIST_NUM 6 // �� ȭ�鿡 �������� �Ǹ� ������ ����.
/*
// ������ ���� ��ũ��.
#define CASHSHOP_PAGE_BUY 0
#define CASHSHOP_PAGE_WEB 1

#define MAX_SELLLIST_ICON 30 // �ǸŸ���� ������ �з� ������ ����.
#define MAX_SELLINFO_ICON 4 // �ǸŸ���� ������ �����κ��� ���� ǥ�� ������.(�÷���,�ɼ�,�ð��� ��.)
*/

#include "Scroll.h"
#include "WebCtrl.h"

#define CASHSHOP_TYPE_NORMAL 0 // �Ϲ� ĳ�� ����
#define MAX_CASHSHOP_KIND 1 // ĳ�������� ����. ����� ĳ������.

#define MAX_LENGTH_CATALOG_NAME 50 // ��ǰ�� ���� ����
#define MAX_LENGTH_CATALOG_INFO 256 // ��ǰ ����� ���� ���� // ������ ���ǰ�� ��ǰ ����� 255������ ���ֱ�� �ߴ�.

#define MAX_VIEWCATALOG_NUM 6 // �� ȭ�鿡 �������� �Ǹ� ������ �ټ�

#define MAX_ITEM_IN_CATALOG 10 // �� īŻ�α׿� �����ִ� ������ ���� ��

#define MAX_INFO_PIXELLINE 2000 // ���� ���� �����ִ� �ȼ� ���μ�
#define VIEW_INFO1_LINE 167 // ���� ���� ������ �ǽÿ� �� ȭ�鿡 �������� �ȼ� ���μ�(�ȼ� �����ӿ� ����.)
#define VIEW_INFO2_LINE 9 // ���� ���� ���ų��� �ǽÿ� �� ȭ�鿡 �������� ���μ�

#define CASHSHOP_INFO_FONT_SIZE 14 // ���� ���� ��Ʈ ������.


#define CASHSHOP_DTA_ENC_INT_SEED		(2136)
#define CASHSHOP_DTA_ENC_CHAR_SEED		(43)
#define CASHSHOP_DTA_ENC_INT_VAL			(3765)
#define CASHSHOP_DTA_ENC_CHAR_VAL		(139)
#define CASHSHOP_DTA_ENC_DUMMY_COUNT		(17)

struct sImageInfo // �� �̹���,��ǰ(Catalog)�� �̹��� ������ ����Ʈ.(������ ���� ���� ��.)
{
	int m_Index;
//	char m_strName[256]; // �� �̸���..
	char m_strFileName[256]; // ���ϸ�. ������ ���� ���.
};

struct sSellCatalogInfo // �Ǹ� ��� ������.(�Ǹ� ��Ͼȿ� ���� �������� �ִ� ������.)
{
	int m_CatalogIndex; // ��ǰ �ε���.
	int m_IconImageIndex; // �̹��� ������ �ε���.
	char m_strCatalogName[MAX_LENGTH_CATALOG_NAME]; // ��ǰ �̸�.
	char m_strCatalogInfo[MAX_LENGTH_CATALOG_INFO]; // ��ǰ �����. ���̿� ������ �־��Ѵ�.
	DWORD m_Price; // ����
	int m_Milease; // ���ϸ���.

	int m_GroupIndex; // �Ҽ� �׷�(��)�� �ε���.
	int m_bNewCatalog; // �Ż�ǰ������ üũ.

	int m_ItemCount; // �� īŻ�α׿� ���ִ� ������ ����.
	CItem m_Item[MAX_ITEM_IN_CATALOG]; // �� ��ǰ�� ���մ� �����۵�.

	SYSTEMTIME m_BuyTime; // ���Žð�.(���� ���� ����Ʈ������ ���δ�.)

	CSurface* m_pTextSur; // īŻ�α׾ȿ� ���� �ؽ�Ʈ�� �̸� ���� ���������� �༮.

};

struct sCashShopDta // ���� ������ dta���� ����ü. ���� �� ������ �׷�(��),��ǰ(Catalog) �̹���, ��ǰ(Catalog) ����, ������ ���� �� ����Ʈ�� �����Ǿ��ִ�.
{
	vector<sImageInfo *> m_GruopList; // ��ǰ(Catalog)�� �̹��� ������ ����Ʈ.
	vector<sImageInfo *> m_CatalogImageList; // ��ǰ(Catalog)�� �̹��� ������ ����Ʈ.
	vector<sSellCatalogInfo *> m_SellCatalogList; // �Ǹ� ��� ������.(�Ǹ� ��Ͼȿ� ���� �������� �ִ� ������.)

	BOOL m_bImageLoad; // dta ������ �������� �ε������� �̹����� �ѹ��� �ε��ϱ� ���� �÷���.
};

class CControlCashShop // ��������� ���� Ŭ����.
{
public:
	CControlCashShop();
	~CControlCashShop();

	void	Init();

	void	DeleteRes();
	void	LoadRes();
	void RestoreSurfaces();

	void    OpenInit(); // ���������� �ʱ�ȭ������ϴ� �༮��.

	void ReadDta( int ShopType, const char *FileName ); // Dta ������ �ε�.
	void DummyDta( int ShopType ); // Dta ������ ����. �ӽ� �׽�Ʈ��
	void ClearDta(); // Dta �������� �ʱ�ȭ.
	void ClearDta( int i ); // Ư�� Dta ���� �ʱ�ȭ.
	void LoadFromDta( int ShopType ); // Dta�� ����� ���ϸ��� �������� �ϴ� �̹��� �ε�.

	CItem*  Draw(); // �� �Լ��� ������ DrawItemInfo()�Լ����� ǥ�õǴ� ������ ������ �����ϱ� ���� ���δ�.
	void DrawCatalogList(); // ��ǰ ��� ���.
	void GetInfo1Text( int ItemIndex ); // �������κ��� MultiText�� �о� ���� �κ�.(���� �ɶ��� �ѹ��� ȣ��������)
	int DrawInfo(); // ������,���ų��� ���� ���. ���ϰ��� ������ ����(�Ǵ� �ȼ�)���� ����.
	void DrawInven();

	LRESULT MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
	HRESULT BuyPopupMsg( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, int nMsg ); // ���� �˾��� ���ν���.

	void GroupChange( int TabIndex ); // ���� ���� �ٲ���� ���� ó��.

	int FindIconImage( int DbIndex ); // ������ �̹����� DB�ε����� ���� ã�´�.

	BOOL	IsInside( int x, int y );
	BOOL    IsInsideInven( int x, int y );

	HWND			m_hWnd;
	RECT			m_ClientRc;

	BOOL m_bResLoad; // ���ҽ� �ε�Ǿ����� �÷���

	int m_iShopKind; // � ���� �ε���. (0�� �Ϲ� ĳ������)

	CTextOutBox m_TextOut; // �ؽ�Ʈ ��� ����.
	sCashShopDta m_DtaData[MAX_CASHSHOP_KIND]; // ������ Dta ������. ����Ʈ�� ����.


	CSurface *m_pBackSur; // �������̽� ���

	CBasicButton	m_CloseBtn;        // �ݱ� ��ư

	CTabControl m_GroupTab; // ���� ���� �� (������ ������ �������ִ� ��)
	int m_NowViewCatalogPage; // ���� �׷��� �����ִ� ������.
	CSurface *m_pItemSlotSur; // ������ �ǸŸ���� �� ������ �� ��� �׷���.
	CSurface *m_pSelectSlotSur; // ���õ� ���� ǥ�����ִ� �༮.

	CSurface **m_pCatalogImageList; // ��ǰ ��� �� �������� �̹�����.

	sSellCatalogInfo *m_pWantBuyCatalog; // ���� ����� �ϴ� ���� īŻ�α��� ���� ������.




	CTabControl m_InfoTab; // ������,���ų������� �������ִ� ��.
	int m_NowOnCatalogIndex; // ���� ���콺�� �ö� ī���α��� �ε���.
	sSellCatalogInfo *m_pSelectCatalog; // ���� ���õ� īŻ�α��� ���� ������.
	CScroll m_InfoScroll; // ������ ���� ��ũ��.
	CMultiText m_MultiText; // Info ��¿� ��Ƽ�ؽ�Ʈ
	int m_NowInfoPage; // ���������� ������ ������ �ش��ϴ� ������ ��ȣ.
	CSurface *m_pInfo1BackSur; // Info1 �ÿ� ��濡 ��µ� �༮.
	CSurface *m_pInfo2BackSur; // Info2 �ÿ� ��濡 ��µ� �༮.
	CBasicButton	m_InfoPrevBtn; // ���� ������ ���� ��ư.
	CBasicButton	m_InfoNextBtn; // ���� ������ ���� ��ư.
	int m_TotalUseCash; // ������������ ǥ�õǴ� �� ��� ĳ����.




	RECT m_InvenRect; // �κ� ���� üũ��.
	CBasicButton	m_InvenTabBtn[PACK_NUM]; // �κ� �� ��ư
	CItem *m_pInvenOverItem; // �κ��� ���콺�� �ö� �������� ���� ������.



	CBasicButton	m_PrevBtn; // ���� �Ǹ� ��� ���� ��ư.
	CBasicButton	m_NextBtn; // ���� �Ǹ� ��� ���� ��ư.


	CTextOutBox m_HaveCash; // ���� ĳ��
	CBasicButton	m_DummyBuyBtn;        // ���� ��ư. ���� �̹��� �ε�� ���⼭�� �Ѵ�.
	CBasicButton	m_BuyBtn[MAX_VIEWCATALOG_NUM];        // ���� ��ư. �������� �� ����ŭ �������ִ���.������ �̹����� �ε� ���� �ʰ� ���� Dummy�� �����ؼ� ����Ѵ�.

	vector<sSellCatalogInfo *> m_ViewCatalogList; // �������� ��ǰ(Catalog)�� �̹��� ������ ����Ʈ. �� ���� �������� ����Ʈ�� �޶����� ������ �ʿ�.

	vector<sSellCatalogInfo *> m_BuyCatalogList; // ������ ��ǰ ����Ʈ.

	// ��� �ӵ� ����...���� TextOutBox�� �ϳ� �ּ� �Ź� SetString()�ϴ� ����� ���ϱ��ص� �ſ� ������. �ӵ� ����� ���� ��ټ��� ��Ʈ���� �ѹ��� ���ǽ��� �����ΰ� ����Ѵ�.(���� �ٲ�� �༮�� �׳� ���� TextOutBox�� �̿��Ѵ�.(���� ĳ�� ���� ���) )
	void MakeCatalogTextSurface(); // īŻ�α׾ȿ� ���� �ؽ�Ʈ�� ���ǽ��� �����.(�� īŻ�α� ����Ʈ�� ���鼭...)
	void MakeBuyListText( sSellCatalogInfo *pCatalog );  // ���ų����� �ؽ�Ʈ�θ� �̸� ����.
	CSurface *m_pItemInfoTextSur; // ���õ� ������ ������ �ؽ�Ʈ�� �̸� ���ǽ��� ����� ���� �༮.



};

class CCashShopIntro // ĳ�� ���� ���� �������� ���������� �� ���� ������
{
public:
	CCashShopIntro();
	~CCashShopIntro();

	void	Init();

	void	DeleteRes();
	void	LoadRes();
	void RestoreSurfaces();

	void Open(); // ���� ���ٰ�.
	void Close(); // ������ ���ٰ�.

	void  Draw(); // �� �Լ��� ������ DrawItemInfo()�Լ����� ǥ�õǴ� ������ ������ �����ϱ� ���� ���δ�.

	LRESULT MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

	BOOL	IsInside( int x, int y );

	HWND			m_hWnd;
	RECT			m_ClientRc;

	CSurface *m_pBackSur; // �������̽� ���

	CWebWnd m_WebPage1;
	CWebWnd m_WebPage2;

	CBasicButton	m_InBtn;        // ���������� ���� ��ư
	CBasicButton	m_CloseBtn;     // �ݱ� ��ư

};

#endif // _CONTROL_CASHSHOP_H_