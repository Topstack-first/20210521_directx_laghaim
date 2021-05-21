#include "stdafx.h"
#include "multilistbox.h"

#include "dxutil.h"
#include "main.h"
#include "LoadEffectSound.h"
#include "TextBoard.h"



extern char g_strFontName[];

//------------------------------------------------------------
//	Nams : ListBox ()
//	Desc :	생성자
//------------------------------------------------------------
//
int MouseOverLine = -1;

CMutiListBox::CMutiListBox()
	: state(NONE)
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
	, m_nScrollbarH(0)
	, Hfont(NULL)
	, TotalLine(0)
	, CurLine(0)
	, SelectedLine(0)
	, LineHeight(0)
	, MaxVisibleLine(0)
{
	memset(Title, 0, sizeof(Title));

	// 버튼 파일이름
	fnScrollBar = NULL;
	// 버튼을 위한 SURFACE
	ScrollBarSurface = NULL;

	m_pTextBoard = NULL;
}

CMutiListBox::CMutiListBox(char *title, int nX, int nY, int nH, int nLineNum, int nColumn, int nStringSize/* = 20*/)
	: fnDecrementBtn(NULL)
	, fnIncrementBtn(NULL)
{
	m_pTextBoard = NULL;

	PosX = nX;
	PosY = nY;
	Width = 0;
	Height = nH;

	m_nScrollbarH = 0;
	InitVar(title, nX, nY, nH, nLineNum, nColumn, nStringSize);

	SetFont (5, 12, g_strFontName);
}

CMutiListBox :: ~CMutiListBox ()
{
	DeleteRes();

	SAFE_DELETE (ScrollBarSurface);
	SAFE_DELETE(m_pTextBoard);

	SAFE_DELETE_ARRAY(fnScrollBar);
}

//------------------------------------------------------------
//	Name : InitVar ()
//	Desc :	멤버 변수 초기화...
//------------------------------------------------------------
//
void CMutiListBox :: InitVar (char *title, int nX, int nY, int nH, int nLineNum, int nColumn, int nStringSize/* = 20*/)
{
	state = NONE;

	if (strlen(title)+1 <= MAX_TITLE)
		memcpy (Title, title, strlen(title)+1);
	else
	{
		memcpy (Title, title, MAX_TITLE-1);
		Title[MAX_TITLE-1] = '\0';
	}

	fnScrollBar = NULL;
	ScrollBarSurface = NULL;

	// Color Member / Font Member
	Hfont = NULL;
	TitleColor = RGB(255, 255, 255);
	TitleBkColor = 0;
	TextColor = /*TEXTLIST_RGB_NOR*/ RGB(228, 203, 156);
	TextBkColor = 0;

	CurLine = 0;
	SelectedLine = -1;
	TotalLine = 0;
	LineHeight = 20;
	MaxVisibleLine = (Height-BOARDER_OFF*2-TITLE_H-CONTROL_OFF) / LineHeight;

	ScrollPosX = PosX+Width-SCROLLBTN_W-BOARDER_OFF;
	DecBtnPosY = PosY+TITLE_H+BOARDER_OFF;
	IncBtnPosY = PosY+Height-SCROLLBTN_H-BOARDER_OFF;
	ScrollBarPosY = PosY+TITLE_H+BOARDER_OFF+SCROLLBTN_H;

	m_pTextBoard = new CTextBoard(nStringSize);

	m_pTextBoard->Init(nX, nY, nH, nLineNum, nColumn);

	m_pTextBoard->SetBasisPos(nX, nY);
}

void CMutiListBox::SetBtnPosY(int nIncY, int nDecY)
{
	DecBtnPosY = nDecY;
	IncBtnPosY = nIncY;
}

void CMutiListBox::AddLine(int dwTextIdx, float fBoxWidth)
{
	m_pTextBoard->AddLine(dwTextIdx, fBoxWidth);
}

//------------------------------------------------------------
//	Name : SetPosDim ()
//	Desc :	Position, Dimension Set  (BaseUI class fn. override)
//			ListBox의 위치와 크기를 변경시킨다.
//------------------------------------------------------------
void CMutiListBox :: SetPosDim (const int x, const int y, const int w, const int h)
{
	PosX = x;
	PosY = y;
	Width = w;
	Height = h;

	m_pTextBoard->SetBasisPos(x, y);

	MaxVisibleLine = (Height-BOARDER_OFF*2-TITLE_H-CONTROL_OFF) / LineHeight;

	// Scroll Bar, Btn Position 재 계산
	ScrollPosX = PosX+Width-SCROLLBTN_W-BOARDER_OFF;
	DecBtnPosY = PosY+TITLE_H+BOARDER_OFF;
	IncBtnPosY = PosY+Height-SCROLLBTN_H-BOARDER_OFF;
	ScrollBarPosY = PosY+TITLE_H+BOARDER_OFF+SCROLLBTN_H;
}

void CMutiListBox::SetMove(int nX, int nY, int nWidth, int nHeight)
{
	m_pTextBoard->SetBasisPos(nWidth, nHeight);

	ScrollPosX += nX;
	DecBtnPosY += nY;
	IncBtnPosY += nY;
	ScrollBarPosY += nY;
}
//---------------------------------------------------------
//		 주의 할 점은 처음 두개의 파일명은 버튼 형식으로
//		 마지막 파일명은 경로까지 쓰는걸 잊으면 않된다
//----------------------------------------------------------
void CMutiListBox::SetImage (char *DecBtn, char *IncBtn, char *ScrollBar, int nBarHeight)
{
	if( DecBtn == NULL || IncBtn == NULL || ScrollBar == NULL )
		return;

	int len = strlen (ScrollBar);
	fnScrollBar = new char [len+1];
	memcpy (fnScrollBar, ScrollBar, len+1);

	m_IncBtn.SetFileName(IncBtn);
	m_IncBtn.SetSize(SCROLLBTN_W, SCROLLBTN_H);
	m_IncBtn.SetDisable( false );

	m_DecBtn.SetFileName(DecBtn);
	m_DecBtn.SetSize(SCROLLBTN_W, SCROLLBTN_H);
	m_DecBtn.SetDisable( false );

	m_nScrollbarH = nBarHeight;
}

void CMutiListBox::LoadRes()
{
	if( !g_pDisplay )
		return;

	m_IncBtn.LoadRes();
	m_DecBtn.LoadRes();

	g_pDisplay->CreateSurfaceFromBitmap (&ScrollBarSurface, fnScrollBar, 0, 0);
	if(ScrollBarSurface)
		ScrollBarSurface->SetColorKey( RGB(0, 0, 0) );
}

void CMutiListBox::DeleteRes()
{
	m_IncBtn.DeleteRes();
	m_DecBtn.DeleteRes();

//	SAFE_DELETE (ScrollBarSurface);
//	SAFE_DELETE(m_pTextBoard);
}

void CMutiListBox::all_Reset()
{
	m_pTextBoard->Clear();

	TotalLine = 0;

	ScrollPosX = PosX+Width-SCROLLBTN_W-BOARDER_OFF;
	DecBtnPosY = PosY+TITLE_H+BOARDER_OFF;
	IncBtnPosY = PosY+Height-SCROLLBTN_H-BOARDER_OFF;
	ScrollBarPosY = PosY+TITLE_H+BOARDER_OFF+SCROLLBTN_H;
}

//------------------------------------------------------------
//	Name : Restore ()
//	Desc :	Surface Restore
//------------------------------------------------------------
HRESULT	CMutiListBox :: Restore ()
{
	if ( !g_pDisplay || g_pDisplay->GetBackBuffer() == NULL)
		return E_FAIL;

	HRESULT hr;
	// 이 함수는 Main Procedure의 Restore() 부분에서 호출된다고
	// 가정을 한다.
	// Image File Name이 유효하다고 가정을 한다.
	if( ScrollBarSurface && FAILED( hr = ScrollBarSurface->DrawBitmap(fnScrollBar, 0, 0) ) )
		return hr;

	m_pTextBoard->Restore();

	return S_OK;
}

//------------------------------------------------------------
//	Name : Draw ()
//	Desc :	그리기 수행		(BaseUI class fn. override)
//------------------------------------------------------------
void CMutiListBox :: Draw (  )
{
	if ( !g_pDisplay || g_pDisplay->GetBackBuffer () == NULL)
		return;

	// 내용글 (Item) Display
	int	scx		= PosX + BOARDER_OFF ;
	int	scy		= PosY + BOARDER_OFF + TITLE_H + CONTROL_OFF ;
	int	EndLine = CurLine + MaxVisibleLine;

	// Text Drawing...
	BOOL over = FALSE;
	for (int i = CurLine; i < TotalLine; i++)
	{
		if (i >= EndLine)
			break;

		if(MouseOverLine == i && MouseOverLine != SelectedLine)
			over = TRUE;
		else
			over = FALSE;

		m_pTextBoard->DrawLine(i, scx, scy, over);
		scy += LineHeight;
	}


	// Scroll Btn을 그린다.
	m_IncBtn.Draw(ScrollPosX, DecBtnPosY);
	m_DecBtn.Draw(ScrollPosX, IncBtnPosY);

	// Scroll Bar를 그려야할지를 체크한다.
	if (TotalLine > MaxVisibleLine)
	{
		if (ScrollBarSurface != NULL)
			g_pDisplay->Blt( ScrollPosX, ScrollBarPosY, ScrollBarSurface );
	}
}


BOOL CMutiListBox::IsInsideBar(int x, int y)
{
	if(x >= ScrollPosX &&
			x <  ScrollPosX + ScrollBarSurface->GetWidth() &&
			y >= ScrollBarPosY &&
			y <  ScrollBarPosY + ScrollBarSurface->GetHeight())
		return TRUE;

	state = NONE;

	return FALSE;
}

//------------------------------------------------------------
//	Name : GetMessage ()
//	Desc :	ListBox로 들어오는 message 처리
//			Window Message는 여과없이 들어온다고 가정..
//-----------------------------------------------------------

int CMutiListBox::GetMessage (HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int x = LOWORD(lParam);
	int	y = HIWORD(lParam);

	//static int MouseOverLine = -1;

	m_pTextBoard->MsgProc(hWnd, msg, wParam, lParam);

	switch (msg)
	{
	case 0x020A : // 휠마우스 메세지

		if( !IsInside( x, y) )
			return E_FAIL;

		if( IsInContentArea( x, y) )
		{
			if( (short)HIWORD(wParam) < 0 )
				IncrementPos();
			else if( (short)HIWORD(wParam) > 0 )
				DecrementPos();

			return S_OK;
		}
		break;

	case WM_LBUTTONDOWN :

		if( !IsInside( x, y) )
			return E_FAIL;

		if( E_FAIL != m_IncBtn.MsgProc( hWnd, msg, wParam, lParam ) )
		{
			DecrementPos();
			return S_OK;
		}

		if( E_FAIL != m_DecBtn.MsgProc( hWnd, msg, wParam, lParam ) )
		{
			IncrementPos();
			return S_OK;
		}

		{
			int ret;

			// Title Bar
			if (IsInTitleArea(x, y))
			{
				state = TITLE;
				return S_OK;
			}

			// Content(내용글) 안..
			if ((ret = IsInContentArea(x, y)) != -1)
			{
				state = CONTENT;

				if (ret < TotalLine)
				{
					if (SelectedLine >= 0)
					{
						m_pTextBoard->SelectLine(SelectedLine, false);
					}

					SelectedLine = ret;
					m_pTextBoard->SelectLine(SelectedLine, true);
					return ret;
				}
				return S_OK;
			}

			// Scroll Btn 안..
			if (ret = IsInScrollBtnArea(x, y))
			{
				state = SCROLLBTN;
				if (ret == SCROLL_DEC_BTN)
					DecrementPos ();
				else
					IncrementPos ();

				return S_OK;
			}

			// Scroll Bar가 움직일 수 공간 안인지..
			if (ret = IsInScrollBarArea(x, y))
			{
				if (ret == SCROLL_BAR_POLL)
					state = SCROLLBAR_POLL;
				else
				{
					state = SCROLLBAR_FLAT;
					SetBarPosToGo (y);
				}
				return S_OK;
			}
			else
			{
				state = NONE;
			}
		}
		break;

	case WM_LBUTTONUP :

		if( !IsInside( x, y) )
			return E_FAIL;

		if( E_FAIL != m_IncBtn.MsgProc( hWnd, msg, wParam, lParam ) )
			return  S_OK;

		if( E_FAIL != m_DecBtn.MsgProc( hWnd, msg, wParam, lParam ) )
			return S_OK;

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
				SetBarPosToGo (y);
			}
			// Content(내용글) 안..
			//wan:2004-12	마우스 위의 리스트 색상을 변경
			else if ((ret = IsInContentArea(x, y)) != -1)
			{
				state = CONTENT;
				if (ret < TotalLine)
				{
					if( MouseOverLine >= 0 && MouseOverLine != SelectedLine ) // 선택된 녀석은 색상 안바꾸도록
					{
						//m_pTextBoard->SelectLine(MouseOverLine, false);
					}

					MouseOverLine = ret;

					if( MouseOverLine != SelectedLine )  // 선택된 녀석은 색상 안바꾸도록
					{

					}

					return ret;
				}
			}
			else
			{
				if( MouseOverLine >= 0 && MouseOverLine < TotalLine && MouseOverLine != SelectedLine )  // 선택된 녀석은 색상 안바꾸도록
				{
					MouseOverLine = -1;
				}
				state = NONE;
			}
		}
		break;

	default :
		break;
	}


	return E_FAIL;
}



//------------------------------------------------------------
//	Name : IsInTitleArea (), IsInContextArea (),
//			IsInScrollBarArea (), IsInScrollBtnArea ()
//	Desc : 영역 체크 Func.
//-----------------------------------------------------------
// Title Area안의 위치 안에 있는지 체크한다.
bool CMutiListBox:: IsInTitleArea (int x, int y)
{
	if( y <  PosY+BOARDER_OFF+TITLE_H &&
			y >= PosY+BOARDER_OFF &&
			x >= PosX+BOARDER_OFF &&
			x <  PosX+Width-BOARDER_OFF)
		return true;
	else
		return false;
}

// 내용 글(Item Content)이 있는 위치 안인지 체크한다.
int CMutiListBox:: IsInContentArea (int x, int y)
{
	if( y >= PosY+BOARDER_OFF+TITLE_H+CONTROL_OFF &&
			y <  PosY+Height-BOARDER_OFF &&
			x >= PosX+BOARDER_OFF &&
			//x <  m_IncBtn.PosX)
			x <  PosX + Width - m_IncBtn.Width )

	{
		// clicked position이 글(item)의 유무와 관계없이 계산한다.
		int Selected;
		//wan:2004-12 리스트 오차수정
		Selected = y - (PosY+BOARDER_OFF+TITLE_H);
		Selected = Selected / LineHeight;
		Selected += CurLine;

		return Selected;
	}
	else
		return -1;
}


// Scroll Btn의 위치 안인지 체크한다.
int CMutiListBox:: IsInScrollBtnArea (int x, int y)
{
	// x 좌표 계산
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

}


// 스크롤 바가 움직일 수 있는 위치 안인지 체크한다.
int CMutiListBox:: IsInScrollBarArea (int x, int y)
{
	if (y >= PosY+BOARDER_OFF+TITLE_H+CONTROL_OFF+SCROLLBTN_H &&
			y < PosY+Height-BOARDER_OFF-SCROLLBTN_H &&
			x >= PosX+Width-BOARDER_OFF-SCROLLBTN_W &&
			x < PosX+Width-BOARDER_OFF)
	{
		// Scroll Bar의 몸땡이를 눌렀을 때는 무시한다.
		if (y >= ScrollBarPosY && y < ScrollBarPosY+m_nScrollbarH)
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
void CMutiListBox :: IncrementPos ()
{
	if (CurLine+MaxVisibleLine < TotalLine)
		CurLine++;

	CalculateBarPos ();
}



void CMutiListBox :: DecrementPos ()
{
	if (CurLine > 0)
		CurLine--;

	CalculateBarPos ();
}



//------------------------------------------------------------
//	Name : GoHome () / GoEnd()
//	Desc :	처음과 끝으로..
//------------------------------------------------------------
//
void CMutiListBox :: GoHome ()
{
	CurLine = 0;
	ScrollBarPosY = DecBtnPosY+SCROLLBTN_H;

}


void CMutiListBox :: GoEnd ()
{
	if (TotalLine > MaxVisibleLine)
	{
		CurLine = TotalLine-MaxVisibleLine;
		ScrollBarPosY = IncBtnPosY-m_nScrollbarH;

	}
	else
	{
		CurLine = 0;
		ScrollBarPosY = DecBtnPosY+SCROLLBTN_H;
	}
}



//------------------------------------------------------------
//	Name : CalculateBarPos ()
//	Desc :	Scroll Bar의 위치를 계산한다.
//			CurLine은 변경시키지 않구 Scroll Bar의 위치만 계산시킨다.
//------------------------------------------------------------
//
void CMutiListBox :: CalculateBarPos ()
{
	if (TotalLine > MaxVisibleLine)
	{
		int n1 = TotalLine - MaxVisibleLine;	// 화면상에 보여지지 않는 라인 num
		int n2 = CurLine;						// 현재 보여지기 시작하는 라인 num

		if (n1 != n2)
		{
			// 화면상에 스크롤될 수 있는 height pixel 수 (Bar Height 포함하지 않음)
			int HeightCanBeScrolled = Height - 2*BOARDER_OFF - TITLE_H
									  - CONTROL_OFF - 2*SCROLLBTN_H - m_nScrollbarH;

			// 움직여지는 양..
			int	interval = HeightCanBeScrolled * n2 / n1;
			ScrollBarPosY = DecBtnPosY + SCROLLBTN_H + interval;
		}
		else
		{
			// 맨 밑으로 내려간 경우이다.
			// 이경우는 바닥에 정확히 붙이기 위한 계산을 한다.
			ScrollBarPosY = IncBtnPosY - m_nScrollbarH;
		}
	}
}

void CMutiListBox :: CalculateBarPos2( int ScrollBarHeight ) // 가변형 그래픽에 맞춰서 다시 작성.(06-12-21 원석)
{
	if (TotalLine > MaxVisibleLine)
	{
		int n1 = TotalLine - MaxVisibleLine;	// 화면상에 보여지지 않는 라인 num
		int n2 = CurLine;						// 현재 보여지기 시작하는 라인 num

		if (n1 != n2)
		{
			// 화면상에 스크롤될 수 있는 height pixel 수 (Bar Height 포함하지 않음)
//			int HeightCanBeScrolled = Height - 2*BOARDER_OFF /*- TITLE_H*/
//						- CONTROL_OFF - 2*m_DecBtn.Height - ScrollBarHeight;

			int HeightCanBeScrolled = m_DecBtn.PosY - (m_IncBtn.PosY+m_IncBtn.Height) - ScrollBarHeight;

			// 움직여지는 양..
			int	interval = HeightCanBeScrolled * n2 / n1;
			ScrollBarPosY = DecBtnPosY + m_DecBtn.Height + interval;
		}
		else
		{
			// 맨 밑으로 내려간 경우이다.
			// 이경우는 바닥에 정확히 붙이기 위한 계산을 한다.
			ScrollBarPosY = IncBtnPosY - ScrollBarHeight;
		}
	}
}

//------------------------------------------------------------
//	Name : SetBarPosToGo ()
//	Desc :	Scroll Bar의 위치를 계산한다.
//			Scroll Bar를 이동시킨다. (마우스 Event에 의하여)
//------------------------------------------------------------
//
void CMutiListBox :: SetBarPosToGo (int y) ///071102 원석 수정 : 스크롤 계산을 int로 하면 졸라 라인수가 많으면 삐꾸된다. float으로 계산하도록 수정.
{
	if (TotalLine > MaxVisibleLine)
	{
		// Bar가 위치해야 할 포지션 구함
		if (y < DecBtnPosY+SCROLLBTN_H+m_nScrollbarH/2)
			GoHome ();
		else if (y > IncBtnPosY - m_nScrollbarH/2)
			GoEnd ();
		else
		{
			// 화면상에 스크롤될 수 있는 height pixel 수 (Bar Height 포함)
			int HeightCanBeScrolled = Height - 2*BOARDER_OFF - TITLE_H - CONTROL_OFF - 2*SCROLLBTN_H;

			// 한 line당의 pixel수
			float interval = (float)HeightCanBeScrolled / (float)(TotalLine-MaxVisibleLine);

			if(interval <= 0.0f)
				interval = 1.0f;

			// 현재의 Cur Line Position을 구한다.
			CurLine = (float)(y-DecBtnPosY-SCROLLBTN_H) / interval;

			CalculateBarPos ();

		}
	}
}


//------------------------------------------------------------
//	Name : AddString ()
//	Desc :	String 추가
//------------------------------------------------------------
//

void CMutiListBox::AddString(int nLineIdx, int nTextIdx, char *szText)
{
//	if ( TotalLine >= MAX_LINE )//  || LineText[TotalLine] != NULL) ///071102 원석 수정 : 길드 수가 엄청 많을수있으므로 리스트 갯수를 무제한으로 함.이건 좀 문제가 될수도 있다. 주의!
//		return;

	int len = strlen(szText);
	if (len <= 0)
		return;

	m_pTextBoard->SetText(nLineIdx, nTextIdx, szText);
}

void CMutiListBox::SetIncLine()
{
	TotalLine++;

	CalculateBarPos();
}

void	CMutiListBox :: SetFont (int f_w, int f_h, char *fontname)
{
	if( fontname == NULL )
		return;

	HDC hDC = GetDC(NULL);

	int ptHeight = -MulDiv(f_h, GetDeviceCaps(hDC, LOGPIXELSY), 72);
	ReleaseDC(NULL, hDC);

	// Font 생성

	Hfont = G_GetFont(ptHeight,0,fontname);
	// Line Height 계산 요망..
	//LineHeight = f_h+2;		// 2 pixel 더...
	LineHeight = 0-(ptHeight-2);	// 2 pixel 더...
	LineHeight = LineHeight+2;
	MaxVisibleLine = (Height) / LineHeight;

	// Bar Position 변경
	CalculateBarPos ();
}


//------------------------------------------------------------
//	Name : GetString ()
//	Desc :	index번째 string item값을 get
//------------------------------------------------------------
//
// char*	CMutiListBox :: GetString (int index)
// {
// // 	if (index < 0 || index >= TotalLine)
// // 		return NULL;
// // 	else
// // 		return LineText[index];
// 	return NULL;
// }

char* CMutiListBox::GetString(int nTextId)
{
	return m_pTextBoard->GetText(SelectedLine, nTextId);
}

//------------------------------------------------------------
//	Name : GetIndex (const char *str)
//	Desc :	str을 가지고 있는 Item의 index return
//------------------------------------------------------------

int 	CMutiListBox :: GetIndex (const char *str)
{
//	if( str == NULL )
//		return -1;
//
//	for (int i = 0; i < TotalLine; i++)
//	{
//		if ( !strcmp (str, LineText[i]) )
//			return i;
//	}
	return -1;
}



//--------------------------------	Set / Get Function


//------------------------------------------------------------
//	Name : SetTitle (const char *title)
//	Desc :	Title Set
//------------------------------------------------------------
//
void	CMutiListBox :: SetTitle (const char *title)
{
	if (title == NULL)
		return;

	int len = strlen(title) + 1;
	if (len > MAX_TITLE)
	{
		memcpy (Title, title, MAX_TITLE-1);
		Title[MAX_TITLE-1] = '\0';
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
void	CMutiListBox :: SetTitleColor (COLORREF dwGDIColor)
{
	TitleColor = dwGDIColor;
}

void	CMutiListBox :: SetTitleBkColor (COLORREF dwGDIColor)
{
	TitleBkColor = dwGDIColor;
}

void	CMutiListBox :: SetTextColor (COLORREF dwGDIColor)
{
	TextColor = dwGDIColor;
}

void	CMutiListBox :: SetTextBkColor (COLORREF dwGDIColor)
{
	TextBkColor = dwGDIColor;
}
