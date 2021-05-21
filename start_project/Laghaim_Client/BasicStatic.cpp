#include "stdafx.h"
#include "BasicStatic.h"
#include "headers.h"



CBasicStatic::CBasicStatic()
	:	m_surface(NULL)
	,	m_x(0)
	,	m_y(0)
	,	m_w(0)
	,	m_h(0)
	,	m_scaleX(1.0f)
	,	m_scaleY(1.0f)
	,	m_visible(true)
{
	memset(m_bmpfile, 0, sizeof(m_bmpfile));
}

CBasicStatic::CBasicStatic(const char* filename, int w, int h)
	:	m_surface(NULL)
	,	m_x(0)
	,	m_y(0)
	,	m_w(0)
	,	m_h(0)
	,	m_scaleX(1.0f)
	,	m_scaleY(1.0f)
	,	m_visible(true)
{
	SetFilename( filename );
	LoadRes( w, h );
}


CBasicStatic::~CBasicStatic()
{
	DeleteRes();
}

void CBasicStatic::Init()
{
}

void CBasicStatic::Init(const char* filename, int w, int h)
{
	DeleteRes();

	SetFilename( filename );
	LoadRes( w, h );
}

void CBasicStatic::Release()
{
}

void CBasicStatic::SetFilename(const char* filename)
{
	strcpy( m_bmpfile, "interface/" );
	strcat( m_bmpfile, filename );
	strcat( m_bmpfile, ".bmp" );
}

void CBasicStatic::LoadRes(int w, int h)
{
	if( !g_pDisplay )
		return;

	if( w > 0 )
	{
		if( FAILED( g_pDisplay->CreateSurfaceFromBitmap( &m_surface , m_bmpfile , w , h ) ) )
		{
			//			OutLog( m_bmpfile );
			return;
		}
	}
	else
	{
		if( FAILED( g_pDisplay->CreateSurfaceFromBitmap( &m_surface , m_bmpfile ) ) )
		{
			//			OutLog( m_bmpfile );
			return;
		}
	}

	if( m_surface )
	{
		m_surface->SetColorKey( TRANS_COLOR );
		m_w = m_surface->GetWidth();
		m_h = m_surface->GetHeight();

		UpdateTransform();
	}
}

void CBasicStatic::DeleteRes()
{
	SAFE_DELETE(m_surface);
}

void CBasicStatic::Draw()
{
	if( !m_visible )
		return;

	if( FAILED( g_pDisplay->Blt( m_x, m_y , m_surface ) ) )
	{
		int x;
		x=0;
	}
}

void CBasicStatic::Draw(int x, int y)
{
	if( !m_visible )
		return;

	if( FAILED( g_pDisplay->Blt( x, y , m_surface ) ) )
	{
		int x;
		x=0;
	}
}

void CBasicStatic::Draw(float ratio,int dir)
{
	if( !m_visible )
		return;

	RECT rc;

	int x , y;

	x=m_x;
	y=m_y;

	switch(dir)
	{
	case 0: // up->down
		{
			y = m_y + ( m_h - m_h * ratio );
			SetRect( &rc , 0 , m_h-(m_h*ratio) , m_w , m_h );
		}
		break;
	case 1: // down->up
		{
			SetRect( &rc , 0 , 0 , m_w , m_h*ratio );
		}
		break;
	case 2: // left - > right
		{
			x = m_x + ( m_w - m_w * ratio );
			SetRect( &rc , m_w-(m_w * ratio) , 0 , m_w , m_h );
		}
		break;
	case 3: // right -> left
		{
			// x = m_x + ( m_w - m_w * ratio );
			SetRect( &rc , 0 , 0 , m_w * ratio , m_h );
		}
		break;
	}

	g_pDisplay->Blt( x , y , m_surface , &rc );
}

void CBasicStatic::DrawToBlt(bool checked)
{
	if( !m_visible || !g_pDisplay )
		return;

	DDBLTFX  ddbltfx;
	ZeroMemory(&ddbltfx, sizeof(ddbltfx));
	ddbltfx.dwSize = sizeof(ddbltfx);
	ddbltfx.ddckSrcColorkey ;

	SetRect(&m_rcThis
			,(int) (m_x*m_scaleX)
			,(int) (m_y*m_scaleY)
			,(int) ((m_x+m_w)*m_scaleX)
			,(int) ((m_y+m_h)*m_scaleY));

	if( FAILED ( g_pDisplay->GetBackBuffer()->Blt(&m_rcThis, m_surface->GetDDrawSurface(), &m_rcThis, DDBLT_KEYSRC | DDBLT_WAIT, &ddbltfx) ))
	{
	}
}

void CBasicStatic::UpdateTransform()
{
	SetRect( &m_rcThis , m_x , m_y , m_x+m_w , m_y+m_h );
}

void CBasicStatic::Align(int align_type , int w, int h)
{
	int nx = 0;
	int ny = 0;

	switch(align_type)
	{
	case 0: // bottom
		{
			nx = (w - m_w)>>1;
			ny = h - m_h;
		}
		break;
	case 1: // top
		{
			nx = (w - m_w)>>1;
			ny = 0;
		}
		break;
	case 2: // top left
		{
			nx = 0;
			ny = 0;
		}
		break;
	case 3: // top right
		{
			nx = w - m_w;
			ny = 0;
		}
		break;
	case 4: // right
		{
			nx = w - m_w;
			ny = (h - m_h)>>1;
		}
		break;
	case 5: // left
		{
			nx = 0;
			ny = (h - m_h)>>1;
		}
		break;
	case 6: // center
		{
			nx = (w - m_w)>>1;
			ny = (h - m_h)>>1;
		}
		break;
	case 7: // bottom right
		{

		}
		break;
	case 8: // bottom left
		{

		}
		break;
	}

	SetPos( nx , ny );
}

CBasicStaticEx::CBasicStaticEx()
	:	m_x(0)
	,	m_y(0)
	,	m_h(0)
	,	m_w(0)
	,	m_num_col(1)
	,	m_num_row(1)
	,	m_current(0)
	,	m_surface(NULL)
{
	memset(m_bmpfile, 0, sizeof(m_bmpfile));
}

CBasicStaticEx::~CBasicStaticEx()
{
	DeleteRes();
}

void CBasicStaticEx::Init(const char* filename, int col, int row)
{
	SetFilename( filename );
	LoadRes();

	m_num_col = col;
	m_num_row = row;

	UpdateTransform();
	UpdateTransformImg();
}

void CBasicStaticEx::InitEx(int col , int row)
{
	m_num_col = col;
	m_num_row = row;
}

void CBasicStaticEx::LoadRes()
{
	if( FAILED( g_pDisplay->CreateSurfaceFromBitmap( &m_surface , m_bmpfile ) ) )
		return;

	m_surface->SetColorKey( TRANS_COLOR );
	m_w = m_surface->GetWidth();
	m_h = m_surface->GetHeight();
}

void CBasicStaticEx::DeleteRes()
{
	SAFE_DELETE(m_surface);
}

void CBasicStaticEx::SetFilename(const char* filename)
{
	strcpy( m_bmpfile, "interface/" );
	strcat( m_bmpfile, filename );
	strcat( m_bmpfile, ".bmp" );
}

void CBasicStaticEx::Draw()
{
	g_pDisplay->Blt( m_x, m_y , m_surface , &m_rcDraw );
}

void CBasicStaticEx::Draw(int x, int y)
{
	g_pDisplay->Blt( x, y , m_surface , &m_rcDraw );
}

void CBasicStaticEx::Draw(int index)
{
	if( m_current != index )
	{
		m_current = index;
		UpdateTransformImg();
	}

	g_pDisplay->Blt( m_x, m_y , m_surface , &m_rcDraw );
}

void CBasicStaticEx::Draw(int x, int y, int index)
{
	if( m_current != index )
	{
		m_current = index;
		UpdateTransformImg();
	}
	g_pDisplay->Blt( x, y , m_surface , &m_rcDraw );
}

void CBasicStaticEx::SetPos(int x, int y)
{
	m_x = x;
	m_y = y;
	UpdateTransform();
}

bool CBasicStaticEx::IsIn(int x, int y)
{
	POINT pt = {x,y};
	return PtInRect( &m_rcThis , pt)?true:false;
}

void CBasicStaticEx::UpdateTransform()
{
	SetRect( &m_rcThis, m_x , m_y , m_x+m_w/m_num_col , m_y+m_h/m_num_row );
}

void CBasicStaticEx::UpdateTransformImg()
{
	if( m_current >= m_num_col*m_num_row )
		return;

	int w = m_w / m_num_col;
	int h = m_h / m_num_row;

	SetRect( &m_rcDraw , w * (m_current % m_num_col) , h * ( m_current / m_num_col ) ,
			 w * (m_current % m_num_col) + w , h * ( m_current / m_num_col ) + h);
}