#ifndef _MULTI_LISTBOX_
#define _MULTI_LISTBOX_

#include "Config.h"

#include "BaseUI.h"
#include "BasicButton.h"

class CBasicButton;
class CTextBoard;

class CMutiListBox : public CBaseUI
{
public :

	enum	{ MAX_TITLE = 40, MAX_LINE = 500 };
	enum	{ DEFAULT_LINEHEIGHT = 20 };
	enum	{ SCROLL_DEC_BTN = 1, SCROLL_INC_BTN = 2 };
	enum	{ SCROLL_BAR_FLAT = 1, SCROLL_BAR_POLL = 2 };
	enum	{ SCROLLBTN_W = 17, SCROLLBTN_H = 17 };
	enum	{ SCROLLBAR_W = 17, SCROLLBAR_H = 28 };
	enum	{ BOARDER_OFF = 0, TITLE_H = 0, CONTROL_OFF = 0  };
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

	int		m_nScrollbarH;

	CTextBoard *m_pTextBoard;

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
	BOOL	IsInsideBar(int x, int y);

	//******************************
	// Scrolling
	void	IncrementPos();		// Position Inc
	void	DecrementPos();		// Position Dec
	void	GoHome();				// �� ó������..
	void	GoEnd();				// �� ������..

	void	CalculateBarPos();		// Scroll Bar�� ��ġ�� ����Ѵ�.
	void	CalculateBarPos2( int ScrollBarHeight ); // ������ �׷��ȿ� ���缭 �ٽ� �ۼ�. ���� ����. ������ (06-12-21 ����)

	void	SetBarPosToGo(int y);	// Scroll Bar�� Mouse Click�� Bar�� ��ġ�� �̵���Ų��.

	//******************************

	CMutiListBox();
	CMutiListBox (char *title, int nX, int nY, int nH, int nLineNum, int nColumn, int nStringSize = 20);	// ������
	~CMutiListBox ();			// �Ҹ���

	void	InitVar (char *title);
	void	InitVar (char *title, int nX, int nY, int nH, int nLineNum, int nColumn, int nStringSize = 20);

	// Set Image, Position, Create Surface
	void	SetPosDim (const int x, const int y, const int w, const int h);
	void	SetMove(int nX, int nY, int nWidth, int nHeight);
	void	SetImage (char *DecBtn, char *IncBtn, char *ScrollBar, int nBarHeight = 28);
	void	SetBtnPosY(int nIncY, int nDecY);

	void	LoadRes();	// ��ũ�� ��ư�� "��ư Ŭ����"�� ó���Ͽ��� ������ creatsurface()�Լ���ſ�
	// LoadRes(pDis)�� ��ü�Ͽ��� �ؼ� 	������"setDisplayPtr()"�Լ��� ���� ����� ������

	void	DeleteRes();			// Surface Deletion
	HRESULT	Restore ();					// Image Restore

	// Message ó��
	int	    GetMessage (HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

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
	void	AddString (int nLineIdx, int nTextIdx, char *szText);
	void	SetIncLine();

	//char*	GetString (int index);				// index��° Line�� string Get
	char*	GetString(int nTextId);
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

	void	all_Reset();

	void	AddLine(int dwTextIdx, float fBoxWidth);

};
#endif // _MULTI_LISTBOX_