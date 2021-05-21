#include "stdafx.h"


#include <string.h>
#include "main.h"
#include "basicbutton.h"
#include "headers.h"
#include "Event_Ancient.h"
#include "ItemRender.h"
#include "ItemProto.h"
#include "Nk2DFrame.h"
#include "tcpipcon.h"
#include "TcpUtil.h" ///다중교환 시 추가
#include "AlphaPopup.h"
#include "textoutbox.h"
#include "SpecialItem.h"
#include "controlbottom.h"
#include "controlinven.h"
#include "msgpopup.h"
#include "mouse.h"
#include "gamble.h"
#include "alphapopup.h"
#include "g_stringmanager.h"
#include "Country.h"
#include "LHParam_10YearEvent.h"
#include "ControlExchange.h"

extern DWORD	g_dwClientCountry;

extern int g_EffectSound[];

const int BEIJING_EVENT_MAX = 24;
const int BEIJING_EVENT_GOLD_NUM = 12;
const int BEIJING_EVENT_SILVER_NUM = 8;
const int BEIJING_EVENT_BRONZE_NUM = 4;
const int GEM_STONE_2008_MAX = 24;
const int BOSSRAID_SEAL_INDEX = 2304;


//-------------------------------------------------------------------
// name : 생성자
//-------------------------------------------------------------------

CEventAncient::CEventAncient()
	: m_iType(0)
	, m_pNkCha(NULL)
	, m_my_slotstart_x(0)
	, m_my_slotstart_y(0)
{
	memset(m_MsgBuf, 0, sizeof(m_MsgBuf));

	m_pBack = NULL;
	m_pBack_Title = NULL;
	m_pBtnCancel = NULL;	 // "cancle" 버튼
	m_pBtnExchange = NULL;	 // "교환" 버튼
	m_pBtnExchangePaper = NULL;
	m_pBtnExchangeRing = NULL;
	m_pTextInfo = NULL;
	m_pAlphaPopUp = NULL;
	m_pEnableSlot = NULL;
	m_pDisableSlot = NULL;
	m_pExistSlot  = NULL;
	m_pItemRender = NULL;
	m_pOverItem = NULL;
	m_bMainActive = false;
	m_bClick = m_bMove = m_bMoveEnd = false;
	m_bExchange = false;
	m_bQuestWindow = FALSE;
	m_dwMouseClickTickTime = timeGetTime();

	m_pMsgPopUp_OkCancel = NULL;

	m_p10YearEvent	= new LHParam_10YearEvent;
}

CEventAncient::~CEventAncient()
{
	DeleteRes();
	m_pNkCha = NULL;
	if ( NULL != m_p10YearEvent )
	{
		delete m_p10YearEvent;
		m_p10YearEvent = NULL;
	}
}


#include <direct.h>
//-------------------------------------------------------------------
// naem : LoadRes
//-------------------------------------------------------------------
void CEventAncient::LoadRes(int Type, CItemRender *pItemRender)
{
	if(!g_pDisplay)
		return;

	m_pItemRender = pItemRender;
	m_bMoveEnd = FALSE;

	if(Type == 0) // 퀘스트
		m_bQuestWindow = TRUE;
	else
		m_bQuestWindow = FALSE;

	m_iType = Type;

	g_pDisplay->CreateSurfaceFromBitmap (&m_pBack, "interface/gamble/bg_gamble.bmp");

	if( !m_pBack )
		return;

	g_pDisplay->CreateSurfaceFromBitmap (&m_pEnableSlot, "interface/slot/slot_enable.bmp");
	g_pDisplay->CreateSurfaceFromBitmap (&m_pDisableSlot, "interface/slot/slot_disable.bmp");
	g_pDisplay->CreateSurfaceFromBitmap (&m_pExistSlot, "interface/slot/slot_exist.bmp");

	m_pBack->SetColorKey( TRANS_COLOR );
	m_pBack->Xpos = g_pNk2DFrame->GetClientWidth() - 291 - m_pBack->GetWidth();
	m_pBack->Ypos = 0;
	m_my_slotstart_x = m_pBack->Xpos + 35;
	m_my_slotstart_y = m_pBack->Ypos + 251;

	LoadExchange();
	m_bMainActive = TRUE;
}



//  교환시 로드 루틴
void CEventAncient::LoadExchange()
{
	if( !g_pDisplay )
		return;

	m_pBtnCancel = new CBasicButton();
	if( m_pBtnCancel )
		m_pBtnCancel->SetFileName("common/btn_close_01");	  // 취소버튼

	m_pBtnExchange = new CBasicButton();

	if( g_dwClientCountry == CTRY_TH && m_iType == EVENT_TYPE_WORLDCUP ) // 대만은 교환 버튼 다르게...
		m_pBtnExchange->SetFileName("gamble/WorldCup"); // 태국용 축구 버튼.
	else
	{
		switch( m_iType )
		{
		case COMMON_ESTIMATE:
			m_pBtnExchange->SetFileName("gamble/btn_estimate"); // 교환버튼
			g_pDisplay->CreateSurfaceFromBitmap (&m_pBack_Title, "interface/gamble/title_estimate.bmp");
			break;

		default:
			m_pBtnExchange->SetFileName("gamble/btn_trade"); // 감정버튼
			g_pDisplay->CreateSurfaceFromBitmap (&m_pBack_Title, "interface/gamble/title_exchange.bmp");
			break;
		}
	}

	if( m_pBack_Title )
		m_pBack_Title->SetColorKey(TRANS_COLOR);

	m_pBtnExchangePaper = new CBasicButton();
	if( m_pBtnExchangePaper )
		m_pBtnExchangePaper->SetFileName("gamble/btn_paper");
	m_pBtnExchangeRing = new CBasicButton();
	if( m_pBtnExchangeRing )
		m_pBtnExchangeRing->SetFileName("gamble/btn_ring");

	switch( m_iType )
	{
	case 0:
		g_pDisplay->CreateSurfaceFromBitmap(&m_pTextInfo, "interface/description/ancient/info_event_quest.bmp");
		break;
	case EVENT_TYPE_CHILDDAY:
		g_pDisplay->CreateSurfaceFromBitmap(&m_pTextInfo, "interface/description/ancient/info_event_child.bmp");
		break;
	case EVENT_TYPE_PARENTSDAY:
		g_pDisplay->CreateSurfaceFromBitmap(&m_pTextInfo, "interface/description/ancient/info_event_parentsday.bmp");
		break;
	case 3:	// 펑펑이벤트: 산할아버지255
		g_pDisplay->CreateSurfaceFromBitmap(&m_pTextInfo, "interface/description/ancient/info_event_pp.bmp");
		break;
	case 4:
		g_pDisplay->CreateSurfaceFromBitmap(&m_pTextInfo, "interface/description/ancient/info_event_cusuk.bmp");
		break;
	case 5: ///산타추가 //wan:2005-01 :크리스마스이벤트:산타할아버지351,
		g_pDisplay->CreateSurfaceFromBitmap(&m_pTextInfo, "interface/description/ancient/info_event_matrix.bmp");
		break; // 신비의 선물교환.
	case 6: //mungmae 06/02/24 3.1절 이벤트 추가
		g_pDisplay->CreateSurfaceFromBitmap(&m_pTextInfo, "interface/description/ancient/info_event_3-1jul.bmp");
		break;	// 산타
	case 7: ///대만재신이벤트
		g_pDisplay->CreateSurfaceFromBitmap(&m_pTextInfo, "interface/description/ancient/info_event_moneygod.bmp");
		break;
	case EVENT_TYPE_WORLDCUP: // 월드컵 공 교환.
		g_pDisplay->CreateSurfaceFromBitmap(&m_pTextInfo, "interface/description/ancient/info_event_WorldCupBall.bmp");
		break;
	case EVENT_TYPE_THAI_MOTHERS_DAY: // 태국 왕비 생신 이벤트.
		g_pDisplay->CreateSurfaceFromBitmap(&m_pTextInfo, "interface/description/ancient/info_event_mother_day.bmp");
		break;
	case EVENT_TYPE_THAI_XMAS_TREE: // 태국 크리스마스 트리
		g_pDisplay->CreateSurfaceFromBitmap(&m_pTextInfo, "interface/description/ancient/info_event_xmas_tree.bmp");
		break;
	case EVENT_TYPE_LAGHAIM_STRING: // Laghaim 글자 교환 ( 지금은 2000일 기념겸 해서 임시로 Laghaim2000 )
		g_pDisplay->CreateSurfaceFromBitmap(&m_pTextInfo, "interface/description/ancient/info_event_cusuk.bmp");
		break;
	case EVENT_TYPE_THAI_VALENTINE:
		g_pDisplay->CreateSurfaceFromBitmap(&m_pTextInfo, "interface/description/ancient/info_event_th_valentine.bmp");
		break;
	case EVENT_TYPE_KOR2007_VALENTINE:
		g_pDisplay->CreateSurfaceFromBitmap(&m_pTextInfo, "interface/description/ancient/info_event_valentine.bmp");
		break;
	case EVENT_TYPE_KOR2008_NEWYEAR: // else if(m_iType == EVENT_TYPE_KOR2007_NEWYEAR )
		g_pDisplay->CreateSurfaceFromBitmap(&m_pTextInfo, "interface/description/ancient/info_event_newyear.bmp");
		break;
	case EVENT_TYPE_KOR2007_WHITEDAY: // 2007.03.14 화이트
		g_pDisplay->CreateSurfaceFromBitmap(&m_pTextInfo, "interface/description/ancient/info_event_whiteday.bmp");
		break;
	case EVENT_TYPE_KOR_6YEAR: // [8/6/2007 Theodoric] 6주년 기념 이벤트
		g_pDisplay->CreateSurfaceFromBitmap(&m_pTextInfo, "interface/description/ancient/info_event_kor_6year.bmp");
		break;
	case EVENT_TYPE_KOR2007_PEPERO: // 2007.11.01 빼빼로 데이 이벤트
		g_pDisplay->CreateSurfaceFromBitmap(&m_pTextInfo, "interface/description/ancient/info_event_pepero.bmp");
		break;
	case EVENT_TYPE_STARSTONE_EXCH:// 스타스톤 교환
		g_pDisplay->CreateSurfaceFromBitmap(&m_pTextInfo, "interface/description/ancient/info_starstone_exch.bmp");
		break;
	case EVENT_TYPE_GIRASI_EXCH:
		g_pDisplay->CreateSurfaceFromBitmap(&m_pTextInfo, "interface/description/ancient/info_girasi_exch.bmp");
		break;
	case EVENT_TYPE_SK_200804:
		g_pDisplay->CreateSurfaceFromBitmap(&m_pTextInfo, "interface/description/ancient/info_sk_200804_exch.bmp");
		break;
	case EVENT_TYPE_BP_CAMPAIN_200804:
		g_pDisplay->CreateSurfaceFromBitmap(&m_pTextInfo, "interface/description/ancient/info_bp_event_20080414_exch.bmp");
		break;
	case EVENT_TYPE_LP_GIFTBOX_CHANGE_JP: //  [10/29/2008 parkmj] 일본보석함 시스템
		g_pDisplay->CreateSurfaceFromBitmap(&m_pTextInfo, "interface/description/ancient/info_LP_jewelbox_exch.bmp");
		break;
	case EVENT_TYPE_LP_GIFTBOX_CHANGE:
		g_pDisplay->CreateSurfaceFromBitmap(&m_pTextInfo, "interface/description/ancient/info_LP_jewelbox_exch.bmp");
		break;
	case EVENT_TYPE_NEW_DEFENITEM_ITEM_EXCH:
		g_pDisplay->CreateSurfaceFromBitmap(&m_pTextInfo, "interface/description/ancient/info_new_defitem_exch.bmp");
		break;

	case EVENT_TYPE_2008_BEIJING_OLYMPIC:
		g_pDisplay->CreateSurfaceFromBitmap(&m_pTextInfo, "interface/description/ancient/info_2008_beijing_exch.bmp");
		break;

	case EVENT_TYPE_2008_GEMSTONE:
		g_pDisplay->CreateSurfaceFromBitmap(&m_pTextInfo, "interface/description/ancient/info_2008_gemstone_exch.bmp");
		break;
	case EVENT_TYPE_HALLOWEEN:
		g_pDisplay->CreateSurfaceFromBitmap(&m_pTextInfo, "interface/description/ancient/info_halloween_exch.bmp");
		break;
	case EVENT_TYPE_081114_PREMIUM_EVENT:
		g_pDisplay->CreateSurfaceFromBitmap(&m_pTextInfo, "interface/description/ancient/info_LP_jewelbox_exch.bmp");
		break;

	case EVENT_TYPE_BALLOON_EXCH:
		g_pDisplay->CreateSurfaceFromBitmap(&m_pTextInfo, "interface/description/ancient/info_balloon_exch.bmp");
		break;
	case EVENT_TYPE_CUSUK_2010:
		g_pDisplay->CreateSurfaceFromBitmap(&m_pTextInfo, "interface/description/ancient/info_event_cusuk2009.bmp");
		break;

	case EVENT_TYPE_BIRTH_2010:
		g_pDisplay->CreateSurfaceFromBitmap(&m_pTextInfo, "interface/description/ancient/info_birthday_2010_1.bmp");
		break;
	case EVENT_TYPE_PRESENT_2010:
		g_pDisplay->CreateSurfaceFromBitmap(&m_pTextInfo, "interface/description/ancient/info_birthday_2010_2.bmp");
		break;

	case EVENT_TYPE_BIRTH_CELEBRATE_2010:
		g_pDisplay->CreateSurfaceFromBitmap(&m_pTextInfo, "interface/description/ancient/info_birthday_2010.bmp");
		break;

	case EVENT_LP_PRESENT_BOX:
		g_pDisplay->CreateSurfaceFromBitmap(&m_pTextInfo, "interface/description/ancient/info_10thannivesary.bmp");
		break;

	case EVENT_GOD_WEAPON_EXPERIENCE:
		g_pDisplay->CreateSurfaceFromBitmap(&m_pTextInfo, "interface/description/ancient/info_glpbox.bmp");
		break;
	case EVENT_TYPE_NGA_OPEN:
		g_pDisplay->CreateSurfaceFromBitmap(&m_pTextInfo, "interface/description/ancient/info_ngaopen.bmp");
		break;
	case EVENT_TYPE_AWARD_2010:
		g_pDisplay->CreateSurfaceFromBitmap(&m_pTextInfo, "interface/description/ancient/info_award2010.bmp");
		break;
	case EVENT_TYPE_NEWYEAR_2011:
		g_pDisplay->CreateSurfaceFromBitmap(&m_pTextInfo, "interface/description/ancient/info_rabbit_trade.bmp");
		break;
	case EVENT_TYPE_NEWYEAR_LUCKYBAG:
		g_pDisplay->CreateSurfaceFromBitmap(&m_pTextInfo, "interface/description/ancient/info_luckybag.bmp");
		break;
	case EVENT_TYPE_NATAL:
		g_pDisplay->CreateSurfaceFromBitmap(&m_pTextInfo, "interface/description/ancient/info_natal2019.bmp");
		break;
	case EVENT_TYPE_NEWYEAR_CARD:
		g_pDisplay->CreateSurfaceFromBitmap(&m_pTextInfo, "interface/description/ancient/info_card.bmp");
		break;
	case EVENT_TYPE_FORLOM_MAP:
		g_pDisplay->CreateSurfaceFromBitmap(&m_pTextInfo, "interface/description/ancient/info_forlornruinwarp.bmp");
		break;
	case EVENT_TYPE_FORLOM_DEVICE:
		g_pDisplay->CreateSurfaceFromBitmap(&m_pTextInfo, "interface/description/ancient/info_z20exchange.bmp");
		break;
	case EVENT_TYPE_FORLOM_DEVICE_DM:
		g_pDisplay->CreateSurfaceFromBitmap(&m_pTextInfo, "interface/description/ancient/info_dmitronscienst.bmp");
		break;
	case EVENT_TYPE_GREENBUD_2011:
		g_pDisplay->CreateSurfaceFromBitmap(&m_pTextInfo, "interface/description/ancient/info_event_greenbud.bmp");
		break;
	case EVENT_TYPE_NEW_SUPPORT_2011:
		g_pDisplay->CreateSurfaceFromBitmap(&m_pTextInfo, "interface/description/ancient/info_event_support.bmp");
		break;
	case EVENT_TYPE_SUMMER_2011:
		g_pDisplay->CreateSurfaceFromBitmap(&m_pTextInfo, "interface/description/ancient/info_2011summer_JPN.bmp");
		break;

	case EVENT_TYPE_10YEAR_ITEM:
		g_pDisplay->CreateSurfaceFromBitmap(&m_pTextInfo, "interface/description/ancient/info_10thannivesary.bmp");
		break;

	case EVENT_BULLION_EXCHANGE:
		g_pDisplay->CreateSurfaceFromBitmap(&m_pTextInfo, "interface/description/ancient/Info_Gold_To_Platinum.bmp");
		break;

	case EVENT_TYPE_S1TOS2:
		g_pDisplay->CreateSurfaceFromBitmap(&m_pTextInfo, "interface/description/ancient/info_s1_to_s2.bmp");
		break;

	case EVENT_TYPE_JPN_9YEAR_ITEM:
		g_pDisplay->CreateSurfaceFromBitmap(&m_pTextInfo, "interface/description/ancient/info_9thAnniversary.bmp");
		break;

	case EVENT_TYPE_PCBANG_ARUPAKA:
		g_pDisplay->CreateSurfaceFromBitmap(&m_pTextInfo, "interface/description/ancient/info_arupaka.bmp");
		break;
	}

	if( m_pBtnCancel )			m_pBtnCancel->LoadRes();
	if( m_pBtnExchange )		m_pBtnExchange->LoadRes();
	if( m_pBtnExchangePaper )	m_pBtnExchangePaper->LoadRes();
	if( m_pBtnExchangeRing )	m_pBtnExchangeRing->LoadRes();

	if( m_pBtnCancel )
		m_pBtnCancel->SetPosition(m_pBack->Xpos+214, m_pBack->Ypos+6);
	if( m_pBtnExchange )
		m_pBtnExchange->SetPosition(m_pBack->Xpos+47, m_pBack->Ypos+198);
	if( m_pBtnExchangePaper )
		m_pBtnExchangePaper->SetPosition(m_pBack->Xpos+36, m_pBack->Ypos+199);
	if( m_pBtnExchangeRing )
		m_pBtnExchangeRing->SetPosition(m_pBack->Xpos+120, m_pBack->Ypos+199);

	if( m_pTextInfo )
	{
		m_pTextInfo->Xpos = m_pBack->Xpos + 34;
		m_pTextInfo->Ypos = m_pBack->Ypos + 80;
	}
}

//-------------------------------------------------------------------
// naem : DeleteRes()
//-------------------------------------------------------------------
void CEventAncient::DeleteRes()
{
	SAFE_DELETE(m_pEnableSlot);
	SAFE_DELETE(m_pDisableSlot);
	SAFE_DELETE(m_pExistSlot);

	SAFE_DELETE(m_pMsgPopUp_OkCancel);

	DeleteExchange();
}

void CEventAncient::DeleteExchange()
{
	SAFE_DELETE(m_pBack);
	SAFE_DELETE(m_pBack_Title);
	SAFE_DELETE(m_pBtnCancel);
	SAFE_DELETE(m_pBtnExchange);
	SAFE_DELETE(m_pBtnExchangePaper);
	SAFE_DELETE(m_pBtnExchangeRing);
	SAFE_DELETE(m_pTextInfo);
}


//-------------------------------------------------------------------
// name : Draw
//-------------------------------------------------------------------
CItem* CEventAncient::Draw()
{
	if(!m_bMainActive)
		return NULL;

	m_pOverItem = NULL;

	DrawExchange();

	return m_pOverItem;
}

void CEventAncient::DrawExchange()
{
	if( !g_pDisplay )
		return;

	if( m_pBack )
		g_pDisplay->Blt(m_pBack->Xpos, m_pBack->Ypos, m_pBack);
	if( m_pBack_Title )
		g_pDisplay->Blt(m_pBack->Xpos, m_pBack->Ypos, m_pBack_Title);
	if( m_pTextInfo )
		g_pDisplay->Blt(m_pTextInfo->Xpos, m_pTextInfo->Ypos, m_pTextInfo);
	if( m_pBtnCancel )
		m_pBtnCancel->Draw();

	switch( m_iType)
	{
	case COMMON_ESTIMATE:
		m_pBtnExchangePaper->Draw();
		m_pBtnExchangeRing->Draw();
		break;
	case EVENT_TYPE_CHILDDAY:
	case EVENT_TYPE_PARENTSDAY:	// 가정의 달 이벤트
	case 3:
	case 4:
	case 5:
	case 6:
	case 7:
	case EVENT_TYPE_WORLDCUP:
	case EVENT_TYPE_THAI_MOTHERS_DAY:
	case EVENT_TYPE_THAI_XMAS_TREE:
	case EVENT_TYPE_LAGHAIM_STRING:
	case EVENT_TYPE_THAI_VALENTINE:
	case EVENT_TYPE_KOR2007_VALENTINE:
	case EVENT_TYPE_KOR2008_NEWYEAR:
	// case EVENT_TYPE_KOR2007_NEWYEAR:
	case EVENT_TYPE_KOR_6YEAR: // [8/6/2007 Theodoric] 6주년 기념 이벤트
	case EVENT_TYPE_KOR2007_WHITEDAY: // 2007.03.14 화이트
	case EVENT_TYPE_KOR2007_PEPERO: // 2007.11.01 빼빼로 데이 이벤트
	case EVENT_TYPE_STARSTONE_EXCH:
	case EVENT_TYPE_GIRASI_EXCH:
	case EVENT_TYPE_SK_200804:
	case EVENT_TYPE_BP_CAMPAIN_200804:
	case EVENT_TYPE_LP_GIFTBOX_CHANGE_JP:
	case EVENT_TYPE_LP_GIFTBOX_CHANGE:
	case EVENT_TYPE_NEW_DEFENITEM_ITEM_EXCH:
	case EVENT_TYPE_2008_BEIJING_OLYMPIC:
	case EVENT_TYPE_2008_GEMSTONE:
	case EVENT_TYPE_HALLOWEEN:
	case EVENT_TYPE_081114_PREMIUM_EVENT:
	case EVENT_TYPE_BALLOON_EXCH:
	case EVENT_TYPE_BIRTH_2010:
	case EVENT_TYPE_PRESENT_2010:
	case EVENT_TYPE_BIRTH_CELEBRATE_2010:
	case EVENT_LP_PRESENT_BOX:
	case EVENT_GOD_WEAPON_EXPERIENCE:
	case EVENT_TYPE_NGA_OPEN:
	case EVENT_TYPE_AWARD_2010:
	case EVENT_TYPE_NEWYEAR_2011:
	case EVENT_TYPE_NEWYEAR_LUCKYBAG:
	case EVENT_TYPE_NATAL:
	case EVENT_TYPE_NEWYEAR_CARD:
	case EVENT_TYPE_FORLOM_MAP:
	case EVENT_TYPE_FORLOM_DEVICE:
	case EVENT_TYPE_FORLOM_DEVICE_DM:
	case EVENT_TYPE_GREENBUD_2011:
	case EVENT_TYPE_NEW_SUPPORT_2011:
	case EVENT_TYPE_SUMMER_2011:
	case EVENT_TYPE_10YEAR_ITEM:
	case EVENT_BULLION_EXCHANGE:
	case EVENT_TYPE_S1TOS2:
	case EVENT_TYPE_JPN_9YEAR_ITEM:
	case EVENT_TYPE_PCBANG_ARUPAKA:
		m_pBtnExchange->Draw();
		break;
	}

	DrawMySlot();
}


//-------------------------------------------------------------------
// name : MsgProc
//-------------------------------------------------------------------
LRESULT CEventAncient::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int ret = 0;
	int ren = -1;
	char buf[128] = "";

	int x = LOWORD (lParam);
	int y = HIWORD (lParam);

	switch ( msg )
	{
	case WM_KEYDOWN:
		if(wParam ==VK_ESCAPE )
		{}
		break;

	case WM_CHAR:
		break;

	case WM_RBUTTONDBLCLK:
		{
			x = LOWORD( lParam );
			y = HIWORD( lParam );

			if( pCMyApp->m_bEquipLock == 1 )
				return 1;
			if( pCMyApp->m_bRingLock == 1 )
				return 1;
			if( pCMyApp->m_bBrLock == 1 )
				return 1;
			if( pCMyApp->m_bNecklaceLock == 1 )
				return 1;
			if( m_pNkCha && m_pNkCha->m_ExtraSlot )
				return 1;

			if( IsInside( x, y ) )
			{
				// 마석 업그레이드 창이 떠있을 경우
				if( g_pNk2DFrame
						&& g_pNk2DFrame->GetControlInven()
						&& g_pNk2DFrame->GetControlInven()->m_bAct_Masuk )
				{
					g_pNk2DFrame->GetControlInven()->Close_Masuk();
					return 1;
				}

				// 영역 검사
				if( m_pNkCha
						&& ( x >= m_my_slotstart_x )
						&& ( x < m_my_slotstart_x + ( SLOT_WIDTH * EXG_SLOT_X_NUM ) )
						&& ( y >= m_my_slotstart_y )
						&& ( y < m_my_slotstart_y + ( SLOT_HEIGHT * EXG_SLOT_Y_NUM ) ) )
				{
					int nSlot_X_Num = 0;
					int nSlot_Y_Num = 0;

					// 슬롯 계산
					nSlot_X_Num = ( x - m_my_slotstart_x ) / SLOT_WIDTH;
					nSlot_Y_Num = ( y - m_my_slotstart_y ) / SLOT_HEIGHT;

					int nItemIndex = m_pNkCha->GetExgSlotIndex( nSlot_X_Num, nSlot_Y_Num );
					if( nItemIndex < 0 )
						return 1;

					int nVNum = 0;
					int nPlusNum = 0;
					int nSpecialNum = 0;

					CItem* pItem = m_pNkCha->m_ExgInven;
					CItem* pTempItem = pItem;

					// 인텍스 값을 이용한 아이테의 wnum 추출
					while( pItem )
					{
						if( pItem->m_Index == nItemIndex )
						{
							int nWidth = GET_ITEM_WIDTH( pItem );
							int nHeight = GET_ITEM_HEIGHT( pItem );

							if( nWidth != 1 || nHeight != 1 )
								return 0;

							nVNum = pItem->m_Vnum;
							nPlusNum = pItem->m_PlusNum;
							nSpecialNum = pItem->m_Special;

							break;
						}
						pItem = pItem->m_Next;
					}

					// const 변수 선언
					char szTmp[ 10 ] = {0,};
					char szStr[ 100 ] = {0,};

					// 작업동안 딜레이 타임 1.5초
					// 최소한의 안전장치
					if( g_pNk2DFrame && g_pNk2DFrame->GetControlExchange() )
					{
						if( g_pNk2DFrame->GetControlExchange()->m_byExchLock )
						{
							DWORD curTime = timeGetTime();

							if (curTime > g_pNk2DFrame->GetControlExchange()->m_timeExchLock + 1500) // 1.5초가 지났으면
								g_pNk2DFrame->GetControlExchange()->m_byExchLock = 0;
							else
								return 1;
						}

						// 딜레이가 지났으면 플레그 해제
						g_pNk2DFrame->GetControlExchange()->SetExchLock( 1 );
					}

					// 선택한 아이템과 동일 아이템 추출
					while( pTempItem )
					{
						if( ( pTempItem->m_Vnum == nVNum )
								&& ( pTempItem->m_PlusNum == nPlusNum )
								&& ( pTempItem->m_Special == nSpecialNum ) )
						{
							sprintf( szTmp, "%d %d ", pTempItem->m_SlotX, pTempItem->m_SlotY );
							strcat( szStr, szTmp );
						}
						pTempItem = pTempItem->m_Next;
					}

					sprintf( m_MsgBuf, "exch_get %d %s\n", m_pNkCha->m_CurPackNum, szStr );
					SendMsg();
				}
			}
		}
		break;

	case WM_RBUTTONDOWN:
	case WM_MOUSEMOVE:
	case WM_LBUTTONUP:
	case WM_LBUTTONDOWN:

		switch( m_iType )
		{
		case EVENT_TYPE_PARENTSDAY:
			ret = parentsDayMsg(hWnd, msg, wParam, lParam);
			break;

		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		case EVENT_TYPE_WORLDCUP :
		case EVENT_TYPE_THAI_MOTHERS_DAY:
		case EVENT_TYPE_THAI_XMAS_TREE :
		case EVENT_TYPE_LAGHAIM_STRING:
		case EVENT_TYPE_THAI_VALENTINE :
		case EVENT_TYPE_KOR2007_VALENTINE:
		case EVENT_TYPE_KOR2008_NEWYEAR: // : case EVENT_TYPE_KOR2007_NEWYEAR
		case EVENT_TYPE_KOR_6YEAR:  // [8/6/2007 Theodoric] 6주년 기념 이벤트
		case EVENT_TYPE_KOR2007_WHITEDAY: // 2007.03.14 화이트
		case EVENT_TYPE_KOR2007_PEPERO: // 2007.03.14 화이트
		case EVENT_TYPE_STARSTONE_EXCH:
		case EVENT_TYPE_GIRASI_EXCH:
		case EVENT_TYPE_SK_200804:
		case EVENT_TYPE_BP_CAMPAIN_200804:

		case EVENT_TYPE_LP_GIFTBOX_CHANGE_JP: //  [10/29/2008 parkmj] 일본보석함 시스템
		case EVENT_TYPE_LP_GIFTBOX_CHANGE:

		case EVENT_TYPE_NEW_DEFENITEM_ITEM_EXCH:
		case EVENT_TYPE_2008_BEIJING_OLYMPIC:
		case EVENT_TYPE_2008_GEMSTONE:
		case EVENT_TYPE_HALLOWEEN:
		case EVENT_TYPE_081114_PREMIUM_EVENT:

		case EVENT_TYPE_BALLOON_EXCH:

		case EVENT_TYPE_BIRTH_2010:
		case EVENT_TYPE_PRESENT_2010:
		case EVENT_TYPE_BIRTH_CELEBRATE_2010:
		case EVENT_LP_PRESENT_BOX:
		case EVENT_GOD_WEAPON_EXPERIENCE:
		case EVENT_TYPE_NGA_OPEN:
		case EVENT_TYPE_AWARD_2010:
		case EVENT_TYPE_NEWYEAR_2011:
		case EVENT_TYPE_NEWYEAR_LUCKYBAG:
		case EVENT_TYPE_NATAL:
		case EVENT_TYPE_NEWYEAR_CARD:
		case EVENT_TYPE_FORLOM_MAP:
		case EVENT_TYPE_FORLOM_DEVICE:
		case EVENT_TYPE_FORLOM_DEVICE_DM:
		case EVENT_TYPE_GREENBUD_2011:
		case EVENT_TYPE_NEW_SUPPORT_2011:
		case EVENT_TYPE_SUMMER_2011:
		case EVENT_TYPE_10YEAR_ITEM:
		case EVENT_BULLION_EXCHANGE:
		case EVENT_TYPE_S1TOS2:
		case EVENT_TYPE_JPN_9YEAR_ITEM:
		case EVENT_TYPE_PCBANG_ARUPAKA:
			ret = santaExgMsg(hWnd, msg, wParam, lParam);
			break;

		default:
			ret = ExgMessage(  hWnd,  msg,  wParam,  lParam);
			break;

		}
		break;
	}
	return ret;
}
//wan:2005-01 : 산타 선물 교환시 메세지루틴 - 펑펑 이벤트(산할아버지)
LRESULT CEventAncient::santaExgMsg( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static char commOutBuf[512];	// 나중에 변수 다른 곳에 설정.
	int x, y;
	int hr = -1;
	int chip = -1;

	switch (msg)
	{
	case WM_LBUTTONDOWN:

		x = LOWORD (lParam);
		y = HIWORD (lParam);

		if(m_bExchange) // 교환하는순간 extra 하는 것을 막기 위함이다.lock
			return 1;

		if (IsInside(x, y))
		{
			if( m_pBtnCancel )
				m_pBtnCancel->MsgProc(hWnd, msg, wParam, lParam);
			if( m_pBtnExchange )
				m_pBtnExchange->MsgProc(hWnd, msg, wParam, lParam);

			if (x >= m_my_slotstart_x && x < m_my_slotstart_x + (SLOT_WIDTH * EXG_SLOT_X_NUM) &&
					y >= m_my_slotstart_y && y < m_my_slotstart_y + (SLOT_HEIGHT * EXG_SLOT_Y_NUM) )
			{
				if( timeGetTime() - m_dwMouseClickTickTime < 300)  // 0.3초
					return 1;

				m_dwMouseClickTickTime = timeGetTime();
				int slot_x_num, slot_y_num;

				if( g_pNk2DFrame && g_pNk2DFrame->GetControlExchange() )
				{
					if( g_pNk2DFrame->GetControlExchange()->m_byExchLock )
					{
						DWORD curTime = timeGetTime();

						if (curTime > g_pNk2DFrame->GetControlExchange()->m_timeExchLock + 1500) // 1.5초가 지났으면
							g_pNk2DFrame->GetControlExchange()->m_byExchLock = 0;
						else
							return 1;
					}
				}

				// 엑스트라 슬롯에 있는 경우, 교환창 슬롯에 놓는다.
				if (g_pRoh->m_ExtraSlot)
				{
					if (GET_ITEM_WIDTH(g_pRoh->m_ExtraSlot) % 2)
						slot_x_num = (x - m_my_slotstart_x) / SLOT_WIDTH - (GET_ITEM_WIDTH(g_pRoh->m_ExtraSlot)/2);
					else
						slot_x_num = (x - m_my_slotstart_x + (SLOT_WIDTH/2)) / SLOT_WIDTH - (GET_ITEM_WIDTH(g_pRoh->m_ExtraSlot)/2);

					if (GET_ITEM_HEIGHT(g_pRoh->m_ExtraSlot) % 2)
						slot_y_num = (y - m_my_slotstart_y) / SLOT_HEIGHT - (GET_ITEM_HEIGHT(g_pRoh->m_ExtraSlot)/2);
					else
						slot_y_num = (y - m_my_slotstart_y + (SLOT_HEIGHT/2)) / SLOT_HEIGHT - (GET_ITEM_HEIGHT(g_pRoh->m_ExtraSlot)/2);

					if (slot_x_num >= 0 && slot_x_num + GET_ITEM_WIDTH(g_pRoh->m_ExtraSlot) - 1 < EXG_SLOT_X_NUM &&
							slot_y_num >= 0 && slot_y_num + GET_ITEM_HEIGHT(g_pRoh->m_ExtraSlot) - 1 < EXG_SLOT_Y_NUM)
					{
						if (g_pRoh->AddToExgInven(slot_x_num, slot_y_num))
						{
							sprintf(m_MsgBuf, "exch inven %d %d\n", slot_x_num, slot_y_num);
							SendMsg();
						}
					}
				}
				else
				{
					if (g_pRoh->AddToExgInven((x-m_my_slotstart_x) / SLOT_WIDTH, (y-m_my_slotstart_y) / SLOT_HEIGHT))
					{
						sprintf(m_MsgBuf, "exch inven %d %d\n", (x-m_my_slotstart_x) / SLOT_WIDTH, (y-m_my_slotstart_y) / SLOT_HEIGHT);
						SendMsg();
					}
				}
			}
		}
		else if (g_pNk2DFrame && IsPlayArea(x, y))
			g_pNk2DFrame->ShowEventAncientWindow(FALSE);
		else
			return 0;

		return 1;

	case WM_LBUTTONUP:
	case WM_MOUSEMOVE:

		x = LOWORD (lParam);
		y = HIWORD (lParam);
		if (!IsInside(x, y))
			return 0;

		if( m_pBtnCancel )
			m_pBtnCancel->MsgProc(hWnd, msg, wParam, lParam);
		if( m_pBtnExchange )
			m_pBtnExchange->MsgProc(hWnd, msg, wParam, lParam);

		if (m_pBtnCancel->GetState() == BTN_ACTION)
			g_pNk2DFrame->ShowEventAncientWindow(FALSE);

		if (m_pBtnExchange->GetState() == BTN_ACTION)
		{
			CheckMyslot_Santa();
		}

		return 1;

	case WM_RBUTTONDOWN:

		x = LOWORD (lParam);
		y = HIWORD (lParam);
		if (IsInside(x, y))
			return 1;
		break;
	}
	return 0;
}
// 칩에서 보석으로의 교환시에 메세지루틴..
LRESULT CEventAncient::ExgMessage( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static char commOutBuf[512];	// 나중에 변수 다른 곳에 설정.
	int x, y;
	int hr = -1;
	int chip = -1;

	switch (msg)
	{
	case WM_LBUTTONDOWN:

		x = LOWORD (lParam);
		y = HIWORD (lParam);

		if(m_bExchange) // 교환하는순간 extra 하는 것을 막기 위함이다.lock
			return 1;

		if (IsInside(x, y))
		{
			if (m_pBtnCancel->IsInside(x, y))
				m_pBtnCancel->MsgProc(hWnd, msg, wParam, lParam);
			else if (!m_bQuestWindow && m_pBtnExchange->IsInside(x, y))
				m_pBtnExchange->MsgProc(hWnd, msg, wParam, lParam);
			else if(m_bQuestWindow && m_pBtnExchangePaper->IsInside(x, y))
				m_pBtnExchangePaper->MsgProc(hWnd, msg, wParam, lParam);
			else if(m_bQuestWindow && m_pBtnExchangeRing->IsInside(x, y))
				m_pBtnExchangeRing->MsgProc(hWnd, msg, wParam, lParam);
			else if (x >= m_my_slotstart_x && x < m_my_slotstart_x + (SLOT_WIDTH * EXG_SLOT_X_NUM) &&
					 y >= m_my_slotstart_y && y < m_my_slotstart_y + (SLOT_HEIGHT * EXG_SLOT_Y_NUM) )
			{
				if( timeGetTime() - m_dwMouseClickTickTime < 300)  // 0.3초
					return 1;

				m_dwMouseClickTickTime = timeGetTime();
				int slot_x_num, slot_y_num;
				// 엑스트라 슬롯에 있는 경우, 교환창 슬롯에 놓는다.
				if (g_pRoh && g_pRoh->m_ExtraSlot)
				{
					//if (m_bExch)
					//	return 1;
					if (GET_ITEM_WIDTH(g_pRoh->m_ExtraSlot) % 2)
						slot_x_num = (x - m_my_slotstart_x) / SLOT_WIDTH - (GET_ITEM_WIDTH(g_pRoh->m_ExtraSlot)/2);
					else
						slot_x_num = (x - m_my_slotstart_x + (SLOT_WIDTH/2)) / SLOT_WIDTH - (GET_ITEM_WIDTH(g_pRoh->m_ExtraSlot)/2);

					if (GET_ITEM_HEIGHT(g_pRoh->m_ExtraSlot) % 2)
						slot_y_num = (y - m_my_slotstart_y) / SLOT_HEIGHT - (GET_ITEM_HEIGHT(g_pRoh->m_ExtraSlot)/2);
					else
						slot_y_num = (y - m_my_slotstart_y + (SLOT_HEIGHT/2)) / SLOT_HEIGHT - (GET_ITEM_HEIGHT(g_pRoh->m_ExtraSlot)/2);

					if (slot_x_num >= 0 && slot_x_num + GET_ITEM_WIDTH(g_pRoh->m_ExtraSlot) - 1 < EXG_SLOT_X_NUM &&
							slot_y_num >= 0 && slot_y_num + GET_ITEM_HEIGHT(g_pRoh->m_ExtraSlot) - 1 < EXG_SLOT_Y_NUM)
					{
						if (g_pRoh->AddToExgInven(slot_x_num, slot_y_num))
						{
							sprintf(m_MsgBuf, "exch inven %d %d\n", slot_x_num, slot_y_num);
							SendMsg();
						}
					}
				}
				else
				{
					if (g_pRoh
							&& g_pRoh->AddToExgInven((x-m_my_slotstart_x) / SLOT_WIDTH, (y-m_my_slotstart_y) / SLOT_HEIGHT))
					{
						sprintf(m_MsgBuf, "exch inven %d %d\n", (x-m_my_slotstart_x) / SLOT_WIDTH, (y-m_my_slotstart_y) / SLOT_HEIGHT);
						SendMsg();
					}
				}
			}
		}
		else if (IsPlayArea(x, y))
			g_pNk2DFrame->ShowEventAncientWindow(FALSE);
		else
			return 0;

		return 1;

	case WM_LBUTTONUP:
	case WM_MOUSEMOVE:

		x = LOWORD (lParam);
		y = HIWORD (lParam);
		if (!IsInside(x, y))
			return 0;

		if( m_pBtnCancel )
			m_pBtnCancel->MsgProc(hWnd, msg, wParam, lParam);
		if(!m_bQuestWindow && m_pBtnExchange)
			m_pBtnExchange->MsgProc(hWnd, msg, wParam, lParam);
		else
		{
			if( m_pBtnExchangePaper )
				m_pBtnExchangePaper->MsgProc(hWnd, msg, wParam, lParam);
			if( m_pBtnExchangeRing )
				m_pBtnExchangeRing->MsgProc(hWnd, msg, wParam, lParam);
		}

		if (g_pNk2DFrame && m_pBtnCancel && m_pBtnCancel->GetState() == BTN_ACTION)
			g_pNk2DFrame->ShowEventAncientWindow(FALSE);

		if (m_pBtnExchange && m_pBtnExchange->GetState() == BTN_ACTION && !m_bQuestWindow)
		{
			if((chip = Check2Myslot_Exg()) !=-1)
			{
				// 해당 아이템만 있는지 있는지 체크
				if(chip==1)
				{
					if( g_dwClientCountry == CTRY_JPN )
					{
						sprintf(m_MsgBuf, "giftch\n");	// [4/8/2008 반재승] 일본 어린이날 이벤트, 패킷 이름이 다르다.
					}
					else
					{
						sprintf(m_MsgBuf, "ex_item toy\n"); // by 원석
					}
				}
				else
				{
					sprintf(m_MsgBuf, "sd_ex\n");
				}

				SendMsg();
				m_bExchange = TRUE;
			}

		}
		if (m_pBtnExchangePaper && m_pBtnExchangePaper->GetState() == BTN_ACTION)
		{
			if(CheckMyslot_Exg()==3 ||CheckMyslot_Exg()==4)
			{
				sprintf(m_MsgBuf, "q_ex\n");
				SendMsg();
				m_bExchange = TRUE;
			}
		}
		if (m_pBtnExchangeRing && m_pBtnExchangeRing->GetState() == BTN_ACTION)
		{
			m_pBtnExchangeRing->m_iBtnFlg = BTN_NORMAL;
			if(g_pRoh->m_Wearing[IARMOR_RING]

					&& ( g_pRoh->m_Wearing[IARMOR_RING]->m_Vnum == 922 ||
						 g_pRoh->m_Wearing[IARMOR_RING]->m_Vnum == 933 ||
						 g_pRoh->m_Wearing[IARMOR_RING]->m_Vnum == 934 ||
						 g_pRoh->m_Wearing[IARMOR_RING]->m_Vnum == 935 ||
						 g_pRoh->m_Wearing[IARMOR_RING]->m_Vnum == 1032 )
			  )
				//-- IDS_EVENTACIENT_WARNING_RING : 정말로 반지를 회수해도 되겠습니까?
				g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_EVENTACIENT_WARNING_RING),TYPE_NOR_OKCANCLE, 7);
			else
			{
				g_pNk2DFrame->AddChatStr( (char*)G_STRING(IDS_LHSTRING1841) , -1 );
			}
			return 1;
		}
		if( m_pBtnExchange )
			m_pBtnExchange->SetState(BTN_NORMAL);
		if( m_pBtnExchangePaper )
			m_pBtnExchangePaper->SetState(BTN_NORMAL);
		if( m_pBtnExchangeRing )
			m_pBtnExchangeRing->SetState(BTN_NORMAL);
		if( m_pBtnCancel )
			m_pBtnCancel->SetState(BTN_NORMAL);
		return 1;

	case WM_RBUTTONDOWN:

		x = LOWORD (lParam);
		y = HIWORD (lParam);
		if (IsInside(x, y))
			return 1;
		break;
	}
	return 0;
}



LRESULT CEventAncient::parentsDayMsg( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static char commOutBuf[512] = "";	// 나중에 변수 다른 곳에 설정.
	int x = 0, y = 0;
	int hr = -1;
	int chip = -1;

	switch (msg)
	{
	case WM_LBUTTONDOWN:

		x = LOWORD (lParam);
		y = HIWORD (lParam);

		if(m_bExchange) // 교환하는순간 extra 하는 것을 막기 위함이다.lock
			return 1;

		if (IsInside(x, y))
		{
			if( m_pBtnCancel )
				m_pBtnCancel->MsgProc(hWnd, msg, wParam, lParam);
			if( m_pBtnExchange )
				m_pBtnExchange->MsgProc(hWnd, msg, wParam, lParam);

			if (x >= m_my_slotstart_x && x < m_my_slotstart_x + (SLOT_WIDTH * EXG_SLOT_X_NUM) &&
					y >= m_my_slotstart_y && y < m_my_slotstart_y + (SLOT_HEIGHT * EXG_SLOT_Y_NUM) )
			{

				if( timeGetTime() - m_dwMouseClickTickTime < 300)  // 0.3초
					return 1;

				m_dwMouseClickTickTime = timeGetTime();
				int slot_x_num = 0, slot_y_num = 0;
				// 엑스트라 슬롯에 있는 경우, 교환창 슬롯에 놓는다.
				if (g_pRoh && g_pRoh->m_ExtraSlot)
				{
					if (GET_ITEM_WIDTH(g_pRoh->m_ExtraSlot) % 2)
						slot_x_num = (x - m_my_slotstart_x) / SLOT_WIDTH - (GET_ITEM_WIDTH(g_pRoh->m_ExtraSlot)/2);
					else
						slot_x_num = (x - m_my_slotstart_x + (SLOT_WIDTH/2)) / SLOT_WIDTH - (GET_ITEM_WIDTH(g_pRoh->m_ExtraSlot)/2);

					if (GET_ITEM_HEIGHT(g_pRoh->m_ExtraSlot) % 2)
						slot_y_num = (y - m_my_slotstart_y) / SLOT_HEIGHT - (GET_ITEM_HEIGHT(g_pRoh->m_ExtraSlot)/2);
					else
						slot_y_num = (y - m_my_slotstart_y + (SLOT_HEIGHT/2)) / SLOT_HEIGHT - (GET_ITEM_HEIGHT(g_pRoh->m_ExtraSlot)/2);

					if (slot_x_num >= 0 && slot_x_num + GET_ITEM_WIDTH(g_pRoh->m_ExtraSlot) - 1 < EXG_SLOT_X_NUM &&
							slot_y_num >= 0 && slot_y_num + GET_ITEM_HEIGHT(g_pRoh->m_ExtraSlot) - 1 < EXG_SLOT_Y_NUM)
					{
						if (g_pRoh->AddToExgInven(slot_x_num, slot_y_num))
						{
							sprintf(m_MsgBuf, "exch inven %d %d\n", slot_x_num, slot_y_num);
							SendMsg();
						}
					}
				}
				else
				{
					if (g_pRoh->AddToExgInven((x-m_my_slotstart_x) / SLOT_WIDTH, (y-m_my_slotstart_y) / SLOT_HEIGHT))
					{
						sprintf(m_MsgBuf, "exch inven %d %d\n", (x-m_my_slotstart_x) / SLOT_WIDTH, (y-m_my_slotstart_y) / SLOT_HEIGHT);
						SendMsg();
					}
				}
			}
		}
		else if (IsPlayArea(x, y))
			g_pNk2DFrame->ShowEventAncientWindow(FALSE);
		else
			return 0;

		return 1;

	case WM_LBUTTONUP:
	case WM_MOUSEMOVE:

		x = LOWORD (lParam);
		y = HIWORD (lParam);

		if (!IsInside(x, y))
			return 0;

		if( m_pBtnCancel )
			m_pBtnCancel->MsgProc(hWnd, msg, wParam, lParam);
		if( m_pBtnExchange )
			m_pBtnExchange->MsgProc(hWnd, msg, wParam, lParam);

		if (g_pNk2DFrame && m_pBtnCancel && m_pBtnCancel->GetState() == BTN_ACTION)
			g_pNk2DFrame->ShowEventAncientWindow(FALSE);

		if (m_pBtnExchange && m_pBtnExchange->GetState() == BTN_ACTION)
		{
			CheckMyslot_ParentsDay();
		}

		return 1;

	case WM_RBUTTONDOWN:

		x = LOWORD (lParam);
		y = HIWORD (lParam);

		if (IsInside(x, y))
			return 1;

		break;
	}
	return 0;
}

#define GET_MY_SLOT_X(point_x)		((point_x - m_my_slotstart_x) / SLOT_WIDTH)
#define GET_MY_SLOT_Y(point_y)		((point_y - m_my_slotstart_y) / SLOT_HEIGHT)
void CEventAncient::DrawMySlot()
{
	if( !g_pDisplay )
		return;

	RECT rcRect;
	POINT point;
	GetCursorPos(&point);
	ScreenToClient(g_pDisplay->GetHWnd(), &point);

	if (pCMyApp->m_pMouse && IsInside(point.x, point.y))
		pCMyApp->m_pMouse->SetMouseType(M_NORMAL);

	// 아이템이 위치한 슬롯 표시
	CItem *pItem = NULL;
	if( g_pRoh )
		pItem = g_pRoh->m_ExgInven;

	while (pItem)
	{
		rcRect.left = 0;
		rcRect.right = SLOT_WIDTH * GET_ITEM_WIDTH(pItem);
		rcRect.top = 0;
		rcRect.bottom = SLOT_HEIGHT * GET_ITEM_HEIGHT(pItem);
		g_pDisplay->Blt( m_my_slotstart_x + (SLOT_WIDTH*pItem->m_SlotX),
						 m_my_slotstart_y + (SLOT_HEIGHT*pItem->m_SlotY),
						 m_pExistSlot, &rcRect);
		pItem = pItem->m_Next;
	}

	// Extra Slot에 있는 아이템이 놓일 곳 표시
	int slot_x_num = 0, slot_y_num = 0;
	if (g_pRoh && g_pRoh->m_ExtraSlot)
	{
		if (point.x >= m_my_slotstart_x
				&& point.x < m_my_slotstart_x + (SLOT_WIDTH * EXG_SLOT_X_NUM )
				&& point.y >= m_my_slotstart_y
				&& point.y < m_my_slotstart_y + (SLOT_HEIGHT * EXG_SLOT_Y_NUM) )
		{
			if (GET_ITEM_WIDTH(g_pRoh->m_ExtraSlot) % 2)
				slot_x_num = (point.x - m_my_slotstart_x) / SLOT_WIDTH - (GET_ITEM_WIDTH(g_pRoh->m_ExtraSlot)/2);
			else
				slot_x_num = (point.x - m_my_slotstart_x + (SLOT_WIDTH/2)) / SLOT_WIDTH - (GET_ITEM_WIDTH(g_pRoh->m_ExtraSlot)/2);

			if (GET_ITEM_HEIGHT(g_pRoh->m_ExtraSlot) % 2)
				slot_y_num = (point.y - m_my_slotstart_y) / SLOT_HEIGHT - (GET_ITEM_HEIGHT(g_pRoh->m_ExtraSlot)/2);
			else
				slot_y_num = (point.y - m_my_slotstart_y + (SLOT_HEIGHT/2)) / SLOT_HEIGHT - (GET_ITEM_HEIGHT(g_pRoh->m_ExtraSlot)/2);

			if (slot_x_num >= 0
					&& slot_x_num + GET_ITEM_WIDTH(g_pRoh->m_ExtraSlot) - 1 < EXG_SLOT_X_NUM
					&& slot_y_num >= 0
					&& slot_y_num + GET_ITEM_HEIGHT(g_pRoh->m_ExtraSlot) - 1 < EXG_SLOT_Y_NUM)
			{
				rcRect.left = 0;
				rcRect.right = SLOT_WIDTH * GET_ITEM_WIDTH(g_pRoh->m_ExtraSlot);
				rcRect.top = 0;
				rcRect.bottom = SLOT_HEIGHT * GET_ITEM_HEIGHT(g_pRoh->m_ExtraSlot);
				if (g_pRoh->GetItemIndexInExgSlot(slot_x_num, slot_y_num,
												  GET_ITEM_WIDTH(g_pRoh->m_ExtraSlot), GET_ITEM_HEIGHT(g_pRoh->m_ExtraSlot))
						>= -1)
				{
					g_pDisplay->Blt( m_my_slotstart_x + (SLOT_WIDTH*slot_x_num),
									 m_my_slotstart_y + (SLOT_HEIGHT*slot_y_num),
									 m_pEnableSlot, &rcRect);
				}
				else
				{
					g_pDisplay->Blt( m_my_slotstart_x + (SLOT_WIDTH*slot_x_num),
									 m_my_slotstart_y + (SLOT_HEIGHT*slot_y_num),
									 m_pDisableSlot, &rcRect);
				}
			}
		}
	}

	// 내 교환품 그리기..
	if (point.x >= m_my_slotstart_x && point.y >= m_my_slotstart_y)
	{
		slot_x_num = GET_MY_SLOT_X(point.x);
		slot_y_num = GET_MY_SLOT_Y(point.y);
	}
	else
	{
		slot_x_num = -1;
		slot_y_num = -1;
	}
	int index = 0;
	if( g_pRoh )
		index = g_pRoh->GetExgSlotIndex(slot_x_num, slot_y_num);

	if( g_pRoh )
		pItem = g_pRoh->m_ExgInven;

	if( m_pItemRender )
	{
		m_pItemRender->BeginRenderLolo();		// 중요..  m_pItemRender->RenderLolo의 파라미터에 영향을 준다.
		while (pItem)
		{
			if (pItem->m_Index == index)
			{

				g_pNk2DFrame->RenderItemInUI(m_my_slotstart_x + pItem->m_SlotX * SLOT_WIDTH,
											 m_my_slotstart_y + pItem->m_SlotY * SLOT_HEIGHT,
											 pItem,  TRUE , FALSE, FALSE, TRUE );
				m_pOverItem = pItem;
			}
			else
			{
				DWORD ambient = 0x00555555;
				if (GET_TYPE(pItem) == ITYPE_ETC && GET_SHAPE(pItem) == ITYPE_ETC)
					ambient = 0x00cccccc;
				else if (pItem->m_PlusNum > 0)
					ambient = UPGRADE_ITEM_COLOR;

				g_pNk2DFrame->RenderItemInUI(m_my_slotstart_x + pItem->m_SlotX * SLOT_WIDTH,
											 m_my_slotstart_y + pItem->m_SlotY * SLOT_HEIGHT,
											 pItem,  FALSE , FALSE , FALSE, TRUE );
			}
			pItem = pItem->m_Next;
		}
		m_pItemRender->EndRenderLolo();
	}
}

int CEventAncient::CheckMyslot_Exg()
{
	int  vnum = -1;
	int  kind = 0;

	CItem* pItem = NULL;
	if( g_pRoh )
		pItem = g_pRoh->m_ExgInven;

	if(!pItem)
		return -1;

	while (pItem)
	{
		vnum = pItem->m_Vnum;

		// nate 2004 - 4 : quest - 아이템 처리
		if( vnum ==925 || vnum == 1054 )	//0409 느와젠의 기록 추가
			kind = 3;
		else if(vnum ==901)//0409 데카둔의 팔찌
			kind = 4;
		else
		{
			//-- IDS_QUEST_ITEM_NOT_MATCH : 다른 종류의 아이템이 섞여있습니다.
			g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_QUEST_ITEM_NOT_MATCH), TYPE_NOR_OK, 1 );
			return -1;
		}
		pItem = pItem->m_Next;
	}

	return kind;
}

int CEventAncient::Check2Myslot_Exg() //임시 이벤트용 함수
{
	int  vnum = -1;
	int  kind = 0;

	CItem* pItem = NULL;
	if( g_pRoh )
		pItem = g_pRoh->m_ExgInven;

	if(!pItem)
		return -1;

	while (pItem)
	{
		vnum = pItem->m_Vnum;

		if(vnum == 945 || vnum == 946 )
			kind = 1;
		else
		{
			//-- IDS_QUEST_ITEM_NOT_MATCH : 다른 종류의 아이템이 섞여있습니다.
			g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_QUEST_ITEM_NOT_MATCH), TYPE_NOR_OK, 1 );
			return -1;
		}
		pItem = pItem->m_Next;
	}

	return kind;
}

//wan:2005-01
//산타 선물 체크
//펑펑 이벤트 선물 체크
int CEventAncient::CheckMyslot_Santa()
{
	int  vnum = -1;
	BOOL itemNumOver = FALSE;

	CItem* pItem = NULL;
	if( g_pRoh )
		pItem = g_pRoh->m_ExgInven;

	int a = 0, b = 0, c = 0;

	if(!pItem)
		return -1;
	else
	{
		switch ( m_iType )
		{
		case 3:
		case 4: ///산타추가
			{
				//	while (pItem) {
				vnum = pItem->m_Vnum;
				int nVnum = 0;
				if( m_iType == 3 )
					nVnum = 911;
			}
			break;

		case 5:
			{
				int count = 0;
				do
				{
					vnum = pItem->m_Vnum;

					switch( g_dwClientCountry )
					{
					case CTRY_BRA:
					case CTRY_TH:
					case CTRY_JPN: // 일본은 선물상자 10개 교환이 따로 있다.
						if( !(vnum == 1126||vnum == 1131) )//붉은양말,신비의 선물
						{
							//-- IDS_QUEST_ITEM_NOT_MATCH : 다른 종류의 아이템이 섞여있습니다.
							g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_QUEST_ITEM_NOT_MATCH), TYPE_NOR_OK, 1 );
							return -1;
						}

						if( count > 0 && vnum == 1126 ) // 붉은 양말은 1개만 교환된다.
						{
							//-- IDS_CHECK_ITEM_COUNT : 아이템 개수를 확인해주세요.
							g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_CHECK_ITEM_COUNT), TYPE_NOR_OK, 1 );
							return -1;
						}
						break;
					case CTRY_TWN: // 일본은 선물상자 10개 교환이 따로 있다.
						if( !(vnum == 1126||vnum == 1131) )//붉은양말,신비의 선물
						{
							//-- IDS_QUEST_ITEM_NOT_MATCH : 다른 종류의 아이템이 섞여있습니다.
							g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_QUEST_ITEM_NOT_MATCH), TYPE_NOR_OK, 1 );
							return -1;
						}
						break;

					default: // 다른 나라는 신비의 선물만 교환.
						if( vnum != 1131 ) //신비의 선물
						{
							//-- IDS_QUEST_ITEM_NOT_MATCH : 다른 종류의 아이템이 섞여있습니다.
							g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_QUEST_ITEM_NOT_MATCH), TYPE_NOR_OK, 1 );
							return -1;
						}
						break;
					}

					++count;
					pItem = pItem->m_Next;
				}
				while( pItem );   // 교환창 아이템을 전부 체크.

				if( g_dwClientCountry == CTRY_JPN || g_dwClientCountry == CTRY_TH || g_dwClientCountry == CTRY_BRA ) // 일본은 선물상자 10개 교환이 따로 있다.
				{
					if( count == 1 ) // 아이템 갯수가 1개 였다면...
					{
						if( g_pRoh )
							pItem = g_pRoh->m_ExgInven;

						vnum = pItem->m_Vnum;

						// 다시 검사. 조건이 좀 복잡해서...
						if( !(vnum == 1126||vnum == 1131) )//붉은양말,신비의 선물
						{
							//-- IDS_QUEST_ITEM_NOT_MATCH : 다른 종류의 아이템이 섞여있습니다.
							g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_QUEST_ITEM_NOT_MATCH), TYPE_NOR_OK, 1 );
							return -1;
						}

					}
					else if( count != 10 ) // 수량체크.
					{
						//-- IDS_CHECK_ITEM_COUNT : 아이템 개수를 확인해주세요.
						g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_CHECK_ITEM_COUNT), TYPE_NOR_OK, 1 );
						return -1;
					}
				}
				else
				{
					if( count != 1 ) // 다른 나라는 갯수가 1개가 아니면 잘못된거다.
					{
						//-- IDS_CHECK_ITEM_COUNT : 아이템 개수를 확인해주세요.
						g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_CHECK_ITEM_COUNT), TYPE_NOR_OK, 1 );
						return -1;
					}
				}
			}
			break;

		case 6:
			{
				vnum = pItem->m_Vnum;
				if( vnum != 488 ) //태극기 아이템
				{
					//-- IDS_QUEST_ITEM_NOT_MATCH : 다른 종류의 아이템이 섞여있습니다.
					g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_QUEST_ITEM_NOT_MATCH), TYPE_NOR_OK, 1 );
					return -1;
				}

				if(pItem->m_Next)
				{
					//아이템이 두개 이상일때
					//--IDS_INTERCHA_PUT_ITEM : 바꿀 아이템 하나만 놓아주세요.
					g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_INTERCHA_PUT_ITEM), TYPE_NOR_OK, 1 );
					return -1;
				}
			}
			break;

		case 7: ///대만재신이벤트
			{
				vnum = pItem->m_Vnum;
				if(!(vnum == 1390||vnum == 1391)) // 은색,금색 복주머니
				{
					//-- IDS_QUEST_ITEM_NOT_MATCH : 다른 종류의 아이템이 섞여있습니다.
					g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_QUEST_ITEM_NOT_MATCH), TYPE_NOR_OK, 1 );
					return -1;
				}

				if(pItem->m_Next)
				{
					//아이템이 두개 이상일때
					//--IDS_INTERCHA_PUT_ITEM : 바꿀 아이템 하나만 놓아주세요.
					g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_INTERCHA_PUT_ITEM), TYPE_NOR_OK, 1 );
					return -1;
				}
			}
			break;

		case EVENT_TYPE_WORLDCUP: // 월드컵 공 교환 이벤트.
			{
				vnum = pItem->m_Vnum;
				if( vnum != 1429 ) // 월드컵 공
				{
					//-- IDS_QUEST_ITEM_NOT_MATCH : 다른 종류의 아이템이 섞여있습니다.
					g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_QUEST_ITEM_NOT_MATCH), TYPE_NOR_OK, 1 );
					return -1;
				}

				if(pItem->m_Next)
				{
					//아이템이 두개 이상일때
					//--IDS_INTERCHA_PUT_ITEM : 바꿀 아이템 하나만 놓아주세요.
					g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_INTERCHA_PUT_ITEM), TYPE_NOR_OK, 1 );
					return -1;
				}
			}
			break;

		case EVENT_TYPE_THAI_MOTHERS_DAY: // 태국 왕비 생신 이벤트. 쟈스민 꽃 5개를 교환한다.
			{
				int count = 0;

				do
				{
					vnum = pItem->m_Vnum;

					if( vnum != 1467 ) // 쟈스민 꽃
					{
						//-- IDS_QUEST_ITEM_NOT_MATCH : 다른 종류의 아이템이 섞여있습니다.
						g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_QUEST_ITEM_NOT_MATCH), TYPE_NOR_OK, 1 );
						return -1;
					}

					++count;
					pItem = pItem->m_Next;

				}
				while( pItem );   // 교환창 아이템을 전부 체크.

				if( count != 5 ) // 5개 수량체크.
				{
					//-- IDS_CHECK_ITEM_COUNT : 아이템 개수를 확인해주세요.
					g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_CHECK_ITEM_COUNT), TYPE_NOR_OK, 1 );
					return -1;
				}
			}
			break;

		case EVENT_TYPE_THAI_XMAS_TREE: // 태국 크리스마스 트리
			{
				int count = 0;

				do
				{
					vnum = pItem->m_Vnum;

					if( vnum != 1500 ) // 쟈스민 꽃
					{
						//-- IDS_QUEST_ITEM_NOT_MATCH : 다른 종류의 아이템이 섞여있습니다.
						g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_QUEST_ITEM_NOT_MATCH), TYPE_NOR_OK, 1 );
						return -1;
					}

					++count;

					pItem = pItem->m_Next;


				}
				while( pItem );   // 교환창 아이템을 전부 체크.

				if( count != 5 ) // 5개 수량체크.
				{
					//-- IDS_CHECK_ITEM_COUNT : 아이템 개수를 확인해주세요.
					g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_CHECK_ITEM_COUNT), TYPE_NOR_OK, 1 );
					return -1;
				}
			}
			break;

		case EVENT_TYPE_LAGHAIM_STRING: // 라그하임 글자 교환 이벤트, 2000일 이벤트 포함.
			{
				int CheckItemNum = 9;
				int CheckItemIndex[9] = { 390,391,392,393,394,395,396,1507,1508 }; // L A G H A I M 2 0 0 0
				int CheckItemCountRight[9] = {   1,  1,  1,  1,  1,  1,  1,   1,   3 }; // 각 아이템당 필요 카운트
				int CheckItemCount[9] = {   0,  0,  0,  0,  0,  0,  0,   0,   0 }; // 각 아이템당 카운트.

				int i = 0;

				do
				{
					vnum = pItem->m_Vnum;
					int k = 0;

					for( i = 0 ; i < CheckItemNum ; i++ )
					{
						if( vnum == CheckItemIndex[i] ) // 해당 아이템이 아닌게 있으면...
						{
							k = 1;
							CheckItemCount[i]++; // 해당 아이템의 카운트를 증가 시킨다.
						}
					}

					if( k == 0 ) // 전부 해당하지 않는 아이템있었으면..
					{
						//-- IDS_QUEST_ITEM_NOT_MATCH : 다른 종류의 아이템이 섞여있습니다.
						g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_QUEST_ITEM_NOT_MATCH), TYPE_NOR_OK, 1 );
						return -1;
					}

					pItem = pItem->m_Next;

				}
				while( pItem );   // 교환창 아이템을 전부 체크.


				for( i = 0 ; i < CheckItemNum ; i++ )
				{
					if( CheckItemCount[i] != CheckItemCountRight[i] ) // 필요 카운트가 틀리면...
					{
						//-- IDS_CHECK_ITEM_COUNT : 아이템 개수를 확인해주세요.
						g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_CHECK_ITEM_COUNT), TYPE_NOR_OK, 1 );
						return -1;
					}
				}
			}
			break;

		case EVENT_TYPE_THAI_VALENTINE :
			{
				int count = 0;

				if( !g_pRoh )
					return -1;

				int CheckIndex = 0;

				if( g_pRoh->m_Sex == 0 ) // 남자
					CheckIndex = 1511;
				else // 1이면 여자
					CheckIndex = 1510;

				do
				{
					vnum = pItem->m_Vnum;

					if( vnum != CheckIndex ) // 원래 아이템과 틀리면...
					{
						//-- IDS_QUEST_ITEM_NOT_MATCH : 다른 종류의 아이템이 섞여있습니다.
						g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_QUEST_ITEM_NOT_MATCH), TYPE_NOR_OK, 1 );
						return -1;
					}

					++count;

					pItem = pItem->m_Next;


				}
				while( pItem );   // 교환창 아이템을 전부 체크.

				// 수량은 좀 복잡해서 패스.( 중복 아이템도 들어가 있어서. )
			}
			break;

		case EVENT_TYPE_KOR2007_VALENTINE:
			{
				int count = 0;

				if( !g_pRoh )
					return -1;

				int CheckIndex = 0;

				if( g_pRoh->m_Sex == 0 ) // 남자
					CheckIndex = 1511;
				else // 1이면 여자
					CheckIndex = 1510;

				do
				{
					vnum = pItem->m_Vnum;

					if( vnum != CheckIndex ) // 원래 아이템과 틀리면...
					{
						//-- IDS_QUEST_ITEM_NOT_MATCH : 다른 종류의 아이템이 섞여있습니다.
						g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_QUEST_ITEM_NOT_MATCH), TYPE_NOR_OK, 1 );
						return -1;
					}

					++count;

					pItem = pItem->m_Next;


				}
				while( pItem );   // 교환창 아이템을 전부 체크.

				if( count != 24 ) // 수량체크.
				{
					//-- IDS_CHECK_ITEM_COUNT : 아이템 개수를 확인해주세요.
					g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_CHECK_ITEM_COUNT), TYPE_NOR_OK, 1 );
					return -1;
				}
			}
			break;

		case EVENT_TYPE_KOR2007_NEWYEAR:
		case EVENT_TYPE_KOR_6YEAR: // [8/6/2007 Theodoric] 6주년 기념 이벤트
			{
				int count1 = 0;
				int count2 = 0;
				int count3 = 0;

				if( !g_pRoh )
					return -1;

				do
				{
					vnum = pItem->m_Vnum;

					switch( vnum )
					{
					case 1520: // 금 펜던트
						++count1;
						break;
					case 1525: // 은 펜던트
						++count2;
						break;
					case 1526: // 동 펜던트
						++count3;
						break;
					default: // 다른 아이템이 있으면...
						//-- IDS_QUEST_ITEM_NOT_MATCH : 다른 종류의 아이템이 섞여있습니다.
						g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_QUEST_ITEM_NOT_MATCH), TYPE_NOR_OK, 1 );
						return -1;
					}

					pItem = pItem->m_Next;

				}
				while( pItem );   // 교환창 아이템을 전부 체크.

				if( count1 != 4 || count2 != 8 || count3 != 12 ) // 수량체크.
				{
					//-- IDS_CHECK_ITEM_COUNT : 아이템 개수를 확인해주세요.
					g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_CHECK_ITEM_COUNT), TYPE_NOR_OK, 1 );
					return -1;
				}
			}
			break;

		case EVENT_TYPE_KOR2008_NEWYEAR:
			{
				int count = 0;

				if( !g_pRoh )
					return -1;

				do
				{
					vnum = pItem->m_Vnum;

					if( vnum < 390 || vnum > 396 ) // 원래 아이템과 틀리면...
					{
						//-- IDS_QUEST_ITEM_NOT_MATCH : 다른 종류의 아이템이 섞여있습니다.
						g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_QUEST_ITEM_NOT_MATCH), TYPE_NOR_OK, 1 );
						return -1;
					}
					else
						count++;

					pItem = pItem->m_Next;

				}
				while( pItem );   // 교환창 아이템을 전부 체크.

				if( count != 7) // 수량체크.
				{
					//-- IDS_CHECK_ITEM_COUNT : 아이템 개수를 확인해주세요.
					g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_CHECK_ITEM_COUNT), TYPE_NOR_OK, 1 );
					return -1;
				}

			}
			break;

		case EVENT_TYPE_KOR2007_WHITEDAY: // 2007.03.14 화이트
			{
				int count = 0;

				if( !g_pRoh )
					return -1;
				do

				{
					vnum = pItem->m_Vnum;

					if( vnum != 1527 && vnum != 1528 &&
							vnum != 1529 && vnum != 1530  )  // 원래 아이템과 틀리면...
					{
						//-- IDS_QUEST_ITEM_NOT_MATCH : 다른 종류의 아이템이 섞여있습니다.
						g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_QUEST_ITEM_NOT_MATCH), TYPE_NOR_OK, 1 );
						return -1;
					}

					++count;

					pItem = pItem->m_Next;


				}
				while( pItem );   // 교환창 아이템을 전부 체크.

				// 			if( count != 24 ) // 수량체크.
				// 			{
				// 					//-- IDS_CHECK_ITEM_COUNT : 아이템 개수를 확인해주세요.
				// 				g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_CHECK_ITEM_COUNT), TYPE_NOR_OK, 1 );
				// 				return -1;
				// 			}
			}
			break;


		case EVENT_TYPE_STARSTONE_EXCH:
			{
				int num_count = 0;

				while(pItem)
				{
					switch(pItem->m_Vnum)
					{
					case 1898:
					case 1899:
					case 1900:
					case 1901:
						num_count++;
						break;
					default:
						{
							g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_QUEST_ITEM_NOT_MATCH), TYPE_NOR_OK, 1 );
							return -1;
						}
					}
					pItem = pItem->m_Next;
				}

				if( num_count <= 0 || num_count > 24 )
				{
					g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_CHECK_ITEM_COUNT), TYPE_NOR_OK, 1 );
					return -1;
				}

				int PER_PRICE = 10000;
				if( num_count * PER_PRICE > g_pRoh->m_Money)
				{
					g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_GUILD_CREATE_GUILD_FAIL_NEED_LAIM), TYPE_NOR_OK, 1 );
					return -1;
				}

				LH_SEND_NET_MSG("ex_item starstone\n");

			}
			break;

		case EVENT_TYPE_GIRASI_EXCH:
			{
				int count = 0;

				do
				{
					vnum = pItem->m_Vnum;

					switch( vnum )
					{
					case 1931:
						count++;
						break;

					default: // 다른 아이템이 있으면...
						{
							//-- IDS_QUEST_ITEM_NOT_MATCH : 다른 종류의 아이템이 섞여있습니다.
							g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_QUEST_ITEM_NOT_MATCH), TYPE_NOR_OK, 1 );
							return -1;
						}
					}

					pItem = pItem->m_Next;

				}
				while( pItem );   // 교환창 아이템을 전부 체크.

				if( count != 10) // 수량체크.
				{
					//-- IDS_CHECK_ITEM_COUNT : 아이템 개수를 확인해주세요.
					g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_CHECK_ITEM_COUNT), TYPE_NOR_OK, 1 );
					return -1;
				}
			}
			break;

		case EVENT_TYPE_BALLOON_EXCH:
			{
				int count = 0;

				do
				{
					vnum = pItem->m_Vnum;

					switch( vnum )
					{
					case 2367:
						count++;
						break;

					default: // 다른 아이템이 있으면...
						{
							//-- IDS_QUEST_ITEM_NOT_MATCH : 다른 종류의 아이템이 섞여있습니다.
							g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_QUEST_ITEM_NOT_MATCH), TYPE_NOR_OK, 1 );
							return -1;
						}
					}

					pItem = pItem->m_Next;

				}
				while( pItem );   // 교환창 아이템을 전부 체크.

				if( count != 10) // 수량체크.
				{
					//-- IDS_CHECK_ITEM_COUNT : 아이템 개수를 확인해주세요.
					g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_CHECK_ITEM_COUNT), TYPE_NOR_OK, 1 );
					return -1;
				}
			}
			break;


		case EVENT_TYPE_SK_200804:
			{
				int count = 0;

				do
				{
					vnum = pItem->m_Vnum;

					switch( vnum )
					{
					case 1932:
						count++;
						break;

					default: // 다른 아이템이 있으면...
						{
							//-- IDS_QUEST_ITEM_NOT_MATCH : 다른 종류의 아이템이 섞여있습니다.
							g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_QUEST_ITEM_NOT_MATCH), TYPE_NOR_OK, 1 );
							return -1;
						}
					}

					pItem = pItem->m_Next;

				}
				while( pItem );   // 교환창 아이템을 전부 체크.

				if( count != 1) // 수량체크.
				{
					//-- IDS_CHECK_ITEM_COUNT : 아이템 개수를 확인해주세요.
					g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_CHECK_ITEM_COUNT), TYPE_NOR_OK, 1 );
					return -1;
				}
			}
			break;

		case EVENT_TYPE_BP_CAMPAIN_200804:
			{
				int nitmeIndex_1933 = 0;
				int nitmeIndex_1934 = 0;

				do
				{
					vnum = pItem->m_Vnum;

					switch( vnum )
					{
					case 1933:
						nitmeIndex_1933++;
						break;
					case 1934:
						nitmeIndex_1934++;
						break;

					default: // 다른 아이템이 있으면...
						{
							//-- IDS_QUEST_ITEM_NOT_MATCH : 다른 종류의 아이템이 섞여있습니다.
							g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_QUEST_ITEM_NOT_MATCH), TYPE_NOR_OK, 1 );
							return -1;
						}
					}

					pItem = pItem->m_Next;

				}
				while( pItem );   // 교환창 아이템을 전부 체크.

				if( nitmeIndex_1933 + nitmeIndex_1934 != 1) // 수량체크.
				{
					//-- IDS_CHECK_ITEM_COUNT : 아이템 개수를 확인해주세요.
					g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_CHECK_ITEM_COUNT), TYPE_NOR_OK, 1 );
					return -1;
				}
			}
			break;

		//  [10/29/2008 parkmj] 일본보석함 시스템
		case EVENT_TYPE_LP_GIFTBOX_CHANGE_JP:
			{
				int nitemIndex_1942 = 0; // 보석함

				do
				{
					vnum = pItem->m_Vnum;

					switch( vnum )
					{
					case 1942:
						nitemIndex_1942++;
						break;

					default: // 다른 아이템이 있으면...
						{
							//-- IDS_QUEST_ITEM_NOT_MATCH : 다른 종류의 아이템이 섞여있습니다.
							g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_QUEST_ITEM_NOT_MATCH), TYPE_NOR_OK, 1 );
							return -1;
						}
					}

					pItem = pItem->m_Next;

				}
				while( pItem );   // 교환창 아이템을 전부 체크.

				if( nitemIndex_1942 == 1 ) //&& nItemIndex_2061 == 0 )
				{
					g_pTcpIp->SendNetMessage("ex_item jewelbox\n");
				}

				else
				{
					//-- IDS_CHECK_ITEM_COUNT : 아이템 개수를 확인해주세요.
					g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_CHECK_ITEM_COUNT), TYPE_NOR_OK, 1 );
					return  -1;
				}
			}
			break;

		case EVENT_TYPE_LP_GIFTBOX_CHANGE:
			{
				int nitemIndex_1942 = 0; // 보석함
//				int nItemIndex_2061 = 0; // 놈놈놈 이벤트용 선물 상자, 이곳에서 교환하도록 하자.

				do
				{
					vnum = pItem->m_Vnum;

					switch( vnum )
					{
					case 1942:
						nitemIndex_1942++;
						break;
//					case 2061:	nItemIndex_2061++; break;

					default: // 다른 아이템이 있으면...
						{
							//-- IDS_QUEST_ITEM_NOT_MATCH : 다른 종류의 아이템이 섞여있습니다.
							g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_QUEST_ITEM_NOT_MATCH), TYPE_NOR_OK, 1 );
							return -1;
						}
					}

					pItem = pItem->m_Next;

				}
				while( pItem );   // 교환창 아이템을 전부 체크.

				if( nitemIndex_1942 == 1 ) //&& nItemIndex_2061 == 0 )
				{
					g_pTcpIp->SendNetMessage("ex_item lp_jwel_box\n");
				}
				else
				{
					g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_CHECK_ITEM_COUNT), TYPE_NOR_OK, 1 );
					return  -1;
				}


			}
			break;

		case EVENT_TYPE_NEW_DEFENITEM_ITEM_EXCH:
			{
				int DrakeHunt[16] = {0,};
				int DrakeHuntTotal = 0;

				int AncientRelic_wepon[5] = {0,};
				int AncientRelic_wepon_Total = 0;

				int AncientRelic_amor[4] = {0,};
				int AncientRelic_amor_Total = 0;

				while(pItem)
				{
					vnum = pItem->m_Vnum;

					if( vnum >=2028 && vnum <= 2043 )	// 용 사냥꾼의 인장
					{
						DrakeHunt[vnum-2028] ++;
						DrakeHuntTotal ++;

						if( DrakeHunt[vnum-2028] > 1 || DrakeHuntTotal > 16)
						{
							//-- IDS_CHECK_ITEM_COUNT : 아이템 개수를 확인해주세요.
							g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_CHECK_ITEM_COUNT), TYPE_NOR_OK, 1 );
							return -1;
						}
					}
					else if( vnum >= 2044 && vnum <=2047 )  // 고대 유물 (방어구)
					{
						AncientRelic_amor[vnum-2044] ++;
						AncientRelic_amor_Total ++;

						if( AncientRelic_amor[vnum-2044] > 1 || AncientRelic_amor_Total > 1)
						{
							//-- IDS_CHECK_ITEM_COUNT : 아이템 개수를 확인해주세요.
							g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_CHECK_ITEM_COUNT), TYPE_NOR_OK, 1 );
							return -1;
						}
					}
					else if( vnum >= 1078 && vnum <= 1081 )  // 고대 유물 (무기) // 하이브리더는 1958이다 밑에서 처리
					{
						AncientRelic_wepon[vnum-1078] ++;
						AncientRelic_wepon_Total ++;

						if( AncientRelic_wepon[vnum-1078] > 1 || AncientRelic_wepon_Total > 1)
						{
							//-- IDS_CHECK_ITEM_COUNT : 아이템 개수를 확인해주세요.
							g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_CHECK_ITEM_COUNT), TYPE_NOR_OK, 1 );
							return -1;
						}
					}
					else if( vnum == 1958 ) // 고대 유물 (무기) 하이브리더 교환 품목 인덱스가 떨어져 있다 ㅡㅡ;;
					{
						AncientRelic_wepon[4] ++;
						AncientRelic_wepon_Total ++;

						if( AncientRelic_wepon[4] > 1 || AncientRelic_wepon_Total > 1)
						{
							//-- IDS_CHECK_ITEM_COUNT : 아이템 개수를 확인해주세요.
							g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_CHECK_ITEM_COUNT), TYPE_NOR_OK, 1 );
							return -1;
						}
					}
					else
					{
						//-- IDS_QUEST_ITEM_NOT_MATCH : 다른 종류의 아이템이 섞여있습니다.
						g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_QUEST_ITEM_NOT_MATCH), TYPE_NOR_OK, 1 );
						return -1;
					}

					pItem = pItem->m_Next;
				}

				if( DrakeHuntTotal != 16 || AncientRelic_wepon_Total != 1 || AncientRelic_amor_Total != 1)
				{
					//-- IDS_CHECK_ITEM_COUNT : 아이템 개수를 확인해주세요.
					g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_CHECK_ITEM_COUNT), TYPE_NOR_OK, 1 );
					return -1;
				}
			}
			break;

		case EVENT_TYPE_2008_BEIJING_OLYMPIC:
			{
				int item2118 = 0;
				int item2119 = 0;
				int item2120 = 0;

				while( pItem )	// 교환창 아이템을 전부 체크.
				{
					switch( pItem->m_Vnum )
					{
					case 2118:
						item2118++;
						break;
					case 2119:
						item2119++;
						break;
					case 2120:
						item2120++;
						break;

					default: // 다른 아이템이 있으면...
						{
							//-- IDS_QUEST_ITEM_NOT_MATCH : 다른 종류의 아이템이 섞여있습니다.
							g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_QUEST_ITEM_NOT_MATCH), TYPE_NOR_OK, 1 );
							return -1;
						}
					}
					pItem = pItem->m_Next;

				}

				int total = item2118 + item2119 + item2120;

				// 12 , 8 , 4
				//if( total == 20 && ( (total == item2118) || (total == item2119) || (total == item2120) ) )
				if( total == BEIJING_EVENT_MAX &&
						item2118 == BEIJING_EVENT_GOLD_NUM &&
						item2119 == BEIJING_EVENT_SILVER_NUM &&
						item2120 == BEIJING_EVENT_BRONZE_NUM )
				{
					if(g_pTcpIp)
						g_pTcpIp->SendNetMessage("ex_item beijing_olympic\n");
				}
				else
				{
					//-- IDS_QUEST_ITEM_NOT_MATCH : 다른 종류의 아이템이 섞여있습니다.
					g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_QUEST_ITEM_NOT_MATCH), TYPE_NOR_OK, 1 );
					return -1;
				}
			}
			break;
		case EVENT_TYPE_2008_GEMSTONE:
			{
				//	1159 , 24
				int num_1159 = 0;
				while( pItem )
				{
					if( pItem->m_Vnum == 1159 )
						++num_1159;
					else
						return -1;

					pItem = pItem->m_Next;
				}

				if( num_1159 == GEM_STONE_2008_MAX )
				{
					if(g_pTcpIp)
						g_pTcpIp->SendNetMessage("ex_item gem_stone\n");
				}
				else
				{
					g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_CHECK_ITEM_COUNT), TYPE_NOR_OK, 1 );
					return -1;
				}
			}
			break;

		case EVENT_TYPE_HALLOWEEN:
			{
				int num_1109 = 0; // 호박
				while( pItem )
				{
					if( pItem->m_Vnum == 1109 )
						++num_1109;
					else
						return -1;

					pItem = pItem->m_Next;
				}

				if( num_1109 == 24 )
				{
					if(g_pTcpIp)
						g_pTcpIp->SendNetMessage("ex_item hall\n");

				}
				else
				{
					g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_CHECK_ITEM_COUNT), TYPE_NOR_OK, 1 );
					return -1;
				}
			}
			break;
		case EVENT_TYPE_081114_PREMIUM_EVENT:
			{
				int num_2160 = 0; // 행운의상자
				int num_1942 = 0; // 보석함

				while( pItem )
				{
					if( pItem->m_Vnum == 2160 )
					{
						++num_2160;
						if( num_2160 != 1)
							break;
					}
					else if( pItem->m_Vnum == 1942 )
					{
						++num_1942;
						if( num_1942 != 1)
							break;
					}
					else
					{
						//-- IDS_QUEST_ITEM_NOT_MATCH : 다른 종류의 아이템이 섞여있습니다.
						g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_QUEST_ITEM_NOT_MATCH), TYPE_NOR_OK, 1 );
						return -1;
					}
					pItem = pItem->m_Next;
				}

				if( num_2160 == 1 && num_1942 == 0 )
				{
					if(g_pTcpIp)
						g_pTcpIp->SendNetMessage("ex_item lp_goodluck_box\n");

				}
				else if( num_2160 == 0 && num_1942 == 1 )
				{
					if(g_pTcpIp)
						g_pTcpIp->SendNetMessage("ex_item lp_jwel_box\n");
				}
				else
				{
					//-- 아이템 갯수를 확인해주세요.
					g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_CHECK_ITEM_COUNT), TYPE_NOR_OK, 1 );
					return -1;
				}
			}
			break;
			
		case EVENT_TYPE_BIRTH_CELEBRATE_2010:
			{

				int num_count = 0;
				while( pItem )	// 교환창 아이템을 전부 체크.
				{
					if( pItem->m_Vnum != 2689)
					{
						g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_QUEST_ITEM_NOT_MATCH), TYPE_NOR_OK, 1 );
						return -1;
					}
					else
					{
						num_count++;

					}
					pItem = pItem->m_Next;
				}

				if( num_count > 1 )
				{
					g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_CHECK_ITEM_COUNT), TYPE_NOR_OK, 1 );
					return -1;
				}

				LH_SEND_NET_MSG( "ex_item birthday\n" );
			}
			break;

		case EVENT_TYPE_BIRTH_2010:
			{
				// 2589-2603
				int ex_item[9] = {0,};

				while( pItem )	// 교환창 아이템을 전부 체크.
				{
					if( pItem->m_Vnum >= 2676 && pItem->m_Vnum <= 2684 )
					{
						ex_item[pItem->m_Vnum - 2676]++;
					}
					else
					{
						g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_QUEST_ITEM_NOT_MATCH), TYPE_NOR_OK, 1 );
						return -1;
					}
					pItem = pItem->m_Next;
				}

				for( int i=0 ; i<9 ; i++ )
				{
					if( ex_item[i] != 1 )
					{
						g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_CHECK_ITEM_COUNT), TYPE_NOR_OK, 1 );
						return -1;
					}
				}

				LH_SEND_NET_MSG( "ex_item g2010 1\n" );
			}
			break;
		case EVENT_TYPE_PRESENT_2010:
			{
				int num_count = 0;
				while( pItem )	// 교환창 아이템을 전부 체크.
				{
					if( pItem->m_Vnum != 2685)
					{
						g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_QUEST_ITEM_NOT_MATCH), TYPE_NOR_OK, 1 );
						return -1;
					}
					else
					{
						num_count++;

					}
					pItem = pItem->m_Next;
				}

				if( num_count > 1 )
				{
					g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_CHECK_ITEM_COUNT), TYPE_NOR_OK, 1 );
					return -1;
				}

				LH_SEND_NET_MSG( "ex_item g2010 2\n" );
			}
			break;

		case EVENT_LP_PRESENT_BOX:
			{
				int num_count = 0;
				int num_count_2742 = 0;
				while(pItem)
				{
					if( pItem->m_Vnum == 2742 )
					{
						num_count_2742++;
						num_count++;
					}
					else
					{
						num_count++;
					}
					pItem = pItem->m_Next;
				}				

				if( num_count_2742 > 0 )
				{
					if( num_count > 1 )
					{
						g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_CHECK_ITEM_COUNT), TYPE_NOR_OK, 1 );
						return -1;
					}

					LH_SEND_NET_MSG( "ex_item lpbox\n" );
				}
				else
				{
					LH_SEND_NET_MSG( "ex_item bbox\n" );
				}
			}
			break;

		case EVENT_GOD_WEAPON_EXPERIENCE:
			{
				int num_item=0;
				while( pItem )	// 교환창 아이템을 전부 체크.
				{
					switch(pItem->m_Vnum)
					{
					case 2774:
					case 2775:
					case 2776:
					case 2777:
					case 2778:
					case 2779:
					case 2780:
						{
							num_item++;
						}
						break;
					default:
						{
							g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_QUEST_ITEM_NOT_MATCH), TYPE_NOR_OK, 1 );
							return -1;
						}
						break;
					}

					if( num_item > 1 )
					{
						g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_CHECK_ITEM_COUNT), TYPE_NOR_OK, 1 );
						return -1;
					}

					pItem = pItem->m_Next;
				}

				LH_SEND_NET_MSG( "ex_item glpbox\n" );
			}
			break;

		case EVENT_TYPE_NGA_OPEN:
			{
				int ex_item1[7] = {0,};

				while( pItem )
				{
					switch( pItem->m_Vnum )
					{
					case 390:
						ex_item1[0]++;
						break;
					case 391:
						ex_item1[1]++;
						break;
					case 392:
						ex_item1[2]++;
						break;
					case 393:
						ex_item1[3]++;
						break;
					case 394:
						ex_item1[4]++;
						break;
					case 395:
						ex_item1[5]++;
						break;
					case 396:
						ex_item1[6]++;
						break;

					default:
						g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_QUEST_ITEM_NOT_MATCH), TYPE_NOR_OK, 1 );
						return -1;
					}
					pItem = pItem->m_Next;
				}

				for( int i=0 ; i<7 ; i++ )
				{
					if( ex_item1[i] != 1 )
					{
						g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_CHECK_ITEM_COUNT), TYPE_NOR_OK, 1 );
						return -1;
					}
				}

				LH_SEND_NET_MSG( "ex_item ngaopen\n" );
				return 1;
			}
			break;
		case EVENT_TYPE_AWARD_2010:
			{
				int num_count = 0;
				while(pItem)
				{
					if(pItem->m_Vnum != 2783)
					{
						g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_QUEST_ITEM_NOT_MATCH), TYPE_NOR_OK, 1 );
						return -1;
					}
					else
					{
						num_count++;
					}
					pItem = pItem->m_Next;
				}

				if( num_count > 1 )
				{
					g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_CHECK_ITEM_COUNT), TYPE_NOR_OK, 1 );
					return -1;
				}

				LH_SEND_NET_MSG( "ex_item e049\n" );
			}
			break;
		case EVENT_TYPE_NEWYEAR_2011:
			{
				int ex_item1[16] = {0,};

				while( pItem )
				{
					switch( pItem->m_Vnum )
					{
					case 2790:
						ex_item1[0]++;
						break;
					case 2791:
						ex_item1[1]++;
						break;
					case 2792:
						ex_item1[2]++;
						break;
					case 2793:
						ex_item1[3]++;
						break;
					case 2794:
						ex_item1[4]++;
						break;
					case 2795:
						ex_item1[5]++;
						break;
					case 2796:
						ex_item1[6]++;
						break;
					case 2797:
						ex_item1[7]++;
						break;
					case 2798:
						ex_item1[8]++;
						break;
					case 2799:
						ex_item1[9]++;
						break;
					case 2800:
						ex_item1[10]++;
						break;
					case 2801:
						ex_item1[11]++;
						break;
					case 2802:
						ex_item1[12]++;
						break;
					case 2803:
						ex_item1[13]++;
						break;
					case 2804:
						ex_item1[14]++;
						break;
					case 2805:
						ex_item1[15]++;
						break;

					default:
						g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_QUEST_ITEM_NOT_MATCH), TYPE_NOR_OK, 1 );
						return -1;
					}
					pItem = pItem->m_Next;
				}
				for( int i=0 ; i<16 ; i++ )
				{
					if( ex_item1[i] != 1 )
					{
						g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_CHECK_ITEM_COUNT), TYPE_NOR_OK, 1 );
						return -1;
					}
				}

				LH_SEND_NET_MSG( "ex_item e50\n" );
				return 1;
			}
			break;

		case EVENT_TYPE_NEWYEAR_LUCKYBAG: //MARVEL EVENT
			{
				int ex_item1[6] = {0,};

				while( pItem )
				{
					switch( pItem->m_Vnum )
					{
					case 4500:
						ex_item1[0]++;
						break;
					case 4501:
						ex_item1[1]++;
						break;
					case 4502:
						ex_item1[2]++;
						break;
					case 4503:
						ex_item1[3]++;
						break;
					case 4504:
						ex_item1[4]++;
						break;
					case 4505:
						ex_item1[5]++;
						break;
					default:
						g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_QUEST_ITEM_NOT_MATCH), TYPE_NOR_OK, 1 );
						return -1;
					}
					pItem = pItem->m_Next;
				}
				for( int i=0 ; i<6 ; i++ )
				{
					if( ex_item1[i] != 1 )
					{
						g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_CHECK_ITEM_COUNT), TYPE_NOR_OK, 1 );
						return -1;
					}
				}

				LH_SEND_NET_MSG( "ex_item e51\n" );
				return 1;
			}
			break;

			case EVENT_TYPE_NATAL: //MARVEL EVENT
			{
				int ex_item1[4] = {0,};

				while( pItem )
				{
					switch( pItem->m_Vnum )
					{
					case 11218:
						ex_item1[0]++;
						break;
					case 11219:
						ex_item1[1]++;
						break;
					case 11220:
						ex_item1[2]++;
						break;
					case 11221:
						ex_item1[3]++;
						break;
					default:
						g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_QUEST_ITEM_NOT_MATCH), TYPE_NOR_OK, 1 );
						return -1;
					}
					pItem = pItem->m_Next;
				}
				for( int i=0 ; i<4 ; i++ )
				{
					if( ex_item1[i] != 1 )
					{
						g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_CHECK_ITEM_COUNT), TYPE_NOR_OK, 1 );
						return -1;
					}
				}

				LH_SEND_NET_MSG( "ex_item n2019\n" );
				return 1;
			}
			break;

			case EVENT_TYPE_NEWYEAR_CARD: //DEKAREN CARD
			{
				int ex_item1[24] = {0,};

				while( pItem )
				{
					switch( pItem->m_Vnum )
					{
					case 4700:
						ex_item1[0]++;
						break;
					case 4701:
						ex_item1[1]++;
						break;
					case 4702:
						ex_item1[2]++;
						break;
					case 4703:
						ex_item1[3]++;
						break;
					case 4704:
						ex_item1[4]++;
						break;
					case 4705:
						ex_item1[5]++;
						break;
					case 4706:
						ex_item1[6]++;
						break;
					case 4707:
						ex_item1[7]++;
						break;
					case 4708:
						ex_item1[8]++;
						break;
					case 4709:
						ex_item1[9]++;
						break;
					case 4710:
						ex_item1[10]++;
						break;
					case 4711:
						ex_item1[11]++;
						break;
					case 4712:
						ex_item1[12]++;
						break;
					case 4713:
						ex_item1[13]++;
						break;
					case 4714:
						ex_item1[14]++;
						break;
					case 4715:
						ex_item1[15]++;
						break;
					case 4716:
						ex_item1[16]++;
						break;
					case 4717:
						ex_item1[17]++;
						break;
					case 4718:
						ex_item1[18]++;
						break;
					case 4719:
						ex_item1[19]++;
						break;
					case 4720:
						ex_item1[20]++;
						break;
					case 4721:
						ex_item1[21]++;
						break;
					case 4722:
						ex_item1[22]++;
						break;
					case 4723:
						ex_item1[23]++;
						break;
					default:
						g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_QUEST_ITEM_NOT_MATCH), TYPE_NOR_OK, 1 );
						return -1;
					}
					pItem = pItem->m_Next;
				}
				for( int i=0 ; i<24 ; i++ )
				{
					if( ex_item1[i] != 1 )
					{
						g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_CHECK_ITEM_COUNT), TYPE_NOR_OK, 1 );
						return -1;
					}
				}

				LH_SEND_NET_MSG( "ex_item y55\n" );
				return 1;
			}
			break;
			/*{
				// 1390 1391
				int ex_item1 = 0; // 은복주머니
				int ex_item2 = 0; // 금복주머니

				while( pItem )
				{
					switch( pItem->m_Vnum )
					{
					case 1390:
						ex_item1++;
						break;
					case 1391:
						ex_item2++;
						break;

					default:
						g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_QUEST_ITEM_NOT_MATCH), TYPE_NOR_OK, 1 );
						return -1;
					}
					pItem = pItem->m_Next;
				}

				if( ex_item1 == 1 && ex_item2 == 0 )
				{
					LH_SEND_NET_MSG( "ex_item e51 1\n" );
				}
				else if( ex_item1 == 0 && ex_item2 == 1 )
				{
					LH_SEND_NET_MSG( "ex_item e51 2\n" );
				}
				else
				{
					g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_CHECK_ITEM_COUNT), TYPE_NOR_OK, 1 );
					return -1;
				}
			}
			break;*/
		case EVENT_TYPE_FORLOM_MAP:
			{
				int ex_item[6] = {0,};

				while( pItem )
				{
					switch(pItem->m_Vnum)
					{
					case 2808:
						ex_item[0]++;
						break;
					case 2809:
						ex_item[1]++;
						break;
					case 2810:
						ex_item[2]++;
						break;
					case 2811:
						ex_item[3]++;
						break;
					case 2812:
						ex_item[4]++;
						break;
					case 2813:
						ex_item[5]++;
						break;
					default:
						g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_QUEST_ITEM_NOT_MATCH), TYPE_NOR_OK, 1 );
						return -1;
					}
					pItem = pItem->m_Next;
				}

				for( int i=0; i<6 ; i++ )
				{
					if( ex_item[i] != 1)
					{
						g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_CHECK_ITEM_COUNT), TYPE_NOR_OK, 1 );
						return -1;
					}
				}

				LH_SEND_NET_MSG( "ex_item e52\n" );
			}
			break;
		case EVENT_TYPE_FORLOM_DEVICE:
			{
				int ex_item[2] = {0,};

				while( pItem )
				{
					switch(pItem->m_Vnum)
					{
					case 2815:
						ex_item[0]++;
						break;
					case 2807:
						ex_item[1]++;
						break;
					default:
						g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_QUEST_ITEM_NOT_MATCH), TYPE_NOR_OK, 1 );
						return -1;
					}
					pItem = pItem->m_Next;
				}

				for( int i=0; i<2 ; i++ )
				{
					if( ex_item[i] != 3)
					{
						g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_CHECK_ITEM_COUNT), TYPE_NOR_OK, 1 );
						return -1;
					}
				}

				LH_SEND_NET_MSG( "ex_item e53\n" );
			}
			break;
		case EVENT_TYPE_FORLOM_DEVICE_DM:
			{
				// 레벨제한 350 이상
				const __int64 NEED_MONEY = 10000000;
				if( g_pRoh->m_Money < NEED_MONEY )
				{
					g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_GUH_LAIM), TYPE_NOR_OK, 1 );
					return -1;
				}

				int ex_item = 0;
				while( pItem )
				{
					switch(pItem->m_Vnum)
					{
					case 1930:
						ex_item++;
						break;
					default:
						g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_QUEST_ITEM_NOT_MATCH), TYPE_NOR_OK, 1 );
						return -1;
					}
					pItem = pItem->m_Next;
				}

				if( ex_item != 1)
				{
					g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_CHECK_ITEM_COUNT), TYPE_NOR_OK, 1 );
					return -1;
				}

				LH_SEND_NET_MSG( "ex_item e54\n" );
			}
			break;

		case EVENT_TYPE_GREENBUD_2011:
			{
				int ex_item = 0;
				while( pItem )
				{
					switch(pItem->m_Vnum)
					{
					case 2853:
						ex_item++;
						break;
					default:
						g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_QUEST_ITEM_NOT_MATCH), TYPE_NOR_OK, 1 );
						return -1;
					}
					pItem = pItem->m_Next;
				}

				if( ex_item != 10 )
				{
					g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_CHECK_ITEM_COUNT), TYPE_NOR_OK, 1 );
					return -1;
				}

				LH_SEND_NET_MSG( "ex_item e55\n" );
			}
			break;

		case EVENT_TYPE_NEW_SUPPORT_2011:
			{
				int ex_item = 0;
				CItem * selected_item = NULL;
				while( pItem )
				{
					switch(pItem->m_Vnum)
					{
					case 2879:	//	ex_item++; break;		// 30~49
					case 2880:	//	ex_item++; break;		// 50~69
					case 2881:	//	ex_item++; break;		// 70~89
					case 2882:	//	ex_item++; break;		// 90~119
					case 2883:	//	ex_item++; break;		// 120~149
					case 2884:	//	ex_item++; break;		// 150~179
					case 2885:
						ex_item++;
						selected_item = pItem;
						break;		// 180~
					default:
						g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_QUEST_ITEM_NOT_MATCH), TYPE_NOR_OK, 1 );
						return -1;
					}
					pItem = pItem->m_Next;
				}

				if( ex_item != 1 )
				{
					g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_CHECK_ITEM_COUNT), TYPE_NOR_OK, 1 );
					return -1;
				}


				if( selected_item )
				{
					if( g_pRoh->m_Level < GET_ITEM_LEVEL(selected_item)
//						|| g_pRoh->m_Level > GET_ITEM_MAXLEVEL(selected_item)
					  )
					{
						g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_GUILD_INVASION_FAIL), TYPE_NOR_OK, 1 );
						return -1;
					}
				}

				LH_SEND_NET_MSG( "ex_item e56\n" );
			}
			break;

		case EVENT_TYPE_SUMMER_2011:
			{
				int ex_item = 0;
				while(pItem)
				{
					switch(pItem->m_Vnum)
					{
					case 2930:
						{
							ex_item++;
						}
						break;
					default:
						{
							g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_QUEST_ITEM_NOT_MATCH), TYPE_NOR_OK, 1 );
							return -1;
						}
						break;
					}
					pItem = pItem->m_Next;
				}

				if( ex_item != 3 && ex_item != 10 )
				{
					g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_CHECK_ITEM_COUNT), TYPE_NOR_OK, 1 );
					return -1;
				}

				if( ex_item == 3 )
					LH_SEND_NET_MSG( "ex_item e57 1\n" );
				else if( ex_item == 10 )
					LH_SEND_NET_MSG( "ex_item e57 2\n" );
			}
			break;

		case EVENT_TYPE_10YEAR_ITEM:
			{
				LH_SEND_NET_MSG( "comm_eve\n" );
			}
			break;
		case EVENT_BULLION_EXCHANGE:
			{
				LH_SEND_NET_MSG( "comm_eve\n" );
			}
			break;

		//case EVENT_TYPE_10YEAR_TOKEN:
		//	{
		//		// LH_SEND_NET_MSG( "\n" );
		//	}
		//	break;
		//case EVENT_TYPE_10YEAR_COUPON:
		//	{
		//		// LH_SEND_NET_MSG( "ex_item e59\n" );
		//	}
		//	break;
		//case EVENT_TYPE_10YEAR_BOX:
		//	{
		//		// LH_SEND_NET_MSG( "ex_item e60\n" );
		//	}
		//	break;

		case EVENT_TYPE_S1TOS2:
			{
				int ex_item = 0;
				while(pItem)
				{
					switch(pItem->m_Vnum)
					{
					case 2732:
						{
							ex_item++;
						}
						break;
					default:
						{
							g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_QUEST_ITEM_NOT_MATCH), TYPE_NOR_OK, 1 );
							return -1;
						}
						break;
					}
					pItem = pItem->m_Next;
				}

				if( ex_item != 10 )
				{
					g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_CHECK_ITEM_COUNT), TYPE_NOR_OK, 1 );
					return -1;
				}

				if( ex_item == 10 )
					LH_SEND_NET_MSG( "ex_item e61\n" );
			}
			break;

		case EVENT_TYPE_JPN_9YEAR_ITEM:
			{
				bool bOK = false;

				const int nCount = 7;
				int nItemVnum[nCount]= {2676,2677,2678,2679,2680,2681,2682};

				CItem * pItem = g_pRoh->m_ExgInven;
				int nItemCount = 0;
				for ( nItemCount = 0 ; NULL != pItem; pItem = pItem->m_Next, ++nItemCount )
				{
					bool bEqual = false;
					for ( int i=0; i<nCount; ++i)
					{
						if ( nItemVnum[i] == pItem->m_Vnum )
						{
							bEqual = true;
							nItemVnum[i] = 0;
							break;
						}
					}
					bOK = bEqual;
					if ( false == bEqual )
					{
						break;
					}
				}

				if ( false == bOK || nItemCount != nCount )
				{
					//교환 불가
					g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_INVEN_EXCHA_UNABLE), TYPE_NOR_OK, 1 );
					return -1;
				}

				LH_SEND_NET_MSG( "ex_item g2010\n" );
			}
			break;
		case EVENT_TYPE_PCBANG_ARUPAKA:
			{
				int count = 0;

				do
				{
					vnum = pItem->m_Vnum;

					switch( vnum )
					{
					case 3561:
						count++;
						break;

					default: // 다른 아이템이 있으면...
						{
							//-- IDS_QUEST_ITEM_NOT_MATCH : 다른 종류의 아이템이 섞여있습니다.
							g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_QUEST_ITEM_NOT_MATCH), TYPE_NOR_OK, 1 );
							return -1;
						}
					}

					pItem = pItem->m_Next;

				}
				while( pItem );   // 교환창 아이템을 전부 체크.

				if( count != 1) // 수량체크.
				{
					//-- IDS_CHECK_ITEM_COUNT : 아이템 개수를 확인해주세요.
					g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_CHECK_ITEM_COUNT), TYPE_NOR_OK, 1 );
					return -1;
				}

				LH_SEND_NET_MSG("ex_item timeitemswap\n");
			}
			break;
		}
	}

	if( g_pTcpIp )
	{
		switch( m_iType )
		{
		case 3:
			g_pTcpIp->SendNetMessage("ex_item pp\n");
			break;
		case 4:
		case 6: // 뫼비우스 아레나 신비의 선물 교환에서 mungmae-2005/10/5 맞고&고스톱머니로 바꿈, 3.1절 이벤트
			g_pTcpIp->SendNetMessage("ex_item\n");
			//	g_pTcpIp->SendNetMessage("ex_item 2p4p\n");
			break;
		case 5:
		case EVENT_TYPE_THAI_XMAS_TREE: ///산타추가 // 크리스마스 트리도 xmas 패킷으로 보낸다. 구분은 서버에서 알아서...
			//		g_pTcpIp->SendNetMessage("ex_item xmas\n"); // 산타 이벤트.
			g_pTcpIp->SendNetMessage("ex_item bag\n"); // 070828 책가방 이벤트.
			break;
		case 7: ///대만재신이벤트
			g_pTcpIp->SendNetMessage("ex_item god\n");
			break;
		case EVENT_TYPE_WORLDCUP: // 월드컵 공 교환 이벤트.
			g_pTcpIp->SendNetMessage("ex_item wc\n");
			break;
		case EVENT_TYPE_THAI_MOTHERS_DAY: // 태국 왕비 생신 이벤트.
			g_pTcpIp->SendNetMessage("ex_item mother\n");
			break;
		case EVENT_TYPE_LAGHAIM_STRING: // 라그하임 글자 교환 이벤트, 2000일 이벤트 포함.
			g_pTcpIp->SendNetMessage("ex_item puzzle\n");
			break;
		case EVENT_TYPE_THAI_VALENTINE:
		case EVENT_TYPE_KOR2007_VALENTINE: // 태국 발렌타인 데이 이벤트
			g_pTcpIp->SendNetMessage("ex_item val\n");
			break;
		case EVENT_TYPE_KOR2007_NEWYEAR:
			g_pTcpIp->SendNetMessage("ex_item newyear\n");
			break;
		case EVENT_TYPE_KOR2008_NEWYEAR:// else if( m_iType == EVENT_TYPE_KOR2007_NEWYEAR )
			g_pTcpIp->SendNetMessage("ex_item newyear2008\n");
			break;
		case EVENT_TYPE_KOR2007_WHITEDAY: // 2007.03.14 화이트
			g_pTcpIp->SendNetMessage("ex_item val\n");
			break;
		case EVENT_TYPE_KOR_6YEAR: // [8/6/2007 Theodoric] 6주년 기념 이벤트
			g_pTcpIp->SendNetMessage("ex_item 6year\n");
			break;
		case EVENT_TYPE_KOR2007_PEPERO: // 2007.11.01 빼빼로 데이 이벤트, 빼빼로 데이 패킷으로 변경 할것.
			{
				g_pTcpIp->SendNetMessage("ex_item pepero\n");
				return 1;
			}
			break;
		case EVENT_TYPE_STARSTONE_EXCH:
			break;
		case EVENT_TYPE_GIRASI_EXCH:
		case EVENT_TYPE_BALLOON_EXCH:
			g_pTcpIp->SendNetMessage("lfexch\n");
			break;
		case EVENT_TYPE_SK_200804:
			g_pTcpIp->SendNetMessage("ex_item sk_service\n");
			break;
		case EVENT_TYPE_BP_CAMPAIN_200804:
			g_pTcpIp->SendNetMessage("ex_item bp_gift\n");
			break;
		case EVENT_TYPE_NEW_DEFENITEM_ITEM_EXCH:
			g_pTcpIp->SendNetMessage("ex_item  newfc\n");
			break;

		}
	}

	//-- Remove All
	if( m_iType != 4
			&& m_iType != EVENT_TYPE_THAI_VALENTINE
			&& m_iType != EVENT_TYPE_KOR2008_NEWYEAR
			&& m_iType != EVENT_TYPE_KOR2007_WHITEDAY
			&& m_iType != EVENT_TYPE_NEW_DEFENITEM_ITEM_EXCH
			&& m_iType != EVENT_LP_PRESENT_BOX
			&& m_iType != EVENT_TYPE_10YEAR_ITEM
			&& m_iType != EVENT_BULLION_EXCHANGE
	  ) // 태국 발렌타인 데이도 아이템 안지우게 함.(갯수 체크를 안하기 때문에)
	{
		CItem *nextItem = NULL;
		while (pItem)
		{
			nextItem = pItem->m_Next;
			delete pItem;
			pItem = nextItem;
		}

		if( g_pRoh )
			g_pRoh->m_ExgInven = NULL;

		int x_idx = 0, y_idx = 0;

		for (x_idx = 0; x_idx < EXG_SLOT_X_NUM; x_idx++)
		{
			for (y_idx = 0; y_idx < EXG_SLOT_Y_NUM; y_idx++)
				if( g_pRoh )
					g_pRoh->m_ExgSlot[x_idx][y_idx] = -1;
		}
	}
	return 1;
}


int CEventAncient::CheckMyslot_ParentsDay()
{
	int  vnum = -1;

	CItem* pItem = NULL;
	if( g_pRoh )
		pItem = g_pRoh->m_ExgInven;

	int a = 0, b = 0, c = 0;

	if(!pItem)
		return -1;

	while (pItem)
	{
		vnum = pItem->m_Vnum;
		if(vnum == 957)
			a++;
		else if(vnum ==958)
			b++;
		else if(vnum == 959)
			c++;
		else
		{
			//-- IDS_QUEST_ITEM_NOT_MATCH : 다른 종류의 아이템이 섞여있습니다.
			g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_QUEST_ITEM_NOT_MATCH), TYPE_NOR_OK, 1 );
			return -1;
		}
		pItem = pItem->m_Next;
	}

	if((a==1 && b==0 && c==1)
			||(a==1 && b==1 && c==1))
	{
		// 성공
		if( g_pTcpIp )
			g_pTcpIp->SendNetMessage("powerup\n");
	}
	else
	{
		//-- IDS_CHECK_EVENT_ITEM : 색종이,풀 아이템을 확인해 주세요
		g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_CHECK_EVENT_ITEM), TYPE_NOR_OK, 1 );
	}

	return 1;
}



BOOL CEventAncient::IsInside(int x, int y)
{
	if( !m_pBack )
		return false;
	return m_pBack->IsIn(x,y);
}

BOOL CEventAncient::IsPlayArea(int x, int y)
{
	if( !IsInside(x,y) && !g_pNk2DFrame->IsInPopup(x,y) )
		return true;

	return false;
}

void CEventAncient::SendMsg()
{
	if (g_pTcpIp)
		g_pTcpIp->SendNetMessage(m_MsgBuf);
}

void CEventAncient::Exchange(int result, int type, int vnum_money, int index, int gem_num)
{
	if(result == 0)
	{
		//실패
		if(type == 0)
		{
			//감정결과 실패
			RemoveAllMyInven();
			//-- IDS_EVENTACIENT_TOY : 이런! 장남감이 부셔져서 쓸모가 없네요.
			g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_EVENTACIENT_TOY), TYPE_NOR_OK, 1 );
		}
		else if(type == 1)
		{
			//-- IDS_QUEST_ITEM_NOT_MATCH : 다른 종류의 아이템이 섞여있습니다.
			g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_QUEST_ITEM_NOT_MATCH), TYPE_NOR_OK, 1 );
		}
	}
	else if(result == 1)
	{
		//성공
		if(type == 0)
		{
			// 아이템 교환
			ChangeTo(vnum_money, index, -1);
			//-- IDS_EVENTACIENT_HAVE_ITEM : 아이템을 선물로 받았습니다.
			g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_EVENTACIENT_HAVE_ITEM), TYPE_NOR_OK, 1 );
		}
		else if(type == 1)
		{
			// +아이템 교환
			ChangeTo(vnum_money, index, gem_num);
			//-- IDS_EVENTACIENT_HAVE_PLUSEITEM : 플러스 아이템을 선물로 받았습니다.
			g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_EVENTACIENT_HAVE_PLUSEITEM), TYPE_NOR_OK, 1 );
		}
		else if(type == 2)
		{
			//마석 교환
			ChangeTo(vnum_money, index, -1);
			//-- IDS_HAVE_MASUK : 마석을 선물로 받았습니다.
			g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_HAVE_MASUK), TYPE_NOR_OK, 1 );
		}
	}
	m_bExchange = FALSE;
}

void CEventAncient::ExchangeTree(int result, int type, int vnum, int index)
{
	if(result == 0)
	{
		//실패
		if(type == 1)
			//-- IDS_EVENTACIENT_ONLYONE : 1개씩만 교환 가능합니다.
			g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_EVENTACIENT_ONLYONE), TYPE_NOR_OK, 1 );
		else if(type == 2)
			//-- IDS_QUEST_ITEM_NOT_MATCH : 다른 종류의 아이템이 섞여있습니다.
			g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_QUEST_ITEM_NOT_MATCH), TYPE_NOR_OK, 1 );
		else if(type == 3)
			//-- IDS_EVENTACIENT_ERROR : Exchange Error!
			g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_EVENTACIENT_ERROR), TYPE_NOR_OK, 1 );
	}
	else if(result == 1)	//성공
		ChangeTo(vnum, index, -1);	//마석 교환

	m_bExchange = FALSE;
}
//////////////

void CEventAncient::MultiExchange( int NumItem, char* Packet ) // 이벤트 인터페이스 상에서 여러개 아이템 교환 받을때 들어오는 함수. (by 원석) ///다중교환
{
	// 교환 실패의 경우 여기로 들어오지 않고 위쪽의 Exchange() 함수에서 처리되고있다.(구조상 마음에 안 들긴하다! 하지만 고치려면 서버에서 수정해야하니 그냥 그대로 쓴다.)
	char TempStr[30] = {0,}; // 패킷을 파싱해올 버퍼. 한 인자가 30자리를 넘어버리면 곤란하다.(스트링이 아닌 숫자 인자들 이므로)
	int Temp1 = 0, Temp2 = 0, Temp3 = 0, Temp4 = 0, Temp5 = 0, Temp6 = 0;
	int x_idx, y_idx; // 아이템 넣을때 쓰는 루프용 변수.
	CItem *pNextItem = NULL;


	RemoveAllMyInven(); // 우선 한번 싹 지워준다.

	for( int i = 0 ; i < NumItem ; ++i ) // 서버로 부터 받은 아이템의 갯수.
	{
		// 아래의 순서로 정보가 반복되어 들어온다.(텍스트 파싱은 아래 함수에서 함.)
		Packet = AnyOneArg(Packet, TempStr);
		Temp1 = atoi(TempStr); // vnum
		Packet = AnyOneArg(Packet, TempStr);
		Temp2 = atoi(TempStr); // server index
		Packet = AnyOneArg(Packet, TempStr);
		Temp3 = atoi(TempStr); // AddFlag(마석 옵션값)
		Packet = AnyOneArg(Packet, TempStr);
		Temp4 = atoi(TempStr); // Plus값(보석+ 수치)
		Packet = AnyOneArg(Packet, TempStr);
		Temp5 = atoi(TempStr); // 인벤 X
		Packet = AnyOneArg(Packet, TempStr);
		Temp6 = atoi(TempStr); // 인벤 Y




		if (Temp1 == -1) // 아이템 번호가 잘 못들어왔으면 스킵.
			continue;
		/*
				if( pTempItem )
				{
					delete pTempItem;
				}
		*/
		CItem *pTempItem = new CItem;

		pTempItem->m_Index = Temp2;
		pTempItem->m_SlotX = Temp5;
		pTempItem->m_SlotY = Temp6;
		pTempItem->m_Vnum = Temp1;
		pTempItem->m_ProtoNum = g_ItemList.FindItem( Temp1 );

//		if( Temp4 > 0 ) // Plus 수치가 붙은 아이템일 경우.
//			pTempItem->SetGemNum( Temp4 );
		if( Temp4 > 0 )
		{
			if( GET_ITEM_LEVEL(pTempItem) >= 181 ) //다이아급
			{
				pTempItem->SetDiaGemNum( Temp4 );
			}
			else
			{
				pTempItem->SetGemNum( Temp4 );
			}
		}

		ResetItemValue(pTempItem);
		// 아이템의 스페셜값 셋팅
		pTempItem->m_Special = Temp3;
		SetSpecialValue( pTempItem, GET_TYPE(pTempItem), GET_SHAPE(pTempItem), Temp3, 0 );


		if( m_pNkCha->m_ExgInven == NULL )
		{
			m_pNkCha->m_ExgInven = pTempItem;
			pNextItem = m_pNkCha->m_ExgInven;

		}
		else
		{
			pNextItem->m_Next = pTempItem;
			pNextItem = pTempItem;
		}

		// 아이템을 넣어준다.
		if( Temp5 < 0 || Temp6 < 0 || Temp5+GET_ITEM_WIDTH(m_pNkCha->m_ExgInven) > EXG_SLOT_X_NUM
				|| Temp6+GET_ITEM_HEIGHT(m_pNkCha->m_ExgInven) > EXG_SLOT_Y_NUM ) // 좌표값 오류 검사
		{
			MessageBox( NULL, "Wrong Item Pos Come!", "Event_Ancient MultiExchange Error", MB_OK );
			continue; // 다음 아이템은 처리를 해준다.
		}
		else
		{
			for( x_idx = Temp5 ; x_idx < Temp5+GET_ITEM_WIDTH(m_pNkCha->m_ExgInven) ; ++x_idx )
			{
				for( y_idx = Temp6 ; y_idx < Temp6+GET_ITEM_HEIGHT(m_pNkCha->m_ExgInven) ; ++y_idx )
				{
					if( m_pNkCha->m_ExgSlot[x_idx][y_idx] != -1 ) // 만약 넣을려는 자리에 이미 아이템이 들어가 있으면...
					{
						RemoveAllMyInven(); // 이 경우는 깔끔하게 지워버린다.(엉키지 않게)
						MessageBox( NULL, "Wrong Item Pos Come! (Duplicated Pos)", "Event_Ancient MultiExchange Error", MB_OK );

						return; // 이 경우는 부득이 하게 리턴.(다른 for문 중이니까 continue는 에매하다.)

					}
					else // 정상적으로 넣을 수 있는 경우.
					{
						m_pNkCha->m_ExgSlot[x_idx][y_idx] = Temp2;
					}
				}
			}
		}

	}

	m_bExchange = FALSE; // 교환 락. (셋팅되어있으면 추가 교환이나 아이템 빼기가 안된다.)

	// 성공했다는 메세지를 띄워준다.
	//-- IDS_EVENTACIENT_HAVE_ITEM : 아이템을 선물로 받았습니다.
	g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_EVENTACIENT_HAVE_ITEM), TYPE_NOR_OK, 1 );

}


void CEventAncient::GetMsgParentsDay(BOOL bsuccess, int vnum, int sver_idx)
{
	if(bsuccess)
		ChangeTo(vnum, sver_idx, 0);
	else
		//-- IDS_EVENTACIENT_FAIL : 감정에 실패했습니다.
		g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_EVENTACIENT_FAIL), TYPE_NOR_OK, 1 );
}


void CEventAncient::ChangeTo(int vnum, int svr_idx, int gem_num)
{
	int x_idx = 0, y_idx = 0;

	RemoveAllMyInven();

	if (vnum == -1)
		return;

	if( g_pRoh )
	{
		g_pRoh->m_ExgInven = new CItem;

		if( g_pRoh->m_ExgInven )
		{
			g_pRoh->m_ExgInven->m_Index = svr_idx;
			g_pRoh->m_ExgInven->m_SlotX = 0;
			g_pRoh->m_ExgInven->m_SlotY = 0;
			g_pRoh->m_ExgInven->m_Vnum = vnum;
			g_pRoh->m_ExgInven->m_ProtoNum = g_ItemList.FindItem(vnum);

			if(gem_num > 0)
				g_pRoh->m_ExgInven->SetGemNum(gem_num);

			g_pRoh->m_ExgInven->m_Num0 = GET_ITEM_NUM0(g_pRoh->m_ExgInven);
			g_pRoh->m_ExgInven->m_Num1 = GET_ITEM_NUM1(g_pRoh->m_ExgInven);
			g_pRoh->m_ExgInven->m_Num2 = GET_ITEM_NUM2(g_pRoh->m_ExgInven);
			g_pRoh->m_ExgInven->m_Num3 = GET_ITEM_NUM3(g_pRoh->m_ExgInven);
			g_pRoh->m_ExgInven->m_Num4 = GET_ITEM_NUM4(g_pRoh->m_ExgInven);
			g_pRoh->m_ExgInven->m_Num5 = GET_ITEM_NUM5(g_pRoh->m_ExgInven);
			g_pRoh->m_ExgInven->m_Num6 = GET_ITEM_NUM6(g_pRoh->m_ExgInven);
			g_pRoh->m_ExgInven->m_Num7 = GET_ITEM_NUM7(g_pRoh->m_ExgInven);
			g_pRoh->m_ExgInven->m_Num8 = GET_ITEM_NUM8(g_pRoh->m_ExgInven);
			g_pRoh->m_ExgInven->m_Num9 = GET_ITEM_NUM9(g_pRoh->m_ExgInven);
			g_pRoh->m_ExgInven->m_MinLevel = GET_ITEM_LEVEL(g_pRoh->m_ExgInven);
			g_pRoh->m_ExgInven->m_MinLevel_Org = g_pRoh->m_ExgInven->m_MinLevel;
			g_pRoh->m_ExgInven->m_Int = GET_ITEM_INT(g_pRoh->m_ExgInven);
			g_pRoh->m_ExgInven->m_Dex = GET_ITEM_DEX(g_pRoh->m_ExgInven);
			g_pRoh->m_ExgInven->m_Weight = GET_WEIGHT(g_pRoh->m_ExgInven);

			g_pRoh->m_ExgInven->m_Next= NULL;
		}

		// Clear
		for (x_idx = 0; x_idx < EXG_SLOT_X_NUM; x_idx++)
		{
			for (y_idx = 0; y_idx < EXG_SLOT_Y_NUM; y_idx++)
			{
				g_pRoh->m_ExgSlot[x_idx][y_idx] = -1;
			}
		}
		// Add
		for (x_idx = 0; x_idx < GET_ITEM_WIDTH(g_pRoh->m_ExgInven); x_idx++)
		{
			for (y_idx = 0; y_idx < GET_ITEM_HEIGHT(g_pRoh->m_ExgInven); y_idx++)
			{
				g_pRoh->m_ExgSlot[x_idx][y_idx] = svr_idx;
			}
		}
	}
	m_bExchange = FALSE;
}


void CEventAncient::RemoveAllMyInven()
{
	CItem *pItem = NULL;
	CItem *nextItem = NULL;

	if( !g_pRoh )
		return;

	pItem = g_pRoh->m_ExgInven;

	while (pItem)
	{
		nextItem = pItem->m_Next;
		delete pItem;
		pItem = nextItem;
	}
	g_pRoh->m_ExgInven = NULL;

	int x_idx, y_idx;

	for (x_idx = 0; x_idx < EXG_SLOT_X_NUM; x_idx++)
	{
		for (y_idx = 0; y_idx < EXG_SLOT_Y_NUM; y_idx++)
		{
			g_pRoh->m_ExgSlot[x_idx][y_idx] = -1;
		}
	}
}

void CEventAncient::QuestExchange(int money)
{
	if(money == -1)
		//-- IDS_EVENTACIENT_FAIL : 감정에 실패했습니다.
		g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_EVENTACIENT_FAIL), TYPE_NOR_OK, 1 );
	else
	{
		RemoveAllMyInven();
		//-- IDS_EVENTACIENT_HAVE_LAIM2 : %d 라임을 얻으셨습니다.
		char strTemp[128] = "";
		sprintf(strTemp, G_STRING(IDS_EVENTACIENT_HAVE_LAIM2),money);
		g_pNk2DFrame->InsertPopup(strTemp, TYPE_NOR_OK, 1 );
	}
	m_bExchange= FALSE;
}

HRESULT CEventAncient::PopupMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, int nMsg)
{
	if( nMsg == 1 )
	{
		if(m_bQuestWindow)
		{
			if (g_pTcpIp)
			{
				g_pTcpIp->SendNetMessage("rg 2\n");	// 고대반지 회수
				m_bExchange = TRUE;
			}
		}
	}
	return 0L;
}

int CEventAncient::GetExItemNum(int item_no)
{
	if( item_no < 0 )
		return 0;

	CItem* pItem = NULL;
	if( g_pRoh )
		pItem = g_pRoh->m_ExgInven;

	int num = 0;

	while( pItem )
	{
		if( pItem->m_Vnum == item_no )
			++num;

		pItem = pItem->m_Next;
	}

	return num;
}

bool CEventAncient::LoadScript()
{
	return true;
}
