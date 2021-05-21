#include "stdafx.h"
#include ".\lhui_gate.h"


#include "main.h"
#include "UIMgr.h"
#include "Nk2DFrame.h"
#include "g_stringmanager.h"
#include "Country.h"
#include "Mouse.h"
#include "tcpipcon.h"
#include "ddutil.h"
#include "NkCharacter.h"

#include "BasicStatic.h"
#include "BasicButton.h"
#include "ListBox.h"
#include "Land.h"





extern DWORD				g_dwClientCountry;			// 0: Korean 1: Japanese  2: English
extern BOOL					g_bActivePackage;
extern BOOL					g_bGlobalMatrix; ///국가대항전 용 클라이언트 인지 여부.



LHUI_Gate::LHUI_Gate(void)
	:	m_background(NULL)
	,	m_btn_close(NULL)
	,	m_lst_zone(NULL)
{
	ResetAll();
	Init();
}

LHUI_Gate::~LHUI_Gate(void)
{
	DeleteRes();
}

void LHUI_Gate::Init()
{
	LoadRes();
}

void LHUI_Gate::LoadRes()
{
	m_background = new CBasicStatic();
	m_btn_close = new CBasicButton();
	m_lst_zone = new CListBox(" ", 0, 0, 0, 0);

	m_background->Init( "gate/gate_back" );
	m_btn_close->Init("common/btn_close_01");

	if( m_lst_zone )
	{
		m_lst_zone->SetImage("common/scroll_dn_t01", "common/scroll_up_t01", "interface/common/scroll_thumb_01.bmp");
		m_lst_zone->SetSelectedLine(-1);
		m_lst_zone->SetTextColor(RGB(255, 255, 0));
//		m_lst_zone->AddString(G_STRING(IDS_GATE_WORLD_MOVE));
	}

	int bgx = (g_pNk2DFrame->GetClientWidth() - m_background->m_w)>>1;
	int bgy = (g_pNk2DFrame->GetClientHeight() - m_background->m_h)>>1;

	m_background->SetPos( bgx , bgy );
	m_btn_close->SetPosition( bgx+209,bgy+15 );
	m_lst_zone->SetPosDim(bgx+45, bgy+86, 191, 226);
	m_lst_zone->LoadRes();


	_SetVisible(true);
	_SetDisable(false);
}

void LHUI_Gate::DeleteRes()
{
	SAFE_DELETE(m_background);
	SAFE_DELETE(m_btn_close);
	SAFE_DELETE(m_lst_zone);
}

void LHUI_Gate::Draw()
{
	if( !_GetVisible() )
		return;

	m_background->Draw();
	m_btn_close->Draw();
	m_lst_zone->Draw();

	if (pCMyApp->m_pMouse)
		pCMyApp->m_pMouse->SetMouseType(M_NORMAL);
}

LRESULT LHUI_Gate::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if( !_GetVisible() || _GetDisalbe() )
		return 0;

	if( g_pNk2DFrame->Check_All_MsgPopUp() )
		return 1;

	// 마우스 메세지일경우만
	m_btn_close->MsgProc( hWnd , msg , wParam , lParam );

	switch( msg )
	{
	case WM_MOUSEMOVE:
	case WM_LBUTTONUP:
		{
			POINT pt = { LOWORD(lParam) , HIWORD(lParam) };
			if( !m_background->IsIn( pt.x , pt.y) )
				return 0;

			m_lst_zone->GetMessage(hWnd, msg, wParam, lParam);

			if( m_btn_close->GetState() == BTN_ACTION )
			{
				_SetVisible(false);
				_SetDisable(true);
				return 2;
			}
		}
		break;
	case WM_LBUTTONDOWN:
		{
			POINT pt = { LOWORD(lParam) , HIWORD(lParam) };
			if( !m_background->IsIn( pt.x , pt.y) )
				return 0;

			if( m_btn_close->GetState() == BTN_DOWN )
				return 2;

			int sel = -1;
			if (m_lst_zone && (sel = m_lst_zone->GetMessage(hWnd, msg, wParam, lParam )) >= 0)
			{
				int cur_sel = m_lst_zone->GetSelectedLine();

				if( m_slot_index == -1 )
				{
					if( false == CheckRestrict(m_zone_num[cur_sel]) )
					{
						g_pNk2DFrame->InsertPopup( "Can not move because conditions are not correct." ,TYPE_NOR_OK, 1);
						return 2;
					}

					pCMyApp->m_pUIMgr->PrepareChangeWorld( TRUE, m_zone_num[cur_sel], 0 );
					return 2;
				}
				else
				{
					char param[32];
					sprintf( param, "%d" , m_zone_num[cur_sel] );
					if( pCMyApp->m_pUIMgr->UseWarp( m_slot_index, param ) )
					{
						return 2;
					}
				}
			}
		}
		break;
	}
	return 0;
}

void LHUI_Gate::ResetAll()
{
	_SetVisible(false);
	_SetDisable(true);

	m_slot_index = -1;
	m_num_zonelist = 0;
	for( int i=0 ; i<ZONE_MAX ; i++ )
	{
		m_zone_num[i] = -1;
	}

	DeleteRes();
}

void LHUI_Gate::SetZoneList(int value , bool direct)
{
	if( direct )
	{
		if( !AddList(value) )
			return;
	}
	else
	{
		if( value )
		{
			ResetAll();
			Init();

			for( int i=0 ; i<ZONE_MAX ; i++ )
			{
				if( (1<<i)&value )
				{
					if( !AddList(i) )
					{
						return;
					}
				}
			}
		}
	}

	m_lst_zone->SetPosDim(m_background->m_x+45, m_background->m_y+86, 191, 226);
	m_lst_zone->LoadRes();
}

bool LHUI_Gate::AddList(int zone_no)
{
	const char * zone_name = GetZoneName(zone_no);
	if( !zone_name )
		return false;

	m_lst_zone->AddString( zone_name );
	m_zone_num[m_num_zonelist++] = zone_no;

	return true;
}


const char * LHUI_Gate::GetZoneName(int zone_no)
{
	switch( zone_no )
	{
	case 0:
		return G_STRING(IDS_GATE_LOST_REALM);
		break;
	case 1:
		return G_STRING(IDS_GATE_DEKALEN);
		break;
	case 2:
		return G_STRING(IDS_GATE_DEKADUNE);
		break;
	case 3:
		return G_STRING(IDS_GATE_SHAILON);
		break;
	case 4:
		return G_STRING(IDS_GATE_LAGRAMIA);
		break;
	case 5:
		return G_STRING(IDS_GATE_SIRUSTE);
		break;
	case 6:
		return G_STRING(IDS_MAP_PHAROS);
		break;
	case 7:
		return G_STRING(IDS_GATE_WHITEHORN);
		break;
	case 8:
		return G_STRING(IDS_DUNGEON4);
		break;
	case 9:
		return G_STRING(IDS_GATE_DMITRON);
		break;
	case 13:
		return G_STRING(IDS_WORLD_13);
		break;
	case 14:
		return G_STRING(IDS_WORLD_14);
		break;
	case 15:
		return G_STRING(IDS_WORLD_15);
		break;
	case 16:
		return G_STRING(IDS_WORLD_16);
		break;
	case 17:
		return G_STRING(IDS_WORLD_17);
		break;
	case 18:
		return G_STRING(IDS_WORLD_18);

	case 20: //
		return G_STRING(IDS_LHSTRING1829);
	case 21:
		return G_STRING(IDS_LHSTRING1830);
	case 22:
		return G_STRING(IDS_LHSTRING1892);
	case 23:
		return G_STRING(IDS_LHSTRING1896);
	case 24:
		return G_STRING(IDS_LHSTRING1896);    
		
	case 25:
		return G_STRING(IDS_LHSTRING4555);

	case 26:
		return G_STRING(IDS_LHSTRING4556);

	case 27:
		return G_STRING(IDS_LHSTRING4557);
	case 28:
		return G_STRING(IDS_LHSTRING4558);
	case 29:
		return G_STRING(IDS_LHSTRING4662);
	case 30:
		return G_STRING(IDS_LHSTRING4663);

	default:
		break;
	}

	return NULL;
}

void LHUI_Gate::ExceptionItem()
{
	ResetAll();
	Init();

	AddList(0);
	AddList(1);
	AddList(2);
	AddList(3);
	AddList(4);
	AddList(5);
	AddList(6);
	AddList(7);

	if(g_pRoh->m_Level > 249)
		AddList(8);


	m_lst_zone->SetPosDim(m_background->m_x+45, m_background->m_y+86, 191, 226);
	m_lst_zone->LoadRes();
}


bool LHUI_Gate::CheckRestrict(int zone_index)
{
	switch( zone_index )
	{
	case WORLD_FIELD:
	case WORLD_DUNGEON1:
	case WORLD_DUNGEON2:
	case WORLD_GUILD:
	case WORLD_START:
	case WORLD_DUNGEON_S:
	case WORLD_SKY:
	case WORLD_WHITEHORN:
	case WORLD_DUNGEON4:
	case WORLD_DMITRON:
	case WORLD_MATRIX:
	case WORLD_GUILD_WAR:
	case WORLD_DMITRON_BATTLE:
	case WORLD_BOSSRAID_BULKAN:
	case WORLD_BOSSRAID_KAILIPTON:
	case WORLD_BOSSRAID_AIDIA:
	case WORLD_BOSSRAID_HUMAN:
	case WORLD_BOSSRAID_HYBRIDER:
	case WORLD_BOSSRAID_FINALBOSS:
	case WORLD_DISPOSAL:
	case WORLD_VIDBLINE:
	case WORLD_FORLORN_S2:
	case WORLD_FORLORN_S3:
	case WORLD_FORLORN_S4:
	case WORLD_HIGHTLAND:
	case WORLD_TRITIAN:
	case WORLD_SPACE:
	case WORLD_ROYALE:
	case WORLD_DUN910:
	case WORLD_MAP930:
		break;
	default:
		return true;
	}

	return true;
}