#ifndef _BASE_UI_H_
#define _BASE_UI_H_

#include "Config.h"

#include "headers.h"


#ifndef NULL
#define NULL 0L
#endif

class CBaseUI;

class CBaseUI
{
public:
	CBaseUI ();
	CBaseUI (int x, int y, int w, int h);
	~CBaseUI ();

	int		PosX, PosY;		// Interface ��ü�� ������ ��ġ
	int		Width, Height;	// Interface ��ü�� ����/���� ����

	// Access Routine
	void	SetPosX (const int x)
	{
		PosX = x;
	}
	void	SetPosY (const int y)
	{
		PosY = y;
	}
	void	SetPosition (const int x, const int y)
	{
		PosX = x, PosY = y;
	}

	int		GetPosX ()
	{
		return PosX;
	}
	int		GetPosY ()
	{
		return PosY;
	}
	void	GetPosXY (int &x, int &y)
	{
		x = PosX, y = PosY;
	}

	void	SetWidth (const int w)
	{
		Width = w;
	}
	void	SetHeight (const int h)
	{
		Height = h;
	}
	void	SetDimension (const int w, const int h)
	{
		Width = w, Height = h;
	}

	int		GetWidth ()
	{
		return Width;
	}
	int		GetHeight ()
	{
		return Height;
	}
	void	GetDimension (int &w, int &h)
	{
		w = Width, h = Height;
	}

	virtual void	SetPosDim (const int x, const int y, const int w, const int h)
	{
		PosX = x, PosY = y, Width = w, Height = h;
	}

	//******************************

	// (x, y) point�� Interface��ü�� �����ϰ� �ִ� ������ �ִ��� üũ
	bool 	IsInside (int x, int y)
	{
		if (x >= PosX && x < PosX+Width && y >= PosY && y < PosY+Height)
			return true;
		else
			return false;
	}

	virtual void	Draw ();
};

#endif // _BASE_UI_H_

