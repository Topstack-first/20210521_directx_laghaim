#include "stdafx.h"


#include "headers.h"
#include "Help.h"
#include "main.h"
#include "ddutil.h"
#include "Nk2DFrame.h"




//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHelp::CHelp()
	: m_ScreenWidth(0)
	, m_ScreenHeight(0)
{
	m_pSurfaceBack = NULL;
}

CHelp::~CHelp()
{
	DeleteRes();
}

void CHelp::LoadRes()
{
	if( !g_pNk2DFrame || !g_pDisplay )
		return;

	m_ScreenWidth = g_pNk2DFrame->GetClientWidth();
	m_ScreenHeight = g_pNk2DFrame->GetClientHeight();

	SAFE_DELETE(m_pSurfaceBack);

	g_pDisplay->CreateSurfaceFromBitmap(&m_pSurfaceBack, "interface/top/help.bmp");
	if( m_pSurfaceBack )
		m_pSurfaceBack->SetColorKey(RGB(0, 0, 0));
}

void CHelp::DeleteRes()
{
	SAFE_DELETE(m_pSurfaceBack);
}

void CHelp::Draw()
{
	if (g_pDisplay && m_pSurfaceBack)
		g_pDisplay->Blt(HELP_X, HELP_Y, m_pSurfaceBack);
}

LRESULT CHelp::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch ( msg )
	{
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
		if( g_pNk2DFrame )
			g_pNk2DFrame->ShowHelpWindow(FALSE);
		break;
	}
	return 0;
}




