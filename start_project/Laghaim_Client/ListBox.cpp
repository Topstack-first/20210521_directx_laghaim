#include "stdafx.h"
#include "ListBox.h"
#include "dxutil.h"

#include "textbox.h"
#include "main.h"
#include "LoadEffectSound.h"



extern char g_strFontName[];


//------------------------------------------------------------
//	Nams : ListBox ()
//	Desc :	������
//------------------------------------------------------------
//
CListBox::CListBox()
	: m_MaxTitle(0)
	, m_MaxLine(0)
	, state(NONE)
	, TitleColor(0)
	, TitleBkColor(0)
	, TextColor(0)
	, TextBkColor(0)
	, fnDecrementBtn(NULL)
	, fnIncrementBtn(NULL)
	, ScrollPosX(0)
	, DecBtnPosY(0)
	, IncBtnPosY(0)
	, ScrollBarPosY(0)
	, Hfont(NULL)
	, TotalLine(0)
	, CurLine(0)
	, SelectedLine(0)
	, LineHeight(0)
	, MaxVisibleLine(0)
{
	memset(Title, 0, sizeof(Title));
	memset(LineText, 0, sizeof(LineText));
	memset(m_pTextSurf, 0, sizeof(m_pTextSurf));

	// ��ư �����̸�
	fnScrollBar = NULL;
	// ��ư�� ���� SURFACE
	ScrollBarSurface = NULL;
}

CListBox::CListBox (char *title)
	: fnDecrementBtn(NULL)
	, fnIncrementBtn(NULL)
{
	m_MaxTitle = 40;

	m_MaxLine = 50;

	PosX = 100;
	PosY = 100;
	Width = 200;
	Height = 200;
	InitVar (title);
	SetFont (5, 9, g_strFontName);
}


CListBox::CListBox (char *title, int x, int y, int w, int h)
	: CBaseUI (x, y, w, h)
	, fnDecrementBtn(NULL)
	, fnIncrementBtn(NULL)
{
	m_MaxTitle = 40;

	m_MaxLine = 50;

	InitVar (title);
	SetFont (0, 9, g_strFontName);
}

CListBox::CListBox (char *title, int x, int y, int w, int h, int flag)
	: CBaseUI (x, y, w, h)
	, m_MaxLine(0)
	, TitleColor(0)
	, TitleBkColor(0)
	, fnDecrementBtn(NULL)
	, fnIncrementBtn(NULL)
{
	memset(Title, 0, sizeof(Title));

	m_MaxTitle = 50;

	state = NONE;
	fnScrollBar = NULL;
	ScrollBarSurface = NULL;

	for (int i = 0; i < 50 /*m_MaxLine*/; i++)
	{
		LineText[i] = NULL;
		m_pTextSurf[i] = NULL;
	}

	CurLine = 0;
	SelectedLine = -1;
	TotalLine = 0;
	LineHeight = 20;
	MaxVisibleLine = (Height-BOARDER_OFF*2-TITLE_H-CONTROL_OFF) / LineHeight;

	// Scroll Bar, Btn Position
	ScrollPosX = PosX+Width-17-BOARDER_OFF; //��ũ�ѹ�ư ũ�� 16,7
	DecBtnPosY = PosY+TITLE_H+BOARDER_OFF;
	IncBtnPosY = PosY+Height-8-BOARDER_OFF;
	ScrollBarPosY = DecBtnPosY+SCROLLBTN_H;
	SetFont (0, 9, g_strFontName);
}

//-- 2004.03.15 - �����Ϸ�
//-- Lyul
//-- m_Font �� handle �� �����´�. ���� �ı��� �ʿ䰡 ����.
CListBox :: ~CListBox ()
{
	DeleteRes();

	for (int i = 0; i < 50 /*m_MaxLine*/; i++)
		SAFE_DELETE(LineText[i]);

	SAFE_DELETE_ARRAY( fnScrollBar );
}



//------------------------------------------------------------
//	Name : InitVar ()
//	Desc :	��� ���� �ʱ�ȭ...
//------------------------------------------------------------
//
void	CListBox :: InitVar (char *title)
{
	state = NONE;

	if (strlen(title)+1 <= m_MaxTitle)
		memcpy (Title, title, strlen(title)+1);
	else
	{
		memcpy (Title, title, m_MaxTitle-1);
		Title[m_MaxTitle-1] = '\0';
	}

	fnScrollBar = NULL;
	ScrollBarSurface = NULL;

	// List Box Content Text
	for (int i = 0; i < 50 /*m_MaxLine*/; i++)
	{
		LineText[i] = NULL;
		m_pTextSurf[i] = NULL;
	}

	// Color Member / Font Member
	Hfont = NULL;
	TitleColor = RGB(255, 255, 255);
	TitleBkColor = 0;
	TextColor = RGB(228, 203, 156);
	TextBkColor = 0;

	//
	CurLine = 0;
	SelectedLine = -1;
	TotalLine = 0;
	LineHeight = 20;
	MaxVisibleLine = (Height-BOARDER_OFF*2-TITLE_H-CONTROL_OFF) / LineHeight;

	// Scroll Bar, Btn Position
	ScrollPosX = PosX+Width-SCROLLBTN_W-BOARDER_OFF;
	DecBtnPosY = PosY+TITLE_H+BOARDER_OFF;
	IncBtnPosY = PosY+Height-SCROLLBTN_H-BOARDER_OFF;
	ScrollBarPosY = DecBtnPosY+SCROLLBTN_H;
}



//------------------------------------------------------------
//	Name : SetPosDim ()
//	Desc :	Position, Dimension Set  (BaseUI class fn. override)
//			ListBox�� ��ġ�� ũ�⸦ �����Ų��.
//------------------------------------------------------------
void CListBox :: SetPosDim (const int x, const int y, const int w, const int h)
{
	PosX = x;
	PosY = y;
	Width = w;
	Height = h;

	// �� ȭ�鿡 ������ ���� �� �� ���
	MaxVisibleLine = (Height-BOARDER_OFF*2-TITLE_H-CONTROL_OFF) / LineHeight;

	// Scroll Bar, Btn Position �� ���
// 	ScrollPosX = PosX+Width-SCROLLBTN_W-BOARDER_OFF;
// 	DecBtnPosY = PosY+TITLE_H+BOARDER_OFF;
// 	IncBtnPosY = PosY+Height-SCROLLBTN_H-BOARDER_OFF;
// 	ScrollBarPosY = PosY+TITLE_H+BOARDER_OFF+SCROLLBTN_H;

	// by evilkiki 2009.08.27
	ScrollPosX = PosX+Width-SCROLLBTN_W;
	DecBtnPosY = PosY+TITLE_H;
	IncBtnPosY = PosY+Height-SCROLLBTN_H;
	ScrollBarPosY = PosY+TITLE_H+SCROLLBTN_H;

// 	ScrollPosX = PosX+Width-m_DecBtn.Width;
// 	DecBtnPosY = PosY+TITLE_H;
// 	IncBtnPosY = PosY+Height-m_IncBtn.Height;
// 	ScrollBarPosY = PosY+TITLE_H+m_DecBtn.Height;
}

//---------------------------------------------------------
//		 ���� �� ���� ó�� �ΰ��� ���ϸ��� ��ư ��������
//		 ������ ���ϸ��� ��α��� ���°� ������ �ʵȴ�
//----------------------------------------------------------
void CListBox::SetImage (char *DecBtn, char *IncBtn, char *ScrollBar)
{
	if( DecBtn == NULL || IncBtn == NULL || ScrollBar == NULL )
		return;

	int len = strlen (ScrollBar);
	fnScrollBar = new char [len+1];
	memcpy (fnScrollBar, ScrollBar, len+1);

	m_IncBtn.SetFileName(IncBtn);
//	m_IncBtn.SetSize(SCROLLBTN_W, SCROLLBTN_H);
	m_IncBtn.SetDisable( false );

	m_DecBtn.SetFileName(DecBtn);
//	m_DecBtn.SetSize(SCROLLBTN_W, SCROLLBTN_H);
	m_DecBtn.SetDisable( false );
}



void CListBox::LoadRes()
{
	if( !g_pDisplay )
		return;

	m_IncBtn.LoadRes();
	m_DecBtn.LoadRes();

	g_pDisplay->CreateSurfaceFromBitmap (&ScrollBarSurface, fnScrollBar, 0, 0);
	ScrollBarSurface->SetColorKey((RGB(0, 0, 0)));
	for (int i = 0; i < m_MaxLine; i++)
	{
		if (LineText[i])
		{
			g_pDisplay->CreateSurfaceFromText(&m_pTextSurf[i], Hfont, LineText[i], RGB(0, 0, 1), TextColor);
			if( m_pTextSurf[i] )
				m_pTextSurf[i]->SetColorKey(RGB(0, 0, 1));
		}
	}
}

void CListBox::DeleteRes()
{
	m_IncBtn.DeleteRes();
	m_DecBtn.DeleteRes();
	for (int i = 0; i < m_MaxLine; i++)
		SAFE_DELETE(m_pTextSurf[i]);
	SAFE_DELETE(ScrollBarSurface);
}

//------------------------------------------------------------
//	Name : Restore ()
//	Desc :	Surface Restore
//------------------------------------------------------------
HRESULT	CListBox :: Restore ()
{
	if ( !g_pDisplay || g_pDisplay->GetBackBuffer() == NULL)
		return E_FAIL;

	HRESULT hr;
	// �� �Լ��� Main Procedure�� Restore() �κп��� ȣ��ȴٰ�
	// ������ �Ѵ�.
	// Image File Name�� ��ȿ�ϴٰ� ������ �Ѵ�.
	if( ScrollBarSurface && FAILED( hr = ScrollBarSurface->DrawBitmap(fnScrollBar, 0, 0) ) )
		return hr;

	for (int i = 0; i < m_MaxLine; i++)
	{
		if (LineText[i])
		{
			SAFE_DELETE(m_pTextSurf[i]);
			COLORREF color = TextColor;
			if( i == SelectedLine )
			{
				color = RGB(255, 179, 0);
			}

			g_pDisplay->CreateSurfaceFromText(&m_pTextSurf[i], Hfont, LineText[i], RGB(0, 0, 1), color);
			if( m_pTextSurf[i] )
				m_pTextSurf[i]->SetColorKey(RGB(0, 0, 1));
		}
	}

	return S_OK;
}

//------------------------------------------------------------
//	Name : Draw ()
//	Desc :	�׸��� ����		(BaseUI class fn. override)
//------------------------------------------------------------
void CListBox :: Draw (  )
{
	if ( !g_pDisplay || g_pDisplay->GetBackBuffer () == NULL)
		return;

	// ����� (Item) Display
	int	scx		= PosX + BOARDER_OFF;
	int	scy		= PosY + BOARDER_OFF + TITLE_H + CONTROL_OFF ;
	int	EndLine = CurLine + MaxVisibleLine;

	// Text Drawing...
	for (int i = CurLine; i < m_MaxLine; i++)
	{
		if (i >= EndLine)
			break;

		if (m_pTextSurf[i])
		{
			g_pDisplay->Blt(scx, scy, m_pTextSurf[i]);
			scy += LineHeight;
		}
		else	// ������ Item(���� ��)�� ������ ���
			break;
	}

	// Scroll Btn�� �׸���.

	m_IncBtn.Draw(ScrollPosX, DecBtnPosY);
	m_DecBtn.Draw(ScrollPosX, IncBtnPosY);

	// Scroll Bar�� �׷��������� üũ�Ѵ�.
	if (TotalLine > MaxVisibleLine)
	{
		if (ScrollBarSurface != NULL)
			g_pDisplay->Blt (ScrollPosX, ScrollBarPosY, ScrollBarSurface);
	}
}


//------------------------------------------------------------
//	Name : GetMessage ()
//	Desc :	ListBox�� ������ message ó��
//			Window Message�� �������� ���´ٰ� ����..
//-----------------------------------------------------------

int CListBox :: GetMessage (HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int		x = LOWORD(lParam);
	int		y = HIWORD(lParam);

	static int MouseOverLine = -1;

	/*
		if( m_IncBtn.bDown )
		{
			DecrementPos ();
		}
		else if( m_DecBtn.bDown )
		{
			IncrementPos ();
		}
	*/

	switch (msg)
	{
	case 0x020A: // �ٸ��콺 �޼���

		if( (short)HIWORD(wParam) < 0 )
			IncrementPos();
		else if( (short)HIWORD(wParam) > 0 )
			DecrementPos();

		break;

	case WM_LBUTTONDOWN :
		m_IncBtn.MsgProc( hWnd, msg, wParam, lParam );
		m_DecBtn.MsgProc( hWnd, msg, wParam, lParam );
		{
			int ret;

			// Title Bar
			if (IsInTitleArea(x, y))
				state = TITLE;
			// Content(�����) ��..
			else if ((ret = IsInContentArea(x, y)) != -1)
			{
				state = CONTENT;
				if (ret < TotalLine)
				{
					if (SelectedLine >= 0)
					{
						SAFE_DELETE(m_pTextSurf[SelectedLine]);
						if( g_pDisplay )
							g_pDisplay->CreateSurfaceFromText(&m_pTextSurf[SelectedLine], Hfont, LineText[SelectedLine], RGB(0, 0, 1), TextColor);
						if( m_pTextSurf[SelectedLine] )
							m_pTextSurf[SelectedLine]->SetColorKey(RGB(0, 0, 1));
					}
					SelectedLine = ret;
					SAFE_DELETE(m_pTextSurf[SelectedLine]);
					if( g_pDisplay )
						g_pDisplay->CreateSurfaceFromText(&m_pTextSurf[SelectedLine], Hfont, LineText[SelectedLine], RGB(0, 0, 1), RGB(255, 179, 0)/*RGB(255, 142, 103)*/);
					if( m_pTextSurf[SelectedLine] )
						m_pTextSurf[SelectedLine]->SetColorKey(RGB(0, 0, 1));
					return ret;
				}
			}
			// Scroll Btn ��..
			else if (ret = IsInScrollBtnArea(x, y))
			{
				state = SCROLLBTN;
				if (ret == SCROLL_DEC_BTN)	DecrementPos ();
				else						IncrementPos ();
			}
			// Scroll Bar�� ������ �� ���� ������..
			else if (ret = IsInScrollBarArea(x, y))
			{
				if (ret == SCROLL_BAR_POLL)
					state = SCROLLBAR_POLL;
				else
				{
					state = SCROLLBAR_FLAT;
					SetBarPosToGo (y);
				}
			}
			else
				state = NONE;
			break;
		}


	case WM_LBUTTONUP :
		m_IncBtn.MsgProc( hWnd, msg, wParam, lParam );
		m_DecBtn.MsgProc( hWnd, msg, wParam, lParam );

	case WM_RBUTTONUP :
		{
			state = NONE;
			break;
		}

	case WM_MOUSEMOVE :
		{
			m_IncBtn.MsgProc( hWnd, msg, wParam, lParam );
			m_DecBtn.MsgProc( hWnd, msg, wParam, lParam );

			int ret;

			if (state == SCROLLBAR_POLL)
			{
				// if (IsInScrollBarArea(x,y))
				SetBarPosToGo (y);
			}
			// Content(�����) ��..
			//wan:2004-12	���콺 ���� ����Ʈ ������ ����
			else if ((ret = IsInContentArea(x, y)) != -1)
			{
				state = CONTENT;
				if (ret < TotalLine )
				{
					if( MouseOverLine >= 0 && MouseOverLine != SelectedLine ) // ���õ� �༮�� ���� �ȹٲٵ���
					{
						SAFE_DELETE(m_pTextSurf[MouseOverLine]);
						if( g_pDisplay )
							g_pDisplay->CreateSurfaceFromText(&m_pTextSurf[MouseOverLine], Hfont, LineText[MouseOverLine], RGB(0, 0, 1), TextColor);
						if( m_pTextSurf[MouseOverLine] )
							m_pTextSurf[MouseOverLine]->SetColorKey(RGB(0, 0, 1));
					}

					MouseOverLine = ret;

					if( MouseOverLine != SelectedLine )  // ���õ� �༮�� ���� �ȹٲٵ���
					{
						SAFE_DELETE(m_pTextSurf[MouseOverLine]);
						if( g_pDisplay )
							g_pDisplay->CreateSurfaceFromText(&m_pTextSurf[MouseOverLine], Hfont, LineText[MouseOverLine], RGB(0, 0, 1), RGB(162, 252, 74));
						if( m_pTextSurf[MouseOverLine] )
							m_pTextSurf[MouseOverLine]->SetColorKey(RGB(0, 0, 1));
					}

					return ret;
				}
			}
			else
			{
				if( MouseOverLine >= 0 && MouseOverLine < TotalLine && MouseOverLine != SelectedLine )  // ���õ� �༮�� ���� �ȹٲٵ���
				{
					SAFE_DELETE(m_pTextSurf[MouseOverLine]);
					if( g_pDisplay )
						g_pDisplay->CreateSurfaceFromText(&m_pTextSurf[MouseOverLine], Hfont, LineText[MouseOverLine], RGB(0, 0, 1), TextColor);
					if( m_pTextSurf[MouseOverLine] )
						m_pTextSurf[MouseOverLine]->SetColorKey(RGB(0, 0, 1));

					MouseOverLine = -1;
				}
				state = NONE;
			}
		}
		break;

	default :
		break;
	}
	return -1;
}



//------------------------------------------------------------
//	Name : IsInTitleArea (), IsInContextArea (),
//			IsInScrollBarArea (), IsInScrollBtnArea ()
//	Desc : ���� üũ Func.
//-----------------------------------------------------------
// Title Area���� ��ġ �ȿ� �ִ��� üũ�Ѵ�.
bool CListBox :: IsInTitleArea (int x, int y)
{
	if (y < PosY+BOARDER_OFF+TITLE_H &&
			y >= PosY+BOARDER_OFF &&
			x >= PosX+BOARDER_OFF &&
			x < PosX+Width-BOARDER_OFF)
		return true;
	else
		return false;
}

// ���� ��(Item Content)�� �ִ� ��ġ ������ üũ�Ѵ�.
int  	CListBox :: IsInContentArea (int x, int y)
{
	if (y >= PosY+BOARDER_OFF+TITLE_H+CONTROL_OFF &&
			y < PosY+Height-BOARDER_OFF &&
			x >= PosX+BOARDER_OFF &&
			x < m_IncBtn.PosX)
	{
		// clicked position�� ��(item)�� ������ ������� ����Ѵ�.
		int Selected;
		//wan:2004-12 ����Ʈ ��������
		Selected = y - (PosY+BOARDER_OFF+TITLE_H);
		Selected = Selected / LineHeight;
		Selected += CurLine;

		return Selected;
	}
	else
		return -1;
	/*
		if (y >= PosY+BOARDER_OFF+TITLE_H+CONTROL_OFF &&
				y < PosY+Height-BOARDER_OFF &&
				x >= PosX+BOARDER_OFF &&
				x < PosX+Width-BOARDER_OFF-SCROLLBTN_W+CONTROL_OFF)
		{
			// clicked position�� ��(item)�� ������ ������� ����Ѵ�.
			int Selected;
			//wan:2004-12 ����Ʈ ��������
			Selected = y - (PosY+BOARDER_OFF+TITLE_H);
			Selected = Selected / LineHeight;
			Selected += CurLine;

			return Selected;
		}
		else
			return -1;
	*/
}


// Scroll Btn�� ��ġ ������ üũ�Ѵ�.
int 	CListBox :: IsInScrollBtnArea (int x, int y)
{
	// x ��ǥ ���
	if (x >= m_IncBtn.PosX &&
			x < m_IncBtn.PosX+m_IncBtn.Width )
	{
		if (y >= m_IncBtn.PosY &&
				y < m_IncBtn.PosY+m_IncBtn.Height)
		{
			return SCROLL_DEC_BTN;	// Dec
		}
		else if (y >= m_DecBtn.PosY  &&
				 y < m_DecBtn.PosY+m_DecBtn.Height)
		{
			return SCROLL_INC_BTN;	// Inc
		}
		else
			return 0;
	}
	else
		return 0;

	/*
		// x ��ǥ ���
		if (x >= PosX+Width-BOARDER_OFF-SCROLLBTN_W &&
				x < PosX+Width-BOARDER_OFF)
		{
			if (y >= PosY+BOARDER_OFF+TITLE_H+CONTROL_OFF &&
					y < PosY+BOARDER_OFF+TITLE_H+CONTROL_OFF+SCROLLBTN_H)
			{
				return SCROLL_DEC_BTN;	// Dec
			}
			else if (y >= PosY+Height-BOARDER_OFF-SCROLLBTN_H &&
						y < PosY+Height-BOARDER_OFF)
			{
				return SCROLL_INC_BTN;	// Inc
			}
			else
				return 0;
		}
		else
			return 0;
	*/
}


// ��ũ�� �ٰ� ������ �� �ִ� ��ġ ������ üũ�Ѵ�.
int 	CListBox :: IsInScrollBarArea (int x, int y)
{
	if (y >= PosY+BOARDER_OFF+TITLE_H+CONTROL_OFF+SCROLLBTN_H &&
			y < PosY+Height-BOARDER_OFF-SCROLLBTN_H &&
			x >= PosX+Width-BOARDER_OFF-SCROLLBTN_W &&
			x < PosX+Width-BOARDER_OFF)
	{
		// Scroll Bar�� �����̸� ������ ���� �����Ѵ�.
		if (y >= ScrollBarPosY && y < ScrollBarPosY+SCROLLBAR_H)
			return SCROLL_BAR_POLL;
		return SCROLL_BAR_FLAT;
	}
	else
		return false;
}



//------------------------------------------------------------
//	Name : IncrementPos (), DecrementPos ()
//	Desc :	scroll
//------------------------------------------------------------
//
void	CListBox :: IncrementPos ()
{
	if (CurLine+MaxVisibleLine < TotalLine)
		CurLine++;

	CalculateBarPos ();
}



void	CListBox :: DecrementPos ()
{
	if (CurLine > 0)
		CurLine--;

	CalculateBarPos ();
}



//------------------------------------------------------------
//	Name : GoHome () / GoEnd()
//	Desc :	ó���� ������..
//------------------------------------------------------------
//
void	CListBox :: GoHome ()
{
	CurLine = 0;
	ScrollBarPosY = DecBtnPosY+SCROLLBTN_H;

}


void	CListBox :: GoEnd ()
{
	if (TotalLine > MaxVisibleLine)
	{
		CurLine = TotalLine-MaxVisibleLine;
		ScrollBarPosY = IncBtnPosY-SCROLLBAR_H;

	}
	else
	{
		CurLine = 0;
		ScrollBarPosY = DecBtnPosY+SCROLLBTN_H;
	}
}



//------------------------------------------------------------
//	Name : CalculateBarPos ()
//	Desc :	Scroll Bar�� ��ġ�� ����Ѵ�.
//			CurLine�� �����Ű�� �ʱ� Scroll Bar�� ��ġ�� ����Ų��.
//------------------------------------------------------------
//
void	CListBox :: CalculateBarPos ()
{
	if (TotalLine > MaxVisibleLine)
	{
		int n1 = TotalLine - MaxVisibleLine;	// ȭ��� �������� �ʴ� ���� num
		int n2 = CurLine;						// ���� �������� �����ϴ� ���� num

		if (n1 != n2)
		{
			// ȭ��� ��ũ�ѵ� �� �ִ� height pixel �� (Bar Height �������� ����)
			int HeightCanBeScrolled = Height - 2*BOARDER_OFF - TITLE_H
									  - CONTROL_OFF - 2*SCROLLBTN_H - SCROLLBAR_H;

			// ���������� ��..
			int	interval = HeightCanBeScrolled * n2 / n1;
			ScrollBarPosY = DecBtnPosY + SCROLLBTN_H + interval;
		}
		else
		{
			// �� ������ ������ ����̴�.
			// �̰��� �ٴڿ� ��Ȯ�� ���̱� ���� ����� �Ѵ�.
			ScrollBarPosY = IncBtnPosY - SCROLLBAR_H;
		}
	}
}

void	CListBox :: CalculateBarPos2( int ScrollBarHeight ) // ������ �׷��ȿ� ���缭 �ٽ� �ۼ�.(06-12-21 ����)
{
	if (TotalLine > MaxVisibleLine)
	{
		int n1 = TotalLine - MaxVisibleLine;	// ȭ��� �������� �ʴ� ���� num
		int n2 = CurLine;						// ���� �������� �����ϴ� ���� num

		if (n1 != n2)
		{
			// ȭ��� ��ũ�ѵ� �� �ִ� height pixel �� (Bar Height �������� ����)
//			int HeightCanBeScrolled = Height - 2*BOARDER_OFF /*- TITLE_H*/
//						- CONTROL_OFF - 2*m_DecBtn.Height - ScrollBarHeight;

			int HeightCanBeScrolled = m_DecBtn.PosY - (m_IncBtn.PosY+m_IncBtn.Height) - ScrollBarHeight;

			// ���������� ��..
			int	interval = HeightCanBeScrolled * n2 / n1;
			ScrollBarPosY = DecBtnPosY + m_DecBtn.Height + interval;
		}
		else
		{
			// �� ������ ������ ����̴�.
			// �̰��� �ٴڿ� ��Ȯ�� ���̱� ���� ����� �Ѵ�.
			ScrollBarPosY = IncBtnPosY - ScrollBarHeight;
		}
	}
}

//------------------------------------------------------------
//	Name : SetBarPosToGo ()
//	Desc :	Scroll Bar�� ��ġ�� ����Ѵ�.
//			Scroll Bar�� �̵���Ų��. (���콺 Event�� ���Ͽ�)
//------------------------------------------------------------
//
void	CListBox :: SetBarPosToGo (int y)
{
	if (TotalLine > MaxVisibleLine)
	{
		// Bar�� ��ġ�ؾ� �� ������ ����
		if (y < DecBtnPosY+SCROLLBTN_H+SCROLLBAR_H/2)
			GoHome ();
		else if (y > IncBtnPosY-SCROLLBAR_H/2)
			GoEnd ();
		else
		{
			// ȭ��� ��ũ�ѵ� �� �ִ� height pixel �� (Bar Height ����)
			int HeightCanBeScrolled = Height - 2*BOARDER_OFF - TITLE_H
									  - CONTROL_OFF - 2*SCROLLBTN_H;

			// �� line���� pixel��
			int interval = HeightCanBeScrolled / (TotalLine-MaxVisibleLine);

			// ������ Cur Line Position�� ���Ѵ�.
			CurLine = (y-DecBtnPosY-SCROLLBTN_H) / interval;
			CalculateBarPos ();
		}
	}
}


//------------------------------------------------------------
//	Name : AddString ()
//	Desc :	String �߰�
//------------------------------------------------------------
//
void	CListBox :: AddString (const char *str)
{
	if ( TotalLine >= 40 || LineText[TotalLine] != NULL)
		return;

	int len = strlen(str);
	if (len <= 0)
		return;
	LineText[TotalLine] = new char [len+1];

	// �޸� ���簡 �� ������ ������.. (�ι��� ���� ����)
	memcpy (LineText[TotalLine], str, len+1);

	TotalLine++;

	// Bar�� ��ġ ����
	CalculateBarPos ();
}



//------------------------------------------------------------
//	Name : SetStringToNth ()
//	Desc :	index��° string item���� set..
//			����, 3���� item�� �ִµ�, 10��°�� ����Ϸ� �ϸ�,
//			4��°�� ��ϵȴ�.
//------------------------------------------------------------
//
void	CListBox :: SetStringToNth (int index, const char *str)
{
	if ( str == NULL || index < 0 || index >= m_MaxLine)
		return;

	// Index ���� ��...
	int newIndex;

	if (LineText[index] != NULL)
	{
		newIndex = index;
		delete LineText[index];
	}
	else
		newIndex = TotalLine++;

	// �޸� ����
	int len = strlen(str);
	LineText[newIndex] = new char [len+1];
	memcpy (LineText[newIndex], str, len+1);

	// Bar�� ��ġ ����
	CalculateBarPos ();
}




//----------------------------------------------------------------------------
//-- Set Font
//----------------------------------------------------------------------------
//-- Global Font �� �ϳ��� �޾ƿ´�.

//-- 2004.03.12 - �����Ϸ�
//-- Lyul
//-- ��ü �������� Global ������� ����

void	CListBox :: SetFont (int f_w, int f_h, char *fontname)
{
	if( fontname == NULL )
		return;

	HDC hDC = GetDC(NULL);

	int ptHeight = -MulDiv(f_h, GetDeviceCaps(hDC, LOGPIXELSY), 72);
	ReleaseDC(NULL, hDC);

	// Font ����

	Hfont = G_GetFont(ptHeight,0,fontname);
	// Line Height ��� ���..
	//LineHeight = f_h+2;		// 2 pixel ��...
	LineHeight = 0-(ptHeight-2);	// 2 pixel ��...
	LineHeight = LineHeight+2;
	MaxVisibleLine = (Height-BOARDER_OFF*2-TITLE_H-CONTROL_OFF) / LineHeight;

	// Bar Position ����
	CalculateBarPos ();
}


//------------------------------------------------------------
//	Name : GetString ()
//	Desc :	index��° string item���� get
//------------------------------------------------------------
//
char*	CListBox :: GetString (int index)
{
	if (index < 0 || index >= TotalLine)
		return NULL;
	else
		return LineText[index];
}



//------------------------------------------------------------
//	Name : GetIndex (const char *str)
//	Desc :	str�� ������ �ִ� Item�� index return
//------------------------------------------------------------
//
int 	CListBox :: GetIndex (const char *str)
{
	if( str == NULL )
		return -1;

	for (int i = 0; i < TotalLine; i++)
	{
		if ( !strcmp (str, LineText[i]) )
			return i;
	}
	return -1;
}



//--------------------------------	Set / Get Function


//------------------------------------------------------------
//	Name : SetTitle (const char *title)
//	Desc :	Title Set
//------------------------------------------------------------
//
void	CListBox :: SetTitle (const char *title)
{
	if (title == NULL)
		return;

	int len = strlen(title) + 1;
	if (len > m_MaxTitle)
	{
		memcpy (Title, title, m_MaxTitle-1);
		Title[m_MaxTitle-1] = '\0';
	}
	else
	{
		memcpy (Title, title, len);
	}
}



//------------------------------------------------------------
//	Name : SetTitleColor(), SetTitleBkColor()
//		   SetTextColor(), SetTextBkColor()
//	Desc :	Title/ Text Color Set
//------------------------------------------------------------
//
void	CListBox :: SetTitleColor (COLORREF dwGDIColor)
{
	TitleColor = dwGDIColor;
}

void	CListBox :: SetTitleBkColor (COLORREF dwGDIColor)
{
	TitleBkColor = dwGDIColor;
}

void	CListBox :: SetTextColor (COLORREF dwGDIColor)
{
	TextColor = dwGDIColor;
}

void	CListBox :: SetTextBkColor (COLORREF dwGDIColor)
{
	TextBkColor = dwGDIColor;
}



//************************************************************
//************************************************************

