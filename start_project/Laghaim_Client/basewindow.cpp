#include "stdafx.h"
#include "basewindow.h"
#include "headers.h"
#include "basicbutton.h"
#include "ddutil.h"
#include "main.h"
#include "Nk2DFrame.h"
#include "window_resource.h"




CUIBaseWindow::CUIBaseWindow()
	: m_WndId(0)
{
	m_pBackScene	= NULL;
	m_pBtExit		= NULL;
	m_eResolution	= R800;
	m_bMove			= true;
	m_nPosX			= 0;
	m_nPosY			= 0;
	m_nPosFX		= 0;
	m_nPosFY		= 0;
	m_bMouseDown	= false;
}

CUIBaseWindow::~CUIBaseWindow()
{
	DeleteRes();
}

void CUIBaseWindow::SetBackground(int nResNum)
{
	m_pBackScene = g_pNk2DFrame->GetWindowRes()->GetWindowUI(nResNum);
}

void CUIBaseWindow::MakeCloseBtn(char *szName)
{
	if(m_pBtExit)	SAFE_DELETE(m_pBtExit);

	if(m_pBtExit == NULL)
	{
		m_pBtExit = new CBasicButton;
		m_pBtExit->SetFileName(szName);
		m_pBtExit->LoadRes();
	}
}

void CUIBaseWindow::SetExitBtnPostion(int nX, int nY)
{
	if(m_pBtExit)m_pBtExit->SetPosition(m_nPosX + nX, m_nPosY + nY);
}

void CUIBaseWindow::DeleteRes()
{
	SAFE_DELETE(m_pBtExit);
}

int	CUIBaseWindow::Draw()
{
	if(!m_pBackScene)	return 0;

	g_pDisplay->Blt(m_nPosX, m_nPosY, m_pBackScene);

	if(m_pBtExit)
		m_pBtExit->Draw();

	return 1;
}

BOOL CUIBaseWindow::MoveWindow(UINT msg, int x, int y)
{
	if( !m_bMove )
		return 0;

	switch( msg )
	{
	case WM_LBUTTONDOWN:
		if( !m_bMouseDown && IsInside(x, y) )
		{
			m_bMouseDown = true;
			m_nPosFX = x - m_nPosX;
			m_nPosFY = y - m_nPosY;
			return 1;
		}
		break;

	case WM_LBUTTONUP:
		m_nPosFX = m_nPosFY = 0;
		m_bMouseDown = false;
		return 1;
		break;

	case WM_MOUSEMOVE:
		if( m_bMouseDown )
		{
			int backX = m_nPosX;
			int backY = m_nPosY;

			m_nPosX = x - m_nPosFX;
			m_nPosY = y - m_nPosFY;

			int cw = g_pNk2DFrame->GetClientWidth() -  m_pBackScene->GetWidth() -1;
			int ch = g_pNk2DFrame->GetClientHeight() - m_pBackScene->GetHeight()-1;

			if( m_nPosX < 1 )	m_nPosX = 1;
			if( m_nPosX > cw )	m_nPosX = cw;
			if( m_nPosY < 1 )	m_nPosY = 1;
			if( m_nPosY > ch )	m_nPosY = ch;
			SetPosition();

			return 1;
		}
	}

	return 0;

}


BOOL CUIBaseWindow::IsInside(int x, int y)
{
	if(!m_pBackScene)	return FALSE;

	if (x >= (int)(m_nPosX) && x < (int)(m_nPosX + m_pBackScene->GetWidth()) &&
			y >= (int)(m_nPosY) && y < (int)(m_nPosY + m_pBackScene->GetHeight()) )
	{
		return TRUE;
	}

	return FALSE;
}

LRESULT CUIBaseWindow::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if(m_pBtExit)
		m_pBtExit->MsgProc(hWnd, msg, wParam, lParam);

	switch(msg)
	{
	case WM_LBUTTONDOWN:
		if( m_pBtExit && m_pBtExit->GetState() == BTN_DOWN )
			return IM_INPUT;

		break;

	case WM_LBUTTONUP :
		if(m_pBtExit && m_pBtExit->GetState() == BTN_ACTION)
			return IM_CLOSE;

		break;
	}

	if( MoveWindow( msg, LOWORD(lParam), HIWORD(lParam) )	)
		return IM_INPUT;

	return IM_NONE;
}
