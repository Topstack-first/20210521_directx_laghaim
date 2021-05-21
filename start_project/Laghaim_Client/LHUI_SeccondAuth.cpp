#include "stdafx.h"
#include "LHUI_SeccondAuth.h"

#include "main.h"
#include "Nk2DFrame.h"
#include "sheet.h"
#include "ddutil.h"
#include "g_stringmanager.h"

#include <stdlib.h>
#include <time.h>
#include <algorithm>

#define PARENTCLASS LHUI_Base

LHUI_SeccondAuth::LHUI_SeccondAuth(CSheet* sheet)
	:	m_sheet(sheet)
	,	m_captcha_number(0)
	,	m_current(0)
{
	memset(m_input_number, 0, sizeof(m_input_number));

	for( int i=0 ; i<MAX_CAPTCHA_BUTTON ; ++i )
		m_arrange_num.push_back(i);

	_SetVisible(false);
	_SetDisable(true);
}

LHUI_SeccondAuth::~LHUI_SeccondAuth(void)
{
	DeleteRes();
}

void LHUI_SeccondAuth::_generate()
{
}

void LHUI_SeccondAuth::_reset()
{
	m_current = 0;

	_relocate_button();

	m_captcha_output.SetString(m_captcha_number);
	m_captcha_output_test.SetString("");
}

void LHUI_SeccondAuth::_relocate_button()
{
	std::random_shuffle(m_arrange_num.begin(), m_arrange_num.end());

	ArrangeNum_it it = m_arrange_num.begin();
	for( int i=0 ; it!=m_arrange_num.end() ; ++i, ++it )
		m_btn_captcha[*it].SetPosition(m_bg.m_x+42+(21*i), m_bg.m_y+179);
}

bool LHUI_SeccondAuth::_check(const int iv)
{
	return true;
}


void LHUI_SeccondAuth::Init()
{
}

void LHUI_SeccondAuth::LoadRes()
{
	m_MsgPopUp.Init(TYPE_NOR_OK);

	m_bg.Init("captcha/bg_captcha");
	m_btn_confirm.Init("captcha/btn_ok");
	m_btn_cancel.Init("captcha/btn_exit");
	m_btn_refresh.Init("captcha/btn_change");
	m_btn_back.Init("passwordlock/btn_back");

	char btn_filename[64];
	for(int i=0 ; i<MAX_CAPTCHA_BUTTON ; ++i)
	{
		sprintf( btn_filename, "passwordlock/btn_%d" , i );
		m_btn_captcha[i].Init(btn_filename);
	}

	m_captcha_output.Init(20, RGB(255, 210, 0),0, 0);
	m_captcha_output_test.Init(15, RGB(255, 210, 0),0, 0);


	int sx = (g_pNk2DFrame->GetClientWidth() - m_bg.m_w)>>1;
	int sy = (g_pNk2DFrame->GetClientHeight() - m_bg.m_h)>>1;

	m_bg.SetPos(sx, sy);
	m_btn_confirm.SetPosition(sx+83,sy+216);
	m_btn_cancel.SetPosition(sx+183,sy+216);
	m_btn_refresh.SetPosition(sx+213,sy+111);
	m_btn_back.SetPosition(sx+210+42,sy+179);

	m_MsgPopUp.LoadRes();

	_reset();
}

void LHUI_SeccondAuth::DeleteRes()
{
	m_MsgPopUp.DeleteRes();
}

void LHUI_SeccondAuth::SetPos(int x, int y)
{
}

void LHUI_SeccondAuth::Draw()
{
	if( !_GetVisible() )
		return;

	m_captcha_output.SetString(m_captcha_number);
	m_captcha_output.SetString_withOutLine(m_captcha_output.GetString(), RGB(255, 210, 0), RGB(255, 255, 255));
	m_captcha_output_test.SetString("");

	m_bg.Draw();
	m_btn_confirm.Draw();
	m_btn_cancel.Draw();
	m_btn_refresh.Draw();
	m_btn_back.Draw();

	for(int i=0 ; i<MAX_CAPTCHA_BUTTON ; i++)
		m_btn_captcha[i].Draw();

	m_captcha_output.Draw(m_bg.m_x+51, m_bg.m_y+77,211);
	m_captcha_output_test.Draw(m_bg.m_x+40, m_bg.m_y+148,235);

	m_MsgPopUp.Draw();
}

LRESULT LHUI_SeccondAuth::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if( !_GetVisible() && _GetDisalbe())
		return 0;

	if(m_MsgPopUp.m_bActive == TRUE)
	{
		int ret = -1;
		if ((ret = m_MsgPopUp.MsgProc(hWnd, msg, wParam, lParam)) == 1)
		{
		}
		else if (ret == -1)
		{
			_reset();
		}
		return 0;
	}

	switch( msg )
	{
	case WM_MOUSEMOVE:
	case WM_LBUTTONUP:
	case WM_LBUTTONDOWN:
		return MsgProc_Mouse(hWnd, msg, wParam , lParam);
	}

	return 0;
}

UINT LHUI_SeccondAuth::MsgProc_Mouse( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int x = LOWORD(lParam);
	int y = HIWORD(lParam);

	if( false == m_bg.IsIn( x, y ) )
		return 0;

	m_btn_confirm.MsgProc(hWnd, msg, wParam, lParam);
	m_btn_cancel.MsgProc(hWnd, msg, wParam, lParam);
	m_btn_refresh.MsgProc(hWnd, msg, wParam, lParam);
	m_btn_back.MsgProc(hWnd, msg, wParam, lParam);
	for(int i=0 ; i<MAX_CAPTCHA_BUTTON ; i++)
		m_btn_captcha[i].MsgProc(hWnd, msg, wParam, lParam);

	switch( msg )
	{
	case WM_MOUSEMOVE:
		break;
	case WM_LBUTTONUP:
		{
			if( m_btn_confirm.GetState() == BTN_ACTION )
			{
				m_btn_confirm.SetState(BTN_NORMAL);
				return 1;
			}
			if( m_btn_cancel.GetState() == BTN_ACTION)
			{
				_SetVisible(false);
				_SetDisable(true);
				m_btn_cancel.SetState(BTN_NORMAL);
				return 0;
			}
			if( m_btn_back.GetState() == BTN_ACTION)
			{
				if( m_current <= 0 )
					m_current = 0;
				else
					--m_current;
				m_input_number[m_current] = -1;


				m_btn_back.SetState(BTN_NORMAL);
				return 1;
			}
			for(int i=0 ; i<MAX_CAPTCHA_BUTTON ; i++)
			{
				if( m_btn_captcha[i].GetState() == BTN_ACTION )
				{
					if( m_current < 4 )
						m_input_number[m_current++] = i;

					m_btn_captcha[i].SetState(BTN_NORMAL);
					return 1;
				}
			}
		}
		break;
	case WM_LBUTTONDOWN:
		{
			if( m_btn_confirm.GetState() == BTN_DOWN )
			{
				return 0;
			}
			if( m_btn_cancel.GetState() == BTN_DOWN)
			{
				return 0;
			}
			if( m_btn_refresh.GetState() == BTN_DOWN)
			{
				return 0;
			}
			if( m_btn_back.GetState() == BTN_DOWN)
			{
				return 0;
			}
			for(int i=0 ; i<MAX_CAPTCHA_BUTTON ; i++)
			{
				if( m_btn_captcha[i].GetState() == BTN_DOWN )
				{
					return 0;
				}
			}
		}
		break;
	}
	return 0;
}