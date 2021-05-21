#include "stdafx.h"



#include <string.h>
#include <TCHAR.h>
#include "main.h"
#include "headers.h"
#include "ddutil.h"
#include "AlphaPopUp.h"
#include "Nk2DFrame.h"
#include "ControlBottom.h"
#include "Scroll.h"
#include "Mouse.h"
#include "BasicButton.h"
#include "textoutbox.h"
#include "country.h"
#include "g_stringmanager.h"
#include "ImageDefine.h"

#include "ControlNotice.h"





extern int					g_EffectTextr[];

extern char g_strFontName[];
extern HFONT G_GetFont(int h, int w,const char  * const fontname);

///�����˾�â
CControlNotice::CControlNotice()
	: m_hWnd(NULL)
	, m_PosX(0)
	, m_PosY(0)
{
	m_pTitleBar = NULL;
	m_pTitleBarLight = NULL;
	m_pTitle = NULL;     // â Ÿ��Ʋ �κ��� ����.(���ý� ������ ������ ������ �� �������ֵ���...

	m_pContentsSurface = NULL; // ǥ�� ������ �ؽ�Ʈ�� ���ǽ��� ��ȯ�س��� ��. ������ �����ø��� �ٽ� ����� ����Ѵ�.

	m_pSizeControl = NULL;     // ������ ������ ���� Ŭ���� �̹���

	m_pNewMessage = NULL; // �� �޼������� ȿ����.


	Init(); // ������ �ʱ�ȭ.
}

CControlNotice::~CControlNotice()
{
	DeleteRes();
}

void CControlNotice::Init()
{
	m_nType = NOTICE_TYPE_NORMAL; // ���� �����ΰ�..
	m_nState = NOTICE_STATE_FULL; // �������̽��� ���� ( 0 : ���� ����, 1 : �ּ�ȭ ���� )
	m_bMoveDrag = FALSE; // ��ġ�̵� �巡�� ���ΰ�.
	m_bSizeDrag = FALSE; // ������ ���� �巡�� ���ΰ�.
	m_bNewMessage = FALSE; // ���ο� �޼����� ���ִ���. (��������)
	m_SizeRect.left = 0; // ����Ʈ ������
	m_SizeRect.top = 0; // ����Ʈ ������ (�ٸ� �� WindowChange���� ����)
	m_ViewLineNum = 0;
	m_TotalLineNum = 0;

	ZeroMemory( &m_Contents, sizeof(CMultiText2) ); // ���� ����� ����� ��������Ѵ�.

	m_MiniBtn.SetFileName("PopupNotice/btn_mini");
	m_MaxBtn.SetFileName("PopupNotice/btn_max");
	m_CloseBtn.SetFileName("PopupNotice/btn_close");
}

void CControlNotice::DeleteRes()
{
	SAFE_DELETE( m_pSizeControl );
	m_CloseBtn.DeleteRes();
	m_MaxBtn.DeleteRes();
	m_MiniBtn.DeleteRes();
	m_Scroll.DeleteRes();
	SAFE_DELETE( m_pContentsSurface );
	SAFE_DELETE( m_pTitle );
	SAFE_DELETE( m_pTitleBarLight );
	SAFE_DELETE( m_pTitleBar );
}

void CControlNotice::LoadRes()
{
	if( !g_pDisplay )
		return;

	m_hWnd =  g_pDisplay->GetHWnd();
	GetClientRect(m_hWnd, &m_ClientRc);

	g_pDisplay->CreateSurfaceFromBitmap( &m_pTitleBar, "interface/PopupNotice/title_bar.bmp" );
	if( m_pTitleBar )
		m_pTitleBar->SetColorKey( RGB(0, 0, 0) );

	g_pDisplay->CreateSurfaceFromBitmap( &m_pTitleBarLight, "interface/PopupNotice/title_bar2.bmp" );
	if( m_pTitleBarLight )
		m_pTitleBarLight->SetColorKey( RGB(0, 0, 0) );


	m_Scroll.LoadRes( "PopupNotice//scr_up", "PopupNotice//scr_down", "interface//PopupNotice//scr_bar.bmp" );
	m_Scroll.SetRange( 1 ); // ��ũ�� ������ ������ ������ �ʱ�ȭ ���� �ʿ䰡 �ִ�.

	m_MiniBtn.LoadRes();
	m_MaxBtn.LoadRes();
	m_CloseBtn.LoadRes();


	g_pDisplay->CreateSurfaceFromBitmap( &m_pSizeControl, "interface/PopupNotice/sizecontroler.bmp" );
	if( m_pSizeControl )
		m_pSizeControl->SetColorKey( RGB(0, 0, 0) );

	WindowChange( 10, 100, 150, 143 ); // �ʱ���ġ�� ������ ����(��ư�� ��ǥ ���õ� ���⼭ ���ش�.)
}

HRESULT CControlNotice::RestoreSurfaces()
{
	DeleteRes();
	LoadRes();
	MakeTextSurface();

	return S_OK;
}

void CControlNotice::SetType( int Type ) // Ÿ��Ʋ ����.
{
	m_nType = Type;

	if( m_pTitle ) // �ϴ� �����..
		SAFE_DELETE( m_pTitle );

	switch( m_nType ) // Ÿ�Կ� ���� Ÿ��Ʋ�� �ٲ��ش�.
	{
	case NOTICE_TYPE_MOBIUS:
		g_pDisplay->CreateSurfaceFromBitmap( &m_pTitle, "interface/PopupNotice/mobiusarena.bmp" );
		break;
	}
}

void CControlNotice::SetContents( char *String ) // ��Ʈ�� ����.
{
	if( !String )
		return;

	if( m_Contents.m_LineCount >= 150 ) // ������ �̻� ���̸�..
		m_Contents.Reset(); // ����� �ٽ� ó������ �ִ´�.

	m_Contents.AddString( String, RGB(255,255,255) );
	m_Contents.AddString( " ", RGB(255,255,255) ); // ���� ������ �־��ش�.

	MakeTextSurface(); // ���ǽ��� ���� �����.

	// ��ũ���� �ڵ� ���� ���ش�.
	m_Scroll.SetPos( m_Scroll.RangeMax ); // �̷��� �ϸ� �ڵ����� �Ʒ��� ���õ� ��.
}

void CControlNotice::Draw() // �� �Լ��� ������ DrawItemInfo()�Լ����� ǥ�õǴ� ������ ������ �����ϱ� ���� ���δ�.
{
	// ���� �������̽� ���� ��ο�.
	RenderOj( m_PosX, m_PosY, m_SizeRect.right, m_SizeRect.bottom );

	// Ÿ��Ʋ �� �׸���.
	if( m_pTitleBar )
		g_pDisplay->Blt( m_PosX, m_PosY,  m_PosX+m_SizeRect.right, m_PosY+m_pTitleBar->GetHeight(), m_pTitleBar );
	if( m_bNewMessage && m_pTitleBarLight && (timeGetTime()%1000) > 500 )
		g_pDisplay->Blt( m_PosX, m_PosY,  m_PosX+m_SizeRect.right, m_PosY+m_pTitleBarLight->GetHeight(), m_pTitleBarLight );

	// 3D â ���� 2D ��µ�
	if( m_nState == NOTICE_STATE_FULL ) // ���� ����
	{
		if( g_pDisplay && m_pContentsSurface )
		{
			RECT TempRect;
			TempRect.left = 0;
			TempRect.right = m_pContentsSurface->GetWidth();
			TempRect.top = m_Scroll.GetPos()*POPUP_NOTICE_TEXTLINE_HEIGHT;

			if( m_TotalLineNum > m_ViewLineNum )
				TempRect.bottom = (m_Scroll.GetPos()*POPUP_NOTICE_TEXTLINE_HEIGHT) + (m_SizeRect.bottom - POPUP_NOTICE_TEXTSUR_H_SPACE);
			else
				TempRect.bottom = m_pContentsSurface->GetHeight();

			g_pDisplay->Blt( m_PosX+10, m_PosY+25, m_pContentsSurface, &TempRect );
		}

		if( m_pTitle )
		{
			if( m_SizeRect.right >= 148 )
				g_pDisplay->Blt( m_PosX+5, m_PosY+1, m_pTitle );
			else // ����� ������ Ŭ����.
			{
				RECT TempRect;
				TempRect.left = 0;
				TempRect.right = m_pTitle->GetWidth() - (148-m_SizeRect.right);
				TempRect.top = 0;
				TempRect.bottom = m_pTitle->GetHeight();
				g_pDisplay->Blt( m_PosX+5, m_PosY+1, m_pTitle, &TempRect );
			}
		}

		if( m_Scroll.RangeMax != 1 )
			m_Scroll.Draw();
		m_MiniBtn.Draw(); // �ּ�ȭ ��ư

		if( m_pSizeControl )
		{
			g_pDisplay->Blt( m_pSizeControl->Xpos, m_pSizeControl->Ypos, m_pSizeControl );
		}
	}
	else if( m_nState == NOTICE_STATE_MINI ) // �̴ϸ� ����
	{
		m_MaxBtn.Draw(); // �ִ�ȭ ��ư

	}

	m_CloseBtn.Draw(); // �ݱ� ��ư
}

//-------------------------------------------------------------------
// name : RenderOj
// desc : 3d ����� ����Ͽ� �˾�â�� �������� ����(BACK)�� �׸���.
//-------------------------------------------------------------------
void CControlNotice::RenderOj(int xpos, int ypos, int cx, int cy)
{
	if( m_nState == NOTICE_STATE_MINI ) // �̴� ����϶� �̰� �ȱ׸���.
		return;

	LPDIRECTDRAWSURFACE7 lpTexture = NULL;

	D3DMATRIX matWorld, matView, matProjOld, matProjNew;
	D3DVIEWPORT7 vpOld, vpNew;

	DWORD	dwColor;

	// ������ ����Ʈ�� �����ϰ� ���Ӱ� ����Ʈ�� �����Ѵ�.
	GET_D3DDEVICE()->GetViewport(&vpOld);
	vpNew = vpOld;
	vpNew.dwWidth  = m_ClientRc.right;
	vpNew.dwHeight = m_ClientRc.bottom;
	GET_D3DDEVICE()->SetViewport(&vpNew);

	//// �������� ��Ʈ���� ����.
	float fAspect = (float) ((double) vpNew.dwHeight / (double) vpNew.dwWidth);
	GET_D3DDEVICE()->GetTransform(D3DTRANSFORMSTATE_PROJECTION, &matProjOld);
	D3DUtil_SetProjectionMatrix(matProjNew, 1.0f, fAspect, 1.0f, 2000.0f );
	GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_PROJECTION, &matProjNew);

	// ������
	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_AMBIENT, &dwColor );
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_AMBIENT, 0x00FFFFFF);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_LIGHTING, FALSE);

	//----------------�׸��� ���� / �� --------------
	if( SUCCEEDED( GET_D3DDEVICE()->BeginScene() ) )
	{
		//-----------------------TLVERTEX �� �̿��� ������ ���(ȭ���� ���� ��ǥ�� �Է��Ѵ�)

		// ���� â
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE   , TRUE);
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCCOLOR);
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_DESTCOLOR);
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHATESTENABLE   , TRUE);

		m_TLVec[0] = D3DTLVERTEX(D3DVECTOR(xpos,    ypos,     0), 1, 0x88000000, 0, 0, 0);
		m_TLVec[1] = D3DTLVERTEX(D3DVECTOR(xpos+cx, ypos,     0), 1, 0x88000000, 0, 1, 0);
		m_TLVec[2] = D3DTLVERTEX(D3DVECTOR(xpos,    ypos+cy,  0), 1, 0x88000000, 0, 0, 1);
		m_TLVec[3] = D3DTLVERTEX(D3DVECTOR(xpos+cx, ypos+cy,  0), 1, 0x88000000, 0, 1, 1);

		GET_D3DDEVICE()->SetTexture(0, lpTexture );
		GET_D3DDEVICE()->DrawPrimitive(D3DPT_TRIANGLESTRIP, D3DFVF_TLVERTEX, m_TLVec, 4, 0);

		GET_D3DDEVICE()->EndScene();
	}

	// ���󺹱�.
	GET_D3DDEVICE()->SetViewport(&vpOld);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_AMBIENT, dwColor );
	GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_PROJECTION, &matProjOld);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_LIGHTING, TRUE);
}

LRESULT CControlNotice::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	static int DistX = 0, DistY = 0; // �巡�׽� Ŭ���� ���� ���콺 ��ǥ�� ���� ��ġ.
	static int OldSizeX = 0, OldSizeY = 0; // ������ ������ ���� ����� ����ϱ� ���ѳ༮
	int x = 0, y = 0;

	if( m_nState == NOTICE_STATE_FULL && m_Scroll.MsgProc(hWnd, msg, wParam, lParam) == S_OK)
		return 1;

	switch (msg)
	{
	case 0x020A: // �ٸ��콺 �޼���(����Ʈ �����ȿ����� �� �޼����� ������ ��ũ�ѿ� ���������ش�.)

		x = LOWORD (lParam);
		y = HIWORD (lParam);

		if( m_nState == NOTICE_STATE_FULL && IsInside(x,y-45) ) // ����Ʈ ���������� ó�� (�� �޼��������� y���� �̻��ؼ� �ణ ����)
		{
			if( (short)HIWORD(wParam) < 0 )
			{
				m_Scroll.NowPos += 1;

				if( m_Scroll.NowPos > m_Scroll.RangeMax-1 )
					m_Scroll.NowPos = m_Scroll.RangeMax-1;
			}
			else if( (short)HIWORD(wParam) > 0 )
			{
				m_Scroll.NowPos -= 1;

				if( m_Scroll.NowPos < 0 )
					m_Scroll.NowPos = 0;
			}
			return 1;
		}

		break;

	case WM_LBUTTONDOWN:
		x = LOWORD (lParam);
		y = HIWORD (lParam);

		if(!IsInside(x, y)) //������ ���
			return 0;

		if( m_nState == NOTICE_STATE_FULL && m_bNewMessage ) // ���� �� �����̴� ���̸� Ŭ�������ε� ����
			m_bNewMessage = FALSE;

		if( !m_MiniBtn.IsInside(x,y) && !m_MaxBtn.IsInside(x,y) && !m_CloseBtn.IsInside(x,y) ) // ��ư ���������� �ƴϸ�...
		{
			// �巡�� ���� üũ.
			if( IsSizeControlInside(x,y) && m_nState == NOTICE_STATE_FULL ) // ������ ���� ��Ʈ�� �����̸�..(�������¿����� ����)
			{
				m_bSizeDrag = TRUE; // ������ �巡������ ǥ��
				DistX = x; // �� �༮�� ���� ��ġ�� ���
				DistY = y;
				OldSizeX = m_SizeRect.right; // ���� ����� ���
				OldSizeY = m_SizeRect.bottom;
			}
			else if( IsTitleBarInside(x,y) )
			{
				m_bMoveDrag = TRUE; // �� �ܿ� �̵� �巡��.
				DistX = m_PosX - x; // �̳༮�� ��ǥ�� ���� ���� ���̸� ���.
				DistY = m_PosY - y;
			}
		}

		if( m_MiniBtn.m_bVisible ) // ���϶��� ó��
			m_MiniBtn.MsgProc(hWnd, msg, wParam, lParam);
		if( m_MaxBtn.m_bVisible ) // ���϶��� ó��
			m_MaxBtn.MsgProc(hWnd, msg, wParam, lParam);
		m_CloseBtn.MsgProc(hWnd, msg, wParam, lParam);

		return 1;

	case WM_LBUTTONUP:
		x = LOWORD (lParam);
		y = HIWORD (lParam);

		// ���콺 ���� �巡�״� ������ ���.
		m_bSizeDrag = FALSE; // ������ �巡������ ǥ��
		m_bMoveDrag = FALSE; // �� �ܿ� �̵� �巡��.

		if(!IsInside(x, y))//������ ���
			return 0;

		if( m_bSizeDrag || m_bMoveDrag ) // �巡�� �߿� ��ư ���� ���ϰ�..
			return 0;

		if( m_MiniBtn.m_bVisible ) // ���϶��� ó��
		{
			m_MiniBtn.MsgProc(hWnd, msg, wParam, lParam);
			if( m_MiniBtn.GetState() == BTN_ACTION )
			{
				m_nState = NOTICE_STATE_MINI;
				// �ּ�ȭ �ϱ��� ũ�⸦ ���.
				OldSizeX = m_SizeRect.right;
				OldSizeY = m_SizeRect.bottom;

				// ũ��� ��ġ�� ����.
				WindowChange( m_PosX+(OldSizeX-POPUP_NOTICE_MINIMIZE_SIZE_X), m_PosY, POPUP_NOTICE_MINIMIZE_SIZE_X, POPUP_NOTICE_MINIMIZE_SIZE_Y ); // ������ ����

				m_MiniBtn.SetState(BTN_NORMAL);

				m_MiniBtn.SetVisible( FALSE );
				m_MaxBtn.SetVisible( TRUE );

				return 1;
			}
		}

		if( m_MaxBtn.m_bVisible ) // ���϶��� ó��
		{
			m_MaxBtn.MsgProc(hWnd, msg, wParam, lParam);
			if( m_MaxBtn.GetState() == BTN_ACTION )
			{
				m_nState = NOTICE_STATE_FULL;


				WindowChange( m_PosX-(OldSizeX-POPUP_NOTICE_MINIMIZE_SIZE_X), m_PosY, OldSizeX, OldSizeY ); // ������ ����

				if( m_bNewMessage )
				{
					MakeTextSurface(); // �ּ�ȭ ���¿��� �� �޼����� ���Ծ��ٸ� ������ ���ǽ��� �ٽ� ����� ����Ѵ�.
					m_bNewMessage = FALSE; // �����ÿ� ������ Ǯ���ش�
				}

				m_MaxBtn.SetState(BTN_NORMAL);

				m_MiniBtn.SetVisible( TRUE );
				m_MaxBtn.SetVisible( FALSE );

				return 1;
			}
		}

		m_CloseBtn.MsgProc(hWnd, msg, wParam, lParam);
		if( m_CloseBtn.GetState() == BTN_ACTION )
		{
			m_CloseBtn.SetState(BTN_NORMAL);

			g_pNk2DFrame->ShowControlNoticeWindow( FALSE ); // â�� ����.(�޸𸮿��� ����)

			return 1;
		}

		return 1;

	case WM_MOUSEMOVE:

		x = LOWORD (lParam);
		y = HIWORD (lParam);

		if( m_bSizeDrag ) // ������ ���� ��.
		{
			WindowChange( m_PosX, m_PosY, OldSizeX+(x-DistX), OldSizeY+(y-DistY) ); // ������ ����

			// ������ �����Ǿ����� ��ũ�ѹ� ���� ���ǽ��� �ٽ� ����� ����Ѵ�.
			MakeTextSurface(); // �ؽ�Ʈ ���ǽ��� �ٽ� �����.
		}
		else if( m_bMoveDrag ) // ��ġ �̵���.
		{
			WindowChange( x + DistX, y + DistY, m_SizeRect.right, m_SizeRect.bottom ); // ��ǥ ����
		}

		break;

	}

	return 0;
}

BOOL CControlNotice::IsInside( int x, int y )
{
	if ( ( ( x > m_PosX) && ( x < (m_PosX+m_SizeRect.right)) )
			&& ( ( y > m_PosY) && ( y < (m_PosY+m_SizeRect.bottom)) ) )
		return TRUE;

	return FALSE;
}

BOOL CControlNotice::IsTitleBarInside( int x, int y ) // ������ ���� Ÿ��Ʋ ��.
{
	if ( ( ( x > m_PosX) && ( x < (m_PosX+m_SizeRect.right)) )
			&& ( ( y > m_PosY) && ( y < (m_PosY+POPUP_NOTICE_TITLEBAR_SIZE_Y)) ) )
		return TRUE;

	return FALSE;
}

BOOL CControlNotice::IsSizeControlInside( int x, int y ) // ������ ������ ���� �����ڸ��� Ŭ���ߴ°�.
{
	if( !m_pSizeControl )
		return FALSE;

	if ( ( x > m_pSizeControl->Xpos ) && ( x < m_pSizeControl->Xpos+m_pSizeControl->GetWidth() )
			&& ( y > m_pSizeControl->Ypos ) && ( y < m_pSizeControl->Ypos+m_pSizeControl->GetHeight() ) )
		return TRUE;

	return FALSE;
}

void CControlNotice::WindowChange( int x, int y, int size_right, int size_bottom ) // â ��ġ�� ����� �ٲ� ������ϴ� �͵�
{
	m_PosX = x;
	m_PosY = y;

	m_SizeRect.right = size_right;
	m_SizeRect.bottom = size_bottom;

	if( m_bSizeDrag ) // ������ ���� ���̶��...
	{
		// �ִ� �ּ� ������ ����
		if( m_nState == NOTICE_STATE_FULL ) // ���� ���¿����� �����Ѵ�.
		{
			if( m_SizeRect.right < 120 ) // �ּ� ������
				m_SizeRect.right = 120;
			if( m_SizeRect.bottom < 143 )
				m_SizeRect.bottom = 143;
			if( m_SizeRect.right > 500 ) // �ִ� ������
				m_SizeRect.right = 500;
			if( m_SizeRect.bottom > 300 )
				m_SizeRect.bottom = 300;
		}

		// ȭ�� �Ѿ�� ����.
		if( m_PosX+m_SizeRect.right > m_ClientRc.right )
			m_SizeRect.right = m_ClientRc.right - m_PosX;
		if( m_PosY+m_SizeRect.bottom > m_ClientRc.bottom - g_pNk2DFrame->GetControlBottom()->GetRefHeight() )
			m_SizeRect.bottom = m_ClientRc.bottom - g_pNk2DFrame->GetControlBottom()->GetRefHeight() - m_PosY;
	}
	else if( m_bMoveDrag ) // ȭ�� �Ѿ�� ����. (��ġ �̵� �����ε� �������� ������ �����Ǿ���Կ� ����.)
	{
		if( m_PosX < 0 )
			m_PosX = 0;
		if(	m_PosY < 0 )
			m_PosY = 0;
		if( m_PosX > m_ClientRc.right - m_SizeRect.right )
			m_PosX = m_ClientRc.right - m_SizeRect.right;
		if( m_PosY > m_ClientRc.bottom - m_SizeRect.bottom - g_pNk2DFrame->GetControlBottom()->GetRefHeight() )
			m_PosY = m_ClientRc.bottom - m_SizeRect.bottom - g_pNk2DFrame->GetControlBottom()->GetRefHeight();
	}
	else // �� ���� �Ƹ��� ���� ��� �ϰŴ�.
	{
		// ����� ������ä ��ǥ�� ����
		if( m_PosX < 0 )
			m_PosX = 0;
		if( m_PosY > m_ClientRc.bottom - m_SizeRect.bottom - g_pNk2DFrame->GetControlBottom()->GetRefHeight() )
			m_PosY = m_ClientRc.bottom - m_SizeRect.bottom - g_pNk2DFrame->GetControlBottom()->GetRefHeight();
	}


	// ��ġ ������.
	if( m_pSizeControl )
	{
		m_pSizeControl->Xpos = m_PosX+m_SizeRect.right-m_pSizeControl->GetWidth()-2;
		m_pSizeControl->Ypos = m_PosY+m_SizeRect.bottom-m_pSizeControl->GetHeight()-2;
	}
	m_MiniBtn.SetPosition( m_PosX+m_SizeRect.right-39, m_PosY+1 );	// ��ư
	m_MaxBtn.SetPosition( m_PosX+m_SizeRect.right-39, m_PosY+1 );	// ��ư
	m_CloseBtn.SetPosition( m_PosX+m_SizeRect.right-20, m_PosY+1 );	// ��ư
	m_Scroll.SetDrawPos( m_PosX+m_SizeRect.right-20, m_PosY+25, m_SizeRect.bottom-25-20 ); // ��ũ��
}

void CControlNotice::MakeTextSurface() // CMultiText �� ���ǽ��� ��ȯ���ֱ� ���� �༮.
{
	int OldSurfaceHeight = 0;

	if( m_pContentsSurface )
	{
		OldSurfaceHeight = m_pContentsSurface->GetHeight();

		SAFE_DELETE( m_pContentsSurface ); // ����� �����...
	}

	if( !g_pDisplay  )	// m_Font �� �ڵ��̴�
		return;

	if( m_Contents.m_LineCount == 0 ) // �ؽ�Ʈ�� ��� �׳� ����
		return;


	CMultiText2 TempMulti; // ����� ���� �����.
	int NextLinePos = 0; // ���ڿ� ¥�� ����Ʈ ��.

	int FontHeight = 14;
	HFONT hFont = G_GetFont( FontHeight,0, g_strFontName );

	HDC hdc;
	hdc = GetDC(pCMyApp->Get_hWnd());
	HFONT hOldFont = (HFONT) SelectObject( hdc, hFont );

	// ���ڿ� ¥�� ����Ʈ �� ���.(��Ƽ ����Ʈ�� �������� ©���ֱ� ���� 2�� ����� ������)(��Ƽ����Ʈ ������ �Ʒ� AddStringMulti���� �ڵ����� ���ٲ�?
	SIZE	sizeFont;
	getFontSize( &sizeFont, hFont, "A", 1 );
	int FontWidth = sizeFont.cx; // �� ¥���� ����� ��Ʈ ����ũ��.���Ƿ� ����. ���� ũ�� ����.


	NextLinePos = (m_SizeRect.right-50) / FontWidth;

	if( NextLinePos % 2 )
		NextLinePos++; // 2�� ����� �����.

	int i = 0;
	for( i = 0 ; i < m_Contents.m_LineCount ; ++i ) // ���� ������ŭ.
	{
		TempMulti.AddStringMulti( m_Contents.m_Text[i], NextLinePos, TRUE, m_Contents.m_TextColor[i], m_Contents.m_TextBackColor[i] );
	}

	// �������� �ټ��� ���ټ��� ���.
	m_ViewLineNum = (m_SizeRect.bottom-POPUP_NOTICE_TEXTSUR_H_SPACE) / POPUP_NOTICE_TEXTLINE_HEIGHT;
	m_TotalLineNum = TempMulti.m_LineCount;

	int TextSurWidth = NextLinePos * FontWidth;
	int TextSurHeight = TempMulti.m_LineCount * FontHeight;

	// ���� ũ��� ���ǽ� ����

	g_pDisplay->CreateSurface( &m_pContentsSurface, TextSurWidth, TextSurHeight );
	if( !m_pContentsSurface )
		return;

	m_pContentsSurface->SetWidth( TextSurWidth );
	m_pContentsSurface->SetHeight( TextSurHeight );


	// fillcolor�� �̿��� textsurface Ŭ�����ϱ�
	m_pContentsSurface->Clear();

	int Ypos = 0;
	for( i = 0 ; i < TempMulti.m_LineCount ; ++i )
	{
		m_pContentsSurface->DrawText( hFont, TempMulti.m_Text[i], 0, Ypos,
									  TempMulti.m_TextBackColor[i], TempMulti.m_TextColor[i], 0);

		Ypos+=FontHeight;
	}
	m_pContentsSurface->SetColorKey(TEXT_BACK_COLOR);

	SelectObject(hdc, hOldFont);
	ReleaseDC(pCMyApp->Get_hWnd(), hdc);

	// ���ǽ��� ���� ��ũ���� �ٽ� �������ش�.
	if( m_TotalLineNum > m_ViewLineNum )
	{
		m_Scroll.SetRange( m_TotalLineNum-m_ViewLineNum ); // �ȼ������� �ƴ� ���δ����� ����.
	}
	else // ��ũ���� �ʿ���� ���� �϶�.
	{
		m_Scroll.SetRange( 1 ); // ��ũ��
	}

	m_Scroll.SetPos( m_Scroll.RangeMax ); // �̷��� �ϸ� �ڵ����� �Ʒ��� ���õ� ��. (���� ��ũ�� ��ġ�� �����ֱ� ����� ����Ƿ� �׳� �� ������ �Ű��ش�.)
}