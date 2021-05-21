#include "stdafx.h"
#include "headers.h"
#include "BasicButton.h"
#include "DirtSnd.h"



CBasicButton::CBasicButton()
	: PosX(0)
	, PosY(0)
	, Width(0)
	, Height(0)
{
	memset(m_bmpfile, 0, sizeof(m_bmpfile));

	bDown = false;
	m_iBtnFlg = BTN_NORMAL;
	m_bVisible = TRUE;
	m_bDisable = FALSE;
	m_surface = NULL;
}

CBasicButton::CBasicButton(const char * filename,int w, int h)
	: PosX(0)
	, PosY(0)
{
	bDown = false;
	m_iBtnFlg = BTN_NORMAL;
	m_bVisible = TRUE;
	m_bDisable = FALSE;
	m_surface = NULL;

	Init( filename , w, h );
}

CBasicButton::~CBasicButton()
{
	DeleteRes();
}

void CBasicButton::DeleteRes()
{
	SAFE_DELETE(m_surface);
}

void CBasicButton::Init( const char * filename , int w , int h )
{
	SetFileName( filename );
	LoadRes(w, h);
}

void CBasicButton::SetSize( int x, int y)
{
	Width = x;
	Height = y;
}

bool CBasicButton::IsInside( int x, int y )
{
	if( PosX < x && PosX+Width > x &&
			PosY < y && PosY+Height > y )
		return true;

	return false;

// 	if( !m_surface )
// 		return false;
//
// 	return m_surface->IsIn(x,y);
}

void CBasicButton::SetFileName( const char* str )
{
	strcpy( m_bmpfile, "interface/" );
	strcat( m_bmpfile, str );
	strcat( m_bmpfile, ".bmp" );
}

HRESULT CBasicButton::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch ( msg )
	{
	case WM_LBUTTONDOWN:
		if ( (IsInside(LOWORD(lParam), HIWORD(lParam)))	&& (!m_bDisable) )
		{
			bDown = true;
			m_iBtnFlg = BTN_DOWN;

			return S_OK;
		}
		break;

	case WM_LBUTTONUP:
		if ( (IsInside(LOWORD(lParam), HIWORD(lParam))) && (bDown) )
		{
			m_iBtnFlg = BTN_ACTION;
			bDown = false;

			return S_OK;
		}

		if ( (!IsInside(LOWORD(lParam), HIWORD(lParam))) && (bDown) )
			bDown = false;

		break;

	case WM_MOUSEMOVE:
		if ( (IsInside(LOWORD(lParam), HIWORD(lParam)))&& (!m_bDisable))
			m_iBtnFlg = BTN_ON;

		if ( (IsInside(LOWORD(lParam), HIWORD(lParam))) && (bDown))
			m_iBtnFlg = BTN_DOWN;

		if ( ! (IsInside(LOWORD(lParam), HIWORD(lParam))) && (!m_bDisable))
			m_iBtnFlg = BTN_NORMAL;

		break;
	}

	return E_FAIL;
}

BOOL CBasicButton::IsDisable()
{
	return m_bDisable;
}

void CBasicButton::SetDisable( bool dis )
{
	m_bDisable = dis;	// true:disable, false:able
}

void CBasicButton::SetDisable2( bool dis )
{
	m_bDisable = dis;
	if (dis)
	{
		m_iBtnFlg = BTN_DISABLE;
	}
	else
	{
		m_iBtnFlg = BTN_NORMAL;
	}
}

void CBasicButton::LoadRes(int w, int h )
{
	if( !g_pDisplay )
		return;

	DeleteRes();

// 	if (w > 0)
// 	{
// 		g_pDisplay->CreateSurfaceFromBitmap( &m_pNormalSur, m_szBmpFile, w, h);
// 		g_pDisplay->CreateSurfaceFromBitmap( &m_pOnSur, m_szBmpFile2, w, h);
// 		g_pDisplay->CreateSurfaceFromBitmap( &m_pClickSur, m_szBmpFile3, w, h);
// 		g_pDisplay->CreateSurfaceFromBitmap( &m_pDisableSur, m_szBmpFile4, w, h);
// 	}
// 	else
	{
		g_pDisplay->CreateSurfaceFromBitmap( &m_surface, m_bmpfile);
	}

	if( m_surface )
	{
		Width = m_surface->GetWidth();
		Height = m_surface->GetHeight() >> 2;

		m_surface->SetColorKey(TRANS_COLOR);
	}
	else
	{
		char buf[256];
		sprintf( buf , "Not Found Button : %s" , m_bmpfile );
	}
}

BOOL CBasicButton::DrawClip(int nMinArea, int nMaxArea, int nScrollPos)
{
	if (!m_bVisible || !g_pDisplay )
		return FALSE;

	RECT src, dest;

	int top = 0;
	if( m_iBtnFlg == BTN_DOWN )
		top = Height;
	else if( m_iBtnFlg == BTN_ON )
		top = Height*2;
	else if( m_iBtnFlg == BTN_DISABLE || m_bDisable )
		top = Height*3;

	SetRect( &src , 0, top , Width , top+Height);

	dest.left   = PosX;
	dest.top    = PosY;
	dest.right  = PosX + Width;
	dest.bottom = PosY + Height;

	if(dest.top - nScrollPos <= nMinArea &&
			dest.bottom - nScrollPos <= nMinArea)	// ��� �ۿ� ����..
	{
		return FALSE;
	}
	else if(dest.top - nScrollPos <= nMinArea &&		// �� ������ ������.
			dest.bottom - nScrollPos >= nMinArea)
	{
		src.top  += abs((dest.top - nScrollPos) - nMinArea );
		dest.top = nMinArea;
		dest.bottom = (dest.bottom - nScrollPos);
	}
	else if( dest.top - nScrollPos >= nMinArea &&		// ��� ���� �ȿ� ����.
			 dest.bottom - nScrollPos <= nMaxArea/*BOUNDARY_LINE_POSY_TOP*/)
	{
		dest.left   = PosX;
		dest.top    = PosY  - nScrollPos;
		dest.right  = PosX + Width;
		dest.bottom = dest.top + Height;
	}
	else if(dest.top - nScrollPos <= nMaxArea &&
			dest.bottom - nScrollPos >= nMaxArea)	// �Ʒ� ������ ������
	{
		src.bottom  -= abs( nMaxArea - (dest.bottom - nScrollPos) );
		dest.top    = (dest.top- nScrollPos);
		dest.bottom = nMaxArea;
	}
	else if(dest.top - nScrollPos >= nMaxArea &&
			dest.bottom - nScrollPos >= nMaxArea) // �Ʒ����� ������ ��� ������.
	{
		return FALSE;
	}


// 	if ( m_pClickSur && m_iBtnFlg == BTN_DOWN )
// 		g_pDisplay->Blt(dest.left, dest.top, dest.right, dest.bottom, m_pClickSur, &src);
// 	else if ( m_pOnSur && m_iBtnFlg == BTN_ON )
// 		g_pDisplay->Blt(dest.left, dest.top, dest.right, dest.bottom, m_pOnSur, &src);
// 	else if ( m_pNormalSur && m_iBtnFlg == BTN_NORMAL)
// 		g_pDisplay->Blt(dest.left, dest.top, dest.right, dest.bottom, m_pNormalSur, &src);
// 	else if ( m_pDisableSur && m_iBtnFlg == BTN_DISABLE || m_bDisable)
// 		g_pDisplay->Blt(dest.left, dest.top, dest.right, dest.bottom, m_pDisableSur, &src);
// 	else if( m_pNormalSur )
// 		g_pDisplay->Blt(dest.left, dest.top, dest.right, dest.bottom, m_pNormalSur, &src);


	g_pDisplay->Blt( dest.left, dest.top, dest.right, dest.bottom , m_surface , &src );

	return TRUE;
}

BOOL CBasicButton::IsInsideClip(int x, int y, int nMinArea, int nMaxArea, int nScrollPos)
{
	if (!m_bVisible || !g_pDisplay )
		return FALSE;

	RECT src, dest;

	src.left = 0;
	src.top  = 0;
	src.right  = Width;
	src.bottom = Height;

	dest.left   = PosX;
	dest.top    = PosY;
	dest.right  = PosX + Width;
	dest.bottom = PosY + Height;

	if(dest.top - nScrollPos <= nMinArea &&
			dest.bottom - nScrollPos <= nMinArea)	// ��� �ۿ� ����..
	{
		return FALSE;
	}
	else if(dest.top - nScrollPos <= nMinArea &&		// �� ������ ������.
			dest.bottom - nScrollPos >= nMinArea)
	{
		src.top  = abs((dest.top - nScrollPos) - nMinArea );
		dest.top = nMinArea;
		dest.bottom = (dest.bottom - nScrollPos);
	}
	else if(dest.top - nScrollPos >= nMinArea &&		// ��� ���� �ȿ� ����.
			dest.bottom - nScrollPos <= nMaxArea/*BOUNDARY_LINE_POSY_TOP*/)
	{
		dest.left   = PosX;
		dest.top    = PosY  - nScrollPos;
		dest.right  = PosX + Width;
		dest.bottom = dest.top + Height;
	}
	else if(dest.top - nScrollPos <= nMaxArea &&
			dest.bottom - nScrollPos >= nMaxArea)	// �Ʒ� ������ ������
	{
		src.bottom  = abs( nMaxArea - (dest.top - nScrollPos) );
		dest.top    = (dest.top- nScrollPos);
		dest.bottom = nMaxArea;
	}
	else if(dest.top - nScrollPos >= nMaxArea &&
			dest.bottom - nScrollPos >= nMaxArea) // �Ʒ����� ������ ��� ������.
	{
		return FALSE;
	}

	if ( ( ( x > PosX ) && ( x < PosX + Width ) )
			&& ( ( y > dest.top ) && ( y < dest.bottom ) ) )
		return TRUE;
	else
		return FALSE;
}

void CBasicButton::MsgProcClip( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, int nScrollPos, int nMin, int nMax)
{
	switch ( msg )
	{
	case WM_LBUTTONDOWN:
		if ( (IsInsideClip(LOWORD(lParam), HIWORD(lParam), nMin, nMax, nScrollPos))	&& (!m_bDisable) )
		{
			bDown = true;
			m_iBtnFlg = BTN_DOWN;
		}
		break;

	case WM_LBUTTONUP:
		if ( (IsInsideClip(LOWORD(lParam), HIWORD(lParam), nMin, nMax, nScrollPos)) && (bDown) )
		{
			m_iBtnFlg = BTN_ACTION;
			bDown = false;
		}

		if ( (!IsInsideClip(LOWORD(lParam), HIWORD(lParam), nMin, nMax, nScrollPos)) && (bDown) )
			bDown = false;

		break;

	case WM_MOUSEMOVE:
		if ( (IsInsideClip(LOWORD(lParam), HIWORD(lParam), nMin, nMax, nScrollPos)) && (!m_bDisable))
			m_iBtnFlg = BTN_ON;

		if ( (IsInsideClip(LOWORD(lParam), HIWORD(lParam), nMin, nMax, nScrollPos)) && (bDown))
			m_iBtnFlg = BTN_DOWN;

		if ( (!IsInsideClip(LOWORD(lParam), HIWORD(lParam), nMin, nMax, nScrollPos)) && (!m_bDisable))
			m_iBtnFlg = BTN_NORMAL;

		break;
	}
}

void CBasicButton::Draw()
{
	if (!m_bVisible || !g_pDisplay || !m_surface )
		return;

	RECT rc;
	int top = 0;
	SetRect( &rc , 0, 0 , Width , Height );

	if( m_iBtnFlg == BTN_DOWN )
		top = Height;
	else if( m_iBtnFlg == BTN_ON )
		top = Height*2;
	else if( m_iBtnFlg == BTN_DISABLE || m_bDisable )
		top = Height*3;

	SetRect( &rc , 0, top , Width , top+Height);
	g_pDisplay->Blt( PosX, PosY, m_surface , &rc );
}


void CBasicButton::DrawToBlt(BOOL bChecked)
{
	DDBLTFX  ddbltfx;
	ZeroMemory(&ddbltfx, sizeof(ddbltfx));
	ddbltfx.dwSize = sizeof(ddbltfx);
	ddbltfx.ddckSrcColorkey ;

	SetRect(&m_BtnRc ,PosX ,PosY ,PosX+Width ,PosY+Height);

	if ( !m_bVisible || !g_pDisplay || !m_surface )
		return;

	if( g_pDisplay->GetBackBuffer() )
	{
		RECT rc;
		int top = 0;
		SetRect( &rc , 0, 0 , Width , Height );

		if (bChecked)
			top = Height*3;
		else if( m_iBtnFlg == BTN_DOWN )
			top = Height;
		else if( m_iBtnFlg == BTN_ON )
			top = Height*2;
		else if( m_iBtnFlg == BTN_DISABLE || m_bDisable )
			top = Height*3;

		SetRect( &rc , 0, top , Width , top+Height);

		g_pDisplay->GetBackBuffer()->Blt(&m_BtnRc, m_surface->GetDDrawSurface(), &rc, DDBLT_KEYSRC | DDBLT_WAIT, &ddbltfx);
	}
}

void CBasicButton::Draw( int x, int y, BOOL bChecked )
{
	if (!m_bVisible || !g_pDisplay || !m_surface )
		return;

	PosX = x;
	PosY = y;

	RECT rc;
	int top = 0;
	SetRect( &rc , 0, 0 , Width , Height );

	if (bChecked)
		top = Height*3;
	else if ( m_iBtnFlg == BTN_DOWN )
		top = Height;
	else if ( m_iBtnFlg == BTN_ON )
		top = Height*2;
	else if ( m_iBtnFlg == BTN_DISABLE )
		top = Height*3;

	SetRect( &rc , 0, top , Width , top+Height);

	g_pDisplay->Blt( PosX, PosY, m_surface, &rc);
}

///������� �� �߰�
CTabControl::CTabControl()
	: m_pDis(NULL)
{
	m_PosX = 0;
	m_PosY = 0;
	m_TabControlWidth = 0;
	m_TabControlHeight = 0;

	NowDrawStartIndex = 0;
	NowSelectedIndex = 0;

	m_TapAlign = 0; // �������� �������� ����.(0�̸� ����, 1�̸� ����)
	m_TabDrawPosX = 0; // �� �׷����� �����ϴ� ��ǥ.(�� ��Ʈ���� ������ �ְ� ������ ��밡��)
	m_TabDrawPosY = 0; // �� �׷����� �����ϴ� ��ǥ.(�� ��Ʈ���� ������ �ְ� ������ ��밡��)
	m_TabSpace = 0;

	m_TabCount = 0;

	m_TabBtnList.reserve(30);
	m_ListExtraString.reserve(30);
}

CTabControl::~CTabControl()
{
	DeleteRes();
}

void CTabControl::SetPosition( int x, int y, int w, int h )
{
	m_PosX = x;
	m_PosY = y;

	m_TabControlWidth = w;
	m_TabControlHeight = h;
}

void CTabControl::SetTabStyle( int Align, int DrawPosX, int DrawPosY, int Space )
{
	m_TapAlign = Align; // �������� �������� ����.(0�̸� ����, 1�̸� ����)
	m_TabDrawPosX = DrawPosX; // �� �׷����� �����ϴ� ��ǥ.(�� ��Ʈ���� ������ �ְ� ������ ��밡��)
	m_TabDrawPosY = DrawPosY; // �� �׷����� �����ϴ� ��ǥ.(�� ��Ʈ���� ������ �ְ� ������ ��밡��)
	m_TabSpace = Space;
}

void CTabControl::SetIncDecBtn( char* DecFileName, int DecX, int DecY,  char* IncFileName, int IncX, int IncY )
{
	m_DecBtn.SetFileName( DecFileName );
	m_DecBtn.LoadRes();
	m_DecBtn.SetPosition( DecX, DecY );

	m_IncBtn.SetFileName( IncFileName );
	m_IncBtn.LoadRes();
	m_IncBtn.SetPosition( IncX, IncY );
}

void CTabControl::Clear() // �ǹ�ư ����Ʈ�� �� �����.
{
	CBasicButton *pTabBtn;
	vector<CBasicButton *>::iterator pr = m_TabBtnList.begin();
	while( pr != m_TabBtnList.end() )
	{
		pTabBtn = *pr;
		SAFE_DELETE(pTabBtn);
		pr = m_TabBtnList.erase(pr);
	}

	char *pExtraString;
	vector<char *>::iterator pr2 = m_ListExtraString.begin();
	while( pr2 != m_ListExtraString.end() )
	{
		pExtraString = *pr2;
		SAFE_DELETE_ARRAY(pExtraString);
		pr2 = m_ListExtraString.erase(pr2);
	}

	m_TabCount = 0;
}

BOOL CTabControl::AddTab( char* BtnFileName, char* ExString )
{
	CBasicButton *TempBtn = NULL;

	TempBtn = new CBasicButton();

	if( !TempBtn )
		return FALSE;

	TempBtn->SetFileName( BtnFileName );
	TempBtn->LoadRes();

	if( TempBtn->m_surface == NULL )
		return FALSE;

	m_TabBtnList.push_back( TempBtn );

	char *TempString = NULL;

	if( ExString != NULL && strlen(ExString) != 0 )
	{
		TempString = new char[strlen(ExString)+1];

		if( TempString )
		{
			strcpy( TempString, ExString );
			TempString[strlen(ExString)] = '\0';
		}
	}

	m_ListExtraString.push_back( TempString );


	++m_TabCount;

	return TRUE;
}

char* CTabControl::GetExtraString( int Index )
{
	if( Index >= m_ListExtraString.size() || !m_ListExtraString[Index] )
		return NULL;

	return m_ListExtraString[Index];
}


void CTabControl::DeleteRes()
{
	m_DecBtn.DeleteRes();
	m_IncBtn.DeleteRes();

	Clear(); // �� ����Ʈ �����.
}

void CTabControl::RestoreSurfaces()
{
	m_DecBtn.DeleteRes();
	m_IncBtn.DeleteRes();

	m_DecBtn.LoadRes();
	m_IncBtn.LoadRes();

	CBasicButton *pTabBtn;
	vector<CBasicButton *>::iterator pr;

	for( pr = m_TabBtnList.begin() ; pr != m_TabBtnList.end() ; pr++ )
	{
		pTabBtn = *pr;

		pTabBtn->DeleteRes();
		pTabBtn->LoadRes();
	}
}

void CTabControl::Draw()
{
	m_DecBtn.Draw();

	CBasicButton *pTabBtn;
	vector<CBasicButton *>::iterator pr;
	int k = 0;
	int TabDrawPos = 0; // ���� ��¿��� ������ �ȼ� ����ġ

	for( pr = m_TabBtnList.begin() ; pr != m_TabBtnList.end() ; pr++ )
	{
		pTabBtn = *pr;

		pTabBtn->SetDisable( TRUE ); // �ϴ� ���� ��Ȱ��

		if( k >= NowDrawStartIndex ) // �׸��� �ȱ׸��� ���...
		{
			if( k == NowSelectedIndex ) // ���õǾ��� �༮��
				pTabBtn->SetState( BTN_DOWN ); // ������ ���·�...

			if( m_TapAlign == 0 ) // ���� ��
			{
				if( TabDrawPos+pTabBtn->Width < m_TabControlWidth ) // �׸� ��ġ(����ġ)�� ������ ��ġ�� �Ѿ�� �ʾ������� ���
				{
					pTabBtn->Draw( m_PosX+m_TabDrawPosX+TabDrawPos, m_PosY+m_TabDrawPosY );
					pTabBtn->SetDisable( FALSE ); // ���̴� �༮�� Ȱ��.
				}

				TabDrawPos += pTabBtn->Width+m_TabSpace; // ���鵵 ���.
			}
			else // ������
			{
				if( TabDrawPos+pTabBtn->Height < m_TabControlHeight ) // �׸� ��ġ(����ġ)�� ������ ��ġ�� �Ѿ�� �ʾ������� ���
				{
					pTabBtn->Draw( m_PosX+m_TabDrawPosX, m_PosY+m_TabDrawPosY+TabDrawPos );
					pTabBtn->SetDisable( FALSE ); // ���̴� �༮�� Ȱ��.
				}

				TabDrawPos += pTabBtn->Height+m_TabSpace; // ���鵵 ���.
			}
		}

		++k;
	}

	m_IncBtn.Draw();
}

LRESULT CTabControl::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	int x = LOWORD(lParam);
	int y = HIWORD(lParam);

	if( msg != WM_LBUTTONDOWN && msg != WM_LBUTTONUP && msg != WM_MOUSEMOVE )

		if( !IsInside(x,y) )
			return 0;

	if( m_DecBtn.IsInside(x,y) || m_IncBtn.IsInside(x,y) )
	{
		m_DecBtn.MsgProc( hWnd, msg, wParam, lParam );
		m_IncBtn.MsgProc( hWnd, msg, wParam, lParam );

		switch ( msg )
		{
		case WM_LBUTTONDOWN:
			return 1; // ���� ��ư ���ʿ��� Ŭ���� �ٸ� �ǹ�ư�� down ������ �ȵǵ��� �����ش�.

		case WM_LBUTTONUP:

			if( m_DecBtn.GetState() == BTN_ACTION )
			{
				--NowDrawStartIndex;

				if( NowDrawStartIndex < 0 )
					NowDrawStartIndex = 0;

				return 1;
			}
			if( m_IncBtn.GetState() == BTN_ACTION )
			{
				++NowDrawStartIndex;

				if( NowDrawStartIndex >= m_TabCount-1 )
					NowDrawStartIndex = m_TabCount-1; // �ּ��� �ϳ��� ���̰�.

				return 1;
			}

			break;

		case WM_MOUSEMOVE:
			return 1; // ���� ��ư ���ʿ��� Ŭ���� �ٸ� �ǹ�ư�� over ������ �ȵǵ��� �����ش�.
		}

		return 1;
	}

	CBasicButton *pTabBtn;
	vector<CBasicButton *>::iterator pr;
	int k = 0;

	for( pr = m_TabBtnList.begin() ; pr != m_TabBtnList.end() ; pr++ )
	{
		pTabBtn = *pr;

		pTabBtn->MsgProc( hWnd, msg, wParam, lParam );

		switch ( msg )
		{
		case WM_LBUTTONDOWN:
			break;

		case WM_LBUTTONUP:

			if( pTabBtn->GetState()==BTN_ACTION )
			{
				m_TabBtnList[NowSelectedIndex]->SetState( BTN_NORMAL ); // ������ �ٷ� Ǯ��.

				NowSelectedIndex = k; // ���õ� �ε����� ����.

				pTabBtn->SetState( BTN_DOWN ); // ������ ���·�...


				return 1;
			}
			break;

		case WM_MOUSEMOVE:
			break;
		}

		++k;
	}

	return 0;
}

bool CTabControl::IsInside( int x, int y )
{
	if(  (x > m_PosX) && (x < m_PosX+m_TabControlWidth)
			&& (y > m_PosY) && (y < m_PosY+m_TabControlHeight)  )
		return true;
	else
		return false;
}

int CTabControl::GetMouseOnIndex( int x, int y )
{
	CBasicButton *pTabBtn;
	vector<CBasicButton *>::iterator pr;
	int k = 0;

	for( pr = m_TabBtnList.begin() ; pr != m_TabBtnList.end() ; pr++ )
	{
		pTabBtn = *pr;

		if(  (x > pTabBtn->PosX) && ( x < pTabBtn->PosX+pTabBtn->Width )
				&& (y > pTabBtn->PosY) && ( y < pTabBtn->PosY+pTabBtn->Height )   )
			return k;

		++k;
	}

	return -1;
}
