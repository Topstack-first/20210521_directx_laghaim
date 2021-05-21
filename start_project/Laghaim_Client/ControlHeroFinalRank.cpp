#include "stdafx.h"


#include "headers.h"
#include "BaseInterface.h"
#include "main.h"
#include "ControlSocial.h"
#include "ddutil.h"
#include "Nk2DFrame.h"
#include "tcpipcon.h"
#include "TcpUtil.h"
#include "ListBox.h"
#include "ControlHeroFinalRank.h"
#include "Land.h"
#include "UIMgr.h"
#include "g_stringmanager.h"
#include "GUtil.h"
#include "Mouse.h"




extern char					g_strFontName[];
extern HFONT G_GetFont(int h, int w,const char  * const fontname);

CControlHeroFinalRank::CControlHeroFinalRank()
{
	m_pBack = NULL;

	for( int i=0; i < MAX_HERO_FINAL_RANK; i++ )
	{
		m_Rank[i].bEnable	= FALSE;
		m_Rank[i].pSur		= NULL;
	}

	SetFont(5, 12, g_strFontName);
}

CControlHeroFinalRank::~CControlHeroFinalRank()
{
	DeleteRes();
}

void CControlHeroFinalRank::SetFont (int f_w, int f_h, char *fontname)
{
	m_Font = G_GetFont(f_h,0,fontname);
}

void CControlHeroFinalRank::LoadRes()
{
	HWND	  hWnd;

	hWnd = g_pDisplay->GetHWnd();
	GetClientRect(hWnd, &m_ClientRc);

	g_pDisplay->CreateSurfaceFromBitmap( &m_pBack, "interface/Bossraid/HeroFinalRank/bg.bmp");
	if(m_pBack)
	{
		m_pBack->SetColorKey(RGB(0, 0, 0));
		m_pBack->Xpos = m_ClientRc.right - m_pBack->GetWidth();
		m_pBack->Ypos = 29;
	}
}


void CControlHeroFinalRank::SetHeroRank(char* buf)
{
	char strTemp[128] = {0,};

	for(int i=0; i<MAX_HERO_FINAL_RANK; i++)
	{		
		m_Rank[i].bEnable = FALSE;
		SAFE_DELETE( m_Rank[i].pSur );

		buf = AnyOneArg(buf, strTemp);

		if( atoi(strTemp) != -1 )
		{
			m_Rank[i].bEnable = TRUE;
			g_pDisplay->CreateSurfaceFromText( &m_Rank[i].pSur, m_Font, strTemp, RGB(0, 0, 0), RGB(255,200,1000) );

			if( m_Rank[i].pSur )
				m_Rank[i].pSur->SetColorKey(RGB(0, 0, 0));
		}

		if( m_Rank[i].bEnable )
		{
			m_Rank[i].pSur->Xpos = m_pBack->Xpos + 155;
			m_Rank[i].pSur->Ypos = m_pBack->Ypos + 77 + i*22 + ((i/3) * 3 );
		}
	}

}

void CControlHeroFinalRank::DeleteRes()
{
	SAFE_DELETE(m_pBack);

	for(int i=0; i<MAX_HERO_FINAL_RANK; i++)
	{
		m_Rank[i].bEnable = FALSE;
		SAFE_DELETE( m_Rank[i].pSur );
	}
}


int CControlHeroFinalRank::IsInside(int x, int y)
{
	// 마우스가 bg안에 있는지 검사
	int left, right, top, bottom;
	left	= m_pBack->Xpos;
	top		= m_pBack->Ypos;
	right	= left + m_pBack->GetWidth();
	bottom	= top  + m_pBack->GetHeight();

	if( left < x && right > x && top < y && bottom > y)
		return 1;

	return 0;

}


void CControlHeroFinalRank::Draw()
{
	if( m_pBack )
		g_pDisplay->Blt(m_pBack->Xpos, m_pBack->Ypos, m_pBack);

	for(int i=0; i<MAX_HERO_FINAL_RANK; i++)
	{
		if( m_Rank[i].bEnable )
			g_pDisplay->Blt(m_Rank[i].pSur->Xpos, m_Rank[i].pSur->Ypos, m_Rank[i].pSur);
	}
}


LRESULT CControlHeroFinalRank::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int x = LOWORD (lParam);
	int y = HIWORD (lParam);

	switch ( msg )
	{
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:

		if(IsInside(x, y) == 1)
			return 1;
		//g_pNk2DFrame->ShowHeroFinalRankWindow(FALSE);
		break;
	}
	return 0;
}

