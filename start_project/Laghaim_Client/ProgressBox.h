#ifndef _PROGRESS_BOX_H_
#define _PROGRESS_BOX_H_

#include "Config.h"



class CSurface;

class CProgressBox
{
public:
	CProgressBox();
	virtual ~CProgressBox();

	//LoadRes( �ڽ��� ���� X,Y ��ǥ / �ڽ��� ����,���� / (�Ʒ�)�ڽ� �����̸� , (��)�ڽ� �����̸�)
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
	int m_PosX;			// ȭ�鿡 ��ġ�� x ��ǥ
	int m_PosY;			// ȭ�鿡 ��ġ�� y ��ǥ
	int m_Width;		// ��ư �̹����� ���α���
	int m_Height;		// ��ư �̹����� ���α���

	int m_MaxPos;
	int m_CurPos;
	COLORREF m_TextColor;
	int m_FontSize;

	BOOL m_bVisible;
};

#endif // _PROGRESS_BOX_H_
