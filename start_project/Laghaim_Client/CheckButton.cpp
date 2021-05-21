#include "stdafx.h"
#include "CheckButton.h"
#include "d3dutil.h"
#include "headers.h"




//

CCheckbutton* CCheckbutton::Create(int nNum/* = 2*/)
{
	return new CCheckbutton[nNum];
}

CCheckbutton::CCheckbutton()
{
	m_uiwidth  = 0;
	m_uiheight = 0;

	m_nPosX	   = 0;
	m_nPosY	   = 0;


	m_iBtnFlg  = BUTTON_UNCHECK;

	m_bDisable = FALSE;
	m_bVisible = TRUE;
	m_bDown	   = FALSE;

	m_pUnCheckSurface = NULL;
	m_pCheckSurface   = NULL;


	ZeroMemory(m_szuncheckfile, sizeof(m_szuncheckfile));
	ZeroMemory(m_szcheckfile, sizeof(m_szcheckfile));
}

CCheckbutton::~CCheckbutton()
{
	DeleteRes();
}

void CCheckbutton::DeleteRes()
{
	SAFE_DELETE(m_pUnCheckSurface);
	SAFE_DELETE(m_pCheckSurface);
}

bool CCheckbutton::IsInside(int x, int y)
{
	if ( ( ( x > m_nPosX) && ( x < m_nPosX + (int)m_uiwidth ) )
			&& ( ( y > m_nPosY) && ( y < (m_nPosY + (int)m_uiheight) ) ) )
		return true;
	else
		return false;
}

void CCheckbutton::SetFileName(char* szName)
{
	strcpy( m_szuncheckfile, "interface/" );
	strcat( m_szuncheckfile, szName );
	strcat( m_szuncheckfile, "_uncheck.bmp");

	strcpy( m_szcheckfile, "interface/" );
	strcat( m_szcheckfile, szName );
	strcat( m_szcheckfile, "_check.bmp" );
}

void CCheckbutton::LoadRes(int w /* = -1 */, int h /* = -1  */)
{
	if( !g_pDisplay ) return;

	DeleteRes();

	if (w > 0)
	{
		g_pDisplay->CreateSurfaceFromBitmap( &m_pUnCheckSurface, m_szuncheckfile, w, h);
		g_pDisplay->CreateSurfaceFromBitmap( &m_pCheckSurface, m_szcheckfile, w, h);
	}
	else
	{
		g_pDisplay->CreateSurfaceFromBitmap( &m_pUnCheckSurface, m_szuncheckfile);
		g_pDisplay->CreateSurfaceFromBitmap( &m_pCheckSurface  , m_szcheckfile);
	}

	if( m_pUnCheckSurface )
		m_pUnCheckSurface->SetColorKey(TRANS_COLOR);

	if (m_pCheckSurface)
		m_pCheckSurface->SetColorKey(TRANS_COLOR);

	m_uiwidth  = m_pUnCheckSurface->GetWidth();
	m_uiheight = m_pUnCheckSurface->GetHeight();
}

int CCheckbutton::GetState()
{
	return m_iBtnFlg;
}

void CCheckbutton::SetStatus(BOOL bable)
{
	if(bable)
	{
		m_iBtnFlg = BUTTON_CHECK;
		m_bDisable = bable;
		m_bDown = TRUE;
	}
	else
	{
		m_iBtnFlg = BUTTON_UNCHECK;
		m_bDisable = bable;
		m_bDown = FALSE;
	}

}

void CCheckbutton::DrawToBlt(BOOL bChecked)
{
	if(!bChecked)	return;

	DDBLTFX  ddbltfx;
	ZeroMemory(&ddbltfx, sizeof(ddbltfx));
	ddbltfx.dwSize = sizeof(ddbltfx);
	ddbltfx.ddckSrcColorkey ;

	SetRect(&m_BtnRc
			,(int)m_nPosX
			,(int)m_nPosY
			,(int)((m_nPosX+(int)m_uiwidth))
			,(int)((m_nPosY+(int)m_uiheight)));

	if ( !m_bVisible || !g_pDisplay )
		return;

	if( g_pDisplay->GetBackBuffer() )
	{
		if(m_pUnCheckSurface->GetDDrawSurface() && m_iBtnFlg == BUTTON_UNCHECK)
		{
			g_pDisplay->GetBackBuffer()->Blt(&m_BtnRc, m_pUnCheckSurface->GetDDrawSurface(), NULL, DDBLT_KEYSRC |DDBLT_WAIT, &ddbltfx);
		}
		else if(m_pCheckSurface->GetDDrawSurface() && m_iBtnFlg == BUTTON_CHECK)
		{
			g_pDisplay->GetBackBuffer()->Blt(&m_BtnRc, m_pCheckSurface->GetDDrawSurface(), NULL, DDBLT_KEYSRC |DDBLT_WAIT, &ddbltfx);
		}
	}
}

bool CCheckbutton::MsgProcGroup( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch ( msg )
	{
	case WM_LBUTTONDOWN:
		if( (IsInside(LOWORD(lParam), HIWORD(lParam)))	&& (!m_bDisable) )
		{
			m_bDisable = TRUE;
			m_bDown    = TRUE;
			m_iBtnFlg  = BUTTON_CHECK;

			return m_bDisable?true:false;
		}
		break;

	case WM_LBUTTONUP:
		if( (IsInside(LOWORD(lParam), HIWORD(lParam))) && (m_bDown) )
		{
			m_iBtnFlg = BUTTON_CHECK;
		}
		break;

	case WM_MOUSEMOVE:
		if( (IsInside(LOWORD(lParam), HIWORD(lParam)))&& (m_bDown))
			m_iBtnFlg = BUTTON_CHECK;

		if( (IsInside(LOWORD(lParam), HIWORD(lParam)))&& (!m_bDown))
			m_iBtnFlg = BUTTON_UNCHECK;

		break;
	}

	return FALSE;
}

BOOL CCheckbutton::DrawClip(int nMinArea, int nMaxArea, int nScrollPos, BOOL bCheck)
{
	if(!bCheck)
		return FALSE;

	RECT src, dest;

	src.left = 0;
	src.top  = 0;
	src.right  = m_uiwidth;
	src.bottom = m_uiheight;

	dest.left   = m_nPosX;
	dest.top    = m_nPosY;
	dest.right  = m_nPosX + m_uiwidth;
	dest.bottom = m_nPosY + m_uiheight;

	if(dest.top - nScrollPos <= nMinArea &&
			dest.bottom - nScrollPos <= nMinArea)	// 모두 밖에 있음..
	{
		return FALSE;
	}
	else if(dest.top - nScrollPos <= nMinArea &&		// 윗 영역에 걸쳤음.
			dest.bottom - nScrollPos >= nMinArea)
	{
		src.top  = abs((dest.top - nScrollPos) - nMinArea );
		dest.top = nMinArea;
		dest.bottom = (dest.bottom - nScrollPos);
	}
	else if(dest.top - nScrollPos >= nMinArea &&		// 모두 영역 안에 있음.
			dest.bottom - nScrollPos <= nMaxArea)
	{
		dest.left   = m_nPosX;
		dest.top    = m_nPosY  - nScrollPos;
		dest.right  = m_nPosX + m_uiwidth;
		dest.bottom = dest.top + m_uiheight;
	}
	else if(dest.top - nScrollPos <= nMaxArea &&
			dest.bottom - nScrollPos >= nMaxArea)	// 아랫 영역에 걸쳤음
	{
		src.bottom  = abs( nMaxArea - (dest.top - nScrollPos) );
		dest.top    = (dest.top- nScrollPos);
		dest.bottom = nMaxArea;
	}
	else if(dest.top - nScrollPos >= nMaxArea &&
			dest.bottom - nScrollPos >= nMaxArea) // 아랫영역 밖으로 모두 나갔삼.
	{
		return FALSE;
	}


	DDBLTFX  ddbltfx;
	ZeroMemory(&ddbltfx, sizeof(ddbltfx));
	ddbltfx.dwSize = sizeof(ddbltfx);
	ddbltfx.ddckSrcColorkey ;

	SetRect(&m_BtnRc
			,(int)dest.left
			,(int)dest.top
			,(int)((dest.left+(int)m_uiwidth))
			,(int)((dest.top+(int)(dest.bottom- dest.top))));

	if ( !m_bVisible || !g_pDisplay )
		return FALSE;

	if( g_pDisplay->GetBackBuffer() )
	{
		if(m_pUnCheckSurface->GetDDrawSurface() && m_iBtnFlg == BUTTON_UNCHECK)
		{
			g_pDisplay->GetBackBuffer()->Blt(&m_BtnRc, m_pUnCheckSurface->GetDDrawSurface(), NULL, DDBLT_KEYSRC |DDBLT_WAIT, &ddbltfx);
		}
		else if(m_pCheckSurface->GetDDrawSurface() && m_iBtnFlg == BUTTON_CHECK)
		{
			g_pDisplay->GetBackBuffer()->Blt(&m_BtnRc, m_pCheckSurface->GetDDrawSurface(), NULL, DDBLT_KEYSRC |DDBLT_WAIT, &ddbltfx);
		}
	}

	return TRUE;
}

BOOL CCheckbutton::IsInsideClip(int x, int y, int nMinArea, int nMaxArea, int nScrollPos)
{
	if (!m_bVisible || !g_pDisplay )
		return FALSE;

	RECT src, dest;

	src.left = 0;
	src.top  = 0;
	src.right  = m_uiwidth;
	src.bottom = m_uiheight;

	dest.left   = m_nPosX;
	dest.top    = m_nPosY;
	dest.right  = m_nPosX + m_uiwidth;
	dest.bottom = m_nPosY + m_uiheight;

	if(dest.top - nScrollPos <= nMinArea &&
			dest.bottom - nScrollPos <= nMinArea)	// 모두 밖에 있음..
	{
		return FALSE;
	}
	else if(dest.top - nScrollPos <= nMinArea &&		// 윗 영역에 걸쳤음.
			dest.bottom - nScrollPos >= nMinArea)
	{
		src.top  = abs((dest.top - nScrollPos) - nMinArea );
		dest.top = nMinArea;
		dest.bottom = (dest.bottom - nScrollPos);
	}
	else if(dest.top - nScrollPos >= nMinArea &&		// 모두 영역 안에 있음.
			dest.bottom - nScrollPos <= nMaxArea/*BOUNDARY_LINE_POSY_TOP*/)
	{
		dest.left   = m_nPosX;
		dest.top    = m_nPosY  - nScrollPos;
		dest.right  = m_nPosX + m_uiwidth;
		dest.bottom = dest.top + m_uiheight;
	}
	else if(dest.top - nScrollPos <= nMaxArea &&
			dest.bottom - nScrollPos >= nMaxArea)	// 아랫 영역에 걸쳤음
	{
		src.bottom  = abs( nMaxArea - (dest.top - nScrollPos) );
		dest.top    = (dest.top- nScrollPos);
		dest.bottom = nMaxArea;
	}
	else if(dest.top - nScrollPos >= nMaxArea &&
			dest.bottom - nScrollPos >= nMaxArea) // 아랫영역 밖으로 모두 나갔삼.
	{
		return FALSE;
	}

	if ( ( ( x > m_nPosX ) && ( x < m_nPosX + (int)m_uiwidth ) )
			&& ( ( y > dest.top ) && ( y < dest.bottom ) ) )
		return TRUE;
	else
		return FALSE;
}

void CCheckbutton::MsgProcClip( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, int nScrollPos, int nMin, int nMax)
{
	switch ( msg )
	{
	case WM_LBUTTONDOWN:
		if( (IsInsideClip(LOWORD(lParam), HIWORD(lParam), nMin, nMax, nScrollPos))	&& (!m_bDisable) )
		{
			m_bDisable = TRUE;
			m_bDown    = TRUE;
			m_iBtnFlg  = BUTTON_CHECK;
		}
		else if( (IsInsideClip(LOWORD(lParam), HIWORD(lParam), nMin, nMax, nScrollPos))	&& (m_bDisable) )
		{
			m_bDisable = FALSE;
			m_bDown	   = FALSE;
			m_iBtnFlg  = BUTTON_UNCHECK;
		}

		break;
	case WM_LBUTTONUP:
		if( (IsInsideClip(LOWORD(lParam), HIWORD(lParam), nMin, nMax, nScrollPos)) && (m_bDown) )
		{
			m_iBtnFlg = BUTTON_CHECK;
		}
		else if ( (IsInsideClip(LOWORD(lParam), HIWORD(lParam), nMin, nMax, nScrollPos)) && (!m_bDown) )
		{
			m_iBtnFlg = BUTTON_UNCHECK;
		}

		break;

	case WM_MOUSEMOVE:
		if( (IsInsideClip(LOWORD(lParam), HIWORD(lParam), nMin, nMax, nScrollPos)) && (m_bDown))
			m_iBtnFlg = BUTTON_CHECK;

		if( (IsInsideClip(LOWORD(lParam), HIWORD(lParam), nMin, nMax, nScrollPos)) && (!m_bDown))
			m_iBtnFlg = BUTTON_UNCHECK;

		break;
	}

}

void CCheckbutton::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch ( msg )
	{
	case WM_LBUTTONDOWN:
		if( (IsInside(LOWORD(lParam), HIWORD(lParam)))	&& (!m_bDisable) )
		{
			m_bDisable = TRUE;
			m_bDown    = TRUE;
			m_iBtnFlg  = BUTTON_CHECK;
		}
		else if( (IsInside(LOWORD(lParam), HIWORD(lParam)))	&& (m_bDisable) )
		{
			m_bDisable = FALSE;
			m_bDown	   = FALSE;
			m_iBtnFlg  = BUTTON_UNCHECK;
		}

		break;
	case WM_LBUTTONUP:
		if( (IsInside(LOWORD(lParam), HIWORD(lParam))) && (m_bDown) )
		{
			m_iBtnFlg = BUTTON_CHECK;
		}
		else if ( (IsInside(LOWORD(lParam), HIWORD(lParam))) && (!m_bDown) )
		{
			m_iBtnFlg = BUTTON_UNCHECK;
		}

		break;

	case WM_MOUSEMOVE:
		if( (IsInside(LOWORD(lParam), HIWORD(lParam)))&& (m_bDown))
			m_iBtnFlg = BUTTON_CHECK;

		if( (IsInside(LOWORD(lParam), HIWORD(lParam)))&& (!m_bDown))
			m_iBtnFlg = BUTTON_UNCHECK;

		break;
	}
}
