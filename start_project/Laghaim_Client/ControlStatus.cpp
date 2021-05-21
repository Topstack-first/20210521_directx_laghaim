#include "stdafx.h"
// ControlStatus.cpp: implementation of the CControlStatus class.
//
//////////////////////////////////////////////////////////////////////

#include "ControlStatus.h"

#include "NkCharacter.h"
#include "Nk2DFrame.h"
#include "ddutil.h"
#include "ControlBottom.h"






char* facename[] = { "status/bm_face" , "status/bw_face"
					 ,"status/km_face" , "status/kw_face"
					 ,"status/am_face" , "status/aw_face"
					 ,"status/hm_face" , "status/hw_face"
					 ,"status/fm_face" , "status/fw_face"
					 ,"status/pm_face" , "status/pw_face"
				   };



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CControlStatus::CControlStatus()
	:	m_face_load(false)
{
}

CControlStatus::~CControlStatus()
{
	m_face_load = false;
}

void CControlStatus::Init()
{
}

void CControlStatus::LoadRes()
{
	if( !g_pDisplay )
		return;

	m_background.Init( "status/status" );

	// top 27
	m_background.SetPos( 0 , 30 );
	m_hp.Init( "status/Top_HP" );
	m_mp.Init( "status/Top_Mana" );
	m_ep.Init( "status/Top_EL" );
	m_sp.Init( "status/Top_ST" );

	int bgx = m_background.m_x;
	int bgy = m_background.m_y;

	m_face.SetPos( bgx + 9 , bgy + 9 );
	m_hp.SetPos( bgx + 77 , bgy + 17 );
	m_mp.SetPos( bgx + 77 , bgy + 30 );
	m_ep.SetPos( bgx + 77 , bgy + 30 );
	m_sp.SetPos( bgx + 77 , bgy + 30 );

	
	//m_textout[0].Init( 9 , RGB(231, 103, 102) , bgx + 94 , bgy + 48 );
	//m_textout[1].Init( 9 , RGB(231, 103, 102) , bgx + 175 , bgy + 17 );
	//m_textout[2].Init( 9 , RGB(231, 103, 102) , bgx + 175 , bgy + 30 );
	

	//  [8/13/2009 ppmmjj83] 캐릭터 상태 텍스트 좌표 수정
	m_textout[0].Init( 12 , RGB(255, 255, 0) , bgx + 94 , bgy + 48 );
	m_textout[1].Init( 12 , RGB(255, 255, 0) , bgx + 175 , bgy + 17 );
	m_textout[2].Init( 12 , RGB(255, 255, 0) , bgx + 175 , bgy + 30 );
}

void CControlStatus::LoadRes_Char()
{
	if( m_face_load || !g_pRoh )
		return;

	int sid = 0;
	switch( g_pRoh->m_Race )
	{
	case RACE_BULKAN: // 불칸
		break;
	case RACE_KAILIPTON: // 카이립톤
		sid = 2;
		break;
	case RACE_AIDIA: // 에이디아
		sid = 4;
		break;
	case RACE_HUMAN: // 휴먼
		sid = 6;
		break;
	case RACE_FREAK: // 하이브리더
		sid = 8;
		break;
	case RACE_PEROM:
		sid = 10;
		break;
	}

	if( g_pRoh->m_Sex == 1 )
		sid++;

	m_face.Init( facename[sid] );
	m_textout[0].SetString( g_pRoh->m_pstrName );
	m_textout[1].SetString( "100%" );
	m_textout[2].SetString( "100%" );

	m_face_load = true;
}

void CControlStatus::SetName()
{
	m_textout[0].SetString( g_pRoh->m_pstrName );
}

void CControlStatus::DeleteRes()
{
	m_background.DeleteRes();
	m_face.DeleteRes();
	m_hp.DeleteRes();
	m_mp.DeleteRes();
	m_ep.DeleteRes();
	m_sp.DeleteRes();

	m_face_load = false;
}

void CControlStatus::Draw()
{
	if( !g_pDisplay || !g_pRoh )
		return;

	LoadRes_Char();

	m_background.Draw();
	m_face.Draw();
	m_textout[0].Draw();

	Draw_Bar( &m_hp , (int)g_pRoh->m_Vital, (int)g_pRoh->m_MaxVital );
	Draw_Percent( &m_textout[1] , (int)g_pRoh->m_Vital, (int)g_pRoh->m_MaxVital );

	switch( g_pNk2DFrame->GetControlBottom()->m_rcurrent_view )
	{
	case GageView_Type__Stamina:
		Draw_Bar( &m_sp , (int)g_pRoh->m_Stamina, (int)g_pRoh->m_MaxStamina );
		Draw_Percent( &m_textout[2] , (int)g_pRoh->m_Stamina, (int)g_pRoh->m_MaxStamina );
		break;
	case GageView_Type__Mana:
		Draw_Bar( &m_mp , (int)g_pRoh->m_Mana, (int)g_pRoh->m_MaxMana );
		Draw_Percent( &m_textout[2] , (int)g_pRoh->m_Mana, (int)g_pRoh->m_MaxMana );
		break;
	case GageView_Type__EPower:
		Draw_Bar( &m_ep , (int)g_pRoh->m_Epower, (int)g_pRoh->m_MaxEpower);
		Draw_Percent( &m_textout[2] , (int)g_pRoh->m_Epower, (int)g_pRoh->m_MaxEpower );
		break;
	}

	m_textout[1].Draw();
	m_textout[2].Draw();
}

void CControlStatus::Draw_Bar(CBasicStatic* ui , int base_value, int max_value)
{
	double ratio = (double)base_value / max_value;
	if( ratio > 1.0f )
	{
		ratio = 1.0f;
	}
	else if( ratio < 0.0f )
	{
		ratio = 0.0f;
	}

	ui->Draw( (float)ratio , 3 );
}

void CControlStatus::Draw_Percent( CTextOutBox* textout , int base_value, int max_value)
{
	double ratio = (double)base_value / max_value;
	if( ratio > 1.0f )
	{
		ratio = 1.0f;
	}
	else if( ratio < 0.0f )
	{
		ratio = 0.0f;
	}

	char buf[256] = {0};
	sprintf( buf , "%d%%" , (int)(ratio * 100) );
	textout->SetString( buf );
}

LRESULT CControlStatus::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	switch( msg )
	{
	case WM_LBUTTONDOWN:
		{
			int x = LOWORD(lParam);
			int y = HIWORD(lParam);
			if( IsIn(x,y) )
				return 1;
		}
		break;
	case WM_LBUTTONUP:
		{
			int x = LOWORD(lParam);
			int y = HIWORD(lParam);
			if( IsIn(x,y) )
				return 1;
		}
		break;
	}
	return 0;
}

void CControlStatus::SetNkChar()
{
	int sid = 0;
	switch( g_pRoh->m_Race )
	{
	case RACE_BULKAN: // 불칸
		break;
	case RACE_KAILIPTON: // 카이립톤
		sid = 2;
		break;
	case RACE_AIDIA: // 에이디아
		sid = 4;
		break;
	case RACE_HUMAN: // 휴먼
		sid = 6;
		break;
	case RACE_FREAK: // 하이브리더
		sid = 8;
		break;
	case RACE_PEROM: // 하이브리더
		sid = 10;
		break;
	}

	if( g_pRoh->m_Sex == 1 )
		sid++;

	m_face.Init( facename[sid] );
}

HRESULT CControlStatus::RestoreSurfaces()
{
	m_face_load = false;
	LoadRes_Char();

	return S_OK;
}