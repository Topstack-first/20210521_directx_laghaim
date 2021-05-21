#include "stdafx.h"
#include "Slider.h"
#include "d3dutil.h"
#include "headers.h"





#define SLIDER_BT_RADIUS 5.f
#define SLIDER_BT_Y	2.f

#define SLIDER_RANGE_MAX (m_fMaxWidth-SLIDER_BT_RADIUS)

CSliderButton* CSliderButton::Create(int nNum/* = 2*/)
{
	return new CSliderButton[nNum];
}

CSliderButton::CSliderButton()
{

	m_uiwidth  = 0;
	m_uiheight = 0;

	m_uibwidth  = 140; // 슬라이더 백그라운드
	m_uibheight = 10; // 현재 슬라이더의 이미지는 없다.

	m_nPosX	   = 0;
	m_nPosY	   = 0;

	m_fBackBasisX = 0;
	m_fBackBasisY = 0;

	m_iBtnFlg  = BUTTON_UNCHECK1;

	m_bDisable = FALSE;
	m_bVisible = TRUE;
	m_bDown	   = FALSE;
	m_bbtDown  = FALSE;

	m_pUnCheckSurface = NULL;
	m_pCheckSurface   = NULL;

	m_fMaxWidth = 139.f;
	m_fPer		= 0.f;


	ZeroMemory(m_szuncheckfile, sizeof(m_szuncheckfile));
	ZeroMemory(m_szcheckfile  , sizeof(m_szcheckfile)  );
}

CSliderButton::~CSliderButton()
{
	DeleteRes();
}

void CSliderButton::DeleteRes()
{
	if(m_pUnCheckSurface)
		SAFE_DELETE(m_pUnCheckSurface);

	if(m_pCheckSurface)
		SAFE_DELETE(m_pCheckSurface);

	m_btPlus.DeleteRes();
	m_btMinus.DeleteRes();
}

bool CSliderButton::IsInsidebt(int x, int y)
{

	if ( ( ( x > m_BtnRc.left) && ( x < m_BtnRc.left + m_uiwidth ) )
			&& ( ( y > m_BtnRc.top) && ( y <  m_BtnRc.top + m_uiheight) ) )
		return true;
	else
		return false;


}

bool CSliderButton::IsInside(int x, int y)
{
	if ( ( ( x > m_fBackBasisX) && ( x < m_fBackBasisX + (int)m_uibwidth ) )
			&& ( ( y > m_fBackBasisY) && ( y < (m_fBackBasisY + (int)m_uibheight) ) ) )
		return true;
	else
		return false;


}

void CSliderButton::SetFileName(char* szName)
{
	strcpy( m_szuncheckfile, "interface/" );
	strcat( m_szuncheckfile, szName );
	strcat( m_szuncheckfile, "_normal.bmp");

	strcpy( m_szcheckfile, "interface/" );
	strcat( m_szcheckfile, szName );
	strcat( m_szcheckfile, "_click.bmp" );

	// 슬라이드가 가지고 있음..
	// 증가, 감소..
	m_btPlus.SetFileName("test/vol_plus");
	m_btMinus.SetFileName("test/vol_minus");
}

void CSliderButton::LoadRes(int w /* = -1 */, int h /* = -1  */)
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

	m_btPlus.LoadRes();
	m_btMinus.LoadRes();

}

int CSliderButton::GetState()
{
	return m_iBtnFlg;
}

void CSliderButton::SetStatus(BOOL bable)
{
	if(bable)
	{
		m_iBtnFlg = BUTTON_CHECK1;
		m_bDisable = bable;
		m_bDown = TRUE;
	}
	else
	{
		m_iBtnFlg = BUTTON_UNCHECK1;
		m_bDisable = bable;
		m_bDown = FALSE;
	}

}

void CSliderButton::DrawToBlt(BOOL bChecked)
{
	if(!bChecked)	return;

	DDBLTFX  ddbltfx;
	ZeroMemory(&ddbltfx, sizeof(ddbltfx));
	ddbltfx.dwSize = sizeof(ddbltfx);
	ddbltfx.ddckSrcColorkey ;

	int x,y,r,b;

	x = (int)(m_nPosX + (SLIDER_RANGE_MAX * m_fPer));
	y = (int)m_nPosY;
	r = (int)(x + m_uiwidth);
	b = (int)(y + m_uiheight);

	SetRect(&m_BtnRc, x, y, r, b);

	if ( !m_bVisible || !g_pDisplay )
		return;

	if( g_pDisplay->GetBackBuffer() )
	{
		if(m_pUnCheckSurface->GetDDrawSurface() && m_iBtnFlg == BUTTON_UNCHECK1)
		{
			g_pDisplay->GetBackBuffer()->Blt(&m_BtnRc, m_pUnCheckSurface->GetDDrawSurface(), NULL, DDBLT_KEYSRC |DDBLT_WAIT, &ddbltfx);
		}
		else if(m_pCheckSurface->GetDDrawSurface() && m_iBtnFlg == BUTTON_CHECK1)
		{
			g_pDisplay->GetBackBuffer()->Blt(&m_BtnRc, m_pCheckSurface->GetDDrawSurface(), NULL, DDBLT_KEYSRC |DDBLT_WAIT, &ddbltfx);
		}
	}

	m_btPlus.DrawToBlt(FALSE);
	m_btMinus.DrawToBlt(FALSE);
}

void CSliderButton::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	bool	bResult = false;

	if(msg == WM_LBUTTONUP)
	{
		m_bbtDown = false;
		bResult = true;
		m_iBtnFlg = BUTTON_UNCHECK1;
	}

	int mouse_x = LOWORD (lParam);
	int mouse_y = HIWORD (lParam);

	if(msg == WM_LBUTTONDOWN)
	{
		if(IsInside(mouse_x, mouse_y))
		{
			float fNowWidth = (float)mouse_x;

			m_fPer = (fNowWidth-m_fBackBasisX-SLIDER_BT_RADIUS)/(m_fMaxWidth-SLIDER_BT_RADIUS);

			if(m_fPer < 0.f)
				m_fPer = 0.f;
			else if(m_fPer > 1.f)
				m_fPer = 1.f;
		}

		if(IsInsidebt(mouse_x, mouse_y))
		{
			m_bbtDown = true;
			bResult   = true;
			m_iBtnFlg = BUTTON_CHECK1;
		}
	}

	if(m_bbtDown)
	{
		float fNowWidth = (float)mouse_x;
		m_fPer = (fNowWidth-m_fBackBasisX-SLIDER_BT_RADIUS)/SLIDER_RANGE_MAX;

		if(m_fPer < 0.f)
			m_fPer = 0.f;
		else if(m_fPer > 1.f)
			m_fPer = 1.f;
	}

	m_btPlus.MsgProc(hWnd, msg, wParam, lParam);
	m_btMinus.MsgProc(hWnd, msg, wParam, lParam);

	if(m_btPlus.GetState() == BTN_ACTION)
	{
		float fNowWidth = m_BtnRc.right;

		m_fPer = (fNowWidth-m_fBackBasisX-SLIDER_BT_RADIUS)/(m_fMaxWidth-SLIDER_BT_RADIUS);

		if(m_fPer < 0.f)
			m_fPer = 0.f;
		else if(m_fPer > 1.f)
			m_fPer = 1.f;


	}

	if(m_btMinus.GetState() == BTN_ACTION)
	{
		float fNowWidth = m_BtnRc.left;

		m_fPer = (fNowWidth-m_fBackBasisX-SLIDER_BT_RADIUS)/(m_fMaxWidth-SLIDER_BT_RADIUS);

		if(m_fPer < 0.f)
			m_fPer = 0.f;
		else if(m_fPer > 1.f)
			m_fPer = 1.f;
	}
}

