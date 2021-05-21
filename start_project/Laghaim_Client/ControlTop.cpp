#include "stdafx.h"
#include "Config.h"
#include "LHAutoPlay_Define.h"
#include "headers.h"
#include "UIMgr.h"
#include "main.h"
#include "ControlTop.h"
#include "ddutil.h"
#include "Nk2DFrame.h"
#include "NkMob.h"
#include "NkPet.h"
#include "NkMyPet.h"
#include "ReservationWindow.h"
#include "g_stringmanager.h"
#include "AutoPlay_test.h"
#include "ControlInven.h"
#include "ControlBottom.h"
#include "AlphaPopUp.h"
#include "Country.h"
#include "LHUI_OpEvent.h"
#include "Mouse.h"
#include "Tcpipcon.h"
#include "CommonConfig.h"

extern BOOL					g_bUseCashShop; ///유료상점
extern int					g_MatrixSvr;
extern	SVR_TYPE			g_SvrType;
extern DWORD				g_dwClientCountry;

CControlTop::CControlTop()
{
	m_pNkCha = NULL;
	m_pEtcInfo = new CAlphaPopUp;
	Init();
}

CControlTop::~CControlTop()
{
	DeleteRes();
	m_pNkCha = NULL;
	SAFE_DELETE(m_pEtcInfo);
}

void CControlTop::Init()
{
	if( g_bUseCashShop )
		m_IconCashShopBtn.SetFileName("top/btn_menu_cashshop");              ///유료상점	

	if( CommonConfig::Instance()->GetLagrush() == true )
	{
		if( !g_MatrixSvr )
		{
			m_IconAutoBtn.SetFileName("top/btn_menu_rushoption");
			m_iConAutoOnBtn.SetFileName("top/btn_menu_startrush");
		}
	}

	m_IconScoreBtn.SetFileName("top/btn_menu_characinfo");
	m_IconInvenBtn.SetFileName("top/btn_menu_bag");
	m_IconSkillBtn.SetFileName("top/btn_menu_skill");
	m_IconPetBtn.SetFileName("top/btn_menu_pet");
	m_IconMapBtn.SetFileName("top/btn_menu_map");
	m_IconHelpBtn.SetFileName("top/btn_menu_help");
	m_IconPartyBtn.SetFileName("top/btn_menu_party");
	m_IconHelperBtn.SetFileName("top/btn_menu_guardian");
	m_IconGuildBtn.SetFileName("top/btn_menu_guild");
	m_IconExitBtn.SetFileName("top/btn_menu_exit");

	m_IconOptionBtn.SetFileName("top/option");				// 사용하나?



}

void CControlTop::LoadRes()
{
	if( !g_pDisplay )
		return;

	if( CommonConfig::Instance()->GetLagrush() == true )
	{
		if( !g_MatrixSvr )
		{
			m_IconAutoBtn.LoadRes();
			m_iConAutoOnBtn.LoadRes();
		}
	}

	m_IconInvenBtn.LoadRes();
	m_IconScoreBtn.LoadRes();
	m_IconSkillBtn.LoadRes();
	m_IconPetBtn.LoadRes();
	m_IconMapBtn.LoadRes();
	m_IconHelpBtn.LoadRes();
	m_IconPartyBtn.LoadRes();
	m_IconHelperBtn.LoadRes();
	m_IconGuildBtn.LoadRes();
	m_IconExitBtn.LoadRes();

	m_IconOptionBtn.LoadRes(); // ???

	if( g_bUseCashShop )
		m_IconCashShopBtn.LoadRes();




	// Set Position
	int num_icon = 0;
	int ox = 30;

	m_IconScoreBtn.SetPosition( ox*num_icon++ , 0 );
	m_IconInvenBtn.SetPosition( ox*num_icon++ , 0 );
	m_IconSkillBtn.SetPosition( ox*num_icon++ , 0 );

	if( g_MatrixSvr == 0 )
		m_IconPetBtn.SetPosition( ox*num_icon++ , 0 );
	m_IconMapBtn.SetPosition( ox*num_icon++ , 0 );
	if( g_MatrixSvr == 0 )
		m_IconHelpBtn.SetPosition( ox*num_icon++ , 0 );
	m_IconPartyBtn.SetPosition( ox*num_icon++ , 0 );
	if( g_MatrixSvr == 0 )
		m_IconHelperBtn.SetPosition( ox*num_icon++ , 0 );
	if( g_MatrixSvr == 0 )
		m_IconGuildBtn.SetPosition( ox*num_icon++ , 0 );

	if( g_bUseCashShop )
		m_IconCashShopBtn.SetPosition( ox*num_icon++ , 0 );

	if( CommonConfig::Instance()->GetLagrush() == true )
	{
		if( g_MatrixSvr == 0 )
		{
			m_iConAutoOnBtn.SetPosition( ox*num_icon++ , 0 );
			m_IconAutoBtn.SetPosition( ox*num_icon++ , 0 );
		}
	}

	m_IconExitBtn.SetPosition( ox*num_icon++ , 0 );

	SetRect(&m_rcThis , 0 , 0 , ox*num_icon , 27 );


	m_pEtcInfo->Init(14);

	if( m_pEtcInfo && g_pNk2DFrame && pCMyApp )
		m_pEtcInfo->LoadRes(g_pNk2DFrame->GetClientWidth(), g_pNk2DFrame->GetClientHeight() );
}

void CControlTop::DeleteRes()
{
	if( g_bUseCashShop )
		m_IconCashShopBtn.DeleteRes(); ///유료상점

	if( CommonConfig::Instance()->GetLagrush() == true )
	{
		if( !g_MatrixSvr )
		{
			m_IconAutoBtn.DeleteRes();
			m_iConAutoOnBtn.DeleteRes();
		}
	}	

	m_IconInvenBtn.DeleteRes();
	m_IconScoreBtn.DeleteRes();
	m_IconSkillBtn.DeleteRes();
	m_IconPetBtn.DeleteRes();
	m_IconMapBtn.DeleteRes();
	m_IconHelpBtn.DeleteRes();
	m_IconOptionBtn.DeleteRes();
	m_IconPartyBtn.DeleteRes();
	m_IconExitBtn.DeleteRes();
	m_IconHelperBtn.DeleteRes();
	m_IconGuildBtn.DeleteRes();


	SAFE_DELETE_RES(m_pEtcInfo);
}

void CControlTop::Draw()
{
// 	if( !_GetVisible() )
// 		return;

	if( g_bUseCashShop )
		m_IconCashShopBtn.DrawToBlt(g_pNk2DFrame->IsCashShopVisible()); ///유료상점

	m_IconInvenBtn.DrawToBlt(g_pNk2DFrame->IsInvenVisible());
	m_IconScoreBtn.DrawToBlt(g_pNk2DFrame->IsInterfaceVisible(SCORE));
	m_IconSkillBtn.DrawToBlt(g_pNk2DFrame->IsSkillVisible());
	if( g_MatrixSvr == 0 )
		m_IconPetBtn.DrawToBlt(g_pNk2DFrame->IsPetStatusVisible());
	m_IconMapBtn.DrawToBlt(g_pNk2DFrame->IsInterfaceVisible(MAP));
	if( g_MatrixSvr == 0 )
		m_IconHelpBtn.DrawToBlt(g_pNk2DFrame->IsHelpVisible());
	m_IconPartyBtn.DrawToBlt(g_pNk2DFrame->IsPartyVisible());
	if( g_MatrixSvr == 0 )
		m_IconHelperBtn.DrawToBlt(g_pNk2DFrame->IsHelperVisible());
	if( g_MatrixSvr == 0 )
		m_IconGuildBtn.DrawToBlt(g_pNk2DFrame->IsControlGuild());

	if( CommonConfig::Instance()->GetLagrush() == true )
	{
		if( g_MatrixSvr == 0 )
		{
			m_IconAutoBtn.DrawToBlt( FALSE );
			m_iConAutoOnBtn.DrawToBlt( FALSE );
		}
	}

	m_IconExitBtn.DrawToBlt(FALSE);

	if( g_pNk2DFrame && g_pNk2DFrame->GetControlBottom() && g_pNk2DFrame->GetControlBottom()->m_pEtcInfo )
	{
		CMultiText multiText;
		POINT point;
		GetCursorPos(&point);
		ScreenToClient(g_pDisplay->GetHWnd(), &point);

		if( m_IconScoreBtn.IsInside(point.x, point.y ) )
		{
			if (g_pNk2DFrame->GetControlBottom()->m_pEtcInfo->GetIndex() != -108)
			{
				// IDS_BOTTOM_INFO_CHARACTER : 캐릭터 정보
				multiText.AddString((char*)G_STRING(IDS_BOTTOM_INFO_CHARACTER), _FCOLOR_YELLOW);
				g_pNk2DFrame->GetControlBottom()->m_pEtcInfo->PutString(&multiText);
				g_pNk2DFrame->GetControlBottom()->m_pEtcInfo->SetIndex(-108);
			}
			g_pNk2DFrame->GetControlBottom()->m_pEtcInfo->Draw(point.x, point.y);
		}
		else if( m_IconInvenBtn.IsInside(point.x, point.y ) )
		{
			if (g_pNk2DFrame->GetControlBottom()->m_pEtcInfo->GetIndex() != -109)
			{
				// IDS_BOTTOM_HAVINGS : 소지품
				multiText.AddString((char*)G_STRING(IDS_BOTTOM_HAVINGS), _FCOLOR_YELLOW);
				g_pNk2DFrame->GetControlBottom()->m_pEtcInfo->PutString(&multiText);
				g_pNk2DFrame->GetControlBottom()->m_pEtcInfo->SetIndex(-109);
			}
			g_pNk2DFrame->GetControlBottom()->m_pEtcInfo->Draw(point.x, point.y);
		}
		else if( m_IconSkillBtn.IsInside(point.x, point.y ) )
		{
			if (g_pNk2DFrame->GetControlBottom()->m_pEtcInfo->GetIndex() != -110)
			{
				// IDS_BOTTOM_SKILL : 스킬
				multiText.AddString((char*)G_STRING(IDS_BOTTOM_SKILL), _FCOLOR_YELLOW);
				g_pNk2DFrame->GetControlBottom()->m_pEtcInfo->PutString(&multiText);
				g_pNk2DFrame->GetControlBottom()->m_pEtcInfo->SetIndex(-110);
			}
			g_pNk2DFrame->GetControlBottom()->m_pEtcInfo->Draw(point.x, point.y);
		}

		else if( g_MatrixSvr == 0 && m_IconPetBtn.IsInside(point.x, point.y ) )
		{
			if (g_pNk2DFrame->GetControlBottom()->m_pEtcInfo->GetIndex() != -111)
			{
				// IDS_BOTTOM_PET : 애완동물
				multiText.AddString((char*)G_STRING(IDS_BOTTOM_PET), _FCOLOR_YELLOW);
				g_pNk2DFrame->GetControlBottom()->m_pEtcInfo->PutString(&multiText);
				g_pNk2DFrame->GetControlBottom()->m_pEtcInfo->SetIndex(-111);
			}
			g_pNk2DFrame->GetControlBottom()->m_pEtcInfo->Draw(point.x, point.y);
		}
		else if( m_IconMapBtn.IsInside(point.x, point.y ) )
		{
			if (g_pNk2DFrame->GetControlBottom()->m_pEtcInfo->GetIndex() != -112)
			{
				// IDS_BOTTOM_MAP : 지도
				multiText.AddString((char*)G_STRING(IDS_BOTTOM_MAP), _FCOLOR_YELLOW);
				g_pNk2DFrame->GetControlBottom()->m_pEtcInfo->PutString(&multiText);
				g_pNk2DFrame->GetControlBottom()->m_pEtcInfo->SetIndex(-112);
			}
			g_pNk2DFrame->GetControlBottom()->m_pEtcInfo->Draw(point.x, point.y);
		}
		else if( g_MatrixSvr == 0 && m_IconHelpBtn.IsInside(point.x, point.y ) )
		{
			if (g_pNk2DFrame->GetControlBottom()->m_pEtcInfo->GetIndex() != -113)
			{
				// IDS_BOTTOM_HELP : 도움말
				multiText.AddString((char*)G_STRING(IDS_BOTTOM_HELP), _FCOLOR_YELLOW);
				g_pNk2DFrame->GetControlBottom()->m_pEtcInfo->PutString(&multiText);
				g_pNk2DFrame->GetControlBottom()->m_pEtcInfo->SetIndex(-113);
			}
			g_pNk2DFrame->GetControlBottom()->m_pEtcInfo->Draw(point.x, point.y);
		}
		else if( m_IconPartyBtn.IsInside(point.x, point.y ) )
		{
			if (g_pNk2DFrame->GetControlBottom()->m_pEtcInfo->GetIndex() != -114)
			{
				// IDS_BOTTOM_INFO_PARTY : 파티
				multiText.AddString((char *)G_STRING(IDS_BOTTOM_INFO_PARTY), _FCOLOR_YELLOW);
				g_pNk2DFrame->GetControlBottom()->m_pEtcInfo->PutString(&multiText);
				g_pNk2DFrame->GetControlBottom()->m_pEtcInfo->SetIndex(-114);
			}
			g_pNk2DFrame->GetControlBottom()->m_pEtcInfo->Draw(point.x, point.y);
		}
		else if( g_MatrixSvr == 0 && m_IconHelperBtn.IsInside(point.x, point.y ) )
		{
			if (g_pNk2DFrame->GetControlBottom()->m_pEtcInfo->GetIndex() != -122)
			{
				// IDS_BOTTOM_HELPER : 후견인
				multiText.AddString( (char*)G_STRING(IDS_BOTTOM_HELPER), _FCOLOR_YELLOW);
				g_pNk2DFrame->GetControlBottom()->m_pEtcInfo->PutString(&multiText);
				g_pNk2DFrame->GetControlBottom()->m_pEtcInfo->SetIndex(-122);
			}
			g_pNk2DFrame->GetControlBottom()->m_pEtcInfo->Draw(point.x, point.y);
		}
		else if(g_MatrixSvr == 0 && m_IconGuildBtn.IsInside(point.x, point.y) )
		{
			if (g_pNk2DFrame->GetControlBottom()->m_pEtcInfo->GetIndex() != -117)
			{
				// IDS_BOTTOM_GUILD : 길드
				multiText.AddString((char * )G_STRING(IDS_BOTTOM_GUILD), _FCOLOR_YELLOW);
				g_pNk2DFrame->GetControlBottom()->m_pEtcInfo->PutString(&multiText);
				g_pNk2DFrame->GetControlBottom()->m_pEtcInfo->SetIndex(-117);
			}
			g_pNk2DFrame->GetControlBottom()->m_pEtcInfo->Draw(point.x, point.y);
		}
		else if( g_MatrixSvr == 0 && m_iConAutoOnBtn.IsInside(point.x, point.y) )
		{
			if (g_pNk2DFrame->GetControlBottom()->m_pEtcInfo->GetIndex() != -118)
			{
				// 라그러쉬 실행
				//multiText.AddString((char * )G_STRING(IDS_BOTTOM_GUILD), _FCOLOR_YELLOW);
				multiText.AddString("Laugh Rush", _FCOLOR_YELLOW);
				g_pNk2DFrame->GetControlBottom()->m_pEtcInfo->PutString(&multiText);
				g_pNk2DFrame->GetControlBottom()->m_pEtcInfo->SetIndex(-118);
			}
			g_pNk2DFrame->GetControlBottom()->m_pEtcInfo->Draw(point.x, point.y);
		}
		else if(g_MatrixSvr == 0 && m_IconAutoBtn.IsInside(point.x, point.y) )
		{
			if (g_pNk2DFrame->GetControlBottom()->m_pEtcInfo->GetIndex() != -119)
			{
				// 라그러쉬 설정
				//multiText.AddString((char * )G_STRING(IDS_BOTTOM_GUILD), _FCOLOR_YELLOW);
				multiText.AddString("Laugh Rush Settings", _FCOLOR_YELLOW);
				g_pNk2DFrame->GetControlBottom()->m_pEtcInfo->PutString(&multiText);
				g_pNk2DFrame->GetControlBottom()->m_pEtcInfo->SetIndex(-119);
			}
			g_pNk2DFrame->GetControlBottom()->m_pEtcInfo->Draw(point.x, point.y);
		}
		else if( m_IconExitBtn.IsInside(point.x, point.y ) )
		{
			if (g_pNk2DFrame->GetControlBottom()->m_pEtcInfo->GetIndex() != -125)
			{
				// IDS_BOTTOM_EXIT : 종료
				multiText.AddString((char * )G_STRING(IDS_BOTTOM_EXIT), _FCOLOR_YELLOW);
				g_pNk2DFrame->GetControlBottom()->m_pEtcInfo->PutString(&multiText);
				g_pNk2DFrame->GetControlBottom()->m_pEtcInfo->SetIndex(-125);
			}
			g_pNk2DFrame->GetControlBottom()->m_pEtcInfo->Draw(point.x, point.y);
		}
	}
}

BOOL CControlTop::IsInside(int x, int y)
{
	POINT pt = {x,y};
	
	BOOL bResult = PtInRect(&m_rcThis , pt);
	BOOL bRush = FALSE;
	if( CommonConfig::Instance()->GetLagrush() == true )
	{
		if( m_iConAutoOnBtn.IsInside(x,y) )
		{
			bRush = TRUE;
		}
	}
	
	return (bResult || bRush);
}

LRESULT CControlTop::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static char commOutBuf[512];

	int x = LOWORD(lParam);
	int y = HIWORD(lParam);

	/* ocarina 130426
	if(IsInside(x, y))
		pCMyApp->m_pMouse->SetMouseType(M_NORMAL);/**/

	switch ( msg )
	{
	case WM_LBUTTONUP:
		{
			if (pCMyApp->IsSoccerZone()) // mungmae-06/05/29 : 메트릭스 월드컵 경기장 - 인터페이스 비활성화
				return 0;

			if (!IsInside(x, y) || !g_pNk2DFrame )
				return 0;

			if( pCMyApp->m_pUIMgr )
				if( pCMyApp->m_pUIMgr->m_bIsGuildHunt || pCMyApp->m_pUIMgr->m_bIsReservationWindow )
					return 0;

			if( g_bUseCashShop )
			{
				m_IconCashShopBtn.MsgProc(hWnd, msg, wParam, lParam);
				if (m_IconCashShopBtn.GetState() == BTN_ACTION)
				{
					g_pNk2DFrame->ToggleCashShopWindow( 0 );
				}
			}

			if( g_MatrixSvr == 0 )
			{
				m_IconAutoBtn.MsgProc( hWnd, msg, wParam, lParam );
				if( m_IconAutoBtn.GetState() == BTN_ACTION && !g_MatrixSvr )
					ShellExecute( hWnd, "open", "Laghaim_Lagrush.exe", g_pRoh->m_pstrName, "", SW_SHOWNORMAL );
				m_iConAutoOnBtn.MsgProc( hWnd, msg, wParam, lParam );
			}

			if( g_MatrixSvr == 0 && m_iConAutoOnBtn.GetState() == BTN_ACTION )
			{

				if( pCMyApp->m_pUIMgr->GetLagrushPayment() == FALSE )
					return 1;


				if( pCMyApp->m_pUIMgr->m_AutoPlay->IsRun() )
				{
					m_iConAutoOnBtn.SetState( BTN_NORMAL );
					pCMyApp->m_pUIMgr->m_AutoPlay->StopProc();
				}
				else
				{
					if( g_pRoh->GetMotionState() != CHA_STAND )
					{
						g_pNk2DFrame->InsertPopup( "Can not be used while attacking or moving." , TYPE_NOR_OK , POPUP_OK );
						return 1;
					}

					g_pNk2DFrame->InsertPopup( "Do you want to run Rag Rush?" , TYPE_NOR_OKCANCLE , POPUP_LAGRUSH_START );
					return 1;
				}
			}

			m_IconInvenBtn.MsgProc(hWnd, msg, wParam, lParam);
			if (m_IconInvenBtn.GetState() == BTN_ACTION)
			{
				if(g_pNk2DFrame->IsWareHouseVisible())
				{
					g_pNk2DFrame->ShowWareHouseWindow(false);
					return 1;
				}
				g_pNk2DFrame->ToggleInvenWindow();
			}

			m_IconScoreBtn.MsgProc(hWnd, msg, wParam, lParam);

			if (m_IconScoreBtn.GetState() == BTN_ACTION)
			{
				g_pNk2DFrame->ToggleInterfaceWindow(SCORE);
			}

			m_IconSkillBtn.MsgProc(hWnd, msg, wParam, lParam);

			if (m_IconSkillBtn.GetState() == BTN_ACTION)
				g_pNk2DFrame->ToggleSkillWindow();

			if( g_MatrixSvr == 0 )
			{
				m_IconPetBtn.MsgProc(hWnd, msg, wParam, lParam);

				if (m_IconPetBtn.GetState() == BTN_ACTION)
				{
					switch (g_MyPetStatus.mode)
					{
					case MYPET_MODE_NONE:
						//-- IDS_TOP_PET_NOTEXIST : 당신은 애완동물을 기르고 있지 않습니다.
						g_pNk2DFrame->InsertPopup((char *)G_STRING(IDS_TOP_PET_NOTEXIST),TYPE_NOR_OK, 1);
						break;
					case MYPET_MODE_EGG:
						//-- IDS_TOP_PET_NOTYET : 당신의 애완동물은 아직 부화되지 않습니다.
						g_pNk2DFrame->InsertPopup((char *)G_STRING(IDS_TOP_PET_NOTYET),TYPE_NOR_OK, 1);
						break;
					case MYPET_MODE_KEEP:
						//-- IDS_TOP_PET_KEEPING : 당신의 애완동물은 현재 보관상태입니다.
						g_pNk2DFrame->InsertPopup((char *)G_STRING(IDS_TOP_PET_KEEPING),TYPE_NOR_OK, 1);
						break;
					case MYPET_MODE_HOLD:
						g_pNk2DFrame->TogglePetStatusWindow();
						break;
					default:
						g_pNk2DFrame->TogglePetStatusWindow();
						break;
					}

				}
			}
			m_IconMapBtn.MsgProc(hWnd, msg, wParam, lParam);
			if (m_IconMapBtn.GetState() == BTN_ACTION)
				g_pNk2DFrame->ToggleInterfaceWindow(MAP);

			if( g_MatrixSvr == 0 )
			{
				m_IconHelpBtn.MsgProc(hWnd, msg, wParam, lParam);
				if (m_IconHelpBtn.GetState() == BTN_ACTION)
					g_pNk2DFrame->ToggleHelpWindow();
			}

			m_IconPartyBtn.MsgProc(hWnd, msg, wParam, lParam);
			if (m_IconPartyBtn.GetState() == BTN_ACTION)
				g_pNk2DFrame->TogglePartyWindow();

			//  [4/24/2009 ppmmjj83] 일본요청에 의해 후견인을 막아둔다
			if( g_dwClientCountry != CTRY_JPN )
			{
				if( g_MatrixSvr == 0 )
				{
					m_IconHelperBtn.MsgProc(hWnd, msg, wParam, lParam);		//후견인
					if (m_IconHelperBtn.GetState() == BTN_ACTION
							&& g_MatrixSvr == 0)
						g_pNk2DFrame->ToggleHelperWindow();
				}
			}

			m_IconExitBtn.MsgProc(hWnd, msg, wParam, lParam);
			if (m_IconExitBtn.GetState() == BTN_ACTION)
				pCMyApp->m_MsgPopUp.PutString((char*)G_STRING(IDS_MAIN_END_GAME));

			if( g_MatrixSvr == 0 )
			{
				m_IconGuildBtn.MsgProc(hWnd, msg, wParam, lParam);
				if(m_IconGuildBtn.GetState() == BTN_ACTION && g_MatrixSvr == 0)
					g_pNk2DFrame->ToggleControlGuildWindow();
			}

			return 1;
		}

	case WM_MOUSEMOVE:
	case WM_LBUTTONDOWN:
		{
			if( pCMyApp->IsSoccerZone() )
				return 0;

			if( pCMyApp->m_pUIMgr )
			{
				if( pCMyApp->m_pUIMgr->m_bIsGuildHunt || pCMyApp->m_pUIMgr->m_bIsReservationWindow )
					return 0;
			}

			m_IconInvenBtn.MsgProc(hWnd, msg, wParam, lParam);
			m_IconScoreBtn.MsgProc(hWnd, msg, wParam, lParam);
			m_IconSkillBtn.MsgProc(hWnd, msg, wParam, lParam);
			m_IconMapBtn.MsgProc(hWnd, msg, wParam, lParam);
			m_IconPartyBtn.MsgProc(hWnd, msg, wParam, lParam);
			m_IconExitBtn.MsgProc(hWnd, msg, wParam, lParam);

			if( g_MatrixSvr == 0 )
			{
				m_IconPetBtn.MsgProc(hWnd, msg, wParam, lParam);
				m_IconHelpBtn.MsgProc(hWnd, msg, wParam, lParam);
				m_IconHelperBtn.MsgProc(hWnd, msg, wParam, lParam);		//후견인
				m_IconGuildBtn.MsgProc(hWnd, msg, wParam, lParam);

				if( g_bUseCashShop )
					m_IconCashShopBtn.MsgProc(hWnd, msg, wParam, lParam);

				m_IconAutoBtn.MsgProc( hWnd, msg, wParam, lParam );
				m_iConAutoOnBtn.MsgProc( hWnd, msg, wParam, lParam );
				if(  m_iConAutoOnBtn.GetState() != BTN_ON )
				{
					if( pCMyApp->m_pUIMgr->m_bAutoHunt )
						m_iConAutoOnBtn.SetState(BTN_DISABLE);
					else
						m_iConAutoOnBtn.SetState(BTN_NORMAL);
				}
			}

			if ( !IsInside(x, y) )
				return 0;
			else
				return 1;
		}
		break;
	}
	return 0;
}
