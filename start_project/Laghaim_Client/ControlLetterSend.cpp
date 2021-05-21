#include "stdafx.h"
#include "ControlLetterSend.h"
#include "Nk2DFrame.h"
#include "ControlBottom.h"
#include "PacketSend.h"
#include "ControlLetterWindow.h"

ControlLetterSend::ControlLetterSend()
	: m_pLetterBox(NULL)
	, m_isLock(false)
{
}

ControlLetterSend::~ControlLetterSend()
{
	DeleteRes();
}

HRESULT ControlLetterSend::RestoreSurfaces()
{
	DeleteRes();
	LoadRes();

	return S_OK;
}

void ControlLetterSend::LoadRes()
{
	m_background.Init("LETTER/BG_SEND_MSG");
	m_btnClose.Init("common/btn_close_01");
	m_btnSend.Init("LETTER/BTN_SEND");

	m_background.Align(6, g_pNk2DFrame->GetClientWidth(), g_pNk2DFrame->GetClientHeight());

	int bgx = m_background.m_x;
	int bgy = m_background.m_y;

	m_btnClose.SetPosition(bgx + 346, bgy + 6);
	m_btnSend.SetPosition(bgx + 140, bgy + 343);
	m_tbTitle.Init(bgx + 84, bgy + 46, 36, RGB(175,140,93), RGB(255,255,255));
	m_tbTitle.SetfontSize();
	m_tbName.Init(bgx + 84, bgy + 121, 40, RGB(175,140,93), RGB(255,255,255));
	m_tbName.SetfontSize();

	m_pLetterBox = new CLetterBox();
	m_pLetterBox->Init_MEBox(false, 9, 34);
	m_pLetterBox->Init(0, 0, 30, RGB(175,140,93), RGB(255,255,255));
	m_pLetterBox->SetPos(bgx + 84, bgy + 162);
	m_pLetterBox->SetMaxLen(400);
	m_pLetterBox->SetFontSize(12);
	m_pLetterBox->SetEnable(false);
}

void ControlLetterSend::DeleteRes()
{
	m_background.DeleteRes();
	m_btnClose.DeleteRes();
	m_btnSend.DeleteRes();
	m_tbTitle.DeleteRes();
	m_tbName.DeleteRes();

	if( m_pLetterBox )
		m_pLetterBox->DeleteRes();
	SAFE_DELETE(m_pLetterBox);
}

float ControlLetterSend::Update()
{
	DWORD timeCur = GetTickCount();
	DWORD timeElapsed = timeCur - m_time;
	m_time = timeCur;

	float fElapsed = (float)(timeElapsed) / 1000.0f;
	m_timeAccum += fElapsed;

	return 0;
}

void ControlLetterSend::Draw()
{
	m_background.Draw();
	m_btnClose.Draw();
	m_btnSend.Draw();
	m_tbTitle.Draw();
	m_tbName.Draw();

	if( m_pLetterBox )
		m_pLetterBox->Draw();
}

void ControlLetterSend::ActionClose()
{
	SetLock(false);

	g_pNk2DFrame->SetVisibleInterface(false, LETTER_SEND);
	((ControlLetterWindow*)g_pNk2DFrame->GetInterface(LETTER_WINDOW))->SetLock(false);
}

void ControlLetterSend::ActionSend()
{
	SetLock(true);

	SendNoteMessageSend(m_tbName.GetString(), m_tbTitle.GetString(), m_pLetterBox->m_MEBox.Get_CoreString());
}

bool ControlLetterSend::IsInside_Prompt(int x, int y)
{
	int bgx = m_background.m_x;
	int bgy = m_background.m_y;

	int start_x, start_y;
	int check_cx, check_cy;

	start_y = bgy + 160;
	check_cy = 149;

	start_x = bgx + 82;
	check_cx = 249;

	if ( ( ( x > start_x) && ( x < (start_x + check_cx)) )
		&& ( ( y > start_y ) && ( y < (start_y + check_cy) ) ) )
		return true;
	else
		return false;
}

LRESULT ControlLetterSend::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	if( m_tbTitle.GetEnable() && g_pNk2DFrame->GetControlBottom()->HasFocus() )
	{
		m_tbTitle.SetEnable(FALSE);
		pCMyApp->ConvertImmStatus();
	}
	if( m_tbName.GetEnable() && g_pNk2DFrame->GetControlBottom()->HasFocus() )
	{
		m_tbName.SetEnable(FALSE);
		pCMyApp->ConvertImmStatus();
	}
	if( m_pLetterBox && m_pLetterBox->GetEnable() && g_pNk2DFrame->GetControlBottom()->HasFocus() )
	{
		m_pLetterBox->SetEnable(FALSE);
		pCMyApp->ConvertImmStatus();
	}

	switch(msg)
	{
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_MOUSEMOVE:
		{
			int x = LOWORD (lParam);
			int y = HIWORD (lParam);

			if( m_background.IsIn(x, y) == false )
				return 0;

			if( msg == WM_LBUTTONDOWN )
			{
				bool notFail = false;
				{				
					int bInputEnable = m_tbTitle.GetEnable();

					if( E_FAIL != m_tbTitle.MsgProc(hWnd, msg, wParam, lParam) )
					{
						notFail = true;						
					}
					if( bInputEnable == FALSE && m_tbTitle.GetEnable() )
					{
						pCMyApp->RestoreImmStatus();						
					}
				}
				{
					int bInputEnable = m_tbName.GetEnable();

					if( E_FAIL != m_tbName.MsgProc(hWnd, msg, wParam, lParam) )
					{
						notFail = true;						
					}
					if( bInputEnable == FALSE && m_tbName.GetEnable() )
					{
						pCMyApp->RestoreImmStatus();	
					}
				}
				
				if( notFail == true )
				{
					if( m_pLetterBox->GetEnable() )
					{
						m_pLetterBox->SetEnable(FALSE);
					}
					return 1;
				}

				if( m_pLetterBox->IsInside(x, y) )
				{
					m_pLetterBox->MsgProc(hWnd, msg, wParam, lParam);
				}
				else
				{
					m_pLetterBox->SetEnable(FALSE);
				}
			}
			else if( msg == WM_LBUTTONUP )
			{
				m_tbTitle.MsgProc(hWnd, msg, wParam, lParam);
				m_tbName.MsgProc(hWnd, msg, wParam, lParam);

				m_pLetterBox->MsgProc(hWnd, msg, wParam, lParam);

				if( IsInside_Prompt(x, y) == true )
				{
					m_pLetterBox->SetEnable(TRUE);					
				}
				else
				{
					m_pLetterBox->SetEnable(FALSE);
					pCMyApp->RestoreImmStatus();	
				}
			}

			m_btnClose.MsgProc(hWnd,msg, wParam, lParam);
			m_btnSend.MsgProc(hWnd, msg, wParam, lParam);

			if( m_btnClose.GetState() == BTN_ACTION )
			{
				m_btnClose.SetState(BTN_NORMAL);
				ActionClose();

				return 1;
			}

			if( m_btnSend.GetState() == BTN_ACTION )
			{
				m_btnSend.SetState(BTN_NORMAL);
				ActionSend();

				return 1;
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
		{
			bool bEnable = false;
			if( m_tbTitle.GetEnable() )
			{
				m_tbTitle.MsgProc(hWnd, msg, wParam, lParam);
			}
			if( m_tbTitle.GetEnable() )
			{
				bEnable = true;
			}
			if( m_tbName.GetEnable() )
			{
				m_tbName.MsgProc(hWnd, msg, wParam, lParam);
			}
			if( m_tbName.GetEnable() )
			{
				bEnable = true;
			}
			if( m_pLetterBox && m_pLetterBox->GetEnable() )
			{
				m_pLetterBox->MsgProc(hWnd, msg, wParam, lParam);
			}
			if( m_pLetterBox && m_pLetterBox->GetEnable() )
			{
				bEnable = true;
			}

			if( bEnable == true )
			{
				return 1;
			}
		}
		break;
	}

	return 0;
}

void ControlLetterSend::ClearText()
{
	m_pLetterBox->ClearText();
	m_pLetterBox->m_MEBox.Reset();

	m_tbTitle.ClearText();
	m_tbTitle.SetText("\0");
	m_tbName.ClearText();
	m_tbName.SetText("\0");
}

void ControlLetterSend::SetName(const std::string& name)
{
	m_tbName.ClearText();
	m_tbName.SetText((char*)name.c_str());
}

bool ControlLetterSend::IsFocus()
{
	if( m_tbTitle.GetEnable() != 0 )
		return true;

	if( m_tbName.GetEnable() != 0 )
		return true;

	if( m_pLetterBox && m_pLetterBox->GetEnable() != 0 )
		return true;

	return false;
}
