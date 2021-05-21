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
	// BOARDER_OFF => List Box �ܰ��� offset
	// CONTROL_OFF => List Box ���� control���� offset
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
	CBasicButton	m_IncBtn;		// ������ �̹����ΰ��� ������ ��ưŬ������ �ٲ��..
	CBasicButton	m_DecBtn;
	/////////////////////////////////////////////////////////////

	int		ScrollPosX;					// ScrollBar X Position
	int		DecBtnPosY;					// Decrement Button Y Position
	int		IncBtnPosY;					// Increment Button Y Position
	int		ScrollBarPosY;				// ScrollBar Y Position

	char	*LineText[MAX_LINE];		// ListBox content Text
	CSurface *m_pTextSurf[MAX_LINE];

	HFONT	Hfont;						// Font..

	int		TotalLine;					// Item�� ������ �ִ� �� ���μ�
	int		CurLine;					// ������ ó�� ���� offset
	int		SelectedLine;				// ���õǾ��� line offset
	int		LineHeight;					// �Ѷ����� �����ϴ� pixel��
	int		MaxVisibleLine;				// ListBox�� ������ MaxLine




	// Hit Area Test ....  (�Լ����� �ϳ��� ��ġ�°� ȿ�����̳�,
	// ���⿡�� �������� Hit Test Func���� �ۼ��Ͽ���..)
	bool	IsInTitleArea (int x, int y);
	int 	IsInContentArea (int x, int y);
	int		IsInScrollBtnArea (int x, int y);
	int 	IsInScrollBarArea (int x, int y);

	//******************************
	// Scrolling
	void	IncrementPos ();		// Position Inc
	void	DecrementPos ();		// Position Dec
	void	GoHome ();				// �� ó������..
	void	GoEnd ();				// �� ������..

	void	CalculateBarPos ();		// Scroll Bar�� ��ġ�� ����Ѵ�.
	void	SetBarPosToGo (int y);	// Scroll Bar�� Mouse Click�� Bar�� ��ġ�� �̵���Ų��.

	//******************************



	CNewListBox();
	CNewListBox (char *title);	// ������
	CNewListBox (char *title, int x, int y, int w, int h);
	CNewListBox (char *title, int x, int y, int w, int h, int flag);
	~CNewListBox ();			// �Ҹ���

	void	InitVar (char *title);

	// Set Image, Position, Create Surface
	void	SetPosDim (const int x, const int y, const int w, const int h);
	void	SetImage (char *DecBtn, char *IncBtn, char *ScrollBar);

	void	LoadRes();	// ��ũ�� ��ư�� "��ư Ŭ����"�� ó���Ͽ��� ������ creatsurface()�Լ���ſ�
	// LoadRes(pDis)�� ��ü�Ͽ��� �ؼ� 	������"setDisplayPtr()"�Լ��� ���� ����� ������

	void	DeleteRes();			// Surface Deletion
	HRESULT	Restore ();					// Image Restore

	// Message ó��
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
	// List�� string Item  Set / Get

	void	AddString (const char *str);		// Line �߰�
	void	SetStringToNth (int index, const char *str);	// Nth Index�� Line �߰�
	char*	GetString (int index);				// index��° Line�� string Get
	int		GetIndex (const char *str);			// Str�� ������ �ִ� line index Get
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