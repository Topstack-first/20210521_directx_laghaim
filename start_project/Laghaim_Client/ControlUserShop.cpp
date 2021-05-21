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
extern BOOL g_bUseCashShop; ///�������
extern DWORD			g_dwClientCountry;

extern int InsertPriceComma( char *DestString, char *SrcString, char FindChar ); // ���� ��Ʈ������ ���� �ο� �޸��� �־��ִ� �Լ�. ///����ǥ�ð���
extern char *ConvertMoneyToString( char *DestString, INT64 Money, int SpaceSize, int Type = 0 ); // �� �ݾ��� ������ ���� �ٲ��ִ� �Լ�.


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
	m_States = USHOP_STATES_NOTOPEN; // �̳༮�� Init()�����ϸ� Ÿ�ֻ̹� �ȵȴ�.

	for( k = 0 ; k < USERSHOP_SOUND_NUM ; ++k )
	{
		m_pSound[k] = NULL;
	}

	m_pSelledPriceInfo = NULL;
	Init(); // ������ �ʱ�ȭ.
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
	m_iType = -1;  // �Ǹ������� ���������� �÷���.
	m_iShopKind = -1; // ���� ����. 0�̸� �� ����.
	m_iPackNum = 1; // ���� ����.
	m_iNowPack = 0; // ���� ���õ� ��.
	m_my_slotstart_x = 0;
	m_my_slotstart_y = 0;
	m_YesNoType = -1;
	m_TopWidth = 0; // Top �޴��� ���� ����. In/Out��ư�� ������ �ֹǷ� �ʿ�.

	m_ShopNameText.SetString( _T(" "), true);     // "XXX�� ���λ���"�� ǥ�����ֱ� ���� �Ƕ���.
	m_ShopMentText.ClearText();     // ���������� ����� ��Ʈ ��¿� ���ǽ�.
	m_SellMoneyText.SetString( _T(" "), true);   // �Ǹ� �ݾ׶�.

	m_bSlotLock = FALSE; // ��ü�� ���Կ��� ���� ���� ������Ű�� ���� �÷���.
	m_bSrvMsgWaiting = FALSE;
	ZeroMemory( &m_TempMoveInfo, sizeof(sMoveItemInfo) );
	InOutMovePlusX = 0.0f; // In/Out �ִϸ��̼� �ÿ� ������ ���� Xġ.
	m_StatesTick = 0; // ���°� ���Ҷ� ����صδ� ƽ.(In/Out �ִϸ��̼ǽ� ����Ѵ�.)
	m_SrvMsgWaitTick = 0; // ���� �޼����� ��ٸ��� �ֱ� ������ ������ ����صδ� ƽ. �����ð� �����޼����� ������ �÷��׸� Ǯ���ֱ����� �д�.(�̷��� Ǯ����� ������ �ݾƹ����� �ҵ�.)

	ClearSlot(); // ������ �����۵��� �ʱ�ȭ���ִ� �༮.

	m_SelledRedLime = 0; // Red Bullion
	m_SelledPlatinumLime = 0; // Platinum Bullion
	m_SelledGoldLime = 0; // �ȸ� �ݱ� ��.(�ȾƼ� ���� �ݱ�����.)
	m_SelledSilverLime = 0; // �Ǹ� ���� ��.
	m_SelledBronzeLime = 0; // �ȸ� ���� ��.
	m_SelledLime = 0; // �ȸ� ����.
	m_bDrawSelledPricePopUp = FALSE;
	m_pRefTempExtraItem = NULL; // ���� �Է½� ExtraSlot�� �������� �����ֱ� ���� �ӽ� ���� ������.(�����Է��߿� ExtraSlot�� �����͸� ���⿡ ����صΰ� ExtraSlot�� NULL �����Ѵ�. ���� �Է��� �������ش�.)
	// ���� ������ �ʱ�ȭ.
	m_pItemRender = NULL;
	m_pOverItem = NULL;
}

void CControlUserShop::Open( int Type, int grade, char *ShopName, char *ShopMent ) // ���½ÿ� ����� �� �ʱ�ȭ��.(���� Init()�� ���½ÿ� �ϸ� �ȵǴ°� �����־ ���� ����.)
{
	int k = 0, x = 0, y = 0;

	m_iType = Type;  // �Ǹ������� ���������� �÷���.
	m_iPackNum = grade; // ���� ����.
	m_iNowPack = 0; // ���� ���õ� ��.

	// �� ��ư �ʱ�ȭ.
	m_TabBtn[0].SetState(BTN_DOWN); // �����ִ� ���·�...
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

	m_ShopNameText.SetString( ShopName, FALSE );     // "XXX�� ���λ���"�� ǥ�����ֱ� ���� �Ƕ���.
	m_ShopMentText.ClearText();     // ���������� ����� ��Ʈ ��¿� ���ǽ�.
	m_ShopMentText.SetText( ShopMent );     // ���������� ����� ��Ʈ ��¿� ���ǽ�.
	m_SellMoneyText.SetString( _T(" "), true);   // �Ǹ� �ݾ׶�.

	m_bSlotLock = TRUE; // ��ü�� ���Կ��� ���� ���� ������Ű�� ���� �÷���. ����Ʈ�� ���� �Ұ���.
	m_bSrvMsgWaiting = FALSE;
	ZeroMemory( &m_TempMoveInfo, sizeof(sMoveItemInfo) );

	ClearSlot();

	InOutMovePlusX = 0.0f;
	m_SelledRedLime = 0; // Red Bullion
	m_SelledPlatinumLime = 0; // Platinum Bullion
	m_SelledGoldLime = 0; // �ȸ� �ݱ� ��.(�ȾƼ� ���� �ݱ�����.)
	m_SelledSilverLime = 0; // �Ǹ� ���� ��.
	m_SelledBronzeLime = 0; // �ȸ� ���� ��.
	m_SelledLime = 0; // �ȸ� ����.

	// ���� �˾��� 0���� �ʱ�ȭ ������Ѵ�.
	CMultiText multiText;
	char strTemp[256] = "";
	// Platinum Lime
	sprintf( strTemp, G_STRING(IDS_SELLED_PLATINUMLIME), m_SelledPlatinumLime );
	multiText.AddString( strTemp, RGB(255,165,0) );
	// Red Lime
	sprintf( strTemp, G_STRING(IDS_SELLED_REDLIME), m_SelledRedLime );
	multiText.AddString( strTemp, RGB(255,0,0) );
	//-- IDS_SELLED_GOLDLIME : �ȸ� �ݱ�: %d
	sprintf( strTemp, G_STRING(IDS_SELLED_GOLDLIME), m_SelledGoldLime );
	multiText.AddString( strTemp, RGB(234,236,22) );
	//-- IDS_SELLED_SILVERLIME : �ȸ� ����: %d
	sprintf( strTemp, G_STRING(IDS_SELLED_SILVERLIME), m_SelledSilverLime );
	multiText.AddString( strTemp, RGB(122,220,122) );
	//-- IDS_SELLED_BRONZELIME : �ȸ� ����: %d
	sprintf( strTemp, G_STRING(IDS_SELLED_BRONZELIME), m_SelledBronzeLime );
	multiText.AddString( strTemp, RGB(255,152,57) );
	//-- IDS_SELLED_LIME : �ȸ� ����: %I64d
	sprintf( strTemp, G_STRING(IDS_SELLED_LIME), m_SelledLime );
	multiText.AddString( strTemp, RGB(255,255,255) );
	m_pSelledPriceInfo->PutString(&multiText);

	if( m_iType == USHOP_TYPE_SELLER ) // ���λ��� ������...
	{
		m_ShopMentText.SetEnable( TRUE ); // �����ڴ� ��Ʈ�� �����Ҽ�����.(�������ε� ���������� ���� �����ȵȴ�.)
		m_bSlotLock = FALSE; // �Ǹ��� ���½ÿ��� �̰� ����Ʈ�� Ǯ���ش�.

		m_SellStartBtn.SetDisable( FALSE );
		m_ModifyBtn.SetDisable( TRUE );
	}
	else if( m_iType == USHOP_TYPE_STORE ) // ������ �����Ϸ��� ������ ���... ///��������
	{
		m_ShopMentText.SetEnable( TRUE ); // �����ڴ� ��Ʈ�� �����Ҽ�����.(�������ε� ���������� ���� �����ȵȴ�.)
		m_bSlotLock = FALSE; // �Ǹ��� ���½ÿ��� �̰� ����Ʈ�� Ǯ���ش�.

		m_SellStartBtn.SetDisable( FALSE );
		m_ModifyBtn.SetDisable( TRUE );
	}
	else if( m_iType == USHOP_TYPE_ADMIN ) // ������ �����Ϸ��� ������ ���... ///��������
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
	m_SrvMsgWaitTick = timeGetTime(); // ���� �޼����� ��ٸ��� �ֱ� ������ ������ ����صδ� ƽ. �����ð� �����޼����� ������ �÷��׸� Ǯ���ֱ����� �д�.(�̷��� Ǯ����� ������ �ݾƹ����� �ҵ�.)
}

void CControlUserShop::ClearSlot() // ������ �����۵��� �ʱ�ȭ���ִ� �༮.
{
	int k = 0, x = 0, y = 0;

	for( k = 0 ; k < USHOP_PACK_NUM ; ++k )
	{
		for( x = 0 ; x < USHOP_SLOT_X_NUM ; ++x )
		{
			for( y = 0 ; y < USHOP_SLOT_Y_NUM ; ++y )
			{
				m_SlotItemCount[k][x][y] = 0;	// ���� ī��Ʈ
				SAFE_DELETE( m_pSellItem[k][x][y] );
				m_CountText[k][x][y].SetString( _T(" "), true);
			}
		}
	}

	m_pOverItem = NULL; // �̰Ŷ��� ������ ��쵵 ������ �ݵ�� �ʱ�ȭ����.
}


void CControlUserShop::DeleteRes()
{
	int i = 0, k = 0, x = 0, y = 0;

	for( i = 0 ; i < USERSHOP_SOUND_NUM ; ++i )
		m_pSound[i] = NULL;

	m_ModifyBtn.DeleteRes();		// ���� ��ư
	m_SellStartBtn.DeleteRes();        // �ǸŽ��� ��ư
	m_CloseShopBtn.DeleteRes();        // �����ݱ� ��ư
//	m_RefundBtn.DeleteRes();

	for( i = 0 ; i < USHOP_PACK_NUM ; ++i )
		m_TabBtn[i].DeleteRes();

	m_InBtn.DeleteRes();
	m_OutBtn.DeleteRes();

	m_ShopNameText.DeleteRes();     // "XXX�� ���λ���"�� ǥ�����ֱ� ���� �Ƕ���.
	m_ShopMentText.DeleteRes();     // ���������� ����� ��Ʈ ��¿� ���ǽ�.
	m_SellMoneyText.DeleteRes();   // �Ǹ� �ݾ׶�.

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
//		if( g_bUseCashShop ) // �±��� ��� ������� ��ư�� �� �ٴ´�.
//			m_TopWidth = g_pNk2DFrame->GetControlTop()->m_pSurfaceBack->GetWidth()+g_pNk2DFrame->GetControlTop()->m_IconCashShopBtn.Width-16; // top �޴��� ���� ���̸� ���صд�.
//		else
// 			m_TopWidth = g_pNk2DFrame->GetControlTop()->m_pSurfaceBack->GetWidth(); // top �޴��� ���� ���̸� ���صд�.
	}

	m_pItemRender = pItemRender;

	m_hWnd =  g_pDisplay->GetHWnd();
	GetClientRect( m_hWnd, &m_ClientRc );

	if( FAILED( g_pDisplay->CreateSurfaceFromBitmap( &m_pBackSur, "interface/UserShop/usershop.bmp" ) ) )
		return;
	m_pBackSur->SetColorKey( TRANS_COLOR );

	g_pDisplay->CreateSurfaceFromBitmap( &m_pPlzWait, "interface/UserShop/PlzWait.bmp" );
	g_pDisplay->CreateSurfaceFromBitmap( &m_pNowSelling, "interface/UserShop/NowSelling.bmp" );

	g_pDisplay->CreateSurfaceFromBitmap( &m_pSelledMoneyText, "interface/UserShop/SelledMoneyText.bmp" ); // �Ǹ� �ݾ� ����.(�������� ��� ������ؾ�������. ���� ������ �ִ´�.
	g_pDisplay->CreateSurfaceFromBitmap( &m_pSelledMoneyBack, "interface/UserShop/SelledMoneyBack.bmp" ); // �Ǹ� �ݾ� ǥ�ÿ� �а�
	m_pSelledMoneyBack->SetWidth( 85 ); // �� CreateSurface���� CSurface �� ���� �����صδ� m_dwWidth,m_dwHeight�� ���� ��������ִ� ���װ� �־ �׳� ���� ����.
	m_pSelledMoneyBack->SetHeight( 19 );

	m_ModifyBtn.SetFileName( "UserShop/btn_modify" );		// ���� ��ư
	m_SellStartBtn.SetFileName( "UserShop/btn_start" );        // �ǸŽ��� ��ư
	m_CloseShopBtn.SetFileName( "common/btn_close_01" );        // �����ݱ� ��ư
//	m_RefundBtn.SetFileName( "UserShop/btn_rime" );
	m_ModifyBtn.LoadRes();
	m_SellStartBtn.LoadRes();
	m_CloseShopBtn.LoadRes();
	//m_RefundBtn.LoadRes();

	// �ǹ�ư �ε�.
	m_TabBtn[0].SetFileName("common/btn_tab_01");
	m_TabBtn[1].SetFileName("common/btn_tab_02");
	m_TabBtn[2].SetFileName("common/btn_tab_03");	// ��ư�ɹ��� �̸��� ����
	m_TabBtn[0].LoadRes();
	m_TabBtn[1].LoadRes();
	m_TabBtn[2].LoadRes();
	m_TabBtn[0].SetState(BTN_DOWN); // ó���� �����ֵ��� ����.

	m_InBtn.SetFileName( "UserShop/btn_In" );
	m_InBtn.LoadRes();
	m_OutBtn.SetFileName( "UserShop/btn_Out" );
	m_OutBtn.LoadRes();

	m_ShopNameText.Init( 12, RGB(255,255,0), 0, 0 );     // "XXX�� ���λ���"�� ǥ�����ֱ� ���� �Ƕ���.
	//	m_ShopMentText.Init( 0, 0, 17*2, RGB(255, 255, 255), RGB(255, 255, 0) );
	m_ShopMentText.Init( 0, 0, 25, RGB(255, 255, 255), RGB(255, 255, 0) ); // ���� ���� ///07-04-10
	m_ShopMentText.SetfontSize(); // ����Ʈ ��Ʈ ������� �ʱ�ȭ.(�̰� ���ϸ� �ٸ��� �ʱ�ȭ �ȵǼ� �ȵȴ�.)
	m_SellMoneyText.Init( 12, RGB(255,255,0), 0, 0 );   // �Ǹ� �ݾ׶�.

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

	g_pDisplay->CreateSurface( &m_pInOutTempImage, 546, 419 ); // In/Out�ÿ� ����� ¦�� �κ� �̹���. ũ��� �ػ󵵺� �κ� �������̽� ũ��
	m_pInOutTempImage->SetWidth( 546 ); // �� CreateSurface���� CSurface �� ���� �����صδ� m_dwWidth,m_dwHeight�� ���� ��������ִ� ���װ� �־ �׳� ���� ����.
	m_pInOutTempImage->SetHeight( 419 );

	g_pDisplay->CreateSurface( &m_pInOutTempImage2, g_pNk2DFrame->GetClientWidth(), g_pNk2DFrame->GetClientHeight()); // In/Out�ÿ� ����� ¦�� �κ� �̹���. ũ��� �ػ󵵺� �κ� �������̽� ũ��
	m_pInOutTempImage2->SetWidth( g_pNk2DFrame->GetClientWidth() ); // �� CreateSurface���� CSurface �� ���� �����صδ� m_dwWidth,m_dwHeight�� ���� ��������ִ� ���װ� �־ �׳� ���� ����.
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

	m_bResLoad = TRUE; // �ӽ� �ּ� ó��
}

HRESULT CControlUserShop::RestoreSurfaces()
{
	// �����ϰ� �� �����ٰ� �ε�.
	DeleteRes();

	LoadRes( m_pItemRender );

	return S_OK;
}

CItem* CControlUserShop::Draw()
{
	if( !g_pDisplay )
		return NULL;

	if( m_States != USHOP_STATES_NORMAL ) // �Ϲ����� ��»��°� �ƴϸ�...�� �ƿ����� ���� ������ �������� ǥ���� ����Ƿ� ����ϰ� �߰��� ����� ����ä���Ѵ�.
	{
		// ������ ���� �����̴� ���� �ƴ϶� ��ĵ ���� �������ִ� ����̴�.
		RECT TempRect;
		int TempX = 0, TempY = 0; // ��ǥ ������ ���� �ӽ÷� ����صα� ���� ����.

		DWORD ElapsedTick = timeGetTime()-m_StatesTick;

		if( m_States == USHOP_STATES_HIDING_IN ) // ���� �ִ� ��.
		{
			float fPower = 8.0f; // �̴���.
			float fRevPower = 14.0f - ( (float)(ElapsedTick) * 0.001f * 25.0f ); // ���׷�. �ð��� ����Ͽ� ��������.(���߿��� ������ �ӷ¿� ��������.)

			if( fRevPower > 0.0f ) // ������ ����������...
			{
				fRevPower += rand()%34-20; // ���� �ణ�� ���� ������ �ش�.
			}

			InOutMovePlusX += (fPower - fRevPower)*(float)(ElapsedTick) * 0.001f; // �̴� ������ �پ��� ���׷��� ����.

			if( InOutMovePlusX < 0.0f )
				InOutMovePlusX = 0.0f;

			if( InOutMovePlusX > 546.0f ) // �Ѿ�����...
			{
				InOutMovePlusX = 546.0f;

				m_States = USHOP_STATES_HIDE; // ���������� ����.
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
			else // ���� �̻� �Ѿ�� �ƿ���ư���� ���.
			{
				m_OutBtn.Draw();
			}


			if( pCMyApp && g_pNk2DFrame && g_pNk2DFrame->GetControlTop() )
				g_pNk2DFrame->GetControlTop()->Draw(); // Top �������̽��� �ӽ÷� ���� ���.(�ֳĸ� ���� �̹����� �������Ƿ�...)

		}
		else if( m_States == USHOP_STATES_HIDING_OUT || m_States == USHOP_STATES_HIDING_OUT2 ) // ������ �ִ� ��. �Ǵ� // ��.�ϰ� ��鸮�� �κ�.
		{
			if( m_States == USHOP_STATES_HIDING_OUT2 ) // �Ĺ��� ����ϸ� ��鸮�� �κ��̴�.
			{
				if( ElapsedTick > 300 ) // ���� �ð��̻� ��������
				{
					InOutMovePlusX = 0.0f;

					m_States = USHOP_STATES_HIDE; // �̰ɷ� �ӽ÷� �ٲ��ش�. ������ ���� CloseAllWindow()�� ���ֱ����ؼ�.(m_States�� HIDE�� CloseAllWindow()�� Inven�� �����ִ�)
					g_pNk2DFrame->ShowInvenWindow( TRUE ); // �κ�â�� ���ش�.
					m_States = USHOP_STATES_NORMAL; // ���������� ����.
				}
				else
				{
					if( ( ElapsedTick / 70 ) % 2 )
						InOutMovePlusX += (float)(rand()%20)* 0.5f * ((float)ElapsedTick*0.001f);
					else
						InOutMovePlusX -= (float)(rand()%20)* 0.5f * ((float)ElapsedTick*0.001f);
				}
			}
			else // ������ �ִ� ��.
			{
				float fPower = 4.0f; // �̴���.
				float fRevPower = 10.0f - ( (float)(ElapsedTick) * 0.001f * 30.0f ); // ���׷�. �ð��� ����Ͽ� ��������.(���߿��� ������ �ӷ¿� ��������.)

				InOutMovePlusX -= (fPower - fRevPower)*(float)(ElapsedTick) * 0.001f; // �̴� ������ �پ��� ���׷��� ����.

				if( InOutMovePlusX > 546.0f ) // �Ѿ�����...
					InOutMovePlusX = 546.0f;

				if( InOutMovePlusX < 0.0f ) // �Ѿ�����...
				{
					InOutMovePlusX = 0.0f; // ��鸮���� 0.0�� �������� �ϵ���.

					m_States = USHOP_STATES_HIDING_OUT2; // �̰ɷ� �ӽ÷� �ٲ��ش�. ������ ���� CloseAllWindow()�� ���ֱ����ؼ�.(m_States�� HIDE�� CloseAllWindow()�� Inven�� �����ִ�)

					m_StatesTick = timeGetTime(); // �ð��� ����. ����� ����� ���ϰ� �ϱ����ؼ�.

					if( g_pDSound && m_pSound[USERSHOP_SOUND_IN] )
						g_pDSound->PlayToOutside( m_pSound[USERSHOP_SOUND_OUT] ); // ȿ������ ������ش�.
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
			else // ���� �̻� �Ѿ�� �������� ��ġ��.
				m_OutBtn.Draw();

			if( pCMyApp && g_pNk2DFrame && g_pNk2DFrame->GetControlTop() )
				g_pNk2DFrame->GetControlTop()->Draw(); // Top �������̽��� �ӽ÷� ���� ���.(�ֳĸ� ���� �̹����� �������Ƿ�...)
		}
		else if( m_States == USHOP_STATES_HIDE ) // ������ �ִ� ��.
		{
			if( !g_pNk2DFrame->CheckFullScreenInterface() )
				m_OutBtn.Draw();
		}

		return NULL;
	}

	int k = 0, x = 0, y = 0;
	int xpos = 0, ypos = 0; // ī��Ʈ ��¿� ����

	g_pDisplay->Blt( m_pBackSur->Xpos, m_pBackSur->Ypos, m_pBackSur );
	DrawButtons(); // ��ư�� Ÿ�Ե��� �ٲ��� ������ �Լ��� ó��.
	DrawItemSlot(); // ������ �׸���.

	// ������ ī��Ʈ ���
	for( x = 0 ; x < USHOP_SLOT_X_NUM ; ++x )
	{
		for( y = 0 ; y < USHOP_SLOT_Y_NUM ; ++y )
		{
			if( m_SlotItemCount[m_iNowPack][x][y] > 0 ) // �ش�������� �������� ���.
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

	// ���� �̸��� ��Ʈ ���.
	m_ShopNameText.Draw();
	m_ShopMentText.Draw();

	if( m_iType != USHOP_TYPE_BUYER ) // �Ĵ� ����� ����� �༮.
	{
		// �Ǹ� �ݾ� ���.
		g_pDisplay->Blt( m_pBackSur->Xpos + 31 , m_pBackSur->Ypos + 137 , m_pSelledMoneyText );
		g_pDisplay->Blt( m_pBackSur->Xpos + 89 , m_pBackSur->Ypos + 136 , m_pSelledMoneyBack );

		m_SellMoneyText.Draw();

		if( m_bDrawSelledPricePopUp )
		{
			POINT point;
			GetCursorPos(&point);
			ScreenToClient(g_pDisplay->GetHWnd(), &point);

			m_pSelledPriceInfo->Draw(point.x,point.y); // ��ǥ ����
		}

	}

	if( m_iType != USHOP_TYPE_BUYER && m_bSlotLock && !m_bSrvMsgWaiting ) // �Ǹ� �߿� ����� �޼��� ǥ��... // �������ش�.
	{
		if( (timeGetTime()%1500) > 450 )
			g_pDisplay->Blt( m_pNowSelling->Xpos, m_pNowSelling->Ypos, m_pNowSelling );
	}

	pCMyApp->m_pMouse->SetMouseType( M_NORMAL );

	if( !g_pNk2DFrame->m_Popup ) // �޼��� �˾��� �ȶ��������� ������ ������ Ŀ����ȯ�� ����.
	{
		if( pCMyApp && pCMyApp->m_pMouse )
		{
			if( m_pOverItem ) // ����Ű���ִ� �������� �ִٸ�.
				pCMyApp->m_pMouse->SetMouseType( M_HOLD );
		}
	}
	else // �޽��� �˾��� ���ְ�, ������ �޼����� ��µǴ� ���� �ƴ϶��...
		return NULL; // ������ ������ ��¾��Ѵ�.

	if( m_bSrvMsgWaiting ) // ���� �޼����� ��ٸ��� �߿� ����� �޼��� ǥ��... // �������ش�.
	{
		if( (timeGetTime()%800) > 400 )
			g_pDisplay->Blt( m_pPlzWait->Xpos, m_pPlzWait->Ypos, m_pPlzWait );
	}

	return m_pOverItem; // �� ������ �ϴ� ������ DrawItemInfo()�Լ����� ǥ�õǴ� ������ ������ �����ؼ� ǥ�����ֱ� �����ε�...
}

void CControlUserShop::DrawButtons() // ��Ȳ�� ���� �޶����� ��ư���� �����Ͽ� ���.
{
	int i = 0;

	// �� ��ư ���. ���� ����. ���� �� ��ȣ�� ���� ������.
	if( m_iNowPack >= 0 && m_iNowPack < USHOP_PACK_NUM && m_iPackNum > 0 && m_iPackNum <= USHOP_PACK_NUM ) // ������ �����϶��� ���
	{
		for( i = 0 ; i < m_iPackNum ; ++i )
		{
			if( i != m_iNowPack ) // ������ ���� �༮�� �켱 ���
				m_TabBtn[i].Draw();
		}

		m_TabBtn[m_iNowPack].Draw();
	}

	if( m_iType != USHOP_TYPE_BUYER ) // �Ĵ� ����� ����� �༮.
	{
		m_SellStartBtn.Draw( m_pBackSur->Xpos + 91, m_pBackSur->Ypos + 407, m_SellStartBtn.m_bDisable ); // m_SellStartBtn.PosX, m_SellStartBtn.PosY, m_SellStartBtn.m_bDisable ); // �𽺿��̺� üũ���� ��Ȱ������ ����� �ʿ䰡 ����.
		m_ModifyBtn.Draw( m_pBackSur->Xpos + 199, m_pBackSur->Ypos + 407, m_ModifyBtn.m_bDisable ); // �𽺿��̺� üũ���� ��Ȱ������ ����� �ʿ䰡 ����.
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

	// �������� ��ġ�� ���� ǥ��
	CItem *pItem = NULL;

	if( m_pItemRender )
	{
		m_pItemRender->BeginRenderLolo();
		for( x = 0 ; x < USHOP_SLOT_X_NUM ; ++x )
		{
			for( y = 0 ; y < USHOP_SLOT_Y_NUM ; ++y )
			{
				if( m_SlotItemCount[m_iNowPack][x][y] > 0 ) // �ش� ������ ������ ������ 0�� �ƴϸ�...
				{
					pItem = m_pSellItem[m_iNowPack][x][y];
					DWORD ambient = 0x00555555;

					if (g_pNk2DFrame->IsScroll(pItem)
							|| pItem->m_Vnum == 1251
							|| pItem->m_Vnum == 1308 // ���ǻ��� ���� ��� ���� 05-10-05 ����
							|| pItem->m_Vnum == 1286
							|| ( pItem->m_Vnum >= 1598 && pItem->m_Vnum <= 1695 ) // ����� ��û ���̱������ ������ �����⿡�� �����̹Ƿ� ����ó��.
							|| pItem->m_Vnum == 1287 ) // �̱� ������ ����, ������ ������ ��Ӱ� ������ Ŭ���̾�Ʈ���� ���� ��� ó��.(by ����)
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

	if( m_bSrvMsgWaiting ) // �����Է����̰ų� �޼��� ��ٸ��� �߿� �� �� ���ϵ��� ó��.
	{
		// ���� m_bSrvMsgWaiting �� �ʹ� ���� ���ӵǾ��ٸ� ������ ���� �����Ų��.
		if( m_bSrvMsgWaiting && timeGetTime()-m_SrvMsgWaitTick > 10000 ) // 10�� ���� �޼����� �ȿԴٸ�...
		{
			Close();

			m_bSrvMsgWaiting = FALSE;
		}

		return 1;
	}

	int x = LOWORD( lParam );
	int y = HIWORD( lParam );

	m_InBtn.MsgProc(hWnd, msg, wParam, lParam); // In/Out��ư�� Move�� ���⼭ �ѹ� �� ��������Ѵ�.
	m_OutBtn.MsgProc(hWnd, msg, wParam, lParam);

	if( msg == WM_LBUTTONDOWN && IsPlayArea(x,y) && !IsInOutBtnInside(x,y) ) ///�������� �� ����
	{
		if( m_iType == USHOP_TYPE_BUYER )// ���������� �����ڰ� ��Ŭ���� �ݾ� ��..
		{
			Close();
			return 0;
		}
		else if( m_iType == USHOP_TYPE_ADMIN ) ///�������� ������嵵 ��Ŭ�� �ݱ� �߰�(��� �������̽��� �ݴ´�.)
		{
			g_pNk2DFrame->ShowUserShopWindow( FALSE ); // �������̽��� �ݾ��ش�. ���ƴٴҼ��ֵ���.

			if( m_bSlotLock ) // �Ǹ��߿� ��Ŭ���� �� ��� �ݱ�
			{
				sprintf( commOutBuf, "u_shop m_close\n" ); // �������̽��� �ݾҴٰ� ������ �˷��ֱ� ���� ��Ŷ.

				if( g_pTcpIp )
					g_pTcpIp->SendNetMessage( commOutBuf );
			}
			else // ������ ������ ��Ŭ���� ���� �ݱ�
			{
				Close();
			}

			return 0;
		}
	}

	if( IsInOutBtnInside(x,y) ) // In/Out ��ư�� ������ ó��
	{
		// In/Out ��ư ó�� ( �������̽� ���� �ۿ� ������ ����.
		if( m_States == USHOP_STATES_NORMAL ) // In ó��
		{
			if( m_InBtn.GetState()==BTN_ACTION ) // ��ư Ŭ����
			{
				m_pInOutTempImage2->GetDDrawSurface()->Blt( NULL, g_pDisplay->GetBackBuffer(), NULL, 0, NULL ); // �鼭�ǽ��� �纻�� �����д�.
				g_pDisplay->Clear( 0 ); // ����� �����ش�����...
				Draw(); // ���λ����� �׸���.
				if( pCMyApp && g_pNk2DFrame && g_pNk2DFrame->GetControlInven() )
				{
					g_pNk2DFrame->GetControlInven()->Draw(); // �κ��� �׸���
				}


				// �̵��߿� ����� ¦�� �κ� �̹����� pDis�κ��� ���´�.
				RECT TempRect;
				TempRect.left = m_pBackSur->Xpos;
				TempRect.right = TempRect.left + m_pInOutTempImage->GetWidth();
				TempRect.top = m_pBackSur->Ypos;
				TempRect.bottom = TempRect.top + m_pInOutTempImage->GetHeight();

				m_pInOutTempImage->GetDDrawSurface()->Blt( NULL, g_pDisplay->GetBackBuffer(), &TempRect, 0, NULL );

				m_pInOutTempImage->Xpos = m_pBackSur->Xpos;
				m_pInOutTempImage->Ypos = m_pBackSur->Ypos;

				// �� �߶����� ����.
				g_pDisplay->Blt( 0, 0, m_pInOutTempImage2 );



				m_States = USHOP_STATES_HIDE; // �̰ɷ� �ӽ÷� �ٲ��ش�. ������ ���� CloseAllWindow()�� ���ֱ����ؼ�.(m_States�� HIDE�� CloseAllWindow()�� Inven�� �����ִ�)
				g_pNk2DFrame->CloseAllWindow(); // Inven �����츦 �������� �༮. Inven�� ���������� �����¿��� ���� ���ǽ��� ��� ���� ������ �߿� ǥ�õǰ� �Ѵ�. (m_States�� Normal�� �ɶ� �ٽ� ���ش�.)
				m_States = USHOP_STATES_HIDING_IN;

				m_StatesTick = timeGetTime(); // ���°� ���� �ð��� ���. //�ִϿ� ���ȴ�.

				m_InBtn.SetState(BTN_NORMAL);	// ��ư�� �������ͽ��� ���� Ǯ������ϳ�����.

				if( g_pDSound && m_pSound[USERSHOP_SOUND_IN] )
				{
					g_pDSound->PlayToOutside( m_pSound[USERSHOP_SOUND_IN] ); // ȿ������ ������ش�.
				}

				return 1; // In/Out ��ư ���� Ŭ���ÿ� �ٸ� ���ν��� ����.
			}
		}
		else if( m_States == USHOP_STATES_HIDE ) // Out ó��
		{
			if( m_OutBtn.GetState()==BTN_ACTION ) // ��ư Ŭ����
			{
				m_States = USHOP_STATES_HIDING_OUT;
				m_StatesTick = timeGetTime(); // ���°� ���� �ð��� ���. //�ִϿ� ���ȴ�.

				m_OutBtn.SetState(BTN_NORMAL);	// ��ư�� �������ͽ��� ���� Ǯ������ϳ�����.

				if( g_pDSound && m_pSound[USERSHOP_SOUND_IN] )
					g_pDSound->PlayToOutside( m_pSound[USERSHOP_SOUND_IN] ); // ȿ������ ������ش�.

				return 1; // In/Out ��ư ���� Ŭ���ÿ� �ٸ� ���ν��� ����.
			}
		}
	}

	if( m_States != USHOP_STATES_NORMAL ) // �Ϲ� �������ͽ��� �ƴϸ� �ٸ� �κ��� ���ν����� ����.
		return 0;

	switch (msg)
	{
	case WM_RBUTTONDBLCLK: // ���� �̵�.(�κ��ʿ��� ����Ŭ���Ѱ͵� ����� ���´�.)

		if( IsSlotInside(x,y) ) // ���� ������ ������ ����Ŭ���� ���..
		{
			int slot_x_num, slot_y_num, slot_index;

			slot_x_num = (x - m_my_slotstart_x) / USHOP_SLOT_WIDTH;
			slot_y_num = (y - m_my_slotstart_y) / USHOP_SLOT_HEIGHT;
			slot_index = (m_iNowPack*USHOP_SLOT_X_NUM*USHOP_SLOT_Y_NUM) + slot_x_num * USHOP_SLOT_Y_NUM + slot_y_num;

			if( !m_SlotItemCount[m_iNowPack][slot_x_num][slot_y_num] ) // �ƹ��͵� �ȵ���ִ� �����̶��...
				return 1; // ����

			if( m_iType == USHOP_TYPE_BUYER ) // ������ ����Ŭ��. ���� ����.
			{
				// ���⼭ ��Ŷ�� �����°� �ƴ϶� ���⼭ Ȯ��â�� ����� �Ѵ�. Ȯ�ν� �ʿ��� ������ �̸� ����صд�.
				m_TempMoveInfo.SlotIndex = slot_index; // ������ x y�� ������ �迭 �ε����� ��ȯ�� �༮. // ������ ���� �ε����� ���⼭ �̸� ����صд�.
				m_TempMoveInfo.InvenPack = g_pRoh->m_CurPackNum; // ���� �κ�. ���� �����ִ� �κ��̴�.
				m_TempMoveInfo.Count = 10; // �����ϰ�� ī��Ʈ�� ������ 10

				// ��� �� ����Ʈ�� ���԰� ������� Ŭ���ϸ� ����� Ȯ�δ����� �κ��� �ٷ� ���´�. ���ߵ� ���� ������...
				int TempCount = 0; // ������ ���� ���� ���ش�.
				if( m_SlotItemCount[m_iNowPack][slot_x_num][slot_y_num] < 10 )
					TempCount = m_SlotItemCount[m_iNowPack][slot_x_num][slot_y_num];
				else
					TempCount = 10;

				UINT64 TotalLime = m_pSellItem[m_iNowPack][slot_x_num][slot_y_num]->m_ExPrice * TempCount; // ���� Ŀ���������� 8����Ʈ�� ����.
				//-- IDS_USHOP_BUY_CHECK : [%s] [%d]�� �ݱ�:[%d] ����:[%d] ����:[%d] ����:[%I64d] ����?
				sprintf( strTemp, (char*)G_STRING(IDS_USHOP_BUY_CHECK)
						 , GET_ITEM_NAME_VNUM(m_pSellItem[m_iNowPack][slot_x_num][slot_y_num]->m_Vnum), TempCount
						 , m_pSellItem[m_iNowPack][slot_x_num][slot_y_num]->m_NeedPlatinumLime * TempCount
						 , m_pSellItem[m_iNowPack][slot_x_num][slot_y_num]->m_NeedRedLime * TempCount
						 , m_pSellItem[m_iNowPack][slot_x_num][slot_y_num]->m_NeedGoldLime * TempCount
						 , m_pSellItem[m_iNowPack][slot_x_num][slot_y_num]->m_NeedSilverLime * TempCount
						 , m_pSellItem[m_iNowPack][slot_x_num][slot_y_num]->m_NeedBronzeLime * TempCount, TotalLime ); // ���� �ڼ��ϰ� ������ ���������. ���� �������� � �󸶿�....

				g_pNk2DFrame->InsertPopup(strTemp,TYPE_NOR_OKCANCLE, 6,strlen(strTemp)/2);
				m_YesNoType = 1;
			}
			else if( !m_bSlotLock ) // �Ǹ���.(���������� ��� ����) // ���� ��� ���. �������� ������� üũ���ش�.
			{
				sprintf( commOutBuf, "u_shop unreg %d %d %d\n", slot_index, g_pRoh->m_CurPackNum, 10 ); // �����ϰ�� ī��Ʈ�� ������ 10
				if( g_pTcpIp )
					g_pTcpIp->SendNetMessage( commOutBuf );

				m_bSrvMsgWaiting = TRUE;
				m_SrvMsgWaitTick = timeGetTime(); // ���� �޼����� ��ٸ��� �ֱ� ������ ������ ����صδ� ƽ. �����ð� �����޼����� ������ �÷��׸� Ǯ���ֱ����� �д�.(�̷��� Ǯ����� ������ �ݾƹ����� �ҵ�.)
			}
		}
		else if( m_iType != USHOP_TYPE_BUYER ) // ���λ��� ������ �ƴϸ� �κ��� üũ�غ���. // ��, �����ڴ� ����.
		{
			// ���� ����� ��� Ŭ���̾�Ʈ���� �ּ����� ���� üũ�� �ϰ� ��� �������� üũ���ֱ�� ��. (���ǿ� ������ ��쵵 ������ ����� �����ٸ� return 1���ؼ� �ٸ� �޼����� ���ý�Ų��.)
			CControlInven *pRefInven = NULL; // �κ��� ���� ������.

			if( pCMyApp && g_pNk2DFrame && g_pNk2DFrame->GetControlInven() )
			{
				pRefInven = g_pNk2DFrame->GetControlInven(); // ���� �����͸� �޾ƿ���...
			}

			if( pRefInven && pRefInven->IsInside(x, y) && !m_bSlotLock ) // �κ��� ������ ����Ŭ���� ���.
			{
				//===========================================================
				// Ŭ���� ��ġ�� �ι� �ε��� �˻�
				int slot_x_num, slot_y_num;
				slot_x_num = (x - pRefInven->m_slotstart_x ) / SLOT_WIDTH;		// ���� Ŭ�� ��ġ
				slot_y_num = (y - pRefInven->m_slotstart_y ) / SLOT_HEIGHT;		// ���� Ŭ�� ��ġ

				// ���õ� �������� �ε��� ����
				int index = g_pRoh->GetSlotIndex(slot_x_num, slot_y_num);
				if( index < 0 )
					return 1;

				CItem* pItem = g_pRoh->FindItemByIndexFromAll( index );
				if( (GET_ATT_FLAG(pItem) & IATT_NO_EXCHANGE) || pItem->m_bTimeLimit  // ��ȯ �Ұ� �������� ���ȵ���.
						|| pItem->m_Vnum == 1336 || pItem->m_Vnum == 1337 || pItem->m_Vnum == 1338  || pItem->m_Vnum == 10021 || pItem->m_Vnum == 10022 // Add Red Bullion and Platinum Bullion
						|| pItem->m_Vnum == 1510 || pItem->m_Vnum == 1511 || (g_dwClientCountry == CTRY_KOR&&pItem->m_Vnum == 1519) ) // �±� �߷�Ÿ�� ���� ���ڴ� ���λ������� ���ȵ���...����� ������ ���� ������... // 1519 ����ġ �������� ��� �ѱ��� �ؿ��� �Ӽ� ������ ������ �ѱ��� ��ȯ�Ұ��� ���ܷ� �����Ų��.
				{
					if( pCMyApp && pCMyApp->m_pUIMgr ) // �����޼��� ������ش�.
					{
						//-- IDS_ITEM_CANT_SELL : �ش� �������� �ǸŰ� �Ұ����մϴ�.
						pCMyApp->m_pUIMgr->CmdErrorMsg( (char*)G_STRING(IDS_ITEM_CANT_SELL) );
					}

					return 1;
				}

				if( pRefInven->m_bInvenLock ) // �κ��� ���� Ŭ���� �����ϴ� ���� �ɷ��ִٸ�...
				{
					DWORD curTime = timeGetTime();

					if (curTime > pRefInven->m_timeInvenLock + 1500) // 1.5�ʰ� ��������
						pRefInven->m_bInvenLock = FALSE;
					else
						return 1;
				}

				// ���⼭ ��Ŷ�� �����°� �ƴ϶� ���⼭ ���� ���� â�� ����� �Ѵ�.
				m_TempMoveInfo.SlotIndex = 0; // ������ ��� ���� �ε����� 0���� �����ش�. ���� �� ���� �������� ������ش�.
				m_TempMoveInfo.InvenPack = g_pRoh->m_CurPackNum;
				m_TempMoveInfo.InvenX[0] = slot_x_num; // �κ� ������ ��ġ.
				m_TempMoveInfo.InvenY[0] = slot_y_num;
				m_TempMoveInfo.Count = 0; // ������ �Ⱦ��δ�. �������� �˾Ƽ� �������. �̰� �׳� �ʱ�ȭ �ϻ�.

				// ��� Ȯ�ν� ������ ���� ����ϱ� ���� ���������͸� ����صд�.(�̰�� ���߿� Extra���Կ� ������ ���ؾ��Կ� ����.)
				if( pItem )
					m_pRefTempExtraItem = pItem;

				//-- IDS_USHOP_INPUT_ITEM_1PRICE : �Ǹ��Ͻ� �������� ���� ������ �Է��ϼ���.
				g_pNk2DFrame->InsertPopup("."/* (char*)G_STRING(IDS_USHOP_INPUT_ITEM_1PRICE)*/, TYPE_EXCHANGE_EX, 18 );
				pRefInven->SetInvenLock( TRUE ); // ���Ӱ� �κ����� �ɾ�д�.(1.5�ʵ��� ����� �������ش�.)

				return 1;
			}
		}

		return 0;

		break;

	case WM_LBUTTONDBLCLK:
	case WM_LBUTTONDOWN:

		if( IsInside(x,y) ) // �������̽��� ������ Ŭ������ ���.
		{
			if( m_bSlotLock == FALSE ) // ���������� ����϶���...
				m_ShopMentText.MsgProc(hWnd, msg, wParam, lParam);

			// ���� �̺�Ʈ üũ (�ٿ�ÿ� ó���Ѵ�.)
			if( IsSlotInside(x,y) )
			{
				int slot_x_num, slot_y_num, slot_index;
				int vnum;

				slot_x_num = (x - m_my_slotstart_x) / USHOP_SLOT_WIDTH;
				slot_y_num = (y - m_my_slotstart_y) / USHOP_SLOT_HEIGHT;
				slot_index = (m_iNowPack*USHOP_SLOT_X_NUM*USHOP_SLOT_Y_NUM)+ slot_x_num * USHOP_SLOT_Y_NUM + slot_y_num;

				if( m_iType != USHOP_TYPE_BUYER && !m_bSlotLock && msg == WM_LBUTTONDOWN ) // ������ ����ϰų� �� ���ִ� ����. ///�������� �� ����
				{
					// ����Ʈ�� ���Կ� �������� ���� ���� �������� �ִ°Ŵ�.
					if (g_pRoh && g_pRoh->m_ExtraSlot)
					{
						if( (GET_ATT_FLAG(g_pRoh->m_ExtraSlot) & IATT_NO_EXCHANGE) || g_pRoh->m_ExtraSlot->m_bTimeLimit  // ��ȯ �Ұ� �������� ���ȵ���.
								|| g_pRoh->m_ExtraSlot->m_Vnum == 1336 || g_pRoh->m_ExtraSlot->m_Vnum == 1337 || g_pRoh->m_ExtraSlot->m_Vnum == 1338  || g_pRoh->m_ExtraSlot->m_Vnum == 10021 || g_pRoh->m_ExtraSlot->m_Vnum == 10022 // Add Red Bullion & Platinum Bullion
								|| g_pRoh->m_ExtraSlot->m_Vnum == 1510 || g_pRoh->m_ExtraSlot->m_Vnum == 1511 || (g_dwClientCountry == CTRY_KOR&&g_pRoh->m_ExtraSlot->m_Vnum == 1519)  // �±� �߷�Ÿ�� ���� ���ڴ� ���λ������� ���ȵ���...����� ������ ���� ������... // 1519 ����ġ �������� ��� �ѱ��� �ؿ��� �Ӽ� ������ ������ �ѱ��� ��ȯ�Ұ��� ���ܷ� �����Ų��.
						  )
						{
							if( pCMyApp && pCMyApp->m_pUIMgr ) // �����޼��� ������ش�.
							{
								//-- IDS_ITEM_CANT_SELL : �ش� �������� �ǸŰ� �Ұ����մϴ�.
								pCMyApp->m_pUIMgr->CmdErrorMsg( (char*)G_STRING(IDS_ITEM_CANT_SELL) );
							}
						}
						else
						{
							vnum = g_pRoh->m_ExtraSlot->m_Vnum;

							int cur_cnt = m_SlotItemCount[m_iNowPack][slot_x_num][slot_y_num];

							// ������ �������� 1���̻��̰� extraslot�� vnum�� �ٸ� ��� �ƹ� ���۵���������~
							if((cur_cnt >= 1))
							{
								// �̰� ���߿� Vnum �Ӹ��ƴϰ� �ٸ� �κб��� ���� �˻�����.
								if( m_pSellItem[m_iNowPack][slot_x_num][slot_y_num]->m_Vnum != vnum || (cur_cnt >= MAX_COUNT_IN_ONESLOT) )
									return 1;
							}

							// ExtraSlot �����͸� ���������Ϳ� ����ϰ� NULL �����ؼ� ���� �Է��� �������� ���� ô �����ش�..
							if( g_pRoh->m_ExtraSlot )
							{
								m_pRefTempExtraItem = g_pRoh->m_ExtraSlot;
								g_pRoh->m_ExtraSlot = NULL;
							}

							// ���⼭ ��Ŷ�� �����°� �ƴ϶� ���⼭ ���� ���� â�� ����� �Ѵ�.
							m_TempMoveInfo.SlotIndex = slot_index; // ������ x y�� ������ �迭 �ε����� ��ȯ�� �༮. // ������ ���� �ε����� ���⼭ �̸� ����صд�.
							m_TempMoveInfo.InvenPack = -1;
							m_TempMoveInfo.Count = -1; // ������ -1�̸� Extra ������ �༮�� ������� ��.(�κ� ��ǥ���� �ʿ����.)
							//-- IDS_USHOP_INPUT_ITEM_1PRICE : �Ǹ��Ͻ� �������� ���� ������ �Է��ϼ���.
							g_pNk2DFrame->InsertPopup("." /*(char*)G_STRING(IDS_USHOP_INPUT_ITEM_1PRICE*/, TYPE_EXCHANGE_EX, 18 );
						}
					}
					else if( m_SlotItemCount[m_iNowPack][slot_x_num][slot_y_num] > 0 ) // �������� ���� ���.
					{
						// ���⼱ �ٷ� ��Ŷ�� ������. (��� ���� �Ŀ� ���� ������ ������ ��ȿ���� �˻��ϴ°� ��������)
						sprintf( commOutBuf, "u_shop unreg %d %d %d\n", slot_index, -1, 1 ); // �� ���� ī��Ʈ�� 1�̴�.

						if( g_pTcpIp )
							g_pTcpIp->SendNetMessage( commOutBuf );

						m_bSrvMsgWaiting = TRUE;
						m_SrvMsgWaitTick = timeGetTime(); // ���� �޼����� ��ٸ��� �ֱ� ������ ������ ����صδ� ƽ. �����ð� �����޼����� ������ �÷��׸� Ǯ���ֱ����� �д�.(�̷��� Ǯ����� ������ �ݾƹ����� �ҵ�.)
					}
				}
				else if( m_iType == USHOP_TYPE_BUYER && msg == WM_LBUTTONDBLCLK ) // ��� ���.
				{
					// ������ �������� 1���̻��̰� extraslot�� vnum�� �ٸ� ��� �ƹ� ���۵���������~
					if( !m_SlotItemCount[m_iNowPack][slot_x_num][slot_y_num] ) // �ƹ��͵� �ȵ���ִ� �����̶��...
					{
						return 1; // ����
					}

					// ���⼭ ��Ŷ�� �����°� �ƴ϶� ���⼭ Ȯ��â�� ����� �Ѵ�. Ȯ�ν� �ʿ��� ������ �̸� ����صд�.
					m_TempMoveInfo.SlotIndex = slot_index; // ������ x y�� ������ �迭 �ε����� ��ȯ�� �༮. // ������ ���� �ε����� ���⼭ �̸� ����صд�.
					m_TempMoveInfo.InvenPack = g_pRoh->m_CurPackNum; // ���� �κ�. ���� �����ִ� �κ��̴�.
					m_TempMoveInfo.Count = 1; // ������ �ϳ�.

					// ��� �� ����Ʈ�� ���԰� ������� Ŭ���ϸ� ����� Ȯ�δ����� �κ��� �ٷ� ���´�. ���ߵ� ���� ������...
					UINT64 TotalLime = m_pSellItem[m_iNowPack][slot_x_num][slot_y_num]->m_ExPrice * m_TempMoveInfo.Count; // ���� Ŀ���������� 8����Ʈ�� ����.
					//-- IDS_USHOP_BUY_CHECK : [%s] [%d]�� �ݱ�:[%d] ����:[%d] ����:[%d] ����:[%I64d] ����?
					sprintf( strTemp, (char*)G_STRING(IDS_USHOP_BUY_CHECK)
							 , GET_ITEM_NAME_VNUM(m_pSellItem[m_iNowPack][slot_x_num][slot_y_num]->m_Vnum), m_TempMoveInfo.Count
							 , m_pSellItem[m_iNowPack][slot_x_num][slot_y_num]->m_NeedRedLime * m_TempMoveInfo.Count
							 , m_pSellItem[m_iNowPack][slot_x_num][slot_y_num]->m_NeedPlatinumLime * m_TempMoveInfo.Count
							 , m_pSellItem[m_iNowPack][slot_x_num][slot_y_num]->m_NeedGoldLime * m_TempMoveInfo.Count
							 , m_pSellItem[m_iNowPack][slot_x_num][slot_y_num]->m_NeedSilverLime * m_TempMoveInfo.Count
							 , m_pSellItem[m_iNowPack][slot_x_num][slot_y_num]->m_NeedBronzeLime * m_TempMoveInfo.Count, TotalLime ); // ���� �ڼ��ϰ� ������ ���������. ���� �������� � �󸶿�....
					g_pNk2DFrame->InsertPopup(strTemp,TYPE_NOR_OKCANCLE, 6,strlen(strTemp)/2);
					m_YesNoType = 1;
				}
			}

			if( m_iType != USHOP_TYPE_BUYER ) // �Ĵ� ����� ó���� �༮ ///�������� �� ����
			{
				m_SellStartBtn.MsgProc(hWnd, msg, wParam, lParam);
				m_ModifyBtn.MsgProc(hWnd, msg, wParam, lParam);
//				m_RefundBtn.MsgProc(hWnd, msg, wParam, lParam);
			}
			m_CloseShopBtn.MsgProc(hWnd, msg, wParam, lParam);

			// �� ��ư ó��( ��ư Ŭ������ �޽��� ���ν����� ���� �ʰ� �����ϰ� ó���Ѵ�.�̹��� ũ��� ��ư �����̶� �ٸ��� ����. )
			if( m_TabBtn[0].IsInside(x, y) )
			{
				// 1��â ��ư Ŭ��
				m_TabBtn[0].SetState(BTN_DOWN); // �����ִ� ���·�...
				m_TabBtn[1].SetState(BTN_NORMAL); // �ٸ� ��ư���� �ȴ��� ���·�..
				m_TabBtn[2].SetState(BTN_NORMAL);

				m_iNowPack = 0; // ���� ���� �� ��ȣ ����
			}
			else if( m_TabBtn[1].IsInside(x, y) && m_iPackNum >= 2 )
			{
				// 2��â ��ư Ŭ��
				m_TabBtn[0].SetState(BTN_NORMAL); // �����ִ� ���·�...
				m_TabBtn[1].SetState(BTN_DOWN); // �ٸ� ��ư���� �ȴ��� ���·�..
				m_TabBtn[2].SetState(BTN_NORMAL);

				m_iNowPack = 1; // ���� ���� �� ��ȣ ����
			}
			else if( m_TabBtn[2].IsInside(x, y) && m_iPackNum >= 3 )
			{
				// 3��â ��ư Ŭ��
				m_TabBtn[0].SetState(BTN_NORMAL); // �����ִ� ���·�...
				m_TabBtn[1].SetState(BTN_NORMAL); // �ٸ� ��ư���� �ȴ��� ���·�..
				m_TabBtn[2].SetState(BTN_DOWN);

				m_iNowPack = 2; // ���� ���� �� ��ȣ ����
			}

			return 1;
		}

		return 0; // �ٸ� �������̽� �κ��� ����� ���...(�κ�,Bottom��...)

	case WM_MOUSEMOVE:

		m_pOverItem = GetOverItem( x, y ); // ���콺 ����üũ.

		if( IsInsideSelledMoney(x,y) ) // �Ǹ� �ݾ׶� �ȿ� ���콺�� ��ġ�ߴٸ�...
			m_bDrawSelledPricePopUp = TRUE; // �� �÷��׸� ���ٻ�.
		else // �ƴϸ�...
			m_bDrawSelledPricePopUp = FALSE; // ����.

	case WM_LBUTTONUP:

		if( IsInside(x,y) ) // �������̽��� ������ Ŭ������ ���.
		{
			if( m_iType != USHOP_TYPE_BUYER ) // �Ĵ� ����� ó���� �༮ ///�������� �� ����
			{
				m_SellStartBtn.MsgProc(hWnd, msg, wParam, lParam);
				m_ModifyBtn.MsgProc(hWnd, msg, wParam, lParam);
//				m_RefundBtn.MsgProc(hWnd, msg, wParam, lParam);
			}

			m_CloseShopBtn.MsgProc(hWnd, msg, wParam, lParam);

			if( m_ModifyBtn.GetState()==BTN_ACTION ) // ���� ��ư Ŭ����
			{
				// ��Ŷ�� ������.
				sprintf( commOutBuf, "u_shop modify\n" );
				if( g_pTcpIp )
					g_pTcpIp->SendNetMessage( commOutBuf );

				m_bSrvMsgWaiting = TRUE;
				m_SrvMsgWaitTick = timeGetTime(); // ���� �޼����� ��ٸ��� �ֱ� ������ ������ ����صδ� ƽ. �����ð� �����޼����� ������ �÷��׸� Ǯ���ֱ����� �д�.(�̷��� Ǯ����� ������ �ݾƹ����� �ҵ�.)


				m_ModifyBtn.SetState(BTN_NORMAL);	// ��ư�� �������ͽ��� ���� Ǯ������ϳ�����.
			}

			if( m_SellStartBtn.GetState()==BTN_ACTION ) // �ȱ���� ��ư Ŭ����
			{
				strcpy( g_pRoh->m_strUserShopMent, m_ShopMentText.GetStrBuf() ); // �̰� ����ص־� �ڱ� ���� ������ �� ������ �ȴ�.

				char strTemp3[50] = {0,}; // �Ϻ��� �Է� ���� ������ �Ѿ�� �Է��� �Ǿ������ ������ �ִ��� �׷��� Ȥ�� �Ѿ���� Ŭ���̾�Ʈ���� �߶� ��������. ///07-04-10
				strncpy( strTemp3, m_ShopMentText.GetStrBuf(), 49 );
				strTemp3[49] = '\0';

				// ��Ŷ�� ������.
//				sprintf( commOutBuf, "u_shop start %s\n", m_ShopMentText.GetStrBuf() );
				sprintf( commOutBuf, "u_shop start %s\n", strTemp3 ); ///07-04-10
				if( g_pTcpIp )
					g_pTcpIp->SendNetMessage( commOutBuf );

				m_bSrvMsgWaiting = TRUE;
				m_SrvMsgWaitTick = timeGetTime(); // ���� �޼����� ��ٸ��� �ֱ� ������ ������ ����صδ� ƽ. �����ð� �����޼����� ������ �÷��׸� Ǯ���ֱ����� �д�.(�̷��� Ǯ����� ������ �ݾƹ����� �ҵ�.)

				m_SellStartBtn.SetState(BTN_NORMAL);	// ��ư�� �������ͽ��� ���� Ǯ������ϳ�����.
			}

//			if( m_RefundBtn.GetState() == BTN_ACTION )
			//{
			//	SendShopGetLaim();
//

			//	m_RefundBtn.SetState(BTN_NORMAL);
			//}

			if( m_CloseShopBtn.GetState()==BTN_ACTION ) // �����ݱ� ��ư Ŭ����
			{
				// �̰� ���� �������� ������ �� �ٲ����Ѵ�.(������ '�׸�����' ��ư�� �־�� �ҵ�.
				if( m_iType != USHOP_TYPE_BUYER && m_bSlotLock ) // �ȴ� �߿� �ݱ� ��û�̸�...
				{
					//-- IDS_USHOP_CLOSE : ���λ����� �����ðڽ��ϱ�?
					g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_USHOP_CLOSE),TYPE_NOR_OKCANCLE, 6);
					m_YesNoType = 0;
				}
				else
					Close();

				m_CloseShopBtn.SetState(BTN_NORMAL);	// ��ư�� �������ͽ��� ���� Ǯ������ϳ�����.
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

		if( m_bSlotLock == FALSE ) // ���������� ����϶���...
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

BOOL CControlUserShop::IsSlotInside( int x, int y ) // ���Ժκ� ����üũ.
{
	if ( x >= m_my_slotstart_x
			&& x < m_my_slotstart_x + USHOP_SLOT_WIDTH*USHOP_SLOT_X_NUM
			&& y >= m_my_slotstart_y
			&& y < m_my_slotstart_y + USHOP_SLOT_HEIGHT*USHOP_SLOT_Y_NUM )
		return TRUE;

	return FALSE;
}

BOOL CControlUserShop::IsInOutBtnInside( int x, int y ) // �������̽� ���� �ۿ� �ִ� ��/�ƿ� ��ư��ġ�˻�.
{
	if( m_States == USHOP_STATES_NORMAL ) // In üũ.
	{
		if (x >= m_InBtn.PosX
				&& x < m_InBtn.PosX + m_InBtn.Width
				&& y >= m_InBtn.PosY
				&& y < m_InBtn.PosY + m_InBtn.Height )
			return TRUE;
	}
	else if( m_States == USHOP_STATES_HIDE ) // Out üũ
	{
		if (x >= m_OutBtn.PosX
				&& x < m_OutBtn.PosX + m_OutBtn.Width
				&& y >= m_OutBtn.PosY
				&& y < m_OutBtn.PosY + m_OutBtn.Height )
			return TRUE;
	}

	return FALSE;
}

CItem* CControlUserShop::GetOverItem( int x, int y ) // Ư�� ������ ���� ���콺�� �ö󰬴����� üũ.
{
	static CItem *OldOverItem = NULL; // ���� OverItem�� ������. ���� ���� ���ο�� Ʋ���ٸ� InvenŬ������ DrawItemInfo�� ���� �÷��׸� �������ش�. �� ������ DrawItemInfo�ȿ��� �ε����� ���� ������������ ���ϴµ� ������ ��ϵ� �����۵��� �ε����� �Ⱦ��⶧��...

	if( IsSlotInside(x,y) ) // ���Ծȿ� �����Ͱ� �ִٸ�...
	{
		// ��ü���� ���� ��ġ�� ���´�.
		int slot_x_num, slot_y_num;

		// ���� ���� ����
		slot_x_num = (x - m_my_slotstart_x) / USHOP_SLOT_WIDTH;
		slot_y_num = (y - m_my_slotstart_y) / USHOP_SLOT_HEIGHT;

		if( OldOverItem != m_pSellItem[m_iNowPack][slot_x_num][slot_y_num] ) // �ּҰ� �ٲ���ٸ�..
		{
			if( pCMyApp && g_pNk2DFrame && g_pNk2DFrame->GetControlInven() )
				g_pNk2DFrame->GetControlInven()->m_bDrawInfoUpdate = TRUE; // �̰����ָ� ������ �������ش�.

			OldOverItem = m_pSellItem[m_iNowPack][slot_x_num][slot_y_num];
		}

		return m_pSellItem[m_iNowPack][slot_x_num][slot_y_num];
	}

	return NULL;
}

BOOL CControlUserShop::SlotIn( int SlotPosNum, int RedLime,int PlatinumLime , int GoldLime, int SilverLime, int BronzeLime, int Price, int InvenPack, int InvenX, int InvenY ) // �κ����� ���� �������� ����.(�Ѱ��� �־��ִ� �Լ���. ��Ŷ�� ���� �������� ī��Ʈ ��ŭ �� �Լ��� ȣ��ȴ�.)(���������� �������� ���� ������ ó����.)
{
	// SlotPosNum �� ���� ���Կ� InvenPack�� InvenX,InvenY�� �������� �ִ´�.
	int x = 0, y = 0, pack_num = 0, slot_num = 0;
	CItem *pItem=NULL;
	int InItemIndex = 0; // ���� �������� �ε���.

	if( InvenPack == -1 ) // ����Ʈ�� ������ ���� �ִ� ���.
	{
		if( !g_pRoh->m_ExtraSlot ) // ����Ʈ�� ������ ������ ���� �߸��ȰǴ�.
			return FALSE;

		pItem = g_pRoh->m_ExtraSlot; // �����Ѵ�.
	}
	else // �κ��� ���� �ִ� ���(������ ��� �̷��� �´�)
	{
		InItemIndex = g_pRoh->GetItemIndexInSlot( InvenPack, InvenX, InvenY, 1, 1 ); // ������ ������ ������ 1*1¥���� �ȴ�.

		if( InItemIndex == -1 )
			return FALSE;

		pItem = g_pRoh->FindItemByIndexFromAll( InItemIndex ); // �̰� �κ����� �ƴ϶� extra�� ��ȯ���Ե� ��� üũ�ع����� ����.
	}

	// ���Գ����� �ε��� ���
	// ���Գ��� ������ ( ���� ���������� ���� ������(?)���� ���� )
	pack_num = SlotPosNum / (USHOP_SLOT_X_NUM*USHOP_SLOT_Y_NUM);
	slot_num = SlotPosNum % (USHOP_SLOT_X_NUM*USHOP_SLOT_Y_NUM);

	// ���� ������ ��ǥ ���
	x = slot_num / USHOP_SLOT_Y_NUM;
	y = slot_num % USHOP_SLOT_Y_NUM;

	if( m_SlotItemCount[pack_num][x][y] == 0 ) // ����ִ� ���Կ� �ִ� ���
	{
		if( m_pSellItem[pack_num][x][y] ) // Ȥ�ö� �������� �� �ִٸ� ���� �߸��ȰŴ�.
			return FALSE;

		m_pSellItem[pack_num][x][y] = new CItem; // ���� �����...

		if( !m_pSellItem[pack_num][x][y] )
			return FALSE;

		m_pSellItem[pack_num][x][y]->m_SlotX = x;
		m_pSellItem[pack_num][x][y]->m_SlotX = x;
		m_pSellItem[pack_num][x][y]->m_SlotY = y;

		m_pSellItem[pack_num][x][y]->m_Index = pItem->m_Index;
		m_pSellItem[pack_num][x][y]->m_Vnum = pItem->m_Vnum;
		m_pSellItem[pack_num][x][y]->m_ProtoNum = g_ItemList.FindItem(pItem->m_Vnum);

		m_pSellItem[pack_num][x][y]->m_PlusNum = pItem->m_PlusNum; // �̰� �̹� ���̾�,���̾� �ƴԵ��� ���� ���̴�. �״�� �����ϸ�ȴ�.
		m_pSellItem[pack_num][x][y]->m_GemNum = pItem->m_GemNum; // �̰͵� ����������Ѵ�.(���� ���� �ܿ� ����)

		m_pSellItem[pack_num][x][y]->m_Special = pItem->m_Special;

		///�Ź���ȭ�ý���
		m_pSellItem[pack_num][x][y]->m_Special2 = pItem->m_Special2; // �߰� �ɼ�
		m_pSellItem[pack_num][x][y]->m_UpgradeEndurance = pItem->m_UpgradeEndurance;           // ���� ��ȭ ������
		m_pSellItem[pack_num][x][y]->m_MaxUpgradeEndurance = pItem->m_MaxUpgradeEndurance;    // ��ȭ ������ �ִ�ġ

		m_pSellItem[pack_num][x][y]->m_NeedPlatinumLime = PlatinumLime;
		m_pSellItem[pack_num][x][y]->m_NeedRedLime = RedLime;
		m_pSellItem[pack_num][x][y]->m_NeedGoldLime = GoldLime; // �ǸŰ����� BP������ �۾��� Ư���������ݿ� �����Ѵ�. // �ǸŰ����� �������� �����ϸ� �ȴ�.
		m_pSellItem[pack_num][x][y]->m_NeedSilverLime = SilverLime; // �ǸŰ����� BP������ �۾��� Ư���������ݿ� �����Ѵ�. // �ǸŰ����� �������� �����ϸ� �ȴ�.
		m_pSellItem[pack_num][x][y]->m_NeedBronzeLime = BronzeLime; // �ǸŰ����� BP������ �۾��� Ư���������ݿ� �����Ѵ�. // �ǸŰ����� �������� �����ϸ� �ȴ�.
		m_pSellItem[pack_num][x][y]->m_ExPrice = Price; // �ǸŰ����� BP������ �۾��� Ư���������ݿ� �����Ѵ�. // �ǸŰ����� �������� �����ϸ� �ȴ�.

		// ������ �Ӽ��� ����
		// ������ ���� �Է��غ���!
		ResetItemValue( m_pSellItem[pack_num][x][y] );
		SetSpecialValue( m_pSellItem[pack_num][x][y], GET_TYPE(m_pSellItem[pack_num][x][y]), GET_SHAPE(m_pSellItem[pack_num][x][y]), pItem->m_Special, pItem->m_Special );

		m_SlotItemCount[pack_num][x][y] = 1;
	}
	else // �� ��� �ִ� ���Կ� �ִ� ���(���λ��������� ������ �������ϱ�� �ߴ�. ��, �Ⱥ���ִ°�� ������ ������ �������̶� �������ų� �ƴϸ� �ƿ� �ȵ��°Ŵ�.)
	{
		if( !m_pSellItem[pack_num][x][y] ) // �� ���� ������ �߲ٴ�.
			return FALSE;

		if( !m_pSellItem[pack_num][x][y]->IsSameItem(pItem) ) // ���� ������ ���� ��.
			return FALSE;

		if( m_SlotItemCount[pack_num][x][y] > MAX_COUNT_IN_ONESLOT-1 ) // ���� ���ִ� �Ѱ�
			return FALSE;

		m_pSellItem[pack_num][x][y]->m_SlotX = x;
		m_pSellItem[pack_num][x][y]->m_SlotY = y;

		++m_SlotItemCount[pack_num][x][y]; // ���� �����ϰ�.
	}

	// �������� �����. ���� �����͸� ����� �ȵɰ� ����. �̷��� ����� ���� ������ ������ NULL ������ �ȵ����ʳ�?
	if( InvenPack == -1 ) // ����Ʈ�� ������ ���� �ִ� ���.
	{
		SAFE_DELETE( g_pRoh->m_ExtraSlot ); // �����.
	}
	else // �κ��� ���� �ִ� ���(������ ��� �̷��� �´�)
	{
		g_pRoh->RemoveByIndex( InItemIndex );
		g_pRoh->CountQuickAll(); // ������ �������� �־��� ���� ������ �ٽ� �����ش�.
	}

	char tmp[10] = "";
	// ���Կ� �������� ������ ���԰��� ���
	if( m_SlotItemCount[pack_num][x][y] > 0 )
	{
		sprintf( tmp, "%d", m_SlotItemCount[pack_num][x][y] );
		m_CountText[pack_num][x][y].SetString( tmp );
	}

	return TRUE;
}

BOOL CControlUserShop::SlotOut( int SlotPosNum, int Count ) // ���� ���Կ��� �κ����� ����. (���� �������� �̵��� �������� inven ��Ŷ���� �����ش�. ���⼱ ���� �������� ī��Ʈ ���Ÿ� ���ش�.)
{
	int x = 0, y = 0, pack_num = 0, slot_num = 0;
	CItem *pItem=NULL;

	// ���Գ����� �ε��� ���
	// ���Գ��� ������ ( ���� ���������� ���� ������(?)���� ���� )
	pack_num = SlotPosNum / (USHOP_SLOT_X_NUM*USHOP_SLOT_Y_NUM);
	slot_num = SlotPosNum % (USHOP_SLOT_X_NUM*USHOP_SLOT_Y_NUM);

	// ���� ������ ��ǥ ���
	x = slot_num / USHOP_SLOT_Y_NUM;
	y = slot_num % USHOP_SLOT_Y_NUM;

	if( m_SlotItemCount[pack_num][x][y] == 0 || !m_pSellItem[pack_num][x][y] ) // ����ִ� ������ �ų���� �ѰŸ�...�߲���?
		return FALSE;
	else
	{
		char tmp[10] = "";

		// ���� ������ ī��Ʈ ���̱�.
		m_SlotItemCount[pack_num][x][y] -= Count; // ������ ī��Ʈ ��ŭ ����.

		if( m_SlotItemCount[pack_num][x][y] <= 0 ) // ���� �� ī��Ʈ�� 0�� �ȰŶ��...
		{
			SAFE_DELETE( m_pSellItem[pack_num][x][y] ); // �����.
			m_CountText[pack_num][x][y].SetString( "" ); // ī��Ʈ ��Ʈ���� �ʱ�ȭ.
			sprintf( tmp, "" ); // ī��Ʈ ��Ʈ���� ����.

			m_pOverItem = NULL; // �̰Ŷ��� ������ ��쵵 ������ �ݵ�� �ʱ�ȭ����.
		}
		else
			sprintf( tmp, "%d", m_SlotItemCount[pack_num][x][y] ); // ī��Ʈ ��Ʈ���� ����.

		m_CountText[pack_num][x][y].SetString( tmp );
	}

	return TRUE;
}

// [6/1/2007 Theodoric] ������ ���� :: �Լ� �Ķ���� ������ ����
// SlotIn���� �޸� �Ϲ������� ���Կ� �����ϴ� �Լ�. �ش� ������ ������ִٴ°� �����Ͽ� �Ѵ�.
void CControlUserShop::SetSlotItem( int SlotPosNum, int vnum, int plus, int special, int special2,
									int UpEndur, int MaxUpdEndur, int Endur, int MaxEndur ,
									int RedLime, int PlatinumLime, int GoldLime, int SilverLime, int BronzeLime, int price, int count) // ������ ������ ����.
{
	int x = 0, y = 0, pack_num = 0, slot_num = 0;
	char tmp[10] = ""; // ī��Ʈ �Է¿� ��Ʈ�� ����.

	// ���Գ����� �ε��� ���
	// ���Գ��� ������ ( ���� ���������� ���� ������(?)���� ���� )
	pack_num = SlotPosNum / (USHOP_SLOT_X_NUM*USHOP_SLOT_Y_NUM);
	slot_num = SlotPosNum % (USHOP_SLOT_X_NUM*USHOP_SLOT_Y_NUM);

	// ���� ������ ��ǥ ���
	x = slot_num / USHOP_SLOT_Y_NUM;
	y = slot_num % USHOP_SLOT_Y_NUM;

	SAFE_DELETE( m_pSellItem[pack_num][x][y] ); // Ȥ�� �𸣴ϱ� ������ �����.
	m_pOverItem = NULL; // �̰Ŷ��� ������ ��쵵 ������ �ݵ�� �ʱ�ȭ����.

	// �������� �����Ͽ� �ִ´�.
	m_pSellItem[pack_num][x][y] = new CItem; // ���� �����...
	if( !m_pSellItem[pack_num][x][y] )
		return;

	m_pSellItem[pack_num][x][y]->m_SlotX = x;
	m_pSellItem[pack_num][x][y]->m_SlotY = y;

	m_pSellItem[pack_num][x][y]->m_Vnum = vnum;
	m_pSellItem[pack_num][x][y]->m_ProtoNum = g_ItemList.FindItem( vnum );

	if( GET_TYPE(m_pSellItem[pack_num][x][y] ) != ITYPE_SUB )
	{
		if( g_ItemList.m_ItemArray[m_pSellItem[pack_num][x][y]->m_ProtoNum].m_MinLevel >= 181 )
			m_pSellItem[pack_num][x][y]->SetDiaGemNum( plus ); 	// ���̾Ʊ� ���� : ���̾Ʊ��� �����ִ� gem�Ӽ��� ���� ���̾� ������ �ٷ� �÷������̴�.
		else
			m_pSellItem[pack_num][x][y]->SetGemNum( plus );	// �׿� ���ѿ����� �÷����� ����
	}
	else
		m_pSellItem[pack_num][x][y]->m_PlusNum = plus; // ���� �������� ���� ������ �÷��� ��

	m_pSellItem[pack_num][x][y]->m_Special = special;

	///�Ź���ȭ�ý���
	m_pSellItem[pack_num][x][y]->m_Special2 = special2; // �߰� �ɼ�
	m_pSellItem[pack_num][x][y]->m_UpgradeEndurance = UpEndur;           // ���� ��ȭ ������
	m_pSellItem[pack_num][x][y]->m_MaxUpgradeEndurance = MaxUpdEndur;    // ��ȭ ������ �ִ�ġ

	m_pSellItem[pack_num][x][y]->m_Endurance = Endur;
	m_pSellItem[pack_num][x][y]->m_MaxEndurance = MaxEndur;

	m_pSellItem[pack_num][x][y]->m_NeedPlatinumLime = PlatinumLime;
	m_pSellItem[pack_num][x][y]->m_NeedRedLime = RedLime;
	m_pSellItem[pack_num][x][y]->m_NeedGoldLime = GoldLime;
	m_pSellItem[pack_num][x][y]->m_NeedSilverLime = SilverLime;
	m_pSellItem[pack_num][x][y]->m_NeedBronzeLime = BronzeLime;

	m_pSellItem[pack_num][x][y]->m_ExPrice = price;

	// ������ �Ӽ��� ����
	// ������ ���� �Է��غ���!
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

	// �Էµ� ��Ʈ���� �˻��Ѵ�
	int len = strlen(szMoney);

	for(int i=0; i<len; i++)
	{
		if(isdigit(szMoney[i]) == FALSE)
		{
			if(is_space(szMoney[i]) ==TRUE)
				//-- IDS_WAREHOUSE_BLANK : ���鹮�ڴ� ������� �ʽ��ϴ�.
				g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_WAREHOUSE_BLANK), TYPE_EXCHANGE_EX, 18 );
			else
				//-- IDS_WAREHOUSE_NOTNUM : �߸��� �ݾ��Դϴ�. �ٽ� �Է��ϼ���
				g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_WAREHOUSE_NOTNUM), TYPE_EXCHANGE_EX, 18 );

			return FALSE;
		}
	}

	// �ݾ� ���� üũ.
	nMoney = _atoi64( szMoney ); // int �� ȯ���� ��

	if( nMoney < 0 || nMoney > LIME_LIMIT ) // -�ų� �Ѱ�ġ�� �Ѿ�����...
	{
		//-- IDS_WAREHOUSE_NOTNUM : �߸��� �ݾ��Դϴ�. �ٽ� �Է��ϼ���
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
	char tmp[20] = ""; // ī��Ʈ �Է¿� ��Ʈ�� ����.

	if( SlotPosNum != -1 ) // -1�� ī��Ʈ �������� �ݾ׸� ���� ���.(�ڱ� ���� ���� ���½� ��...)�̴�.
	{
		// ���Գ����� �ε��� ���
		// ���Գ��� ������ ( ���� ���������� ���� ������(?)���� ���� )
		pack_num = SlotPosNum / (USHOP_SLOT_X_NUM*USHOP_SLOT_Y_NUM);
		slot_num = SlotPosNum % (USHOP_SLOT_X_NUM*USHOP_SLOT_Y_NUM);

		// ���� ������ ��ǥ ���
		x = slot_num / USHOP_SLOT_Y_NUM;
		y = slot_num % USHOP_SLOT_Y_NUM;

		m_SlotItemCount[pack_num][x][y] -= SelledCount; // ���� ī��Ʈ�� �ȸ���ŭ ���ش�.

		if( m_SlotItemCount[pack_num][x][y] <= 0 ) // ���ȷȴٸ�...
		{
			m_SlotItemCount[pack_num][x][y] = 0;
			SAFE_DELETE( m_pSellItem[pack_num][x][y] ); // ����������.
			sprintf( tmp, "" ); // ī��Ʈ ��Ʈ���� ����.

			m_pOverItem = NULL; // �̰Ŷ��� ������ ��쵵 ������ �ݵ�� �ʱ�ȭ����.

			// ���ȷ����� �ٸ� ������ �˻��ؼ� �ٸ� ���Ա��� �� �ȷȴ��� �˻��Ѵ�.
			bool bAllSelled = TRUE;

			for( int k = 0 ; k < USHOP_PACK_NUM ; ++k )
			{
				for( int i = 0 ; i < USHOP_SLOT_X_NUM ; ++i )
				{
					for( int j = 0 ; j < USHOP_SLOT_Y_NUM ; ++j )
					{
						if( m_SlotItemCount[k][i][j] ) // ī��Ʈ�� ������..
							bAllSelled = FALSE; // ���ǰ� �ƴϴ�.
					}
				}
			}

			if( bAllSelled ) // �� �ȷȴٸ�...
			{
				if( pCMyApp && pCMyApp->m_pUIMgr ) // �޼��� ������ش�.
					//-- IDS_ITEM_ALL_SELLED : �������� ���� �ǸŵǾ����ϴ�.
					pCMyApp->m_pUIMgr->CmdErrorMsg( (char*)G_STRING(IDS_ITEM_ALL_SELLED) );
			}
		}
		else
			sprintf( tmp, "%d", m_SlotItemCount[pack_num][x][y] ); // ī��Ʈ ��Ʈ���� ����.

		m_CountText[pack_num][x][y].SetString( tmp );
	}


	if( (m_SelledPlatinumLime!=TotalSelledPlatinumLime) || (m_SelledRedLime!=TotalSelledRedLime) || (m_SelledGoldLime!=TotalSelledGoldLime) 
		|| (m_SelledSilverLime!=TotalSelledSilverLime) || (m_SelledBronzeLime!=TotalSelledBronzeLime) || (m_SelledLime!=TotalSelledMoney) ) // �ٲ� ��ġ�� �ִٸ�...
	{
		CMultiText multiText;
		char strTemp[256];
		char strTemp2[256];
		int CommaPos = 0; ///����ǥ�ð���
		// Red Line
		sprintf( strTemp, "Sold red bar:%d", TotalSelledRedLime );
		multiText.AddString( strTemp, RGB(255,0,0) );
		
		// Red Line
		sprintf( strTemp, "Sold platinum bar:%d", TotalSelledPlatinumLime );
		multiText.AddString( strTemp, RGB(255,165,0) );
		//-- IDS_SELLED_GOLDLIME : �ȸ� �ݱ�: %d
		sprintf( strTemp, G_STRING(IDS_SELLED_GOLDLIME), TotalSelledGoldLime );
		multiText.AddString( strTemp, RGB(234,236,22) );
		//-- IDS_SELLED_SILVERLIME : �ȸ� ����: %d
		sprintf( strTemp, G_STRING(IDS_SELLED_SILVERLIME), TotalSelledSilverLime );
		multiText.AddString( strTemp, RGB(122,220,122) );
		//-- IDS_SELLED_BRONZELIME : �ȸ� ����: %d
		sprintf( strTemp, G_STRING(IDS_SELLED_BRONZELIME), TotalSelledBronzeLime );
		multiText.AddString( strTemp, RGB(255,152,57) );
		//-- IDS_SELLED_LIME : �ȸ� ����: %I64d
		sprintf( strTemp, G_STRING(IDS_SELLED_LIME), TotalSelledMoney );
		CommaPos = InsertPriceComma( strTemp2, strTemp, ':' ); // �޸��� ���� ��Ʈ������ �ٲ۴�.
		multiText.AddString( strTemp2,  RGB(255,255,255) ); ///����ǥ�ð���

		if( g_dwClientCountry == CTRY_KOR && TotalSelledMoney != 0 ) // ������ 0�̸� ǥ�� ���ϵ��� ���� 07-02-27 ����
		{
			ConvertMoneyToString( strTemp2, TotalSelledMoney, CommaPos ); ///����ǥ�ð���
			multiText.AddString(strTemp2, RGB(255,255,255)); ///����ǥ�ð���
		}

		m_pSelledPriceInfo->PutString(&multiText);

		__int64 TempMoney = 0; // ���Ӵ��� �ݾ�ȯ���.

		int PlatinumLime_price = 0, RedLime_price = 0, GoldLime_price = 0, SileverLime_Price = 0, BronzeLime_Price = 0, proto_num = 0; // �ݱ�,����,������ ������ �˾ƿ������� �ʿ��� �༮��
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

		// �Ǹűݾ� ����.
		sprintf( tmp, "%I64d", TempMoney ); // ������ �� �Ǹűݾ��� �´�.
		m_SellMoneyText.SetString( tmp );

		// ����.
		m_SelledRedLime = TotalSelledRedLime;  // Red Bullion
		m_SelledPlatinumLime = TotalSelledPlatinumLime; // Platinum Bullion
		m_SelledGoldLime = TotalSelledGoldLime;
		m_SelledSilverLime = TotalSelledSilverLime;
		m_SelledBronzeLime = TotalSelledBronzeLime;
		m_SelledLime = TotalSelledMoney;
	}

}

BOOL CControlUserShop::IsPlayArea( int x, int y ) // �������̽��� ��� ���� Ŭ���ߴ����� üũ.(�κ��� �����ϰ� üũ.)
{
	if( !IsInside(x,y) && !g_pNk2DFrame->IsInPopup(x,y) )
		return true;

	return false;
}

BOOL CControlUserShop::IsInsideSelledMoney( int x, int y ) // �Ǹ� �ݾ׶� üũ.
{
	if( !m_pBackSur || !m_pSelledMoneyBack )
		return FALSE;

	return m_pSelledMoneyBack->IsIn(x, y);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// ���� ���� �˻��� �������̽�
CControlUShopSearch::CControlUShopSearch()
	: m_hWnd(NULL)
{
	m_pBackSur = NULL; // �������̽� ���
	m_pDetailInfoAlpha = NULL; // �˻��� �������� ���� ����(��,��ǥ)���� �ڼ��� �˷��ֱ����� �˾�.
	m_pItemImageAlpha = NULL; // ������ �̹��� ����.

	m_ResultList.reserve( 100 ); // �ִ� 100�� ������ �����ֱ�� ����.

	Init(); // ������ �ʱ�ȭ.
}

CControlUShopSearch::~CControlUShopSearch()
{
	DeleteRes();

	ClearList(); // ����Ʈ�� ��������.
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
	m_SearchNameInput.ClearText();   // ã�� ���ڿ��� �Է��ϴ� �༮.
	m_ViewListNum = 0;
	m_ClickTick = 0; // ã�⸦ ���� ������ ���ϰ�...
	m_bDrawInfoAlpha = FALSE;
	m_bNowSearching = FALSE;
	m_NowSearchTick = 0;
	ZeroMemory( m_strSearchName, sizeof(m_strSearchName) );
	// ���� ������ �ʱ�ȭ.
	m_pItemRender = NULL;
	m_pRefSelOnItem = NULL;
}

void CControlUShopSearch::DeleteRes()
{
	ClearList(); // ����Ʈ�� ��������.
	m_CharNameText.DeleteRes();
	m_ItemNameText.DeleteRes();
	m_NowSearchingText.SetString( _T(" "), true);
	m_ListScroll.DeleteRes();
	m_CloseBtn.DeleteRes();
	m_SearchBtn.DeleteRes();
	m_SearchNameInput.DeleteRes();     // ���������� ����� ��Ʈ ��¿� ���ǽ�.
	SAFE_DELETE_RES( m_pDetailInfoAlpha );
	SAFE_DELETE_RES( m_pItemImageAlpha );
	SAFE_SURFACE_DESTROY( m_pBackSur );
}

void CControlUShopSearch::LoadRes(CItemRender *pItemRender)
{
	if( !g_pDisplay  )
		return;

	int k = 0, x = 0, y = 0;

	Init(); // �������� �ѹ� �ʱ�ȭ.

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
	m_ListScroll.SetRange( MAX_USHOP_SEARCH_NUM + 1 - m_ViewListNum ); // +1 ����� �´���.
	m_ListScroll.SetDrawPos( m_pBackSur->Xpos+261, m_pBackSur->Ypos+126, 358 ); // ����� ��ü���⿡���� �����ϴ� ��ü���⸸ ��ǥ�� ���� �������ش�.


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

	m_SearchNameInput.SetEnable( TRUE ); // �ٷ� �Է��� ���ֵ��� ����.
	pCMyApp->RestoreImmStatus(); ///�Ϻ������ݰ����׼���
}

void CControlUShopSearch::Draw() // �� �Լ��� ������ DrawItemInfo()�Լ����� ǥ�õǴ� ������ ������ �����ϱ� ���� ���δ�.
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
			m_NowSearchingText.Draw(); // �����̸� ��µǵ���...

		if( timeGetTime()-m_NowSearchTick > 5000 ) // 5���Ŀ� �׸� �����̰� �׸� �ް�.
		{
			if( g_pNk2DFrame->GetUserShopSearch()->m_ResultList.size() <= 0 ) // ���� ����Ʈ�� ���ٸ�...
			{
				// �����޼��� ���
				char StrTemp[256];
				sprintf( StrTemp, (char*)G_STRING(IDS_LHSTRING1856) );

				pCMyApp->m_pUIMgr->CmdErrorMsg( StrTemp );
			}

			g_pNk2DFrame->GetUserShopSearch()->m_bNowSearching = FALSE; // �� ã���ִ��� ǥ�ÿ�...
		}
	}

	// �����ۿ� ���� �ڼ��� ������ ǥ�����ش�.
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

			// ������ ����
			if( g_pNk2DFrame && g_pNk2DFrame->GetControlInven() )
			{
				g_pNk2DFrame->GetControlInven()->m_pItemInfo->SetAlphaPopup_Type(AlphaPopUp_Type__Char);
				g_pNk2DFrame->GetControlInven()->DrawItemInfo( m_pRefSelOnItem, ITEM_INFO_INVEN );

				ItemInfoWidth = g_pNk2DFrame->GetControlInven()->m_pItemInfo->GetSurWidth();
				ItemInfoHeight = g_pNk2DFrame->GetControlInven()->m_pItemInfo->GetSurHeight();

				ItemInfoPosX = g_pNk2DFrame->GetControlInven()->m_pItemInfo->GetXpos();
				ItemInfoPosY = g_pNk2DFrame->GetControlInven()->m_pItemInfo->GetYpos() + ItemInfoHeight;
			}

			// ������ �̹���
			if( m_pItemRender )
			{
				// ��� �׸���.
				CMultiText multiText;
				char strTemp[256];
				//-- IDS_IMAGE :   IMAGE
				sprintf(strTemp,(char*)G_STRING(IDS_IMAGE) );
				multiText.AddString(strTemp, _FCOLOR_YELLOW);

				for( int k = 0 ; k < 6 ; ++k )
				{
					sprintf(strTemp, " " ); // ������� �־ ���̸� ����.
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
				ImageWidth = SLOT_WIDTH*2; // �ѷ� �̹����� ȭ�� ���� ����.(������ 2���� ���� ���̷� ����.)(���� ���ΰ� 2������ �Ѵ°� �����Ƿ�.

				if( SelOnItemLolo )
				{
					g_pNk2DFrame->RenderItemInUI(ItemInfoPosX-ImageWidth-10, ItemInfoPosY-ItemInfoHeight+10,
												 m_pRefSelOnItem , FALSE , FALSE , TRUE , TRUE );
				}

				m_pItemRender->EndRenderLolo();
			}

			// �Ǹ� ����
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
		if( m_ResultList.size() > i ) // �ش� ��ġ�� ����� �������� ���
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
	int ListSelOn = -1; // ���콺�� ����Ű���ִ� ����Ʈ �ε���.
	static int OldListSelOn = -1; // �� ����Ű�� �ε��� ����. �ٲ�������� �����Ѵ�.
	BOOL bInputEnable = FALSE; // �Է�â ��Ŀ�� ��ȯ üũ�� ///�Ϻ������ݰ����׼���

	if( m_ListScroll.MsgProc(hWnd, msg, wParam, lParam) == S_OK )
		return 1;

	if( m_SearchNameInput.GetEnable() && g_pNk2DFrame->GetControlBottom()->HasFocus() ) // ä���Է�â�� ��Ŀ���� ���ִٸ�... ///�Ϻ������ݰ����׼���
	{
		m_SearchNameInput.SetEnable( FALSE ); // ������ ��Ŀ���� Ǯ�������.(ó���� �ָ��ؼ� �̷��� �����ϰ� ��)
		pCMyApp->ConvertImmStatus(); // �ؽ�Ʈ �Է� ��Ŀ���� ���� �̰� ������ϴ� ��. ///�Ϻ������ݰ����׼���
	}

	switch (msg)
	{
	case 0x020A: // �ٸ��콺 �޼���(����Ʈ �����ȿ����� �� �޼����� ������ ��ũ�ѿ� ���������ش�.)

		if( IsListInside(x,y) ) // ����Ʈ ���������� ó��
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

		bInputEnable = m_SearchNameInput.GetEnable(); // ����صξ��ٰ�... ///�Ϻ������ݰ����׼���

		m_SearchNameInput.MsgProc( hWnd, msg, wParam, lParam ); // ��Ŀ���� ���� �Ѱ� ���ؼ�...

		if( bInputEnable == FALSE && m_SearchNameInput.GetEnable() ) // �����ִ� ���¿��� �����ٸ�... ///�Ϻ������ݰ����׼���
			pCMyApp->RestoreImmStatus(); // �̰� ���ָ� �Ϻ����� ���� ���� �ݰ� ���װ� ������� ������ �ʹ�.

		if( IsInside(x,y) ) // �������̽��� ������ Ŭ������ ���.
		{
			m_SearchBtn.MsgProc(hWnd, msg, wParam, lParam);
			m_CloseBtn.MsgProc(hWnd, msg, wParam, lParam);

			return 1;
		}
		else if( IsPlayArea(x,y) ) // ���� �������� �������̽��� �ݴ´�..
			g_pNk2DFrame->ToggleUserShopSearchWindow( TRUE ); // Toggle �Լ��� ����ؼ� ���� �����Ų��.

		return 0; // �ٸ� �������̽� �κ��� ����� ���...(�κ�,Bottom��...)

	case WM_MOUSEMOVE:

		ListSelOn = IsListInside( x, y );

		if( OldListSelOn != ListSelOn ) // ���콺�� �ö� ����Ʈ�� �ִٸ�...
		{
			CMultiText multiText;
			char strTemp[256];
			char strTemp2[256];
			char strTemp3[256];
			int CommaPos = 0; ///����ǥ�ð���

			if( ListSelOn == -1 ) // ���ÿ����ȿ� �ִٰ� ������ �������
				m_bDrawInfoAlpha = FALSE;
			else if( m_ResultList.size() > ListSelOn )
			{
				m_pRefSelOnItem = &(m_ResultList[ListSelOn]->Item); // �׷��ֱ����ؼ� ���������͸� ������ �ִ´�.

				g_pNk2DFrame->GetControlInven()->m_bDrawInfoUpdate = TRUE;

				//-- IDS_WHAT_ZONE : ���� [%s]
				strcpy( strTemp2, (char*)G_STRING(IDS_WHAT_ZONE) ); // sprintf �ѹ��� G_STRING ��ũ�� �ι����� ����� �ȵ�����. �׷��� ����� ����.

				switch( m_ResultList[ListSelOn]->ZoneNum ) // ����ȣ�� �̸����� �ٲ��ش�.
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
					//-- IDS_WHAT_ZONE : ���� [%s]
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

				//-- IDS_USHOP_POS : ������ǥ (%d,%d)
				sprintf( strTemp, (char*)G_STRING(IDS_USHOP_POS), ShopPos.x, ShopPos.y );
				multiText.AddString( strTemp, _FCOLOR_YELLOW);

				// ������ ���������.
				if( m_ResultList[ListSelOn]->MinPrice == m_ResultList[ListSelOn]->MaxPrice ) // �ִ� �ּҰ����� ������ ���� �����̴�.
				{
					//-- IDS_USHOP_SELL_PRICE : �Ǹ� ����: ( %I64d )
					sprintf( strTemp, (char*)G_STRING(IDS_USHOP_SELL_PRICE), m_ResultList[ListSelOn]->MaxPrice  ); // �ϳ��� ����Ѵ�.

					///����ǥ�ð���
					CommaPos = InsertPriceComma( strTemp2, strTemp, '(' ); // �޸��� ���� ��Ʈ������ �ٲ۴�.
					multiText.AddString( strTemp2, RGB(234, 236, 22) ); ///����ǥ�ð���
				}
				else
				{
					//-- IDS_USHOP_SELL_PRICE2 : �Ǹ� ����: ( %I64d ~ %I64d )
					sprintf( strTemp, (char*)G_STRING(IDS_USHOP_SELL_PRICE2), m_ResultList[ListSelOn]->MinPrice, m_ResultList[ListSelOn]->MaxPrice  );

					///����ǥ�ð���
					CommaPos = InsertPriceComma( strTemp2, strTemp, '(' ); // �޸��� ���� ��Ʈ������ �ٲ۴�.
					CommaPos = InsertPriceComma( strTemp3, strTemp2, '~' ); // �޸��� ���� ��Ʈ������ �ٲ۴�.
					multiText.AddString( strTemp3, RGB(234, 236, 22) ); ///����ǥ�ð���
				}

				//-- IDS_USHOP_SELL_COUNT : �Ǹ� ����: %d
				sprintf(strTemp, (char*)G_STRING(IDS_USHOP_SELL_COUNT), m_ResultList[ListSelOn]->Count );
				multiText.AddString(strTemp, RGB(233, 233, 233));

				m_pDetailInfoAlpha->PutString(&multiText);

				m_bDrawInfoAlpha = TRUE;
			}
			else // �׸��� ���� ���� ���.
				m_bDrawInfoAlpha = FALSE;

			OldListSelOn = ListSelOn;
		}

		if( ListSelOn == -1 )
			m_bDrawInfoAlpha = FALSE;

	case WM_LBUTTONUP:

		if( IsInside(x,y) ) // �������̽��� ������ Ŭ������ ���.
		{
			m_SearchBtn.MsgProc(hWnd, msg, wParam, lParam);
			m_CloseBtn.MsgProc(hWnd, msg, wParam, lParam);

			if( m_SearchBtn.GetState()==BTN_ACTION ) // ���� ��ư Ŭ����
			{
				if( timeGetTime() - m_ClickTick > 500 ) // �����ð� ��������..
				{
					// ��Ŷ�� ������.
					int TempInt = GetSearchVnum( m_SearchNameInput.GetStrBuf() );
					if( TempInt != -1 )
					{
						// �˻��� ��Ʈ���� ������ش�.
						char StrTemp[256];

						//-- IDS_USHOP_NOW_SEARCHING : [%s] ������ �˻� ��...
						strcpy( StrTemp, (char*)G_STRING(IDS_LHSTRING1857));


						m_NowSearchingText.SetString( StrTemp, FALSE );
						m_NowSearchingText.SetPos( m_pBackSur->Xpos+(m_pBackSur->GetWidth()/2)-(m_NowSearchingText.GetBoxWidth()/2)
												   , m_pBackSur->Ypos+(m_pBackSur->GetHeight()/2)-(m_NowSearchingText.GetBoxHeight()/2) );

						sprintf( commOutBuf, "u_shop search %d\n", TempInt ); // Vnum�� �����ش�.
						if( g_pTcpIp )
							g_pTcpIp->SendNetMessage( commOutBuf );
					}

					m_ClickTick = timeGetTime(); // ���� �޼����� ��ٸ��� �ֱ� ������ ������ ����صδ� ƽ. �����ð� �����޼����� ������ �÷��׸� Ǯ���ֱ����� �д�.(�̷��� Ǯ����� ������ �ݾƹ����� �ҵ�.)
				}
				m_SearchBtn.SetState(BTN_NORMAL);	// ��ư�� �������ͽ��� ���� Ǯ������ϳ�����.
			}

			if( m_CloseBtn.GetState()==BTN_ACTION ) // �����ݱ� ��ư Ŭ����
			{
				g_pNk2DFrame->ToggleUserShopSearchWindow( TRUE ); // Toggle �Լ��� ����ؼ� ���� �����Ų��.
				m_CloseBtn.SetState(BTN_NORMAL);	// ��ư�� �������ͽ��� ���� Ǯ������ϳ�����.
			}

			// ���콺 Ŭ���� �׻�� �̸��� �Ӹ��� ����.
			if( msg == WM_LBUTTONUP )
			{
				int Sel = IsListInside( x, y ); // ���� Move���� �����ؼ� ������ ������Ѵ�.

				if( Sel != -1 && m_ResultList.size() > Sel ) // ��������� Ŭ��..
				{
					if( m_ResultList[Sel]->CharName ) // �̸��� �ִٸ�..
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
			if( timeGetTime() - m_ClickTick > 500 ) // �����ð� ��������..
			{
				// ��Ŷ�� ������.
				int TempInt = GetSearchVnum( m_SearchNameInput.GetStrBuf() );
				if( TempInt != -1 )
				{
					// �˻��� ��Ʈ���� ������ش�.
					char StrTemp[256];

					strcpy( StrTemp, (char*)G_STRING(IDS_LHSTRING1857));

					m_NowSearchingText.SetString( StrTemp, FALSE );
					m_NowSearchingText.SetPos( m_pBackSur->Xpos+(m_pBackSur->GetWidth()/2)-(m_NowSearchingText.GetBoxWidth()/2)
											   , m_pBackSur->Ypos+(m_pBackSur->GetHeight()/2)-(m_NowSearchingText.GetBoxHeight()/2) );


					sprintf( commOutBuf, "u_shop search %d\n", TempInt ); // Vnum�� �����ش�.
					if( g_pTcpIp )
						g_pTcpIp->SendNetMessage( commOutBuf );
				}

				m_ClickTick = timeGetTime(); // ���� �޼����� ��ٸ��� �ֱ� ������ ������ ����صδ� ƽ. �����ð� �����޼����� ������ �÷��׸� Ǯ���ֱ����� �д�.(�̷��� Ǯ����� ������ �ݾƹ����� �ҵ�.)
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

int CControlUShopSearch::IsListInside(int x, int y) // ����Ʈ ���� üũ��.
{
	int OnPos = -1;

	if (x >= m_ListRect.left
			&& x < m_ListRect.right
			&& y >= m_ListRect.top
			&& y < m_ListRect.bottom)
	{
		OnPos = ( (y - m_ListRect.top-3) / USS_LIST_GAP )+ m_ListScroll.GetPos();
	}

	return OnPos; // ����Ʈ ������ �´µ� �ȿ� �������� ���� �ƴҰ��.( �ٸ� �޼��� ���ø� ���� �ʿ��ϴ� )
}

BOOL CControlUShopSearch::IsPlayArea( int x, int y ) // �������̽��� ��� ���� Ŭ���ߴ����� üũ.(�κ��� �����ϰ� üũ.)
{
	if( !IsInside(x,y) && !g_pNk2DFrame->IsInPopup(x,y) )
		return true;

	return false;
}

void CControlUShopSearch::ClearList()
{
	// ����Ʈ �ʱ�ȭ.
	for( vector<sUShopSearchData *>::iterator iter = m_ResultList.begin() ; iter != m_ResultList.end() ; ++iter )
		delete (*iter);

	m_ResultList.clear();
}

int CControlUShopSearch::GetSearchVnum( const char *SearchName ) // ã������ �������� Vnum�� �����ִ� �Լ�.
{
	if( g_dwClientCountry == CTRY_JPN && strlen(SearchName) < 2 ) // �Ϻ��� �α��� �̻� �˻������ϰ�...
	{
		//-- IDS_WAREHOUSE_NOT0 : �Է°��� �߸��Ǿ����ϴ�. �ٽ� �Է��ϼ���
		pCMyApp->m_pUIMgr->CmdErrorMsg( (char*)G_STRING(IDS_WAREHOUSE_NOT0) );
		return -1;
	}

	static int OldSearchIndex = 0; // �������� ã�Ҵ� �迭 �ε���.
	int SearchIndex = OldSearchIndex+1; // ���� ã�Ҵ��� ���� ���ŷ� ã�����ؼ� ����صξ��� �κ� ���������� �ٽ� �˻�.

	for( int i = 0 ; i < g_ItemList.m_ItemCount ; ++i ) // Ƚ���� �߿�. ���� �˻��ϴ� �ε����� �ٸ���.
	{
		if( SearchIndex > g_ItemList.m_ItemCount-1 ) // ���� // ���⼭ üũ�ؾ� �Ѵ�. �ؿ��� �ϸ� ���� OldSearchIndex+1������ ƨ����� �ִ�.
			SearchIndex = 0;

		if( g_dwClientCountry == CTRY_JPN && (g_ItemList.m_ItemArray[SearchIndex].m_AttFlag&IATT_NO_EXCHANGE) ) // �Ϻ��� ��ȯ �Ұ� �Ӽ��������� �˻��ȵǰ�..
		{
			++SearchIndex;
			continue; // �ѱ��.
		}

		const char * itemName = GET_ITEM_NAME_VNUM(g_ItemList.m_ItemArray[SearchIndex].m_Vnum);
		int ItemNameLen = strlen( itemName );
		int SearchLen = strlen( SearchName );

		for( int j = 0 ; j <= ItemNameLen-SearchLen ; ++j )
		{
			if( strncmp(&itemName[j],SearchName,SearchLen) == 0 ) // ���� �̸��� ������...
			{
				OldSearchIndex = SearchIndex; // ã�� �ε����� ����صΰ�...

				strcpy( m_strSearchName, itemName ); // �̸��� ����صд�.

				return g_ItemList.m_ItemArray[SearchIndex].m_Vnum;
			}
		}
		++SearchIndex;
	}

	// �̰��� ��ã�� ���.
	if( pCMyApp && pCMyApp->m_pUIMgr ) // �����޼��� ������ش�.
		//-- IDS_USHOP_SEARCH_FAIL_NAME : �˻�� �´� �������� �����ϴ�.
		pCMyApp->m_pUIMgr->CmdErrorMsg( (char*)G_STRING(IDS_USHOP_SEARCH_FAIL_NAME) );

	return -1;
}

HRESULT CControlUserShop::PopupMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, int nMsg, CMsgPopUp* pArgPopup)
{
	if( nMsg == 1 ) // Ȯ���� �������.
	{
		char commOutBuf[512] = "";	// Temp buffer

		switch( m_YesNoType )
		{
		case 0: // ���� �ݱ� Ȯ��
			Close();
			break;

		case 1: // ���� Ȯ��.
			// ������ ��Ŷ�� ������.
			sprintf( commOutBuf, "u_shop buy %d %d %d\n", m_TempMoveInfo.SlotIndex, m_TempMoveInfo.InvenPack, m_TempMoveInfo.Count );

			if( g_pTcpIp )
				g_pTcpIp->SendNetMessage( commOutBuf );

			m_bSrvMsgWaiting = TRUE;
			m_SrvMsgWaitTick = timeGetTime(); // ���� �޼����� ��ٸ��� �ֱ� ������ ������ ����صδ� ƽ. �����ð� �����޼����� ������ �÷��׸� Ǯ���ֱ����� �д�.(�̷��� Ǯ����� ������ �ݾƹ����� �ҵ�.)

			ZeroMemory( &m_TempMoveInfo, sizeof(sMoveItemInfo) );

			break;

		case 2: // ��� Ȯ��.

			// ������ ��Ŷ�� ������.
			sprintf( commOutBuf, "u_shop reg %d %d %d %d %d %d %d %d %d %d\n", m_TempMoveInfo.SlotIndex
					 , m_TempMoveInfo.InvenPack,  m_TempMoveInfo.RedCount,m_TempMoveInfo.PlatinumCount, m_TempMoveInfo.GoldCount, m_TempMoveInfo.SilverCount
					 , m_TempMoveInfo.CopperCount, m_TempMoveInfo.Price, m_TempMoveInfo.InvenX[0], m_TempMoveInfo.InvenY[0] ); // ī��Ʈ�� �������� �����ʿ��� �ʿ���ٰ� �ؼ� �ϴ� ����. ������ ���� ���� �Է��� ����� ����ü�󿡴� ���ܵд�.

			if( g_pTcpIp )
				g_pTcpIp->SendNetMessage( commOutBuf );

			// ����س��� ExtraSlot �����Ͱ� �ִٸ� �������ش�.
			if( m_pRefTempExtraItem ) // m_TempMoveInfo �� ����� ���� ������Ѵ�.
			{
				if( m_TempMoveInfo.InvenPack == -1 ) // �̰� -1�̸� ���� �̵��̾ƴ� �ϳ� �̵������Ŵ�. Extra������ �������ش�.
					m_pNkCha->m_ExtraSlot = m_pRefTempExtraItem;
				m_pRefTempExtraItem = NULL; // �̰� �����ָ� ������ �������?
			}

			ZeroMemory( &m_TempMoveInfo, sizeof(sMoveItemInfo) ); // �����ش�.

			m_bSrvMsgWaiting = TRUE;
			m_SrvMsgWaitTick = timeGetTime(); // ���� �޼����� ��ٸ��� �ֱ� ������ ������ ����صδ� ƽ. �����ð� �����޼����� ������ �÷��׸� Ǯ���ֱ����� �д�.(�̷��� Ǯ����� ������ �ݾƹ����� �ҵ�.)

			break;
		}
	}
	else if( nMsg == -1 ) // ��Ұ� ���� ���
	{
		if( m_YesNoType == 2 )
		{
			// ����س��� ExtraSlot �����Ͱ� �ִٸ� �������ش�.
			if( m_pRefTempExtraItem ) // m_TempMoveInfo �� ����� ���� ������Ѵ�.
			{
				if( m_TempMoveInfo.InvenPack == -1 ) // �̰� -1�̸� ���� �̵��̾ƴ� �ϳ� �̵������Ŵ�. Extra������ �������ش�.
					m_pNkCha->m_ExtraSlot = m_pRefTempExtraItem;
				m_pRefTempExtraItem = NULL; // �̰� �����ָ� ������ �������?
			}

			ZeroMemory( &m_TempMoveInfo, sizeof(sMoveItemInfo) ); // Ȯ�� �޼��� �ڽ��� �ѹ��� ����ش�.
		}

		m_YesNoType = -1;
	}
	return 0L;
}

LRESULT CControlUserShop::MoneyPopupMsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, int nMsg, CMsgPopUp *pArgPopup)
{

	if( nMsg == 1 ) // Ȯ���� �������.
	{
		if( CheckMoneyStr(pArgPopup) ) // ����� �� ���� �־��� ���
		{
			// ��� Ȯ��â�� �ϳ� �� ����.
			if( m_pRefTempExtraItem )
			{
				m_TempMoveInfo.GoldCount = atoi(pArgPopup->GetInputValue1());
				m_TempMoveInfo.SilverCount = atoi(pArgPopup->GetInputValue2());
				m_TempMoveInfo.CopperCount = atoi(pArgPopup->GetInputValue3());
				m_TempMoveInfo.PlatinumCount = atoi(pArgPopup->GetInputValue4());
				m_TempMoveInfo.RedCount = atoi(pArgPopup->GetInputValue5());
				m_TempMoveInfo.Price = atoi(pArgPopup->GetCheckPW());

				// �ּ� ������ üũ
				if( m_TempMoveInfo.GoldCount == 0 && m_TempMoveInfo.SilverCount == 0
						&& m_TempMoveInfo.CopperCount == 0 && m_TempMoveInfo.RedCount == 0 &&  m_TempMoveInfo.PlatinumCount == 0 && m_TempMoveInfo.Price < SELL_MIN_LIMIT ) // Ư�� �ݾ� ���ϸ� ��ǰ ����� �ȵǵ���...
				{
					//-- IDS_INPUT_MIN_LAIM : �ּ� 1000���� �̻� �Է����ּ���.
					g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_INPUT_MIN_LAIM), TYPE_EXCHANGE_EX, 18 );

					return 0L;
				}


				char strTemp[256] = "";
				//-- IDS_USHOP_SELL_CHECK : [%s] ���� �ݱ�:[%d] ����:[%d] ����:[%d] ����:[%d] �Ǹ�?
				sprintf( strTemp, (char*)G_STRING(IDS_USHOP_SELL_CHECK)
						 , GET_ITEM_NAME_VNUM(m_pRefTempExtraItem->m_Vnum),m_TempMoveInfo.RedCount,m_TempMoveInfo.PlatinumCount, m_TempMoveInfo.GoldCount
						 , m_TempMoveInfo.SilverCount, m_TempMoveInfo.CopperCount, m_TempMoveInfo.RedCount, m_TempMoveInfo.PlatinumCount, m_TempMoveInfo.Price );

				g_pNk2DFrame->InsertPopup(strTemp,TYPE_NOR_OKCANCLE, 6,strlen(strTemp)/2);
				m_YesNoType = 2;
			}
		}
	}
	else if( nMsg == -1 ) // ��Ұ� ���� ���
	{
		// ����س��� ExtraSlot �����Ͱ� �ִٸ� �������ش�.
		if( m_pRefTempExtraItem ) // m_TempMoveInfo �� ����� ���� ������Ѵ�.
		{
			if( m_TempMoveInfo.InvenPack == -1 ) // �̰� -1�̸� ���� �̵��̾ƴ� �ϳ� �̵������Ŵ�. Extra������ �������ش�.
				m_pNkCha->m_ExtraSlot = m_pRefTempExtraItem;
			m_pRefTempExtraItem = NULL; // �̰� �����ָ� ������ �������?
		}

		ZeroMemory( &m_TempMoveInfo, sizeof(sMoveItemInfo) );
	}

	return 0L;
}
