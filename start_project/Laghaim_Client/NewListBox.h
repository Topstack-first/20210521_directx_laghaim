#ifndef _NEW_LISTBOX_H_
#define _NEW_LISTBOX_H_

#include "Config.h"

#include "BaseUI.h"
#include "BasicButton.h"


class CNewListBox;
class CBasicButton;

class CNewListBox : public CBaseUI
{
public :

	enum	{ MAX_TITLE = 40, MAX_LINE = 40 };
	enum	{ DEFAULT_LINEHEIGHT = 20 };
	enum	{ SCROLL_DEC_BTN = 1, SCROLL_INC_BTN = 2 };
	enum	{ SCROLL_BAR_FLAT = 1, SCROLL_BAR_POLL = 2 };
	enum	{ SCROLLBTN_W = 17, SCROLLBTN_H = 8 };
	enum	{ SCROLLBAR_W = 17, SCROLLBAR_H = 18 };
	enum	{ BOARDER_OFF = 4, TITLE_H = 24, CONTROL_OFF = 4  };
	// SCROLLBTN_W, SCROLLBTN_H => Scroll Btn Width, Height
	// SCROLLBAR_W, SCROLLBAR_H => Scroll Bar Width, Height
	// BOARDER_OFF => List Box 외곽의 offset
	// CONTROL_OFF => List Box 안의 control간의 offset
	// TITLE_H => Title Area Height

	enum	STATE	{ NONE, TITLE, CONTENT, SCROLLBAR_POLL, SCROLLBAR_FLAT, SCROLLBTN };

	//******************************



	STATE	state;

	char	Title[MAX_TITLE];			// ListBox Title

	COLORREF  TitleColor;				// Title Color
	COLORREF  TitleBkColor;				// Title Background Color
	COLORREF  TextColor;				// Text Color
	COLORREF  TextBkColor;				// Text Background Color

	char	*fnDecrementBtn;			// Decrement Button File
	char	*fnIncrementBtn;			// Increment Button File
	char	*fnScrollBar;				// Scroll Bar File


	CSurface	*ScrollBarSurface;		// Scroll Bar Surface

	/////////////////////////////////////////////////////////////
	CBasicButton	m_IncBtn;		// 기존에 이미지두개로 쓰던걸 버튼클래스로 바꿨다..
	CBasicButton	m_DecBtn;
	/////////////////////////////////////////////////////////////

	int		ScrollPosX;					// ScrollBar X Position
	int		DecBtnPosY;					// Decrement Button Y Position
	int		IncBtnPosY;					// Increment Button Y Position
	int		ScrollBarPosY;				// ScrollBar Y Position

	char	*LineText[MAX_LINE];		// ListBox content Text
	CSurface *m_pTextSurf[MAX_LINE];

	HFONT	Hfont;						// Font..

	int		TotalLine;					// Item을 가지고 있는 총 라인수
	int		CurLine;					// 보여질 처음 라인 offset
	int		SelectedLine;				// 선택되어진 line offset
	int		LineHeight;					// 한라인이 차지하는 pixel수
	int		MaxVisibleLine;				// ListBox에 보여질 MaxLine




	// Hit Area Test ....  (함수들을 하나로 합치는게 효율적이나,
	// 여기에선 개별적인 Hit Test Func으로 작성하였음..)
	bool	IsInTitleArea (int x, int y);
	int 	IsInContentArea (int x, int y);
	int		IsInScrollBtnArea (int x, int y);
	int 	IsInScrollBarArea (int x, int y);

	//******************************
	// Scrolling
	void	IncrementPos ();		// Position Inc
	void	DecrementPos ();		// Position Dec
	void	GoHome ();				// 맨 처음으로..
	void	GoEnd ();				// 맨 끝으로..

	void	CalculateBarPos ();		// Scroll Bar의 위치를 계산한다.
	void	SetBarPosToGo (int y);	// Scroll Bar에 Mouse Click시 Bar의 위치를 이동시킨다.

	//******************************



	CNewListBox();
	CNewListBox (char *title);	// 생성자
	CNewListBox (char *title, int x, int y, int w, int h);
	CNewListBox (char *title, int x, int y, int w, int h, int flag);
	~CNewListBox ();			// 소멸자

	void	InitVar (char *title);

	// Set Image, Position, Create Surface
	void	SetPosDim (const int x, const int y, const int w, const int h);
	void	SetImage (char *DecBtn, char *IncBtn, char *ScrollBar);

	void	LoadRes();	// 스크롤 버튼을 "버튼 클래스"로 처리하였고 기존의 creatsurface()함수대신에
	// LoadRes(pDis)로 대체하였고 해서 	기존의"setDisplayPtr()"함수는 쓸모가 없어어 지웠다

	void	DeleteRes();			// Surface Deletion
	HRESULT	Restore ();					// Image Restore

	// Message 처리
	int	GetMessage (HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	//******************************
	// Access Routine
	void	SetTitle (const char *title);
	char*	GetTitle ()
	{
		return Title;
	}
	void	SetFont (int f_w, int f_h, char *fontname);

	void	SetTitleColor (COLORREF dwGDIColor);
	void	SetTitleBkColor (COLORREF dwGDIColor);
	void	SetTextColor (COLORREF dwGDIColor);
	void	SetTextBkColor (COLORREF dwGDIColor);
	void	SetColors (COLORREF dwGDITitleColor, COLORREF dwGDITextColor);

	//******************************
	// List에 string Item  Set / Get

	void	AddString (const char *str);		// Line 추가
	void	SetStringToNth (int index, const char *str);	// Nth Index에 Line 추가
	char*	GetString (int index);				// index번째 Line의 string Get
	int		GetIndex (const char *str);			// Str을 가지고 있는 line index Get
	int		GetSelectedLine ()
	{
		return SelectedLine;
	};
	int		SetSelectedLine (int line)
	{
		SelectedLine = line;
		return SelectedLine;
	};

	// Override Function
	void	Draw ();		// Drawing

	void	UpdateList ();
	void	DelString (int index);
	void    SetColorString (int index, int color);
	COLORREF		ColorTable[40];

};


#endif // _NEW_LISTBOX_H_