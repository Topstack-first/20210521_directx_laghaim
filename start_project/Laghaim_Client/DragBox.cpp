#include "stdafx.h"
#include "ddutil.h"
#include "headers.h"
#include "DragBox.h"

#define TERMINATOR(x)	if ( x != NULL ) {	delete x ;	x = NULL ; }

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDragBox::CDragBox()
	: m_BarFn(NULL)
	, m_BtnNormalFn(NULL)
	, m_BtnClickFn(NULL)
	, m_Width(0)
	, m_DBtnX(0)
	, m_DBtnY(0)
	, m_DBtnWidth(0)
	, m_DBtnHeight(0)
	, m_DBarX(0)
	, m_DBarY(0)
	, m_DBarWidth(0)
	, m_DBarHeight(0)
	, m_DragBarXpos(0)
{
	m_pDBarSurf = NULL;
	m_pDBtnNormal = NULL;
	m_pDBtnClick = NULL;

	SetPos(0, 0);
	m_MinPos = 0;
	m_MaxPos = 1;
	m_CurPos = 0;
	m_DisablePos = 0;

	m_bButtonDown = FALSE;
}

CDragBox::~CDragBox()
{
	DeleteRes();
}

void CDragBox::DeleteRes()
{
	TERMINATOR(m_pDBarSurf);
	TERMINATOR(m_pDBtnNormal);
	TERMINATOR(m_pDBtnClick);
}

void CDragBox::Init(int pos_x, int pos_y, int width, char* BtnNormalFn, char* BtnClickFn, char * BarFn)
{
	m_Width			= width;		// 움직일 수 있는 전체 영역의 넓이
	m_BarFn			= BarFn;		// DragBar FileName
	m_BtnNormalFn	= BtnNormalFn;	// DragBtnNormal FileName
	m_BtnClickFn	= BtnClickFn;	// DragBtnClick FileName
	SetPos(pos_x, pos_y);			// m_PosX, m_PosY 의 수치를 설정한다
}

void CDragBox::LoadRes()
{
	if( !g_pDisplay )
		return;

	DeleteRes();
	//______________________________________________ DragBar
	g_pDisplay->CreateSurfaceFromBitmap( &m_pDBarSurf, m_BarFn);
	m_DBarWidth  = m_pDBarSurf->GetWidth();
	m_DBarHeight = m_pDBarSurf->GetHeight();
	m_DBarX = m_PosX;
	m_DBarY = m_PosY - (m_DBarHeight / 2);
	if( m_pDBarSurf )
		m_pDBarSurf->SetColorKey(RGB(0, 0, 0));


	//______________________________________________ DragBtn
	g_pDisplay->CreateSurfaceFromBitmap( &m_pDBtnNormal, m_BtnNormalFn);
	g_pDisplay->CreateSurfaceFromBitmap( &m_pDBtnClick,	 m_BtnClickFn);
	m_DBtnWidth  = m_pDBtnNormal->GetWidth();
	m_DBtnHeight = m_pDBtnNormal->GetHeight();
	if( m_pDBtnNormal )
		m_pDBtnNormal->SetColorKey(RGB(0, 0, 0));
	if( m_pDBtnClick )
		m_pDBtnClick->SetColorKey(RGB(0, 0, 0));

	CalcPos();
}


void CDragBox::SetValues(int min_pos, int max_pos, int cur_pos, int disable_pos)
{
	m_MinPos = min_pos;
	m_MaxPos = max_pos;
	m_CurPos = cur_pos;
	if (disable_pos == -1)
		m_DisablePos = max_pos;
	else
		m_DisablePos = disable_pos;
}

void CDragBox::SetCurPos(int cur_pos, int disable_pos)
{
	if (disable_pos != -1)
	{
		m_DisablePos = disable_pos;
	}

	if (cur_pos < m_MinPos || cur_pos > m_DisablePos)
		return;

	m_CurPos = cur_pos;

	CalcPos();
}


void CDragBox::SetDragBar(int Xpos)
{
	m_DragBarXpos = Xpos;
}


void CDragBox::MoveButton(int x, int y)
{
	// 버튼이미지의 중간을 마우스 포인터로 드래그 한다
	m_DBtnX = x-(m_DBtnWidth/2);

	// 현재 dragbutton의 위치가 "최대치 값" 보다 클 수 없다!!
	if(m_DBtnX+m_DBtnWidth  >= m_DragBarXpos)
		m_DBtnX =m_DragBarXpos;

	if (m_DBtnX > m_PosX + m_Width)
		m_DBtnX = m_PosX + m_Width ;
	if (m_DBtnX< m_PosX)
		m_DBtnX = m_PosX;

	CalcValues();
}


void CDragBox::CalcPos()
{
	m_DBtnX = m_PosX + (m_Width * (m_CurPos - m_MinPos) / (m_MaxPos - m_MinPos));
	m_DBtnY = m_PosY - (m_DBtnHeight / 2);
}

void CDragBox::CalcValues()
{
	m_CurPos = (m_DBtnX - m_PosX) * (m_MaxPos-m_MinPos) / m_Width + m_MinPos;

	if (m_CurPos > m_DisablePos)
	{
		m_CurPos = m_DisablePos;
		CalcPos();
	}
}

void CDragBox::Draw()
{
	if (!g_pDisplay || !m_pDBtnNormal || !m_pDBtnClick)
		return;

	if (m_bButtonDown)
		g_pDisplay->Blt( m_DBtnX, m_DBtnY, m_pDBtnClick, NULL);
	else
		g_pDisplay->Blt( m_DBtnX, m_DBtnY, m_pDBtnNormal, NULL);

	DrawBar();
}


void CDragBox::DrawBar()
{
	// 만약 "DragBar"의 놓여야할 xpos가 "<---------------->" <-- 범위의 크기보다 클때에는 출력하지 않는다
	if(m_DragBarXpos > m_PosX+m_Width)
		return;

	if( g_pDisplay && m_pDBtnClick )
		g_pDisplay->Blt( m_DragBarXpos+m_pDBtnClick->GetWidth()-4, m_DBarY, m_pDBarSurf, NULL);

}

BOOL CDragBox::IsInside(int x, int y)
{
	if (x >= m_DBtnX && x < m_DBtnX + m_DBtnWidth &&
			y >= m_DBtnY && y < m_DBtnY + m_DBtnHeight)
		return TRUE;

	return FALSE;
}

int CDragBox::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch ( uMsg)
	{
	case WM_LBUTTONDOWN:
		if (IsInside(LOWORD(lParam), HIWORD(lParam)))
			m_bButtonDown = TRUE;
		else
			m_bButtonDown = FALSE;

		break;

	case WM_LBUTTONUP:
		if (IsInside(LOWORD(lParam), HIWORD(lParam)))
		{
		}

		m_bButtonDown = FALSE;

		break;

	case WM_MOUSEMOVE:
		if (m_bButtonDown)
		{
			MoveButton(LOWORD(lParam), HIWORD(lParam));
			return GetCurPos();
		}

	default:
		break;
	}

	return -1;
}
