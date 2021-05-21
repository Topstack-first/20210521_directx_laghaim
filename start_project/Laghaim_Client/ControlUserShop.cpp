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
#include "ControlUserShop.h"
#include "Country.h"
#include "PacketSend.h"

extern int g_EffectSound[];
extern int gMapWidth;
extern int gMapHeight;
extern int gMapExtention;
extern BOOL g_bUseCashShop; ///유료상점
extern DWORD			g_dwClientCountry;

extern int InsertPriceComma( char *DestString, char *SrcString, char FindChar ); // 가격 스트링들의 숫자 부에 콤마를 넣어주는 함수. ///가격표시개선
extern char *ConvertMoneyToString( char *DestString, INT64 Money, int SpaceSize, int Type = 0 ); // 돈 금액의 단위를 말로 바꿔주는 함수.


CControlUserShop::CControlUserShop()
	: m_hWnd(NULL)
	, m_pNkCha(NULL)
{
	int k = 0, x = 0, y = 0;

	for( k = 0 ; k < USHOP_PACK_NUM ; ++k )
	{
		for( x = 0 ; x < USHOP_SLOT_X_NUM ; ++x )
		{
			for( y = 0 ; y < USHOP_SLOT_Y_NUM ; ++y )
				m_pSellItem[k][x][y] = NULL;
		}
	}

	m_pBackSur = NULL;
	m_pPlzWait = NULL;
	m_pNowSelling = NULL;
	m_pSelledMoneyText = NULL;
	m_pSelledMoneyBack = NULL;
	m_pInOutTempImage = NULL;
	m_pInOutTempImage2 = NULL;
	m_States = USHOP_STATES_NOTOPEN; // 이녀석은 Init()에서하면 타이밍상 안된다.

	for( k = 0 ; k < USERSHOP_SOUND_NUM ; ++k )
	{
		m_pSound[k] = NULL;
	}

	m_pSelledPriceInfo = NULL;
	Init(); // 변수들 초기화.
}

CControlUserShop::~CControlUserShop()
{
	DeleteRes();

	SAFE_DELETE( m_pSelledPriceInfo );
	SAFE_DELETE(m_pBackSur);
	SAFE_DELETE(m_pPlzWait);
	SAFE_DELETE(m_pNowSelling);
	SAFE_DELETE(m_pSelledMoneyText);
	SAFE_DELETE(m_pSelledMoneyBack);
	SAFE_DELETE(m_pInOutTempImage);
	SAFE_DELETE(m_pInOutTempImage2);
}

void CControlUserShop::Init()
{
	m_bResLoad = FALSE;
	m_iType = -1;  // 판매자인지 구매자인지 플래그.
	m_iShopKind = -1; // 상점 종류. 0이면 미 셋팅.
	m_iPackNum = 1; // 팩의 갯수.
	m_iNowPack = 0; // 현재 선택된 팩.
	m_my_slotstart_x = 0;
	m_my_slotstart_y = 0;
	m_YesNoType = -1;
	m_TopWidth = 0; // Top 메뉴의 가로 길이. In/Out버튼이 영향을 주므로 필요.

	m_ShopNameText.SetString( _T(" "), true);     // "XXX의 개인상점"을 표기해주기 위한 판때기.
	m_ShopMentText.ClearText();     // 상점주인이 써놓은 멘트 출력용 서피스.
	m_SellMoneyText.SetString( _T(" "), true);   // 판매 금액란.

	m_bSlotLock = FALSE; // 교체시 슬롯에서 빼는 것을 방지시키기 위한 플래그.
	m_bSrvMsgWaiting = FALSE;
	ZeroMemory( &m_TempMoveInfo, sizeof(sMoveItemInfo) );
	InOutMovePlusX = 0.0f; // In/Out 애니메이션 시에 보정될 최종 X치.
	m_StatesTick = 0; // 상태가 변할때 기억해두는 틱.(In/Out 애니메이션시 사용한다.)
	m_SrvMsgWaitTick = 0; // 서버 메세지를 기다리고 있기 시작한 시점을 기억해두는 틱. 일정시간 서버메세지가 없으면 플래그를 풀어주기위해 둔다.(이렇게 풀릴경우 상점을 닫아버려야 할듯.)

	ClearSlot(); // 슬롯의 아이템들을 초기화해주는 녀석.

	m_SelledRedLime = 0; // Red Bullion
	m_SelledPlatinumLime = 0; // Platinum Bullion
	m_SelledGoldLime = 0; // 팔린 금괴 수.(팔아서 얻은 금괴수다.)
	m_SelledSilverLime = 0; // 판린 은괴 수.
	m_SelledBronzeLime = 0; // 팔린 동괴 수.
	m_SelledLime = 0; // 팔린 라임.
	m_bDrawSelledPricePopUp = FALSE;
	m_pRefTempExtraItem = NULL; // 가격 입력시 ExtraSlot의 아이템을 숨겨주기 위한 임시 참조 포인터.(가격입력중에 ExtraSlot의 포인터를 여기에 기억해두고 ExtraSlot을 NULL 셋팅한다. 가격 입력후 복구해준다.)
	// 참조 포인터 초기화.
	m_pItemRender = NULL;
	m_pOverItem = NULL;
}

void CControlUserShop::Open( int Type, int grade, char *ShopName, char *ShopMent ) // 오픈시에 해줘야 할 초기화들.(위의 Init()은 오픈시에 하면 안되는게 섞여있어서 따로 만듬.)
{
	int k = 0, x = 0, y = 0;

	m_iType = Type;  // 판매자인지 구매자인지 플래그.
	m_iPackNum = grade; // 팩의 갯수.
	m_iNowPack = 0; // 현재 선택된 팩.

	// 탭 버튼 초기화.
	m_TabBtn[0].SetState(BTN_DOWN); // 눌려있는 상태로...
	m_TabBtn[1].SetState(BTN_NORMAL);
	m_TabBtn[2].SetState(BTN_NORMAL);

	for( int i=0 ; i<m_iPackNum ; i++ )
	{
		if( m_iPackNum < i )
		{
			m_TabBtn[i].SetDisable(TRUE);
			m_TabBtn[i].SetVisible(FALSE);
		}
	}

	m_ShopNameText.SetString( ShopName, FALSE );     // "XXX의 개인상점"을 표기해주기 위한 판때기.
	m_ShopMentText.ClearText();     // 상점주인이 써놓은 멘트 출력용 서피스.
	m_ShopMentText.SetText( ShopMent );     // 상점주인이 써놓은 멘트 출력용 서피스.
	m_SellMoneyText.SetString( _T(" "), true);   // 판매 금액란.

	m_bSlotLock = TRUE; // 교체시 슬롯에서 빼는 것을 방지시키기 위한 플래그. 디폴트는 수정 불가다.
	m_bSrvMsgWaiting = FALSE;
	ZeroMemory( &m_TempMoveInfo, sizeof(sMoveItemInfo) );

	ClearSlot();

	InOutMovePlusX = 0.0f;
	m_SelledRedLime = 0; // Red Bullion
	m_SelledPlatinumLime = 0; // Platinum Bullion
	m_SelledGoldLime = 0; // 팔린 금괴 수.(팔아서 얻은 금괴수다.)
	m_SelledSilverLime = 0; // 판린 은괴 수.
	m_SelledBronzeLime = 0; // 팔린 동괴 수.
	m_SelledLime = 0; // 팔린 라임.

	// 알파 팝업도 0으로 초기화 해줘야한다.
	CMultiText multiText;
	char strTemp[256] = "";
	// Platinum Lime
	sprintf( strTemp, G_STRING(IDS_SELLED_PLATINUMLIME), m_SelledPlatinumLime );
	multiText.AddString( strTemp, RGB(255,165,0) );
	// Red Lime
	sprintf( strTemp, G_STRING(IDS_SELLED_REDLIME), m_SelledRedLime );
	multiText.AddString( strTemp, RGB(255,0,0) );
	//-- IDS_SELLED_GOLDLIME : 팔린 금괴: %d
	sprintf( strTemp, G_STRING(IDS_SELLED_GOLDLIME), m_SelledGoldLime );
	multiText.AddString( strTemp, RGB(234,236,22) );
	//-- IDS_SELLED_SILVERLIME : 팔린 은괴: %d
	sprintf( strTemp, G_STRING(IDS_SELLED_SILVERLIME), m_SelledSilverLime );
	multiText.AddString( strTemp, RGB(122,220,122) );
	//-- IDS_SELLED_BRONZELIME : 팔린 동괴: %d
	sprintf( strTemp, G_STRING(IDS_SELLED_BRONZELIME), m_SelledBronzeLime );
	multiText.AddString( strTemp, RGB(255,152,57) );
	//-- IDS_SELLED_LIME : 팔린 라임: %I64d
	sprintf( strTemp, G_STRING(IDS_SELLED_LIME), m_SelledLime );
	multiText.AddString( strTemp, RGB(255,255,255) );
	m_pSelledPriceInfo->PutString(&multiText);

	if( m_iType == USHOP_TYPE_SELLER ) // 개인상점 개설시...
	{
		m_ShopMentText.SetEnable( TRUE ); // 구매자는 멘트를 변경할수없다.(점포주인도 수정누르기 전엔 수정안된다.)
		m_bSlotLock = FALSE; // 판매자 오픈시에만 이걸 디폴트로 풀어준다.

		m_SellStartBtn.SetDisable( FALSE );
		m_ModifyBtn.SetDisable( TRUE );
	}
	else if( m_iType == USHOP_TYPE_STORE ) // 점포를 개설하려고 열었을 경우... ///점포상점
	{
		m_ShopMentText.SetEnable( TRUE ); // 구매자는 멘트를 변경할수없다.(점포주인도 수정누르기 전엔 수정안된다.)
		m_bSlotLock = FALSE; // 판매자 오픈시에만 이걸 디폴트로 풀어준다.

		m_SellStartBtn.SetDisable( FALSE );
		m_ModifyBtn.SetDisable( TRUE );
	}
	else if( m_iType == USHOP_TYPE_ADMIN ) // 점포를 수정하려고 열었을 경우... ///점포상점
	{
		m_ShopMentText.SetEnable( FALSE );
		m_bSlotLock = TRUE;

		m_SellStartBtn.SetDisable( TRUE );
		m_ModifyBtn.SetDisable( FALSE );
	}
}

void CControlUserShop::Close()
{
	if( g_pTcpIp )
		g_pTcpIp->SendNetMessage( "u_shop close\n" );

	m_bSrvMsgWaiting = TRUE;
	m_SrvMsgWaitTick = timeGetTime(); // 서버 메세지를 기다리고 있기 시작한 시점을 기억해두는 틱. 일정시간 서버메세지가 없으면 플래그를 풀어주기위해 둔다.(이렇게 풀릴경우 상점을 닫아버려야 할듯.)
}

void CControlUserShop::ClearSlot() // 슬롯의 아이템들을 초기화해주는 녀석.
{
	int k = 0, x = 0, y = 0;

	for( k = 0 ; k < USHOP_PACK_NUM ; ++k )
	{
		for( x = 0 ; x < USHOP_SLOT_X_NUM ; ++x )
		{
			for( y = 0 ; y < USHOP_SLOT_Y_NUM ; ++y )
			{
				m_SlotItemCount[k][x][y] = 0;	// 슬롯 카운트
				SAFE_DELETE( m_pSellItem[k][x][y] );
				m_CountText[k][x][y].SetString( _T(" "), true);
			}
		}
	}

	m_pOverItem = NULL; // 이거땜에 뻑나는 경우도 있으니 반드시 초기화하자.
}


void CControlUserShop::DeleteRes()
{
	int i = 0, k = 0, x = 0, y = 0;

	for( i = 0 ; i < USERSHOP_SOUND_NUM ; ++i )
		m_pSound[i] = NULL;

	m_ModifyBtn.DeleteRes();		// 수정 버튼
	m_SellStartBtn.DeleteRes();        // 판매시작 버튼
	m_CloseShopBtn.DeleteRes();        // 상점닫기 버튼
//	m_RefundBtn.DeleteRes();

	for( i = 0 ; i < USHOP_PACK_NUM ; ++i )
		m_TabBtn[i].DeleteRes();

	m_InBtn.DeleteRes();
	m_OutBtn.DeleteRes();

	m_ShopNameText.DeleteRes();     // "XXX의 개인상점"을 표기해주기 위한 판때기.
	m_ShopMentText.DeleteRes();     // 상점주인이 써놓은 멘트 출력용 서피스.
	m_SellMoneyText.DeleteRes();   // 판매 금액란.

	SAFE_SURFACE_DESTROY(m_pBackSur);
	SAFE_SURFACE_DESTROY(m_pPlzWait);
	SAFE_SURFACE_DESTROY(m_pNowSelling);
	SAFE_SURFACE_DESTROY(m_pSelledMoneyText);
	SAFE_SURFACE_DESTROY(m_pSelledMoneyBack);
	SAFE_SURFACE_DESTROY(m_pInOutTempImage);
	SAFE_SURFACE_DESTROY(m_pInOutTempImage2);

	for( k = 0 ; k < USHOP_PACK_NUM ; ++k )
	{
		for( x = 0 ; x < USHOP_SLOT_X_NUM ; ++x )
		{
			for( y = 0 ; y < USHOP_SLOT_Y_NUM ; ++y )
				m_CountText[k][x][y].DeleteRes();
		}
	}

	m_bResLoad = FALSE;
}

void CControlUserShop::LoadRes( CItemRender *pItemRender )
{
	if( !g_pDisplay || !pItemRender)
		return;

	int k = 0, x = 0, y = 0;

	if( pCMyApp && g_pNk2DFrame && g_pNk2DFrame->GetControlTop() )
	{
//		if( g_bUseCashShop ) // 태국의 경우 유료상점 버튼이 더 붙는다.
//			m_TopWidth = g_pNk2DFrame->GetControlTop()->m_pSurfaceBack->GetWidth()+g_pNk2DFrame->GetControlTop()->m_IconCashShopBtn.Width-16; // top 메뉴의 가로 길이를 구해둔다.
//		else
// 			m_TopWidth = g_pNk2DFrame->GetControlTop()->m_pSurfaceBack->GetWidth(); // top 메뉴의 가로 길이를 구해둔다.
	}

	m_pItemRender = pItemRender;

	m_hWnd =  g_pDisplay->GetHWnd();
	GetClientRect( m_hWnd, &m_ClientRc );

	if( FAILED( g_pDisplay->CreateSurfaceFromBitmap( &m_pBackSur, "interface/UserShop/usershop.bmp" ) ) )
		return;
	m_pBackSur->SetColorKey( TRANS_COLOR );

	g_pDisplay->CreateSurfaceFromBitmap( &m_pPlzWait, "interface/UserShop/PlzWait.bmp" );
	g_pDisplay->CreateSurfaceFromBitmap( &m_pNowSelling, "interface/UserShop/NowSelling.bmp" );

	g_pDisplay->CreateSurfaceFromBitmap( &m_pSelledMoneyText, "interface/UserShop/SelledMoneyText.bmp" ); // 판매 금액 글자.(구매자의 경우 안출력해야함으로. 따로 가지고 있는다.
	g_pDisplay->CreateSurfaceFromBitmap( &m_pSelledMoneyBack, "interface/UserShop/SelledMoneyBack.bmp" ); // 판매 금액 표시용 패경
	m_pSelledMoneyBack->SetWidth( 85 ); // 위 CreateSurface에서 CSurface 에 따로 저장해두는 m_dwWidth,m_dwHeight에 값을 저장안해주는 버그가 있어서 그냥 직접 셋팅.
	m_pSelledMoneyBack->SetHeight( 19 );

	m_ModifyBtn.SetFileName( "UserShop/btn_modify" );		// 수정 버튼
	m_SellStartBtn.SetFileName( "UserShop/btn_start" );        // 판매시작 버튼
	m_CloseShopBtn.SetFileName( "common/btn_close_01" );        // 상점닫기 버튼
//	m_RefundBtn.SetFileName( "UserShop/btn_rime" );
	m_ModifyBtn.LoadRes();
	m_SellStartBtn.LoadRes();
	m_CloseShopBtn.LoadRes();
	//m_RefundBtn.LoadRes();

	// 탭버튼 로딩.
	m_TabBtn[0].SetFileName("common/btn_tab_01");
	m_TabBtn[1].SetFileName("common/btn_tab_02");
	m_TabBtn[2].SetFileName("common/btn_tab_03");	// 버튼맴버의 이름만 셋팅
	m_TabBtn[0].LoadRes();
	m_TabBtn[1].LoadRes();
	m_TabBtn[2].LoadRes();
	m_TabBtn[0].SetState(BTN_DOWN); // 처음껀 눌려있도록 설정.

	m_InBtn.SetFileName( "UserShop/btn_In" );
	m_InBtn.LoadRes();
	m_OutBtn.SetFileName( "UserShop/btn_Out" );
	m_OutBtn.LoadRes();

	m_ShopNameText.Init( 12, RGB(255,255,0), 0, 0 );     // "XXX의 개인상점"을 표기해주기 위한 판때기.
	//	m_ShopMentText.Init( 0, 0, 17*2, RGB(255, 255, 255), RGB(255, 255, 0) );
	m_ShopMentText.Init( 0, 0, 25, RGB(255, 255, 255), RGB(255, 255, 0) ); // 길이 조절 ///07-04-10
	m_ShopMentText.SetfontSize(); // 디폴트 폰트 사이즈로 초기화.(이거 안하면 다른게 초기화 안되서 안된다.)
	m_SellMoneyText.Init( 12, RGB(255,255,0), 0, 0 );   // 판매 금액란.

	for( k = 0 ; k < USHOP_PACK_NUM ; ++k )
	{
		for( x = 0 ; x < USHOP_SLOT_X_NUM ; ++x )
		{
			for( y = 0 ; y < USHOP_SLOT_Y_NUM ; ++y )
				m_CountText[k][x][y].Init( 20, RGB(255, 255, 0), 0, 0 );
		}
	}

	m_pBackSur->Xpos = g_pNk2DFrame->GetClientWidth() - m_pBackSur->GetWidth() - 291;
	m_pBackSur->Ypos = 0;

	m_ShopNameText.SetPos( m_pBackSur->Xpos+55, m_pBackSur->Ypos+44 );
	m_ShopMentText.SetPos( m_pBackSur->Xpos+31, m_pBackSur->Ypos+77 );

	m_TabBtn[0].SetPosition(m_pBackSur->Xpos+20, m_pBackSur->Ypos+109 );
	m_TabBtn[1].SetPosition(m_pBackSur->Xpos+99, m_pBackSur->Ypos+109 );
	m_TabBtn[2].SetPosition(m_pBackSur->Xpos+178, m_pBackSur->Ypos+109 );

	m_my_slotstart_x = m_pBackSur->Xpos + 30;
	m_my_slotstart_y = m_pBackSur->Ypos + 155;

	m_pPlzWait->Xpos = m_my_slotstart_x + 53;
	m_pPlzWait->Ypos = m_my_slotstart_y + 100;
	m_pNowSelling->Xpos = m_my_slotstart_x + 7;
	m_pNowSelling->Ypos = m_my_slotstart_y + 100;

	m_pSelledMoneyText->Xpos = m_pBackSur->Xpos + 41;
	m_pSelledMoneyText->Ypos = m_pBackSur->Ypos + 137;
	m_pSelledMoneyBack->Xpos = m_pBackSur->Xpos + 89;
	m_pSelledMoneyBack->Ypos = m_pBackSur->Ypos + 136;
	m_SellMoneyText.SetPos( m_pSelledMoneyBack->Xpos+5, m_pSelledMoneyBack->Ypos+2 );

	m_SellStartBtn.SetPosition( m_pBackSur->Xpos+95, m_pBackSur->Ypos+373 );
	m_ModifyBtn.SetPosition( m_pBackSur->Xpos+161, m_pBackSur->Ypos+373 );
//	m_RefundBtn.SetPosition( m_pBackSur->Xpos+ 65, m_pBackSur->Ypos+375 );
	m_CloseShopBtn.SetPosition( m_pBackSur->Xpos+257, m_pBackSur->Ypos+4 );

	m_InBtn.SetPosition( m_pBackSur->Xpos-m_InBtn.Width, m_pBackSur->Ypos );
	m_OutBtn.SetPosition( m_pBackSur->Xpos-m_OutBtn.Width-m_TopWidth, m_pBackSur->Ypos );

	g_pDisplay->CreateSurface( &m_pInOutTempImage, 546, 419 ); // In/Out시에 찍어줄 짝퉁 인벤 이미지. 크기는 해상도별 인벤 인터페이스 크기
	m_pInOutTempImage->SetWidth( 546 ); // 위 CreateSurface에서 CSurface 에 따로 저장해두는 m_dwWidth,m_dwHeight에 값을 저장안해주는 버그가 있어서 그냥 직접 셋팅.
	m_pInOutTempImage->SetHeight( 419 );

	g_pDisplay->CreateSurface( &m_pInOutTempImage2, g_pNk2DFrame->GetClientWidth(), g_pNk2DFrame->GetClientHeight()); // In/Out시에 찍어줄 짝퉁 인벤 이미지. 크기는 해상도별 인벤 인터페이스 크기
	m_pInOutTempImage2->SetWidth( g_pNk2DFrame->GetClientWidth() ); // 위 CreateSurface에서 CSurface 에 따로 저장해두는 m_dwWidth,m_dwHeight에 값을 저장안해주는 버그가 있어서 그냥 직접 셋팅.
	m_pInOutTempImage2->SetHeight( g_pNk2DFrame->GetClientHeight() );

	if( g_pDSound )
	{
		m_pSound[USERSHOP_SOUND_IN] = g_pDSound->LoadWavToOutside( "data/sound/down.wav" );
		m_pSound[USERSHOP_SOUND_OUT] = g_pDSound->LoadWavToOutside( "data/sound/boom4.wav" );
	}

	m_pSelledPriceInfo = new CAlphaPopUp;
	if( m_pSelledPriceInfo )
	{
		m_pSelledPriceInfo->Init(14);
		m_pSelledPriceInfo->LoadRes( m_ClientRc.right, m_ClientRc.bottom );
	}

	m_bResLoad = TRUE; // 임시 주석 처리
}

HRESULT CControlUserShop::RestoreSurfaces()
{
	// 무식하게 다 지웠다가 로딩.
	DeleteRes();

	LoadRes( m_pItemRender );

	return S_OK;
}

CItem* CControlUserShop::Draw()
{
	if( !g_pDisplay )
		return NULL;

	if( m_States != USHOP_STATES_NORMAL ) // 일반적인 출력상태가 아니면...인 아웃시의 들어가고 나오고 움직임이 표현이 힘드므로 얍삽하게 중간에 출력을 가로채서한다.
	{
		// 실제의 것을 움직이는 것이 아니라 스캔 샷을 움직여주는 방식이다.
		RECT TempRect;
		int TempX = 0, TempY = 0; // 좌표 보정을 위해 임시로 기억해두기 위한 변수.

		DWORD ElapsedTick = timeGetTime()-m_StatesTick;

		if( m_States == USHOP_STATES_HIDING_IN ) // 들어가고 있는 중.
		{
			float fPower = 8.0f; // 미는힘.
			float fRevPower = 14.0f - ( (float)(ElapsedTick) * 0.001f * 25.0f ); // 저항력. 시간에 비례하여 약해진다.(나중에는 오히려 속력에 더해진다.)

			if( fRevPower > 0.0f ) // 저항이 남아있을때...
			{
				fRevPower += rand()%34-20; // 아주 약간의 랜덤 보정을 준다.
			}

			InOutMovePlusX += (fPower - fRevPower)*(float)(ElapsedTick) * 0.001f; // 미는 힘에서 줄어드는 저항력을 뺀다.

			if( InOutMovePlusX < 0.0f )
				InOutMovePlusX = 0.0f;

			if( InOutMovePlusX > 546.0f ) // 넘었으면...
			{
				InOutMovePlusX = 546.0f;

				m_States = USHOP_STATES_HIDE; // 스테이츠를 고정.
			}

			TempRect.left = 0;
			TempRect.right = m_pInOutTempImage->GetWidth()-(int)InOutMovePlusX;
			TempRect.top = 0;
			TempRect.bottom = m_pInOutTempImage->GetHeight();

			g_pDisplay->Blt( m_pInOutTempImage->Xpos+(int)InOutMovePlusX, m_pInOutTempImage->Ypos, m_pInOutTempImage, &TempRect );

			if( InOutMovePlusX < m_TopWidth )
			{
				TempX = m_InBtn.PosX;
				TempY = m_InBtn.PosY;
				m_InBtn.Draw( TempX+(int)InOutMovePlusX, TempY );
				m_InBtn.PosX = TempX;
				m_InBtn.PosY = TempY;
			}
			else // 일정 이상 넘어가면 아웃버튼으로 출력.
			{
				m_OutBtn.Draw();
			}


			if( pCMyApp && g_pNk2DFrame && g_pNk2DFrame->GetControlTop() )
				g_pNk2DFrame->GetControlTop()->Draw(); // Top 인터페이스는 임시로 직접 출력.(왜냐면 가라 이미지에 가려지므로...)

		}
		else if( m_States == USHOP_STATES_HIDING_OUT || m_States == USHOP_STATES_HIDING_OUT2 ) // 나오고 있는 중. 또는 // 쿵.하고 흔들리는 부분.
		{
			if( m_States == USHOP_STATES_HIDING_OUT2 ) // 후반의 쿠궁하며 흔들리는 부분이다.
			{
				if( ElapsedTick > 300 ) // 일정 시간이상 흔들렸으면
				{
					InOutMovePlusX = 0.0f;

					m_States = USHOP_STATES_HIDE; // 이걸로 임시로 바꿔준다. 이유는 밑의 CloseAllWindow()를 해주기위해서.(m_States가 HIDE면 CloseAllWindow()로 Inven을 끌수있다)
					g_pNk2DFrame->ShowInvenWindow( TRUE ); // 인벤창을 켜준다.
					m_States = USHOP_STATES_NORMAL; // 스테이츠를 고정.
				}
				else
				{
					if( ( ElapsedTick / 70 ) % 2 )
						InOutMovePlusX += (float)(rand()%20)* 0.5f * ((float)ElapsedTick*0.001f);
					else
						InOutMovePlusX -= (float)(rand()%20)* 0.5f * ((float)ElapsedTick*0.001f);
				}
			}
			else // 나오고 있는 중.
			{
				float fPower = 4.0f; // 미는힘.
				float fRevPower = 10.0f - ( (float)(ElapsedTick) * 0.001f * 30.0f ); // 저항력. 시간에 비례하여 약해진다.(나중에는 오히려 속력에 더해진다.)

				InOutMovePlusX -= (fPower - fRevPower)*(float)(ElapsedTick) * 0.001f; // 미는 힘에서 줄어드는 저항력을 뺀다.

				if( InOutMovePlusX > 546.0f ) // 넘었으면...
					InOutMovePlusX = 546.0f;

				if( InOutMovePlusX < 0.0f ) // 넘었으면...
				{
					InOutMovePlusX = 0.0f; // 흔들리더라도 0.0을 기준으로 하도록.

					m_States = USHOP_STATES_HIDING_OUT2; // 이걸로 임시로 바꿔준다. 이유는 밑의 CloseAllWindow()를 해주기위해서.(m_States가 HIDE면 CloseAllWindow()로 Inven을 끌수있다)

					m_StatesTick = timeGetTime(); // 시간을 갱신. 쿠궁의 계산을 편하게 하기위해서.

					if( g_pDSound && m_pSound[USERSHOP_SOUND_IN] )
						g_pDSound->PlayToOutside( m_pSound[USERSHOP_SOUND_OUT] ); // 효과음을 출력해준다.
				}
			}

			TempRect.left = 0;
			if( InOutMovePlusX > 0.0f )
				TempRect.right = m_pInOutTempImage->GetWidth()-(int)InOutMovePlusX;
			else
				TempRect.right = m_pInOutTempImage->GetWidth();
			TempRect.top = 0;
			TempRect.bottom = m_pInOutTempImage->GetHeight();

			g_pDisplay->Blt( m_pInOutTempImage->Xpos+(int)InOutMovePlusX, m_pInOutTempImage->Ypos, m_pInOutTempImage, &TempRect );

			if( InOutMovePlusX < m_TopWidth )
			{
				TempX = m_OutBtn.PosX;
				TempY = m_OutBtn.PosY;
				m_OutBtn.Draw( m_pBackSur->Xpos+(int)InOutMovePlusX-m_OutBtn.Width, TempY );
//				m_OutBtn.Draw( TempX-(int)InOutMovePlusX, TempY );
				m_OutBtn.PosX = TempX;
				m_OutBtn.PosY = TempY;
			}
			else // 일정 이상 넘어가면 고정도니 위치에.
				m_OutBtn.Draw();

			if( pCMyApp && g_pNk2DFrame && g_pNk2DFrame->GetControlTop() )
				g_pNk2DFrame->GetControlTop()->Draw(); // Top 인터페이스는 임시로 직접 출력.(왜냐면 가라 이미지에 가려지므로...)
		}
		else if( m_States == USHOP_STATES_HIDE ) // 숨겨져 있는 중.
		{
			if( !g_pNk2DFrame->CheckFullScreenInterface() )
				m_OutBtn.Draw();
		}

		return NULL;
	}

	int k = 0, x = 0, y = 0;
	int xpos = 0, ypos = 0; // 카운트 출력용 변수

	g_pDisplay->Blt( m_pBackSur->Xpos, m_pBackSur->Ypos, m_pBackSur );
	DrawButtons(); // 버튼은 타입따라 바뀌어야 함으로 함수로 처리.
	DrawItemSlot(); // 슬롯을 그린다.

	// 아이템 카운트 출력
	for( x = 0 ; x < USHOP_SLOT_X_NUM ; ++x )
	{
		for( y = 0 ; y < USHOP_SLOT_Y_NUM ; ++y )
		{
			if( m_SlotItemCount[m_iNowPack][x][y] > 0 ) // 해당아이템이 있을때만 출력.
			{
				if( m_pSellItem[m_iNowPack][x][y] )
				{
					xpos = m_my_slotstart_x + (m_pSellItem[m_iNowPack][x][y]->m_SlotX*USHOP_SLOT_WIDTH);
					ypos = m_my_slotstart_y + (m_pSellItem[m_iNowPack][x][y]->m_SlotY*USHOP_SLOT_HEIGHT);
				}
				m_CountText[m_iNowPack][x][y].Draw(xpos, ypos);
			}
		}
	}

	// 상점 이름과 멘트 출력.
	m_ShopNameText.Draw();
	m_ShopMentText.Draw();

	if( m_iType != USHOP_TYPE_BUYER ) // 파는 사람만 출력할 녀석.
	{
		// 판매 금액 출력.
		g_pDisplay->Blt( m_pBackSur->Xpos + 31 , m_pBackSur->Ypos + 137 , m_pSelledMoneyText );
		g_pDisplay->Blt( m_pBackSur->Xpos + 89 , m_pBackSur->Ypos + 136 , m_pSelledMoneyBack );

		m_SellMoneyText.Draw();

		if( m_bDrawSelledPricePopUp )
		{
			POINT point;
			GetCursorPos(&point);
			ScreenToClient(g_pDisplay->GetHWnd(), &point);

			m_pSelledPriceInfo->Draw(point.x,point.y); // 좌표 수정
		}

	}

	if( m_iType != USHOP_TYPE_BUYER && m_bSlotLock && !m_bSrvMsgWaiting ) // 판매 중엔 가운데에 메세지 표시... // 깜빡여준다.
	{
		if( (timeGetTime()%1500) > 450 )
			g_pDisplay->Blt( m_pNowSelling->Xpos, m_pNowSelling->Ypos, m_pNowSelling );
	}

	pCMyApp->m_pMouse->SetMouseType( M_NORMAL );

	if( !g_pNk2DFrame->m_Popup ) // 메세지 팝업이 안떠있을때만 아이템 정보와 커서변환을 하자.
	{
		if( pCMyApp && pCMyApp->m_pMouse )
		{
			if( m_pOverItem ) // 가리키고있는 아이템이 있다면.
				pCMyApp->m_pMouse->SetMouseType( M_HOLD );
		}
	}
	else // 메시지 팝업이 떠있고, 웨이팅 메세지가 출력되는 중이 아니라면...
		return NULL; // 아이템 정보를 출력안한다.

	if( m_bSrvMsgWaiting ) // 서버 메세지를 기다리는 중엔 가운데에 메세지 표시... // 깜빡여준다.
	{
		if( (timeGetTime()%800) > 400 )
			g_pDisplay->Blt( m_pPlzWait->Xpos, m_pPlzWait->Ypos, m_pPlzWait );
	}

	return m_pOverItem; // 이 리턴을 하는 이유는 DrawItemInfo()함수에서 표시되는 아이템 정보를 갱신해서 표시해주기 위함인듯...
}

void CControlUserShop::DrawButtons() // 상황에 따라 달라지는 버튼들을 구분하여 출력.
{
	int i = 0;

	// 탭 버튼 출력. 상점 종류. 현재 탭 번호에 따라 갈린다.
	if( m_iNowPack >= 0 && m_iNowPack < USHOP_PACK_NUM && m_iPackNum > 0 && m_iPackNum <= USHOP_PACK_NUM ) // 범위가 정상일때만 출력
	{
		for( i = 0 ; i < m_iPackNum ; ++i )
		{
			if( i != m_iNowPack ) // 눌리지 않은 녀석만 우선 출력
				m_TabBtn[i].Draw();
		}

		m_TabBtn[m_iNowPack].Draw();
	}

	if( m_iType != USHOP_TYPE_BUYER ) // 파는 사람만 출력할 녀석.
	{
		m_SellStartBtn.Draw( m_pBackSur->Xpos + 91, m_pBackSur->Ypos + 407, m_SellStartBtn.m_bDisable ); // m_SellStartBtn.PosX, m_SellStartBtn.PosY, m_SellStartBtn.m_bDisable ); // 디스에이블 체크따라 비활성으로 출력할 필요가 있음.
		m_ModifyBtn.Draw( m_pBackSur->Xpos + 199, m_pBackSur->Ypos + 407, m_ModifyBtn.m_bDisable ); // 디스에이블 체크따라 비활성으로 출력할 필요가 있음.
//		m_RefundBtn.Draw( m_pBackSur->Xpos +  32, m_pBackSur->Ypos + 409 );
	}

	m_CloseShopBtn.Draw();

	m_InBtn.Draw();
}

void CControlUserShop::DrawItemSlot()
{
	if( !g_pDisplay )
		return;

	int k = 0, x = 0, y = 0;

	// 아이템이 위치한 슬롯 표시
	CItem *pItem = NULL;

	if( m_pItemRender )
	{
		m_pItemRender->BeginRenderLolo();
		for( x = 0 ; x < USHOP_SLOT_X_NUM ; ++x )
		{
			for( y = 0 ; y < USHOP_SLOT_Y_NUM ; ++y )
			{
				if( m_SlotItemCount[m_iNowPack][x][y] > 0 ) // 해당 슬롯의 아이템 갯수가 0이 아니면...
				{
					pItem = m_pSellItem[m_iNowPack][x][y];
					DWORD ambient = 0x00555555;

					if (g_pNk2DFrame->IsScroll(pItem)
							|| pItem->m_Vnum == 1251
							|| pItem->m_Vnum == 1308 // 이판사판 행운권 밝기 조절 05-10-05 원석
							|| pItem->m_Vnum == 1286
							|| ( pItem->m_Vnum >= 1598 && pItem->m_Vnum <= 1695 ) // 브라질 요청 남미국기망토 종류도 성조기에서 응용이므로 예외처리.
							|| pItem->m_Vnum == 1287 ) // 미국 성조기 망토, 성조기 날개도 어둡게 보여서 클라이언트에서 직접 밝게 처리.(by 원석)
						ambient = 0x00cccccc;
					else if (pItem->m_PlusNum > 0)
						ambient = UPGRADE_ITEM_COLOR;

					int w = (GET_ITEM_WIDTH(pItem) * SLOT_WIDTH - ITEM_RENDER_WIDTH) /2;
					g_pNk2DFrame->RenderItemInUI(m_my_slotstart_x + x *USHOP_SLOT_WIDTH
												 , m_my_slotstart_y + y *USHOP_SLOT_HEIGHT + (USHOP_SLOT_HEIGHT - ITEM_RENDER_HEIGHT) /2,
												 pItem , FALSE , FALSE , TRUE , TRUE );
				}
			}
		}
		m_pItemRender->EndRenderLolo();
	}
}

LRESULT CControlUserShop::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static char commOutBuf[512];
	char strTemp[512];
	int i = 0;

	if( m_bSrvMsgWaiting ) // 가격입력중이거나 메세지 기다리는 중엔 딴 짓 못하도록 처리.
	{
		// 만약 m_bSrvMsgWaiting 가 너무 오래 지속되었다면 상점을 강제 종료시킨다.
		if( m_bSrvMsgWaiting && timeGetTime()-m_SrvMsgWaitTick > 10000 ) // 10초 동안 메세지가 안왔다면...
		{
			Close();

			m_bSrvMsgWaiting = FALSE;
		}

		return 1;
	}

	int x = LOWORD( lParam );
	int y = HIWORD( lParam );

	m_InBtn.MsgProc(hWnd, msg, wParam, lParam); // In/Out버튼도 Move는 여기서 한번 더 거쳐줘야한다.
	m_OutBtn.MsgProc(hWnd, msg, wParam, lParam);

	if( msg == WM_LBUTTONDOWN && IsPlayArea(x,y) && !IsInOutBtnInside(x,y) ) ///점포상점 시 수정
	{
		if( m_iType == USHOP_TYPE_BUYER )// 예외적으로 구매자가 땅클릭은 닫아 줌..
		{
			Close();
			return 0;
		}
		else if( m_iType == USHOP_TYPE_ADMIN ) ///점포상점 수정모드도 땅클릭 닫기 추가(얘는 인터페이스만 닫는다.)
		{
			g_pNk2DFrame->ShowUserShopWindow( FALSE ); // 인터페이스를 닫아준다. 돌아다닐수있도록.

			if( m_bSlotLock ) // 판매중에 땅클릭은 뷰 모드 닫기
			{
				sprintf( commOutBuf, "u_shop m_close\n" ); // 인터페이스를 닫았다고 서버에 알려주기 위한 패킷.

				if( g_pTcpIp )
					g_pTcpIp->SendNetMessage( commOutBuf );
			}
			else // 수정을 누른후 땅클릭은 완전 닫기
			{
				Close();
			}

			return 0;
		}
	}

	if( IsInOutBtnInside(x,y) ) // In/Out 버튼은 예외적 처리
	{
		// In/Out 버튼 처리 ( 인터페이스 영역 밖에 있음에 유의.
		if( m_States == USHOP_STATES_NORMAL ) // In 처리
		{
			if( m_InBtn.GetState()==BTN_ACTION ) // 버튼 클릭시
			{
				m_pInOutTempImage2->GetDDrawSurface()->Blt( NULL, g_pDisplay->GetBackBuffer(), NULL, 0, NULL ); // 백서피스의 사본을 만들어둔다.
				g_pDisplay->Clear( 0 ); // 깔끔히 지워준다음에...
				Draw(); // 개인상점을 그리고.
				if( pCMyApp && g_pNk2DFrame && g_pNk2DFrame->GetControlInven() )
				{
					g_pNk2DFrame->GetControlInven()->Draw(); // 인벤도 그리고
				}


				// 이동중에 사용할 짝퉁 인벤 이미지를 pDis로부터 얻어온다.
				RECT TempRect;
				TempRect.left = m_pBackSur->Xpos;
				TempRect.right = TempRect.left + m_pInOutTempImage->GetWidth();
				TempRect.top = m_pBackSur->Ypos;
				TempRect.bottom = TempRect.top + m_pInOutTempImage->GetHeight();

				m_pInOutTempImage->GetDDrawSurface()->Blt( NULL, g_pDisplay->GetBackBuffer(), &TempRect, 0, NULL );

				m_pInOutTempImage->Xpos = m_pBackSur->Xpos;
				m_pInOutTempImage->Ypos = m_pBackSur->Ypos;

				// 다 잘랐으면 복원.
				g_pDisplay->Blt( 0, 0, m_pInOutTempImage2 );



				m_States = USHOP_STATES_HIDE; // 이걸로 임시로 바꿔준다. 이유는 밑의 CloseAllWindow()를 해주기위해서.(m_States가 HIDE면 CloseAllWindow()로 Inven을 끌수있다)
				g_pNk2DFrame->CloseAllWindow(); // Inven 윈도우를 끄기위한 녀석. Inven은 실질적으로 끈상태에서 더미 서피스를 찍어 들어가고 나오는 중에 표시되게 한다. (m_States가 Normal이 될때 다시 켜준다.)
				m_States = USHOP_STATES_HIDING_IN;

				m_StatesTick = timeGetTime(); // 상태가 변한 시간을 기억. //애니에 사용된다.

				m_InBtn.SetState(BTN_NORMAL);	// 버튼의 스테이터스를 직접 풀어줘야하나보다.

				if( g_pDSound && m_pSound[USERSHOP_SOUND_IN] )
				{
					g_pDSound->PlayToOutside( m_pSound[USERSHOP_SOUND_IN] ); // 효과음을 출력해준다.
				}

				return 1; // In/Out 버튼 위에 클릭시엔 다른 프로시져 무시.
			}
		}
		else if( m_States == USHOP_STATES_HIDE ) // Out 처리
		{
			if( m_OutBtn.GetState()==BTN_ACTION ) // 버튼 클릭시
			{
				m_States = USHOP_STATES_HIDING_OUT;
				m_StatesTick = timeGetTime(); // 상태가 변한 시간을 기억. //애니에 사용된다.

				m_OutBtn.SetState(BTN_NORMAL);	// 버튼의 스테이터스를 직접 풀어줘야하나보다.

				if( g_pDSound && m_pSound[USERSHOP_SOUND_IN] )
					g_pDSound->PlayToOutside( m_pSound[USERSHOP_SOUND_IN] ); // 효과음을 출력해준다.

				return 1; // In/Out 버튼 위에 클릭시엔 다른 프로시져 무시.
			}
		}
	}

	if( m_States != USHOP_STATES_NORMAL ) // 일반 스테이터스가 아니면 다른 부분의 프로시져는 무시.
		return 0;

	switch (msg)
	{
	case WM_RBUTTONDBLCLK: // 다중 이동.(인벤쪽에서 더블클릭한것도 여기로 들어온다.)

		if( IsSlotInside(x,y) ) // 개인 상점쪽 슬롯을 더블클릭한 경우..
		{
			int slot_x_num, slot_y_num, slot_index;

			slot_x_num = (x - m_my_slotstart_x) / USHOP_SLOT_WIDTH;
			slot_y_num = (y - m_my_slotstart_y) / USHOP_SLOT_HEIGHT;
			slot_index = (m_iNowPack*USHOP_SLOT_X_NUM*USHOP_SLOT_Y_NUM) + slot_x_num * USHOP_SLOT_Y_NUM + slot_y_num;

			if( !m_SlotItemCount[m_iNowPack][slot_x_num][slot_y_num] ) // 아무것도 안들어있는 슬롯이라면...
				return 1; // 무시

			if( m_iType == USHOP_TYPE_BUYER ) // 구매자 더블클릭. 다중 구매.
			{
				// 여기서 패킷을 보내는게 아니라 여기서 확인창을 띄워야 한다. 확인시 필요한 정보를 미리 기억해둔다.
				m_TempMoveInfo.SlotIndex = slot_index; // 슬롯의 x y를 일차원 배열 인덱스로 변환한 녀석. // 서버에 보낼 인덱스는 여기서 미리 기억해둔다.
				m_TempMoveInfo.InvenPack = g_pRoh->m_CurPackNum; // 넣을 인벤. 현재 열려있는 인벤이다.
				m_TempMoveInfo.Count = 10; // 다중일경우 카운트는 무조건 10

				// 사는 건 엑스트라 슬롯과 상관없이 클릭하면 물어보고 확인누르면 인벤에 바로 들어온다. 다중도 같은 원리로...
				int TempCount = 0; // 다중은 갯수 직접 세준다.
				if( m_SlotItemCount[m_iNowPack][slot_x_num][slot_y_num] < 10 )
					TempCount = m_SlotItemCount[m_iNowPack][slot_x_num][slot_y_num];
				else
					TempCount = 10;

				UINT64 TotalLime = m_pSellItem[m_iNowPack][slot_x_num][slot_y_num]->m_ExPrice * TempCount; // 같이 커질수있으니 8바이트를 쓴다.
				//-- IDS_USHOP_BUY_CHECK : [%s] [%d]개 금괴:[%d] 은괴:[%d] 동괴:[%d] 라임:[%I64d] 구매?
				sprintf( strTemp, (char*)G_STRING(IDS_USHOP_BUY_CHECK)
						 , GET_ITEM_NAME_VNUM(m_pSellItem[m_iNowPack][slot_x_num][slot_y_num]->m_Vnum), TempCount
						 , m_pSellItem[m_iNowPack][slot_x_num][slot_y_num]->m_NeedPlatinumLime * TempCount
						 , m_pSellItem[m_iNowPack][slot_x_num][slot_y_num]->m_NeedRedLime * TempCount
						 , m_pSellItem[m_iNowPack][slot_x_num][slot_y_num]->m_NeedGoldLime * TempCount
						 , m_pSellItem[m_iNowPack][slot_x_num][slot_y_num]->m_NeedSilverLime * TempCount
						 , m_pSellItem[m_iNowPack][slot_x_num][slot_y_num]->m_NeedBronzeLime * TempCount, TotalLime ); // 추후 자세하게 정보를 출력해주자. 무슨 아이템을 몇개 얼마에....

				g_pNk2DFrame->InsertPopup(strTemp,TYPE_NOR_OKCANCLE, 6,strlen(strTemp)/2);
				m_YesNoType = 1;
			}
			else if( !m_bSlotLock ) // 판매자.(점포주인인 경우 포함) // 다중 등록 취소. 수정가능 모드인지 체크해준다.
			{
				sprintf( commOutBuf, "u_shop unreg %d %d %d\n", slot_index, g_pRoh->m_CurPackNum, 10 ); // 다중일경우 카운트는 무조건 10
				if( g_pTcpIp )
					g_pTcpIp->SendNetMessage( commOutBuf );

				m_bSrvMsgWaiting = TRUE;
				m_SrvMsgWaitTick = timeGetTime(); // 서버 메세지를 기다리고 있기 시작한 시점을 기억해두는 틱. 일정시간 서버메세지가 없으면 플래그를 풀어주기위해 둔다.(이렇게 풀릴경우 상점을 닫아버려야 할듯.)
			}
		}
		else if( m_iType != USHOP_TYPE_BUYER ) // 개인상점 슬롯이 아니면 인벤을 체크해본다. // 단, 구매자는 무시.
		{
			// 다중 등록의 경우 클라이언트에선 최소한의 조건 체크만 하고 모두 서버에서 체크해주기로 함. (조건에 실패할 경우도 누른건 제대로 눌렀다면 return 1을해서 다른 메세지를 무시시킨다.)
			CControlInven *pRefInven = NULL; // 인벤의 참조 포인터.

			if( pCMyApp && g_pNk2DFrame && g_pNk2DFrame->GetControlInven() )
			{
				pRefInven = g_pNk2DFrame->GetControlInven(); // 참조 포인터를 받아오고...
			}

			if( pRefInven && pRefInven->IsInside(x, y) && !m_bSlotLock ) // 인벤쪽 슬롯을 더블클릭한 경우.
			{
				//===========================================================
				// 클릭된 위치의 인번 인덱스 검사
				int slot_x_num, slot_y_num;
				slot_x_num = (x - pRefInven->m_slotstart_x ) / SLOT_WIDTH;		// 현재 클릭 위치
				slot_y_num = (y - pRefInven->m_slotstart_y ) / SLOT_HEIGHT;		// 현재 클릭 위치

				// 선택된 아이템의 인덱스 추출
				int index = g_pRoh->GetSlotIndex(slot_x_num, slot_y_num);
				if( index < 0 )
					return 1;

				CItem* pItem = g_pRoh->FindItemByIndexFromAll( index );
				if( (GET_ATT_FLAG(pItem) & IATT_NO_EXCHANGE) || pItem->m_bTimeLimit  // 교환 불가 아이템은 못팔도록.
						|| pItem->m_Vnum == 1336 || pItem->m_Vnum == 1337 || pItem->m_Vnum == 1338  || pItem->m_Vnum == 10021 || pItem->m_Vnum == 10022 // Add Red Bullion and Platinum Bullion
						|| pItem->m_Vnum == 1510 || pItem->m_Vnum == 1511 || (g_dwClientCountry == CTRY_KOR&&pItem->m_Vnum == 1519) ) // 태국 발렌타인 데이 쵸코는 개인상점에선 못팔도록...사랑을 돈으로 사지 말란다... // 1519 경험치 아이템의 경우 한국과 해외의 속성 관리가 꼬여서 한국만 교환불가를 예외로 적용시킨다.
				{
					if( pCMyApp && pCMyApp->m_pUIMgr ) // 에러메세지 출력해준다.
					{
						//-- IDS_ITEM_CANT_SELL : 해당 아이템은 판매가 불가능합니다.
						pCMyApp->m_pUIMgr->CmdErrorMsg( (char*)G_STRING(IDS_ITEM_CANT_SELL) );
					}

					return 1;
				}

				if( pRefInven->m_bInvenLock ) // 인벤에 연속 클릭을 제한하는 락이 걸려있다면...
				{
					DWORD curTime = timeGetTime();

					if (curTime > pRefInven->m_timeInvenLock + 1500) // 1.5초가 지났으면
						pRefInven->m_bInvenLock = FALSE;
					else
						return 1;
				}

				// 여기서 패킷을 보내는게 아니라 여기서 가격 묻는 창을 띄워야 한다.
				m_TempMoveInfo.SlotIndex = 0; // 다중의 경우 슬롯 인덱스를 0으로 보내준다. 실제 들어갈 곳은 서버에서 계산해준다.
				m_TempMoveInfo.InvenPack = g_pRoh->m_CurPackNum;
				m_TempMoveInfo.InvenX[0] = slot_x_num; // 인벤 슬롯의 위치.
				m_TempMoveInfo.InvenY[0] = slot_y_num;
				m_TempMoveInfo.Count = 0; // 수량도 안쓰인다. 서버에서 알아서 계산해줌. 이건 그냥 초기화 일뿐.

				// 등록 확인시 아이템 명을 출력하기 위해 참조포인터를 기억해둔다.(이경우 나중에 Extra슬롯에 복구는 안해야함에 유의.)
				if( pItem )
					m_pRefTempExtraItem = pItem;

				//-- IDS_USHOP_INPUT_ITEM_1PRICE : 판매하실 아이템의 개당 가격을 입력하세요.
				g_pNk2DFrame->InsertPopup("."/* (char*)G_STRING(IDS_USHOP_INPUT_ITEM_1PRICE)*/, TYPE_EXCHANGE_EX, 18 );
				pRefInven->SetInvenLock( TRUE ); // 새롭게 인벤락을 걸어둔다.(1.5초동안 명령을 무시해준다.)

				return 1;
			}
		}

		return 0;

		break;

	case WM_LBUTTONDBLCLK:
	case WM_LBUTTONDOWN:

		if( IsInside(x,y) ) // 인터페이스의 안쪽을 클릭했을 경우.
		{
			if( m_bSlotLock == FALSE ) // 수정가능한 모드일때만...
				m_ShopMentText.MsgProc(hWnd, msg, wParam, lParam);

			// 슬롯 이벤트 체크 (다운시에 처리한다.)
			if( IsSlotInside(x,y) )
			{
				int slot_x_num, slot_y_num, slot_index;
				int vnum;

				slot_x_num = (x - m_my_slotstart_x) / USHOP_SLOT_WIDTH;
				slot_y_num = (y - m_my_slotstart_y) / USHOP_SLOT_HEIGHT;
				slot_index = (m_iNowPack*USHOP_SLOT_X_NUM*USHOP_SLOT_Y_NUM)+ slot_x_num * USHOP_SLOT_Y_NUM + slot_y_num;

				if( m_iType != USHOP_TYPE_BUYER && !m_bSlotLock && msg == WM_LBUTTONDOWN ) // 물건을 등록하거나 뺄 수있는 경우다. ///점포상점 시 수정
				{
					// 엑스트라 슬롯에 아이템이 있을 경우는 아이템을 넣는거다.
					if (g_pRoh && g_pRoh->m_ExtraSlot)
					{
						if( (GET_ATT_FLAG(g_pRoh->m_ExtraSlot) & IATT_NO_EXCHANGE) || g_pRoh->m_ExtraSlot->m_bTimeLimit  // 교환 불가 아이템은 못팔도록.
								|| g_pRoh->m_ExtraSlot->m_Vnum == 1336 || g_pRoh->m_ExtraSlot->m_Vnum == 1337 || g_pRoh->m_ExtraSlot->m_Vnum == 1338  || g_pRoh->m_ExtraSlot->m_Vnum == 10021 || g_pRoh->m_ExtraSlot->m_Vnum == 10022 // Add Red Bullion & Platinum Bullion
								|| g_pRoh->m_ExtraSlot->m_Vnum == 1510 || g_pRoh->m_ExtraSlot->m_Vnum == 1511 || (g_dwClientCountry == CTRY_KOR&&g_pRoh->m_ExtraSlot->m_Vnum == 1519)  // 태국 발렌타인 데이 쵸코는 개인상점에선 못팔도록...사랑을 돈으로 사지 말란다... // 1519 경험치 아이템의 경우 한국과 해외의 속성 관리가 꼬여서 한국만 교환불가를 예외로 적용시킨다.
						  )
						{
							if( pCMyApp && pCMyApp->m_pUIMgr ) // 에러메세지 출력해준다.
							{
								//-- IDS_ITEM_CANT_SELL : 해당 아이템은 판매가 불가능합니다.
								pCMyApp->m_pUIMgr->CmdErrorMsg( (char*)G_STRING(IDS_ITEM_CANT_SELL) );
							}
						}
						else
						{
							vnum = g_pRoh->m_ExtraSlot->m_Vnum;

							int cur_cnt = m_SlotItemCount[m_iNowPack][slot_x_num][slot_y_num];

							// 슬롯의 아이템이 1개이상이고 extraslot의 vnum과 다를 경우 아무 동작도하지마라~
							if((cur_cnt >= 1))
							{
								// 이거 나중에 Vnum 뿐만아니고 다른 부분까지 몽땅 검사하자.
								if( m_pSellItem[m_iNowPack][slot_x_num][slot_y_num]->m_Vnum != vnum || (cur_cnt >= MAX_COUNT_IN_ONESLOT) )
									return 1;
							}

							// ExtraSlot 포인터를 참조포인터에 기억하고 NULL 셋팅해서 가격 입력중 아이템이 없는 척 시켜준다..
							if( g_pRoh->m_ExtraSlot )
							{
								m_pRefTempExtraItem = g_pRoh->m_ExtraSlot;
								g_pRoh->m_ExtraSlot = NULL;
							}

							// 여기서 패킷을 보내는게 아니라 여기서 가격 묻는 창을 띄워야 한다.
							m_TempMoveInfo.SlotIndex = slot_index; // 슬롯의 x y를 일차원 배열 인덱스로 변환한 녀석. // 서버에 보낼 인덱스는 여기서 미리 기억해둔다.
							m_TempMoveInfo.InvenPack = -1;
							m_TempMoveInfo.Count = -1; // 수량이 -1이면 Extra 슬롯의 녀석을 넣으라는 뜻.(인벤 좌표등이 필요없다.)
							//-- IDS_USHOP_INPUT_ITEM_1PRICE : 판매하실 아이템의 개당 가격을 입력하세요.
							g_pNk2DFrame->InsertPopup("." /*(char*)G_STRING(IDS_USHOP_INPUT_ITEM_1PRICE*/, TYPE_EXCHANGE_EX, 18 );
						}
					}
					else if( m_SlotItemCount[m_iNowPack][slot_x_num][slot_y_num] > 0 ) // 아이템을 빼낼 경우.
					{
						// 여기선 바로 패킷을 보낸다. (결과 받은 후에 실제 아이템 포인터 유효한지 검사하는거 잊지말자)
						sprintf( commOutBuf, "u_shop unreg %d %d %d\n", slot_index, -1, 1 ); // 이 경우는 카운트는 1이다.

						if( g_pTcpIp )
							g_pTcpIp->SendNetMessage( commOutBuf );

						m_bSrvMsgWaiting = TRUE;
						m_SrvMsgWaitTick = timeGetTime(); // 서버 메세지를 기다리고 있기 시작한 시점을 기억해두는 틱. 일정시간 서버메세지가 없으면 플래그를 풀어주기위해 둔다.(이렇게 풀릴경우 상점을 닫아버려야 할듯.)
					}
				}
				else if( m_iType == USHOP_TYPE_BUYER && msg == WM_LBUTTONDBLCLK ) // 사는 경우.
				{
					// 슬롯의 아이템이 1개이상이고 extraslot의 vnum과 다를 경우 아무 동작도하지마라~
					if( !m_SlotItemCount[m_iNowPack][slot_x_num][slot_y_num] ) // 아무것도 안들어있는 슬롯이라면...
					{
						return 1; // 무시
					}

					// 여기서 패킷을 보내는게 아니라 여기서 확인창을 띄워야 한다. 확인시 필요한 정보를 미리 기억해둔다.
					m_TempMoveInfo.SlotIndex = slot_index; // 슬롯의 x y를 일차원 배열 인덱스로 변환한 녀석. // 서버에 보낼 인덱스는 여기서 미리 기억해둔다.
					m_TempMoveInfo.InvenPack = g_pRoh->m_CurPackNum; // 넣을 인벤. 현재 열려있는 인벤이다.
					m_TempMoveInfo.Count = 1; // 왼쪽은 하나.

					// 사는 건 엑스트라 슬롯과 상관없이 클릭하면 물어보고 확인누르면 인벤에 바로 들어온다. 다중도 같은 원리로...
					UINT64 TotalLime = m_pSellItem[m_iNowPack][slot_x_num][slot_y_num]->m_ExPrice * m_TempMoveInfo.Count; // 같이 커질수있으니 8바이트를 쓴다.
					//-- IDS_USHOP_BUY_CHECK : [%s] [%d]개 금괴:[%d] 은괴:[%d] 동괴:[%d] 라임:[%I64d] 구매?
					sprintf( strTemp, (char*)G_STRING(IDS_USHOP_BUY_CHECK)
							 , GET_ITEM_NAME_VNUM(m_pSellItem[m_iNowPack][slot_x_num][slot_y_num]->m_Vnum), m_TempMoveInfo.Count
							 , m_pSellItem[m_iNowPack][slot_x_num][slot_y_num]->m_NeedRedLime * m_TempMoveInfo.Count
							 , m_pSellItem[m_iNowPack][slot_x_num][slot_y_num]->m_NeedPlatinumLime * m_TempMoveInfo.Count
							 , m_pSellItem[m_iNowPack][slot_x_num][slot_y_num]->m_NeedGoldLime * m_TempMoveInfo.Count
							 , m_pSellItem[m_iNowPack][slot_x_num][slot_y_num]->m_NeedSilverLime * m_TempMoveInfo.Count
							 , m_pSellItem[m_iNowPack][slot_x_num][slot_y_num]->m_NeedBronzeLime * m_TempMoveInfo.Count, TotalLime ); // 추후 자세하게 정보를 출력해주자. 무슨 아이템을 몇개 얼마에....
					g_pNk2DFrame->InsertPopup(strTemp,TYPE_NOR_OKCANCLE, 6,strlen(strTemp)/2);
					m_YesNoType = 1;
				}
			}

			if( m_iType != USHOP_TYPE_BUYER ) // 파는 사람만 처리할 녀석 ///점포상점 시 수정
			{
				m_SellStartBtn.MsgProc(hWnd, msg, wParam, lParam);
				m_ModifyBtn.MsgProc(hWnd, msg, wParam, lParam);
//				m_RefundBtn.MsgProc(hWnd, msg, wParam, lParam);
			}
			m_CloseShopBtn.MsgProc(hWnd, msg, wParam, lParam);

			// 탭 버튼 처리( 버튼 클래스의 메시지 프로시져를 쓰지 않고 무식하게 처리한다.이미지 크기랑 버튼 영역이랑 다르기 때문. )
			if( m_TabBtn[0].IsInside(x, y) )
			{
				// 1번창 버튼 클릭
				m_TabBtn[0].SetState(BTN_DOWN); // 눌려있는 상태로...
				m_TabBtn[1].SetState(BTN_NORMAL); // 다른 버튼들은 안눌린 상태로..
				m_TabBtn[2].SetState(BTN_NORMAL);

				m_iNowPack = 0; // 현재 눌린 팩 번호 갱신
			}
			else if( m_TabBtn[1].IsInside(x, y) && m_iPackNum >= 2 )
			{
				// 2번창 버튼 클릭
				m_TabBtn[0].SetState(BTN_NORMAL); // 눌려있는 상태로...
				m_TabBtn[1].SetState(BTN_DOWN); // 다른 버튼들은 안눌린 상태로..
				m_TabBtn[2].SetState(BTN_NORMAL);

				m_iNowPack = 1; // 현재 눌린 팩 번호 갱신
			}
			else if( m_TabBtn[2].IsInside(x, y) && m_iPackNum >= 3 )
			{
				// 3번창 버튼 클릭
				m_TabBtn[0].SetState(BTN_NORMAL); // 눌려있는 상태로...
				m_TabBtn[1].SetState(BTN_NORMAL); // 다른 버튼들은 안눌린 상태로..
				m_TabBtn[2].SetState(BTN_DOWN);

				m_iNowPack = 2; // 현재 눌린 팩 번호 갱신
			}

			return 1;
		}

		return 0; // 다른 인터페이스 부분을 찍었을 경우...(인벤,Bottom등...)

	case WM_MOUSEMOVE:

		m_pOverItem = GetOverItem( x, y ); // 마우스 인포체크.

		if( IsInsideSelledMoney(x,y) ) // 판매 금액란 안에 마우스가 위치했다면...
			m_bDrawSelledPricePopUp = TRUE; // 뷰 플래그를 켜줄뿐.
		else // 아니면...
			m_bDrawSelledPricePopUp = FALSE; // 끈다.

	case WM_LBUTTONUP:

		if( IsInside(x,y) ) // 인터페이스의 안쪽을 클릭했을 경우.
		{
			if( m_iType != USHOP_TYPE_BUYER ) // 파는 사람만 처리할 녀석 ///점포상점 시 수정
			{
				m_SellStartBtn.MsgProc(hWnd, msg, wParam, lParam);
				m_ModifyBtn.MsgProc(hWnd, msg, wParam, lParam);
//				m_RefundBtn.MsgProc(hWnd, msg, wParam, lParam);
			}

			m_CloseShopBtn.MsgProc(hWnd, msg, wParam, lParam);

			if( m_ModifyBtn.GetState()==BTN_ACTION ) // 수정 버튼 클릭시
			{
				// 패킷을 보내자.
				sprintf( commOutBuf, "u_shop modify\n" );
				if( g_pTcpIp )
					g_pTcpIp->SendNetMessage( commOutBuf );

				m_bSrvMsgWaiting = TRUE;
				m_SrvMsgWaitTick = timeGetTime(); // 서버 메세지를 기다리고 있기 시작한 시점을 기억해두는 틱. 일정시간 서버메세지가 없으면 플래그를 풀어주기위해 둔다.(이렇게 풀릴경우 상점을 닫아버려야 할듯.)


				m_ModifyBtn.SetState(BTN_NORMAL);	// 버튼의 스테이터스를 직접 풀어줘야하나보다.
			}

			if( m_SellStartBtn.GetState()==BTN_ACTION ) // 팔기시작 버튼 클릭시
			{
				strcpy( g_pRoh->m_strUserShopMent, m_ShopMentText.GetStrBuf() ); // 이걸 기억해둬야 자기 점포 열었을 때 유지가 된다.

				char strTemp3[50] = {0,}; // 일본은 입력 길이 제한이 넘어가도 입력이 되어버리는 문제가 있더라 그래서 혹시 넘어가더라도 클라이언트에서 잘라서 보내주자. ///07-04-10
				strncpy( strTemp3, m_ShopMentText.GetStrBuf(), 49 );
				strTemp3[49] = '\0';

				// 패킷을 보내자.
//				sprintf( commOutBuf, "u_shop start %s\n", m_ShopMentText.GetStrBuf() );
				sprintf( commOutBuf, "u_shop start %s\n", strTemp3 ); ///07-04-10
				if( g_pTcpIp )
					g_pTcpIp->SendNetMessage( commOutBuf );

				m_bSrvMsgWaiting = TRUE;
				m_SrvMsgWaitTick = timeGetTime(); // 서버 메세지를 기다리고 있기 시작한 시점을 기억해두는 틱. 일정시간 서버메세지가 없으면 플래그를 풀어주기위해 둔다.(이렇게 풀릴경우 상점을 닫아버려야 할듯.)

				m_SellStartBtn.SetState(BTN_NORMAL);	// 버튼의 스테이터스를 직접 풀어줘야하나보다.
			}

//			if( m_RefundBtn.GetState() == BTN_ACTION )
			//{
			//	SendShopGetLaim();
//

			//	m_RefundBtn.SetState(BTN_NORMAL);
			//}

			if( m_CloseShopBtn.GetState()==BTN_ACTION ) // 상점닫기 버튼 클릭시
			{
				// 이거 조건 점포들어가면 조건이 좀 바뀌어야한다.(점포는 '그만보기' 버튼도 넣어야 할듯.
				if( m_iType != USHOP_TYPE_BUYER && m_bSlotLock ) // 팔던 중에 닫기 요청이면...
				{
					//-- IDS_USHOP_CLOSE : 개인상점을 닫으시겠습니까?
					g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_USHOP_CLOSE),TYPE_NOR_OKCANCLE, 6);
					m_YesNoType = 0;
				}
				else
					Close();

				m_CloseShopBtn.SetState(BTN_NORMAL);	// 버튼의 스테이터스를 직접 풀어줘야하나보다.
			}

			return 1;
		}

		break;

	case WM_KEYDOWN:
	case WM_CHAR:
	case WM_IME_STARTCOMPOSITION:
	case WM_IME_COMPOSITION:
	case WM_IME_ENDCOMPOSITION:
	case WM_IME_CHAR:
	case WM_IME_NOTIFY:

		if( m_bSlotLock == FALSE ) // 수정가능한 모드일때만...
		{
			if( m_ShopMentText.GetEnable() )
				m_ShopMentText.MsgProc( hWnd, msg, wParam, lParam );

			if( m_ShopMentText.GetEnable() )
				return 1;
			else
				return 0;
		}

		break;
	}
	return 0;
}


BOOL CControlUserShop::IsInside( int x, int y )
{
	if( !m_pBackSur )
		return FALSE;

	return m_pBackSur->IsIn(x,y);
}

BOOL CControlUserShop::IsSlotInside( int x, int y ) // 슬롯부분 영역체크.
{
	if ( x >= m_my_slotstart_x
			&& x < m_my_slotstart_x + USHOP_SLOT_WIDTH*USHOP_SLOT_X_NUM
			&& y >= m_my_slotstart_y
			&& y < m_my_slotstart_y + USHOP_SLOT_HEIGHT*USHOP_SLOT_Y_NUM )
		return TRUE;

	return FALSE;
}

BOOL CControlUserShop::IsInOutBtnInside( int x, int y ) // 인터페이스 영역 밖에 있는 인/아웃 버튼위치검사.
{
	if( m_States == USHOP_STATES_NORMAL ) // In 체크.
	{
		if (x >= m_InBtn.PosX
				&& x < m_InBtn.PosX + m_InBtn.Width
				&& y >= m_InBtn.PosY
				&& y < m_InBtn.PosY + m_InBtn.Height )
			return TRUE;
	}
	else if( m_States == USHOP_STATES_HIDE ) // Out 체크
	{
		if (x >= m_OutBtn.PosX
				&& x < m_OutBtn.PosX + m_OutBtn.Width
				&& y >= m_OutBtn.PosY
				&& y < m_OutBtn.PosY + m_OutBtn.Height )
			return TRUE;
	}

	return FALSE;
}

CItem* CControlUserShop::GetOverItem( int x, int y ) // 특정 아이템 위로 마우스가 올라갔는지의 체크.
{
	static CItem *OldOverItem = NULL; // 이전 OverItem의 포인터. 이전 껏과 새로운게 틀리다면 Inven클래스의 DrawItemInfo에 갱신 플래그를 셋팅해준다. 그 이유는 DrawItemInfo안에서 인덱스로 같은 아이템인지를 비교하는데 상점에 등록된 아이템들은 인덱스를 안쓰기때문...

	if( IsSlotInside(x,y) ) // 슬롯안에 포인터가 있다면...
	{
		// 구체적인 슬롯 위치를 얻어온다.
		int slot_x_num, slot_y_num;

		// 현재 슬롯 셋팅
		slot_x_num = (x - m_my_slotstart_x) / USHOP_SLOT_WIDTH;
		slot_y_num = (y - m_my_slotstart_y) / USHOP_SLOT_HEIGHT;

		if( OldOverItem != m_pSellItem[m_iNowPack][slot_x_num][slot_y_num] ) // 주소가 바뀌었다면..
		{
			if( pCMyApp && g_pNk2DFrame && g_pNk2DFrame->GetControlInven() )
				g_pNk2DFrame->GetControlInven()->m_bDrawInfoUpdate = TRUE; // 이걸해주면 무조건 갱신해준다.

			OldOverItem = m_pSellItem[m_iNowPack][slot_x_num][slot_y_num];
		}

		return m_pSellItem[m_iNowPack][slot_x_num][slot_y_num];
	}

	return NULL;
}

BOOL CControlUserShop::SlotIn( int SlotPosNum, int RedLime,int PlatinumLime , int GoldLime, int SilverLime, int BronzeLime, int Price, int InvenPack, int InvenX, int InvenY ) // 인벤에서 상점 슬롯으로 넣음.(한개를 넣어주는 함수다. 패킷을 받은 시점에서 카운트 만큼 이 함수가 호출된다.)(실질적으로 서버에서 받은 이후의 처리임.)
{
	// SlotPosNum 의 상점 슬롯에 InvenPack의 InvenX,InvenY의 아이템을 넣는다.
	int x = 0, y = 0, pack_num = 0, slot_num = 0;
	CItem *pItem=NULL;
	int InItemIndex = 0; // 넣을 아이템의 인덱스.

	if( InvenPack == -1 ) // 엑스트라 슬롯의 것을 넣는 경우.
	{
		if( !g_pRoh->m_ExtraSlot ) // 엑스트라 슬롯이 없으면 뭔가 잘못된건다.
			return FALSE;

		pItem = g_pRoh->m_ExtraSlot; // 참조한다.
	}
	else // 인벤의 것을 넣는 경우(다중일 경우 이렇게 온다)
	{
		InItemIndex = g_pRoh->GetItemIndexInSlot( InvenPack, InvenX, InvenY, 1, 1 ); // 다중은 무조건 사이즈 1*1짜리만 된다.

		if( InItemIndex == -1 )
			return FALSE;

		pItem = g_pRoh->FindItemByIndexFromAll( InItemIndex ); // 이거 인벤만이 아니라 extra나 교환슬롯도 모두 체크해버림에 유의.
	}

	// 슬롯넘으로 인덱스 계산
	// 슬롯넘은 선형적 ( 팩은 여러개지만 값은 선형적(?)으로 관리 )
	pack_num = SlotPosNum / (USHOP_SLOT_X_NUM*USHOP_SLOT_Y_NUM);
	slot_num = SlotPosNum % (USHOP_SLOT_X_NUM*USHOP_SLOT_Y_NUM);

	// 현재 슬롯의 좌표 계산
	x = slot_num / USHOP_SLOT_Y_NUM;
	y = slot_num % USHOP_SLOT_Y_NUM;

	if( m_SlotItemCount[pack_num][x][y] == 0 ) // 비어있는 슬롯에 넣는 경우
	{
		if( m_pSellItem[pack_num][x][y] ) // 혹시라도 아이템이 들어가 있다면 뭔가 잘못된거다.
			return FALSE;

		m_pSellItem[pack_num][x][y] = new CItem; // 새로 만들고...

		if( !m_pSellItem[pack_num][x][y] )
			return FALSE;

		m_pSellItem[pack_num][x][y]->m_SlotX = x;
		m_pSellItem[pack_num][x][y]->m_SlotX = x;
		m_pSellItem[pack_num][x][y]->m_SlotY = y;

		m_pSellItem[pack_num][x][y]->m_Index = pItem->m_Index;
		m_pSellItem[pack_num][x][y]->m_Vnum = pItem->m_Vnum;
		m_pSellItem[pack_num][x][y]->m_ProtoNum = g_ItemList.FindItem(pItem->m_Vnum);

		m_pSellItem[pack_num][x][y]->m_PlusNum = pItem->m_PlusNum; // 이거 이미 다이아,다이아 아님등이 계산된 값이다. 그대로 복사하면된다.
		m_pSellItem[pack_num][x][y]->m_GemNum = pItem->m_GemNum; // 이것도 저장해줘야한다.(저급 보석 잔여 갯수)

		m_pSellItem[pack_num][x][y]->m_Special = pItem->m_Special;

		///신방어구강화시스템
		m_pSellItem[pack_num][x][y]->m_Special2 = pItem->m_Special2; // 추가 옵션
		m_pSellItem[pack_num][x][y]->m_UpgradeEndurance = pItem->m_UpgradeEndurance;           // 현재 강화 내구도
		m_pSellItem[pack_num][x][y]->m_MaxUpgradeEndurance = pItem->m_MaxUpgradeEndurance;    // 강화 내구도 최대치

		m_pSellItem[pack_num][x][y]->m_NeedPlatinumLime = PlatinumLime;
		m_pSellItem[pack_num][x][y]->m_NeedRedLime = RedLime;
		m_pSellItem[pack_num][x][y]->m_NeedGoldLime = GoldLime; // 판매가격을 BP상점시 작업한 특별지정가격에 저장한다. // 판매가격은 넣을때만 셋팅하면 된다.
		m_pSellItem[pack_num][x][y]->m_NeedSilverLime = SilverLime; // 판매가격을 BP상점시 작업한 특별지정가격에 저장한다. // 판매가격은 넣을때만 셋팅하면 된다.
		m_pSellItem[pack_num][x][y]->m_NeedBronzeLime = BronzeLime; // 판매가격을 BP상점시 작업한 특별지정가격에 저장한다. // 판매가격은 넣을때만 셋팅하면 된다.
		m_pSellItem[pack_num][x][y]->m_ExPrice = Price; // 판매가격을 BP상점시 작업한 특별지정가격에 저장한다. // 판매가격은 넣을때만 셋팅하면 된다.

		// 아이템 속성값 셋팅
		// 아이템 값을 입력해보자!
		ResetItemValue( m_pSellItem[pack_num][x][y] );
		SetSpecialValue( m_pSellItem[pack_num][x][y], GET_TYPE(m_pSellItem[pack_num][x][y]), GET_SHAPE(m_pSellItem[pack_num][x][y]), pItem->m_Special, pItem->m_Special );

		m_SlotItemCount[pack_num][x][y] = 1;
	}
	else // 안 비어 있는 슬롯에 넣는 경우(개인상점에서는 스왑을 지원안하기로 했다. 즉, 안비어있는경우 완전히 동일한 아이템이라서 겹쳐지거나 아니면 아예 안들어가는거다.)
	{
		if( !m_pSellItem[pack_num][x][y] ) // 이 경우는 없으면 삐꾸다.
			return FALSE;

		if( !m_pSellItem[pack_num][x][y]->IsSameItem(pItem) ) // 같은 아이템 인지 비교.
			return FALSE;

		if( m_SlotItemCount[pack_num][x][y] > MAX_COUNT_IN_ONESLOT-1 ) // 넣을 수있는 한계
			return FALSE;

		m_pSellItem[pack_num][x][y]->m_SlotX = x;
		m_pSellItem[pack_num][x][y]->m_SlotY = y;

		++m_SlotItemCount[pack_num][x][y]; // 갯수 증가하고.
	}

	// 아이템을 지운다. 참조 포인터를 지우면 안될것 같다. 이렇게 지우면 원래 포인터 변수엔 NULL 셋팅이 안되지않나?
	if( InvenPack == -1 ) // 엑스트라 슬롯의 것을 넣는 경우.
	{
		SAFE_DELETE( g_pRoh->m_ExtraSlot ); // 지운다.
	}
	else // 인벤의 것을 넣는 경우(다중일 경우 이렇게 온다)
	{
		g_pRoh->RemoveByIndex( InItemIndex );
		g_pRoh->CountQuickAll(); // 퀵슬롯 아이템을 넣었을 수도 있으니 다시 세어준다.
	}

	char tmp[10] = "";
	// 슬롯에 아이템이 있으면 슬롯갯수 출력
	if( m_SlotItemCount[pack_num][x][y] > 0 )
	{
		sprintf( tmp, "%d", m_SlotItemCount[pack_num][x][y] );
		m_CountText[pack_num][x][y].SetString( tmp );
	}

	return TRUE;
}

BOOL CControlUserShop::SlotOut( int SlotPosNum, int Count ) // 상점 슬롯에서 인벤으로 넣음. (실제 아이템의 이동은 서버에서 inven 패킷으로 보내준다. 여기선 상점 슬롯쪽의 카운트 갱신만 해준다.)
{
	int x = 0, y = 0, pack_num = 0, slot_num = 0;
	CItem *pItem=NULL;

	// 슬롯넘으로 인덱스 계산
	// 슬롯넘은 선형적 ( 팩은 여러개지만 값은 선형적(?)으로 관리 )
	pack_num = SlotPosNum / (USHOP_SLOT_X_NUM*USHOP_SLOT_Y_NUM);
	slot_num = SlotPosNum % (USHOP_SLOT_X_NUM*USHOP_SLOT_Y_NUM);

	// 현재 슬롯의 좌표 계산
	x = slot_num / USHOP_SLOT_Y_NUM;
	y = slot_num % USHOP_SLOT_Y_NUM;

	if( m_SlotItemCount[pack_num][x][y] == 0 || !m_pSellItem[pack_num][x][y] ) // 비어있는 슬롯을 거내라고 한거면...삐꾸지?
		return FALSE;
	else
	{
		char tmp[10] = "";

		// 상점 슬롯의 카운트 줄이기.
		m_SlotItemCount[pack_num][x][y] -= Count; // 보내준 카운트 만큼 끈다.

		if( m_SlotItemCount[pack_num][x][y] <= 0 ) // 줄인 후 카운트가 0이 된거라면...
		{
			SAFE_DELETE( m_pSellItem[pack_num][x][y] ); // 지운다.
			m_CountText[pack_num][x][y].SetString( "" ); // 카운트 스트링도 초기화.
			sprintf( tmp, "" ); // 카운트 스트링을 비운다.

			m_pOverItem = NULL; // 이거땜에 뻑나는 경우도 있으니 반드시 초기화하자.
		}
		else
			sprintf( tmp, "%d", m_SlotItemCount[pack_num][x][y] ); // 카운트 스트링을 셋팅.

		m_CountText[pack_num][x][y].SetString( tmp );
	}

	return TRUE;
}

// [6/1/2007 Theodoric] 내구도 수리 :: 함수 파라미터 내구도 포함
// SlotIn과는 달리 일방적으로 슬롯에 셋팅하는 함수. 해당 슬롯이 비어져있다는걸 전제하에 한다.
void CControlUserShop::SetSlotItem( int SlotPosNum, int vnum, int plus, int special, int special2,
									int UpEndur, int MaxUpdEndur, int Endur, int MaxEndur ,
									int RedLime, int PlatinumLime, int GoldLime, int SilverLime, int BronzeLime, int price, int count) // 아이템 정보를 셋팅.
{
	int x = 0, y = 0, pack_num = 0, slot_num = 0;
	char tmp[10] = ""; // 카운트 입력용 스트링 버퍼.

	// 슬롯넘으로 인덱스 계산
	// 슬롯넘은 선형적 ( 팩은 여러개지만 값은 선형적(?)으로 관리 )
	pack_num = SlotPosNum / (USHOP_SLOT_X_NUM*USHOP_SLOT_Y_NUM);
	slot_num = SlotPosNum % (USHOP_SLOT_X_NUM*USHOP_SLOT_Y_NUM);

	// 현재 슬롯의 좌표 계산
	x = slot_num / USHOP_SLOT_Y_NUM;
	y = slot_num % USHOP_SLOT_Y_NUM;

	SAFE_DELETE( m_pSellItem[pack_num][x][y] ); // 혹시 모르니까 무조건 지운다.
	m_pOverItem = NULL; // 이거땜에 뻑나는 경우도 있으니 반드시 초기화하자.

	// 아이템을 생성하여 넣는다.
	m_pSellItem[pack_num][x][y] = new CItem; // 새로 만들고...
	if( !m_pSellItem[pack_num][x][y] )
		return;

	m_pSellItem[pack_num][x][y]->m_SlotX = x;
	m_pSellItem[pack_num][x][y]->m_SlotY = y;

	m_pSellItem[pack_num][x][y]->m_Vnum = vnum;
	m_pSellItem[pack_num][x][y]->m_ProtoNum = g_ItemList.FindItem( vnum );

	if( GET_TYPE(m_pSellItem[pack_num][x][y] ) != ITYPE_SUB )
	{
		if( g_ItemList.m_ItemArray[m_pSellItem[pack_num][x][y]->m_ProtoNum].m_MinLevel >= 181 )
			m_pSellItem[pack_num][x][y]->SetDiaGemNum( plus ); 	// 다이아급 보석 : 다이아급은 박혀있는 gem속성은 없구 다이아 갯수가 바로 플러스넘이다.
		else
			m_pSellItem[pack_num][x][y]->SetGemNum( plus );	// 그외 젬넘에따른 플러스값 설정
	}
	else
		m_pSellItem[pack_num][x][y]->m_PlusNum = plus; // 서브 아이템은 보석 갯수가 플러스 값

	m_pSellItem[pack_num][x][y]->m_Special = special;

	///신방어구강화시스템
	m_pSellItem[pack_num][x][y]->m_Special2 = special2; // 추가 옵션
	m_pSellItem[pack_num][x][y]->m_UpgradeEndurance = UpEndur;           // 현재 강화 내구도
	m_pSellItem[pack_num][x][y]->m_MaxUpgradeEndurance = MaxUpdEndur;    // 강화 내구도 최대치

	m_pSellItem[pack_num][x][y]->m_Endurance = Endur;
	m_pSellItem[pack_num][x][y]->m_MaxEndurance = MaxEndur;

	m_pSellItem[pack_num][x][y]->m_NeedPlatinumLime = PlatinumLime;
	m_pSellItem[pack_num][x][y]->m_NeedRedLime = RedLime;
	m_pSellItem[pack_num][x][y]->m_NeedGoldLime = GoldLime;
	m_pSellItem[pack_num][x][y]->m_NeedSilverLime = SilverLime;
	m_pSellItem[pack_num][x][y]->m_NeedBronzeLime = BronzeLime;

	m_pSellItem[pack_num][x][y]->m_ExPrice = price;

	// 아이템 속성값 셋팅
	// 아이템 값을 입력해보자!
	ResetItemValue( m_pSellItem[pack_num][x][y] );
	SetSpecialValue( m_pSellItem[pack_num][x][y], GET_TYPE(m_pSellItem[pack_num][x][y]), GET_SHAPE(m_pSellItem[pack_num][x][y]), special, special2 );

	m_SlotItemCount[pack_num][x][y] = count;

	sprintf( tmp, "%d", m_SlotItemCount[pack_num][x][y] );
	m_CountText[pack_num][x][y].SetString( tmp );
}

BOOL CControlUserShop::CheckMoneyStr(CMsgPopUp* pPopup)
{
	char* szMoney = pPopup->GetCheckPW();
	int nGold = atoi(pPopup->GetInputValue1());
	int nSilver = atoi(pPopup->GetInputValue2());
	int nBronze = atoi(pPopup->GetInputValue3());
	int nPlatinum = atoi(pPopup->GetInputValue4());
	int nRed = atoi(pPopup->GetInputValue5());

	__int64 nMoney = 0;

	// 입력된 스트링을 검사한다
	int len = strlen(szMoney);

	for(int i=0; i<len; i++)
	{
		if(isdigit(szMoney[i]) == FALSE)
		{
			if(is_space(szMoney[i]) ==TRUE)
				//-- IDS_WAREHOUSE_BLANK : 공백문자는 허용하지 않습니다.
				g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_WAREHOUSE_BLANK), TYPE_EXCHANGE_EX, 18 );
			else
				//-- IDS_WAREHOUSE_NOTNUM : 잘못된 금액입니다. 다시 입력하세요
				g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_WAREHOUSE_NOTNUM), TYPE_EXCHANGE_EX, 18 );

			return FALSE;
		}
	}

	// 금액 범위 체크.
	nMoney = _atoi64( szMoney ); // int 로 환산한 후

	if( nMoney < 0 || nMoney > LIME_LIMIT ) // -거나 한계치를 넘었으면...
	{
		//-- IDS_WAREHOUSE_NOTNUM : 잘못된 금액입니다. 다시 입력하세요
		g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_WAREHOUSE_NOTNUM), TYPE_EXCHANGE_EX, 18 );

		return FALSE;
	}

	int itemCount = nGold + nSilver + nBronze + nPlatinum + nRed;
	int itemMaxCount = 48 * 4;
	if( itemCount > itemMaxCount )
	{
		g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_WAREHOUSE_NOTNUM), TYPE_EXCHANGE_EX, 18 );

		return FALSE;
	}

	return TRUE;
}

void CControlUserShop::SelledItem( int SlotPosNum, int TotalSelledRedLime, int TotalSelledPlatinumLime, int TotalSelledGoldLime, int TotalSelledSilverLime
								   , int TotalSelledBronzeLime, __int64 TotalSelledMoney, __int64 SelledCount )
{
	int x = 0, y = 0, pack_num = 0, slot_num = 0;
	char tmp[20] = ""; // 카운트 입력용 스트링 버퍼.

	if( SlotPosNum != -1 ) // -1은 카운트 변동없이 금액만 오는 경우.(자기 점포 구경 오픈시 등...)이다.
	{
		// 슬롯넘으로 인덱스 계산
		// 슬롯넘은 선형적 ( 팩은 여러개지만 값은 선형적(?)으로 관리 )
		pack_num = SlotPosNum / (USHOP_SLOT_X_NUM*USHOP_SLOT_Y_NUM);
		slot_num = SlotPosNum % (USHOP_SLOT_X_NUM*USHOP_SLOT_Y_NUM);

		// 현재 슬롯의 좌표 계산
		x = slot_num / USHOP_SLOT_Y_NUM;
		y = slot_num % USHOP_SLOT_Y_NUM;

		m_SlotItemCount[pack_num][x][y] -= SelledCount; // 기존 카운트에 팔린만큼 빼준다.

		if( m_SlotItemCount[pack_num][x][y] <= 0 ) // 다팔렸다면...
		{
			m_SlotItemCount[pack_num][x][y] = 0;
			SAFE_DELETE( m_pSellItem[pack_num][x][y] ); // 지워버린다.
			sprintf( tmp, "" ); // 카운트 스트링을 비운다.

			m_pOverItem = NULL; // 이거땜에 뻑나는 경우도 있으니 반드시 초기화하자.

			// 다팔렸으면 다른 슬롯을 검사해서 다른 슬롯까지 다 팔렸는지 검사한다.
			bool bAllSelled = TRUE;

			for( int k = 0 ; k < USHOP_PACK_NUM ; ++k )
			{
				for( int i = 0 ; i < USHOP_SLOT_X_NUM ; ++i )
				{
					for( int j = 0 ; j < USHOP_SLOT_Y_NUM ; ++j )
					{
						if( m_SlotItemCount[k][i][j] ) // 카운트가 있으면..
							bAllSelled = FALSE; // 다판게 아니다.
					}
				}
			}

			if( bAllSelled ) // 다 팔렸다면...
			{
				if( pCMyApp && pCMyApp->m_pUIMgr ) // 메세지 출력해준다.
					//-- IDS_ITEM_ALL_SELLED : 아이템이 전부 판매되었습니다.
					pCMyApp->m_pUIMgr->CmdErrorMsg( (char*)G_STRING(IDS_ITEM_ALL_SELLED) );
			}
		}
		else
			sprintf( tmp, "%d", m_SlotItemCount[pack_num][x][y] ); // 카운트 스트링을 셋팅.

		m_CountText[pack_num][x][y].SetString( tmp );
	}


	if( (m_SelledPlatinumLime!=TotalSelledPlatinumLime) || (m_SelledRedLime!=TotalSelledRedLime) || (m_SelledGoldLime!=TotalSelledGoldLime) 
		|| (m_SelledSilverLime!=TotalSelledSilverLime) || (m_SelledBronzeLime!=TotalSelledBronzeLime) || (m_SelledLime!=TotalSelledMoney) ) // 바뀐 수치가 있다면...
	{
		CMultiText multiText;
		char strTemp[256];
		char strTemp2[256];
		int CommaPos = 0; ///가격표시개선
		// Red Line
		sprintf( strTemp, "Sold red bar:%d", TotalSelledRedLime );
		multiText.AddString( strTemp, RGB(255,0,0) );
		
		// Red Line
		sprintf( strTemp, "Sold platinum bar:%d", TotalSelledPlatinumLime );
		multiText.AddString( strTemp, RGB(255,165,0) );
		//-- IDS_SELLED_GOLDLIME : 팔린 금괴: %d
		sprintf( strTemp, G_STRING(IDS_SELLED_GOLDLIME), TotalSelledGoldLime );
		multiText.AddString( strTemp, RGB(234,236,22) );
		//-- IDS_SELLED_SILVERLIME : 팔린 은괴: %d
		sprintf( strTemp, G_STRING(IDS_SELLED_SILVERLIME), TotalSelledSilverLime );
		multiText.AddString( strTemp, RGB(122,220,122) );
		//-- IDS_SELLED_BRONZELIME : 팔린 동괴: %d
		sprintf( strTemp, G_STRING(IDS_SELLED_BRONZELIME), TotalSelledBronzeLime );
		multiText.AddString( strTemp, RGB(255,152,57) );
		//-- IDS_SELLED_LIME : 팔린 라임: %I64d
		sprintf( strTemp, G_STRING(IDS_SELLED_LIME), TotalSelledMoney );
		CommaPos = InsertPriceComma( strTemp2, strTemp, ':' ); // 콤마를 넣은 스트링으로 바꾼다.
		multiText.AddString( strTemp2,  RGB(255,255,255) ); ///가격표시개선

		if( g_dwClientCountry == CTRY_KOR && TotalSelledMoney != 0 ) // 가격이 0이면 표시 안하도록 수정 07-02-27 원석
		{
			ConvertMoneyToString( strTemp2, TotalSelledMoney, CommaPos ); ///가격표시개선
			multiText.AddString(strTemp2, RGB(255,255,255)); ///가격표시개선
		}

		m_pSelledPriceInfo->PutString(&multiText);

		__int64 TempMoney = 0; // 라임단위 금액환상용.

		int PlatinumLime_price = 0, RedLime_price = 0, GoldLime_price = 0, SileverLime_Price = 0, BronzeLime_Price = 0, proto_num = 0; // 금괴,은괴,동괴의 가격을 알아오기위해 필요한 녀석들
		proto_num = g_ItemList.FindItem(10021);
		PlatinumLime_price = g_ItemList.m_ItemArray[proto_num].m_Price;
		proto_num = g_ItemList.FindItem(10022);
		RedLime_price = g_ItemList.m_ItemArray[proto_num].m_Price;
		proto_num = g_ItemList.FindItem(1336);
		GoldLime_price = g_ItemList.m_ItemArray[proto_num].m_Price;
		proto_num = g_ItemList.FindItem(1337);
		SileverLime_Price = g_ItemList.m_ItemArray[proto_num].m_Price;
		proto_num = g_ItemList.FindItem(1338);
		BronzeLime_Price = g_ItemList.m_ItemArray[proto_num].m_Price;

		TempMoney = (__int64)TotalSelledMoney;

		// 판매금액 조정.
		sprintf( tmp, "%I64d", TempMoney ); // 누적된 총 판매금액이 온다.
		m_SellMoneyText.SetString( tmp );

		// 갱신.
		m_SelledRedLime = TotalSelledRedLime;  // Red Bullion
		m_SelledPlatinumLime = TotalSelledPlatinumLime; // Platinum Bullion
		m_SelledGoldLime = TotalSelledGoldLime;
		m_SelledSilverLime = TotalSelledSilverLime;
		m_SelledBronzeLime = TotalSelledBronzeLime;
		m_SelledLime = TotalSelledMoney;
	}

}

BOOL CControlUserShop::IsPlayArea( int x, int y ) // 인터페이스를 벗어나 땅을 클릭했는지의 체크.(인벤은 제외하고 체크.)
{
	if( !IsInside(x,y) && !g_pNk2DFrame->IsInPopup(x,y) )
		return true;

	return false;
}

BOOL CControlUserShop::IsInsideSelledMoney( int x, int y ) // 판매 금액란 체크.
{
	if( !m_pBackSur || !m_pSelledMoneyBack )
		return FALSE;

	return m_pSelledMoneyBack->IsIn(x, y);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// 개인 상점 검색용 인터페이스
CControlUShopSearch::CControlUShopSearch()
	: m_hWnd(NULL)
{
	m_pBackSur = NULL; // 인터페이스 배경
	m_pDetailInfoAlpha = NULL; // 검색된 아이템의 관련 정보(존,좌표)등을 자세히 알려주기위한 팝업.
	m_pItemImageAlpha = NULL; // 아이템 이미지 배경용.

	m_ResultList.reserve( 100 ); // 최대 100명 까지만 보내주기로 했음.

	Init(); // 변수들 초기화.
}

CControlUShopSearch::~CControlUShopSearch()
{
	DeleteRes();

	ClearList(); // 리스트도 지워주자.
	SAFE_DELETE( m_pDetailInfoAlpha );
	SAFE_DELETE( m_pItemImageAlpha );
	SAFE_DELETE( m_pBackSur );
}

void CControlUShopSearch::Init()
{
	m_ItemNameText.SetString( _T(" "), true);
	m_CharNameText.SetString( _T(" "), true);
	m_NowSearchingText.SetString( _T(" "), true);
	m_ListRect.left = 0;
	m_ListRect.top = 0;
	m_ListRect.right = 0;
	m_ListRect.bottom = 0;
	m_SearchNameInput.ClearText();   // 찾을 문자열을 입력하는 녀석.
	m_ViewListNum = 0;
	m_ClickTick = 0; // 찾기를 마구 누르지 못하게...
	m_bDrawInfoAlpha = FALSE;
	m_bNowSearching = FALSE;
	m_NowSearchTick = 0;
	ZeroMemory( m_strSearchName, sizeof(m_strSearchName) );
	// 참조 포인터 초기화.
	m_pItemRender = NULL;
	m_pRefSelOnItem = NULL;
}

void CControlUShopSearch::DeleteRes()
{
	ClearList(); // 리스트도 지워주자.
	m_CharNameText.DeleteRes();
	m_ItemNameText.DeleteRes();
	m_NowSearchingText.SetString( _T(" "), true);
	m_ListScroll.DeleteRes();
	m_CloseBtn.DeleteRes();
	m_SearchBtn.DeleteRes();
	m_SearchNameInput.DeleteRes();     // 상점주인이 써놓은 멘트 출력용 서피스.
	SAFE_DELETE_RES( m_pDetailInfoAlpha );
	SAFE_DELETE_RES( m_pItemImageAlpha );
	SAFE_SURFACE_DESTROY( m_pBackSur );
}

void CControlUShopSearch::LoadRes(CItemRender *pItemRender)
{
	if( !g_pDisplay  )
		return;

	int k = 0, x = 0, y = 0;

	Init(); // 변수들을 한번 초기화.

	m_pItemRender = pItemRender;
	m_hWnd = g_pDisplay->GetHWnd();
	GetClientRect( m_hWnd, &m_ClientRc );


	if( FAILED( g_pDisplay->CreateSurfaceFromBitmap( &m_pBackSur, "interface/usershop/search/ushopsearch.BMP" ) ) )
		return;

	m_pBackSur->SetColorKey( TRANS_COLOR );

	m_SearchBtn.SetFileName( "UserShop/search/btn_find" );
	m_CloseBtn.SetFileName( "common/btn_close_01" );
	m_SearchBtn.LoadRes();
	m_CloseBtn.LoadRes();

	m_SearchNameInput.Init( 0, 0, 10*2, RGB(255, 255, 255), RGB(255, 255, 0) );
	m_SearchNameInput.SetfontSize();
	m_SearchNameInput.ClearText();

	m_ListScroll.LoadRes( "UserShop/search/btn_scrollup", "UserShop/search/btn_scrolldown", "interface/UserShop/search/scroll_bar.bmp" );

	m_ItemNameText.Init( 12, RGB(255,255,0), 0, 0 );
	m_CharNameText.Init( 12, RGB(255,255,0), 0, 0 );
	m_NowSearchingText.Init( 12, RGB(255,255,0), 0, 0 );

	m_pBackSur->Xpos = g_pNk2DFrame->GetClientWidth() - m_pBackSur->GetWidth() - 291;
	m_pBackSur->Ypos = 0;

	m_ListRect.left = m_pBackSur->Xpos+14;
	m_ListRect.top = m_pBackSur->Ypos+125;
	m_ListRect.right = m_ListRect.left+243;
	m_ListRect.bottom = m_ListRect.top+483;

	m_SearchNameInput.SetPos( m_pBackSur->Xpos+28, m_pBackSur->Ypos+57 );

	m_SearchBtn.SetPosition( m_pBackSur->Xpos+213, m_pBackSur->Ypos+55 );
	m_CloseBtn.SetPosition( m_pBackSur->Xpos+272, m_pBackSur->Ypos+6 );

	m_ViewListNum = USHOP_SEARCH_NUM;
	m_ListScroll.SetRange( MAX_USHOP_SEARCH_NUM + 1 - m_ViewListNum ); // +1 해줘야 맞더라.
	m_ListScroll.SetDrawPos( m_pBackSur->Xpos+261, m_pBackSur->Ypos+126, 358 ); // 현재는 전체보기에서만 지원하니 전체보기만 좌표를 세로 셋팅해준다.


	m_pDetailInfoAlpha = new CAlphaPopUp;
	if( m_pDetailInfoAlpha )
	{
		m_pDetailInfoAlpha->Init(14,AlphaPopUp_Type__Char);
		m_pDetailInfoAlpha->LoadRes( m_ClientRc.right, m_ClientRc.bottom );
	}

	m_pItemImageAlpha = new CAlphaPopUp;
	if( m_pItemImageAlpha )
	{
		m_pItemImageAlpha->Init(14,AlphaPopUp_Type__Char);
		m_pItemImageAlpha->LoadRes( m_ClientRc.right, m_ClientRc.bottom );
	}

	m_SearchNameInput.SetEnable( TRUE ); // 바로 입력할 수있도록 셋팅.
	pCMyApp->RestoreImmStatus(); ///일본전각반각버그수정
}

void CControlUShopSearch::Draw() // 이 함수의 리턴은 DrawItemInfo()함수에서 표시되는 아이템 정보를 갱신하기 위해 쓰인다.
{
	g_pDisplay->Blt( m_pBackSur->Xpos, m_pBackSur->Ypos, m_pBackSur );

	m_SearchBtn.Draw();
	m_CloseBtn.Draw();

	m_SearchNameInput.Draw(m_pBackSur->Xpos+48, m_pBackSur->Ypos+61, 144);

	m_ListScroll.Draw();
	DrawResultList();

	POINT point;
	GetCursorPos(&point);
	ScreenToClient(g_pDisplay->GetHWnd(), &point);

	if (pCMyApp->m_pMouse && IsInside(point.x, point.y))
		pCMyApp->m_pMouse->SetMouseType(M_NORMAL);

	if( m_bNowSearching )
	{
		if( (timeGetTime()%1500) > 450 )
			m_NowSearchingText.Draw(); // 깜빡이며 출력되도록...

		if( timeGetTime()-m_NowSearchTick > 5000 ) // 5초후엔 그만 깜빡이고 그만 받게.
		{
			if( g_pNk2DFrame->GetUserShopSearch()->m_ResultList.size() <= 0 ) // 들어온 리스트가 없다면...
			{
				// 에러메세지 출력
				char StrTemp[256];
				sprintf( StrTemp, (char*)G_STRING(IDS_LHSTRING1856) );

				pCMyApp->m_pUIMgr->CmdErrorMsg( StrTemp );
			}

			g_pNk2DFrame->GetUserShopSearch()->m_bNowSearching = FALSE; // 뭘 찾고있는지 표시용...
		}
	}

	// 아이템에 대한 자세한 정보를 표시해준다.
	if( m_bDrawInfoAlpha )
	{
		if( m_pRefSelOnItem )
		{
			int ItemInfoPosX = 0;
			int ItemInfoPosY = 0;
			int ItemInfoWidth = 0;
			int ItemInfoHeight = 0;
			int SellInfoWidth = 0;
			int SellInfoHeight = 0;
			int ImagePopupWidth = 0;
			int ImagePopupHeight = 0;
			int ImageWidth = 0;

			// 아이템 설명
			if( g_pNk2DFrame && g_pNk2DFrame->GetControlInven() )
			{
				g_pNk2DFrame->GetControlInven()->m_pItemInfo->SetAlphaPopup_Type(AlphaPopUp_Type__Char);
				g_pNk2DFrame->GetControlInven()->DrawItemInfo( m_pRefSelOnItem, ITEM_INFO_INVEN );

				ItemInfoWidth = g_pNk2DFrame->GetControlInven()->m_pItemInfo->GetSurWidth();
				ItemInfoHeight = g_pNk2DFrame->GetControlInven()->m_pItemInfo->GetSurHeight();

				ItemInfoPosX = g_pNk2DFrame->GetControlInven()->m_pItemInfo->GetXpos();
				ItemInfoPosY = g_pNk2DFrame->GetControlInven()->m_pItemInfo->GetYpos() + ItemInfoHeight;
			}

			// 아이템 이미지
			if( m_pItemRender )
			{
				// 배경 그리기.
				CMultiText multiText;
				char strTemp[256];
				//-- IDS_IMAGE :   IMAGE
				sprintf(strTemp,(char*)G_STRING(IDS_IMAGE) );
				multiText.AddString(strTemp, _FCOLOR_YELLOW);

				for( int k = 0 ; k < 6 ; ++k )
				{
					sprintf(strTemp, " " ); // 공백들을 넣어서 길이를 조절.
					multiText.AddString(strTemp, _FCOLOR_YELLOW);
				}
				m_pItemImageAlpha->PutString(&multiText);

				ImagePopupWidth = m_pItemImageAlpha->GetSurWidth();
				ImagePopupHeight = m_pItemImageAlpha->GetSurHeight();
				m_pItemImageAlpha->Draw( ItemInfoPosX-((ImagePopupWidth/2)+10), ItemInfoPosY-ItemInfoHeight+ImagePopupHeight );



				m_pItemRender->BeginRenderLolo();

				DWORD ambient = 0x00555555;
				if (g_pNk2DFrame->IsScroll(m_pRefSelOnItem) )
					ambient = 0x00cccccc;
				else if (m_pRefSelOnItem->m_PlusNum > 0)
					ambient = UPGRADE_ITEM_COLOR;

				CLolos *SelOnItemLolo = GET_LOLO(m_pRefSelOnItem);
				ImageWidth = SLOT_WIDTH*2; // 롤로 이미지의 화면 가로 길이.(무조건 2슬롯 가로 길이로 젠다.)(현재 가로가 2슬롯이 넘는게 없으므로.

				if( SelOnItemLolo )
				{
					g_pNk2DFrame->RenderItemInUI(ItemInfoPosX-ImageWidth-10, ItemInfoPosY-ItemInfoHeight+10,
												 m_pRefSelOnItem , FALSE , FALSE , TRUE , TRUE );
				}

				m_pItemRender->EndRenderLolo();
			}

			// 판매 정보
			SellInfoWidth = m_pDetailInfoAlpha->GetSurWidth();
			SellInfoHeight = m_pDetailInfoAlpha->GetSurHeight();

			m_pDetailInfoAlpha->Draw( ItemInfoPosX+ItemInfoWidth+(SellInfoWidth/2)+11, ItemInfoPosY-ItemInfoHeight+SellInfoHeight );

			if( g_pNk2DFrame && g_pNk2DFrame->GetControlInven() )
			{
				g_pNk2DFrame->GetControlInven()->m_pItemInfo->SetAlphaPopup_Type(AlphaPopUp_Type__Default);
			}
		}
	}
}

void CControlUShopSearch::DrawResultList()
{
	if( !g_pDisplay )
		return;

	int i = 0;
	int x, z;

	x = m_pBackSur->Xpos + m_ListRect.left;

	for( i = m_ListScroll.GetPos() ; i < m_ListScroll.GetPos()+m_ViewListNum ; ++i )
	{
		if( m_ResultList.size() > i ) // 해당 위치의 멤버가 있을때만 출력
		{
			z = i - m_ListScroll.GetPos();

			m_ItemNameText.SetPos( m_ListRect.left+2, m_ListRect.top+3+(z*USS_LIST_GAP) );
			m_ItemNameText.SetString( (char*)GET_ITEM_NAME_VNUM(m_ResultList[i]->Item.m_Vnum) , FALSE );
			m_ItemNameText.Draw();

			m_CharNameText.SetPos( m_ListRect.left+114, m_ListRect.top+3+(z*USS_LIST_GAP) );
			m_CharNameText.SetString( m_ResultList[i]->CharName, FALSE );
			m_CharNameText.Draw();
		}
	}
}

LRESULT CControlUShopSearch::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	static char commOutBuf[512];
	int x, y;
	x = LOWORD( lParam );
	y = HIWORD( lParam );
	int ListSelOn = -1; // 마우스가 가리키고있는 리스트 인덱스.
	static int OldListSelOn = -1; // 위 가리키는 인덱스 기억용. 바뀌었을때만 갱신한다.
	BOOL bInputEnable = FALSE; // 입력창 포커스 전환 체크용 ///일본전각반각버그수정

	if( m_ListScroll.MsgProc(hWnd, msg, wParam, lParam) == S_OK )
		return 1;

	if( m_SearchNameInput.GetEnable() && g_pNk2DFrame->GetControlBottom()->HasFocus() ) // 채팅입력창에 포커스가 가있다면... ///일본전각반각버그수정
	{
		m_SearchNameInput.SetEnable( FALSE ); // 강제로 포커스를 풀어버린다.(처리가 애매해서 이렇게 무식하게 함)
		pCMyApp->ConvertImmStatus(); // 텍스트 입력 포커스를 끌땐 이걸 해줘야하는 듯. ///일본전각반각버그수정
	}

	switch (msg)
	{
	case 0x020A: // 휠마우스 메세지(리스트 영역안에서의 휠 메세지를 강제로 스크롤에 연동시켜준다.)

		if( IsListInside(x,y) ) // 리스트 영역에서만 처리
		{
			if( (short)HIWORD(wParam) < 0 )
			{
				m_ListScroll.NowPos++;

				if( m_ListScroll.NowPos > m_ListScroll.RangeMax-1 )
					m_ListScroll.NowPos = m_ListScroll.RangeMax-1;
			}
			else if( (short)HIWORD(wParam) > 0 )
			{
				m_ListScroll.NowPos--;

				if( m_ListScroll.NowPos < 0 )
					m_ListScroll.NowPos = 0;
			}
			return 1;
		}

		break;

	case WM_LBUTTONDOWN:

		bInputEnable = m_SearchNameInput.GetEnable(); // 기억해두었다가... ///일본전각반각버그수정

		m_SearchNameInput.MsgProc( hWnd, msg, wParam, lParam ); // 포커스를 끄고 켜고를 위해서...

		if( bInputEnable == FALSE && m_SearchNameInput.GetEnable() ) // 꺼져있던 상태에서 켜졌다면... ///일본전각반각버그수정
			pCMyApp->RestoreImmStatus(); // 이걸 해주면 일본에서 말한 전각 반각 버그가 사라지지 않을까 싶다.

		if( IsInside(x,y) ) // 인터페이스의 안쪽을 클릭했을 경우.
		{
			m_SearchBtn.MsgProc(hWnd, msg, wParam, lParam);
			m_CloseBtn.MsgProc(hWnd, msg, wParam, lParam);

			return 1;
		}
		else if( IsPlayArea(x,y) ) // 땅을 찍었을경우 인터페이스를 닫는다..
			g_pNk2DFrame->ToggleUserShopSearchWindow( TRUE ); // Toggle 함수를 사용해서 강제 종료시킨다.

		return 0; // 다른 인터페이스 부분을 찍었을 경우...(인벤,Bottom등...)

	case WM_MOUSEMOVE:

		ListSelOn = IsListInside( x, y );

		if( OldListSelOn != ListSelOn ) // 마우스가 올라간 리스트가 있다면...
		{
			CMultiText multiText;
			char strTemp[256];
			char strTemp2[256];
			char strTemp3[256];
			int CommaPos = 0; ///가격표시개선

			if( ListSelOn == -1 ) // 선택영역안에 있다가 밖으로 나간경우
				m_bDrawInfoAlpha = FALSE;
			else if( m_ResultList.size() > ListSelOn )
			{
				m_pRefSelOnItem = &(m_ResultList[ListSelOn]->Item); // 그려주기위해서 참조포인터를 가지고 있는다.

				g_pNk2DFrame->GetControlInven()->m_bDrawInfoUpdate = TRUE;

				//-- IDS_WHAT_ZONE : 지역 [%s]
				strcpy( strTemp2, (char*)G_STRING(IDS_WHAT_ZONE) ); // sprintf 한번에 G_STRING 매크로 두번쓰면 제대로 안들어가더라. 그래서 나누어서 넣음.

				switch( m_ResultList[ListSelOn]->ZoneNum ) // 존번호를 이름으로 바꿔준다.
				{
				case 0:
					sprintf( strTemp, strTemp2, (char*)G_STRING(IDS_GATE_LOST_REALM) );
					break;
				case 1:
					sprintf( strTemp, strTemp2, (char*)G_STRING(IDS_GATE_DEKALEN) );
					break;
				case 2:
					sprintf( strTemp, strTemp2, (char*)G_STRING(IDS_GATE_DEKADUNE) );
					break;
				case 3:
					sprintf( strTemp, strTemp2, (char*)G_STRING(IDS_GATE_SHAILON) );
					break;
				case 4:
					sprintf( strTemp, strTemp2, (char*)G_STRING(IDS_GATE_LAGRAMIA) );
					break;
				case 5:
					sprintf( strTemp, strTemp2, (char*)G_STRING(IDS_GATE_SEARUSTE) );
					break;
				case 6:
					sprintf( strTemp, strTemp2, (char*)G_STRING(IDS_MAP_PHAROS) );
					break;
				case 7:
					sprintf( strTemp, strTemp2, (char*)G_STRING(IDS_GATE_WHITEHORN) );
					break;
				case 8:
					sprintf( strTemp, strTemp2, (char*)G_STRING(IDS_DUNGEON4) );
					break;
				case 9:
					sprintf( strTemp, strTemp2, (char*)G_STRING(IDS_MAP_DMITRON) );
					break;

				default:
					//-- IDS_WHAT_ZONE : 지역 [%s]
					sprintf( strTemp, (char*)G_STRING(IDS_WHAT_ZONE), _T("????") );
					break;
				}
				multiText.AddString( strTemp, _FCOLOR_YELLOW);

				POINT ShopPos;

				if( pCMyApp->GetCurWorld() == 1
						|| pCMyApp->GetCurWorld() == 2
						|| pCMyApp->GetCurWorld() == 5 )
				{
					ShopPos.x = (m_ResultList[ListSelOn]->PosX/(gMapExtention/4));
					ShopPos.y = (int)(gMapHeight*4-(m_ResultList[ListSelOn]->PosY/(gMapExtention/4)));
				}
				else
				{
					ShopPos.x = (m_ResultList[ListSelOn]->PosX/(gMapExtention/5));
					ShopPos.y = (int)(gMapHeight*5-(m_ResultList[ListSelOn]->PosY/(gMapExtention/5)));
				}

				//-- IDS_USHOP_POS : 상점좌표 (%d,%d)
				sprintf( strTemp, (char*)G_STRING(IDS_USHOP_POS), ShopPos.x, ShopPos.y );
				multiText.AddString( strTemp, _FCOLOR_YELLOW);

				// 아이템 출력해주자.
				if( m_ResultList[ListSelOn]->MinPrice == m_ResultList[ListSelOn]->MaxPrice ) // 최대 최소가격이 없으면 균일 가격이다.
				{
					//-- IDS_USHOP_SELL_PRICE : 판매 가격: ( %I64d )
					sprintf( strTemp, (char*)G_STRING(IDS_USHOP_SELL_PRICE), m_ResultList[ListSelOn]->MaxPrice  ); // 하나만 출력한다.

					///가격표시개선
					CommaPos = InsertPriceComma( strTemp2, strTemp, '(' ); // 콤마를 넣은 스트링으로 바꾼다.
					multiText.AddString( strTemp2, RGB(234, 236, 22) ); ///가격표시개선
				}
				else
				{
					//-- IDS_USHOP_SELL_PRICE2 : 판매 가격: ( %I64d ~ %I64d )
					sprintf( strTemp, (char*)G_STRING(IDS_USHOP_SELL_PRICE2), m_ResultList[ListSelOn]->MinPrice, m_ResultList[ListSelOn]->MaxPrice  );

					///가격표시개선
					CommaPos = InsertPriceComma( strTemp2, strTemp, '(' ); // 콤마를 넣은 스트링으로 바꾼다.
					CommaPos = InsertPriceComma( strTemp3, strTemp2, '~' ); // 콤마를 넣은 스트링으로 바꾼다.
					multiText.AddString( strTemp3, RGB(234, 236, 22) ); ///가격표시개선
				}

				//-- IDS_USHOP_SELL_COUNT : 판매 수량: %d
				sprintf(strTemp, (char*)G_STRING(IDS_USHOP_SELL_COUNT), m_ResultList[ListSelOn]->Count );
				multiText.AddString(strTemp, RGB(233, 233, 233));

				m_pDetailInfoAlpha->PutString(&multiText);

				m_bDrawInfoAlpha = TRUE;
			}
			else // 항목이 없는 곳일 경우.
				m_bDrawInfoAlpha = FALSE;

			OldListSelOn = ListSelOn;
		}

		if( ListSelOn == -1 )
			m_bDrawInfoAlpha = FALSE;

	case WM_LBUTTONUP:

		if( IsInside(x,y) ) // 인터페이스의 안쪽을 클릭했을 경우.
		{
			m_SearchBtn.MsgProc(hWnd, msg, wParam, lParam);
			m_CloseBtn.MsgProc(hWnd, msg, wParam, lParam);

			if( m_SearchBtn.GetState()==BTN_ACTION ) // 수정 버튼 클릭시
			{
				if( timeGetTime() - m_ClickTick > 500 ) // 일정시간 못누르게..
				{
					// 패킷을 보내자.
					int TempInt = GetSearchVnum( m_SearchNameInput.GetStrBuf() );
					if( TempInt != -1 )
					{
						// 검색중 스트링을 만들어준다.
						char StrTemp[256];

						//-- IDS_USHOP_NOW_SEARCHING : [%s] 아이템 검색 중...
						strcpy( StrTemp, (char*)G_STRING(IDS_LHSTRING1857));


						m_NowSearchingText.SetString( StrTemp, FALSE );
						m_NowSearchingText.SetPos( m_pBackSur->Xpos+(m_pBackSur->GetWidth()/2)-(m_NowSearchingText.GetBoxWidth()/2)
												   , m_pBackSur->Ypos+(m_pBackSur->GetHeight()/2)-(m_NowSearchingText.GetBoxHeight()/2) );

						sprintf( commOutBuf, "u_shop search %d\n", TempInt ); // Vnum을 보내준다.
						if( g_pTcpIp )
							g_pTcpIp->SendNetMessage( commOutBuf );
					}

					m_ClickTick = timeGetTime(); // 서버 메세지를 기다리고 있기 시작한 시점을 기억해두는 틱. 일정시간 서버메세지가 없으면 플래그를 풀어주기위해 둔다.(이렇게 풀릴경우 상점을 닫아버려야 할듯.)
				}
				m_SearchBtn.SetState(BTN_NORMAL);	// 버튼의 스테이터스를 직접 풀어줘야하나보다.
			}

			if( m_CloseBtn.GetState()==BTN_ACTION ) // 상점닫기 버튼 클릭시
			{
				g_pNk2DFrame->ToggleUserShopSearchWindow( TRUE ); // Toggle 함수를 사용해서 강제 종료시킨다.
				m_CloseBtn.SetState(BTN_NORMAL);	// 버튼의 스테이터스를 직접 풀어줘야하나보다.
			}

			// 마우스 클릭시 그사람 이름을 귓말로 셋팅.
			if( msg == WM_LBUTTONUP )
			{
				int Sel = IsListInside( x, y ); // 위의 Move꺼랑 구분해서 별도로 해줘야한다.

				if( Sel != -1 && m_ResultList.size() > Sel ) // 정상범위를 클릯..
				{
					if( m_ResultList[Sel]->CharName ) // 이름이 있다면..
					{
						if( g_pNk2DFrame
								&& g_pNk2DFrame->GetControlBottom()
						  )
						{
							char strTemp[256];

							sprintf( strTemp,"/%s ", m_ResultList[Sel]->CharName );
							g_pNk2DFrame->GetControlBottom()->SetFocus();
							g_pNk2DFrame->GetControlBottom()->m_InputBox.ClearText();
							g_pNk2DFrame->GetControlBottom()->m_InputBox.SetText( strTemp );
						}
					}
				}
			}
			return 1;
		}
		break;

	case WM_KEYDOWN:

		if( wParam ==VK_RETURN )
		{
			if( timeGetTime() - m_ClickTick > 500 ) // 일정시간 못누르게..
			{
				// 패킷을 보내자.
				int TempInt = GetSearchVnum( m_SearchNameInput.GetStrBuf() );
				if( TempInt != -1 )
				{
					// 검색중 스트링을 만들어준다.
					char StrTemp[256];

					strcpy( StrTemp, (char*)G_STRING(IDS_LHSTRING1857));

					m_NowSearchingText.SetString( StrTemp, FALSE );
					m_NowSearchingText.SetPos( m_pBackSur->Xpos+(m_pBackSur->GetWidth()/2)-(m_NowSearchingText.GetBoxWidth()/2)
											   , m_pBackSur->Ypos+(m_pBackSur->GetHeight()/2)-(m_NowSearchingText.GetBoxHeight()/2) );


					sprintf( commOutBuf, "u_shop search %d\n", TempInt ); // Vnum을 보내준다.
					if( g_pTcpIp )
						g_pTcpIp->SendNetMessage( commOutBuf );
				}

				m_ClickTick = timeGetTime(); // 서버 메세지를 기다리고 있기 시작한 시점을 기억해두는 틱. 일정시간 서버메세지가 없으면 플래그를 풀어주기위해 둔다.(이렇게 풀릴경우 상점을 닫아버려야 할듯.)
			}

			return 1;
		}
	//	break;

	case WM_CHAR:
	case WM_IME_STARTCOMPOSITION:
	case WM_IME_COMPOSITION:
	case WM_IME_ENDCOMPOSITION:
	case WM_IME_CHAR:
	case WM_IME_NOTIFY:

		if( m_SearchNameInput.GetEnable() )
			m_SearchNameInput.MsgProc( hWnd, msg, wParam, lParam );

		if( m_SearchNameInput.GetEnable() )
			return 1;
		else
			return 0;

		break;
	}
	return 0;
}

BOOL CControlUShopSearch::IsInside( int x, int y )
{
	if( !m_pBackSur )
		return FALSE;

	return m_pBackSur->IsIn(x,y);
}

int CControlUShopSearch::IsListInside(int x, int y) // 리스트 영역 체크용.
{
	int OnPos = -1;

	if (x >= m_ListRect.left
			&& x < m_ListRect.right
			&& y >= m_ListRect.top
			&& y < m_ListRect.bottom)
	{
		OnPos = ( (y - m_ListRect.top-3) / USS_LIST_GAP )+ m_ListScroll.GetPos();
	}

	return OnPos; // 리스트 영역은 맞는데 안에 아이템의 안은 아닐경우.( 다른 메세지 무시를 위해 필요하다 )
}

BOOL CControlUShopSearch::IsPlayArea( int x, int y ) // 인터페이스를 벗어나 땅을 클릭했는지의 체크.(인벤은 제외하고 체크.)
{
	if( !IsInside(x,y) && !g_pNk2DFrame->IsInPopup(x,y) )
		return true;

	return false;
}

void CControlUShopSearch::ClearList()
{
	// 리스트 초기화.
	for( vector<sUShopSearchData *>::iterator iter = m_ResultList.begin() ; iter != m_ResultList.end() ; ++iter )
		delete (*iter);

	m_ResultList.clear();
}

int CControlUShopSearch::GetSearchVnum( const char *SearchName ) // 찾으려는 아이템의 Vnum을 구해주는 함수.
{
	if( g_dwClientCountry == CTRY_JPN && strlen(SearchName) < 2 ) // 일본은 두글자 이상만 검색가능하게...
	{
		//-- IDS_WAREHOUSE_NOT0 : 입력값이 잘못되었습니다. 다시 입력하세요
		pCMyApp->m_pUIMgr->CmdErrorMsg( (char*)G_STRING(IDS_WAREHOUSE_NOT0) );
		return -1;
	}

	static int OldSearchIndex = 0; // 마지막에 찾았던 배열 인덱스.
	int SearchIndex = OldSearchIndex+1; // 이전 찾았던거 말고 딴거로 찾기위해서 기억해두었던 부분 다음꺼부터 다시 검색.

	for( int i = 0 ; i < g_ItemList.m_ItemCount ; ++i ) // 횟수만 중요. 실제 검사하는 인덱스는 다르다.
	{
		if( SearchIndex > g_ItemList.m_ItemCount-1 ) // 루프 // 여기서 체크해야 한다. 밑에서 하면 위의 OldSearchIndex+1때문에 튕길수가 있다.
			SearchIndex = 0;

		if( g_dwClientCountry == CTRY_JPN && (g_ItemList.m_ItemArray[SearchIndex].m_AttFlag&IATT_NO_EXCHANGE) ) // 일본은 교환 불가 속성아이템은 검색안되게..
		{
			++SearchIndex;
			continue; // 넘긴다.
		}

		const char * itemName = GET_ITEM_NAME_VNUM(g_ItemList.m_ItemArray[SearchIndex].m_Vnum);
		int ItemNameLen = strlen( itemName );
		int SearchLen = strlen( SearchName );

		for( int j = 0 ; j <= ItemNameLen-SearchLen ; ++j )
		{
			if( strncmp(&itemName[j],SearchName,SearchLen) == 0 ) // 같은 이름이 있으면...
			{
				OldSearchIndex = SearchIndex; // 찾은 인덱스를 기억해두고...

				strcpy( m_strSearchName, itemName ); // 이름을 기억해둔다.

				return g_ItemList.m_ItemArray[SearchIndex].m_Vnum;
			}
		}
		++SearchIndex;
	}

	// 이경우는 못찾은 경우.
	if( pCMyApp && pCMyApp->m_pUIMgr ) // 에러메세지 출력해준다.
		//-- IDS_USHOP_SEARCH_FAIL_NAME : 검색어에 맞는 아이템이 없습니다.
		pCMyApp->m_pUIMgr->CmdErrorMsg( (char*)G_STRING(IDS_USHOP_SEARCH_FAIL_NAME) );

	return -1;
}

HRESULT CControlUserShop::PopupMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, int nMsg, CMsgPopUp* pArgPopup)
{
	if( nMsg == 1 ) // 확인이 눌린경우.
	{
		char commOutBuf[512] = "";	// Temp buffer

		switch( m_YesNoType )
		{
		case 0: // 상점 닫기 확인
			Close();
			break;

		case 1: // 구매 확인.
			// 서버로 패킷을 보낸다.
			sprintf( commOutBuf, "u_shop buy %d %d %d\n", m_TempMoveInfo.SlotIndex, m_TempMoveInfo.InvenPack, m_TempMoveInfo.Count );

			if( g_pTcpIp )
				g_pTcpIp->SendNetMessage( commOutBuf );

			m_bSrvMsgWaiting = TRUE;
			m_SrvMsgWaitTick = timeGetTime(); // 서버 메세지를 기다리고 있기 시작한 시점을 기억해두는 틱. 일정시간 서버메세지가 없으면 플래그를 풀어주기위해 둔다.(이렇게 풀릴경우 상점을 닫아버려야 할듯.)

			ZeroMemory( &m_TempMoveInfo, sizeof(sMoveItemInfo) );

			break;

		case 2: // 등록 확인.

			// 서버로 패킷을 보낸다.
			sprintf( commOutBuf, "u_shop reg %d %d %d %d %d %d %d %d %d %d\n", m_TempMoveInfo.SlotIndex
					 , m_TempMoveInfo.InvenPack,  m_TempMoveInfo.RedCount,m_TempMoveInfo.PlatinumCount, m_TempMoveInfo.GoldCount, m_TempMoveInfo.SilverCount
					 , m_TempMoveInfo.CopperCount, m_TempMoveInfo.Price, m_TempMoveInfo.InvenX[0], m_TempMoveInfo.InvenY[0] ); // 카운트는 보내려다 서버쪽에서 필요없다고 해서 일단 뺐다. 하지만 추후 갯수 입력을 고려해 구조체상에는 남겨둔다.

			if( g_pTcpIp )
				g_pTcpIp->SendNetMessage( commOutBuf );

			// 기억해놓은 ExtraSlot 포인터가 있다면 복구해준다.
			if( m_pRefTempExtraItem ) // m_TempMoveInfo 를 지우기 전에 해줘야한다.
			{
				if( m_TempMoveInfo.InvenPack == -1 ) // 이게 -1이면 다중 이동이아닌 하나 이동었던거다. Extra슬롯을 복구해준다.
					m_pNkCha->m_ExtraSlot = m_pRefTempExtraItem;
				m_pRefTempExtraItem = NULL; // 이걸 안해주면 문제가 생기겠지?
			}

			ZeroMemory( &m_TempMoveInfo, sizeof(sMoveItemInfo) ); // 지워준다.

			m_bSrvMsgWaiting = TRUE;
			m_SrvMsgWaitTick = timeGetTime(); // 서버 메세지를 기다리고 있기 시작한 시점을 기억해두는 틱. 일정시간 서버메세지가 없으면 플래그를 풀어주기위해 둔다.(이렇게 풀릴경우 상점을 닫아버려야 할듯.)

			break;
		}
	}
	else if( nMsg == -1 ) // 취소가 눌린 경우
	{
		if( m_YesNoType == 2 )
		{
			// 기억해놓은 ExtraSlot 포인터가 있다면 복구해준다.
			if( m_pRefTempExtraItem ) // m_TempMoveInfo 를 지우기 전에 해줘야한다.
			{
				if( m_TempMoveInfo.InvenPack == -1 ) // 이게 -1이면 다중 이동이아닌 하나 이동었던거다. Extra슬롯을 복구해준다.
					m_pNkCha->m_ExtraSlot = m_pRefTempExtraItem;
				m_pRefTempExtraItem = NULL; // 이걸 안해주면 문제가 생기겠지?
			}

			ZeroMemory( &m_TempMoveInfo, sizeof(sMoveItemInfo) ); // 확인 메세지 박스를 한번더 띄워준다.
		}

		m_YesNoType = -1;
	}
	return 0L;
}

LRESULT CControlUserShop::MoneyPopupMsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, int nMsg, CMsgPopUp *pArgPopup)
{

	if( nMsg == 1 ) // 확인이 눌린경우.
	{
		if( CheckMoneyStr(pArgPopup) ) // 제대로 된 값이 넣어진 경우
		{
			// 등록 확인창을 하나 더 띄운다.
			if( m_pRefTempExtraItem )
			{
				m_TempMoveInfo.GoldCount = atoi(pArgPopup->GetInputValue1());
				m_TempMoveInfo.SilverCount = atoi(pArgPopup->GetInputValue2());
				m_TempMoveInfo.CopperCount = atoi(pArgPopup->GetInputValue3());
				m_TempMoveInfo.PlatinumCount = atoi(pArgPopup->GetInputValue4());
				m_TempMoveInfo.RedCount = atoi(pArgPopup->GetInputValue5());
				m_TempMoveInfo.Price = atoi(pArgPopup->GetCheckPW());

				// 최소 가격을 체크
				if( m_TempMoveInfo.GoldCount == 0 && m_TempMoveInfo.SilverCount == 0
						&& m_TempMoveInfo.CopperCount == 0 && m_TempMoveInfo.RedCount == 0 &&  m_TempMoveInfo.PlatinumCount == 0 && m_TempMoveInfo.Price < SELL_MIN_LIMIT ) // 특정 금액 이하면 물품 등록이 안되도록...
				{
					//-- IDS_INPUT_MIN_LAIM : 최소 1000라임 이상 입력해주세요.
					g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_INPUT_MIN_LAIM), TYPE_EXCHANGE_EX, 18 );

					return 0L;
				}


				char strTemp[256] = "";
				//-- IDS_USHOP_SELL_CHECK : [%s] 개당 금괴:[%d] 은괴:[%d] 동괴:[%d] 라임:[%d] 판매?
				sprintf( strTemp, (char*)G_STRING(IDS_USHOP_SELL_CHECK)
						 , GET_ITEM_NAME_VNUM(m_pRefTempExtraItem->m_Vnum),m_TempMoveInfo.RedCount,m_TempMoveInfo.PlatinumCount, m_TempMoveInfo.GoldCount
						 , m_TempMoveInfo.SilverCount, m_TempMoveInfo.CopperCount, m_TempMoveInfo.RedCount, m_TempMoveInfo.PlatinumCount, m_TempMoveInfo.Price );

				g_pNk2DFrame->InsertPopup(strTemp,TYPE_NOR_OKCANCLE, 6,strlen(strTemp)/2);
				m_YesNoType = 2;
			}
		}
	}
	else if( nMsg == -1 ) // 취소가 눌린 경우
	{
		// 기억해놓은 ExtraSlot 포인터가 있다면 복구해준다.
		if( m_pRefTempExtraItem ) // m_TempMoveInfo 를 지우기 전에 해줘야한다.
		{
			if( m_TempMoveInfo.InvenPack == -1 ) // 이게 -1이면 다중 이동이아닌 하나 이동었던거다. Extra슬롯을 복구해준다.
				m_pNkCha->m_ExtraSlot = m_pRefTempExtraItem;
			m_pRefTempExtraItem = NULL; // 이걸 안해주면 문제가 생기겠지?
		}

		ZeroMemory( &m_TempMoveInfo, sizeof(sMoveItemInfo) );
	}

	return 0L;
}
