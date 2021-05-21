#ifndef _SPIN_BOX_H_
#define _SPIN_BOX_H_

#include "Config.h"


#include "basicbutton.h"

class CSurface;
class CBasicButton;

class CSpinBox
{
public:
	CSpinBox();
	virtual ~CSpinBox();

	void Init(int pos_x, int pos_y, int width, int height,
			  int font_size, COLORREF font_color);
	void LoadRes(char* RBtnFn, char* LBtnFn);
	void DeleteRes();
	void LoadSurface(int width, int height);
	void Draw();

	void SetPos(int x, int y)
	{
		m_PosX = x;
		m_PosY = y;
	}
	void SetSize(int width, int height)
	{
		m_Width = width;
		m_Height = height;
	}

	BOOL IsInsideLSpin(int x, int y);
	BOOL IsInsideRSpin(int x, int y);
	void SetFont (int f_w, int f_h, char *fontname);
	void SetSize(int size);
	void SetString(int idx, char *string);
	void DeleteString();

	int MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam,
				 LPARAM lParam );
	// 기존의 버는이미지생성을  "버튼클레스로" 바꿨다.
	CBasicButton	m_LSpinBtn;
	CBasicButton	m_RSpinBtn;


	int m_PosX;
	int m_PosY;
	int m_Width;
	int m_Height;
	HFONT m_Font;
	COLORREF m_FontColor;
	int m_FontSize;
	int m_SpinWidth;
	int m_SpinHeight;
	int m_Size;
	char **m_String;
	int m_CurSel;
};

#endif // _SPIN_BOX_H_
