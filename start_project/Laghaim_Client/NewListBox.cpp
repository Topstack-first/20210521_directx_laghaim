#include "stdafx.h"
#include "NewListBox.h"
#include "dxutil.h"

#include "textbox.h"
#include "main.h"
#include "LoadEffectSound.h"



extern char g_strFontName[];

//------------------------------------------------------------
//	Nams : ListBox ()
//	Desc :	생성자
//------------------------------------------------------------
CNewListBox::CNewListBox()
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
	memset(ColorTable, 0, sizeof(ColorTable));

	// 버튼 파일이름
	fnScrollBar = NULL;
	// 버튼을 위한 SURFACE
	ScrollBarSurface = NULL;
}

CNewListBox :: CNewListBox (char *title)
	: fnDecrementBtn(NULL)
	, fnIncrementBtn(NULL)
{
	memset(ColorTable, 0, sizeof(ColorTable));

	PosX = 100;
	PosY = 100;
	Width = 200;
	Height = 200;
	InitVar (title);
	SetFont (5, 9, g_strFontName);
}


CNewListBox :: CNewListBox (char *title, int x, int y, int w, int h)
	: CBaseUI (x, y, w, h)
	, fnDecrementBtn(NULL)
	, fnIncrementBtn(NULL)
{
	memset(ColorTable, 0, sizeof(ColorTable));

	InitVar (title);
	SetFont (0, 9, g_strFontName);
}

CNewListBox :: CNewListBox (char *title, int x, int y, int w, int h, int flag)
	: CBaseUI (x, y, w, h)
	, TitleColor(0)
	, TitleBkColor(0)
	, TextColor(0)
	, TextBkColor(0)
	, fnDecrementBtn(NULL)
	, fnIncrementBtn(NULL)
{
	memset(Title, 0, sizeof(Title));
	memset(ColorTable, 0, sizeof(ColorTable));

	state = NONE;
	fnScrollBar = NULL;
	ScrollBarSurface = NULL;
	// List Box Content Text
	for (int i = 0; i < MAX_LINE; i++)
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
	ScrollPosX = PosX+Width-17-BOARDER_OFF; //스크롤버튼 크기 16,7
	DecBtnPosY = PosY+TITLE_H+BOARDER_OFF;
	IncBtnPosY = PosY+Height-8-BOARDER_OFF;
	ScrollBarPosY = DecBtnPosY+SCROLLBTN_H;

	SetFont (0, 9, g_strFontName);
}

//-- 2004.03.15	- 수정완료
//-- Lyul
//-- m_Font 가 handle 를 가져온다. 따라서 파괴할 필요가 없다.
CNewListBox :: ~CNewListBox ()
{
	DeleteRes();

	for (int i = 0; i < MAX_LINE; i++)
	{
		if(m_pTextSurf[i])
			SAFE_DELETE(m_pTextSurf[i]);
	}

	for (int i = 0; i < MAX_LINE; i++)
		SAFE_DELETE_ARRAY(LineText[i]);

	SAFE_DELETE (ScrollBarSurface);
}

//------------------------------------------------------------
//	Name : InitVar ()
//	Desc :	멤버 변수 초기화...
//------------------------------------------------------------
void CNewListBox :: InitVar (char *title)
{
	if( !title )
		return;

	state = NONE;

	if (strlen(title)+1 <= MAX_TITLE)
		memcpy (Title, title, strlen(title)+1);
	else
	{
		memcpy (Title, title, MAX_TITLE-1);
		Title[MAX_TITLE-1] = '\0';
	}

	// Image File Name
	fnScrollBar = NULL;
	ScrollBarSurface = NULL;
	// List Box Content Text
	for (int i = 0; i < MAX_LINE; i++)
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
//			ListBox의 위치와 크기를 변경시킨다.
//------------------------------------------------------------
void	CNewListBox :: SetPosDim (const int x, const int y, const int w, const int h)
{
	PosX = x;
	PosY = y;
	Width = w;
	Height = h;

	// 한 화면에 보여질 라인 수 재 계산
	MaxVisibleLine = (Height-BOARDER_OFF*2-TITLE_H-CONTROL_OFF) / LineHeight;

	// Scroll Bar, Btn Position 재 계산
	ScrollPosX = PosX+Width-SCROLLBTN_W-BOARDER_OFF;
	DecBtnPosY = PosY+TITLE_H+BOARDER_OFF;
	IncBtnPosY = PosY+Height-SCROLLBTN_H-BOARDER_OFF;
	ScrollBarPosY = PosY+TITLE_H+BOARDER_OFF+SCROLLBTN_H;
}

//---------------------------------------------------------
//		 주의 할 점은 처음 두개의 파일명은 버튼 형식으로
//		 마지막 파일명은 경로까지 쓰는걸 잊으면 않된다
//----------------------------------------------------------
void CNewListBox::SetImage (char *DecBtn, char *IncBtn, char *ScrollBar)
{
	if( !DecBtn || !IncBtn || !ScrollBar )
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

}

void CNewListBox::LoadRes()
{
	if( !g_pDisplay )
		return;

	m_IncBtn.LoadRes();
	m_DecBtn.LoadRes();

	g_pDisplay->CreateSurfaceFromBitmap (&ScrollBarSurface, fnScrollBar, 0, 0);

	if( ScrollBarSurface )
		ScrollBarSurface->SetColorKey((RGB(0, 0, 0)));

	for (int i = 0; i < MAX_LINE; i++)
	{
		if (LineText[i])
		{
			g_pDisplay->CreateSurfaceFromText(&m_pTextSurf[i], Hfont, LineText[i], RGB(0, 0, 1), TextColor);
			m_pTextSurf[i]->SetColorKey(RGB(0, 0, 1));
		}
	}
}

void CNewListBox::DeleteRes()
{

	m_IncBtn.DeleteRes();
	m_DecBtn.DeleteRes();
	for (int i = 0; i < MAX_LINE; i++)
	{
		if(m_pTextSurf[i])
			SAFE_SURFACE_DESTROY(m_pTextSurf[i]);
	}

	if(ScrollBarSurface)
		SAFE_SURFACE_DESTROY(ScrollBarSurface);
}

//------------------------------------------------------------
//	Name : Restore ()
//	Desc :	Surface Restore
//------------------------------------------------------------
HRESULT	CNewListBox :: Restore ()
{
	if (!g_pDisplay || g_pDisplay->GetBackBuffer() == NULL)
		return E_FAIL;

	HRESULT hr;
	// 이 함수는 Main Procedure의 Restore() 부분에서 호출된다고
	// 가정을 한다.
	// Image File Name이 유효하다고 가정을 한다.
	if( FAILED( hr = ScrollBarSurface->DrawBitmap(fnScrollBar, 0, 0) ) )
		return hr;

	return S_OK;
}

//------------------------------------------------------------
//	Name : Draw ()
//	Desc :	그리기 수행		(BaseUI class fn. override)
//------------------------------------------------------------
void CNewListBox :: Draw (  )
{
	if (!g_pDisplay || g_pDisplay->GetBackBuffer () == NULL)
		return;

	// 내용글 (Item) Display
	int	scx		= PosX + BOARDER_OFF;
	int	scy		= PosY + BOARDER_OFF + TITLE_H + CONTROL_OFF ;
	int	EndLine = CurLine + MaxVisibleLine;

	// Text Drawing...
	for (int i = CurLine; i < MAX_LINE; i++)
	{
		if (i >= EndLine)
			break;
		if (m_pTextSurf[i])
		{
			g_pDisplay->Blt(scx, scy, m_pTextSurf[i]);

			scy += LineHeight;
		}
		else	// 마지막 Item(내용 글)에 도달한 경우
			break;
	}

	// Scroll Bar를 그려야할지를 체크한다.
	if (TotalLine > MaxVisibleLine)
	{
		m_IncBtn.Draw(ScrollPosX, DecBtnPosY);
		m_DecBtn.Draw(ScrollPosX, IncBtnPosY);

		if (ScrollBarSurface != NULL)
			g_pDisplay->Blt (ScrollPosX, ScrollBarPosY, ScrollBarSurface);
	}
}


//------------------------------------------------------------
//	Name : GetMessage ()
//	Desc :	ListBox로 들어오는 message 처리
//			Window Message는 여과없이 들어온다고 가정..
//-----------------------------------------------------------

int CNewListBox :: GetMessage (HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int		x = LOWORD(lParam);
	int		y = HIWORD(lParam);

	switch (msg)
	{
	case WM_LBUTTONDOWN :
		m_IncBtn.MsgProc( hWnd, msg, wParam, lParam );
		m_DecBtn.MsgProc( hWnd, msg, wParam, lParam );
		{
			int ret;

			// Title Bar
			if (IsInTitleArea(x, y))
				state = TITLE;
			// Content(내용글) 안..
			else if ((ret = IsInContentArea(x, y)) != -1)
			{
				state = CONTENT;
				if (g_pDisplay && ret < TotalLine)
				{
					if (SelectedLine >= 0)
					{
						delete m_pTextSurf[SelectedLine];
						g_pDisplay->CreateSurfaceFromText(&m_pTextSurf[SelectedLine], Hfont, LineText[SelectedLine], RGB(0, 0, 1), ColorTable[SelectedLine]);
						if( m_pTextSurf[SelectedLine] )
							m_pTextSurf[SelectedLine]->SetColorKey(RGB(0, 0, 1));
					}
					SelectedLine = ret;
					delete m_pTextSurf[SelectedLine];
					g_pDisplay->CreateSurfaceFromText(&m_pTextSurf[SelectedLine], Hfont, LineText[SelectedLine], RGB(100, 100, 100)/*RGB(0, 0, 1)*/, ColorTable[SelectedLine]/*RGB(255, 142, 103)*/);
					if( m_pTextSurf[SelectedLine] )
						m_pTextSurf[SelectedLine]->SetColorKey(RGB(0, 0, 1));
					return ret;
				}
			}
			// Scroll Btn 안..
			else if (ret = IsInScrollBtnArea(x, y))
			{
				state = SCROLLBTN;
				if (ret == SCROLL_DEC_BTN)	DecrementPos ();
				else						IncrementPos ();
			}
			// Scroll Bar가 움직일 수 공간 안인지..
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

			if (state == SCROLLBAR_POLL)
				SetBarPosToGo (y);
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
//	Desc : 영역 체크 Func.
//-----------------------------------------------------------
// Title Area안의 위치 안에 있는지 체크한다.
bool	CNewListBox :: IsInTitleArea (int x, int y)
{
	if (y < PosY+BOARDER_OFF+TITLE_H &&
			y >= PosY+BOARDER_OFF &&
			x >= PosX+BOARDER_OFF &&
			x < PosX+Width-BOARDER_OFF)
		return true;
	else
		return false;
}

// 내용 글(Item Content)이 있는 위치 안인지 체크한다.
int  	CNewListBox :: IsInContentArea (int x, int y)
{
	if (y >= PosY+BOARDER_OFF+TITLE_H+CONTROL_OFF &&
			y < PosY+Height-BOARDER_OFF &&
			x >= PosX+BOARDER_OFF &&
			x < PosX+Width-BOARDER_OFF-SCROLLBTN_W+CONTROL_OFF - 10)//10은 스크롤과의 간격
	{
		// clicked position이 글(item)의 유무와 관계없이 계산한다.
		int Selected = 0;
		Selected = y - (PosY+BOARDER_OFF+TITLE_H+CONTROL_OFF);
		Selected = Selected / LineHeight;
		Selected += CurLine;

		return Selected;
	}
	else
		return -1;
}


// Scroll Btn의 위치 안인지 체크한다.
int 	CNewListBox :: IsInScrollBtnArea (int x, int y)
{
	// x 좌표 계산
	if (x +2 >= PosX+Width-BOARDER_OFF-SCROLLBTN_W  &&
			x +2 < PosX+Width-BOARDER_OFF )
	{
		if (y+3 >= PosY+BOARDER_OFF+TITLE_H+CONTROL_OFF&&
				y+3 < PosY+BOARDER_OFF+TITLE_H+CONTROL_OFF+SCROLLBTN_H)
			return SCROLL_DEC_BTN;	// Dec
		else if (y+3 >= PosY+Height-BOARDER_OFF-SCROLLBTN_H &&
				 y+3 < PosY+Height-BOARDER_OFF)
			return SCROLL_INC_BTN;	// Inc
		else
			return 0;
	}
	else
		return 0;
}


// 스크롤 바가 움직일 수 있는 위치 안인지 체크한다.
int 	CNewListBox :: IsInScrollBarArea (int x, int y)
{
	if (y >= PosY+BOARDER_OFF+TITLE_H+CONTROL_OFF+SCROLLBTN_H &&
			y < PosY+Height-BOARDER_OFF-SCROLLBTN_H &&
			x >= PosX+Width-BOARDER_OFF-SCROLLBTN_W &&
			x < PosX+Width-BOARDER_OFF)
	{
		// Scroll Bar의 몸땡이를 눌렀을 때는 무시한다.
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
void	CNewListBox :: IncrementPos ()
{
	if (CurLine+MaxVisibleLine < TotalLine)
		CurLine++;

	CalculateBarPos ();
}

void	CNewListBox :: DecrementPos ()
{
	if (CurLine > 0)
		CurLine--;

	CalculateBarPos ();
}

//------------------------------------------------------------
//	Name : GoHome () / GoEnd()
//	Desc :	처음과 끝으로..
//------------------------------------------------------------
void	CNewListBox :: GoHome ()
{
	CurLine = 0;
	ScrollBarPosY = DecBtnPosY+SCROLLBTN_H;
}

void	CNewListBox :: GoEnd ()
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
//	Desc :	Scroll Bar의 위치를 계산한다.
//			CurLine은 변경시키지 않구 Scroll Bar의 위치만 계산시킨다.
//------------------------------------------------------------
void	CNewListBox :: CalculateBarPos ()
{
	if (TotalLine > MaxVisibleLine)
	{
		int n1 = TotalLine - MaxVisibleLine;	// 화면상에 보여지지 않는 라인 num
		int n2 = CurLine;						// 현재 보여지기 시작하는 라인 num

		if (n1 != n2)
		{
			// 화면상에 스크롤될 수 있는 height pixel 수 (Bar Height 포함하지 않음)
			int HeightCanBeScrolled = Height - 2*BOARDER_OFF - TITLE_H
									  - CONTROL_OFF - 2*SCROLLBTN_H - SCROLLBAR_H;

			// 움직여지는 양..
			int	interval = HeightCanBeScrolled * n2 / n1;
			ScrollBarPosY = DecBtnPosY + SCROLLBTN_H + interval;
		}
		else
			// 맨 밑으로 내려간 경우이다.
			// 이경우는 바닥에 정확히 붙이기 위한 계산을 한다.
			ScrollBarPosY = IncBtnPosY - SCROLLBAR_H;
	}
}

//------------------------------------------------------------
//	Name : SetBarPosToGo ()
//	Desc :	Scroll Bar의 위치를 계산한다.
//			Scroll Bar를 이동시킨다. (마우스 Event에 의하여)
//------------------------------------------------------------
void	CNewListBox :: SetBarPosToGo (int y)
{
	if (TotalLine > MaxVisibleLine)
	{
		// Bar가 위치해야 할 포지션 구함
		if (y < DecBtnPosY+SCROLLBTN_H+SCROLLBAR_H/2)
			GoHome ();
		else if (y > IncBtnPosY-SCROLLBAR_H/2)
			GoEnd ();
		else
		{
			// 화면상에 스크롤될 수 있는 height pixel 수 (Bar Height 포함)
			int HeightCanBeScrolled = Height - 2*BOARDER_OFF - TITLE_H
									  - CONTROL_OFF - 2*SCROLLBTN_H;

			// 한 line당의 pixel수
			int interval = HeightCanBeScrolled / (TotalLine-MaxVisibleLine);

			// 현재의 Cur Line Position을 구한다.
			CurLine = (y-DecBtnPosY-SCROLLBTN_H) / interval;
			CalculateBarPos ();
		}
	}
}

//------------------------------------------------------------
//	Name : AddString ()
//	Desc :	String 추가
//------------------------------------------------------------
void	CNewListBox :: AddString (const char *str)
{
	if (TotalLine >= 40)
		return;
	if (LineText[TotalLine] != NULL)
		return;

	int len = strlen(str);
	if (len <= 0)
		return;

	LineText[TotalLine] = new char [len+1];
	// 메모리 복사가 더 빠르기 때문에.. (널문자 까지 복사)
	memcpy (LineText[TotalLine], str, len+1);
	TotalLine++;
	// Bar의 위치 재계산
	CalculateBarPos ();
}

//------------------------------------------------------------
//	Name : SetStringToNth ()
//	Desc :	index번째 string item값을 set..
//			만일, 3개의 item이 있는데, 10번째로 기록하려 하면,
//			4번째로 기록된다.
//------------------------------------------------------------
void	CNewListBox :: SetStringToNth (int index, const char *str)
{
	if (index < 0 || index >= MAX_LINE)		return;

	// Index 조정 및...
	int newIndex;

	if (LineText[index] != NULL)
	{
		newIndex = index;
		delete [] LineText[index];
		LineText[index] = NULL;
	}
	else
		newIndex = TotalLine++;

	// 메모리 복사
	int len = strlen(str);
	LineText[newIndex] = new char [len+1];
	memcpy (LineText[newIndex], str, len+1);

	// Bar의 위치 재계산
	CalculateBarPos ();
}

//-- Global Font 중 하나를 받아온다.
//-- 2004.03.15 - 수정완료
//-- Lyul
//-- 자체 생성에서 Global 사용으로 변경
void	CNewListBox :: SetFont (int f_w, int f_h, char *fontname)
{
	if( !fontname )
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
	MaxVisibleLine = (Height-BOARDER_OFF*2-TITLE_H-CONTROL_OFF) / LineHeight;

	// Bar Position 변경
	CalculateBarPos ();
}

//------------------------------------------------------------
//	Name : GetString ()
//	Desc :	index번째 string item값을 get
//------------------------------------------------------------
char*	CNewListBox :: GetString (int index)
{
	if (index < 0 || index >= TotalLine)
		return NULL;
	else
		return LineText[index];
}

//------------------------------------------------------------
//	Name : GetIndex (const char *str)
//	Desc :	str을 가지고 있는 Item의 index return
//------------------------------------------------------------
int 	CNewListBox :: GetIndex (const char *str)
{
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
void	CNewListBox :: SetTitle (const char *title)
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
		memcpy (Title, title, len);
}

//------------------------------------------------------------
//	Name : SetTitleColor(), SetTitleBkColor()
//		   SetTextColor(), SetTextBkColor()
//	Desc :	Title/ Text Color Set
//------------------------------------------------------------
void	CNewListBox :: SetTitleColor (COLORREF dwGDIColor)
{
	TitleColor = dwGDIColor;
}

void	CNewListBox :: SetTitleBkColor (COLORREF dwGDIColor)
{
	TitleBkColor = dwGDIColor;
}

void	CNewListBox :: SetTextColor (COLORREF dwGDIColor)
{
	TextColor = dwGDIColor;

	for(int i=0; i<MAX_LINE; i++)
		ColorTable[i] = dwGDIColor;

}

void	CNewListBox :: SetTextBkColor (COLORREF dwGDIColor)
{
	TextBkColor = dwGDIColor;
}

void CNewListBox :: UpdateList ()
{
	for (int i = 0; i < MAX_LINE; i++)
	{
		if (LineText[i])
		{
			delete m_pTextSurf[/*SelectedLine*/i];
			g_pDisplay->CreateSurfaceFromText(&m_pTextSurf[i], Hfont, LineText[i], RGB(0, 0, 1), ColorTable[i]/*TextColor*/);
			m_pTextSurf[i]->SetColorKey(RGB(0, 0, 1));
		}
	}
	GoHome ();
}

void	CNewListBox :: DelString (int index)
{
	if (TotalLine >= 40)				return;
	if (LineText[TotalLine] != NULL)	return;

	for(int i=index; i<TotalLine-1; i++)
	{
		SAFE_DELETE_ARRAY( LineText[i] );
		LineText[i] = new char[strlen(LineText[i+1])+1];
		strcpy(LineText[i], LineText[i+1]);
	}

	ColorTable[index] = TextColor;

	for(int i=index; i<TotalLine-1; i++)
		ColorTable[i] = ColorTable[i+1];

	SAFE_DELETE_ARRAY(LineText[TotalLine-1]);
	SAFE_DELETE(m_pTextSurf[TotalLine-1]);

	TotalLine--;
	UpdateList();
	SelectedLine = -1;
	// Bar의 위치 재계산
	CalculateBarPos ();
}

void CNewListBox :: SetColorString (int index, int color)
{
	switch(color)
	{
	case 0: //ready
		ColorTable[index] = RGB(0, 255, 0);
		break;
	case 1: //wait 블루 - A -> 홍팀이 맞음
		ColorTable[index] = RGB(255, 100, 100);
		break;
	case 2://wait 레드
		ColorTable[index] = RGB(100, 100, 255);
		break;
	case 3://방장
		ColorTable[index] = RGB(255, 255, 255);
		break;
	case 4://wait 서바이벌
		ColorTable[index] = RGB(255, 255, 0);
		break;
	}

	if(g_pDisplay && index != -1)
	{
		delete m_pTextSurf[index];
		if(SelectedLine != index)
			g_pDisplay->CreateSurfaceFromText(&m_pTextSurf[index], Hfont, LineText[index], RGB(0, 0, 1), ColorTable[index]);
		else
			g_pDisplay->CreateSurfaceFromText(&m_pTextSurf[index], Hfont, LineText[index], RGB(100, 100, 100), ColorTable[index]);

		if( m_pTextSurf[index] )
			m_pTextSurf[index]->SetColorKey(RGB(0, 0, 1));
	}
}