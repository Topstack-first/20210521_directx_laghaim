#include "stdafx.h"
#include "ExtraButton.h"
#include "headers.h"



LRESULT CToggleButton::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch ( msg )
	{
	case WM_LBUTTONDOWN:

		if ( (IsInside(LOWORD(lParam), HIWORD(lParam)))	&& (!m_bDisable) )
		{
			bDown = true;
			m_iBtnFlg = BTN_DOWN;
			// ������ ��� ������Ʈ �� �ٲٵ��� �Ѵ�.
			ToggleState ^= 1;

			return 1; // ����� ��� ��ư�� Down�� ����� ó��. �� �� �ٸ� �༮�鵵 �Ŀ� �ʿ��ϸ� return �� �߰��� �ʿ䰡 �ִ�.
		}
		break;

	case WM_LBUTTONUP:

		if ( (IsInside(LOWORD(lParam), HIWORD(lParam))) && (bDown) )
		{
			m_iBtnFlg = BTN_ACTION;
			bDown = false;
		}

		if ( (!IsInside(LOWORD(lParam), HIWORD(lParam))) && (bDown) )
			bDown = false;

		break;

	case WM_MOUSEMOVE:
		if ( (IsInside(LOWORD(lParam), HIWORD(lParam)))&& (!m_bDisable))
			m_iBtnFlg = BTN_ON;

		if ( (IsInside(LOWORD(lParam), HIWORD(lParam)))&& (bDown))
			m_iBtnFlg = BTN_DOWN;

		if ( ! (IsInside(LOWORD(lParam), HIWORD(lParam))) && (!m_bDisable))
			m_iBtnFlg = BTN_NORMAL;

		break;
	}

	return 0;
}

void CToggleButton::Draw()
{
	if (!m_bVisible || !g_pDisplay)
		return;

	RECT rc;
	int top = 0;
	SetRect( &rc , 0, 0 , Width , Height );

	if( ToggleState ) // ������ ��.
	{
		top = Height;
		SetRect( &rc , 0, top , Width , top+Height);
	}

	g_pDisplay->Blt( PosX, PosY, m_surface , &rc );
}


void CToggleButton::DrawToBlt(BOOL bChecked)
{
	DDBLTFX  ddbltfx;
	ZeroMemory(&ddbltfx, sizeof(ddbltfx));
	ddbltfx.dwSize      = sizeof(ddbltfx);

	SetRect(&m_BtnRc, (int) (PosX), (int) (PosY),
			(int) ((PosX+Width)), (int) ((PosY+Height)));

	if ( !m_bVisible || !g_pDisplay || !g_pDisplay->GetBackBuffer() )
		return;

	// by evilkiki
	RECT rc;
	int top = 0;
	SetRect( &rc , 0, 0 , Width , Height );

	if( ToggleState ) // ������ ��.
	{
		top = Height;
		SetRect( &rc , 0, top , Width , top+Height);
	}

	g_pDisplay->GetBackBuffer()->Blt(&m_BtnRc, m_surface->GetDDrawSurface(), &rc, DDBLT_KEYSRC |DDBLT_WAIT, &ddbltfx);
}

void CLoopButton::SetFileName( char* str )
{
	if( NumOfState >= MAX_LOOP_BUTTON-1 )
	{
		MessageBox( NULL, "Loop Button Number Over", "Error", MB_OK );
		return;
	}

	strcpy( m_arr_bmpfile[NumOfState], "interface/" );
	strcat( m_arr_bmpfile[NumOfState], str );
	strcat( m_arr_bmpfile[NumOfState], ".bmp" );

	NumOfState++;
}

void CLoopButton::DeleteRes()
{
	// ���⼭ �������ִ°� �޸����� Array��ҵ鸸 ���ܿ� ����.
	for( int i = 0 ; i < MAX_LOOP_BUTTON ; ++i )
	{
		SAFE_DELETE( m_arr_surface[i] );
	}

	m_surface = NULL;
}

void CLoopButton::LoadRes( int w, int h )
{
	if( !g_pDisplay )
		return;

	DeleteRes();

	for( int i = 0 ; i < NumOfState ; ++i )
	{
		// by evilkiki
		//	w , h �� ������ �θ�������
		//	�ǹ� ����
		//if( w > 0 )
		//{
		//	g_pDisplay->CreateSurfaceFromBitmap( &m_arr_surface[i], m_arr_bmpfile[i], w, h);
		//}
		//else
		{
			g_pDisplay->CreateSurfaceFromBitmap( &m_arr_surface[i], m_arr_bmpfile[i] );
		}

		if( m_arr_surface[i] )
			m_arr_surface[i]->SetColorKey(TRANS_COLOR);
	}

	if( m_arr_surface[0] )
	{
		Width = m_arr_surface[0]->GetWidth();
		Height = m_arr_surface[0]->GetHeight() >> 2;

		m_surface = m_arr_surface[0];
	}
}

int CLoopButton::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch ( msg )
	{
	case WM_LBUTTONDOWN:

		if ( (IsInside(LOWORD(lParam), HIWORD(lParam)))	&& (!m_bDisable) )
		{
			bDown = true;
			m_iBtnFlg = BTN_DOWN;

			return 1;
		}
		break;

	case WM_LBUTTONUP:

		if ( (IsInside(LOWORD(lParam), HIWORD(lParam))) && (bDown) )
		{
			m_iBtnFlg = BTN_ACTION;
			bDown = false;

			LoopState++;
			if( LoopState > NumOfState-1 )
				LoopState = 0;

			m_surface = m_arr_surface[LoopState];

			return 2;
		}

		if ( (!IsInside(LOWORD(lParam), HIWORD(lParam))) && (bDown) )
			bDown = false;

		break;

	case WM_MOUSEMOVE:

		if ( (IsInside(LOWORD(lParam), HIWORD(lParam)))&& (!m_bDisable))
			m_iBtnFlg = BTN_ON;

		if ( (IsInside(LOWORD(lParam), HIWORD(lParam)))&& (bDown))
			m_iBtnFlg = BTN_DOWN;

		if ( ! (IsInside(LOWORD(lParam), HIWORD(lParam))) && (!m_bDisable))
			m_iBtnFlg = BTN_NORMAL;

		break;
	}

	return 0;
}
