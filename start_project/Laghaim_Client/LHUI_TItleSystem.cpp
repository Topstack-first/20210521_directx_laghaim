#include "stdafx.h"
#include "LHUI_TItleSystem.h"

#include "BasicButton.h"
#include "BasicStatic.h"
#include "ListBox.h"

#include "main.h"
#include "UIMgr.h"
#include "Nk2DFrame.h"
#include "Tcpipcon.h"






LHUI_TItleSystem::LHUI_TItleSystem(TitleSystem_Type type)
	:	m_background(NULL)
	,	m_btn_close(NULL)
	,	m_text(NULL)
	,	m_btn_execute(NULL)
	,	m_lst_title(NULL)
	,	m_type(type)
	,	m_current_title(-1)
{
	for( int i=0 ; i<MAX_TITLE; i++ )
		m_my_title[i] = -1;

	Init();
}

LHUI_TItleSystem::~LHUI_TItleSystem(void)
{
	DeleteRes();

	m_current_title = -1;
	for( int i=0 ; i<MAX_TITLE; i++ )
		m_my_title[i] = -1;
}

void LHUI_TItleSystem::Init()
{
	m_btn_close = new CBasicButton("common/btn_close_01");
	if( m_type == TitleSystem_Regist )
	{
		m_background = new CBasicStatic("");
		m_text = new CBasicStatic("");
		m_btn_execute = new CBasicButton("");
	}
	else
	{
		m_background = new CBasicStatic("");
		m_text = new CBasicStatic("");
		m_btn_execute = new CBasicButton("");
	}

	m_lst_title = new CListBox(" ", 0, 0, 0, 0);

	m_current_title = -1;
}

void LHUI_TItleSystem::LoadRes()
{
	m_background->SetPos(0,0);
	m_btn_close->SetPosition(0,0);
	m_btn_execute->SetPosition(0,0);
	//m_lst_title->SetPosDim();
}

void LHUI_TItleSystem::DeleteRes()
{
	SAFE_DELETE(m_background);
	SAFE_DELETE(m_btn_close);
	SAFE_DELETE(m_btn_execute);
	SAFE_DELETE(m_lst_title);
}

void LHUI_TItleSystem::Draw()
{
	m_background->Draw();
	m_btn_close->Draw();
	m_btn_execute->Draw();
	m_lst_title->Draw();
}

LRESULT LHUI_TItleSystem::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	if( g_pNk2DFrame->Check_All_MsgPopUp() )
		return 1;

	switch( msg )
	{
	case WM_LBUTTONUP:
	case WM_MOUSEMOVE:
		{
			m_lst_title->GetMessage(hWnd, msg, wParam, lParam);

			POINT pt = { LOWORD(lParam) , HIWORD(lParam) };
			if( !m_background->IsIn( pt.x , pt.y) )
				return 0;

			if( m_btn_close->GetState() == BTN_ACTION )
				return 1;

			if( m_btn_execute->GetState() == BTN_ACTION )
			{
				if( m_type == TitleSystem_Regist)
				{
				}
				else
				{
				}
			}
		}
		break;

	case WM_LBUTTONDOWN:
		{
			POINT pt = { LOWORD(lParam) , HIWORD(lParam) };
			if( !m_background->IsIn( pt.x , pt.y) )
				return 0;

			if( m_btn_close->GetState() == BTN_DOWN )
				return 1;

			if( m_btn_execute->GetState() == BTN_DOWN )
				return 1;

			int sel = m_lst_title->GetMessage(hWnd, msg, wParam, lParam );
			if( sel >= 0 )
			{
				int cur_sel = m_lst_title->GetSelectedLine();
			}
		}
		break;
	}

	return 0;
}