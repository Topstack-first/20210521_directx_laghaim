#include "stdafx.h"
#include "UI_GmMacro.h"
#include "UIMgr.h"
#include "Nk2DFrame.h"
#include "ControlBottom.h"

UI_GmMacro::UI_GmMacro()
	: m_IsVisible(false)
{
	m_pBack = NULL;
	LoadRes();
}

UI_GmMacro::~UI_GmMacro()
{
	DeleteRes();
}

void UI_GmMacro::LoadRes()
{
	if( g_pDisplay == NULL )
		return;

	g_pDisplay->CreateSurfaceFromBitmap(&m_pBack, "interface/teleport/teleport.bmp");
	m_pBack->SetColorKey( TRANS_COLOR );
	m_pBack->Xpos = ( g_pNk2DFrame->GetClientWidth() - m_pBack->GetWidth() ) >> 1;
	m_pBack->Ypos = ( g_pNk2DFrame->GetClientHeight() - m_pBack->GetHeight() ) >> 1;

	int bgx = m_pBack->Xpos;
	int bgy = m_pBack->Ypos;

	for( int i = 0; i < MAX_GMMACRO_COUNT; i++ )
	{
		m_SendChatBtn[i].Init("teleport/btn_save");
		m_SendChatBtn[i].SetPosition(bgx + 272, bgy + 62 + (28 * i));

		m_MacroText[i].Init(bgx + 40, bgy + 68 + (28 * i), 17*2, RGB(255, 255, 255), _FCOLOR_YELLOW);
		m_MacroText[i].SetMaxLen(60);
		m_MacroText[i].SetfontSize(12);
		m_MacroText[i].DrawText();
	}

	m_CloseBtn.Init("common/btn_close_01");
	m_CloseBtn.SetPosition( bgx + 397 , bgy + 5 );

	loadMacro();
}

void UI_GmMacro::DeleteRes()
{
	SAFE_DELETE(m_pBack);

	for( int i = 0; i < MAX_GMMACRO_COUNT; i++ )
	{
		m_SendChatBtn[i].DeleteRes();
		m_MacroText[i].DeleteRes();
	}

	m_CloseBtn.DeleteRes();
}

void UI_GmMacro::Draw()
{
	if( m_IsVisible == false )
		return;

	if( g_pDisplay && m_pBack )
		g_pDisplay->Blt(m_pBack->Xpos, m_pBack->Ypos, m_pBack);

	int bgx = m_pBack->Xpos;
	int bgy = m_pBack->Ypos;

	for( int i = 0; i < MAX_GMMACRO_COUNT; i++ )
	{
		m_SendChatBtn[i].Draw();
		m_MacroText[i].Draw();
	}
	m_CloseBtn.Draw();
}

LRESULT UI_GmMacro::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if( m_IsVisible == false )
		return 0;

	int x = 0;
	int y = 0;

	x = LOWORD(lParam);
	y = HIWORD(lParam);

	switch(msg)
	{
	case WM_LBUTTONDOWN:
		{
			for( int i = 0; i < MAX_GMMACRO_COUNT; i++ )
			{
				m_SendChatBtn[i].MsgProc(hWnd, msg, wParam, lParam);
				m_MacroText[i].MsgProc(hWnd, msg, wParam, lParam);
			}

			m_CloseBtn.MsgProc(hWnd, msg, wParam, lParam);

			return 1;
		}
		break;

	case WM_LBUTTONUP:
		{
			for( int i = 0; i < MAX_GMMACRO_COUNT; i++ )
			{
				m_SendChatBtn[i].MsgProc(hWnd, msg, wParam, lParam);
				if( m_SendChatBtn[i].m_iBtnFlg == BTN_ACTION )
				{
					m_SendChatBtn[i].m_iBtnFlg = BTN_NORMAL;
					sendChatMacro(i);
					return 1;
				}
			}

			m_CloseBtn.MsgProc(hWnd, msg, wParam, lParam);
			if( m_CloseBtn.m_iBtnFlg == BTN_ACTION )
			{
				m_CloseBtn.m_iBtnFlg = BTN_NORMAL;
				SetVisible(false);
				return 1;
			}

			return 1;
		}
		break;

	case WM_MOUSEMOVE:
		{
			for( int i = 0; i < MAX_GMMACRO_COUNT; i++ )
			{
				m_SendChatBtn[i].MsgProc(hWnd, msg, wParam, lParam);
			}

			m_CloseBtn.MsgProc(hWnd, msg, wParam, lParam);

			return 1;
		}
		break;

	case WM_KEYDOWN:
		{
			if( wParam ==VK_RETURN )
			{
				for( int i = 0; i < MAX_GMMACRO_COUNT; i++ )
				{
					if( m_MacroText[i].GetEnable() == TRUE )
					{
						m_MacroText[i].CloseIMC();
						m_MacroText[i].SetEnable(FALSE);
						saveMacro();
						return 1;
					}
				}
			}
			if( wParam == VK_F2 )
			{
				SetVisible(false);
				return 1;
			}

			return 1;

		}
		break;

	case WM_CHAR:
	case WM_IME_STARTCOMPOSITION:
	case WM_IME_COMPOSITION:
	case WM_IME_ENDCOMPOSITION:
	case WM_IME_CHAR:
	case WM_IME_NOTIFY:
		{
			for( int i = 0; i < MAX_GMMACRO_COUNT; i++ )
			{
				if( m_MacroText[i].GetEnable() == TRUE )
				{
					m_MacroText[i].MsgProc(hWnd, msg, wParam, lParam);
					return 1;
				}
			}

			return 1;

		}
		break;

	case WM_KEYUP:
	case WM_SYSKEYDOWN:
	case WM_SYSCHAR:
		return 1;
	}

	return 0;
}

void UI_GmMacro::SetVisible(bool visible)
{
	m_IsVisible = visible;
}

bool UI_GmMacro::IsVisible()
{
	return m_IsVisible;
}

void UI_GmMacro::sendChatMacro(int index)
{
	if( index < 0 || index >= MAX_GMMACRO_COUNT )
		return;

	g_pNk2DFrame->GetControlBottom()->SetFocus();
	g_pNk2DFrame->GetControlBottom()->m_InputBox.ClearText();
	g_pNk2DFrame->GetControlBottom()->m_InputBox.SetText( m_MacroText[index].GetString() );

	saveMacro();
}

void UI_GmMacro::loadMacro()
{
	FILE* fp = fopen("gmmacro.txt", "rt");
	if( fp == NULL )
		return;

	char buf[1024] = {0,};
	int len = 0;
	for( int i = 0; i < MAX_GMMACRO_COUNT; i++ )
	{
		fgets(buf, 1024, fp);
		len = strlen(buf);
		if( len > 0 )
		{
			buf[len-1] = '\0';
			m_MacroText[i].SetText(buf);
		}
	}

	fclose(fp);
}

void UI_GmMacro::saveMacro()
{
	FILE* fp = fopen("gmmacro.txt", "wt");
	if( fp == NULL )
		return;

	for( int i = 0; i < MAX_GMMACRO_COUNT; i++ )
	{
		fprintf(fp, "%s\n", m_MacroText[i].GetString());
	}

	fclose(fp);
}