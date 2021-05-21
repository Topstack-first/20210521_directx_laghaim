#include "stdafx.h"
// SpinBox.cpp: implementation of the CSpinBox class.
//
//////////////////////////////////////////////////////////////////////

#include "SpinBox.h"
#include "ddutil.h"
#include "headers.h"
#include "textbox.h"
#include "main.h"
#include "LoadEffectSound.h"




extern char g_strFontName[];
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSpinBox::CSpinBox()
	: m_Font(NULL)
	, m_FontColor(0)
	, m_FontSize(0)
	, m_SpinWidth(0)
	, m_SpinHeight(0)
{
	SetPos(0, 0);
	SetSize(0, 0);
	m_Size = 0;
	m_String = 0;
	m_CurSel = -1;
}
//----------------------------------------------------------------------------
//-- Destructor
//----------------------------------------------------------------------------
//-- Destructor

//-- 2004.03.15 - �����Ϸ�
//-- Lyul
//-- m_Font �� handle �� �����´�. ���� �ı��� �ʿ䰡 ����.
CSpinBox::~CSpinBox()
{
	DeleteRes();
	DeleteString();
}

void CSpinBox::DeleteRes()
{
	m_RSpinBtn.DeleteRes();
	m_LSpinBtn.DeleteRes();
}



void CSpinBox::Init(int pos_x, int pos_y, int width, int height,
					int font_size, COLORREF font_color)
{
	SetPos(pos_x, pos_y);
	SetSize(width, height);

	SetFont (font_size/2, font_size, g_strFontName);
	m_FontSize = font_size;
	m_FontColor = font_color;
}



void CSpinBox::LoadRes(char* RBtnFn, char* LBtnFn)
{
	if( !g_pDisplay || RBtnFn == NULL || LBtnFn == NULL )
		return;

	DeleteRes();

	m_RSpinBtn.SetFileName(RBtnFn);
	m_RSpinBtn.SetDisable( false );

	m_LSpinBtn.SetFileName(LBtnFn);
	m_LSpinBtn.SetDisable( false );


	m_RSpinBtn.LoadRes();
	m_LSpinBtn.LoadRes();

	m_SpinWidth  = m_LSpinBtn.Width;
	m_SpinHeight = m_LSpinBtn.Height;
}



void CSpinBox::SetSize(int size)
{
	DeleteString();

	m_String = new char*[size];
	for (int i = 0; i < size; i++)
	{
		m_String[i] = NULL;
	}
	m_Size = size;

	m_CurSel = 0;
}

void CSpinBox::DeleteString()
{
	for (int i = 0; i < m_Size; i++)
	{
		if (m_String[i])
		{
			delete[] m_String[i];
			m_String[i] = NULL;
		}
	}
	if (m_String)
	{
		delete[] m_String;
		m_String = NULL;
	}
}

void CSpinBox::SetString(int idx, char *string)
{
	if (idx < 0 || idx >= m_Size)
		return;

	if (m_String[idx])
	{
		delete[] m_String[idx];
		m_String[idx] = NULL;
	}

	m_String[idx] = new char[strlen(string)+1];
	strcpy(m_String[idx], string);
}

BOOL CSpinBox::IsInsideLSpin(int x, int y)
{
	if (x >= m_PosX && x < m_PosX + m_SpinWidth && y >= m_PosY && y < m_PosY + m_SpinHeight)
		return TRUE;

	return FALSE;
}

BOOL CSpinBox::IsInsideRSpin(int x, int y)
{
	if (x >= m_PosX + m_Width - m_SpinWidth && x < m_PosX + m_Width &&
			y >= m_PosY && y < m_PosY + m_SpinHeight)
		return TRUE;

	return FALSE;
}


void CSpinBox::Draw()
{
	if( !g_pDisplay )
		return;

	m_LSpinBtn.Draw(m_PosX, m_PosY);
	m_RSpinBtn.Draw(m_PosX+ m_Width - m_SpinWidth, m_PosY);

	if (!m_Font || m_CurSel < 0 || !m_String[m_CurSel])
		return;

	int len = strlen(m_String[m_CurSel]);
	if (len <= 0)
		return;

	CSurface *Surface;

	g_pDisplay->CreateSurfaceFromText(&Surface, m_Font, m_String[m_CurSel], RGB(0, 0, 1), m_FontColor);
	if( Surface == NULL )
		return;

	Surface->SetColorKey(RGB(0, 0, 1));

//	int x = m_PosX + (m_Width - (len * m_FontSize / 2)) / 2;
//	int y = m_PosY + (m_Height - m_FontSize) / 2;

	SIZE	sizeFont;
	getFontSize(&sizeFont, m_Font, "A", 1);

	int x = m_PosX + (m_Width - (len * sizeFont.cx)) / 2;
	int y = m_PosY + (m_Height - sizeFont.cy) / 2;

	g_pDisplay->Blt(x, y + pCMyApp->Font_AddPosY, Surface);

	delete Surface;
}
//----------------------------------------------------------------------------
//-- Set Font
//----------------------------------------------------------------------------
//-- Global Font �� �ϳ��� �޾ƿ´�.

//-- 2004.03.15 - �����Ϸ�
//-- Lyul
//-- ��ü �������� Global ������� ����


void CSpinBox::SetFont (int f_w, int f_h, char *fontname)
{
	if( fontname == NULL )
		return;
	// Font ����

	m_Font = G_GetFont(f_h,0,fontname);
	/*
		m_Font = CreateFont (-f_h, 0,	// ����, ���� ũ��
							0, 0,		// ������ ����
							FW_NORMAL,	// ��Ʈ�� ����
							FALSE,		// ���Ÿ� �÷���
							FALSE,		// ������� �÷���
							FALSE,		// Strikeout �÷���
							DEFAULT_CHARSET,		// Char SET
							OUT_DEFAULT_PRECIS,
							CLIP_DEFAULT_PRECIS,
							DEFAULT_QUALITY,
							DEFAULT_PITCH,
							fontname);
	*/
	// Line Height ��� ���..
	//LineHeight = f_h+2;		// 2 pixel ��...
	//MaxVisibleLine = (Height-BOARDER_OFF*2-TITLE_H-CONTROL_OFF) / LineHeight;

	// Bar Position ����
	//CalculateBarPos ();
}

int CSpinBox::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch ( uMsg)
	{
	case WM_LBUTTONDOWN:
		m_RSpinBtn.MsgProc(hWnd, uMsg, wParam, lParam);
		m_LSpinBtn.MsgProc(hWnd, uMsg, wParam, lParam);
		break;

	case WM_LBUTTONUP:
		m_RSpinBtn.MsgProc(hWnd, uMsg, wParam, lParam);
		m_LSpinBtn.MsgProc(hWnd, uMsg, wParam, lParam);
		if (m_LSpinBtn.GetState()==BTN_ACTION && IsInsideLSpin(LOWORD(lParam), HIWORD(lParam)))
		{
			if (m_CurSel >= 0)
			{
				m_CurSel--;
				if (m_CurSel < 0)
					m_CurSel = m_Size-1;
			}
		}
		else if (m_RSpinBtn.GetState()==BTN_ACTION && IsInsideRSpin(LOWORD(lParam), HIWORD(lParam)))
		{
			if (m_CurSel >= 0)
			{
				m_CurSel++;
				if (m_CurSel >= m_Size)
					m_CurSel = 0;
			}
		}
		else
		{
			//wan:2005-01
			//spinBtn�� ������ ������� ��ư�� ���¸� �븻�� ������.
			m_LSpinBtn.m_iBtnFlg = 1;
			m_RSpinBtn.m_iBtnFlg = 1;
		}

		return m_CurSel;

		break;

	case WM_MOUSEMOVE :
		m_RSpinBtn.MsgProc(hWnd, uMsg, wParam, lParam);
		m_LSpinBtn.MsgProc(hWnd, uMsg, wParam, lParam);
		break;

	default:
		break;
	}

	return -1;
}
