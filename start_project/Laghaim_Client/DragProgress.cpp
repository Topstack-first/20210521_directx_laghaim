#include "stdafx.h"
#include "DragProgress.h"



#define TRANS_COLOR			RGB(0, 0, 0)
#define BAR_NOR		11
#define BAR_OVER	12
#define BAR_CLICK   13
#define BAR_DIS     14


CDragProgress::CDragProgress()
{
	memset(m_Fn_Progress, 0, sizeof(m_Fn_Progress));
	memset(m_Fn_Progress_gray, 0, sizeof(m_Fn_Progress_gray));
	memset(m_Fn_DragBar, 0, sizeof(m_Fn_DragBar));
	memset(m_Fn_DragBar_Nor, 0, sizeof(m_Fn_DragBar_Nor));
	memset(m_Fn_DragBar_Over, 0, sizeof(m_Fn_DragBar_Over));
	memset(m_Fn_DragBar_Click, 0, sizeof(m_Fn_DragBar_Click));
	memset(m_Fn_DragBar_Dis, 0, sizeof(m_Fn_DragBar_Dis));

	m_pProgress		= NULL;
	m_pProgress_gray= NULL;
	m_pDragBar_Nor	= NULL;
	m_pDragBar_Over = NULL;
	m_pDragBar_Click= NULL;
	m_pDragBar_Dis  = NULL;
	m_bVisible      = TRUE;
	m_bBarClick     = FALSE; // 드레그바를 마우스로 클릭했는가?
	m_CurBarLevel     = 50;    // 드레그바의 현제위치를 정한다
	m_CurBarState   = BAR_NOR;
}

CDragProgress::~CDragProgress()
{
	DeleteRes();
}

void CDragProgress::Init(char* fn_progress, char* fn_dragbar)
{
	strcpy( m_Fn_Progress, "interface/" );			// 프로그래스 파일이름
	strcat( m_Fn_Progress, fn_progress );
	strcat( m_Fn_Progress, ".bmp" );

	strcpy( m_Fn_Progress_gray, "interface/" );		// 그레이처리된 프로그래스 파일이름
	strcat( m_Fn_Progress_gray, fn_progress );
	strcat( m_Fn_Progress_gray, "_gray.bmp" );

	strcpy( m_Fn_DragBar_Nor, "interface/" );		// 드레그버튼 "노멀"
	strcat( m_Fn_DragBar_Nor, fn_dragbar );
	strcat( m_Fn_DragBar_Nor, "_normal.bmp" );

	strcpy( m_Fn_DragBar_Over, "interface/" );		// 그레그버튼 "오버"
	strcat( m_Fn_DragBar_Over, fn_dragbar );
	strcat( m_Fn_DragBar_Over, "_over.bmp" );

	strcpy( m_Fn_DragBar_Click, "interface/" );		// 그레그버튼 "클릭"
	strcat( m_Fn_DragBar_Click, fn_dragbar );
	strcat( m_Fn_DragBar_Click, "_click.bmp" );

	strcpy( m_Fn_DragBar_Dis, "interface/" );		// 그레그버튼 "클릭"
	strcat( m_Fn_DragBar_Dis, fn_dragbar );
	strcat( m_Fn_DragBar_Dis, "_unable.bmp" );
}


void CDragProgress::LoadRes(int xpos, int ypos, int pw, int ph, int dw, int dh)
{
	if(!g_pDisplay)
		return;

	if(pw >0 || ph > 0)
	{
		g_pDisplay->CreateSurfaceFromBitmap(&m_pProgress, m_Fn_Progress, pw, ph);
		g_pDisplay->CreateSurfaceFromBitmap(&m_pProgress_gray, m_Fn_Progress, pw, ph);
	}
	else
	{
		g_pDisplay->CreateSurfaceFromBitmap(&m_pProgress, m_Fn_Progress);
		g_pDisplay->CreateSurfaceFromBitmap(&m_pProgress_gray, m_Fn_Progress);
	}
	if(dw>0 || dh>0)
	{
		g_pDisplay->CreateSurfaceFromBitmap(&m_pDragBar_Nor, m_Fn_DragBar_Nor, dw, dh);
		g_pDisplay->CreateSurfaceFromBitmap(&m_pDragBar_Over, m_Fn_DragBar_Over, dw, dh);
		g_pDisplay->CreateSurfaceFromBitmap(&m_pDragBar_Click, m_Fn_DragBar_Click, dw, dh);
		g_pDisplay->CreateSurfaceFromBitmap(&m_pDragBar_Dis, m_Fn_DragBar_Dis, dw, dh);
	}
	else
	{
		g_pDisplay->CreateSurfaceFromBitmap(&m_pDragBar_Nor, m_Fn_DragBar_Nor);
		g_pDisplay->CreateSurfaceFromBitmap(&m_pDragBar_Over, m_Fn_DragBar_Over);
		g_pDisplay->CreateSurfaceFromBitmap(&m_pDragBar_Click, m_Fn_DragBar_Click);
		g_pDisplay->CreateSurfaceFromBitmap(&m_pDragBar_Dis, m_Fn_DragBar_Dis);
	}


	if( m_pProgress )
		m_pProgress->SetColorKey(TRANS_COLOR);

	if(m_pProgress_gray)
		m_pProgress_gray->SetColorKey(TRANS_COLOR);
	else
		m_pProgress_gray = m_pProgress;


	if( m_pDragBar_Nor )
		m_pDragBar_Nor->SetColorKey(TRANS_COLOR); // 이건 반드시 있어야 한다. 없으면 에러가 나야 당연...

	if (m_pDragBar_Over)
		m_pDragBar_Over->SetColorKey(TRANS_COLOR);
	else
		m_pDragBar_Over = m_pDragBar_Nor;

	if (m_pDragBar_Click)
		m_pDragBar_Click->SetColorKey(TRANS_COLOR);
	else
		m_pDragBar_Click = m_pDragBar_Nor;

	if (m_pDragBar_Dis)
		m_pDragBar_Dis->SetColorKey(TRANS_COLOR);
	else
		m_pDragBar_Dis = m_pDragBar_Nor;

	SetPos(xpos, ypos);

	if( m_pDragBar_Nor && m_pProgress )
	{
		m_pDragBar_Nor->Xpos = m_pProgress->Xpos+ (m_CurBarLevel * (m_pProgress->GetWidth())) / (100) - m_pDragBar_Nor->GetWidth();
		m_pDragBar_Nor->Ypos = m_pProgress->Ypos+(m_pProgress->GetHeight() / 2) -(m_pDragBar_Nor->GetHeight()/2) +1;
	}
}

void CDragProgress::DeleteRes()
{
	if(m_pProgress_gray && m_pProgress != m_pProgress_gray)
		SAFE_DELETE(m_pProgress_gray);
	if(m_pProgress)
		SAFE_DELETE(m_pProgress);

	if (m_pDragBar_Over && m_pDragBar_Nor != m_pDragBar_Over)
		delete m_pDragBar_Over;
	if (m_pDragBar_Click && m_pDragBar_Nor != m_pDragBar_Click)
		delete m_pDragBar_Click;
	if (m_pDragBar_Dis && m_pDragBar_Nor != m_pDragBar_Dis)
		delete m_pDragBar_Nor;
	if (m_pDragBar_Nor)
		delete m_pDragBar_Nor;
}


void CDragProgress::Draw()
{
	if (!m_bVisible)
		return;
	DrawProgress();
	DrawBar();
}


void CDragProgress::DrawProgress()
{
	if( !g_pDisplay || !m_pDragBar_Nor || !m_pProgress )
		return;

	RECT rc;
	SetRect(&rc, 0, 0, m_pDragBar_Nor->Xpos - m_pProgress->Xpos+m_pDragBar_Nor->GetWidth(), m_pProgress->GetHeight());

	if(m_pDragBar_Nor->Xpos != m_pProgress->Xpos)
		g_pDisplay->Blt(m_pProgress->Xpos , m_pProgress->Ypos, m_pProgress, &rc);
	else
		g_pDisplay->Blt(m_pProgress->Xpos , m_pProgress->Ypos, m_pProgress_gray, &rc);
}



void CDragProgress::DrawBar()
{
	if( !m_pDragBar_Nor || !g_pDisplay )
		return;

	int x = m_pDragBar_Nor->Xpos;
	int y = m_pDragBar_Nor->Ypos;

	if ( m_CurBarState == BAR_CLICK )
		g_pDisplay->Blt( x, y, m_pDragBar_Click, NULL);
	else if ( m_CurBarState == BAR_OVER )
		g_pDisplay->Blt( x, y, m_pDragBar_Over, NULL);
	else if ( m_CurBarState == BAR_DIS || m_bVisible == FALSE)
		g_pDisplay->Blt( x, y, m_pDragBar_Dis, NULL);
	else
		g_pDisplay->Blt( x, y, m_pDragBar_Nor, NULL);
}



LRESULT CDragProgress::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	int mouse_x = LOWORD (lParam);
	int mouse_y = HIWORD (lParam);

	switch ( msg )
	{
	case WM_LBUTTONDOWN:
		if(IsInside(m_pProgress, LOWORD (lParam), HIWORD (lParam)))//마우스로 찍었을때 이동처리
		{
			if(m_pDragBar_Nor->Xpos+m_pDragBar_Nor->GetWidth() < (DWORD)mouse_x)
				m_pDragBar_Nor->Xpos  = mouse_x - m_pDragBar_Nor->GetWidth();
			if(m_pDragBar_Nor->Xpos > mouse_x)
				m_pDragBar_Nor->Xpos  = mouse_x;
		}
		if(IsInside(m_pDragBar_Nor, LOWORD (lParam), HIWORD (lParam)))
		{
			m_bBarClick = TRUE;  // 드레그바를 마우스로 누른상태
			m_CurBarState = BAR_CLICK;
		}

		break;

	case WM_LBUTTONUP:
		if(m_bBarClick)
		{
			m_bBarClick = FALSE; // 눌렀다가 뗀 상태
			m_CurBarState = BAR_OVER;
		}
		break;

	case WM_MOUSEMOVE:
		if(IsInside(m_pDragBar_Nor, LOWORD (lParam), HIWORD (lParam)))
			m_CurBarState = BAR_OVER;

		if(m_bBarClick && m_pDragBar_Nor && m_pProgress)
		{
			m_pDragBar_Nor->Xpos    = mouse_x - m_pDragBar_Nor->GetWidth()/2;
			if(m_pDragBar_Nor->Xpos+m_pDragBar_Nor->GetWidth() > m_pProgress->Xpos+m_pProgress->GetWidth())
				m_pDragBar_Nor->Xpos = m_pProgress->Xpos+m_pProgress->GetWidth()-m_pDragBar_Nor->GetWidth();
			if(m_pDragBar_Nor->Xpos < m_pProgress->Xpos)
				m_pDragBar_Nor->Xpos  = m_pProgress->Xpos;

			m_CurBarState = BAR_OVER;
			return 1;
		}
		else
		{
			m_CurBarState = BAR_NOR;
			return 0;
		}
		break;
	}

	if( m_pDragBar_Nor && m_pProgress )
		m_CurBarLevel = (((m_pDragBar_Nor->Xpos+m_pDragBar_Nor->GetWidth()) - m_pProgress->Xpos )*100) / (m_pProgress->GetWidth());

	return 0;
}

BOOL CDragProgress::IsInside(CSurface* surface, int x, int y)
{
	if (surface == NULL)
		return FALSE;

	if ( ( ( x > surface->Xpos ) && ( (DWORD)x < (surface->Xpos + surface->GetWidth())))
			&& ( ( y > surface->Ypos ) && ( (DWORD)y < (surface->Ypos + surface->GetHeight()))))
		return true;

	else
		return false;
}

void CDragProgress::SetPos(int x, int y)
{
	if( m_pProgress )
	{
		m_pProgress->Xpos = x;
		m_pProgress->Ypos = y;
	}
}
