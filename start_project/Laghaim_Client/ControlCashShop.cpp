#include "stdafx.h"



#include <string.h>
#include <TCHAR.h>
#include <direct.h>
#include "main.h"
#include "basicbutton.h"
#include "headers.h"
#include "Item.h"
#include "ItemRender.h"
#include "ItemProto.h"
#include "SpecialItem.h"
#include "Nk2DFrame.h"
#include "UIMgr.h"
#include "tcpipcon.h"
#include "TcpUtil.h"
#include "AlphaPopUp.h"
#include "textoutbox.h"
#include "controlbottom.h"
#include "controlinven.h"
#include "controltop.h"
#include "controlshop.h"
#include "controlmap.h"
#include "msgpopup.h"
#include "g_stringmanager.h"
#include "NkCharacter.h"
#include "DirtSnd.h"
#include "Mouse.h"
#include "Scroll.h"
#include "WebWorld.h"
#include "Country.h"

#include "WebCtrl.h"
#include "ControlCashShop.h"

extern DWORD	g_dwClientCountry;

extern int g_EffectSound[];
extern int gMapWidth;
extern int gMapHeight;
extern int gMapExtention;
extern char g_strFontName[];
extern BOOL	g_bFFullMode;

extern HFONT G_GetFont(int h, int w,const char  * const fontname);

bool ViewCatalogCompare(sSellCatalogInfo* l, sSellCatalogInfo* r)
{
	return l->m_CatalogIndex > r->m_CatalogIndex;
}

CControlCashShop::CControlCashShop()
	: m_hWnd(NULL)
{
	m_pBackSur = NULL;
	m_pItemSlotSur = NULL; // 아이템 판매목록의 한 아이템 당 배경 그래픽.
	m_pSelectSlotSur = NULL;
	m_pCatalogImageList = NULL;

	m_pInfo1BackSur = NULL;
	m_pInfo2BackSur = NULL;

	m_pInvenOverItem = NULL;

	m_pItemInfoTextSur = NULL;

	Init(); // 변수들 초기화.
}

CControlCashShop::~CControlCashShop()
{
	DeleteRes();
}

void CControlCashShop::Init()
{
	m_bResLoad = FALSE;

	m_iShopKind = 0;
	m_NowViewCatalogPage = 0;
	m_NowInfoPage = 0;

	m_TextOut.SetString( _T(" "), true);

	m_pWantBuyCatalog = NULL;

	m_NowOnCatalogIndex = -1;
	m_pSelectCatalog = NULL;

	m_TotalUseCash = 0;

	ZeroMemory( &m_MultiText, sizeof(CMultiText) );

	m_BuyCatalogList.reserve( 100 );
	m_ViewCatalogList.reserve( 50 );

	ClearDta(); // Dta 정보 쪽을 깔끔히 지워주기 위한 녀석.

	for( int i = 0 ; i < MAX_CASHSHOP_KIND ; ++i ) // 이건 따로 한번 더 해줘야한다.
		m_DtaData[i].m_bImageLoad = FALSE; // 이미지 로드 플래그를 해제해 두면 다음 오픈때 다시 읽어올 것이다.
}



void CControlCashShop::DeleteRes()
{
	int i = 0;

	ClearDta();

	// 구매 내역 초기화.
	vector<sSellCatalogInfo *>::iterator pr;
	for( pr = m_BuyCatalogList.begin() ; pr != m_BuyCatalogList.end() ; pr++ )
	{
		SAFE_DELETE( (*pr)->m_pTextSur ); // 서피스가 있다면 직접 지워주기.
		delete (*pr);
	}
	m_BuyCatalogList.clear();

	m_ViewCatalogList.clear(); // 참조 포인터 리스트일 뿐이므로 그냥  clear만

	m_DummyBuyBtn.DeleteRes(); // 구매 버튼의 메모리해제는 실제 메모리를 가지고 있는 이 녀석만 하면 될듯.
	for( i = 0 ; i < MAX_VIEWCATALOG_NUM ; ++i )
	{
		ZeroMemory( &m_BuyBtn[i], sizeof(m_BuyBtn[i]) ); // 참조포인터를 초기화해야 소멸자에서 메모리 해제를 안한다.
	}
	m_HaveCash.DeleteRes();
	m_InfoScroll.DeleteRes();
	m_InfoTab.DeleteRes(); // 메인탭은 열릴때마다 바뀌므로 리소스를 풀어준다.
	m_InfoPrevBtn.DeleteRes(); // 이전 아이템 보기 버튼.
	m_InfoNextBtn.DeleteRes(); // 다음 아이템 보기 버튼.

	SAFE_DELETE( m_pSelectSlotSur );
	SAFE_DELETE( m_pItemSlotSur );
	m_GroupTab.DeleteRes();
	SAFE_DELETE( m_pInfo2BackSur );
	SAFE_DELETE( m_pInfo1BackSur );


	for( i = 0 ; i < PACK_NUM ; ++i )
		m_InvenTabBtn[i].DeleteRes(); // 인벤 탭 버튼

	m_PrevBtn.DeleteRes(); // 이전 판매 목록 보기 버튼.
	m_NextBtn.DeleteRes(); // 다음 판매 목록 보기 버튼.

	m_CloseBtn.DeleteRes();
	SAFE_DELETE_ARRAY( m_pCatalogImageList );
	SAFE_DELETE( m_pBackSur );

	m_TextOut.DeleteRes();     // 텍스트 출력용 판때기

	SAFE_DELETE( m_pItemInfoTextSur);

	m_bResLoad = FALSE;
}

void CControlCashShop::LoadRes()
{
	if( !g_pDisplay )
		return;

	int i = 0, k = 0, x = 0, y = 0;

	m_hWnd =  g_pDisplay->GetHWnd();
	GetClientRect( m_hWnd, &m_ClientRc );

	g_pDisplay->CreateSurfaceFromBitmap( &m_pBackSur, "interface/CashShop/CashShopBack.bmp" );

	if( m_pBackSur )
	{
// 		m_pBackSur->SetWidth( 800 ); // 위 CreateSurface에서 CSurface 에 따로 저장해두는 m_dwWidth,m_dwHeight에 값을 저장안해주는 버그가 있어서 그냥 직접 셋팅.
// 		m_pBackSur->SetHeight( 600 );

		{
			m_pBackSur->Xpos = ( m_ClientRc.right - m_pBackSur->GetWidth() ) / 2;
			m_pBackSur->Ypos = ( m_ClientRc.bottom - m_pBackSur->GetHeight() ) / 2;
		}

		m_GroupTab.SetPosition( m_pBackSur->Xpos+40, m_pBackSur->Ypos+47, 460, 35 );
		m_GroupTab.SetIncDecBtn( "CashShop/Tab/TabDec", m_pBackSur->Xpos+19, m_pBackSur->Ypos+57, "CashShop/Tab/TabInc", m_pBackSur->Xpos+510, m_pBackSur->Ypos+57 );
		m_GroupTab.SetTabStyle( TAB_ALIGN_HORIZON, 0, 0, 0 ); // 탭 배치 ( 가로/세로, x그리기시작, y그리기시작, 여백 )

		m_InfoTab.SetPosition( m_pBackSur->Xpos+531, m_pBackSur->Ypos+45, 176, 27 );
		m_InfoTab.SetTabStyle( TAB_ALIGN_HORIZON, 10, 3, -1 ); // 탭 배치 ( 가로/세로, x그리기시작, y그리기시작, 여백 )
		m_InfoTab.AddTab( "CashShop/Tab/Tab_Info" );
		m_InfoTab.AddTab( "CashShop/Tab/Tab_BuyInfo" );

		m_InfoPrevBtn.SetFileName( "CashShop/btn_InfoPrev" );
		m_InfoPrevBtn.LoadRes();
		m_InfoPrevBtn.SetPosition(m_pBackSur->Xpos+612, m_pBackSur->Ypos+349 );

		m_InfoNextBtn.SetFileName( "CashShop/btn_InfoNext" );
		m_InfoNextBtn.LoadRes();
		m_InfoNextBtn.SetPosition(m_pBackSur->Xpos+705, m_pBackSur->Ypos+349 );

		m_InfoScroll.LoadRes( "CashShop/InfoScrollUp", "CashShop/InfoScrollDown", "interface/CashShop/InfoScrollBar.bmp" );
		m_InfoScroll.SetDrawPos( m_pBackSur->Xpos+773, m_pBackSur->Ypos+176, 164 ); // 보여지는 타입에 따라 스크롤 위치와 길이가 바뀌어야 한다.

		g_pDisplay->CreateSurfaceFromBitmap( &m_pInfo1BackSur, "interface/CashShop/Info1_Back.bmp" );
		g_pDisplay->CreateSurfaceFromBitmap( &m_pInfo2BackSur, "interface/CashShop/Info2_Back.bmp" );


		m_InvenTabBtn[0].SetFileName( "inven/inven1" );
		m_InvenTabBtn[0].LoadRes();
		m_InvenTabBtn[0].SetPosition(m_pBackSur->Xpos+760, m_pBackSur->Ypos+377 );
		m_InvenTabBtn[1].SetFileName( "inven/inven2" );
		m_InvenTabBtn[1].LoadRes();
		m_InvenTabBtn[1].SetPosition(m_pBackSur->Xpos+760, m_pBackSur->Ypos+433 );
		m_InvenTabBtn[2].SetFileName( "inven/inven3" );
		m_InvenTabBtn[2].LoadRes();
		m_InvenTabBtn[2].SetPosition(m_pBackSur->Xpos+760, m_pBackSur->Ypos+487 );


		m_PrevBtn.SetFileName( "CashShop/btn_prev" );
		m_PrevBtn.LoadRes();
		m_PrevBtn.SetPosition(m_pBackSur->Xpos+180, m_pBackSur->Ypos+565 );

		m_NextBtn.SetFileName( "CashShop/btn_next" );
		m_NextBtn.LoadRes();
		m_NextBtn.SetPosition(m_pBackSur->Xpos+330, m_pBackSur->Ypos+565 );


		m_HaveCash.Init( 12, RGB(255,255,0), 0, 0 );

		m_DummyBuyBtn.SetFileName( "CashShop/btn_buy" );
		m_DummyBuyBtn.LoadRes();

		for( i = 0 ; i < MAX_VIEWCATALOG_NUM ; ++i )
		{
			memcpy( &m_BuyBtn[i], &m_DummyBuyBtn, sizeof(m_BuyBtn[i]) ); // 이렇게 하면 이미지 서피스를 참조해서 쓸수있다.
		}

		m_CloseBtn.SetFileName( "CashShop/btn_close" );
		m_CloseBtn.LoadRes();
		m_CloseBtn.SetPosition(m_pBackSur->Xpos+766, m_pBackSur->Ypos+9 );

		// 영역 지정
		// 인벤 영역 설정
		m_InvenRect.left = m_pBackSur->Xpos + 545;
		m_InvenRect.top  = m_pBackSur->Ypos + 379;
		m_InvenRect.right = m_pBackSur->Xpos + 760;
		m_InvenRect.bottom  = m_pBackSur->Ypos + 539;
	}

	g_pDisplay->CreateSurfaceFromBitmap( &m_pItemSlotSur, "interface/CashShop/CatalogBack.bmp" );
	m_pItemSlotSur->SetWidth( 259 ); // 위 CreateSurface에서 CSurface 에 따로 저장해두는 m_dwWidth,m_dwHeight에 값을 저장안해주는 버그가 있어서 그냥 직접 셋팅.
	m_pItemSlotSur->SetHeight( 156 );
	m_pItemSlotSur->SetColorKey( RGB(255,0,255) );

	g_pDisplay->CreateSurfaceFromBitmap( &m_pSelectSlotSur, "interface/CashShop/SelCatalog.bmp" );
	m_pSelectSlotSur->SetWidth( 259 ); // 위 CreateSurface에서 CSurface 에 따로 저장해두는 m_dwWidth,m_dwHeight에 값을 저장안해주는 버그가 있어서 그냥 직접 셋팅.
	m_pSelectSlotSur->SetHeight( 256 );
	m_pSelectSlotSur->SetColorKey( RGB(255,0,255) );

	m_TextOut.Init( 12, RGB(100,100,100), 0, 0 );

	m_bResLoad = TRUE; // 임시 주석 처리
}

void CControlCashShop::RestoreSurfaces()
{
	// 무식하게 다 지웠다가 로딩.
	DeleteRes();
	LoadRes();

	LoadFromDta( m_iShopKind ); // 현재 텝의 dta 리소스만 리스토어. 나머진 플래그만 풀어준다.

	for( int i = 0 ; i < MAX_CASHSHOP_KIND ; ++i )
		m_DtaData[i].m_bImageLoad = FALSE; // 이미지 로드 플래그를 해제해 두면 다음 오픈때 다시 읽어올 것이다.
}

void CControlCashShop::OpenInit() // 열릴때마다 초기화해줘야하는 녀석들.
{
	m_GroupTab.NowDrawStartIndex = 0; // 이것도 여기서 초기화해주자.
	m_GroupTab.NowSelectedIndex = 0; // 이것도 여기서 초기화해주자.
	GroupChange( 0 ); // 처음 선택된 탭을 지정.(번 인덱스의 탭이 없으면 난감.)

	m_InfoTab.NowDrawStartIndex = 0; // 이것도 여기서 초기화해주자.
	m_InfoTab.NowSelectedIndex = 0; // 이것도 여기서 초기화해주자.

	m_InfoScroll.SetPos( 0 ); // 스크롤도 초기위치로.
	m_InfoScroll.SetDrawPos( m_pBackSur->Xpos+773, m_pBackSur->Ypos+176, 164 ); // 보여지는 타입에 따라 스크롤 위치와 길이가 바뀌어야 한다.

	m_pSelectCatalog = NULL;
	m_pWantBuyCatalog = NULL; // 혹시 꼬여도 다시 열때 풀릴수있게 여기서 구입대기 참조 포인터를 초기화.

	m_NowInfoPage = 0;

}

void CControlCashShop::ReadDta( int ShopType, const char *FileName ) // Dta 정보를 로드.
{
	PFILE *fp;
	int i = 0, j = 0;

	if(  !( fp = pfopen(FileName,"rb") )  )
	{
		MessageBox(NULL, "CashShop Dta Loading fail", "Error", MB_OK);
		return;
	}

	// 더미를 읽어서 없앤다!
	for( int dummy_count = 0 ; dummy_count < CASHSHOP_DTA_ENC_DUMMY_COUNT ; dummy_count++ )
	{
		int dummy;
		pfread( &dummy, sizeof(int), 1, fp );
	}

	int int_seed = CASHSHOP_DTA_ENC_INT_SEED;
	char char_seed = CASHSHOP_DTA_ENC_CHAR_SEED;

	int TempInt = 0;
	char strTemp[256] = ""; // 가장 길어질수있는 상품 설명글도 255까지만 받기로 약속했다.
	char strPath[256] = ""; // 경로 넣어주기용.
	int Count;

	/////////////////////////////////////////////////////////////////
	// 탭 정보 부분.
	sImageInfo *TempImageInfo = NULL;
	g_ItemList.ReadInt( &Count, fp, int_seed ); // 암호화 풀어서 값 얻어오기.
	m_DtaData[ShopType].m_GruopList.reserve( Count );

	for( i = 0 ; i < Count ; ++i )
	{
		TempImageInfo = new sImageInfo;

		if( !TempImageInfo )
			continue;

		g_ItemList.ReadInt( &TempInt, fp, int_seed ); // 암호화 풀어서 값 얻어오기.
		TempImageInfo->m_Index = TempInt; // 그룹 인덱스.

//		g_ItemList.ReadString( strTemp, fp, int_seed, char_seed ); // 이 안에서 내부적으로 스트링 길이 4바이트를 읽음에 유의.
//		strcpy( TempImageInfo->m_strName, strPath ); // 탭 이름

		g_ItemList.ReadString( strTemp, fp, int_seed, char_seed ); // 이 안에서 내부적으로 스트링 길이 4바이트를 읽음에 유의.
		sprintf( strPath, "CashShop/Tab/" );
		strcat( strPath, strTemp );
		strcpy( TempImageInfo->m_strFileName, strPath ); // 이미지 파일명...로드는 나중에

		m_DtaData[ShopType].m_GruopList.push_back( TempImageInfo );
	}
	///////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
	// 상품 이미지 리스트.
	g_ItemList.ReadInt( &Count, fp, int_seed ); // 리스트 갯수.
	m_DtaData[ShopType].m_CatalogImageList.reserve( Count );

	for( i = 0 ; i < Count ; ++i )
	{
		TempImageInfo = new sImageInfo;

		if( !TempImageInfo )
			continue;

		g_ItemList.ReadInt( &TempInt, fp, int_seed ); // 암호화 풀어서 값 얻어오기.
		TempImageInfo->m_Index = TempInt; // 이미지 인덱스.

		g_ItemList.ReadString( strTemp, fp, int_seed, char_seed ); // 이 안에서 내부적으로 스트링 길이 4바이트를 읽음에 유의.
		sprintf( strPath, "interface/CashShop/Icon/" );
		strcat( strPath, strTemp );
		strcpy( TempImageInfo->m_strFileName, strPath ); // 이미지 파일명...로드는 나중에

		m_DtaData[ShopType].m_CatalogImageList.push_back( TempImageInfo );
	}
	///////////////////////////////////////////////////////////////////



	/////////////////////////////////////////////////////////////////
	// 판매 목록 데이터.
	sSellCatalogInfo *TempCatalogInfo = NULL;
	g_ItemList.ReadInt( &Count, fp, int_seed ); // 리스트 갯수.
	m_DtaData[ShopType].m_SellCatalogList.reserve( Count );

	for( i = 0 ; i < Count ; ++i )
	{
		TempCatalogInfo = new sSellCatalogInfo;

		if( !TempCatalogInfo )
			continue;

		TempCatalogInfo->m_pTextSur = NULL; // 이 녀석 만은 직접해주자.

		g_ItemList.ReadInt( &TempInt, fp, int_seed ); // 암호화 풀어서 값 얻어오기.
		TempCatalogInfo->m_CatalogIndex = TempInt; // 상품 인덱스.

		g_ItemList.ReadInt( &TempInt, fp, int_seed ); // 암호화 풀어서 값 얻어오기.
		TempCatalogInfo->m_IconImageIndex = TempInt; // 아이콘 이미지 인덱스.

		g_ItemList.ReadString( strTemp, fp, int_seed, char_seed ); // 이 안에서 내부적으로 스트링 길이 4바이트를 읽음에 유의.
		if( strlen(strTemp) < MAX_LENGTH_CATALOG_NAME ) // 상품명 길이 제한 체크.
			strcpy( TempCatalogInfo->m_strCatalogName, strTemp ); // 상품 명

		g_ItemList.ReadString( strTemp, fp, int_seed, char_seed ); // 이 안에서 내부적으로 스트링 길이 4바이트를 읽음에 유의.
		strcpy( TempCatalogInfo->m_strCatalogInfo, strTemp ); // 상품 설명

		g_ItemList.ReadInt( &TempInt, fp, int_seed ); // 암호화 풀어서 값 얻어오기.
		TempCatalogInfo->m_Price = TempInt; // 상품 가격.

		g_ItemList.ReadInt( &TempInt, fp, int_seed ); // 암호화 풀어서 값 얻어오기.
		TempCatalogInfo->m_Milease = TempInt; // 상품 마일리지.

		g_ItemList.ReadInt( &TempInt, fp, int_seed ); // 암호화 풀어서 값 얻어오기.
		TempCatalogInfo->m_GroupIndex = TempInt; // 속해있는 그룹 번호.

		g_ItemList.ReadInt( &TempInt, fp, int_seed ); // 암호화 풀어서 값 얻어오기.
		TempCatalogInfo->m_bNewCatalog = TempInt; // 신상품 여부. 인트형이지만 현재는 0,1만 들어온다.

		g_ItemList.ReadInt( &TempInt, fp, int_seed ); // 암호화 풀어서 값 얻어오기.
		TempCatalogInfo->m_ItemCount = TempInt; // 속해있는 아이템 갯수.

		for( j = 0 ; j < TempCatalogInfo->m_ItemCount ; ++j )
		{
			CItem *TempItem = &(TempCatalogInfo->m_Item[j]); // 참조 포인터.

			g_ItemList.ReadInt( &TempInt, fp, int_seed ); // 암호화 풀어서 값 얻어오기.
			TempItem->m_Vnum = TempInt; // vnum

			TempItem->m_ProtoNum = g_ItemList.FindItem( TempItem->m_Vnum ); // ProtoNum 입력

			g_ItemList.ReadInt( &TempInt, fp, int_seed ); // 암호화 풀어서 값 얻어오기.
			if( GET_TYPE(TempItem) == ITYPE_SUB ) // plus
				TempItem->m_PlusNum = TempInt;
			else if( TempInt > 0 )
			{
				if( GET_ITEM_LEVEL(TempItem) >= 181 )
					TempItem->SetDiaGemNum( TempInt );
				else
					TempItem->SetGemNum( TempInt );
			}


			g_ItemList.ReadInt( &TempInt, fp, int_seed ); // 암호화 풀어서 값 얻어오기.
			TempItem->m_Special = TempInt; // 옵션1

			g_ItemList.ReadInt( &TempInt, fp, int_seed ); // 암호화 풀어서 값 얻어오기.
			TempItem->m_Special2 = TempInt; // 옵션2 // 이 버젼엔 옵션2가 없으니 읽기만하고 띵깐다.

			// 기타 값들 셋팅.
			ResetItemValue( TempItem );
			SetSpecialValue( TempItem, GET_TYPE(TempItem), GET_SHAPE(TempItem), TempItem->m_Special, TempItem->m_Special2 );

			g_ItemList.ReadInt( &TempInt, fp, int_seed ); // 암호화 풀어서 값 얻어오기.
//			TempItem->m_UpgradeEndurance = TempInt; // 내구도. // 이 버젼엔 내구도가 없으니 읽기만하고 띵깐다.
//			TempItem->m_MaxUpgradeEndurance = TempInt; // 내구도.

			g_ItemList.ReadInt( &TempInt, fp, int_seed ); // 암호화 풀어서 값 얻어오기.
			if( TempInt ) // 시간제한.
				TempItem->SetTimeLimitItem( TempInt );

		}

		m_DtaData[ShopType].m_SellCatalogList.push_back( TempCatalogInfo );
	}
	///////////////////////////////////////////////////////////////////

	pfclose(fp);

	MakeCatalogTextSurface(); // 텍스트 서피스를 미리 만들어준다.
}

// 임시테스트용.
void CControlCashShop::DummyDta( int ShopType ) // Dta 정보를 만듬.
{
	int i = 0, j = 0;

	int TempInt = 0;
	char strTemp[256] = ""; // 가장 길어질수있는 상품 설명글도 255까지만 받기로 약속했다.
	char strPath[256] = "";
	int Count;

	/////////////////////////////////////////////////////////////////
	// 탭 정보 부분.
	sImageInfo *TempImageInfo = NULL;
	Count = 4;
	m_DtaData[ShopType].m_GruopList.reserve( Count );

	char TabName[4][256] =
	{
		{ "All" } , { "Armor" }, { "Sub" }, { "Etc" }
	};

	char TabFileName[4][256] =
	{
		{ "Tab_Weapon" } , { "Tab_Armor" }, { "Tab_Sub" }, { "Tab_Etc" }
	};

	for( i = 0 ; i < Count ; ++i )
	{
		TempImageInfo = new sImageInfo;

		if( !TempImageInfo )
			continue;

		TempImageInfo->m_Index = i; // 그룹 인덱스.

//		sprintf( strPath, TabName[i] );
//		strcpy( TempImageInfo->m_strName, strPath ); // 탭 이름

		sprintf( strPath, "CashShop/Tab/" );
		strcat( strPath, TabFileName[i] );
		strcpy( TempImageInfo->m_strFileName, strPath ); // 이미지 파일명...로드는 나중에

		m_DtaData[ShopType].m_GruopList.push_back( TempImageInfo );
	}
	///////////////////////////////////////////////////////////////////

	char IconName[2][256] =
	{
		{ "Icon_Weapon" }, { "Icon_Armor" } // , { "Icon_Helmet" }, { "Icon_Shield" }, { "Icon_Etc" }
	};

	/////////////////////////////////////////////////////////////////
	// 상품 이미지 리스트.
	Count = 2;
	m_DtaData[ShopType].m_CatalogImageList.reserve( Count );

	for( i = 0 ; i < Count ; ++i )
	{
		TempImageInfo = new sImageInfo;

		if( !TempImageInfo )
			continue;

		TempImageInfo->m_Index = i; // 이미지 인덱스.

		sprintf( strPath, "interface/CashShop/Icon/" );
		strcat( strPath, IconName[i] );
		strcat( strPath, ".bmp" ); // 툴에서의 입력방식 통일을 위해 확장자는 클라이언트에서 붙여주자.
		strcpy( TempImageInfo->m_strFileName, strPath ); // 이미지 파일명...로드는 나중에

		m_DtaData[ShopType].m_CatalogImageList.push_back( TempImageInfo );
	}
	///////////////////////////////////////////////////////////////////



	/////////////////////////////////////////////////////////////////
	// 판매 목록 데이터.
	sSellCatalogInfo *TempCatalogInfo = NULL;
	Count = 20;
	m_DtaData[ShopType].m_SellCatalogList.reserve( Count );

	for( i = 0 ; i < Count ; ++i )
	{
		TempCatalogInfo = new sSellCatalogInfo;

		if( !TempCatalogInfo )
			continue;

		TempCatalogInfo->m_pTextSur = NULL; // 이 녀석 만은 직접해주자.

		TempCatalogInfo->m_CatalogIndex = i; // 상품 인덱스.

		TempCatalogInfo->m_IconImageIndex = rand()%m_DtaData[ShopType].m_CatalogImageList.size(); // 아이콘 이미지 인덱스.

		sprintf( strTemp, G_STRING(IDS_LHSTRING1735), i );
		if( strlen(strTemp) < MAX_LENGTH_CATALOG_NAME ) // 상품명 길이 제한 체크.
			strcpy( TempCatalogInfo->m_strCatalogName, strTemp ); // 상품 명

		sprintf( strTemp, G_STRING(IDS_LHSTRING1736), i );
		strcpy( TempCatalogInfo->m_strCatalogInfo, strTemp ); // 상품 설명

		TempCatalogInfo->m_Price = 100 * i; // 상품 가격.

		TempCatalogInfo->m_Milease = 10*i; // 상품 마일리지.

		TempCatalogInfo->m_GroupIndex = rand()%4; // 속해있는 그룹 번호.


		TempCatalogInfo->m_ItemCount = rand()%4 + 1; // 속해있는 아이템 갯수.

		for( j = 0 ; j < TempCatalogInfo->m_ItemCount ; ++j )
		{
			CItem *TempItem = &(TempCatalogInfo->m_Item[j]); // 참조 포인터.

			TempItem->m_Vnum = 860+i+j; // vnum

			TempItem->m_ProtoNum = g_ItemList.FindItem( TempItem->m_Vnum ); // ProtoNum 입력

			TempInt = rand()%15;
			if( GET_TYPE(TempItem) == ITYPE_SUB ) // plus
				TempItem->m_PlusNum = TempInt;
			else if( TempInt > 0 )
			{
				if( GET_ITEM_LEVEL(TempItem) >= 181 )
					TempItem->SetDiaGemNum( TempInt );
				else
					TempItem->SetGemNum( TempInt );
			}


			TempItem->m_Special = 4095; // 옵션1

			TempItem->m_Special2 = 0; // 옵션2

			// 기타 값들 셋팅.
			ResetItemValue( TempItem );
			SetSpecialValue( TempItem, GET_TYPE(TempItem), GET_SHAPE(TempItem), TempItem->m_Special, TempItem->m_Special2 );

//			TempItem->m_UpgradeEndurance = 0; // 내구도.
//			TempItem->m_MaxUpgradeEndurance = 0; // 내구도.

			TempInt = 0;
			if( TempInt ) // 시간제한.
				TempItem->SetTimeLimitItem( TempInt );

		}

		m_DtaData[ShopType].m_SellCatalogList.push_back( TempCatalogInfo );
	}
	///////////////////////////////////////////////////////////////////

	MakeCatalogTextSurface(); // 텍스트 서피스를 미리 만들어준다.
}

void CControlCashShop::ClearDta() // Dta 정보들을 초기화.
{
	for( int i = 0 ; i < MAX_CASHSHOP_KIND ; ++i )
	{
		vector<sImageInfo *>::iterator pr4;
		for( pr4= m_DtaData[i].m_GruopList.begin() ; pr4 != m_DtaData[i].m_GruopList.end() ; pr4++ )
			delete (*pr4);
		m_DtaData[i].m_GruopList.clear();

		vector<sImageInfo *>::iterator pr;
		for( pr = m_DtaData[i].m_CatalogImageList.begin() ; pr != m_DtaData[i].m_CatalogImageList.end() ; pr++ )
			delete (*pr);
		m_DtaData[i].m_CatalogImageList.clear();

		vector<sSellCatalogInfo *>::iterator pr2;
		for( pr2 = m_DtaData[i].m_SellCatalogList.begin() ; pr2 != m_DtaData[i].m_SellCatalogList.end() ; pr2++ )
		{
			SAFE_DELETE( (*pr2)->m_pTextSur ); // 서피스가 있다면 직접 지워주기.
			delete (*pr2);
		}
		m_DtaData[i].m_SellCatalogList.clear();
	}

	m_pInvenOverItem = NULL; // 닫힐때 아이템 정보창이 꺼지도록...
}

void CControlCashShop::ClearDta( int i ) // 특정 Dta 만을 초기화.
{
	vector<sImageInfo *>::iterator pr4;
	for( pr4= m_DtaData[i].m_GruopList.begin() ; pr4 != m_DtaData[i].m_GruopList.end() ; pr4++ )
		delete (*pr4);
	m_DtaData[i].m_GruopList.clear();

	vector<sImageInfo *>::iterator pr;
	for( pr = m_DtaData[i].m_CatalogImageList.begin() ; pr != m_DtaData[i].m_CatalogImageList.end() ; pr++ )
		delete (*pr);
	m_DtaData[i].m_CatalogImageList.clear();

	vector<sSellCatalogInfo *>::iterator pr2;
	for( pr2 = m_DtaData[i].m_SellCatalogList.begin() ; pr2 != m_DtaData[i].m_SellCatalogList.end() ; pr2++ )
	{
		SAFE_DELETE( (*pr2)->m_pTextSur ); // 서피스가 있다면 직접 지워주기.
		delete (*pr2);
	}
	m_DtaData[i].m_SellCatalogList.clear();
}

void CControlCashShop::LoadFromDta( int ShopType )
{
	if( !g_pDisplay )
		return;

	int i = 0;
	char strTemp[5]; // 탭 인덱스를 집어넣기 위한녀석.

	// 탭 로드.
	for( i = 0 ; i < m_DtaData[ShopType].m_GruopList.size() ; ++i )
	{
		sprintf( strTemp, "%d", m_DtaData[ShopType].m_GruopList[i]->m_Index );
		m_GroupTab.AddTab( m_DtaData[ShopType].m_GruopList[i]->m_strFileName, strTemp );
	}
	m_GroupTab.NowSelectedIndex = 0; // 유료료 상점은 2번째 탭을 우선 보여주도록.

	// 상품 이미지 로드
	int ImageCount = m_DtaData[ShopType].m_CatalogImageList.size();

	m_pCatalogImageList = new CSurface*[ImageCount];

	if( m_pCatalogImageList )
	{
		for( i = 0 ; i < ImageCount ; ++i )
		{
			g_pDisplay->CreateSurfaceFromBitmap( &(m_pCatalogImageList[i]), m_DtaData[ShopType].m_CatalogImageList[i]->m_strFileName );
		}
	}

	m_DtaData[ShopType].m_bImageLoad = TRUE;
}

CItem* CControlCashShop::Draw() // 이 함수의 리턴은 DrawItemInfo()함수에서 표시되는 아이템 정보를 갱신하기 위해 쓰인다.
{
	char strTemp[256] = ""; // 가장 길어질수있는 상품 설명글도 255까지만 받기로 약속했다.

	if( !g_pDisplay )
		return NULL;

	g_pDisplay->Blt( m_pBackSur->Xpos, m_pBackSur->Ypos, m_pBackSur );

	m_GroupTab.Draw();

	// 판매목록 드로우.
	DrawCatalogList();

	m_InfoTab.Draw();
	// 정보 드로우.
	DrawInfo();
	m_InfoScroll.Draw();

	m_InvenTabBtn[g_pRoh->m_CurPackNum].SetState( BTN_DOWN ); // 선택된건 눌려서 표시되게...
	for( int i = 0 ; i < PACK_NUM ; ++i )
		m_InvenTabBtn[i].Draw(); // 인벤 탭 버튼

	DrawInven();


	m_PrevBtn.Draw(); // 이전 판매 목록 보기 버튼.
	m_NextBtn.Draw(); // 다음 판매 목록 보기 버튼.

	// 보유 캐쉬 출력.
	sprintf( strTemp, "%d", g_pRoh->m_HaveCash );
	m_TextOut.SetString( strTemp, RGB(255,255,0) );
	m_TextOut.Draw( m_pBackSur->Xpos+715, m_pBackSur->Ypos+563, 0 );

	m_CloseBtn.Draw();

	pCMyApp->m_pMouse->SetMouseType( M_NORMAL ); // 마우스 타입을 강제로...

	return m_pInvenOverItem; // 아이템 정보를 출력안한다.
}

void CControlCashShop::DrawCatalogList() // 상품 목록 출력.
{
	char strTemp[256] = ""; // 가장 길어질수있는 상품 설명글도 255까지만 받기로 약속했다.

	int ViewInPageNum = MAX_VIEWCATALOG_NUM; // 한 페이지에 보여지는 상품 갯수.(추후 해상도별로 추가되야할지 모른다.)
	int ViewHorizonNum = 2; // 보이는 상품 가로 갯수.
	int ViewVerticalNum = 3; // 보이는 상품 세로 갯수.

	int TempInt = 0;
	int TempX = 0, TempY = 0;

	int i = 0;

	for( i = 0 ; i < MAX_VIEWCATALOG_NUM ; ++i )
		m_BuyBtn[i].SetDisable( TRUE ); // 일단 버튼들을 전부 비활성화 시킨다.(메시지 프로시져 체크를 위해..)

	m_NowOnCatalogIndex = -1; // 현재 마우스가 올라간 상품 번호 초기화.

	POINT point;
	GetCursorPos(&point);
	ScreenToClient(g_pDisplay->GetHWnd(), &point); // 현재 마우스 올라가있는 녀석을 기억하기 위해서...

	sSellCatalogInfo *pCatalog;
	vector<sSellCatalogInfo *>::iterator pr;

	int DrawCount = 0;

	// 보여질 상품들을 전부 돌면서.
	for( pr = m_ViewCatalogList.begin() ; pr != m_ViewCatalogList.end() ; pr++ )
	{
		pCatalog = *pr;

		if( DrawCount < m_NowViewCatalogPage*ViewInPageNum ) // 보여질 녀석들 보다 앞쪽이면..
		{
			++DrawCount; // 카운트는 증가시켜야한다.
			continue; // 띵깐다.
		}

		TempInt = DrawCount % ViewInPageNum; // 6개중 몇번째 녀석인가..
		TempX = m_pBackSur->Xpos + 12 + ( (TempInt%ViewHorizonNum) * 261 ); // 그려질 X좌표 계산
		TempY = m_pBackSur->Ypos + 90 + ( (TempInt/ViewHorizonNum) * 156 ); // 그려질 Y좌표 계산

		g_pDisplay->Blt( TempX, TempY, m_pItemSlotSur ); // 카탈로그 배경 출력.

		// 아이템 이미지(아이콘) 출력
		if( m_pCatalogImageList[ FindIconImage(pCatalog->m_IconImageIndex) ] )
			g_pDisplay->Blt( TempX+5, TempY+14, m_pCatalogImageList[ FindIconImage(pCatalog->m_IconImageIndex) ] ); // 아이콘 이미지 출력.

		if( pCatalog->m_pTextSur )
			g_pDisplay->Blt( TempX, TempY, pCatalog->m_pTextSur ); // 텍스트들 출력.

		// 구매 버튼 출력.
		m_BuyBtn[TempInt].SetPosition( TempX+187, TempY+126 ); // 이걸 여기서 하는건 비효율 적이지만 좌표계산이 편하니 그냥 여기서한다.
		m_BuyBtn[TempInt].SetDisable( FALSE ); // 보이는 녀석만 활성화.(메시지 프로시져때문에 필요.)
		m_BuyBtn[TempInt].Draw();

		// 선택된 녀석 처리.
		if( pCatalog == m_pSelectCatalog ) // 선택된 녀석이면...
			g_pDisplay->Blt( TempX, TempY, m_pSelectSlotSur ); // 테두리를 처준다.


		++DrawCount;

		// 마우스가 올라갔는지 체크.
		if( point.x >= TempX && point.x <= TempX+m_pItemSlotSur->GetWidth()
				&& point.y >= TempY && point.y <= TempY+m_pItemSlotSur->GetHeight() ) // 이 녀석한테 마우스가 올라가있으면...
			m_NowOnCatalogIndex = pCatalog->m_CatalogIndex;

		if( DrawCount >= m_NowViewCatalogPage*ViewInPageNum+ViewInPageNum ) // 보여질 녀석들을 다 지나갔으면...
			break; // 중단
	}

	// 페이지 번호 출력
	sprintf( strTemp, "[%02d/%02d]", m_NowViewCatalogPage+1, m_ViewCatalogList.size()?(m_ViewCatalogList.size()-1)/ViewInPageNum+1:1 );
	m_TextOut.SetString( strTemp, FALSE );
	m_TextOut.Draw( m_pBackSur->Xpos+243, m_pBackSur->Ypos+568 );

}

void CControlCashShop::GetInfo1Text( int ItemIndex ) // 상세정보부분의 MultiText를 읽어 오는 부분.(변경 될때만 한번씩 호출해주자)
{
	m_MultiText.Reset(); // 일단 한번 지우고.
	SAFE_DELETE( m_pItemInfoTextSur ); // 이 녀석도 아이템 바뀔땐 무조건 지우고 본다.

	if( m_pSelectCatalog->m_ItemCount == 0 || ItemIndex >= m_pSelectCatalog->m_ItemCount ) // 아이템이 없거나 이상 범위체크.
		return;

	CItem *pItem = &(m_pSelectCatalog->m_Item[ItemIndex]); // 포인터로 바꿔서...

	if( !pItem || pItem->m_ProtoNum == -1 ) // protoNum도 반드시 체크해야한다.
		return;

	// MultiText를 갱신.
	g_pNk2DFrame->GetControlInven()->m_bDrawInfoUpdate = TRUE; // 아이템 정보를 강제로 갱신시킨다.(이렇게 안하면 아이템을 들거나 다른 아이템을 봤다가 와야지 갱신된다.)
	g_pNk2DFrame->GetControlInven()->DrawItemInfo( pItem, ITEM_INFO_CASHSHOP ); // ITEM_INFO_CASHSHOP 은 화면에 출력하지 않도록...
	////////////////////////////////////////////////////////////////////////
	// 멀티 텍스트를 미리 서피스로 만들어 놓는다.
	// 크기 계산.
	int TextSurHeight = m_MultiText.m_LineCount*(14+14);	// 세로 길이를 여백 고려해서 좀 넉넉하게...

	g_pDisplay->CreateSurface( &m_pItemInfoTextSur, 230, TextSurHeight ); // 한 구매 내역 출력 위치로 잡고...
	if( m_pItemInfoTextSur )
	{
		m_pItemInfoTextSur->SetWidth( 230 ); // 가로 크기는 픽스.
		m_pItemInfoTextSur->SetHeight( TextSurHeight );

		m_pItemInfoTextSur->Clear( 0 );
		m_pItemInfoTextSur->SetColorKey( 0 );

		HFONT TempFont = G_GetFont(14,0,g_strFontName);

		for( int i = 0 ; i < m_MultiText.m_LineCount ; ++i ) // 라인 수만큼 루프.
			m_pItemInfoTextSur->DrawText( TempFont, m_MultiText.m_Text[i], 10, 10+(i*13), 0, m_MultiText.m_TextColor[i], 0 );
	}


	// 상세정보 보기 중이었으면 스크롤 레인지도 갱신.(서피스의 픽셀 단위로...)
	if( m_InfoTab.NowSelectedIndex == 0 )
	{
		if( m_pItemInfoTextSur->GetHeight() >= VIEW_INFO1_LINE+1 )
			m_InfoScroll.SetRange( m_pItemInfoTextSur->GetHeight() - VIEW_INFO1_LINE + 1 ); // 보여질 라인에 맡춰서 범위 지정.
		else
			m_InfoScroll.SetRange( 1 );

		m_InfoScroll.SetPos( 0 );
	}
}

int CControlCashShop::DrawInfo() // 상세정보,구매내역 등의 출력.
{
	char strTemp[256];
	int i = 0, j = 0;
	RECT DrawRect; // 출력 영역 계산용.

	int DrawCount = 0; // 그려진 픽셀단위 카운트. 스크롤위치에 따른 스킵에 사용.

	switch( m_InfoTab.NowSelectedIndex )
	{
	case 0: // 아이템 상세 정보.

		g_pDisplay->Blt( m_pBackSur->Xpos+540, m_pBackSur->Ypos+67, m_pInfo1BackSur ); // 인포 배경 출력.

		if( m_pSelectCatalog && m_pSelectCatalog->m_ItemCount > 0 && m_pSelectCatalog->m_ItemCount <= MAX_ITEM_IN_CATALOG ) // 참조 포인터가 있다면...
		{
			CItem *pItem = &(m_pSelectCatalog->m_Item[m_NowInfoPage]); // 포인터로 바꿔서...

			if( !pItem )
				return DrawCount;

			// 아이템 이미지를 출력.
			CLolos *pItemLolo = GET_LOLO(pItem);

			if( pItem->m_ProtoNum != -1 && pItemLolo ) // ProtoNum도 꼭 체크해야함에 유의.
			{
				DWORD ambient = 0x00555555;

				if( GET_TYPE(pItem) == ITYPE_ETC && GET_SHAPE(pItem) == IETC_SCROLL )
					ambient = 0x00cccccc;
				else if( pItem->m_PlusNum > 0 )
					ambient = UPGRADE_ITEM_COLOR;

				int ImageWidth = SLOT_WIDTH*GET_ITEM_WIDTH(pItem); // 롤로 이미지의 화면 가로 길이.
				int ImageHeight = SLOT_HEIGHT*GET_ITEM_HEIGHT(pItem); // 롤로 이미지의 화면 세로 길이.

				if( GET_ITEM_WIDTH(pItem) == 1 ) // 아이템 크기가 1인 경우 위치가 이상하더라 좀 조정해준다.
					DrawRect.left = m_pBackSur->Xpos+649-(ImageWidth/2);
				else
					DrawRect.left = m_pBackSur->Xpos+663-(ImageWidth/2);

				DrawRect.top = m_pBackSur->Ypos+70;


				g_pNk2DFrame->GetItemRender()->BeginRenderLolo();

				g_pNk2DFrame->RenderItemInUI( DrawRect.left, DrawRect.top, pItem , ambient, FALSE, TRUE , TRUE );


				g_pNk2DFrame->GetItemRender()->EndRenderLolo();
			}

			if( m_pItemInfoTextSur ) // 설명부 텍스트 서피스가 있다면..
			{
				// 클리핑해서 출력해야한다.
				DrawRect.left = 0;
				DrawRect.top = m_InfoScroll.GetPos();
				DrawRect.right =  m_pItemInfoTextSur->GetWidth();
				if( m_pItemInfoTextSur->GetHeight() > VIEW_INFO1_LINE+DrawRect.top )
					DrawRect.bottom =  VIEW_INFO1_LINE+DrawRect.top;
				else
					DrawRect.bottom =  m_pItemInfoTextSur->GetHeight();


				g_pDisplay->Blt( m_pBackSur->Xpos+545, m_pBackSur->Ypos+175, m_pItemInfoTextSur, &DrawRect ); // 텍스트들 출력.(클리핑해야한다.)
			}

			// 페이지 번호 출력
			sprintf( strTemp, "[%02d/%02d]", m_NowInfoPage+1, m_pSelectCatalog->m_ItemCount );
			m_TextOut.SetString( strTemp, RGB(255,255,255) );
			m_TextOut.Draw( m_pBackSur->Xpos+665, m_pBackSur->Ypos+353, 0 );

			m_InfoPrevBtn.Draw(); // 이전 판매 목록 보기 버튼.
			m_InfoNextBtn.Draw(); // 다음 판매 목록 보기 버튼.
		}
		break;

	case 1: // 구매 내역.

		g_pDisplay->Blt( m_pBackSur->Xpos+540, m_pBackSur->Ypos+67, m_pInfo2BackSur ); // 인포 배경 출력.

		sSellCatalogInfo *pCatalog;
		vector<sSellCatalogInfo *>::iterator pr;

		// 샀던 상품들을 전부 돌면서.
		for( pr = m_BuyCatalogList.begin() ; pr != m_BuyCatalogList.end() ; pr++ )
		{
			pCatalog = *pr;

			if( DrawCount < m_InfoScroll.GetPos() ) // 여기선 픽셀이 아니라 상품단위로 사용.
			{
				++DrawCount; // 상품단위로 증가.
				continue;
			}

			if( pCatalog->m_pTextSur )
				g_pDisplay->Blt( m_pBackSur->Xpos+548, m_pBackSur->Ypos+69+((DrawCount-m_InfoScroll.GetPos())*30), pCatalog->m_pTextSur ); // 텍스트들 출력.

			++DrawCount;

			if( DrawCount >= m_InfoScroll.GetPos() + VIEW_INFO2_LINE ) // 보여질 량을 넘어섰으면...(맨 마지막은 총합을 출력하기 위해 비워둔다.)
				break; // 루프만 중단.
		}

		//-- IDS_TOTAL_COUNT : TotalCount : %d
		sprintf( strTemp, (char*)G_STRING(IDS_TOTAL_COUNT), m_BuyCatalogList.size() );
		m_TextOut.SetString( strTemp, RGB(255,255,200) ); // 총 구입 갯수.
		m_TextOut.Draw( m_pBackSur->Xpos+548, m_pBackSur->Ypos+343 );
		//-- IDS_TOTAL_CASH : TotalCash  : %d
		sprintf( strTemp, (char*)G_STRING(IDS_TOTAL_CASH), m_TotalUseCash );
		m_TextOut.SetString( strTemp, RGB(255,255,200) ); // 총 캐쉬
		m_TextOut.Draw( m_pBackSur->Xpos+548, m_pBackSur->Ypos+357 );

		break;
	}

	return DrawCount;
}

#define GET_SLOT_X(point_x)		((point_x - m_slotstart_x) / SLOT_WIDTH)
#define GET_SLOT_Y(point_y)		((point_y - m_slotstart_y) / SLOT_HEIGHT)

void CControlCashShop::DrawInven()
{
	POINT point;
	GetCursorPos(&point);
	ScreenToClient(g_pDisplay->GetHWnd(), &point);

	CItem *pItem = NULL;
	int slot_x_num, slot_y_num;

	if( g_pNk2DFrame->GetItemRender() && g_pRoh )
	{
		g_pNk2DFrame->GetItemRender()->BeginRenderLolo();

		// 소지품 그리기...
		if( point.x >= m_InvenRect.left && point.y >= m_InvenRect.top )
		{
			slot_x_num = (point.x - m_InvenRect.left) / SLOT_WIDTH;
			slot_y_num = (point.y - m_InvenRect.top) / SLOT_HEIGHT;
		}
		else
		{
			slot_x_num = -1;
			slot_y_num = -1;
		}
		int index = g_pRoh->GetSlotIndex(slot_x_num, slot_y_num);
		pItem = g_pRoh->m_Inven[g_pRoh->m_CurPackNum];

		m_pInvenOverItem = NULL;

		while (pItem)
		{
			RECT rcRect;
			rcRect.left = 0;
			rcRect.right = SLOT_WIDTH * GET_ITEM_WIDTH(pItem);
			rcRect.top = 0;
			rcRect.bottom = SLOT_HEIGHT * GET_ITEM_HEIGHT(pItem);
			g_pDisplay->Blt( m_InvenRect.left + (SLOT_WIDTH*pItem->m_SlotX),
							 m_InvenRect.top + (SLOT_HEIGHT*pItem->m_SlotY),
							 g_pNk2DFrame->GetControlInven()->m_pExistSlot, &rcRect);


			if (pItem->m_Index == index )
			{
				if( pItem->m_SlotY == 5 && GET_ITEM_HEIGHT(pItem) == 1 ) // ITEM_RENDER_HEIGHT 가 잘못되어있는 문제때문에 예외처리.
				{
					// 캐쉬샵에서 가장 마지막줄의 1슬롯짜리 아이템은 출력이 잘못되는 문제가 있다.
					// 원인은 ITEM_RENDER_HEIGHT가 잘못되어서 이지만 이걸 줄여서 정상으로 고치면 기존 아이템 크기보다
					// 작게 보인다.(이게 정상 크기이긴하지만) 작게 보이면 안좋으니 ITEM_RENDER_HEIGHT 는 그대로 버그상태로 두고
					// 당장 문제가 되는 캐쉬샵에서 마지막줄의 1슬롯짜리 아이템만 예외처리한다.
					// 퀵슬롯의 방식을 따라한다. (위치 계산과 가로,세로크기를 -1로 넣어주면 정상 출력이 된다.)
					int x = m_InvenRect.left + pItem->m_SlotX * SLOT_WIDTH;
					int y = m_InvenRect.top + pItem->m_SlotY * SLOT_HEIGHT;

					g_pNk2DFrame->RenderItemInUI( x, y , pItem , TRUE, FALSE, FALSE, TRUE );

				}
				else
				{
					g_pNk2DFrame->RenderItemInUI( m_InvenRect.left + pItem->m_SlotX * SLOT_WIDTH,
												  m_InvenRect.top + pItem->m_SlotY * SLOT_HEIGHT,
												  pItem , TRUE, FALSE, FALSE , TRUE );
				}

				m_pInvenOverItem = pItem; // 참조 포인터만 기억.(출력은 외부에서 한다.글야 배경이 제대로 나오더라.)
			}
			else
			{
				DWORD ambient = 0x00555555;
				//스크롤과 재분배권 ambient값 변경

				if( g_pNk2DFrame->IsScroll(pItem)
						|| pItem->m_Vnum == 1251
						|| pItem->m_Vnum == 1308 // 이판사판 행운권 밝기 조절 05-10-05 원석
						|| pItem->m_Vnum == 1286
						|| ( pItem->m_Vnum >= 1598 && pItem->m_Vnum <= 1695 ) // 브라질 요청 남미국기망토 종류도 성조기에서 응용이므로 예외처리.
						|| pItem->m_Vnum == 1287 ) // 미국 성조기 망토, 성조기 날개도 어둡게 보여서 클라이언트에서 직접 밝게 처리.(by 원석)
					ambient = 0x00cccccc;
				else if (pItem->m_PlusNum > 0)
					ambient = UPGRADE_ITEM_COLOR;

				if( pItem->m_SlotY == 5 && GET_ITEM_HEIGHT(pItem) == 1 ) // ITEM_RENDER_HEIGHT 가 잘못되어있는 문제때문에 예외처리.
				{
					// 캐쉬샵에서 가장 마지막줄의 1슬롯짜리 아이템은 출력이 잘못되는 문제가 있다.
					// 원인은 ITEM_RENDER_HEIGHT가 잘못되어서 이지만 이걸 줄여서 정상으로 고치면 기존 아이템 크기보다
					// 작게 보인다.(이게 정상 크기이긴하지만) 작게 보이면 안좋으니 ITEM_RENDER_HEIGHT 는 그대로 버그상태로 두고
					// 당장 문제가 되는 캐쉬샵에서 마지막줄의 1슬롯짜리 아이템만 예외처리한다.
					// 퀵슬롯의 방식을 따라한다. (위치 계산과 가로,세로크기를 -1로 넣어주면 정상 출력이 된다.)
					int x = m_InvenRect.left + pItem->m_SlotX * SLOT_WIDTH;
					int y = m_InvenRect.top + pItem->m_SlotY * SLOT_HEIGHT;

					g_pNk2DFrame->RenderItemInUI( x, y, pItem , FALSE , FALSE, FALSE, TRUE );

				}
				else
				{
					g_pNk2DFrame->RenderItemInUI( m_InvenRect.left + pItem->m_SlotX * SLOT_WIDTH,
												  m_InvenRect.top + pItem->m_SlotY * SLOT_HEIGHT,
												  pItem , FALSE, FALSE, FALSE, TRUE );
				}
			}
			pItem = pItem->m_Next;
		}
		g_pNk2DFrame->GetItemRender()->EndRenderLolo();
	}
}


LRESULT CControlCashShop::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	char strTemp[256];
	int x = LOWORD( lParam );
	int y = HIWORD( lParam );

	if( m_pWantBuyCatalog ) // 구입 대기 상품 포인터가 있다면...팝업이나 패킷 기다리는 중이다.
		return 1; // 다른짓 못하게...

	if( m_InfoScroll.MsgProc(hWnd, msg, wParam, lParam) == S_OK )
		return 1;

	int i = 0;

	switch (msg)
	{
	case 0x020A: // 휠마우스 메세지(리스트 영역안에서의 휠 메세지를 강제로 스크롤에 연동시켜준다.)

		if( IsInside(x,y) ) // 영역안의 휠은 하나 뿐.
		{
			if( (short)HIWORD(wParam) < 0 )
			{
				if( m_InfoTab.NowSelectedIndex == 0 ) // 상세 정보는 픽셀 단위므로 좀 많이 움직여준다.
					m_InfoScroll.NowPos += 10;
				else
					m_InfoScroll.NowPos += 1;

				if( m_InfoScroll.NowPos > m_InfoScroll.RangeMax-1 )
					m_InfoScroll.NowPos = m_InfoScroll.RangeMax-1;
			}
			else if( (short)HIWORD(wParam) > 0 )
			{
				if( m_InfoTab.NowSelectedIndex == 0 ) // 상세 정보는 픽셀 단위므로 좀 많이 움직여준다.
					m_InfoScroll.NowPos -= 10;
				else
					m_InfoScroll.NowPos -= 1;

				if( m_InfoScroll.NowPos < 0 )
					m_InfoScroll.NowPos = 0;
			}
			return 1;
		}
		break;

	case WM_LBUTTONDOWN:

		if( IsInside(x,y) ) // 인터페이스의 안쪽을 클릭했을 경우.
		{
			m_GroupTab.MsgProc(hWnd, msg, wParam, lParam);
			m_InfoTab.MsgProc(hWnd, msg, wParam, lParam);
			m_InfoPrevBtn.MsgProc(hWnd, msg, wParam, lParam);
			m_InfoNextBtn.MsgProc(hWnd, msg, wParam, lParam);

			// BUY 버튼 체크
			for( i = 0 ; i < MAX_VIEWCATALOG_NUM ; ++i )
				m_BuyBtn[i].MsgProc(hWnd, msg, wParam, lParam);

			for( i = 0 ; i < PACK_NUM ; ++i )
				m_InvenTabBtn[i].MsgProc(hWnd, msg, wParam, lParam);

			m_PrevBtn.MsgProc(hWnd, msg, wParam, lParam);
			m_NextBtn.MsgProc(hWnd, msg, wParam, lParam);

			m_CloseBtn.MsgProc(hWnd, msg, wParam, lParam);

			// 상품 선택 체크.
			if( m_NowOnCatalogIndex != -1 ) // 이게 -1이 아니면 마우스가 어느 상품 위에 올라가 있던거다.
			{
				sSellCatalogInfo *pCatalog;
				vector<sSellCatalogInfo *>::iterator pr;

				int DrawCount = 0;

				// 보여질 상품들을 전부 돌면서.
				for( pr = m_ViewCatalogList.begin() ; pr != m_ViewCatalogList.end() ; pr++ )
				{
					pCatalog = *pr;

					if( pCatalog->m_CatalogIndex == m_NowOnCatalogIndex ) // 마우스가 올려져있던 상품이면...
					{
						m_pSelectCatalog = pCatalog; // 참조 포인터 변경.
						m_NowOnCatalogIndex = -1; // 계속들어오지 않게 이녀석은 초기화.
						m_NowInfoPage = 0; // 아이템 정보 페이지도 초기화.
						GetInfo1Text( m_NowInfoPage ); // MultiText 갱신

						break;
					}
				}
			}

			return 1;
		}

		return 0; // 다른 인터페이스 부분을 찍었을 경우...(인벤,Bottom등...)

	case WM_MOUSEMOVE:
	case WM_LBUTTONUP:

		if( IsInside(x,y) ) // 인터페이스의 안쪽을 클릭했을 경우.
		{
			if( m_GroupTab.MsgProc(hWnd, msg, wParam, lParam) ) // 1이면 눌린 게 바뀐거다.
			{
				GroupChange( m_GroupTab.NowSelectedIndex ); // 메인 탭이 바뀌었을 때의 처리.(탭 인덱스를 넘겨줘야함에 유의)

				return 1;
			}

			if( m_InfoTab.MsgProc(hWnd, msg, wParam, lParam) ) // info 부분 탭
			{
				if( m_InfoTab.NowSelectedIndex == 0 ) // 상세정보.
				{
					m_InfoScroll.SetDrawPos( m_pBackSur->Xpos+773, m_pBackSur->Ypos+176, 164 ); // 보여지는 타입에 따라 스크롤 위치와 길이가 바뀌어야 한다.

					if( m_pSelectCatalog ) // 선택되어진 상품이 있을때.
					{
						// 보여질 상품이 바뀌었으면 스크롤 레인지도 변경
						GetInfo1Text( 0 );
					}
					else // 선택되어진게 없을때...
						m_InfoScroll.SetRange( 1 );
				}
				else if( m_InfoTab.NowSelectedIndex == 1 ) // 구매 내역.
				{
					m_InfoScroll.SetDrawPos( m_pBackSur->Xpos+773, m_pBackSur->Ypos+70, 300 );

					if( m_BuyCatalogList.size() >= VIEW_INFO2_LINE+1 )
						m_InfoScroll.SetRange( m_BuyCatalogList.size() - VIEW_INFO2_LINE + 1 ); // 보여질 라인에 맡춰서 범위 지정.
					else
						m_InfoScroll.SetRange( 1 );
				}
			}
			m_InfoPrevBtn.MsgProc(hWnd, msg, wParam, lParam);
			if( m_InfoPrevBtn.GetState()==BTN_ACTION ) // 이전 아이템 보기
			{
				if( m_NowInfoPage > 0 )
				{
					--m_NowInfoPage;
					GetInfo1Text( m_NowInfoPage ); // MultiText 갱신
				}

				m_InfoPrevBtn.SetState(BTN_NORMAL);	// 버튼의 스테이터스를 직접 풀어줘야하나보다.
				return 1;
			}

			m_InfoNextBtn.MsgProc(hWnd, msg, wParam, lParam);
			if( m_InfoNextBtn.GetState()==BTN_ACTION ) // 다음 아이템 보기 클릭시
			{
				if( m_NowInfoPage < m_pSelectCatalog->m_ItemCount-1 )
				{
					++m_NowInfoPage;
					GetInfo1Text( m_NowInfoPage ); // MultiText 갱신
				}


				m_InfoNextBtn.SetState(BTN_NORMAL);	// 버튼의 스테이터스를 직접 풀어줘야하나보다.
				return 1;
			}

			// BUY 버튼 체크
			for( i = 0 ; i < MAX_VIEWCATALOG_NUM ; ++i )
			{
				m_BuyBtn[i].MsgProc(hWnd, msg, wParam, lParam);
				if( m_BuyBtn[i].GetState()==BTN_ACTION )
				{
					m_pWantBuyCatalog = m_ViewCatalogList[m_NowViewCatalogPage*MAX_VIEWCATALOG_NUM+i]; // 살려고 한 카탈로그를 기억해둔다.

					//-- IDS_CASHSHOP_BUYCHECK : %s 상품을 구매하시겠습니까?
					sprintf(strTemp, G_STRING(IDS_CASHSHOP_BUYCHECK), m_pWantBuyCatalog->m_strCatalogName );
					g_pNk2DFrame->InsertPopup( strTemp,TYPE_NOR_OKCANCLE, 29 ); // 팝업 출력.

					m_BuyBtn[i].SetState(BTN_NORMAL);	// 버튼의 스테이터스를 직접 풀어줘야하나보다.
					return 1;
				}
			}

			for( i = 0 ; i < PACK_NUM ; ++i )
			{
				m_InvenTabBtn[i].MsgProc(hWnd, msg, wParam, lParam);
				if( m_InvenTabBtn[i].GetState()==BTN_ACTION ) // 상점닫기 버튼 클릭시
				{
					g_pRoh->ChangePackNum(i);
					m_InvenTabBtn[i].SetState(BTN_NORMAL);	// 버튼의 스테이터스를 직접 풀어줘야하나보다.
					return 1;
				}
			}

			m_PrevBtn.MsgProc(hWnd, msg, wParam, lParam);
			if( m_PrevBtn.GetState()==BTN_ACTION ) // 상점닫기 버튼 클릭시
			{
				if( m_NowViewCatalogPage > 0 )
					--m_NowViewCatalogPage;

				m_PrevBtn.SetState(BTN_NORMAL);	// 버튼의 스테이터스를 직접 풀어줘야하나보다.
				return 1;
			}

			m_NextBtn.MsgProc(hWnd, msg, wParam, lParam);
			if( m_NextBtn.GetState()==BTN_ACTION ) // 상점닫기 버튼 클릭시
			{
				if( m_NowViewCatalogPage < (m_ViewCatalogList.size()?(m_ViewCatalogList.size()-1)/MAX_VIEWCATALOG_NUM:0) )
					++m_NowViewCatalogPage;

				m_NextBtn.SetState(BTN_NORMAL);	// 버튼의 스테이터스를 직접 풀어줘야하나보다.
				return 1;
			}

			m_CloseBtn.MsgProc(hWnd, msg, wParam, lParam);
			if( m_CloseBtn.GetState()==BTN_ACTION ) // 상점닫기 버튼 클릭시
			{
				g_pNk2DFrame->ToggleCashShopWindow( -1 ); // Toggle 함수를 사용해서 강제 종료시킨다.(여기로 들어온다는건 켜있는 상태니까 무조건 꺼져야 정상)

				m_CloseBtn.SetState(BTN_NORMAL);	// 버튼의 스테이터스를 직접 풀어줘야하나보다.

				return 1;
			}
		}

		break;
	}

	return 0;
}

HRESULT CControlCashShop::BuyPopupMsg( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, int nMsg ) // 구입 팝업용 프로시져.
{
	char commBuf[512];

	if( nMsg == 1 ) // 확인이 눌린경우.
	{
		sprintf( commBuf, "multi_shop buy %d %d\n", g_pRoh->m_CurPackNum, m_pWantBuyCatalog->m_CatalogIndex ); // 팩넘, 상품 인덱스.
		if( g_pTcpIp )
			g_pTcpIp->SendNetMessage( commBuf, TRUE); // 구입 요청 패킷을 보낸다.
	}
	else if( nMsg == -1 ) // 취소가 눌린 경우
	{
		m_pWantBuyCatalog = NULL; // 구매 상품 기억해놓은걸 풀어준다.(확인시엔 패킷받고 복사하고 풀어준다.
	}

	return 0L;
}

void CControlCashShop::GroupChange( int TabIndex ) // 메인 탭이 바뀌었을 때의 처리. (인자가 그룹인덱스가 아니라 탭인덱스임에 유의)
{
	// 그룹 인덱스 구하기.
	int GroupIndex = 0;
	char strTemp[255];
	strcpy( strTemp, m_DtaData[m_iShopKind].m_GruopList[TabIndex]->m_strFileName );
	if( strcmp(m_DtaData[m_iShopKind].m_GruopList[TabIndex]->m_strFileName,"CashShop/Tab/Tab_All") == 0 ) // 이름이 "All" 이면 전체 보기...
		GroupIndex = -1;
	else if( strcmp(m_DtaData[m_iShopKind].m_GruopList[TabIndex]->m_strFileName,"CashShop/Tab/Tab_New") == 0 ) // 이름이 "New" 이면 신상품 보기...
		GroupIndex = -2;
	else
		sscanf( m_GroupTab.GetExtraString(TabIndex), "%d", &GroupIndex ); // ExtraString에 기록해둔 그룹 인덱스를 얻어온다.

	m_ViewCatalogList.clear(); // 참조 포인터 리스트일 뿐이므로 그냥  clear만

	// 리스트를 옮겨 담는다.
	sSellCatalogInfo *pCatalog;
	vector<sSellCatalogInfo *>::iterator pr;

	// 등록된 상품들을 전부 돌면서.
	for( pr = m_DtaData[m_iShopKind].m_SellCatalogList.begin() ; pr != m_DtaData[m_iShopKind].m_SellCatalogList.end() ; pr++ )
	{
		pCatalog = *pr;

		if( GroupIndex == -1 ) // 그룹 인덱스 -1 전체 보기다. (만약 전체보기 넣기 싫으면 -1이 없게 하라고 할 것)
		{
			m_ViewCatalogList.push_back( pCatalog ); // 검사없이 그냥 넣는다.
		}
		else if( GroupIndex == -2 ) // 그룹 인덱스 -2 신상품 보기다.
		{
			if( pCatalog->m_bNewCatalog ) // 신상품이면...
				m_ViewCatalogList.push_back( pCatalog ); // 넣는다.
		}
		else
		{
			if( pCatalog->m_GroupIndex == GroupIndex ) // 이 그룹의 상품이면...
				m_ViewCatalogList.push_back( pCatalog ); // 넣는다.
		}
	}

	std::sort(m_ViewCatalogList.begin(), m_ViewCatalogList.end(), ViewCatalogCompare);

	m_NowViewCatalogPage = 0;
	m_pSelectCatalog = NULL; // 선택된 상품 없게..
}

BOOL CControlCashShop::IsInside( int x, int y )
{
	if( !m_pBackSur )
		return FALSE;

	if (x >= m_pBackSur->Xpos
			&& x < m_pBackSur->Xpos + m_pBackSur->GetWidth()
			&& y >= m_pBackSur->Ypos
			&& y < m_pBackSur->Ypos + m_pBackSur->GetHeight() )
		return TRUE;

	return FALSE;
}

BOOL CControlCashShop::IsInsideInven( int x, int y )
{
	if( !m_pBackSur )
		return FALSE;

	if (x >= m_InvenRect.left
			&& x < m_InvenRect.right
			&& y >= m_InvenRect.top
			&& y < m_InvenRect.bottom )
		return TRUE;

	return FALSE;
}

int CControlCashShop::FindIconImage( int DbIndex ) // 아이콘 이미지를 DB인덱스로 부터 찾는다.
{
	sImageInfo *pImage;
	vector<sImageInfo *>::iterator pr;

	int VectorIndex = 0;

	// 보여질 상품들을 전부 돌면서.
	for( pr = m_DtaData[m_iShopKind].m_CatalogImageList.begin() ; pr != m_DtaData[m_iShopKind].m_CatalogImageList.end() ; pr++ )
	{
		pImage = *pr;

		if( pImage->m_Index == DbIndex ) // 같은 인덱스를 찾아서..
			return VectorIndex;

		++VectorIndex;
	}

	return 0;
}

void CControlCashShop::MakeCatalogTextSurface() // 카탈로그안에 들어가는 텍스트를 서피스로 만든다.
{
	int i = 0;
	char strTemp[256] = ""; // 가장 길어질수있는 상품 설명글도 255까지만 받기로 약속했다.
	HFONT TempFont;

	sSellCatalogInfo *pCatalog;
	vector<sSellCatalogInfo *>::iterator pr;

	// 등록된 상품들을 전부 돌면서.
	for( pr = m_DtaData[m_iShopKind].m_SellCatalogList.begin() ; pr != m_DtaData[m_iShopKind].m_SellCatalogList.end() ; pr++ )
	{
		pCatalog = *pr;

		SAFE_DELETE( pCatalog->m_pTextSur ); // 혹시 모르니...
		// 배경 크기만큼 서피스 생성.
		g_pDisplay->CreateSurface( &pCatalog->m_pTextSur, m_pItemSlotSur->GetWidth(), m_pItemSlotSur->GetHeight() );
		if( pCatalog->m_pTextSur )
		{
			pCatalog->m_pTextSur->Clear( 0 );
			pCatalog->m_pTextSur->SetColorKey( 0 );

			// 상품명.
			if( g_dwClientCountry == CTRY_BRA ) // 브라질은 상품명 글자 크기 작게...
				TempFont = G_GetFont(14,0,g_strFontName);
			else
				TempFont = G_GetFont(20,0,g_strFontName);
			pCatalog->m_pTextSur->DrawText( TempFont, pCatalog->m_strCatalogName, 100, 15, 0, RGB(255,100,200), 0 );

			// 상품 설명.
			TempFont = G_GetFont(12,0,g_strFontName);
			CMultiText TempMultiText;
			TempMultiText.AddStringMulti( pCatalog->m_strCatalogInfo, 25, TRUE ); // 스트링을 여러줄 단위로 끊어준다.
			for( i = 0 ; i < TempMultiText.m_LineCount ; ++i ) // 라인 수만큼 루프.
				pCatalog->m_pTextSur->DrawText( TempFont, TempMultiText.m_Text[i], 95, 38+(i*13), 0, m_TextOut.m_FontColor, 0 );

			TempFont = G_GetFont(14,0,g_strFontName);
			// 상품 가격.
			//-- IDS_CASHSHOP_PRICE : Price  : %d
			sprintf( strTemp, (char*)G_STRING(IDS_CASHSHOP_PRICE), pCatalog->m_Price );
			pCatalog->m_pTextSur->DrawText( TempFont, strTemp, 97, 117, 0, RGB(100,100,255), 0 );

			// 마일리지.
			//-- IDS_CASHSHOP_MILEASE : Milease: %d
			sprintf( strTemp, (char*)G_STRING(IDS_CASHSHOP_MILEASE), pCatalog->m_Milease );
			pCatalog->m_pTextSur->DrawText( TempFont, strTemp, 97, 130, 0, RGB(100,100,255), 0 );

		}
	}
}

void CControlCashShop::MakeBuyListText( sSellCatalogInfo *pCatalog )  // 구매내역의 텍스트부를 미리 생성.
{
	char strTemp[256] = ""; // 가장 길어질수있는 상품 설명글도 255까지만 받기로 약속했다.
	HFONT TempFont = G_GetFont(14,0,g_strFontName);

	SAFE_DELETE( pCatalog->m_pTextSur ); // 혹시 모르니...
	// 배경 크기만큼 서피스 생성.
	g_pDisplay->CreateSurface( &pCatalog->m_pTextSur, 230, 30 ); // 한 구매 내역 출력 위치로 잡고...
	if( pCatalog->m_pTextSur )
	{
		pCatalog->m_pTextSur->Clear( 0 );
		pCatalog->m_pTextSur->SetColorKey( 0 );

		// 상품명.
		sprintf( strTemp, "[%s]", pCatalog->m_strCatalogName );
		pCatalog->m_pTextSur->DrawText( TempFont, strTemp, 5, 1, 0, RGB(230,230,230), 0 );

		// 가격
		//-- IDS_CASHSHOP_PRICE : Price: %d
		char strPrice[256] = {0,};
		sprintf( strTemp, (char*)G_STRING(IDS_CASHSHOP_PRICE), pCatalog->m_Price );
		sprintf( strPrice, "[%s]", strTemp ); // 가로로 묶기.
		pCatalog->m_pTextSur->DrawText( TempFont, strPrice, 5, 15, 0, RGB(255,255,0), 0 );

		// 구매 일시
		// [5/6/2008 반재승] 태국은 유료상점 아이템 구입시 시간표시에 +7시간 해줘야 한다.
		if( g_dwClientCountry == CTRY_TH )
		{
			sprintf( strTemp, "[%02d/ %02d:%02d:%02d]", pCatalog->m_BuyTime.wDay, pCatalog->m_BuyTime.wHour + 7, pCatalog->m_BuyTime.wMinute, pCatalog->m_BuyTime.wSecond );
		}

		else
		{
			sprintf( strTemp, "[%02d/ %02d:%02d:%02d]", pCatalog->m_BuyTime.wDay, pCatalog->m_BuyTime.wHour, pCatalog->m_BuyTime.wMinute, pCatalog->m_BuyTime.wSecond );
		}

		pCatalog->m_pTextSur->DrawText( TempFont, strTemp, 150, 15, 0, RGB(255,255,0), 0 );
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////
// 캐쉬 샵의 메인 페이지가 열리기전의 웹 공지 페이지
CCashShopIntro::CCashShopIntro()
	: m_hWnd(NULL)
{
	m_pBackSur = NULL;

	Init();
}

CCashShopIntro::~CCashShopIntro()
{
	DeleteRes();
}

void CCashShopIntro::Init()
{

}

void CCashShopIntro::DeleteRes()
{
	m_InBtn.DeleteRes();
	m_CloseBtn.DeleteRes();
	SAFE_DELETE( m_pBackSur );
}

void CCashShopIntro::LoadRes()
{
	if( !g_pDisplay )
		return;

	m_hWnd =  g_pDisplay->GetHWnd();
	GetClientRect( m_hWnd, &m_ClientRc );

	g_pDisplay->CreateSurfaceFromBitmap( &m_pBackSur, "interface/CashShop/intro/IntroCashShopBack.bmp" );

	if( m_pBackSur )
	{
// 		m_pBackSur->SetWidth( 800 ); // 위 CreateSurface에서 CSurface 에 따로 저장해두는 m_dwWidth,m_dwHeight에 값을 저장안해주는 버그가 있어서 그냥 직접 셋팅.
// 		m_pBackSur->SetHeight( 600 );

		{
			m_pBackSur->Xpos = ( m_ClientRc.right - m_pBackSur->GetWidth() ) / 2;
			m_pBackSur->Ypos = ( m_ClientRc.bottom - m_pBackSur->GetHeight() ) / 2;
		}

		m_InBtn.SetFileName( "CashShop/Intro/btn_in" );
		m_InBtn.LoadRes();
		m_InBtn.SetPosition(m_pBackSur->Xpos+133, m_pBackSur->Ypos+524 );

		m_CloseBtn.SetFileName( "CashShop/btn_close" );
		m_CloseBtn.LoadRes();
		m_CloseBtn.SetPosition(m_pBackSur->Xpos+766, m_pBackSur->Ypos+9 );
	}

}

void CCashShopIntro::RestoreSurfaces()
{
	// 리스토어 안해준다. 필요도 없는데 전체화면 모드 웹페이지 open 시 화면전환되는 문제가 있다.
	DeleteRes();
	LoadRes();
}

void CCashShopIntro::Open()
{
	m_WebPage1.LoadRes(); // 위도우를 만들고.
	m_WebPage2.LoadRes(); // 위도우를 만들고.

	char strUrl1[256] = "";
	char strUrl2[256] = "";

	switch( g_dwClientCountry )
	{
	case CTRY_TH:
//		strcpy( strUrl1, "http://61.90.198.67/update2/info.htm" );
//		strcpy( strUrl2, "http://61.90.198.67/update2/update.htm" );
		GetCurrentDirectory( 256, strUrl1 );
		strcat( strUrl1, "/interface/CashShop/Intro/html/info.htm" ); // 태국 웹이 느려서 하드안에서 읽도록 변경.(바꿀땐 패치하게...)
//		strcpy( strUrl1, "./interface/CashShop/Intro/html/info.htm" );
		GetCurrentDirectory( 256, strUrl2 );
		strcat( strUrl2, "/interface/CashShop/Intro/html/update.htm" );
//		strcpy( strUrl2, "http://61.90.198.67/update2/update.htm" );
		break;

	default:
// 		strcpy( strUrl1, "www.naver.co.kr" );
// 		strcpy( strUrl2, "www.yahoo.co.kr" );
		GetCurrentDirectory( 256, strUrl1 );
		strcat( strUrl1, "/interface/CashShop/Intro/html/info.htm" ); // 태국 웹이 느려서 하드안에서 읽도록 변경.(바꿀땐 패치하게...)
		GetCurrentDirectory( 256, strUrl2 );
		strcat( strUrl2, "/interface/CashShop/Intro/html/update.htm" );
		break;
	}

	// 풀스크린과 윈도우 모드에서 위치가 자꾸 틀려지더라. 그래서 그냥 위치를 따로 처리...
	if( g_bFFullMode )
	{
		m_WebPage1.Open( strUrl1, m_pBackSur->Xpos+19, m_pBackSur->Ypos+63, 357, 424 ); // 창을 연다.
		m_WebPage2.Open( strUrl2, m_pBackSur->Xpos+398, m_pBackSur->Ypos+63, 382, 504 ); // 창을 연다.
	}
	else
	{
		m_WebPage1.Open( strUrl1, m_pBackSur->Xpos+21, m_pBackSur->Ypos+112, 359, 426 ); // 창을 연다.
		m_WebPage2.Open( strUrl2, m_pBackSur->Xpos+400, m_pBackSur->Ypos+112, 385, 504 ); // 창을 연다.
	}
}

void CCashShopIntro::Close()
{
	m_WebPage1.Close();
	m_WebPage2.Close();
	m_WebPage2.DeleteRes();
	m_WebPage1.DeleteRes();
}

void CCashShopIntro::Draw() // 이 함수의 리턴은 DrawItemInfo()함수에서 표시되는 아이템 정보를 갱신하기 위해 쓰인다.
{
	if( !g_pDisplay )
		return;

	g_pDisplay->Blt( m_pBackSur->Xpos, m_pBackSur->Ypos, m_pBackSur );

	m_WebPage1.Draw(); // 이녀석은 실제론 윈도우 모드시 창이동 조정밖에 없다.
	m_WebPage2.Draw(); // 이녀석은 실제론 윈도우 모드시 창이동 조정밖에 없다.

	m_InBtn.Draw();
	m_CloseBtn.Draw();
}

LRESULT CCashShopIntro::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	int x = LOWORD( lParam );
	int y = HIWORD( lParam );

	switch (msg)
	{
	case WM_LBUTTONDOWN:

		if( IsInside(x,y) ) // 인터페이스의 안쪽을 클릭했을 경우.
		{
			m_InBtn.MsgProc(hWnd, msg, wParam, lParam);
			m_CloseBtn.MsgProc(hWnd, msg, wParam, lParam);

			return 1;
		}

		return 0; // 다른 인터페이스 부분을 찍었을 경우...(인벤,Bottom등...)

	case WM_MOUSEMOVE:
	case WM_LBUTTONUP:

		if( IsInside(x,y) ) // 인터페이스의 안쪽을 클릭했을 경우.
		{
			m_InBtn.MsgProc(hWnd, msg, wParam, lParam);
			if( m_InBtn.GetState()==BTN_ACTION ) // 들어가기
			{
				g_pNk2DFrame->ToggleCashShopWindow( CASHSHOP_TYPE_NORMAL ); // 일반 캐쉬상점 인터페이스를 열어준다.

				m_InBtn.SetState(BTN_NORMAL);	// 버튼의 스테이터스를 직접 풀어줘야하나보다.
				return 1;
			}

			m_CloseBtn.MsgProc(hWnd, msg, wParam, lParam);
			if( m_CloseBtn.GetState()==BTN_ACTION ) // 상점닫기 버튼 클릭시
			{
				g_pNk2DFrame->ToggleCashShopIntroWindow( -1 ); // Toggle 함수를 사용해서 강제 종료시킨다.(여기로 들어온다는건 켜있는 상태니까 무조건 꺼져야 정상)

				m_CloseBtn.SetState(BTN_NORMAL);	// 버튼의 스테이터스를 직접 풀어줘야하나보다.

				return 1;
			}
		}

		break;
	}
	return 0;
}

BOOL CCashShopIntro::IsInside( int x, int y )
{
	if( !m_pBackSur )
		return FALSE;

	return m_pBackSur->IsIn(x,y);
}

