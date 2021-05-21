#ifndef _CBasicStatic_H_
#define _CBasicStatic_H_

#include "ddutil.h"

class CBasicStatic
{
public:
	CSurface*	m_surface;
	char		m_bmpfile[80];

	int m_x;
	int m_y;
	int m_w;
	int m_h;

	float m_scaleX;
	float m_scaleY;

	bool m_visible;

	RECT m_rcThis;


public:
	void SetSize(int w, int h)
	{
		m_w = w;
		m_h = h;
		UpdateTransform();
	}
	void SetScale(float w, float h)
	{
		m_scaleX = w;
		m_scaleY = h;
	}
	void SetPos(int x, int y)
	{
		m_x = x;
		m_y = y;
		UpdateTransform();
	}
	void SetVisible(bool visible)
	{
		m_visible = visible;
	}
	void UpdateTransform();

	void Align(int align_type , int w, int h);

	bool IsIn(int x, int y)
	{
		POINT pt = {x,y};
		return PtInRect( &m_rcThis , pt)?true:false;
	}


public:
	void Init();
	void Init(const char* filename,int w=0, int h=0);

	void Release();
	void SetFilename(const char* filename);
	void LoadRes(int w=0, int h=0);
	void DeleteRes();
	void Draw();
	void Draw(int x, int y);
	void Draw(float ratio,int dir);
	void DrawToBlt(bool checked=false);

public:
	CBasicStatic();
	CBasicStatic(const char* filename,int w=0, int h=0);
	virtual ~CBasicStatic();

};

class CBasicStaticEx
{
public:
	CSurface * m_surface;

	int m_num_col;
	int m_num_row;
	int m_current;

	int m_x;
	int m_y;
	int m_w;
	int m_h;

	char		m_bmpfile[80];

	RECT		m_rcThis;
	RECT		m_rcDraw;

public:
	int GetColSize()
	{
		return m_w/m_num_col;
	}
	int GetRowSize()
	{
		return m_h/m_num_row;
	}

	int GetCol()
	{
		return m_num_col;
	}
	int GetRow()
	{
		return m_num_row;
	}


public:
	void Init(const char* filename, int col=1, int row=1);
	void InitEx(int col , int row);
	void LoadRes();
	void DeleteRes();
	void SetFilename(const char* filename);
	void Draw();
	void Draw(int x, int y);

	void Draw(int index);
	void Draw(int x, int y,int index);

	void SetPos(int x, int y);
	bool IsIn(int x, int y);
	void UpdateTransform();
	void UpdateTransformImg();

	void SetCurrent(int cur)
	{
		m_current = cur;
		UpdateTransformImg();
	}


public:
	CBasicStaticEx();
	virtual ~CBasicStaticEx();
};

#endif // _CBasicStatic_H_