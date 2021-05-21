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
	m_pItemSlotSur = NULL; // ������ �ǸŸ���� �� ������ �� ��� �׷���.
	m_pSelectSlotSur = NULL;
	m_pCatalogImageList = NULL;

	m_pInfo1BackSur = NULL;
	m_pInfo2BackSur = NULL;

	m_pInvenOverItem = NULL;

	m_pItemInfoTextSur = NULL;

	Init(); // ������ �ʱ�ȭ.
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

	ClearDta(); // Dta ���� ���� ����� �����ֱ� ���� �༮.

	for( int i = 0 ; i < MAX_CASHSHOP_KIND ; ++i ) // �̰� ���� �ѹ� �� ������Ѵ�.
		m_DtaData[i].m_bImageLoad = FALSE; // �̹��� �ε� �÷��׸� ������ �θ� ���� ���¶� �ٽ� �о�� ���̴�.
}



void CControlCashShop::DeleteRes()
{
	int i = 0;

	ClearDta();

	// ���� ���� �ʱ�ȭ.
	vector<sSellCatalogInfo *>::iterator pr;
	for( pr = m_BuyCatalogList.begin() ; pr != m_BuyCatalogList.end() ; pr++ )
	{
		SAFE_DELETE( (*pr)->m_pTextSur ); // ���ǽ��� �ִٸ� ���� �����ֱ�.
		delete (*pr);
	}
	m_BuyCatalogList.clear();

	m_ViewCatalogList.clear(); // ���� ������ ����Ʈ�� ���̹Ƿ� �׳�  clear��

	m_DummyBuyBtn.DeleteRes(); // ���� ��ư�� �޸������� ���� �޸𸮸� ������ �ִ� �� �༮�� �ϸ� �ɵ�.
	for( i = 0 ; i < MAX_VIEWCATALOG_NUM ; ++i )
	{
		ZeroMemory( &m_BuyBtn[i], sizeof(m_BuyBtn[i]) ); // ���������͸� �ʱ�ȭ�ؾ� �Ҹ��ڿ��� �޸� ������ ���Ѵ�.
	}
	m_HaveCash.DeleteRes();
	m_InfoScroll.DeleteRes();
	m_InfoTab.DeleteRes(); // �������� ���������� �ٲ�Ƿ� ���ҽ��� Ǯ���ش�.
	m_InfoPrevBtn.DeleteRes(); // ���� ������ ���� ��ư.
	m_InfoNextBtn.DeleteRes(); // ���� ������ ���� ��ư.

	SAFE_DELETE( m_pSelectSlotSur );
	SAFE_DELETE( m_pItemSlotSur );
	m_GroupTab.DeleteRes();
	SAFE_DELETE( m_pInfo2BackSur );
	SAFE_DELETE( m_pInfo1BackSur );


	for( i = 0 ; i < PACK_NUM ; ++i )
		m_InvenTabBtn[i].DeleteRes(); // �κ� �� ��ư

	m_PrevBtn.DeleteRes(); // ���� �Ǹ� ��� ���� ��ư.
	m_NextBtn.DeleteRes(); // ���� �Ǹ� ��� ���� ��ư.

	m_CloseBtn.DeleteRes();
	SAFE_DELETE_ARRAY( m_pCatalogImageList );
	SAFE_DELETE( m_pBackSur );

	m_TextOut.DeleteRes();     // �ؽ�Ʈ ��¿� �Ƕ���

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
// 		m_pBackSur->SetWidth( 800 ); // �� CreateSurface���� CSurface �� ���� �����صδ� m_dwWidth,m_dwHeight�� ���� ��������ִ� ���װ� �־ �׳� ���� ����.
// 		m_pBackSur->SetHeight( 600 );

		{
			m_pBackSur->Xpos = ( m_ClientRc.right - m_pBackSur->GetWidth() ) / 2;
			m_pBackSur->Ypos = ( m_ClientRc.bottom - m_pBackSur->GetHeight() ) / 2;
		}

		m_GroupTab.SetPosition( m_pBackSur->Xpos+40, m_pBackSur->Ypos+47, 460, 35 );
		m_GroupTab.SetIncDecBtn( "CashShop/Tab/TabDec", m_pBackSur->Xpos+19, m_pBackSur->Ypos+57, "CashShop/Tab/TabInc", m_pBackSur->Xpos+510, m_pBackSur->Ypos+57 );
		m_GroupTab.SetTabStyle( TAB_ALIGN_HORIZON, 0, 0, 0 ); // �� ��ġ ( ����/����, x�׸������, y�׸������, ���� )

		m_InfoTab.SetPosition( m_pBackSur->Xpos+531, m_pBackSur->Ypos+45, 176, 27 );
		m_InfoTab.SetTabStyle( TAB_ALIGN_HORIZON, 10, 3, -1 ); // �� ��ġ ( ����/����, x�׸������, y�׸������, ���� )
		m_InfoTab.AddTab( "CashShop/Tab/Tab_Info" );
		m_InfoTab.AddTab( "CashShop/Tab/Tab_BuyInfo" );

		m_InfoPrevBtn.SetFileName( "CashShop/btn_InfoPrev" );
		m_InfoPrevBtn.LoadRes();
		m_InfoPrevBtn.SetPosition(m_pBackSur->Xpos+612, m_pBackSur->Ypos+349 );

		m_InfoNextBtn.SetFileName( "CashShop/btn_InfoNext" );
		m_InfoNextBtn.LoadRes();
		m_InfoNextBtn.SetPosition(m_pBackSur->Xpos+705, m_pBackSur->Ypos+349 );

		m_InfoScroll.LoadRes( "CashShop/InfoScrollUp", "CashShop/InfoScrollDown", "interface/CashShop/InfoScrollBar.bmp" );
		m_InfoScroll.SetDrawPos( m_pBackSur->Xpos+773, m_pBackSur->Ypos+176, 164 ); // �������� Ÿ�Կ� ���� ��ũ�� ��ġ�� ���̰� �ٲ��� �Ѵ�.

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
			memcpy( &m_BuyBtn[i], &m_DummyBuyBtn, sizeof(m_BuyBtn[i]) ); // �̷��� �ϸ� �̹��� ���ǽ��� �����ؼ� �����ִ�.
		}

		m_CloseBtn.SetFileName( "CashShop/btn_close" );
		m_CloseBtn.LoadRes();
		m_CloseBtn.SetPosition(m_pBackSur->Xpos+766, m_pBackSur->Ypos+9 );

		// ���� ����
		// �κ� ���� ����
		m_InvenRect.left = m_pBackSur->Xpos + 545;
		m_InvenRect.top  = m_pBackSur->Ypos + 379;
		m_InvenRect.right = m_pBackSur->Xpos + 760;
		m_InvenRect.bottom  = m_pBackSur->Ypos + 539;
	}

	g_pDisplay->CreateSurfaceFromBitmap( &m_pItemSlotSur, "interface/CashShop/CatalogBack.bmp" );
	m_pItemSlotSur->SetWidth( 259 ); // �� CreateSurface���� CSurface �� ���� �����صδ� m_dwWidth,m_dwHeight�� ���� ��������ִ� ���װ� �־ �׳� ���� ����.
	m_pItemSlotSur->SetHeight( 156 );
	m_pItemSlotSur->SetColorKey( RGB(255,0,255) );

	g_pDisplay->CreateSurfaceFromBitmap( &m_pSelectSlotSur, "interface/CashShop/SelCatalog.bmp" );
	m_pSelectSlotSur->SetWidth( 259 ); // �� CreateSurface���� CSurface �� ���� �����صδ� m_dwWidth,m_dwHeight�� ���� ��������ִ� ���װ� �־ �׳� ���� ����.
	m_pSelectSlotSur->SetHeight( 256 );
	m_pSelectSlotSur->SetColorKey( RGB(255,0,255) );

	m_TextOut.Init( 12, RGB(100,100,100), 0, 0 );

	m_bResLoad = TRUE; // �ӽ� �ּ� ó��
}

void CControlCashShop::RestoreSurfaces()
{
	// �����ϰ� �� �����ٰ� �ε�.
	DeleteRes();
	LoadRes();

	LoadFromDta( m_iShopKind ); // ���� ���� dta ���ҽ��� �������. ������ �÷��׸� Ǯ���ش�.

	for( int i = 0 ; i < MAX_CASHSHOP_KIND ; ++i )
		m_DtaData[i].m_bImageLoad = FALSE; // �̹��� �ε� �÷��׸� ������ �θ� ���� ���¶� �ٽ� �о�� ���̴�.
}

void CControlCashShop::OpenInit() // ���������� �ʱ�ȭ������ϴ� �༮��.
{
	m_GroupTab.NowDrawStartIndex = 0; // �̰͵� ���⼭ �ʱ�ȭ������.
	m_GroupTab.NowSelectedIndex = 0; // �̰͵� ���⼭ �ʱ�ȭ������.
	GroupChange( 0 ); // ó�� ���õ� ���� ����.(�� �ε����� ���� ������ ����.)

	m_InfoTab.NowDrawStartIndex = 0; // �̰͵� ���⼭ �ʱ�ȭ������.
	m_InfoTab.NowSelectedIndex = 0; // �̰͵� ���⼭ �ʱ�ȭ������.

	m_InfoScroll.SetPos( 0 ); // ��ũ�ѵ� �ʱ���ġ��.
	m_InfoScroll.SetDrawPos( m_pBackSur->Xpos+773, m_pBackSur->Ypos+176, 164 ); // �������� Ÿ�Կ� ���� ��ũ�� ��ġ�� ���̰� �ٲ��� �Ѵ�.

	m_pSelectCatalog = NULL;
	m_pWantBuyCatalog = NULL; // Ȥ�� ������ �ٽ� ���� Ǯ�����ְ� ���⼭ ���Դ�� ���� �����͸� �ʱ�ȭ.

	m_NowInfoPage = 0;

}

void CControlCashShop::ReadDta( int ShopType, const char *FileName ) // Dta ������ �ε�.
{
	PFILE *fp;
	int i = 0, j = 0;

	if(  !( fp = pfopen(FileName,"rb") )  )
	{
		MessageBox(NULL, "CashShop Dta Loading fail", "Error", MB_OK);
		return;
	}

	// ���̸� �о ���ش�!
	for( int dummy_count = 0 ; dummy_count < CASHSHOP_DTA_ENC_DUMMY_COUNT ; dummy_count++ )
	{
		int dummy;
		pfread( &dummy, sizeof(int), 1, fp );
	}

	int int_seed = CASHSHOP_DTA_ENC_INT_SEED;
	char char_seed = CASHSHOP_DTA_ENC_CHAR_SEED;

	int TempInt = 0;
	char strTemp[256] = ""; // ���� ��������ִ� ��ǰ ����۵� 255������ �ޱ�� ����ߴ�.
	char strPath[256] = ""; // ��� �־��ֱ��.
	int Count;

	/////////////////////////////////////////////////////////////////
	// �� ���� �κ�.
	sImageInfo *TempImageInfo = NULL;
	g_ItemList.ReadInt( &Count, fp, int_seed ); // ��ȣȭ Ǯ� �� ������.
	m_DtaData[ShopType].m_GruopList.reserve( Count );

	for( i = 0 ; i < Count ; ++i )
	{
		TempImageInfo = new sImageInfo;

		if( !TempImageInfo )
			continue;

		g_ItemList.ReadInt( &TempInt, fp, int_seed ); // ��ȣȭ Ǯ� �� ������.
		TempImageInfo->m_Index = TempInt; // �׷� �ε���.

//		g_ItemList.ReadString( strTemp, fp, int_seed, char_seed ); // �� �ȿ��� ���������� ��Ʈ�� ���� 4����Ʈ�� ������ ����.
//		strcpy( TempImageInfo->m_strName, strPath ); // �� �̸�

		g_ItemList.ReadString( strTemp, fp, int_seed, char_seed ); // �� �ȿ��� ���������� ��Ʈ�� ���� 4����Ʈ�� ������ ����.
		sprintf( strPath, "CashShop/Tab/" );
		strcat( strPath, strTemp );
		strcpy( TempImageInfo->m_strFileName, strPath ); // �̹��� ���ϸ�...�ε�� ���߿�

		m_DtaData[ShopType].m_GruopList.push_back( TempImageInfo );
	}
	///////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
	// ��ǰ �̹��� ����Ʈ.
	g_ItemList.ReadInt( &Count, fp, int_seed ); // ����Ʈ ����.
	m_DtaData[ShopType].m_CatalogImageList.reserve( Count );

	for( i = 0 ; i < Count ; ++i )
	{
		TempImageInfo = new sImageInfo;

		if( !TempImageInfo )
			continue;

		g_ItemList.ReadInt( &TempInt, fp, int_seed ); // ��ȣȭ Ǯ� �� ������.
		TempImageInfo->m_Index = TempInt; // �̹��� �ε���.

		g_ItemList.ReadString( strTemp, fp, int_seed, char_seed ); // �� �ȿ��� ���������� ��Ʈ�� ���� 4����Ʈ�� ������ ����.
		sprintf( strPath, "interface/CashShop/Icon/" );
		strcat( strPath, strTemp );
		strcpy( TempImageInfo->m_strFileName, strPath ); // �̹��� ���ϸ�...�ε�� ���߿�

		m_DtaData[ShopType].m_CatalogImageList.push_back( TempImageInfo );
	}
	///////////////////////////////////////////////////////////////////



	/////////////////////////////////////////////////////////////////
	// �Ǹ� ��� ������.
	sSellCatalogInfo *TempCatalogInfo = NULL;
	g_ItemList.ReadInt( &Count, fp, int_seed ); // ����Ʈ ����.
	m_DtaData[ShopType].m_SellCatalogList.reserve( Count );

	for( i = 0 ; i < Count ; ++i )
	{
		TempCatalogInfo = new sSellCatalogInfo;

		if( !TempCatalogInfo )
			continue;

		TempCatalogInfo->m_pTextSur = NULL; // �� �༮ ���� ����������.

		g_ItemList.ReadInt( &TempInt, fp, int_seed ); // ��ȣȭ Ǯ� �� ������.
		TempCatalogInfo->m_CatalogIndex = TempInt; // ��ǰ �ε���.

		g_ItemList.ReadInt( &TempInt, fp, int_seed ); // ��ȣȭ Ǯ� �� ������.
		TempCatalogInfo->m_IconImageIndex = TempInt; // ������ �̹��� �ε���.

		g_ItemList.ReadString( strTemp, fp, int_seed, char_seed ); // �� �ȿ��� ���������� ��Ʈ�� ���� 4����Ʈ�� ������ ����.
		if( strlen(strTemp) < MAX_LENGTH_CATALOG_NAME ) // ��ǰ�� ���� ���� üũ.
			strcpy( TempCatalogInfo->m_strCatalogName, strTemp ); // ��ǰ ��

		g_ItemList.ReadString( strTemp, fp, int_seed, char_seed ); // �� �ȿ��� ���������� ��Ʈ�� ���� 4����Ʈ�� ������ ����.
		strcpy( TempCatalogInfo->m_strCatalogInfo, strTemp ); // ��ǰ ����

		g_ItemList.ReadInt( &TempInt, fp, int_seed ); // ��ȣȭ Ǯ� �� ������.
		TempCatalogInfo->m_Price = TempInt; // ��ǰ ����.

		g_ItemList.ReadInt( &TempInt, fp, int_seed ); // ��ȣȭ Ǯ� �� ������.
		TempCatalogInfo->m_Milease = TempInt; // ��ǰ ���ϸ���.

		g_ItemList.ReadInt( &TempInt, fp, int_seed ); // ��ȣȭ Ǯ� �� ������.
		TempCatalogInfo->m_GroupIndex = TempInt; // �����ִ� �׷� ��ȣ.

		g_ItemList.ReadInt( &TempInt, fp, int_seed ); // ��ȣȭ Ǯ� �� ������.
		TempCatalogInfo->m_bNewCatalog = TempInt; // �Ż�ǰ ����. ��Ʈ�������� ����� 0,1�� ���´�.

		g_ItemList.ReadInt( &TempInt, fp, int_seed ); // ��ȣȭ Ǯ� �� ������.
		TempCatalogInfo->m_ItemCount = TempInt; // �����ִ� ������ ����.

		for( j = 0 ; j < TempCatalogInfo->m_ItemCount ; ++j )
		{
			CItem *TempItem = &(TempCatalogInfo->m_Item[j]); // ���� ������.

			g_ItemList.ReadInt( &TempInt, fp, int_seed ); // ��ȣȭ Ǯ� �� ������.
			TempItem->m_Vnum = TempInt; // vnum

			TempItem->m_ProtoNum = g_ItemList.FindItem( TempItem->m_Vnum ); // ProtoNum �Է�

			g_ItemList.ReadInt( &TempInt, fp, int_seed ); // ��ȣȭ Ǯ� �� ������.
			if( GET_TYPE(TempItem) == ITYPE_SUB ) // plus
				TempItem->m_PlusNum = TempInt;
			else if( TempInt > 0 )
			{
				if( GET_ITEM_LEVEL(TempItem) >= 181 )
					TempItem->SetDiaGemNum( TempInt );
				else
					TempItem->SetGemNum( TempInt );
			}


			g_ItemList.ReadInt( &TempInt, fp, int_seed ); // ��ȣȭ Ǯ� �� ������.
			TempItem->m_Special = TempInt; // �ɼ�1

			g_ItemList.ReadInt( &TempInt, fp, int_seed ); // ��ȣȭ Ǯ� �� ������.
			TempItem->m_Special2 = TempInt; // �ɼ�2 // �� ������ �ɼ�2�� ������ �б⸸�ϰ� ����.

			// ��Ÿ ���� ����.
			ResetItemValue( TempItem );
			SetSpecialValue( TempItem, GET_TYPE(TempItem), GET_SHAPE(TempItem), TempItem->m_Special, TempItem->m_Special2 );

			g_ItemList.ReadInt( &TempInt, fp, int_seed ); // ��ȣȭ Ǯ� �� ������.
//			TempItem->m_UpgradeEndurance = TempInt; // ������. // �� ������ �������� ������ �б⸸�ϰ� ����.
//			TempItem->m_MaxUpgradeEndurance = TempInt; // ������.

			g_ItemList.ReadInt( &TempInt, fp, int_seed ); // ��ȣȭ Ǯ� �� ������.
			if( TempInt ) // �ð�����.
				TempItem->SetTimeLimitItem( TempInt );

		}

		m_DtaData[ShopType].m_SellCatalogList.push_back( TempCatalogInfo );
	}
	///////////////////////////////////////////////////////////////////

	pfclose(fp);

	MakeCatalogTextSurface(); // �ؽ�Ʈ ���ǽ��� �̸� ������ش�.
}

// �ӽ��׽�Ʈ��.
void CControlCashShop::DummyDta( int ShopType ) // Dta ������ ����.
{
	int i = 0, j = 0;

	int TempInt = 0;
	char strTemp[256] = ""; // ���� ��������ִ� ��ǰ ����۵� 255������ �ޱ�� ����ߴ�.
	char strPath[256] = "";
	int Count;

	/////////////////////////////////////////////////////////////////
	// �� ���� �κ�.
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

		TempImageInfo->m_Index = i; // �׷� �ε���.

//		sprintf( strPath, TabName[i] );
//		strcpy( TempImageInfo->m_strName, strPath ); // �� �̸�

		sprintf( strPath, "CashShop/Tab/" );
		strcat( strPath, TabFileName[i] );
		strcpy( TempImageInfo->m_strFileName, strPath ); // �̹��� ���ϸ�...�ε�� ���߿�

		m_DtaData[ShopType].m_GruopList.push_back( TempImageInfo );
	}
	///////////////////////////////////////////////////////////////////

	char IconName[2][256] =
	{
		{ "Icon_Weapon" }, { "Icon_Armor" } // , { "Icon_Helmet" }, { "Icon_Shield" }, { "Icon_Etc" }
	};

	/////////////////////////////////////////////////////////////////
	// ��ǰ �̹��� ����Ʈ.
	Count = 2;
	m_DtaData[ShopType].m_CatalogImageList.reserve( Count );

	for( i = 0 ; i < Count ; ++i )
	{
		TempImageInfo = new sImageInfo;

		if( !TempImageInfo )
			continue;

		TempImageInfo->m_Index = i; // �̹��� �ε���.

		sprintf( strPath, "interface/CashShop/Icon/" );
		strcat( strPath, IconName[i] );
		strcat( strPath, ".bmp" ); // �������� �Է¹�� ������ ���� Ȯ���ڴ� Ŭ���̾�Ʈ���� �ٿ�����.
		strcpy( TempImageInfo->m_strFileName, strPath ); // �̹��� ���ϸ�...�ε�� ���߿�

		m_DtaData[ShopType].m_CatalogImageList.push_back( TempImageInfo );
	}
	///////////////////////////////////////////////////////////////////



	/////////////////////////////////////////////////////////////////
	// �Ǹ� ��� ������.
	sSellCatalogInfo *TempCatalogInfo = NULL;
	Count = 20;
	m_DtaData[ShopType].m_SellCatalogList.reserve( Count );

	for( i = 0 ; i < Count ; ++i )
	{
		TempCatalogInfo = new sSellCatalogInfo;

		if( !TempCatalogInfo )
			continue;

		TempCatalogInfo->m_pTextSur = NULL; // �� �༮ ���� ����������.

		TempCatalogInfo->m_CatalogIndex = i; // ��ǰ �ε���.

		TempCatalogInfo->m_IconImageIndex = rand()%m_DtaData[ShopType].m_CatalogImageList.size(); // ������ �̹��� �ε���.

		sprintf( strTemp, G_STRING(IDS_LHSTRING1735), i );
		if( strlen(strTemp) < MAX_LENGTH_CATALOG_NAME ) // ��ǰ�� ���� ���� üũ.
			strcpy( TempCatalogInfo->m_strCatalogName, strTemp ); // ��ǰ ��

		sprintf( strTemp, G_STRING(IDS_LHSTRING1736), i );
		strcpy( TempCatalogInfo->m_strCatalogInfo, strTemp ); // ��ǰ ����

		TempCatalogInfo->m_Price = 100 * i; // ��ǰ ����.

		TempCatalogInfo->m_Milease = 10*i; // ��ǰ ���ϸ���.

		TempCatalogInfo->m_GroupIndex = rand()%4; // �����ִ� �׷� ��ȣ.


		TempCatalogInfo->m_ItemCount = rand()%4 + 1; // �����ִ� ������ ����.

		for( j = 0 ; j < TempCatalogInfo->m_ItemCount ; ++j )
		{
			CItem *TempItem = &(TempCatalogInfo->m_Item[j]); // ���� ������.

			TempItem->m_Vnum = 860+i+j; // vnum

			TempItem->m_ProtoNum = g_ItemList.FindItem( TempItem->m_Vnum ); // ProtoNum �Է�

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


			TempItem->m_Special = 4095; // �ɼ�1

			TempItem->m_Special2 = 0; // �ɼ�2

			// ��Ÿ ���� ����.
			ResetItemValue( TempItem );
			SetSpecialValue( TempItem, GET_TYPE(TempItem), GET_SHAPE(TempItem), TempItem->m_Special, TempItem->m_Special2 );

//			TempItem->m_UpgradeEndurance = 0; // ������.
//			TempItem->m_MaxUpgradeEndurance = 0; // ������.

			TempInt = 0;
			if( TempInt ) // �ð�����.
				TempItem->SetTimeLimitItem( TempInt );

		}

		m_DtaData[ShopType].m_SellCatalogList.push_back( TempCatalogInfo );
	}
	///////////////////////////////////////////////////////////////////

	MakeCatalogTextSurface(); // �ؽ�Ʈ ���ǽ��� �̸� ������ش�.
}

void CControlCashShop::ClearDta() // Dta �������� �ʱ�ȭ.
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
			SAFE_DELETE( (*pr2)->m_pTextSur ); // ���ǽ��� �ִٸ� ���� �����ֱ�.
			delete (*pr2);
		}
		m_DtaData[i].m_SellCatalogList.clear();
	}

	m_pInvenOverItem = NULL; // ������ ������ ����â�� ��������...
}

void CControlCashShop::ClearDta( int i ) // Ư�� Dta ���� �ʱ�ȭ.
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
		SAFE_DELETE( (*pr2)->m_pTextSur ); // ���ǽ��� �ִٸ� ���� �����ֱ�.
		delete (*pr2);
	}
	m_DtaData[i].m_SellCatalogList.clear();
}

void CControlCashShop::LoadFromDta( int ShopType )
{
	if( !g_pDisplay )
		return;

	int i = 0;
	char strTemp[5]; // �� �ε����� ����ֱ� ���ѳ༮.

	// �� �ε�.
	for( i = 0 ; i < m_DtaData[ShopType].m_GruopList.size() ; ++i )
	{
		sprintf( strTemp, "%d", m_DtaData[ShopType].m_GruopList[i]->m_Index );
		m_GroupTab.AddTab( m_DtaData[ShopType].m_GruopList[i]->m_strFileName, strTemp );
	}
	m_GroupTab.NowSelectedIndex = 0; // ����� ������ 2��° ���� �켱 �����ֵ���.

	// ��ǰ �̹��� �ε�
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

CItem* CControlCashShop::Draw() // �� �Լ��� ������ DrawItemInfo()�Լ����� ǥ�õǴ� ������ ������ �����ϱ� ���� ���δ�.
{
	char strTemp[256] = ""; // ���� ��������ִ� ��ǰ ����۵� 255������ �ޱ�� ����ߴ�.

	if( !g_pDisplay )
		return NULL;

	g_pDisplay->Blt( m_pBackSur->Xpos, m_pBackSur->Ypos, m_pBackSur );

	m_GroupTab.Draw();

	// �ǸŸ�� ��ο�.
	DrawCatalogList();

	m_InfoTab.Draw();
	// ���� ��ο�.
	DrawInfo();
	m_InfoScroll.Draw();

	m_InvenTabBtn[g_pRoh->m_CurPackNum].SetState( BTN_DOWN ); // ���õȰ� ������ ǥ�õǰ�...
	for( int i = 0 ; i < PACK_NUM ; ++i )
		m_InvenTabBtn[i].Draw(); // �κ� �� ��ư

	DrawInven();


	m_PrevBtn.Draw(); // ���� �Ǹ� ��� ���� ��ư.
	m_NextBtn.Draw(); // ���� �Ǹ� ��� ���� ��ư.

	// ���� ĳ�� ���.
	sprintf( strTemp, "%d", g_pRoh->m_HaveCash );
	m_TextOut.SetString( strTemp, RGB(255,255,0) );
	m_TextOut.Draw( m_pBackSur->Xpos+715, m_pBackSur->Ypos+563, 0 );

	m_CloseBtn.Draw();

	pCMyApp->m_pMouse->SetMouseType( M_NORMAL ); // ���콺 Ÿ���� ������...

	return m_pInvenOverItem; // ������ ������ ��¾��Ѵ�.
}

void CControlCashShop::DrawCatalogList() // ��ǰ ��� ���.
{
	char strTemp[256] = ""; // ���� ��������ִ� ��ǰ ����۵� 255������ �ޱ�� ����ߴ�.

	int ViewInPageNum = MAX_VIEWCATALOG_NUM; // �� �������� �������� ��ǰ ����.(���� �ػ󵵺��� �߰��Ǿ����� �𸥴�.)
	int ViewHorizonNum = 2; // ���̴� ��ǰ ���� ����.
	int ViewVerticalNum = 3; // ���̴� ��ǰ ���� ����.

	int TempInt = 0;
	int TempX = 0, TempY = 0;

	int i = 0;

	for( i = 0 ; i < MAX_VIEWCATALOG_NUM ; ++i )
		m_BuyBtn[i].SetDisable( TRUE ); // �ϴ� ��ư���� ���� ��Ȱ��ȭ ��Ų��.(�޽��� ���ν��� üũ�� ����..)

	m_NowOnCatalogIndex = -1; // ���� ���콺�� �ö� ��ǰ ��ȣ �ʱ�ȭ.

	POINT point;
	GetCursorPos(&point);
	ScreenToClient(g_pDisplay->GetHWnd(), &point); // ���� ���콺 �ö��ִ� �༮�� ����ϱ� ���ؼ�...

	sSellCatalogInfo *pCatalog;
	vector<sSellCatalogInfo *>::iterator pr;

	int DrawCount = 0;

	// ������ ��ǰ���� ���� ���鼭.
	for( pr = m_ViewCatalogList.begin() ; pr != m_ViewCatalogList.end() ; pr++ )
	{
		pCatalog = *pr;

		if( DrawCount < m_NowViewCatalogPage*ViewInPageNum ) // ������ �༮�� ���� �����̸�..
		{
			++DrawCount; // ī��Ʈ�� �������Ѿ��Ѵ�.
			continue; // ����.
		}

		TempInt = DrawCount % ViewInPageNum; // 6���� ���° �༮�ΰ�..
		TempX = m_pBackSur->Xpos + 12 + ( (TempInt%ViewHorizonNum) * 261 ); // �׷��� X��ǥ ���
		TempY = m_pBackSur->Ypos + 90 + ( (TempInt/ViewHorizonNum) * 156 ); // �׷��� Y��ǥ ���

		g_pDisplay->Blt( TempX, TempY, m_pItemSlotSur ); // īŻ�α� ��� ���.

		// ������ �̹���(������) ���
		if( m_pCatalogImageList[ FindIconImage(pCatalog->m_IconImageIndex) ] )
			g_pDisplay->Blt( TempX+5, TempY+14, m_pCatalogImageList[ FindIconImage(pCatalog->m_IconImageIndex) ] ); // ������ �̹��� ���.

		if( pCatalog->m_pTextSur )
			g_pDisplay->Blt( TempX, TempY, pCatalog->m_pTextSur ); // �ؽ�Ʈ�� ���.

		// ���� ��ư ���.
		m_BuyBtn[TempInt].SetPosition( TempX+187, TempY+126 ); // �̰� ���⼭ �ϴ°� ��ȿ�� �������� ��ǥ����� ���ϴ� �׳� ���⼭�Ѵ�.
		m_BuyBtn[TempInt].SetDisable( FALSE ); // ���̴� �༮�� Ȱ��ȭ.(�޽��� ���ν��������� �ʿ�.)
		m_BuyBtn[TempInt].Draw();

		// ���õ� �༮ ó��.
		if( pCatalog == m_pSelectCatalog ) // ���õ� �༮�̸�...
			g_pDisplay->Blt( TempX, TempY, m_pSelectSlotSur ); // �׵θ��� ó�ش�.


		++DrawCount;

		// ���콺�� �ö󰬴��� üũ.
		if( point.x >= TempX && point.x <= TempX+m_pItemSlotSur->GetWidth()
				&& point.y >= TempY && point.y <= TempY+m_pItemSlotSur->GetHeight() ) // �� �༮���� ���콺�� �ö�������...
			m_NowOnCatalogIndex = pCatalog->m_CatalogIndex;

		if( DrawCount >= m_NowViewCatalogPage*ViewInPageNum+ViewInPageNum ) // ������ �༮���� �� ����������...
			break; // �ߴ�
	}

	// ������ ��ȣ ���
	sprintf( strTemp, "[%02d/%02d]", m_NowViewCatalogPage+1, m_ViewCatalogList.size()?(m_ViewCatalogList.size()-1)/ViewInPageNum+1:1 );
	m_TextOut.SetString( strTemp, FALSE );
	m_TextOut.Draw( m_pBackSur->Xpos+243, m_pBackSur->Ypos+568 );

}

void CControlCashShop::GetInfo1Text( int ItemIndex ) // �������κ��� MultiText�� �о� ���� �κ�.(���� �ɶ��� �ѹ��� ȣ��������)
{
	m_MultiText.Reset(); // �ϴ� �ѹ� �����.
	SAFE_DELETE( m_pItemInfoTextSur ); // �� �༮�� ������ �ٲ� ������ ����� ����.

	if( m_pSelectCatalog->m_ItemCount == 0 || ItemIndex >= m_pSelectCatalog->m_ItemCount ) // �������� ���ų� �̻� ����üũ.
		return;

	CItem *pItem = &(m_pSelectCatalog->m_Item[ItemIndex]); // �����ͷ� �ٲ㼭...

	if( !pItem || pItem->m_ProtoNum == -1 ) // protoNum�� �ݵ�� üũ�ؾ��Ѵ�.
		return;

	// MultiText�� ����.
	g_pNk2DFrame->GetControlInven()->m_bDrawInfoUpdate = TRUE; // ������ ������ ������ ���Ž�Ų��.(�̷��� ���ϸ� �������� ��ų� �ٸ� �������� �ôٰ� �;��� ���ŵȴ�.)
	g_pNk2DFrame->GetControlInven()->DrawItemInfo( pItem, ITEM_INFO_CASHSHOP ); // ITEM_INFO_CASHSHOP �� ȭ�鿡 ������� �ʵ���...
	////////////////////////////////////////////////////////////////////////
	// ��Ƽ �ؽ�Ʈ�� �̸� ���ǽ��� ����� ���´�.
	// ũ�� ���.
	int TextSurHeight = m_MultiText.m_LineCount*(14+14);	// ���� ���̸� ���� ����ؼ� �� �˳��ϰ�...

	g_pDisplay->CreateSurface( &m_pItemInfoTextSur, 230, TextSurHeight ); // �� ���� ���� ��� ��ġ�� ���...
	if( m_pItemInfoTextSur )
	{
		m_pItemInfoTextSur->SetWidth( 230 ); // ���� ũ��� �Ƚ�.
		m_pItemInfoTextSur->SetHeight( TextSurHeight );

		m_pItemInfoTextSur->Clear( 0 );
		m_pItemInfoTextSur->SetColorKey( 0 );

		HFONT TempFont = G_GetFont(14,0,g_strFontName);

		for( int i = 0 ; i < m_MultiText.m_LineCount ; ++i ) // ���� ����ŭ ����.
			m_pItemInfoTextSur->DrawText( TempFont, m_MultiText.m_Text[i], 10, 10+(i*13), 0, m_MultiText.m_TextColor[i], 0 );
	}


	// ������ ���� ���̾����� ��ũ�� �������� ����.(���ǽ��� �ȼ� ������...)
	if( m_InfoTab.NowSelectedIndex == 0 )
	{
		if( m_pItemInfoTextSur->GetHeight() >= VIEW_INFO1_LINE+1 )
			m_InfoScroll.SetRange( m_pItemInfoTextSur->GetHeight() - VIEW_INFO1_LINE + 1 ); // ������ ���ο� ���缭 ���� ����.
		else
			m_InfoScroll.SetRange( 1 );

		m_InfoScroll.SetPos( 0 );
	}
}

int CControlCashShop::DrawInfo() // ������,���ų��� ���� ���.
{
	char strTemp[256];
	int i = 0, j = 0;
	RECT DrawRect; // ��� ���� ����.

	int DrawCount = 0; // �׷��� �ȼ����� ī��Ʈ. ��ũ����ġ�� ���� ��ŵ�� ���.

	switch( m_InfoTab.NowSelectedIndex )
	{
	case 0: // ������ �� ����.

		g_pDisplay->Blt( m_pBackSur->Xpos+540, m_pBackSur->Ypos+67, m_pInfo1BackSur ); // ���� ��� ���.

		if( m_pSelectCatalog && m_pSelectCatalog->m_ItemCount > 0 && m_pSelectCatalog->m_ItemCount <= MAX_ITEM_IN_CATALOG ) // ���� �����Ͱ� �ִٸ�...
		{
			CItem *pItem = &(m_pSelectCatalog->m_Item[m_NowInfoPage]); // �����ͷ� �ٲ㼭...

			if( !pItem )
				return DrawCount;

			// ������ �̹����� ���.
			CLolos *pItemLolo = GET_LOLO(pItem);

			if( pItem->m_ProtoNum != -1 && pItemLolo ) // ProtoNum�� �� üũ�ؾ��Կ� ����.
			{
				DWORD ambient = 0x00555555;

				if( GET_TYPE(pItem) == ITYPE_ETC && GET_SHAPE(pItem) == IETC_SCROLL )
					ambient = 0x00cccccc;
				else if( pItem->m_PlusNum > 0 )
					ambient = UPGRADE_ITEM_COLOR;

				int ImageWidth = SLOT_WIDTH*GET_ITEM_WIDTH(pItem); // �ѷ� �̹����� ȭ�� ���� ����.
				int ImageHeight = SLOT_HEIGHT*GET_ITEM_HEIGHT(pItem); // �ѷ� �̹����� ȭ�� ���� ����.

				if( GET_ITEM_WIDTH(pItem) == 1 ) // ������ ũ�Ⱑ 1�� ��� ��ġ�� �̻��ϴ��� �� �������ش�.
					DrawRect.left = m_pBackSur->Xpos+649-(ImageWidth/2);
				else
					DrawRect.left = m_pBackSur->Xpos+663-(ImageWidth/2);

				DrawRect.top = m_pBackSur->Ypos+70;


				g_pNk2DFrame->GetItemRender()->BeginRenderLolo();

				g_pNk2DFrame->RenderItemInUI( DrawRect.left, DrawRect.top, pItem , ambient, FALSE, TRUE , TRUE );


				g_pNk2DFrame->GetItemRender()->EndRenderLolo();
			}

			if( m_pItemInfoTextSur ) // ����� �ؽ�Ʈ ���ǽ��� �ִٸ�..
			{
				// Ŭ�����ؼ� ����ؾ��Ѵ�.
				DrawRect.left = 0;
				DrawRect.top = m_InfoScroll.GetPos();
				DrawRect.right =  m_pItemInfoTextSur->GetWidth();
				if( m_pItemInfoTextSur->GetHeight() > VIEW_INFO1_LINE+DrawRect.top )
					DrawRect.bottom =  VIEW_INFO1_LINE+DrawRect.top;
				else
					DrawRect.bottom =  m_pItemInfoTextSur->GetHeight();


				g_pDisplay->Blt( m_pBackSur->Xpos+545, m_pBackSur->Ypos+175, m_pItemInfoTextSur, &DrawRect ); // �ؽ�Ʈ�� ���.(Ŭ�����ؾ��Ѵ�.)
			}

			// ������ ��ȣ ���
			sprintf( strTemp, "[%02d/%02d]", m_NowInfoPage+1, m_pSelectCatalog->m_ItemCount );
			m_TextOut.SetString( strTemp, RGB(255,255,255) );
			m_TextOut.Draw( m_pBackSur->Xpos+665, m_pBackSur->Ypos+353, 0 );

			m_InfoPrevBtn.Draw(); // ���� �Ǹ� ��� ���� ��ư.
			m_InfoNextBtn.Draw(); // ���� �Ǹ� ��� ���� ��ư.
		}
		break;

	case 1: // ���� ����.

		g_pDisplay->Blt( m_pBackSur->Xpos+540, m_pBackSur->Ypos+67, m_pInfo2BackSur ); // ���� ��� ���.

		sSellCatalogInfo *pCatalog;
		vector<sSellCatalogInfo *>::iterator pr;

		// ��� ��ǰ���� ���� ���鼭.
		for( pr = m_BuyCatalogList.begin() ; pr != m_BuyCatalogList.end() ; pr++ )
		{
			pCatalog = *pr;

			if( DrawCount < m_InfoScroll.GetPos() ) // ���⼱ �ȼ��� �ƴ϶� ��ǰ������ ���.
			{
				++DrawCount; // ��ǰ������ ����.
				continue;
			}

			if( pCatalog->m_pTextSur )
				g_pDisplay->Blt( m_pBackSur->Xpos+548, m_pBackSur->Ypos+69+((DrawCount-m_InfoScroll.GetPos())*30), pCatalog->m_pTextSur ); // �ؽ�Ʈ�� ���.

			++DrawCount;

			if( DrawCount >= m_InfoScroll.GetPos() + VIEW_INFO2_LINE ) // ������ ���� �Ѿ����...(�� �������� ������ ����ϱ� ���� ����д�.)
				break; // ������ �ߴ�.
		}

		//-- IDS_TOTAL_COUNT : TotalCount : %d
		sprintf( strTemp, (char*)G_STRING(IDS_TOTAL_COUNT), m_BuyCatalogList.size() );
		m_TextOut.SetString( strTemp, RGB(255,255,200) ); // �� ���� ����.
		m_TextOut.Draw( m_pBackSur->Xpos+548, m_pBackSur->Ypos+343 );
		//-- IDS_TOTAL_CASH : TotalCash  : %d
		sprintf( strTemp, (char*)G_STRING(IDS_TOTAL_CASH), m_TotalUseCash );
		m_TextOut.SetString( strTemp, RGB(255,255,200) ); // �� ĳ��
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

		// ����ǰ �׸���...
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
				if( pItem->m_SlotY == 5 && GET_ITEM_HEIGHT(pItem) == 1 ) // ITEM_RENDER_HEIGHT �� �߸��Ǿ��ִ� ���������� ����ó��.
				{
					// ĳ�������� ���� ���������� 1����¥�� �������� ����� �߸��Ǵ� ������ �ִ�.
					// ������ ITEM_RENDER_HEIGHT�� �߸��Ǿ ������ �̰� �ٿ��� �������� ��ġ�� ���� ������ ũ�⺸��
					// �۰� ���δ�.(�̰� ���� ũ���̱�������) �۰� ���̸� �������� ITEM_RENDER_HEIGHT �� �״�� ���׻��·� �ΰ�
					// ���� ������ �Ǵ� ĳ�������� ���������� 1����¥�� �����۸� ����ó���Ѵ�.
					// �������� ����� �����Ѵ�. (��ġ ���� ����,����ũ�⸦ -1�� �־��ָ� ���� ����� �ȴ�.)
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

				m_pInvenOverItem = pItem; // ���� �����͸� ���.(����� �ܺο��� �Ѵ�.�۾� ����� ����� ��������.)
			}
			else
			{
				DWORD ambient = 0x00555555;
				//��ũ�Ѱ� ��й�� ambient�� ����

				if( g_pNk2DFrame->IsScroll(pItem)
						|| pItem->m_Vnum == 1251
						|| pItem->m_Vnum == 1308 // ���ǻ��� ���� ��� ���� 05-10-05 ����
						|| pItem->m_Vnum == 1286
						|| ( pItem->m_Vnum >= 1598 && pItem->m_Vnum <= 1695 ) // ����� ��û ���̱������ ������ �����⿡�� �����̹Ƿ� ����ó��.
						|| pItem->m_Vnum == 1287 ) // �̱� ������ ����, ������ ������ ��Ӱ� ������ Ŭ���̾�Ʈ���� ���� ��� ó��.(by ����)
					ambient = 0x00cccccc;
				else if (pItem->m_PlusNum > 0)
					ambient = UPGRADE_ITEM_COLOR;

				if( pItem->m_SlotY == 5 && GET_ITEM_HEIGHT(pItem) == 1 ) // ITEM_RENDER_HEIGHT �� �߸��Ǿ��ִ� ���������� ����ó��.
				{
					// ĳ�������� ���� ���������� 1����¥�� �������� ����� �߸��Ǵ� ������ �ִ�.
					// ������ ITEM_RENDER_HEIGHT�� �߸��Ǿ ������ �̰� �ٿ��� �������� ��ġ�� ���� ������ ũ�⺸��
					// �۰� ���δ�.(�̰� ���� ũ���̱�������) �۰� ���̸� �������� ITEM_RENDER_HEIGHT �� �״�� ���׻��·� �ΰ�
					// ���� ������ �Ǵ� ĳ�������� ���������� 1����¥�� �����۸� ����ó���Ѵ�.
					// �������� ����� �����Ѵ�. (��ġ ���� ����,����ũ�⸦ -1�� �־��ָ� ���� ����� �ȴ�.)
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

	if( m_pWantBuyCatalog ) // ���� ��� ��ǰ �����Ͱ� �ִٸ�...�˾��̳� ��Ŷ ��ٸ��� ���̴�.
		return 1; // �ٸ��� ���ϰ�...

	if( m_InfoScroll.MsgProc(hWnd, msg, wParam, lParam) == S_OK )
		return 1;

	int i = 0;

	switch (msg)
	{
	case 0x020A: // �ٸ��콺 �޼���(����Ʈ �����ȿ����� �� �޼����� ������ ��ũ�ѿ� ���������ش�.)

		if( IsInside(x,y) ) // �������� ���� �ϳ� ��.
		{
			if( (short)HIWORD(wParam) < 0 )
			{
				if( m_InfoTab.NowSelectedIndex == 0 ) // �� ������ �ȼ� �����Ƿ� �� ���� �������ش�.
					m_InfoScroll.NowPos += 10;
				else
					m_InfoScroll.NowPos += 1;

				if( m_InfoScroll.NowPos > m_InfoScroll.RangeMax-1 )
					m_InfoScroll.NowPos = m_InfoScroll.RangeMax-1;
			}
			else if( (short)HIWORD(wParam) > 0 )
			{
				if( m_InfoTab.NowSelectedIndex == 0 ) // �� ������ �ȼ� �����Ƿ� �� ���� �������ش�.
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

		if( IsInside(x,y) ) // �������̽��� ������ Ŭ������ ���.
		{
			m_GroupTab.MsgProc(hWnd, msg, wParam, lParam);
			m_InfoTab.MsgProc(hWnd, msg, wParam, lParam);
			m_InfoPrevBtn.MsgProc(hWnd, msg, wParam, lParam);
			m_InfoNextBtn.MsgProc(hWnd, msg, wParam, lParam);

			// BUY ��ư üũ
			for( i = 0 ; i < MAX_VIEWCATALOG_NUM ; ++i )
				m_BuyBtn[i].MsgProc(hWnd, msg, wParam, lParam);

			for( i = 0 ; i < PACK_NUM ; ++i )
				m_InvenTabBtn[i].MsgProc(hWnd, msg, wParam, lParam);

			m_PrevBtn.MsgProc(hWnd, msg, wParam, lParam);
			m_NextBtn.MsgProc(hWnd, msg, wParam, lParam);

			m_CloseBtn.MsgProc(hWnd, msg, wParam, lParam);

			// ��ǰ ���� üũ.
			if( m_NowOnCatalogIndex != -1 ) // �̰� -1�� �ƴϸ� ���콺�� ��� ��ǰ ���� �ö� �ִ��Ŵ�.
			{
				sSellCatalogInfo *pCatalog;
				vector<sSellCatalogInfo *>::iterator pr;

				int DrawCount = 0;

				// ������ ��ǰ���� ���� ���鼭.
				for( pr = m_ViewCatalogList.begin() ; pr != m_ViewCatalogList.end() ; pr++ )
				{
					pCatalog = *pr;

					if( pCatalog->m_CatalogIndex == m_NowOnCatalogIndex ) // ���콺�� �÷����ִ� ��ǰ�̸�...
					{
						m_pSelectCatalog = pCatalog; // ���� ������ ����.
						m_NowOnCatalogIndex = -1; // ��ӵ����� �ʰ� �̳༮�� �ʱ�ȭ.
						m_NowInfoPage = 0; // ������ ���� �������� �ʱ�ȭ.
						GetInfo1Text( m_NowInfoPage ); // MultiText ����

						break;
					}
				}
			}

			return 1;
		}

		return 0; // �ٸ� �������̽� �κ��� ����� ���...(�κ�,Bottom��...)

	case WM_MOUSEMOVE:
	case WM_LBUTTONUP:

		if( IsInside(x,y) ) // �������̽��� ������ Ŭ������ ���.
		{
			if( m_GroupTab.MsgProc(hWnd, msg, wParam, lParam) ) // 1�̸� ���� �� �ٲ�Ŵ�.
			{
				GroupChange( m_GroupTab.NowSelectedIndex ); // ���� ���� �ٲ���� ���� ó��.(�� �ε����� �Ѱ�����Կ� ����)

				return 1;
			}

			if( m_InfoTab.MsgProc(hWnd, msg, wParam, lParam) ) // info �κ� ��
			{
				if( m_InfoTab.NowSelectedIndex == 0 ) // ������.
				{
					m_InfoScroll.SetDrawPos( m_pBackSur->Xpos+773, m_pBackSur->Ypos+176, 164 ); // �������� Ÿ�Կ� ���� ��ũ�� ��ġ�� ���̰� �ٲ��� �Ѵ�.

					if( m_pSelectCatalog ) // ���õǾ��� ��ǰ�� ������.
					{
						// ������ ��ǰ�� �ٲ������ ��ũ�� �������� ����
						GetInfo1Text( 0 );
					}
					else // ���õǾ����� ������...
						m_InfoScroll.SetRange( 1 );
				}
				else if( m_InfoTab.NowSelectedIndex == 1 ) // ���� ����.
				{
					m_InfoScroll.SetDrawPos( m_pBackSur->Xpos+773, m_pBackSur->Ypos+70, 300 );

					if( m_BuyCatalogList.size() >= VIEW_INFO2_LINE+1 )
						m_InfoScroll.SetRange( m_BuyCatalogList.size() - VIEW_INFO2_LINE + 1 ); // ������ ���ο� ���缭 ���� ����.
					else
						m_InfoScroll.SetRange( 1 );
				}
			}
			m_InfoPrevBtn.MsgProc(hWnd, msg, wParam, lParam);
			if( m_InfoPrevBtn.GetState()==BTN_ACTION ) // ���� ������ ����
			{
				if( m_NowInfoPage > 0 )
				{
					--m_NowInfoPage;
					GetInfo1Text( m_NowInfoPage ); // MultiText ����
				}

				m_InfoPrevBtn.SetState(BTN_NORMAL);	// ��ư�� �������ͽ��� ���� Ǯ������ϳ�����.
				return 1;
			}

			m_InfoNextBtn.MsgProc(hWnd, msg, wParam, lParam);
			if( m_InfoNextBtn.GetState()==BTN_ACTION ) // ���� ������ ���� Ŭ����
			{
				if( m_NowInfoPage < m_pSelectCatalog->m_ItemCount-1 )
				{
					++m_NowInfoPage;
					GetInfo1Text( m_NowInfoPage ); // MultiText ����
				}


				m_InfoNextBtn.SetState(BTN_NORMAL);	// ��ư�� �������ͽ��� ���� Ǯ������ϳ�����.
				return 1;
			}

			// BUY ��ư üũ
			for( i = 0 ; i < MAX_VIEWCATALOG_NUM ; ++i )
			{
				m_BuyBtn[i].MsgProc(hWnd, msg, wParam, lParam);
				if( m_BuyBtn[i].GetState()==BTN_ACTION )
				{
					m_pWantBuyCatalog = m_ViewCatalogList[m_NowViewCatalogPage*MAX_VIEWCATALOG_NUM+i]; // ����� �� īŻ�α׸� ����صд�.

					//-- IDS_CASHSHOP_BUYCHECK : %s ��ǰ�� �����Ͻðڽ��ϱ�?
					sprintf(strTemp, G_STRING(IDS_CASHSHOP_BUYCHECK), m_pWantBuyCatalog->m_strCatalogName );
					g_pNk2DFrame->InsertPopup( strTemp,TYPE_NOR_OKCANCLE, 29 ); // �˾� ���.

					m_BuyBtn[i].SetState(BTN_NORMAL);	// ��ư�� �������ͽ��� ���� Ǯ������ϳ�����.
					return 1;
				}
			}

			for( i = 0 ; i < PACK_NUM ; ++i )
			{
				m_InvenTabBtn[i].MsgProc(hWnd, msg, wParam, lParam);
				if( m_InvenTabBtn[i].GetState()==BTN_ACTION ) // �����ݱ� ��ư Ŭ����
				{
					g_pRoh->ChangePackNum(i);
					m_InvenTabBtn[i].SetState(BTN_NORMAL);	// ��ư�� �������ͽ��� ���� Ǯ������ϳ�����.
					return 1;
				}
			}

			m_PrevBtn.MsgProc(hWnd, msg, wParam, lParam);
			if( m_PrevBtn.GetState()==BTN_ACTION ) // �����ݱ� ��ư Ŭ����
			{
				if( m_NowViewCatalogPage > 0 )
					--m_NowViewCatalogPage;

				m_PrevBtn.SetState(BTN_NORMAL);	// ��ư�� �������ͽ��� ���� Ǯ������ϳ�����.
				return 1;
			}

			m_NextBtn.MsgProc(hWnd, msg, wParam, lParam);
			if( m_NextBtn.GetState()==BTN_ACTION ) // �����ݱ� ��ư Ŭ����
			{
				if( m_NowViewCatalogPage < (m_ViewCatalogList.size()?(m_ViewCatalogList.size()-1)/MAX_VIEWCATALOG_NUM:0) )
					++m_NowViewCatalogPage;

				m_NextBtn.SetState(BTN_NORMAL);	// ��ư�� �������ͽ��� ���� Ǯ������ϳ�����.
				return 1;
			}

			m_CloseBtn.MsgProc(hWnd, msg, wParam, lParam);
			if( m_CloseBtn.GetState()==BTN_ACTION ) // �����ݱ� ��ư Ŭ����
			{
				g_pNk2DFrame->ToggleCashShopWindow( -1 ); // Toggle �Լ��� ����ؼ� ���� �����Ų��.(����� ���´ٴ°� ���ִ� ���´ϱ� ������ ������ ����)

				m_CloseBtn.SetState(BTN_NORMAL);	// ��ư�� �������ͽ��� ���� Ǯ������ϳ�����.

				return 1;
			}
		}

		break;
	}

	return 0;
}

HRESULT CControlCashShop::BuyPopupMsg( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, int nMsg ) // ���� �˾��� ���ν���.
{
	char commBuf[512];

	if( nMsg == 1 ) // Ȯ���� �������.
	{
		sprintf( commBuf, "multi_shop buy %d %d\n", g_pRoh->m_CurPackNum, m_pWantBuyCatalog->m_CatalogIndex ); // �ѳ�, ��ǰ �ε���.
		if( g_pTcpIp )
			g_pTcpIp->SendNetMessage( commBuf, TRUE); // ���� ��û ��Ŷ�� ������.
	}
	else if( nMsg == -1 ) // ��Ұ� ���� ���
	{
		m_pWantBuyCatalog = NULL; // ���� ��ǰ ����س����� Ǯ���ش�.(Ȯ�νÿ� ��Ŷ�ް� �����ϰ� Ǯ���ش�.
	}

	return 0L;
}

void CControlCashShop::GroupChange( int TabIndex ) // ���� ���� �ٲ���� ���� ó��. (���ڰ� �׷��ε����� �ƴ϶� ���ε����ӿ� ����)
{
	// �׷� �ε��� ���ϱ�.
	int GroupIndex = 0;
	char strTemp[255];
	strcpy( strTemp, m_DtaData[m_iShopKind].m_GruopList[TabIndex]->m_strFileName );
	if( strcmp(m_DtaData[m_iShopKind].m_GruopList[TabIndex]->m_strFileName,"CashShop/Tab/Tab_All") == 0 ) // �̸��� "All" �̸� ��ü ����...
		GroupIndex = -1;
	else if( strcmp(m_DtaData[m_iShopKind].m_GruopList[TabIndex]->m_strFileName,"CashShop/Tab/Tab_New") == 0 ) // �̸��� "New" �̸� �Ż�ǰ ����...
		GroupIndex = -2;
	else
		sscanf( m_GroupTab.GetExtraString(TabIndex), "%d", &GroupIndex ); // ExtraString�� ����ص� �׷� �ε����� ���´�.

	m_ViewCatalogList.clear(); // ���� ������ ����Ʈ�� ���̹Ƿ� �׳�  clear��

	// ����Ʈ�� �Ű� ��´�.
	sSellCatalogInfo *pCatalog;
	vector<sSellCatalogInfo *>::iterator pr;

	// ��ϵ� ��ǰ���� ���� ���鼭.
	for( pr = m_DtaData[m_iShopKind].m_SellCatalogList.begin() ; pr != m_DtaData[m_iShopKind].m_SellCatalogList.end() ; pr++ )
	{
		pCatalog = *pr;

		if( GroupIndex == -1 ) // �׷� �ε��� -1 ��ü �����. (���� ��ü���� �ֱ� ������ -1�� ���� �϶�� �� ��)
		{
			m_ViewCatalogList.push_back( pCatalog ); // �˻���� �׳� �ִ´�.
		}
		else if( GroupIndex == -2 ) // �׷� �ε��� -2 �Ż�ǰ �����.
		{
			if( pCatalog->m_bNewCatalog ) // �Ż�ǰ�̸�...
				m_ViewCatalogList.push_back( pCatalog ); // �ִ´�.
		}
		else
		{
			if( pCatalog->m_GroupIndex == GroupIndex ) // �� �׷��� ��ǰ�̸�...
				m_ViewCatalogList.push_back( pCatalog ); // �ִ´�.
		}
	}

	std::sort(m_ViewCatalogList.begin(), m_ViewCatalogList.end(), ViewCatalogCompare);

	m_NowViewCatalogPage = 0;
	m_pSelectCatalog = NULL; // ���õ� ��ǰ ����..
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

int CControlCashShop::FindIconImage( int DbIndex ) // ������ �̹����� DB�ε����� ���� ã�´�.
{
	sImageInfo *pImage;
	vector<sImageInfo *>::iterator pr;

	int VectorIndex = 0;

	// ������ ��ǰ���� ���� ���鼭.
	for( pr = m_DtaData[m_iShopKind].m_CatalogImageList.begin() ; pr != m_DtaData[m_iShopKind].m_CatalogImageList.end() ; pr++ )
	{
		pImage = *pr;

		if( pImage->m_Index == DbIndex ) // ���� �ε����� ã�Ƽ�..
			return VectorIndex;

		++VectorIndex;
	}

	return 0;
}

void CControlCashShop::MakeCatalogTextSurface() // īŻ�α׾ȿ� ���� �ؽ�Ʈ�� ���ǽ��� �����.
{
	int i = 0;
	char strTemp[256] = ""; // ���� ��������ִ� ��ǰ ����۵� 255������ �ޱ�� ����ߴ�.
	HFONT TempFont;

	sSellCatalogInfo *pCatalog;
	vector<sSellCatalogInfo *>::iterator pr;

	// ��ϵ� ��ǰ���� ���� ���鼭.
	for( pr = m_DtaData[m_iShopKind].m_SellCatalogList.begin() ; pr != m_DtaData[m_iShopKind].m_SellCatalogList.end() ; pr++ )
	{
		pCatalog = *pr;

		SAFE_DELETE( pCatalog->m_pTextSur ); // Ȥ�� �𸣴�...
		// ��� ũ�⸸ŭ ���ǽ� ����.
		g_pDisplay->CreateSurface( &pCatalog->m_pTextSur, m_pItemSlotSur->GetWidth(), m_pItemSlotSur->GetHeight() );
		if( pCatalog->m_pTextSur )
		{
			pCatalog->m_pTextSur->Clear( 0 );
			pCatalog->m_pTextSur->SetColorKey( 0 );

			// ��ǰ��.
			if( g_dwClientCountry == CTRY_BRA ) // ������� ��ǰ�� ���� ũ�� �۰�...
				TempFont = G_GetFont(14,0,g_strFontName);
			else
				TempFont = G_GetFont(20,0,g_strFontName);
			pCatalog->m_pTextSur->DrawText( TempFont, pCatalog->m_strCatalogName, 100, 15, 0, RGB(255,100,200), 0 );

			// ��ǰ ����.
			TempFont = G_GetFont(12,0,g_strFontName);
			CMultiText TempMultiText;
			TempMultiText.AddStringMulti( pCatalog->m_strCatalogInfo, 25, TRUE ); // ��Ʈ���� ������ ������ �����ش�.
			for( i = 0 ; i < TempMultiText.m_LineCount ; ++i ) // ���� ����ŭ ����.
				pCatalog->m_pTextSur->DrawText( TempFont, TempMultiText.m_Text[i], 95, 38+(i*13), 0, m_TextOut.m_FontColor, 0 );

			TempFont = G_GetFont(14,0,g_strFontName);
			// ��ǰ ����.
			//-- IDS_CASHSHOP_PRICE : Price  : %d
			sprintf( strTemp, (char*)G_STRING(IDS_CASHSHOP_PRICE), pCatalog->m_Price );
			pCatalog->m_pTextSur->DrawText( TempFont, strTemp, 97, 117, 0, RGB(100,100,255), 0 );

			// ���ϸ���.
			//-- IDS_CASHSHOP_MILEASE : Milease: %d
			sprintf( strTemp, (char*)G_STRING(IDS_CASHSHOP_MILEASE), pCatalog->m_Milease );
			pCatalog->m_pTextSur->DrawText( TempFont, strTemp, 97, 130, 0, RGB(100,100,255), 0 );

		}
	}
}

void CControlCashShop::MakeBuyListText( sSellCatalogInfo *pCatalog )  // ���ų����� �ؽ�Ʈ�θ� �̸� ����.
{
	char strTemp[256] = ""; // ���� ��������ִ� ��ǰ ����۵� 255������ �ޱ�� ����ߴ�.
	HFONT TempFont = G_GetFont(14,0,g_strFontName);

	SAFE_DELETE( pCatalog->m_pTextSur ); // Ȥ�� �𸣴�...
	// ��� ũ�⸸ŭ ���ǽ� ����.
	g_pDisplay->CreateSurface( &pCatalog->m_pTextSur, 230, 30 ); // �� ���� ���� ��� ��ġ�� ���...
	if( pCatalog->m_pTextSur )
	{
		pCatalog->m_pTextSur->Clear( 0 );
		pCatalog->m_pTextSur->SetColorKey( 0 );

		// ��ǰ��.
		sprintf( strTemp, "[%s]", pCatalog->m_strCatalogName );
		pCatalog->m_pTextSur->DrawText( TempFont, strTemp, 5, 1, 0, RGB(230,230,230), 0 );

		// ����
		//-- IDS_CASHSHOP_PRICE : Price: %d
		char strPrice[256] = {0,};
		sprintf( strTemp, (char*)G_STRING(IDS_CASHSHOP_PRICE), pCatalog->m_Price );
		sprintf( strPrice, "[%s]", strTemp ); // ���η� ����.
		pCatalog->m_pTextSur->DrawText( TempFont, strPrice, 5, 15, 0, RGB(255,255,0), 0 );

		// ���� �Ͻ�
		// [5/6/2008 �����] �±��� ������� ������ ���Խ� �ð�ǥ�ÿ� +7�ð� ����� �Ѵ�.
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
// ĳ�� ���� ���� �������� ���������� �� ���� ������
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
// 		m_pBackSur->SetWidth( 800 ); // �� CreateSurface���� CSurface �� ���� �����صδ� m_dwWidth,m_dwHeight�� ���� ��������ִ� ���װ� �־ �׳� ���� ����.
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
	// ������� �����ش�. �ʿ䵵 ���µ� ��üȭ�� ��� �������� open �� ȭ����ȯ�Ǵ� ������ �ִ�.
	DeleteRes();
	LoadRes();
}

void CCashShopIntro::Open()
{
	m_WebPage1.LoadRes(); // �����츦 �����.
	m_WebPage2.LoadRes(); // �����츦 �����.

	char strUrl1[256] = "";
	char strUrl2[256] = "";

	switch( g_dwClientCountry )
	{
	case CTRY_TH:
//		strcpy( strUrl1, "http://61.90.198.67/update2/info.htm" );
//		strcpy( strUrl2, "http://61.90.198.67/update2/update.htm" );
		GetCurrentDirectory( 256, strUrl1 );
		strcat( strUrl1, "/interface/CashShop/Intro/html/info.htm" ); // �±� ���� ������ �ϵ�ȿ��� �е��� ����.(�ٲܶ� ��ġ�ϰ�...)
//		strcpy( strUrl1, "./interface/CashShop/Intro/html/info.htm" );
		GetCurrentDirectory( 256, strUrl2 );
		strcat( strUrl2, "/interface/CashShop/Intro/html/update.htm" );
//		strcpy( strUrl2, "http://61.90.198.67/update2/update.htm" );
		break;

	default:
// 		strcpy( strUrl1, "www.naver.co.kr" );
// 		strcpy( strUrl2, "www.yahoo.co.kr" );
		GetCurrentDirectory( 256, strUrl1 );
		strcat( strUrl1, "/interface/CashShop/Intro/html/info.htm" ); // �±� ���� ������ �ϵ�ȿ��� �е��� ����.(�ٲܶ� ��ġ�ϰ�...)
		GetCurrentDirectory( 256, strUrl2 );
		strcat( strUrl2, "/interface/CashShop/Intro/html/update.htm" );
		break;
	}

	// Ǯ��ũ���� ������ ��忡�� ��ġ�� �ڲ� Ʋ��������. �׷��� �׳� ��ġ�� ���� ó��...
	if( g_bFFullMode )
	{
		m_WebPage1.Open( strUrl1, m_pBackSur->Xpos+19, m_pBackSur->Ypos+63, 357, 424 ); // â�� ����.
		m_WebPage2.Open( strUrl2, m_pBackSur->Xpos+398, m_pBackSur->Ypos+63, 382, 504 ); // â�� ����.
	}
	else
	{
		m_WebPage1.Open( strUrl1, m_pBackSur->Xpos+21, m_pBackSur->Ypos+112, 359, 426 ); // â�� ����.
		m_WebPage2.Open( strUrl2, m_pBackSur->Xpos+400, m_pBackSur->Ypos+112, 385, 504 ); // â�� ����.
	}
}

void CCashShopIntro::Close()
{
	m_WebPage1.Close();
	m_WebPage2.Close();
	m_WebPage2.DeleteRes();
	m_WebPage1.DeleteRes();
}

void CCashShopIntro::Draw() // �� �Լ��� ������ DrawItemInfo()�Լ����� ǥ�õǴ� ������ ������ �����ϱ� ���� ���δ�.
{
	if( !g_pDisplay )
		return;

	g_pDisplay->Blt( m_pBackSur->Xpos, m_pBackSur->Ypos, m_pBackSur );

	m_WebPage1.Draw(); // �̳༮�� ������ ������ ���� â�̵� �����ۿ� ����.
	m_WebPage2.Draw(); // �̳༮�� ������ ������ ���� â�̵� �����ۿ� ����.

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

		if( IsInside(x,y) ) // �������̽��� ������ Ŭ������ ���.
		{
			m_InBtn.MsgProc(hWnd, msg, wParam, lParam);
			m_CloseBtn.MsgProc(hWnd, msg, wParam, lParam);

			return 1;
		}

		return 0; // �ٸ� �������̽� �κ��� ����� ���...(�κ�,Bottom��...)

	case WM_MOUSEMOVE:
	case WM_LBUTTONUP:

		if( IsInside(x,y) ) // �������̽��� ������ Ŭ������ ���.
		{
			m_InBtn.MsgProc(hWnd, msg, wParam, lParam);
			if( m_InBtn.GetState()==BTN_ACTION ) // ����
			{
				g_pNk2DFrame->ToggleCashShopWindow( CASHSHOP_TYPE_NORMAL ); // �Ϲ� ĳ������ �������̽��� �����ش�.

				m_InBtn.SetState(BTN_NORMAL);	// ��ư�� �������ͽ��� ���� Ǯ������ϳ�����.
				return 1;
			}

			m_CloseBtn.MsgProc(hWnd, msg, wParam, lParam);
			if( m_CloseBtn.GetState()==BTN_ACTION ) // �����ݱ� ��ư Ŭ����
			{
				g_pNk2DFrame->ToggleCashShopIntroWindow( -1 ); // Toggle �Լ��� ����ؼ� ���� �����Ų��.(����� ���´ٴ°� ���ִ� ���´ϱ� ������ ������ ����)

				m_CloseBtn.SetState(BTN_NORMAL);	// ��ư�� �������ͽ��� ���� Ǯ������ϳ�����.

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

