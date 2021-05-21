#ifndef _PROGRESS_BOX_H_
#define _PROGRESS_BOX_H_

#include "Config.h"



class CSurface;

class CProgressBox
{
public:
	CProgressBox();
	virtual ~CProgressBox();

	//LoadRes( 박스가 찍힐 X,Y 좌표 / 박스의 넓이,높이 / (아래)박스 파일이름 , (위)박스 파일이름)
	void LoadRes( int x, int y, int width, int height, char *backFile, char*progressFile );
	void DeleteRes();
	void Draw();
	void SetMaxPos(int pos)
	{
		m_MaxPos = pos;
	}
	void SetCurPos(int pos)
	{
		m_CurPos = pos;
	}
	void SetValues(int maxPos, int curPos, COLORREF textColor, int fontSize = 12);
	void SetFont(int f_w, int f_h, char *fontname);
	void SetVisible(BOOL bVisible)
	{
		m_bVisible = bVisible;
	}

	CSurface* m_pBackSur;
	CSurface* m_pProgressSur;

	HFONT m_Font;
	int m_PosX;			// 화면에 위치할 x 좌표
	int m_PosY;			// 화면에 위치할 y 좌표
	int m_Width;		// 버튼 이미지의 가로길이
	int m_Height;		// 버튼 이미지의 세로길이

	int m_MaxPos;
	int m_CurPos;
	COLORREF m_TextColor;
	int m_FontSize;

	BOOL m_bVisible;
};

#endif // _PROGRESS_BOX_H_
