#include "stdafx.h"
#include "headers.h"
#include "Sheet.h"
#include "Startpage.h"
#include "LogoPage.h"
#include "SMRender.h"



#define GOSTART 1
#define START_LOGO		"interface/back_logopage.bmp"

CLogoPage::CLogoPage()
{
	m_pBatangSur = NULL;
	m_pSheet = NULL;
	Init();
}

CLogoPage::CLogoPage(CSheet *pSheet)
{
	m_pBatangSur = NULL;
	m_pSheet = pSheet;
	Init();
}

CLogoPage::~CLogoPage()
{
	DeleteRes();
}

void CLogoPage::Init()
{}

void CLogoPage::LoadRes()
{
	SetBackground();
}

void CLogoPage::DeleteRes()
{
	SAFE_DELETE(m_pBatangSur);
}

void CLogoPage::SetBackground()
{
	if (m_pBatangSur)
		SAFE_DELETE(m_pBatangSur);

	if( g_pDisplay )
		g_pDisplay->CreateSurfaceFromBitmap( &m_pBatangSur, START_LOGO, 640, 480 );
}

void CLogoPage::Draw()
{
	if( g_pDisplay )
		g_pDisplay->Blt(0, 0, m_pBatangSur, NULL);
}

void CLogoPage::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch ( msg )
	{
	case WM_LBUTTONDOWN:
	case WM_MOUSEMOVE:
		break;

	case WM_LBUTTONUP:
		break;

	case WM_KEYUP:
		if (wParam == VK_SPACE)
			Action(GOSTART);
		break;
	}
}

void CLogoPage::Action(int i)
{
	switch (i)
	{
	case GOSTART:
		if( m_pSheet )
			m_pSheet->GoPage( STARTPAGE );
		return ;
	}
}


